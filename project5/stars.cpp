#include <climits>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "utilities.h"

constexpr size_t MAXWORDS = 10000;
constexpr size_t MAXINPUT = 110;
constexpr char WORDFILENAME[] = "words.txt";

// defining min/max macros to avoid using std::algorithm for safety's sake
#define MIN(i, j) ((i < j) ? (i) : (j))
#define MAX(i, j) ((i > j) ? (i) : (j))

// check if c-string is all lower case
bool allLowerCase(const char str[]) {
  for (size_t i = 0; str[i] != '\0'; ++i) {
    if (!islower(str[i])) {
      return false;
    }
  }
  return true;
}

// check if c-string is in array f c-strings
bool inStrArray(const char strs[][MAXWORDLEN + 1], size_t len, char str[]) {
  for (size_t i = 0; i < len; ++i) {
    if (std::strcmp(strs[i], str) == 0) {
      return true;
    }
  }
  return false;
}

int runOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum) {
  if (wordnum < 0 || wordnum >= nWords || nWords <= 0)
    return -1;
  auto word = words[wordnum];

  char wordBuffer[MAXINPUT + 1] = {};

  for (int tries = 1;; ++tries) {
    // structured goto, basically
    while (true) {
      std::cout << "Trial word: ";
      // check for EOF.
      if (!std::cin.getline(wordBuffer, MAXINPUT)) {
        // if the game isn't complete, there's now no way to complete it, so
        // quit
        std::cout << std::endl;
        std::exit(1);
      }
      size_t wordBufferLen = strlen(wordBuffer);
      if (wordBufferLen < 4 || wordBufferLen > 6 || !allLowerCase(wordBuffer)) {
        std::cout
            << "Your trial word must be a word of 4 to 6 lower case letters."
            << std::endl;
      } else if (!inStrArray(words, nWords, wordBuffer)) {
        std::cout << "I don't know that word." << std::endl;
      } else {
        break;
      }
    };

    if (strcmp(word, wordBuffer) == 0) {
      return tries;
    }

    int starCount = 0;
    int planetCount = 0;

    char SAPWord[MAXWORDLEN + 1];
    // we modify SAPWord to indicate used stars & planets
    strcpy(SAPWord, word);
    char *SAP_planetLoc;

    // using '1' as a sentinel for used values
    // finding stars
    for (size_t i = 0; word[i] != '\0' && wordBuffer[i] != '\0'; ++i) {
      if (word[i] == wordBuffer[i]) {
        ++starCount;
        SAPWord[i] = '1';
        wordBuffer[i] = '1';
      }
    }
    // finding planets
    for (size_t i = 0; word[i] != '\0' && wordBuffer[i] != '\0'; ++i) {
      // if strchr finds the character in the current word,
      // assign the pointer to it to SAP_planetLoc and mark it as a used letter
      if (wordBuffer[i] != '1' &&
          (SAP_planetLoc = strchr(SAPWord, wordBuffer[i])) != nullptr) {
        ++planetCount;
        *SAP_planetLoc = '1';
        wordBuffer[i] = '1';
      }
    }
    std::cout << "Stars: " << starCount << ", Planets: " << planetCount
              << std::endl;
  }
}

int main() {
  char wordList[MAXWORDS][MAXWORDLEN + 1];
  const int nWords = getWords(wordList, MAXWORDS, WORDFILENAME);
  if (nWords <= 0) {
    std::cout << "No words were loaded, so I can't play the game." << std::endl;
    return 1;
  }

  int numRounds;
  int totalTries = 0;
  int minTries = INT_MAX;
  int maxTries = 0;

  std::cout << std::fixed << std::showpoint << std::setprecision(2);

  std::cout << "How many rounds do you want to play? ";
  std::cin >> numRounds;
  std::cin.ignore(10000, '\n');

  if (numRounds <= 0) {
    std::cout << "The number of rounds must be positive." << std::endl;
    return 1;
  }

  for (int r = 1; r <= numRounds; ++r) {
    std::cout << "\nRound " << r << std::endl;
    auto wordInd = randInt(0, nWords - 1);
    auto word = wordList[wordInd];
    auto wordLen = strlen(word);
    std::cout << "The secret word is " << wordLen << " letters long."
              << std::endl;

    int tries = runOneRound(wordList, nWords, wordInd);
    // check if constants are wrong
    if (tries == -1) {
      return 1;
    }
    totalTries += tries;

    std::cout << "You got it in " << tries << " ";
    if (tries == 1) {
      std::cout << "try." << std::endl;
    } else {
      std::cout << "tries." << std::endl;
    }
    minTries = MIN(tries, minTries);
    maxTries = MAX(tries, maxTries);
    double average = (double)totalTries / r;
    std::cout << "Average: " << average << ", minimum: " << minTries
              << ", maximum: " << maxTries << std::endl;
  }

  return 0;
}
