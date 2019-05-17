#include "opengl.h"
#include "agent.h"
#include "configuration.h"

//the entire game, remember to change NewGame button when anything is added here
cGame game;
cPlayer comp = gWHITE;
int difficulty = 1;
const int nGRID = 6;
bool process = false;
pair<int, int> hint(-1, -1); //store the hint
string info;

//windows
GLuint winMain;
GLuint winBoard;
GLuint winInfo;
GLuint winConfig;

//configuration
const int gapLEFT = 15;
const int gapSTRING = 75;
const int gapLS = gapLEFT + gapSTRING;
const int gapNUM = 55;
const int gapTOP = 35;

cButton newgame(1, gapLEFT, gapTOP * 1, "New Game");
cButton hint1(2, gapLEFT, gapTOP*1.7, "Hint");
cButton undo(4, gapLS, gapTOP*1.7, "Undo");
cButton redo(6, gapLS, gapTOP*2.2, "Redo");
cTextBox filename(5, gapLS, gapTOP * 3, "filename");
cButton save(8, gapLS + gapNUM * 0, gapTOP*3.7, "Save");
cButton load(16, gapLS + gapNUM * 1, gapTOP*3.7, "Load");
cButton comptag(129, gapLS, gapTOP*4.7, "On");
cButton level(32, gapLEFT, gapTOP*5.4, "Level");
cButton switchp(64, gapLEFT, gapTOP*6.1, "Switch");

//set if the testing option is avail, this is not on screen
cButton testtag(300, -100, -100, "Off");//set this to On or Off

cButton test(200, gapLS, gapTOP*7.0, "Complete");
cButton hint2(3, gapLEFT, gapTOP*2.3, "Hint2");

void displayMain(){
	glClearColor(0.8f, 0.8f, 0.8f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
};

void reshapeMain(int width, int height){
	if (width == 0 && height == 0) return;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.8f, 0.8f, 0.8f, 0.0);

	GLdouble rWidth = GLdouble(width) / (mWIDTH + 3 * GAP);
	GLdouble rHeight = GLdouble(height) / (mHEIGHT + 2 * GAP);

	glutSetWindow(winBoard);
	glutPositionWindow(bLEFT * rWidth, bTOP * rHeight);
	glutReshapeWindow(bWIDTH * rWidth, bHEIGHT * rHeight);

	glutSetWindow(winInfo);
	glutPositionWindow(iLEFT * rWidth, iTOP * rHeight);
	glutReshapeWindow(iWIDTH * rWidth, iHEIGHT * rHeight);

	glutSetWindow(winConfig);
	glutPositionWindow(cLEFT * rWidth, cTOP * rHeight);
	glutReshapeWindow(cWIDTH * rWidth, cHEIGHT * rHeight);
}

void idleMain(void){
	if (selection != 0){
		//to get the flicker (blink) on selected textbox
		glutSetWindow(winConfig);
		glutPostRedisplay();
	}

	//if player's move has not been shown yet, wait
	if (!process) return;

	//if the game ends, comp don't play anymore
	if (game.gameIsOver()) return;

	static double last = clock();

	//if it is comp's turn, make the move
	//diff = 3 is player
	if (game.player == comp && comptag.IsOn()) {
		if (difficulty == 2) game.SetDepth(4);
		else if (difficulty == 1) game.SetDepth(2);
		else game.SetDepth(1);

		pair<int, int> lastmove = game.Play1();
		game.MakeMove(lastmove.first, lastmove.second);
		process = false;

		//save the time of the last move, ensure that it will be at most 1 move per half sec
		while (double(clock() - last) < CLOCKS_PER_SEC / 2.0);

		last = clock();
		RedisplayAll();
	}
}

void DrawGrid(){
	for (int i = 0; i<nGRID; i++){
		for (int j = 0; j<nGRID; j++){
			//color grids with white and gray
			if ((i + j) % 2 == 0) glColor3f(0.95, 0.95, 0.95);
			else				glColor3f(0.75, 0.75, 0.75);

			glBegin(GL_POLYGON);
			glVertex2f(i, j);
			glVertex2f(i + 1, j);
			glVertex2f(i + 1, j + 1);
			glVertex2f(i, j + 1);
			glEnd();
		}
	}
	glutPostRedisplay();
}

