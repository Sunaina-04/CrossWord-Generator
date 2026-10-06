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
#include "trie.h"
#include "hashtable.h"
#include "grid.h"
#include "slot.h"

typedef enum {EASY, MEDIUM, HARD} Difficulty;

#define MAX_WORDS 500
#define MAX_WORD_LENGTH 20

/*
 * Read one word per line from `filename` into `words`.
 * Returns the number of words read, or -1 if the file couldn't be opened.
 *
 * TODO: implement this. It's the file-handling step: fopen -> read
 * line by line -> fclose. Rough shape to work from:
 */

int loadWordsFromFile(const char* filename, char words[][MAX_WORD_LENGTH + 1], int maxWords) {

    return 0; /* placeholder */
}

int main(int args, char* argv[]) {
    printf("Welcome to CrossWord Generator \n");

    /* TODO 1: Load the word list from a file */

    /* TODO 2: Build lookup structures from the word list. */

    TrieNode* wordTrie = trieCreateNode();
    LengthHashTable* lengthTable = hashtableCreate();

    /* for each word: trieInsert(wordTrie, word); hashtableInsert(lengthTable, word); */
 
    /* TODO 3: Set up the grid and identify its fillable slots. */
    Grid grid;
    gridInit(&grid);
    SlotList* slots = slotListCreate(16);

    /* TODO: populate `slots` by scanning the grid (see slot.h TODO) */
 
    /* TODO 4: Difficulty parameter.
     *   Decide here how `difficulty` influences slot order / word
     *   selection preference (e.g. HARD -> prefer longer-word buckets
     *   from lengthTable first, fewer black squares in gridInit).
     */

     Difficulty difficulty = MEDIUM;

     /* TODO 5: Run the backtracking fill.
     *   A recursive function roughly like:
     *     int fillSlots(Grid* grid, SlotList* slots, int slotIndex,
     *                    TrieNode* trie, LengthHashTable* table);
     *   that tries each candidate word (hashtable bucket -> trie
     *   pattern match -> gridIsValidPlacement) for slots[slotIndex],
     *   places it, recurses on slotIndex + 1, and backtracks
     *   (gridRemoveWord) if the recursive call fails.
     */

     gridPrint(&grid);

     /* Cleanup */
     slotListFree(slots);
     hashtableFree(lengthTable);
     trieFree(wordTrie);

     return 0; 
}