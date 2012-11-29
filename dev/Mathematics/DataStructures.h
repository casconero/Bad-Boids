#ifndef DATASTUCTURES_H_INCLUDED
#define DATASTUCTURES_H_INCLUDED

#define OBJECTTYPE int
#define SPHERE 0
#define CILINDER 1
#define POLYFACES 2

// Vector struct definition
typedef struct
{
	double x;
	double y;
	double z;
} Vector;

// Boid struct definition
typedef struct boid
{
	long int id;
	Vector currentPosition;
	Vector currentVelocity;
	Vector acceleration;
	Vector nextPosition;
	Vector nextVelocity;

	double maxSpeed;
	double maxAcceleration;
	double wanderTheta;

	// for future implementation
	double maxForce;
	float mass;
	double boundingBoxRadius;

}Boid;



/* 3 dimensional object struct declaration and description:
	a 3d object is a list of faces, each face is built with a list of segments.
	Each segment is built with 2 vertex. A vertex is a point (vector)


	maya trovare api x mesh (che è il metodo con cui gestisce gli oggetti 3d)
*/

typedef struct side 
{
	int id;
	Vector point1;
	Vector point2;
}Side;


typedef struct face 
{
	int id;
	Side *sides;	//clock wise ordered
}Face;

typedef struct object3D
{
	int id;
	OBJECTTYPE objectType;
	Vector center;
	Face *faces;
	double ray;
	double high;
}Object3D;


/*algebric structures*/
typedef struct parametricLine
{
	/*
		X=X1 +A1*T
		Y=Y1 +B1*T
		Z=Z1 +C1*T
	*/
	Vector point;			//(X1,Y1,Z1)
	Vector directionVector;	//(A1,B1,C1)
}ParametricLine;

typedef struct plane
{
	/*		|X-X1	Y-Y1	Z-Z1 |
		DET |X2-X1	Y2-Y1	Z2-Z1|=0	----->	alfa*X + beta*Y + gamma*Z =delta
			|X3-X1	Y3-Y1	Z3-Z1|

		alfa  =	(Y2-Y1)(Z3-Z1)-(Z2-Z1)(Y3-Y1)
		beta  =	(Z2-Z1)(X3-X1)-(X2-X1)(Z3-Z1)
		gamma =	(X2-X1)(Y3-Y1)-(X3-X1)(Y2-Y1)
		delta=alfa*X1 + beta*Y1 + gamma-*Z1

	*/
	Vector p1,p2,p3;
	Vector planeNormal;
	double alfa,beta,gamma,delta;

}Plane;

#endif