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

#define MAX_LOADSTRING 100

//Boutons de l'interface
#define OPEN_FILE_BUTTON 1
#define HIDE_MESSAGE_BUTTON 2
#define EXTRACT_MESSAGE_BUTTON 3



// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

//Déclaration d'un handle bitmap pour charger une image
HBITMAP hbitmap = NULL;

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
void                Export();
//New
/////
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// TODO: a retirer
HWND l_background;
HWND l_text;
HWND entry = 0;

ImageLoader* imgLoader;
WCHAR* imagePath;


const std::string MESSAGE_TO_HIDE =
"A long time ago, in a galaxy far, far, away... \
A vast sea of stars serves as the backdrop for the main \
title.War drums echo through the heavens as a rollup slowly \
crawls into infinity. \
It is a period of civil war.Rebel spaceships, striking from \
a hidden base, have won their first victory against the evil \
Galactic Empire. \
During the battle, Rebel spies managed to steal secret plans \
to the Empire's ultimate weapon, the Death Star, an armored \
space station with enough power to destroy an entire planet. \
Pursued by the Empire's sinister agents, Princess Leia races \
home aboard her starship, custodian of the stolen plans that \
can save her people and restore freedom to the galaxy... \
The awesome yellow planet of Tatooine emerges from a total \
eclipse, her two moons glowing against the darkness.A tiny \
silver spacecraft, a Rebel Blockade Runner firing lasers \
from the back of the ship, races through space.It is pursed \
by a giant Imperial Stardestroyer.Hundreds of deadly \
laserbolts streak from the Imperial Stardestroyer, causing \
the main solar fin of the Rebel craft to disintegrate.";


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
    imgLoader = new ImageLoader();

    
    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STEGANOGRAPHY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STEGANOGRAPHY));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STEGANOGRAPHY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STEGANOGRAPHY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW | CS_DROPSHADOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void AddButtons(HWND hWnd)
{
    //Tableau des boutons de l'interface
    HWND buttons[4];

    //Struct pour les paramètres du bouton
    struct buttonsetup
    {
        int s_x = 0; /*Position X du bouton*/
        int s_y = 0; /*Position Y du bouton*/
        int s_width = 0; /*Longueur du bouton*/
        int s_height = 0; /*Largeur du bouton*/
        const char* title = " "; /*Texte inscrit sur le bouton*/
        int buttonnum = 0; /*Numéro du bouton*/
    };

    buttonsetup buttonmap[4];
    buttonmap[0] = { 80, 10, 250, 40, "Importer un fichier", ID_FICHIER_IMPORTERUNFICHIER }; /*Charger un fichier bitmap*/
    buttonmap[1] = { 1125, 560, 210, 40, "Cacher et exporter le message", HIDE_MESSAGE_BUTTON }; /*Cacher le message*/
    buttonmap[2] = { 500, 560, 150, 40, "Extraire le message", EXTRACT_MESSAGE_BUTTON }; /*Extraire le message*/
    buttonmap[3] = { 80, 60, 250, 40, "Exporter un fichier", ID_FICHIER_EXPORTERUNFICHIER };

    for (int i = 0; i < 4; i++)
    {

        buttons[i] = CreateWindowA("Button", buttonmap[i].title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonmap[i].s_x, buttonmap[i].s_y, buttonmap[i].s_width, buttonmap[i].s_height, hWnd, (HMENU)buttonmap[i].buttonnum, NULL, NULL);

    }
    return;

}


