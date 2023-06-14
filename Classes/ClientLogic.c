#include "ClientLogic.h"


#define KEYACTIVEMAP_SIZE 2
byte g_bKeyActiveMap[KEYACTIVEMAP_SIZE] = { 0 };

// bitmap test
void SetKeyActiveMap(unsigned long ulActiveStauts, unsigned int unNegation)
{
	const int cnStatusBits = GetNumBinaryBits(ulActiveStauts);
	const int cnArrPos = cnStatusBits / 8;
	const int cnBitPos = cnStatusBits % 8;

	if (KEYACTIVE == unNegation)
	{
		g_bKeyActiveMap[cnArrPos] |= (1 << cnBitPos);
		THLOG("key active. now map: %d\n", g_bKeyActiveMap[cnArrPos]);
	}
	else if (KEYSHUT == unNegation)
	{
		g_bKeyActiveMap[cnArrPos] &= (~(1 << cnBitPos));
		THLOG("key shut. now map: %d\n", g_bKeyActiveMap[cnArrPos]);
	}
	
}

void GetKeyActiveMap()
{
	byte bTmp = 0;
	for (int i = 0; i < KEYACTIVEMAP_SIZE; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			bTmp = g_bKeyActiveMap[i] << j;

		}
		THLOG("arr %d, bit: %d\n", i, g_bKeyActiveMap[i]);
	}
}

const int GetNumBinaryBits(unsigned long ulNum)
{
	int nRet = 0;
	while (0 != ulNum)
	{
		ulNum >>= 1;
		nRet++;
	}

	return nRet;
}

const float GetClientFPS(LPCLIENTTIMER ptTimer)
{
	float fFPS = 0.f;
	float fFpsMs = 0.f;
	static int nFrameCnt = 0;
	static float fTimeElapsed = 0.f;

	nFrameCnt++;
	if (1.f <= ptTimer->GetTotalTime(ptTimer) - fTimeElapsed)
	{
		// 1s.
		fFPS = (float)nFrameCnt;
		fFpsMs = 1000.f / fFPS;
		nFrameCnt = 0;
		fTimeElapsed += 1.f;
	}

	return fFPS;
}

void ClientTimerInit(LPCLIENTTIMER* pptInstance)
{
	unsigned __int64 llCntPerSec = 0;
	BOOL bFnRet = FALSE;
	LPCLIENTTIMER ptInstance = (LPCLIENTTIMER)malloc(sizeof(CLIENTTIMER));
	TH_CHECKERR_FALSE(ptInstance);
	
	ptInstance->dSecondsPerCnt = 0.f;
	ptInstance->dDeltaTime = -1.0f;
	ptInstance->llBaseTime = 0;
	ptInstance->llPausedTime = 0;
	ptInstance->llPrevTime = 0;
	ptInstance->llCurrTime = 0;
	ptInstance->llStopTime = 0;
	ptInstance->nStopped = C_FALSE;
	ptInstance->Record = ClientTimerRecording;
	ptInstance->Reset = ClientTimerReset;
	ptInstance->Start = ClientTimerStart;
	ptInstance->Stop = ClientTimerStop;
	ptInstance->GetTotalTime = GetClientTimerTotalTime;

	bFnRet = QueryPerformanceFrequency((LARGE_INTEGER*)&llCntPerSec);
	TH_CHECKERR_FALSE(bFnRet);
	ptInstance->dSecondsPerCnt = 1.f / (double)llCntPerSec;

	*pptInstance = ptInstance;

Exit0:
	return;
}

void ClientTimerReset(LPCLIENTTIMER ptTimer)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&(ptTimer->llCurrTime));
	ptTimer->llBaseTime = ptTimer->llCurrTime;
	ptTimer->llPrevTime = ptTimer->llCurrTime;
	ptTimer->llStopTime = 0;
	ptTimer->nStopped = C_FALSE;
}

void ClientTimerRecording(LPCLIENTTIMER ptTimer)
{
	if (C_TRUE == ptTimer->nStopped)
	{
		ptTimer->dDeltaTime = 0.f;
		goto Exit0;
	}

	__int64 llCurrTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&llCurrTime);

	ptTimer->llCurrTime = llCurrTime;
	ptTimer->dDeltaTime = (ptTimer->llCurrTime - ptTimer->llPrevTime) * ptTimer->dSecondsPerCnt;
	ptTimer->llPrevTime = ptTimer->llCurrTime;
	if (0.f > ptTimer->dDeltaTime)
	{
		ptTimer->dDeltaTime = 0.f;
	}

Exit0:
	return;
}

void ClientTimerStart(LPCLIENTTIMER ptTimer)
{
	__int64 llStartTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&llStartTime);

	if (C_TRUE == ptTimer->nStopped)
	{
		ptTimer->llPausedTime += (llStartTime - ptTimer->llStopTime);
		ptTimer->llPrevTime = llStartTime;
		ptTimer->llStopTime = 0;
		ptTimer->nStopped = C_FALSE;
	}

	return;
}

void ClientTimerStop(LPCLIENTTIMER ptTimer)
{
	if (C_TRUE == ptTimer->nStopped)
	{
		goto Exit0;
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&(ptTimer->llCurrTime));
	ptTimer->llStopTime = ptTimer->llCurrTime;
	ptTimer->nStopped = C_TRUE;

Exit0:
	return;
}

const float GetClientTimerTotalTime(LPCLIENTTIMER ptTimer)
{
	float fTotalTime = 0.f;
	if (C_TRUE == ptTimer->nStopped)
	{
		fTotalTime = (float)(((ptTimer->llStopTime - ptTimer->llPausedTime) - ptTimer->llBaseTime) * ptTimer->dSecondsPerCnt);
	}
	else
	{
		fTotalTime = (float)(((ptTimer->llCurrTime - ptTimer->llPausedTime) - ptTimer->llBaseTime) * ptTimer->dSecondsPerCnt);
	}

Exit0:
	return fTotalTime;
}

VOID thLogA(const CHAR* cszpText, ...)
{
	CHAR szarrText[1024] = { 0 };
	va_list vlArgs;

	va_start(vlArgs, cszpText);
	_vsnprintf_s(szarrText, 1024, 1024, cszpText, vlArgs);
	va_end(vlArgs);
	OutputDebugStringA(szarrText);

	return;
}
