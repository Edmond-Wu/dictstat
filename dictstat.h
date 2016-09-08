#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {false, true} bool;

//Defines node struct
typedef struct node {
        int word_count;
	      int superword_count;
	      int prefix_count;
        char *key;
        struct node *children[26];
} node;

//Initializes trie
node* makeRoot();

//Creates a word-node
node* makeNode();

//Converts word to lower case
char* stringLower(char *word);

//Inserts word-node into tree (from dictionary)
void insertWord(char *word);

//Increments prefixes
void incrementPrefixes(node *word_node);

/*Searches trie for a existence of a word and increments count if found
Also looks for prefixes and superwords*/
void addWord(char *word);

//Prints out all entries in the trie
void printTrie(node *temp);

//Frees up memory taken by the trie
void destroyTrie(node *temp);

//Reads dictionary file
void readDict(FILE *dict_file);

//Reads data file
void scanData(FILE *data_file);
