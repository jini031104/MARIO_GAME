#pragma once
#include "Define.h"
#include <list>

using namespace std;

class CCoin
{
public:
	CCoin();
	~CCoin();

	int GetCoinX() const { return nCoinStandX; }
	int GetCoinY() const { return nCoinStandY; }

	void Rotation(WPARAM wParam);
	void CoinRotaion(WPARAM wParam, CCoin*& coin);
	//void CoinRotaion(WPARAM wParam, list<CCoin> test, list<CCoin>::iterator iter);

	void SetCoin_POS_X(int x) { nCoinX = x; }
	void SetCoin_POS_Y(int y) { nCoinY = y; }

	BOOL GetCoin(int MarioX1, int MarioY1);

private:
	int i = 0;
	int nCoinMotionX[4]{ 38, 56, 74, 92 }, nCoinMotionY = 2;
	int nCoinStandX, nCoinStandY, nCoinX, nCoinY;
	BOOL bGetCoin;
};