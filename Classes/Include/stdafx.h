#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <stdio.h>
#include <Windows.h>

#include "BaseRecord.h"

#define C_FALSE -1
#define C_TRUE	0
#define WINDOW_TITLE_W		L"th21"
#define WINDOW_MAX_HIGH		600
#define WINDOW_MAX_WIDTH	800

#define KEYACTIVE_UP	0x0001
#define KEYACTIVE_DOWN	0x0002
#define KEYACTIVE_LEFT	0x0004
#define KEYACTIVE_RIGHT	0x0008
#define KEYACTIVE_J		0x0100
#define KEYACTIVE_K		0x0200
#define KEYACTIVE		1
#define KEYSHUT			0

typedef int cdxBool;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT unMsg, WPARAM wParam, LPARAM lParam);


#endif
