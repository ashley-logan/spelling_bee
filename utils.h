#ifdef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <stdbool.h>

void buildHive(char *str, char *hive);
bool strLower(char *word);
bool noDups(char *word);
bool isValidWord(char *word, char *hive, char reqLet);
int findLetter(char *str, char aLet);
int countUniqueLetters(char *str);
bool isPangram(char *str, char *hive);
bool isPrefix(char *partWord, char *fullWord);
int getScore(char *word, char *hive);

#endif