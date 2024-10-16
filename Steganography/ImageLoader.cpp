#include "ImageLoader.h"

#include "framework.h"

#include <objidl.h>

#include <windows.h>
#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;



ImageLoader::ImageLoader() // Constructeur
{
}

ImageLoader::~ImageLoader() // Destructeur
{
}



void ImageLoader::ConvertToBmp(const WCHAR* pngFilePath, const WCHAR* bmpFilePath) // recopie une image sur une bitmap
{

    // Charger l'image PNG
    Image refImage(pngFilePath);
    if (refImage.GetLastStatus() != Ok) {
        std::cerr << "Erreur de chargement de l'image" << std::endl;
    }

    if (bmpImage != nullptr)
    {
        delete bmpImage;
        bmpImage = nullptr;
    }

    // deffinit la resolution de notre bitmap a celle de l'image choisit
    bmpImage = new Bitmap(pngFilePath);
   
    bmpImage->SetResolution(refImage.GetWidth(), refImage.GetHeight());

    // Dessiner l'image PNG sur le bitmap
    Graphics graphics(bmpImage);
    graphics.DrawImage(&refImage, 0, 0);

    // Enregistrer le bitmap au format BMP
    CLSID bmpClsid;
    GetEncoderClsid(L"image/bmp", &bmpClsid);
    bmpImage->Save(bmpFilePath, &bmpClsid, nullptr);

}


int ImageLoader::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{

    UINT num = 0;          // nombre d'encodeurs
    UINT size = 0;        // taille de l'encodeur

    Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) return -1; // erreur

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j) {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j; // retourne l'index de l'encodeur
        }
    }

    free(pImageCodecInfo);
    return -1; // échec

}

Bitmap* ImageLoader::GetPictureToDisplay() // Donne l'image a afficher
{
    if (bmpImage == nullptr)
    {
        return bmpImage;
    }
    if(bmpImage->GetWidth() == 0 || bmpImage->GetHeight() == 0)
    {
        std::cerr << "L'image est vide" << std::endl;
        return nullptr;
    }


    return bmpImage;
}