#ifndef HELPERS_H
#define HELPERS_H
#include "puzzle_state.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/*
all functions are constant
this library only performs checks or counts
and does not modify the inputs
*/
bool strLower(const char *word);
bool noDups(const char *word);
int isValidWord(const char *word, const Puzzle *puzzle);
int findLetter(const char *str, const char aLet);
int countUniqueLetters(const char *str);
bool isPangram(const char *str, const char *hive);
bool isPrefix(const char *partWord, const char *fullWord);

#endif