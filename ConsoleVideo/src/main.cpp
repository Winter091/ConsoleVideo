#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <windows.h>

#include <fcntl.h>
#include <io.h>
using namespace std;

#include "ImageDrawer.h"
#include "Timer.h"

void setConsoleCursorVisibility(bool state)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	info.bVisible = state;
	SetConsoleCursorInfo(hOut, &info);
}

void setConsoleFontSize(int size)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = size;                   // Width of each character in the font
	cfi.dwFontSize.Y = size;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void printStringAttrW(const HANDLE& hHandle, PCHAR_INFO buf, COORD bufSize, SMALL_RECT rect)
{
	int success = WriteConsoleOutputW(
		hHandle,
		buf,
		bufSize,
		{ 0, 0 },
		&rect
	);
	if (!success)
	{
		cout << "Err!\n";
	}
}

/*
int main()
{
	//setConsoleFontSize();
	_setmode(_fileno(stdout), _O_U16TEXT);

	HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CHAR_INFO buffer[10];
	for (int i = 0; i < 10; i++)
	{
		buffer[i].Char.UnicodeChar = L'\x2776';
		buffer[i].Attributes = 0x0002;
	}

	printStringAttrW(
		hHandle,
		buffer,
		{ 5, 2 },
		{10, 10, 100, 100}
	);
}
*/


int main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	setConsoleCursorVisibility(false);
	setConsoleFontSize(2);

	ImageDrawer id1(480, 230);

	for (int i = 592; i <= 672; i++)
	{
		string path = "res\\video1-bmp\\video_0";
		path += to_string(i);
		path += ".bmp";
		id1.loadBMP(path.c_str());

	}

	Timer t;
	while (true)
		id1.drawImageSequence();
}