void DrawPiece(cGrid p, GLdouble x, GLdouble y, GLdouble r = 0.4){
	const GLint nSTEP = 30;
	const GLdouble PI = 3.1415926;

	if (p == gBLACK) glColor3f(0.1, 0.1, 0.1);
	else if (p == gWHITE) glColor3f(1.0, 1.0, 1.0);
	else{
		assert(false);
		return;
	}

	//Draw the content
	glBegin(GL_POLYGON);
	for (int s = 0; s<nSTEP; s++){
		GLdouble degree = 2.0 * PI * (GLdouble(s) / nSTEP);
		glVertex2f(x + r * cos(degree), y + r * sin(degree));
	}
	glEnd();

	//Draw the outline
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	for (int s = 0; s<nSTEP; s++){
		GLdouble degree = 2.0 * PI * (GLdouble(s) / nSTEP);
		glVertex2f(x + r * cos(degree), y + r * sin(degree));
	}
	glEnd();
}

void DrawValid(){
	cMatrix<bool> mValid;
	mValid.Resize(nGRID, nGRID, gBLANK);

	int nValid = game.board.Valid(game.player, mValid);
	if (nValid == 0) return;

	glColor3f(0.8, 0.2, 0.2);
	for (int i = 0; i<nGRID; i++)	{
		for (int j = 0; j<nGRID; j++){
			if (mValid[i][j]){
				glBegin(GL_POLYGON);
				glVertex2f(i, j);
				glVertex2f(i + 1, j);
				glVertex2f(i + 1, j + 1);
				glVertex2f(i, j + 1);
				glEnd();
			}
		}
	}
}

void DrawBoard(){
	assert(nGRID == game.board.size());
	assert(nGRID == game.board[0].size());

	for (int i = 0; i<nGRID; i++){
		for (int j = 0; j<nGRID; j++){
			if (game.board[i][j] != gBLANK){
				DrawPiece(game.board[i][j], i + 0.5, j + 0.5);
			}
		}
	}
}

void DrawLine(){
	glColor3f(0.0, 0.0, 0.0);

	//draw the vertical line
	glBegin(GL_LINES);
	for (int i = 0; i <= nGRID; i++){
		glVertex2f(i, 0);
		glVertex2f(i, nGRID);
	}
	glEnd();

	//draw the horiz line
	glBegin(GL_LINES);
	for (int j = 0; j <= nGRID; j++){
		glVertex2f(0, j);
		glVertex2f(nGRID, j);
	}
	glEnd();
}

void DrawMove(pair<int, int> move){
	//check the move
	if (move.first < 0) return;
	if (move.second < 0) return;

	int i = move.first;
	int j = move.second;

	glColor3f(0.9, 0.9, 0.1);

	glBegin(GL_POLYGON);
	glVertex2f(i, j);
	glVertex2f(i + 1, j);
	glVertex2f(i + 1, j + 1);
	glVertex2f(i, j + 1);
	glEnd();

}

void displayBoard(){
	glClearColor(0.2f, 0.2f, 0.2f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/); //2d, no depth needed

															//draw the grid
	DrawGrid();

	//show the valid moves, if it is player's turn
	//Note: for some reason, possibly the way glut handle very close point, this line below has to appear after DrawBoard, if nSTEP = 100
	if (comp != game.player || comptag.IsOff()) DrawValid();

	//darw the hint
	DrawMove(hint);

	//draw the last move by computer or human
	DrawMove(game.LastMove());

	//draw the game pieces
	DrawBoard();

	//draw the line
	DrawLine();

	//set the process to true, so comp can move now
	process = true;

	glutSwapBuffers();
}

