#pragma once
#include <graphics.h> //easyxͼ�ο�ͷ�ļ�
#include <vector>

using namespace std;

struct ChessPos {
	int row;
	int col;

	ChessPos(int r = 0, int c = 0): row(r), col(c){}
};

typedef enum {
	CHESS_WHITE = -1, //����
	CHESS_BLACK = 1	  //����
}chess_kind_t;


class Chess
{
public:
	Chess(int gradesize, int marginx, int marginy, float chesssize);

	void init(); //���̳�ʼ��

	void chessDown(ChessPos * pos, chess_kind_t kind);	//����

	bool clickBoard(int x, int y, ChessPos* pos); //�����Ч���ӣ������λ��

	int getGradeSize(); //��ȡ���̴�С

	//��ȡָ��λ���Ǻ��廹�ǰ��壬���ǿհ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	bool checkOver();

private:
	int gradesize; //���̳ߴ磨13  15  17   19��
	int margin_x;//���̱߾�x(��)
	int margin_y;//���̱߾�y���ϣ�
	float chesssize;//���Ӵ�С�����̷����С��

	IMAGE chessBlackImg;  //��������
	IMAGE chessWhiteImg;  //��������

	//�洢��ǰ��Ϸ���������ӵ����������1 ����-1  �հ�0
	vector<vector<int>>chessmap;

	//��ʶ���巽��true�� ���巽����    false: ���巽
	bool playerflag;

	//�������
	ChessPos lastPos;

	void updateGameMap(ChessPos * pos);

	bool checkWin();

};

