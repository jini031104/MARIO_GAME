#include "CCoin.h"

CCoin::CCoin()
{
	nCoinStandX = 38, nCoinStandY = 0;
	nCoinX = 0, nCoinY = 0;
	bGetCoin = FALSE;
	//OutputDebugString("Coin Create\n");
}

CCoin::~CCoin()
{
}

void CCoin::Rotation(WPARAM wParam)
{
	if (wParam == ID_TM_ANIMATION)
	{
		nCoinStandX = nCoinMotionX[i];
		i++;
		if (3 < i)
			i = 0;
	}
}

void CCoin::CoinRotaion(WPARAM wParam, CCoin*& coin)
{
	if (coin != NULL)
		coin->Rotation(wParam);
}

BOOL CCoin::GetCoin(int MarioX1, int MarioY1)
{
	if (((nCoinX < MarioX1 + PLAYER_X_LEN && MarioX1 + PLAYER_X_LEN < nCoinX + OBJECT_COIN_X_LEN) || (nCoinX < MarioX1 && MarioX1 < nCoinX + OBJECT_COIN_X_LEN)) &&
		(MarioY1 < nCoinY && nCoinY < MarioY1 + PLAYER_Y_LEN))
	{
		bGetCoin = TRUE;
	}

	return bGetCoin;
}