#include "puzzle_state.h"
#include "helpers.h"
#include "print_utils.h"
#include <stdio.h>
#include <stdlib.h>

scoreCard createScoreCard() {
	// constructor function for the score struct
	scoreCard card = {0, 0, 0, 0, 0};
	return card;
}
WordList *createWordList() {
	WordList *newList =
		(WordList *)malloc(sizeof(WordList)); // allocates memory for a new word list
	newList->capacity = 4;					  // sets the capacity to 4 words
	newList->numWords = 0;					  // sets the current number of words to 0
	newList->words =
		malloc(newList->capacity * sizeof(char *)); // allocates memory for the words array with
													// enough mem to store 4 character pointers

	return newList; // returns the newly creates word list
}

Puzzle *createPuzzle() {
	// constructor function for the puzzle struct
	Puzzle *puzzle = (Puzzle *)malloc(sizeof(Puzzle)); // allocates memory for the puzzle
	puzzle->hive[0] = '\0';							   // sets the hive to the null terminator
	puzzle->reqLet = '\0';				   // sets the required letter to the null terminator
	puzzle->userScore = createScoreCard(); // calls the constructor function for a scoreCard to hold
										   // the user's score for the current puzzle
	puzzle->maxScore = createScoreCard();  // calls the constructor function for a scoreCard to hold
										   // the maximum possible score for the current puzzle
	puzzle->solvedList = createWordList(); // calls the constructor function for a WordList to hold

	return puzzle;
}

void getUserWord(char *userWord) {
	printf("To keep your total score and get a new hive enter NEW\n");
	printf("  Enter a word (enter DONE to quit): ");
	scanf("%s", userWord);
	printf("\n");
}

void buildHive(char *str, char *hive) {
	int i = 0;
	for (char c = 'a'; c <= 'z'; c++) { // loop through every lowercase letter in the alphabet
		if (i == MAX_HIVE_SIZE) {
			break;
		}
		if (findLetter((str), c) != -1 || findLetter(str, toupper(c)) != -1) {
			// if the word has the current letter (lowercase or uppercase)
			// append it to hive
			hive[i] = c;
			i++; // increment index
		}
	}
	hive[i] = '\0'; // null terminate hive
}
void randHive(WordList *dictionaryList, Puzzle ***puzzleArr, int *numPuzzles, int hiveSize) {
	*puzzleArr = (Puzzle **)realloc(*puzzleArr, sizeof(Puzzle *) * (*numPuzzles + 1));
	(*puzzleArr)[*numPuzzles] = createPuzzle();
	(*numPuzzles)++;
	char previousHive[MAX_HIVE_SIZE + 1];
	previousHive[0] = '\0';
	char previousReqLet = '\0';
	if (*numPuzzles > 1) {
		// creating first hive
		strcpy(previousHive, (*puzzleArr)[*numPuzzles - 2]->hive);
		previousReqLet = (*puzzleArr)[*numPuzzles - 2]->reqLet;
	}
	char *hive = (*puzzleArr)[*numPuzzles - 1]->hive;
	char *reqLet = &((*puzzleArr)[*numPuzzles - 1]->reqLet);
	// hold old hive (or empty hive if new game)
	WordList *fitWords = findAllFitWords(dictionaryList, hiveSize);

	printf("==== SET HIVE: RANDOM MODE ====\n");
	do { // get a new random hive that is different than the previous hive
		int pickOne = rand() % fitWords->numWords;
		char *chosenFitWord = fitWords->words[pickOne];
		buildHive(chosenFitWord, hive);
		printf("new hive created\n");
	} while (strcmp(previousHive, hive) == 0);

	do { // get a new required letter that is different from the previous required letter
		int reqLetInd = rand() % hiveSize;
		(*puzzleArr)[*numPuzzles - 1]->reqLet = hive[reqLetInd];
		printf("new letter set\n");
	} while (previousReqLet == *reqLet);

	freeWordList(fitWords);
	printf("leaving rand hive creation\n");
}

