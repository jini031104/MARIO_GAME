#pragma once
#include "Define.h"
#include "CCharacter.h"

void __MapMove(WPARAM wParam, CCharacter*& character, int* nTotalBgX, int* nBgX, int* nBgGroundX, int* nMapCount, BOOL KeyBuffer[]);

void EnemyMove(WPARAM wParam, CCharacter*& Monster, CCharacter*& character, BOOL KeyBuffer[], int* nBgX, int* nTotalBgX, float speed, int nX1 = 0);

void FlagPosAndMove(WPARAM wParam, int* flagMove);

void FireworksUp(WPARAM wParam, int* fireBall_POS_Y, int* fireBallMoveX);
void FireworksBoom(WPARAM wParam, int* fireBall_POS_Y, int* fireBall_Stand_X, BOOL* bPlayFireworks);

void TimeScore(WPARAM wParam, int* nTime, int* nScore);
void TimeCount(WPARAM wParam, int* nTime, BOOL* TimeOver);

void GameOver(CCharacter*& character, int* nTotalBgX, BOOL* GAME_CLEAR, int* nCharacterLife, BOOL* Player_Die);

void EndingMove(WPARAM wParam, CCharacter*& character, BOOL* bSleep, BOOL* bEnding, BOOL* bStanding, BOOL KeyBuffer[],
			int* nTotalBgX, int* nBgX, int* nBgGroundX, int* nMapCount, float* speed);