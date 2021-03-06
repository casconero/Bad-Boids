// computing "aiming" wills (outputting array of desire vectors)
#ifndef _GPU_DESIRES_H_
#define _GPU_DESIRES_H_
#include "GPU_DataStructuresOperations.cuh"
#define MAXNUMBEROFDESIRES 3 
//typedef void(* Action)(Boid *, Boid *, Vector *);
//Action *actions;

//
////	Desires rules list initialization
//inline __device__ void initDesires(int nDesires)
//{
//	int j;
//	for(j=0;j<nDesires;j++)
//	{
//		if((rParameters[j]->enabled))
//		{
//			switch(rParameters[j]->ruleName)
//			{
//			case COHESIONRULE:
//				actions[j]=cohesionRule;
//				break;
//			case ALIGNMENTRULE:
//				actions[j]=alignmentRule;
//				break;
//			case SEPARATIONRULE:
//				actions[j]=separationRule;
//				break;
//			case WANDERRULE:
//				break;
//			case FOLLOWRULE:
//				actions[j]=followRule;
//				break;
//				//future implementation
//				break;
//			default:
//				printf("error assigning rules!!\n");
//				break;
//			}
//		}
//	}
//}
//
//// Computing "desire vectors" command
//inline __device__  void desiresComputation(Boid *boidSet, Boid * currentBoid, Boid * lBoid, Vector *desiresVector)
//{
//	int i;
//	// execute "Desires" 
//	for(i=0;i<nDesires;i++)
//	{
//		if((rParameters[i]->enabled))
//		{
//			initVector(&(desiresVector[i]));
//			actions[i](boidSet, currentBoid,&(desiresVector[i]));
//		}
//	}
//}
//
//
//
///*	Rules Implementation	*/
//
//// COHESION RULE IMPLEMENTATION
//inline __device__  void cohesionRule( Boid *nearestBoidsSet, Boid * currentBoid, Vector * steer)
//{
//	Vector tempVector;
//	int dimSet,count;
//	double pos[3];
//
//	initVector(&tempVector);
//	initVector(steer);
//	count=0;
//
//	//printf("Cohesion priority=%d\n",rParameters[COHESIONRULE]->precedence);
//	// ciclo sull'insieme dei boid che influenzano il boid in esame
//	dimSet=kd_res_size(nearestBoidsSet)-1;
//	if(dimSet>0)
//	{
//		while( !kd_res_end( nearestBoidsSet ) )
//		{
//
//			Boid *thisBoid = (Boid *)(kd_res_item( nearestBoidsSet, pos ));
//			if(thisBoid->id!=currentBoid->id)
//			{
//				if(rParameters[COHESIONRULE]->visibilityOption==TRUE)
//				{
//					if(isVisible(thisBoid, currentBoid,rParameters[COHESIONRULE]->aov)==TRUE)
//					{
//						// TARGET BOID INSIDE MAIN BOID ANGLE OF VIEWS 
//						addVector(&(thisBoid->currentPosition), &tempVector, &tempVector);
//						count++;
//					}
//				}
//				else
//				{
//					// VISIBILITY OPTION NOT ALLOWED
//					addVector(&(thisBoid->currentPosition), &tempVector, &tempVector);
//					count++;
//				}
//			}
//			kd_res_next( nearestBoidsSet );
//		}
//		if(count>0)
//		{
//			multiply(&tempVector,1.0f/count,&tempVector);
//			steering(currentBoid, &tempVector, steer);
//			multiply(steer, rParameters[COHESIONRULE]->ruleFactor,steer);
//		}
//	}
//}
//
//
//// ALIGNMENT RULE IMPLEMENTATION
//inline __device__ void alignmentRule( Boid *nearestBoidsSet, Boid * currentBoid, Vector * steer)
//{
//	int dimSet,count;
//	double pos[3];
//	initVector(steer);
//	count=0;
//	//printf("Alignment priority=%d\n",rParameters[ALIGNMENTRULE]->precedence);
//	// ciclo sull'insieme dei boid che influenzano il boid in esame
//	dimSet=kd_res_size(nearestBoidsSet)-1;
//	if(dimSet>0)
//	{
//		while( !kd_res_end( nearestBoidsSet ) )
//		{
//			Boid *thisBoid = (Boid *)(kd_res_item( nearestBoidsSet, pos ));
//			if(thisBoid->id!=currentBoid->id)
//			{
//				if(rParameters[ALIGNMENTRULE]->visibilityOption==TRUE)
//				{
//					if(isVisible(thisBoid, currentBoid,rParameters[ALIGNMENTRULE]->aov)==TRUE)
//					{
//						// TARGET BOID INSIDE MAIN BOID ANGLE OF VIEWS 
//						addVector(&(thisBoid->currentVelocity), steer, steer);
//						count++;
//					}
//				}
//				else
//				{
//					// VISIBILITY OPTION NOT ALLOWED
//					addVector(&(thisBoid->currentVelocity), steer, steer);
//					count++;
//				}
//			}
//			kd_res_next( nearestBoidsSet );
//		}
//
//		if(count>0)
//		{
//			multiply(steer,1.0f/count,steer);
//			if(magnitude(steer)>0)
//			{
//				normalize(steer);
//				multiply(steer,currentBoid->maxSpeed,steer);
//				subVector(steer,&(currentBoid->currentVelocity),steer);
//				limitation(steer,currentBoid->maxForce);
//				multiply(steer, rParameters[ALIGNMENTRULE]->ruleFactor,steer);
//			}
//		}
//	}
//}
//
//
//// SEPARATION RULE IMPLEMENTATION
//inline __device__ void separationRule(Boid *nearestBoidsSet, Boid * currentBoid, Vector * steer)
//{
//	Vector tempVector;
//	int dimSet,count;
//	double pos[3];
//
//	initVector(steer);
//	initVector(&tempVector);
//	count=0;
//	//printf("Separation priority=%d\n",rParameters[SEPARATIONRULE]->precedence);
//	// ciclo sull'insieme dei boid che influenzano il boid in esame
//	dimSet=kd_res_size(nearestBoidsSet)-1;
//	if(dimSet>0)
//	{
//		while( !kd_res_end( nearestBoidsSet ) )
//		{
//			double distance=0;
//			Boid *thisBoid = (Boid *)(kd_res_item( nearestBoidsSet, pos ));
//			if(thisBoid->id!=currentBoid->id)
//			{
//				if(rParameters[SEPARATIONRULE]->visibilityOption==TRUE)
//				{
//					if(isVisible(thisBoid, currentBoid, rParameters[SEPARATIONRULE]->aov)==TRUE)
//					{
//						subVector(&(thisBoid->currentPosition),&(currentBoid->currentPosition),&tempVector);
//						distance = magnitude(&tempVector);
//						//normalize(&tempVector);
//						multiply(&tempVector,1.0f/distance,&tempVector);
//						subVector(steer,&tempVector,steer);
//						count++;
//					}
//				}
//				else
//				{
//					// VISIBILITY OPTION NOT ALLOWED
//					subVector(&(thisBoid->currentPosition),&(currentBoid->currentPosition),&tempVector);
//					distance = magnitude(&tempVector);
//					//normalize(&tempVector);
//					multiply(&tempVector,1.0f/distance,&tempVector);
//					subVector(steer,&tempVector,steer);
//					count++;
//				}
//			}
//			kd_res_next( nearestBoidsSet );
//		}
//
//		if(count>0)
//		{
//			multiply(steer,1.0f/count,steer);
//			if(magnitude(steer)>0)
//			{	
//				normalize(steer);
//				multiply(steer,currentBoid->maxSpeed,steer);
//				subVector(steer,&(currentBoid->currentVelocity),steer);
//				limitation(steer,currentBoid->maxForce);
//				multiply(steer, rParameters[SEPARATIONRULE]->ruleFactor,steer);
//			}
//		}
//	}
//}
//
//// Extended Rules
//
//// Follow
//inline __device__  void followRule(Boid *nearestBoidsSet, Boid * currentBoid, Vector * outputVector)
//{
//	initVector(outputVector);
//	if(leaderBoid!=NULL)
//	{
//		steering(currentBoid, &leaderBoid->currentPosition, outputVector);
//		multiply(outputVector, rParameters[FOLLOWRULE]->ruleFactor,outputVector);
//	}
//}
//
//// Wander
//void wanderRule(Kdres *nearestBoidsSet, Boid * currentBoid, Vector * steer)
//{
//	Vector circle,circleOffset;
//	double wanderR,wanderD,change,value;
//
//
//	wanderR=16;
//	wanderD=60;
//	change=0.25;
//
//	// limiting random value in [-value/2][value/2] range
//	value = change*rand();
//	value /= (RAND_MAX+1);
//	value -= change/2;
//	currentBoid->wanderTheta+=value;
//
//	copyVector(&currentBoid->currentVelocity,&circle);
//	normalize(&circle);
//	multiply(&circle,wanderD,&circle);
//	addVector(&circle,&currentBoid->currentPosition,&circle);
//
//	//always "wander" on the xy plane
//	// to do, changing the "wander" plane 
//	circleOffset.x=wanderR*cos(currentBoid->wanderTheta);
//	circleOffset.y=wanderR*sin(currentBoid->wanderTheta);
//	circleOffset.z=0;
//
//	addVector(&circleOffset,&circle,&circle);
//	steering(currentBoid,&circle,steer);
//}
//
//
//
///************************************************************
//*				General Functions implementation			*
//************************************************************/
//
//// checking visibility option
//BOOL isVisible(Boid *currentBoid, Boid * nearBoid, double aov)
//{
//	Vector vectorToObject;
//	BOOL visible=FALSE;
//	subVector(&(nearBoid->currentPosition),&(currentBoid->currentPosition),&vectorToObject);
//
//	// This isn't perfectly accurate, since we're not always facing in
//	// the direction of our velocity, but it's close enough.
//	if(angleBetween(&(currentBoid->currentVelocity),&vectorToObject)<=aov)
//		visible=TRUE;
//
//	return visible;
//}
//
//// steering implementation 
//void steering(Boid * currentBoid, Vector *target, Vector *steer)
//{
//	Vector desired;
//	double desiredMagnitude;
//
//	initVector(steer);
//	subVector(target,&(currentBoid->currentPosition),&desired);
//	desiredMagnitude=magnitude(&desired);
//
//	if(desiredMagnitude>0)
//	{
//		normalize(&desired);
//		//max speed option
//		multiply(&desired,currentBoid->maxSpeed,&desired);
//		subVector(&desired,&(currentBoid->currentVelocity) ,steer);
//		limitation(steer,currentBoid->maxForce);
//	}	
//}

#endif