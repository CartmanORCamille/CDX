#include "D3DMain.h"
#include "ClientLogic.h"

int CdxRender(CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	const float arrfRGBA[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	HRESULT hrRet = E_FAIL;

	P1_ExternCRender(ptCdx, NULL);
	g_dwTotalTick = GetTickCount();
	nResult = C_TRUE;
Exit0:
	return nResult;
}

/*
pkg

	ptCdx->pD3dContext->lpVtbl->ClearRenderTargetView(
		ptCdx->pD3dContext,
		ptCdx->pRenderTargetView,
		arrfRGBA
	);
	ptCdx->pD3dContext->lpVtbl->ClearDepthStencilView(
		ptCdx->pD3dContext,
		ptCdx->pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.f,
		0
	);

	hrRet = ptCdx->pDxgiSwapChain->lpVtbl->Present(ptCdx->pDxgiSwapChain, 0, 0);
	TH_CHECKERR_FAILED(hrRet);
*/
