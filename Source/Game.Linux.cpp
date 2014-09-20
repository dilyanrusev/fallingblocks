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

#ifndef WIN32

#include "Game.h"
#include "Board.h"
#include "Log.h"

#include "SDL.h"
#include "SDL_image.h"
#include <cassert>
#include <algorithm>

using std::begin;
using std::end;

static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 720;
static const int BLOCK_WIDTH = 35;
static const int BLOCK_HEIGHT = 35;	
static const char* APP_TITLE = "Falling Blocks";

struct Game::Impl {
	bool m_isSdlInitialized;
	bool m_isImgInitialized;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	Board m_board;
	SDL_Texture* m_terimonos[Count_Tetrimonos];
	SDL_Rect m_boardPos;
	SDL_Rect m_nextOffset;

	Impl()
			: m_isSdlInitialized(false) 
			, m_isImgInitialized(false)
			, m_window(nullptr)
			, m_renderer(nullptr) {
		InitializeLog();
		std::fill(begin(m_terimonos), end(m_terimonos), nullptr);
	}

	~Impl();

	void Initialize();
	void RunGameLoop();
	void Update(float ms);
	void Render();
	void LoadTextures();
	bool HandleKeyboardEvents(const SDL_Event& event);
	SDL_Texture* LoadTexture(const char* path);
};

Game::Game()
		: m_impl(new Impl()) {

}

Game::~Game() {
	
}

void Game::Initialize() {
	printf("%s\n", "Initialize...");
	m_impl->Initialize();
}

void Game::Impl::Initialize() {
	assert(!m_isSdlInitialized);

	assert( ::SDL_Init(SDL_INIT_VIDEO) == 0);
	m_isSdlInitialized = true;	

	m_window = ::SDL_CreateWindow(
		APP_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0
	);
	assert(m_window);

	m_renderer = ::SDL_CreateRenderer(m_window,	-1,	0);
	assert(m_renderer);

	::SDL_SetRenderDrawColor(m_renderer, 240, 248, 255, 255);

	assert( ::IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG );
	m_isImgInitialized = true;

	LoadTextures();

	m_boardPos.x = (WINDOW_WIDTH - m_board.WIDTH * BLOCK_WIDTH) / 2;
	m_boardPos.y = (WINDOW_HEIGHT - m_board.HEIGHT * BLOCK_HEIGHT) / 2;
	m_nextOffset.x = m_boardPos.x + m_board.WIDTH * BLOCK_WIDTH + 30;
	m_nextOffset.y = m_boardPos.y;
}

void Game::Impl::LoadTextures() {
	m_terimonos[Tetrimono_Empty] = LoadTexture("Assets/block_empty.png");
	m_terimonos[Tetrimono_I] = LoadTexture("Assets/block_blue.png");
	m_terimonos[Tetrimono_J] = LoadTexture("Assets/block_cyan.png");
	m_terimonos[Tetrimono_L] = LoadTexture("Assets/block_green.png");
	m_terimonos[Tetrimono_O] = LoadTexture("Assets/block_orange.png");
	m_terimonos[Tetrimono_S] = LoadTexture("Assets/block_red.png");
	m_terimonos[Tetrimono_T] = LoadTexture("Assets/block_violet.png");
	m_terimonos[Tetrimono_Z] = LoadTexture("Assets/block_yellow.png");
}

Game::Impl::~Impl() {
	if (!m_isSdlInitialized) return;

	if (m_isImgInitialized) {
		for (auto tex: m_terimonos) {
			if (tex) ::SDL_DestroyTexture(tex);
		}

		::IMG_Quit();
	}

	if (m_renderer) ::SDL_DestroyRenderer(m_renderer);
	if (m_window) ::SDL_DestroyWindow(m_window);		

	::SDL_Quit();
	CloseLog();
}

void Game::RunMainLoop() {
	Log("%s\n", "Running main loop...");
	m_impl->RunGameLoop();
}

void Game::Impl::RunGameLoop() {
	assert(m_isSdlInitialized);
	assert(m_window);
	assert(m_renderer);

	SDL_Event event;
	event.type = 0;

	Uint32 before, now;
	float elapsedInMs;
	bool running = true;

	before = ::SDL_GetTicks();
	while (running && event.type != SDL_QUIT) {
		while (::SDL_PollEvent(&event)) {	
			running = HandleKeyboardEvents(event);
		}

		now = ::SDL_GetTicks();
		elapsedInMs = static_cast<float>(now - before);

		Update(elapsedInMs);
		Render();
		before = now;
	}
}

bool Game::Impl::HandleKeyboardEvents(const SDL_Event& event) {
	bool running = true;

	if (event.type != SDL_KEYDOWN) return running;

	switch (event.key.keysym.sym) {
	case SDLK_LEFT:
		m_board.MoveCurrent(-1, 0); break;
	case SDLK_RIGHT:
		m_board.MoveCurrent(1, 0); break;
	case SDLK_UP:
		m_board.Rotate(RotateDirection_AntiClockwize);; break;
	case SDLK_DOWN:
		m_board.Rotate(RotateDirection_Clockwize); break;
	case SDLK_SPACE:
		m_board.FallDown(); break;
	case SDLK_ESCAPE:
		running = false; break;
	default: break;
	}

	return running;
}

void Game::Impl::Update(float ms) {
	m_board.Update(ms);
}

void Game::Impl::Render() {
	assert( ::SDL_RenderClear(m_renderer) == 0 );

	SDL_Rect dest;
	dest.w = BLOCK_WIDTH;
	dest.h = BLOCK_HEIGHT;
	for (int y = 0; y < m_board.HEIGHT; y++) {
		for (int x = 0; x < m_board.WIDTH; x++) {
			Tetrimonos type = m_board.GetAt(x, y);
			SDL_Texture* texture = m_terimonos[type];
			dest.x = m_boardPos.x + x * BLOCK_WIDTH;
			dest.y = m_boardPos.y + y * BLOCK_HEIGHT;

			::SDL_RenderCopy(m_renderer,
				texture, nullptr, &dest);
		}
	}	
	
	for (int y = 0; y < m_board.MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < m_board.MAX_TETRIMONO_WIDTH; x++) {
			Tetrimonos type = m_board.GetNextAt(x, y);
			SDL_Texture* texture = m_terimonos[type];

			dest.x = m_nextOffset.x + x * BLOCK_WIDTH;
			dest.y = m_nextOffset.y + y * BLOCK_HEIGHT;

			::SDL_RenderCopy(m_renderer,
				texture, nullptr, &dest);
		}
	}

	::SDL_RenderPresent(m_renderer);
}

SDL_Texture* Game::Impl::LoadTexture(const char* path) {
	SDL_Surface* inMem = ::IMG_Load(path);
	if (!inMem) {
		Log("LoadTexture(\"%s\"): %s\n", path, IMG_GetError());
		return nullptr;
	}

	SDL_Texture* inGpu = ::SDL_CreateTextureFromSurface(
		m_renderer, inMem);
	
	::SDL_FreeSurface(inMem);

	if (!inGpu) {
		Log("LoadTexture(\"%s\"): %s\n", path, SDL_GetError());
		return nullptr;
	}

	return inGpu;
}

#endif