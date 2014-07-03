#include "stdafx.h"
#include <exception>
#include "Game.h"
#include "Com.h"


int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t* cmdLine, int cmdShow) {	
	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdShow);

	try {
		Com com;
		Game game;
		game.Initialize(hInst);
		game.RunMainLoop();
		return 0;
	} catch (const std::exception& error) {
		::MessageBoxA(nullptr, error.what(), "Falling Blocks Error", MB_OK);
		return 1;
	}
}