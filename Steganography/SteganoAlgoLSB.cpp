#include <iostream>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include "SteganoAlgoLSB.h"


HRESULT GetGdiplusEncoderClsid(const std::wstring& format, GUID* pGuid)
{
	HRESULT hr = S_OK;
	UINT  nEncoders = 0;          // number of image encoders
	UINT  nSize = 0;              // size of the image encoder array in bytes
	std::vector<BYTE> spData;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::Status status;
	bool found = false;

	if (format.empty() || !pGuid)
	{
		hr = E_INVALIDARG;
	}

	if (SUCCEEDED(hr))
	{
		*pGuid = GUID_NULL;
		status = Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

		if ((status != Gdiplus::Ok) || (nSize == 0))
		{
			hr = E_FAIL;
		}
	}

	if (SUCCEEDED(hr))
	{

		spData.resize(nSize);
		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)&spData.front();
		status = Gdiplus::GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

		if (status != Gdiplus::Ok)
		{
			hr = E_FAIL;
		}
	}

	if (SUCCEEDED(hr))
	{
		for (UINT j = 0; j < nEncoders && !found; j++)
		{
			if (pImageCodecInfo[j].MimeType == format)
			{
				*pGuid = pImageCodecInfo[j].Clsid;
				found = true;
			}
		}

		hr = found ? S_OK : E_FAIL;
	}

	return hr;
}



void SteganoAlgoLSB::HideMessage(Gdiplus::Bitmap& bmp, const std::string& message)
{
	m_bmp = &bmp; 
	ParseImage();
	LSBAlgo(message.c_str());
	UnparseImage();
	GUID guidPng = {};
	GetGdiplusEncoderClsid(L"image/png", &guidPng);
	m_bmp->Save(L"Images/FileChanged.png", &guidPng, NULL);
	std::cout << "Hide message LSB" << std::endl;
}

bool SteganoAlgoLSB::FindMessage(Gdiplus::Bitmap& bmp, std::string& message)
{
	m_bmp = &bmp;
	ParseImage();
	LSBAlgoReverse(message);


	return false;
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
	int bitModified = 0;
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
			bitModified++;
		}
	}
	return;
}

void SteganoAlgoLSB::LSBAlgoReverse(std::string& message)
{
	//for (int i = 0; i < m_imageBytesRGB; i++)
	//{

	//}
}