#include "CGoomba.h"

CGoomba::CGoomba()
{
	nCharacterMoveXY[0] = 0.0f, nCharacterMoveXY[1] = 0.0f;
	jump1 = 0.0f, jump2 = 0.0f, jumpHeight = 0.0f, jumpTime = 0.0f;
	nStand[0] = 26, nStand[1] = 7;
	nGoombaMove = 0, nMapMove = 0;
}

CGoomba::~CGoomba()
{

}

void CGoomba::__MonsterMove(WPARAM wParam, BOOL KeyBuffer[256], int nBgX, int nPlayerX, BOOL bMarioFast, int nX1, int nTotalBgX, float speed)
{
	nGoombaMove = GetCharacterMoveX();
	if (wParam == ID_TM_MAINLOOP)
	{
		if (nCharacterMoveXY[0] + MONSTER_GOOMBA_X_LEN + nTotalBgX < nX1)
		{
			if (nCharacterMoveXY[1] < MONSTER_GOOMBA_Y)
			{
				jump1 = (GOOMBA_JUMP * jumpTime) - (0.5f * GRAVITY * (jumpTime * jumpTime));
				jump2 = (GOOMBA_JUMP * (jumpTime + 0.4f)) - (0.5f * GRAVITY * ((jumpTime + 0.4f) * (jumpTime + 0.4f)));
				jumpHeight = jump2 - jump1;

				nCharacterMoveXY[1] -= jumpHeight;
				jumpTime += 0.2f;
			}
			else
			{
				nCharacterMoveXY[1] = MONSTER_GOOMBA_Y;
			}
		}

		nGoombaMove -= speed;
		SetMonsterMoveX(nGoombaMove + nMapMove);
	}

	if (wParam == ID_TM_ANIMATION)
	{
		nStand[0] += 25;
		if (nStand[0] > 51)
			nStand[0] = 26;
	}
}