#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <Windows.h>

#define  FALSE           0
#define  TRUE            1
#define  BOARD_WIDTH     4
#define  BOARD_SIZE     16

#define  WDTBL_SIZE  24964 /* WalkingDistance TableSize */
#define  IDTBL_SIZE    106 /* InvertDistance TableSize  */

typedef  unsigned __int64  u64;
using namespace std;


int  BOARD[BOARD_SIZE] = {
	/*	sample 0
	0,15,14,13,  // sample
	12,11,10, 9,
	8, 7, 6, 5,
	4, 3, 2, 1
	*/

	/*	sample 1
	11,3,1,7,
	4,6,8,2,
	15,9,10,13,
	14,12,5,0
	//*/

	/*	sample 2
	14,10,6,0,
	4,9,1,8,
	2,3,5,11,
	12,13,7,15
	*/

	/*	sample 3
	0,5,15,14,
	7,9,6,13,
	1,2,12,10,
	8,11,4,3
	*/

	//*	sample 4
	6,10,3,15,
	14,8,7,11,
	5,1,0,2,
	13,12,9,4
	//*/
};

u64   WDPTN[WDTBL_SIZE];        //pattern table 
short WDLNK[WDTBL_SIZE][2][BOARD_WIDTH];  //link
char  WDTBL[WDTBL_SIZE];        //WD Calculation table
char  IDTBL[IDTBL_SIZE];        //ID Calculation table 
char  RESULT[100];             
int   DEPTH;                    //threshold
int   MOVAL[BOARD_SIZE][5] = {  /* 格子0~15相邻的格子，-1之后表示没有棋盘内的邻居了 */
	1,  4, -1,  0,  0,
	2,  5,  0, -1,  0,
	3,  6,  1, -1,  0,
	7,  2, -1,  0,  0,
	0,  5,  8, -1,  0,
	1,  6,  9,  4, -1,
	2,  7, 10,  5, -1,
	3, 11,  6, -1,  0,
	4,  9, 12, -1,  0,
	5, 10, 13,  8, -1,
	6, 11, 14,  9, -1,
	7, 15, 10, -1,  0,
	8, 13, -1,  0,  0,
	9, 14, 12, -1,  0,
	10, 15, 13, -1,  0,
	11, 14, -1,  0,  0
};
int  CONV[BOARD_SIZE] = {       /* 棋子编号的converse */

	0,
	1, 5, 9,13,
	2, 6,10,14,
	3, 7,11,15,
	4, 8,12
};

void Initialize(void)
{
	int   i, j, k, nextd;
	u64   table;
	char  *filename = "puz15wd.db";
	FILE  *fp;

	/* IDTBL[] */
	for (i = 0; i<106; i++)
		IDTBL[i] = (char)((i / 3) + (i % 3));

	/* WDPTN[], WDTBL[], WDLNK[][][] */
	fp = fopen(filename, "rb");
	for (i = 0; i<WDTBL_SIZE; i++) {
		/* WDPTN */
		table = 0;
		//八个char类型变量(8 bits)拼接出一个u64类型变量(64 bits)
		for (j = 0; j<8; j++)
			table = (table << 8) | getc(fp);
		WDPTN[i] = table;
		/* WDTBL */
		WDTBL[i] = (char)getc(fp);
		/* WDLNK */
		for (j = 0; j<2; j++)
			for (k = 0; k<4; k++) {
				nextd = getc(fp);
				WDLNK[i][j][k] = (short)((nextd << 8) | getc(fp));
			}
	}
	fclose(fp);
}

bool IDA(int space, int prev, int idx1o, int idx2o, int inv1o, int inv2o, int depth)
{
	int  i, j, n, n2, piece, wd1, wd2, id1, id2, diff;
	int  idx1, idx2, inv1, inv2, lowb1, lowb2;

	/* try all of next moves */
	depth++;
	for (i = 0; ; i++) {
		piece = MOVAL[space][i];		/*  空格可以移动到的坐标 */
		if (piece == -1) break;			/* 没得移动了 */
		if (piece == prev) continue;	/* 防止重复的移动（和移之前相同） */
		n = BOARD[piece];				/* n为该坐标上的棋子编号 */
										/* 即该空格与这个棋子交换位置 */

										/* Find the next phase (WD, ID) by the direction of movement of the piece*/

		//idx - index
		//to speed up search in WD database
		idx1 = idx1o;
		idx2 = idx2o;
		//inv - inversion numebr
		inv1 = inv1o;
		inv2 = inv2o;
		diff = piece - space;
		if (diff > 0) {
			if (diff == 4) {
				/* move upward */
				for (j = space + 1; j<piece; j++)
					if (BOARD[j] > n) inv1--; else inv1++;
				idx1 = WDLNK[idx1o][0][(n - 1) >> 2];
			}
			else {
				/* move leftward */
				n2 = CONV[n];
				for (j = space + 4; j<16; j += 4)
					if (CONV[BOARD[j]] > n2) inv2--; else inv2++;
				for (j = piece - 4; j >= 0; j -= 4)
					if (CONV[BOARD[j]] > n2) inv2--; else inv2++;
				idx2 = WDLNK[idx2o][0][(n2 - 1) >> 2];
			}
		}
		else {
			if (diff == -4) {
				/* move downward */
				for (j = piece + 1; j<space; j++)
					if (BOARD[j] > n) inv1++; else inv1--;
				idx1 = WDLNK[idx1o][1][(n - 1) >> 2];
			}
			else {
				/* move rightward */
				n2 = CONV[n];
				for (j = piece + 4; j<16; j += 4)
					if (CONV[BOARD[j]] > n2) inv2++; else inv2--;
				for (j = space - 4; j >= 0; j -= 4)
					if (CONV[BOARD[j]] > n2) inv2++; else inv2--;
				idx2 = WDLNK[idx2o][1][(n2 - 1) >> 2];
			}
		}

		/* 下限値(LowerBound) */
		wd1 = WDTBL[idx1];
		wd2 = WDTBL[idx2];
		id1 = IDTBL[inv1];
		id2 = IDTBL[inv2];
		lowb1 = (wd1 > id1) ? wd1 : id1;
		lowb2 = (wd2 > id2) ? wd2 : id2;


		if (depth + lowb1 + lowb2 <= DEPTH) {
			BOARD[piece] = 0;
			BOARD[space] = n;
			if (depth == DEPTH || IDA(piece, space, idx1, idx2, inv1, inv2, depth)) {
				RESULT[depth - 1] = (char)n;  /*Record the answer steps */
				return TRUE;
			}
			BOARD[space] = 0;
			BOARD[piece] = n;
		}
	}
	return FALSE;
}

