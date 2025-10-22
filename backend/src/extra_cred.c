//---------------------------------------------------------------------
/* TODO Extra credit: Display frequency table
- Display a table showing the number of words starting with each letter and duration in a table.
- For example, with the hive abcde and required letter e:
		 Frequency Table:
		 let  4  5  6  7  8
		 -------------------
		  a:  3  1  1  1  0
		  b:  3  1  4  0  0
		  c:  3  1  0  0  1
		  d:  3  0  3  0  0
		  e:  0  1  0  0  0
   The table shows that there are 3 words that start with a and are 4 letters long,
   and 1 each that are 5, 6, and 7 letters long and start with a.
   Note that the lengths 4-8 are shown here because the longest word is 8 letters long,
   but that will change depending on the hive
*/
#include "extra_cred.h"

int getMaxLen(const WordList *list) {
	int maxLen = 0;
	if (list->numWords == 0) {
		return 0;
	}
	for (int i = 0; i < list->numWords; i++) {
		if (strlen(list->words[i]) > maxLen) {
			maxLen = strlen(list->words[i]);
		}
	}
	return maxLen;
}
void createFreqTable(const WordList *list, const char *hive) {
	int maxLen = getMaxLen(list); // get the length of the longest string in the list
	if (maxLen == 0) {
		printf("No valid words found\n");
		return;
	}
	int numRows = strlen(hive); // number of rows is equal to the length of the hive
	int numCols = (maxLen + 1) - MIN_WORD_LENGTH; // number of columns is the different between the
												  // maximum length and the minimum length plus 1
	int **freqTable = (int **)malloc(
		sizeof(int *) * numRows); // 2D array of integers (numRows arrays of integer pointers)
	for (int i = 0; i < numRows; i++) {
		freqTable[i] = (int *)calloc(numCols, sizeof(int)); // intializes all elements to 0
	}
	int colInd = -1;
	int rowInd = -1;
	for (int i = 0; i < list->numWords; i++) {
		colInd = strlen(list->words[i]) - MIN_WORD_LENGTH;
		rowInd = strchr(hive, (list->words[i])[0]) - hive;
		(freqTable[rowInd][colInd])++;
	}

	printf("let   ");
	for (int i = MIN_WORD_LENGTH; i <= maxLen; i++) {
		printf("%5d", i);
	}
	printf("\n");
	for (int i = MIN_WORD_LENGTH; i <= maxLen; i++) {
		printf("------");
	}
	printf("\n");
	for (int i = 0; i < numRows; i++) {
		printf("  %c:   ", hive[i]);
		for (int j = 0; j < numCols; j++) {
			printf("%5d", freqTable[i][j]);
		}
		printf("\n");
		free(freqTable[i]);
	}
	free(freqTable);
}