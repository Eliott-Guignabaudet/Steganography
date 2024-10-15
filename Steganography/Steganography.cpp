// Steganography.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Steganography.h"

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale


// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
//New
/////
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Placez le code ici.

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_MINIMIZEBOX | WS_SYSMENU | CS_DROPSHADOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
   HWND l_background;
   l_background = CreateWindowA("Static", " ", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 20, 250, 550, 400, hWnd, NULL, NULL, NULL);

   //Texte du message caché à gauche de l'écran
   HWND l_text;
   l_text = CreateWindowA("Static", "Message de l'image: ", WS_TABSTOP | WS_VISIBLE | WS_CHILD, 20, 60, 550, 40, hWnd, NULL, NULL, NULL);

   //Log à droite de l'écran
   HWND log;
   log = CreateWindowA("Static", "LOG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 850, 150, 550, 380, hWnd, NULL, NULL, NULL);

   //Tableau des boutons de l'interface
   HWND buttons[3];

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

   buttonsetup buttonmap[3];
   buttonmap[0] = { 180, 180, 180, 40, "Charger un fichier bitmap", 1}; /*Charger un fichier bitmap*/
   buttonmap[1] = { 1250, 580, 150, 40, "Cacher le message", 2}; /*Cacher le message*/
   buttonmap[2] = { 600, 600, 150, 40, "Extraire le message", 3}; /*Extraire le message*/

   for (int i = 0; i < 3; i++)
   {
       
       buttons[i] = CreateWindowA("Button", buttonmap[i].title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonmap[i].s_x, buttonmap[i].s_y, buttonmap[i].s_width, buttonmap[i].s_height, hWnd, (HMENU) buttonmap[i].buttonnum, NULL, NULL);
       
   }

   //Boîte de texte qu'on peut éditer
   HWND entry;
   entry = CreateWindowA("Edit", "Saisissez votre message ici", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 850, 580, 360, 80, hWnd, NULL, NULL, NULL);

   ////
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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

void UpdateText(HWND hWnd)
{

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

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            // Analyse les sélections de menu :
            switch (wmId)
            {
                /*Heimanu*/
                //Déclenche un event lorsque le joueur appuie sur un bouton créé dans InitInstance()

            case 1: /*Charger un fichier bitmap*/
                MessageBox(hWnd, L"Test", L"Test", 0);
                break;
            case 2: /*Cacher le message*/
                MessageBox(hWnd, L"Test", L"Test", 0);
                break;
            case 3: /*Extraire le message*/
                MessageBox(hWnd, L"Test", L"Test", 0);
                break;

                ////////////////////////////////
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
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
            // TODO: Ajoutez ici le code de dessin qui utilise hdc...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CREATE:
    {
        break;
    }
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
