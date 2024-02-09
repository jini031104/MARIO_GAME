#include <stdio.h>
#include "GraphicEngine.h"

//// dcScreen�� ȣȯ�Ǵ� �ĸ�ǥ��DC ����
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

//// SURFACEINFO �̹��� �ʱ�ȭ
void __SetImgSurface(SURFACEINFO* psfInfo)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
}

//// SURFACEINFO ��������Ʈ �ʱ�ȭ
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

//// dcScreen�� ǥ���� ���� ( �ĸ�ǥ�� -> ȭ��DC )
BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo)
{
	BitBlt(dcScreen, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
	return TRUE;
}

//// DIB -> DDB�� ��ȯ
HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path)
{
	HANDLE  hFile;
	DWORD   FileSize, dwRead;
	HBITMAP hBit;
	BITMAPFILEHEADER* fh;
	BITMAPINFO* ih;
	BITMAPINFOHEADER* iheader;
	LPBYTE			 lpMemBlock;

	//// ������ ����
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

	//// DDB�� ��ȯ�Ѵ�.
	hBit = CreateDIBitmap(hdc,						// hdc
		iheader,					// BITMAPINFOHEADER ���
		CBM_INIT,					// 0 �Ǵ� CBM_INIT ( �ʱ�ȭ )
		lpMemBlock + fh->bfOffBits,	// ������ ��巡��
		ih,							// BITMAPINFO ���
		DIB_RGB_COLORS);

	free(lpMemBlock);

	return hBit;
}

//// ǥ�� ���� �� �ε�
//// ���� ������Ʈ �Ǵ� ��Ʈ�� DC ���� �� ��Ʈ�� �ε�
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

//// �̹��� ���
BOOL __PutImage(HDC dcDst, int x, int y, int Width, int Height, int xStart, int yStart, SURFACEINFO* psInfo)
{
	return BitBlt(dcDst, x, y, Width, Height, psInfo->dcSurface, xStart, yStart, SRCCOPY);
}

//// ��������Ʈ
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