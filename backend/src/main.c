#include "extra_cred.h"
#include "helpers.h"
#include "print_utils.h"
#include "puzzle_state.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
	strcpy(dictFile, "../data/dictionary.txt");
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

int main(int argc, char *argv[]) {
	printf("\n----- Welcome to the CS 211 Spelling Bee Game & Solver! -----\n\n");
	Puzzle **puzzleArr = NULL;
	int numPuzzles = 0;
	bool randMode = false;
	char dict[100] = "dictionary.txt";
	bool playMode = false;
	bool bruteForce = true;
	bool seedSelection = false;
	int hiveSize = 0;
	char reqLet = '\0';
	int reqLetInd = -1;

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
	int maxWordLength = buildDictionary(dict, dictionaryList,
										MIN_WORD_LENGTH); // creates a word list from dictionary
	if (maxWordLength == -1) {
		// checks that building dictionary list was successful
		printf("  ERROR in building word array.\n");
		printf("  File not found or incorrect number of valid words.\n");
		printf("Terminating program...\n");
		return 0;
	}
	printf("   Word array built!\n\n");

	printf("Analyzing dictionary...\n");

	if (dictionaryList->numWords < 0) {
		// makes sure dictionary isn't empty
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

	(randMode) ? randHive(dictionaryList, &puzzleArr, &numPuzzles, hiveSize)
			   : customHive(dictionaryList, &puzzleArr, &numPuzzles);
	// builds the hive; either prompting the user for input or creating it randomly
	if (puzzleArr == NULL) {
		printf("array is null\n");
		return 0;
	}
	Puzzle *currPuzzle = puzzleArr[numPuzzles - 1];

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

		printf("............................................\n");
		printHive(currPuzzle);

		printf("  Enter a word (enter DONE to quit): ");
		scanf("%s", userWord);
		printf("\n");

		while (strcmp(userWord, "DONE") != 0) {
			if (strcmp(userWord, "NEW") == 0) {
				populateSolvedList(dictionaryList, maxWordLength, currPuzzle, bruteForce);
				populateMaxScore(currPuzzle);
				printPuzzleResult(currPuzzle);
				(randMode) ? randHive(dictionaryList, &puzzleArr, &numPuzzles, hiveSize)
						   : customHive(dictionaryList, &puzzleArr, &numPuzzles);
				currPuzzle = puzzleArr[numPuzzles - 1];
				printHive(currPuzzle);
				getUserWord(userWord);
				continue;
			}
			int result = isValidWord(userWord, currPuzzle);
			if (result == -3) {
				printf("INVALID INPUT: %s is not long enough (must be >= %d letters)\n", userWord,
					   MIN_WORD_LENGTH);
			} else if (result == -2) {
				printf("INVALID INPUT: %s does not contain the required letter %c\n", userWord,
					   currPuzzle->reqLet);
			} else if (result == -1) {
				printf(
					"INVALID INPUT: %s contains one or more letters not in the current hive (%s)\n",
					userWord, currPuzzle->hive);
			} else if (findWord(dictionaryList, userWord, 0, dictionaryList->numWords, false) < 0) {
				printf("INVALID INPUT: %s is not present in the current dictionary\n", userWord);
			} else {
				appendWord(userWordList, userWord);
				updateUserScore(userWord, currPuzzle);
			}

			// prints the list and the hive, and gets the next input
			printf("\n");
			printList(userWordList, currPuzzle);
			printf("............................................\n");
			printHive(currPuzzle);
			getUserWord(userWord); // prompts user for the next word
		}

		freeWordList(userWordList);
		free(userWord);

		printPuzzleResult(currPuzzle);
		populateSolvedList(dictionaryList, maxWordLength, currPuzzle, bruteForce);
		printList(currPuzzle->solvedList, currPuzzle);
		populateMaxScore(currPuzzle);

		scoreCard finalMaxScore = createScoreCard();
		scoreCard finalUserScore = createScoreCard();
		calcEndGameScore(&finalUserScore, &finalMaxScore, puzzleArr, numPuzzles);

		printf("    FINAL Words Found: %d/%d\n", finalUserScore.numWords, finalMaxScore.numWords);
		printf("    FINAL ( * ) Pangrams Found: %d/%d\n", finalUserScore.pangrams,
			   finalMaxScore.pangrams);
		printf("    FINAL (***) Perfect Pangrams Found: %d/%d\n", finalUserScore.perfPangrams,
			   finalMaxScore.perfPangrams);
		printf("    FINAL Bingos Achieved: %d/%d\n", finalUserScore.bingos, finalMaxScore.bingos);
		printf("    FINAL Score: %d/%d\n", finalUserScore.score, finalMaxScore.score);
		createFreqTable(currPuzzle->solvedList, currPuzzle->hive);

		//---------------------------------------------------------------------
		//                 END OF OPEN-ENDED GAMEPLAY SECTION
		//---------------------------------------------------------------------
	}

	// Additional results are printed here:

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
	freeGame(&puzzleArr, numPuzzles);
	printf("\n\n");
	return 0;
}
