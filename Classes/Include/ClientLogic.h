#ifndef __CLIENTLOGIC_H__
#define __CLIENTLOGIC_H__

#include "D3DMain.h"

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

	void(*Reset)(struct _tClientTimer* ptInstance);
	void(*Record)(struct _tClientTimer* ptInstance);
	void(*Start)(struct _tClientTimer* ptInstance);
	void(*Stop)(struct _tClientTimer* ptInstance);
	const float(*GetTotalTime)(struct _tClientTimer* ptInstance);
	
} CLIENTTIMER, *LPCLIENTTIMER;

void SetKeyActiveMap(unsigned long ulActiveStauts, unsigned int unNegation);
void GetKeyActiveMap();
const int GetNumBinaryBits(unsigned long lNum);
const float GetClientFPS(LPCLIENTTIMER ptTimer);

void ClientTimerInit(LPCLIENTTIMER* pptInstance);
void ClientTimerReset(LPCLIENTTIMER ptTimer);
void ClientTimerRecording(LPCLIENTTIMER ptTimer);
void ClientTimerStart(LPCLIENTTIMER ptTimer);
void ClientTimerStop(LPCLIENTTIMER ptTimer);
const float GetClientTimerTotalTime(LPCLIENTTIMER ptTimer);

void thLogA(const CHAR* cszpText, ...);
#endif
