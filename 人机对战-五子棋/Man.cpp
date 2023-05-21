#include "Man.h"
#include <iostream>
using namespace std;

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go()
{
	ChessPos pos;
	
	while (1) {
		//获取鼠标点击信息
		MOUSEMSG msg = GetMouseMsg();

		//通过chess对象，判断落子是否有小，以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y,&pos)) {
			break;
		}
	}

	cout << "("<<pos.row <<", "<< pos.col<<")\n";

	// 落子
	chess->chessDown(&pos, CHESS_BLACK);
}


Man::~Man()
{
}
