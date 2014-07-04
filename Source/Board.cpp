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
#include "Board.h"
#include <cassert>

Board::Board()
		: WIDTH(20)
		, HEIGHT(10)
		, MAX_TETRIMONO_WIDTH(4)
		, MAX_TETRIMONO_HEIGHT(4)
		, m_currentX(0)
		, m_currentY(0)		
		, m_currentType(Tetrimono_Empty) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			m_matrix[y][x] = Tetrimono_Empty;
		}
	}

	for (int y = 0; y < MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < MAX_TETRIMONO_WIDTH; x++) {
			m_current[y][x] = Tetrimono_Empty;
		}
	}
}

Board::~Board() {
}

void Board::EmptyCurrent() {
	for (int y = 0; y < MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < MAX_TETRIMONO_WIDTH; x++) {
			m_current[y][x] = Tetrimono_Empty;
		}
	}
}

void Board::CleanBoardAtCurrent() {
	int boardX, boardY;
	for (int y = 0; y < MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < MAX_TETRIMONO_WIDTH; x++) {
			boardX = m_currentX + x;
			boardY = m_currentY + y;
			if (m_current[y][x] != Tetrimono_Empty) {
				m_matrix[boardY][boardX] = Tetrimono_Empty;
			}			
		}
	}
}

void Board::Spawn(Tetrimonos type) {
	EmptyCurrent();

	int currentWidth = 0;
	int currentHeight = 0;

	switch (type) {
	case Tetrimono_I:		
		m_current[0][1] = type;
		m_current[1][1] = type;
		m_current[2][1] = type;
		m_current[3][1] = type;
		currentWidth = 1;
		currentHeight = 4;
		break;

	case Tetrimono_J:
		m_current[0][2] = type;
		m_current[1][2] = type;
		m_current[2][2] = type;
		m_current[2][1] = type;
		currentWidth = 2;
		currentHeight = 3;
		break;

	case Tetrimono_L:
		m_current[0][1] = type;
		m_current[1][1] = type;
		m_current[2][1] = type;
		m_current[2][2] = type;
		currentWidth = 2;
		currentHeight = 3;
		break;

	case Tetrimono_O:
		m_current[0][1] = type;
		m_current[0][2] = type;
		m_current[1][1] = type;
		m_current[1][2] = type;
		currentWidth = 2;
		currentHeight = 2;
		break;

	case Tetrimono_S:
		m_current[0][1] = type;
		m_current[0][2] = type;
		m_current[1][0] = type;
		m_current[1][1] = type;
		currentWidth = 3;
		currentHeight = 2;
		break;

	case Tetrimono_T:
		m_current[0][0] = type;
		m_current[0][1] = type;
		m_current[0][2] = type;
		m_current[1][1] = type;
		currentWidth = 3;
		currentHeight = 2;
		break;

	case Tetrimono_Z:
		m_current[0][0] = type;
		m_current[0][1] = type;
		m_current[1][1] = type;
		m_current[1][2] = type;
		currentWidth = 3;
		currentHeight = 2;
		break;

	default:
		assert(false);
		break;
	}

	m_currentType = type;	
	m_currentX = (WIDTH - currentWidth) / 2;
	m_currentY = -currentHeight;
}
