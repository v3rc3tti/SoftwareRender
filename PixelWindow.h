#pragma once

#include <cstdint>
#include <string>

class PixelWindow {
public:
	virtual bool initialize(uint32_t winWidth, uint32_t winHeight, std::wstring title) = 0;
	virtual bool shutdown() = 0;
	virtual void redraw() = 0;
	virtual void clearColor(uint8_t r, uint8_t g, uint8_t b) = 0;
	virtual bool procSysMsgs() = 0;
};