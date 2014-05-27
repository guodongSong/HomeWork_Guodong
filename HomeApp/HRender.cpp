//基础层库
#include "HRender.h"
#include <windows.h>
#include <math.h>
#include "glew.h"
#include <GL/gl.h>
//底层运行库
#include "HData.h"
#include "HCore.h"
#include "HCoreWin32.h"
#include "HCoreModel_OpenGL.h"
#include "HCoreTexture_OpenGL.h"

//投影矩阵
static Matrix4x4 g_proj_matrix;

//模型对象
static CHCoreModel_OpenGL g_Model_OpenGL;
static CHCoreModel_OpenGL g_SSAOModel_OpenGL;

//Shader对象
static GLuint g_Texture = 0;
static GLuint g_VertexShader = 0;
static GLuint g_PixelShader = 0;
static GLuint g_Shader = 0;
static GLuint g_VettexShader_SSAO = 0;
static GLuint g_PixelShader_SSAO = 0;
static GLuint g_Shader_SSAO = 0;

//窗口宽高
static int w, h;

//随机数据结构
struct SAMPLE_POINTS
{
	//随机点
	Vector4 point[256];
	//随机向量
	Vector4 random_vectors[256];
};
SAMPLE_POINTS point_data;

//随机数种子
static unsigned int seed = 0x13371337;

//Shader Uniform变量
GLboolean Vflag = GL_TRUE;
GLboolean Vrandomize_points = GL_TRUE;
GLfloat Vssao_radius = 0.002f;
GLfloat Vobject_level = 1.0f;
GLfloat Vssao_level = 0.05f;
GLboolean Vshow_normal = GL_FALSE;
GLboolean Vshow_original = GL_FALSE;

//随机数生成
static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

//渲染通道1
static void Frame_Prepare(void)
{
	//灯光
	GLfloat lightpos[] = {0.0f,0.0f,20.0f};
	GLint reg = glGetUniformLocation(g_Shader,"LightPosition");
	glUniform3fv(reg, 1, (float *) &lightpos);
	
	//高程范围
	GLfloat maxh = 1.0f;
	reg = glGetUniformLocation(g_Shader,"maxh");
	glUniform1f(reg, maxh);

	GLfloat minh = -1.0f;
	reg = glGetUniformLocation(g_Shader,"minh");
	glUniform1f(reg, minh);
	
	//灯光衰减因子
	GLfloat lightfac = 1.0f;
	reg = glGetUniformLocation(g_Shader,"LightFactor");
	glUniform1f(reg, lightfac);
	
	//漫反射
	GLfloat diffuse_albedo[] = {0.8f,0.8f,0.9f};
	reg = glGetUniformLocation(g_Shader,"diffuse_albedo");
	glUniform3fv(reg, 1, (float *) &diffuse_albedo);
	
	//镜面反射
	GLfloat specular_albedo[] = {0.01f,0.01f,0.01f};
	reg = glGetUniformLocation(g_Shader,"specular_albedo");
	glUniform3fv(reg, 1, (float *) &specular_albedo);
	
	//镜面反射强度因子
	GLfloat specular_power = 128.0f;
	reg = glGetUniformLocation(g_Shader,"specular_power");
	glUniform1f(reg, specular_power);
	
	//光线强度
	GLfloat shading_level = 1.0f;
	reg = glGetUniformLocation(g_Shader,"shading_level");
	glUniform1f(reg, shading_level);
	
	//高程纹理
	GLint teximage_param = glGetUniformLocation(g_Shader, "tex");
	glUniform1i(teximage_param,1);
	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, g_Texture);

	//清理帧缓存
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绑定帧缓存
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_Framebuffer);
	GLuint mrt[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT};
	glDrawBuffers(3, mrt);

	// 清除画面
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	glUseProgram(g_Shader);

	// 使用矩阵
	Matrix4x4 object_matrix = g_Control.GetObjectMatrix();
	Matrix4x4 view_matrix = g_Control.GetViewMatrix();
	Matrix4x4 wv_matrix = object_matrix * view_matrix;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_proj_matrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf( (float *) &wv_matrix);
	
	//渲染模型
	g_Model_OpenGL.Render(SUBMIT_SHADER | SUBMIT_TEXTURES);
	g_SSAOModel_OpenGL.Render(SUBMIT_SHADER | SUBMIT_TEXTURES);
	
	//绘制缓存
	GLuint default_mrt[] = {GL_FRONT, GL_NONE, GL_NONE};
	glDrawBuffers(3, default_mrt);
	
	//使用固态渲染管道
	glUseProgram(0);
}

