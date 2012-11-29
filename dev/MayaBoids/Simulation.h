// management of simulation's steps and output 

#ifndef _SIMULATION_H_
#define _SIMULATION_H_
#include "CpuRules.h"
#include "Decisions.h"
#include "Desires.h"
#include "Locomotion.h"
#include <Definition.h>
#include "Output.h"

SimulationParameters simParameters;
int simulationProgress;
double dt;
Boid *boidSet;
Boid *leader;
Vector *targetPoint;
Kdtree * k3;

BOOL abortSimulation;
InfoCache cacheFileOption;

float *position,*velocity;
double *id, *birthtime,*lifespanPP,*finalLifespanPP;
double count;


//RulesParameters *appgRules;

void initSim(SimulationParameters *simParams, RulesParameters *applyingRules,InfoCache *cache, int numberOfDesires,Vector *leaderPosition);

int update();

#endif

