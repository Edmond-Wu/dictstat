#include "dictstat.h"
#define ALPHABET 26

/*Program that takes words from a dictionary file, creates a trie out of those words,
then reads from a data file, counting the number of occurrences, prefixes, and superwords based on what's
in the dictionary.*/

Node *root = NULL;

//Creates a generic word-node
Node* makeNode() {
	Node *temp = malloc(sizeof(Node));
	temp->key = NULL;
	temp->word_count = 0;
	temp->prefix_count = 0;
	temp->superword_count = 0;
	return temp;
}

//Converts word to lower-case
char* stringLower(char *word) {
	int length = strlen(word);
        char *copy = (char *)malloc(sizeof(char) * (1 + length));
	strcpy(copy, word);
        for (int i = 0; i < length; i++)
                copy[i] = tolower(word[i]);
	copy[length] = '\0';
	return copy;
}

//Adds a word-node to the trie
void insertWord(char *word) {
	int word_length = strlen(word);
	Node *pointer = root;
	char *copy = stringLower(word);
	for (int i = 0; i < word_length; i++) {
	        char current_letter = copy[i];
	        int index = current_letter - 'a';
		if (pointer->children[index] == NULL)
			pointer->children[index] = makeNode();
		pointer = pointer->children[index];
	}
	pointer->key = copy;
}

//Increments the prefixes of word
void incrementPrefixes(Node *word_node) {
	if (word_node == NULL)
                return;
	if (word_node->key != NULL)
		word_node->prefix_count++;
        for (int i = 0; i < ALPHABET; i++) {
		if (word_node->children[i] != NULL)
			incrementPrefixes(word_node->children[i]); //recursive call
	}
}


/*Takes a word from the data file, searches through the tree and increments its occurrence if it is in the trie
Also increments the superword and prefix count of any word nodes reached before the end of the word*/
void addWord(char *word) {
        int word_length = strlen(word);
	if (root == NULL) {
		fprintf(stderr, "Empty dictionary.\n");
		return;
	}
        Node *pointer = root;
        char *copy = stringLower(word);
        for (int i = 0; i < word_length; i++) {
		char current_letter = copy[i];
		int index = current_letter - 'a';
		if (pointer->key != NULL)
			pointer -> superword_count++;
		if (pointer->children[index] == NULL) {
			free(copy);
			return;
		}
                pointer = pointer->children[index];
        }
	free(copy);
	if (pointer->key != NULL) {
		pointer->word_count++;
		//calls the prefix function on all the word's children
		for (int i = 0; i < ALPHABET; i++) {
                	if (pointer->children[i] != NULL)
                        	incrementPrefixes(pointer->children[i]);
		}
        }
	else
		incrementPrefixes(pointer); //else if the last pointer didn't have a key then just call the prefix functio
}


//Traverses the tree and prints out all the entries
void printTrie(Node *temp) {
        if (temp == NULL)
                return;
        if (temp->key && temp != root)
		printf("%s %d %d %d\n", temp->key, temp->word_count, temp->prefix_count, temp->superword_count);
        for (int i = 0; i < ALPHABET; i++) {
		if (temp->children[i] != NULL)
			printTrie(temp->children[i]); //recursive call
        }
}

//Frees the trie
void destroyTrie(Node *temp) {
	if (temp == NULL)
		return;
	for (int i = 0; i < ALPHABET; i++)
		destroyTrie(temp->children[i]);
	free(temp->key);
	free(temp);
}

//Reads a dictionary file
void readDict(FILE *dict_file) {
	if (dict_file == NULL) {
		fprintf(stderr, "invalid input\n");
		return;
	}
	//stores the contents in the file as a string
	fseek(dict_file, 0, SEEK_END);
	size_t length = ftell(dict_file);
	rewind(dict_file);
	char *buffer = (char *)malloc(sizeof(char) * length);
	fread(buffer, 1, length, dict_file);
	char word[100];
	memset(word, 0, sizeof(word));
	bool is_empty = true;
	for (int i = 0; i < length; i++) {
		buffer[i] = tolower(buffer[i]);
		if (isalpha(buffer[i])) {
			is_empty = false;
			*(word + strlen(word)) = buffer[i];
		}
		else {
			if (word[0] != 0)
				insertWord(word);
			memset(word, 0, sizeof(word));
		}
	}
	free(buffer);
	if (is_empty == true)
		printf("Empty dictionary.\n");
}

//Scans the data file for words from the dictionary
void scanData(FILE *data_file) {
	if (data_file == NULL) {
                fprintf(stderr, "invalid input\n");
		return;
        }
	//stores file contents as a string
        fseek(data_file, 0, SEEK_END);
        size_t length = ftell(data_file);
        rewind(data_file);
        char *buffer = (char *)malloc(sizeof(char) * length);
        fread(buffer, 1, length, data_file);
        char word[100];
        memset(word, 0, sizeof(word));
        for (int i = 0; i < length; i++) {
                buffer[i] = tolower(buffer[i]);
                if (isalpha(buffer[i]))
                        *(word + strlen(word)) = buffer[i];
                else {
                        if (word[0] != 0)
				addWord(word);
                        memset(word, 0, sizeof(word));
                }
        }
	free(buffer);
}

//Main method
int main(int argc, char **argv) {
	if (argc != 3)
                fprintf(stderr, "invalid number of arguments\n");
	else {
		if (argv[1] == NULL || argv[2] == NULL)
			fprintf(stderr, "invalid input files\n");
		else {
			FILE *dict_pointer = fopen(argv[1], "r");
			if (dict_pointer == NULL)
				fprintf(stderr, "invalid dictionary file\n");
			else {
				root = makeNode();
				readDict(dict_pointer);
				FILE *data_pointer = fopen(argv[2], "r");
				if (data_pointer == NULL)
					fprintf(stderr, "invalid data file\n");
				else {
					scanData(data_pointer);
					fclose(data_pointer);
				}
				fclose(dict_pointer);
				printTrie(root);
				destroyTrie(root);
			}
		}
	}
	return 0;
}
