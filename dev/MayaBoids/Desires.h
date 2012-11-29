// computing "aiming" wills (outputting array of desire vectors)
#ifndef _DESIRES_H_
#define _DESIRES_H_
#include "CpuRules.h"

Boid * leaderBoid;
Vector *arrivalPoint;

void initDesires(Boid *lb);
void desiresComputation(Kdtree *kd, Boid * thisBoid, Boid * lBoid, Vector *desiresVector);

#endif