//渲染通道2
static void Frame_Process()
{
	//清理帧缓存
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//使用第二渲染通道
	glUseProgram(g_Shader_SSAO);
	
	//shader参数变量
	GLint camerarange = glGetUniformLocation(g_Shader_SSAO, "camerarange");
	GLint screensize = glGetUniformLocation(g_Shader_SSAO, "screensize");
	GLint fNearClipPlane = glGetUniformLocation(g_Shader_SSAO, "fNearClipPlane");
	GLint fFarClipPlane = glGetUniformLocation(g_Shader_SSAO, "fFarClipPlane");
	GLint flag = glGetUniformLocation(g_Shader_SSAO, "flag");
	GLint show_normal = glGetUniformLocation(g_Shader_SSAO, "show_normal");
	GLint show_original = glGetUniformLocation(g_Shader_SSAO, "show_original");
	GLint randomize_points = glGetUniformLocation(g_Shader_SSAO, "randomize_points");
	GLint fTime0_2PI = glGetUniformLocation(g_Shader_SSAO, "fTime0_2PI");
	GLint ssao_radius = glGetUniformLocation(g_Shader_SSAO, "ssao_radius");
	GLint object_level = glGetUniformLocation(g_Shader_SSAO, "object_level");
	GLint ssao_level = glGetUniformLocation(g_Shader_SSAO, "ssao_level");
	GLint randpos = glGetUniformLocation(g_Shader_SSAO, "randpos");
	GLint randvectors = glGetUniformLocation(g_Shader_SSAO, "randvectors");

	GLint texture0 = glGetUniformLocation(g_Shader_SSAO, "texture0");
	GLint texture1 = glGetUniformLocation(g_Shader_SSAO, "texture1");

	//视口近远切面
	GLfloat Vcamerarange[] = {0.001f,10000.0f};
	glUniform2fv(camerarange, 1, (float *)&Vcamerarange);
	GLfloat VfNearClipPlane = 0.001f;
	glUniform1f(fNearClipPlane, VfNearClipPlane);
	GLfloat VfFarClipPlane = 10000.0f;
	glUniform1f(fFarClipPlane , VfFarClipPlane);
	
	//视口宽高
	GLfloat Vscreensize[] = {(float)w,(float)h};
	glUniform2fv(screensize, 1, (float *)&Vscreensize);

	//打开或关闭SSAO像素采样图像
	glUniform1i(flag , Vflag);
	//查看法线及深度信息帧缓存
	glUniform1i(show_normal,Vshow_normal);
	//查看原渲染帧缓存
	glUniform1i(show_original,Vshow_original);
	//打开或关闭SSAO像素采样范围随机或固定半径
	glUniform1i(randomize_points, Vrandomize_points);
	//时间正玄参数
	GLfloat VfTime0_2PI = 3.0f;
	glUniform1f(fTime0_2PI , VfTime0_2PI);
	//SSAO像素采样范围
	glUniform1f(ssao_radius , Vssao_radius);
	//SSAO物体对比度
	glUniform1f(object_level , Vobject_level);
	//SSAO全局对比度
	glUniform1f(ssao_level , Vssao_level);
	//随机参数
	glUniform4fv(randpos,256,(float*)&point_data.point);
	glUniform4fv(randvectors,256,(float*)&point_data.random_vectors);
	
	//缓存纹理通道
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glUniform1i(texture1, 0);
	glBindTexture(GL_TEXTURE_2D, g_BufferTexture[BUFFER_COLOR]);
	glActiveTexture(GL_TEXTURE1_ARB);
	glUniform1i(texture0, 1);
	glBindTexture(GL_TEXTURE_2D, g_BufferTexture[BUFFER_NORMAL]);
	
	//渲染模型
	g_Model_OpenGL.Render(SUBMIT_SHADER );
	g_SSAOModel_OpenGL.Render(SUBMIT_SHADER);
	
	//使用固态渲染管线
	glUseProgram(0);
}

