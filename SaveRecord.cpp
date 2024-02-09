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
		
		player[0].nScore = nScore;					// ���� ����
		strcpy(player[0].chPlayerName, chName);		// �̸� ����
		for (int i = 1; i < 10; i++)
		{
			player[i].nScore = 0;
		}

		//nScoreList[0] = nScore;
		nList[0] = nScore;
		strcpy(nameList[0], chName);
		
		fwrite(player, sizeof(PLAYER_INFO), 10, fp);	// �ʱ� ĳ���� ���� ����
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
				open_info[i].nScore = minScore;				// ���� ����
				nList[i] = open_info[i].nScore;				// ���� ���

				strcpy(open_info[i].chPlayerName, minName);		// �̸� ����
				strcpy(nameList[i], open_info[i].chPlayerName);
				break;
			}
			else if (open_info[i].nScore < minScore)	// �̹� ����� �������� ���� ���� ȹ���� ���
			{
				int beforeScore = open_info[i].nScore;
				open_info[i].nScore = minScore;
				minScore = beforeScore;


				char beforeName[23];
				strcpy(beforeName, open_info[i].chPlayerName);		// ���� �̸� �̵�
				strcpy(open_info[i].chPlayerName, minName);			// ���� �̸� ����
				strcpy(minName, beforeName);			// ���� �̸� ����

			}
			nList[i] = open_info[i].nScore;
			strcpy(nameList[i], open_info[i].chPlayerName);
		}

		fp = fopen("Save_Score.bin", "wb");
		fwrite(open_info, sizeof(PLAYER_INFO), 10, fp);	// �ʱ� ĳ���� ���� ����
		fclose(fp);
	}
}