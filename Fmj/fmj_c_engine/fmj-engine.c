// fmj-engine.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "fmj-engine.h"
#include "engine.h"
#include "debug.h"

#define MAX_LOADSTRING 100
#define SCREEN_WIDTH   159
#define SCREEN_HEIGHT  96

// 全局变量:
extern UINT8* pGameData;
HINSTANCE hInst;                                // 当前实例
CHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
CHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
LONG frame_width;
LONG frame_height;
LONG screen_mult = 4;
UINT8* pVideoRam;
HANDLE hGameThread = NULL;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void    LoadGameFileA(LPSTR pGameFilePath);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    INIT_CONSOLE();
    LoadGameFileA(lpCmdLine);

    // 初始化全局字符串
    LoadStringA(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringA(hInstance, IDC_FMJENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FMJENGINE));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    RELEASE_CONSOLE();
    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FMJENGINE));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExA(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   RECT rect = { 0, 0, SCREEN_WIDTH * screen_mult, SCREEN_HEIGHT * screen_mult };
   AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_ACCEPTFILES);
   frame_width = rect.right - rect.left - SCREEN_WIDTH * screen_mult;
   frame_height = rect.bottom - rect.top - SCREEN_HEIGHT * screen_mult;
   HWND hWnd = CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2,
       (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2,
       rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


void CalcWindowSize(HWND hWnd, RECT* rect, FIXED_WINDOW fixed)
{
    RECT old_rect;
    GetWindowRect(hWnd, &old_rect);
    rect->right = rect->left + SCREEN_WIDTH * screen_mult;
    rect->bottom = rect->top + SCREEN_HEIGHT * screen_mult;
    AdjustWindowRectEx(rect,
        GetWindowLong(hWnd, GWL_STYLE),
        FALSE,
        GetWindowLong(hWnd, GWL_EXSTYLE)
    );
    LONG width = rect->right - rect->left;
    LONG height = rect->bottom - rect->top;
    if (fixed == FIEXD_LEFTCENTER || fixed == FIEXD_TOPLEFT || fixed == FIEXD_BOTTOMLEFT)
    {
        rect->left = old_rect.left;
        rect->right = old_rect.left + width;
    }
    if (fixed == FIEXD_RIGHTCENTER || fixed == FIEXD_TOPRIGHT || fixed == FIEXD_BOTTOMRIGHT)
    {
        rect->right = old_rect.right;
        rect->left = old_rect.right - width;
    }
    if (fixed == FIEXD_TOPCENTER || fixed == FIEXD_TOPLEFT || fixed == FIEXD_TOPRIGHT)
    {
        rect->top = old_rect.top;
        rect->bottom = old_rect.top + height;
    }
    if (fixed == FIEXD_BOTTOMCENTER || fixed == FIEXD_BOTTOMLEFT || fixed == FIEXD_BOTTOMRIGHT)
    {
        rect->bottom = old_rect.bottom;
        rect->top = old_rect.bottom - height;
    }
    if (fixed == FIEXD_LEFTCENTER || fixed == FIEXD_RIGHTCENTER)
    {
        rect->top = __max(0, (old_rect.bottom + old_rect.top - height) / 2);
        rect->bottom = rect->top + height;
    }
    if (fixed == FIEXD_TOPCENTER || fixed == FIEXD_BOTTOMCENTER)
    {
        rect->left = (old_rect.right + old_rect.left - width) / 2;
        rect->right = rect->left + width;
    }
}

void LoadGameFileA(LPSTR pGameFilePath)
{
    DWORD dwNumberOfBytesRead = 0;
    int path_length = lstrlenA(pGameFilePath);
    HANDLE hGameFile = CreateFileA(pGameFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hGameFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwDataOffset = 0;
        DWORD dwFileSize = GetFileSize(hGameFile, NULL);
        if (CompareStringA(GetUserDefaultLCID(), LINGUISTIC_IGNORECASE, pGameFilePath + path_length - 4, 4, ".gam", 4) == CSTR_EQUAL)
        {
            SetFilePointer(hGameFile, 0x42, NULL, FILE_BEGIN);
            if (!ReadFile(hGameFile, &dwDataOffset, sizeof(dwDataOffset), &dwNumberOfBytesRead, NULL))
            {
                CloseHandle(hGameFile);
                return;
            }
            SetFilePointer(hGameFile, dwDataOffset, NULL, FILE_BEGIN);
        }
        HANDLE hHeap = GetProcessHeap();
        if (pGameData != NULL)
        {
            HeapFree(hHeap, 0, pGameData);
        }
        pGameData = (UINT8*)HeapAlloc(hHeap, 0, dwFileSize- dwDataOffset);
        if (!ReadFile(hGameFile, pGameData, dwFileSize- dwDataOffset, &dwNumberOfBytesRead, NULL))
        {
            HeapFree(hHeap, 0, pGameData);
            pGameData = NULL;
        }
        CloseHandle(hGameFile);
    }
}

DWORD WINAPI EngineStart(LPVOID lpThreadParameter)
{
    pVideoRam = MCU_memory+0x400;
    // todo 要支持第二次及以后的游戏文件打开，需要在这里将_9BankNumber置0
    memset(MCU_memory, 0, 0x9000);
    memcpy(MCU_memory + 0x9000, pGameData, 0x4000);
    _00200046();
    return 0;
}

void StartGame()
{
    if (pGameData == NULL)
    {
        return;
    }
    if (hGameThread != NULL)
    {
        TerminateThread(hGameThread, 0);
    }
    hGameThread = CreateThread(NULL, 0, EngineStart, NULL, 0, NULL);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        DragAcceptFiles(hWnd, TRUE);
        StartGame();
        SetTimer(hWnd, 0x1234, 100, NULL); // 10 fps
        break;
    }
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;
        UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
        WCHAR strFileName[MAX_PATH];
        if (nFileNum > 0)
        {
            DragQueryFileA(hDrop, 0, strFileName, MAX_PATH);
            LoadGameFileA(strFileName);
            StartGame();
        }
        break;
    }
    case WM_KEYDOWN:
        PostThreadMessage(GetThreadId(hGameThread), message, wParam, lParam);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case 0x1234:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            InvalidateRect(hWnd, &rect, FALSE);
            break;
        }
        }
        break;
    case WM_PAINT:
    {
        if (pVideoRam == NULL)
        {
            break;
        }
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        int x = 0;
        int y = 0;
        LONG mult = screen_mult;
        if (IsZoomed(hWnd))
        {
            RECT rect;
            GetClientRect(hWnd, &rect);
            mult = (LONG)(ceil(__min((double)(rect.right - rect.left) / SCREEN_WIDTH,
                (double)(rect.bottom - rect.top) / SCREEN_HEIGHT)));
            x = (rect.right - rect.left - SCREEN_WIDTH * mult) / 2;
            y = (rect.bottom - rect.top - SCREEN_HEIGHT * mult) / 2;
        }
        // pVideoRam里面是：1黑色，0白色，所以显示前要取反，一次计算32位（4字节）以提升速度
        UINT32 tmpVideoRam[160 * 96 / 8 / 4];
        for (UINT16 i = 0; i < ARRAYSIZE(tmpVideoRam); i++)
        {
            tmpVideoRam[i] = ~(*(UINT32*)(pVideoRam + i * 4));
        }
        HBITMAP hbmScreen = CreateBitmap(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1, tmpVideoRam);
        HDC hdcScreen = CreateCompatibleDC(hdc);
        SelectObject(hdcScreen, hbmScreen);
        StretchBlt(hdc, x, y, SCREEN_WIDTH * mult, SCREEN_HEIGHT * mult,
            hdcScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SRCCOPY);
        DeleteDC(hdcScreen);
        DeleteObject(hbmScreen);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SIZING:
    {
        RECT* wrect = (RECT*)lParam;
        FIXED_WINDOW fixed = FIEXD_NONE;

        int h = wrect->bottom - wrect->top;
        int w = wrect->right - wrect->left;
        double mult = 1.0;
        switch (wParam)
        {
        case WMSZ_LEFT:
        case WMSZ_RIGHT:
            mult = (double)(w - frame_width) / SCREEN_WIDTH;
            break;
        case WMSZ_TOP:
        case WMSZ_BOTTOM:
            mult = (double)(h - frame_height) / SCREEN_HEIGHT;
            break;
        default:
            mult = __max((double)(w - frame_width) / SCREEN_WIDTH,
                (double)(h - frame_height) / SCREEN_HEIGHT);
            break;
        }
        //screen_mult = __max(1, LONG(round(mult)));
        screen_mult = 1. > mult + 0.5 ? 1 : (LONG)(mult + 0.5);
        switch (wParam)
        {
        case WMSZ_LEFT:
            fixed = FIEXD_RIGHTCENTER;
            break;
        case WMSZ_RIGHT:
            fixed = FIEXD_LEFTCENTER;
            break;
        case WMSZ_TOP:
            fixed = FIEXD_BOTTOMCENTER;
            break;
        case WMSZ_BOTTOM:
            fixed = FIEXD_TOPCENTER;
            break;
        case WMSZ_BOTTOMRIGHT:
            fixed = FIEXD_TOPLEFT;
            break;
        case WMSZ_TOPRIGHT:
            fixed = FIEXD_BOTTOMLEFT;
            break;
        case WMSZ_TOPLEFT:
            fixed = FIEXD_BOTTOMRIGHT;
            break;
        case WMSZ_BOTTOMLEFT:
            fixed = FIEXD_TOPRIGHT;
            break;
        }
        CalcWindowSize(hWnd, wrect, fixed);
        break;
    }
    case WM_DESTROY:
        if (hGameThread != NULL)
        {
            TerminateThread(hGameThread, 0);
        }
        KillTimer(hWnd, 1);
        if (pGameData != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pGameData);
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}