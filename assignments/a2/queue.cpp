/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452

   Modiified by: Brandon Hillbom
   Course: ENSE452
   October 15, 2023
   ID: 200426340
   
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

Queue::Queue()
{
    head = 0;
    tail = 0;
    nelements = 0;
    verbose = false;
}

Queue::~Queue()
{
    for (QElement* qe = head; qe != 0;)
    {
	QElement* temp = qe;
	qe = qe->next;
	delete(temp);
    }
}

void Queue::remove(Data* d)
{
    if (size() > 0)
    {
        QElement* qe = head;
        head = head->next;
        nelements--;
        *d = qe->data;
	delete qe;
    }
    else
    {
        cerr << "Error: Queue is empty.\n";
        exit(1);
    }
}

void Queue::insert(Data d)
{
    if (verbose) std::cout << "insert(d)\n";
    QElement* el = new QElement(d);
    if (size() > 0)
    {
        tail->next = el;
    }
    else
    {
        head = el;
    }
    tail = el;
    nelements++;
}

//insert Data d at specified position in the linked list
void Queue::insert(Data d, unsigned position)
{
    if (verbose) std::cout << "insert(d) at position:" << position << endl;

    if (position > size()) {
        //Trying to insert at a position that is beyond the scope of the list
        std::cerr << "Insert: range error" << endl; 
        exit(3); //exit with code 3
    }
    QElement* el = new QElement(d);
    if (size() == 0) {
        //empty list, head and tail are assigned value of el
        head = el;
        tail = el;
    }
    else if (position == 0) {
        //insert at head
        el->next = head; //point our new head to the former head
        head = el; //assign our new head
    }
    else {
        QElement* currentEl = head;
        for (unsigned i = 0; i < position - 1; i++)
            {
                currentEl = currentEl -> next; //traverse list 
            }
        el->next = currentEl->next; //point our element to former position element
        currentEl->next = el; //point the previous element to our inserted element
        if (position == size()) {
            //if the position is tail, assign the tail to el
            tail = el;
        }

    }
     nelements++; //increase size
}

bool Queue::search(Data otherData) const
{
    QElement* insideEl = head;
    for (int i = 0; i < nelements; i++)
    {
        if (insideEl->data.equals(otherData))
            return true;
        insideEl = insideEl->next;
    }
    return false;
}

void Queue::print() const
{
    QElement* qe = head;
    if (size() > 0)
    {
        for (unsigned i = 0; i < size(); i++)
        {
            cout << i << ":(" << qe->data.x << "," << qe->data.y << ") ";
            qe = qe->next;
        }
    }
    cout << "\n";
}

unsigned Queue::size() const
{
    return unsigned(nelements);
}
