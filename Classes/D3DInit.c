#include "D3DMain.h"


int CdxInit(HWND hWindow, CDXDISPATCH_PTR ptCdx)
{
	int nRet							= C_FALSE;
	HRESULT hFnRet						= E_FAIL;
	D3D_FEATURE_LEVEL					emFeatureLevel;
	UINT unDeviceFlag					= 0;
	UINT unM4xMsaaQuality				= 0;
	ID3D11Device* pD3d					= NULL;
	DXGI_SWAP_CHAIN_DESC tSwapChainDesc = { 0 };
	D3D11_TEXTURE2D_DESC tZBufferDesc	= { 0 };
	D3D11_VIEWPORT tViewPort = { 0 };
	IDXGIDevice* pDxgiDevice = NULL;
	IDXGIAdapter* pDxgiAdapter = NULL;
	IDXGIFactory* pDxgiFactory = NULL;
	ID3D11Texture2D* pBackBuffterTex = NULL;
	ID3D11Texture2D* pDepthStencilBuffterTex = NULL;

	tSwapChainDesc.BufferDesc.Width						= WINDOW_MAX_WIDTH;
	tSwapChainDesc.BufferDesc.Height					= WINDOW_MAX_HIGH;
	tSwapChainDesc.BufferDesc.RefreshRate.Denominator	= 60;
	tSwapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	tSwapChainDesc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	tSwapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	tSwapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChainDesc.BufferCount							= 1;
	tSwapChainDesc.OutputWindow							= hWindow;
	tSwapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	tSwapChainDesc.Flags								= 0;
	tSwapChainDesc.Windowed								= TRUE;

	tZBufferDesc.Width = WINDOW_MAX_WIDTH;
	tZBufferDesc.Height = WINDOW_MAX_HIGH;
	tZBufferDesc.MipLevels = 1;
	tZBufferDesc.ArraySize = 1;
	tZBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tZBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tZBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tZBufferDesc.CPUAccessFlags = 0;
	tZBufferDesc.MiscFlags = 0;

	tViewPort.Width = WINDOW_MAX_WIDTH;
	tViewPort.Height = WINDOW_MAX_HIGH;
	tViewPort.TopLeftX = 0;
	tViewPort.TopLeftY = 0;
	tViewPort.MinDepth = 0.0f;
	tViewPort.MaxDepth = 1.0f;

	// create device and context.
#if defined(DEBUG) || defined(_DEBUG)
	unDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hFnRet = D3D11CreateDevice(
		NULL, D3D_DRIVER_TYPE_HARDWARE, 0, unDeviceFlag, NULL, 0,
		D3D11_SDK_VERSION, &pD3d, &emFeatureLevel, &(ptCdx->pD3dContext)
	);
	TH_CHECKERR_FAILED(hFnRet);

	// create msaa.
	hFnRet = pD3d->lpVtbl->CheckMultisampleQualityLevels(pD3d, DXGI_FORMAT_R8G8B8A8_UINT, 4, &unM4xMsaaQuality);
	ASSERT(0 < unM4xMsaaQuality);
	tSwapChainDesc.SampleDesc.Count = 4;
	tSwapChainDesc.SampleDesc.Quality = unM4xMsaaQuality - 1;
	
	// create swap chain.
	hFnRet = pD3d->lpVtbl->QueryInterface(pD3d, &IID_IDXGIDevice, (void**)&pDxgiDevice);
	TH_CHECKERR_FAILED(hFnRet);
	hFnRet = pDxgiDevice->lpVtbl->GetParent(pDxgiDevice, &IID_IDXGIAdapter, (void**)&pDxgiAdapter);
	TH_CHECKERR_FAILED(hFnRet);
	hFnRet = pDxgiAdapter->lpVtbl->GetParent(pDxgiAdapter, &IID_IDXGIFactory, (void**)&pDxgiFactory);
	TH_CHECKERR_FAILED(hFnRet);
	hFnRet = pDxgiFactory->lpVtbl->CreateSwapChain(pDxgiFactory, pD3d, &tSwapChainDesc, &(ptCdx->pDxgiSwapChain));
	TH_CHECKERR_FAILED(hFnRet);

	// create render target.
	hFnRet = ptCdx->pDxgiSwapChain->lpVtbl->GetBuffer(ptCdx->pDxgiSwapChain, 0, &IID_ID3D11Texture2D, (void**)&pBackBuffterTex);
	TH_CHECKERR_FAILED(hFnRet);
	hFnRet = pD3d->lpVtbl->CreateRenderTargetView(pD3d, pBackBuffterTex, NULL, &(ptCdx->pRenderTargetView));
	TH_CHECKERR_FAILED(hFnRet);

	// create z-buffer and depth render target.
	tZBufferDesc.SampleDesc.Count = 4;
	tZBufferDesc.SampleDesc.Quality = unM4xMsaaQuality - 1;
	hFnRet = pD3d->lpVtbl->CreateTexture2D(pD3d, &tZBufferDesc, NULL, &pDepthStencilBuffterTex);
	TH_CHECKERR_FAILED(hFnRet);
	hFnRet = pD3d->lpVtbl->CreateDepthStencilView(pD3d, pDepthStencilBuffterTex, NULL, &(ptCdx->pDepthStencilView));
	TH_CHECKERR_FAILED(hFnRet);

	// bind OM.
	ptCdx->pD3dContext->lpVtbl->OMSetRenderTargets(ptCdx->pD3dContext, 1, &(ptCdx->pRenderTargetView), ptCdx->pDepthStencilView);

	// set viewport.
	ptCdx->pD3dContext->lpVtbl->RSSetViewports(ptCdx->pD3dContext, 1, &tViewPort);

	nRet = C_TRUE;

Exit0:
	TH_C_COM_RELEASE(pD3d);
	TH_C_COM_RELEASE(pDxgiDevice);
	TH_C_COM_RELEASE(pDxgiAdapter);
	TH_C_COM_RELEASE(pDxgiFactory);
	TH_C_COM_RELEASE(pBackBuffterTex);

	return nRet;
}