#pragma once

#include <Windows.h>
#include "PixelWindow.h"

class Win32PixelWindow: public PixelWindow
{
public:
	bool initialize(uint32_t winWidth, uint32_t winHeight, std::wstring title) override;
	bool shutdown() override;
	void redraw() override;
	void clearColor(uint8_t r, uint8_t g, uint8_t b) override;
	bool procSysMsgs();
private:
	HWND m_hwnd;
	BITMAPINFO m_bmInfo;
	uint32_t m_winWidth;
	uint32_t m_winHeight;
	uint8_t* m_screenBits;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

