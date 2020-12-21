#include <Windows.h>
#include "Win32PixelWindow.h"

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    PixelWindow* pWindow = new Win32PixelWindow();
    pWindow->initialize(320, 200, 3, L"TestWindow");
    while (true) {
        if (!pWindow->procSysMsgs()) {
            break;
        }
        pWindow->clearColor(246, 1, 157);
        pWindow->drawLine(0, 0, 319, 199, 3, 94, 232);
        pWindow->drawLine(0, 199, 319, 0, 151, 0, 204);
        pWindow->redraw();
        Sleep(10);
    }
    pWindow->shutdown();
}