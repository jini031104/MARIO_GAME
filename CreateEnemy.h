#pragma once
#include "CGoomba.h"

void CreateEnemyPoint(CCharacter*& monster, int nMonsterType, int nSpawnX, int nSpawnY);

void EnemyOrPlayerDie(CCharacter*& monster, CCharacter*& character, int* nTotalBgX, int* nMonster_XSize, int* nMonster_YSize, int* nScore, int* nCharacterLife);