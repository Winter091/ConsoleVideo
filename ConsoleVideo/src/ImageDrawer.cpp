#include <string>

#include "ImageDrawer.h"
#include "Exceptions.h"
#include "BMP.h"

CHAR_INFO& ImageDrawer::getCell(int x, int y, int index)
{
	if (x < 0 || x >= cols || y < 0 || y >= rows)
	{
		throw ConsoleVideoException("getCell(): index is out of bounds");
	}
	else if (index < 0 || index >= framesCount)
	{
		throw ConsoleVideoException("getCell(): this frame does not exist");
	}
	return this->data.at(index).at(y * cols + x);
}

ImageDrawer::ImageDrawer(int cols, int rows)
{
	this->cols = cols;
	this->rows = rows;

	this->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->writeRegion = { 0, 0, (short)cols, (short)rows };
}

void ImageDrawer::loadBMP(const char* filepath)
{
	BMP image(filepath);

	framesCount++;

	int width = image.bmp_info_header.width;
	int height = image.bmp_info_header.height;

	std::vector<CHAR_INFO> currentImage;
	currentImage.clear();
	
	// Get pixels, but in mirrored order
	for (int i = 0; i < width * height * 3; i += 3)
	{
		int r = (int)image.data.at(i + 2);
		int g = (int)image.data.at(i + 1);
		int b = (int)image.data.at(i);

		int color = (r > 128 | g > 128 | b > 128) ? FOREGROUND_INTENSITY : 0;
		color |= (r > 80 ? FOREGROUND_RED   : 0);
		color |= (g > 80 ? FOREGROUND_GREEN : 0);
		color |= (b > 80 ? FOREGROUND_BLUE  : 0);

		wchar_t character;
		float brightness = (0.2126 * r + 0.7152 * g + 0.0722 * b); // 0 to 255 

		if      (brightness < 32) character = L'\x2591';
		else if (brightness < 64) character = L'\x2592';
		else if (brightness < 128) character = L'\x2593';
		else  character = L'\x2588';

		CHAR_INFO pixel;
		pixel.Char.UnicodeChar = character;
		pixel.Attributes = color;

		currentImage.push_back(pixel);
	}

	this->data.push_back(currentImage);

	// Mirror pixels horisontally
	for (int i = 0; i < width; i++) 
		for (int j = 0; j < height / 2; j++) 
			std::swap(this->getCell(i, j, framesCount - 1), this->getCell(i, height - 1 - j, framesCount - 1));

}

void ImageDrawer::setCursorVisibility(int mode)
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(this->hConsole, &info);
	info.bVisible = mode;
	SetConsoleCursorInfo(this->hConsole, &info);
}

void ImageDrawer::setPixelSize(int size)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = size;   // Width of each character in the font
	cfi.dwFontSize.Y = size;   // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(this->hConsole, FALSE, &cfi);
}

void ImageDrawer::drawSingleImage()
{
	int success = WriteConsoleOutputW(
		this->hConsole,
		&this->data.at(0).at(0),
		{ (short)cols, (short)rows },
		{ 0, 0 },
		&this->writeRegion
	);

	if (!success)
		throw ConsoleVideoException("Unsuccessful image output in drawSingleImage()");
}

void ImageDrawer::drawImageSequence()
{
	int success = WriteConsoleOutputW(
		this->hConsole,
		&this->data.at(currentFrame).at(0),
		{ (short)cols, (short)rows },
		{ 0, 0 },
		&this->writeRegion
	);

	if (!success)
		throw ConsoleVideoException("Unsuccessful image output in drawImageSequence()");

	currentFrame++;
	if (currentFrame == framesCount)
		currentFrame = 0;
}

