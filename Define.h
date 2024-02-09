#pragma once
#include <windows.h>

#define ID_TM_MAINLOOP		1
#define ID_TM_MOVE			2
#define ID_TM_JUMPTIME		3
#define ID_TM_GAMETIME		4
#define ID_TM_ANIMATION		5
#define ID_TM_FIREBALL		6

#define GAME_LIMIT_TIME		30

#define SCREEN_X			640
#define SCREEN_Y			480
#define MAPLIMIT			3		// �ִ� �� ���� ����

#define TILE_MAX			41
#define TILE_Y				320		// Ÿ���� �⺻ Y ��ǥ

#define TILE_X_LEN			16		// Ÿ���� �⺻ X ����
#define TILE_Y_LEN			16		// Ÿ���� �⺻ Y ����
									
#define LEFT_LIMIT			140		
#define RIGHT_LIMIT			500		

#define GRAVITY				9.8f	// �߷�
									
#define MOVE_JUMP			32		// ����
#define MOVE_LEFT			37		// ����
#define MOVE_LEFT_A			65		//
#define MOVE_RIGHT			39		// ������
#define MOVE_RIGHT_D		68		//
#define MOVE_DOWN			40		// �Ʒ�
#define MOVE_DOWN_S			83		//

#define FIREWORK_STAND_Y	7		// �Ҳɳ��� Y ��

#define SLOW_SPEED			1.0f
#define FAST_SPEED			2.0f
									
#define SHORT_JUMP			30.0f
#define LONG_JUMP			35.0f
#define GOOMBA_JUMP			15.0f
									
#define MONSTER_GOOMBA		1000	// ���� ���� �ڵ�

#define MONSTER_GOOMBA_MAX	7		// ���� �ִ� ���� ����
#define OBJECT_COIN_MAX		23		// ���� �ִ� ���� ����

// 320 �� ��������...
#define PLAYER_Y					273.0f	// �÷��̾� ���� Y��ǥ
#define MONSTER_GOOMBA_Y			298.0f	// ���� ���� Y��ǥ
									
#define PLAYER_X_LEN				34		// �÷��̾� X ������
#define PLAYER_Y_LEN				49		// �÷��̾� Y ������
#define MONSTER_GOOMBA_X_LEN		24		// ���� X ������
#define MONSTER_GOOMBA_Y_LEN		24		// ���� X ������

#define OBJECT_COIN_X_LEN			15		// ���� X ������
#define OBJECT_COIN_Y_LEN			15		// ���� Y ������
#define OBJECT_FLAG_X_LEN			16		// ���� Y ������
#define OBJECT_FLAG_Y_LEN			16		// ���� Y ������

#define GAME_OVER_LINE				1616	// ���� ���� ����