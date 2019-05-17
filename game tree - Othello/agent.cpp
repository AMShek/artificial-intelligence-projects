#include "agent.h"
#include <iomanip>
#include <Windows.h>
using namespace std;

//piece values
cMatrix<double> InitGrid(cMatrix<double> input)
{
	cMatrix<double> result = input;
	result.Resize(nROW, nCOL, 0);

	if (input.size() == 0)
	{

		// for eval1
		result[0][0] = 25.7957;
		result[1][0] = -5.72912; result[1][1] = -9.23786;
		result[2][0] = -1.32907; result[2][1] = -2.66268; result[2][2] = 0.481184;
		//result[3][0] = 8.42829; result[3][1] = -3.63214; result[3][2] = 0.544214; result[3][3] = 0.538515;
		//
	}

	int r, c;

	//get the corner, i.e. 4x4 matrix filled
	for (r = 0; r<nROW / 2; r++)
		for (c = r + 1; c<nCOL / 2; c++)
			result[r][c] = result[c][r];

	//get the left half filled
	for (r = nROW / 2; r<nROW; r++)
		for (c = 0; c<nCOL / 2; c++)
			result[r][c] = result[nROW - r - 1][c];

	//get the entire filled
	for (r = 0; r<nROW; r++)
		for (c = nCOL / 2; c<nCOL; c++)
			result[r][c] = result[r][nCOL - c - 1];

	return result;
}
//piece values v2
cMatrix<double> InitGrid2(cMatrix<double> input)
{
	cMatrix<double> result = input;
	result.Resize(nROW, nCOL, 0);

	if (input.size() == 0)
	{

		/*
		//for eval2
		result[0][0] =    25.1309;
		result[1][0] =   -5.82591; result[1][1] =   -9.42345;
		result[2][0] =   -1.32907; result[2][1] =   -2.54628; result[2][2] =  0.0623483;
		result[3][0] =    8.86331; result[3][1] =   -3.46086; result[3][2] =   0.792597; result[3][3] =   0.820598;
		*/

		result[0][0] = 25.1309;
		result[1][0] = -4.66327; result[1][1] = -9.3608;
		result[2][0] = -1.98504; result[2][1] = -6.96296; result[2][2] = 0.151777;
		//result[3][0] = 11.7463; result[3][1] = -6.10546; result[3][2] = -0.234369; result[3][3] = -1.78741;
		//TIME: 1536218
	}

	int r, c;

	//get the corner, i.e. 4x4 matrix filled
	for (r = 0; r<nROW / 2; r++)
		for (c = r + 1; c<nCOL / 2; c++)
			result[r][c] = result[c][r];

	//get the left half filled
	for (r = nROW / 2; r<nROW; r++)
		for (c = 0; c<nCOL / 2; c++)
			result[r][c] = result[nROW - r - 1][c];

	//get the entire filled
	for (r = 0; r<nROW; r++)
		for (c = nCOL / 2; c<nCOL; c++)
			result[r][c] = result[r][nCOL - c - 1];

	return result;
}
//piece values v3
cMatrix<double> InitGrid3(cMatrix<double> input)
{
	cMatrix<double> result = input;
	result.Resize(nROW, nCOL, 0);

	if (input.size() == 0)
	{
		result[0][0] = 27.0643;
		result[1][0] = -4.50207; result[1][1] = -16.6209;
		result[2][0] = -4.94808; result[2][1] = -12.8019; result[2][2] = 5.02245;
		//result[3][0] = 14.6643; result[3][1] = -11.8225; result[3][2] = -0.250991; result[3][3] = -0.785909;
		//TIME: 436609

		/*
		result[0][0] =    23.2344;
		result[1][0] =   -3.69439; result[1][1] =   -17.2441;
		result[2][0] =   -5.10631; result[2][1] =   -10.6033; result[2][2] =   -1.63993;
		result[3][0] =    19.0306; result[3][1] =   -7.73482; result[3][2] =    6.50128; result[3][3] =    1.44682;
		//TIME: 276172
		*/

		/*
		//for corner bonus added
		result[0][0] =    27.0323;
		result[1][0] =   -4.85872; result[1][1] =   -12.7205;
		result[2][0] =   -4.53146; result[2][1] =   -11.6486; result[2][2] =    1.99817;
		result[3][0] =    13.0726; result[3][1] =   -14.9568; result[3][2] =   -2.27627; result[3][3] =    1.56109;
		//TIME: 12516
		*/
	}

	int r, c;

	//get the corner, i.e. 4x4 matrix filled
	for (r = 0; r<nROW / 2; r++)
		for (c = r + 1; c<nCOL / 2; c++)
			result[r][c] = result[c][r];

	//get the left half filled
	for (r = nROW / 2; r<nROW; r++)
		for (c = 0; c<nCOL / 2; c++)
			result[r][c] = result[nROW - r - 1][c];

	//get the entire filled
	for (r = 0; r<nROW; r++)
		for (c = nCOL / 2; c<nCOL; c++)
			result[r][c] = result[r][nCOL - c - 1];

	return result;
}

