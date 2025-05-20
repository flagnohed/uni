// This is the .cpp file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace it with your own

#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"
#include <algorithm>
#include "vector.h"



static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// TODO: implement the members you declared in Boggle.h
void Boggle::createRandomBoard(){

    shuffle(CUBES,NUM_CUBES);
    const int randHigh = 5;
    for(int j=0; j<4; j++){
        boggleGrid[j][0] = CUBES[4*j][randomInteger(0,randHigh)];
        boggleGrid[j][1] = CUBES[4*j+1][randomInteger(0,randHigh)];
        boggleGrid[j][2] = CUBES[4*j+2][randomInteger(0,randHigh)];
        boggleGrid[j][3] = CUBES[4*j+3][randomInteger(0,randHigh)];
    }
}
//returna sträng, printa i boggleplay
string Boggle::printBoard(){
    string res;
    for(int i = 0; i<BOARD_SIZE; i++){
        for(int j = 0; j<BOARD_SIZE; j++){
            res += boggleGrid[i][j];
        }
        res += '\n';
    }
    return res;
}

//lägg till i boggle
void Boggle::computerTurn(){
    Grid<bool> visitedGrid = Grid<bool>(BOARD_SIZE, BOARD_SIZE);
    for (int r = 0; r < BOARD_SIZE; r++){
        for (int c = 0; c < BOARD_SIZE; c++){
            visitedGrid[r][c] = false;
        }
    }

    std::string current = "";
    for (int r = 0; r < BOARD_SIZE; r++){
        for (int c = 0; c < BOARD_SIZE; c++){
            traverseHelp(boggleGrid, visitedGrid, r, c, current);
        }
    }

}

void Boggle::createCustomBoard(std::string input){
    if(checkValidString(input)){
        for(int j = 0; j<4; j++){
            boggleGrid[j][0] = input[4*j];
            boggleGrid[j][1] = input[4*j+1];
            boggleGrid[j][2] = input[4*j+2];
            boggleGrid[j][3] = input[4*j+3];
        }
    }

}

const bool Boggle::checkValidString(std::string input){

    if(input.length() != 16){
        return false;
    }
    for(char letter:input){
        char temp = std::toupper(letter);
        if(temp < 'A' || temp > 'Z'){
            return false;
        }
    }
    return true;
}

bool Boggle::checkWord(std::string guess, Vector<std::string> words){
    bool notFound = std::find(words.begin(), words.end(), guess) == words.end(); // if not in guessedwords
    if(guess.length() >= 4){
        if(english.contains(guess)){
            if(notFound) {
               return true;
            } else {
                std::cout << "you have already guessed that word." << std::endl;
            }
        } else {
            std::cout << "This is not a valid english word." << std::endl;
        }
    } else {
        std::cout << "Your guess needs to have at least 4 characters." << std::endl;

    }
    return false;
}

void Boggle::trackWordsHelp(const Grid<char>& boggleGrid, Grid<bool>& visitedGrid,
                            int r, int c, std::string current, const std::string& guess, bool& wentThrough){
    visitedGrid[r][c] = true;
    current += boggleGrid[r][c];
    if (current == guess){
        if (std::find(guessedWords.begin(), guessedWords.end(), guess) == guessedWords.end()){

            wentThrough = true;
        }

    }
    //kolla neighbors
    for(int row = r-1; row <= r+1 && row < BOARD_SIZE; row++){
        for (int col = c-1; col <= c+1 && col < BOARD_SIZE; col++){
            if (row >= 0 && col >= 0 && !visitedGrid[row][col]){ // så att row och col inte är negativt och att vi inte har varit där än
                int index = current.length();
                if (boggleGrid[row][col] == guess[index]){
                    trackWordsHelp(boggleGrid, visitedGrid, row, col, current, guess, wentThrough);
                }

            }
        }
    }
    current.pop_back();
    visitedGrid[r][c] = false; // fuck go back
}

void Boggle::trackWords(const Grid<char>& boggleGrid, const std::string guess, bool& wentThrough){
    Grid<bool> visitedGrid = Grid<bool>(BOARD_SIZE, BOARD_SIZE);
    for (int r = 0; r < BOARD_SIZE; r++){
        for (int c = 0; c < BOARD_SIZE; c++){
            visitedGrid[r][c] = false;
        }
    }

    std::string current = "";
    for (int r = 0; r < BOARD_SIZE; r++){
        for (int c = 0; c < BOARD_SIZE; c++){
            if (boggleGrid[r][c] == guess[0]){
                trackWordsHelp(boggleGrid, visitedGrid, r, c, current, guess, wentThrough);
            }
        }
    }
}

void Boggle::traverseHelp(const Grid<char>& boggleGrid, Grid<bool>& visitedGrid, int r, int c, std::string current){
    visitedGrid[r][c] = true;
    current += boggleGrid[r][c];
    if (!english.containsPrefix(current)){
        visitedGrid[r][c] = false;
        return;
    }

    bool notFound = std::find(computerWords.begin(), computerWords.end(), current) == computerWords.end();
    if (current.length() >= 4 && english.contains(current) && notFound){
        addToComputerWords(current);
    }
    for(int row = r-1; row <= r+1 && row < BOARD_SIZE; row++){
        for (int col = c-1; col <= c+1 && col < BOARD_SIZE; col++){
            if (row >= 0 && col >= 0 && !visitedGrid[row][col]){ // så att row och col inte är negativt och att vi inte har varit där än
                traverseHelp(boggleGrid, visitedGrid, row, col, current);
            }
        }
    }
    visitedGrid[r][c] = false; // fuck go back

}

Vector<string> Boggle::getGuessedWords(){
    return guessedWords;
}

int Boggle::getHumanPoints(){
    return humanPoints;
}

int Boggle::getComputerPoints(){
    return computerPoints;
}

void Boggle::addToGuessedWords(std::string guess){
    guessedWords.push_back(guess);
    humanPoints += guess.length() - 3;

    removeWord(guess);    //ta bort från datorns ord
    computerPoints -= guess.length() - 3;
}

Grid<char> Boggle::getGrid(){
    return boggleGrid;
}

void Boggle::removeWord(std::string input){
    int index = std::distance(computerWords.begin(), std::find(computerWords.begin(), computerWords.end(), input));
    computerWords.remove(index);


}

void Boggle::addToComputerWords(std::string guess){
    computerWords.push_back(guess);
    computerPoints += guess.length() - 3;
}
Vector<string> Boggle::getComputerWords(){
    return computerWords;
}

void Boggle::refreshBoggle(){
    computerPoints = 0;
    computerWords.clear();
    humanPoints = 0;
    guessedWords.clear();
}

