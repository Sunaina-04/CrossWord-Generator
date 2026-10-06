#ifndef HASHTABLE_H
#define HASHTABLE_H

#define MAX_WORD_LEN 20

// singly linked list for chaining with bucket
typedef struct WordNode {
    char word[MAX_WORD_LEN + 1];
    struct WordNode* next;
} WordNode;

typedef struct {
    WordNode* buckets[MAX_WORD_LEN + 1];
}LengthHashTable;

// allocate and zero-initialize a new table
LengthHashTable* hashtableCreate(void);

// insert word into bucket matching its length
void hashtableInsert(LengthHashTable* table, const char* word);

// return head of linked list os words with given length 
WordNode* hashtableGetByLength(LengthHashTable* table, int length);

void hashtableFree(LengthHashTable* table);

#endif