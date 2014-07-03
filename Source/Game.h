#pragma once

#include "stdafx.h"

class Game {
public:
	Game();
	~Game();

	void Initialize(HINSTANCE instance);
	void RunMainLoop();

private:	
	Game(const Game&);
	Game& operator=(const Game&);

	void CreateAppWindow();

	static LRESULT CALLBACK GameProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT OnDestroy();
	LRESULT OnClose();

	void Update(float ms);
	void Render();
	void CreateDeviceResources();
	void CleanDeviceResources();

	const wchar_t* APP_TITLE;
	const wchar_t* APP_CLASS;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	const int BLOCK_WIDTH;
	const int BLOCK_HEIGHT;
	const int BLOCKS_PER_WIDTH;
	const int BLOCKS_PER_HEIGHT;
		
	bool m_isClassRegistered;
	HWND m_window;
	HINSTANCE m_instance;
	LARGE_INTEGER m_perfFrequency;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_drawFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_writeFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_imageFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
};

