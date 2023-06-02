#include "stdafx.h"
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

void ClientTimerInit(LPCLIENTTIMER* pptInstance)
{
	__int64 llCntPerSec = 0;
	BOOL bFnRet = FALSE;
	*pptInstance = (LPCLIENTTIMER)malloc(sizeof(CLIENTTIMER));
	TH_CHECKERR_FALSE(*pptInstance);
	
	(*pptInstance)->dSecondsPerCnt = 0.f;
	(*pptInstance)->dDeltaTime = -1.0f;
	(*pptInstance)->llBaseTime = 0;
	(*pptInstance)->llPausedTime = 0;
	(*pptInstance)->llPrevTime = 0;
	(*pptInstance)->llCurrTime = 0;
	(*pptInstance)->nStopped = C_FALSE;
	(*pptInstance)->Start = ClientTimerStart;
	(*pptInstance)->Reset = ClientTimerReset;
	(*pptInstance)->Stop = ClientTimerStop;

	bFnRet = QueryPerformanceFrequency((LARGE_INTEGER*)&llCntPerSec);
	TH_CHECKERR_FALSE(bFnRet);
	(*pptInstance)->dSecondsPerCnt = 1.0 / (double)llCntPerSec;

Exit0:
	return;
}

void ClientTimerReset()
{}
void ClientTimerStart()
{}
void ClientTimerStop()
{}


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
