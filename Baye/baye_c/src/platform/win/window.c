#include <stdio.h>
#include <windows.h>
#include <baye/consdef.h>

#define		BY_VK_PGUP				0x20
#define		BY_VK_PGDN				0x21
#define		BY_VK_UP				0x22
#define		BY_VK_DOWN				0x23
#define		BY_VK_LEFT				0x24
#define		BY_VK_RIGHT			    0x25
#define		BY_VK_HELP				0x26
#define		BY_VK_ENTER			    0x27
#define		BY_VK_EXIT				0x28
#define		BY_VK_INSERT			0x30
#define		BY_VK_DEL				0x31
#define		BY_VK_MODIFY			0x32
#define		BY_VK_SEARCH			0x33
#define		BY_VK_SHIFT			    0x34			/* shift */
#define		BY_VK_SYM				0x35			/*  符号 */

void bayeStart();
void winInitTimer();
void GamSetLcdFlushCallback(void (*lcd_fluch_cb)(char *buffer));
void bayeSendKey(int key);

const char g_szClassName[] = "BayeWindowClass";
static HWND hMainWin = 0;
static char *lcdBuffer = NULL;
static int lcdDirty = 0;

typedef struct
{
    HDC memDC;
    DWORD *bmpBuffer;
    DWORD bufferSize;
    HBITMAP hBmp;
    COLORREF black, white;
    int w, h;
} UserData;

#define SCR_W (SCR_WID * 2)
#define SCR_H (SCR_HGT * 2)
#define BYTES_PERLINE (SCR_LINE * 8 * 2)

static HBITMAP _createBitmap(int w, int h, DWORD **pBuffer)
{
    BITMAPINFO bmp = {0};
    bmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp.bmiHeader.biWidth = w;
    bmp.bmiHeader.biHeight = h;
    bmp.bmiHeader.biPlanes = 1;
    bmp.bmiHeader.biBitCount = 32;
    bmp.bmiHeader.biCompression = BI_RGB;
    return CreateDIBSection(NULL, &bmp, DIB_RGB_COLORS, pBuffer, NULL, 0);
}

static UserData *_getUserData(HWND hwnd, HDC hDC, int w, int h)
{
    UserData *data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!data)
    {
        data = (UserData *)malloc(sizeof(*data));
        data->memDC = CreateCompatibleDC(hDC);
        data->w = w;
        data->h = h;
        data->black = RGB(0, 0, 0);
        data->white = RGB(200, 200, 200);
        data->hBmp = _createBitmap(w, h, &data->bmpBuffer);
        data->bufferSize = sizeof(DWORD) * w * h;
        
        SelectObject(data->memDC, data->hBmp);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, data);
    }

    if (data->h != h || data->w != w)
    {
        HBITMAP hBmp = _createBitmap(w, h, &data->bmpBuffer);
        SelectObject(data->memDC, hBmp);
        DeleteObject(data->hBmp);
        data->hBmp = hBmp;
        data->w = w;
        data->h = h;
        data->bufferSize = sizeof(DWORD) * w * h;
    }

    return data;
}

static inline void _setPixel(UserData* data, int x, int y, COLORREF color)
{
    data->bmpBuffer[y*data->w + x] = color;
}

static void _redrawWindow(HWND hwnd, HDC hDC)
{
    UserData *data = _getUserData(hwnd, hDC, SCR_W, SCR_H);
    int x, y;
    for (y = 0; y < SCR_H; y++)
    {
        for (x = 0; x < SCR_W; x++)
        {
            int ind = BYTES_PERLINE * (SCR_H - y) + (x);
            COLORREF color = lcdBuffer[ind] ? data->black : data->white;
            _setPixel(data, x, y, color);
        }
    }
    BitBlt(hDC, 0, 0, data->w, data->h, data->memDC, 0, 0, SRCCOPY);
}

static void drawCached(HWND hwnd, HDC hDC)
{
    UserData *data = _getUserData(hwnd, hDC, SCR_W, SCR_H);
    BitBlt(hDC, 0, 0, data->w, data->h, data->memDC, 0, 0, SRCCOPY);
}

LRESULT CALLBACK BayeWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hwnd, &ps);

        if (lcdDirty)
        {
            lcdDirty = 0;
            _redrawWindow(hwnd, hDC);
        }
        else
        {
            drawCached(hwnd, hDC);
        }
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_KEYDOWN:
    {
        int key;
        switch(wParam) {
            case 13:
                key = BY_VK_ENTER;
                break;
            case 27:
            case 32:
                key = BY_VK_EXIT;
                break;
            case 33:
                key = BY_VK_PGUP;
                break;
            case 34:
                key = BY_VK_PGDN;
                break;
            case 38:
                key = BY_VK_UP;
                break;
            case 40:
                key = BY_VK_DOWN;
                break;
            case 37:
                key = BY_VK_LEFT;
                break;
            case 39:
                key = BY_VK_RIGHT;
                break;
            case 72:
            case 112:
                key = BY_VK_HELP;
                break;
            case 83:
            case 113:
                key = BY_VK_SEARCH;
                break;
            default:
                key = wParam;
        }
        bayeSendKey(key);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void RegisterMainWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = BayeWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "窗口注册失败!", "错误", MB_ICONEXCLAMATION | MB_OK);
        exit(0);
    }
}

HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd;
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "iBaye 预览版",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 300,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "窗口创建失败!", "错误", MB_ICONEXCLAMATION | MB_OK);
        exit(0);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return hwnd;
}

static void _lcd_flush_cb(char *buffer)
{
    lcdBuffer = buffer;
    lcdDirty = 1;
    InvalidateRect(hMainWin, NULL, FALSE);
}

int main(int argc, char *argv[])
{
    MSG msg;
    HINSTANCE hInstance;

    hInstance = GetModuleHandle(0);
    RegisterMainWindow(hInstance);
    hMainWin = CreateMainWindow(hInstance, SW_SHOW);

    GamSetLcdFlushCallback(_lcd_flush_cb);

    winInitTimer();
    bayeStart();

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
