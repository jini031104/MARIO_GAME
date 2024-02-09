#include "CCharacter.h"

CCharacter::CCharacter()
{
	nCharacterMoveXY[0] = 0, nCharacterMoveXY[0] = 0;
	bCrash = FALSE, bMonsterDie = FALSE;
}

CCharacter::~CCharacter()
{
}

void CCharacter::CrashCheck(int MarioX1, int MarioX2, int MarioY1, int MarioY2, int Goomba1, int Goomba2, int GoombaY1, int GoombaY2)
{
	int middleX = MarioX2 - 17;
	
	if ((Goomba1 <= MarioX2 && MarioX2 <= Goomba2) || (Goomba1 <= MarioX1 && MarioX1 <= Goomba2) || (Goomba1 <= middleX && middleX <= Goomba2))
	{
		if (GoombaY1 < MarioY2 && MarioY2 < GoombaY2)
		{
			OutputDebugString(" Die\n");
			bMonsterDie = TRUE;
		}
		if ((MarioY1 <= GoombaY1 && GoombaY1 <= MarioY2) && (MarioY1 <= GoombaY2 && GoombaY2 <= MarioY2))
		{
			bCrash = TRUE;
		}
	}
	else
	{
		//bCrash = FALSE;
	}
}