//generate random value in (min, max)
double Rand(double min, double max){
	int r = rand();
	if (r > 32003){
		return Rand(min, max);
	}
	else{
		double ratio = (double(r) / 32003.0);
		return min + (max - min) * ratio;
	}
}

//train to find an ideal initial grid
//namely more precise PIECE VALUES
bool cGame::SearchGrid(const clock_t duration){
	//make mGrid2 slight different from mGrid1
	mGrid2 = mGrid1;

	//randomly choose a vertex to be change
	//NOTE: this favor change in corner over center
	int r = rand() % nROW / 2;
	int c = rand() % (r + 1);

	//make mGrid2 symmetrical
	const double RANGE = 1.0;
	mGrid2[r][c] += Rand(-RANGE, RANGE);
	mGrid2 = InitGrid(mGrid2);

	//test which one is better

	//play the first game
	Load("new.txt");
	for (;;){
		if (gameOver == true) break;

		//ask comp to decide of move
		pair<int, int> move;
		if (player == gBLACK) move = Play1();
		else if (player == gWHITE) move = Play2();
		else assert(false);

		//make the move
		MakeMove(move.first, move.second);
	}

	//if tie, well, count as win, since I want to favor original
	bool win1 = (board.Count(gBLACK) >= board.Count(gWHITE));
	//OutGame();

	//please the second game
	//play the first game
	Load("new.txt");
	for (;;){
		if (gameOver == true) break;

		//ask comp to decide of move
		pair<int, int> move;
		if (player == gWHITE) move = Play1();
		else if (player == gBLACK) move = Play2();
		else assert(false);

		//make the move
		MakeMove(move.first, move.second);
	}

	//if tie, well, count as win, since I want to favor original
	bool win2 = (board.Count(gWHITE) >= board.Count(gBLACK));
	//OutGame();

	//change only if orginal lost both game
	if (!win1 && !win2){
		mGrid1 = mGrid2;
	}
	else{
		mGrid2 = mGrid1;
	}

	if (!win1 && !win2){
		//output the grid to screen and file
		ofstream oGrid("grid.txt", ios::app);

		oGrid << "TIME: " << duration << endl;

		for (int i = 0; i<nROW / 2; i++){
			oGrid << '\t';
			for (int j = 0; j <= i; j++){
				//cout << "result[" << i << "]" << "[" << j << "] = ";
				oGrid << "result[" << i << "]" << "[" << j << "] = ";

				cout << setw(10) << setprecision(6) << mGrid1[i][j] << "; ";
				oGrid << setw(10) << setprecision(6) << mGrid1[i][j] << "; ";
			}
			cout << endl;
			oGrid << endl;
		}
		oGrid.close();

		return true;
	}

	return false;
}

void cGame::Endgame(){
	gameOver = true;
	player = gBLANK;
}

