#include <iostream>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include "SteganoAlgoLSB.h"

void SteganoAlgoLSB::HideMessage(Gdiplus::Bitmap& bmp, std::string message)
{
	m_bmp = &bmp; 
	ParseImage();
	BYTE first = (*m_imageBytesRGB)[0];
	BYTE test = 00001001;
	const char* c = message.c_str();
	bool testcheck = (test >> 1);
	test &= ~(1 << 0);

	std::cout << "Hide message LSB" << std::endl;
}

std::string SteganoAlgoLSB::FindMessage()
{
	return std::string();
}

void SteganoAlgoLSB::ParseImage()
{
	if (m_imageBytesRGB != nullptr)
	{
		delete m_imageBytesRGB;
		m_imageBytesRGB = nullptr;
	}
	if (m_imageBytesA != nullptr)
	{
		delete m_imageBytesA;
		m_imageBytesA = nullptr;
	}

	UINT h, w;
	std::vector<BYTE>* result = new std::vector<BYTE>();
	std::vector<BYTE>* bytesA = new std::vector<BYTE>();
	h = m_bmp->GetHeight();
	w = m_bmp->GetWidth();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Gdiplus::Color color;
			m_bmp->GetPixel(i, j, &color);
			result->push_back(color.GetR());
			result->push_back(color.GetG());
			result->push_back(color.GetB());
		}
	}
	
	m_imageBytesRGB = result;
}

void SteganoAlgoLSB::UnparseImage()
{
}
