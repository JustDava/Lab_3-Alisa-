// Lab_3(Alisa).cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Lab_3(Alisa).h"

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "Resource.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

Image* background, * sand, * metal;
unsigned int fIndex, fCount;
int step = 0;
float distance = 0.f;
float angle2 = 0.f;
float t = 0.f;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID Display(HDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB3ALISA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3ALISA));

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

    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3ALISA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB3ALISA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   sand = new Bitmap(TEXT("sand.png"));
   metal = new Bitmap(TEXT("metal.png"));

   if (!hWnd)
   {
      return FALSE;
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
    case WM_CREATE:
    {
        background = Image::FromFile(L"background.gif");
        if (background == NULL) return -1;

        fIndex = 0;
        fCount = background->GetFrameCount(&FrameDimensionTime);

        SetTimer(hWnd, 1, 10, NULL);
        return 0;
    }
    case WM_TIMER:
    {
        fIndex = (fIndex + 1) % fCount;
        background->SelectActiveFrame(&FrameDimensionTime, fIndex);
        step++;
        t += 0.005f;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
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
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_ERASEBKGND: return 1;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            Display(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        if (metal != NULL)
        {
            delete metal;
        }
        if (sand != NULL)
        {
            delete sand;
        }
        if (background != NULL)
        {
          delete background;
        }
        PostQuitMessage(0);
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

PointF Tween(const PointF& A, const PointF& B, float t)
{
    return PointF(A.X * (1.f - t) + B.X * t, A.Y * (1.f - t) + B.Y * t);
}

void Tween(const PointF* A, const PointF* B, PointF* P, int count, float t)
{
    for (int i = 0; i < count; i++)
    {
        P[i] = Tween(A[i], B[i], t);
    }
}

VOID DrawTweening(Graphics& g)
{
    Rect rect;
    g.GetVisibleClipBounds(&rect);
    Bitmap backbuffer(rect.Width, rect.Height, &g);

    Pen blackpen(Color::Black, 2.f);
    Graphics temp(&backbuffer);
    temp.SetSmoothingMode(SmoothingModeHighQuality);

    PointF start_pos[] = {
        PointF(1000.f, 20.f),
        PointF(1050.f, 70.f),
        PointF(1000.f, 120.f),
        PointF(950.f, 70.f)
    };
    PointF end_pos[] = {
    PointF(950.f, 20.f),
    PointF(1050.f, 20.f),
    PointF(1050.f, 120.f),
    PointF(950.f, 120.f)
    };
    PointF int_pos[4];

    Tween(start_pos, end_pos, int_pos, 4, t);
    g.DrawPolygon(&blackpen, int_pos, 4);
}

VOID DisplayPicture(Graphics& g) 
{

    Rect rect;
    g.GetVisibleClipBounds(&rect);
    Bitmap backbuffer(rect.Width, rect.Height, &g);
    Graphics temp(&backbuffer);
    temp.SetSmoothingMode(SmoothingModeHighQuality);

    //песок
    TextureBrush tBrush(sand);
    g.FillRectangle(&tBrush, 385, 220, 270, 130);

    //кузов
    SolidBrush solidBrush(Color::Sienna);
    g.FillRectangle(&solidBrush, 380, 280, 280, 110);

    //металл
    TextureBrush tBrush2(metal);
    Pen pen1(&tBrush2, 3);
    g.DrawLine(&pen1, 380, 307, 660, 307);
    g.DrawLine(&pen1, 380, 335, 660, 335);
    g.DrawLine(&pen1, 380, 362, 660, 362);


    solidBrush.SetColor(Color::LightBlue);
    GraphicsPath path;
    path.AddEllipse(470, 220, 180, 150);
    Region rgn2(&path);
    g.SetClip(&rgn2, CombineModeExclude);
    g.FillRectangle(&solidBrush, 570, 220, 90, 60);
    Region infiniteRegion;
    g.SetClip(&infiniteRegion);

    GraphicsPath path1;
    path1.AddEllipse(385, 220, 160, 150);
    Region rgn(&path1);
    g.SetClip(&rgn, CombineModeExclude);
    g.FillRectangle(&solidBrush, 385, 220, 80, 60);
    g.SetClip(&infiniteRegion);



    //кабина
    solidBrush.SetColor(Color::Violet);
    g.FillRectangle(&solidBrush, 210, 280, 167, 110);
    g.FillRectangle(&solidBrush, 260, 220, 117, 60);


    //окно
    LinearGradientBrush linGrBrush(Rect(270, 230, 100, 60), Color::White, Color::Blue, LinearGradientMode::LinearGradientModeBackwardDiagonal);
    g.FillRectangle(&linGrBrush, 270, 230, 100, 60);

    //колеса
    solidBrush.SetColor(Color::LightGray);
    HatchBrush hatchBrush(HatchStyleDashedUpwardDiagonal, Color::Black, Color::Gray);
    g.FillEllipse(&hatchBrush, 280, 370, 80, 80);
    g.FillEllipse(&hatchBrush, 545, 370, 80, 80);
    g.FillEllipse(&solidBrush, 292, 382, 55, 55);
    g.FillEllipse(&solidBrush, 557, 382, 55, 55);

    Pen pen(Color::SaddleBrown, 4);
}

VOID Display(HDC hdc)
{
    Graphics g(hdc);

    Rect rect;
    g.GetVisibleClipBounds(&rect);

    Bitmap backBuffer(rect.Width, rect.Height, &g);

    Graphics temp(&backBuffer);
    temp.DrawImage(background, 0, 0, rect.Width, rect.Height);

    distance = 5.f * (step - 50);
    angle2 = -90.f * step / (fCount + 500);
    if (t < 1)
    {
        DrawTweening(temp);
    }
    else t = 0;
    if (step < 50)
    {
        temp.TranslateTransform(450.f - distance, 50.f);
        DisplayPicture(temp);
    }
    else if (step < 200)
    {
        temp.TranslateTransform(450.f - distance, 50.f);
        temp.RotateTransform(angle2+8);
        DisplayPicture(temp);
    }
    else
    {
        step = -1;
        temp.ResetTransform();
    }


    g.DrawImage(&backBuffer, rect);
}
