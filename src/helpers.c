#include "helpers.h"
#include "puzzle_state.h"
#include <stdio.h>
#include <stdlib.h>

bool strLower(const char *word) {
	for (int i = 0; word[i] != '\0'; i++) {
		if (!islower(word[i])) {
			return false;
		}
	}
	return true;
}

bool noDups(const char *word) {
	for (int i = 0; i < strlen(word); i++) {
		if (strchr(word, word[i]) != &(word[i])) {
			return false;
		}
	}
	return true; // otherwise there are no duplicates
}

int isValidWord(const char *word, const Puzzle *puzzle) {
	if (strlen(word) < MIN_WORD_LENGTH) {
		return -3;
	}
	if (strchr(word, puzzle->reqLet) == NULL) {
		// if the word doesn't use the required letter return false
		return -2;
	}

	for (int i = 0; word[i] != '\0'; i++) {
		// if the word contains a letter that isn't in the hive return false
		if (strchr(puzzle->hive, word[i]) == NULL) {
			return -1;
		}
	}
	return 1; // otherwise return true
}

int findLetter(const char *str, const char aLet) {
	char *ptr = strchr(str, aLet);
	if (ptr == NULL) {
		return -1;
	}

	return ptr - str;
}

int countUniqueLetters(const char *str) {
	int count = 0;
	int letterCount[26] = {0};
	int asciiLowerBound = 97;
	char currChar;
	for (int i = 0; i < strlen(str); i++) {
		currChar = tolower(str[i]);
		if (isalpha(currChar)) {
			int ind = abs(asciiLowerBound - (int)currChar);
			letterCount[ind]++;
		}
	}
	for (int i = 0; i < 26; i++) {
		if (letterCount[i] >= 1) {
			count++;
		}
	}
	return count;
}

bool isPangram(const char *str, const char *hive) {
	return countUniqueLetters(str) == countUniqueLetters(hive);
}

bool isPrefix(const char *partWord, const char *fullWord) {
	if (strlen(partWord) >= strlen(fullWord)) {
		// if the prefix is equal or longer than the full word it canno be a
		// prefix; return false
		return false;
	}

	return strncmp(partWord, fullWord, strlen(partWord)) == 0;
}
