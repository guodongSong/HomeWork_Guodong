#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <string>
#include <stack>

#include "HCoreWin32.h"
#include "HCore.h"
#include "HCoreOpenGL.h"

HCoreCallBack g_HCoreCallBack;
static char g_szDeviceName[8];
HCoreEnum g_eDeviceType = HCore_UNKNOWN;

HCoreCallBack::HCoreCallBack(void)
{
	memset(this, 0, sizeof(HCoreCallBack));
}

void HCoreResizeFunc( void (*onsize)(int x, int y) )
{
	g_HCoreCallBack.OnSize = onsize;
}

void HCoreDisplayFunc( void (*onpaint)(void) )
{
	g_HCoreCallBack.OnPaint = onpaint;
}

void HCoreIdleFunc( void (*onidle)(void) )
{
	g_HCoreCallBack.OnIdle = onidle;
}

void HCoreOnCloseFunc( void (*onclose)(void) )
{
	g_HCoreCallBack.OnClose = onclose;
}

bool HCoreInitGraphicsDevice(const char *device)
{
	strcpy(g_szDeviceName, device);
	g_eDeviceType = HCoreGetGraphicsDeviceType(device);

	switch(g_eDeviceType)
	{
	case HCore_OPENGL:
		return HCoreInitGraphicsDeviceOpenGL();
		break;
	}

	g_eDeviceType = HCore_UNKNOWN;
	g_szDeviceName[0] = '\0';

	return false;
}

bool HCoreInitGraphicsDevice(HCoreDeviceSpec &spec)
{
	strcpy(g_szDeviceName, spec.m_szDevice);
	g_eDeviceType = HCoreGetGraphicsDeviceType(spec.m_szDevice);

	switch(g_eDeviceType)
	{
	case HCore_OPENGL:
		return HCoreInitGraphicsDeviceOpenGL(&spec);
		break;
	}

	g_eDeviceType = HCore_UNKNOWN;
	g_szDeviceName[0] = '\0';

	return false;
}

bool HCoreReleaseGraphicsDevice(void)
{
	if ( !stricmp(g_szDeviceName, "opengl") )
	{
		return HCoreReleaseGraphicsDeviceOpenGL();
	}
	return false;
}

HCoreEnum HCoreGetGraphicsDeviceType(const char *device)
{
	if ( device )
	{
		if ( !stricmp(device, "opengl") )
		{
			return HCore_OPENGL;
		}
		else
		{
			return HCore_UNKNOWN;
		}
	}
	else
	{
		return g_eDeviceType;
	}
}

Matrix4x4 HCoreMatrixLookAtLH(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (lookat - eye); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	Matrix4x4 matrix; 
	matrix.Identity();

	matrix.SetColumn(0, xaxis);
	matrix.SetColumn(1, yaxis);
	matrix.SetColumn(2, zaxis);
	matrix[3][0] = -Vector3Dot(xaxis, eye)[0];
	matrix[3][1] = -Vector3Dot(yaxis, eye)[0];
	matrix[3][2] = -Vector3Dot(zaxis, eye)[0];

	return matrix;
}

Matrix4x4 HCoreMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up)
{
	Vector4 up_normalized = VectorNormalize(up);
	Vector4 zaxis = (eye - lookat); zaxis.Normalize();
	Vector4 xaxis = Vector3CrossProduct(up_normalized, zaxis);
	Vector4 yaxis = Vector3CrossProduct(zaxis, xaxis);

	Matrix4x4 matrix; 
	matrix.Identity();

	matrix.SetColumn(0, xaxis);
	matrix.SetColumn(1, yaxis);
	matrix.SetColumn(2, zaxis);
	matrix[3][0] = -Vector3Dot(xaxis, eye)[0];
	matrix[3][1] = -Vector3Dot(yaxis, eye)[0];
	matrix[3][2] = -Vector3Dot(zaxis, eye)[0];

	return matrix;
}

Matrix4x4 HCoreMatrixOrthoRH(float w, float h, float z_near, float z_far)
{
	switch(HCoreGetGraphicsDeviceType())
	{
	case HCore_OPENGL:
		return HCoreMatrixOrthoRH_OpenGL(w, h, z_near, z_far);
		break;
	}
}

Matrix4x4 HCoreMatrixPerspectiveRH(float fovy, float aspect, float z_near, float z_far)
{
	switch(HCoreGetGraphicsDeviceType())
	{
	case HCore_OPENGL:
		return HCoreMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);
		break;
	}
}


Matrix4x4 HCoreMatrixOrthoRH_OpenGL(float w, float h, float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();
	
	matrix[0][0] = 2.0f/w;
	matrix[1][1] = 2.0f/h;
	matrix[2][2] = 2.0f/(z_near - z_far);
	matrix[3][2] = (z_far + z_near)/(z_near - z_far);
	matrix[3][3] = 1.0f;

	return matrix;
}

Matrix4x4 HCoreMatrixOrtho_OpenGL(float w, float h, float z_near, float z_far)
{
	return HCoreMatrixOrthoRH_OpenGL(w, h, z_near, z_far);
}

