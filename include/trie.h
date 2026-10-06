#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

// this function creates and intializes trie node
TrieNode* trieCreateNode (void);

// insert lowercase word into trie
void trieInsert(TrieNode* root, const char* word);

// return 1 if 'word' exists as a complete word, else 0
int trieSearch(TrieNode* root, const char* word);

// if word ins trie starts with the 'prefix' provided return 1, else 0
int trieStartsWith(TrieNode* root, const char* prefix);

// return 1 if word in trie matches 'pattern', where pattern uses '_' as a wildcard for 'letter not known yet' 
int trieMatchPattern(TrieNode* root, const char* pattern);

void trieFree(TrieNode* root);

#endif