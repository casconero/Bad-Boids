#ifndef GEOMETRICCOMPUTING_INCLUDED
#define GEOMETRICCOMPUTING_INCLUDED

#include "Vector.h"
#include "DataStructures.h"

#define Point Vector	// ridefinizione di vettore in punto geometrico

Plane getPlane(Point *p1,Point *p2,Point *p3);
void printPlaneEquation(Plane *plane);

ParametricLine getLine(Point *p1,Point *p2);
void printParametricLine(ParametricLine *line);

Point linePlaneIntersection(ParametricLine *line, Plane *plane);

//punto di intersezione tra una retta ortogonale alla retta "line" e passante per il punto "p"  
Point pointAtMinorDistance(Point *p, ParametricLine *line);

void computePlaneNormal(Plane *plane);

int crossingNumberTest();

void computeObjectRepulsionVector(Point p, Vector normalVector, double distance, double alfa, Vector* repulsionVector);
#endif