#pragma once
#include <vector>
#include "ASteganoAlgo.h"

typedef unsigned char       BYTE;

class SteganoAlgoLSB :
    public ASteganoAlgo
{
public:
    friend class Gdiplus::Bitmap;
    void HideMessage(Gdiplus::Bitmap& bmp, const std::string& message) override;
    bool FindMessage(Gdiplus::Bitmap& bmp, std::string& message) override;
protected:

    Gdiplus::Bitmap* m_bmp;
    std::vector<BYTE>* m_imageBytesRGB;
    std::vector<BYTE>* m_imageBytesA;
    void ParseImage() override;
    void UnparseImage() override;
    void LSBAlgo(const char* message);
    void LSBAlgoReverse(std::string& message);
};

