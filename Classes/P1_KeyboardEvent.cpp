#include "P1_Vertex.h"

#define CAMERA_POSITION_UNIT 0.005

void P1_KChangeRenderMethodWR(CDXDISPATCH_PTR ptCdx, void* vpIsKeyDown)
{
	ptCdx->pD3dContext->RSSetState(g_p1DemoPtr->pWirerameRS);
}

void P1_KChangeRenderMethodSO(CDXDISPATCH_PTR ptCdx, void* vpIsKeyDown)
{
	ptCdx->pD3dContext->RSSetState(g_p1DemoPtr->pSoildRS);
}

void P1_KChangeCameraUp(CDXDISPATCH_PTR ptCdx, void* vpIsKeyDown)
{
	g_p1DemoPtr->tCameraPositionArgs.fY += CAMERA_POSITION_UNIT;
}

void P1_KChangeCameraDown(CDXDISPATCH_PTR ptCdx, void* vpIsKeyDown)
{
	g_p1DemoPtr->tCameraPositionArgs.fY -= CAMERA_POSITION_UNIT;
}

void P1_KChangeCameraLeft(CDXDISPATCH_PTR ptCdx, void* vpIsKeyDown)
{
	g_p1DemoPtr->tCameraPositionArgs.fX -= CAMERA_POSITION_UNIT;
}

void P1_KChangeCameraRight(CDXDISPATCH_PTR ptCdx, void* vpIsKeyDown)
{
	g_p1DemoPtr->tCameraPositionArgs.fX += CAMERA_POSITION_UNIT;
}