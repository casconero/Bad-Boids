// management of boids caching
#ifndef _DATASAVING_H_
#define _DATASAVING_H_

// mayancache dll project
#include <MayaNCache.h>	// mayancache file
#include <Boid.h>
#include <Definition.h>

//#include "TextDataFile.h"	// Example: saving data to txt data file format
//#include "XMLDataFile.h"	// Example: saving data to xml data file format 

void (*initMethod)(char *particleSysName,char *fileName, CACHEFORMAT cacheFormat,int numberOfElements, unsigned int fps, double start, double end,char *extra[], int nE);
void (*enableChannelMethod)(CHANNELTYPE channelActive, ENABLEDISABLED ea);
void(*assignChannelMethod)(CHANNELTYPE channelActive, void * values);
void (*saveMethod)();
void (*closeMethod)();
void (*deleteMethod)();

void _DataSaving(InfoCache *cache);
void _EnableChannel(CHANNELTYPE channelActive, ENABLEDISABLED ea);
void _assignChannelValues(CHANNELTYPE channelActive, void * values);
void _write();
void _delete();
void _close();
#endif 