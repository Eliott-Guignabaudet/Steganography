#include <iostream>
#include <string>
#include "CLSIDEncoder.h"

#include <objidl.h>
#include <gdiplus.h>
#include "SteganoAlgoLSB.h"





void SteganoAlgoLSB::HideMessage(Gdiplus::Bitmap& bmp, const std::string& message)
{
	m_bmp = &bmp; 
	ParseImage();
	LSBAlgo(message.c_str());
	UnparseImage();
	CLSID clsidPng;
	//CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &guidPng);
	CLSIDEncoder::GetEncoderClsid(L"image/png", &clsidPng);
	m_bmp->Save(L"Images/FileChanged.png", &clsidPng, NULL);
	std::cout << "Hide message LSB" << std::endl;
}

bool SteganoAlgoLSB::FindMessage(Gdiplus::Bitmap& bmp, std::string& message)
{
	m_bmp = &bmp;
	ParseImage();
	LSBAlgoReverse(message);

	return message != "";
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
			bytesA->push_back(color.GetA());
			result->push_back(color.GetR());
			result->push_back(color.GetG());
			result->push_back(color.GetB());
		}
	}
	
	m_imageBytesRGB = result;
	m_imageBytesA = bytesA;
}

void SteganoAlgoLSB::UnparseImage()
{
	UINT h, w;
	h = m_bmp->GetHeight();
	w = m_bmp->GetWidth();
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int a = (i * w) + j;
			int r = ((i * w) + j) * 3;
			int g = ((i * w) + j) * 3 + 1;
			int b = ((i * w) + j) * 3 + 2;

			Gdiplus::Color color = Gdiplus::Color(
				(*m_imageBytesA)[a], 
				(*m_imageBytesRGB)[r], 
				(*m_imageBytesRGB)[g], 
				(*m_imageBytesRGB)[b]);
			m_bmp->SetPixel(i, j, color);
		}
	}
	Gdiplus::Color color;
	m_bmp->GetPixel(0, 0, &color);
	return;
}

void SteganoAlgoLSB::LSBAlgo(const char* message)
{
	int length = strlen(message);
	for (int i = 0; i < length; i++)
	{
		const char letter = message[i];
		for (int j = 0; j < 8; j++)
		{
			unsigned int index = i * 8 + j;
			bool isBitTrue = letter & (1 << j);
			if (isBitTrue)
			{
				(*m_imageBytesRGB)[index] |= 1 << 0;
			}
			else 
			{
				(*m_imageBytesRGB)[index] &= ~(1 << 0);
			}
		}
	}
	return;
}

void SteganoAlgoLSB::LSBAlgoReverse(std::string& message)
{
	message = "";
	for (int i = 0; (i + 8) < m_imageBytesRGB->size(); i+=8)
	{
		char letter = 0;
		for (int j = 0; j < 8; j++)
		{
			bool isBitTrue = (*m_imageBytesRGB)[i + j] & (1 << 0);
			if (isBitTrue)
			{
				letter |= 1 << j;
			}
			else
			{
				letter &= ~(1 << j);
			}
		}
		if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == ' ')
		{
			message += letter;
		}
	}
	return;
}