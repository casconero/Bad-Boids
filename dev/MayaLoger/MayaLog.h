// management of output "channels" (file/video/cache/log....and so on)
#ifndef _MayaLog_H_
#define _MayaLog_H_
#include <time.h>


#define BLENGTH 65536

char *logFileName;
FILE *mayaLogFile;
char mayaLogChannelBuffer[BLENGTH];

void logInitialization(char *logName);
void writeLog();
void closeLog();

#endif