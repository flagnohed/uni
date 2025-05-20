#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

class TileList {
public:
    /*
     * Constructs a new empty TileList with capacity 10.
     */
    TileList();

    /*
     * Destructor that frees the memory allocated by the TileList.
     */
    ~TileList();

    /*
     * Adds a tile to the end of a TileList.
     */
    void addTile(Tile tile);

    /*
     * Draws all the Tiles in the TileList.
     */
    void drawAll(QGraphicsScene* scene) const;

    /*
     * Returns the index of the last (highest) Tile
     * at those given coordinates in the TileList.
     */
    int indexOfTopTile(int x, int y) const;

    /*
     * Puts the highest of the Tiles at those
     * coordinates at the front of the TileList.
     */
    void lower(int x, int y);

    /*
     * Puts the highest of the Tiles at those
     * coordinates at the end of the TileList.
     */
    void raise(int x, int y);

    /*
     * Removes the highest Tile with those given
     * coordinates from the TileList.
     */
    void remove(int x, int y);

    /*
     * Removes all Tiles with those coordinates in the
     * TileList.
     */
    void removeAll(int x, int y);

private:
    /*
     * Helper function to check if resize is needed.
     */
    void checkCapacity();

    int m_size = 0;                     // number of elements added
    int m_capacity = 10;                // length of array
    Tile* tiles = new Tile[m_capacity]; // array of Tiles

};

#endif // TILELIST_H
