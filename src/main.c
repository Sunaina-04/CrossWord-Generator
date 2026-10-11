/*
 *--------------------------------------
 * Program Name: CrossWord generator
 * Author: Sunaina Sharma
 * License:LINCENSE
 *--------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "trie.h"
#include "hashtable.h"
#include "grid.h"
#include "slot.h"

typedef enum {EASY, MEDIUM, HARD} Difficulty;

#define MAX_WORDS 500
#define MAX_WORD_LENGTH 20
#define MAX_ATTEMPTS 5000
#define MAX_STEPS 20000L // fill steps allowed per layout


static int storeWord(char* word, int len, int tooLong, char words[][MAX_WORD_LENGTH + 1], int count, int maxWords) {
    if (len == 0 && !tooLong){
        return 0;
    }
    if (tooLong) {
        printf(stderr, "Skipping a word longer than %d letters \n", MAX_WORD_LENGTH);
        return 0;
    }

    word[len] = '\0';
    for (int i = 0; i < len; i++) {
        word[i] = (char)tolower((unsigned char)word[i]);
        if (word[i] < 'a' || word[i] > 'z') {
            printf(stderr, "Skipping '%s': only the letters a-z are supported\n",word);
            return 0;
        }
    }
 
    if (count >= maxWords) {
        return -1;
    }
    strcpy(words[count], word);
    return 1;
}

/*
 * Read one word per line from `filename` into `words`.
 * Returns the number of words read, or -1 if the file couldn't be opened.
 */
int loadWordsFromFile(const char* filename, char words[][MAX_WORD_LENGTH + 1], int maxWords) {
    int fd = open (filename,O_RDONLY);
    if (fd < 0) {
        printf(stderr, "Error: could not open word list file '%s'\n", filename);
        return -1;
    }

    int count = 0;
    int truncated = 0;
    char word[MAX_WORD_LENGTH + 1];
    int len = 0;
    int tooLong = 0;

    char chunk[256];
    ssize_t bytesRead;

    while ((bytesRead = read (fd, chunk, sizeof(chunk))) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            char ch = chunk[i];

            if (ch == '\r') {
                continue;
            }
            if (ch != '\n') {
                if (len < MAX_WORD_LENGTH) {
                    word[len++] = ch;
                }else {
                    tooLong = 1;
                }
                continue;
            }
            int result = storeWord(word, len, tooLong, words, count, maxWords);

            if (result == 1) {
                count++;
            }
            if (result == -1) {
                truncated = 1;
            }
            len = 0;
            tooLong = 0; 
        }
    }

    if (bytesRead < 0){
        printf(stderr, "Error: count not read '%s \n", filename);
        close(fd);
        return -1;
    }

    int result = storeWord(word, len, tooLong, words, count, maxWords);
    if (result == 1){
        count++;
    }
    if (result == -1) {
        truncated = 1;
    }

    close (fd);

    if (truncated) {
        printf(stderr,"Warning: only the first %d words were use \n", maxWords);
    }

    return count;
}

static int blackPercentFor(Difficulty difficulty) {
    switch (difficulty)
    {
    case EASY :
        return 30;
    case HARD :
        return 15;
    case MEDIUM :
    default:
        return 22;
    }
}

static const char* difficultyName(Difficulty difficulty){
    switch (difficulty) {
        case EASY: return "easy";
        case HARD: return "hard";
        default:   return "medium";
    }
}

static int parseDifficulty(const char* text, Difficulty* out) {
    if (strcmp(text, "easy") == 0)        *out = EASY;
    else if (strcmp(text, "medium") == 0) *out = MEDIUM;
    else if (strcmp(text, "hard") == 0)   *out = HARD;
    else return 0;
    return 1;
}

static void slotPattern(const Grid* grid, const Slot* slot, char* out) {
    int r = slot->row;
    int c = slot->col;
    for (int i = 0; i < slot->length; i++) {
        char cell = grid->cells[r][c];
        out[i] = (cell >= 'a' && cell <= 'z') ? cell : '_';
        if (slot->direction == ACROSS) c++; else r++;
    }
    out[slot->length] = '\0';
}

static int isWordUsed(const char** placed, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(placed[i], word) == 0) return 1;
    }
    return 0;
}

static int everySlotLengthHasWords(const SlotList* slots,
                                    LengthHashTable* table) {
    for (int i = 0; i < slots->count; i++) {
        if (hashtableGetByLength(table, slots->slots[i].length) == NULL) {
            return 0;
        }
    }
    return 1;
}

