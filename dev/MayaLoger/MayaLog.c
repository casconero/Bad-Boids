#include "MayaLog.h"


void logInitialization(char *logName)
{
	// adding extension ".log"
	logFileName = (char*)malloc(sizeof(char)*(strlen(logName)+5));
	strcpy(logName, logFileName);
	strcat(logFileName, ".log");

	mayaLogFile=fopen(logFileName, "w");

	if(mayaLogFile==NULL)
		printf("Impossibile aprire il file di log %s in scrittura\n",logFileName);
	else
		printf("Il file %s e' aperto in  scrittura\n",logFileName);
	setvbuf(mayaLogFile, mayaLogChannelBuffer, _IOFBF, BLENGTH);
}


void closeLog()
{
	printf("closing log file \n");

	if(mayaLogFile!=NULL)
	{
		fflush(mayaLogFile);
		printf("File log '%s' was successfully closed and saved\n", logFileName);
		fclose(mayaLogFile);
	}
	else
		printf("Maya log file '%s' not saved!\n", logFileName);
}