void reshapeBoard(int width, int height){
	// if minimize
	if (width == 0 && height == 0) return;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, nGRID, 0, nGRID);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void mouseBoard(int button, int state, int x, int y){
	//any click remove the hint display
	if (state == GLUT_DOWN) hint.first = hint.second = -1;

	if (game.player == comp && comptag.IsOn()) return;
	if (game.gameIsOver()) return;

	GLint* viewport = new GLint[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	if (state == GLUT_DOWN){
		GLdouble aX = x / GLdouble(viewport[2] - viewport[0]);
		GLdouble aY = (viewport[3] - y) / GLdouble(viewport[3] - viewport[1]);

		aX *= nGRID;
		aY *= nGRID;

		//if the move is sucessful, process is needed before comp can play
		if (game.MakeMove(GLint(aX), GLint(aY))){
			process = false;
			//lastmove.first = GLint(aX);
			//lastmove.second = GLint(aY);
			info = "";

			RedisplayAll();
		}
	}
}

void displayInfo(){
	//overwrite the color for debug
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0);
	glClearColor(0.8f, 0.8f, 0.8f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//get the window situation
	GLint* viewport = new GLint[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLint width = viewport[2] - viewport[0];
	GLint height = viewport[3] - viewport[1];

	//change the window to -1, 1, -1, 1
	glPushMatrix();{
		glScalef(width / 2.0, height / 2.0, 1.0);
		glTranslatef(1.0, 1.0, 0.0);

		//draw the player
		if (!game.gameIsOver()) DrawPiece(game.player, 0, -0.5);

		//draw the indicator for number of piece
		DrawPiece(gBLACK, -0.75, 0.50, 0.2);
		DrawPiece(gWHITE, 0.25, 0.50, 0.2);
	}
	glPopMatrix();

	//set the font for the number of pieces
	//setfont("times roman", 24);

	const int Y = height * 6.3 / 6.0;

	//write the number of piece
	glColor3f(0.0, 0.0, 0.0);
	drawstr(width / 4.0, Y,std::to_string( game.board.Count(gBLACK)));
	drawstr(width * 3.0 / 4.0, Y,std::to_string(game.board.Count(gBLACK)));

	glutSwapBuffers();
}

void reshapeInfo(int width, int height){
	// if minimize
	if (width == 0 && height == 0) return;

	//handle the matrix and color
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void displayConfig(){
	//overwrite the color for debug
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0);
	glClearColor(0.8f, 0.8f, 0.8f, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setfont("helvetica", 18);

	newgame.Render();
	filename.Render();
	save.Render();
	load.Render();
	hint1.Render();
	undo.Render();
	redo.Render();
	comptag.Render();

	if (testtag.IsOn()){
		hint2.Render();
		test.Render();
	}

	//Write comment
	glColor3f(0.0, 0.0, 0.0);
	drawstr(gapLEFT, filename.y, "File:");
	drawstr(gapLEFT, comptag.y, "Comp:");

	//Write the difficulty level
	if (comptag.IsOn()){
		//show level
		level.Render();
		glColor3f(0.0, 0.0, 0.0);
		drawstr(gapLEFT, comptag.y, "Comp:");
		if (difficulty == 2) drawstr(gapLS, level.y, "Impossible");
		else if (difficulty == 1) drawstr(gapLS, level.y, "Normal");
		else				 drawstr(gapLS, level.y, "Easy");

		//Write who is comp and switch
		switchp.Render();
		glColor3f(0.0, 0.0, 0.0);
		if (comp == gBLACK)		drawstr(gapLS, switchp.y, "Comp is Black");
		else if (comp == gWHITE)drawstr(gapLS, switchp.y, "Comp is White");
		else assert(false);
	}


	//Write infomation
	drawstr(gapLEFT, gapTOP * 6, info);

	glutSwapBuffers();
}

void reshapeConfig(int width, int height)
{
	// if minimize
	if (width == 0 && height == 0) return;

	//handle the matrix and color
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouseConfig(int button, int state, int x, int y){
	//any click remove the hint display
	if (state == GLUT_DOWN) hint.first = hint.second = -1;

	if (selection != filename.id) selection = 0;

	if (state == GLUT_DOWN)	{
		selection = 0;

		selection += newgame.Hit(x, y);
		selection += filename.Hit(x, y);
		selection += save.Hit(x, y);
		selection += load.Hit(x, y);
		selection += hint1.Hit(x, y);


		if (comptag.IsOn())		{
			selection += level.Hit(x, y);
			selection += switchp.Hit(x, y);
		}

		selection += undo.Hit(x, y);
		selection += redo.Hit(x, y);
		selection += comptag.Hit(x, y);


		if (testtag.IsOn())		{
			selection += hint2.Hit(x, y);
			selection += test.Hit(x, y);

			if (selection == test.id)			{
				const int nTIMES = 100;

				int nP1, nP2;
				cPlayer p1, p2;

				for (int k = 0; k <= 1; k++)				{
					nP1 = 0;
					nP2 = 0;

					if (k == 0)					{
						p1 = gWHITE;
						p2 = gBLACK;
					}
					else					{
						p1 = gBLACK;
						p2 = gWHITE;
					}

					for (int t = 0; t<nTIMES; t++){
						//reset the game
						game.NewGame();

						//play the entire game
						while (!game.gameIsOver()){
							pair<int, int> move;
							if (game.player == p1)		move = game.Play3();
							else if (game.player == p2)	move = game.Play1();
							else break;

							//cout << "move: " << move.first << move.second << endl;
							game.MakeMove(move.first, move.second);
						}

						if (game.board.Count(p1) >= game.board.Count(p2))	nP1++;
						else												nP2++;

						cout << "p1/p2: " << nP1 << ' ' << nP2 << "---" << game.board.Count(p1) << ' ' << game.board.Count(p2) << endl;
					}
				}
			}
		};

		if (selection == undo.id){
			bool bUndo;

			if (comptag.IsOff()){
				bUndo = game.Undo(gBLANK);
			}
			else{
				if (comp == gBLACK)			bUndo = game.Undo(gWHITE);
				else if (comp == gWHITE)	bUndo = game.Undo(gBLACK);
				else{
					bUndo = game.Undo(gBLANK);
					assert(false);
				}
			}

			if (!bUndo){
				info = "FAIL to UNDO";
			}
			else{
				info = "Sucessfully Undo";
				//lastmove = game.LastMove();
				//lastmove.first = lastmove.second = -1;
			}
		}

		if (selection == redo.id){

			bool bRedo;

			if (comptag.IsOff()){
				bRedo = game.Redo(gBLANK);
			}
			else{
				if (comp == gBLACK)			bRedo = game.Redo(gWHITE);
				else if (comp == gWHITE)	bRedo = game.Redo(gBLACK);
				else{
					bRedo = game.Redo(gBLANK);
					assert(false);
				}
			}

			if (!bRedo){
				info = "FAIL to REDO";
			}
			else{
				info = "Sucessfully Redo";
			}
		}


		if (selection == newgame.id){
			game.NewGame();
			process = false;
			//lastmove.first = -1;
			//lastmove.second = -1;
		}

		if (selection == hint1.id){
			hint = game.Play1();
			cout << "hint1: " << hint.first << ' ' << hint.second << endl;
		}

		if (selection == hint2.id){
			hint = game.Play2();
			cout << "hint2: " << hint.first << ' ' << hint.second << endl;
		}

		if (selection == level.id){
			if (difficulty == 0){
				difficulty = 1;
				game.SetDepth(2);
			}
			else if (difficulty == 1){
				difficulty = 2;
				game.SetDepth(4);
			}
			//else if (difficulty == 2) difficulty = 3;
			else{
				difficulty = 0;
				game.SetDepth(1);
			}

		}

		if (selection == switchp.id){
			if (comp == gWHITE) comp = gBLACK;
			else if (comp == gBLACK) comp = gWHITE;
			else assert(false);
			process = false; //redisplay before comp move
		}

		if (selection == save.id){
			bool suc = game.Save(filename.str);

			if (!suc){
				cout << "Unable to save " << filename.str << endl;
				info = "FAIL to save!";
			}
			else info = "Game has been saved";
		}

		if (selection == load.id){
			bool suc = game.Load(filename.str);

			if (!suc)		{
				cout << "Unable to load " << filename.str << endl;
				info = "FAIL to load!";
			}
			else			{
				info = "Game has been loaded";
				//lastmove.first = -1;
				//lastmove.second = -1;
			}
		}

	}

	RedisplayAll();
}

void keyboardConfig(unsigned char ch, int x, int y){
	filename.Update(ch);

	RedisplayAll();
}

void RedisplayAll(){
	glutSetWindow(winMain);
	glutPostRedisplay();

	glutSetWindow(winBoard);
	glutPostRedisplay();

	glutSetWindow(winInfo);
	glutPostRedisplay();

	glutSetWindow(winConfig);
	glutPostRedisplay();
}