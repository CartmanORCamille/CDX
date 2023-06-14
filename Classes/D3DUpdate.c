#include "D3DMain.h"


int CdxUpdate(CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	
	P1_ExternCUpdate(NULL, ptCdx);

	nResult = C_TRUE;
Exit0:
	return nResult;
}