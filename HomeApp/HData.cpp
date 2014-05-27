//加载底层运行库
#include "HData.h"
#include "Vector4.h"
#include "HCore.h"
#include "HCoreModel.h"
#include "HCoreUserControl.h"

//操控对象
CHCoreUserControl g_Control;
//模型对象
CHCoreModel g_Model;
CHCoreModel g_SSAOModel;
//相机设置
float g_fFOV = 45.0f;
//缓存Buffer
GLuint g_Framebuffer = 0;
GLuint g_BufferTexture[BUFFER_NUM] = {0, 0, 0, 0};
//按键输入状态
int g_iSwitchMode = -1;