#ifndef "SCORE_H"
#define "SCORE_H"

typedef struct {
	int numWords;
	int score;
	int pangrams;
	int perfPangrams;
	int bingos;
} scoreCard;

scoreCard createScoreCard();

#endif