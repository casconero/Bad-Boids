#define DLL
#include "cforcuda.h"

extern void GPUCompute(Boid *boids);
extern int GPUInit(SimulationParameters simParam, RulesParameters *rulesP, int nDes);

void cachingData(Channel* channels);
void freeChannel(Channel* channels);
void cudaBoidInitialization(Boid *boid, int id,double width, double depth, double high,Vector * startingV,Vector * startingA);


/*DLL*/
/*START DEFINITION*/
int cudaInit(SimulationParameters *simParams, RulesParameters *applyingRules,InfoCache *cache, int numberOfDesires)
{
	unsigned long int i;
	Vector acc,vel;
	abortSimulation=FALSE;
	nDesires=numberOfDesires;
	dt=1/(double)(simParams->fps);
	simulationProgress = 0;
	// initializing cache files and rules settings
	memcpy(&cacheFileOption,cache,sizeof(InfoCache));
	memcpy(&simParameters,simParams,sizeof(SimulationParameters));

	// init values for simulation
	// check cuda device presence, on false exit 
	if (GPUInit(simParameters,applyingRules,numberOfDesires)== NO_CUDA_DEVICE)
		return NO_CUDA_DEVICE;
	
	// preparing the boids set
	//ndes=nDesires;
	boidSet=(Boid *)malloc(sizeof(Boid)*simParams->numberOfBoids);	
	if(boidSet!=NULL)
	{
		// initializing boids starting position and starting velocity (only when no external boids passed to the simulation dll)
		randomVector(&acc, simParams->maxAcceleration, 0, 0);
		randomVector(&vel, simParams->maxVelocity, 0, 0);
		for(i=0; i<simParams->numberOfBoids; i++)
			cudaBoidInitialization(&boidSet[i],i+1, 20, 10, 5, &vel, &acc);
	}
	else
		printf("impossibile allocare memoria");
	return OKCUDA;
}

int mycudaCompute()
{
	// ri scrivo la funzione di update presente in dll "seriale"
	// utilizzando "GPUCompute(boidSet)" come chiamata cuda anzichè la normale "compute()"

	unsigned int simulationLenght,progress;
	int exitValue;

	int test=0;

	progress=0;
	simulationLenght=(unsigned int)ceil(simParameters.fps * simParameters.lenght);

	_Output(&cacheFileOption);

	while((!abortSimulation) && (progress<=simulationLenght))
	{
		Channel *channels;

		channels=(Channel*)malloc(sizeof(Channel)*info.option);
		
		GPUCompute(boidSet);
		
		// data management
		cachingData(channels);

		// write data
		writeData(progress,channels);

		// update the index job progress 
		simulationProgress = ((int)(100*progress)/simulationLenght);

		//advance to the next frame
		progress++;
		test++;
		// free channels memory
		freeChannel(channels);
	}

	simulationProgress=100;

	closeMethod();
	if(abortSimulation)
	{
		printf("Simulation interrupted\n");
		deleteData();
		exitValue=INTERRUPTED_SIM;
	}
	// restoring abortSimulation flag
	abortSimulation=FALSE;
	exitValue=SUCCESS_SIM;

	// free resources
	free(boidSet);	
	return exitValue;
}

int cudaStartSim() { return mycudaCompute();}

int cudaGetProgression() { return simulationProgress; }

void cudaStopSim(){ abortSimulation=TRUE; }
/*DLL*/
/*CLOSE DEFINITION*/

void cudaBoidInitialization(Boid *boid, int id,double width, double depth, double high,Vector * startingV,Vector * startingA)
{
	double mass, force;
	Vector position, vVelocity, vAcceleration;
	mass=1;
	force=0.1;

	initVector(&vAcceleration);
	initVector(&vVelocity);

	// vettore posizione
	randomVector(&position, width, depth, high);

	initBoid(&position, &vVelocity, &vAcceleration, simParameters.maxVelocity, simParameters.maxAcceleration, force, mass, id, boid);
}

void cachingData(Channel *channels)
{
	int i;
	unsigned long j,totalNumberOfBoids;

	totalNumberOfBoids = simParameters.numberOfBoids;

	count = (double)totalNumberOfBoids;
	id = (double*)malloc(sizeof(double)*totalNumberOfBoids);
	position = (float*)malloc(3*sizeof(float)*totalNumberOfBoids);
	velocity = (float*)malloc(3*sizeof(float)*totalNumberOfBoids);
	birthtime = (double*)calloc(totalNumberOfBoids, sizeof(double));
	lifespanPP = (double*)calloc(totalNumberOfBoids, sizeof(double));
	finalLifespanPP = (double*)calloc(totalNumberOfBoids, sizeof(double));

	for(i=0;i<POSITIONVELOCITY;i++)
	{
		channels[i].name=cName.names[i];
		channels[i].attribute=aName.names[i];

		if(i==_COUNT)
			channels[i].numberOfElements = 1;
		else
			channels[i].numberOfElements = totalNumberOfBoids;

		switch(i)
		{
		case _COUNT:
		case _ID:
		case _BIRTHTIME:
		case _LIFESPANPP:
		case _FINALLIFESPANPP:
			channels[i].type = DBLA;
			break;
		default:
			channels[i].type = FVCA;
			break;
		};
	}

	for(j=0;j<simParameters.numberOfBoids;j++)
	{
		// id
		id[j] = boidSet[j].id;

		// position
		position[3*j]=(float)(boidSet[j].currentPosition.x);
		position[3*j+1]=(float)(boidSet[j].currentPosition.y);
		position[3*j+2]=(float)(boidSet[j].currentPosition.z);

		// velocity
		velocity[3*j]=(float)(boidSet[j].currentVelocity.x);
		velocity[3*j+1]=(float)(boidSet[j].currentVelocity.y);
		velocity[3*j+2]=(float)(boidSet[j].currentVelocity.z);
	}

	channels[_COUNT].elements = &count;
	channels[_ID].elements = id;
	channels[_VELOCITY].elements = velocity;
	channels[_POSITION].elements = position;
	channels[_BIRTHTIME].elements = birthtime;
	channels[_LIFESPANPP].elements = lifespanPP;
	channels[_FINALLIFESPANPP].elements = finalLifespanPP;
}

void freeChannel(Channel* channels)
{
	// free memory
	if(id!=NULL)
		free(id);
	if(position!=NULL)
		free(position);
	if(velocity!=NULL)
		free(velocity);
	if(birthtime!=NULL)
		free(birthtime);
	if(lifespanPP!=NULL)
		free(lifespanPP);
	if(finalLifespanPP!=NULL)
		free(finalLifespanPP);
	if(channels!=NULL)
		free(channels);
}
