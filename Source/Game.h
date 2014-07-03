#pragma once

#include "stdafx.h"

class Game {
public:
	Game();
	~Game();

private:
	const wchar_t* APP_TITLE;
	const wchar_t* APP_CLASS;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;

	HWND m_window;
	HINSTANCE m_instance;
};