//资源初始化
bool InitResource(void)
{
	//判断是否支持动态渲染管线
	if ( NULL==glCreateProgram )
	{
		return false;
	}
	if ( NULL==glBindFramebufferEXT )
	{
		return false;
	}
	
	//获取窗口大小
	HCoreGetWindowSize(w, h);
	//宽高比例
	float fAspect = (float)h/(float)w;
	// 投影矩阵
	g_proj_matrix = HCoreMatrixPerspectiveRH_OpenGL(g_fFOV, fAspect, 0.001f, 10000.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_proj_matrix);
	glMatrixMode(GL_MODELVIEW);	
	
	//开启纹理及深度检测
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
	//设置纹理路径及读取纹理
	CHCoreModel::SetTexturePath("../textures/");
	g_Texture = HCoreLoadTexture_OpenGL("../textures/mountfrist.bmp");
	
	//绑定模型
	g_Model_OpenGL.ConvertToOpenGLModel(&g_Model);
	g_SSAOModel_OpenGL.ConvertToOpenGLModel(&g_SSAOModel);

	//创建帧缓存渲染对象
	GLuint fmt = GL_RGBA16F_ARB;
	if ( !HCoreCreateRenderTargetOpenGL(
		w, h, &g_Framebuffer, 
		fmt, g_BufferTexture, 3,
		GL_DEPTH_COMPONENT24, &g_BufferTexture[BUFFER_DEPTH]) )
	{
		HCorePushMessage("创建 渲染对象 失败");
		return false;
	}
	
	//绑定帧缓存
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, g_BufferTexture[BUFFER_DEPTH], 0);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if ( status!=GL_FRAMEBUFFER_COMPLETE_EXT )
	{
		return false;
	}
	
	//创建第一通道Shader
	g_VertexShader = HCoreLoadVertexShaderOpenGL_GLSL("../shaders/TerrianLine.glvs");
	g_PixelShader = HCoreLoadFragmentShaderOpenGL_GLSL("../shaders/TerrianLine.glfs");
	g_Shader = HCoreCreateProgram(g_VertexShader, g_PixelShader);
	//创建第二通道Shader
	g_VettexShader_SSAO = HCoreLoadVertexShaderOpenGL_GLSL("../shaders/SSAO.glvs");
	g_PixelShader_SSAO = HCoreLoadFragmentShaderOpenGL_GLSL("../shaders/SSAO.glfs");
	g_Shader_SSAO = HCoreCreateProgram(g_VettexShader_SSAO, g_PixelShader_SSAO);

	//随机参数生成
	int i;
	float tobreak = 0.0;
	for (i = 0; i < 256; i++)
	{
		do
		{
			point_data.point[i][0] = random_float() * 2.0f - 1.0f;
			point_data.point[i][1] = random_float() * 2.0f - 1.0f;
			point_data.point[i][2] = random_float(); //  * 2.0f - 1.0f;
			point_data.point[i][3] = 0.0f;
			tobreak =  sqrt(point_data.point[i][0] * point_data.point[i][0]
			+ point_data.point[i][1] * point_data.point[i][1]
			+ point_data.point[i][2] * point_data.point[i][2]);
		} while (tobreak > 1.0f); //length
		//normalize
		point_data.point[i] = point_data.point[i] / tobreak;
	}
	for (i = 0; i < 256; i++)
	{
		point_data.random_vectors[i][0] = random_float();
		point_data.random_vectors[i][1] = random_float();
		point_data.random_vectors[i][2] = random_float();
		point_data.random_vectors[i][3] = random_float();
	}

	return true;
}

//释放资源
bool ReleaseResource(void)
{
	//删除帧缓存
	glDeleteFramebuffersEXT(1, &g_Framebuffer);
	for ( int i=0; i<BUFFER_NUM; i++ )
	{
		glDeleteTextures(1, &g_BufferTexture[i]);
	}
	
	//删除shader
	glDeleteShader(g_VertexShader);
	glDeleteShader(g_PixelShader);
	glDeleteProgram(g_Shader);

	glDeleteShader(g_VettexShader_SSAO);
	glDeleteShader(g_PixelShader_SSAO);
	glDeleteProgram(g_Shader_SSAO);
	return true;
}

void RenderFrame(void)
{
	//判断输入按键
	switch (g_iSwitchMode)
	{
	case 0:
		Vflag = !Vflag; 
		break;
	case 1:
		Vrandomize_points = !Vrandomize_points;
		break;
	case 2:
		Vssao_radius += 0.01f;
		break;
	case 3:
		Vssao_radius -= 0.01f;
		break;
	case 4:
		Vobject_level += 0.1f;
		break;
	case 5:
		Vobject_level -= 0.1f;
		break;
	case 6:
		Vssao_level += 0.01f;
		break;
	case 7:
		Vssao_level -= 0.01f;
		break;
	case 8:
		Vshow_normal = !Vshow_normal;
		break;
	case 9:
		Vshow_original = !Vshow_original;
		break;
	default:
		break;
	}
	//按键输入信息复位
	g_iSwitchMode = -1;
	
	//第一渲染通道
	Frame_Prepare();
	//第二渲染通道
	Frame_Process();
	
	//刷新当前渲染帧
	HCoreSwapBuffersOpenGL();
}

void ResizeWindow(int width, int height)
{
	// 使用新的窗口大小做为新的绘图分辨率
	glViewport(0, 0, width, height);
	w = width;
	h = height;
	// 投影矩阵, 重设水平和垂直方向的视角.
	float aspect = (float) height / (float) width;
	g_proj_matrix = HCoreMatrixPerspectiveRH_OpenGL(60.0f, aspect, 0.001f, 10000.0f);
	// 设置视角转换矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &g_proj_matrix);
}
