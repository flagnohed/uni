// You will edit and turn in this CPP file.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace with your own

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
// TODO: include any other header files you need

/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif


}


void createBoard(Boggle& boggle){
    std::string prompt = "Do you want to generate a random board? ";
    if(yesOrNo(prompt)){
        boggle.createRandomBoard();
    }else{
        std::string customInput;
        cout << "Enter custom input: "<<endl;
        cin >> customInput;
        boggle.createCustomBoard(customInput);
    }

}

void humanTurn(Boggle& boggle){
    while(true){
        std::cout << std::endl;
        std::cout << boggle.printBoard() << std::endl;
        std::cout << "Your words (" << boggle.getGuessedWords().size() << "): " << boggle.getGuessedWords().toString() << std::endl;
        std::cout << "Your score: " << boggle.getHumanPoints() << std::endl;

        bool wentThrough = false;
        cout << "Type a word (or press Enter to end your turn): ";
        std::string guess;
        getline(std::cin, guess);
        if (guess.empty()){
            break;
        }
        guess = toUpperCase(guess);
        if (boggle.checkWord(guess, boggle.getGuessedWords())){
            boggle.trackWords(boggle.getGrid(), guess, wentThrough);
            if (wentThrough){
                std::cout << "You found a new word! '" << guess << "'" << std::endl;
                boggle.addToGuessedWords(guess);
                clearConsole();
            }

        }




    }
    clearConsole();
}

void summarize(Boggle& boggle){

    std::cout << "My words(" << boggle.getComputerWords().size() << "): " << boggle.getComputerWords().toString() << endl;
    std::cout << "My score: " << boggle.getComputerPoints() << endl;

    if (boggle.getComputerPoints() > boggle.getHumanPoints()){
        std::cout << "Hahaha, I destroyed you. Better luck next time, puny human!" << endl;
    } else {
        std::cout << "fan du vann" << std::endl;
    }
}

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    // TODO: implement this function (and add any other functions you like to help you)
    createBoard(boggle);
    boggle.computerTurn();
    cout << "It's your turn" << endl;
    humanTurn(boggle);
    std::cout << "It's my turn!" << std::endl;

    summarize(boggle);
}


