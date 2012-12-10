// management of output "channels" (file/video/cache/log....and so on)
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "DataSaving.h"	

// salvataggio dei dati su file
// #include "3dview.h" // management video output (far future)
// #include "Log.h" // logging every "action"

void _Output(InfoCache *cache);
void _EnableChannelData(CHANNELTYPE channelActive, ENABLEDISABLED ea);
void writeData();
void deleteData();
void closeData();

#endif