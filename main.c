#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "struct.h"
#include "utils.h"

void printHive(char *hive, int reqLetInd) {
	printf("  Hive: \"%s\"\n", hive);
	printf("         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^");
	for (int i = reqLetInd + 1; i < strlen(hive); i++) {
		printf(" ");
	}
	printf(" (all words must include \'%c\')\n\n", hive[reqLetInd]);
}

void printList(WordList *thisWordList, char *hive, int totScore) {
	printf("  Word List:\n");
	for (int i = 0; i < thisWordList->numWords; i++) {
		if (isPangram(thisWordList->words[i], hive)) {
			(strlen(thisWordList->words[i]) == strlen(hive)) ? printf("***") : printf("*");
		}
		printf("  (%d) %s\n", getScore(thisWordList->words[i], hive), thisWordList->words[i]);
	}

	printf("  Total Score: %d\n", totScore);
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

void randHive(WordList *dictionaryList, char *hive, int hiveSize, char *reqLet) {
	WordList *fitWords = findAllFitWords(dictionaryList, hiveSize);
	char oldHive[hiveSize + 1]; // hold old hive (or empty hive if new game)
	strcpy(oldHive, hive);		// copy old hive into the temporary string
	char oldReqLet = *reqLet;	// hold old required letter (or null terminator if new game)

	do { // get a new random hive that is different than the previous hive
		int pickOne = rand() % fitWords->numWords;
		char *chosenFitWord = fitWords->words[pickOne];
		buildHive(chosenFitWord, hive);
	} while (strcmp(oldHive, hive) == 0);

	do { // get a new required letter that is different from the previous required letter
		int reqLetInd = rand() % hiveSize;
		*reqLet = hive[reqLetInd];
	} while (oldReqLet == reqLet);

	freeWordList(fitWords)
}

int main(int argc, char *argv[]) {
	printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");

	bool randMode = false;
	int hiveSize = 0;
	char dict[100] = "dictionary.txt";
	bool playMode = false;
	bool bruteForce = true;
	bool seedSelection = false;
	char hive[MAX_HIVE_SIZE + 1];
	hive[0] = '\0';
	int reqLetInd = -1;
	char reqLet = '\0';

	if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce,
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

	if (randMode) {
		printf("==== SET HIVE: RANDOM MODE ====\n");
		// find number of words in words array that use hiveSize unique letters
		newHive(fitWords, hive, hiveSize, &reqLet);
		// and alaphabetize the unique letters to make the letter hive

		reqLetInd = rand() % hiveSize;
		reqLet = hive[reqLetInd];
	} else {
		bool okInput = false;
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
			hiveSize = strlen(hive);
			reqLetInd = -1;
			reqLet = '\0';
			printf("  Enter the letter from \"%s\"\n  that is required for all "
				   "words: ",
				   hive);
			scanf(" %c", &reqLet);
			reqLet = tolower(reqLet);
			if (reqLet == '\0') {
				continue;
			}
			if (strchr(hive, reqLet) == NULL) {
				printf("  HIVE ERROR: \"%s\" does not contain the letter "
					   "\'%c\'\n\n",
					   hive, reqLet);
			} else {
				okInput = true;
				reqLetInd = strchr(hive, reqLet) - hive;
			}
		}
	}

	printHive(hive, reqLetInd);

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
		int userScore = 0;
		int thisHiveScore = 0;

		printf("............................................\n");
		printHive(hive, reqLetInd);

		printf("  Enter a word (enter DONE to quit): ");
		scanf("%s", userWord);
		printf("\n");

		while (strcmp(userWord, "DONE") != 0) {
			if (strcmp(userWord, "NEW") == 0 && thisHiveScore >= 20) {
				thisHiveScore = 0;
				// TODO create new hive
				// TODO build new solved list
			}
			if (isValidWord(userWord, hive, reqLet) &&
				findWord(dictionaryList, userWord, 0, dictionaryList->numWords - 1, false) >= 0) {
				appendWord(userWordList, userWord);
				userScore += getScore(userWord, hive);
				thisHiveScore += getScore(userWord, hive);
			}

			// prints the list and the hive, and gets the next input
			printf("\n");
			printList(userWordList, hive, userScore);
			printf("............................................\n");
			printHive(hive, reqLetInd);
			if (thisHiveScore >= 20) {
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
