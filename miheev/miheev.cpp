// miheev.cpp : Определяет точку входа для приложения.
//
#include<Windows.h>
#include<WindowsX.h>
#include "framework.h"
#include "miheev.h"
#include <commctrl.h>
#include <commdlg.h>
#include <WinUser.h>
#include <string.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
FINDREPLACE findrep;
LONG_PTR lptr;
UINT uFindMsgString = 0;
HWND Progbar,EdBox;
HWND ReplDialog=NULL;
DWORD IDC_TIMER, nCounter;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MIHEEV, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MIHEEV));

    MSG msg;

    // Цикл основного сообщения:
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



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MIHEEV));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MIHEEV);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return FALSE;
    }
    else {
        CreateWindowEx(0, TEXT("SysTabControl32"), TEXT("табконтрол32"), WS_CHILD | WS_VISIBLE, 0,
            0, 1000, 600, hWnd, NULL, hInstance, NULL);
        Progbar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 10, 50, 200, 30, hWnd,
            (HMENU)IDC_PROGBAR, hInstance, NULL);
        SendMessage(Progbar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(Progbar, PBM_SETSTEP, (WPARAM)10, 0);
       EdBox= CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("Введите текст"), ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE, 10, 
            10, 200, 25, hWnd, NULL, hInstance, NULL);
        CreateWindowEx(0, TEXT("msctls_updown32"), TEXT("Стрелка"), WS_CHILD | WS_VISIBLE, 10,
            100, 30, 40, hWnd,(HMENU)IDC_EDIT1, hInstance, NULL);
        CreateWindowEx(0, TEXT("Button"), TEXT("Открытие файла"), BS_CENTER | WS_CHILD | WS_VISIBLE, 10,
            200, 300, 40, hWnd, (HMENU)IDC_BUTTON1, hInstance, NULL);
        CreateWindowEx(0, TEXT("Button"), TEXT("Замена текста"), BS_CENTER | WS_CHILD | WS_VISIBLE, 10,
            250, 300, 40, hWnd, (HMENU)IDC_BUTTON2, hInstance, NULL);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDC_BUTTON2: {
            WCHAR WhatFind[255]=TEXT("\0"), WhatReplace[255] = TEXT("\0");
            memset(WhatFind, 0, _countof(WhatFind));
            if (uFindMsgString == 0) {
                uFindMsgString = RegisterWindowMessage(FINDMSGSTRING);
            }
            if (IsWindow(ReplDialog) == false) {
                findrep.lStructSize = sizeof(FINDREPLACE);
                findrep.hInstance = hInst;
                findrep.hwndOwner = hWnd;
                findrep.lpstrFindWhat = WhatFind;
                findrep.lpstrReplaceWith = WhatReplace;
                findrep.wFindWhatLen = 100;
                findrep.wReplaceWithLen = 100;
                findrep.lCustData=lptr;
                findrep.Flags = FR_FINDNEXT;
                ReplDialog=ReplaceText(&findrep);
            }
            break;
        }
        case IDC_BUTTON1: {
            TCHAR szFile[100];
            OPENFILENAME ofn = { sizeof(OPENFILENAME) };
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            GetOpenFileName(&ofn);
            break;
        }
        
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        TextOut(hdc, 5, 5, TEXT("HELLO"), _tcslen(TEXT("HELLO")));
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MBUTTONDBLCLK: {
        MessageBox(hWnd, TEXT("Вы сделали 2 клика средней кнопкой мыши. Зачем?"), TEXT("СОБЫТИЕ!!!"), MB_OK);
        break;
    }
    case WM_MBUTTONDOWN: {
        SetTimer(hWnd, IDC_TIMER, 300, 0);
        nCounter = 0;
        break;
    }
    case WM_MOUSEWHEEL: {
        MessageBox(hWnd, TEXT("Вы покрутили колёсико"), TEXT("СОБЫТИЕ!!!"), MB_OK); 
        break;
    }
    case WM_MBUTTONUP: {
        SendMessage(Progbar, PBM_SETPOS, 0, 0);
        break;
    }
    case WM_TIMER: {
        nCounter++;
        SendMessage(Progbar, PBM_STEPIT, 0, 0);
        if (nCounter == 10) {
            KillTimer(hWnd, IDC_TIMER);
        }
        break;
    }
    case WM_SYSCHAR: {
       const wchar_t *b =  L"Вы нажали кнопку";
        Edit_SetText(EdBox, b);
        break;
    }
    case WM_SYSKEYUP: {
        Edit_SetText(EdBox, L"Вы отжали кнопку");
        break;
    }
    case WM_SIZING: {
        MessageBox(hWnd, TEXT("Не надо"), TEXT("СОБЫТИЕ!!!"), MB_OK);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_INITDIALOG:

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
