#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <cstdio>

#include "GraphicEngine.h"
#include "CPlayer.h"
#include "CCoin.h"

#include "CreateEnemy.h"
#include "CreateCoin.h"
#include "CreateAnimation.h"

#include "SaveRecord.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void __DestroyAll();
void ___aMainLoop(HWND hWnd);
void __GroundPaint();
void __MapPaint();
void BackGroundPaint();
void FireworksAnimation();
void ScoreInputUI(HWND hWnd);
void ScoreList(HWND hWnd);

HINSTANCE g_hInst;
HWND hWndMain;
LPSTR lpszClass="MARIO";

SURFACEINFO g_sfBack;			// 버퍼 표면
SURFACEINFO g_sfBG;				// 배경 표면 
SURFACEINFO g_sfBG_Ground;		// 배경 땅 표면 
SURFACEINFO g_sfBG_Object;		// 배경 오브젝트 표면 

SURFACEINFO g_sfCaracter;		// 캐릭터 표면
SURFACEINFO g_sfMonster;		// 몬스터 표면
SURFACEINFO g_sCoin;			// 코인 표면
SURFACEINFO g_sWrite;			// 글씨 표면

CCoin* Coin[OBJECT_COIN_MAX];
CCharacter* Mario = new CPlayer;
CCharacter* Goomba[MONSTER_GOOMBA_MAX];
BOOL MonsterLifeCode[MONSTER_GOOMBA_MAX] = { FALSE };
BOOL CoinCreateCheck[OBJECT_COIN_MAX] = { FALSE };

int nBgX, nBgGroundX, nTotalBgX = 0, nTime = GAME_LIMIT_TIME, nScore = 0, nCoin = 0, nMapCount = 1;
BOOL KeyBuffer[256], TimeOver = FALSE, GAME_CLEAR = FALSE, Player_Die = FALSE, EndingSleep = FALSE, Next_UI = FALSE;

int flagMove = 120;
int fireBall_POS_X[2] = { 1792, 1800 }, fireBall_POS_Y = 160, fireBall_Stand_X[3] = { 188, 188, 188 }, fireBallMoveX = 0;
BOOL bRePlayFireBall[2] = { FALSE, FALSE }, bTimeScore = FALSE;

BOOL bSleep = FALSE, bStanding = FALSE, bEnding = FALSE;
int tile[TILE_MAX];// = { 0, 16, 32, 48, 64, 80... };

int nCharacterLife = 1;
BOOL check = FALSE;

LARGE_INTEGER	g_second;
LARGE_INTEGER	g_time;
float			g_deltatime;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT, SCREEN_X, SCREEN_Y,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	QueryPerformanceFrequency(&g_second);
	QueryPerformanceCounter(&g_time);

	while (1)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&Message, NULL, 0, 0)) return (int)Message.wParam;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			___aMainLoop(hWnd);
		}
	}
	return Message.wParam;
}

float speed = 1.0f;
float GoombaSpeed = 1.0f;

// fps =		1초 동안 표시되는 프레임 수
// deltaTime =	이전 프레임과 현재 프레임의 시간 차이
float fps = 0;
float timeDelta;
void FPS()
{
	static DWORD frameCount = 0;					//프레임 카운트수
	static float timeElapsed = 0.0f;				//흐른 시간
	static DWORD lastTime = timeGetTime();			//마지막 시간(temp변수)
													
	DWORD curTime = timeGetTime();					//현재 시간
	timeDelta = (curTime - lastTime) * 0.001f;		//timeDelta(1번생성후 흐른 시간) 1초단위로 바꿔준다.

	timeElapsed += timeDelta;
	frameCount++;

	if (timeElapsed >= 1.0f)         //흐른시간이 1초이상이면 내가 하고싶은것 처리
	{
		fps = (float)frameCount / timeElapsed;

		frameCount = 0;
		timeElapsed = 0.0f;
		if (Mario->GetCaracterFast() == TRUE)
		{
			speed = fps * FAST_SPEED * timeDelta;
		}
		else
		{
			speed = fps * SLOW_SPEED * timeDelta;
		}
		GoombaSpeed = SLOW_SPEED * timeDelta;
	}
	else
	{
		Sleep(0.002f * 1000);   // 강의실에선 65fps...
		//흐른 시간이 1초가 안되면 생략함  
		//Sleep() 해도되고 안해도 되구~ 
	}

	lastTime = curTime;
}

