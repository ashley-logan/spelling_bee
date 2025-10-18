#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct WordList_struct {
    char** words; // dynmaic array of strings
    int numWords; // number of strings
    int capacity; // maximum amount of strings that can be contained in the array
} WordList;


WordList* createWordList();
void appendWord(WordList* thisWordList, char* newWord);
int buildDictionary(char* filename, WordList* dictionaryList, int minLength);
void freeWordList(WordList* list);

extern const int MIN_HIVE_SIZE; // smallest amount of words allowed
extern const int MAX_HIVE_SIZE; // largest amount of words allowed
extern const int MIN_WORD_LENGTH;

#endif