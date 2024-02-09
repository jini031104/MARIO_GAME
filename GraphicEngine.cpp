#include <stdio.h>
#include "GraphicEngine.h"

//// dcScreen와 호환되는 후면표면DC 생성
BOOL __CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, SURFACEINFO* psInfo)
{
	psInfo->dcSurface = CreateCompatibleDC(dcScreen);
	psInfo->hBmp = CreateCompatibleBitmap(dcScreen, nWidth, nHeight);
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp);
	PatBlt(psInfo->dcSurface, 0, 0, nWidth, nHeight, PATCOPY);
	psInfo->nWidth = nWidth;
	psInfo->nHeight = nHeight;

	return TRUE;
}

//// SURFACEINFO 이미지 초기화
void __SetImgSurface(SURFACEINFO* psfInfo)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
}

//// SURFACEINFO 스프라이트 초기화
void __SetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey)
{
	memset(psfInfo, 0, sizeof(SURFACEINFO));
	psfInfo->crColorKey = crColorKey;
}

void __ReleaseSurface(SURFACEINFO* psInfo)
{
	SelectObject(psInfo->dcSurface, psInfo->hOldBmp);
	DeleteDC(psInfo->dcSurface);
	DeleteObject(psInfo->hBmp);
}

//// dcScreen에 표면을 복사 ( 후면표면 -> 화면DC )
BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo)
{
	BitBlt(dcScreen, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
	return TRUE;
}

//// DIB -> DDB로 변환
HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path)
{
	HANDLE  hFile;
	DWORD   FileSize, dwRead;
	HBITMAP hBit;
	BITMAPFILEHEADER* fh;
	BITMAPINFO* ih;
	BITMAPINFOHEADER* iheader;
	LPBYTE			 lpMemBlock;

	//// 파일을 연다
	hFile = CreateFile(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize = GetFileSize(hFile, NULL);

	lpMemBlock = (LPBYTE)malloc(FileSize);
	fh = (BITMAPFILEHEADER*)lpMemBlock;
	ReadFile(hFile, lpMemBlock, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	ih = (BITMAPINFO*)(lpMemBlock + sizeof(BITMAPFILEHEADER));
	iheader = (BITMAPINFOHEADER*)(lpMemBlock + sizeof(BITMAPFILEHEADER));

	//// DDB로 변환한다.
	hBit = CreateDIBitmap(hdc,						// hdc
		iheader,					// BITMAPINFOHEADER 헤더
		CBM_INIT,					// 0 또는 CBM_INIT ( 초기화 )
		lpMemBlock + fh->bfOffBits,	// 래스터 어드래스
		ih,							// BITMAPINFO 헤더
		DIB_RGB_COLORS);

	free(lpMemBlock);

	return hBit;
}

//// 표면 생성 및 로드
//// 각종 오브젝트 또는 비트맵 DC 생성 및 비트맵 로드
void __LoadSurface(HDC dcScreen, SURFACEINFO* psInfo)
{
	BITMAP  bit;

	psInfo->dcSurface = CreateCompatibleDC(dcScreen);
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp);

	GetObject(psInfo->hBmp, sizeof(BITMAP), &bit);
	psInfo->nWidth = bit.bmWidth;
	psInfo->nHeight = bit.bmHeight;
}

//
//
//
//
//
//
//
//		
//
//
//
//
//
//
//

//// 이미지 출력
BOOL __PutImage(HDC dcDst, int x, int y, int Width, int Height, int xStart, int yStart, SURFACEINFO* psInfo)
{
	return BitBlt(dcDst, x, y, Width, Height, psInfo->dcSurface, xStart, yStart, SRCCOPY);
}

//// 스프라이트
BOOL __PutSprite(HDC dcDst, int ScreenX, int ScreenY, int nWeight, int nHight, int nStandX, int nStandY, SURFACEINFO* psInfo)
{
	TransparentBlt(dcDst,
		ScreenX, ScreenY, nWeight, nHight,
		psInfo->dcSurface,
		nStandX, nStandY, nWeight, nHight,
		psInfo->crColorKey);
	return 0;
}

BOOL __BigPutSprite(HDC dcDst, int ScreenX, int ScreenY, int nWeight, int nHight, int nStandX, int nStandY, int nSizeX, int nSizeY, SURFACEINFO* psInfo)
{
	TransparentBlt(dcDst,
		ScreenX, ScreenY, nSizeX, nSizeY,
		psInfo->dcSurface,
		nStandX, nStandY, nWeight, nHight,
		psInfo->crColorKey);
	return 0;
}