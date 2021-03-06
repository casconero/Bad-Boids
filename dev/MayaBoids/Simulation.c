#include "Simulation.h"

// functions declaration
void compute();
void updateBoids();
void sortingRules();
void boidInitialization(Boid *boid, int id,double width, double depth, double high,Vector * startingV,Vector * startingA);
void cachingData(Channel* channels);
void freeChannel(Channel* channels);
void leaderInit(double radius);

/********************************************/
void initSim2(SimulationParameters *simParams, RulesParameters *applyingRules,InfoCache *cache, int numberOfDesires,Vector *leaderPosition, Vector *target)
{
	initSim(simParams, applyingRules, cache, numberOfDesires, leaderPosition);
	copyVector(target,targetPoint);
}

void initSim(SimulationParameters *simParams, RulesParameters *applyingRules,InfoCache *cache, int numberOfDesires,Vector *leaderPosition)
{
	int j;
	unsigned long int i;
	Vector acc,vel;

	abortSimulation=FALSE;
	nDesires=numberOfDesires;
	dt=1/(double)(simParams->fps);
	simulationProgress = 0;
	leader=NULL;

	// initializing cache files and rules settings
	memcpy(&cacheFileOption,cache,sizeof(InfoCache));
	rParameters=(RulesParameters**)malloc(nDesires*sizeof(RulesParameters*));
	
	for(j=0;j<nDesires;j++)
	{
		rParameters[j]=(RulesParameters*)malloc(sizeof(RulesParameters));
		memcpy(rParameters[j],&applyingRules[j],sizeof(RulesParameters));
	}

	memcpy(&simParameters,simParams,sizeof(SimulationParameters));
	actions=(Action*)malloc(sizeof(Action)*nDesires);

	// preparing the boids set
	boidSet=(Boid *)malloc(sizeof(Boid)*simParameters.numberOfBoids);	

	// initializing boids starting position and starting velocity (only when no external boids passed to the simulation dll)
	randomVector(&acc, simParams->maxAcceleration, 0, 0);
	randomVector(&vel, simParams->maxVelocity, 0, 0);
	
	// initializing and filling the kdtree structure
	k3=kd_create(3);
	for(i=0; i<simParameters.numberOfBoids; i++)
	{
		boidInitialization(&boidSet[i],i+1, 20, 10, 5, &vel, &acc);
		boidSet[i].mass=10;
		kd_insert3(k3,boidSet[i].currentPosition.x, boidSet[i].currentPosition.y, boidSet[i].currentPosition.z, &boidSet[i]);
	}

	//initializing leader
	if(leaderPosition!=NULL)
	{
		// maya interface passed the leader to follow
		unsigned int k;
		Vector nullVector;
		unsigned int frames=(unsigned int)ceil(simParameters.fps * simParameters.lenght);

		initVector(&nullVector);
		leader=(Boid *)malloc(sizeof(Boid)*frames);

		// the leader only need the position ,so the others variables will be not setup
		for (k=0;k<frames;k++)
			initBoid(&leaderPosition[k],&nullVector,&nullVector,0,0,0,0,-1,&leader[k]);
	}
	else
		if(applyingRules[FOLLOWRULE].enabled)
		{
			//automaticly generate a leader to follow
			leader=(Boid*)malloc(sizeof(Boid)*(unsigned)(simParameters.fps*simParameters.lenght));
			leaderInit(20);
		}
	initDesires(leader);

	_Output(&cacheFileOption);

}

