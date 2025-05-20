/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef UNIT_H
#define UNIT_H

#include "utilities.h"
#include <QGraphicsScene>


/* Root class for all pieces on the board.
 * Subclasses are Robot, Hero and Junk.
 */
class Unit {
public:
    /*
     * Constructs a new Unit
     */
    Unit();

    /*
     * Copy constructor for Unit
     */
    Unit(const Unit& u);

    /*
     * Constructs a Unit of a Point
     */
    Unit(const Point& p);

    /*
     * Virtual destructor for Unit that frees the allocated memory
     */
    virtual ~Unit();

    /*
     * Function that clones the Unit
     */
    virtual Unit* clone() const;

    /*
    * Return Point representation of Unit
    */
    Point asPoint() const;

    /*
    * Am I in the same square as u?
    */
    bool at(const Unit& u) const;

    /*
    * Can I catch u in one move?
    */
    virtual bool attacks(const Unit& u) const;

    /*
    * Take one step closer to u
    */
    virtual void moveTowards(const Unit& u);

    virtual void moveTowards(const Point& p);

    /*
    * Teleport. Does not check for collision
    */
    void teleport();

    /*
    * Euclidean distance to u
    */
    double distanceTo(const Unit& u) const;

    virtual void draw(QGraphicsScene *scene) const{}

private:
    int x;  // x position of this unit
    int y;  // y position of this unit

    // private helpers
    void checkBounds();
};

#endif // UNIT_H
