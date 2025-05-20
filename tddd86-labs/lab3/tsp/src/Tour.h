#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:

    /*
     * Constructs a new empty Tour with a nulöpointer as front node
     */
    Tour();

    /*
     * Tour constructor for test case
     */
    Tour(Point a, Point b, Point c, Point d);

    /*
     * Destructor that frees the memory allocated by the Tour
     */
    ~Tour();

    /*
     * Prints the current tour
     */
    void show();

    /*
     * Draws out the path between the nodes
     */
    void draw(QGraphicsScene* scene);

    /*
     * Gives the number of nodes in the Tour
     */
    int size();

    /*
     * Gives the total distance for the Tour
     */
    double distance();

    /*
     * Uses the insert nearest method, which means
     * it finds the node with the shortest distance to Point p
     * and inserts p there
     */
    void insertNearest(Point p);

    /*
     * Uses the insert smallest method, which means
     * it places Point p where the total traveling distance increase
     * is the smallest
     */
    void insertSmallest(Point p);

private:
    Node* front_node = nullptr;  //default node nullptr
};

#endif // TOUR_H
