// Steganography.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include <windows.h>
#include <commdlg.h>
#include <wchar.h>
#include <stdlib.h>
#include "Steganography.h"
#include "SteganoSystem.h"
#include "ImageLoader.h"
#include "CLSIDEncoder.h"
#include "shlobj_core.h"
#include "string"
#include "MainWindow.h"

#define MAX_LOADSTRING 100

//Boutons de l'interface
#define OPEN_FILE_BUTTON 1
#define HIDE_MESSAGE_BUTTON 2
#define EXTRACT_MESSAGE_BUTTON 3



// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    // TODO: Placez le code ici.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    MainWindow mainWin;
    LoadStringW(hInstance, IDS_APP_TITLE, mainWin.m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STEGANOGRAPHY, mainWin.m_szWindowClass, MAX_LOADSTRING);

    UIEventSystem::GetInstance();
    return mainWin.Run(hInstance, nCmdShow);
}
