#include <stdio.h>
#include <conio.h>

//加载底层运行库
#include "HRender.h"
#include "HData.h"
#include "HCore.h"
#include "HCoreWin32.h"
#include "HCoreInput.h"
#include "HCoreTimer.h"

HCoreTimer g_Timer;

//各种状态开关
void KeyDown_0(void)
{
	//打开或关闭SSAO像素采样图像
	printf("flag on off\n");
	g_iSwitchMode = 0;
}
void KeyDown_1(void)
{
	//打开或关闭SSAO像素采样范围随机或固定半径
	printf("Vrandomize_points on off\n");
	g_iSwitchMode = 1;
}
void KeyDown_2(void)
{
	//增加SSAO像素采样范围
	printf("Vssao_radius+\n");
	g_iSwitchMode = 2;
}
void KeyDown_3(void)
{
	//降低SSAO像素采样范围
	printf("Vssao_radius-\n");
	g_iSwitchMode = 3;
}
void KeyDown_4(void)
{
	//增加SSAO物体对比度
	printf("Vobject_level+\n");
	g_iSwitchMode = 4;
}
void KeyDown_5(void)
{
	//降低SSAO物体对比度
	printf("Vobject_level-\n");
	g_iSwitchMode = 5;
}
void KeyDown_6(void)
{
	//增加SSAO全局对比度
	printf("Vssao_level+\n");
	g_iSwitchMode = 6;
}
void KeyDown_7(void)
{
	//降低SSAO全局对比度
	printf("Vssao_level-\n");
	g_iSwitchMode = 7;
}
void KeyDown_8(void)
{
	//查看法线及深度信息帧缓存
	printf("show_normal on off\n");
	g_iSwitchMode = 8;
}
void KeyDown_9(void)
{
	//查看原渲染帧缓存
	printf("show_original on off\n");
	g_iSwitchMode = 9;
}

//获取按键输入
void GetUserInput(void)
{
	float fTimeAdvance = g_Timer.Stop();
	g_Timer.Restart();
	HCoreReadKeyboard();
	g_Control.Update(fTimeAdvance, CHCoreUserControl::CONTROLLER_FPSCAMERA);
}

//主进程
void main(void)
{
	//创建Opengl渲染环境及窗口
	char *device = "opengl";
	HCoreResizeFunc( ResizeWindow );
	HCoreCreateWindow(100, 100, 800, 600, device);

	if ( !HCoreInitGraphicsDevice(device) )
	{
		printf("初始化失败 \n", device);
		exit(0);
	}
	
	//注册输入函数
	HCoreInputInit();

	HCoreRegisterKeyDown(HCoreKEY_0, KeyDown_0);
	HCoreRegisterKeyDown(HCoreKEY_1, KeyDown_1);
	HCoreRegisterKeyDown(HCoreKEY_2, KeyDown_2);
	HCoreRegisterKeyDown(HCoreKEY_3, KeyDown_3);
	HCoreRegisterKeyDown(HCoreKEY_4, KeyDown_4);
	HCoreRegisterKeyDown(HCoreKEY_5, KeyDown_5);
	HCoreRegisterKeyDown(HCoreKEY_6, KeyDown_6);
	HCoreRegisterKeyDown(HCoreKEY_7, KeyDown_7);
	HCoreRegisterKeyDown(HCoreKEY_8, KeyDown_8);
	HCoreRegisterKeyDown(HCoreKEY_9, KeyDown_9);
	
	//设置相机
	g_Control.SetCamera(Vector4(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f) );

	//读取模型
	//地形
	g_Model.Load_ASCII("..\\models\\terrain.gma");
	//棋盘
	g_SSAOModel.Load_ASCII("..\\models\\chess.gma");

	// 载入shader
	if ( !InitResource() )
	{
		ReleaseResource();
		exit(0);
	}

	// 主循环
	while( HCoreProcessMessage() )
	{
		GetUserInput();
		RenderFrame();
	}

	// 释放shader
	ReleaseResource();

	// 关闭OpenGL绘图设备
	HCoreReleaseGraphicsDevice();
}