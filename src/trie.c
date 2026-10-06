#include <stdio.h>
#include <stdlib.h>;
#include "trie.h"


TrieNode* trieCreateNode (void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (!node) {
        fprintf(stderr,"Malloc allocation for trieNode creation failed");
        exit(EXIT_FAILURE);
    }
    node->isEndOfWord = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// insert lowercase word into trie
void trieInsert(TrieNode* root, const char* word) {
   int i = 0;
   while (word[i] != '\0') {
    int index = word[i] - 'a';
    if (root-> children[index] == NULL) {
        root->children[index] = trieCreateNode();
    }
    root = root->children[index];
    i++; 
   }
   root->isEndOfWord = 1;
}

// return 1 if 'word' exists as a complete word, else 0
int trieSearch(TrieNode* root, const char* word) {
    int i = 0;
    while(word[i] != '\0') {
        int index = word[i] - 'a';
        if (root->children[index] == NULL) {
            return 0;
        }
        i++;
        root = root->children[index];
    }
    return root->isEndOfWord;
}

// if word ins trie starts with the 'prefix' provided return 1, else 0
int trieStartsWith(TrieNode* root, const char* prefix){
    int i = 0;
    while (prefix[i] != '\0') {
        int index = prefix[i] - 'a';
        if(root->children[index] == NULL){
            return 0;
        }
        i++;
        root = root->children[index];
    }
    return 1;
}

// return 1 if word in trie matches 'pattern', where pattern uses '_' as a wildcard for 'letter not known yet' 
int trieMatchPattern(TrieNode* root, const char* pattern){
     /* TODO (the interesting one):
     * This needs recursion, not a simple loop, because '_' means
     * "try every possible letter here."
     *
     * Base case: if *pattern == '\0', return root->isEndOfWord.
     * If *pattern == '_': try trieMatchPattern(child, pattern+1) for
     *   every non-NULL child; return 1 if any succeeds.
     * Otherwise: follow the single matching child, same as trieSearch,
     *   and recurse with pattern+1.
     */
}

void trieFree(TrieNode* root) {
    if(!root) {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]){
            trieFree(root->children[i]);
        }
    }

    free(root);
}