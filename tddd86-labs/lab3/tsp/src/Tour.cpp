#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour()
{
}

Tour::Tour(Point a, Point b, Point c, Point d){
    Node *p = new Node(a,front_node);
    Node *q = new Node(b,front_node);
    Node *r = new Node(c,front_node);
    Node *s = new Node(d,front_node);

    front_node = p;
    p->next = front_node;

    Node *ptr = front_node;
    ptr->next = q;
    q->next = front_node;
    front_node = q;

    ptr->next = r;
    r->next = front_node;
    front_node = r;

    ptr->next = s;
    s->next = front_node;
    front_node = s;


}

Tour::~Tour()
{
    Node* head = front_node;
    Node* next = front_node->next;
    Node* curr;

    do{
        curr = next;
        next = next->next;
        delete curr;
    }
    while (next != head);
    delete head;
}

void Tour::show()
{
    Node* temp = front_node;
    if (front_node != nullptr){
        do {
            cout << temp->toString() << " ";
            temp = temp->next;
        }
        while (temp != front_node);

    }
}

void Tour::draw(QGraphicsScene *scene)
{
    Node* temp = front_node;
    if(front_node != nullptr){
        do{
            Node* nxtNode = temp->next;
            temp->point.drawTo(nxtNode->point,scene);
            temp = temp->next;
        }while(temp != front_node);
    }

}

int Tour::size()
{
    Node* temp = front_node;
    int counter = 0;
    if(front_node != nullptr){
        do{
            counter++;
            temp = temp->next;
        }while(temp != front_node);
    }
    return counter;
}

double Tour::distance()
{
    Node* temp = front_node;
    double distanceTour = 0;
    if(front_node != nullptr){
        do{
            Node* nxtNode = temp->next;
            distanceTour += temp->point.distanceTo(nxtNode->point);
            temp = temp->next;
        }while(temp != front_node);
    }
    return distanceTour;
}

void Tour::insertNearest(Point p)
{
    Node* temp = front_node;
    Node* nearestNode = front_node;
    Node* curr = new Node(p,temp);
    if(front_node != nullptr){
        while(temp->next != front_node){

            Node* nxtNode = temp->next;
            if(nearestNode->point.distanceTo(p) > nxtNode->point.distanceTo(p)){
                nearestNode = nxtNode;
            }
            temp = temp->next;
        }
        Node *near = new Node(p,nearestNode->next);
        nearestNode->next = near;
    }else{
        front_node = curr;
        curr->next = front_node;
    }

}

void Tour::insertSmallest(Point p)
{
    Node* temp = front_node;
    Node* smallestNode = front_node;

    double tempSize = -1;

    if(front_node != nullptr){
        while (temp->next != front_node){
            double newTempSize = temp->point.distanceTo(p) + p.distanceTo(temp->next->point) - temp->point.distanceTo(temp->next->point);
            if (tempSize == -1 || newTempSize < tempSize){
                tempSize = newTempSize;
                smallestNode = temp;
            }
            temp = temp->next;
        }
        Node* near = new Node(p, smallestNode->next);
        smallestNode->next = near;
    } else {
        Node* curr = new Node(p);
        front_node = curr;
        curr->next =front_node;
    }
}