static int fillSlots(Grid* grid, const SlotList* slots, int slotIndex,
                      TrieNode* trie, LengthHashTable* table,
                      const char** placed, long* budget) {
    if (slotIndex == slots->count) {
        return 1; 
    }
    if (*budget <= 0) {
        return 0;
    }
    (*budget)--;
 
    const Slot* slot = &slots->slots[slotIndex];
    char pattern[MAX_WORD_LENGTH + 1];
    char snapshot[MAX_WORD_LENGTH + 1];
    slotPattern(grid, slot, pattern);
 
    if (!trieMatchPattern(trie, pattern)) {
        return 0;
    }
 
    for (int i = 0; i < slot->length; i++) {
        snapshot[i] = (pattern[i] == '_') ? EMPTY_CELL : pattern[i];
    }
    snapshot[slot->length] = '\0';
 
    for (WordNode* node = hashtableGetByLength(table, slot->length);
         node != NULL; node = node->next) {
        if (*budget <= 0) {
            return 0;
        }
        if (isWordUsed(placed, slotIndex, node->word)) {
            continue;
        }
        if (!gridIsValidPlacement(grid, slot->row, slot->col,
                                   slot->direction, node->word)) {
            continue;
        }
 
        gridPlaceWord(grid, slot->row, slot->col, slot->direction, node->word);
        placed[slotIndex] = node->word;
 
        if (fillSlots(grid, slots, slotIndex + 1, trie, table, placed, budget)) {
            return 1;
        }
 
        // Dead end below us: undo and try the next candidate.
        gridPlaceWord(grid, slot->row, slot->col, slot->direction, snapshot);
    }
    return 0;
}


int main(int argc, char* argv[]) {
    printf("Welcome to CrossWord Generator \n");

    const char* wordListPath = "data/wordlist.txt";
    Difficulty difficulty = MEDIUM;
 
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [wordlist-file] [easy|medium|hard]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc >= 2) {
        wordListPath = argv[1];
    } else {
        printf("No word list file given, using default: %s\n", wordListPath);
        printf("Usage: %s [wordlist-file] [easy|medium|hard]\n", argv[0]);
    }
    if (argc == 3 && !parseDifficulty(argv[2], &difficulty)) {
        fprintf(stderr, "Unknown difficulty '%s' (use easy, medium or hard)\n",
                argv[2]);
        return EXIT_FAILURE;
    }
 
    /* 1. Load the words. */
    static char words[MAX_WORDS][MAX_WORD_LENGTH + 1];
    int wordCount = loadWordsFromFile(wordListPath, words, MAX_WORDS);
    if (wordCount < 0) {
        return EXIT_FAILURE;
    }
    if (wordCount == 0) {
        fprintf(stderr, "No usable words found in '%s'\n", wordListPath);
        return EXIT_FAILURE;
    }
    printf("Loaded %d words from %s\n", wordCount, wordListPath);
 
    /* 2. Index them: trie for pattern matching, table for length lookup. */
    TrieNode* wordTrie = trieCreateNode();
    LengthHashTable* lengthTable = hashtableCreate();
    for (int i = 0; i < wordCount; i++) {
        trieInsert(wordTrie, words[i]);
        hashtableInsert(lengthTable, words[i]);
    }
 
    /* 3-7. Generate a random layout, check it, scan its slots, try to
     * fill it; if anything fails, try a fresh layout. Seed once, here
     * (to reproduce a run while debugging, pass a constant instead of
     * time(NULL)). */
    srand((unsigned int)time(NULL));
    int blackPercent = blackPercentFor(difficulty);
 
    Grid grid;
    int success = 0;
    int attempt = 0;
 
    while (!success && attempt < MAX_ATTEMPTS) {
        attempt++;
        gridInit(&grid, blackPercent);
        if (!gridLayoutIsClean(&grid)) {
            continue;
        }
 
        SlotList* slots = slotListFromGrid(&grid);
        if (slots->count > 0 && everySlotLengthHasWords(slots, lengthTable)) {
            const char** placed =
                (const char**)calloc((size_t)slots->count, sizeof(const char*));
            if (!placed) {
                fprintf(stderr, "main: calloc failed\n");
                exit(EXIT_FAILURE);
            }
            long budget = MAX_STEPS;
            success = fillSlots(&grid, slots, 0, wordTrie, lengthTable,
                                placed, &budget);
            free(placed);
        }
        slotListFree(slots);
    }
 
    /* 8. Report. */
    if (success) {
        printf("\nGenerated a crossword on attempt %d (difficulty: %s):\n\n",
               attempt, difficultyName(difficulty));
        gridPrint(&grid);
    } else {
        fprintf(stderr,
                "\nNo crossword could be generated from this word list after "
                "%d layouts (difficulty: %s).\n"
                "Try a larger word list or an easier difficulty.\n",
                attempt, difficultyName(difficulty));
    }
 
    hashtableFree(lengthTable);
    trieFree(wordTrie);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}