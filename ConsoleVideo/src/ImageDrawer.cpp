#include "ImageDrawer.h"
#include "BMP.h"
#include <string>

ImageDrawer::ImageDrawer(int cols, int rows)
{
	this->cols = cols;
	this->rows = rows;

	this->hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
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
		color |= (r > 64 ? FOREGROUND_RED   : 0);
		color |= (g > 64 ? FOREGROUND_GREEN : 0);
		color |= (b > 64 ? FOREGROUND_BLUE  : 0);

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

/*
int ImageDrawer::loadBMP(const char* filepath)
{
	FILE* f = fopen(filepath, "rb");
	if (!f) perror("fopen");
	if (!f) return 0;

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	int width = *(int*)& info[18];
	int height = *(int*)& info[22];

	//if (width != this->cols || height != this->rows)
		//return 0;

	// Loading raw data
	int row_padded = (width * 3 + 3) & (~3);
	std::vector<unsigned char*> rows;
	unsigned char* currentRow = new unsigned char[row_padded];
	unsigned char temp;

	for (int i = 0; i < height; i++)
	{
		fread(currentRow, sizeof(unsigned char), row_padded, f);
		//for (int j = 0; j < width * 3; j += 3)
		//{
			// Convert (B, G, R) to (R, G, B)
			//temp = currentRow[j];
			//currentRow[j] = currentRow[j + 2];
			//currentRow[j + 2] = temp;
		//}
		rows.push_back(currentRow);
	}

	fclose(f);

	// Color translation (B, G, R)
	for (auto it = rows.begin(); it != rows.end(); it++)
	{
		for (int i = 0; i < width * 3; i += 3)
		{
			CHAR_INFO curr;

			float pixel[3] = { (int)(*it)[i + 2] / 255.0f, (int)(*it)[i + 1] / 255.0f, (int)(*it)[i] / 255.0f };
			unsigned char pix[3] = { *(it)[i + 2], *(it)[i + 1], *(it)[i + 0] };

			curr.Attributes = (*(it))[i] + 2;
			curr.Char.UnicodeChar = L'\x2588';
			this->data.push_back(curr);
		}
	}
}
*/

CHAR_INFO& ImageDrawer::getCell(int x, int y, int index)
{
	if (x < 0 || x >= cols || y < 0 || y >= rows)
	{
		__debugbreak();
	}
	else if (index < 0 || index >= framesCount)
	{
		__debugbreak();
	}
	return this->data.at(index).at(y * cols + x);
}

void ImageDrawer::drawSingleImage()
{
	int success = WriteConsoleOutputW(
		this->hHandle,
		&this->data.at(0).at(0),
		{ (short)cols, (short)rows },
		{ 0, 0 },
		&this->writeRegion
	);
	if (!success)
	{
		__debugbreak();
	}
}

void ImageDrawer::drawImageSequence()
{
	int success = WriteConsoleOutputW(
		this->hHandle,
		&this->data.at(currentFrame).at(0),
		{ (short)cols, (short)rows },
		{ 0, 0 },
		&this->writeRegion
	);
	if (!success)
	{
		__debugbreak();
	}

	currentFrame++;
	if (currentFrame == framesCount)
		currentFrame = 0;
}

