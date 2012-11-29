#include "Output.h"

//initialization, at the moment "output will drive DataSaving only
void _Output(InfoDataCache *cache)
{
	printf("Output initialization...\n");
	_DataSaving(cache);
}

void _EnableChannelData(CHANNELTYPE channelActive, ENABLEDISABLED ea
{
	_EnableChannel(channelActive, ea);
}
void writeData(int frame,Channel *channels)
{
	_write(frame,channels);
}

void deleteData()
{
	_delete();
}

void closeData()
{
	_close();
}