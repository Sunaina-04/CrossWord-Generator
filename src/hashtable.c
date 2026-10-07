#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"


// allocate and zero-initialize a new table
LengthHashTable* hashtableCreate(void) {
    LengthHashTable* table = (LengthHashTable*)malloc(sizeof(LengthHashTable));
    if (!table) {
        fprintf(stderr, "hashtableCreate: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= MAX_WORD_LEN; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

// insert word into bucket matching its length
void hashtableInsert(LengthHashTable* table, const char* word) {

}

// return head of linked list os words with given length 
// can we use binary search to getByLength as there are naturally alphabetically ordered table
WordNode* hashtableGetByLength(LengthHashTable* table, int length) {
    if (length < 0 || length > MAX_WORD_LEN) return NULL;
    return table->buckets[length];
}

void hashtableFree(LengthHashTable* table) {
    if (!table) return;
    for (int i = 0; i <= MAX_WORD_LEN; i++) {
        WordNode* node = table->buckets[i];
        while (node) {
            WordNode* next = node->next;
            free(node);
            node = next;
        }
    }
    free(table);
}