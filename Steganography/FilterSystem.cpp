#include "FilterSystem.h"
#include "UIEventSystem.h"
#include "Resource.h"
#include "ImageLoader.h"

using namespace std::placeholders;

void FilterSystem::Init(HWND hwnd, ImageLoader* imgLoader)
{
    m_mainWnd = hwnd;
    m_imgLoader = imgLoader;
    m_currentFilter = Filter::None;
    m_filteredBitmap = nullptr;
    RegisterForEvents();
}

Bitmap* FilterSystem::GetFilteredBitmap()
{
    return m_filteredBitmap;
}

Filter FilterSystem::GetFilter()
{
    return m_currentFilter;
}

void FilterSystem::SetFilter(Filter filter)
{
    m_currentFilter = filter;
    Bitmap* bmp = m_imgLoader->GetPictureToDisplay();
    if (bmp   == nullptr)
    {
        return;
    }
    m_filteredBitmap = bmp->Clone(0,0, bmp->GetWidth(), bmp->GetHeight(), PixelFormat32bppARGB);
    switch (filter)
    {
    case Filter::None:
        break;
    case Filter::BlanckAndWhite:
        SetBlackAndWhiteFilter();
        break;
    case Filter::Reverse:
        SetReverseFilter();
        break;
    default:
        break;
    }

    InvalidateRect(m_mainWnd, NULL, NULL);
}

void FilterSystem::RegisterForEvents()
{
    auto bind = std::bind(
        &FilterSystem::HandleCommandEvent, this, _1
    );
    OnCommand = bind;
    UIEventSystem::GetInstance()->RegisterFoMessage(WM_COMMAND, OnCommand);
}

void FilterSystem::SetBlackAndWhiteFilter()
{
    for (unsigned int i = 0; i < m_filteredBitmap->GetHeight(); i++)
    {
        for (unsigned int j = 0; j < m_filteredBitmap->GetWidth(); j++)
        {
            Color color;
            m_filteredBitmap->GetPixel(j, i, &color);
            float r, g, b, c;
            r = (float)color.GetR();
            g = (float)color.GetG();
            b = (float)color.GetB();
            c = (r + g + b) / 3;
            Color newColor(c, c, c);
            m_filteredBitmap->SetPixel(j, i, newColor);
        }
    }
}

void FilterSystem::SetReverseFilter()
{
    for (int i = 0; i < m_filteredBitmap->GetHeight(); i++)
    {
        for (int j = 0; j < m_filteredBitmap->GetWidth(); j++)
        {
            Color color;
            m_filteredBitmap->GetPixel(j, i, &color);
            BYTE r, g, b;
            r = ~color.GetR();
            g = ~color.GetG();
            b = ~color.GetB();
            
            Color newColor(r, g, b);

            m_filteredBitmap->SetPixel(j, i, newColor);
        }
    }
}

LRESULT FilterSystem::HandleCommandEvent(EventParams params)
{
    int wmId = LOWORD(params.wParam);
    switch (wmId)
    {
    case ID_FILTRES_AUCUN:
        SetFilter(Filter::None);
        break;
    case ID_FILTRES_NOIRETBLANC:
        SetFilter(Filter::BlanckAndWhite);
        break;

    case ID_FILTRES_INVERES32792:
        SetFilter(Filter::Reverse);
        break;

    default:
        break;
    }

    return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);
}
