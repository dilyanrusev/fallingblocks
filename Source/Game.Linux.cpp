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

#include "SDL.h"
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
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	Board m_board;
	SDL_Texture* m_terimonos[Count_Tetrimonos];

	Impl()
			: m_isSdlInitialized(false) 
			, m_window(nullptr)
			, m_renderer(nullptr) {
		std::fill(begin(m_terimonos), end(m_terimonos), nullptr);
	}

	~Impl() {
		if (!m_isSdlInitialized) return;

		if (m_renderer) ::SDL_DestroyRenderer(m_renderer);
		if (m_window) ::SDL_DestroyWindow(m_window);

		::SDL_Quit();
	}

	void Initialize();
	void RunGameLoop();
	void Update(float ms);
	void Render();
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
}

void Game::RunMainLoop() {
	printf("%s\n", "Running main loop...");
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

	before = ::SDL_GetTicks();
	while (event.type != SDL_QUIT) {
		while (::SDL_PollEvent(&event)) {			
		}

		now = ::SDL_GetTicks();
		elapsedInMs = static_cast<float>(now - before);

		Update(elapsedInMs);
		Render();
		before = now;
	}
}

void Game::Impl::Update(float ms) {
	m_board.Update(ms);
}

void Game::Impl::Render() {
	assert( ::SDL_RenderClear(m_renderer) == 0 );

	::SDL_RenderPresent(m_renderer);
}

SDL_Texture* Game::Impl::LoadTexture(const char* path) {
	return nullptr;
}

#endif