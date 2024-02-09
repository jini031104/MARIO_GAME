// �̰��� �������� ��ũ��Ʈ��.
#include "CPlayer.h"

CPlayer::CPlayer()
{
	nStand[0] = 36, nStand[1] = 132;
	nCharacterMoveXY[0] = 200, nCharacterMoveXY[1] = PLAYER_Y;
	nDy = 0.0f, spaceDownTime = 0, MoveTime = 0;
	BeforStandX = 0, BeforStandY = 0;
	UnderStand = FALSE, characterFast = FALSE, Downing = FALSE, CrossBorder = FALSE, GameClear = FALSE;
	jumpTime = 0.0f, jump1 = 0.0f, jump2 = 0.0f, jumpHeight = 0.0f, deltaTime = 0.0f;

	nX1 = 0, nX2 = 0, nY = 0;
}

CPlayer::~CPlayer()
{

}

void CPlayer::__StandState(WPARAM wParam)
{
	//UnderStand = FALSE;
	switch (wParam)
	{
	case MOVE_LEFT:	// ����
	case MOVE_LEFT_A:
		nStand[0] = 911;
		nStand[1] = 132;
		break;
	case MOVE_RIGHT:	// ������
	case MOVE_RIGHT_D:
		nStand[0] = 36;
		nStand[1] = 132;
		break;
	case MOVE_DOWN:	// �Ʒ�
	case MOVE_DOWN_S:
		if (!UnderStand)
		{
			BeforStandX = nStand[0];
			BeforStandY = nStand[1];
		}
		UnderStand = TRUE;
		if (BeforStandX == 36)
		{
			nStand[0] = 421;
			nStand[1] = 132;
		}
		else if (BeforStandX == 911)
		{
			nStand[0] = 491;
			nStand[1] = 132;
		}
		break;
	}
}

