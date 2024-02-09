#pragma once
#include "Define.h"
#include "CCharacter.h"

class CGoomba : public CCharacter
{
public:
	CGoomba();
	~CGoomba();

	// 몬스터 움직임
	void	__MonsterMove(WPARAM wParam, BOOL KeyBuffer[256], int nBgX, int nPlayerX, BOOL bMarioFast, int nX1, int nTotalBgX, float speed);

	// 몬스터 위치
	void	SetMonsterMoveX(int nMoveX) { nCharacterMoveXY[0] = nMoveX; }
	void	SetMonsterMoveY(int nMoveY) { nCharacterMoveXY[1] = nMoveY; }

	int		GetStandX()				const { return nStand[0]; }
	int		GetStandY()				const { return nStand[1]; }

	float	GetCharacterMoveX()		const { return nCharacterMoveXY[0]; }
	float	GetCharacterMoveY()		const { return nCharacterMoveXY[1]; }

private:
	float nCharacterMoveXY[2], jump1, jump2, jumpHeight, jumpTime, nGoombaMove;
	int nMapMove, nStand[2];
};