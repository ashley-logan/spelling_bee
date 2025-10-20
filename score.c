#include "score.h"
#include "utils.h"

scoreCard createScoreCard() {
	scoreCard card = {0, 0, 0, 0, 0};
	return card;
}

int getScore(char *word, char *hive) {
	/* Valid words with MIN_WORD_LENGTH = 4 letters are worth 1 point each.
	Longer valid words earn 1 point per letter. (So a 5-letter word is worth 5
	points.) Each puzzle includes at least one “pangram” which uses every
	letter. These are worth hiveSize extra points! (Ex: if hiveSize = 7, then a
	pangram of length 9 will earn 9 + 7 = 16 points.)
	*/
	if (strlen(word) < MIN_WORD_LENGTH) {
		printf("ERROR: word too small line::421");
		return -1;
	}
	int pts = (strlen(word) == MIN_WORD_LENGTH) ? 1 : strlen(word);
	if (isPangram(word, hive)) {
		pts += strlen(hive);
	}
	return pts;
}

void update