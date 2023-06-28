#include "P1_Vertex.h"

#define MAX_INDEXCOUNT 36
P1DEMO_PTR g_p1DemoPtr = NULL;

int P1_CreateGeometryBuffers(P1DEMO_PTR ptP1Demo, CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	HRESULT hrRet = E_FAIL;
	const short csMaxVertices = 8;

	// Vertex buffer.
	P1VERTEXT arrtVertices[csMaxVertices] =
	{
		// 0
		{XMFLOAT3(-0.5f, 0.f, 0.f), COLOR_WHILE},
		// 1
		{XMFLOAT3(0.f, 0.f, 0.f), COLOR_BLACK},
		// 2
		{XMFLOAT3(-0.5f, -0.5f, 0.f), COLOR_RED},
		// 3
		{XMFLOAT3(0.f, -0.5f, 0.f), COLOR_GREEN},
		// 4
		{XMFLOAT3(-0.5f, 0.f, 0.5f), COLOR_BLUE},
		// 5
		{XMFLOAT3(-0.5f, -0.5f, 0.5f), COLOR_YELLOW},
		// 6
		{XMFLOAT3(0.f, 0.f, 0.5f), COLOR_CYAN},
		// 7
		{XMFLOAT3(0.f, -0.5f, 0.5f), COLOR_MAGENTA},
	};

	D3D11_BUFFER_DESC tVertexBufDesc = { 0 };
	tVertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	tVertexBufDesc.ByteWidth = sizeof(P1VERTEXT) * csMaxVertices;
	tVertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVertexBufDesc.CPUAccessFlags = 0;
	tVertexBufDesc.MiscFlags = 0;
	tVertexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVertexData = { 0 };
	tVertexData.pSysMem = arrtVertices;

	// Index buffer.
	// 1 surface = 2 triangle.
	UINT arrunIndices[MAX_INDEXCOUNT] =
	{
		// font surface.
		0, 1, 2,
		1, 3, 2,
		// rear surface.
		4, 6, 5,
		6, 7, 5,
		// left surface.
		4, 0, 5,
		0, 2, 5,
		// right surface.
		6, 1, 7,
		1, 3, 7,
		// upper surface.
		4, 6, 0,
		6, 1, 0,
		// lower surface.
		5, 7, 2,
		7, 3, 2
	};
	D3D11_BUFFER_DESC tIndexBufDesc = { 0 };
	tIndexBufDesc.ByteWidth = sizeof(UINT) * MAX_INDEXCOUNT;
	tIndexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIndexBufDesc.CPUAccessFlags = 0;
	tIndexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	tIndexBufDesc.MiscFlags = 0;
	tIndexBufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIndexData = { 0 };
	tIndexData.pSysMem = arrunIndices;

	// Create vertex buffer.
	hrRet = ptCdx->pD3d->CreateBuffer(&tVertexBufDesc, &tVertexData, &(ptP1Demo->pVertexBuffer));
	TH_CHECKERR_FAILED(hrRet);
	// Create index buffer.
	hrRet = ptCdx->pD3d->CreateBuffer(&tIndexBufDesc, &tIndexData, &(ptP1Demo->pIndexBuffer));
	TH_CHECKERR_FAILED(hrRet);

	nResult = C_OK;
Exit0:
	return nResult;
}