//switch player to take action
bool cGame::Switch(){
	if (player == gBLANK){
		Endgame();
		return false;
	}

	//switch the player
	if (player == gBLACK) player = gWHITE;
	else if (player == gWHITE) player = gBLACK;
	else assert(false);

	//check whether there is still valid actions
	cMatrix<bool> mValid;
	mValid.Resize(nROW, nCOL, false);
	int nValid = board.Valid(player, mValid);

	if (nValid == 0){
		//switch the player again
		if (player == gBLACK) player = gWHITE;
		else if (player == gWHITE) player = gBLACK;
		else assert(false);

		nValid = board.Valid(player, mValid);

		//if both player can't move
		//game is over
		if (nValid == 0){
			Endgame();
			return false;
		}
	}

	return true;
}

//make a move at grid (r,c)
bool cGame::MakeMove(int r, int c){
	//invalid grid
	if (r < 0 ||r >= nROW)
		return false;
	//invalid grid
	else if ( c < 0 || c >= nROW) 
		return false;
	else if (gameOver) 
		return false;
	else if (!board.MakeMove(player, r, c))
		return false;
	else{
		//figure out the index for history
		int n = nROW * nCOL - board.Count(gBLANK);
		n -= 4; //since we start with 4 pieces
		n -= 1; //to account of the additional piece that is placed

		int curindex = n;

		if (history.size() < curindex)
		{
			//incomplete history
			assert(false);
		}
		else
		{
			if (history.size() > curindex)
			{
				if (history[curindex].r == r &&
					history[curindex].c == c &&
					history[curindex].p == player)
				{
					Switch();
					return true;
					//do nothing
				}
				else //if this is a new move
				{
					history.erase(history.begin() + curindex, history.end());
				}
			}

			//if it gets to this point, need item should be added
			assert(history.size() == curindex);

			//record info for undo
			cAction act;
			act.r = r;
			act.c = c;
			act.p = player;

			history.push_back(act);

		}

		Switch();
		return true;
	}
}

pair<int, int> cGame::LastMove(void)
{
	pair<int, int> bad(-1, -1);

	//figure out the number of moves made
	int n = nROW * nCOL - board.Count(gBLANK); n -= 4;
	if (history.size() < n) return bad;

	if (n == 0) return bad;

	return pair<int, int>(history[n - 1].r, history[n - 1].c);
}

bool cGame::Undo(cPlayer p)
{
	//if the history is missing (possibly due to previous version
	if (history.size() + 4 < board.Count(gBLACK) + board.Count(gWHITE)) return false;

	//figure out the index for undo
	int n = nROW * nCOL - board.Count(gBLANK);
	n -= 4;

	//last entry for undo
	int last = (n - 1) - 1;
	int index = -2; //-1 is reserved for new game

	if (p == gBLANK) index = last;
	else
	{
		for (int i = last; i >= 0; i--)
		{
			if (history[i].p == p)
			{
				index = i - 1;
				break;
			}
		}
	}

	if (index <= -2) return false; //undo fail
	else
	{
		//since history will be deleted when game reset
		vector<cAction> backup = history;

		if (index == -1) NewGame();
		else
		{

			//reset the game
			NewGame();
			for (int m = 0; m <= index; m++)
			{
				if (!MakeMove(backup[m].r, backup[m].c))
				{
					assert(false);
				}
			}
		}

		//copy the history back
		history = backup;

		return true;
	}
}

bool cGame::Redo(cPlayer p)
{
	//if the history is missing (possibly due to previous version
	if (history.size() + 4 < board.Count(gBLACK) + board.Count(gWHITE)) return false;

	//figure out the index for redo
	int n = nROW * nCOL - board.Count(gBLANK);
	n -= 4;

	//the first entry for redo
	int first = (n - 1) + 1;
	int index = nROW * nCOL;

	if (p == gBLANK) index = first;
	else
	{
		for (int i = first;; i++)
		{
			int h = history.size();

			//if index i is too big
			if (history.size() < i) break;

			//if end is reached, just stop at the previous move
			if (history.size() == i)
			{
				index = i - 1;
			}

			//unless there is no history to redo
			if (first > index) return false;

			if (history[i].p == p)
			{
				//if you found a move that made by you, stop here
				index = i;
				break;
			}
		}
	}

	//if the resulting index is too big
	if (index >= history.size()) return false; //undo fail
	else
	{
		//make the moves describled in history
		for (int f = first; f <= index; f++)
		{
			int r = history[f].r;
			int c = history[f].c;

			bool suc = MakeMove(r, c);
			assert(suc);
		}

		return true;
	}
}

