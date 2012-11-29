#include "GeometricComputing.h"

Plane getPlane(Point *p1,Point *p2,Point *p3)
{
	Plane pl;

	copyVector(p1,&pl.p1);
	copyVector(p2,&pl.p2);
	copyVector(p3,&pl.p3);

		//alfa=(Y2-Y1)(Z3-Z1)-(Z2-Z1)(Y3-Y1)
	pl.alfa=(p2->y-p1->y)*(p3->z-p1->z)-(p2->z-p1->z)*(p3->y-p1->y);

		//beta=(Z2-Z1)(X3-X1)-(X2-X1)(Z3-Z1)
	pl.beta=(p2->z-p1->z)*(p3->x-p1->x)-(p2->x-p1->x)*(p3->z-p1->z);

		//gamma=(X2-X1)(Y3-Y1)-(X3-X1)(Y2-Y1)
	pl.gamma=(p2->x-p1->x)*(p3->y-p1->y)-(p3->x-p1->x)*(p2->y-p1->y);

		//delta=alfa*Z1 + beta*Y1 + gamma*Z1
	pl.delta=pl.alfa*p1->x + pl.beta*p1->y + pl.gamma*p1->z;

	return pl;
};


void printPlaneEquation(Plane *pl)
{
	//a*X + b*Y + c*Z =d
	printf("Plane equation: %f X %+f Y %+f Z = %f\n",pl->alfa,pl->beta,pl->gamma,pl->delta);
};


ParametricLine getLine(Point *p1,Point *p2)
{
	ParametricLine line;
	subVector(p1,p2,&line.directionVector);
	copyVector(p2,&line.point);
	return line;
}

void printParametricLine(ParametricLine *line)
{
	/*
		X=X1 +A1*T
		Y=Y1 +B1*T
		Z=Z1 +C1*T
	*/
	printf("Parametric line equation:\n");
	printf("X= %f %+f T\n",line->point.x,line->directionVector.x);
	printf("Y= %f %+f T\n",line->point.y,line->directionVector.y);
	printf("Z= %f %+f T\n",line->point.z,line->directionVector.z);
};


Point linePlaneIntersection(ParametricLine *line,Plane *p)
{
	/*
		Plane equation: 
		alfa*X + beta*Y + gamma*Z =delta
		
		Parametric Line equation:
		X=X1 +A1*T
		Y=Y1 +B1*T
		Z=Z1 +C1*T

		Computing algorithm:
		L= alfa*A1 + beta*B1 + gamma*C1
		K= alfa*X1 + beta*Y1 + gamma*Z1
		T=(delta - K) / L
	*/

	Point intersectionPoint;
	double T,K,L;
	initVector(&intersectionPoint);
	K = (p->alfa * (line->point.x)) + (p->beta * (line->point.y)) + (p->gamma * (line->point.z));
	L = (p->alfa * (line->directionVector.x)) + (p->beta * (line->directionVector.y)) + (p->gamma * (line->directionVector.z));
	T = (p->delta - K)/L;

	multiply(&line->directionVector, T,&intersectionPoint);
	addVector(&line->point,&intersectionPoint,&intersectionPoint);

	return intersectionPoint;
};

Point pointAtMinorDistance(Point *p, ParametricLine *line)
{
	//http://it.answers.yahoo.com/question/index?qid=20100710081547AA6F7mK

	Plane pl;	
	Point point;

	pl.delta=p->x*line->directionVector.x + p->y*line->directionVector.y + p->z*line->directionVector.z ;
	pl.alfa=line->directionVector.x;
	pl.beta=line->directionVector.y;
	pl.gamma=line->directionVector.z;

	point=linePlaneIntersection(line,&pl);
	return point;
};

