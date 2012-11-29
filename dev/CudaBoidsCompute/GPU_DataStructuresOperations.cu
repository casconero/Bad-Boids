#include "GPU_DataStructuresOperations.cuh"
/**/
/*
*********************************************
*     "Vector" struct operation            *
*********************************************
*/

//__device__ void initVector(Vector *v)
//{
//	v->x=0;v->y=0;v->z=0;
//}
//
//__device__ void copyVector(const Vector *inputVector, Vector *outputVector)
//{
//	outputVector->x=inputVector->x;
//	outputVector->y=inputVector->y;
//	outputVector->z=inputVector->z;
//}
//
//__device__ void addVector(Vector *v1,Vector *v2,Vector *v1plusv2)
//{
//	v1plusv2->x=v1->x + v2->x;
//	v1plusv2->y=v1->y + v2->y;
//	v1plusv2->z=v1->z + v2->z;
//}
//
//__device__ void subVector(Vector *v1,Vector *v2,Vector *v1minusv2)
//{
//	v1minusv2->x=v1->x - v2->x;
//	v1minusv2->y=v1->y - v2->y;
//	v1minusv2->z=v1->z - v2->z;
//}
//
//__device__ void multiply(Vector *vettore, double m,Vector *outputVector)
//{
//	outputVector->x=m*vettore->x;
//	outputVector->y=m*vettore->y;
//	outputVector->z=m*vettore->z;
//}
//
//__device__ double magnitude(const Vector *v)
//{
//	double value = v->x * v->x + v->y * v->y + v->z * v->z;
//	return sqrt(value);
//}
//
//__device__ void normalize(Vector *v)
//{
//	double mag = magnitude(v);
//	if (mag==0)
//		return;
//	multiply(v, 1/mag, v);
//}
//
//__device__  double dotProduct(const Vector *v1,const Vector *v2){return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z ); }
//
//__device__  double angleBetween(const Vector *v1, const Vector *v2)
//{
//
//	double p1 = dotProduct(v1, v2);
//	double p2 = magnitude(v1) * magnitude(v2);
//	double p3 = acos(p1/p2);
//	return p3;
//}
//
//__device__ BOOL isNotEqual(const Vector *v1,const Vector *v2){ return(v1->x != v2->x || v1->y != v2->y || v1->z != v2->z); }
//
//__device__ BOOL isEqual(const Vector *v1,const Vector *v2){ return(v1->x == v2->x && v1->y == v2->y && v1->z == v2->z); }
//
//__device__ void crossProduct(const Vector *v1,const Vector *v2, Vector *outputVector)
//{
//	outputVector->x = v1->y * v2->z - v1->z * v2->y;
//	outputVector->y = v1->z * v2->x - v1->x * v2->z;
//	outputVector->z = v1->x * v2->y - v1->y * v2->x;
//}
//
//__device__ void direction(const Vector *v, Vector *oVector)
//{
//	copyVector(v, oVector);
//	normalize(oVector);
//}
//
//__device__ void setMagnitude(Vector *v, double magnitude)
//{
//	normalize(v);
//	multiply(v, magnitude, v);
//}
//
//__device__ void copyMagnitude(const Vector *inputVector, Vector *outputVector)
//{
//	double mag = magnitude(inputVector);
//	setMagnitude(outputVector, mag);
//}
//
//__device__ void setDirection(Vector *v)
//{
//	double m = magnitude(v);
//	Vector *d = v;
//	normalize(d);
//	multiply(d, m, d);
//	copyVector(d, v);
//}
//
//__device__ void copyDirection(const Vector *inputVector, Vector *outputVector)
//{
//	double m = magnitude(outputVector);
//	Vector copyV;
//	copyVector(inputVector, &copyV);
//	normalize(&copyV);
//	multiply(&copyV, m, outputVector);
//}
//
//__device__ void limitation(Vector *inputVector, double limit)
//{
//	if(magnitude(inputVector)>limit)
//	{
//		normalize(inputVector);
//		multiply(inputVector,limit,inputVector);
//	}
//}
//
//__device__ double distance(Vector *v1, Vector*v2)
//{
//	Vector vd;
//	subVector(v1,v2,&vd);
//	return magnitude(&vd);
//}
//
////*
////*********************************************
////*     "Boid" struct operation					*
////*********************************************
////*/
//
//__device__ void copyBoid(const Boid *boidSource,Boid *boidDestination)
//{
//	copyVector(&(boidSource->acceleration),&(boidDestination->acceleration));
//
//	copyVector(&(boidSource->currentPosition),&(boidDestination->currentPosition));
//	copyVector(&(boidSource->nextPosition),&(boidDestination->nextPosition));
//
//	copyVector(&(boidSource->currentVelocity),&(boidDestination->currentVelocity));
//	copyVector(&(boidSource->nextVelocity),&(boidDestination->nextVelocity));
//
//	boidDestination->id=boidSource->id;
//	boidDestination->boundingBoxRadius=boidSource->boundingBoxRadius;
//	boidDestination->mass=boidSource->mass;
//	boidDestination->maxAcceleration=boidSource->maxAcceleration;
//	boidDestination->maxForce=boidSource->maxForce;
//	boidDestination->maxSpeed=boidSource->maxSpeed;
//	boidDestination->wanderTheta=boidSource->wanderTheta;
//
//
//}
//
//__device__ void initBoid(const Vector *p, const Vector *v, const Vector *a, double speed, double acceleration, double mForce, double bMass, long int id, Boid *boid)
//{
//	copyVector(p,&(boid->currentPosition));
//	copyVector(p,&(boid->nextPosition));
//	copyVector(v,&(boid->currentVelocity));
//	copyVector(p,&(boid->nextVelocity));
//	copyVector(a,&(boid->acceleration));
//
//	boid->id=id;
//	boid->mass=bMass;
//	boid->maxAcceleration=acceleration;
//	boid->maxForce=mForce;
//	boid->maxSpeed=speed;
//	boid->wanderTheta=0;
//}

