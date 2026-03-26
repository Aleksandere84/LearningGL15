#pragma once
#include "CApp.h"
#include <glad/glad.h>
#include <iostream>
#include <chrono>
#include <strsafe.h>


#define DeltaTime(timer) timer/1000.0f

class CGame : public CApp
{
public:
    bool OnCreate() override;
    bool OnUpdate() override;
    void OnClose()  override;
    void OnResize() override;

    void OnKeyDown(WPARAM key) override;
    void OnKeyUp(WPARAM key)   override;

private:
    void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    

private:
    // rendering
    HDC m_hdc;
    GLuint m_pf; // pixel format
    HGLRC m_rc; // opengl rendering context

    float rtri, rquad = 0.0f;
    // timing
    std::chrono::steady_clock::time_point m_tStartTime, m_tEndTime; // fps/deltatime counter
    double m_tDeltaTime;
    size_t m_tFrameCounter = 0;
    TCHAR titlebar[64];
    
};