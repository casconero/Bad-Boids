#ifndef BOID_H_INCLUDED
#define BOID_H_INCLUDED

#include "Vector.h"
#include "DataStructures.h"



// Boid duplication "boidSource" nel bod destinazione "boidDestination"
void copyBoid(const Boid *boidSource,Boid *boidDestination);

// Boid initialization
void initBoid(const Vector *p, const Vector *v, const Vector *a, double speed, double acceleration, double mForce, double bMass, long int id, Boid *boid);

void printBoid(Boid *output);

#endif // BOID_H_INCLUDED