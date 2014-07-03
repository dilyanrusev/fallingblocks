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

#include "stdafx.h"
#include "Game.h"
#include <cassert>

Game::Game()
		: APP_TITLE(L"Falling Blocks")
		, APP_CLASS(L"FallingBlocks")
		, WINDOW_WIDTH(1280)
		, WINDOW_HEIGHT(720)
		, BLOCKS_PER_WIDTH(10)
		, BLOCKS_PER_HEIGHT(20)
		, BLOCK_WIDTH(50)
		, BLOCK_HEIGHT(50)
		, m_isClassRegistered(false)
		, m_window(nullptr)
		, m_instance(nullptr) {
}

Game::~Game() {
	if (m_isClassRegistered && m_instance) {
		::UnregisterClass(Game::APP_CLASS, m_instance);
	}
}

void Game::Initialize(HINSTANCE instance) {
	assert(instance != nullptr);
	m_instance = instance;

	assert(::QueryPerformanceFrequency(&m_perfFrequency));

	CreateAppWindow();
	InitImaging();
	InitWriting();
	InitDrawing();
}

void Game::InitImaging() {	
	HRESULT hr = ::CoCreateInstance(CLSID_WICImagingFactory1, nullptr,
		CLSCTX_INPROC_SERVER, __uuidof(m_imageFactory), 
		reinterpret_cast<void**>(m_imageFactory.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void Game::InitWriting() {

}

void Game::InitDrawing() {

}

void Game::CreateAppWindow() {
	assert(m_instance != nullptr);	

	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = nullptr;
	wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = ::LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = m_instance;
	wc.lpfnWndProc = &Game::GameProc;
	wc.lpszClassName = Game::APP_CLASS;
	wc.lpszMenuName = nullptr;
	wc.style = 0;

	assert(::RegisterClassEx(&wc));
	m_isClassRegistered = true;
	
	DWORD windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	DWORD exStyle = 0;
	RECT windowSize = { 0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT };
	assert(::AdjustWindowRectEx(&windowSize, windowStyle, FALSE, exStyle));

	int width = windowSize.right - windowSize.left;
	int height = windowSize.bottom - windowSize.top;
	int x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	m_window = ::CreateWindowExW(exStyle, Game::APP_CLASS, Game::APP_TITLE,
		windowStyle, x, y, width, height, HWND_DESKTOP, nullptr, m_instance,
		reinterpret_cast<void*>(this));
	assert(m_window != nullptr);
}

void Game::RunMainLoop() {
	assert(m_instance);
	assert(m_window);

	::ShowWindow(m_window, SW_SHOW);
	::UpdateWindow(m_window);

	MSG msg;
	msg.message = WM_NULL;

	LARGE_INTEGER before, now;
	float elapsedInMs;

	::QueryPerformanceCounter(&before);
	while (msg.message != WM_QUIT) {
		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		::QueryPerformanceCounter(&now);
		elapsedInMs = static_cast<float>((now.QuadPart - before.QuadPart) * 1000 / m_perfFrequency.QuadPart);

		Update(elapsedInMs);
		Render();
	}
}

void Game::Update(float ms) {
	UNREFERENCED_PARAMETER(ms);
}

void Game::Render() {

}