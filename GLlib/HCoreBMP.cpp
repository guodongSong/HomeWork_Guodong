
#include <iostream>
#include <fstream>
#include <memory.h>
 
#include "HCoreBMP.h"

BMPImg::BMPImg(void)
{ 
	pImage=pPalette=pData=NULL;
	iWidth=iHeight=iBPP=iPlanes=iEnc=0;
}
 
BMPImg::~BMPImg()
{
	if(pImage)
	{
		delete [] pImage;
		pImage=NULL;
	}
 
	if(pPalette)
	{
		delete [] pPalette;
		pPalette=NULL;
	}
 
	if(pData)
	{
		delete [] pData;
		pData=NULL;
	}
}
 
int BMPImg::Load(const char* szFilename)
{
	int iRet;
 
	if(pImage)
    {
		delete [] pImage;
		pImage=NULL;
    }
 
	if(pPalette)
    {
		delete [] pPalette;
		pPalette=NULL;
    }
 

	iRet=GetFile(szFilename);
 
	if(iRet!=IMG_OK)
		return iRet;
 

	iRet=ReadBmpHeader();
 
	if(iRet!=IMG_OK)
		return iRet;
 
	if(iBPP<8) 
		return IMG_ERR_UNSUPPORTED;
 
	switch(iEnc)
    {
    case 0: 
		iRet=LoadBmpRaw(); 
		break;
 
    case 1: 
		iRet=LoadBmpRLE8();
		break;
 
    case 2: 
		return IMG_ERR_UNSUPPORTED;
 
    case 3: 
		return IMG_ERR_UNSUPPORTED;
 
    default:
		return IMG_ERR_UNSUPPORTED;
    }
 
	if(iRet!=IMG_OK)
		return iRet;
 
	FlipImg();
 
	iRet=LoadBmpPalette();
 
	if(iRet!=IMG_OK)
		return iRet;
 
	delete [] pData;
	pData=NULL;
 
	return IMG_OK;
}
 
int BMPImg::GetFile(const char* szFilename)
{
	using namespace std;
	ifstream fIn;
	unsigned long ulSize;
 
	fIn.open(szFilename,ios::binary);
    
	if(fIn==NULL)
		return IMG_ERR_NO_FILE;
 
	fIn.seekg(0,ios_base::end);
	ulSize=fIn.tellg();
	fIn.seekg(0,ios_base::beg);
 
	if(pData)
    {
		delete [] pData; 
		pData=NULL;
    }
 
	pData=new unsigned char[ulSize];
 
	if(pData==NULL)
    {
		fIn.close();
		return IMG_ERR_MEM_FAIL;
    }
 
	fIn.read((char*)pData,ulSize);
 
	fIn.close();
 
	return IMG_OK;
}
 
int BMPImg::ReadBmpHeader(void)
{
	int iInfo;
	
	if(pData==NULL)
		return IMG_ERR_NO_FILE;
 
	if(pData[0x0]!='B' || pData[0x1]!='M')
	    return IMG_ERR_BAD_FORMAT;
 
	memcpy(&iImgOffset,&pData[0xA],4); 
   
	memcpy(&iInfo,&pData[0xE],4); 
   
	if(iInfo!=0x28)
		return IMG_ERR_BAD_FORMAT;
 
	memcpy(&iWidth,&pData[0x12],4);  
	memcpy(&iHeight,&pData[0x16],4); 
	memcpy(&iPlanes,&pData[0x1A],2); 
	memcpy(&iBPP,&pData[0x1C],2);    
	memcpy(&iEnc,&pData[0x1E],4);    
 
	iDataSize=(iWidth*iHeight*(iBPP/8));
 
	return IMG_OK;
}
 
int BMPImg::LoadBmpRaw(void)
{
	if(pImage)
    {
		delete [] pImage;
		pImage=NULL;
    }
 
	pImage=new unsigned char[iDataSize];
 
	if(pImage==NULL)
		return IMG_ERR_MEM_FAIL;
 
	memcpy(pImage,&pData[iImgOffset],iDataSize);
 
	return IMG_OK;
}
 
