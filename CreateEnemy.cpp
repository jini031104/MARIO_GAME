#include "CreateEnemy.h"
// ���� ������ ��ġ�� �����ϴ� ��

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
	{	// ���� ���
		(*nScore) += 10;
		monster = NULL;
		delete(monster);
	}
	else if (monster->CrashReturn() && monster->DieCheck() == FALSE && 0 < (*nCharacterLife))
	{
		(*nCharacterLife)--;
		OutputDebugString("�浹! ���ӿ���!\n");
	}
}