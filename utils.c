#include "utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

void buildHive(char *str, char *hive) {
	char *travHive = hive;
	for (char c = 'a'; c <= 'z'; c++) { // loop through every lowercase letter in the alphabet
		if (findLetter((str), c) != -1 || findLetter(str, toupper(c)) != -1) {
			// if the word has the current letter (lowercase or uppercase)
			// append it to hive
			*travHive = c;
			travHive++; // increment hive
		}
	}
	*travHive = '\0'; // null terminate hive
					  //---------------------------------------------------------------------
					  /* TODO (Task 2-B): Write buildHive
					  - take each character of the string and add it to the hive in alphabetical
					  order
					  - no letter should be added twice
					  - ensure that hive is null-terminated at the end
					  (findLetter is helpful here)
					  */
}

bool strLower(char *word) {
	for (int i = 0; word[i] != '\0'; i++) {
		if (!islower(word[i])) {
			return false;
		}
	}
	return true;
}

bool noDups(char *word) {
	char *travWord = word;
	while (*travWord != '\0') {
		if (strchr(word, *travWord) != travWord) {
			// if strchr returns a pointer to a location that is not the current
			// pointer, then another instance of the letter occurs
			return false;
		}
		travWord++; // increment the pointer
	}
	return true; // otherwise there are no duplicates
}

bool isValidWord(char *word, char *hive, char reqLet) {
	if (strchr(word, reqLet) == NULL) {
		// if the word doesn't use the required letter return false
		return false;
	}

	for (int i = 0; word[i] != '\0'; i++) {
		// if the word contains a letter that isn't in the hive return false
		if (strchr(hive, word[i]) == NULL) {
			return false;
		}
	}
	//---------------------------------------------------------------------
	/* TODO (Task 4-A, part 1 or 2): Write isValidWord
	- Returns true if word
	(a) uses ONLY letters that are in the hive, and
	(b) MUST use the required letter.
	- Returns false otherwise
	*/
	return true; // otherwise return true
}

int findLetter(char *str, char aLet) {
	char *ptr = strchr(str, aLet);
	if (ptr == NULL) {
		return -1;
	}

	return ptr - str;
	//---------------------------------------------------------------------
	/* TODO (Task 2-A): Write findLetter
	- Returns the index of aLet in str, if it exists
	- Otherwise returns -1
	*/
}

int countUniqueLetters(char *str) {
	int count = 0;
	for (int i = 0; i < 256; i++) {
		if (findLetter(str, (char)i) != -1) {
			count++;
		}
	}
	return count;
	//---------------------------------------------------------------------
	/* TODO (Task 3-A): Write countUniqueLetters
	- Counts and returns the number of unique letters in the input string
	(note: this only needs to work for lower-case alpha characters, i.e.
	letters, for this project, but it should work generally for all characters)
	*/
}

bool isPangram(char *str, char *hive) {
	return countUniqueLetters(str) == countUniqueLetters(hive);
	//---------------------------------------------------------------------
	/* TODO (Task 4-B): Write isPangram
	- Returns true if str is a pangram (uses all the letters in hive at least
	once)
	- Returns false otherwise
	*/
}

bool isPrefix(char *partWord, char *fullWord) {
	if (strlen(partWord) >= strlen(fullWord)) {
		// if the prefix is equal or longer than the full word it canno be a
		// prefix; return false
		return false;
	}

	return strncmp(partWord, fullWord, strlen(partWord)) == 0;
	//---------------------------------------------------------------------
	/* TODO (Task 6-A): Write isPrefix
	- checks if partWord is the root/prefix of fullWord, such that
		partWord is a perfect match to the beginning (arbitrary length) of
	fullWord
	- returns true is partWord is a root/prefix of fullWord; returns false
	otherwise
	*/
}
