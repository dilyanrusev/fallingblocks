#include "stdafx.h"
#include "Game.h"
#include <exception>

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t* cmdLine, int cmdShow) {	
	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdShow);

	try {
		Game game;
		game.Initialize(hInst);
		game.RunMainLoop();
		return 0;
	} catch (const std::exception& error) {
		::MessageBoxA(nullptr, error.what(), "Falling Blocks Error", MB_OK);
		return 1;
	}
}