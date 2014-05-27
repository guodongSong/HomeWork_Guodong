#include <stdio.h>
#include "Gut.h"

void RenderFrameDX9(void);
void RenderFrameOpenGL(void);

void main(int argc, char *argv[])
{
	char *device = "dx9";

	if ( argc > 1 )
	{
		if ( stricmp(argv[1], "opengl")==0 )
		{
			device = "opengl";
		}
	}

	GutCreateWindow(100, 100, 512, 512, device);

	if ( !GutInitGraphicsDevice(device) )
	{
		printf("Failed to initialize %s device\n", device);
		exit(0);
	}

	// Ö÷Ñ­»·
	while( GutProcessMessage() )
	{
		if ( !strcmp(device, "dx9") )
		{
			RenderFrameDX9();
		}
		else
		{
			RenderFrameOpenGL();
		}
	}

	GutReleaseGraphicsDevice();
}

float vertex[] = {0.0f, 0.0f, 0.0f};

void RenderFrameDX9(void)
{

	LPDIRECT3DDEVICE9 device = GutGetGraphicsDeviceDX9();

    device->SetRenderState( D3DRS_LIGHTING, FALSE );

    device->Clear( 
		0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0, 0, 0, 0), 
		1.0f, 
		0 
	);

	device->BeginScene(); 
	device->SetFVF(D3DFVF_XYZ); 
	device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vertex, 12); 
	device->EndScene();		

    device->Present( NULL, NULL, NULL, NULL );
}

void RenderFrameOpenGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_POINTS);
	glVertex3fv(vertex);
	glEnd();

	GutSwapBuffersOpenGL();
}