int BMPImg::LoadBmpRLE8(void)
{
	unsigned char bOpCode,bVal;
	unsigned char *pSrc;
	int iDcode=1,iCount,iPos,iIndex;
 
	if(pImage)
		delete [] pImage;
 
	pImage=new unsigned char[iDataSize];
 
	if(pImage==NULL)
		return IMG_ERR_MEM_FAIL;
 
	pSrc=&pData[iImgOffset];
 
	iPos=0;
	iIndex=0;
 
	while(iDcode)
    {
		while(iPos%2)
		{
	        iPos++;
		}
 
		bOpCode=pSrc[iPos];
		bVal=pSrc[iPos+1];
		iPos+=2;
 
		if(bOpCode>0) 
		{
			for(iCount=0;iCount!=bOpCode;iCount++)
			{
				pImage[iIndex]=bVal;
				++iIndex;
			}
		}
		else 
		{
			switch(bVal)
			{
			case 0: 
				break;
 
			case 1:  
				iDcode=0;
				break;
 
			case 2:  
				break;
 
			default: 
				for(iCount=bVal;iCount!=0;iCount--)
				{
					pImage[iIndex]=pSrc[iPos];
					++iIndex;
					++iPos;
				}
				break;
			}
		}
		
		if(iIndex>iDataSize) 
			iDcode=0;
	}
  
	return IMG_OK;
}
 
int BMPImg::LoadBmpPalette(void)
{
	int iIndex;
	unsigned char *pPalPos, *pDatPos;
  
	if(pPalette)
	{
		delete [] pPalette;
		pPalette=NULL;
    }
 
	if(iBPP>8) 
		return IMG_OK;
 
	pPalette=new unsigned char[768];
 
	if(pPalette==NULL)
		return IMG_ERR_MEM_FAIL;
 
	pPalPos=pPalette;
	pDatPos=&pData[0x36];
 
	for(iIndex=0;iIndex!=256;++iIndex)
    {
		pPalPos[0]=pDatPos[2]; // Red
		pPalPos[1]=pDatPos[1]; // Green
		pPalPos[2]=pDatPos[0]; // Blue

		pPalPos+=3;
		pDatPos+=4;
    }
 
	return IMG_OK;
}
 
void BMPImg::FlipImg(void)
{
	unsigned char bTemp;
	unsigned char *pLine1, *pLine2;
	int iLineLen,iIndex;

	iLineLen=iWidth*(iBPP/8);
	pLine1=pImage;
	pLine2=&pImage[iLineLen * (iHeight - 1)];

	for( ;pLine1<pLine2;pLine2-=(iLineLen*2))
	{
		for(iIndex=0;iIndex!=iLineLen;pLine1++,pLine2++,iIndex++)
		{
			bTemp=*pLine1;
			*pLine1=*pLine2;
			*pLine2=bTemp;       
		}
	} 
}
 
int BMPImg::GetBPP(void)
{
	return iBPP;
}
 
int BMPImg::GetWidth(void)
{
	return iWidth;
}
 
int BMPImg::GetHeight(void)
{
	return iHeight;
}
 
unsigned char* BMPImg::GetImg(void)
{
	return pImage;
}
 
unsigned char* BMPImg::GetPalette(void)
{
	return pPalette;
}

unsigned char *BMPImg::GetPixel(int x, int y)
{
	if ( x < 0 ) x = 0;
	if ( x >= iWidth ) x = iWidth-1;
	if ( y < 0 ) y = 0;
	if ( y >= iHeight ) y = iHeight-1;

	int bytes_per_pixel = iBPP / 8;
	int loc = (y * iWidth + x) * bytes_per_pixel;

	return pImage + loc;
}

bool BMPImg::DownSampling()
{
	if ( iWidth==1 && iHeight==1 )
		return false; 

	int bytes_per_pixel = iBPP/8;
	int newWidth = iWidth / 2;
	int newHeight = iHeight / 2;

	if ( newWidth < 1 ) newWidth = 1;
	if ( newHeight < 1 ) newHeight = 1;

	unsigned char *pNewImage = new unsigned char[newWidth*newHeight*bytes_per_pixel];
	unsigned char *pDest = pNewImage;

	for (int y=0; y<newHeight; y++ )
	{
		for ( int x=0; x<newWidth; x++ )
		{
			unsigned char *p0 = GetPixel(x*2, y*2);
			unsigned char *p1 = GetPixel(x*2+1, y*2  );
			unsigned char *p2 = GetPixel(x*2  , y*2+1);
			unsigned char *p3 = GetPixel(x*2+1, y*2+1);

			for ( int i=0; i<bytes_per_pixel; i++ )
			{
				pDest[i] = (p0[i] + p1[i] + p2[i] + p3[i])/4;
			}
			
			pDest += bytes_per_pixel;
		}
	}

	delete [] pImage;

	pImage = pNewImage;
	iWidth = newWidth;
	iHeight = newHeight;

	return true;
}