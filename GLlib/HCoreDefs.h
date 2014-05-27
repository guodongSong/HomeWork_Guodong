#ifndef _HCore_DEFS_
#define _HCore_DEFS_

#define SAFE_RELEASE(x) if (x) { x->Release(); x=NULL; }
#define RELEASE_ARRAY(x) if ( x ) { delete [] x; x=NULL; }

#include "Vector4.h"

enum HCoreEnum
{
	HCore_UNKNOWN = 1,
	// device
	HCore_OPENGL,
	HCore_DX9,
	HCore_DX10,
};

struct Vertex_V3T2
{
	float m_Position[3]; 
	float m_Texcoord[2]; 
};

struct Vertex_V3
{
	float m_Position[3];
};

struct Vertex_V3N3
{
	float m_Position[3];
	float m_Normal[3];
};

struct Vertex_V3N3C
{
	float m_Position[3];
	float m_Normal[3];
	unsigned char m_Color[4];
};

struct Vertex_V4T4
{
	Vector4 m_vPosition; 
	Vector4 m_vTexcoord; 
};

struct HCoreCallBack
{
	void (*OnSize)(int x, int y);
	void (*OnPaint)(void);
	void (*OnIdle)(void);
	void (*OnClose)(void);

	HCoreCallBack(void);
};

struct HCoreDeviceSpec
{
	int m_iX, m_iY; 
	int m_iWidth, m_iHeight; 
	int m_iMultiSamples;
	bool m_bFullScreen; 
	char *m_szDevice;

	HCoreDeviceSpec(void)
	{
		m_iX = m_iY = 256;
		m_iWidth = m_iHeight = 512;
		m_iMultiSamples = 0;
		m_bFullScreen = false;
		m_szDevice = NULL;
	}
};

#define LIGHT_POINT			0
#define LIGHT_DIRECTIONAL	1
#define LIGHT_SPOT			2
#define LIGHT_MAX_LIGHTS	8

struct sHCoreLight
{

	Vector4 m_vPosition;
	Vector4 m_vDirection;

	Vector4 m_vAmbient;
	Vector4 m_vDiffuse;
	Vector4 m_vSpecular;

	Vector4 m_vAttenuation;

	float  m_fSpotlightInnerCone;
	float  m_fSpotlightCutoffCosine;
	float  m_fSpotlightExponent;	
	float  m_fPadding;

	int	   m_iLightType;
	int	   m_bEnabled;
	int	   m_padding[2];

	sHCoreLight(void)
	{
		m_vPosition.Set(0.0f, 0.0f, 0.0f, 1.0f);
		m_vDirection.Set(0.0f, 0.0f, 1.0f, 0.0f);

		m_vAmbient.Set(1.0f);
		m_vDiffuse.Set(1.0f);
		m_vSpecular.Set(0.0f);

		m_vAttenuation.Set(1.0f, 0.0f, 0.0f);

		m_iLightType = LIGHT_POINT;
		m_bEnabled = false;
	}
};


#define MAX_NUM_TEXTURES 3
#define MAX_TEXCOORDS 3

#define MAP_SPHEREMAP 32
#define MAP_CUBEMAP   33
#define MAP_NOOVERWRITE 0xff

struct sVertexDecl
{
	// position
	int m_iPositionOffset;
	int m_iNumPositionElements;
	// normal
	int m_iNormalOffset;
	int m_iNumNormalElements;
	// color
	int m_iColorOffset;
	int m_iNumColorElements;
	// texcoord
	int m_iTexcoordOffset[MAX_TEXCOORDS];
	int m_iNumTexcoordElements[MAX_TEXCOORDS];
	// tangent
	int m_iTangentOffset;
	int m_iNumTangentElements;
	// binormal
	int m_iBiNormalOffset;
	int m_iBiNormalElements;
	// stride
	int m_iVertexSize;

	sVertexDecl(void)
	{
		m_iPositionOffset = 0;
		m_iNumPositionElements = 3;
		m_iNormalOffset = -1;
		m_iNumNormalElements = 0;
		m_iColorOffset = -1;
		m_iNumColorElements = 0;
		for ( int i=0; i<MAX_TEXCOORDS; i++ )
		{
			m_iTexcoordOffset[i] = -1;
			m_iNumTexcoordElements[i] = 0;
		}
		m_iTangentOffset = -1;
		m_iNumTangentElements = 0;
		m_iBiNormalOffset = -1;
		m_iBiNormalElements = 0;
		m_iVertexSize = 0;
	}

	void SetVertexFormat(int PosElems, int NormalElems, int ColorElems, 
		int Tex0Elems=0, int Tex1Elems=0, int Tex2Elems=0, int Tex3Elems=0);
};

#endif // _HCoreIL_
