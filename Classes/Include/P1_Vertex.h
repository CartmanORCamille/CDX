#ifndef __P1_VERTEX_H__
#define __P1_VERTEX_H__

#include "CDX_D3DMath.h"
#include "D3DMain.h"

using namespace DirectX;
struct _tP1Vertex
{
	XMFLOAT3 tPos;
	XMFLOAT4 tColor;
};

struct _tP1Demo
{
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
	ID3DX11Effect* pFX;
	ID3DX11EffectTechnique* pTech;
	ID3DX11EffectMatrixVariable* pMfxWorldViewProj;
	ID3D11InputLayout* pInputLayout;

	XMFLOAT4X4 tWord;
	XMFLOAT4X4 tView;
	XMFLOAT4X4 tProj;

	float fTheta;
	float fPhi;
	float fRadius;
};

typedef _tP1Vertex P1VERTEXT, * P1VERTEXT_PTR;
typedef _tP1Demo P1DEMO, * P1DEMO_PTR;

P1DEMO_PTR g_p1DemoPtr;

int P1_CreateGeometryBuffers(P1DEMO_PTR ptP1Demo = g_p1DemoPtr, CDXDISPATCH_PTR ptCdx = NULL);
int P1_CreateFX(P1DEMO_PTR ptP1Demo = g_p1DemoPtr, CDXDISPATCH_PTR ptCdx = NULL);
int P1_CreateVertexLayout(P1DEMO_PTR ptP1Demo = g_p1DemoPtr, CDXDISPATCH_PTR ptCdx = NULL);
void P1_InitDemo(P1DEMO_PTR ptP1Demo = g_p1DemoPtr);
int P1_Init(CDXDISPATCH_PTR ptCdx, P1DEMO_PTR ptP1Demo = g_p1DemoPtr);

#ifdef  __cplusplus
extern "C" {
#endif
	void thLogA(const CHAR* cszpText, ...);
	int P1_ExternCMain(CDXDISPATCH_PTR ptCdx);
	int P1_ExternCUpdate(float dt);
	int P1_ExternCRender(CDXDISPATCH_PTR ptCdx);
#ifdef __cplusplus
}
#endif

#endif
