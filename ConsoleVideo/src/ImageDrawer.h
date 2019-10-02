#pragma once

#include <Windows.h>
#include <vector>
#include <string>


class ImageDrawer
{
private:
	int cols, rows;
	std::vector<std::vector<CHAR_INFO>> data;
	int framesCount = 0;
	int currentFrame = 0;
	HANDLE hHandle;
	SMALL_RECT writeRegion;

public:
	ImageDrawer(int cols, int rows);

	void loadBMP(const char* filepath);
	CHAR_INFO& getCell(int x, int y, int index);

	void drawSingleImage();
	void drawImageSequence();
};
