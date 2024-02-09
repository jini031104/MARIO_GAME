#pragma once
#include <stdio.h>
#include <string.h>

typedef struct
{
	int	nScore;				// 점수
	char chPlayerName[23];	// 이름
} PLAYER_INFO;

void SaveRecord(int nScore, int nList[10], char chName[23], char nameList[10][23]);