void customHive(WordList *dictionaryList, Puzzle ***puzzleArr, int *numPuzzles) {
	*puzzleArr = realloc(*puzzleArr, sizeof(Puzzle *) * (*numPuzzles + 1));
	(*puzzleArr)[*numPuzzles] = createPuzzle();
	(*numPuzzles)++;
	char *hive = (*puzzleArr)[*numPuzzles - 1]->hive;
	char *reqLet = &((*puzzleArr)[*numPuzzles - 1]->reqLet);
	bool okInput = false;
	int reqLetInd = -1;
	while (!okInput) {
		printf("==== SET HIVE: USER MODE ====\n");
		printf("  Enter a single string of lower-case,\n  unique letters for "
			   "the "
			   "letter hive... ");
		scanf("%s", hive);
		if (strlen(hive) < MIN_HIVE_SIZE || strlen(hive) > MAX_HIVE_SIZE) {
			printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive "
				   "size is "
				   "between %d and %d, inclusive\n\n",
				   hive, MIN_HIVE_SIZE, MAX_HIVE_SIZE);
		} else if (!strLower(hive)) {
			printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid "
				   "characters are lower-case alpha only\n\n",
				   hive);
		} else if (!noDups(hive)) {
			printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n", hive);
		} else {
			okInput = true;
		}
	}

	okInput = false;
	while (!okInput) {
		*reqLet = '\0';
		printf("  Enter the letter from \"%s\"\n  that is required for all "
			   "words: ",
			   hive);
		scanf(" %c", reqLet);
		*reqLet = tolower(*reqLet);
		if (*reqLet == '\0') {
			continue;
		}
		if (strchr(hive, *reqLet) == NULL) {
			printf("  HIVE ERROR: \"%s\" does not contain the letter "
				   "\'%c\'\n\n",
				   hive, *reqLet);
		} else {
			okInput = true;
		}
	}
}

void setScore(
	scoreCard *score, int newWords, int newScore, int newPangrams, int newPerfs, int newBingos) {
	// increases the fields of the score parameter based on the new values
	score->numWords += newWords;
	score->score += newScore;
	score->pangrams += newPangrams;
	score->perfPangrams += newPerfs;
	score->bingos += newBingos;
}

int checkBingo(WordList *list, Puzzle *puzzle) {
	int bingo = 1;
	int *bingoTrack = calloc(strlen(puzzle->hive), sizeof(int));
	for (int i = 0; i < list->numWords; i++) {
		char *hiveInd = strchr(puzzle->hive, (list->words[i])[0]);
		bingoTrack[hiveInd - puzzle->hive]++;
	}
	for (int i = 0; i < strlen(puzzle->hive); i++) {
		if (bingoTrack[i] < 0) {
			bingo = 0;
		}
	}
	free(bingoTrack);
	return bingo;
}

int calcScore(const char *word, const char *hive) {
	// calculates the score for the word based on the current puzzle
	if (strlen(word) < MIN_WORD_LENGTH) {
		return -1;
	}
	int pts = (strlen(word) == MIN_WORD_LENGTH) ? 1 : strlen(word);
	if (isPangram(word, hive)) {
		pts += strlen(hive);
	}
	return pts;
}

void populateSolvedList(WordList *dictionaryList,
						int maxWordLength,
						Puzzle *puzzle,
						bool bruteForce) {
	// this function fills the solved list with all possible words for the current puzzle
	if (bruteForce) { // find all words that work... (1) brute force
		bruteForceSolve(dictionaryList, puzzle);
	} else {
		char *tryWord = (char *)malloc(sizeof(char) * (maxWordLength + 1));

		tryWord[0] = puzzle->hive[0];
		tryWord[1] = '\0';
		int *letTracker =
			calloc(strlen(puzzle->hive), sizeof(int)); // tracks the index of the letter (in
													   // the hive) being appended or replaced
		findAllMatches(dictionaryList, puzzle->solvedList, tryWord, puzzle, maxWordLength,
					   letTracker);
		free(tryWord);
		free(letTracker);
	}
	printList(puzzle->solvedList, puzzle);

	int longestSolvedWordLen = 0;
	for (int i = 0; i < puzzle->solvedList->numWords; i++) {
		if (strlen(puzzle->solvedList->words[i]) > longestSolvedWordLen) {
			longestSolvedWordLen = strlen(puzzle->solvedList->words[i]);
		}
	}
}
void populateMaxScore(Puzzle *puzzle) {
	// populates the maxScore field of the current puzzle
	WordList *solvedList = puzzle->solvedList;
	int newScore = 0, newPangrams = 0, newPerfPangrams = 0, newBingo = 0;
	int newWords = solvedList->numWords;
	for (int i = 0; i < solvedList->numWords; i++) {
		newScore += calcScore(solvedList->words[i], puzzle->hive);
		if (isPangram(solvedList->words[i], puzzle->hive)) {
			newPangrams++;
			if (strlen(solvedList->words[i]) == strlen(puzzle->hive)) {
				newPerfPangrams++;
			}
		}
	}
	newBingo = checkBingo(solvedList, puzzle);
	setScore(&(puzzle->maxScore), newWords, newScore, newPangrams, newPerfPangrams, newBingo);
	printf("max score: %d\n", newScore);
}

