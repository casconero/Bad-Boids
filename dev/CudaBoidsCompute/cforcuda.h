#include "..\MayaBoids\RulesSetup.h"
#include "..\MayaBoids\Output.h"
#include "..\Mathematics\Boid.h"
#include "..\Mathematics\DataStructures.h"

#include <stdio.h>

#ifdef DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

SimulationParameters simParameters;
//int ndes;
int simulationProgress;
double dt;

Boid *boidSet;

BOOL abortSimulation;
InfoCache cacheFileOption;
float *position,*velocity;
double *id, *birthtime,*lifespanPP,*finalLifespanPP;
double count;


#ifdef __cplusplus
extern "C" 
{
#endif

	// FUNZIONI ESPOSTE LATO CUDA PER LA SIMULAZIONE
	int DLL_EXPORT cudaInit(SimulationParameters *simParams, RulesParameters *applyingRules,InfoCache *cache, int numberOfDesires);

	int DLL_EXPORT cudaStartSim();

	void DLL_EXPORT cudaStopSim();

	int DLL_EXPORT cudaGetProgression();

#ifdef __cplusplus
}
#endif