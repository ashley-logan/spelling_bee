#include "struct.h"


const int MIN_HIVE_SIZE = 2; // smallest amount of words allowed
const int MAX_HIVE_SIZE = 12; // largest amount of words allowed
const int MIN_WORD_LENGTH = 4;


WordList* createWordList() {
    WordList* newList = malloc(sizeof(WordList)); // allocates memory for a new word list
    newList->capacity = 4; // sets the capacity to 4 words
    newList->numWords = 0; // sets the current number of words to 0
    newList->words = malloc(newList->capacity * sizeof(char*)); // allocates memory for the words array with enough mem to store 4 character pointers

    return newList; // returns the newly creates word list
}

void appendWord(WordList* thisWordList, char* newWord) {
    if(thisWordList->numWords == thisWordList->capacity) {
        char** largerList = realloc(thisWordList->words, (thisWordList->capacity * 2) * sizeof(char*));
        if (largerList != NULL) {
            thisWordList->words = largerList;
            thisWordList->capacity *= 2;
        }
    }
    thisWordList->words[thisWordList->numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char)); // allocate new memory for another word in the word list at the last position
    strcpy(thisWordList->words[thisWordList->numWords], newWord); // copy input word into the new string allocated for the word list
    (thisWordList->numWords)++;
    //---------------------------------------------------------------------
    /* TODO (Task 1-A): Write appendWord
    - adds newWord to the end of the thisWordList (malloc'ing memory and copying the string; do NOT just copy the input pointer)
    - increments thisWordList's numWords. If there is no space for the new word, reallocate with double the capacity
    - by malloc'ing new memory for the words array and freeing the old memory
    */

}

int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    FILE* dictFile = fopen(filename, "r");
    if (dictFile == NULL) { // if file doesn't open return -1
        return -1;
    }
    char tempStr[50];
    int maxLen = 0; // initialize maxLen to default of -1, will contain the length of the longest word

    while(fscanf(dictFile, "%s", tempStr) == 1) { // while not at the end of file and dictionaryList is not full
        if((int)strlen(tempStr) >= minLength) { // add word to list if length is at least the minimum parameter
            maxLen = (strlen(tempStr) > maxLen)? strlen(tempStr) : maxLen;
            appendWord(dictionaryList, tempStr);
        }
    }
    return maxLen;
    
    //---------------------------------------------------------------------
    /* TODO (Task 1-B): Write buildDictionary
    - Opens a valid file and adds all words that are at least minLength long to the dictionaryList
    - Returns the length of the longest valid word if all goes well, -1 otherwise
    */
}

void freeWordList(WordList* list) {
    for(int i = 0; i < list->numWords; i++) {
        free(list->words[i]);
    }
    free(list->words);
    free(list);
    //---------------------------------------------------------------------
    /* TODO (Task 1-C): Write freeWordList
    - Frees the memory used by the WordList, both the words' char* arrays and the char** words array itself
    */
}