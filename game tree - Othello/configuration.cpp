#include "configuration.h"
#include <string>
#include <strstream>

using namespace std;

GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;
int selection = 0;
//int font_size;

void
setfont(char* name, int size)
{
	font_style = GLUT_BITMAP_HELVETICA_10;
	if (strcmp(name, "helvetica") == 0) {
		if (size == 12)
			font_style = GLUT_BITMAP_HELVETICA_12;
		else if (size == 18)
			font_style = GLUT_BITMAP_HELVETICA_18;
	}
	else if (strcmp(name, "times roman") == 0) {
		font_style = GLUT_BITMAP_TIMES_ROMAN_10;
		if (size == 24)
			font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	}
	else if (strcmp(name, "8x13") == 0) {
		font_style = GLUT_BITMAP_8_BY_13;
	}
	else if (strcmp(name, "9x15") == 0) {
		font_style = GLUT_BITMAP_9_BY_15;
	}
	//	font_size = size;
}

void
drawstr(GLuint x, GLuint y, char* format, ...)
{
	va_list args;
	char buffer[255], *s;

	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	glRasterPos2i(x, y);
	for (s = buffer; *s; s++)
		glutBitmapCharacter(font_style, *s);
}

void
drawstr(GLuint x, GLuint y, string str)
{
	const char *s = str.c_str();

	glRasterPos2i(x, y);
	for (s; *s; s++)
		glutBitmapCharacter(font_style, *s);
}

void
cell_draw(cell* cell)
{
	glColor3ub(0, 255, 128);
	if (selection == cell->id) {
		glColor3ub(255, 255, 0);
		drawstr(10, 300, cell->info);
		glColor3ub(255, 0, 0);
	}

	drawstr(cell->x, cell->y, cell->format, cell->value);
}

int
cell_hit(cell* cell, int x, int y)
{
	if (x > cell->x && x < cell->x + 50 &&
		y > cell->y - 20 && y < cell->y + 10)
		return cell->id;
	return 0;
}

void
cell_update(cell* cell, int update, bool select)
{
	if (selection != cell->id && select == false)
		return;

	cell->value += update * cell->step;

	if (cell->value < cell->min)
		cell->value = cell->min;
	else if (cell->value > cell->max)
		cell->value = cell->max;

}

void
cell_vector(float* dst, cell* cell, int num)
{
	while (--num >= 0)
		dst[num] = cell[num].value;
}

void cell_set(cell* cell, int _id, int _x, int _y, float _min, float _max, float _value, float _step, char* _info, char* _format)
{
	cell->id = _id;
	cell->x = _x;
	cell->y = _y;
	cell->min = _min;
	cell->max = _max;
	cell->value = _value;
	cell->step = _step;
	cell->info = _info;
	cell->format = _format;
}

cButton::cButton(int _id, int _x, int _y, string _str)
{
	id = _id;
	x = _x;
	y = _y;
	str = _str;

	hit = false;
}

void cButton::Render(bool active)
{
	if (selection == id) hit = true;
	else hit = false;

	if (hit) glColor3ub(255, 0, 0);
	else
	{
		if (!active) glColor3ub(0, 128, 64);
		else glColor3ub(0, 128, 64);
		//		else glColor3ub(0, 255, 128);
	}
	drawstr(x, y, str);
}

int cButton::Hit(int hx, int hy)
{
	if (hx > x && hx < x + (12 * str.size()) &&
		hy > y - 20 && hy < y + 10)
	{
		hit = true;

		//if it's On/off, otherwise, this does nothing
		Switch();

		return id;
	}

	hit = false;
	return 0;
}

void cButton::SetHit(bool h)
{
	hit = h;
}

cTextBox::cTextBox(int _id, int _x, int _y, string _str)
{
	id = _id;
	x = _x;
	y = _y;
	str = _str;

	hit = false;
};

void cTextBox::Render(void)
{
	if (selection == id) hit = true;
	else hit = false;

	if (hit) glColor3ub(255, 0, 0);
	else glColor3ub(0, 128, 64);
	//	else glColor3ub(0, 255, 128);

	if (selection == id)
	{
		//to get the flash
		const int rFLASH = 50;
		static int counter = 0;
		counter++;
		counter %= rFLASH;

		if (counter > rFLASH / 2)	drawstr(x, y, str + "|");
		else					drawstr(x, y, str);
	}
	else
	{
		drawstr(x, y, str);
	}

}

