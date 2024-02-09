#pragma once
#include "Define.h"

class CCharacter
{
public:
	CCharacter();
	~CCharacter();

	// 플레이어
	virtual void	__CharacterMove(WPARAM wParam, BOOL KeyBuffer[256], int nBgX, int nMapCount, float speed) {}
	virtual void	__StandState(WPARAM wParam)		{}
	virtual void	SetCharacterMoveX(int nMoveX)	{}
	virtual void	SetCharacterMoveY(int nMoveY)	{}
	
	virtual void	SetStandX(int x)				{}
	virtual void	SetStandY(int y)				{}

	virtual void	SetGameClear(BOOL bClear)		{}
	virtual void	SetCaracterFast(BOOL bFast)		{}
	virtual void	SetObstacleXY(int x1, int x2, int y) {}
	virtual int		GetCaracterFast()	const { return 0; }

	// 몬스터
	virtual void	__MonsterMove(WPARAM wParam, BOOL KeyBuffer[256], int nBgX, int nPlayerX, BOOL bMarioFast, int nX1, int nTotalBgX, float speed) {}
	virtual void	SetMonsterMoveX(int nMoveX) {}
	virtual void	SetMonsterMoveY(int nMoveY) {}

	// 공통
	virtual int		GetStandX()			const = 0;
	virtual int		GetStandY()			const = 0;

	virtual float	GetCharacterMoveX()	const = 0;
	virtual float	GetCharacterMoveY()	const = 0;

	// 전용
	void CrashCheck(int MarioX1, int MarioX2, int MarioY1, int MarioY2, int Goomba1, int Goomba2, int GoombaY1, int GoombaY2);
	BOOL CrashReturn()	{ return bCrash; }
	BOOL DieCheck()		{ return bMonsterDie; }

private:
	float nCharacterMoveXY[2];
	BOOL bCrash, bMonsterDie;	// 충돌 감지
};