#pragma once
#include <stdio.h>
#include <string.h>

typedef struct
{
	int	nScore;				// ����
	char chPlayerName[23];	// �̸�
} PLAYER_INFO;

void SaveRecord(int nScore, int nList[10], char chName[23], char nameList[10][23]);