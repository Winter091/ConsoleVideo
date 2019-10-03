#include "ImageDrawer.h"
#include "Timer.h"

int main()
{
	//              w    h
	ImageDrawer id(240, 115);

	// Loading images one by one
	for (int i = 100; i <= 504; i++)
	{
		std::string path = "res\\144p-video\\video";
		path += std::to_string(i) += ".bmp";
		id.loadBMP(path.c_str());
	}

	id.setCursorVisibility(false);
	id.setPixelSize(4);
	
	while (true)
		id.drawImageSequence();
}
