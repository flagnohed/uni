/*
 * TDDD86 Pattern Recognition
 * This program computes and plots all line segments involving 4 points
 * in a file using Qt.
 */

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "Point.h"

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for(const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input1600.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Brute Force Pattern Recognition");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
    sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();
    //BRUTE-KODEN
    /*
    // iterate through all combinations of 4 points
    for (int i = 0 ; i < N-3 ; ++i) {
        for (int j = i+1 ; j < N-2 ; ++j) {
            for (int k = j+1 ; k < N-1 ; ++k) {
                //only consider fourth point if first three are collinear
                if (points.at(i).slopeTo(points.at(j)) == points.at(i).slopeTo(points.at(k))) {
                    for (int m{k+1} ; m < N ; ++m) {
                        if (points.at(i).slopeTo(points.at(j)) == points.at(i).slopeTo(points.at(m))) {
                            render_line(scene, points.at(i), points.at(m));
                            a.processEvents(); // show rendered line
                        }
                    }
                }
            }
        }
    }
*/
    //Det här är vår kod som vi har skrivit
    for (unsigned int i = 0; i < points.size(); ++i){
        map<double, vector<Point>> sortingMap;

        Point p = points[i];
        for (unsigned int j = i+1; j < points.size(); ++j){ //jämför p med alla framför p
            double slope = p.slopeTo(points[j]);
            sortingMap[slope].push_back(points[j]); //sortera in i map med slope som key
        }
        for (auto const & pair : sortingMap){
            vector<Point> linear = pair.second;
            if (linear.size() >= 3){
                for (unsigned int i = 0; i < linear.size(); i++){
                    render_line(scene, p, linear[i]);
                    a.processEvents();
                }
            }
        }
    }
    //Slut på vår kod

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
