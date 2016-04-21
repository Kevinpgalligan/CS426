#pragma once
#undef ERROR
#include <windows.h>
#include <math.h>
#include <GL/gl.h>   // OpenGL Linker add openGL32.lib, gdi32.lib and User32.lib
#include <GL/glu.h>  // OpenGL Utility Library Linker
#include <glm.h>
#include <glm.c>
#include <glmimg.c>
#include <glm_util.c>
//#include<glmint.h> // Might be required another day
//#include<glmimg_devil.c>
//#include<glmimg_jpg.c>
//#include<glmimg_png.c>
//#include<glmimg_sdl.c>
//#include<glmimg_sim.c>
 
using namespace System::Windows::Forms;

namespace OpenGLForm
{
    public ref class Create_OpenGL: 
    public System::Windows::Forms::NativeWindow
{
public: double longitude;
public: double radius;
public: double latitude;
public: double rotation;

GLMmodel *objmodel_stars;
GLMmodel *objmodel_sun;
GLMmodel *objmodel_earth;
GLMmodel *objmodel_moon;

public:
    Create_OpenGL(System::Windows::Forms::Form ^parentForm, GLsizei iX, GLsizei iY,
            GLsizei iWidth, GLsizei iHeight)
    {
		// "gcnew" is an operator, just like the "new" operator, except that you don't have to delete anything created with it. It's garbase collected.
		// "CreateParams" Gets the required creation parameters when the control handle is created
		// The "^" is the managed equivalent of a * (pointer) which in C++/CLI terminology is called a 'handle' to a 'reference type' (since you can still have unmanaged pointers).
        CreateParams^ cp = gcnew CreateParams;

        // Set the position on the form
        cp->X = iX;
        cp->Y = iY;
        cp->Height = iHeight;
        cp->Width = iWidth;

        // Specify the form as the parent.
        cp->Parent = parentForm->Handle;

        // Create as a child of the specified parent
        // and make OpenGL compliant (no clipping)
        cp->Style = WS_CHILD | WS_VISIBLE | 
                    WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

        // Create the actual window
        this->CreateHandle(cp);

        m_hDC = GetDC((HWND)this->Handle.ToPointer());

        if(m_hDC)
            MySetPixelFormat(m_hDC);

		longitude = M_PI / 2;
		radius = 10;
		rotation = 0;

		objmodel_stars = glmReadOBJ("stars.obj");
		glmUnitize(objmodel_stars);
		glmFacetNormals(objmodel_stars);
		glmVertexNormals(objmodel_stars, 90.0,GL_FALSE);
			
		objmodel_sun = glmReadOBJ("sun.obj");
		glmUnitize(objmodel_sun);
		glmFacetNormals(objmodel_sun);
		glmVertexNormals(objmodel_sun, 90.0,GL_FALSE);

		objmodel_earth = glmReadOBJ("earth.obj");
		glmUnitize(objmodel_earth);
		glmFacetNormals(objmodel_earth);
		glmVertexNormals(objmodel_earth, 90.0,GL_FALSE);

		objmodel_moon = glmReadOBJ("moon.obj");
		glmUnitize(objmodel_moon);
		glmFacetNormals(objmodel_moon);
		glmVertexNormals(objmodel_moon, 90.0,GL_FALSE);

		glEnable(GL_DEPTH_TEST); // Enable Z buffer depth test  
		glEnable(GL_CULL_FACE);  // Enable backface culling
		glFrontFace(GL_CCW);     // Counterclockwise
    }

 virtual System::Void Render(System::Void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // Turn on Z-depth buffering
	glEnable(GL_CULL_FACE);  // Turn on backface culling          
	glFrontFace(GL_CCW);     // Treat counter-clockwise ordering of vertices as front facing

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 640.0/480.0, 1.0, 1000.0); // Field of view 40, width/height, near clip, far clip
			 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	double Cx, Cy, Cz;
	Cx = radius * cos(latitude) * cos(longitude);
	Cy = radius * sin(latitude);
	Cz = radius * cos(latitude) * sin(longitude);
	if ((latitude <= -(M_PI/2)) || (latitude > (M_PI/2)))
	{
		gluLookAt(Cx, Cy, Cz,   // eye, (where the camera is located)
		0.0, 0.0, 0.0,          // target, (where the camera is looking)
		0.0, -1.0, 0.0);        // down, vector of the camera (is Down)
	}
	else
	{
		gluLookAt(Cx, Cy, Cz,   // eye, (where the camera is located)
		0.0, 0.0, 0.0,          // target, (where the camera is looking)
		0.0, 1.0, 0.0);         // up, vector of the camera (is Up)
	}

	glDisable(GL_LIGHTING);	 // Draw the star-sphere and sun as textures without lighting 
	glRotatef(rotation*0.01,0,1,0);  // Rotate everything

	glPushMatrix();
	glScalef(250, 250, 250);
	glmDraw(objmodel_stars, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL ); // star_sphere
	glPopMatrix();

	glmDraw(objmodel_sun,  GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL );  // sun

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 }; // position light at centre of sun
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
	GLfloat w[] = {0.1f, 0.1f, 0.1f, 1.f};             // even the unlit regions emit some light
	glMaterialfv(GL_FRONT, GL_EMISSION, w);

