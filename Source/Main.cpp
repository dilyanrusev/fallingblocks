#include "stdafx.h"
#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t* cmdLine, int cmdShow) {
	UNREFERENCED_PARAMETER(hInst);
	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdShow);

	MessageBox(nullptr, L"Compiles", L"Falling Blocks", MB_OK);
	return 0;
}