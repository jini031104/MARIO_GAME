#include "CreateAnimation.h"

// ���� �̵�
void __MapMove(WPARAM wParam, CCharacter*& character, int* nTotalBgX, int* nBgX, int* nBgGroundX, int* nMapCount, BOOL KeyBuffer[])
{
	if (wParam == ID_TM_MAINLOOP)
	{
		if (character->GetCharacterMoveX() > RIGHT_LIMIT && (KeyBuffer[MOVE_RIGHT] || KeyBuffer[MOVE_RIGHT_D]))
		{	// ������ �̵�
			if ((*nMapCount) != MAPLIMIT || ((*nMapCount) == MAPLIMIT && 0 < (*nBgX)))
			{
				if (character->GetCaracterFast() == TRUE)	// ĳ���� �̵��� �������� ���� �̵��� ������
				{
					(*nTotalBgX) -= 5;
					(*nBgGroundX) -= 2;
					(*nBgX) -= 5;
				}
				else
				{
					(*nTotalBgX) -= 2;
					(*nBgGroundX) -= 1;
					(*nBgX) -= 2;
				}
			}
			if ((*nBgX) <= -SCREEN_X)
			{
				(*nBgX) = 0;
				(*nTotalBgX) = -SCREEN_X * (*nMapCount);
				(*nMapCount)++;
			}
			if ((*nMapCount) == MAPLIMIT && (*nBgX) < 0)
				(*nBgX) = 0;
		}
		else if (LEFT_LIMIT > character->GetCharacterMoveX() && (KeyBuffer[MOVE_LEFT] || KeyBuffer[MOVE_LEFT_A]))
		{	// ���� �̵�
			if ((*nMapCount) > 1 || ((*nMapCount) == 1 && (*nBgX) < 0))
			{
				if (character->GetCaracterFast() == TRUE)	// ĳ���� �̵��� �������� ���� �̵��� ������
				{
					(*nTotalBgX) += 5;
					(*nBgX) += 5;
				}
				else
				{
					(*nTotalBgX) += 2;
					(*nBgX) += 2;
				}
			}
			if ((*nBgX) >= SCREEN_X)
			{
				(*nBgX) = 0;
				(*nMapCount)--;
				(*nTotalBgX) = (-SCREEN_X * ((*nMapCount) - 1));
			}
			if ((*nMapCount) == 1 && 0 < (*nBgX))
				(*nBgX) = 0;
		}
	}
}

// ���� �̵�
void EnemyMove(WPARAM wParam, CCharacter*& Monster, CCharacter*& character, BOOL KeyBuffer[], int* nBgX, int* nTotalBgX, float speed, int nX1)
{
	if (Monster != NULL)
		Monster->__MonsterMove(wParam, KeyBuffer, *nBgX, character->GetCharacterMoveX(), character->GetCaracterFast(), nX1, *nTotalBgX, speed);
}

// �Ҳɳ��� �̵�
void FlagPosAndMove(WPARAM wParam, int* flagMove)
{
	if (wParam == ID_TM_ANIMATION)
	{
		(*flagMove) += 17;
		if (154 < (*flagMove))
			(*flagMove) = 120;
	}
}

void FireworksUp(WPARAM wParam, int* fireBall_POS_Y, int* fireBallMoveX)
{
	if (wParam == ID_TM_JUMPTIME)
	{
		if (64 < (*fireBall_POS_Y))
		{
			(*fireBall_POS_Y) -= 4;
			(*fireBallMoveX)++;
			if (1 < (*fireBallMoveX))
				(*fireBallMoveX) = 0;
		}
	}
}

void FireworksBoom(WPARAM wParam, int* fireBall_POS_Y, int* fireBall_Stand_X, BOOL *bPlayFireworks)
{
	if (wParam == ID_TM_JUMPTIME)
	{
		if ((*fireBall_POS_Y) <= 64)
		{
			if ((*fireBall_Stand_X) < 290)
				(*fireBall_Stand_X) += 34;
			else
				(*bPlayFireworks) = TRUE;
		}
	}
}

// ���� �ð� ����ȭ
void TimeScore(WPARAM wParam, int* nTime, int* nScore)
{
	if (wParam == ID_TM_JUMPTIME)
	{
		if (-1 < (*nTime))
		{
			if (0 < (*nTime))
				(*nScore)++;
			(*nTime)--;
		}
	}
}

void TimeCount(WPARAM wParam, int* nTime, BOOL* TimeOver)
{
	if (wParam == ID_TM_GAMETIME && 0 < (*nTime))
		(*nTime)--;
	else if ((*nTime) <= 0 && (*TimeOver) == FALSE)
	{
		OutputDebugString("Time Over\n");
		(*TimeOver) = TRUE;
	}
}

void GameOver(CCharacter*& character, int* nTotalBgX, BOOL* GAME_CLEAR, int* nCharacterLife, BOOL* Player_Die)
{
	if (GAME_OVER_LINE + 8 + (*nTotalBgX) <= character->GetCharacterMoveX() + PLAYER_X_LEN && (*GAME_CLEAR) == FALSE)
	{
		character->SetStandX(246);
		character->SetStandY(182);
		(*GAME_CLEAR) = TRUE;
	}

	if ((*nCharacterLife) == 0)
	{
		(*Player_Die) = TRUE;
	}
}

// ������ ���� �ִϸ��̼�
void EndingMove(WPARAM wParam, CCharacter*& character, BOOL* bSleep, BOOL* bEnding, BOOL* bStanding, BOOL KeyBuffer[],
			int* nTotalBgX, int* nBgX, int* nBgGroundX, int* nMapCount, float* speed)
{
	character->SetGameClear(TRUE);
	if (!(*bSleep))
	{	// ��� �����
		Sleep(2000);
		(*bSleep) = TRUE;
	}
	else
	{
		int MarioMoveY = character->GetCharacterMoveY();
		if (MarioMoveY != PLAYER_Y)
		{	// ��߿� �Ŵ޸� ���·� �����´�.
			MarioMoveY++;
			character->SetStandX(281);
			character->SetCharacterMoveY(MarioMoveY);
		}
		else
		{
			// �������� ���������� ���Ѵ�.
			character->SetCharacterMoveY(PLAYER_Y);
			if (!(*bStanding))
			{	// �ڼ� ����
				character->SetStandX(36);
				character->SetStandY(132);
				(*bStanding) = TRUE;
			}
			if ((*nTotalBgX) != -1280)
			{
				KeyBuffer[MOVE_RIGHT] = TRUE;
				character->SetCaracterFast(FALSE);
				character->__CharacterMove(wParam, KeyBuffer, *nBgX, *nMapCount, *speed);
				__MapMove(wParam, character, &(*nTotalBgX), &(*nBgX), &(*nBgGroundX), &(*nMapCount), KeyBuffer);
			}
			else
			{
				character->SetStandX(386);
				character->SetStandY(182);
				(*bEnding) = TRUE;
			}
		}
	}
}