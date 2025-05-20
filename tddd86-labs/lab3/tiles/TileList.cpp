// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"

TileList::TileList()
{
}

TileList::~TileList()
{
    delete []tiles;
}

void TileList::addTile(Tile tile)
{
    checkCapacity();
    tiles[m_size] = tile;
    m_size++;
}

void TileList::drawAll(QGraphicsScene* scene) const
{
    for (int i = 0; i < m_size; i++){
        tiles[i].draw(scene);
    }
}

int TileList::indexOfTopTile(int x, int y) const
{
    for (int i = m_size - 1; i >= 0; i--){
        if (tiles[i].contains(x,y)){
            return i;
        }
    }
    return -1;
}

void TileList::raise(int x, int y)
{
     int pos = indexOfTopTile(x, y);

    if (pos != -1){
        Tile selectedTile = tiles[pos];
        for (int i = pos; i < m_size-1; i++){
            tiles[i] = tiles[i+1];
        }
        tiles[m_size-1] = selectedTile;
    }


}

void TileList::lower(int x, int y)
{
     int pos = indexOfTopTile(x, y);


    if (pos != -1){
        Tile selectedTile = tiles[pos];
        for (int i = pos; i > 0; i--){
            tiles[i] = tiles[i-1];
        }
        tiles[0] = selectedTile;
    }

}

void TileList::remove(int x, int y)
{
    int pos = indexOfTopTile(x, y);

    if (pos != -1){
        for (int i = pos; i < m_size; i++) {
                tiles[i] = tiles[i+1];
            }

        m_size--;
    }
}

void TileList::removeAll(int x, int y)
{
    for (int i = 0; i < m_size; i++){
        if (tiles[i].contains(x,y)){
            remove(x,y);
        }
    }
}

void TileList::checkCapacity(){
    if(m_size == m_capacity){
        Tile* newTiles = new Tile[m_capacity * 2];
        for (int i = 0; i < m_size; i++){
            newTiles[i] = tiles[i];
        }
        delete[] tiles;
        tiles = newTiles;
        m_capacity *= 2;
    }
}
