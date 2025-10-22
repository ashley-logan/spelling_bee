#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H

#include "puzzle_state.h"
#include <stdbool.h>

void printONorOFF(const bool mode);
void printYESorNO(const bool mode);
void printHive(const Puzzle *puzzle);
void printList(WordList *thisWordList, const Puzzle *puzzle);
void printPuzzleResult(const Puzzle *puzzle);

#endif