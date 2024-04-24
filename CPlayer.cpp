// 이것은 마리오의 스크립트다.
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
	case MOVE_LEFT:	// 왼쪽
	case MOVE_LEFT_A:
		nStand[0] = 911;
		nStand[1] = 132;
		break;
	case MOVE_RIGHT:	// 오른쪽
	case MOVE_RIGHT_D:
		nStand[0] = 36;
		nStand[1] = 132;
		break;
	case MOVE_DOWN:	// 아래
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

	// 움직이는 애니메이션
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
	// 실제 움직임
	if (wParam == ID_TM_MOVE)
	{	// 점프
		if ((KeyBuffer[MOVE_JUMP] == FALSE || (KeyBuffer[MOVE_JUMP] == TRUE && Downing == TRUE)) && !GameClear)
		{
			//char numString[20] = { 0 };
			
			spaceDownTime = 0.0f;
			if (nDy == LONG_JUMP || nDy == SHORT_JUMP)
			{
				Downing = TRUE;
				jump1 = (nDy * jumpTime) - (0.5f * GRAVITY * (jumpTime * jumpTime));
				jump2 = (nDy * (jumpTime + 0.4f)) - (0.5f * GRAVITY * ((jumpTime + 0.4f) * (jumpTime + 0.4f)));
				jumpHeight = jump2 - jump1;

				if ((nX1 < nCharacterMoveXY[0] + PLAYER_X_LEN && nCharacterMoveXY[0] < nX2)
					&& nY <= nCharacterMoveXY[1] + PLAYER_Y_LEN && nCharacterMoveXY[1] <= nY)
				{
					if (CrossBorder)
					{
						// 캐릭터의 Y축이 전부 허공 지형의 경계선을 넘을 경우에만
						// 즉, 허공 지형에 온전히 착지했을 때는 떨어지는 것을 멈춘다
						nCharacterMoveXY[1] = nY - PLAYER_Y_LEN + 2;
						Downing = FALSE;
					}
					else
					{
						// 허공 지형을 제대로 넘지 못했다면
						// 즉, 제대로 착지하지 못했다면 계속 떨어진다
						nCharacterMoveXY[1] -= jumpHeight;
						jumpTime += 0.2f;
					}
				}
				else
				{
					if (nCharacterMoveXY[1] <= nY && nCharacterMoveXY[1] + PLAYER_Y_LEN <= nY)
					{	// nY는 허공 지형의 Y축 위치
						// 캐릭터의 y축이 전부 경계선을 넘었을 경우에만
						CrossBorder = TRUE;
					}

					nCharacterMoveXY[1] -= jumpHeight;
					jumpTime += 0.2f;
				}

				if (PLAYER_Y < nCharacterMoveXY[1])
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
			if (spaceDownTime < 3)			// 0.2 미만일 경우 짧은 점프
			{
				nDy = SHORT_JUMP;
			}
			else
			{
				nDy = LONG_JUMP;
			}
		}

		if (UnderStand == TRUE && KeyBuffer[MOVE_DOWN] == FALSE && KeyBuffer[MOVE_DOWN_S] == FALSE)	// 아래
		{	// 앉았다가 다시 일어나는 상황
			nStand[0] = BeforStandX;
			nStand[1] = BeforStandY;
			UnderStand = FALSE;
		}
		if (KeyBuffer[MOVE_LEFT] || KeyBuffer[MOVE_LEFT_A])		// 왼쪽
		{
			if (100 <= MoveTime )//&& nSpeed <= 10)	// 5초 이상 이동시 속도 증가
			{
				//nSpeed += 5;
				characterFast = TRUE;
			}
			else
				MoveTime++;

			if (nMapCount > 1 || (nMapCount == 1 && nBgX < 0))
			{	// 맵이 한번 변경되었거나, 최초의 맵이 약간 밀렸을 때, 캐릭터는 nLefttLimit 이상 나아갈 수 없다.
				if (LEFT_LIMIT <= nCharacterMoveXY[0])
					nCharacterMoveXY[0] -= speed;
			}
			else if (0 <= nCharacterMoveXY[0] && nMapCount == 1 && nBgX == 0)
			{	// 최초의 맵이고 움직임이 없었다면 왼쪽 화면까지밖에 못 움직인다.
				nCharacterMoveXY[0] -= speed;
			}
		}
		if (KeyBuffer[MOVE_RIGHT] || KeyBuffer[MOVE_RIGHT_D])		// 오른쪽
		{
			if (100 <= MoveTime )//&& nSpeed <= 10)	// 5초 이상 이동시 속도 증가
			{
				//nSpeed += 5;
				characterFast = TRUE;
			}
			else
				MoveTime++;

			if (nMapCount != MAPLIMIT || (nMapCount == MAPLIMIT && 0 < nBgX))
			{	// 마지막 맵인데 조금 밀렸을 경우, 캐릭터는 nRightLimit 이상 나아갈 수 없다.
				if (nCharacterMoveXY[0] <= RIGHT_LIMIT)
					nCharacterMoveXY[0] += speed;
			}
			else if (nCharacterMoveXY[0] <= SCREEN_X - 50 && nMapCount == MAPLIMIT)
			{
				nCharacterMoveXY[0] += speed;
			}
		}

		if (!KeyBuffer[MOVE_LEFT] && !KeyBuffer[MOVE_LEFT_A] && !KeyBuffer[MOVE_RIGHT] && !KeyBuffer[MOVE_RIGHT_D])
		{	// 움직임을 멈췄을 땐 스피드를 원상태로 돌림
			MoveTime = 0; //nSpeed = 10;
			characterFast = FALSE;
		}
	}
}
