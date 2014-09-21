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

#ifdef WIN32

#include "Game.h"
#include"Board.h"
#include <cassert>
#include <algorithm>

using namespace std;
using Microsoft::WRL::ComPtr;

struct Game::Impl {
	Impl(Game* parent);
	~Impl();

	void CreateAppWindow();
	void InitGraphicsSystems();		

	void RunMainLoop();
	void Initialize();

	static LRESULT CALLBACK GameProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT OnDestroy();
	LRESULT OnClose();
	LRESULT OnPaint();
	LRESULT OnKeyDown(int vk);
	LRESULT OnKeyUp(int vk);

	void Update(float ms);
	void Render();
	void CreateDeviceResources();
	void CleanDeviceResources();

	ComPtr<ID2D1Bitmap> LoadImage(const wchar_t* fileName) const;

	const wchar_t* APP_TITLE;
	const wchar_t* APP_CLASS;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	const int BLOCK_WIDTH;
	const int BLOCK_HEIGHT;		
	
	Game* m_parent;
	RECT m_clientRect;
	bool m_isClassRegistered;
	HWND m_window;
	HINSTANCE m_instance;
	LARGE_INTEGER m_perfFrequency;
	Board m_board;
	D2D1_POINT_2F m_boardPos;
	ComPtr<ID2D1Factory> m_drawFactory;
	ComPtr<IDWriteFactory> m_writeFactory;
	ComPtr<IWICImagingFactory> m_imageFactory;
	ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
	ComPtr<ID2D1Brush> m_testBrush;
	ComPtr<ID2D1Bitmap> m_terimonoBitmaps[Count_Tetrimonos];
};

Game::Impl::Impl(Game* parent) 
		: APP_TITLE(L"Falling Blocks")
		, APP_CLASS(L"FallingBlocks")
		, WINDOW_WIDTH(1280)
		, WINDOW_HEIGHT(720)
		, BLOCK_WIDTH(35)
		, BLOCK_HEIGHT(35)
		, m_parent(parent)
		, m_isClassRegistered(false)
		, m_window(nullptr)
		, m_instance(nullptr) {
}

Game::Impl::~Impl() {
	if (m_isClassRegistered && m_instance) {
		::UnregisterClass(APP_CLASS, m_instance);
	}
}

Game::Game()
		: m_impl(new Impl(this)) {
}

Game::~Game() {	
}

void Game::Initialize() {
	m_impl->Initialize();
}

void Game::Impl::Initialize() {
	m_instance = static_cast<HINSTANCE>(::GetModuleHandle(nullptr));

	assert(::QueryPerformanceFrequency(&m_perfFrequency));

	CreateAppWindow();
	InitGraphicsSystems();
	CreateDeviceResources();
}

void Game::Impl::InitGraphicsSystems() {
	HRESULT hr = ::CoCreateInstance(CLSID_WICImagingFactory1, nullptr,
		CLSCTX_INPROC_SERVER, __uuidof(m_imageFactory), 
		reinterpret_cast<void**>(m_imageFactory.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_writeFactory), 
		reinterpret_cast<IUnknown**>(m_writeFactory.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&m_drawFactory));
	assert(SUCCEEDED(hr));
}

void Game::Impl::CreateAppWindow() {
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
	wc.lpfnWndProc = &GameProc;
	wc.lpszClassName = APP_CLASS;
	wc.lpszMenuName = nullptr;
	wc.style = 0;

	assert(::RegisterClassEx(&wc));
	m_isClassRegistered = true;
	
	DWORD windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	DWORD exStyle = 0;
	RECT windowSize = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	assert(::AdjustWindowRectEx(&windowSize, windowStyle, FALSE, exStyle));

	int width = windowSize.right - windowSize.left;
	int height = windowSize.bottom - windowSize.top;
	int x = (::GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int y = (::GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	m_window = ::CreateWindowExW(exStyle, APP_CLASS, APP_TITLE,
		windowStyle, x, y, width, height, HWND_DESKTOP, nullptr, m_instance,
		reinterpret_cast<void*>(m_parent));
	assert(m_window != nullptr);

	m_clientRect.left = 0;
	m_clientRect.right = WINDOW_WIDTH;
	m_clientRect.top = 0;
	m_clientRect.bottom = WINDOW_HEIGHT;

	m_boardPos.x = (WINDOW_WIDTH - m_board.WIDTH * BLOCK_WIDTH) / 2.0f;
	m_boardPos.y = (WINDOW_HEIGHT - m_board.HEIGHT * BLOCK_HEIGHT) / 2.0f;
}

void Game::RunMainLoop() {
	m_impl->RunMainLoop();
}

void Game::Impl::RunMainLoop() {
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
		before = now;
	}
}

void Game::Impl::Update(float ms) {
	m_board.Update(ms);
}

void Game::Impl::Render() {	
	m_renderTarget->BeginDraw();
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));

	D2D1_RECT_F dest;	
	for (int y = 0; y < m_board.HEIGHT; y++) {
		for (int x = 0; x < m_board.WIDTH; x++) {
			auto t = m_board.GetAt(x, y);
			auto& bitmap = m_terimonoBitmaps[t];
			dest.left = m_boardPos.x + static_cast<float>(x * BLOCK_WIDTH);
			dest.top = m_boardPos.y + static_cast<float>(y * BLOCK_HEIGHT);
			dest.right = dest.left + BLOCK_WIDTH;
			dest.bottom = dest.top + BLOCK_HEIGHT;
			
			m_renderTarget->DrawBitmap(bitmap.Get(), &dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
		}
	}

	D2D1_POINT_2F nextOffset = D2D1::Point2F(m_boardPos.x + m_board.WIDTH * BLOCK_WIDTH + 30, m_boardPos.y);
	for (int y = 0; y < m_board.MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < m_board.MAX_TETRIMONO_WIDTH; x++) {
			auto t = m_board.GetNextAt(x, y);
			auto& bitmap = m_terimonoBitmaps[t];
			dest.left = nextOffset.x + static_cast<float>(x * BLOCK_WIDTH);
			dest.top = nextOffset.y + static_cast<float>(y * BLOCK_HEIGHT);
			dest.right = dest.left + BLOCK_WIDTH;
			dest.bottom = dest.top + BLOCK_HEIGHT;

			m_renderTarget->DrawBitmap(bitmap.Get(), &dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
		}
	}

	if (m_renderTarget->EndDraw() == D2DERR_RECREATE_TARGET) {
		CleanDeviceResources();
		CreateDeviceResources();
	}		
}