int P1_CreateFX(P1DEMO_PTR ptP1Demo, CDXDISPATCH_PTR ptCdx)
{
	int nRet = C_FALSE;
	HRESULT hrResult = E_FAIL;
	DWORD dwShaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3D10_SHADER_DEBUG;
	dwShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif // defined(DEBUG) || defined(_DEBUG)

	ID3D10Blob* pCompiledShader = NULL;
	ID3D10Blob* pCompiledErrMsgs = NULL;
	char arrszCompiledErrMsgs[1024] = { 0 };

	// build shader.
	hrResult = D3DX11CompileFromFile(
		L".\\FX\\P1.fx", NULL, NULL, NULL, "fx_5_0", dwShaderFlags, 0, NULL, &pCompiledShader, &pCompiledErrMsgs, NULL
	);

	if (NULL != pCompiledErrMsgs)
	{
		sprintf_s(arrszCompiledErrMsgs, 1024, "[ERROR] Compiled fx err: %s", (char*)pCompiledErrMsgs->GetBufferPointer());
		OutputDebugStringA(arrszCompiledErrMsgs);
		goto Exit0;
	}
	TH_CHECKERR_FAILED(hrResult);

	// create shader effect.
	hrResult = D3DX11CreateEffectFromMemory(
		pCompiledShader->GetBufferPointer(),
		pCompiledShader->GetBufferSize(),
		0,
		ptCdx->pD3d,
		&(ptP1Demo->pFX)
	);
	TH_CHECKERR_FAILED(hrResult);

	ptP1Demo->pTech = ptP1Demo->pFX->GetTechniqueByName("ColorTech");
	ptP1Demo->pMfxWorldViewProj = ptP1Demo->pFX->GetVariableByName("gWorldViewProj")->AsMatrix();

	nRet = C_OK;
Exit0:
	TH_COM_RELEASE(pCompiledErrMsgs);
	TH_COM_RELEASE(pCompiledShader);

	return nRet;
}

int P1_CreateVertexLayout(P1DEMO_PTR ptP1Demo, CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	const short csMaxVertexDesc = 2;
	HRESULT hrRet = E_FAIL;
	D3D11_INPUT_ELEMENT_DESC arrtVertexDesc[csMaxVertexDesc] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC tPassDesc = { 0 };

	hrRet = ptP1Demo->pTech->GetPassByIndex(0)->GetDesc(&tPassDesc);
	TH_CHECKERR_FAILED(hrRet);

	hrRet = ptCdx->pD3d->CreateInputLayout(
		arrtVertexDesc, csMaxVertexDesc, tPassDesc.pIAInputSignature, tPassDesc.IAInputSignatureSize, &(ptP1Demo->pInputLayout)
	);
	TH_CHECKERR_FAILED(hrRet);

	nResult = C_OK;
Exit0:
	return nResult;
}

int P1_CreateRasterizer(CDXDISPATCH_PTR ptCdx, P1DEMO_PTR ptP1Demo)
{
	int nResult = C_FALSE;
	HRESULT hrRet = E_FAIL;
	D3D11_RASTERIZER_DESC tWrDesc;
	D3D11_RASTERIZER_DESC tSoDesc;

	ZeroMemory(&tWrDesc, sizeof(D3D11_RASTERIZER_DESC));
	tWrDesc.FillMode = D3D11_FILL_WIREFRAME;
	tWrDesc.CullMode = D3D11_CULL_BACK;
	tWrDesc.FrontCounterClockwise = false;
	tWrDesc.DepthClipEnable = true;

	ZeroMemory(&tSoDesc, sizeof(D3D11_RASTERIZER_DESC));
	tSoDesc.FillMode = D3D11_FILL_SOLID;
	tSoDesc.CullMode = D3D11_CULL_BACK;
	tSoDesc.FrontCounterClockwise = false;
	tSoDesc.DepthClipEnable = true;

	hrRet = ptCdx->pD3d->CreateRasterizerState(&tWrDesc, &(ptP1Demo->pWirerameRS));
	TH_CHECKERR_FAILED(hrRet);
	hrRet = ptCdx->pD3d->CreateRasterizerState(&tSoDesc, &(ptP1Demo->pSoildRS));
	TH_CHECKERR_FAILED(hrRet);
	ptCdx->pD3dContext->RSSetState(ptP1Demo->pWirerameRS);

	nResult = C_OK;
Exit0:
	return nResult;
}

