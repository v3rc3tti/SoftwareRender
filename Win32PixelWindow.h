#pragma once

#include <Windows.h>
#include "PixelWindow.h"

class Win32PixelWindow: public PixelWindow
{
public:
	bool initialize(uint32_t winWidth, uint32_t winHeight, uint32_t pixDim, std::wstring title) override;
	bool shutdown() override;
	void redraw() override;
	void clearColor(uint8_t r, uint8_t g, uint8_t b) override;
	void drawPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) override;
	void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t r, uint8_t g, uint8_t b) override;
	bool procSysMsgs() override;
private:
	HWND m_hwnd;
	BITMAPINFO m_bmInfo;
	uint32_t m_winWidth;
	uint32_t m_winHeight;
	uint32_t m_pixelDim;
	uint8_t* m_screenBits;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

