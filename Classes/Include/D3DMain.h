#ifndef __D3DINIT_H__
#define __D3DINIT_H__


#include "stdafx.h"
#include "CDX_D3DColor.h"
#include <d3d11_1.h>
#include <D3DX11async.h>
#include <d3dx11effect.h>


struct _tCdxDispatch
{
	float fFps;
	float fFpsMS;
	ID3D11Device* pD3d;
	ID3D11DeviceContext* pD3dContext;
	IDXGISwapChain* pDxgiSwapChain;
	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11RenderTargetView* pRenderTargetView;
};
typedef struct _tCdxDispatch CDXDISPATCH, * CDXDISPATCH_PTR;

int CdxInit(HWND hWindow, CDXDISPATCH_PTR ptCdx);
int CdxUpdate(CDXDISPATCH_PTR ptCdx);
int CdxRender(CDXDISPATCH_PTR ptCdx);

#endif