void P1_InitDemo(P1DEMO_PTR ptP1Demo)
{
	XMMATRIX tmp = XMMatrixIdentity();

	XMStoreFloat4x4(&(ptP1Demo->tWord), tmp);
	XMStoreFloat4x4(&(ptP1Demo->tView), tmp);
	XMStoreFloat4x4(&(ptP1Demo->tProj), tmp);

	ptP1Demo->fTheta = 1.5f * CDX_PI;
	ptP1Demo->fPhi = 0.25 * CDX_PI;
	ptP1Demo->fRadius = 1.f;

	ptP1Demo->tCameraPositionArgs = { 0 };
	ptP1Demo->tCameraTargetArgs = { 0 };
	ptP1Demo->tCameraPositionArgs.fX = -0.1f;
	ptP1Demo->tCameraPositionArgs.fZ = -0.3f;
	ptP1Demo->tCameraTargetArgs.fY = -0.2f;
}


int P1_Init(CDXDISPATCH_PTR ptCdx, P1DEMO_PTR ptP1Demo)
{
	int nResult = C_FALSE;
	P1_InitDemo(ptP1Demo);

	nResult = P1_CreateGeometryBuffers(ptP1Demo, ptCdx);
	TH_CHECKERR_NEGATIVE(nResult);

	nResult = P1_CreateFX(ptP1Demo, ptCdx);
	TH_CHECKERR_NEGATIVE(nResult);

	nResult = P1_CreateVertexLayout(ptP1Demo, ptCdx);
	TH_CHECKERR_NEGATIVE(nResult);

	nResult = P1_CreateRasterizer(ptCdx, ptP1Demo);
	TH_CHECKERR_NEGATIVE(nResult);

	nResult = C_OK;
Exit0:
	return nResult;
}


int P1_ExternCMain(CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	P1DEMO_PTR ptP1Demo = (P1DEMO_PTR)malloc(sizeof(P1DEMO));
	
	nResult = P1_Init(ptCdx, ptP1Demo);
	TH_CHECKERR_NEGATIVE(nResult);

	g_p1DemoPtr = ptP1Demo;

	nResult = C_OK;
Exit0:
	return nResult;
}


int P1_ExternCUpdate(float dt, CDXDISPATCH_PTR ptCdx)
{
	if (NULL == g_p1DemoPtr)
	{
		__debugbreak();
	}

	int nResult = C_FALSE;
	HRESULT hrRet = E_FAIL;

	XMVECTOR tPos = XMVectorSet(g_p1DemoPtr->tCameraPositionArgs.fX, g_p1DemoPtr->tCameraPositionArgs.fY, g_p1DemoPtr->tCameraPositionArgs.fZ, 0.0f);
	XMVECTOR tTarget = XMVectorSet(g_p1DemoPtr->tCameraTargetArgs.fX, g_p1DemoPtr->tCameraTargetArgs.fY, g_p1DemoPtr->tCameraTargetArgs.fZ, 0.0f);
	XMVECTOR tUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMMATRIX tViewPort = XMMatrixLookAtLH(tPos, tTarget, tUp);
	XMStoreFloat4x4(&(g_p1DemoPtr->tView), tViewPort);
	XMMATRIX tWorld = XMLoadFloat4x4(&g_p1DemoPtr->tWord);
	XMMATRIX tView = XMLoadFloat4x4(&g_p1DemoPtr->tView);
	XMMATRIX tProj = XMLoadFloat4x4(&g_p1DemoPtr->tProj);
	XMMATRIX tWorldViewProj = tWorld * tView * tProj;

	THCALLBACK(pfnChangeRenderMethodWR) = P1_KChangeRenderMethodWR;
	THCALLBACK(pfnChangeRenderMethodSO) = P1_KChangeRenderMethodSO;
	THCALLBACK(pfnChangeCameraUp) = P1_KChangeCameraUp;
	THCALLBACK(pfnChangeCameraDown) = P1_KChangeCameraDown;
	THCALLBACK(pfnChangeCameraLeft) = P1_KChangeCameraLeft;
	THCALLBACK(pfnChangeCameraRight) = P1_KChangeCameraRight;

	hrRet = g_p1DemoPtr->pMfxWorldViewProj->SetMatrix((float*)&tWorldViewProj);
	TH_CHECKERR_FAILED(hrRet);

	// keyboard event.
	nResult = P1_CreateKeyboardEvent(KEYACTIVE_J | KEYACTIVE_K, ptCdx, pfnChangeRenderMethodSO, pfnChangeRenderMethodWR);
	TH_CHECKERR_NEGATIVE(nResult);
	nResult = P1_CreateKeyboardEvent(KEYACTIVE_UP, ptCdx, pfnChangeCameraUp, NULL);
	TH_CHECKERR_NEGATIVE(nResult);
	nResult = P1_CreateKeyboardEvent(KEYACTIVE_DOWN, ptCdx, pfnChangeCameraDown, NULL);
	TH_CHECKERR_NEGATIVE(nResult);
	nResult = P1_CreateKeyboardEvent(KEYACTIVE_LEFT, ptCdx, pfnChangeCameraLeft, NULL);
	TH_CHECKERR_NEGATIVE(nResult);
	nResult = P1_CreateKeyboardEvent(KEYACTIVE_RIGHT, ptCdx, pfnChangeCameraRight, NULL);
	TH_CHECKERR_NEGATIVE(nResult);

	nResult = C_OK;
Exit0:
	return nResult;

}