void Game::Impl::CreateDeviceResources() {
	HRESULT hr;
	hr = m_drawFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(), 96.0f, 96.0f),
		D2D1::HwndRenderTargetProperties(m_window, D2D1::SizeU(WINDOW_WIDTH, WINDOW_HEIGHT)),
		m_renderTarget.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(hr));

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> solidBrush;
	assert(SUCCEEDED(
		m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &solidBrush)
		));
	solidBrush.As(&m_testBrush);

	m_terimonoBitmaps[Tetrimono_Empty] = LoadImage(L"Assets/block_empty.png");
	m_terimonoBitmaps[Tetrimono_I] = LoadImage(L"Assets/block_blue.png");
	m_terimonoBitmaps[Tetrimono_J] = LoadImage(L"Assets/block_cyan.png");
	m_terimonoBitmaps[Tetrimono_L] = LoadImage(L"Assets/block_green.png");
	m_terimonoBitmaps[Tetrimono_O] = LoadImage(L"Assets/block_orange.png");
	m_terimonoBitmaps[Tetrimono_S] = LoadImage(L"Assets/block_red.png");
	m_terimonoBitmaps[Tetrimono_T] = LoadImage(L"Assets/block_violet.png");
	m_terimonoBitmaps[Tetrimono_Z] = LoadImage(L"Assets/block_yellow.png");
}

ComPtr<ID2D1Bitmap> Game::Impl::LoadImage(const wchar_t* fileName) const {
	ComPtr<IWICBitmapDecoder> decoder;
	assert(SUCCEEDED(
		m_imageFactory->CreateDecoderFromFilename(fileName, nullptr, GENERIC_READ, 
			WICDecodeMetadataCacheOnDemand, &decoder)
	));

	ComPtr<IWICBitmapFrameDecode> frame;
	assert(SUCCEEDED( decoder->GetFrame(0, &frame) ));

	ComPtr<IWICFormatConverter> formatConverter;
	assert(SUCCEEDED( m_imageFactory->CreateFormatConverter(&formatConverter) ));

	assert(SUCCEEDED(
		formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom)
	));


	ComPtr<ID2D1Bitmap> bitmap;
	assert(SUCCEEDED(
		m_renderTarget->CreateBitmapFromWicBitmap(formatConverter.Get(), &bitmap)
	));

	return bitmap;
}


void Game::Impl::CleanDeviceResources() {
	m_testBrush = nullptr;
	m_renderTarget = nullptr;
}

LRESULT Game::Impl::GameProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_CREATE) {
		CREATESTRUCT* createData = reinterpret_cast<CREATESTRUCT*>(lparam);
		Game* game = reinterpret_cast<Game*>(createData->lpCreateParams);

		::SetWindowLongPtr(window, GWLP_USERDATA,
#ifdef _WIN64
			reinterpret_cast<LONG_PTR>(game)
#else
			reinterpret_cast<LONG>(game)
#endif
			);
		return 0;
	} else {
		Game* game = reinterpret_cast<Game*>(::GetWindowLongPtr(window, GWLP_USERDATA));
		std::unique_ptr<Game::Impl>& impl = game->m_impl;

		switch (message) {
		case WM_PAINT:
			return impl->OnPaint();
		case WM_CLOSE:
			return impl->OnClose();
		case WM_DESTROY:
			return impl->OnDestroy();
		case WM_KEYDOWN:
			return impl->OnKeyDown(static_cast<int>(wparam));
		case WM_KEYUP:
			return impl->OnKeyUp(static_cast<int>(wparam));
		default:
			return ::DefWindowProc(window, message, wparam, lparam);
		}
	}
}

LRESULT Game::Impl::OnKeyDown(int vk) {
	switch (vk) {
	case VK_LEFT:
		m_board.MoveCurrent(-1, 0); break;
	case VK_RIGHT:
		m_board.MoveCurrent(1, 0); break;
	case VK_UP:
		m_board.Rotate(RotateDirection_Clockwize); break;
	case VK_DOWN:
		m_board.MoveCurrent(0, 1); break;
		 break;
	case VK_SPACE:
		m_board.FallDown(); break;
	case VK_ESCAPE:
		::PostQuitMessage(0); break;
	}
	return 0;
}

LRESULT Game::Impl::OnKeyUp(int vk) {
	UNREFERENCED_PARAMETER(vk);
	return 0;
}

LRESULT Game::Impl::OnDestroy() {
	::PostQuitMessage(0);
	return 0;
}

LRESULT Game::Impl::OnClose() {
	::DestroyWindow(m_window);
	return 0;
}

LRESULT Game::Impl::OnPaint() {
	::ValidateRect(m_window, &m_clientRect);
	return 0;
}

#endif