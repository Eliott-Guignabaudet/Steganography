#include "Resource.h"
#include "framework.h"

#include "MainWindow.h"
#include "ImageLoader.h"
#include "SteganoSystem.h"

using namespace std::placeholders;
using namespace Gdiplus;

int MainWindow::Init(HINSTANCE hInstance, int  nCmdShow)
{

	RegisterForEvents();
    m_imgLoader = new ImageLoader();
    MyRegisterClass(hInstance);

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

}

void MainWindow::HandleCreateEvent(EventParams params)
{
	m_imageZone = CreateWindowA("Static", " ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 250, 550, 400, m_hWnd, NULL, NULL, NULL);
	m_logWidowElement = CreateWindowA("Static", "Message de l'image: ", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 60, 550, 40, m_hWnd, NULL, NULL, NULL);
	m_messageDisplayElement = CreateWindowA("Static", "LOG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL, 850, 150, 550, 380, m_hWnd, NULL, NULL, NULL);
	m_messageInputField = CreateWindowA("Edit", "Saisissez votre message ici", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER, 850, 580, 360, 80, m_hWnd, NULL, NULL, NULL);

	AddButtons();
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

void MainWindow::AddButtons()
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

        buttons[i] = CreateWindowA("Button", buttonmap[i].title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, buttonmap[i].s_x, buttonmap[i].s_y, buttonmap[i].s_width, buttonmap[i].s_height, m_hWnd, (HMENU)buttonmap[i].buttonnum, NULL, NULL);

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
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

int MainWindow::OpenFile(HWND hwnd)
{
    return 0;
}

void MainWindow::HideMessage(HWND hwnd)
{
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
