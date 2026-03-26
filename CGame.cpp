#include "CGame.h"

bool CGame::OnCreate()
{
    m_hdc = GetDC(m_hwnd); // get device context
    if (!m_hdc)
    {
        HandleError(TEXT("Cannot create DC!"));
        return false;
    }

    static PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
        1,                                          // Version Number
        PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
        PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
        PFD_TYPE_RGBA,                              // Request An RGBA Format
        32,                                         // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
        0,                                          // No Alpha Buffer
        0,                                          // Shift Bit Ignored
        0,                                          // No Accumulation Buffer
        0, 0, 0, 0,                                 // Accumulation Bits Ignored
        32,                                         // 16Bit Z-Buffer (Depth Buffer)  
        0,                                          // No Stencil Buffer
        0,                                          // No Auxiliary Buffer
        PFD_MAIN_PLANE,                             // Main Drawing Layer
        0,                                          // Reserved
        0, 0, 0                                     // Layer Masks Ignored
    };

    m_pf = ChoosePixelFormat(m_hdc,&pfd);
    if (!m_pf)
    {
        HandleError(TEXT("Cannot create pixel format!"));
        return false;
    }

    if (!SetPixelFormat(m_hdc,m_pf,&pfd))
    {
        HandleError(TEXT("Cannot set pixel format!"));
        return false;
    }

    m_rc = wglCreateContext(m_hdc);

    if (!m_rc)
    {
        HandleError(TEXT("Cannot create OpenGL rendering context!"));
        return false;
    }

    if (!wglMakeCurrent(m_hdc,m_rc))
    {
        HandleError(TEXT("Cannot activate OpenGL rendering context!"));
        return false;
    }

    // load OpenGL function pointers via GLAD
    if (!gladLoadGL())
    {
        HandleError(TEXT("Failed to initialize GLAD (load OpenGL functions)!"));
        return false;
    }

    ZeroMemory(titlebar, sizeof(titlebar));
    
    // OpenGL Init
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
    glClearColor(0.1f, 0.2f, 0.3f, 0.5f);               // Black Background
    glClearDepth(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
    
    OnResize();

    return true;
}

