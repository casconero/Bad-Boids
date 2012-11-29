#include "GeometricComputing.h"

void main()
{
	/*Vector v1;

	v1.x=10;
	v1.y=20;
	v1.z=2;
	printf("v1=[%f][%f][%f]\n",v1.x,v1.y,v1.z);
	addVector(&v1,&v1,&v1);
	limitation(&v1,5);
	printf("limit v1 a 5 v1=[%f][%f][%f]\n ",v1.x,v1.y,v1.z);
	getchar();*/
	Vector p1,p2,p3;
	Plane plane;
	ParametricLine line;

	

	p1.x=1;p1.y=1;p1.z=3;
	p2.x=2;p2.y=1;p2.z=4;
	p3.x=1;p3.y=0;p3.z=5;
	plane=getPlane(&p1,&p2,&p3);
	printPlaneEquation(&plane);

	p1.x=1;p1.y=1;p1.z=3;
	p2.x=12;p2.y=1;p2.z=0;
	line=getLine(&p1,&p2);
	printParametricLine(&line);

	p3=linePlaneIntersection(&line, &plane);
	printVector("Punto di intersezione",&p3);
	getchar();

}