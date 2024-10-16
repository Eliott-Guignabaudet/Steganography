#include "framework.h"

#include <objidl.h>

#include <windows.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;




#pragma once



class ImageLoader
{
public:

	ImageLoader();
	~ImageLoader();

	void ConvertToBmp(const WCHAR* pngFilePath, const WCHAR* bmpFilePath);

	Bitmap GetPictureToDisplay();

protected:

	Bitmap bmpImage;

};


