#include "Resource.h"

#include "framework.h"
#include <commdlg.h>
#include <stdlib.h>


#include "MainWindow.h"
#include "ImageLoader.h"
#include "SteganoSystem.h"
#include "CLSIDEncoder.h"


using namespace std::placeholders;
using namespace Gdiplus;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int MainWindow::Run(HINSTANCE hInstance, int  nCmdShow)
{

	RegisterForEvents();
    m_imgLoader = new ImageLoader();
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

void MainWindow::HandleCreateEvent(EventParams params)
{
	m_imageZone = CreateWindowA("Static", " ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 250, 550, 400, params.hWnd, NULL, NULL, NULL);
	m_logWidowElement = CreateWindowA("Static", "Message de l'image: ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 60, 550, 40, params.hWnd, NULL, NULL, NULL);
	m_messageDisplayElement = CreateWindowA("Static", "LOG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 850, 150, 550, 380, params.hWnd, NULL, NULL, NULL);
	m_messageInputField = CreateWindowA("Edit", "Saisissez votre message ici", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 850, 580, 360, 80, params.hWnd, NULL, NULL, NULL);

	AddButtons(params.hWnd);
}

void MainWindow::HandleCommandEvent(EventParams params)
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

        ////////////////////////////////

    case IDM_ABOUT:
        DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), params.hWnd, About);

        break;
    case IDM_EXIT:
        DestroyWindow(params.hWnd);

        break;

    default:

        DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);
    }
}

void MainWindow::HandlePaintEvent(EventParams params)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    EndPaint(m_hWnd, &ps);

    hdc = BeginPaint(m_imageZone, &ps);

    Graphics graphics(hdc);

    if (m_imgLoader->GetPictureToDisplay() != nullptr)
    {
        Rect myrect(0, 0, 550, 400);
        graphics.DrawImage(m_imgLoader->GetPictureToDisplay(), myrect);
    }
    EndPaint(m_imageZone, &ps);
}

void MainWindow::HandleDestroyEvent(EventParams params)
{
    PostQuitMessage(0);
    return;
}

void MainWindow::AddButtons(HWND hwnd)
{
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
    buttonmap[0] = { 180, 180, 180, 40, "Charger un fichier bitmap", OPEN_FILE_BUTTON }; /*Charger un fichier bitmap*/
    buttonmap[1] = { 1250, 580, 150, 40, "Cacher le message", HIDE_MESSAGE_BUTTON }; /*Cacher le message*/
    buttonmap[2] = { 600, 600, 150, 40, "Extraire le message", EXTRACT_MESSAGE_BUTTON }; /*Extraire le message*/

    for (int i = 0; i < 3; i++)
    {

        buttons[i] = CreateWindowA("Button", buttonmap[i].title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonmap[i].s_x, buttonmap[i].s_y, buttonmap[i].s_width, buttonmap[i].s_height, hwnd, (HMENU)buttonmap[i].buttonnum, NULL, NULL);

    }
    return;
}

ATOM MainWindow::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
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
    OPENFILENAMEA ofn;
    char file_name[700] = " ";

    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));

    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(file_name);
    ofn.lpstrFilter = "Bitmap Files\0*.BMP\0All Files\0*.*\0";  // Types de fichiers à filtrer
    ofn.nFilterIndex = 1;   // Index de départ des filtres (commence à 1)
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;  // Dossier initial (NULL pour le dossier actuel)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  // Options (doit exister)

    // Affiche la boîte de dialogue "Ouvrir"
    if (GetOpenFileNameA(&ofn) == TRUE) {

        m_hbitmap = (HBITMAP)LoadImageA(NULL, file_name, IMAGE_BITMAP, 600, 600, LR_LOADFROMFILE);

        if (m_hbitmap == NULL)
        {
            MessageBox(hWnd, L"Erreur de chargement du bitmap!", L"Erreur", MB_OK | MB_ICONERROR);
        }
        else
        {
            const size_t file_nameSize = strlen(file_name) + 1;
            wchar_t* file_name_wc = new wchar_t[file_nameSize];

            size_t outSize;
            mbstowcs_s(&outSize, file_name_wc, file_nameSize, file_name, file_nameSize - 1);
            m_imagePath = file_name_wc;
            m_imgLoader->ConvertToBmp(file_name_wc, L"Images/bmptest.bmp");
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
        }
    }
    else
    {
        MessageBox(hWnd, L"Fermeture de la boîte de dialogue", L"Erreur", MB_OK | MB_ICONERROR);
    }

    return 0;
}

void MainWindow::HideMessage(HWND hWnd)
{
    Bitmap* bmp = m_imgLoader->GetPictureToDisplay();
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
