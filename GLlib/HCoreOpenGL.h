#ifndef _HCoreOPENGL_
#define _HCoreOPENGL_

#include "HCoreDefs.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

bool HCoreInitGraphicsDeviceOpenGL(HCoreDeviceSpec *spec=NULL);
bool HCoreReleaseGraphicsDeviceOpenGL(void);
void HCoreSwapBuffersOpenGL(void);

GLuint HCoreLoadVertexProgramOpenGL_ASM(const char *filename);
GLuint HCoreLoadFragmentProgramOpenGL_ASM(const char *filename);

void HCoreReleaseVertexProgramOpenGL(GLuint shader_id);
void HCoreReleasePixelProgramOpenGL(GLuint shader_id);

GLuint HCoreLoadVertexShaderOpenGL_GLSL(const char *filename);
GLuint HCoreLoadFragmentShaderOpenGL_GLSL(const char *filename);
GLuint HCoreCreateProgram(GLuint vs, GLuint fs);

void HCoreReleaseVertexShaderGLSL(GLuint shader_id);
void HCoreReleaseFragmentShaderGLSL(GLuint shader_id);

bool HCoreCreateRenderTargetOpenGL(int w, int h, GLuint color_fmt, GLuint *framebuffer, GLuint *texture);
bool HCoreCreateRenderTargetOpenGL(int w, int h, GLuint *framebuffer, 
								 GLuint color_fmt, GLuint *frametexture,
								 GLuint depth_fmt, GLuint *depthtexture);
bool HCoreCreateRenderTargetOpenGL(int w, int h, GLuint *pFrameBuffer,
								 GLuint color_fmt, GLuint *pFrameTexture, int num_mrts,
								 GLuint depth_fmt, GLuint *pDepthTexture);

void HCoreSetupLightOpenGL(int index, sHCoreLight &light);

#endif // _HCoreOPENGL_