#include "struct.h"

const int MIN_HIVE_SIZE = 2;  // smallest amount of words allowed
const int MAX_HIVE_SIZE = 12; // largest amount of words allowed
const int MIN_WORD_LENGTH = 4;

WordList *createWordList() {
	WordList *newList = malloc(sizeof(WordList)); // allocates memory for a new word list
	newList->capacity = 4;						  // sets the capacity to 4 words
	newList->numWords = 0;						  // sets the current number of words to 0
	newList->words =
		malloc(newList->capacity * sizeof(char *)); // allocates memory for the words array with
													// enough mem to store 4 character pointers

	return newList; // returns the newly creates word list
}

void appendWord(WordList *thisWordList, char *newWord) {
	if (thisWordList->numWords == thisWordList->capacity) {
		char **largerList =
			realloc(thisWordList->words, (thisWordList->capacity * 2) * sizeof(char *));
		if (largerList != NULL) {
			thisWordList->words = largerList;
			thisWordList->capacity *= 2;
		}
	}
	thisWordList->words[thisWordList->numWords] = (char *)malloc(
		(strlen(newWord) + 1) * sizeof(char)); // allocate new memory for another word
											   // in the word list at the last position
	strcpy(thisWordList->words[thisWordList->numWords],
		   newWord); // copy input word into the new string allocated for the word
					 // list
	(thisWordList->numWords)++;
	//---------------------------------------------------------------------
	/* TODO (Task 1-A): Write appendWord
	- adds newWord to the end of the thisWordList (malloc'ing memory and copying
	the string; do NOT just copy the input pointer)
	- increments thisWordList's numWords. If there is no space for the new word,
	reallocate with double the capacity
	- by malloc'ing new memory for the words array and freeing the old memory
	*/
}

int buildDictionary(char *filename, WordList *dictionaryList, int minLength) {
	FILE *dictFile = fopen(filename, "r");
	if (dictFile == NULL) { // if file doesn't open return -1
		return -1;
	}
	char tempStr[50];
	int maxLen = 0; // initialize maxLen to default of -1, will contain the length
					// of the longest word

	while (fscanf(dictFile, "%s", tempStr) ==
		   1) { // while not at the end of file and dictionaryList is not full
		if ((int)strlen(tempStr) >= minLength) { // add word to list if length is at
												 // least the minimum parameter
			maxLen = (strlen(tempStr) > maxLen) ? strlen(tempStr) : maxLen;
			appendWord(dictionaryList, tempStr);
		}
	}
	return maxLen;

	//---------------------------------------------------------------------
	/* TODO (Task 1-B): Write buildDictionary
	- Opens a valid file and adds all words that are at least minLength long to
	the dictionaryList
	- Returns the length of the longest valid word if all goes well, -1 otherwise
	*/
}

void freeWordList(WordList *list) {
	for (int i = 0; i < list->numWords; i++) {
		free(list->words[i]);
	}
	free(list->words);
	free(list);
	//---------------------------------------------------------------------
	/* TODO (Task 1-C): Write freeWordList
	- Frees the memory used by the WordList, both the words' char* arrays and the
	char** words array itself
	*/
}

WordList *findAllFitWords(WordList *dictionaryList, int hiveSize) {
	WordList *fitWords = createWordList();
	for (int i = 0; i < dictionaryList->numWords; i++) {
		if (countUniqueLetters(dictionaryList->words[i]) == hiveSize) {
			appendWord(fitWords, dictionaryList->words[i]);
		}
	}
	return fitWords;
	//---------------------------------------------------------------------
	/* TODO (Task 3-B): Write findAllFitWords
	- Creates a WordList and adds all fit words from dictionaryList to the new
	WordList (A fit word has exactly hiveSize unique letters)
	*/
}

void bruteForceSolve(WordList *dictionaryList, WordList *solvedList, char *hive, char reqLet) {
	for (int i = 0; i < dictionaryList->numWords; i++) {
		if (isValidWord(dictionaryList->words[i], hive, reqLet)) {
			appendWord(solvedList, dictionaryList->words[i]);
		}
	}
	//---------------------------------------------------------------------
	/* TODO (Task 5-A): Write bruteForceSolve
	- Goes through each word in the dictionary and determines if that is a
	solution to the puzzle
	- Adds the word to solvedList if it is a solution
	*/
}

int findWord(WordList *thisWordList, char *aWord, int loInd, int hiInd, bool isRoot) {
	/* TODO (Task 6-B): Complete findWord
	 * - Fix all the if statements that say 'if(true)' with the correct binary
	 * search cases
	 */
	if (hiInd < loInd) { // Base case 2: aWord not found in words[]

		if ((loInd < thisWordList->numWords) && isRoot) {
			return -1; // words match this root (partial match)
		} else {
			return -99; // no more words matching this root (no match)
		}
	}

	int mdInd = (hiInd + loInd) / 2;

	if (strcmp(aWord, thisWordList->words[mdInd]) == 0) { // Base case 1: found tryWord at midInd
		return mdInd;
	}

	if (isPrefix(aWord, thisWordList->words[mdInd])) {
		// if the word is a prefix of a word in the word list, set the flag to
		// true
		isRoot = true;
	}

	if (strcmp(aWord, thisWordList->words[mdInd]) > 0) { // Recursive case: search upper half
		return findWord(thisWordList, aWord, mdInd + 1, hiInd, isRoot);
	}

	// Recursive case: search lower half
	return findWord(thisWordList, aWord, loInd, mdInd - 1, isRoot);
}

void findAllMatches(WordList *dictionaryList,
					WordList *solvedList,
					char *tryWord,
					char *hive,
					char reqLet,
					int maxLen,
					int *letTracker) {
	int curLen = strlen(tryWord);
	if (curLen > maxLen) {
		return;
	}

	int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1, false);

	if (index >= 0 && isValidWord(tryWord, hive, reqLet)) {
		// tryWord found in dicionary and is valid
		if (findWord(solvedList, tryWord, 0, solvedList->numWords - 1, false) < 0) {
			// word is not already in solved list
			appendWord(solvedList, tryWord);
		}
	}

	if (index == -99) {
		// no match and no prefix, do not append
		return;
	}

	for (int i = 0; hive[i] != '\0'; i++) {
		if (letTracker[i] == 0) {
			letTracker[i] = 1;
			tryWord[curLen] = hive[i];
			tryWord[curLen + 1] = '\0';

			findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet, maxLen, letTracker);

			tryWord[curLen] = '\0';
			letTracker[i] = 0;
		}
	}
}