#include "simCmd.h"

// DLL includes
#include "../MayaBoids/Boids.h"


// static attributes recall
// MCallbackId simulateBoids::idcb;

// creating threads tasks
void ThreadsCreator(void *data, MThreadRootTask *root);

// threaded progress bar function
MThreadRetVal tProgressBar(void *data);

// threaded dll simulation
MThreadRetVal tBoidsDll(void *data);

//int cmdResult;

typedef struct dllData
{
	int result;
}DLLData;

void * simulateBoids::creator() { return new simulateBoids; }

bool simulateBoids::isUndoable() const {return false;}

/*
// TO DO
MSyntax simulateBoids::newSyntax()
{
	MSyntax syntax;
	// syntax.addFlag(kFlagDist, kFlagDistLong, MSyntax::kDouble);
	return syntax;
}
*/


MStatus simulateBoids::doIt( const MArgList& args )
{
	
	//	Description: implements the MEL boids command
	//	Arguments: args - the argument list that was passes to the command from MEL

	MStatus status = MS::kSuccess;

	
    /****************************************
	*	building thread/dll data structure  *
	****************************************/

	InfoCache infoCache;
	SimulationParameters simParams;
	RulesParameters *applyingRules;
	double progressBar=0;
	double aov=pi/3;
	int i,numberOfDesires=0;
	
	// params retrievement
	MSelectionList sel;
	MObject node;
	MFnDependencyNode nodeFn;
	MFnTransform locatorFn;
	MPlug plug;

	// simulation params
	int simulationLengthValue;		// [int] in seconds
	int framesPerSecondValue;		// [int]
	int startFrameValue;			// [int]
	int boidsNumberValue;			// [int]
	// export params
	MString logFilePathValue;		// [char *]
	MString logFileNameValue;		// [char *]
	int logFileTypeValue;			// 0 = nCache; 1 = log file; 2 = XML; 
	// locomotion params
	int locomotionModeValue;		// [int]
	double maxSpeedValue;			// [double]
	double maxForceValue;			// [double]

	// double mass=1;				// [double]

	MTime currentTime, maxTime;
	MPlug plugX, plugY, plugZ;
	double tx, ty, tz;
	int frameLength ;
	Vector * leader = NULL;

	MStatus leaderFound=MStatus::kFailure;

	MGlobal::getActiveSelectionList(sel);
	for ( MItSelectionList listIter(sel); !listIter.isDone(); listIter.next() )
	{
		listIter.getDependNode(node);
		switch(node.apiType())
		{
			case MFn::kTransform:
				// get locator transform to follow
				leaderFound=locatorFn.setObject(node);
				cout << locatorFn.name().asChar() << " is selected as locator" << endl;
				break;
			case MFn::kPluginDependNode:
				nodeFn.setObject(node);
				cout << nodeFn.name().asChar() << " is selected as brain" << endl;
				break;
			default:
				break;
		}
		cout<< node.apiTypeStr()<<endl;
	}

	// rules params
	setRuleVariables(alignment);
	setRuleVariables(cohesion);
	setRuleVariables(separation);
	setRuleVariables(follow);

	getPlugValue(simulationLength);
	getPlugValue(framesPerSecond);
	getPlugValue(startFrame);
	getPlugValue(boidsNumber);
	getPlugValue(logFileType);
	getRulePlugValue(alignment);
	getRulePlugValue(cohesion);
	getRulePlugValue(separation);
	getRulePlugValue(follow);
	getPlugValue(locomotionMode);
	getPlugValue(maxSpeed);
	getPlugValue(maxForce);
	getTypePlugValue(logFilePath);
	getTypePlugValue(logFileName);
		// counting active rules number
	if(alignmentActiveValue)
		numberOfDesires++;
	if(cohesionActiveValue)
		numberOfDesires++;
	if(separationActiveValue)
		numberOfDesires++;
	if(followActiveValue)
		numberOfDesires++;


	currentTime = MTime((double)startFrameValue);														// MAnimControl::minTime();
	maxTime = MTime((double)(startFrameValue + (simulationLengthValue * framesPerSecondValue)));		// MAnimControl::maxTime();
	cout << "time unit enum (6 is 24 fps): " << currentTime.unit() << endl;	
	plugX = locatorFn.findPlug( MString( "translateX" ), &status );
	plugY = locatorFn.findPlug( MString( "translateY" ), &status );
	plugZ = locatorFn.findPlug( MString( "translateZ" ), &status );
	frameLength = simulationLengthValue * framesPerSecondValue;
	
	if(leaderFound==MS::kSuccess)
	{
		leader = new Vector[frameLength];	
		while ( currentTime < maxTime )
		{
			{
				int index = (int)currentTime.value() - startFrameValue;
				/*
				MGlobal::viewFrame(currentTime);
				pos = locatorFn.getTranslation(MSpace::kWorld);
				cout << "pos: " << pos.x << " " << pos.y << " " << pos.z << endl;
				*/
				status = plugX.getValue( tx, MDGContext(currentTime) );
				status = plugY.getValue( ty, MDGContext(currentTime) );
				status = plugZ.getValue( tz, MDGContext(currentTime) );

				leader[index].x = tx;
				leader[index].y = ty;
				leader[index].z = tz;
				//cout << "pos at time " << currentTime.value() << " has x: " << tx << " y: " << ty << " z: " << tz << endl;
				currentTime++;
			}	
		}
	}

	simParams.fps=framesPerSecondValue;
	simParams.lenght=simulationLengthValue;
	simParams.numberOfBoids=boidsNumberValue;
	simParams.maxAcceleration=maxForceValue;
	simParams.maxVelocity=maxSpeedValue;
	simParams.simplifiedLocomotion=TRUE;


	
	
	applyingRules=new RulesParameters[numberOfDesires];
	

	// cache settings
	MString saveString;
	saveString = logFilePathValue+"/"+logFileNameValue;
	infoCache.fileName=new char[saveString.length()+1];
	memcpy(infoCache.fileName,saveString.asChar(),sizeof(char)*(saveString.length()+1));
	infoCache.cacheFormat=ONEFILE;
	infoCache.fps=framesPerSecondValue;
	infoCache.start=startFrameValue/framesPerSecondValue;
	infoCache.end=simulationLengthValue+infoCache.start;	
	infoCache.loging=FALSE;
	infoCache.option=POSITIONVELOCITY;
	infoCache.particleSysName="BoidsNParticles";
	infoCache.saveMethod=MAYANCACHE;

	for(i=0;i<numberOfDesires;i++)
	{
		applyingRules[i].enabled=TRUE;
		applyingRules[i].precedence=1;
		applyingRules[i].aov=aov;
		applyingRules[i].visibilityOption=FALSE;
	}	

	if(cohesionActiveValue==0)
		applyingRules[COHESIONRULE].enabled=FALSE;
	else
	{
		applyingRules[COHESIONRULE].ruleName=COHESIONRULE;
		applyingRules[COHESIONRULE].ruleFactor=cohesionFactorValue;
		applyingRules[COHESIONRULE].ruleRadius=cohesionRadiusValue;
		applyingRules[COHESIONRULE].ruleWeight=cohesionWeightValue;
	}

	if(separationActiveValue==0)
		applyingRules[SEPARATIONRULE].enabled=FALSE;
	else
	{
		applyingRules[SEPARATIONRULE].ruleName=SEPARATIONRULE;
		applyingRules[SEPARATIONRULE].ruleFactor=separationFactorValue;
		applyingRules[SEPARATIONRULE].ruleRadius=separationRadiusValue;
		applyingRules[SEPARATIONRULE].ruleWeight=separationWeightValue;
	}

	if(alignmentActiveValue==0)
		applyingRules[ALIGNMENTRULE].enabled=FALSE;
	else
	{
		applyingRules[ALIGNMENTRULE].ruleName=ALIGNMENTRULE;
		applyingRules[ALIGNMENTRULE].ruleFactor=alignmentFactorValue;
		applyingRules[ALIGNMENTRULE].ruleRadius=alignmentRadiusValue;
		applyingRules[ALIGNMENTRULE].ruleWeight=alignmentWeightValue;
	}
	
	if(followActiveValue==0)
		applyingRules[FOLLOWRULE].enabled=FALSE;
	else
	{
		applyingRules[FOLLOWRULE].ruleName=FOLLOWRULE;
		applyingRules[FOLLOWRULE].ruleRadius=followRadiusValue;
		applyingRules[FOLLOWRULE].ruleFactor=followFactorValue;
		applyingRules[FOLLOWRULE].ruleWeight=followWeightValue;
	}
	

	// initializing simulation parameters
	boidInit(numberOfDesires, applyingRules, simParams , infoCache, leader);

	DLLData datadll;
	// preparing threads pool
	status = MThreadPool::init();
	
	if (status==MStatus::kSuccess)
	{
		MThreadPool::newParallelRegion(ThreadsCreator, &datadll);
		setResult( "Command executed!\n" );
		CHECK_MSTATUS(MProgressWindow::endProgress());
		MThreadPool::release();
	}

	switch(datadll.result)
	{
	case 0:
		status=MS::kSuccess;
		break;

	default:
		status=MS::kFailure;
	}

	MThreadPool::release();
	return status;
}