//// 일괄 표면 파괴
void __DestroyAll()
{
	__ReleaseSurface(&g_sfCaracter);
	__ReleaseSurface(&g_sfBG);
	__ReleaseSurface(&g_sfBack);
	__ReleaseSurface(&g_sfBG_Object);
	__ReleaseSurface(&g_sfMonster);
	__ReleaseSurface(&g_sCoin);
	__ReleaseSurface(&g_sWrite);
}

//// 일괄 표면 생성 
void __Init(HDC dcScreen)
{
	__CreateBackBuffer(dcScreen, SCREEN_X, SCREEN_Y, &g_sfBack);

	__SetImgSurface(&g_sfBG);
	g_sfBG.hBmp = __MakeDDBFromDIB(dcScreen, "Cloud.bmp");
	__LoadSurface(dcScreen, &g_sfBG);

	__SetImgSurface(&g_sfBG_Ground);
	g_sfBG_Ground.hBmp = __MakeDDBFromDIB(dcScreen, "Ground.bmp");
	__LoadSurface(dcScreen, &g_sfBG_Ground);

	__SetSprSurface(&g_sfBG_Object, RGB(255, 255, 255));
	g_sfBG_Object.hBmp = __MakeDDBFromDIB(dcScreen, "Object.bmp");
	__LoadSurface(dcScreen, &g_sfBG_Object);

	__SetSprSurface(&g_sfCaracter, RGB(255, 255, 255));
	g_sfCaracter.hBmp = __MakeDDBFromDIB(dcScreen, "Mario.bmp");
	__LoadSurface(dcScreen, &g_sfCaracter);

	__SetSprSurface(&g_sfMonster, RGB(255, 255, 255));
	g_sfMonster.hBmp = __MakeDDBFromDIB(dcScreen, "Monster.bmp");
	__LoadSurface(dcScreen, &g_sfMonster);

	__SetSprSurface(&g_sCoin, RGB(128, 128, 192));
	g_sCoin.hBmp = __MakeDDBFromDIB(dcScreen, "Coin.bmp");
	__LoadSurface(dcScreen, &g_sCoin);

	__SetSprSurface(&g_sWrite, RGB(192, 192, 192));
	g_sWrite.hBmp = __MakeDDBFromDIB(dcScreen, "Write.bmp");
	__LoadSurface(dcScreen, &g_sWrite);
}

void GroupEnemyMove(WPARAM wParam)
{
	EnemyMove(wParam, Goomba[0], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed);
	EnemyMove(wParam, Goomba[1], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed, 416 + nTotalBgX);
	EnemyMove(wParam, Goomba[2], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed);
	EnemyMove(wParam, Goomba[3], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed, 800 + nTotalBgX);
	EnemyMove(wParam, Goomba[4], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed, 864 + nTotalBgX);
	EnemyMove(wParam, Goomba[5], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed, 1136 + nTotalBgX);
	EnemyMove(wParam, Goomba[6], Mario, KeyBuffer, &nBgX, &nTotalBgX, GoombaSpeed, 1296 + nTotalBgX);
}

void GroupCoinRotaion(WPARAM wParam)
{
	for (int i = 0; i < OBJECT_COIN_MAX; i++)
	{
		Coin[i]->CoinRotaion(wParam, Coin[i]);
	}
}

int nScoreList[10] = { 0 };
char chName[10][23];

