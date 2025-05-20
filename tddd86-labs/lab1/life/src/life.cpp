#include <iostream>
#include "grid.h"
#include "lifeutil.h"
#include <string>
#include <fstream>
#include <grid.h>
using namespace std;


/*
 * Returns the amount of neighbouring coordinates which are in
 * bounds. Takes row- and col indexes and the grid in question.
 */
int getNeighs(const int& nRow, const int& nCol, const Grid<int>& nGrid){
    int counter = 0;
    for (int i = nRow - 1; i <= nRow + 1; i++){
        for (int j = nCol -1; j <= nCol + 1; j++){
            if (!(i == nRow && j == nCol)) {
                if (nGrid.inBounds(i, j)){
                    if (nGrid.get(i, j) == 1) {
                        counter ++;
                    }
                }
            }
        }
    }
    return counter;
}

/*
 * Returns true if the cell in question will be alive or
 * dead in the next generation of the grid.
 */
bool shouldLive(const int& row, const int& col, const Grid<int>& grid){
    if (getNeighs(row, col, grid) < 2){
        return false;
    }
    if (getNeighs(row, col, grid) == 2){
        if (grid.get(row, col) == 0){
            return false;
        } else {
            return true;
        }
    }
    if (getNeighs(row, col, grid) == 3){
        return true;
    }
    else {
        return false;
    }
}

/*
 * Displays the input grid in a more readable form.
 */
void showGrid(const Grid<int>& grid){
    for (int i = 0; i < grid.numRows(); i++){
        for (int j =0; j < grid.numCols(); j++){
            if (grid.get(i, j) == 0){
                cout << '-';
            } else if (grid.get(i, j) == 1){
                cout << 'X';
            }

        }
        cout << '\n';
    }
}

/*
 * Returns the grid for the next generation.
 */
Grid<int> nextGen(Grid<int>& currentGrid){
    Grid<int> newGen = Grid<int>(currentGrid.numRows(), currentGrid.numCols());
    for (int i = 0; i < newGen.numRows(); i++){
        for (int j = 0; j < newGen.numCols(); j++){
            if (shouldLive(i, j, currentGrid)){
                newGen.set(i, j, 1);
            } else {
                newGen.set(i, j, 0);
            }
        }
    }
    return newGen;
}

/*
 * Displays the menu between the grid generations and handles
 * the inputs from user.
 */
void showMenu(Grid<int>& currentGrid){
    char choice;
    cout << "a)nimate, t)ick, q)uit? " << endl;
    cin >> choice;
    if (choice == 't'){
        currentGrid = nextGen(currentGrid);
        showGrid(currentGrid);
        showMenu(currentGrid);

    } else if (choice == 'q'){
        cout << "Have a nice life! " << endl;

    } else if (choice == 'a'){
        for (int i = 0; i < 10; i++){
            currentGrid = nextGen(currentGrid);
            showGrid(currentGrid);
            pause(100);
        }
        cout << "I JUST ANIMATED 10 TIMES" << endl;
        showMenu(currentGrid);
    }
}

/*
 * A program that simulates the Game of Life. It asks for a
 * starting grid in a textfile and then simulates a given number
 * of generations.
 */
int main() {

    string fileName;
    string rows;
    string cols;
    Grid<int> newGrid;

    cout << "Welcome to the Game of Life!"  << endl;
    cout << "Please enter a file name for your game. " << endl;
    cin >> fileName;
    string content;
    fstream ifs;
    ifs.open(fileName);
    getline(ifs,rows);
    getline(ifs,cols);

    //Skapa grid med rows och cols
    newGrid = Grid<int>(stoi(rows), stoi(cols));
    for (int i = 0; i<stoi(rows); i++){
        getline(ifs,content);
        for (unsigned int j = 0; j<content.length(); j++){
            if(content[j]=='X'){
                newGrid.set(i, j, 1);
            }else{
                newGrid.set(i, j, 0);
            }
        }
    }


    showGrid(newGrid);
    showMenu(newGrid);


    return 0;
}