// update simulation
int  update()
{
	unsigned int simulationLenght,progress;
	int exitValue,i;
	
	int test=0;

	progress=0;
	simulationLenght=(unsigned int)ceil(simParameters.fps * simParameters.lenght);
	
	// enabling channels for caching
	for (i=0;i<CACHENUMBEROFCHANNELS;i++)
		if (cacheFileOption.selectedChannels[i])
			_EnableChannel(i, ENABLED);
	while((!abortSimulation) && (progress<=simulationLenght))
	{
		// compute Boids' new positions, velocities, accelerations 
		compute();

		// data management
		cachingData(NULL);

		// write data
		writeData();

		// update Boids properties and kdtree 
		updateBoids();

		// update the index job progress 
		simulationProgress = ((int)(100*progress)/simulationLenght);

		//advance to the next frame
		progress++;
		test++;
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
	kd_free(k3);

	return exitValue;
}





/********************************************/
/*											*/
/*		private functions implementation	*/
/*											*/
/********************************************/
void sortingRules()
{
	// sorting from smallest "precedence" value to the bigest one
	int i,j;
	Action temp;

	for(i=0;i<nDesires;i++)
	{
		for (j=0;j<nDesires;j++)
		{
			if(rParameters[i]->precedence<rParameters[j]->precedence)
			{
				memcpy(&temp,actions[i],sizeof(Action));			
				memcpy(actions[i],actions[j],sizeof(Action));
				memcpy(actions[j],&temp,sizeof(Action));
			}
		}
	}
}

void boidInitialization(Boid *boid, int id,double width, double depth, double high,Vector * startingV,Vector * startingA)
{
	double mass, force;
	Vector position, vVelocity, vAcceleration;

	//Vector offsetAcceleration;
	//double accelerationOffset, velocityOffset;

	////vettore accelerazione iniziale (valore base uguale x tutti + leggero offset)
	//initVector(&vAcceleration);
	//accelerationOffset = simParameters.maxAcceleration/100;
	//randomVector(&offsetAcceleration, accelerationOffset, accelerationOffset, accelerationOffset);
	//addVector(&offsetAcceleration,startingA,&vAcceleration);


	////vettore velocit� iniziale (valore base uguale x tutti + leggero offset)
	//initVector(&vVelocity);
	//velocityOffset = simParameters.maxVelocity/100;
	//randomVector(&offsetVelocity, velocityOffset, velocityOffset, velocityOffset);
	//addVector(&offsetVelocity,startingV,&vVelocity);

	mass=1;
	force=0.1;

	initVector(&vAcceleration);
	initVector(&vVelocity);

    // vettore posizione
    randomVector(&position, width, depth, high);

    initBoid(&position, &vVelocity, &vAcceleration, simParameters.maxVelocity, simParameters.maxAcceleration, force, mass, id, boid);
}

void compute()
{
	Vector *desiresVector;
	Vector decisionVector;
	unsigned long  i=0;
	desiresVector=(Vector *)malloc(sizeof(Vector)*nDesires);

	for (i=0;i<simParameters.numberOfBoids;i++)
	{	

		// execute "Desires" 
		desiresComputation(k3, &(boidSet[i]),NULL, desiresVector);

		// execute "Decision"
		decisionComputation(desiresVector, &(boidSet[i]), &decisionVector);

		// execute "Locomotion"
		if(simParameters.simplifiedLocomotion)
			locomotionComputationSimplified(&decisionVector, &(boidSet[i]), dt);
		else
			locomotionComputation(&decisionVector, &(boidSet[i]), dt);
		
		// execute "SimFinalization"
		// ending the procedure
	}
	free(desiresVector);
}

void updateBoids()
{
	unsigned long j;
	kd_clear(k3);
	for( j=0;j<simParameters.numberOfBoids;j++)
	{
		copyVector(&(boidSet[j].nextPosition), &(boidSet[j].currentPosition));
		copyVector(&(boidSet[j].nextVelocity), &(boidSet[j].currentVelocity));
		
		// kdtree update
		kd_insert3(k3,boidSet[j].currentPosition.x, boidSet[j].currentPosition.y, boidSet[j].currentPosition.z, &boidSet[j]);
	}
}



void cachingData(Channel *channels)
{
	//int i;
	unsigned long j,totalNumberOfBoids;
	
	totalNumberOfBoids = simParameters.numberOfBoids;
	
	count = (double)totalNumberOfBoids;
	id = (double*)malloc(sizeof(double)*totalNumberOfBoids);
	position = (float*)malloc(3*sizeof(float)*totalNumberOfBoids);
	velocity = (float*)malloc(3*sizeof(float)*totalNumberOfBoids);
	birthtime = (double*)calloc(totalNumberOfBoids, sizeof(double));
	lifespanPP = (double*)calloc(totalNumberOfBoids, sizeof(double));
	finalLifespanPP = (double*)calloc(totalNumberOfBoids, sizeof(double));

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

	assignChannelValues(COUNTCHANNEL, &count);
	assignChannelValues(IDCHANNEL,id);
	assignChannelValues(POSITIONCHANNEL,position);
	assignChannelValues(VELOCITYCHANNEL,velocity);
	assignChannelValues(BIRTHTIMECHANNEL,birthtime);
	assignChannelValues(LIFESPANPPCHANNEL,lifespanPP);
	assignChannelValues(FINALLIFESPANPPCHANNEL,finalLifespanPP);
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

void leaderInit(double radius)
{
	double delta,angle, stair,up;
	int j,totalLength;
	totalLength=(int)ceil(simParameters.fps*simParameters.lenght);
	up=10/totalLength;

	stair=0;
	angle=0;
	delta=(double)(360.0/totalLength);

	//leader=(Boid *)malloc((unsigned int)ceil(simParameters.fps * simParameters.lenght)* sizeof(Boid));

	for(j=0;j<totalLength;j++)
	{
		leader[j].id=-1;
		leader[j].mass=1;
		leader[j].maxForce=1;
		leader[j].maxAcceleration=1;
		leader[j].maxSpeed=1;
		leader[j].currentPosition.x = radius*sin(3.14*angle/180.0);
		leader[j].currentPosition.y = radius*cos(3.14*angle/180.0);
		leader[j].currentPosition.z = stair;
		angle+=delta;
		stair+=up;
	}
}