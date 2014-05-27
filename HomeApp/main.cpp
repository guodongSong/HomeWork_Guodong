#include <stdio.h>
#include <conio.h>

//���صײ����п�
#include "HRender.h"
#include "HData.h"
#include "HCore.h"
#include "HCoreWin32.h"
#include "HCoreInput.h"
#include "HCoreTimer.h"

HCoreTimer g_Timer;

//����״̬����
void KeyDown_0(void)
{
	//�򿪻�ر�SSAO���ز���ͼ��
	printf("flag on off\n");
	g_iSwitchMode = 0;
}
void KeyDown_1(void)
{
	//�򿪻�ر�SSAO���ز�����Χ�����̶��뾶
	printf("Vrandomize_points on off\n");
	g_iSwitchMode = 1;
}
void KeyDown_2(void)
{
	//����SSAO���ز�����Χ
	printf("Vssao_radius+\n");
	g_iSwitchMode = 2;
}
void KeyDown_3(void)
{
	//����SSAO���ز�����Χ
	printf("Vssao_radius-\n");
	g_iSwitchMode = 3;
}
void KeyDown_4(void)
{
	//����SSAO����Աȶ�
	printf("Vobject_level+\n");
	g_iSwitchMode = 4;
}
void KeyDown_5(void)
{
	//����SSAO����Աȶ�
	printf("Vobject_level-\n");
	g_iSwitchMode = 5;
}
void KeyDown_6(void)
{
	//����SSAOȫ�ֶԱȶ�
	printf("Vssao_level+\n");
	g_iSwitchMode = 6;
}
void KeyDown_7(void)
{
	//����SSAOȫ�ֶԱȶ�
	printf("Vssao_level-\n");
	g_iSwitchMode = 7;
}
void KeyDown_8(void)
{
	//�鿴���߼������Ϣ֡����
	printf("show_normal on off\n");
	g_iSwitchMode = 8;
}
void KeyDown_9(void)
{
	//�鿴ԭ��Ⱦ֡����
	printf("show_original on off\n");
	g_iSwitchMode = 9;
}

//��ȡ��������
void GetUserInput(void)
{
	float fTimeAdvance = g_Timer.Stop();
	g_Timer.Restart();
	HCoreReadKeyboard();
	g_Control.Update(fTimeAdvance, CHCoreUserControl::CONTROLLER_FPSCAMERA);
}

//������
void main(void)
{
	//����Opengl��Ⱦ����������
	char *device = "opengl";
	HCoreResizeFunc( ResizeWindow );
	HCoreCreateWindow(100, 100, 800, 600, device);

	if ( !HCoreInitGraphicsDevice(device) )
	{
		printf("��ʼ��ʧ�� \n", device);
		exit(0);
	}
	
	//ע�����뺯��
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
	
	//�������
	g_Control.SetCamera(Vector4(0.0f, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 1.0f), Vector4(0.0f, 0.0f, 1.0f) );

	//��ȡģ��
	//����
	g_Model.Load_ASCII("..\\models\\terrain.gma");
	//����
	g_SSAOModel.Load_ASCII("..\\models\\chess.gma");

	// ����shader
	if ( !InitResource() )
	{
		ReleaseResource();
		exit(0);
	}

	// ��ѭ��
	while( HCoreProcessMessage() )
	{
		GetUserInput();
		RenderFrame();
	}

	// �ͷ�shader
	ReleaseResource();

	// �ر�OpenGL��ͼ�豸
	HCoreReleaseGraphicsDevice();
}