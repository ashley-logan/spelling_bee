#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "struct.h"
#include "utils.h"

void printHive(Hive *hive, int reqLetInd) {
	printf("  Hive: \"%s\"\n", hive->content);
	printf("         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^");
	for (int i = reqLetInd + 1; i < hive->size; i++) {
		printf(" ");
	}
	printf(" (all words must include \'%c\')\n\n", hive->content[reqLetInd]);
}

void printList(WordList *thisWordList, Hive *hive, scoreCard *userScore) {
	printf("  Word List:\n");
	for (int i = 0; i < thisWordList->numWords; i++) {
		if (isPangram(thisWordList->words[i], hive->content)) {
			userScore->pangrams++;
			if (strlen(thisWordList->words[i]) == hive->size) {
				userScore->perfPangrams++;
				printf("**");
			}
			printf("*");
		}
		printf
			// TODO all scores
			printf("  (%d) %s\n", getScore(solvedList->words[i], hive->content),
				   thisWordList->words[i]);
		hive->score += getScore(solvedList->words[i], hive->content);
	}

	printf("  Total Score: %d\n", hive->score);
}

// Provided for you, to determine the program settings based on parameters
bool setSettings(int argc,
				 char *argv[],
				 bool *pRandMode,
				 int *pNumLets,
				 char dictFile[100],
				 bool *pPlayMode,
				 bool *pBruteForceMode,
				 bool *pSeedSelection) {
	*pRandMode = false;
	*pNumLets = 0;
	strcpy(dictFile, "dictionary.txt");
	*pPlayMode = false;
	*pBruteForceMode = true;
	*pSeedSelection = false;
	srand((int)time(0));
	//--------------------------------------
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-r") == 0) {
			++i;
			if (argc == i) {
				return false;
			}
			*pRandMode = true;
			*pNumLets = atoi(argv[i]);
			if (*pNumLets < MIN_HIVE_SIZE || *pNumLets > MAX_HIVE_SIZE) {
				return false;
			}
		} else if (strcmp(argv[i], "-d") == 0) {
			++i;
			if (argc == i) {
				return false;
			}
			strcpy(dictFile, argv[i]);
			FILE *filePtr = fopen(dictFile, "r");
			if (filePtr == NULL) {
				return false;
			}
			fclose(filePtr);
		} else if (strcmp(argv[i], "-s") == 0) {
			++i;
			if (argc == i) {
				return false;
			}
			*pSeedSelection = true;
			int seed = atoi(argv[i]);
			srand(seed);
		} else if (strcmp(argv[i], "-p") == 0) {
			*pPlayMode = true;
		} else if (strcmp(argv[i], "-o") == 0) {
			*pBruteForceMode = false;
		} else {
			return false;
		}
	}
	return true;
}

void printONorOFF(bool mode) {
	if (mode) {
		printf("ON\n");
	} else {
		printf("OFF\n");
	}
}

void printYESorNO(bool mode) {
	if (mode) {
		printf("YES\n");
	} else {
		printf("NO\n");
	}
}
typedef struct {
	char content[MAX_HIVE_SIZE + 1];
	int size;
	int score;
} Hive

	Hive *
	createHive() {
	Hive newHive;
	newHive.score = 0;
	newHive.size = 0;
	return &newHive;
}

void randHive(WordList *dictionaryList,
			  Hive **hiveArr,
			  int *numHives,
			  int hiveSize,
			  char *reqLet,
			  int *reqLetInd) {
	hiveArr = realloc(hiveArr, sizeof(Hive *) * (numHives + 1));
	hiveArr[numHives] = createHive();
	hiveArr[numHives]->size = hiveSize;
	*numHives++;
	char oldHive[hiveArr[numHives]->size + 1];	 // hold old hive (or empty hive if new game)
	strcpy(oldHive, hiveArr[numHives]->content); // copy old hive into the temporary string
	char oldReqLet = *reqLet; // hold old required letter (or null terminator if new game)
	WordList *fitWords = findAllFitWords(dictionaryList, hiveSize);

	printf("==== SET HIVE: RANDOM MODE ====\n");

	do { // get a new random hive that is different than the previous hive
		int pickOne = rand() % fitWords->numWords;
		char *chosenFitWord = fitWords->words[pickOne];
		buildHive(chosenFitWord, hiveArr[numHives]->content);
	} while (strcmp(oldHive, hiveArr[numHives]->content) == 0);

	do { // get a new required letter that is different from the previous required letter
		*reqLetInd = rand() % hiveSize;
		*reqLet = hive[*reqLetInd];
	} while (oldReqLet == reqLet);

	freeWordList(fitWords);
}