int P1_CreateKeyboardEvent(int nFlags, CDXDISPATCH_PTR ptCdx, THCALLBACK(fnCallbackKeyDown), THCALLBACK(fnCallbackKeyUp))
{
	int nRet = C_FALSE;
	int nIsKeyDown = C_FALSE;
	static int nIsKeyAlreadyDown = C_FALSE;

	GetKeyActiveMapEx(nFlags, &nIsKeyDown);
	if (NULL != fnCallbackKeyDown && C_TRUE == nIsKeyDown)
	{
		fnCallbackKeyDown(ptCdx, (void*)&nIsKeyDown);
		nIsKeyAlreadyDown = C_TRUE;
	}
	else if (NULL != fnCallbackKeyUp && C_FALSE == nIsKeyDown && C_TRUE == nIsKeyAlreadyDown)
	{
		fnCallbackKeyUp(ptCdx, (void*)&nIsKeyDown);
		nIsKeyAlreadyDown = C_FALSE;
	}
	

	nRet = C_TRUE;
Exit0:
	return nRet;
}


int P1_ExternCRender(CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	HRESULT hrRet = E_FAIL;
	if (NULL == g_p1DemoPtr)
	{
		__debugbreak();
	}

	const float carrfClearColor[4] = COLOR_LIGHTSTEELBLUE;
	const unsigned int cunStride = sizeof(P1VERTEXT);
	const unsigned int cunOffset = 0;

	D3DX11_TECHNIQUE_DESC tTechDesc = { 0 };

	ptCdx->pD3dContext->ClearRenderTargetView(
		ptCdx->pRenderTargetView,
		carrfClearColor
	);
	ptCdx->pD3dContext->ClearDepthStencilView(
		ptCdx->pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	ptCdx->pD3dContext->IASetInputLayout(g_p1DemoPtr->pInputLayout);
	ptCdx->pD3dContext->IASetVertexBuffers(0, 1, &(g_p1DemoPtr->pVertexBuffer), &cunStride, &cunOffset);
	ptCdx->pD3dContext->IASetIndexBuffer(g_p1DemoPtr->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	ptCdx->pD3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	hrRet = g_p1DemoPtr->pTech->GetDesc(&tTechDesc);
	TH_CHECKERR_FAILED(hrRet);

	for (UINT i = 0; i < tTechDesc.Passes; ++i)
	{
		hrRet = g_p1DemoPtr->pTech->GetPassByIndex(i)->Apply(0, ptCdx->pD3dContext);
		TH_CHECKERR_FAILED(hrRet);

		ptCdx->pD3dContext->DrawIndexed(MAX_INDEXCOUNT, 0, 0);
	}

	hrRet = ptCdx->pDxgiSwapChain->Present(0, 0);
	TH_CHECKERR_FAILED(hrRet);

	nResult = C_OK;
Exit0:
	return nResult;

}
