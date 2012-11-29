#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"
#include "..\MayaBoids\RulesSetup.h"

#include "GPU_DataStructuresOperations.cuh"	//inlined function
#include "GPU_Decision.cuh"
#include "GPU_Desires.cuh"
#include "GPU_Locomotion.cuh"

extern "C" void GPUCompute(Boid *boids);
extern "C" int GPUInit(SimulationParameters simParam, RulesParameters *rulesP, int nDes);

/* device global variables */
__device__ __constant__ Vector arrivalPoint;	
__device__ __constant__ RulesParameters rP[MAXNUMBEROFDESIRES];


// host global variables
int nBoids;				// number of boids
uint3 dimBlock;			// cuda resources simulation: block dimension
uint3 dimGrid;			// cuda resources simulation: grid dimension

/*
*********************************************
*     Flock simulation					  *
*********************************************
*/

// locomotion function
__device__  void locomotion(Vector *decisionVector,Boid *currentBoid, double dt)
{
	cudaAddVector(&currentBoid->currentVelocity,decisionVector,&currentBoid->nextVelocity);
	cudaLimitationVector(&currentBoid->nextVelocity, currentBoid->maxSpeed);
	cudaAddVector(&currentBoid->currentPosition,&currentBoid->nextVelocity,&currentBoid->nextPosition);
}

// steering function
__device__ void steering(Boid * currentBoid, Vector *target, Vector *steer)
{
	Vector desired;
	double desiredMagnitude;

	cudaInitVector(steer);
	cudaSubVector(target,&(currentBoid->currentPosition),&desired);
	desiredMagnitude=cudaMagnitudeVector(&desired);

	if(desiredMagnitude>0)
	{
		cudaNormalizeVector(&desired);
		//max speed option
		cudaMultiplyVector(&desired,currentBoid->maxSpeed,&desired);
		cudaSubVector(&desired,&(currentBoid->currentVelocity) ,steer);
		cudaLimitationVector(steer,currentBoid->maxForce);
	}	
}

// COHESION RULE IMPLEMENTATION
__device__ void cohesionRule(Boid*cudaBoidSet, Boid * currentBoid, Vector * steer, int nBoids)
{
	Vector tempVector;
	int count;

	cudaInitVector(&tempVector);
	cudaInitVector(steer);
	count=0;

	for (int i=0;i<nBoids;i++)
	{
		Boid *thisBoid = &cudaBoidSet[i];
		if((thisBoid->id!=currentBoid->id) && (cudaDistanceVector(&thisBoid->currentPosition,&currentBoid->currentPosition)<rP[COHESIONRULE].ruleRadius))
		{
			cudaAddVector(&(thisBoid->currentPosition), &tempVector, &tempVector);
			count++;
		}
	}
	if(count>0)
	{
		cudaMultiplyVector(&tempVector,1.0f/count,&tempVector);
		steering(currentBoid, &tempVector, steer);
		cudaMultiplyVector(steer, rP[COHESIONRULE].ruleFactor,steer);
	}
}

// SEPARATION RULE IMPLEMENTATION
__device__ void separationRule(Boid*cudaBoidSet, Boid * currentBoid, Vector * steer, int nBoids)
{
	Vector tempVector;
	int count;

	cudaInitVector(steer);
	cudaInitVector(&tempVector);
	count=0;

	for (int i=0;i<nBoids;i++)
	{
		double dis=0;
		Boid *thisBoid = &cudaBoidSet[i];
		if((thisBoid->id!=currentBoid->id) && (cudaDistanceVector(&thisBoid->currentPosition,&currentBoid->currentPosition)<rP[SEPARATIONRULE].ruleRadius))
		{
			cudaSubVector(&(thisBoid->currentPosition),&(currentBoid->currentPosition),&tempVector);
			dis = cudaMagnitudeVector(&tempVector);
			cudaMultiplyVector(&tempVector,1.0f/dis,&tempVector);
			cudaSubVector(steer,&tempVector,steer);
			count++;
		}
	}
	if(count>0)
	{
		cudaMultiplyVector(steer,1.0f/count,steer);
		if(cudaMagnitudeVector(steer)>0)
		{	
			cudaNormalizeVector(steer);
			cudaMultiplyVector(steer,currentBoid->maxSpeed,steer);
			cudaSubVector(steer,&(currentBoid->currentVelocity),steer);
			cudaLimitationVector(steer,currentBoid->maxForce);
			cudaMultiplyVector(steer, rP[SEPARATIONRULE].ruleFactor,steer);
		}
	}
}

