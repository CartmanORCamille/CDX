#include "P1_Vertex.h"
#include "ClientLogic.h"


int P1_CreateGeometryBuffers(P1DEMO_PTR ptP1Demo, CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	HRESULT hrRet = E_FAIL;
	const short csMaxVertices = 8;
	const short csMaxIndices = 36;

	// Vertex buffer.
	P1VERTEXT arrtVertices[csMaxVertices] =
	{
		{XMFLOAT3(-1.f, -1.f, -1.f), COLOR_WHILE},
		{XMFLOAT3(-1.f, 1.f, 1.f), COLOR_BLACK},
		{XMFLOAT3(1.f, 1.f, -1.f), COLOR_RED},
		{XMFLOAT3(1.f, -1.f, -1.f), COLOR_GREEN},
		{XMFLOAT3(-1.f, -1.f, 1.f), COLOR_BLUE},
		{XMFLOAT3(-1.f, 1.f, 1.f), COLOR_YELLOW},
		{XMFLOAT3(1.f, 1.f, 1.f), COLOR_CYAN},
		{XMFLOAT3(1.f, -1.f, 1.f), COLOR_MAGENTA}
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
	UINT arrunIndices[csMaxIndices] =
	{
		// font surface. 
		0, 1, 2,
		0, 2, 3,

		// rear surface.
		4, 6, 5,
		4, 7, 6,

		// left surface.
		4, 5, 1,
		4, 1, 0,

		// right surface.
		3, 2, 6,
		3, 6, 7,

		// upper surface.
		1, 5, 6,
		1, 6, 2,

		// lower surface.
		4, 0, 3,
		4, 3, 7
	};
	D3D11_BUFFER_DESC tIndexBufDesc = { 0 };
	tIndexBufDesc.ByteWidth = sizeof(UINT) * csMaxIndices;
	tIndexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIndexBufDesc.CPUAccessFlags = 0;
	tIndexBufDesc.BindFlags = D3D11_USAGE_DEFAULT;
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


void P1_InitDemo(P1DEMO_PTR ptP1Demo)
{
	XMMATRIX tmp = XMMatrixIdentity();
	XMStoreFloat4x4(&(ptP1Demo->tWord), tmp);
	XMStoreFloat4x4(&(ptP1Demo->tView), tmp);
	XMStoreFloat4x4(&(ptP1Demo->tProj), tmp);
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


int P1_ExternCUpdate(float dt)
{
	int nResult = C_FALSE;
	if (NULL == g_p1DemoPtr)
	{
		__debugbreak();
	}
	float fX = g_p1DemoPtr->fRadius * sinf(g_p1DemoPtr->fPhi) * cosf(g_p1DemoPtr->fTheta);
	float fY = g_p1DemoPtr->fRadius * cosf(g_p1DemoPtr->fPhi);
	float fZ = g_p1DemoPtr->fRadius * sinf(g_p1DemoPtr->fPhi) * sinf(g_p1DemoPtr->fTheta);
	XMVECTOR tPos = XMVectorSet(fX, fY, fZ, 1.0f);
	XMVECTOR tTarget = XMVectorZero();
	XMVECTOR tUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMMATRIX tViewPort = XMMatrixLookAtLH(tPos, tTarget, tUp);

	XMStoreFloat4x4(&(g_p1DemoPtr->tView), tViewPort);

	nResult = C_OK;
Exit0:
	return nResult;

}


int P1_ExternCRender(CDXDISPATCH_PTR ptCdx)
{
	int nResult = C_FALSE;
	HRESULT hrRet = E_FAIL;
	if (NULL == g_p1DemoPtr)
	{
		__debugbreak();
	}

	const float carrfClearColor[4] = COLOR_CYAN;
	const unsigned int cunStride = sizeof(P1VERTEXT);
	const unsigned int cunOffset = 0;
	XMMATRIX tWorld = XMLoadFloat4x4(&g_p1DemoPtr->tWord);
	XMMATRIX tView = XMLoadFloat4x4(&g_p1DemoPtr->tView);
	XMMATRIX tProj = XMLoadFloat4x4(&g_p1DemoPtr->tProj);
	
	XMMATRIX tWorldViewProj = tWorld * tView * tProj;
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

	hrRet = g_p1DemoPtr->pMfxWorldViewProj->SetMatrix((float*)&tWorldViewProj);
	TH_CHECKERR_FAILED(hrRet);

	hrRet = g_p1DemoPtr->pTech->GetDesc(&tTechDesc);
	TH_CHECKERR_FAILED(hrRet);

	for (UINT i = 0; i < tTechDesc.Passes; ++i)
	{
		hrRet = g_p1DemoPtr->pTech->GetPassByIndex(i)->Apply(0, ptCdx->pD3dContext);
		TH_CHECKERR_FAILED(hrRet);

		ptCdx->pD3dContext->DrawIndexed(36, 0, 0);
	}

	hrRet = ptCdx->pDxgiSwapChain->Present(0, 0);
	TH_CHECKERR_FAILED(hrRet);

	nResult = C_OK;
Exit0:
	return nResult;

}
