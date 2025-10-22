#include "puzzle_state.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int getMaxLen(const WordList* list);
void createFreqTable(const WordList *list, const char *hive);