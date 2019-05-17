#pragma once
#ifndef BOARDHEADER
#define BOARDHEADER

#pragma warning(disable : 4786)
#pragma warning(disable : 4503)

#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <strstream>
#include <map>
#include <fstream>
#include <cassert>
#include <ctime>

using namespace std;

#define DEBUG if(false)

/////////////////////////////////////////////////////////////
//the size of board
const int nWIDTH = 6;
const int nROW = nWIDTH;
const int nCOL = nWIDTH;

//alias of data structure to denote actual meanings
typedef unsigned char cGrid;
typedef cGrid cPlayer;
const cGrid gBLACK = 'X';
const cGrid gWHITE = 'O';
const cGrid gBLANK = '-';

template <class C>
struct cMatrix : vector< vector<C> >{
	void Resize(int r, int c, const C& init){
		this->resize(r);
		for (int i = 0; i<r; i++){
			(*this)[i].resize(c, init);
		}
	};

	int NumRow(){
		return this->size();
	};

	int NumCol(){
		if (NumRow() == 0) return 0;
		else return (*this)[0].size();
	};

	void Reset(const C& init){
		for (int i = 0; i<this->size(); i++)
			for (int j = 0; j<(*this)[i].size(); j++)
				(*this)[i][j] = init;
	}

	long Count(const C& entry){
		long result = 0;
		for (int i = 0; i<this->size(); i++)
			for (int j = 0; j<(*this)[i].size(); j++)
				if ((*this)[i][j] == entry) result++;
		return result;
	}
};

//output the content of a vector
template <class C>
void OutVec(const vector<C> v, const char delim = ' ')
{
	for (int i = 0; i<v.size(); i++)
	{
		cout << v[i] << delim;
	}

	cout << endl;
}

//output the content of a matrix
template <class C>
void OutMat(const cMatrix<C> m, const char delim = ' ')
{
	for (int r = 0; r<m.size(); r++)
	{
		OutVec(m[r], delim);
	}
}

class cBoard : public cMatrix<cGrid>{
public:
	//constructor
	cBoard(){
		this->Resize(nROW, nCOL, gBLANK);
		Set(nROW / 2, nCOL / 2, gBLACK);
		Set(nROW / 2 - 1, nCOL / 2 - 1, gBLACK);
		Set(nROW / 2 - 1, nCOL / 2, gWHITE);
		Set(nROW / 2, nCOL / 2 - 1, gWHITE);
	};

	//return the number of valid move, and a boolean matrix of all valid move
	int Valid(const cPlayer player, cMatrix<bool>& mValid);

	//make a move, return the number of piece capture, not include the piece dwon
	//also return a matrix for undo
	//NOTE: the piece (*this)[r][c] is NOT SET to player
	int Move(const cPlayer player, const int r, const int c, cMatrix<bool>& mUndo);

	//this function makes a move, return true if valid
	bool MakeMove(const cPlayer player, const int r, const int c);

	void Set(const int r, const int c, const cGrid player) { (*this)[r][c] = player; };
	void Remove(const int r, const int c) { (*this)[r][c] = gBLANK; };

	void Undo(const cMatrix<bool>& mUndo);


private:
	//Reverse the color of the piece in (r,c) into my color
	void Reverse(const int r, const int c){
		cGrid* piece = &((*this)[r][c]);
		if (*piece == gBLACK) 
			*piece = gWHITE;
		else if (*piece == gWHITE) 
			*piece = gBLACK;
		else assert(false);
	}

};

ostream& operator<<(ostream& out, const cBoard& board);

#endif