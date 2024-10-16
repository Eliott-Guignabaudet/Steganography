#pragma once
#include <windows.h>

class CLSIDEncoder
{
public:
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};