void CPlayer::__CharacterMove(WPARAM wParam, BOOL KeyBuffer[256], int nBgX, int nMapCount, float speed)
{
	if (KeyBuffer[MOVE_JUMP] && wParam == ID_TM_JUMPTIME)
		spaceDownTime++;

	// �����̴� �ִϸ��̼�
	if (wParam == ID_TM_ANIMATION)
	{
		if (KeyBuffer[MOVE_LEFT] || KeyBuffer[MOVE_LEFT_A])
		{
			nStand[0] -= 35;
			if (nStand[0] < 841)
				nStand[0] = 911;
		}
		else if (KeyBuffer[MOVE_RIGHT] || KeyBuffer[MOVE_RIGHT_D])
		{
			nStand[0] += 35;
			if (nStand[0] > 106)
				nStand[0] = 36;
		}
	}
	// ���� ������
	if (wParam == ID_TM_MOVE)
	{	// ����
		if ((KeyBuffer[MOVE_JUMP] == FALSE || (KeyBuffer[MOVE_JUMP] == TRUE && Downing == TRUE)) && !GameClear)
		{
			//char numString[20] = { 0 };
			
			spaceDownTime = 0.0f;
			if (nDy == LONG_JUMP || nDy == SHORT_JUMP)
			{
				Downing = TRUE;

				if ((nX1 < nCharacterMoveXY[0] + PLAYER_X_LEN && nCharacterMoveXY[0] < nX2)
					&& nY <= nCharacterMoveXY[1] + PLAYER_Y_LEN && nCharacterMoveXY[1] <= nY)
				{
					jump1 = (nDy * jumpTime) - (0.5f * GRAVITY * (jumpTime * jumpTime));
					jump2 = (nDy * (jumpTime + 0.4f)) - (0.5f * GRAVITY * ((jumpTime + 0.4f) * (jumpTime + 0.4f)));
					jumpHeight = jump2 - jump1;

					if (jumpHeight >= 0)
					{
						nCharacterMoveXY[1] -= jumpHeight;
						jumpTime += 0.2f;
					}
					else
					{
						if (CrossBorder)
						{
							nCharacterMoveXY[1] = nY - PLAYER_Y_LEN + 2;
							Downing = FALSE;
						}
						else
						{
							nCharacterMoveXY[1] -= jumpHeight;
							jumpTime += 0.2f;
						}
					}
				}
				else
				{
					if (nCharacterMoveXY[1] <= nY && nCharacterMoveXY[1] + PLAYER_Y_LEN <= nY)
					{	// y���� ���� ��輱�� �Ѿ��� ��쿡��
						CrossBorder = TRUE;
					}
					jump1 = (nDy * jumpTime) - (0.5f * GRAVITY * (jumpTime * jumpTime));
					jump2 = (nDy * (jumpTime + 0.4f)) - (0.5f * GRAVITY * ((jumpTime + 0.4f) * (jumpTime + 0.4f)));
					jumpHeight = jump2 - jump1;

					char numString[10] = { 0 };
					itoa(jump1, numString, 10);
					OutputDebugString("jump1: ");
					OutputDebugString(numString);

					itoa(jump2, numString, 10);
					OutputDebugString("		jump2: ");
					OutputDebugString(numString);

					itoa(jumpHeight, numString, 10);
					OutputDebugString("		jumpHeight: ");
					OutputDebugString(numString);
					OutputDebugString("\n");

					nCharacterMoveXY[1] -= jumpHeight;
					jumpTime += 0.2f;
				}

				if (273 < nCharacterMoveXY[1])
				{
					nDy = 0.0f;
					nCharacterMoveXY[1] = PLAYER_Y;
					Downing = FALSE;
				}
			}
		}
		else if (KeyBuffer[MOVE_JUMP] == TRUE && Downing == FALSE)
		{
			CrossBorder = FALSE;
			jumpTime = 0.0f;
			if (spaceDownTime < 3)			// 0.2 �̸��� ��� ª�� ����
			{
				nDy = SHORT_JUMP;
			}
			else
			{
				nDy = LONG_JUMP;
			}
		}

		if (UnderStand == TRUE && KeyBuffer[MOVE_DOWN] == FALSE && KeyBuffer[MOVE_DOWN_S] == FALSE)	// �Ʒ�
		{	// �ɾҴٰ� �ٽ� �Ͼ�� ��Ȳ
			nStand[0] = BeforStandX;
			nStand[1] = BeforStandY;
			UnderStand = FALSE;
		}
		if (KeyBuffer[MOVE_LEFT] || KeyBuffer[MOVE_LEFT_A])		// ����
		{
			if (100 <= MoveTime )//&& nSpeed <= 10)	// 5�� �̻� �̵��� �ӵ� ����
			{
				//nSpeed += 5;
				characterFast = TRUE;
			}
			else
				MoveTime++;

			if (nMapCount > 1 || (nMapCount == 1 && nBgX < 0))
			{	// ���� �ѹ� ����Ǿ��ų�, ������ ���� �ణ �з��� ��, ĳ���ʹ� nLefttLimit �̻� ���ư� �� ����.
				if (LEFT_LIMIT <= nCharacterMoveXY[0])
					nCharacterMoveXY[0] -= speed;
			}
			else if (0 <= nCharacterMoveXY[0] && nMapCount == 1 && nBgX == 0)
			{	// ������ ���̰� �������� �����ٸ� ���� ȭ������ۿ� �� �����δ�.
				nCharacterMoveXY[0] -= speed;
			}
		}
		if (KeyBuffer[MOVE_RIGHT] || KeyBuffer[MOVE_RIGHT_D])		// ������
		{
			if (100 <= MoveTime )//&& nSpeed <= 10)	// 5�� �̻� �̵��� �ӵ� ����
			{
				//nSpeed += 5;
				characterFast = TRUE;
			}
			else
				MoveTime++;

			if (nMapCount != MAPLIMIT || (nMapCount == MAPLIMIT && 0 < nBgX))
			{	// ������ ���ε� ���� �з��� ���, ĳ���ʹ� nRightLimit �̻� ���ư� �� ����.
				if (nCharacterMoveXY[0] <= RIGHT_LIMIT)
					nCharacterMoveXY[0] += speed;
			}
			else if (nCharacterMoveXY[0] <= SCREEN_X - 50 && nMapCount == MAPLIMIT)
			{
				nCharacterMoveXY[0] += speed;
			}
		}

		if (!KeyBuffer[MOVE_LEFT] && !KeyBuffer[MOVE_LEFT_A] && !KeyBuffer[MOVE_RIGHT] && !KeyBuffer[MOVE_RIGHT_D])
		{	// �������� ������ �� ���ǵ带 �����·� ����
			MoveTime = 0; //nSpeed = 10;
			characterFast = FALSE;
		}
	}
}