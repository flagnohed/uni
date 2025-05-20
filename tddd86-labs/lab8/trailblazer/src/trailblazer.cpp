// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include <set>
#include "pqueue.h"
#include <vector>
#include <algorithm>
#include "queue"

using namespace std;

//Depth first algorithm
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {

    vector<Vertex*> path;
    start->visited = true;
    start->setColor(GREEN); //visit the node in question

    if(start == end){       //found the target
        path.push_back(start);
        graph.resetData();

    }else{
        for(Node* n : graph.getNeighbors(start)){
            if(!n->visited){
                vector<Node*> sub = depthFirstSearch(graph,n,end);
                if(!sub.empty()){       // if sub is a path to end
                    path.push_back(start);
                    path.insert(path.end(),sub.begin(),sub.end());
                    break;
                }else{
                    n->setColor(GRAY);
                }
            }
        }
    }
    return path;
}

//Breadth first algorithm
vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    vector<Vertex*> path;
    queue<Vertex*> q;
    Vertex* first;

    q.push(start);

    while(!q.empty()){
        first = q.front();
        first->visited = true;
        first->setColor(GREEN);

        if(first == end){   //found end node
            while(first != nullptr){
                path.push_back(first);
                first = first->previous;
            }
            reverse(path.begin(),path.end());   //get correct order
            return path;
        }
        // else if we haven't found end yet
        q.pop();
        for(Vertex* n : graph.getNeighbors(first)){
            if(!n->visited){
                n->previous= first; //make link from first to n
                q.push(n);
                n->setColor(YELLOW);
            }
        }
    }
    return path;
}

//Djikstras algorithm
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Node* > path;
    PriorityQueue<Vertex*> pq;

    graph.resetData();

    for(Vertex* v : graph.getVertexSet()){
        v->cost = INFINITY;
        pq.enqueue(v,v->cost);  //put every vertex in a PriorityQueue with cost infinity
    }
    start->cost = 0.0;
    pq.changePriority(start,start->cost);

    while(!pq.isEmpty()){
        Vertex* v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);
        if(v == end){
            while(v != nullptr){
                path.push_back(v);
                v = v->previous;    //track from end back to start
            }
            reverse(path.begin(),path.end());   //get correct order in path
            return path;
        }
        //else if we haven't found end yet
        for(Vertex* n : graph.getNeighbors(v)){
            if(!n->visited){
                double cost = v->cost+graph.getEdge(v,n)->cost;
                    if(cost < n->cost){     //check if we have found a cheaper cost for node n
                        n->previous = v;
                        n->cost = cost;
                        n->setColor(YELLOW);

                        pq.changePriority(n,cost);      //update in pq accordingly
                }
            }
        }
    }
   return path;
}

//A-star algorithm, similar to Djikstras but with heuruistics.
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;
    PriorityQueue<Vertex*> pq;

    graph.resetData();
    for(Vertex* v : graph.getVertexSet()){
        v->cost = INFINITY;     //enqueue all vertexes with cost infinity
        pq.enqueue(v,v->cost);
    }
    start->cost = 0;
    pq.enqueue(start,start->heuristic(end));    //use heuruistic instead of cost (estimated distance to end)
    while(!pq.isEmpty()){
        Vertex* v = pq.dequeue();
        v->visited = true;
        v->setColor(GREEN);
        if(v == end){       //found target node
            while(v != nullptr){
                path.push_back(v);
                v = v->previous;
            }
            reverse(path.begin(),path.end());
            break;
        }
        //else if we haven't found end yet
        for(Vertex* n : graph.getNeighbors(v)){ //for all unvisited neighbors
            if(!n->visited){
                double cost = v->cost+graph.getEdge(v,n)->cost;
                double prioCost = cost+n->heuristic(end);
                if(cost < n->cost){ //found cheaper cost for neighbor
                    n->previous = v;    //link v to n
                    n->cost = cost;
                    pq.changePriority(n,prioCost);  //update priority in pq
                    n->setColor(YELLOW);
                }
            }
        }
    }
    return path;
}
