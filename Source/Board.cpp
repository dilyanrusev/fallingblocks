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
#include <algorithm>

using namespace std;

Board::Board()
		: WIDTH(10)
		, HEIGHT(20)
		, MAX_TETRIMONO_WIDTH(4)
		, MAX_TETRIMONO_HEIGHT(4)
		, m_currentX(0)
		, m_currentY(0)		
		, m_currentType(Tetrimono_Empty)
		, m_randomDistributor(Tetrimono_I, Tetrimono_Z) {	
	Empty(m_matrix);
	m_nextType = static_cast<Tetrimonos>(m_randomDistributor(m_randomGenerator));
	GetMatrixFor(m_nextType, m_next);
	SpawnNext();
}

Board::~Board() {
}

void Board::Empty(ArrayTetrimonos4x4& matrix) const {
	for (int y = 0; y < MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < MAX_TETRIMONO_WIDTH; x++) {
			matrix[y][x] = Tetrimono_Empty;
		}
	}
}

void Board::Empty(ArrayTetrimonos10x20& matrix) const {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			matrix[y][x] = Tetrimono_Empty;
		}
	}
}

void Board::CleanBoardAtCurrent() {
	int boardX, boardY;
	for (int y = 0; y < MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < MAX_TETRIMONO_WIDTH; x++) {
			boardX = m_currentX + x;
			boardY = m_currentY + y;
			if (boardX >= 0 && boardY >= 0 && m_current[y][x] != Tetrimono_Empty) {
				m_matrix[boardY][boardX] = Tetrimono_Empty;
			}			
		}
	}
}

void Board::GetMatrixFor(Tetrimonos type, ArrayTetrimonos4x4& matrix) const {
	Empty(matrix);
	
	switch (type) {
	case Tetrimono_I:
		matrix[0][1] = type;
		matrix[1][1] = type;
		matrix[2][1] = type;
		matrix[3][1] = type;	
		break;
	case Tetrimono_J:
		matrix[0][2] = type;
		matrix[1][2] = type;
		matrix[2][2] = type;
		matrix[2][1] = type;		
		break;
	case Tetrimono_L:
		matrix[0][1] = type;
		matrix[1][1] = type;
		matrix[2][1] = type;
		matrix[2][2] = type;		
		break;
	case Tetrimono_O:
		matrix[0][1] = type;
		matrix[0][2] = type;
		matrix[1][1] = type;
		matrix[1][2] = type;		
		break;
	case Tetrimono_S:
		matrix[0][2] = type;
		matrix[1][0] = type;
		matrix[0][1] = type;
		matrix[1][1] = type;		
		break;
	case Tetrimono_T:
		matrix[0][0] = type;
		matrix[0][1] = type;
		matrix[0][2] = type;
		matrix[1][1] = type;		
		break;
	case Tetrimono_Z:
		matrix[0][0] = type;
		matrix[0][1] = type;
		matrix[1][1] = type;
		matrix[1][2] = type;		
		break;
	default:
		assert(false);
		break;
	}
}

void Board::Spawn(Tetrimonos type) {
	int currentWidth = 0;
	int currentHeight = 0;
	GetMatrixFor(type, m_current);

	switch (type) {
	case Tetrimono_I:				
		currentWidth = 1;
		currentHeight = 4;
		break;
	case Tetrimono_J:		
		currentWidth = 2;
		currentHeight = 3;
		break;
	case Tetrimono_L:		
		currentWidth = 2;
		currentHeight = 3;
		break;
	case Tetrimono_O:
		currentWidth = 2;
		currentHeight = 2;
		break;
	case Tetrimono_S:		
		currentWidth = 3;
		currentHeight = 2;
		break;
	case Tetrimono_T:		
		currentWidth = 3;
		currentHeight = 2;
		break;
	case Tetrimono_Z:		
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

MergeResult Board::SpawnNext() {
	Spawn(m_nextType);
	
	ArrayTetrimonos10x20 merged;
	MergeResult mergeResult = MergeCurrent(merged);
	std::copy(begin(merged), end(merged), begin(m_matrix));
	
	m_nextType = static_cast<Tetrimonos>(m_randomDistributor(m_randomGenerator));
	GetMatrixFor(m_nextType, m_next);
	
	return mergeResult;
}

MergeResult Board::MergeCurrent(ArrayTetrimonos10x20& result) const {
	MergeResult mergeResult = MergeResult_OK;
	std::copy(begin(m_matrix), end(m_matrix), begin(result));

	int boardX, boardY;	

	for (int y = 0; y < MAX_TETRIMONO_HEIGHT; y++) {
		for (int x = 0; x < MAX_TETRIMONO_WIDTH; x++) {
			boardX = m_currentX + x;
			boardY = m_currentY + y;
			if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT) {
				mergeResult = MergeResult_Conflict;
				continue;
			} else if (boardY < 0) {
				// going out from the top is not conflict by design
				continue;
			}

			const Tetrimonos& source = m_current[y][x];
			Tetrimonos& target = result[boardY][boardX];
			
			if (target == Tetrimono_Empty) {
				target = source;
			} else {
				mergeResult = MergeResult_Conflict;
			}
		}
	}
	return mergeResult;
}

MergeResult Board::MoveCurrentDown() {	
	m_currentY++;
	ArrayTetrimonos10x20 mergedMatrix;
	MergeResult res = MergeCurrent(mergedMatrix);
	if (res == MergeResult_OK) {
		std::copy(begin(mergedMatrix), end(mergedMatrix), begin(m_matrix));
	} else {
		m_currentY--;
	}

	return res;
}