Matrix4x4 HCoreMatrixPerspectiveRH_OpenGL(float fovy, float aspect, 
										float z_near, float z_far)
{
	Matrix4x4 matrix;
	matrix.Identity();

	float fovy_radian = FastMath::DegreeToRadian(fovy);
	float yscale =  FastMath::Cot(fovy_radian/2.0f);
	float xscale = yscale * aspect;

	matrix[0][0] = xscale;
	matrix[1][1] = yscale;
	matrix[2][2] = (z_far + z_near)/(z_near - z_far);
	matrix[2][3] = -1.0f;
	matrix[3][2] = 2.0f * z_far * z_near / (z_near - z_far);
	matrix[3][3] = 0.0f;

	return matrix;
}

Matrix4x4 HCoreMatrixPerspective_OpenGL(float fovy, float aspect, float z_near, float z_far)
{
	return HCoreMatrixPerspectiveRH_OpenGL(fovy, aspect, z_near, z_far);
}


bool HCoreCreateGrids(int x_grids, int y_grids, 
					  Vertex_V3N3 **ppVertices, int &num_vertices, 
					  unsigned short **ppIndices, int &num_indices, int &num_triangles)
{
	const int triangles_per_row = x_grids * 2;
	const int indices_per_row = triangles_per_row + 2;
	
	num_vertices = (x_grids + 1) * (y_grids + 1);
	Vertex_V3N3 *pVertices = (Vertex_V3N3 *) HCoreAllocate16BytesAlignedMemory(sizeof(Vertex_V3N3)*num_vertices);
	*ppVertices = pVertices;
	if ( pVertices==NULL )
		return false;

	num_indices = indices_per_row * y_grids;
	unsigned short *pIndices = new unsigned short[num_indices];
	*ppIndices = pIndices;
	if ( pIndices==NULL )
	{
		HCoreRelease16BytesAlignedMemory(pVertices);
		return false;
	}

	num_triangles = num_indices-2;

	Vector4 vCorner(-0.5f, 0.5f, 0.0f, 1.0f);
	Vector4 vStep(1.0f/float(x_grids), -1.0f/float(y_grids), 0.0f, 0.0f);
	Vector4 vPosition = vCorner;
	Vector4 vNormal(0.0f, 0.0f, 1.0f);

	int x,y;
	int vertex_index = 0;

	for ( y=0; y<=y_grids; y++)
	{
		vPosition[0] = vCorner[0];
		for ( x=0; x<=x_grids; x++, vertex_index++)
		{
			vPosition.StoreXYZ(pVertices[vertex_index].m_Position);
			vNormal.StoreXYZ(pVertices[vertex_index].m_Normal);
			vPosition[0] += vStep[0];
		}
		vPosition[1] += vStep[1];
	}

	const int vertices_per_row = x_grids + 1;
	bool from_left_to_right = true;
	int  index_index = 0;

	for ( y=0; y<y_grids; y++ )
	{
		if ( from_left_to_right )
		{
			pIndices[index_index++] = y * vertices_per_row;
			pIndices[index_index++] = y * vertices_per_row + vertices_per_row;

			for ( x=0; x<x_grids; x++ )
			{
				vertex_index = y * vertices_per_row + x;
				pIndices[index_index++] = vertex_index + 1;
				pIndices[index_index++] = vertex_index + 1 + vertices_per_row;
			}
		}
		else
		{
			pIndices[index_index++] = y * vertices_per_row + x_grids;
			pIndices[index_index++] = (y+1) * vertices_per_row + x_grids;

			for ( x=x_grids; x>0; x-- )
			{
				vertex_index = y * vertices_per_row + x;
				pIndices[index_index++] = vertex_index - 1;
				pIndices[index_index++] = vertex_index - 1 + vertices_per_row;
			}
		}
		from_left_to_right = !from_left_to_right;
	}

	return true;
}

bool HCoreReleaseGrids(Vertex_V3N3 **ppVertices, unsigned short **ppIndices)
{
	if ( *ppVertices )
	{
		HCoreRelease16BytesAlignedMemory(*ppVertices);
		*ppVertices = NULL;
	}

	if ( *ppIndices )
	{
		delete [] *ppIndices;
		*ppIndices = NULL;
	}

	return true;
}

void HCoreConvertColor(Vector4 &vColor, unsigned char rgba[4])
{
	Vector4 c = vColor * 255.0f;
	
	switch( HCoreGetGraphicsDeviceType() )
	{
	case HCore_DX9:
		rgba[0] = (unsigned char ) c[2];
		rgba[1] = (unsigned char ) c[1];
		rgba[2] = (unsigned char ) c[0];
		rgba[3] = (unsigned char ) c[3];
		break;
	default:
		rgba[0] = (unsigned char ) c[0];
		rgba[1] = (unsigned char ) c[1];
		rgba[2] = (unsigned char ) c[2];
		rgba[3] = (unsigned char ) c[3];
		break;
	}
}

static char g_szShaderPath[128] = "";

void HCoreSetShaderPath(const char *path)
{
	strcpy(g_szShaderPath, path);
}

const char *HCoreGetShaderPath(void)
{
	return g_szShaderPath;
}

static std::stack<std::string> g_MessagePool;

void HCorePushMessage(const char *msg)
{
	if ( msg && msg[0] )
	{
		std::string str = msg;
		g_MessagePool.push(str);
	}
}

std::string HCorePopMessage(void)
{
	std::string str;

	if ( g_MessagePool.size() )
	{
		str = g_MessagePool.top();
		g_MessagePool.pop();
	}

	return str;
}
