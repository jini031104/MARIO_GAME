#include "CreateCoin.h"

void CreateCoinPoint(CCoin*& coin, int nSpawnX, int nSpawnY, BOOL* CoinCreateCheck)
{
	//BOOL bCoinGetCheck = FALSE;
	if (coin == NULL && *CoinCreateCheck == FALSE)
	{
		coin = new CCoin;
		*CoinCreateCheck = TRUE;
	}
	else if (coin != NULL)
	{
		coin->SetCoin_POS_X(nSpawnX);
		coin->SetCoin_POS_Y(nSpawnY);
	}
}

void GetCoin(CCoin*& coin, int *nScore, int *nCoin)
{
	*nScore += 100;
	(*nCoin)++;

	coin = NULL;
	delete(coin);
}