void customHive(
	WordList *dictionaryList, Hive **hiveArr, int *numHives, char *reqLet, int *reqLetInd) {
	hiveArr = realloc(hiveArr, sizeof(Hive *) * (numHives + 1));
	hiveArr[numHives] = createHive();
	hiveArr[numHives]->size = hiveSize;
	bool okInput = false;
	int reqLetInd = -1;
	while (!okInput) {
		printf("==== SET HIVE: USER MODE ====\n");
		printf("  Enter a single string of lower-case,\n  unique letters for "
			   "the "
			   "letter hive... ");
		scanf("%s", hiveArr[numHives]->content);
		if (strlen(hiveArr[numHives]->content) < MIN_HIVE_SIZE ||
			strlen(curhiveArr[numHives]->content) > MAX_HIVE_SIZE) {
			printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive "
				   "size is "
				   "between %d and %d, inclusive\n\n",
				   hiveArr[numHives]->content, MIN_HIVE_SIZE, MAX_HIVE_SIZE);
		} else if (!strLower(currHive->content)) {
			printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid "
				   "characters are lower-case alpha only\n\n",
				   hiveArr[numHives]->content);
		} else if (!noDups(hiveArr[numHives]->content)) {
			printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n",
				   hiveArr[numHives]->content);
		} else {
			hiveArr[numHives]->size = strlen(hiveArr[numHives]->content);
			okInput = true;
		}
	}

	okInput = false;
	while (!okInput) {
		*reqLet = '\0';
		printf("  Enter the letter from \"%s\"\n  that is required for all "
			   "words: ",
			   hiveArr[numHives]->content);
		scanf(" %c", reqLet);
		*reqLet = tolower(*reqLet);
		if (*reqLet == '\0') {
			continue;
		}
		if (strchr(hiveArr[numHives]->content, *reqLet) == NULL) {
			printf("  HIVE ERROR: \"%s\" does not contain the letter "
				   "\'%c\'\n\n",
				   hive, *reqLet);
		} else {
			okInput = true;
			*reqLetInd = strchr(hiveArr[numHives]->content, *reqLet) - hiveArr[numHives]->content;
		}
	}
}

int printSingleHiveResult(WordList *solvedList, Hive *hive, scoreCard *possibleScore) {
	bool isBingo = true;
	int *bingoTrack = calloc(strlen(hive), sizeof(int));
	printList(solvedList, hive->content, totScore);
	int numValidWords = solvedList->numWords;
	for (int i = 0; i < numValidWords; i++) {
		if (isPangram(solvedList->words[i], hive->content)) {
			numPangrams++;
			if (strlen(solvedList->words[i]) == hive->size) {
				numPerfectPangrams++;
			}
		}

		char *hiveInd = strchr(hive->content, (solvedList->words[i])[0]);
		totScore +=
			getScore(solvedList->words[i], hive->content) bingoTrack[hiveInd - hive->content]++;
	}

	for (int i = 0; i < hive->size; i++) {
		if (bingoTrack[i] < 1) {
			isBingo = false;
		}
	}
	free(bingoTrack);

	possibleScore->numWords += numValidWords;
	possibleScore->pangrams += numPangrams;
	possibleScore->perfPangrams += numPerfectPangrams;
	if (isBingo) {
		possibleScore->bingos++;
	}
	possibleScore->score += totScore;

	// Additional results are printed here:
	printf("\n");
	printf("  Total counts for hive \"%s\":\n", hive->content);
	printf("                         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^\n");
	printf("    Number of Valid Words: %d\n", numValidWords);
	printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
	printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
	printf("    Bingo: ");
	printYESorNO(isBingo);
	printf("    Total Score Possible: %d\n", totScore);
}

void printAllResults(WordList *solvedList, Hive **hiveArr, int numHives) {

	for (int i = 0; i < numHives; i++) {
		printSingleHiveResult(solvedList, hiveArr[i], scoreCard * possibleScore);
	}
	printf("    Number of Valid Words Found: %d/%d\n", userValidWords, possibleScore->numWords);
	printf("    Number of ( * ) Pangrams Found: %d/%d\n", userPangrams, possibleScore->pangrams);
	printf("    Number of (***) Perfect Pangrams Found: %d/%d\n", userPerf,
		   possibleScore->perfPangrams);
	printf("    Number of Bingos Achieved: %d/%d\n", userBingos, possibleScore->bingos);
	printf("    Total Score Possible: %d\n", possibleScore->score);
}

