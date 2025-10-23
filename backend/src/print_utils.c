#include "print_utils.h"
#include "helpers.h"
#include "puzzle_state.h"
#include <stdio.h>
void printONorOFF(const bool mode) {
	if (mode) {
		printf("ON\n");
	} else {
		printf("OFF\n");
	}
}

void printYESorNO(const bool mode) {
	if (mode) {
		printf("YES\n");
	} else {
		printf("NO\n");
	}
}

void printHive(const Puzzle *puzzle) {
	int reqLetInd = strchr(puzzle->hive, puzzle->reqLet) - puzzle->hive;
	printf("  Hive: \"%s\"\n", puzzle->hive);
	printf("         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^");
	for (int i = reqLetInd + 1; i < strlen(puzzle->hive); i++) {
		printf(" ");
	}
	printf(" (all words must include \'%c\')\n\n", puzzle->hive[reqLetInd]);
}

char *displayHive(const Puzzle *puzzle) {
	int reqLetInd = strchr(puzzle->hive, puzzle->reqLet) - puzzle->hive;
	static char display[1000];
	snprintf(display, sizeof(display), "  Hive: \"%s\"\n", puzzle->hive);
	// printf("  Hive: \"%s\"\n", puzzle->hive);
	strcat(display, "         ");
	for (int i = 0; i < reqLetInd; i++) {
		strcat(display, " ");
	}
	strcat(display, "^");
	for (int i = reqLetInd + 1; i < strlen(puzzle->hive); i++) {
		strcat(display, " ");
	}
	snprintf(strlen(display) + display, sizeof(display), " (all words must include \'%c\')\n\n",
			 puzzle->hive[reqLetInd]);
	return display;
}

char *displayList(WordList *list, const Puzzle *puzzle) {
	static char userWords[1024];
	snprintf(userWords, sizeof(userWords), "  Word List:\n");
	for (int i = 0; i < list->numWords; i++) {
		if (isPangram(list->words[i], puzzle->hive)) {
			strlen(list->words[i]) == strlen(puzzle->hive) ? strcat(userWords, "***")
														   : strcat(userWords, "*");
		}
		int score = calcScore(list->words[i], puzzle->hive);
		snprintf(strlen(userWords) + userWords, sizeof(userWords), "(%d) %s\n", score,
				 list->words[i]);
	}
}

void printList(WordList *thisWordList, const Puzzle *puzzle) {
	// should not modify
	printf("  Word List:\n");
	for (int i = 0; i < thisWordList->numWords; i++) {
		if (isPangram(thisWordList->words[i], puzzle->hive)) {
			if (strlen(thisWordList->words[i]) == strlen(puzzle->hive)) {
				printf("**");
			}
			printf("*");
		}

		printf("  (%d) %s\n", calcScore(thisWordList->words[i], puzzle->hive),
			   thisWordList->words[i]);
	}
}

void printPuzzleResult(const Puzzle *puzzle) {
	int reqLetInd = strchr(puzzle->hive, puzzle->reqLet) - puzzle->hive;
	scoreCard userScore = puzzle->userScore;
	scoreCard maxScore = puzzle->maxScore;

	printf("==== SPELLING BEE SOLVER ====\n");
	int totScore = 0;
	printf("  Valid words from hive \"%s\":\n", puzzle->hive);
	printf("                         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^\n");
	printList(puzzle->solvedList, puzzle);

	printf("\n\n");
	printf("  Total counts for hive \"%s\":\n", puzzle->hive);
	printf("                         ");
	for (int i = 0; i < reqLetInd; i++) {
		printf(" ");
	}
	printf("^\n");
	printf("    Number of Valid Words Found: %d/%d\n", userScore.numWords, maxScore.numWords);
	printf("    Number of ( * ) Pangrams Found: %d/%d\n", userScore.pangrams, maxScore.pangrams);
	printf("    Number of (***) Perfect Pangrams: %d/%d\n", userScore.perfPangrams,
		   maxScore.perfPangrams);
	printf("    Bingo: ");
	printYESorNO((bool)userScore.bingos);
	printf("    Total Score: %d/%d\n", userScore.score, maxScore.score);
}
