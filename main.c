#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

const int MIN_HIVE_SIZE = 2; // smallest amount of words allowed
const int MAX_HIVE_SIZE = 12; // largest amount of words allowed
const int MIN_WORD_LENGTH = 4; // everyword must be at least 4 letters

typedef struct WordList_struct {
    char** words; // dynmaic array of strings
    int numWords; // number of strings
    int capacity; // maximum amount of strings that can be contained in the array
} WordList;

WordList* createWordList() {
    WordList* newList = malloc(sizeof(WordList)); // allocates memory for a new word list
    newList->capacity = 4; // sets the capacity to 4 words
    newList->numWords = 0; // sets the current number of words to 0
    newList->words = malloc(newList->capacity * sizeof(char*)); // allocates memory for the words array with enough mem to store 4 character pointers

    return newList; // returns the newly creates word list
}

void appendWord(WordList* thisWordList, char* newWord) {
    if(thisWordList->numWords == thisWordList->capacity) {
        char** largerList = realloc(thisWordList->words, (thisWordList->capacity * 2) * sizeof(char*));
        if (largerList != NULL) {
            thisWordList->words = largerList;
            thisWordList->capacity *= 2;
        }
    }
    thisWordList->words[thisWordList->numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char)); // allocate new memory for another word in the word list at the last position
    strcpy(thisWordList->words[thisWordList->numWords], newWord); // copy input word into the new string allocated for the word list
    (thisWordList->numWords)++;
    //---------------------------------------------------------------------
    /* TODO (Task 1-A): Write appendWord
    - adds newWord to the end of the thisWordList (malloc'ing memory and copying the string; do NOT just copy the input pointer)
    - increments thisWordList's numWords. If there is no space for the new word, reallocate with double the capacity
    - by malloc'ing new memory for the words array and freeing the old memory
    */

}

int buildDictionary(char* filename, WordList* dictionaryList, int minLength) {
    FILE* dictFile = fopen(filename, "r");
    if (dictFile == NULL) { // if file doesn't open return -1
        return -1;
    }
    char tempStr[50];
    int maxLen = 0; // initialize maxLen to default of -1, will contain the length of the longest word

    while(fscanf(dictFile, "%s", tempStr) == 1) { // while not at the end of file and dictionaryList is not full
        if((int)strlen(tempStr) >= minLength) { // add word to list if length is at least the minimum parameter
            maxLen = (strlen(tempStr) > maxLen)? strlen(tempStr) : maxLen;
            appendWord(dictionaryList, tempStr);
        }
    }
    return maxLen;
    
    //---------------------------------------------------------------------
    /* TODO (Task 1-B): Write buildDictionary
    - Opens a valid file and adds all words that are at least minLength long to the dictionaryList
    - Returns the length of the longest valid word if all goes well, -1 otherwise
    */
}

void freeWordList(WordList* list) {
    for(int i = 0; i < list->numWords; i++) {
        free(list->words[i]);
    }
    free(list->words);
    free(list);
    //---------------------------------------------------------------------
    /* TODO (Task 1-C): Write freeWordList
    - Frees the memory used by the WordList, both the words' char* arrays and the char** words array itself
    */
}

int findLetter(char* str, char aLet) {
    char* ptr = strchr(str, aLet);
    if(ptr == NULL) {
        return -1;
    }

    return ptr - str;
    //---------------------------------------------------------------------
    /* TODO (Task 2-A): Write findLetter
    - Returns the index of aLet in str, if it exists
    - Otherwise returns -1
    */
}

void buildHive(char* str, char* hive) {
    char* travHive = hive;
    for(char c = 'a'; c <= 'z'; c++) { // loop through every lowercase letter in the alphabet
        if(findLetter((str), c) != -1 || findLetter(str, toupper(c)) != -1) {
            // if the word has the current letter (lowercase or uppercase) append it to hive
            *travHive = c; 
            travHive++; // increment hive
        }
    }
    *travHive = '\0'; // null terminate hive
    //---------------------------------------------------------------------
    /* TODO (Task 2-B): Write buildHive
    - take each character of the string and add it to the hive in alphabetical order
    - no letter should be added twice
    - ensure that hive is null-terminated at the end
    (findLetter is helpful here)
    */
}

