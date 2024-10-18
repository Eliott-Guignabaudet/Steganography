#include "Resource.h"

#include "framework.h"
#include <commdlg.h>
#include <stdlib.h>


#include "MainWindow.h"
#include "ImageLoader.h"
#include "SteganoSystem.h"
#include "CLSIDEncoder.h"
#include "ThemeSystem.h"
#include "FilterSystem.h"

using namespace std::placeholders;
using namespace Gdiplus;

int MainWindow::Run(HINSTANCE hInstance, int  nCmdShow)
{

	RegisterForEvents();
    ThemeSystem thmSys;
    thmSys.Init(m_hWnd);

    m_imgLoader = new ImageLoader();
    m_filterSystem = new FilterSystem();
    m_filterSystem->Init(m_hWnd, m_imgLoader);
    ATOM result = MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STEGANOGRAPHY));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

void MainWindow::RegisterForEvents()
{
	auto bindCreate = std::bind(&MainWindow::HandleCreateEvent, this, _1);
	OnCreate = bindCreate;
	UIEventSystem::GetInstance()->RegisterFoMessage(WM_CREATE, OnCreate);

    auto bindCommand = std::bind(&MainWindow::HandleCommandEvent, this, _1);
    OnEventCommand = bindCommand;
    UIEventSystem::GetInstance()->RegisterFoMessage(WM_COMMAND, OnEventCommand);

    auto bindPaint = std::bind(&MainWindow::HandlePaintEvent, this, _1);
    OnEventPaint = bindPaint;
    UIEventSystem::GetInstance()->RegisterFoMessage(WM_PAINT, OnEventPaint);

    auto bindDestroy = std::bind(&MainWindow::HandleDestroyEvent, this, _1);
    OnEventDestroy = bindDestroy;
    UIEventSystem::GetInstance()->RegisterFoMessage(WM_DESTROY, OnEventDestroy);
}

LRESULT MainWindow::HandleCreateEvent(EventParams params)
{
	m_imageZone = CreateWindowA("Static", " ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 120, 550, 400, params.hWnd, NULL, NULL, NULL);
	m_messageDisplayElement = CreateWindowA("Edit", "Message de l'image: ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY, 80, 540, 400, 80, params.hWnd, NULL, NULL, NULL);
    m_logWidowElement = CreateWindowA("Static", "LOG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOHSCROLL, 750, 120, 600, 400, params.hWnd, NULL, NULL, NULL);
	m_messageInputField = CreateWindowA("Edit", "Saisissez votre message ici", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, 750, 550, 360, 80, params.hWnd, NULL, NULL, NULL);

	AddButtons(params.hWnd);
    return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);

}

LRESULT MainWindow::HandleCommandEvent(EventParams params)
{
    int wmId = LOWORD(params.wParam);

    // Analyse les sélections de menu :
    switch (wmId)
    {
        /*Heimanu*/
        //Déclenche un event lorsque le joueur appuie sur un bouton créé dans InitInstance()

    case OPEN_FILE_BUTTON: /*Charger un fichier bitmap*/
        OpenFile(params.hWnd);
        break;
    case HIDE_MESSAGE_BUTTON: /*Cacher le message*/
        HideMessage(params.hWnd);
        break;
    case EXTRACT_MESSAGE_BUTTON: /*Extraire le message*/
        ExtractMessage(params.hWnd);
        break;

    case ID_FICHIER_IMPORTERUNFICHIER: //ALT + V: Import d'un fichier
        OpenFile(params.hWnd);
        break;

    case ID_FICHIER_EXPORTERUNFICHIER: //ALT + B: Export d'un fichier
        Export(params.hWnd);
        break;
        ////////////////////////////////

    case IDM_ABOUT:
        DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), params.hWnd, About);

        break;
    case IDM_EXIT:
        DestroyWindow(params.hWnd);

        break;

    default:

        return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);
    }
}

LRESULT MainWindow::HandlePaintEvent(EventParams params)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    EndPaint(m_hWnd, &ps);

    hdc = BeginPaint(m_imageZone, &ps);

    Graphics graphics(hdc);

    if (m_filterSystem->GetFilteredBitmap() != nullptr)
    {
        Rect myrect(0, 0, 550, 400);
        graphics.DrawImage(m_filterSystem->GetFilteredBitmap(), myrect);
    }
    EndPaint(m_imageZone, &ps);
    return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);

}

LRESULT MainWindow::HandleDestroyEvent(EventParams params)
{
    PostQuitMessage(0);
    return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);
}

void MainWindow::AddButtons(HWND hwnd)
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

        buttons[i] = CreateWindowA("Button", buttonmap[i].title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON , buttonmap[i].s_x, buttonmap[i].s_y, buttonmap[i].s_width, buttonmap[i].s_height, hwnd, (HMENU)buttonmap[i].buttonnum, NULL, NULL);

    }
    return;
}

ATOM MainWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW ;
    wcex.lpfnWndProc = UIEventSystem::HandleMessages;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STEGANOGRAPHY));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_STEGANOGRAPHY);
    wcex.lpszClassName = m_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL MainWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    m_hInstance = hInstance; // Stocke le handle d'instance dans la variable globale

    HWND hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW | CS_DROPSHADOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

int MainWindow::OpenFile(HWND hWnd)
{
    WCHAR* imagePath;

    OPENFILENAMEA ofn;
    char file_name[700] = " ";

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
        m_imgLoader->ConvertToBmp(file_name_wc, L"Images/bmptest.bmp");
        m_filterSystem->SetFilter(Filter::None);
        //RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
        //SendMessage(hWnd, WM_PAINT, 0, 0);

    }
    else
    {

    }

    return 0;
}

void MainWindow::Export(HWND hWnd)
{
    //Bitmap* bmp = m_imgLoader->GetPictureToDisplay();
    Bitmap* bmp = m_filterSystem->GetFilteredBitmap();

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

    if (PathFile.length() > 4) {

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

void MainWindow::HideMessage(HWND hWnd)
{
    Bitmap* bmp = m_filterSystem->GetFilteredBitmap();

    //Bitmap* bmp = m_imgLoader->GetPictureToDisplay();
    if (bmp == nullptr)
    {
        MessageBox(hWnd, L"No picture loaded", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }


    int size = GetWindowTextLength(m_messageInputField);
    LPWSTR message = (LPWSTR)VirtualAlloc((LPVOID)NULL,
        (DWORD)(size + 1), MEM_COMMIT,
        PAGE_READWRITE);
    GetWindowText(m_messageInputField, message, size);



    std::wstring ws(message);
    std::string message_str(ws.begin(), ws.end());

    SteganoSystem::GetInstance()->HideMessage(*bmp, message_str);

    CLSID clsid;
    CLSIDEncoder::GetEncoderClsid(L"image/bmp", &clsid);
    bmp->Save(L"C:\\Users\\eguignabaudet\\Pictures\\FileChanged2.bmp", &clsid, NULL);

    return;
}

void MainWindow::ExtractMessage(HWND hwnd)
{
    Bitmap* bmp = m_imgLoader->GetPictureToDisplay();
    if (bmp == nullptr)
    {
        MessageBoxW(m_hWnd, L"No picture loaded", L"Erreur", MB_OK | MB_ICONERROR);
        return;
    }
    std::string message = SteganoSystem::GetInstance()->FindMessage(*bmp);
    std::wstring widestr = std::wstring(message.begin(), message.end());
    SetWindowTextA(m_messageDisplayElement, message.c_str());
}

INT_PTR MainWindow::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
