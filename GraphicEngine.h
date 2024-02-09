#pragma once
#include <windows.h>

typedef struct SURFACEINFOtag
{
	int			nWidth;
	int			nHeight;
	int			nCentralX;		// �߽���ǥ X
	int			nCentralY;		// �߽���ǥ Y
	COLORREF	crColorKey;		// �÷�Ű
	DWORD		dwDelayTime;	// �����ð� (ms)

	HDC			dcSurface;		// ��Ʈ���� ������ִ� ǥ�� DC
	HBITMAP		hBmp;			// ��Ʈ�� �ڵ�
	HBITMAP		hOldBmp;		// ���� ��Ʈ�� �ڵ� ( dc ������ ���� )
} SURFACEINFO;

BOOL __CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, SURFACEINFO* psInfo);

void __SetImgSurface(SURFACEINFO* psfInfo);

void __ReleaseSurface(SURFACEINFO* psInfo);

void __SetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey);

BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo);

HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path);

void __LoadSurface(HDC dcScreen, SURFACEINFO* psInfo);

BOOL __PutImage(HDC dcDst, int x, int y, int Width, int Height, int xStart, int yStart, SURFACEINFO* psInfo);

BOOL __PutSprite(HDC dcDst, int ScreenX, int ScreenY, int nWeight, int nHight, int nStandX, int nStandY, SURFACEINFO* psInfo);

BOOL __BigPutSprite(HDC dcDst, int ScreenX, int ScreenY, int nWeight, int nHight, int nStandX, int nStandY, int nSizeX, int nSizeY, SURFACEINFO* psInfo);