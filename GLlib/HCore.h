#ifndef _HCoreIL_
#define _HCoreIL_

#include <windows.h>
#include <GL/gl.h>
#include <string>

#include "HCoreDefs.h"
#include "HCoreMem.h"
#include "HCoreOpenGL.h"

#include "Vector4.h"
#include "Matrix4x4.h"

extern HCoreCallBack g_HCoreCallBack;

void HCoreGetMouseState(int &x, int &y, int button[3]);
bool HCoreCreateWindow(int x, int y, int width, int height, const char *title = "HCore Application");
bool HCoreFullScreen(int width, int height);
bool HCoreCreateWindow(HCoreDeviceSpec &spec);
void HCoreEnterMessageLoop(void);
bool HCoreProcessMessage(void);
void HCoreResizeFunc(  void (*resize)(int x, int y) );
void HCoreDisplayFunc( void (*onpaint)(void) );
void HCoreIdleFunc( void (*onidle)(void) );
void HCoreOnCloseFunc( void (*onclose)(void) );
bool HCoreInitGraphicsDevice(const char *device);
bool HCoreInitGraphicsDevice(HCoreDeviceSpec &spec);
bool HCoreReleaseGraphicsDevice(void);

void HCoreSetShaderPath(const char *path);
const char *HCoreGetShaderPath(void);

HCoreEnum HCoreGetGraphicsDeviceType(const char *device = NULL);

Matrix4x4 HCoreMatrixLookAtRH(Vector4 &eye, Vector4 &lookat, Vector4 &up);
Matrix4x4 HCoreMatrixLookAtLH(Vector4 &eye, Vector4 &lookat, Vector4 &up);

Matrix4x4 HCoreMatrixOrthoLH(float w, float h, float z_near, float z_far);
Matrix4x4 HCoreMatrixOrthoRH(float w, float h, float z_near, float z_far);

Matrix4x4 HCoreMatrixPerspectiveRH(float fovy, float aspect, float z_near, float z_far);
Matrix4x4 HCoreMatrixPerspectiveLH(float fovy, float aspect, float z_near, float z_far);

Matrix4x4 HCoreMatrixOrtho_OpenGL(float w, float h, float z_near, float z_far);
Matrix4x4 HCoreMatrixOrthoRH_OpenGL(float w, float h, float z_near, float z_far);
Matrix4x4 HCoreMatrixOrthoLH_OpenGL(float w, float h, float z_near, float z_far);
Matrix4x4 HCoreMatrixPerspectiveRH_OpenGL(float fovy, float aspect, float z_near, float z_far);
Matrix4x4 HCoreMatrixPerspectiveRH_OpenGL(float fovy, float aspect, float z_near, float z_far);
Matrix4x4 HCoreMatrixPerspectiveLH_OpenGL(float fovy, float aspect, float z_near, float z_far);

bool HCoreCreateGrids(int x_grids, int y_grids, 
				   Vertex_V3N3 **ppVertices, int &num_vertices, 
				   unsigned short **ppIndices, int &num_indices,
				   int &num_triangles);

bool HCoreReleaseGrids(Vertex_V3N3 **ppVertices, unsigned short **ppIndices);

void HCoreConvertColor(Vector4 &vColor, unsigned char rgba[4]);

void HCorePushMessage(const char *msg);
std::string HCorePopMessage(void);

#endif // _HCoreIL_
