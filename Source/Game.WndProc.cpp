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
		case WM_PAINT:
			return game->OnPaint();
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

LRESULT Game::OnPaint() {	
	::ValidateRect(m_window, &m_clientRect);
	return 0;
}