int Search(void)
{
	int  space, num1, num2, idx1, idx2, inv1, inv2, wd1, wd2;
	int  id1, id2, lowb1, lowb2, i, j, work[BOARD_WIDTH];
	//棋盘坐标的converse
	int  cnvp[] = { 0, 4, 8,12, 1, 5, 9,13, 2, 6,10,14, 3, 7,11,15 };
	u64  table;

	/* existence check */
	//用oddity of puzzle检查初始状态棋盘是不是无解的
	for (space = 0; BOARD[space]; space++);
	inv1 = (BOARD_WIDTH - 1) - space / BOARD_WIDTH;
	for (i = 0; i<BOARD_SIZE; i++) {
		if (BOARD[i] == 0) continue;
		for (j = i + 1; j<BOARD_SIZE; j++)
			if (BOARD[j] && BOARD[j]<BOARD[i]) inv1++;
	}
	//无解
	if (inv1 & 1) return FALSE;

	/* 初期 IDX1 for WD */
	table = 0;
	//对每一行
	for (i = 0; i<BOARD_WIDTH; i++) {
		for (j = 0; j<BOARD_WIDTH; j++) work[j] = 0;
		for (j = 0; j<BOARD_WIDTH; j++) {
			//num1为在此坐标上的棋子的编号
			num1 = BOARD[i * BOARD_WIDTH + j];
			if (num1 == 0) continue;
			work[(num1 - 1) >> 2]++;
		}
		/*test
		for (int i = 0; i < 4; i++) {
			cout <<"work[ " << i <<"]="<< work[i] << endl;
		}
		*/	
		for (j = 0; j<BOARD_WIDTH; j++)
			table = (table << 3) | work[j];
		/*test
		cout << "table=" << table << endl;
		*/
	}
	for (idx1 = 0; WDPTN[idx1] != table; idx1++);

	/* 初期 IDX2 for WD */
	table = 0;
	//对每一列
	for (i = 0; i<BOARD_WIDTH; i++) {
		for (j = 0; j<BOARD_WIDTH; j++) work[j] = 0;
		for (j = 0; j<BOARD_WIDTH; j++) {
			num2 = CONV[BOARD[j * BOARD_WIDTH + i]];
			if (num2 == 0) continue;
			work[(num2 - 1) >> 2]++;
		}
		for (j = 0; j<BOARD_WIDTH; j++)
			table = (table << 3) | work[j];
	}
	for (idx2 = 0; WDPTN[idx2] != table; idx2++);

	/* 初期 INV1 for ID */
	inv1 = 0;
	for (i = 0; i<BOARD_SIZE; i++) {
		num1 = BOARD[i];
		if (!num1) continue;
		for (j = i + 1; j<BOARD_SIZE; j++) {
			num2 = BOARD[j];
			if (num2 && num2<num1) inv1++;
		}
	}

	/* 初期 INV2 for ID */
	inv2 = 0;
	for (i = 0; i<BOARD_SIZE; i++) {
		num1 = CONV[BOARD[cnvp[i]]];
		if (!num1) continue;
		for (j = i + 1; j<BOARD_SIZE; j++) {
			num2 = CONV[BOARD[cnvp[j]]];
			if (num2 && num2<num1) inv2++;
		}
	}

	/* 初期 LowerBound */
	wd1 = WDTBL[idx1];
	wd2 = WDTBL[idx2];
	id1 = IDTBL[inv1];
	id2 = IDTBL[inv2];
	lowb1 = (wd1 > id1) ? wd1 : id1;
	lowb2 = (wd2 > id2) ? wd2 : id2;
	printf("(WD=%d/%d,ID=%d/%d) LowerBound=%d\n", wd1, wd2, id1, id2, lowb1 + lowb2);

	/* IDA実行 */
	for (DEPTH = lowb1 + lowb2; ; DEPTH += 2) {
		printf("-%d", DEPTH);
		if (IDA(space, -1, idx1, idx2, inv1, inv2, 0)) break;
	}

	return TRUE;
}
int main(void)
{
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	int  i;

	Initialize();

	for (i = 0; i<BOARD_SIZE; i++) {
		if (i && !(i % BOARD_WIDTH)) printf("\n");
		printf("%3d", BOARD[i]);
	}
	printf("\n");

	if (Search()) {
		printf("\n[%d moves]\n", DEPTH);
		QueryPerformanceCounter(&t2);
		printf("Use Time:%f\n", (t2.QuadPart - t1.QuadPart)*1.0 / tc.QuadPart);
		for (i = 0; i<DEPTH; i++) {
			if (i % 10 == 0) printf("\n");
			printf(" %2d ", RESULT[i]);
		}
		printf("\n");
	}
	else {
		printf("Impossible!!\n");
	}

	return 0;
}