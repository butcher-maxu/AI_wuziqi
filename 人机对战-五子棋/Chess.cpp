#include "Chess.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>

void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

Chess::Chess(int gradesize, int marginx, int marginy, float chesssize)
{
	this->gradesize = gradesize;
	this->margin_x = marginx;
	this->margin_y = marginx;
	this->chesssize = chesssize;

	playerflag = CHESS_BLACK;

	//��ʼ���Ӷ��ǿհ�
	for (int i = 0; i < gradesize; i++)
	{
		vector<int> line;
		for (int j = 0; j< gradesize; j++)
			line.push_back(0);
		chessmap.push_back(line);
	}
}

void Chess::init()
{
	// ������Ϸ����
	initgraph(897, 895);

	// ��ʾ����ͼƬ
	loadimage(0, "res/����2.jpg");

	//���ſ�ʼ��ʾ��
	mciSendString("play res/start.wav", 0, 0, 0); //��Ҫ�޸��ַ���Ϊ���ֽ��ַ���

	//���غ���Ͱ������ӵ�ͼƬ���ŵ��ڴ����棩
	loadimage(&chessBlackImg, "res/black.png", chesssize, chesssize, true);
	loadimage(&chessWhiteImg, "res/white.png", chesssize, chesssize, true);

	// ��Ϊ�������壬������0
	for (int i = 0; i < chessmap.size(); i++) {
		for (int j = 0; j < chessmap[0].size(); j++) {
			chessmap[i][j] = 0;
		}
	}

	playerflag = true; //��������
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	mciSendString("play res/down7.wav", 0,  0, 0);
	int x = margin_x + chesssize * pos->col - 0.5 * chesssize;
	int y = margin_x + chesssize * pos->row - 0.5 * chesssize;

	if (kind == CHESS_BLACK)
	{
		putimagePNG(x, y, &chessBlackImg);
	}
	else {
		putimagePNG(x, y, &chessWhiteImg);
	}
	updateGameMap(pos);
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chesssize;
	int row = (y - margin_y) / chesssize;
	int leftTopPosX = margin_x + chesssize * col;
	int leftTopPosY = margin_y + chesssize * row;
	int offset = chesssize * 0.4; // 

	int len;
	bool selectPos = false;

	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col;
			if (chessmap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������Ͻǵľ���
		len = sqrt((x - leftTopPosX - chesssize) * (x - leftTopPosX - chesssize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessmap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chesssize) * (y - leftTopPosY - chesssize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessmap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX - chesssize) * (x - leftTopPosX - chesssize) + (y - leftTopPosY - chesssize) * (y - leftTopPosY - chesssize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col + 1;

			if (chessmap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}
	} while (0);

	return selectPos;
}

int Chess::getGradeSize()
{
	return gradesize;
}

int Chess::getChessData(ChessPos* pos)
{
	return chessmap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
	return chessmap[row][col];
}

bool Chess::checkOver()
{
	//��������true, ���򷵻�false
	if (checkWin())
	{
		Sleep(1000);
		if (playerflag == false)
		{
			mciSendString("play res/����.mp3", 0, 0, 0);
			loadimage(0, "res/ʤ��.jpg");
		}
		else {
			mciSendString("play res/ʧ��.mp3", 0, 0, 0);
			loadimage(0, "res/ʧ��.jpg");
		}
		_getch();
		return true;
		
	}

	return false;
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessmap[pos->row][pos->col] = playerflag ? CHESS_BLACK : CHESS_WHITE;
	playerflag = !playerflag; // �ڰ׽���
}

bool Chess::checkWin()
{
	// ����б���ִ������ÿ����������ݵ�ǰ�����������5�����ӣ���һ�ַ��Ͼ���Ӯ
	// ˮƽ����
	int row = lastPos.row;
	int col = lastPos.col;

	for (int i = 0; i < 5; i++)
	{
		// ����5��������ƥ��4���ӣ�20�����
		if (col - i >= 0 &&
			col - i + 4 < gradesize &&
			chessmap[row][col - i] == chessmap[row][col - i + 1] &&
			chessmap[row][col - i] == chessmap[row][col - i + 2] &&
			chessmap[row][col - i] == chessmap[row][col - i + 3] &&
			chessmap[row][col - i] == chessmap[row][col - i + 4])
			return true;
	}

	// ��ֱ����(��������4��)
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 &&
			row - i + 4 < gradesize &&
			chessmap[row - i][col] == chessmap[row - i + 1][col] &&
			chessmap[row - i][col] == chessmap[row - i + 2][col] &&
			chessmap[row - i][col] == chessmap[row - i + 3][col] &&
			chessmap[row - i][col] == chessmap[row - i + 4][col])
			return true;
	}

	// ��/"����
	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradesize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < gradesize &&
			// ��[row+i]�У���[col-i]�����ӣ������Ϸ�����4�����Ӷ���ͬ
			chessmap[row + i][col - i] == chessmap[row + i - 1][col - i + 1] &&
			chessmap[row + i][col - i] == chessmap[row + i - 2][col - i + 2] &&
			chessmap[row + i][col - i] == chessmap[row + i - 3][col - i + 3] &&
			chessmap[row + i][col - i] == chessmap[row + i - 4][col - i + 4])
			return true;
	}

	// ��\�� ����
	for (int i = 0; i < 5; i++)
	{
		// ��[row+i]�У���[col-i]�����ӣ������·�����4�����Ӷ���ͬ
		if (row - i >= 0 &&
			row - i + 4 < gradesize &&
			col - i >= 0 &&
			col - i + 4 < gradesize &&
			chessmap[row - i][col - i] == chessmap[row - i + 1][col - i + 1] &&
			chessmap[row - i][col - i] == chessmap[row - i + 2][col - i + 2] &&
			chessmap[row - i][col - i] == chessmap[row - i + 3][col - i + 3] &&
			chessmap[row - i][col - i] == chessmap[row - i + 4][col - i + 4])
			return true;
	}
	return false;
}