//output the game
void cGame::OutGame()
{
	if (gameOver)
	{
		int nBlack = board.Count(gBLACK);
		int nWhite = board.Count(gWHITE);

		if (nBlack > nWhite) cout << gBLACK << " wins!" << endl;
		else if (nWhite > nBlack) cout << gWHITE << " wins!" << endl;
		else
		{
			cout << "tie!" << endl;
		}
	}

	cout << "Player: " << player << endl;

	int nBlack = board.Count(gBLACK);
	int nWhite = board.Count(gWHITE);

	cout << gBLACK << " : " << nBlack << endl;
	cout << gWHITE << " : " << nWhite << endl;

	cout << endl;
	cout << board;
}

//load state from file
bool cGame::Load(string fn)
{
	ifstream in(fn.c_str());

	if (in.fail())
	{
		cout << "Unable to open file " << fn << endl;
		return false;
	}

	char ch;

	//get the player
	in >> ch;
	if (ch == 'x' || ch == 'X') player = gBLACK;
	else if (ch == 'o' || ch == 'O') player = gWHITE;
	else if (ch == '-')	player = gBLANK;
	else return false;

	//get the board
	for (int r = 0; r<nROW; r++)
	{
		for (int c = 0; c<nCOL; c++)
		{
			in >> ch;

			if (ch == 'x' || ch == 'X') board[r][c] = gBLACK;
			else if (ch == 'o' || ch == 'O') board[r][c] = gWHITE;
			else board[r][c] = gBLANK;
		}
	}

	//erase the original history
	history.erase(history.begin(), history.end());

	//read in the history (if exist)
	int hSize;
	in >> hSize;

	if (!in.eof())
	{
		for (int h = 0; h<hSize; h++)
		{
			//read in location
			int r, c;
			in >> r >> c;

			//read in player
			cPlayer p;
			in >> ch;
			if (ch == 'x' || ch == 'X') p = gBLACK;
			else if (ch == 'o' || ch == 'O') p = gWHITE;
			else return false;

			//set the action appropriately
			cAction act;
			act.r = r;
			act.c = c;
			act.p = p;

			//add to the database
			history.push_back(act);
		}
	}

	//set the endgame flag
	gameOver = false;

	//switch twice to handle the situation when loaded player can not move
	//	or the game ends
	Switch();
	if (!gameOver) Switch();

	return true;
}

//save current state and previous actions to file
bool cGame::Save(string fn)
{
	ofstream out(fn.c_str());

	//save the player
	out << player << endl;

	//save the game
	for (int r = 0; r<nROW; r++)
	{
		for (int c = 0; c<nCOL; c++)
		{
			if (board[r][c] == gBLANK) out << '-';
			else out << board[r][c];
		}
		out << endl;
	}

	//NOTE that below has all the necessary info to regenerate the game, but above is used for ease of reading file
	//even if below are incorrect or ignored, the file still will load properly
	//save the movement history
	out << history.size() << endl;
	for (int h = 0; h<history.size(); h++)
	{
		out << history[h].r << ' ' << history[h].c << ' ' << history[h].p << endl;
	}

	out.close();
	return true;
}

//choose and return a reasonable depth
int cGame::Depth(){
	//count up the number of pieces
	int nBlack = board.Count(gBLACK);
	int nWhite = board.Count(gWHITE);
	int depth;

	if (nBlack + nWhite < 10) 
		depth = nDepth + 1;
	else if (nBlack + nWhite > 56) 
		depth = nDepth + 8;
	else if (nBlack + nWhite > 52) 
		depth = nDepth + 4;
	else if (nBlack + nWhite > 48) 
		depth = nDepth + 2;
	else if (nBlack + nWhite > 44) 
		depth = nDepth + 1;
	else depth = nDepth;

	return depth;
}

