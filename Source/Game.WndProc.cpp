#include "stdafx.h"
#include "Game.h"

LRESULT Game::GameProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
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

		switch (message) {
		case WM_CLOSE:
			return game->OnClose();
		case WM_DESTROY:
			return game->OnDestroy();
		default:
			return ::DefWindowProc(window, message, wparam, lparam);
		}
	}
}

LRESULT Game::OnDestroy() {
	::PostQuitMessage(0);
	return 0;
}

LRESULT Game::OnClose() {
	::DestroyWindow(m_window);
	return 0;
}