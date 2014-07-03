#pragma once

#include "stdafx.h"

class Game {
public:
	Game();
	~Game();

	void Initialize();
	void RunMainLoop();

private:	
	Game(const Game&);
	Game& operator=(const Game&);

	void Update(float ms);
	void Render();
	void CreateDeviceResources();
	void CleanDeviceResources();

	const wchar_t* APP_TITLE;
	const wchar_t* APP_CLASS;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	const int BLOCKS_PER_WIDTH;
	const int BLOCKS_PER_HEIGHT;

	HWND m_window;
	HINSTANCE m_instance;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_drawFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_writeFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_imageFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
};