//Comp player 1 plays
//using evaluation function 2
pair<int, int> cGame::Play1(){
	cMatrix<bool> mValid;
	mValid.Resize(nROW, nCOL, false);
	int r = -1, c = -1;
	int depth = Depth();
	//note
	//cout << "Initial depth = " << depth << endl;

	double alpha = -INFINITY;
	double beta = +INFINITY; 
	int searchtimes = 0;

	double nScore = (*fMiniMax1)(player, board, depth, mGrid1, mValid, fEval1,alpha,beta,r,c,searchtimes);
	cout << endl << "Evaluation Function #2 " << nScore<<endl;
	cout << "r=" << r << endl;
	cout << "c=" << c << endl;

	return pair<int, int>(r, c);
}

//Comp player 2 plays
//using evaluation function 3
pair<int, int> cGame::Play2(){
	cMatrix<bool> mValid;
	mValid.Resize(nROW, nCOL, false);
	int r = -1, c = -1;
	//note!!!
	double alpha = -INFINITY;
	double beta = +INFINITY;
	int searchtimes = 0;
	int depth = Depth();
	double nScore = (*fMiniMax2)(player, board, depth, mGrid2, mValid, fEval2,alpha,beta, r, c,searchtimes);
	cout <<"Evaluation Function #3 " << nScore << endl;
	cout << "r=" << r << endl;
	cout << "c=" << c << endl;
	return pair<int, int>(r, c);
}

//note
//combination of 1 and 2
pair<int, int> cGame::Play3(){
	cMatrix<bool> mValid;
	mValid.Resize(nROW, nCOL, false);
	int r = -1, c = -1;
	//note!!!
	double alpha = -INFINITY;
	double beta = +INFINITY;
	int searchtimes = 0;
	int depth = Depth();
	double nScore = (*fMiniMax1)(player, board, depth, mGrid1, mValid, fEval1,alpha,beta, r, c,searchtimes);
	//if it's a dangerous move, let decision 2 decide
	if (mGrid1[r][c] == mGrid1[0][1] ||
		mGrid1[r][c] == mGrid1[1][1]){
		double nScore = (*fMiniMax2)(player, board, depth, mGrid2, mValid, fEval2,alpha,beta, r, c,searchtimes);
	}
	cout << "Combined Evaluation Function " << nScore << endl;
	cout << "r=" << r << endl;
	cout << "c=" << c << endl;
	return pair<int, int>(r, c);
}

//Depth-First implementation of MiniMax
//alpha-beta pruned
double MiniMax(const cPlayer player, cBoard& board, int depth, cMatrix<double>& mGrid, \
	cMatrix<bool>& mValid, cStrat* eval, double alpha, double beta,  int& r_out, int& c_out,int& searchtimes){
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	if (depth == 0){
		//boundary condition 1
		//reach deepest search depth
		return (*eval)(player, board, mGrid, mValid);
	}
	//if the depth is not 0
	//do search to find next action
	else{
		//figure out the next player
		//i need to MINIMIZE the next player's payoff
		cPlayer next;
		if (player == gBLACK) next = gWHITE;
		else if (player == gWHITE) next = gBLACK;

		//vector "moves" includes all my possible actions
		vector<cMove> moves = GetOrderMove(player, board, mValid, mGrid);

		//if there is not any valid action
		if (moves.size() == 0){
			int nMove = board.Valid(next, mValid);

			//boundary condition 2
			//no valid move -> evaluate utility using uDiff
			if (nMove != 0){
				return -MiniMax(next, board, depth, mGrid, mValid, eval, alpha, beta,r_out, c_out, searchtimes);
			}
			else{
				//this part assumes that r_out and c_out has already been set
				int nPlayer = board.Count(player);
				int nOther = board.Count(next);
				int nDiff = nPlayer - nOther;

				//divide by 36 to avoid overflow
				return INFINITY / (nROW * nCOL) * nDiff;
			}
		}

		//if there is some valid action(s)
		cMove mBest(-1, -1, -1);
		double nScore = -INFINITY * 2;
		for (int m = 0; m<moves.size(); m++){

			int r = moves[m].r;
			int c = moves[m].c;
			//note
			//cout<<"Tring ("<<r<<","<<c<<")!"<<endl;

			//make the move described by moves[m];
			cMatrix<bool> mUndo;
			mUndo.Resize(nROW, nCOL, false);
			board.Move(player, r, c, mUndo);
			board.Set(r, c, player);


			//figure out the result
			double result = -MiniMax(next, board, depth - 1, mGrid, mValid, eval,alpha, beta, r_out, c_out,searchtimes) ;
			searchtimes++;

			//note
			/*
			cout << "alpha = " << alpha << endl;
			cout << "beta = " << beta << endl;
			cout << "result = " << result << endl;
			//！！！！！！！！！！！！！！！！！！！！
			cout << "alpha = " << alpha << endl;
			cout << "beta = " << beta << endl;
			cout << "result = " << result << endl;
			cout << "depth = " << depth << endl;
			cout << "r = " << r << endl;
			cout << "c = " << c << endl;
			*/

			if (result > nScore){
				nScore = result;
				mBest = moves[m];
			}

			//alpha-beta pruning
			
			alpha = (alpha >= result) ? alpha : result;
			beta = (beta <= result) ? beta : result;
			if (beta <= alpha) {
				//note
				//cout << "Alpha-beta pruned here!" << endl;
				//cout << endl << endl;
				board.Remove(r, c);
				board.Undo(mUndo);
				break;
			}
			

			//undo the move
			board.Remove(r, c);
			board.Undo(mUndo);
		}

		r_out = mBest.r;
		c_out = mBest.c;
		//cout << "r_out = " << r_out << endl;
		//cout << "c_out = " << c_out << endl;
		QueryPerformanceCounter(&t2);
		//printf("Search Time = %f\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
		//printf("Nodes explored = %d\n", searchtimes);
		return nScore;
	}

}


