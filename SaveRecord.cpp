#include "SaveRecord.h"

void SaveRecord(int nScore, int nList[10], char chName[23], char nameList[10][23])
{
	FILE* fp, * op;
	PLAYER_INFO player[10];
	PLAYER_INFO open_info[10];

	int nScoreList[10] = { 0 };
	//char chName[10][23] = { {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"}, {"NULL"} };

	static int minScore = nScore;
	static char minName[23];
	strcpy(minName, chName);

	fp = fopen("Save_Score.bin", "rb");

	if (fp == NULL)
	{
		fp = fopen("Save_Score.bin", "wb");
		
		player[0].nScore = nScore;					// 점수 저장
		strcpy(player[0].chPlayerName, chName);		// 이름 저장
		for (int i = 1; i < 10; i++)
		{
			player[i].nScore = 0;
		}

		//nScoreList[0] = nScore;
		nList[0] = nScore;
		strcpy(nameList[0], chName);
		
		fwrite(player, sizeof(PLAYER_INFO), 10, fp);	// 초기 캐릭터 상태 저장
		fclose(fp);
	}
	else
	{
		fclose(fp);

		op = fopen("Save_Score.bin", "rb");
		fread(open_info, sizeof(PLAYER_INFO), 10, op);
		fclose(op);

		for (int i = 0; i < 10; i++)
		{
			if (open_info[i].nScore == 0)
			{
				open_info[i].nScore = minScore;				// 점수 저장
				nList[i] = open_info[i].nScore;				// 점수 출력

				strcpy(open_info[i].chPlayerName, minName);		// 이름 저장
				strcpy(nameList[i], open_info[i].chPlayerName);
				break;
			}
			else if (open_info[i].nScore < minScore)	// 이미 저장된 점수보다 높은 점수 획득한 경우
			{
				int beforeScore = open_info[i].nScore;
				open_info[i].nScore = minScore;
				minScore = beforeScore;


				char beforeName[23];
				strcpy(beforeName, open_info[i].chPlayerName);		// 이전 이름 이동
				strcpy(open_info[i].chPlayerName, minName);			// 현재 이름 저장
				strcpy(minName, beforeName);			// 현재 이름 저장

			}
			nList[i] = open_info[i].nScore;
			strcpy(nameList[i], open_info[i].chPlayerName);
		}

		fp = fopen("Save_Score.bin", "wb");
		fwrite(open_info, sizeof(PLAYER_INFO), 10, fp);	// 초기 캐릭터 상태 저장
		fclose(fp);
	}
}