void updateUserScore(char *word, Puzzle *puzzle) {
	// updates the userScore field for the current puzzle object
	int pangram = 0;
	int perfPangram = 0;
	int wordScore = calcScore(word, puzzle->hive);
	if (isPangram(word, puzzle->hive)) {
		pangram = 1;
		if (strlen(word) == strlen(puzzle->hive)) {
			perfPangram = 1;
		}
	}
	setScore(&(puzzle->userScore), 1, wordScore, pangram, perfPangram, 0);
}
void calcEndGameScore(scoreCard *finalUserScore,
					  scoreCard *finalMaxScore,
					  Puzzle **puzzleArr,
					  int numPuzzles) {
	// adds up the total max possible score and user score for all puzzles in the game
	for (int i = 0; i < numPuzzles; i++) {
		printPuzzleResult(puzzleArr[i]);
		setScore(finalMaxScore, puzzleArr[i]->maxScore.numWords, puzzleArr[i]->maxScore.score,
				 puzzleArr[i]->maxScore.pangrams, puzzleArr[i]->maxScore.perfPangrams,
				 puzzleArr[i]->maxScore.bingos);
		setScore(finalUserScore, puzzleArr[i]->userScore.numWords, puzzleArr[i]->userScore.score,
				 puzzleArr[i]->userScore.pangrams, puzzleArr[i]->userScore.perfPangrams,
				 puzzleArr[i]->userScore.bingos);
	}
}

void appendWord(WordList *thisWordList, char *newWord) {
	if (thisWordList->numWords == thisWordList->capacity) {
		char **largerList =
			realloc(thisWordList->words, (thisWordList->capacity * 2) * sizeof(char *));
		if (largerList != NULL) {
			thisWordList->words = largerList;
			thisWordList->capacity *= 2;
			thisWordList->words = largerList;
		}
	}
	thisWordList->words[thisWordList->numWords] = (char *)malloc(
		(strlen(newWord) + 1) * sizeof(char)); // allocate new memory for another word
											   // in the word list at the last position
	strcpy(thisWordList->words[thisWordList->numWords],
		   newWord); // copy input word into the new string allocated for the word
					 // list
	(thisWordList->numWords)++;
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

void bruteForceSolve(WordList *dictionaryList, Puzzle *puzzle) {
	for (int i = 0; i < dictionaryList->numWords; i++) {
		if (isValidWord(dictionaryList->words[i], puzzle) > 0) {
			appendWord(puzzle->solvedList, dictionaryList->words[i]);
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
					Puzzle *puzzle,
					int maxLen,
					int *letTracker) {
	int curLen = strlen(tryWord);
	if (curLen > maxLen) {
		return;
	}

	int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1, false);

	if (index >= 0 && isValidWord(tryWord, puzzle) > 0) {
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

	for (int i = 0; puzzle->hive[i] != '\0'; i++) {
		if (letTracker[i] == 0) {
			letTracker[i] = 1;
			tryWord[curLen] = puzzle->hive[i];
			tryWord[curLen + 1] = '\0';

			findAllMatches(dictionaryList, solvedList, tryWord, puzzle, maxLen, letTracker);

			tryWord[curLen] = '\0';
			letTracker[i] = 0;
		}
	}
}
void freeWordList(WordList *list) {
	for (int i = 0; i < list->numWords; i++) {
		free(list->words[i]);
	}
	free(list->words);
	free(list);
	list = NULL;
}

void freeGame(Puzzle ***puzzleArr, int numPuzzles) {
	for (int i = 0; i < numPuzzles; i++) {
		freeWordList((*puzzleArr)[i]->solvedList);
		free((*puzzleArr)[i]);
	}
	free(*puzzleArr);
	puzzleArr = NULL;
}