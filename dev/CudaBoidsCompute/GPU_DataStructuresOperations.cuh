#ifndef GPU_DATA_STRUCTURES_H_INCLUDED
#define GPU_DATA_STRUCTURES_H_INCLUDED
#include "..\Mathematics\DataStructures.h"
#include "..\MayaBoids\Boolean.h"
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

/*
*
*********************************************
*     "Vector" struct operation            *
*********************************************
*/

__device__ inline void cudaInitVector(Vector *v)
{
	v->x=0;v->y=0;v->z=0;
}

__device__ inline void cudaCopyVector(const Vector *inputVector, Vector *outputVector)
{
	outputVector->x=inputVector->x;
	outputVector->y=inputVector->y;
	outputVector->z=inputVector->z;
}

__device__ inline void cudaAddVector(Vector *v1,Vector *v2,Vector *v1plusv2)
{
	v1plusv2->x=v1->x + v2->x;
	v1plusv2->y=v1->y + v2->y;
	v1plusv2->z=v1->z + v2->z;
}

__device__ inline void cudaSubVector(Vector *v1,Vector *v2,Vector *v1minusv2)
{
	v1minusv2->x=v1->x - v2->x;
	v1minusv2->y=v1->y - v2->y;
	v1minusv2->z=v1->z - v2->z;
}

__device__ inline void cudaMultiplyVector(Vector *vettore, double m,Vector *outputVector)
{
	outputVector->x=m*vettore->x;
	outputVector->y=m*vettore->y;
	outputVector->z=m*vettore->z;
}

__device__ inline double cudaMagnitudeVector(const Vector *v)
{
	double value = v->x * v->x + v->y * v->y + v->z * v->z;
	return sqrt(value);
}

__device__ inline void cudaNormalizeVector(Vector *v)
{
	double mag = cudaMagnitudeVector(v);
	if (mag==0)
		return;
	cudaMultiplyVector(v, 1/mag, v);
}

__device__ inline  double cudaDotProduct(const Vector *v1,const Vector *v2){return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z ); }

__device__ inline  double cudaAngleBetweenVector(const Vector *v1, const Vector *v2)
{

	double p1 = cudaDotProduct(v1, v2);
	double p2 = cudaMagnitudeVector(v1) * cudaMagnitudeVector(v2);
	double p3 = acos(p1/p2);
	return p3;
}

__device__ inline BOOL cudaIsNotEqual(const Vector *v1,const Vector *v2){ return(v1->x != v2->x || v1->y != v2->y || v1->z != v2->z); }

__device__  inline BOOL cudaIsEqual(const Vector *v1,const Vector *v2){ return(v1->x == v2->x && v1->y == v2->y && v1->z == v2->z); }

__device__ inline void cudaCrossProduct(const Vector *v1,const Vector *v2, Vector *outputVector)
{
	outputVector->x = v1->y * v2->z - v1->z * v2->y;
	outputVector->y = v1->z * v2->x - v1->x * v2->z;
	outputVector->z = v1->x * v2->y - v1->y * v2->x;
}

__device__ inline void cudaDirectionVector(const Vector *v, Vector *oVector)
{
	cudaCopyVector(v, oVector);
	cudaNormalizeVector(oVector);
}

__device__ inline void cudaSetMagnitudeVector(Vector *v, double magnitude)
{
	cudaNormalizeVector(v);
	cudaMultiplyVector(v, magnitude, v);
}

__device__ inline void cudaCopyMagnitudeVector(const Vector *inputVector, Vector *outputVector)
{
	double mag = cudaMagnitudeVector(inputVector);
	cudaSetMagnitudeVector(outputVector, mag);
}

__device__ inline void cudaSetDirectionVector(Vector *v)
{
	double m = cudaMagnitudeVector(v);
	Vector *d = v;
	cudaNormalizeVector(d);
	cudaMultiplyVector(d, m, d);
	cudaCopyVector(d, v);
}

__device__ inline void cudaCopyDirectionVector(const Vector *inputVector, Vector *outputVector)
{
	double m = cudaMagnitudeVector(outputVector);
	Vector copyV;
	cudaCopyVector(inputVector, &copyV);
	cudaNormalizeVector(&copyV);
	cudaMultiplyVector(&copyV, m, outputVector);
}

__device__ inline void cudaLimitationVector(Vector *inputVector, double limit)
{
	if(cudaMagnitudeVector(inputVector)>limit)
	{
		cudaNormalizeVector(inputVector);
		cudaMultiplyVector(inputVector,limit,inputVector);
	}
}

__device__ inline double cudaDistanceVector(Vector *v1, Vector*v2)
{
	Vector vd;
	cudaSubVector(v1,v2,&vd);
	return cudaMagnitudeVector(&vd);
}

/*
*********************************************
*     "Boid" struct operation					*
*********************************************
*/

__device__ inline void cudaCopyBoid(const Boid *boidSource,Boid *boidDestination)
{
	cudaCopyVector(&(boidSource->acceleration),&(boidDestination->acceleration));

	cudaCopyVector(&(boidSource->currentPosition),&(boidDestination->currentPosition));
	cudaCopyVector(&(boidSource->nextPosition),&(boidDestination->nextPosition));

	cudaCopyVector(&(boidSource->currentVelocity),&(boidDestination->currentVelocity));
	cudaCopyVector(&(boidSource->nextVelocity),&(boidDestination->nextVelocity));

	boidDestination->id=boidSource->id;
	boidDestination->boundingBoxRadius=boidSource->boundingBoxRadius;
	boidDestination->mass=boidSource->mass;
	boidDestination->maxAcceleration=boidSource->maxAcceleration;
	boidDestination->maxForce=boidSource->maxForce;
	boidDestination->maxSpeed=boidSource->maxSpeed;
	boidDestination->wanderTheta=boidSource->wanderTheta;


}

__device__ inline void cudaInitBoid(const Vector *p, const Vector *v, const Vector *a, double speed, double acceleration, double mForce, double bMass, long int id, Boid *boid)
{
	cudaCopyVector(p,&(boid->currentPosition));
	cudaCopyVector(p,&(boid->nextPosition));
	cudaCopyVector(v,&(boid->currentVelocity));
	cudaCopyVector(p,&(boid->nextVelocity));
	cudaCopyVector(a,&(boid->acceleration));

	boid->id=id;
	boid->mass=bMass;
	boid->maxAcceleration=acceleration;
	boid->maxForce=mForce;
	boid->maxSpeed=speed;
	boid->wanderTheta=0;
}
#endif // GPU_DATA_STRUCTURES_H_INCLUDED