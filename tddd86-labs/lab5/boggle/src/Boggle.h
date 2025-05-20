// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header and replace it with your own

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "grid.h"
#include "lexicon.h"
// TODO: include any other header files you need

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;
    // TODO: decide the public member functions and declare them

    /*
     * Creates a random Boggle board
     */
    void createRandomBoard();

    /*
     * Prints the board in the game
     */
    string printBoard();

    /*
     * Creates a board with the user's custom input
     */
    void createCustomBoard(string input);

    /*
     * Checks that a guess has the right length, is a proper word and hasn't been guessed before.
     */
    bool checkWord(string guess, Vector<string> words);

    /*
     * Checks that the input string for the custom board is valid
     */
    const bool checkValidString(string input);

    /*
     * Function that calls trackWordsHelp in every iteration where guess' first letter exists in the board
     */
    void trackWords(const Grid<char>& boggleGrid, const string guess, bool& wentThrough);

    /*
     * Adds a valid guess to user's guessed words, adds points to user and removes guess from computers words.
     */
    void addToGuessedWords(std::string guess);

    /*
     * Removes a word from computerWords
     */
    void removeWord(std::string input);

    /*
     * Help function for computersTurn that traverses all possible paths
     */
    void traverseHelp(const Grid<char>& boggleGrid, Grid<bool>& visitedGrid, int r, int c, std::string current);

    /*
     * Adds a valid word in the boggleGrid and adds points to the computer
     */
    void addToComputerWords(std::string guess);

    /*
     * Sets all point counters to 0 and clears the vectors
     */
    void refreshBoggle();

    /*
     * Helper function for trackWords that checks that guess is in the board
     */
    void trackWordsHelp(const Grid<char>& boggleGrid, Grid<bool>& visitedGrid,
                                int r, int c, string current, const string& guess, bool& wentThrough);

    /*
     * Handles the word search for the computer
     */
    void computerTurn();
    //GETTERS
    Vector<string> getGuessedWords();   //getter for guessedWords
    int getHumanPoints();               //getter for humanPoints
    int getComputerPoints();            //getter for computerPoints
    Grid<char> getGrid();               //getter for boggleGrid
    Vector<string> getComputerWords();  //getter for computerWords



private:

    Grid<char> boggleGrid = Grid<char>(BOARD_SIZE,BOARD_SIZE);  //gameboard
    const Lexicon english = Lexicon(DICTIONARY_FILE);           //english dictionary
    int humanPoints = 0;
    int computerPoints = 0;
    Vector<string> guessedWords = Vector<string>();             //user's guessed words
    Vector<string> computerWords = Vector<string>();

};

#endif