//sort by priority descendingly
bool CompareMove(const cMove& m1, const cMove& m2){
	return (m1.priority > m2.priority);
}

//sort valid actions recorded in "mValid" by priority descendingly
vector<cMove> GetOrderMove(const cPlayer& player, cBoard& board, \
	cMatrix<bool>& mValid, cMatrix<double>& mGrid){
	vector<cMove> result;

	assert(mGrid.size() == board.size());

	int nValid = board.Valid(player, mValid);

	for (int i = 0; i<board.size(); i++){
		//make sure the board is a SQUARE
		assert(board.size() == board[i].size());
		//make sure the board has correct SIZE
		assert(mGrid[i].size() == board[i].size());

		//record valid actions in vector "result"
		for (int j = 0; j<board[i].size(); j++){
			if (mValid[i][j] == true){
				result.push_back(cMove(i, j, mGrid[i][j]));
			}
		}
	}
	//sort by priority descendingly
	sort(result.begin(), result.end(), CompareMove);

	//double check if I sorted in the right order
	//assert(result.size() < 2 || result[0].priority >= result[1].priority);
	if (!(result.size() < 2 || result[0].priority >= result[1].priority)){
		for (int m = 0; m<result.size(); m++){
			cout << result[m].r << ' ' << result[m].c << ' ' << result[m].priority << endl;
		}
	}


	return result;
}

//count up the mobility score
int Mobility(const cPlayer player, cBoard& board, cMatrix<bool>& mValid){
	int result = board.Valid(player, mValid);
	return result;
}

