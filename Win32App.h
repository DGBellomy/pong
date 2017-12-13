#pragma once

#include "Pong.h"

class Win32App
{
public:

	static int Run(Pong* pong, HINSTANCE hInstance, int showCmd);
	static HWND GetHWND() { return m_hWnd; }

protected:

	static LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool InitWindow(HINSTANCE hInstance, int showCmd, LPWSTR title);

private:

	static HWND m_hWnd;
	static int m_MonitorWidth;
	static int m_MonitorHeight;
	static Pong* m_Pong;
};
