
#include <iostream>
#include <fstream>
#include <memory.h>

#include "HCore.h"
#include "HCoreTGA.h"

TGAImg::TGAImg()
{ 
	pImage=pPalette=pData=NULL;
	iWidth=iHeight=iBPP=bEnc=0;
	lImageSize=0;
}


TGAImg::~TGAImg()
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


int TGAImg::Load(const char* szFilename)
{
	using namespace std;
	ifstream fIn;
	unsigned long ulSize;
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

	fIn.open(szFilename,ios::binary);

	if(fIn==NULL)
		return IMG_ERR_NO_FILE;

	fIn.seekg(0,ios_base::end);
	ulSize=fIn.tellg();
	fIn.seekg(0,ios_base::beg);


	if(pData)
		delete [] pData; 

	pData=new unsigned char[ulSize];

	if(pData==NULL)
	{
		fIn.close();
		return IMG_ERR_MEM_FAIL;
	}

	fIn.read((char*)pData,ulSize);

	fIn.close();

	iRet=ReadHeader();

	if(iRet!=IMG_OK)
		return iRet;

	switch(bEnc)
	{
	case 1: 
		{
			if((lImageSize+18+pData[0]+768)>ulSize)
				return IMG_ERR_BAD_FORMAT;

			if(pData[1]!=1)
				return IMG_ERR_BAD_FORMAT;

			iRet=LoadRawData();
			if(iRet!=IMG_OK)
				return iRet;

			iRet=LoadTgaPalette();
			if(iRet!=IMG_OK)
				return iRet;

			break;
		}

	case 2: 
		{
			if((lImageSize+18+pData[0])>ulSize)
				return IMG_ERR_BAD_FORMAT;

			if(pData[1]!=0)
				return IMG_ERR_BAD_FORMAT;

			iRet=LoadRawData();
			if(iRet!=IMG_OK)
				return iRet;

			BGRtoRGB(); 
			break;
		}

	case 9: 
		{
			if(pData[1]!=1)
				return IMG_ERR_BAD_FORMAT;

			iRet=LoadTgaRLEData();
			if(iRet!=IMG_OK)
				return iRet;

			iRet=LoadTgaPalette();
			if(iRet!=IMG_OK)
				return iRet;

			break;
		}

	case 10: 
		{
			if(pData[1]!=0)
				return IMG_ERR_BAD_FORMAT;

			iRet=LoadTgaRLEData();
			if(iRet!=IMG_OK)
				return iRet;

			BGRtoRGB(); 
			break;
		}

	default:
		return IMG_ERR_UNSUPPORTED;
	}

	if((pData[17] & 0x20)==0) 
		FlipImg();

	delete [] pData;
	pData=NULL;

	return IMG_OK;
}


int TGAImg::ReadHeader() 
{
	short ColMapStart,ColMapLen;
	short x1,y1,x2,y2;

	if(pData==NULL)
		return IMG_ERR_NO_FILE;

	if(pData[1]>1)   
		return IMG_ERR_UNSUPPORTED;

	bEnc=pData[2];    

	if(bEnc>11)      
		return IMG_ERR_UNSUPPORTED;


	memcpy(&ColMapStart,&pData[3],2);
	memcpy(&ColMapLen,&pData[5],2);

	if(pData[1]==1)
	{
		if(ColMapStart!=0 || ColMapLen!=256 || pData[7]!=24)
			return IMG_ERR_UNSUPPORTED;
	}

	memcpy(&x1,&pData[8],2);
	memcpy(&y1,&pData[10],2);
	memcpy(&x2,&pData[12],2);
	memcpy(&y2,&pData[14],2);

	iWidth=(x2-x1);
	iHeight=(y2-y1);

	if(iWidth<1 || iHeight<1)
		return IMG_ERR_BAD_FORMAT;

	iBPP=pData[16];

	if(pData[17]>32) 
		return IMG_ERR_UNSUPPORTED;

	lImageSize=(iWidth * iHeight * (iBPP/8));

	return IMG_OK;
}


