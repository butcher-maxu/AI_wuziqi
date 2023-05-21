#pragma once
#include "Chess.h"

class AI
{
public:

	void init(Chess * chess);

	void go();  // ÏÂÆå

private:
	Chess* chess;

	vector<vector<int>> scoreMap; //ÆÀ·Ö

	void calculateScore();

	ChessPos think();
};

