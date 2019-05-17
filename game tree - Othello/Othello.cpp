#include <GL\glut.h>
#include "board.h"
#include "agent.h"
#include <process.h>
#include "opengl.h"
#include "configuration.h"

//a good strat has
//	-restrict opponent's piece
//	-occupies adventages position
//extra
//	balanced border ***
//	saved those loc that opponent can't move to later
//thought about saving previous seen configuration
//IDEA: if the rating for a path has dropped too low, stop
//IDEA: consider real time update of mGrid for both black and white, or recompute at each eval
//IDEA: follow table at beginning
//IDEA: an evaluation is bad if a drastic value change occur frequently

//if it's a sucessful move, return true
void Process(const string command, cGame& game){
	//cBoard board = game.board;
	//cPlayer player = game.player;

	//mark if the game ends
	static bool gameOver = false;

	if (command == "help")
	{
		cout << "COMMANDS:" << endl;
		cout << "\tmove # #: make a move at coordinate # #." << endl;
		cout << "\tcomp: ask computer to make a move." << endl;
		cout << "\thint: ask the computer to give you a hint." << endl;
		cout << "\tvalid: display all valid moves." << endl;
		cout << "\tboard: display the board." << endl;
		cout << "\tdepth #: set the depth." << endl;
		cout << "\tquit: exit the game." << endl;
		cout << "\t<enter>: repeat the previous command." << endl;
		//cout << "search ###: trying to find a better grid" << endl;
		system("pause");
	}
	else if (command == "depth"){
		int depth;
		cin >> depth;

		game.SetDepth(depth);
	}
	else if (command == "search"){
		long trial;
		cin >> trial;

		clock_t start = clock();
		clock_t duration;
		for (int i = 0; i<trial; i++){
			duration = clock() - start;
			bool suc = game.SearchGrid(duration);

			//if this search is sucessful, reset clock
			if (suc){
				start = clock();
			}
		}
	}
	//show grids valid to place the next piece
	else if (command == "valid")
	{
		cMatrix<bool> mValid;
		mValid.Resize(nROW, nCOL, false);
		int nValid = game.board.Valid(game.player, mValid);

		cout << "nValid = " << nValid << endl;
		OutMat(mValid);
	}
	//place a piece at (r,c)
	else if (command == "move"){
		int r, c;
		cin >> r >> c;
		bool suc = game.MakeMove(r, c);
		if (!suc){
			cout << "Error: Invalid Move!" << endl;
		}
	}
	//output the current board state
	else if (command == "board"){
		game.OutGame();
	}
	//load previous game
	else if (command == "load"){
		string fn;
		cin >> fn;
		bool suc = game.Load(fn);
		if (!suc){
			cout << "Unable to open file " << fn << endl;
			return;
		}
	}
	//save this game
	else if (command == "save")
	{
		string fn;
		cin >> fn;

		bool suc = game.Save(fn);

		if (!suc)
		{
			cout << "Unable to save " << fn << endl;
		}
	}
	//agent takes one action
	else if (command == "comp"){
		pair<int, int> move = game.Play2();
		bool suc = game.MakeMove(move.first, move.second);
		//if (!suc) cout << "invalid move!" << endl;
		if (!suc && !game.gameIsOver()) assert(false);
	}
	//agent gives one hint for player's next action
	else if (command == "hint")
	{
		pair<int, int> move = game.Play1();

		cout << "move " << move.first << " " << move.second << endl;
	}
	else
	{
		cout << "Error: Invalid Command: " << command << endl;
	}
}

int main(int argc, char* argv[])
{
	//to use search, remember to turn of randomization
	const bool COMMAND = true;

	//Command line interface
	if (COMMAND){
		// control with commands
		cGame game;
		string command = "help";

		Process("help", game);

		for (;;){
			Process("board", game);
			cout << ">>>";

			//press enter for same command
			char ch;
			cin.get(ch);
			cin.putback(ch);

			if (ch != '\n'){
				//read a command
				cin >> command;
			}

			//if quit is the input, then shut down the program
			if (command == "quit") break;

			//if a move has sucessuflly be made
			Process(command, game);

			//get the last endline
			cin.get(ch);
		}

		return 0;
	}

	//Graphic interface implemented with OpenGL
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(mWIDTH + 3 * GAP, mHEIGHT + 2 * GAP);
		glutInitWindowPosition(50, 50);

		//Init main window
		winMain = glutCreateWindow("Othello by SHI");
		glutDisplayFunc(displayMain);
		glutReshapeFunc(reshapeMain);
		glutIdleFunc(idleMain);
		glutKeyboardFunc(keyboardConfig);

		//initMain();
		//glutKeyboardFunc(keyboardConfig);

		//Init plot window
		winBoard = glutCreateSubWindow(winMain, bLEFT, bTOP, bWIDTH, bHEIGHT);
		glutDisplayFunc(displayBoard);
		glutReshapeFunc(reshapeBoard);
		glutMouseFunc(mouseBoard);
		glutKeyboardFunc(keyboardConfig);

		//Init info window
		winInfo = glutCreateSubWindow(winMain, iLEFT, iTOP, iWIDTH, iHEIGHT);
		glutDisplayFunc(displayInfo);
		glutReshapeFunc(reshapeInfo);
		glutKeyboardFunc(keyboardConfig);

		//Init config window
		winConfig = glutCreateSubWindow(winMain, cLEFT, cTOP, cWIDTH, cHEIGHT);
		glutDisplayFunc(displayConfig);
		glutReshapeFunc(reshapeConfig);
		glutMouseFunc(mouseConfig);
		glutKeyboardFunc(keyboardConfig);

		glutMainLoop();

		return 0;
	}
}