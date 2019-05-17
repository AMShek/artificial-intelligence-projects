#pragma once
#ifndef CONFIGURATIONHEADER
#define CONFIGURATIONHEADER

#include <GL/glut.h>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#define DEFAULTBOUND false

typedef struct _cell {
	int id;
	int x, y;
	float min, max;
	float value;
	float step;
	char* info;
	char* format;
} cell;

ifstream& operator>>(ifstream& in, cell& c);
ofstream& operator<<(ofstream& out, const cell& c);

/*
struct cell {
int id;
int x, y;
float min, max;
float value;
float step;
char* info;
char* format;

cell(int _id, int _x, int _y, float _min, float _max, float _value, float _step, char* _info, char* _format)
{
id = _id;
x = _x;
y = _y;
min = _min;
max = _max;
value = _value;
step = _step;
info = _info;
format = _format;
};

};
*/

void setfont(char* name, int size);
void drawstr(GLuint x, GLuint y, char* format, ...);
void drawstr(GLuint x, GLuint y, string str);
void cell_draw(cell* cell);
int cell_hit(cell* cell, int x, int y);
void cell_update(cell* cell, int update, bool select = false);
void cell_vector(float* dst, cell* cell, int num);
void cell_set(cell* cell, int _id, int _x, int _y, float _min, float _max, float value, float _step, char* _info, char* _format);

struct cButton
{
	int id;
	int x, y;
	string str;

	cButton(int _id, int _x, int _y, string _str);

	int Hit(int hx, int hy);
	void Render(bool active = true);

	void SetHit(bool h);
	void Switch(void)
	{
		if (str == "On") str = "Off";
		else if (str == "Off") str = "On";
		else
		{
			//cout << "Warning: invalid switch" << endl;
		}
	};

	bool IsOn(void) { return (str == "On"); };
	bool IsOff(void) { return (str == "Off"); };

	//bool IsTextBox(void){return false;};

private:
	bool hit;
};

struct cTextBox
{
public:
	int id;
	int x, y;
	string str;

	cTextBox(int _id, int _x, int _y, string _str);

	int Hit(int hx, int hy);

	void Render();

	void Update(unsigned char ch);

	void SetText(string s) { str = s; };
	void SetHit(bool h);

	double ToNum();

	//bool IsTextBox(void){return true;};
private:
	bool hit;
};

#endif