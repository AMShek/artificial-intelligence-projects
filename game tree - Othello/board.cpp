#include "board.h"

//figure out all the valid move (can cause reversion)
//valid grids are recorded in parameter "mValid"
//the number of valid grid is returned
int cBoard::Valid(const cPlayer player, cMatrix<bool>& mValid){
	//make sure matrix "mValid" has correct size
	//valid actions are output via parameter mValid!
	assert(mValid.NumRow() == nROW);
	assert(mValid.NumCol() == nCOL);
	assert(nROW == nCOL);

	int nValid = 0;

	for (int r = 0; r<nROW; r++){
		for (int c = 0; c<nCOL; c++){
			//for each grid in (*this)

			//NOT valid by default
			mValid[r][c] = false;

			//if it is already occupied
			//this grid is NOT valid
			if ((*this)[r][c] != gBLANK) continue;

			//check whether this action causes reversion
			//if it does -> valid
			//if not -> NOT valid
			//there are 8 possible direction to cause reversion
			for (int y = -1; y <= 1; y++){
				//figure out the maximun number of steps
				int nMaxY;
				switch (y){
				//downward
				case  1: 
					nMaxY = nWIDTH - r - 1;	
					break;
				//upward
				case -1: 
					nMaxY = r;					
					break;
				//stay stil
				case  0: 
					nMaxY = nWIDTH;			
					break;
				}

				if (nMaxY == 0) continue;

				for (int x = -1; x <= 1; x++){
					if (x == 0 && y == 0) continue;

					//figure out the maximun number of steps
					int nMaxX;
					switch (x){
					//rightward
					case  1: 
						nMaxX = nWIDTH - c - 1;	
						break;
					//leftward
					case -1: 
						nMaxX = c;					
						break;
					//stay still
					case  0: 
						nMaxX = nWIDTH;			
						break;
					}

					if (nMaxX == 0) continue;

					int nMax;
					//nMax = min(nMaxX, nMaxY);
					if (nMaxX < nMaxY) nMax = nMaxX;
					else nMax = nMaxY;

					for (int i = 1; i <= nMax; i++){
						//get the color of piece (or blank) in this grid
						const cGrid piece = (*this)[r + i * y][c + i * x];

						//if any piece is blank, then this direction fail
						if (piece == gBLANK) break;

						//first piece must not be the same color
						if (i == 1){
							if (piece == player) break;
							else continue;
						}

						if (piece == player){
							mValid[r][c] = true;
							nValid++;
							break;
						}
					}

					//if it's already valid, no more checking
					if (mValid[r][c] == true) break;
				}

				//if it's already valid, no more checking
				if (mValid[r][c] == true) break;
			}
		}
	}

	return nValid;
}

int cBoard::Move(const cPlayer player, const int r, const int c, cMatrix<bool>& mUndo){
	//make sure input are within range
	assert(r >= 0 && c >= 0);
	assert(r < nROW && c < nCOL);

	//make sure the matrix has the correct dimention
	assert(mUndo.NumRow() == nROW);
	assert(mUndo.NumCol() == nCOL);
	assert(nROW == nCOL);

	//make sure the location is blank
	assert((*this)[r][c] == gBLANK);

	//reset the undo matrix to blank
	mUndo.Reset(false);

	int nReverse = 0;

	//this section is very similar to valid, I was thinking about merge this section in valid
	//	however, for ease of debugging, I decide to put it here.
	for (int y = -1; y <= 1; y++){
		//figure out the maximun number of steps
		int nMaxY;
		switch (y){
		case  1: nMaxY = nWIDTH - r - 1;	break;
		case -1: nMaxY = r;					break;
		case  0: nMaxY = nWIDTH;			break;
		}

		if (nMaxY == 0) continue;

		for (int x = -1; x <= 1; x++){
			if (x == 0 && y == 0) continue;

			//figure out the maximun number of steps
			int nMaxX;
			switch (x){
			case  1: nMaxX = nWIDTH - c - 1;	break;
			case -1: nMaxX = c;					break;
			case  0: nMaxX = nWIDTH;			break;
			}

			if (nMaxX == 0) continue;

			int nMax;
			//nMax = min(nMaxX, nMaxY);
			if (nMaxX < nMaxY) nMax = nMaxX;
			else nMax = nMaxY;

			for (int i = 1; i <= nMax; i++)	{
				const cGrid piece = (*this)[r + i * y][c + i * x];

				//if any piece is blank, then this direction fail
				if (piece == gBLANK) break;

				//first piece must not be the same color
				if (i == 1){
					if (piece == player) break;
					else continue;
				}

				if (piece == player){
					//make the capture
					for (int cap = 1; cap < i; cap++){
						const int nR = r + cap * y;
						const int nC = c + cap * x;

						//capture the piece, update undo
						Reverse(nR, nC);
						mUndo[nR][nC] = true;
						nReverse++;
					}

					break;
				}
			}
		}
	}

	return nReverse;
}

bool cBoard::MakeMove(const cPlayer player, const int r, const int c){
	//if it is not blank, this is not a valid move
	if ((*this)[r][c] != gBLANK) return false;

	//create the undo matrix
	cMatrix<bool> mUndo;
	mUndo.Resize(nROW, nCOL, false);

	//makes a capture
	int nReverse = Move(player, r, c, mUndo);

	//if there is no capture, this is not a valid move
	if (nReverse == 0)
	{
		//no capture, no need to undo
		//Undo(mUndo);
		return false;
	}

	//at this point, we knov this move is valid, so put a piece down
	(*this)[r][c] = player;

	return true;
}

void cBoard::Undo(const cMatrix<bool>& mUndo)
{
	for (int r = 0; r<nROW; r++)
	{
		for (int c = 0; c<nCOL; c++)
		{
			if (mUndo[r][c] == true)
			{
				if ((*this)[r][c] == gBLACK) (*this)[r][c] = gWHITE;
				else if ((*this)[r][c] == gWHITE) (*this)[r][c] = gBLACK;
				else assert(false);
			}
		}
	}
}

ostream& operator<<(ostream& out, const cBoard& board)
{
	const int nLength = board.size() * 2 + 2;
	int l;

	//output the first line on top
	cout << '/' << '|';
	for (l = 0; l<board.size(); l++) cout << l;
	cout << '|';
	cout << endl;

	//output the second line
	cout << '-' << '+';
	for (l = 0; l<board.size(); l++) cout << '-';
	cout << '+';
	cout << endl;

	//output the actual board
	for (int r = 0; r<board.size(); r++)
	{
		cout << r << '|';
		for (int c = 0; c<board[r].size(); c++)
		{
			cout << board[r][c];
		}
		cout << '|' << endl;
	}

	//output the line on bottom
	cout << '-' << '+';
	for (l = 0; l<board.size(); l++) cout << '-';
	cout << '+';
	cout << endl;

	return out;
}

