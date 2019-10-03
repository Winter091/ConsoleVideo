#pragma once

#include <Windows.h>
#include <vector>
#include <string>


class ImageDrawer
{
private:
	std::vector<std::vector<CHAR_INFO>> data;
	HANDLE hConsole;
	SMALL_RECT writeRegion;
	int cols, rows;
	int framesCount = 0;
	int currentFrame = 0;

	CHAR_INFO& getCell(int x, int y, int index);

public:
	ImageDrawer(int cols, int rows);

	void loadBMP(const char* filepath);

	void setCursorVisibility(int mode);
	void setPixelSize(int size);

	void drawSingleImage();
	void drawImageSequence();
};
