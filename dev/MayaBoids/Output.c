#include "Output.h"

//initialization, at the moment "output will drive DataSaving only
void _Output(InfoCache *cache)
{
	printf("Output initialization...\n");
	_DataSaving(cache);
}

void _EnableChannelData(CHANNELTYPE channelActive, ENABLEDISABLED ea){_EnableChannel(channelActive, ea);}
void writeData(){_write();}

void deleteData(){_delete();}

void closeData(){_close();}