void computePlaneNormal(Plane *plane)
{
/*
	Il vettore normale N(nx,ny,nz) ad un triangolo di vertici 
	p1=(x1,y1,z1), p2=(x2,y2,z1), p3=(x3,y3,z3) si calcola 
	con la seguente formula:

	nx = dy12*dz13 - dz12*dy13;
	ny = dz12*dx13 - dx12*dz13;
	nz = dx12*dy13 - dy12*dx13;
	
	dove

	dx12 = x1-x2; dy12 = y1-y2; dz12 = z1-z2;
	dx13 = x1-x3; dy13 = y1-y3; dz13 = z1-z3;

	Il vettore N va poi normalizzato.
*/

	double dx12,dy12,dz12,dx13,dy13,dz13;
	
	dx12=plane->p1.x - plane->p2.x;
	dy12=plane->p1.y - plane->p2.y;
	dz12=plane->p1.z - plane->p2.z;
	dx13=plane->p1.x - plane->p3.x;
	dy13=plane->p1.y - plane->p3.y;
	dz13=plane->p1.z - plane->p3.z;

	plane->planeNormal.x=dy12*dz13-dz12*dy13;
	plane->planeNormal.y=dz12*dx13-dx12*dz13;
	plane->planeNormal.z=dx12*dy13-dy12*dx13;

	normalize(&plane->planeNormal);
};

int crossingNumberTest(Point p, Point* vertex,int n)
{	
/* 

	http://softsurfer.com/Archive/algorithm_0103/algorithm_0103.htm
	[1]	the point is inside polygon
	[0] the point is outside the polygon

	3D Planar Polygons
	In 3D applications, one sometimes wants to test a point and polygon that are in the same plane.  
	For example, one may have the intersection point of a ray with the plane of a polyhedron's face, 
	and want to test if it is inside the face.  Or one may want to know if the base of a 3D 
	perpendicular dropped from a point is inside a planar polygon. 3D inclusion is easily determined by 
	projecting the point and polygon into 2D.  To do this, one simply ignores one of the 3D coordinates 
	and uses the other two.  To optimally select the coordinate to ignore, compute a normal vector to the plane, 
	and select the coordinate with the largest absolute value [Snyder & Barr, 1987].  This gives the projection 
	of the polygon with maximum area, and results in robust computations.
*/
	int i,cn = 0;
	double vt=0;
	// loop through all edges of the polygon, ignore "z" dimension
	for (i=0; i<n; i++) 
	{    
		if (((vertex[i].y <= p.y) && (vertex[i+1].y > p.y)) || ((vertex[i].y > p.y) && (vertex[i+1].y <= p.y))) 
		{ 
			vt = (double)(p.y - vertex[i].y) / (vertex[i+1].y - vertex[i].y);
			if (p.x < vertex[i].x + vt * (vertex[i+1].x - vertex[i].x)) // P.x < intersect
				++cn;   // a valid crossing of y=P.y right of P.x
		}
	}
	return cn%2;
};


void computeObjectRepulsionVector(Point p, Vector normalVector, double distance, double alfa, Vector* repulsionVector)
{
	ParametricLine repulsionLine;
	Vector temp;
	double b,c, denominatore, delta,s1,s2;
	copyVector(&normalVector,&repulsionLine.directionVector);
	copyVector(&p,&repulsionLine.point);
	
	denominatore=(repulsionLine.point.x*repulsionLine.point.x + repulsionLine.point.y*repulsionLine.point.y + repulsionLine.point.z*repulsionLine.point.z);
	
	b=2*(repulsionLine.point.x*repulsionLine.directionVector.x + repulsionLine.point.y*repulsionLine.directionVector.y + repulsionLine.point.z*repulsionLine.directionVector.z)/denominatore;
	c=(repulsionLine.point.x*repulsionLine.point.x + repulsionLine.point.y*repulsionLine.point.y + repulsionLine.point.z*repulsionLine.point.z - distance*distance)/denominatore;
	delta=b*b-4*c;
	if(delta>0)
	{
		//2° degree equation solver
		s1=(-1*b-sqrt(delta))/2;
		s2=(-1*b+sqrt(delta))/2;
		

		// choose one of the follow solutions

		// s1 solution
		multiply(&repulsionLine.directionVector,s1,&temp);
		addVector(&repulsionLine.point,&temp,repulsionVector);
		multiply(repulsionVector,alfa,repulsionVector);

		// s2 solution
		multiply(&repulsionLine.directionVector,s2,&temp);
		addVector(&repulsionLine.point,&temp,repulsionVector);
		multiply(repulsionVector,alfa,repulsionVector);
	}
}