int TGAImg::LoadRawData() 
{
	short iOffset;

	if(pImage) 
		delete [] pImage;

	pImage=new unsigned char[lImageSize];

	if(pImage==NULL)
		return IMG_ERR_MEM_FAIL;

	iOffset=pData[0]+18; 

	if(pData[1]==1)
		iOffset+=768; 

	memcpy(pImage,&pData[iOffset],lImageSize);

	return IMG_OK;
}


int TGAImg::LoadTgaRLEData() 
{
	short iOffset,iPixelSize;
	unsigned char *pCur;
	unsigned long Index=0;
	unsigned char bLength,bLoop;

	iOffset=pData[0]+18;

	if(pData[1]==1)
		iOffset+=768; 

	iPixelSize=iBPP/8;

	pCur=&pData[iOffset];

	if(pImage!=NULL)
		delete [] pImage;

	pImage=new unsigned char[lImageSize];

	if(pImage==NULL)
		return IMG_ERR_MEM_FAIL;

	while(Index<lImageSize) 
	{
		if(*pCur & 0x80) 
		{
			bLength=*pCur-127; 
			pCur++;           

			for(bLoop=0;bLoop!=bLength;++bLoop,Index+=iPixelSize)
				memcpy(&pImage[Index],pCur,iPixelSize);

			pCur+=iPixelSize; 
		}
		else 
		{
			bLength=*pCur+1; 
			pCur++;          

			for(bLoop=0;bLoop!=bLength;++bLoop,Index+=iPixelSize,pCur+=iPixelSize)
				memcpy(&pImage[Index],pCur,iPixelSize);
		}
	}

	return IMG_OK;
}


int TGAImg::LoadTgaPalette() 
{
	unsigned char bTemp;
	short iIndex,iPalPtr;

	if(pPalette)
	{
		delete [] pPalette;
		pPalette=NULL;
	}

	pPalette=new unsigned char[768];

	if(pPalette==NULL)
		return IMG_ERR_MEM_FAIL;

	memcpy(pPalette,&pData[pData[0]+18],768);

	for(iIndex=0,iPalPtr=0;iIndex!=256;++iIndex,iPalPtr+=3)
	{
		bTemp=pPalette[iPalPtr];               
		pPalette[iPalPtr]=pPalette[iPalPtr+2]; 
		pPalette[iPalPtr+2]=bTemp;             
	}

	return IMG_OK;
}


void TGAImg::BGRtoRGB()
{
	unsigned long Index,nPixels;
	unsigned char *bCur;
	unsigned char bTemp;
	short iPixelSize;

	bCur=pImage;

	nPixels=iWidth*iHeight;

	iPixelSize=iBPP/8;

	for(Index=0;Index!=nPixels;Index++)  
	{
		bTemp=*bCur;      
		*bCur=*(bCur+2);  
		*(bCur+2)=bTemp;  

		bCur+=iPixelSize; 
	}

}


void TGAImg::FlipImg() 
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


int TGAImg::GetBPP() 
{
	return iBPP;
}


int TGAImg::GetWidth()
{
	return iWidth;
}


int TGAImg::GetHeight()
{
	return iHeight;
}


unsigned char* TGAImg::GetImg()
{
	return pImage;
}


unsigned char* TGAImg::GetPalette()
{
	return pPalette;
}

unsigned char *TGAImg::GetPixel(int x, int y)
{
	if ( x < 0 ) x = 0;
	if ( x >= iWidth ) x = iWidth-1;
	if ( y < 0 ) y = 0;
	if ( y >= iHeight ) y = iHeight-1;

	int bytes_per_pixel = iBPP / 8;
	int loc = (y * iWidth + x) * bytes_per_pixel;

	return pImage + loc;
}

bool TGAImg::DownSampling()
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