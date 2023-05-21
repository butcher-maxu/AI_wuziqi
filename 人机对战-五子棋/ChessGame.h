#pragma once
#include "AI.h"
#include "Chess.h"
#include "Man.h"


class ChessGame
{
public:
	ChessGame(Man*, AI*, Chess*);

	void play(); //��ʼ�Ծ�


private:
	Man* man;

	Chess* chess;

	AI* ai;
};



