#include "CreateEnemy.h"
// 적이 생성될 위치만 선정하는 것

void CreateEnemyPoint(CCharacter*& monster, int nMonsterType, int nSpawnX, int nSpawnY)
{
	if (nMonsterType == MONSTER_GOOMBA)
	{
		monster = new CGoomba;
	}

	monster->SetMonsterMoveX(nSpawnX);
	monster->SetMonsterMoveY(nSpawnY);
}

void EnemyOrPlayerDie(CCharacter*& monster, CCharacter*& character, int* nTotalBgX, int* nMonster_XSize, int* nMonster_YSize, int* nScore, int* nCharacterLife)
{
	monster->CrashCheck(character->GetCharacterMoveX(), character->GetCharacterMoveX() + PLAYER_X_LEN,
		character->GetCharacterMoveY(), character->GetCharacterMoveY() + PLAYER_Y_LEN,
		monster->GetCharacterMoveX() + (*nTotalBgX), monster->GetCharacterMoveX() + (*nMonster_XSize) + (*nTotalBgX),
		monster->GetCharacterMoveY(), monster->GetCharacterMoveY() + (*nMonster_YSize));

	if (monster->DieCheck() && monster->CrashReturn() == FALSE)
	{	// 몬스터 사망
		(*nScore) += 10;
		monster = NULL;
		delete(monster);
	}
	else if (monster->CrashReturn() && monster->DieCheck() == FALSE && 0 < (*nCharacterLife))
	{
		(*nCharacterLife)--;
		OutputDebugString("충돌! 게임오버!\n");
	}
}