void ThreadsCreator(void *data, MThreadRootTask *root)
{
	MThreadPool::createTask(tProgressBar, NULL, root);
	MThreadPool::createTask(tBoidsDll, data, root);
	MThreadPool::executeAndJoin(root);
}

MThreadRetVal tProgressBar(void *data)
{
	MStatus status=MS::kSuccess;

	if(!MProgressWindow::reserve())
	{
		MGlobal::displayError("Progress window already in use");
		stopSim();
		return (MThreadRetVal)-1;
	}

	// Set up and print progress window state
	MProgressWindow::setProgressRange(PROGRESSBARMINVALUE, PROGRESSBARMAXVALUE);
	MProgressWindow::setTitle("Boids Progress");
	MProgressWindow::setInterruptable(true);
	MProgressWindow::setProgressStatus("Simulation Progress");
	MProgressWindow::setProgress(PROGRESSBARMINVALUE);
	MProgressWindow::startProgress();

	while((MProgressWindow::progress()<PROGRESSBARMAXVALUE)&&(!MProgressWindow::isCancelled()))
	{	
		int prog=getProgression();
		int progV=MProgressWindow::progress();
		while(prog>=progV+PROGRESSBARADVANCEVALUE)
		{
			MProgressWindow::advanceProgress(PROGRESSBARADVANCEVALUE);	
			progV+=PROGRESSBARADVANCEVALUE;
		}
		if(progV>=PROGRESSBARMAXVALUE)
			MProgressWindow::setProgress(PROGRESSBARMAXVALUE);	
		
		Sleep(PROGRESSBARSLEEPTIME);
	}

	if (MProgressWindow::isCancelled()) 
	{
		MGlobal::displayInfo("Progress interrupted!");
		stopSim();
	}
	
	return (MThreadRetVal)0;
}

// calling the boids simulation function
MThreadRetVal tBoidsDll(void *data)
{
	DLLData * d=(DLLData*)data;
	d->result=startSim();
	return (MThreadRetVal)0;
}