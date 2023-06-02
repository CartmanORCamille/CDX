#ifndef __CLIENTLOGIC_H__
#define __CLIENTLOGIC_H__

typedef struct _tClientTimer
{
	double dSecondsPerCnt;
	double dDeltaTime;
	__int64 llBaseTime;
	__int64 llPausedTime;
	__int64 llStopTime;
	__int64 llPrevTime;
	__int64 llCurrTime;
	cdxBool nStopped;

	void(*init)(void** ppInstance);
	void(*Reset);
	void(*Start);
	void(*Stop);
	
} CLIENTTIMER, *LPCLIENTTIMER;

void SetKeyActiveMap(unsigned long ulActiveStauts, unsigned int unNegation);
void GetKeyActiveMap();
const int GetNumBinaryBits(long lNum);
void thLogA(const CHAR* cszpText, ...);

void ClientTimerInit(LPCLIENTTIMER* pptInstance);
void ClientTimerReset();
void ClientTimerStart();
void ClientTimerStop();

#endif
