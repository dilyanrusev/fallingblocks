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

enum Tetrimonos {
	Tetrimono_Empty = -1,
	Tetrimono_I,
	Tetrimono_J,
	Tetrimono_L,
	Tetrimono_O,
	Tetrimono_S,
	Tetrimono_T,
	Tetrimono_Z,
	Count_Tetrimonos
};

class Board {
public:
	Board();
	~Board();

	const int WIDTH;
	const int HEIGHT;
	const int MAX_TETRIMONO_WIDTH;
	const int MAX_TETRIMONO_HEIGHT;

private:
	Board(const Board&);
	Board& operator=(const Board&);

	void Spawn(Tetrimonos type);
	void EmptyCurrent();
	void CleanBoardAtCurrent();
	void RotateClockwize();
	void RotateAntiClockwize();
	
	int m_currentX;
	int m_currentY;
	Tetrimonos m_currentType;
	Tetrimonos m_current[4][4];

	Tetrimonos m_matrix[10][20];
};