void cTextBox::Update(unsigned char ch)
{
	if (selection != id) return;

	unsigned int code = (unsigned int)(ch);

	//backspace, remove last char
	if (code == 8)
	{
		if (str.length() != 0)
		{
			str = str.substr(0, str.length() - 1);
		}
	}
	else if (code == 10 || code == 13 || code == '\n') //enter
	{
		selection = 0;	//unselect
		glutPostRedisplay();
	}
	else if (code == '\t')
	{
		selection = 0;	//unselect
		glutPostRedisplay();
	}
	else if (code == 127) //del
	{
		str = "";
	}
	else
	{
		//if it is alpha numeric, append
		if (ch >= '0' && ch <= '9' ||
			ch >= 'a' && ch <= 'z' ||
			ch >= 'A' && ch <= 'Z' ||
			ch == ' ')
		{
			char temp[2];
			temp[0] = ch;
			temp[1] = '\0';

			str += string(temp);
		}

		/*
		//for everything else, append
		char temp[2];
		temp[0] = ch;
		temp[1] = '\0';

		str += string(temp);
		*/
	}
}

/*
void cTextBox::UpdateSpecial(int key)
{


}
*/

int cTextBox::Hit(int hx, int hy)
{
	int width = str.size();
	if (width <= 5) width = 5;

	if (hx > x && hx < x + (10.0*width) &&
		hy > y - 20 && hy < y + 10)
	{
		hit = true;

		return id;
	}

	hit = false;
	return 0;
}

void cTextBox::SetHit(bool h)
{
	hit = h;
}

double cTextBox::ToNum()
{
	strstream temp;

	temp << str;
	temp << " 0"; //in case if the initial part is not acceptable, 0 will be accepted

	double result;
	temp >> result;

	return result;
}

ofstream& operator<<(ofstream& out, const cell& c)
{
	int i;

	string sformat(c.format);
	for (i = 0; i<sformat.size(); i++)
		if (sformat[i] == ' ')
			sformat[i] = '_';

	string sinfo(c.info);
	for (i = 0; i<sinfo.size(); i++)
		if (sinfo[i] == ' ')
			sinfo[i] = '_';

	out << sformat << " ";
	out << c.id << " ";
	out << sinfo << " ";
	out << c.max << " ";
	out << c.min << " ";
	out << c.step << " ";
	out << c.value << " ";
	out << c.x << " ";
	out << c.y << " ";

	out << endl;

	return out;
}

ifstream& operator>>(ifstream& in, cell& c)
{
	string sformat, sinfo;

	if (DEFAULTBOUND)  //don't load bound
	{
		float junk;

		in >> sformat >> c.id >> sinfo >> junk >> junk >> c.step >> c.value >> junk >> junk;
	}
	else
	{
		in >> sformat >> c.id >> sinfo >> c.max >> c.min >> c.step >> c.value >> c.x >> c.y;
	}


	int i;
	c.format = new char[sformat.size() + 1];
	for (i = 0; i<sformat.size(); i++)
	{
		if (sformat[i] == '_')
		{
			sformat[i] = ' ';
		}
		c.format[i] = sformat[i];
	}
	c.format[sformat.size()] = '\0';

	c.info = new char[sinfo.size() + 1];
	for (i = 0; i<sinfo.size(); i++)
	{
		if (sinfo[i] == '_')
		{
			sinfo[i] = ' ';
		}

		c.info[i] = sinfo[i];
	}
	c.info[sinfo.size()] = '\0';

	//TODO check if I need to allocate memory

	/*
	c.format = new char[(sformat.size() + 1) * sizeof(char)];
	c.info = new char[(sinfo.size() + 1) * sizeof(char)];
	memcpy(c.format, sformat.c_str(), (sformat.size() + 1) * sizeof(char));
	memcpy(c.info, sformat.c_str(), (sinfo.size() + 1) * sizeof(char));
	*/

	return in;
}
