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
#include <exception>
#include "Game.h"


#include "Com.h"

#ifdef WIN32

#pragma comment(lib, "D2d1.lib")
#pragma comment(lib, "Dwrite.lib")

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t* cmdLine, int cmdShow) {	
	UNREFERENCED_PARAMETER(hInst);
	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(cmdShow);

#else

#include "SDL.h"

int main(int argc, char** argv) {

#endif

	try {
		Com com;
		Game game;
		game.Initialize();
		game.RunMainLoop();
		return 0;
	} catch (const std::exception& error) {
#ifdef WIN32
		::MessageBoxA(nullptr, error.what(), "Falling Blocks Error", MB_OK);
#else
		if (!::SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Falling Blocks Error",
				error.what(), nullptr)) {
			fprintf(stderr, "%s\n", error.what());
		}
#endif
		return 1;
	}
}
