#pragma once
#ifndef OPENGLHEADER
#define OPENGLHEADER

#include <GL/glut.h>

//window index
extern GLuint winMain;
extern GLuint winBoard;
extern GLuint winInfo;
extern GLuint winConfig;

void RedisplayAll();


//gap
const int GAP = 30;

//main window
const int mWIDTH = 850;
const int mHEIGHT = 600;

//board window
const int bWIDTH = mHEIGHT;
const int bHEIGHT = mHEIGHT;
const int bLEFT = GAP;
const int bTOP = GAP;

//info window
const int iWIDTH = mWIDTH - mHEIGHT;
const int iHEIGHT = mHEIGHT / 2 - GAP;
const int iLEFT = GAP + bWIDTH + GAP;
const int iTOP = GAP;

//config window
extern int selection;
const int cWIDTH = iWIDTH;
const int cHEIGHT = mHEIGHT - iHEIGHT - GAP;
const int cLEFT = iLEFT;
const int cTOP = GAP + iHEIGHT + GAP;

//fuctions
void displayMain();
void reshapeMain(int width, int height);
void idleMain(void);

void displayBoard();
void reshapeBoard(int width, int height);
void mouseBoard(int button, int state, int x, int y);

void displayInfo();
void reshapeInfo(int width, int height);

void displayConfig();
void reshapeConfig(int width, int height);
void mouseConfig(int button, int state, int x, int y);
void keyboardConfig(unsigned char ch, int x, int y);

#endif