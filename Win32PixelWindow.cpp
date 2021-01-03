#include <cstdlib>
#include "Win32PixelWindow.h"

bool Win32PixelWindow::initialize(uint32_t winWidth, uint32_t winHeight, uint32_t pixDim, std::wstring title) {
    m_winWidth = winWidth;
    m_winHeight = winHeight;
    m_pixelDim = pixDim;

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
    windowRect.right = (long)winWidth*m_pixelDim;
    windowRect.bottom = (long)winHeight*m_pixelDim;

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

void Win32PixelWindow::redraw() {
    HDC hdc = GetDC(m_hwnd);
    HBITMAP hBitD = CreateCompatibleBitmap(hdc, m_winWidth*m_pixelDim, m_winHeight*m_pixelDim);
    HDC hDob = CreateCompatibleDC(hdc);
    HBITMAP oldBit = (HBITMAP)SelectObject(hDob, hBitD);
    StretchDIBits(hDob, 0, 0, m_winWidth*m_pixelDim, m_winHeight*m_pixelDim, 0, 0, m_winWidth, m_winHeight,
        m_screenBits, &m_bmInfo, DIB_RGB_COLORS, SRCCOPY);
    
    BitBlt(hdc, 0, 0, m_winWidth*m_pixelDim, m_winHeight*m_pixelDim, hDob, 0, 0, SRCCOPY);
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

void Win32PixelWindow::drawPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || x >= m_winWidth || y < 0 || y >= m_winHeight) {
        return;
    }
    uint32_t pos = 4 * (y * m_winWidth + x);
    m_screenBits[pos] = b;
    m_screenBits[pos + 1] = g;
    m_screenBits[pos + 2] = r;
}

void Win32PixelWindow::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t r, uint8_t g, uint8_t b) {

    int32_t dx = (x2 > x1) ? x2 - x1 : x1 - x2;
    int32_t dy = (y2 > y1) ? y2 - y1 : y1 - y2;
    if (dx < dy) {
        if (y1 > y2) {
            int32_t tmp = y1;
            y1 = y2;
            y2 = tmp;
            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        bool dir = (x1 < x2) ? true : false;
        int32_t slope = dx;
        int32_t err = 0;
        while (y1 <= y2) {
            err += slope;
            if (err >= dy) {
                err -= dy;
                if (dir) {
                    x1++;
                } else {
                    x1--;
                }
            }
            drawPixel(x1, y1, r, g, b);
            y1++;
        }
    }
    else {
        if (x1 > x2) {
            int32_t tmp = y1;
            y1 = y2;
            y2 = tmp;
            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        int32_t dir = (y1 < y2) ? true : false;
        int32_t slope = dy;
        int32_t err = 0;
        while (x1 <= x2) {
            err += slope;
            if (err >= dx) {
                err -= dx;
                if (dir) {
                    y1++;
                } else {
                    y1--;
                }
            }
            drawPixel(x1, y1, r, g, b);
            x1++;
        }
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