#pragma once
#include <graphics.h> //easyx图形库头文件
#include <vector>

using namespace std;

struct ChessPos {
	int row;
	int col;

	ChessPos(int r = 0, int c = 0): row(r), col(c){}
};

typedef enum {
	CHESS_WHITE = -1, //白棋
	CHESS_BLACK = 1	  //黑棋
}chess_kind_t;


class Chess
{
public:
	Chess(int gradesize, int marginx, int marginy, float chesssize);

	void init(); //棋盘初始化

	void chessDown(ChessPos * pos, chess_kind_t kind);	//落子

	bool clickBoard(int x, int y, ChessPos* pos); //如果有效落子，保存该位置

	int getGradeSize(); //获取棋盘大小

	//获取指定位置是黑棋还是白棋，还是空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	bool checkOver();

private:
	int gradesize; //棋盘尺寸（13  15  17   19）
	int margin_x;//棋盘边距x(左)
	int margin_y;//棋盘边距y（上）
	float chesssize;//棋子大小（棋盘方格大小）

	IMAGE chessBlackImg;  //黑棋棋子
	IMAGE chessWhiteImg;  //白棋棋子

	//存储当前游戏棋盘中棋子的情况，黑子1 白子-1  空白0
	vector<vector<int>>chessmap;

	//标识下棋方，true： 黑棋方走棋    false: 白棋方
	bool playerflag;

	//最近落子
	ChessPos lastPos;

	void updateGameMap(ChessPos * pos);

	bool checkWin();

};