int main(int argc, char *argv[]) {
	printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");
	Hive **hiveArr =
		NULL; // array that will hold each hive (only one if the user never resets the hive)
	bool randMode = false;
	char dict[100] = "dictionary.txt";
	bool playMode = false;
	bool bruteForce = true;
	bool seedSelection = false;
	int hiveSize = 0;
	int numHives = 0;
	char reqLet = '\0';
	int reqLetInd = -1;

	if (!setSettings(argc, argv, &randMode, &(currHive->size), dict, &playMode, &bruteForce,
					 &seedSelection)) {
		printf("Invalid command-line argument(s).\nTerminating program...\n");
		return 1;
	} else {
		printf("Program Settings:\n");
		printf("  random mode = ");
		printONorOFF(randMode);
		printf("  play mode = ");
		printONorOFF(playMode);
		printf("  brute force solution = ");
		printONorOFF(bruteForce);
		printf("  dictionary file = %s\n", dict);
		printf("  hive set = ");
		printYESorNO(randMode);
		printf("\n\n");
	}

	// build word array (only words with desired minimum length or longer) from
	// dictionary file
	printf("Building array of words from dictionary... \n");
	WordList *dictionaryList = createWordList();
	int maxWordLength = buildDictionary(dict, dictionaryList, MIN_WORD_LENGTH);
	if (maxWordLength == -1) {
		printf("  ERROR in building word array.\n");
		printf("  File not found or incorrect number of valid words.\n");
		printf("Terminating program...\n");
		return 0;
	}
	printf("   Word array built!\n\n");

	printf("Analyzing dictionary...\n");

	if (dictionaryList->numWords < 0) {
		printf("  Dictionary %s not found...\n", dict);
		printf("Terminating program...\n");
		return 0;
	}

	// end program if file has zero words of minimum desired length or longer
	if (dictionaryList->numWords == 0) {
		printf("  Dictionary %s contains insufficient words of length %d or "
			   "more...\n",
			   dict, MIN_WORD_LENGTH);
		printf("Terminating program...\n");
		return 0;
	} else {
		printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict,
			   dictionaryList->numWords, MIN_WORD_LENGTH);
	}

	(randMode) ? randHive(dictionaryList, hiveArr, &numHives, hiveSize, &reqLet, &reqLetInd)
			   : customHive(dictionaryList, hiveArr, &numHives, &reqLet, &reqLetInd);

	printHive(hiveArr[numHives - 1], reqLetInd);

	if (playMode) {
		printf("==== PLAY MODE ====\n");
		//---------------------------------------------------------------------
		//              BEGINNING OF OPEN-ENDED GAMEPLAY SECTION
		//---------------------------------------------------------------------

		/* TODO (Task 4, OPEN-ENDED GAMEPLAY): develop a creative gameplay
		   extension. The code is currently a framework for repeatedly taking in
		   words from the user, until they enter DONE. Extend/replace/modify
		   this section to implement your creative gameplay extension.

		*/
		char *userWord = (char *)malloc((maxWordLength + 1) * sizeof(char));
		strcpy(userWord, "default");

		WordList *userWordList = createWordList();
		scoreCard userScore = createScoreCard();
		scoreCard possibleScore = createScoreCard();
		int userScore = 0;

		printf("............................................\n");
		printHive(hiveArr[numHives - 1], reqLetInd);

		printf("  Enter a word (enter DONE to quit): ");
		scanf("%s", userWord);
		printf("\n");

		while (strcmp(userWord, "DONE") != 0) {
			if (strcmp(userWord, "NEW") == 0 && hive->score >= 20) {
				printSingleHiveResult(solvedList, hiveArr[numHives - 1], &possibleScore);
				(randMode)
					? randHive(dictionaryList, hiveArr, &numHives, hiveSize, &reqLet, &reqLetInd)
					: customHive(dictionaryList, hiveArr, &numHives, &reqLet, &reqLetInd);
				printf("  Enter a word (enter DONE to quit): ");
				scanf("%s", userWord);
				printf("\n");
				continue;
			}
			if (isValidWord(userWord, hive, reqLet) &&
				findWord(dictionaryList, userWord, 0, dictionaryList->numWords - 1, false) >= 0)
				appendWord(userWordList, userWord);
			hive->score += getScore(userWord, hive->content);
			userScore += getScore(userWord, hive->content);

			// prints the list and the hive, and gets the next input
			printf("\n");
			printList(userWordList, hiveArr[numHives - 1], userScore);
			printf("............................................\n");
			printHive(hive, reqLetInd);
			if (hive->score >= 20) {
				printf("To keep your total score and get a new hive enter NEW\n");
			}
			printf("  Enter a word (enter DONE to quit): ");
			scanf("%s", userWord);
			printf("\n");
		}

		freeWordList(userWordList);
		free(userWord);

		//---------------------------------------------------------------------
		//                 END OF OPEN-ENDED GAMEPLAY SECTION
		//---------------------------------------------------------------------
	}

	printf("==== SPELLING BEE SOLVER ====\n");
	int totScore = 0;
	for (int i = 0; i < numHives; i++) {
	}
	printf("  Valid words from hive \"%s\":\n", hive);
	printf("                         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^\n");

	WordList *solvedList = createWordList();

	if (bruteForce) { // find all words that work... (1) brute force
		bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
	} else {
		char *tryWord = (char *)malloc(sizeof(char) * (maxWordLength + 1));

		tryWord[0] = hive[0];
		tryWord[1] = '\0';
		int *letTracker = calloc(strlen(hive), sizeof(int)); // tracks the index of the letter (in
															 // the hive) being appended or replaced
		findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet, maxWordLength,
					   letTracker);
		free(tryWord);
		free(letTracker);
	}

	int longestSolvedWordLen = 0;
	for (int i = 0; i < solvedList->numWords; i++) {
		if (strlen(solvedList->words[i]) > longestSolvedWordLen) {
			longestSolvedWordLen = strlen(solvedList->words[i]);
		}
	}

	// Helpful variables
	bool isBingo = true;
	int numPangrams = 0;
	int numPerfectPangrams = 0;
	int totScore = 0;
	int *bingoTrack = calloc(strlen(hive), sizeof(int));

	int numValidWords = solvedList->numWords;
	for (int i = 0; i < numValidWords; i++) {
		if (isPangram(solvedList->words[i], hive)) {
			numPangrams++;
			if (strlen(solvedList->words[i]) == strlen(hive)) {
				numPerfectPangrams++;
			}
		}
		totScore += getScore(solvedList->words[i], hive);
		char *hiveInd = strchr(hive, (solvedList->words[i])[0]);
		bingoTrack[hiveInd - hive]++;
	}

	for (int i = 0; i < strlen(hive); i++) {
		if (bingoTrack[i] < 1) {
			isBingo = false;
		}
	}
	free(bingoTrack);

	//---------------------------------------------------------------------
	/* TODO (Task 5-B): Display solver results
	- Display list of words solver found, noting pangrams (*) and perfect
	pangrams
	(***) with asterisks
	- Calculate the number of valid words, number of pangrams, number of perfect
	pangrams, total possible score, and whether there is a bingo. Save those
	values in the corresponding variables, or update the printf statements below
	to dispay those values
	*/
	printList(solvedList, hive, totScore);

	// Additional results are printed here:
	printf("\n");
	printf("  Total counts for hive \"%s\":\n", hive);
	printf("                         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^\n");
	printf("    Number of Valid Words: %d\n", numValidWords);
	printf("    Number of ( * ) Pangrams: %d\n", numPangrams);
	printf("    Number of (***) Perfect Pangrams: %d\n", numPerfectPangrams);
	printf("    Bingo: ");
	printYESorNO(isBingo);
	printf("    Total Score Possible: %d\n", totScore);

	//---------------------------------------------------------------------
	/* TODO Extra credit: Display frequency table
	- Display a table showing the number of words starting with each letter and
	duration in a table.
	- For example, with the hive abcde and required letter e:
			  Frequency Table:
			  let  4  5  6  7  8
			  -------------------
			  a:  3  1  1  1  0
			  b:  3  1  4  0  0
			  c:  3  1  0  0  1
			  d:  3  0  3  0  0
			  e:  0  1  0  0  0
		The table shows that there are 3 words that start with a and are 4
	letters long, and 1 each that are 5, 6, and 7 letters long and start with a.
	Note that the lengths 4-8 are shown here because the longest word is 8
	letters long, but that will change depending on the hive
	*/

	freeWordList(dictionaryList);
	freeWordList(solvedList);
	printf("\n\n");
	return 0;
}
