#pragma once
#ifndef AGENTHEADER
#define AGENTHEADER

#include "board.h"

typedef double (cStrat)(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid);
typedef double (cMiniMax)(const cPlayer player, cBoard& board, int depth, cMatrix<double>& mGrid, cMatrix<bool>& mValid, cStrat* eval, double alpha, double beta, int& r_out, int& c_out, int& searchtimes);

//if the is a input, InitGrid make it symmetrical
cMatrix<double> InitGrid(cMatrix<double> input = cMatrix<double>());
cMatrix<double> InitGrid2(cMatrix<double> input = cMatrix<double>());
cMatrix<double> InitGrid3(cMatrix<double> input = cMatrix<double>());

//handling the evaluation function
int Mobility(const cPlayer player, cBoard& board, cMatrix<bool>& mValid);
double PieceValue(const cPlayer player, cBoard& board, cMatrix<double>& mGrid);
double PieceValue2(const cPlayer player, cBoard& board, cMatrix<double>& mGrid);
double Evaluate(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid);
double Evaluate2(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid);
double Evaluate3(const cPlayer player, cBoard& board, cMatrix<double>& mGrid, cMatrix<bool>& mValid);

double MiniMax(const cPlayer player, cBoard& board, int depth, cMatrix<double>& mGrid, cMatrix<bool>& mValid, cStrat* eval, double alpha, double beta, int& r_out, int& c_out, int&searchtimes);


class cGame{
public:
	cBoard board;
	cPlayer player;
	//constructor
	cGame(){
		NewGame();

		mGrid1 = InitGrid2();
		mGrid2 = InitGrid3();

		fEval1 = &Evaluate2;
		fEval2 = &Evaluate3;

		fMiniMax1 = &MiniMax;
		fMiniMax2 = &MiniMax;

		nDepth = 2;
	};

	//make a move at grid (r,c)
	bool MakeMove(int r, int c);

	//reset to new game
	void NewGame(bool reset_history = true){
		player = gBLACK;
		board = cBoard();
		gameOver = false;
		history.erase(history.begin(), history.end());
		//if (reset_history) history.erase(history.begin(), history.end());
	}

	//output game state
	void OutGame();

	//I/O
	bool Load(string fn);
	bool Save(string fn);

	//Comp player 1 plays
	pair<int, int> Play1();
	//Comp player 2 plays
	pair<int, int> Play2();
	//Comp player 3 plays, a combination of 1 and 2
	pair<int, int> Play3();

	//game end
	bool gameIsOver() { return gameOver; };

	//attempt to find a better mGrid than mGrid1 given the current one
	//by having p1 and p2 play against each other
	bool SearchGrid(const clock_t duration);

	//set depth
	void SetDepth(int depth) { nDepth = depth; };

	//if p == gBLACK, undo last move, if p == player, undo till player's move
	bool Undo(cPlayer p);
	//reverse of undo
	bool Redo(cPlayer p);

	//return the lastmove
	pair<int, int> LastMove(void);

private:
	int nDepth;

	//the positional computation for player 1 and 2
	cMatrix<double> mGrid1;
	cMatrix<double> mGrid2;

	//the evaluation function for both player 1 and 2
	cStrat* fEval1;
	cStrat* fEval2;

	//the decision tree function
	cMiniMax* fMiniMax1;
	cMiniMax* fMiniMax2;

	//record down the history
	struct cAction {
		int r;
		int c;
		cPlayer p;
	};
	vector<cAction> history;

	//record if the game ends
	bool gameOver;
	//figure out a reasonable depth
	int Depth();
	//return true if sucessfully switch, false if end of game
	bool Switch();

	//end the game, output result, set flag
	void Endgame();
};

struct cMove{
	int r;
	int c;
	double priority;

	cMove(int _r, int _c, double _priority){
		r = _r; c = _c; priority = _priority;
	};
};

//handling the priority of move by decide
bool CompareMove(const cMove& m1, const cMove& m2);
vector<cMove> GetOrderMove(const cPlayer& player, cBoard& board, cMatrix<bool>& mValid, cMatrix<double>& mGrid);

#endif
