//加载底层运行库
#include "Vector4.h"
#include "Matrix4x4.h"
#include "HCoreModel.h"
#include "HCoreUserControl.h"
//缓存Buffer类型
enum {
	BUFFER_COLOR,
	BUFFER_NORMAL,
	BUFFER_ALBEDO,
	BUFFER_DEPTH,
	BUFFER_NUM
};
//操控对象
extern CHCoreUserControl g_Control;
//模型对象
extern CHCoreModel g_Model;
extern CHCoreModel g_SSAOModel;
//相机设置
extern float g_fFOV;
//缓存Buffer
extern GLuint g_Framebuffer;
extern GLuint g_BufferTexture[BUFFER_NUM];
//按键输入状态
extern int g_iSwitchMode;