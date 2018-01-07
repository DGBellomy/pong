#include "pch.h"
#include "Win32App.h"
#include <random>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

HWND Win32App::m_hWnd = nullptr;
int Win32App::m_MonitorWidth = GetSystemMetrics(SM_CXSCREEN);
int Win32App::m_MonitorHeight = GetSystemMetrics(SM_CYSCREEN);
Pong* Win32App::m_Pong;

HBRUSH CLEAR_COLOR = (HBRUSH)GetStockObject(BLACK_BRUSH);

LPWSTR title = L"TESTING => ";
int screen_width = 800;
int screen_height = 600;
bool show_cursor = true;

double seconds_per_count = 0.0;
__int64 current_time = 0;
__int64 prev_time = 0;
double delta_time = 0;

double elapsed_time = 0.0;
double draw_rate = 0.0;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void setupTimer()
{
    __int64 counts_per_second = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_second);
    seconds_per_count = 1.0 / (double)counts_per_second;
    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    prev_time = current_time;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int Win32App::Run(Pong* pong, HINSTANCE hInstance, int showCmd)
{
    m_Pong = pong;

    if (!InitWindow(hInstance, showCmd, title))
    {
        return -1;
    }

    m_Pong->reset(m_hWnd);

    MSG msg = { 0 };

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Game Logic
        if (!show_cursor)  // Lock Mouse Pos
            SetCursorPos(m_MonitorWidth / 2, m_MonitorHeight / 2);

        QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
        delta_time = (double)(current_time - prev_time) * seconds_per_count;

        elapsed_time += delta_time;
        draw_rate += delta_time;

        m_Pong->Update();

        // Draw
        HDC hdc = GetDC(m_hWnd);

        // Clear Screen
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, CLEAR_COLOR);
        Rectangle(hdc, 0, 0, screen_width, screen_height);
        SelectObject(hdc, oldBrush);

        m_Pong->Draw(hdc);

        std::wstring text = L"FPS: " + std::to_wstring(1.0 / delta_time);
        TextOut(hdc, 10, screen_height - 70, text.c_str(), text.size());

        SwapBuffers(hdc);
        ReleaseDC(m_hWnd, hdc);
        //InvalidateRect(m_hWnd, 0, true);
        draw_rate = 0.0;

        prev_time = current_time;
    }

    return static_cast<char>(msg.wParam);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PROTECTED METHODS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

LRESULT Win32App::WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (msg)
    {
    case WM_CREATE:

        // initialization
        setupTimer();

        return 0;

    // KEYBOARD INPUT
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;

        default:
            m_Pong->KeyboardInput(wParam);
        }
        return 0;

    // MOUSE INPUT
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        // MouseButtonDown
        return 0;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        // MouseButtonUp
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(window, msg, wParam, lParam);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool Win32App::InitWindow(HINSTANCE hInstance, int showCmd, LPWSTR title)
{
    LPWSTR name = L"Win32App";

    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = name;
    
    RegisterClass(&wc);

    int top = (m_MonitorHeight - screen_height) / 2;
    int left = (m_MonitorWidth - screen_width) / 2;

    m_hWnd = CreateWindow(name, title, WS_OVERLAPPED,
        left, top,     // window (left, top) start position
        screen_width, screen_height, // window (width, height) Handle by Game
        0,        // parent window id
        0,        // menu id
        hInstance, 0);
    
    if (m_hWnd == 0)
    {
        MessageBox(0, L"", L"", MB_OK);
        return false;
    }

    ShowWindow(m_hWnd, showCmd);
    UpdateWindow(m_hWnd);
    ShowCursor(show_cursor);

    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -