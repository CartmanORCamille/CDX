#include "stdafx.h"
#include "ClientLogic.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	int nRet						= C_FALSE;
	BOOL bFnRet						= FALSE;
	HWND hWindow					= NULL;
	MSG tMsg						= { 0 };
	RECT rcWindow					= { 0, 0, WINDOW_MAX_WIDTH, WINDOW_MAX_HIGH };
	HICON hIcon						= (HICON)LoadImage(NULL, L"Resource\\KMS.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	WNDCLASSEX wndClass				=
	{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		MainWndProc,
		NULL,
		NULL,
		hInstance,
		hIcon,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(GRAY_BRUSH),
		NULL,
		WINDOW_TITLE_W
	};

	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);
	if (!RegisterClassEx(&wndClass))
	{
		MessageBox(hWindow, L"window create failed.", L"Failed", 0);
		goto Exit0;
	}

	hWindow = CreateWindow
	(
		WINDOW_TITLE_W, WINDOW_TITLE_W, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, NULL, hInstance, NULL
	);
	ShowWindow(hWindow, nShowCmd);
	UpdateWindow(hWindow);

	while (WM_QUIT != tMsg.message)
	{
		if (PeekMessage(&tMsg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&tMsg);
			DispatchMessage(&tMsg);
			
		}
		else
		{
			
		}
	}

	nRet = C_TRUE;

Exit0:
	return nRet;
}


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRetCode = 0;

	switch (unMsg)
	{
	case WM_KEYDOWN:
		if (VK_LEFT == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_LEFT, KEYACTIVE);
		}
		else if (VK_RIGHT == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_RIGHT, KEYACTIVE);
		}
		else if (VK_UP == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_UP, KEYACTIVE);
		}
		else if (VK_DOWN == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_DOWN, KEYACTIVE);
		}
		else if (0x4A == wParam)
		{
			// J
			SetKeyActiveMap(KEYACTIVE_J, KEYACTIVE);
		}
		else if (0x4B == wParam)
		{
			// K
			SetKeyActiveMap(KEYACTIVE_K, KEYACTIVE);
		}
		GetKeyActiveMap();
		break;

	case WM_KEYUP:
		if (VK_LEFT == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_LEFT, KEYSHUT);
		}
		else if (VK_RIGHT == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_RIGHT, KEYSHUT);
		}
		else if (VK_UP == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_UP, KEYSHUT);
		}
		else if (VK_DOWN == wParam)
		{
			SetKeyActiveMap(KEYACTIVE_DOWN, KEYSHUT);
		}
		else if (0x4A == wParam)
		{
			// J
			SetKeyActiveMap(KEYACTIVE_J, KEYSHUT);
		}
		else if (0x4B == wParam)
		{
			// K
			SetKeyActiveMap(KEYACTIVE_K, KEYSHUT);
		}
		//GetKeyActiveMap();
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		lRetCode = DefWindowProc(hWnd, unMsg, wParam, lParam);
		break;
	}

	return lRetCode;
}
