#include <iostream>
#include <string>
#include <set>
#include <forward_list>
#include <fstream>
#include <map>
#include <list>
#include <algorithm>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Reads in the dictionary and add the words into a set
 */

void getDict(set<string>& wordList){
    fstream ifs;
    ifs.open("dictionary.txt");
    string line;
    while (!ifs.eof()){
        getline(ifs, line);
        wordList.insert(line);
    }
}

/*
 * Asks the user for the desired length of the guessing word.
 */
int askWordLength(){
    cout << "Choose length of the secret word: ";
    int wordLen;
    cin >> wordLen;
    return wordLen;
}

/*
 * Insert all the words with the length of the users input,
 * if not words match the length the users will be askes to insert a new length
 */
void getValidWords(const set<string>& dict, const unsigned int& wordLength, set<string>& validWords){
    //set<string> validWords;
    for (string word : dict){
        if (word.length() == wordLength){
            validWords.insert(word);
        }
    }
    if (validWords.size() == 0){
        int newWordLength = askWordLength();
        getValidWords(dict, newWordLength, validWords);
    }
}

/*
 * Asks the user for the desired amount of guesses
 */
int askNumberOfGuesses(){
    cout << "Please enter the amount of guesses you want: ";
    int guesses;
    cin >> guesses;
    return guesses;
}

/*
 * Asks the user if they want to display the remaing words
 */
bool wantsVisibleWords(){
    cout << "Do you want the remaining words in wordlist to be visible? (y/n): ";
    char answer;
    cin >> answer;
    if (answer == 'y'){
        return true;
    }
    if (answer == 'n'){
        return false;
    }
    else {
        cout << "Didn't understand that." << endl;
        wantsVisibleWords();
    }

    return false;
}

/*
 * All visual information prints for the user
 */
void printGameInfo(const int& remainingGuesses, const string& secretWord, const set<char>& guessedChars,
                   const set<string>& currentWordList,
                   const bool& wantsVisible){

    cout << "----------" << endl;

    if (wantsVisible){
        for (string word : currentWordList){
            cout << word << endl;
        }
    }
    cout << "Remaining guesses: " << remainingGuesses << endl;
    cout << "Guessed characters: ";
    for (char character : guessedChars){
        cout << character << ", ";
    }
    cout << endl;

    cout << currentWordList.size() << endl;

    cout << secretWord << endl;

}

/*
 * Generates wordfamilies in the format of map<string ,set<string>>.
 * Where the key is all the positions that the guessed charactar could be in and the set contains all the words that fits the key.
 * If a new key is found then it will be generated and inserted into the set else the words will be inserted into the set of that specific key.
 */
void makeWordFamilies(const char& guess, const set<string>& currentWordList, map<string,set<string>>& currentMap){
    for (string word: currentWordList){
        string tempWord = word;
        for(unsigned int i = 0; i < word.length(); i++){
            if(word[i] != guess){
                tempWord[i] = '-';
            }

        }



        if(currentMap.find(tempWord) != currentMap.end()){
            currentMap[tempWord].insert(word);
        }else{
            set<string> possibleWords;
            possibleWords.insert(word);
            currentMap[tempWord] = possibleWords;
        }
    }
}

/*
 * Checks if the guessed character is in the alphabet
 */
bool isInAlphabet(const char& letter){
    unsigned int found = ALPHABET.find(letter);
    if (found < ALPHABET.length()){
        return true;
    }
    return false;
}

/*
 * Asks the user for a guess and then checks if the char is in the alpha, not allready guessed and if the guess could possibly be in the final word
 */
char askForGuess(set<char>& guessedChars, set<string>& currentWordList, const string& secretWord){
    cout << "Guess a character: ";
    char guess;
    cin >> guess;
    set<char>::iterator it;
    it = find(begin(guessedChars), end(guessedChars),guess);

    if (!isInAlphabet(guess) || it != guessedChars.end() || secretWord.find(guess) != -1){
        cout << "False input, try again please." << endl;
        askForGuess(guessedChars, currentWordList, secretWord);
    }
    return guess;
}






/*
 * Updates the current wordlist with the largest wordfamily
 */
void updateWordlist(map<string,set<string>>& families, set<string>& newWordlist){
    unsigned int size = 0;
    for(auto& fam: families){
        set<string> firstClass = fam.second;
        if(firstClass.size() > size){
            size = firstClass.size();
            newWordlist = firstClass;
        }
    }
}

/*
 * Asks the user if they want to play again
 */
void askToPlayAgain(bool& play){
    cout << "Do you want to play again? (y/n): ";
    char answer;
    cin >> answer;
    if (answer == 'y'){
        cout << "Restarting game..." << endl;
    } else if (answer == 'n'){
        play = false;
    } else {
        cout << "Sorry, didn't understand that." << endl;
        askToPlayAgain(play);
    }

}

/*
 * Builds the worst "right" word containing only '-'
 */
string buildFirstSecretWord(const int& length){
    string secret;
    for (int i = 0; i < length; i++){
        secret += '-';
    }
    return secret;
}

/*
 * Updates the final word with the first element in validWords, if the guessed char is in the element then replace '-' with the guess
 */
void updateSecretWord(const char& guess, string& secretWord, set<string>& validWords){

    auto i = validWords.begin();
    advance(i, 0);
    string word = *i;

    for (unsigned int i = 0; i < word.length(); i++){
        if (word[i] == guess){
            secretWord[i] = guess;
        }
    }
}

/*
 * The function runned for every geuss the user makes and updates the wordfamilies and the wordlist
 */
void tickGame(char& guess, set<char>& guessedChars, set<string>& validWords, const string& secretWord){
    guess = askForGuess(guessedChars, validWords, secretWord);
    map<string, set<string>> fam;
    makeWordFamilies(guess,validWords, fam);
    updateWordlist(fam, validWords);
}

/*
 * Prints information if the user has guessed the words or ran out of guesses
 */
void printEndInfo(const bool& hasWon, bool& play){
    if (hasWon){
        cout << "Congratulation, you won!" << endl;
    } else {
        cout << "You lost, embarrasment to society." << endl;
    }
    askToPlayAgain(play);
}

/*
 * Checks if the guess is in word
 */
bool isInWord(char& guess, const string& word){
    for (unsigned int i = 0; i < word.length(); i++){
        if (word[i] == guess){
            return true;
        }
    }
    return false;
}

int main() {
    bool play = true;
    while (play){
        cout << "Welcome to Hangman." << endl;
        set<string> wordList;
        getDict(wordList);
        int wordLength = askWordLength();
        set<string> validWords;
        getValidWords(wordList, wordLength, validWords);
        map<string,set<string>> fam;
        bool hasWon = true;


        int remainingGuesses = askNumberOfGuesses();
        set<char> guessedChars;
        bool wantsVisible = wantsVisibleWords();
        string secretWord = buildFirstSecretWord(wordLength);
        char guess;
        char dash = '-';

        while (remainingGuesses > 0 && isInWord(dash, secretWord)){

            tickGame(guess, guessedChars, validWords, secretWord);

            auto i = validWords.begin();
            advance(i, 0);
            string word = *i;

            if (word.find(guess) < word.length()){
                updateSecretWord(guess, secretWord, validWords);
            } else {
                guessedChars.insert(guess);
                remainingGuesses--;
            }

            printGameInfo(remainingGuesses, secretWord, guessedChars, validWords, wantsVisible);



        }

        if (remainingGuesses == 0){
            hasWon = false;
        }
        printEndInfo(hasWon, play);
    }
    return 0;
}