char string[256];
int	nLen = 0;
BOOL Score_UI = FALSE;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC dcScreen;
	static int cx, cy;

	switch (iMessage)
	{
	case WM_CREATE:
		dcScreen = GetDC(hWnd);
		__Init(dcScreen);
		ReleaseDC(hWnd, dcScreen);
		{
			SetTimer(hWnd, ID_TM_MAINLOOP, 16, NULL);
			SetTimer(hWnd, ID_TM_MOVE, 15, NULL);
		
			SetTimer(hWnd, ID_TM_JUMPTIME, 100, NULL);
			SetTimer(hWnd, ID_TM_GAMETIME, 1000, NULL);
			SetTimer(hWnd, ID_TM_ANIMATION, 160, NULL);
			SetTimer(hWnd, ID_TM_FIREBALL, 20, NULL);
		}
		return 0;
	case WM_MOUSEMOVE:
		cx = GET_X_LPARAM(lParam);
		cy = GET_Y_LPARAM(lParam);
		return 0;

	case WM_LBUTTONDOWN:
		if (Next_UI == TRUE)
		{
			if (290 <= cx && cx <= 350 && 250 <= cy && cy <= 290)
			{
				OutputDebugString("색이 달라요!\n");
				SaveRecord(nScore, nScoreList, string, chName);
				Score_UI = TRUE;
			}
		}
		return 0;

	case WM_KEYDOWN:
		if (!GAME_CLEAR && !TimeOver && !Player_Die)
		{
			KeyBuffer[wParam] = TRUE;
			if (check == FALSE)
			{
				Mario->__StandState(wParam);;
				check = TRUE;
			}
		}
		if (EndingSleep)
		{
			string[nLen++] = (char)wParam;
		}
		return 0;

	case WM_TIMER:
		if (GAME_CLEAR)
		{
			EndingMove(wParam, Mario, &bSleep, &bEnding, &bStanding, KeyBuffer, &nTotalBgX, &nBgX, &nBgGroundX , &nMapCount, &speed);
			int MarioMoveY = Mario->GetCharacterMoveY();
			if (MarioMoveY == PLAYER_Y)
			{
				if (nTotalBgX == -1280)
				{
					FireworksUp(wParam, &fireBall_POS_Y, &fireBallMoveX);
					FireworksBoom(wParam, &fireBall_POS_Y, &fireBall_Stand_X[0], &bRePlayFireBall[0]);
					if (bRePlayFireBall[0])
						FireworksBoom(wParam, &fireBall_POS_Y, &fireBall_Stand_X[1], &bRePlayFireBall[1]);
					if (bRePlayFireBall[1])
						FireworksBoom(wParam, &fireBall_POS_Y, &fireBall_Stand_X[2], &bTimeScore);
					if (bTimeScore)
						TimeScore(wParam, &nTime, &nScore);

					if (nTime == 0)
					{
						Next_UI = TRUE;
					}
				}
			}
		}
		else if (!GAME_CLEAR && !TimeOver && !Player_Die)
		{
			TimeCount(wParam, &nTime, &TimeOver);
			Mario->__CharacterMove(wParam, KeyBuffer, nBgX, nMapCount, speed);
			GroupEnemyMove(wParam);
			GroupCoinRotaion(wParam);
			FlagPosAndMove(wParam, &flagMove);
			__MapMove(wParam, Mario, &nTotalBgX, &nBgX, &nBgGroundX, &nMapCount, KeyBuffer);
		}
		return 0;

	case WM_KEYUP:
	{
		if (!GAME_CLEAR && !TimeOver && !Player_Die)
		{
			Mario->__StandState(wParam);
			KeyBuffer[wParam] = FALSE;
			check = FALSE;
		}
		break;
	}

	case WM_DESTROY:
	{
		KillTimer(hWnd, ID_TM_MAINLOOP);
		KillTimer(hWnd, ID_TM_MOVE);
		KillTimer(hWnd, ID_TM_JUMPTIME);
		KillTimer(hWnd, ID_TM_GAMETIME);
		KillTimer(hWnd, ID_TM_ANIMATION);
		KillTimer(hWnd, ID_TM_FIREBALL);
		__DestroyAll();
		PostQuitMessage(0);
		return 0;
	}

	default:
		break;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

// 몬스터를 생성하고 충돌을 체크하여 사망여부를 따진다.
void CreateEnemy(CCharacter*& Monster, int nMonsterType, int nMonsterspawnX, int nMonsterspawnY,
				int nMonster_XSize, int nMonster_YSize, BOOL *MonsterLifeCode)
{
	int nTotal = nMonsterspawnX + nTotalBgX;
	if (Monster == NULL && *MonsterLifeCode == FALSE && nTotal < SCREEN_X)
	{
		// SCREEN_X과 몬스터의 위치를 비교하며 생성 시점을 조절한다
		CreateEnemyPoint(Monster, nMonsterType, nMonsterspawnX, nMonsterspawnY);	// 적 생성 및 생성 위치 선정
		*MonsterLifeCode = TRUE;
	}

	if (Monster != NULL)	// 몬스터가 생성되었을 경우
	{
		// 적
		__PutSprite(g_sfBack.dcSurface, Monster->GetCharacterMoveX() + nTotalBgX, Monster->GetCharacterMoveY(),
			24, 24, Monster->GetStandX(), Monster->GetStandY(), &g_sfMonster);

		EnemyOrPlayerDie(Monster, Mario, &nTotalBgX, &nMonster_XSize, &nMonster_YSize, &nScore, &nCharacterLife);
	}
}

void CreateCoin(CCoin*& coin, int nSpawnX, int nSpawnY, BOOL* CoinCreateCheck)
{
	CreateCoinPoint(coin, nSpawnX, nSpawnY, CoinCreateCheck);
	BOOL bCoinGetCheck = FALSE;

	if (coin != NULL)
	{
		__PutSprite(g_sfBack.dcSurface, nSpawnX, nSpawnY, OBJECT_COIN_X_LEN, OBJECT_COIN_Y_LEN, coin->GetCoinX(), 2, &g_sCoin);
		bCoinGetCheck = coin->GetCoin(Mario->GetCharacterMoveX(), Mario->GetCharacterMoveY());
		if (bCoinGetCheck)
		{
			GetCoin(coin, &nScore, &nCoin);
		}
	}
}
void CoinSet()
{
	CreateCoin(Coin[0], 320 + nTotalBgX, 256, &CoinCreateCheck[0]);
	CreateCoin(Coin[1], 352 + nTotalBgX, 256, &CoinCreateCheck[1]);
	CreateCoin(Coin[2], 416 + nTotalBgX, 160, &CoinCreateCheck[2]);
	CreateCoin(Coin[3], 448 + nTotalBgX, 160, &CoinCreateCheck[3]);
	CreateCoin(Coin[4], 624 + nTotalBgX, 224, &CoinCreateCheck[4]);

	CreateCoin(Coin[5], 656 + nTotalBgX, 224, &CoinCreateCheck[5]);
	CreateCoin(Coin[6], 704 + nTotalBgX, 224, &CoinCreateCheck[6]);
	CreateCoin(Coin[7], 736 + nTotalBgX, 224, &CoinCreateCheck[7]);
	CreateCoin(Coin[8], 784 + nTotalBgX, 224, &CoinCreateCheck[8]);
	CreateCoin(Coin[9], 816 + nTotalBgX, 224, &CoinCreateCheck[9]);
	
	CreateCoin(Coin[10], 864 + nTotalBgX, 176, &CoinCreateCheck[10]);
	CreateCoin(Coin[11], 896 + nTotalBgX, 176, &CoinCreateCheck[11]);
	CreateCoin(Coin[12], 944 + nTotalBgX, 304, &CoinCreateCheck[12]);
	CreateCoin(Coin[13], 976 + nTotalBgX, 304, &CoinCreateCheck[13]);
	CreateCoin(Coin[14], 1008 + nTotalBgX, 304, &CoinCreateCheck[14]);
	
	CreateCoin(Coin[15], 1040 + nTotalBgX, 304, &CoinCreateCheck[15]);
	CreateCoin(Coin[16], 1072 + nTotalBgX, 304, &CoinCreateCheck[16]);
	CreateCoin(Coin[17], 1120 + nTotalBgX, 208, &CoinCreateCheck[17]);
	CreateCoin(Coin[18], 1152 + nTotalBgX, 208, &CoinCreateCheck[18]);
	CreateCoin(Coin[19], 1200 + nTotalBgX, 160, &CoinCreateCheck[19]);
	
	CreateCoin(Coin[20], 1232 + nTotalBgX, 160, &CoinCreateCheck[20]);
	CreateCoin(Coin[21], 1296 + nTotalBgX, 128, &CoinCreateCheck[21]);
	CreateCoin(Coin[22], 1328 + nTotalBgX, 128, &CoinCreateCheck[22]);
}

void ___aMainLoop(HWND hWnd)
{
	HDC   dcScreen;
	FPS();
	
	if (Score_UI == TRUE)
	{
		ScoreList(hWnd);
	}
	else if (nTime < 0 && Next_UI && Score_UI != TRUE)
	{
		if (!EndingSleep)
		{
			Sleep(3000);
			EndingSleep = TRUE;
		}
		ScoreInputUI(hWnd);
	}
	else
	{
		GameOver(Mario, &nTotalBgX, &GAME_CLEAR, &nCharacterLife, &Player_Die);
		// 배경
		{
			__PutImage(g_sfBack.dcSurface, nBgGroundX, 0, g_sfBG.nWidth, g_sfBG.nHeight, 0, 0, &g_sfBG);

			if (0 < nBgGroundX)
				__PutImage(g_sfBack.dcSurface, nBgGroundX - SCREEN_X, 0, g_sfBG.nWidth, g_sfBG.nHeight, 0, 0, &g_sfBG);
			else
				__PutImage(g_sfBack.dcSurface, nBgGroundX + SCREEN_X, 0, g_sfBG.nWidth, g_sfBG.nHeight, 0, 0, &g_sfBG);

			FireworksAnimation();

			BackGroundPaint();
			__GroundPaint();
			__MapPaint();
		}
		CoinSet();

		// 캐릭터
		__PutSprite(g_sfBack.dcSurface, Mario->GetCharacterMoveX(), Mario->GetCharacterMoveY(),
			PLAYER_X_LEN, PLAYER_Y_LEN, Mario->GetStandX(), Mario->GetStandY(), &g_sfCaracter);
		// 적
		{
			CreateEnemy(Goomba[0], MONSTER_GOOMBA, 304, MONSTER_GOOMBA_Y, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[0]);
			CreateEnemy(Goomba[1], MONSTER_GOOMBA, 448, 176 - MONSTER_GOOMBA_Y_LEN + 2, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[1]);
			CreateEnemy(Goomba[2], MONSTER_GOOMBA, 656, MONSTER_GOOMBA_Y, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[2]);
			CreateEnemy(Goomba[3], MONSTER_GOOMBA, 816, 240 - MONSTER_GOOMBA_Y_LEN + 2, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[3]);
			CreateEnemy(Goomba[4], MONSTER_GOOMBA, 896, 192 - MONSTER_GOOMBA_Y_LEN + 2, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[4]);
			CreateEnemy(Goomba[5], MONSTER_GOOMBA, 1152, 224 - MONSTER_GOOMBA_Y_LEN + 2, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[5]);
			CreateEnemy(Goomba[6], MONSTER_GOOMBA, 1328, 144 - MONSTER_GOOMBA_Y_LEN + 2, MONSTER_GOOMBA_X_LEN, MONSTER_GOOMBA_Y_LEN, &MonsterLifeCode[6]);
		}

		{
			if (TimeOver || Player_Die)
			{
				__BigPutSprite(g_sfBack.dcSurface, 200, 100, 9, 9, 167, 17, 50, 50, &g_sWrite);	// G
				__BigPutSprite(g_sfBack.dcSurface, 250, 100, 9, 9, 227, 7, 50, 50, &g_sWrite);	// A
				__BigPutSprite(g_sfBack.dcSurface, 300, 100, 9, 9, 227, 17, 50, 50, &g_sWrite);	// M
				__BigPutSprite(g_sfBack.dcSurface, 350, 100, 9, 9, 147, 17, 50, 50, &g_sWrite);	// E

				__BigPutSprite(g_sfBack.dcSurface, 200, 150, 9, 9, 127, 27, 50, 50, &g_sWrite);	// O
				__BigPutSprite(g_sfBack.dcSurface, 250, 150, 9, 9, 187, 27, 50, 50, &g_sWrite);	// V
				__BigPutSprite(g_sfBack.dcSurface, 300, 150, 9, 9, 147, 17, 50, 50, &g_sWrite);	// E
				__BigPutSprite(g_sfBack.dcSurface, 350, 150, 9, 9, 157, 27, 50, 50, &g_sWrite);	// R
			}

			char  strBuff[24], strBuff2[24], strBuff3[24], strBuff4[24], strBuff5[24], strBuff6[24], strBuff7[40], strBuff8[40], strBuff9[24], strBuff10[20], strBuff11[20], strBuff12[20], strBuff13[20];

			wsprintf(strBuff, "STAGE: %d", nMapCount);
			wsprintf(strBuff2, "nBgX: %d", nBgX);
			wsprintf(strBuff3, "test: %d", nTotalBgX);
			TextOut(g_sfBack.dcSurface, 10, 10, strBuff, strlen(strBuff));
			TextOut(g_sfBack.dcSurface, 10, 30, strBuff2, strlen(strBuff2));
			TextOut(g_sfBack.dcSurface, 10, 50, strBuff3, strlen(strBuff3));

			wsprintf(strBuff4, "TIME: %d", nTime);
			wsprintf(strBuff5, "SCORE: %d", nScore);
			wsprintf(strBuff6, "COIN: %d", nCoin);
			TextOut(g_sfBack.dcSurface, 300, 10, strBuff4, strlen(strBuff4));
			TextOut(g_sfBack.dcSurface, 500, 10, strBuff5, strlen(strBuff5));
			TextOut(g_sfBack.dcSurface, 200, 10, strBuff6, strlen(strBuff6));

			wsprintf(strBuff9, "LIFE: %d", nCharacterLife);
			TextOut(g_sfBack.dcSurface, 10, 70, strBuff9, strlen(strBuff9));

			sprintf(strBuff10, "FPS: %f", fps);
			TextOut(g_sfBack.dcSurface, 300, 30, strBuff10, strlen(strBuff10));

			sprintf(strBuff11, "DEL: %f", timeDelta);
			TextOut(g_sfBack.dcSurface, 300, 50, strBuff11, strlen(strBuff11));

			sprintf(strBuff12, "Speed: %f", speed);
			TextOut(g_sfBack.dcSurface, 300, 70, strBuff12, strlen(strBuff12));

			sprintf(strBuff13, "GOOMSpeed: %f", GoombaSpeed);
			TextOut(g_sfBack.dcSurface, 300, 90, strBuff13, strlen(strBuff13));
		}

		dcScreen = GetDC(hWnd);
		__CompleteBlt(dcScreen, &g_sfBack);
		ReleaseDC(hWnd, dcScreen);
	}
}

int TileMoveCount = 0;
void __GroundPaint()
{
	if (nBgX == 0)
	{	// 타일 위치 초기화
		TileMoveCount = 0;
		for (int i = 0; i < TILE_MAX; i++)
			tile[i] = -16 + (16 * i);
	}

	for (int i = 0; i < TILE_MAX; i++)
	{
		if (tile[i] + nBgX < -16 && (KeyBuffer[MOVE_RIGHT] || KeyBuffer[MOVE_RIGHT_D]))
		{	// 오른쪽으로 향함
			TileMoveCount = 1 + i;	// 몇 개를 오른쪽으로 옮길 것인가.
			for (int i = 0; i < TileMoveCount; i++)
				tile[i] = SCREEN_X + (16 * i);
		}
		else if (tile[TILE_MAX - 1 - i] + nBgX > SCREEN_X && (KeyBuffer[MOVE_LEFT] || KeyBuffer[MOVE_LEFT_A]))
		{	// 왼쪽 이동
			TileMoveCount = 1 + i;
			for (int i = 0; i < TileMoveCount; i++)
				tile[TILE_MAX - 1 - i] = (-32) - (16 * i);
		}

		if (nBgX < 0 && Mario->GetCharacterMoveX() <= LEFT_LIMIT)
		{	// 오른쪽으로 가던 중 왼쪽으로 방향을 틈.
			if (tile[TileMoveCount - 1] + nBgX > SCREEN_X)
			{	// 아직 옮겨지지 않은, 바로 앞 배열의 뒤로 이동
				tile[TileMoveCount - 1] = tile[TileMoveCount] - 16;
			}
			if (tile[i] >= SCREEN_X)
				TileMoveCount = i + 1;
		}
		else if (0 < nBgX && RIGHT_LIMIT <= Mario->GetCharacterMoveX())
		{	// 왼쪽으로 가던 중 오른쪽으로 이동
			if (tile[TILE_MAX - TileMoveCount] + nBgX < -16)
			{
				tile[TILE_MAX - TileMoveCount] = tile[TILE_MAX - TileMoveCount - 1] + 16;
			}
			if (tile[i] < -16 && tile[i - 1] > 0)
				TileMoveCount = TILE_MAX - i;
		}
	}

	// 타일 이미지
	for (int i = 0; i < TILE_MAX; i++)
	{
		__PutImage(g_sfBack.dcSurface, tile[i] + nBgX, TILE_Y, TILE_X_LEN, TILE_Y_LEN, 18, 1, &g_sfBG_Ground);
		for (int Tile_Y_MAX = 1; Tile_Y_MAX <= 9; Tile_Y_MAX++)
			__PutImage(g_sfBack.dcSurface, tile[i] + nBgX, TILE_Y + (TILE_Y_LEN * Tile_Y_MAX), TILE_X_LEN, TILE_Y_LEN, 18, 18, &g_sfBG_Ground);
	}
	//// 지도의 처음과 끝 부분
	if (nMapCount == 1 && nBgX == 0)
	{
		__PutImage(g_sfBack.dcSurface, 0 + nBgX, TILE_Y, TILE_X_LEN, TILE_Y_LEN, 1, 1, &g_sfBG_Ground);
		for (int Tile_Y_MAX = 1; Tile_Y_MAX <= 9; Tile_Y_MAX++)
			__PutImage(g_sfBack.dcSurface, 0 + nBgX, TILE_Y + (TILE_Y_LEN * Tile_Y_MAX), TILE_X_LEN, TILE_Y_LEN, 1, 18, &g_sfBG_Ground);
	}
	else if (nMapCount == MAPLIMIT && nBgX <= 0)
	{
		__PutImage(g_sfBack.dcSurface, SCREEN_X - 32, TILE_Y, TILE_X_LEN, TILE_Y_LEN, 35, 1, &g_sfBG_Ground);
		for (int Tile_Y_MAX = 1; Tile_Y_MAX <= 9; Tile_Y_MAX++)
			__PutImage(g_sfBack.dcSurface, SCREEN_X - 32, TILE_Y + (TILE_Y_LEN * Tile_Y_MAX), TILE_X_LEN, TILE_Y_LEN, 35, 18, &g_sfBG_Ground);
	}
}

void GroupMapPaint(int nX1, int nY, int nBlockCount)
{
	// 허공 그라운드
	int x2;
	for (int i = 0; i < nBlockCount; i++)
	{	// X: 304 ~ 352
		x2 = nX1 + (16 * i);
		__PutImage(g_sfBack.dcSurface, nX1 + (16 * i) + nTotalBgX, nY, 16, 16, 18, 1, &g_sfBG_Ground);
	}

	if (nX1 + nTotalBgX < Mario->GetCharacterMoveX() + PLAYER_X_LEN)
		Mario->SetObstacleXY(nX1 + nTotalBgX, x2 + nTotalBgX, nY);
}
void __MapPaint()
{
	GroupMapPaint(320, 272, 4);
	GroupMapPaint(400, 176, 4);

	GroupMapPaint(624, 240, 3);
	GroupMapPaint(704, 240, 3);
	GroupMapPaint(784, 240, 3);

	GroupMapPaint(848, 192, 4);
	
	GroupMapPaint(1120, 224, 3);
	GroupMapPaint(1184, 176, 4);
	
	GroupMapPaint(1280, 144, 4);
}
void BackGroundPaint()
{
	// 성 위 & 아래
	__PutSprite(g_sfBack.dcSurface, 1763 + nTotalBgX, 160, 101, 84, 1, 7, &g_sfBG_Object);
	__PutSprite(g_sfBack.dcSurface, 1728 + nTotalBgX, 224, 169, 101, 1, 92, &g_sfBG_Object);

	__PutSprite(g_sfBack.dcSurface, 1608 + nTotalBgX, 160, OBJECT_FLAG_X_LEN, OBJECT_FLAG_Y_LEN, flagMove, 58, &g_sfBG_Object);	// 120 137 154
	__PutSprite(g_sfBack.dcSurface, GAME_OVER_LINE + nTotalBgX, 144, OBJECT_FLAG_X_LEN, OBJECT_FLAG_Y_LEN, 103, 58, &g_sfBG_Object);
	for (int i = 0; i < 10; i++)
	{	// 깃발 봉
		__PutSprite(g_sfBack.dcSurface, GAME_OVER_LINE + nTotalBgX, 160 + (i * 16), OBJECT_FLAG_X_LEN, OBJECT_FLAG_Y_LEN, 103, 75, &g_sfBG_Object);
	}
}

void FireworksAnimation()
{
	if (bEnding)
	{
		__PutSprite(g_sfBack.dcSurface, fireBall_POS_X[fireBallMoveX] + nTotalBgX, fireBall_POS_Y, 33, 33, fireBall_Stand_X[0], FIREWORK_STAND_Y, &g_sfBG_Object);
		if (bRePlayFireBall[0])
			__PutSprite(g_sfBack.dcSurface, 1776 + nTotalBgX, 80, 33, 33, fireBall_Stand_X[1], FIREWORK_STAND_Y, &g_sfBG_Object);
		if (bRePlayFireBall[1])
			__PutSprite(g_sfBack.dcSurface, 1824 + nTotalBgX, 96, 33, 33, fireBall_Stand_X[2], FIREWORK_STAND_Y, &g_sfBG_Object);
	}
}

void ScoreInputUI(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH NewBrush, OldBrush;

	NewBrush = CreateSolidBrush(RGB(0, 0, 0));
	OldBrush = (HBRUSH)SelectObject(g_sfBack.dcSurface, NewBrush);

	Rectangle(g_sfBack.dcSurface, 0, 0, SCREEN_X, SCREEN_Y);

	SelectObject(g_sfBack.dcSurface, OldBrush);
	DeleteObject(NewBrush);

	char chScore[24], chNameBuff[10];
	wsprintf(chScore, "SCORE: %d", nScore);
	wsprintf(chNameBuff, "NAME:");

	SetBkMode(g_sfBack.dcSurface, 1);
	SetTextColor(g_sfBack.dcSurface, RGB(255, 255, 255));
	TextOut(g_sfBack.dcSurface, 290, 200, chScore, strlen(chScore));
	TextOut(g_sfBack.dcSurface, 290, 220, chNameBuff, strlen(chNameBuff));
	TextOut(g_sfBack.dcSurface, 340, 220, string, nLen);

	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));
	RECT rt = { 290, 260, 350, 310 };
	Rectangle(g_sfBack.dcSurface, 290, 250, 350, 290);
	DrawText(g_sfBack.dcSurface, "확인", -1, &rt, DT_CENTER);

	__CompleteBlt(hdc, &g_sfBack);
	ReleaseDC(hWnd, hdc);
}

