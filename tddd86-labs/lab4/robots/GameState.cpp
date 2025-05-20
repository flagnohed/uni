/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"

GameState::GameState(){}

GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {
        Robot* robot;
        robot = new Robot();
        while (!isEmpty (*robot)){
            robot->teleport();
        }
        robots.push_back(robot);
    }
    teleportHero();
}

GameState::~GameState(){
    for (unsigned int i = 0; i < robots.size(); i++){
        delete robots[i];
    }
    robots.clear();
}

GameState::GameState(const GameState &gs){
    hero = Hero(gs.hero);
    robots.resize(gs.robots.size());
    for(unsigned int i = 0; i < robots.size(); i++){
        robots[i] = gs.robots[i]->clone();
    }
}

GameState& GameState::operator=(const GameState &gs){
    if(this == &gs) return *this;
    hero = gs.hero;
    for(unsigned int i = 0; i < robots.size(); i++){
        delete robots[i];
    }
    robots.resize(gs.robots.size());
    for(unsigned int i = 0; i < robots.size(); i++){
        robots[i] = gs.robots[i]->clone();
    }
    return *this;
}

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robots.size(); ++i)
        robots[i]->draw(scene);

    hero.draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++)
        robots[i]->moveTowards (hero);
}

int GameState::countCollisions() {
    int numberDestroyed = 0;
    unsigned int i = 0;
    while (i < robots.size()) {
        bool collision = countRobotsAt(*robots[i]) > 1;
        if (robots[i]->getCollisionCount() == 1){
            if (collision){
                Robot* newJunk = new Junk(*robots[i]);
                delete robots[i];
                robots[i] = newJunk;
                numberDestroyed++;
                }
            }
            i++;
    }
    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    for (unsigned int i = 0; i < robots.size(); i++){
        if (robots[i]->getCollisionCount() == 1){
            return true;
        }
    }
    return false;
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++){
        if (robots[i]->attacks(unit)) return false;
        if (!isEmpty(unit)) return false;
    }
    return true;
}

void GameState::moveHeroTowards(const Point& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++){
        if (robots[i]->at(unit)){
            return false;
        }
    }
    return true;
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        if(robots[i]->at(unit)){
         count++;
        }
    }
    return count;
}
