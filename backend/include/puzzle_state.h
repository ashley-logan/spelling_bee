#ifndef PUZZLE_STATE_H
#define PUZZLE_STATE_H
#include <stdbool.h>
#include "cJSON.h"


enum {
	MIN_HIVE_SIZE = 2, // smallest hive allowed
	MAX_HIVE_SIZE = 12, // largest hive allowed
	MIN_WORD_LENGTH = 4 // minimum word length to count as valid
};

typedef struct {
	int numWords;
	int score;
	int pangrams;
	int perfPangrams;
	int bingos;
} scoreCard;

typedef struct {
    char** words; // dynmaic array of strings
    int numWords; // number of strings
    int capacity; // maximum amount of strings that can be contained in the array
} WordList;

typedef struct {
	char hive[MAX_HIVE_SIZE + 1];
	char reqLet;
	scoreCard userScore;
	scoreCard maxScore;
	WordList *solvedList;
} Puzzle;

scoreCard createScoreCard();
WordList *createWordList();
Puzzle *createPuzzle();
void getUserWord(char *userWord);
void buildHive(char *str, char *hive);
void randHive(WordList *dictionaryList, Puzzle ***puzzleArr, int *numPuzzles, int hiveSize);
void customHive(WordList *dictionaryList, Puzzle ***puzzleArr, int *numPuzzles);
void setScore(scoreCard *scoreint, int newWords, int newScore, int newPangrams, int newPerfs, int newBingo);
int calcScore(const char *word, const char *hive);
void populateSolvedList(WordList *dictionaryList, int maxWordLength, Puzzle *puzzle, bool bruteForce);
void populateMaxScore(Puzzle *puzzle);
void updateUserScore(const char *word, Puzzle *puzzle);
void calcEndGameScore(scoreCard *finalUserScore, scoreCard *finalMaxScore, Puzzle **puzzleArr, int numPuzzles);
void appendWord(WordList *thisWordList, char *newWord);
int buildDictionary(char *filename, WordList *dictionaryList, int minLength);
WordList *findAllFitWords(WordList *dictionaryList, int hiveSize);
void bruteForceSolve(WordList *dictionaryList, Puzzle *puzzle);
int findWord(WordList *thisWordList, const char *aWord, int loInd, int hiInd, bool isRoot);
void findAllMatches(WordList *dictionaryList,
					WordList *solvedList,
					char *tryWord,
					Puzzle *puzzle,
					int maxLen,
					int *letTracker);
void freeWordList(WordList *list);
void freeGame(Puzzle ***puzzleArr, int numPuzzles);

void startGame();
int checkWord(const char *word);
char *getMessage(int result);
void getResponse(const char *word, cJSON *response);
void getHiveResponse(int hiveSize, cJSON *response);

#endif