#include "CApp.h"

bool CApp::Init(int width, int height, LPCTSTR title)
{
    LPCTSTR classname = TEXT("GL15Class");

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = CApp::WndProc;
    wc.lpszClassName = classname;
    wc.hInstance = GetModuleHandle(NULL);

    if (!RegisterClass(&wc))
    {
        HandleError(TEXT("RegisterClass failed!"));
        return false;
    }
        
    // pass class in lparam for later lookup
    m_hwnd = CreateWindow(classname, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, wc.hInstance, this);   

    if (!m_hwnd)
    {
        HandleError(TEXT("m_hwnd is NULL!"));
        return false;
    }
        

    ShowWindow(m_hwnd, SW_SHOW);

    return OnCreate();   
}

bool CApp::Update()
{
    MSG msg = {};
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            OnClose();
            return false;   // signal the loop to exit
        } 
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return OnUpdate();
}

int CApp::Release()
{
    
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = NULL;
    }
    return msg.wParam;
}

void CApp::HandleError(LPCTSTR error)
{
    DWORD dw = GetLastError();
    MessageBox(NULL, error, TEXT("Error!"), MB_OK | MB_ICONERROR);
    LPVOID lpMsgBuf;
    

    if (FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL) == 0) 
    {
        MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("Fatal Error!"), MB_OK);
        ExitProcess(dw);
    }
        
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Details"), MB_OK);

    LocalFree(lpMsgBuf);
    ExitProcess(dw);
}

void CApp::OnKeyDown(WPARAM key)
{
}

void CApp::OnKeyUp(WPARAM key)
{
}

LRESULT CALLBACK CApp::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    CApp* pApp = (CApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg)
    {
    case WM_NCCREATE:
    {
        // store the class from CreateWindow
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lparam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA,(LONG_PTR)pCreate->lpCreateParams);
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    case WM_KEYDOWN:
        if (pApp) pApp->OnKeyDown(wparam);
        return 0;
    case WM_KEYUP:
        if (pApp) pApp->OnKeyUp(wparam);
        return 0;
    case WM_SIZE:
        if (pApp) pApp->OnResize();
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}