bool CGame::OnUpdate()
{
    m_tStartTime = std::chrono::steady_clock::now();

    // start of rendering
    
    rquad += m_tDeltaTime * 90.0f;
    rtri += m_tDeltaTime * 90.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	    // Clear Screen And Depth Buffer
    glLoadIdentity();									    // Reset The Current Modelview Matrix
    glTranslatef(-1.5f, 0.0f, -6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
    glRotatef(rtri, 0.0f, 1.0f, 0.0f);						// Rotate The Triangle On The Y axis ( NEW )
    glBegin(GL_TRIANGLES);								    // Start Drawing A Triangle
        glColor3f(1.0f, 0.0f, 0.0f);						// Red
        glVertex3f(0.0f, 1.0f, 0.0f);					    // Top Of Triangle (Front)
        glColor3f(0.0f, 1.0f, 0.0f);						// Green
        glVertex3f(-1.0f, -1.0f, 1.0f);					    // Left Of Triangle (Front)
        glColor3f(0.0f, 0.0f, 1.0f);						// Blue
        glVertex3f(1.0f, -1.0f, 1.0f);	    				// Right Of Triangle (Front)
        glColor3f(1.0f, 0.0f, 0.0f);		   				// Red
        glVertex3f(0.0f, 1.0f, 0.0f);			    		// Top Of Triangle (Right)
        glColor3f(0.0f, 0.0f, 1.0f);						// Blue
        glVertex3f(1.0f, -1.0f, 1.0f);				    	// Left Of Triangle (Right)
        glColor3f(0.0f, 1.0f, 0.0f);						// Green
        glVertex3f(1.0f, -1.0f, -1.0f);	    				// Right Of Triangle (Right)
        glColor3f(1.0f, 0.0f, 0.0f);						// Red
        glVertex3f(0.0f, 1.0f, 0.0f);		    			// Top Of Triangle (Back)
        glColor3f(0.0f, 1.0f, 0.0f);						// Green
        glVertex3f(1.0f, -1.0f, -1.0f);			    		// Left Of Triangle (Back)
        glColor3f(0.0f, 0.0f, 1.0f);						// Blue
        glVertex3f(-1.0f, -1.0f, -1.0f);					// Right Of Triangle (Back)
        glColor3f(1.0f, 0.0f, 0.0f);						// Red
        glVertex3f(0.0f, 1.0f, 0.0f);				    	// Top Of Triangle (Left)
        glColor3f(0.0f, 0.0f, 1.0f);						// Blue
        glVertex3f(-1.0f, -1.0f, -1.0f);					// Left Of Triangle (Left)
        glColor3f(0.0f, 1.0f, 0.0f);						// Green
        glVertex3f(-1.0f, -1.0f, 1.0f);					    // Right Of Triangle (Left)
    glEnd();											    // Done Drawing The Pyramid

    glLoadIdentity();									    // Reset The Current Modelview Matrix
    glTranslatef(1.5f, 0.0f, -7.0f);					    // Move Right 1.5 Units And Into The Screen 7.0
    glRotatef(rquad, 1.0f, 1.0f, 1.0f);					    // Rotate The Quad On The X axis ( NEW )
    glBegin(GL_QUADS);									    // Draw A Quad
        glColor3f(0.0f, 1.0f, 0.0f);					  	// Set The Color To Green
        glVertex3f(1.0f, 1.0f, -1.0f);					    // Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, -1.0f);					    // Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);					    // Bottom Left Of The Quad (Top)
        glVertex3f(1.0f, 1.0f, 1.0f);					    // Bottom Right Of The Quad (Top)
        glColor3f(1.0f, 0.5f, 0.0f);					   	// Set The Color To Orange
        glVertex3f(1.0f, -1.0f, 1.0f);					    // Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f, -1.0f, 1.0f);					    // Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f, -1.0f, -1.0f);				   	// Bottom Left Of The Quad (Bottom)
        glVertex3f(1.0f, -1.0f, -1.0f);					    // Bottom Right Of The Quad (Bottom)
        glColor3f(1.0f, 0.0f, 0.0f);				    	// Set The Color To Red
        glVertex3f(1.0f, 1.0f, 1.0f);					    // Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);					    // Top Left Of The Quad (Front)
        glVertex3f(-1.0f, -1.0f, 1.0f);					    // Bottom Left Of The Quad (Front)
        glVertex3f(1.0f, -1.0f, 1.0f);					    // Bottom Right Of The Quad (Front)
        glColor3f(1.0f, 1.0f, 0.0f);				    	// Set The Color To Yellow
        glVertex3f(1.0f, -1.0f, -1.0f);					    // Top Right Of The Quad (Back)
        glVertex3f(-1.0f, -1.0f, -1.0f);			    	// Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f, -1.0f);					    // Bottom Left Of The Quad (Back)
        glVertex3f(1.0f, 1.0f, -1.0f);					    // Bottom Right Of The Quad (Back)
        glColor3f(0.0f, 0.0f, 1.0f);				    	// Set The Color To Blue
        glVertex3f(-1.0f, 1.0f, 1.0f);					    // Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f, -1.0f);					    // Top Left Of The Quad (Left)
        glVertex3f(-1.0f, -1.0f, -1.0f);			    	// Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f, -1.0f, 1.0f);					    // Bottom Right Of The Quad (Left)
        glColor3f(1.0f, 0.0f, 1.0f);				    	// Set The Color To Violet
        glVertex3f(1.0f, 1.0f, -1.0f);					    // Top Right Of The Quad (Right)
        glVertex3f(1.0f, 1.0f, 1.0f);					    // Top Left Of The Quad (Right)
        glVertex3f(1.0f, -1.0f, 1.0f);					    // Bottom Left Of The Quad (Right)
        glVertex3f(1.0f, -1.0f, -1.0f);					    // Bottom Right Of The Quad (Right)
    glEnd(); // Done Drawing The Quad 

    //Sleep(100);

    SwapBuffers(m_hdc); // swap the buffers to backbuffer

    // end of rendering
    m_tEndTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> dt = m_tEndTime - m_tStartTime;
    m_tDeltaTime = dt.count();

    m_tFrameCounter++;
   
    StringCchPrintf(titlebar, sizeof(titlebar), TEXT("fortnite balls, fps: %.2f, frame %u"), 1.0/m_tDeltaTime, m_tFrameCounter);
    SetWindowText(m_hwnd, titlebar);


    return true;
}

void CGame::OnClose()
{
    if (m_rc)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_rc);
        m_rc = NULL;
    }

    if (m_hdc)
    {
        ReleaseDC(m_hwnd, m_hdc);
        m_hdc = NULL;
    }
}

void CGame::OnResize()
{
    if (!m_rc || !m_hdc) // safeguard
        return; 

    RECT rc;
    GetClientRect(m_hwnd, &rc);
    //printf("width %i, height %i\n",rc.right-rc.left,rc.bottom-rc.top);

    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    if (height == 0) // prevent divide by 0
        height = 1;


    glViewport(0, 0, width, height);
    
    // select and reset the projection matrix
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();

    // Calculate the aspect ratio of the window
    perspectiveGL(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                         // Select the modelview matrix
    glLoadIdentity();                                   // Reset the modelview matrix
}

void CGame::OnKeyDown(WPARAM key)
{
    if (key == VK_ESCAPE)
        DestroyWindow(m_hwnd);
}

void CGame::OnKeyUp(WPARAM key)
{
}

void CGame::perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    // Replaces gluPerspective. Sets the frustum to perspective mode.
    // fovY     - Field of vision in degrees in the y direction
    // aspect   - Aspect ratio of the viewport
    // zNear    - The near clipping distance
    // zFar     - The far clipping distance


    const GLdouble pi = 3.1415926535897932384626433832795; 
    GLdouble fW, fH; // Half of the size of the x and y clipping planes.
    
    // Calculate the distance from 0 of the y clipping plane. Basically trig to calculate position of clipper at zNear.
    // Note: tan(double) uses radians but OpenGL works in degrees so we convert degrees to radians by dividing by 360 then multiplying by pi.
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}