// ALIGNMENT RULE IMPLEMENTATION
__device__ void alignmentRule(Boid*cudaBoidSet, Boid * currentBoid, Vector * steer, int nBoids)
{
	int count=0;
	cudaInitVector(steer);

	for(int i=0;i<nBoids;i++)
	{
		Boid *thisBoid = &cudaBoidSet[i];
		if((thisBoid->id!=currentBoid->id) && (cudaDistanceVector(&thisBoid->currentPosition,&currentBoid->currentPosition)<rP[ALIGNMENTRULE].ruleRadius))
		{	
			cudaAddVector(&(thisBoid->currentVelocity), steer, steer);
			count++;
		}
	}

	if(count>0)
	{
		cudaMultiplyVector(steer,1.0f/count,steer);
		if(cudaMagnitudeVector(steer)>0)
		{
			cudaNormalizeVector(steer);
			cudaMultiplyVector(steer,currentBoid->maxSpeed,steer);
			cudaSubVector(steer,&(currentBoid->currentVelocity),steer);
			cudaLimitationVector(steer,currentBoid->maxForce);
			cudaMultiplyVector(steer, rP[ALIGNMENTRULE].ruleFactor,steer);
		}
	}
}

// Reach RULE IMPLEMENTATION
__device__ void reachRule(Boid *currentBoid, Vector *outputVector)
{
	cudaInitVector(outputVector);
	steering(currentBoid, &arrivalPoint, outputVector);
	cudaMultiplyVector(outputVector, rP[REACHRULE].ruleFactor,outputVector);
}

// Follow RULE IMPLEMENTATION
__device__ void followRule(Boid *currentBoid,Boid* leader, Vector *outputVector)
{
	cudaInitVector(outputVector);
	if(leader!=NULL)
	{
		steering(currentBoid, &leader->currentPosition, outputVector);
		cudaMultiplyVector(outputVector, rP[FOLLOWRULE].ruleFactor,outputVector);
	}
}
// boids update method
__global__ void GPUupdateBoids(Boid* cboids, int nb)
{
	Vector temp,tot;
	cudaInitVector(&temp);
	cudaInitVector(&tot);

	// brute force implementation
	int tx =  blockIdx.x * blockDim.x + threadIdx.x;
	
	cohesionRule(cboids,&cboids[tx],&tot,nb);
	cudaMultiplyVector(&tot, rP[COHESIONRULE].ruleWeight,&tot);

	alignmentRule(cboids,&cboids[tx],&temp,nb);
	cudaMultiplyVector(&temp, rP[ALIGNMENTRULE].ruleWeight,&temp);
	cudaAddVector(&temp,&tot,&tot);

	separationRule(cboids,&cboids[tx],&temp,nb);
	cudaMultiplyVector(&temp, rP[SEPARATIONRULE].ruleWeight ,&temp);
	cudaAddVector(&temp,&tot,&tot);

	locomotion(&tot,&cboids[tx],0.1);

	__syncthreads();

	cudaCopyVector(&cboids[tx].nextPosition,&cboids[tx].currentPosition);
	cudaCopyVector(&cboids[tx].nextVelocity,&cboids[tx].currentVelocity);
}


// initializing variables
int GPUInit(SimulationParameters simParam, RulesParameters *rulesP, int nDes)
{
	// hardware capability and resources
	int deviceCount,nBlocks;
	cudaDeviceProp deviceCuda;
	
	cudaGetDeviceCount(&deviceCount);
	if (deviceCount>0)
		for(int i=0;i<deviceCount;i++)
		{
			cudaGetDeviceProperties(&deviceCuda,i);
			if(deviceCuda.major>=2)
			{
				if (simParam.numberOfBoids>deviceCuda.maxThreadsPerBlock)
				{
					// managing the right procedure for computing boids 
 					nBlocks =(simParam.numberOfBoids +deviceCuda.maxThreadsPerBlock-1)/deviceCuda.maxThreadsPerBlock ;
			
					dimBlock.x=(simParam.numberOfBoids/nBlocks);
					dimBlock.y=1;
					dimBlock.z=1;

					dimGrid.x=nBlocks;
					dimGrid.y=1;
					dimGrid.z=1;
				}
				else
				{
					dimBlock.x=simParam.numberOfBoids;
					dimBlock.y=1;
					dimBlock.z=1;

					dimGrid.x=1;
					dimGrid.y=1;
					dimGrid.z=1;
				}

				// saving host variable
				nBoids=simParam.numberOfBoids;
				// passing viariables to device 
				cudaMemcpyToSymbol(rP,rulesP,nDes*sizeof(RulesParameters));
			}
			return CUDA_DEVICE_NOT_SUPPORTED;
		}
	else
		return NO_CUDA_DEVICE;

	return OKCUDA;
}

// computing values on cuda device
void GPUCompute(Boid *boids)
{
	Boid * d_boids;

	dim3 threadB(dimBlock);
	dim3 gridB(dimGrid);
	
	cudaMalloc((void**)&d_boids,nBoids*sizeof(Boid));
	cudaMemcpy(d_boids,boids,nBoids*sizeof(Boid),cudaMemcpyHostToDevice);
	GPUupdateBoids<<<gridB,threadB>>>(d_boids,nBoids);			//gpu computing
	//retrive data (copy data from device memory to host memory)
	cudaMemcpy(boids,d_boids,nBoids*sizeof(Boid),cudaMemcpyDeviceToHost);
	//free device resources
	cudaFree(d_boids);
}
