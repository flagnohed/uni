#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <list>
#include <set>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/*
 * Reads the dictionary and puts it in a set of valid english words
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
 * Gets the neighbours for a given word, i.e. all words with 1 differing letter. It also notes
 * when a word has been tested already
 */
void getNeighbours(string word, const set<string>& dict, set<string>& testedWords, set<string>& neighbours) {
    for (unsigned int i = 0; i < word.length(); i++){
        for (char letter : ALPHABET){
            string tempWord = word;
            tempWord[i] = letter;
            if (dict.find(tempWord) != dict.end() && testedWords.find(tempWord) == testedWords.end()) {
                neighbours.insert(tempWord);
                testedWords.insert(tempWord);
            }

        }
    }
}

/*
 * Finds and prints the wordchain from word w1 to word w2
 */
void wordChain(string w1, string w2, const set<string>& dict){
    queue<stack<string>> mQueue;
    stack<string> mStack;
    set<string> testedWords;
    stack<string> stackCopy;
    mStack.push(w1);
    mQueue.push(mStack);
    while (!mQueue.empty()) {
        stack<string> firstPlace = mQueue.front();
        mQueue.pop();
        string currentWord = firstPlace.top();
        if (currentWord == w2){
            cout << "Chain from data back to code:" << endl;
            while (!firstPlace.empty()){ // skriver ut wordchainen
                cout << firstPlace.top() << " ";
                firstPlace.pop();
            }
            cout << '\n' << "Have a nice day." << endl;
            break;
        }
        else {
            set<string> neighbours;
            getNeighbours(firstPlace.top(), dict, testedWords, neighbours);
            for (string word : neighbours){
                stack<string> stackCopy(firstPlace);
                stackCopy.push(word);
                mQueue.push(stackCopy);
            }
        }

    }
}

int main() { //vi behöver inte isWordNotInQueue eftersom vi har testedWords nu
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "Please type two words: ";
    set<string> wordList;

    string w1;
    cin >> w1;
    string w2;
    cin >> w2;

    getDict(wordList);

    wordChain(w1, w2, wordList);

    return 0;
}
