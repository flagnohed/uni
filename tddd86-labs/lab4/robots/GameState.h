/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 *
 * Maintains the game state:  Location of all robots, junk and hero.
 * Method isSafe() is used to keep Hero from moving stupidly.
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <vector>
#include <QGraphicsScene>
#include "Unit.h"
#include "Robot.h"
#include "Junk.h"
#include "Hero.h"

class GameState {
public:
    /*
     * Constructs a new GameState with a vector robots and a hero
     */
    GameState();

    /*
     * Destructor that frees the memory allocated by the GameState
     */
    ~GameState();

    /*
     * Constructor that fills the vector robots with the desired amount of Robots.
     */
    GameState(int numberOfRobots);

    /*
     * Copy constructor for GameState
     */
    GameState(const GameState &gs);

    /*
     * Assignment operator for GameState
     */
    GameState& operator =(const GameState &gs);

    /*
     * Clear and redraw entire playing field
     */
    void draw(QGraphicsScene* scene) const;	// Clear and redraw entire playing field

    /*
     * Teleport hero to random location
     */
    void teleportHero();

    /*
     * Move robots one step towards hero
     */
    void moveRobots();

    /* Count colliding robots
     * Also converts robots to junk while
     * checking collisions
     */
    int countCollisions ();

    /*
     * Check if any robots are left on playing field
     */
    bool anyRobotsLeft () const;

    /*
     * Is hero in same place as robot or junk?
     */
    bool heroDead() const;

    /*
     * Can unit safely reside here?
     */
    bool isSafe (const Unit& unit) const; // Can unit safely reside here?

    /*
     * Move hero towards dir
     */
    void moveHeroTowards (const Point& dir);

    /*
     * Return hero
     */
    Hero getHero () const;

private:
    std::vector<Robot*> robots;  // the robots
    Hero hero;                  // the hero

    // private helpers
    bool isEmpty(const Unit& unit) const;
    int countRobotsAt(const Unit& unit) const;

};

#endif // GAMESTATE_H