BOOL UpdateInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Stocke le handle d'instance dans la variable globale

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//Permet d'ouvrir une fenêtre pour chercher une bitmap
int OpenFile(HWND hWnd)
{

    OPENFILENAMEA ofn;
    char file_name [700] = " ";

    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(file_name);
    ofn.lpstrFilter = "Bitmap Files\0*.BMP\0PNG\0*.png";  // Types de fichiers à filtrer
    ofn.nFilterIndex = 1;   // Index de départ des filtres (commence à 1)
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;  // Dossier initial (NULL pour le dossier actuel)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  // Options (doit exister)

    // Affiche la boîte de dialogue "Ouvrir"
    if (GetOpenFileNameA(&ofn) == TRUE) {
        
            const size_t file_nameSize = strlen(file_name) + 1;
            wchar_t* file_name_wc = new wchar_t[file_nameSize];
            
            size_t outSize;
            mbstowcs_s(&outSize, file_name_wc, file_nameSize, file_name, file_nameSize - 1);
            imagePath = file_name_wc;
            imgLoader->ConvertToBmp(file_name_wc, L"Images/bmptest.bmp");
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
            //SendMessage(hWnd, WM_PAINT, 0, 0);
        
    }
    else 
    {
        
    }

    return 0;
}

void ExtractMessage(HWND hWnd)
{
    Bitmap* bmp = imgLoader->GetPictureToDisplay();    
    if (bmp == nullptr)
    {
        MessageBoxW(hWnd, L"No picture loaded", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }
    std::string message = SteganoSystem::GetInstance()->FindMessage(*bmp);
    std::wstring widestr = std::wstring(message.begin(), message.end());
    SetWindowTextA(l_text, message.c_str());
    MessageBox(hWnd, widestr.c_str(), L"Test", 0);
    return;
}

void HideMessage(HWND hWnd)
{
    Bitmap* bmp = imgLoader->GetPictureToDisplay();
    if (bmp == nullptr)
    {
        MessageBox(hWnd, L"No picture loaded", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }


    int size = GetWindowTextLength(entry);
    LPWSTR message = (LPWSTR)VirtualAlloc((LPVOID)NULL,
        (DWORD)(size + 1), MEM_COMMIT,
        PAGE_READWRITE);
    GetWindowText(entry, message, size);
    


    std::wstring ws(message);
    std::string message_str(ws.begin(), ws.end());

    SteganoSystem::GetInstance()->HideMessage(*bmp, message_str);

    return;
}

void Export(HWND hWnd) {

    Bitmap* bmp = imgLoader->GetPictureToDisplay();

    if (bmp == nullptr) {
        MessageBox(hWnd, L"No picture loaded", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }

    char path[MAX_PATH] = " ";

    OPENFILENAMEW ofn;
    wchar_t szFile[MAX_PATH] = L""; // Chemin du fichier

    // Initialiser la structure
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // Taille de la structure
    ofn.hwndOwner = NULL;          // Pas de fenêtre propriétaire
    ofn.lpstrFilter = L"bmp\0*.bmp\0png\0*.png"; // Filtre des fichiers
    ofn.lpstrFile = szFile;        // Chemin du fichier
    ofn.nMaxFile = sizeof(szFile); // Taille maximale du chemin
    ofn.lpstrTitle = L"Enregistrer le fichier"; // Titre de la boîte de dialogue
    ofn.Flags = OFN_OVERWRITEPROMPT; // Demander confirmation pour écraser un fichier
    ofn.lpstrFileTitle;
    // Afficher la boîte de dialogue
    if (GetSaveFileNameW(&ofn)) {
        // Le fichier a été sélectionné et peut être enregistré
        MessageBoxW(NULL, szFile, L"Fichier enregistré", MB_OK);
    }
    else {
        // Gestion d'erreur, par exemple afficher un message d'erreur
        MessageBoxW(NULL, L"Erreur lors de l'enregistrement du fichier.", L"Erreur", MB_OK | MB_ICONERROR);
    }

    const wchar_t* filter = ofn.lpstrFilter;
    int filterIndex = ofn.nFilterIndex; // Index du filtre choisi (1 basé)

    // Avancer au bon filtre
    if (filterIndex % 2 != 0) {
        
    }
    else
    {
        filterIndex++;
    }

    for (int i = 1; i < filterIndex; i++) {
        while (*filter) {
            filter++; // Avancer jusqu'à la fin du nom du filtre
        }
        filter++; // Passer le caractère NULL
    }

    std::wstring PathFile = szFile;
    std::wstring PathFormat = filter;

    std::wstring Path;

    if(PathFile.length() > 4) {
    
        if (PathFile == ((PathFile.substr(0, PathFile.size() - 4)) + (L"." + PathFormat)))
        {
            Path = szFile;
        }
        else
        {
            Path = PathFile + L"." + PathFormat;
        }

    }
    else
    {

    }

    std::wstring ImageFormatPrefix = L"image/";

    std::wstring Format = ImageFormatPrefix + PathFormat;


    CLSID clsid;
    CLSIDEncoder::GetEncoderClsid(Format.c_str(), &clsid);
    bmp->Save(Path.c_str(), &clsid, NULL);
}

void WriteLog(HWND hwnd)
{
    SetWindowTextA(hwnd, "Test");
    return;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{   

    char buffer[250];
    //[] nombre total de caractères que l'on peut mettre dans le tableau
    //sizeof(buffer) => permet d'obtenir la taille/length de la variable

    switch (message)
    {

    case WM_CREATE:
    {
    /*Heimanu*////
    //Création des éléments de l'Interface Utilisateur

    //CreateWindowA(
    // Nom de la classe (voir dans la docu),
    // Texte inscrit sur l'élément d'interface,
    // Styles de la classe (voir dans la docu), 
    // Position X,
    // Position Y,
    // Taille X,
    // Taille Y,
    // Parent HWND,
    // hMenu->NULL par défaut => assigner un numéro identifiable pour WM_Command,
    // hInstance->NULL par défaut,
    // Ip Param->NULL par défaut
    // );

    //Background à gauche de l'écran

        l_background = CreateWindowA("Static", " ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 120, 550, 400, hWnd, NULL, NULL, NULL);

        //Texte du message caché à gauche de l'écran
        l_text = CreateWindowA("Static", "Log", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 750, 120, 600, 400, hWnd, NULL, NULL, NULL);

        //Log à droite de l'écran
        HWND log = NULL;

        l_text = CreateWindowA("Static", "Message de l'image", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 80, 540, 400, 80, hWnd, NULL, NULL, NULL);

        //Boîte de texte qu'on peut éditer
        entry = CreateWindowA("Edit", "Saisissez votre message ici", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 750, 550, 360, 80, hWnd, NULL, NULL, NULL);

        //Ajouts de boutons
        AddButtons(hWnd);

        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            // Analyse les sélections de menu :
            switch (wmId)
            {
                /*Heimanu*/
                //Déclenche un event lorsque le joueur appuie sur un bouton créé dans InitInstance()

            case OPEN_FILE_BUTTON: /*Charger un fichier bitmap*/
                OpenFile(hWnd);
                break;
            case HIDE_MESSAGE_BUTTON: /*Cacher le message*/
                HideMessage(hWnd);
                break;
            case EXTRACT_MESSAGE_BUTTON: /*Extraire le message*/
                ExtractMessage(hWnd);
                break;

                ////////////////////////////////

            //Heimanu
            //Raccourcis et boutons de la barre
            case ID_FICHIER_IMPORTERUNFICHIER: //ALT + V: Import d'un fichier
                OpenFile(hWnd);
                break;

            case ID_FICHIER_EXPORTERUNFICHIER: //ALT + B: Export d'un fichier
                Export(hWnd);
                break;


            case IDM_ABOUT: //ALT + /: Informations à propos du programme
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;

            case IDM_EXIT: //ALT + F4: Fin du programme
                DestroyWindow(hWnd);

                break;

            default:

                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            EndPaint(hWnd, &ps);

            hdc = BeginPaint(l_background, &ps);
           
            Graphics graphics(hdc);

            if (imgLoader->GetPictureToDisplay() != nullptr)
            {
                Rect myrect(0, 0, 550, 400);
                graphics.DrawImage(imgLoader->GetPictureToDisplay(), myrect);
            }
            EndPaint(l_background, &ps);

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
