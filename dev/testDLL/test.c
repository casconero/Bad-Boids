#include <stdio.h>
#include "../MayaBoids/RulesSetup.h"
#include "../MayaBoids/Definition.h"
#include "../MayaBoids/Boids.h"
#include <time.h>
#include "../CudaBoidsCompute/cforcuda.h"

void main()
{
	time_t tStart, tStop;
	double datetime_diff_ms;
	int cudaDeviceError;
	char answer;
	Boid *boidS;
	InfoCache infoCache;
	SimulationParameters simParams;
	RulesParameters *applyingRules;
	double *progressBar=NULL;
	double aov=pi/3;
	int i,numberOfDesires=3;	
	
	// parametri della simulazione
	simParams.fps=24;
	simParams.lenght=10;
	simParams.numberOfBoids=1000;
	simParams.maxAcceleration=0.1;
	simParams.maxVelocity=4;
	simParams.simplifiedLocomotion=TRUE;
	simParams.master=GPU;

	applyingRules=(RulesParameters *)malloc(numberOfDesires*sizeof(RulesParameters));
	
	for(i=0;i<numberOfDesires;i++)
	{
		applyingRules[i].enabled=TRUE;
		applyingRules[i].precedence=9-i;
		applyingRules[i].ruleWeight=1;
		applyingRules[i].aov=aov;
		applyingRules[i].visibilityOption=FALSE;
	}

	applyingRules[COHESIONRULE].ruleName=COHESIONRULE;
	applyingRules[COHESIONRULE].ruleRadius=10;
	applyingRules[COHESIONRULE].ruleFactor=1;

	applyingRules[ALIGNMENTRULE].ruleName=ALIGNMENTRULE;
	applyingRules[ALIGNMENTRULE].ruleRadius=7;
	applyingRules[ALIGNMENTRULE].ruleFactor=1;

	applyingRules[SEPARATIONRULE].ruleName=SEPARATIONRULE;
	applyingRules[SEPARATIONRULE].ruleRadius=5;
	applyingRules[SEPARATIONRULE].ruleFactor=1.2;

	//applyingRules[FOLLOWRULE].ruleName=FOLLOWRULE;
	//applyingRules[FOLLOWRULE].ruleRadius=1;
	//applyingRules[FOLLOWRULE].ruleFactor=1;
	
	// DISABLING FOLLOW RULE
	//applyingRules[FOLLOWRULE].enabled=FALSE;

	infoCache.cacheFormat=ONEFILE;
	infoCache.start=0;
	infoCache.end=infoCache.start + (long )simParams.lenght;
	infoCache.fileName="c:\\temp\\BdirectCacheSimulationFile";
	infoCache.fps=simParams.fps;
	infoCache.loging=FALSE;
	infoCache.option=POSITIONVELOCITY;
	infoCache.particleSysName="BoidsNParticles";
	infoCache.saveMethod=MAYANCACHE;
	
	cudaDeviceError=cudaInit(&simParams,applyingRules,&infoCache,numberOfDesires);
	if (cudaDeviceError!=OKCUDA)
	{		
		if (cudaDeviceError==NO_CUDA_DEVICE)
			printf("No Cuda Device Found.\n");
		else
			printf("Cuda Device not supported.\n");
		
		printf("Proceding using default non parallel simulation? [Y]/[N].\n");
		answer=getchar();
		if (answer=='Y' || answer=='y')
		{
			printf("standard simulation\n");
			boidInit(numberOfDesires, applyingRules, simParams , infoCache, NULL);
			tStart = time(0);
			startSim();
			tStop = time(0);
		}
		else
			printf("press a key to exit simulation\n");
	}
	else
	{
		printf("Simulation started with gpu acceleration\n");
		tStart = time(NULL);
		cudaStartSim();
		tStop = time(NULL);
	}
	datetime_diff_ms = difftime(tStop, tStart) * 1000.;
	printf("Elaboration time %d[mS]\n ",datetime_diff_ms);
	getchar();
}