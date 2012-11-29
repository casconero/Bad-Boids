#ifndef _RULESETUP_H_
#define _RULESETUP_H_

#include "Boolean.h"
#include "..\Mathematics\DataStructures.h"

#define PRECEDENCE int
#define OKCUDA 0
#define NO_CUDA_DEVICE -100
#define CUDA_DEVICE_NOT_SUPPORTED -99
#define VISIBILITY BOOL
#define VISIBILITYON TRUE
#define VISIBILITYOFF FALSE

#define RULE int
#define RULESAVAILABLE 6
#define COHESIONRULE 0			// COHESION RULE (BASIC BOID BEHAVIOUR)
#define ALIGNMENTRULE 1			// ALLIGNEMENT RULE (BASIC BOID BEHAVIOUR)
#define SEPARATIONRULE 2		// SEPARATION RULE (BASIC BOID BEHAVIOUR)
#define REACHRULE 3				// REACH TARGET RULE (BOID MOVES TO TARGET)
#define FOLLOWRULE 4			// FOLLOW LEADER RULE (BOID FOLLOWS A LEADER)
#define WANDERRULE 5			// WANDERER RULE ("DESCRIPTION"?)

#define USEGPU int
#define GPU 1
#define CPU 2

typedef struct rulesParameters
{
	BOOL enabled;
	RULE ruleName;
	PRECEDENCE precedence;
	VISIBILITY visibilityOption;
	double aov;						//angle of view
	double ruleRadius;
	double ruleFactor;
	double ruleWeight;
}RulesParameters;


typedef struct simulationParameters
{
	int fps;
	double lenght;
	unsigned long int numberOfBoids;
	double maxVelocity;
	double maxAcceleration;
	BOOL simplifiedLocomotion;
	USEGPU master;
}SimulationParameters;

typedef struct particlesParameters
{
	Vector position;
	Vector velocity;
	double mass;
	double maxSpeed;
	double maxAcceleration;
	double maxForce;
}ParticlesParameters;


RulesParameters **rParameters;
int nDesires;


#endif