void ScoreList(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	HBRUSH NewBrush, OldBrush;

	NewBrush = CreateSolidBrush(RGB(0, 0, 0));
	OldBrush = (HBRUSH)SelectObject(g_sfBack.dcSurface, NewBrush);

	Rectangle(g_sfBack.dcSurface, 0, 0, SCREEN_X, SCREEN_Y);

	SelectObject(g_sfBack.dcSurface, OldBrush);
	DeleteObject(NewBrush);

	char chScoreList[10], chNameBuffList[10];
	wsprintf(chScoreList, "SCORE");
	wsprintf(chNameBuffList, "NAME");

	SetBkMode(g_sfBack.dcSurface, 1);
	SetTextColor(g_sfBack.dcSurface, RGB(255, 255, 255));
	TextOut(g_sfBack.dcSurface, 200, 100, chNameBuffList, strlen(chNameBuffList));
	TextOut(g_sfBack.dcSurface, 380, 100, chScoreList, strlen(chScoreList));

	char chRank[10];
	char chScore[10][10];

	for (int i = 0; i < 10; i++)
	{
		// 등수
		wsprintf(chRank, "%d", i + 1);
		TextOut(g_sfBack.dcSurface, 170, 130 + (i * 20), chRank, strlen(chRank));

		// 점수
		wsprintf(chScore[i], "%d", nScoreList[i]);
		TextOut(g_sfBack.dcSurface, 380, 130 + (i * 20), chScore[i], strlen(chScore[i]));

		// 이름
		TextOut(g_sfBack.dcSurface, 200, 130 + (i * 20), chName[i], strlen(chName[i]));
	}

	__CompleteBlt(hdc, &g_sfBack);
	ReleaseDC(hWnd, hdc);
}
