/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"
#include <QGraphicsScene>

class Robot : public Unit {
public:
    /*
     * Constructs a new Robot
     */
    Robot();
    Robot(const Unit& c);
    
    /*
     * Virtual destructor that frees the memory allocated by the Robot
     */
    virtual ~Robot() = default;
    
    void draw(QGraphicsScene* scene) const;
    
    /*
     * Shows that this unit is a Robot and not a Junk
     */
    unsigned virtual int getCollisionCount();
    
    /*
     * Clones the current robot
     */
    virtual Robot* clone() const;
};

#endif // ROBOT_H