int countUniqueLetters(char* str) {
    int count = 0;
    for(int i = 0; i < 256; i++) {
        if(findLetter(str, (char)i) != -1) {
            count++;
        }
    }
    return count;
    //---------------------------------------------------------------------
    /* TODO (Task 3-A): Write countUniqueLetters
    - Counts and returns the number of unique letters in the input string 
    (note: this only needs to work for lower-case alpha characters, i.e. letters, for this project, 
            but it should work generally for all characters)
    */
}

WordList* findAllFitWords(WordList* dictionaryList, int hiveSize) {
    WordList* fitWords = createWordList();
    for(int i = 0; i < dictionaryList->numWords; i++) {
        if(countUniqueLetters(dictionaryList->words[i]) == hiveSize) {
            appendWord(fitWords, dictionaryList->words[i]);
        }
    }
    return fitWords;
    //---------------------------------------------------------------------
    /* TODO (Task 3-B): Write findAllFitWords
    - Creates a WordList and adds all fit words from dictionaryList to the new WordList 
      (A fit word has exactly hiveSize unique letters)
    */
}

bool strLower(char* word) {
    for(int i = 0; word[i] != '\0'; i++) {
        if(!islower(word[i])) {
            return false;
        }
    }
    return true;
}

bool noDups(char* word) {
    char* travWord = word;
    while(*travWord != '\0') {
        if(strchr(word, *travWord) != travWord) { 
            // if strchr returns a pointer to a location that is not the current pointer, then another instance of the letter occurs
            return false;
        }
        travWord++; // increment the pointer
    }
    return true; // otherwise there are no duplicates
}

bool isValidWord(char* word, char* hive, char reqLet) {
    if(strchr(word, reqLet) == NULL) {
        // if the word doesn't use the required letter return false
        return false;
    }

    for(int i = 0; word[i] != '\0'; i++) {
        // if the word contains a letter that isn't in the hive return false
        if(strchr(hive, word[i]) == NULL) {
            return false;
        }
    }
    //---------------------------------------------------------------------
    /* TODO (Task 4-A, part 1 or 2): Write isValidWord 
    - Returns true if word 
      (a) uses ONLY letters that are in the hive, and 
      (b) MUST use the required letter. 
    - Returns false otherwise
    */
    return true; // otherwise return true
}

bool isPangram(char* str, char* hive) {
    return countUniqueLetters(str) == countUniqueLetters(hive);
    //---------------------------------------------------------------------
    /* TODO (Task 4-B): Write isPangram
    - Returns true if str is a pangram (uses all the letters in hive at least once)
    - Returns false otherwise
    */

}


