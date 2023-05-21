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
		//��ȡ�������Ϣ
		MOUSEMSG msg = GetMouseMsg();

		//ͨ��chess�����ж������Ƿ���С���Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y,&pos)) {
			break;
		}
	}

	cout << "("<<pos.row <<", "<< pos.col<<")\n";

	// ����
	chess->chessDown(&pos, CHESS_BLACK);
}


Man::~Man()
{
}
