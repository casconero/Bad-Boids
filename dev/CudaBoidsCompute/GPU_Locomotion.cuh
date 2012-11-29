// turning decision into movement
#ifndef _GPU_LOCOMOTION_H_
#define _GPU_LOCOMOTION_H_
#include "GPU_DataStructuresOperations.cuh"

//inline __device__ void locomotionComputation(Vector *decisionVector,Boid *currentBoid, double dt)
//{
//	Vector sV, sA, space, nVelocity;
//	initVector(&sV);
//	initVector(&space);
//	initVector(&sA);
//	initVector(&nVelocity);
//
//	// Aggiornamento dello spostamento, moto accelerato
//	// S, V, A vettori
//	// S = S + V * Dt + 0.5 * A * Dt * Dt
//	// V_new = A * dt + V_old;
//
//	//computing new Boid's position
//	multiply(decisionVector,dt*dt/2,&sA);
//	multiply(&(currentBoid->currentVelocity),dt,&sV);
//	addVector(&sV,&sA,&space);
//	addVector(&(currentBoid->currentPosition),&space,&space);
//
//	// new velocity
//	// NEW_VEL = ACC * dt + OLD_VEL
//	multiply(decisionVector,dt,&nVelocity);
//	limitation(&nVelocity, currentBoid->maxSpeed);
//	addVector(&(currentBoid->currentVelocity),&nVelocity,&nVelocity);
//
//	// new position, velocity & acceleration
//	copyVector(&space,&(currentBoid->nextPosition));	
//	copyVector(&nVelocity,&(currentBoid->nextVelocity));
//	copyVector(decisionVector,&(currentBoid->acceleration));
//}
//
//// simple locomotion
//inline __device__ void locomotionComputationSimplified(Vector *decisionVector,Boid *currentBoid, double dt)
//{
//	addVector(&currentBoid->nextVelocity,decisionVector,&currentBoid->nextVelocity);
//	limitation(&currentBoid->nextVelocity, currentBoid->maxSpeed);
//	addVector(&currentBoid->nextPosition,&currentBoid->nextVelocity,&currentBoid->nextPosition);
//}
#endif