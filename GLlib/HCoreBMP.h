
#include <iostream>
#include <fstream>
#include <memory.h>
 
#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT  0x8
#define IMG_ERR_UNSUPPORTED 0x40
 
#include "HCoreImage.h"

class BMPImg : public HCoreImage
{
public:
	BMPImg();
	~BMPImg();
	int Load(const char* szFilename);
	int GetBPP();
	int GetWidth();
	int GetHeight();
	unsigned char* GetImg(void);       
	unsigned char* GetPalette(void);  
 	bool DownSampling(void);

private:
	unsigned int iWidth,iHeight,iEnc;
	short int iBPP,iPlanes;
	int iImgOffset,iDataSize;
	unsigned char *pImage, *pPalette, *pData;

	int GetFile(const char* szFilename);
	int ReadBmpHeader();
	int LoadBmpRaw();
	int LoadBmpRLE8();
	int LoadBmpPalette();
	void Downsampling(void);
	void FlipImg(); 
	unsigned char *GetPixel(int x, int y);
};
 