/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef HERO_H
#define HERO_H

#include "Unit.h"
#include <QGraphicsScene>

class Hero : public Unit {
public:
    /*
     * Constructs a new Hero
     */
    Hero();
    
    /*
     * Copy constructor for Hero
     */
    Hero(const Hero& o);

    /*
     * Destructs the current Hero
     */
    ~Hero();
    
    /*
     * Function that clones the existing hero
     */
    Hero* clone() const;
    
    /*
    * Draws this hero onto the given QGraphicsScene.
    */
    void draw(QGraphicsScene *scene) const;
};

#endif // HERO_H
