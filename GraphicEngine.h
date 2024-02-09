#pragma once
#include <windows.h>

typedef struct SURFACEINFOtag
{
	int			nWidth;
	int			nHeight;
	int			nCentralX;		// 중심좌표 X
	int			nCentralY;		// 중심좌표 Y
	COLORREF	crColorKey;		// 컬러키
	DWORD		dwDelayTime;	// 지연시간 (ms)

	HDC			dcSurface;		// 비트맵이 적재되있는 표면 DC
	HBITMAP		hBmp;			// 비트맵 핸들
	HBITMAP		hOldBmp;		// 이전 비트맵 핸들 ( dc 해제시 참조 )
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