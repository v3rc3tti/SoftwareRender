#include "Win32PixelWindow.h"

bool Win32PixelWindow::initialize(uint32_t winWidth, uint32_t winHeight, std::wstring title) {
    m_winWidth = winWidth;
    m_winHeight = winHeight;

    m_screenBits = new uint8_t[winWidth * winHeight * 4];
    memset(m_screenBits, 0, winWidth*winHeight*4);

    m_bmInfo.bmiHeader.biSize = sizeof(m_bmInfo.bmiHeader);
    m_bmInfo.bmiHeader.biWidth = winWidth;
    m_bmInfo.bmiHeader.biHeight = -(long)winHeight;
    m_bmInfo.bmiHeader.biPlanes = 1;
    m_bmInfo.bmiHeader.biBitCount = 32;
    m_bmInfo.bmiHeader.biCompression = BI_RGB;

    WNDCLASS wc = { 0 };
    DWORD dwExStyle;
    DWORD dwStyle;

    RECT windowRect = { 0 };
    windowRect.right = (long)winWidth;
    windowRect.bottom = (long)winHeight;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"PixelWindowClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Register The Window class failed.", L"Initialization error",
            MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;

    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

    m_hwnd = CreateWindowEx(dwExStyle,
        wc.lpszClassName,
        title.c_str(),
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
        0, 0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        wc.hInstance,
        NULL);

    if (!m_hwnd) {
        MessageBox(NULL, L"Window creating failed.", L"Initialization error", MB_OK | MB_ICONERROR);
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    return true;
}

bool Win32PixelWindow::shutdown() {
    bool result = true;

    DestroyWindow(m_hwnd);
    m_hwnd = NULL;

    if (!UnregisterClass(L"PixelWindowClass", GetModuleHandle(NULL))) {
        MessageBox(NULL, L"Could not unregister class", L"Shutdown error", MB_OK | MB_ICONERROR);
        result = false;
    }

    delete[] m_screenBits;
    return result;
}

bool Win32PixelWindow::procSysMsgs() {
    MSG msg = { 0 };
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return true;
}

void Win32PixelWindow::redraw() {
    HDC hdc = GetDC(m_hwnd);
    HBITMAP hBitD = CreateCompatibleBitmap(hdc, m_winWidth, m_winHeight);
    HDC hDob = CreateCompatibleDC(hdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(hDob, hBitD);
    StretchDIBits(hDob, 0, 0, m_winWidth, m_winHeight, 0, 0, m_winWidth, m_winHeight,
        m_screenBits, &m_bmInfo, DIB_RGB_COLORS, SRCCOPY);
    
    BitBlt(hdc, 0, 0, m_winWidth, m_winHeight, hDob, 0, 0, SRCCOPY);
    SelectObject(hDob, oldBit);
    DeleteObject(hBitD);
    DeleteDC(hDob);
    ReleaseDC(m_hwnd, hdc);
}

void Win32PixelWindow::clearColor(uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t i = 0; i < m_winHeight * m_winWidth; i++) {
        m_screenBits[i * 4] = b;
        m_screenBits[i * 4 + 1] = g;
        m_screenBits[i * 4 + 2] = r;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE: {
            PostQuitMessage(0);
            return 0;
        }
        default: {
            break;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}