double PieceValue(const cPlayer player, cBoard& board, cMatrix<double>& mGrid)
{
	double result = 0;
	int r, c;
	for (r = 0; r<nROW; r++)
	{
		for (c = 0; c<nCOL; c++)
		{
			if (board[r][c] == player)
			{
				result += mGrid[r][c];
			}
		}
	}

	//add extra negative for corner loc
	for (int i = 0; i <= 4; i++)
	{
		int x, y;
		switch (i)
		{
		case 0: r = 0; c = 0; x = 1; y = 1; break;
		case 1: r = nROW - 1, c = nCOL - 1; x = -1; y = -1; break;
		case 2: r = 0; c = nCOL - 1; x = -1; y = 1; break;
		case 4: r = nROW - 1; c = 0; x = 1; y = -1; break;
		}

		if (board[r][c] == gBLANK)
		{

			//triple the negative for occupying those place diagonally adj to corner
			//since there is only 1 constant, the program should still do fine the "optimial"
			const double MUTIPLIER = 3.0;
			if (board[r + y][c + x] == player)
			{
				result += MUTIPLIER * mGrid[r + y][c + x];
			}

			//don't subtract the adj corner piece if the adj edge piece is player's
			if (board[r + y][c] == player && board[r + 2 * y][c] == player)
			{
				result -= mGrid[r + y][c];
			}

			if (board[r][c + x] == player && board[r][c + 2 * x] == player)
			{
				result -= mGrid[r][c + x];
			}
			/*
			//if you press diagonal, you are as good as losing the corner
			if (board[r+y][c+x] == player)
			{
			result -= mGrid[r][c];
			}

			//similary, if you have an edge next to diagonal, i.e _xxx_x__ this is really bad too
			if (board[r+y][c] == player)
			{
			for(int i=2; i<nROW-1; i++)
			{
			if (board[r+i*y][c] == player) continue;
			else if (board[r+i*y][c] != player)
			{
			if (board[r+i*y+y][c] == player)
			{
			result -= mGrid[r][c];
			}
			}
			else assert(false);
			}
			}

			if (board[r][c+x] == player)
			{
			for(int i=2; i<nCOL-1; i++)
			{
			if (board[r][c+i*x] == player) continue;
			else if (board[r][c+i*x] != player)
			{
			if (board[r][c+i*x+x] == player)
			{
			result -= mGrid[r][c];
			}
			}
			else assert(false);
			}
			}
			*/
		}
	}

	return result;
}

double PieceValue2(const cPlayer player, cBoard& board, cMatrix<double>& mGrid)
{
	double result = 0;
	int r, c;
	for (r = 0; r<nROW; r++)
	{
		for (c = 0; c<nCOL; c++)
		{
			if (board[r][c] == player)
			{
				result += mGrid[r][c];
			}
		}
	}

	//add extra negative for corner loc
	for (int i = 0; i <= 4; i++)
	{
		int x, y;
		switch (i)
		{
		case 0: r = 0; c = 0; x = 1; y = 1; break;
		case 1: r = nROW - 1, c = nCOL - 1; x = -1; y = -1; break;
		case 2: r = 0; c = nCOL - 1; x = -1; y = 1; break;
		case 4: r = nROW - 1; c = 0; x = 1; y = -1; break;
		}

		if (board[r][c] == gBLANK)
		{

			//triple the negative for occupying those place diagonally adj to corner
			//since there is only 1 constant, the program should still do findthe "optimial"
			const double MUTIPLIER = 3.0;
			if (board[r + y][c + x] == player)
			{
				result += MUTIPLIER * mGrid[r + y][c + x];
			}

			//don't subtract the adj corner piece if the adj edge piece is player's
			if (board[r + y][c] == player && board[r + 2 * y][c] == player)
			{
				result -= mGrid[r + y][c];
			}

			if (board[r][c + x] == player && board[r][c + 2 * x] == player)
			{
				result -= mGrid[r][c + x];
			}

			//if you press diagonal, you are as good as losing the corner
			if (board[r + y][c + x] == player)
			{
				result -= mGrid[r][c];
			}

			//similary, if you have an edge next to diagonal, i.e _xxx_x__ this is really bad too
			if (board[r + y][c] == player)
			{
				for (int i = 2; i<nROW - 1; i++)
				{
					if (board[r + i * y][c] == player) continue;
					else if (board[r + i * y][c] != player)
					{
						if (board[r + i * y + y][c] == player)
						{
							result -= mGrid[r][c];
						}
					}
					else assert(false);
				}
			}

			if (board[r][c + x] == player)
			{
				for (int i = 2; i<nCOL - 1; i++)
				{
					if (board[r][c + i * x] == player) continue;
					else if (board[r][c + i * x] != player)
					{
						if (board[r][c + i * x + x] == player)
						{
							result -= mGrid[r][c];
						}
					}
					else assert(false);
				}
			}

		}
	}

	return result;
}

double Evaluate(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid)
{
	//decide who's who
	cPlayer other;
	if (player == gBLACK) other = gWHITE;
	else if (player == gWHITE) other = gBLACK;

	//count up the number of pieces
	int nPlayer = board.Count(player);
	int nOther = board.Count(other);
	double score = nPlayer - nOther;

	//if opp has no piece, wonderful
	if (nOther == 0) return INFINITY;

	//if player has no piece, very bad
	if (nPlayer == 0) return -INFINITY;

	//if this is the very end of the game, pieces decide strategy
	//note that this works correctly only when every single leaf of decision tree is evaluated by the below function
	if (nPlayer + nOther == nROW * nCOL)
	{
		return score;
	}

	//otherwise, get mobility
	int mobPlayer = Mobility(player, board, mValid);
	int mobOther = Mobility(other, board, mValid);
	int mob = mobPlayer - mobOther;

	//get positional score
	double posPlayer = PieceValue(player, board, mGrid);
	double posOther = PieceValue(other, board, mGrid);
	double pos = posPlayer - posOther;

	return (score + mob+pos);
}

double Evaluate2(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid){
	//clarify roles
	cPlayer other;
	if (player == gBLACK) other = gWHITE;
	else if (player == gWHITE) other = gBLACK;

	//feature: score
	int nPlayer = board.Count(player);
	int nOther = board.Count(other);
	double score = nPlayer - nOther;

	//if opp has no piece, wonderful
	if (nOther == 0) return INFINITY;

	//if player has no piece, very bad
	if (nPlayer == 0) return -INFINITY;

	//when game has reach the end
	//use only feature "score" to decide
	if (nPlayer + nOther == nROW * nCOL){
		return score;
	}

	//feature: mobility
	int mobPlayer = Mobility(player, board, mValid);
	int mobOther = Mobility(other, board, mValid);
	int mob = mobPlayer - mobOther;

	//feature: piece values
	double posPlayer = PieceValue2(player, board, mGrid);
	double posOther = PieceValue2(other, board, mGrid);
	double pos = posPlayer - posOther;

	return (score*0.1 + mob+ pos);
}

double Evaluate3(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid){
	//clarify role
	cPlayer other;
	if (player == gBLACK) other = gWHITE;
	else if (player == gWHITE) other = gBLACK;

	//count up the number of pieces
	int nPlayer = board.Count(player);
	int nOther = board.Count(other);
	double score = nPlayer - nOther;

	//if opp has no piece, wonderful
	if (nOther == 0) return INFINITY;

	//if player has no piece, very bad
	if (nPlayer == 0) return -INFINITY;


	//if this is the very end of the game, pieces decide strategy
	//note that this works correctly only when every single leaf of decision tree is evaluated by the below function
	if (nPlayer + nOther == nROW * nCOL){
		return score;
	}

	//feature: mobility
	int mobPlayer = Mobility(player, board, mValid);
	int mobOther = Mobility(other, board, mValid);
	int mob = mobPlayer - mobOther;

	//feature: piece values
	double posPlayer = PieceValue2(player, board, mGrid);
	double posOther = PieceValue2(other, board, mGrid);
	double pos = posPlayer - posOther;

	//feature: corner captured
	double bonus = 0;
	int cPlayer = 0;
	int cOther = 0;
	int r, c;

	r = 0; c = 0;
	if (board[r][c] == player) cPlayer++;
	else if (board[r][c] == other) cOther++;

	r = nROW - 1; c = 0;
	if (board[r][c] == player) cPlayer++;
	else if (board[r][c] == other) cOther++;

	r = 0; c = nCOL - 1;
	if (board[r][c] == player) cPlayer++;
	else if (board[r][c] == other) cOther++;

	r = nROW - 1; c = nCOL - 1;
	if (board[r][c] == player) cPlayer++;
	else if (board[r][c] == other) cOther++;

	//discourage when both has corner
	if (cPlayer >= 1 && cOther >= 1) bonus = -85* mGrid[2][2];
	//else bonus = cPlayer - cOther;

	return (score*0.1 + mob + pos + bonus);
}