void printHive(char* hive, int reqLetInd) {
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

void printList(WordList* thisWordList, char* hive) {
    for(int i = 0; i < thisWordList->numWords; i++) {
        if(isPangram(thisWordList->words[i], hive)) {
            (strlen(thisWordList->words[i]) == strlen(hive))? printf("***") : printf("*");
        }
        printf("(%d) %s", getScore(thisWordList->words[i], hive), thisWordList->words[i]);
    }
    //---------------------------------------------------------------------
    /* TODO (Task 4-C): Write printList
    - Outputs the words in the list 
    - Marks pangrams with asterisk and perfect pangrams with three asterisks
    - Displays score for each word as well as total score at the bottom
    See example shown below.
    Word List:
    *** (16) computer
        ( 7) compute
        ( 1) mute
     *  (17) recompute
        ( 5) comet
    Total Score: 46    
    */

    printf("  Word List:\n");
    int totScore = 0;
    printf("  Total Score: %d\n", totScore);

}

void bruteForceSolve(WordList* dictionaryList, WordList* solvedList, char* hive, char reqLet) {
    for(int i = 0; i < dictionaryList->numWords; i++) {
        if(isValidWord(dictionaryList->words[i], hive, reqLet)) {
            appendWord(solvedList, dictionaryList->words[i]);
        }
    }
    //---------------------------------------------------------------------
    /* TODO (Task 5-A): Write bruteForceSolve
    - Goes through each word in the dictionary and determines if that is a solution to the puzzle
    - Adds the word to solvedList if it is a solution
    */
}

bool isPrefix(char* partWord, char* fullWord) {
    if(strlen(partWord) >= strlen(fullWord)) {
        // if the prefix is equal or longer than the full word it canno be a prefix; return false
        return false;
    }
    return strncmp(partWord, fullWord, strlen(partWord)) == 0;
    //---------------------------------------------------------------------
    /* TODO (Task 6-A): Write isPrefix
    - checks if partWord is the root/prefix of fullWord, such that 
        partWord is a perfect match to the beginning (arbitrary length) of fullWord
    - returns true is partWord is a root/prefix of fullWord; returns false otherwise 
    */
}

int findWord(WordList* thisWordList, char* aWord, int loInd, int hiInd, bool isRoot) {
    /* TODO (Task 6-B): Complete findWord
    * - Fix all the if statements that say 'if(true)' with the correct binary search cases
    */
    if (hiInd < loInd) { // Base case 2: aWord not found in words[]

        if ((loInd < thisWordList->numWords) && isRoot) {
            return -1; //words match this root (partial match)
        }
        else {
            return -99; //no more words matching this root (no match)
        }
    }

    int mdInd = (hiInd + loInd) / 2;

    if (strcmp(aWord, thisWordList->words[mdInd]) == 0) { // Base case 1: found tryWord at midInd
        return mdInd;
    }

    if(isPrefix(aWord, thisWordList->words[mdInd])) {
        // if the word is a prefix of a word in the word list, set the flag to true
        isRoot = true;
    }

    if (strcmp(aWord, thisWordList->words[mdInd]) > 0) { // Recursive case: search upper half
        return findWord(thisWordList, aWord, mdInd + 1, hiInd, isRoot);
    }

    // Recursive case: search lower half
    return findWord(thisWordList, aWord, loInd, mdInd - 1, isRoot);
}

void findAllMatches(WordList* dictionaryList, WordList* solvedList, char* tryWord, char* hive, char reqLet, int maxLen, int* letTracker) {

    int curLen = strlen(tryWord);
    if(curLen > maxLen) {
        return;
    }

    int index = findWord(dictionaryList, tryWord, 0, dictionaryList->numWords - 1, false);

    if (index >= 0 && isValidWord(tryWord, hive, reqLet)) {
        // tryWord found in dicionary and is valid
        if(findWord(solvedList, tryWord, 0, solvedList->numWords-1, false) < 0) {
            // word is not already in solved list
            appendWord(solvedList, tryWord);
        }
    }

    if(index == -99) {
        // no match and no prefix, do not append
        return;
    }

    for(int i = 0; hive[i] != '\0'; i++) {
        if(letTracker[i] == 0) {
            letTracker[i] = 1;
            tryWord[curLen] = hive[i];
            tryWord[curLen+1] = '\0';

            findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet, maxLen, letTracker);

            tryWord[curLen] = '\0';
            letTracker[i] = 0;
        }
    }


}

// Provided for you, to determine the program settings based on parameters
bool setSettings(int argc, char* argv[], bool* pRandMode, int* pNumLets, char dictFile[100], bool* pPlayMode, bool* pBruteForceMode, bool* pSeedSelection) {
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
        }
        else if (strcmp(argv[i], "-d") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            strcpy(dictFile, argv[i]);
            FILE* filePtr = fopen(dictFile, "r");
            if (filePtr == NULL) {
                return false;
            }
            fclose(filePtr);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            ++i;
            if (argc == i) {
                return false;
            }
            *pSeedSelection = true;
            int seed = atoi(argv[i]);
            srand(seed);
        }
        else if (strcmp(argv[i], "-p") == 0) {
            *pPlayMode = true;
        }
        else if (strcmp(argv[i], "-o") == 0) {
            *pBruteForceMode = false;
        }
        else {
            return false;
        }
    }
    return true;
}

