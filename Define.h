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
#define MAPLIMIT			3		// 최대 맵 생성 개수

#define TILE_MAX			41
#define TILE_Y				320		// 타일의 기본 Y 좌표

#define TILE_X_LEN			16		// 타일의 기본 X 길이
#define TILE_Y_LEN			16		// 타일의 기본 Y 길이
									
#define LEFT_LIMIT			140		
#define RIGHT_LIMIT			500		

#define GRAVITY				9.8f	// 중력
									
#define MOVE_JUMP			32		// 점프
#define MOVE_LEFT			37		// 왼쪽
#define MOVE_LEFT_A			65		//
#define MOVE_RIGHT			39		// 오른쪽
#define MOVE_RIGHT_D		68		//
#define MOVE_DOWN			40		// 아래
#define MOVE_DOWN_S			83		//

#define FIREWORK_STAND_Y	7		// 불꽃놀이 Y 축

#define SLOW_SPEED			1.0f
#define FAST_SPEED			2.0f
									
#define SHORT_JUMP			30.0f
#define LONG_JUMP			35.0f
#define GOOMBA_JUMP			15.0f
									
#define MONSTER_GOOMBA		1000	// 굼바 생성 코드

#define MONSTER_GOOMBA_MAX	7		// 굼바 최대 생성 개수
#define OBJECT_COIN_MAX		23		// 코인 최대 생성 개수

// 320 을 기준으로...
#define PLAYER_Y					273.0f	// 플레이어 생성 Y좌표
#define MONSTER_GOOMBA_Y			298.0f	// 굼바 생성 Y좌표
									
#define PLAYER_X_LEN				34		// 플레이어 X 사이즈
#define PLAYER_Y_LEN				49		// 플레이어 Y 사이즈
#define MONSTER_GOOMBA_X_LEN		24		// 굼바 X 사이즈
#define MONSTER_GOOMBA_Y_LEN		24		// 굼바 X 사이즈

#define OBJECT_COIN_X_LEN			15		// 코인 X 사이즈
#define OBJECT_COIN_Y_LEN			15		// 코인 Y 사이즈
#define OBJECT_FLAG_X_LEN			16		// 코인 Y 사이즈
#define OBJECT_FLAG_Y_LEN			16		// 코인 Y 사이즈

#define GAME_OVER_LINE				1616	// 게임 종료 라인