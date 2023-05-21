#include "Chess.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <conio.h>

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
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

	//开始棋子都是空白
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
	// 创建游戏窗口
	initgraph(897, 895);

	// 显示棋盘图片
	loadimage(0, "res/棋盘2.jpg");

	//播放开始提示音
	mciSendString("play res/start.wav", 0, 0, 0); //需要修改字符集为多字节字符集

	//加载黑棋和白棋棋子的图片（放到内存里面）
	loadimage(&chessBlackImg, "res/black.png", chesssize, chesssize, true);
	loadimage(&chessWhiteImg, "res/white.png", chesssize, chesssize, true);

	// 因为反复下棋，棋盘清0
	for (int i = 0; i < chessmap.size(); i++) {
		for (int j = 0; j < chessmap[0].size(); j++) {
			chessmap[i][j] = 0;
		}
	}

	playerflag = true; //黑棋先走
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

		// 距离右上角的距离
		len = sqrt((x - leftTopPosX - chesssize) * (x - leftTopPosX - chesssize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			pos->row = row;
			pos->col = col + 1;
			if (chessmap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离左下角的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chesssize) * (y - leftTopPosY - chesssize));
		if (len < offset) {
			pos->row = row + 1;
			pos->col = col;
			if (chessmap[pos->row][pos->col] == 0) {
				selectPos = true;
			}
			break;
		}

		// 距离右下角的距离
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
	//结束返回true, 否则返回false
	if (checkWin())
	{
		Sleep(1000);
		if (playerflag == false)
		{
			mciSendString("play res/不错.mp3", 0, 0, 0);
			loadimage(0, "res/胜利.jpg");
		}
		else {
			mciSendString("play res/失败.mp3", 0, 0, 0);
			loadimage(0, "res/失败.jpg");
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
	playerflag = !playerflag; // 黑白交换
}

bool Chess::checkWin()
{
	// 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
	// 水平方向
	int row = lastPos.row;
	int col = lastPos.col;

	for (int i = 0; i < 5; i++)
	{
		// 往左5个，往右匹配4个子，20种情况
		if (col - i >= 0 &&
			col - i + 4 < gradesize &&
			chessmap[row][col - i] == chessmap[row][col - i + 1] &&
			chessmap[row][col - i] == chessmap[row][col - i + 2] &&
			chessmap[row][col - i] == chessmap[row][col - i + 3] &&
			chessmap[row][col - i] == chessmap[row][col - i + 4])
			return true;
	}

	// 竖直方向(上下延伸4个)
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

	// “/"方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradesize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < gradesize &&
			// 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
			chessmap[row + i][col - i] == chessmap[row + i - 1][col - i + 1] &&
			chessmap[row + i][col - i] == chessmap[row + i - 2][col - i + 2] &&
			chessmap[row + i][col - i] == chessmap[row + i - 3][col - i + 3] &&
			chessmap[row + i][col - i] == chessmap[row + i - 4][col - i + 4])
			return true;
	}

	// “\“ 方向
	for (int i = 0; i < 5; i++)
	{
		// 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
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
