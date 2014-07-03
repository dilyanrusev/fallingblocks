/*
Copyright (c) 2014, Dilyan Rusev
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
	void InitGraphicsSystems();	

	static LRESULT CALLBACK GameProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT OnDestroy();
	LRESULT OnClose();
	LRESULT OnPaint();

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
		
	RECT m_clientRect;
	bool m_isClassRegistered;
	HWND m_window;
	HINSTANCE m_instance;
	LARGE_INTEGER m_perfFrequency;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_drawFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_writeFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_imageFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
	Microsoft::WRL::ComPtr<ID2D1Brush> m_testBrush;
};

