#pragma once
// �̰��� �������� ��ũ��Ʈ��.
#include "Define.h"
#include "CCharacter.h"

class CPlayer : public CCharacter
{
public:
	CPlayer();
	~CPlayer();

	void	__StandState(WPARAM wParam);																// ĳ���� ���ڸ� �ڼ�
	void	__CharacterMove(WPARAM wParam, BOOL KeyBuffer[256], int nBgX, int nMapCount, float speed);	// ĳ���� ������
	
	void	SetCharacterMoveX(int nMoveX) { nCharacterMoveXY[0] = nMoveX; }		// ĳ���� ��ġ
	void	SetCharacterMoveY(int nMoveY) { nCharacterMoveXY[1] = nMoveY; }

	int		GetStandX()				const { return nStand[0]; }					// �ڼ� ��ȯ
	int		GetStandY()				const { return nStand[1]; }

	float	GetCharacterMoveX()		const { return nCharacterMoveXY[0]; }
	float	GetCharacterMoveY()		const { return nCharacterMoveXY[1]; }

	int		GetCaracterFast()		const { return characterFast; }

	void	SetGameClear(BOOL bClear)				{ GameClear = bClear; }
	void	SetCaracterFast(BOOL bFast)				{ characterFast = bFast; }
	void	SetObstacleXY(int x1, int x2, int y)	{ nX1 = x1, nX2 = x2, nY = y; }		// ��� �� ������, ����, ����. ĳ���� ���� �� ���.

	void	SetStandX(int x)						{ nStand[0] = x; }
	void	SetStandY(int y)						{ nStand[1] = y; }

private:
	int nX1, nX2, nY;
	int spaceDownTime, MoveTime;
	int BeforStandX, BeforStandY, nStand[2]; float nCharacterMoveXY[2];
	float jumpTime, jump1, jump2, jumpHeight, deltaTime, nDy;
	BOOL UnderStand, characterFast, Downing, CrossBorder, GameClear;
};