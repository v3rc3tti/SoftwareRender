#include <Windows.h>
#include "Win32PixelWindow.h"

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    PixelWindow* pWindow = new Win32PixelWindow();
    pWindow->initialize(400, 200, L"TestWindow");
    while (true) {
        if (!pWindow->procSysMsgs()) {
            break;
        }
        pWindow->clearColor(255, 0, 0);
        pWindow->redraw();
        Sleep(10);
    }
    pWindow->shutdown();
}