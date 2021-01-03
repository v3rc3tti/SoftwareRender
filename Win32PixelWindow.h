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
	void drawPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) override;
	void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t r, uint8_t g, uint8_t b) override;
	int getWidth() override { return m_winWidth; }
	int getHeight() override { return m_winHeight; }
private:
	HWND m_hwnd;
	BITMAPINFO m_bmInfo;
	uint32_t m_winWidth;
	uint32_t m_winHeight;
	uint32_t m_pixelDim;
	uint8_t* m_screenBits;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