void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    }
    else {
        printf("OFF\n");
    }
}

void printYESorNO(bool mode) {
    if (mode) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
}

int getScore(char* word, char* hive) {
    /* Valid words with MIN_WORD_LENGTH = 4 letters are worth 1 point each.
    Longer valid words earn 1 point per letter. (So a 5-letter word is worth 5 points.)
    Each puzzle includes at least one “pangram” which uses every letter. 
    These are worth hiveSize extra points! (Ex: if hiveSize = 7, 
        then a pangram of length 9 will earn 9 + 7 = 16 points.)
    */
    if(strlen(word) < MIN_WORD_LENGTH) {
        printf("ERROR: word too small line::421");
        return -1;
    }
    int pts = (strlen(word) == MIN_WORD_LENGTH) ? 1 : strlen(word);
    if(isPangram(word)) {
        pts += strlen(hive);
    }
    return pts;
}


int main(int argc, char* argv[]) {

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


    if (!setSettings(argc, argv, &randMode, &hiveSize, dict, &playMode, &bruteForce, &seedSelection)) {
        printf("Invalid command-line argument(s).\nTerminating program...\n");
        return 1;
    }
    else {
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

    // build word array (only words with desired minimum length or longer) from dictionary file
    printf("Building array of words from dictionary... \n");
    WordList* dictionaryList = createWordList();
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
        printf("  Dictionary %s contains insufficient words of length %d or more...\n", dict, MIN_WORD_LENGTH);
        printf("Terminating program...\n");
        return 0;
    }
    else {
        printf("  Dictionary %s contains \n  %d words of length %d or more;\n", dict, dictionaryList->numWords, MIN_WORD_LENGTH);
    }




    if (randMode) {
        printf("==== SET HIVE: RANDOM MODE ====\n");
        //find number of words in words array that use hiveSize unique letters
        WordList* fitWords = findAllFitWords(dictionaryList, hiveSize);
        int numFitWords = fitWords->numWords;
        //pick one at random
        int pickOne = rand() % numFitWords;
        char* chosenFitWord = fitWords->words[pickOne];

        //and alaphabetize the unique letters to make the letter hive
        buildHive(chosenFitWord, hive);
        freeWordList(fitWords);

        reqLetInd = rand() % hiveSize;
        reqLet = hive[reqLetInd];

    }
    else {
        bool okInput = false;
        while(!okInput) {
            printf("==== SET HIVE: USER MODE ====\n");
            printf("  Enter a single string of lower-case,\n  unique letters for the letter hive... ");
            scanf("%s", hive);
            if(strlen(hive) < MIN_HIVE_SIZE || strlen(hive) > MAX_HIVE_SIZE) {
                printf("  HIVE ERROR: \"%s\" has invalid length;\n  valid hive size is between %d and %d, inclusive\n\n",hive, MIN_HIVE_SIZE, MAX_HIVE_SIZE);
            }
            else if(!strLower(hive)) {
                printf("  HIVE ERROR: \"%s\" contains invalid letters;\n  valid characters are lower-case alpha only\n\n",hive);
            }
            else if(!noDups(hive)) {
                printf("  HIVE ERROR: \"%s\" contains duplicate letters\n\n",hive);
            }
            else {
                okInput = true;
            }
        }

        okInput = false;
        while(!okInput) {
            hiveSize = strlen(hive);
            reqLetInd = -1;
            reqLet = '\0';
            printf("  Enter the letter from \"%s\"\n  that is required for all words: ", hive);
            scanf(" %c", &reqLet);
            reqLet = tolower(reqLet);
            if(reqLet == '\0') {
                continue;
            }
            if(strchr(hive, reqLet) == NULL) {
                printf("  HIVE ERROR: \"%s\" does not contain the letter \'%c\'\n\n",hive,reqLet);
            }
            else {
                okInput = true;
                reqLetInd = strchr(hive, reqLet) - hive;
            }
        }
        

        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 1 of 2)
         - Write code for randMode OFF, where user enters the hive; use the print statements below in the exact order you see them
         - ask the user to enter a string, following these requirements:
             - must be lowercase letters
             - string length must not be below MIN_HIVE_SIZE or above MAX_HIVE_SIZE
             - must not have any duplicate letters (you might find a helper function useful for this part)
         - If a word is invalid, display the appropriate error below and repeat until there is valid input
         - You may use buildHive() to help with this task
         (Task 2-C is continued a few lines below, getting the required letter)
        */


        

        
        //---------------------------------------------------------------------
        /* TODO (Task 2-C): Get hive input (part 2 of 2)
        - Ask the user to enter the required letter, using the print statement above
        - If the letter is not in the hive, display the error message below and repeat the prompt until input is valid
        - If the letter the user enters is within the hive, then find the index of that letter in the hive,
        set that value you found for reqLetInd, and change reqLet to the required letter
        - Note that uppercase letters ARE valid here, unlike earlier; you will find it helpful to make them lowercase in code
        */
    }

    printHive(hive, reqLetInd);

    if (playMode) {
        printf("==== PLAY MODE ====\n");
    //---------------------------------------------------------------------
    //              BEGINNING OF OPEN-ENDED GAMEPLAY SECTION
    //---------------------------------------------------------------------
    
            /* TODO (Task 4, OPEN-ENDED GAMEPLAY): develop a creative gameplay extension.
                The code is currently a framework for repeatedly taking in words from the user, 
                until they enter DONE. Extend/replace/modify this section to implement
                your creative gameplay extension.

            */

        char* userWord = (char*)malloc((maxWordLength + 1) * sizeof(char));
        strcpy(userWord, "default");

        WordList* userWordList = createWordList();

        printf("............................................\n");
        printHive(hive, reqLetInd);


        printf("  Enter a word (enter DONE to quit): ");
        scanf("%s", userWord);
        printf("\n");


        while (strcmp(userWord, "DONE") != 0) {
            
            
            //prints the list and the hive, and gets the next input
            printf("\n");
            printList(userWordList, hive);
            printf("............................................\n");
            printHive(hive, reqLetInd);

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

    WordList* solvedList = createWordList();

    if (bruteForce) { //find all words that work... (1) brute force
        bruteForceSolve(dictionaryList, solvedList, hive, reqLet);
    }
    else {
        char* tryWord = (char*)malloc(sizeof(char) * (maxWordLength + 1));

        tryWord[0] = hive[0];
        tryWord[1] = '\0';
        int* letTracker = calloc(strlen(hive), sizeof(int)); // tracks the index of the letter (in the hive) being appended or replaced
        findAllMatches(dictionaryList, solvedList, tryWord, hive, reqLet, maxWordLength, letTracker);
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
    int* bingoTrack = calloc(strlen(hive), sizeof(int));

    int numValidWords = solvedList->numWords;
    for(int i = 0; i < numValidWords; i++) {

        if(isPangram(solvedList->words[i])) {
            numPangrams++;
            if(strlen(solvedList->words[i]) == strlen(hive)) {
                numPerfectPangrams++;
            }
        }
        totScore += getScore(solvedList->words[i], hive);
        char* hiveInd = strchr(hive, (solvedList->words[i])[0]);
        bingoTrack[hiveInd - hive]++;
    }

    for(int i = 0; i < strlen(hive); i++) {
        if(bingoTrack[i] < 1) {
            isBingo = false;
        }
    }
    free(bingoTrack);


    //---------------------------------------------------------------------
    /* TODO (Task 5-B): Display solver results
    - Display list of words solver found, noting pangrams (*) and perfect pangrams (***) with asterisks
    - Calculate the number of valid words, number of pangrams, number of perfect pangrams, 
          total possible score, and whether there is a bingo. Save those values in the corresponding
          variables, or update the printf statements below to dispay those values
    */
    printList(solvedList, hive);


    
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

    freeWordList(dictionaryList);
    freeWordList(solvedList);
    printf("\n\n");
    return 0;
}

