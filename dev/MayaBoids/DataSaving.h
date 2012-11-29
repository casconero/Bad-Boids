// management of boids caching
#ifndef _DATASAVING_H_
#define _DATASAVING_H_

// mayancache dll project
#include <MayaNCache.h>	// mayancache file
//#include <XMLWriter.h>// mayancache xml file
#include <Boid.h>
#include "Definitions.h"

//#include "TextDataFile.h"	// Example: saving data to txt data file format
//#include "XMLDataFile.h"	// Example: saving data to xml data file format 

void (*initMethod)(char *particleSysName,char *fileName, CACHEFORMAT cacheFormat,int numberOfElements, unsigned int fps, double start, double end,char *extra[], int nE);
void (*enableChannelMethod)(CHANNELTYPE channelActive, ENABLEDISABLED ea);
void (*saveMethod)();
void (*closeMethod)();
void (*deleteMethod)();

void _DataSaving(InfoDataCache *cache);
void _EnableChannel(CHANNELTYPE channelActive, ENABLEDISABLED ea);
void _write(int frame, Channel * channels);
void _delete();
void _close();
#endif 