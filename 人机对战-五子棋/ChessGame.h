#pragma once
#include "AI.h"
#include "Chess.h"
#include "Man.h"


class ChessGame
{
public:
	ChessGame(Man*, AI*, Chess*);

	void play(); //开始对局


private:
	Man* man;

	Chess* chess;

	AI* ai;
};



