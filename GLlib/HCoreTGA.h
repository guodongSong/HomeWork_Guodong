
#define IMG_OK              0x1
#define IMG_ERR_NO_FILE     0x2
#define IMG_ERR_MEM_FAIL    0x4
#define IMG_ERR_BAD_FORMAT  0x8
#define IMG_ERR_UNSUPPORTED 0x40

#include "HCoreImage.h"

class TGAImg : public HCoreImage
{
public:
	TGAImg();
	~TGAImg();
	int Load(const char* szFilename);
	int GetBPP();
	int GetWidth();
	int GetHeight();
	unsigned char* GetImg(void);      
	unsigned char* GetPalette(void); 
	void FlipImg();
	bool DownSampling(void);

private:
	short int iWidth,iHeight,iBPP;
	unsigned long lImageSize;
	char bEnc;
	unsigned char *pImage, *pPalette, *pData;

	int ReadHeader();
	int LoadRawData();
	int LoadTgaRLEData();
	int LoadTgaPalette();
	void BGRtoRGB();
	unsigned char *GetPixel(int x, int y);
};
