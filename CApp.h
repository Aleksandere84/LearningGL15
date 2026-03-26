#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CApp
{
public:
	bool Init(int width, int height, LPCTSTR title);
	bool Update();
	int  Release();
	void HandleError(LPCTSTR error);
	virtual void OnKeyDown(WPARAM key);
	virtual void OnKeyUp(WPARAM key);

	virtual bool OnCreate() { return true; }
	virtual bool OnUpdate() { return true; }
	virtual void OnClose() {}
	virtual void OnResize() {}

public:
	HWND m_hwnd;

private:
	MSG msg;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

