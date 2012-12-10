#include "DataSaving.h"

void _DataSaving(InfoCache *cache)
{
	printf("DataSaving initialization...\n");

	switch(cache->saveMethod)
	{
	case MAYANCACHE:
		initMethod = init;
		saveMethod = mayaCache;
		assignChannelMethod = assignChannelValues;
		enableChannelMethod = enableChannel;
		closeMethod = closeMayaNCacheFile;
		deleteMethod = deleteFile;
		printf("MayaNCache library loaded\n");
		break;
	case TEXTDATA:
		/*initMethod=setFileName;
		saveMethod=printSingleBoid;
		writeHeaderMethod=printHeader;
		deleteMethod = delete;
		closeMethod=closeFile;*/
		printf("TextData library loaded\n");
		break;
	case XMLDATA:
		/*initMethod=setFileName;
		saveMethod=printSingleBoid;
		writeHeaderMethod=printHeader;
		deleteMethod = delete;
		closeMethod=closeFile;*/
		printf("XmlData library loaded\n");
		break;
	default:
		break;
	};
	// remember: Add the extra information (the "Null" and "0" options )
	initMethod(cache->particleSysName,cache->fileName,cache->cacheFormat,cache->option,cache->fps,cache->start,cache->end,NULL,0);
}

void _EnableChannel(CHANNELTYPE channelActive, ENABLEDISABLED ea){enableChannelMethod(channelActive, ea);}

void _write(){saveMethod();}

void _delete(){deleteMethod();}

void _close(){closeMethod();}