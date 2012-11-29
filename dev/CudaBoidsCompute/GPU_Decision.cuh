// computing boids resulting will
#ifndef _GPU_DECISION_H_
#define _GPU_DECISION_H_
#include "GPU_DataStructuresOperations.cuh"

inline __device__  void decisionComputation(Vector *desiresVector,Boid *currentBoid, Vector *decisionVector)
{	
	int i;
	cudaInitVector(decisionVector);
	for(i=0;i<nDesires;i++)
	{
		cudaMultiplyVector(&(desiresVector[i]), rParameters[i]->ruleWeight ,&(desiresVector[i]));
		cudaAddVector(&(desiresVector[i]),decisionVector,decisionVector);
	}
	// acceleration's limit
	cudaLimitationVector(decisionVector,currentBoid->maxAcceleration);
}
#endif