	glRotatef(rotation*-0.1,0,1,0);
	glTranslatef(-3.0f, 0.0f, 0.0f);

	glPushMatrix();
	glScalef(0.25, 0.25, 0.25);
	glRotatef(rotation,0,1,0);
	glmDraw(objmodel_earth,  GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL ); // earth
	glPopMatrix();

	glRotatef(-rotation+(rotation*0.5),0,1,0);  // Moon about the earth
	glTranslatef(0.8f, 0.0f, 0.0f);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(0,0,1,0);
	glmDraw(objmodel_moon,GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL ); // moon

	rotation += 1.0;

	SwapOpenGLBuffers();
}

System::Void SwapOpenGLBuffers(System::Void)
{
    SwapBuffers(m_hDC) ;
}

private:
    HDC m_hDC;
    HGLRC m_hglrc;

	protected:
    ~Create_OpenGL(System::Void)
    {
        this->DestroyHandle();
    }

	 GLint MySetPixelFormat(HDC hdc)
    {
        PIXELFORMATDESCRIPTOR pfd = { 
            sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
            1,                                // version number 
            PFD_DRAW_TO_WINDOW |              // support window 
            PFD_SUPPORT_OPENGL |              // support OpenGL 
            PFD_DOUBLEBUFFER,                 // double buffered 
            PFD_TYPE_RGBA,                    // RGBA type 
            24,                               // 24-bit color depth 
            0, 0, 0, 0, 0, 0,                 // color bits ignored 
            0,                                // no alpha buffer 
            0,                                // shift bit ignored 
            0,                                // no accumulation buffer 
            0, 0, 0, 0,                       // accum bits ignored 
            32,                               // 32-bit z-buffer     
            0,                                // no stencil buffer 
            0,                                // no auxiliary buffer 
            PFD_MAIN_PLANE,                   // main layer 
            0,                                // reserved 
            0, 0, 0                           // layer masks ignored 
        }; 
    
        GLint  iPixelFormat; 
     
        // get the device context's best, available pixel format match 
        if((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
        {
            MessageBox::Show("ChoosePixelFormat Failed");
            return 0;
        }
         
        // make that match the device context's current pixel format 
        if(SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
        {
            MessageBox::Show("SetPixelFormat Failed");
            return 0;
        }
    
        if((m_hglrc = wglCreateContext(m_hDC)) == NULL)
        {
            MessageBox::Show("wglCreateContext Failed");
            return 0;
        }
        
        if((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
        {
            MessageBox::Show("wglMakeCurrent Failed");
            return 0;
        }
    
        return 1;
    }
};
}