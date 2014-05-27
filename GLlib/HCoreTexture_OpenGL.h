#ifndef _HCoreTEXTURE_OPENGL_
#define _HCoreTEXTURE_OPENGL_

#include "HCoreImage.h"

GLuint HCoreLoadTexture_OpenGL(const char *filename, sImageInfo *pInfo=NULL, bool mipmap_enabled = true);
GLuint HCoreLoadCubemapTexture_OpenGL(const char **filename_array, bool mipmap_enabled = true);
GLuint HCoreLoadCubemapTexture_OpenGL(const char *filename);
GLuint HCoreLoadVolumeTexture_OpenGL(const char *filename);

#endif // _HCoreTEXTURE_OPENGL_
