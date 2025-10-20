#ifndef STRUCT_H
#define STRUCT_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WordList_struct {
	char **words; // dynmaic array of strings
	int numWords; // number of strings
	int capacity; // maximum amount of strings that can be contained in the
				  // array
} WordList;

WordList *createWordList();
void appendWord(WordList *thisWordList, char *newWord);
int buildDictionary(char *filename, WordList *dictionaryList, int minLength);
void freeWordList(WordList *list);
WordList *findAllFitWords(WordList *dictionaryList, int hiveSize);
void bruteForceSolve(WordList *dictionaryList, WordList *solvedList, char *hive, char reqLet);
int findWord(WordList *thisWordList, char *aWord, int loInd, int hiInd, bool isRoot);

extern const int MIN_HIVE_SIZE;	  // hive can be no smaller than this constant
extern const int MAX_HIVE_SIZE;	  // hive can be no larger than this constant
extern const int MIN_WORD_LENGTH; // words must be at least this long

#endif