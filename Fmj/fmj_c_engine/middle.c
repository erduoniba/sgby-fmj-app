#include "middle.h"
#include "framework.h"
#include "font.h"

extern UINT8 MCU_memory_dummy[0x8000];
extern UINT8 MCU_memory[0x10000];
#define Mem_Start				*(UINT32*)(MCU_memory+0x2BAB)
#define Mem_Len					*(UINT32*)(MCU_memory+0x2BAF)
#define Mem_Flag				MCU_memory[0x2BB3]
UINT8* pGameData;
static UINT16 _4BankNumber = 4;
static UINT16 _9BankNumber = 0;
static UINT_PTR timer;
FileInfo fileinfos[FILE_NUM];

void fillmem(UINT8* dst, UINT16 size, UINT8 value)
{
    memset(dst, value, size);
}

void SysPrintString(UINT8 x, UINT8 y, const UINT8* str)
{
    //LOG("SysPrintString: %s\n", str);
    UINT32 offset = 0;
    while (str[0] && y<0x51)
    {
        if (str[0] < 0x80)
        {
            // ASCII码
            if (x >= 0x99)
            {
                x = 0x00;
                y += 0x10;
                continue;
            }
        }
        else if (str[0] > 0x80)
        {
            if ((str[0] >= 0xFD) && (str[1] >= 0xA1))
            {
                if (x >= 0x99)
                {
                    x = 0x00;
                    y += 0x10;
                    continue;
                }
            }
            else
            {
                if (x >= 0x91)
                {
                    x = 0x00;
                    y += 0x10;
                    continue;
                }
            }
        }
        if (str[0] < 0x80)
        {
            offset = 0x0003D740 + str[0] * 0x0010;
        }
        else if ((0xB0 <= str[0] && str[0] <= 0xF7) && (0xA1 <= str[1] && str[1] <= 0xFE))
        {
            offset = 0x00000000 + (str[1] - 0xA1 + (str[0] - 0xB0) * 0x5E) * 0x0020;
        }
        else if ((0xA1 <= str[0] && str[0] <= 0xA9) && (0xA1 <= str[1] && str[1] <= 0xFE))
        {
            offset = 0x00034E00 + (str[1] - 0xA1 + (str[0] - 0xA1) * 0x5E) * 0x0020;
        }
        else if ((0xA8 <= str[0] && str[0] <= 0xA9) && (0x40 <= str[1] && str[1] <= 0xA0))
        {
            //                                  跳过0x7F
            offset = 0x0003BD80 + (str[1] - (str[1] < 0x80 ? 0x40 : 0x41) + *(UINT16*)(MCU_memory + 0x20AF)) * 0x0020;
        }
        else if ((0xAA <= str[0] && str[0] <= 0xAF) && (0xA1 <= str[1] && str[1] <= 0xFE))
        {
            offset = 0x0003DF40 + (str[1] - 0xA1 + (str[1] - 0xAA) * 0x5E) * 0x0020;
        }
        else if ((0xF8 <= str[0] && str[0] <= 0xFE) && (0xA1 <= str[1] && str[1] <= 0xFE))
        {
            offset = 0x000425C0 + (str[1] - 0xA1 + (str[0] - 0xF8) * 0x5E) * 0x0020;
        }
        else if ((0xA1 <= str[0] && str[0] <= 0xA7) && (0x40 <= str[1] && str[1] <= 0xA0))
        {
            // 0x7F处字符为空白，不用跳过
            offset = 0x00047800 + (str[1] - 0x40 + (str[0] - 0xA1) * 0x61) * 0x0020;
        }
        else
        {
            offset = 0x00034E00;
        }

        if (str[0] < 0x80)
        {
            // ASCII码
            SysPicture(x, y, x + 7, y + 15, font_data + offset, 0);
            str += 1;
            x += 0x08;
        }
        else if (str[0] > 0x80)
        {
            SysPicture(x, y, x + 15, y + 15, font_data + offset, 0);
            str += 2;
            if ((str[0] >= 0xFD) && (str[1] >= 0xA1))
            {
                x += 0x08;
            }
            else
            {
                x += 0x10;
            }
        }
    }
}

void SysPutPixel(UINT8 x, UINT8 y, UINT8 data)
{
    if (x >= 159 || y >= 96)
    {
        return;
    }
    UINT8* p = MCU_memory + 0x400 + 160 / 8 * y + x / 8;
    UINT8 mask = 0x01 << (7-(x & 0x07));
    if (data == 0)
    {
        *p &= ~mask;
    }
    else
    {
        *p |= mask;
    }
}

void SysLine(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2)
{
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    UINT8 dx = x2 - x1;
    if (y1 > y2)
    {
        UINT8 dy = y1 - y2;
        if (dy > dx)
        {
            for (UINT8 y = y2; y <= y1; y++)
            {
                UINT8 x = x2 - (UINT8)(ceil(dx * (y - y2) * 1.0 / dy));
                SysPutPixel(x, y, 0x01);
            }
        }
        else
        {
            for (UINT8 x = x1; x <= x2; x++)
            {
                UINT8 y = y1 - (UINT8)(ceil(dy * (x - x1) * 1.0 / dx));
                SysPutPixel(x, y, 0x01);
            }
        }
    }
    else
    {
        UINT8 dy = y2 - y1;
        if (dy > dx)
        {
            for (UINT8 y = y1; y <= y2; y++)
            {
                UINT8 x = x1 + (UINT8)(ceil(dx * (y - y1) * 1.0 / dy));
                SysPutPixel(x, y, 0x01);
            }
        }
        else
        {
            for (UINT8 x = x1; x <= x2; x++)
            {
                UINT8 y = y1 + (UINT8)(ceil(dy * (x - x1) * 1.0 / dx));
                SysPutPixel(x, y, 0x01);
            }
        }
    }
}

void SysRect(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    SysLine(x1, y1, x1, y2);
    SysLine(x1, y1, x2, y1);
    SysLine(x2, y1, x2, y2);
    SysLine(x1, y2, x2, y2);
}

void SysFillRect(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        UINT8* p = MCU_memory + 0x400 + 160 / 8 * y + x1 / 8;
        if (x1 & 0x07)
        {
            if ((x1 & 0xF8) == (x2 & 0xF8))
            {
                *p |= (0xFF >> (x1&0x07)) & (0xFF << (7 - (x2 & 0x07)));
                continue;
            }
            *p |= 0xFF >> (x1 & 0x07);
            p++;
        }
        for (UINT8 x = (x1 + 7) & 0xF8; x < (x2 & 0xF8); x += 8)
        {
            *p = 0xFF;
            p++;
        }
        if (x2 & 0x07)
        {
            *p |= 0xFF << (7 - (x2 & 0x07));
        }
    }
}

void SysSaveScreen(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* BuffPoint)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        for (UINT8 offset = x1 / 8; offset <= x2 / 8; offset++)
        {
            BuffPoint[(offset - x1 / 8) + (y - y1) * (x2 / 8 - x1 / 8 + 1)] = MCU_memory[0x400 + 160 / 8 * y + offset];
        }
    }
}

void SysRestoreScreen(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* BuffPoint)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        for (UINT8 offset = x1 / 8; offset <= x2 / 8; offset++)
        {
            MCU_memory[0x400 + 160 / 8 * y + offset] = BuffPoint[(offset - x1 / 8) + (y - y1) * (x2 / 8 - x1 / 8 + 1)];
        }
    }
}

void SysPicture(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* BuffPoint, UINT8 flag)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        for (UINT8 x = x1; x <= x2; x++)
        {
            UINT8* p = MCU_memory + 0x400 + 160 / 8 * y + x / 8;
            UINT16 offset_bits = (y - y1) * ((x2 - x1 + 1+7)&0xF8) + (x - x1);
            UINT8 data = (BuffPoint[offset_bits / 8] >> (7 - (offset_bits & 0x07))) & 0x01;
            UINT8 mask = 1 << (7 - (x & 0x07));
            if (flag == 0)
            {
                if (data > 0)
                {
                    *p |= mask;
                }
                else
                {
                    *p &= ~mask;
                }
            }
            else
            {
                if (data > 0)
                {
                    *p &= ~mask;
                }
                else
                {
                    *p |= mask;
                }
            }
        }
    }
}

void SysLcdPartClear(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        UINT8* p = MCU_memory + 0x400 + 160 / 8 * y + x1 / 8;
        if (x1 & 0x07)
        {
            if ((x1 & 0xF8) == (x2 & 0xF8))
            {
                *p &= ~((0xFF >> (x1 & 0x07)) & (0xFF << (7 - (x2 & 0x07))));
                continue;
            }
            *p &= ~(0xFF >> (x1 & 0x07));
            p++;
        }
        for (UINT8 x = (x1 + 7) & 0xF8; x < (x2 & 0xF8); x += 8)
        {
            *p = 0x00;
            p++;
        }
        if (x2 & 0x07)
        {
            *p &= ~(0xFF << (7 - (x2 & 0x07)));
        }
    }
}

void SysAscii(UINT8 x, UINT8 y, UINT8 asc)
{
    UINT32 offset = 0;
    if (x >= 0x99)
    {
        return;
    }
    if (y >= 0x51)
    {
        return;
    }
    offset = 0x0003D740 + asc * 0x0010;
    SysPicture(x, y, x + 7, y + 15, font_data + offset, 0);
}

void SysLcdReverse(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        UINT8* p = MCU_memory + 0x400 + 160 / 8 * y + x1 / 8;
        if (x1 & 0x07)
        {
            if ((x1 & 0xF8) == (x2 & 0xF8))
            {
                *p ^= (0xFF >> (x1 & 0x07)) & (0xFF << (7 - (x2 & 0x07)));
                continue;
            }
            *p ^= 0xFF >> (x1 & 0x07);
            p++;
        }
        for (UINT8 x = (x1 + 7) & 0xF8; x < (x2 & 0xF8); x += 8)
        {
            *p ^= 0xFF;
            p++;
        }
        if (x2 & 0x07)
        {
            *p ^= 0xFF << (7 - (x2 & 0x07));
        }
    }
}

void SysPictureDummy(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* pic, UINT8* Screen, UINT8 flag)
{
    if ((x1 >= 159) || (x2 >= 159) || (y1 >= 96) || (y2 >= 96))
    {
        return;
    }
    if (x1 > x2)
    {
        UINT8 tmp = x1;
        x2 = x1;
        x1 = tmp;
    }
    if (y1 > y2)
    {
        UINT8 tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    for (UINT8 y = y1; y <= y2; y++)
    {
        for (UINT8 x = x1; x <= x2; x++)
        {
            UINT8* p = Screen + 160 / 8 * y + x / 8;
            UINT16 offset_bits = (y - y1) * ((x2 - x1 + 1 + 7)&0xF8) + (x - x1);
            UINT8 data = (pic[offset_bits / 8] >> (7 - (offset_bits & 0x07))) & 0x01;
            UINT8 mask = 1 << (7 - (x & 0x07));
            if (flag == 0)
            {
                if (data > 0)
                {
                    *p |= mask;
                }
                else
                {
                    *p &= ~mask;
                }
            }
            else
            {
                if (data > 0)
                {
                    *p &= ~mask;
                }
                else
                {
                    *p |= mask;
                }
            }
        }
    }
}

UINT8 SysGetSecond()
{
    SYSTEMTIME lt;
    GetSystemTime(&lt);
    return (UINT8)lt.wSecond;
}

void Timerproc(
    HWND unnamedParam1,
    UINT unnamedParam2,
    UINT_PTR unnamedParam3,
    DWORD unnamedParam4
)
{// 未被调用
    MCU_memory[0x201E] |= SYS_FLAG_TIMER_MASK;
}

void SysTimer1Open(UINT8 times)
{
    SysTimer1Close();
    timer = SetTimer(NULL, 1, times*10, Timerproc);
}

void SysTimer1Close()
{
    if (timer != 0)
    {
        KillTimer(NULL, timer);
        timer = 0;
    }
}

void SysIconAllClear(void)
{

}

void DataBankSwitch(UINT8 logicStartBank, UINT8 bankNumber, UINT16 physicalStartBank)
{
    if (logicStartBank == 0x04 && bankNumber == 0x01)
    {
        memcpy(MCU_memory_dummy + _4BankNumber * 0x1000, MCU_memory + 0x4000, bankNumber*0x1000);
        _4BankNumber = physicalStartBank;
        memcpy(MCU_memory + 0x4000, MCU_memory_dummy + _4BankNumber * 0x1000, bankNumber*0x1000);
    }
    else if (logicStartBank == 0x09 && bankNumber == 0x04)
    {
        _9BankNumber = physicalStartBank;
        memcpy(MCU_memory + 0x9000, pGameData + physicalStartBank * 0x1000, bankNumber*0x1000);
    }
    else
    {
        RaiseException(logicStartBank, bankNumber, 0, NULL);
    }
}

void GetDataBankNumber(UINT8 logicStartBank, UINT16* physicalBankNumber)
{
    if (logicStartBank == 0x09)
    {
        *physicalBankNumber = _9BankNumber;
    }
    else
    {
        RaiseException(logicStartBank, 0xFF, 0, NULL);
    }
}

void SysSetKeySound(UINT8 keySoundFlag)
{

}

UINT8   SysGetKeySound()
{
    return 0;
}

UINT8	 SysGetKey()
{
    MSG msg;
    if (PeekMessage(&msg, NULL, WM_NULL, WM_NULL, PM_REMOVE))
    {
        if (msg.message == WM_KEYDOWN)
        {
            UINT8 keycode = 0xFF;
            switch (LOBYTE(msg.wParam))
            {
            case VK_UP:
                keycode = KEY_UP;
                break;
            case VK_DOWN:
                keycode = KEY_DOWN;
                break;
            case VK_LEFT:
                keycode = KEY_LEFT;
                break;
            case VK_RIGHT:
                keycode = KEY_RIGHT;
                break;
            case VK_PRIOR:
                keycode = KEY_PGUP;
                break;
            case VK_NEXT:
                keycode = KEY_PGDN;
                break;
            case VK_RETURN:
                keycode = KEY_ENTER;
                break;
            case VK_ESCAPE:
                keycode = KEY_EXIT;
                break;
            case VK_SPACE:
                keycode = KEY_SPACE;
                break;
            case 0x41:
                keycode = KEY_A;
                break;
            case 0x44:
                keycode = KEY_D;
                break;
            case 0x52:
                keycode = KEY_R;
                break;
            case 0x45:
                keycode = KEY_E;
                break;
            case 0x57:
                keycode = KEY_W;
                break;
            }
            return keycode;
        }
        else if (msg.message == WM_TIMER)
        {
            //SysTimer1Close();
            MCU_memory[0x201E] |= SYS_FLAG_TIMER_MASK;
        }
    }
    return 0xFF;
}

void SysPlayMelody(UINT8 melodyNum)
{

}

void SysStopMelody()
{

}

void SysMemInit(UINT16 start, UINT16 len)
{
    fillmem(MCU_memory + start, len, 0x00);
    UINT16 _17CD = start & MIN_BLK_MASK;
    if (_17CD)
    {
        start = (start + MIN_BLK_BYTES) & MIN_BLK_NMASK;
        len -= _17CD;
    }
    len &= MIN_BLK_NMASK;
    Mem_Start = start;
    Mem_Len = len;
    MCB* pMcb = (MCB*)(MCU_memory + start);
    pMcb->use_flag = MCB_BLANK;
    pMcb->end_flag = MCB_END;
    pMcb->len = len - MCB_LENGTH;
    Mem_Flag = MEM_OK;
}

UINT8 _00EA3234(MCB* _17D0, UINT16 _17D2) // Mem_MCB_Break
{
    if (_17D0->len - _17D2 <= 0x0004)
    {
        return 0x01;
    }
    UINT8 _17C4 = _17D0->use_flag;
    UINT8 _17C5 = _17D0->end_flag;
    UINT16 _17CA = _17D0->len;
    _17D0->len = _17D2;
    _17D0->end_flag = MCB_NORMAL;
    UINT16 _17C6 = _17CA - _17D2 - 0x0004;
    UINT8* _17CC = (UINT8*)_17D0 + _17D2 + 0x0004;
    _17D0 = (MCB*)_17CC;
    _17D0->end_flag = _17C5;
    _17D0->use_flag = _17C4;
    _17D0->len = _17C6;
    return 0x01;
}

UINT8 Mem_MCB_Valid(MCB* _17D0)
{
    if ((_17D0->end_flag == MCB_NORMAL) || (_17D0->end_flag == MCB_END))
    {
        UINT8* _17CC = (UINT8*)_17D0 + 0x0004 + _17D0->len;
        if ((_17CC > MCU_memory + Mem_Start + Mem_Len) || ((UINT8*)_17D0 < MCU_memory + Mem_Start))
        {
            Mem_Flag = MEM_MCB_ERROR;
            return 0x00;
        }
        else
        {
            return 0x01;
        }
    }
    else
    {
        Mem_Flag = MEM_MCB_ERROR;
        return 0x00;
    }
}

MCB* Mem_MCB_Next(MCB* _17D0)
{
    MCB* _17CE = _17D0;
    if (((UINT8*)_17D0 < MCU_memory + Mem_Start) || ((UINT8*)_17D0 > MCU_memory + Mem_Start + Mem_Len))
    {
        return NULL;
    }
    if (_17CE->end_flag == MCB_END)
    {
        return NULL;
    }
    if (Mem_MCB_Valid(_17CE) == 0x00)
    {
        return NULL;
    }
    _17D0 = (MCB*)(_17CE->len + (UINT8*)_17D0 + 0x0004);
    if (Mem_MCB_Valid(_17D0) == 0x00)
    {
        return NULL;
    }
    return _17D0;
}

UINT8 Mem_MCB_Merge()
{
    MCB* _17CA = (MCB*)(MCU_memory + Mem_Start);
    MCB* _17C8 = (MCB*)(MCU_memory + Mem_Start);
    while (_17CA->end_flag == MCB_NORMAL)
    {
        MCB* _17CC = Mem_MCB_Next(_17CA);
        if (_17CC == NULL)
        {
            return 0x01;
        }
        MCB* _17C8 = _17CC;
        if ((_17CA->use_flag == MCB_BLANK) && (_17C8->use_flag == MCB_BLANK))
        {
            _17CA->len = _17C8->len + _17CA->len + 0x0004;
            _17CA->end_flag = _17C8->end_flag;
        }
        else
        {
            _17CA = _17C8;
        }
    }
    return 0x01;
}

char* SysMemAllocate(UINT16 len)
{
    if (len & 0x0003)
    {
        len = (len + 0x0004) & 0xFFFC;
    }
    MCB* _17CE = (MCB*)(MCU_memory + Mem_Start);
    while ((_17CE->use_flag != MCB_BLANK) || (_17CE->len - 0x0004 < len))
    {
        MCB* _17CC = Mem_MCB_Next(_17CE);
        if (_17CC == NULL)
        {
            return NULL;
        }
        else
        {
            _17CE = _17CC;
        }
    }
    _00EA3234(_17CE, len);
    _17CE->use_flag = MCB_USE;
    return (char*)(_17CE + 1);
}

UINT8 SysMemFree(char* p)
{
    if ((p - 0x0004 < (char*)(MCU_memory + Mem_Start))
        || (p - 0x0004 > (char*)(MCU_memory + Mem_Start + Mem_Len)))
    {
        return 0x00;
    }
    for (MCB* _17CC = (MCB*)(MCU_memory + Mem_Start); _17CC != NULL; _17CC = Mem_MCB_Next(_17CC))
    {
        if (p - 0x0004 == (char*)_17CC)
        {
            if (_17CC->use_flag == MCB_BLANK)
            {
                return 0x01;
            }
            else
            {
                _17CC->use_flag = MCB_BLANK;
                Mem_MCB_Merge();
                return 0x01;
            }
        }
    }
    return 0x00;
}

UINT16 SysRand(PtrRandEnv pRandEnv)
{
    pRandEnv->next = pRandEnv->next * 0x41C64E6D + 0x00003039;
    return (pRandEnv->next / 0x00010000) % (pRandEnv->randMax + 0x0001);
}

void SysSrand(PtrRandEnv pRandEnv, UINT16 seed, UINT16  randMax)
{
    pRandEnv->next = seed;
    pRandEnv->randMax = randMax;
}

void SysMemcpy(UINT8* dest, const UINT8* src, UINT16 len)
{
    memcpy(dest, src, len);
}

UINT8 SysMemcmp(UINT8* dest, const UINT8* src, UINT16 len)
{
    return memcmp(dest, src, len);
}

void GuiSetInputFilter(UINT8 filter)
{

}

void GuiSetKbdType(UINT8 type)
{

}

UINT8      GuiPushMsg(PtrMsg pMsg)
{
    if (MCU_memory[0x2B0B] < 0x08)
    {
        MCU_memory[0x2B09]--;
        MCU_memory[0x2B09] &= 0x07;
        ((PtrMsg)(MCU_memory + 0x2B0F))[MCU_memory[0x2B09]].param = pMsg->param;
        ((PtrMsg)(MCU_memory + 0x2B0F))[MCU_memory[0x2B09]].type = pMsg->type;
        MCU_memory[0x2B0B]++;
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

UINT8 GuiGetMsg(PtrMsg pMsg)
{
    while (1)
    {
        if (MCU_memory[0x201E] & SYS_FLAG_TIMER_MASK)
        {
            MCU_memory[0x201E] &= ~SYS_FLAG_TIMER_MASK;
            pMsg->type = DICT_WM_TIMER;
            pMsg->param = 0x0000;
            return 0x01;
        }
        if (MCU_memory[0x2B0B])
        {
        _51E6:
            pMsg->type = ((PtrMsg)(MCU_memory + 0x2B0F))[MCU_memory[0x2B09]].type;
            pMsg->param = ((PtrMsg)(MCU_memory + 0x2B0F))[MCU_memory[0x2B09]].param;
            MCU_memory[0x2B0B]--;
            MCU_memory[0x2B0B] &= 0x07;
            MCU_memory[0x2B09]++;
            MCU_memory[0x2B09] &= 0x07;
            return 0x01;
        }
        UINT8 _17CD = SysGetKey();
        if (_17CD != 0xFF)
        {
            pMsg->type = DICT_WM_KEY;
            pMsg->param = _17CD;
            return 0x01;
        }
    }
    return 0x00;
}

UINT8 GuiTranslateMsg(PtrMsg pMsg)
{
    UINT8 ConstKeybdMap[0x40][2] = {
        {DICT_WM_CHAR_FUN,CHAR_ON_OFF},{DICT_WM_CHAR_FUN,CHAR_HOME_MENU},{DICT_WM_CHAR_FUN,CHAR_EC_DICT},{DICT_WM_CHAR_FUN,CHAR_CE_DICT},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},
        {DICT_WM_CHAR_ASC,'1'},{DICT_WM_CHAR_ASC,'2'},{DICT_WM_CHAR_ASC,'3'},{DICT_WM_CHAR_ASC,'4'},{DICT_WM_CHAR_ASC,'5'},{DICT_WM_CHAR_ASC,'6'},{DICT_WM_CHAR_ASC,'7'},{DICT_WM_CHAR_ASC,'8'},
        {DICT_WM_CHAR_ASC,'q'},{DICT_WM_CHAR_ASC,'w'},{DICT_WM_CHAR_ASC,'e'},{DICT_WM_CHAR_ASC,'r'},{DICT_WM_CHAR_ASC,'t'},{DICT_WM_CHAR_ASC,'y'},{DICT_WM_CHAR_ASC,'u'},{DICT_WM_CHAR_ASC,'i'},
        {DICT_WM_CHAR_ASC,'a'},{DICT_WM_CHAR_ASC,'s'},{DICT_WM_CHAR_ASC,'d'},{DICT_WM_CHAR_ASC,'f'},{DICT_WM_CHAR_ASC,'g'},{DICT_WM_CHAR_ASC,'h'},{DICT_WM_CHAR_ASC,'j'},{DICT_WM_CHAR_ASC,'k'},
        {DICT_WM_CHAR_FUN,CHAR_INPUT},{DICT_WM_CHAR_ASC,'z'},{DICT_WM_CHAR_ASC,'x'},{DICT_WM_CHAR_ASC,'c'},{DICT_WM_CHAR_ASC,'v'},{DICT_WM_CHAR_ASC,'b'},{DICT_WM_CHAR_ASC,'n'},{DICT_WM_CHAR_ASC,'m'},
        {DICT_WM_CHAR_FUN,CHAR_ZY},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_CHAR_FUN,CHAR_SHIFT},{DICT_WM_CHAR_FUN,CHAR_EXIT},{DICT_WM_CHAR_FUN,CHAR_ENTER},
        {DICT_WM_CHAR_ASC,'9'},{DICT_WM_CHAR_ASC,'0'},{DICT_WM_CHAR_ASC,'o'},{DICT_WM_CHAR_ASC,'p'},{DICT_WM_CHAR_ASC,'l'},{DICT_WM_CHAR_FUN,CHAR_UP},{DICT_WM_CHAR_ASC,' '},{DICT_WM_CHAR_FUN,CHAR_LEFT},
        {DICT_WM_CHAR_FUN,CHAR_DOWN},{DICT_WM_CHAR_FUN,CHAR_RIGHT},{DICT_WM_CHAR_FUN,CHAR_PGUP},{DICT_WM_CHAR_FUN,CHAR_PGDN},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00},{DICT_WM_DUMMY,0x00}};
    if (pMsg->type != DICT_WM_KEY)
    {
        return 0x01;
    }
    UINT8 _17CD = (UINT8)(pMsg->param);
    pMsg->type = ConstKeybdMap[_17CD & 0x3F][0];
    pMsg->param = ConstKeybdMap[_17CD & 0x3F][1];
    return 0x01;
}

UINT8 GuiInit(void)
{
    FillMemory(MCU_memory+0x400, 0xC00, 0xFF);
    return 0;
}

UINT16 GuiGetKbdState()
{
    return 0;
}

void GuiSetKbdState(UINT16 state)
{

}
void SysCalcScrBufSize(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT16* byteNum)
{
    if (x2 >= 160)
    {
        *byteNum = 0x00;
        return;
    }
    if (x2 < x1)
    {
        UINT8 x = x1;
        x1 = x2;
        x2 = x;
    }
    if (y2 >= 96)
    {
        *byteNum = 0x00;
        return;
    }
    if (y2 < y1)
    {
        UINT8 x = y1;
        y1 = y2;
        y2 = x;
    }
_7B16:
    y2 -= y1;
    y2++;
    x1 = (x1 & 0xF8) >> 3;
    x2 = (x2 & 0xF8) >> 3;
    x2 -= x1;
    x2++;
    *byteNum = x2 * y2;
}

// nTimeout单位10ms
UINT8 GuiMsgBox(UINT8* strMsg, UINT16 nTimeout)
{
    //LOG("GuiMsgBox: %s\n", strMsg);
    UINT8 _17BA[20];
    UINT8 _17B9;
    UINT8 _17B8;
    UINT8 _17B7;
    UINT8* _17A9[7];
    UINT8* _17A7;
    UINT8 _17A6;
    UINT8 _17A5;
    UINT8 _17A4;
    UINT8 _17A3;
    UINT8 _17A2;
    UINT8* _17A0 = NULL;
    MsgType _179D;
    UINT16 _179B=0;
    UINT16 _1799;
    UINT8 _1798 = 0x01;
    UINT8 _1797=0;
    _17A4 = (UINT8)strlen((char*)strMsg);
    if (_17A4 == 0x00)
    {
    _81E9:
        return 0xFF;
    }
_81EE:
    _17A6 = 0x00;
    _17A7 = strMsg;
    _17A5 = 0x00;
    _17B7 = 0x00;
    _17A9[_17B7] = _17A7;
    while ((_17B7 < 0x05) && (_17A7[0]))
    {
    _8279:
        if (_17A7[0] > 0x80)
        {
        _8296:
            _17A5++;
        }
        else
        {
        _82A4:
            _17A5 = 0x00;
        }
    _82AA:
        _17A6++;
        if (_17A6 == 0x0010)
        {
        _82CF:
            _17B7++;
            if ((_17A5 % 0x02) == 0x00)
            {
            _82EA:
                _17A9[_17B7] = _17A7 + 0x0001;
                _17A6 = 0x00;
                _17A5 = 0x00;
            }
            else
            {
            _8366:
                _17A9[_17B7] = _17A7;
                _17A6 = 0x01;
                _17A5 = 0x01;
            }
        }
    _83B4:
        _17A7++;
    }
_83E2:
    if (_17B7 < 0x05)
    {
    _83EE:
        _17A9[_17B7+0x01] = _17A7;
    }
_8433:
    if (_17B7 >= 0x05)
    {
    _843F:
        _17B7--;
    }
_844A:
    _17B9 = (0x5A - (0x10 * (_17B7 + 0x01))) / 0x02;
    _17B8 = _17B9 + 0x04 + (0x10 * (_17B7 + 0x01));
_849A:
    SysCalcScrBufSize(0x0B, _17B9, 0x94, _17B8 + 0x02, &_179B);
    _17A0 = (UINT8*)SysMemAllocate(_179B);
    if (_17A0 == 0x0000)
    {
    _853C:
        return 0xFE;
    }
_8541:
    SysSaveScreen(0x0B, _17B9, 0x94, _17B8 + 0x02, _17A0);
_8592:
    SysLcdPartClear(0x0B, _17B9, 0x94, _17B8 + 0x02);
    for (_17A3 = 0x00; _17A3 <= _17B7; _17A3++)
    {
    _85F4:
        _17A2 = (UINT8)(_17A9[_17A3 + 0x0001] - _17A9[_17A3]);
        strncpy((char*)_17BA, (char*)_17A9[_17A3], _17A2);
        _17BA[_17A2] = 0x00;
        SysPrintString(0x0F, _17A3 * 0x10 + _17B9 + 0x02, _17BA);
    }
_8772:
    SysRect(0x0B, _17B9, 0x92, _17B8);
    SysFillRect(0x0E, _17B8, 0x92, _17B8 + 0x02);
    SysFillRect(0x92, _17B9 + 0x03, 0x94, _17B8 + 0x02);
    if (nTimeout)
    {
        if (timer != 0)
        {
            _1797 = 1;
        }
    _8874:
        SysTimer1Open(0x01);
        _1799 = 0x0000;
    }
_888A:
    while (0x01)
    {
    _8891:
        if (GuiGetMsg(&_179D))
        {
        _88C9:
            if (_179D.type == DICT_WM_KEY)
            {
                break;
            }
        _88D8:
            if (_179D.type == DICT_WM_COMMAND)
            {
            _88E4:
                if ((_179D.param & 0xFF) == CMD_RETURN_HOME)
                {
                _88F9:
                    GuiPushMsg(&_179D);
                    _1798 = 0xFD;
                    break;
                }
            }
        _892D:
            if (_179D.type == DICT_WM_TIMER)
            {
            _8939:
                if (nTimeout)
                {
                _8954:
                    _1799 += 0x0001;
                    if (_1799 == nTimeout)
                    {
                    _8995:
                        break;
                    }
                }
            }
        }
    }
_899B:
    if (nTimeout)
    {
    _89B6:
        SysTimer1Close();
        if (_1797 == 0x01)
        {
        _89CD:
            SysTimer1Open(_1797);
        }
    }
_89E8:
    SysRestoreScreen(0x0B, _17B9, 0x94, _17B8 + 0x02, _17A0);
    SysMemFree((char*)_17A0);
_8A62:
    return _1798;
}
/*
;功能    :文件创建函数
;U8 	FileCreat( U8 filetype,U32 filelength, U8 * information,U16 * filename,U8 * filehandle);
;入口    :U8 filetype,U32 filelength, U8 * information;
;出口    :U16 *filename,U8 *filehandle;
;堆栈使用:无
;全局变量:无
; 说明:
; 创建一个文件,分配空间,并打开该文件, 文件指针指向文件的第一的字节.
; U8	information[10],在同类型文件中是唯一的.
*/
UINT8 FileCreat(UINT8 filetype, UINT32 filelength, UINT8* information, UINT16* filename, UINT8* filehandle)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == 0)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    CHAR cFileName[32];
    sprintf(cFileName, "%s\\%s.%02X", pGameData + 3, information, filetype);
    if (!CreateDirectoryA(pGameData + 3, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        return 0;
    }
    LOG("CreateFile: %s\n", cFileName);
    HANDLE hFile = CreateFileA(cFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return 0;
    }
    SetFilePointer(hFile, filelength, NULL, FILE_BEGIN);
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    fileinfos[i].filename = i + 1;
    memcpy(fileinfos[i].information, information, 10);
    fileinfos[i].filetype = filetype;
    fileinfos[i].hFile = hFile;
    *filename = fileinfos[i].filename;
    *filehandle = fileinfos[i].filename;
    return 1;
}
/*
;功能    :打开一个文件
;U8  FileOpen(U16 filename, U8 filetype,U8 openmode,U8 * filehandle,U8 * filelength);
;入口    :U16 filename, U8 filetype,U8 openmode
;出口    :U8 * filehandle
;堆栈使用:无
;全局变量:无
; 说明:
; 打开一个文件.分配文件句柄filehandle.
; U8 openmode三种打开方式:
;	#define NoOpen		 0x00
;	#define	ReadOnly	 0x01
;	#define	ReadAndWrite	 0x02
; 支持同时打开四个文件; 文件指针指向文件的第一的字节.
*/
UINT8 FileOpen(UINT16 filename, UINT8 filetype, UINT8 openmode, UINT8* filehandle, UINT32* filelength)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == filename)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    CHAR cFileName[32];
    sprintf(cFileName, "%s\\%s.%02X", pGameData + 3, fileinfos[i].information, filetype);
    LOG("CreateFile: %s\n", cFileName);
    HANDLE hFile = CreateFileA(cFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return 0;
    }
    fileinfos[i].hFile = hFile;
    *filehandle = fileinfos[i].filename;
    *filelength = GetFileSize(hFile, NULL);
    return 1;
}
/*
;功能    :删除一个文件
;U8 filedel(U8 filehandle);
;入口    :无
;出口    :U8 filehandle
;堆栈使用:无
;全局变量:无
; 说明:
; 删除一个文件,并且关闭此文件.
*/
UINT8 FileDel(UINT8 filehandle)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == filehandle)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    LOG("CloseHandle: %s.%02X\n", fileinfos[i].information, fileinfos[i].filetype);
    CloseHandle(fileinfos[i].hFile);
    fileinfos[i].hFile = NULL;
    fileinfos[i].filename = 0;
    CHAR cFileName[32];
    sprintf(cFileName, "%s\\%s.%02X", pGameData + 3, fileinfos[i].information, fileinfos[i].filetype);
    LOG("DeleteFile: %s\n", cFileName);
    DeleteFileA(cFileName);
    return 0;
}
/*
;功能    :向一个文件写数据
;U8 	FileWrite(U8 filehandle,U8 datalength,U8 * bufadd);
;入口    :U8 filehandle,U8 datalength,U8 * bufadd
;出口    :无
;堆栈使用:无
;全局变量:无
; 说明:
; 写完后,文件指针指向说写数据的下一个字节
*/
UINT8 FileWrite(UINT8 filehandle, UINT8 datalength, UINT8* bufadd)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == filehandle)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    DWORD nNumberOfBytesWritten = 0;
    WriteFile(fileinfos[i].hFile, bufadd, datalength, &nNumberOfBytesWritten, NULL);
    if (datalength != nNumberOfBytesWritten)
    {
        return 0;
    }
    return 1;
}
/*
;功能    :关闭一个文件
;U8 fileclose(U8 filehandle);
;入口    :U8 filehandle
;出口    :无
;堆栈使用:无
;全局变量:无
; 说明:	关闭一个文件.支持同时打开四个文件.
*/
UINT8 FileClose(UINT8 filehandle)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == filehandle)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    LOG("CloseHandle: %s.%02X\n", fileinfos[i].information, fileinfos[i].filetype);
    CloseHandle(fileinfos[i].hFile);
    fileinfos[i].hFile = NULL;
    return 1;
}
/*
;功能    :读一个文件的数据
;U8 FileRead(U8 filehandle,U8 datalength,U8 * bufadd);
;入口    :U8 filehandle,U8 datalength,U8 * bufadd
;出口    :无
;堆栈使用:无
;全局变量:无
; 说明:
; 写完后,文件指针指向说读数据的下一个字节
*/
UINT8 FileRead(UINT8 filehandle, UINT8 datalength, UINT8* bufadd)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == filehandle)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    DWORD nNumberOfBytesRead = 0;
    ReadFile(fileinfos[i].hFile, bufadd, datalength, &nNumberOfBytesRead, NULL);
    if (datalength != nNumberOfBytesRead)
    {
        return 0;
    }
    return 1;
}
/*
;功能    :文件定位函数
;U8 FileSeek(U8 filehandle,U32 fileoffset,U8 origin);
;入口    :U8 filehandle,U32 fileoffset,U8 origin
;出口    :无
;堆栈使用:无
;全局变量:无
; 说明:
;      	origin:
;		#define FromTop		0x01
;		#define	FromCurrent	0x02
;		#define	FromEnd		0x03
;   	fileoffset:
; 		可以为负数.
*/
UINT8 FileSeek(UINT8 filehandle, UINT32 fileoffset, UINT8 origin)
{
    int i = 0;
    for (; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filename == filehandle)
        {
            break;
        }
    }
    if (FILE_NUM == i)
    {
        return 0;
    }
    DWORD dwMoveMethod = FILE_BEGIN;
    switch (origin)
    {
    case FromTop:
        dwMoveMethod = FILE_BEGIN;
        break;
    case FromCurrent:
        dwMoveMethod = FILE_CURRENT;
        break;
    case FromEnd:
        dwMoveMethod = FILE_END;
        break;
    }
    SetFilePointer(fileinfos[i].hFile, (INT32)fileoffset, NULL, dwMoveMethod);
    return 0;
}
/*
;功能    :初始化FLASH的一些变量
;void	FlashInit();
;入口    :无
;出口    :无
;堆栈使用:无
;全局变量:无
; 说明:
; 开机时调用.(进入个模块时也可以调用,可避免别的模块的干扰).
*/
void FlashInit()
{
    for (int i = 0; i < FILE_NUM; i++)
    {
        fileinfos[i].filename = 0;
    }
    CHAR cFileName[32];
    WIN32_FIND_DATAA dFindFileData;
    sprintf(cFileName, "%s\\*", pGameData + 3);
    HANDLE hFile = FindFirstFileA(cFileName, &dFindFileData);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return;
    }
    int i = 0;
    do {
        UINT8 filetype = 0;
        if (strcmp(dFindFileData.cFileName, ".") == 0 || strcmp(dFindFileData.cFileName, "..") == 0)
        {
            continue;
        }
        fileinfos[i].filename = i + 1;
        memset(fileinfos[i].information, 0, ARRAYSIZE(fileinfos[i].information));
        char* pDot = strrchr(dFindFileData.cFileName, '.');
        if (pDot == NULL)
        {
            strcpy(fileinfos[i].information, dFindFileData.cFileName);
        }
        else
        {
            memcpy(fileinfos[i].information, dFindFileData.cFileName, pDot - dFindFileData.cFileName);
            sscanf(pDot + 1, "%hhX", &filetype);
        }
        fileinfos[i].filetype = filetype;
        i++;
    } while (FindNextFileA(hFile, &dFindFileData));
}
/*
;功能    :返回某类型文件的数量.
;U8 	Filenum( U8 filetype,U16 * filenum);
;入口    :U8 filetype;
;出口    :U16 * filenum
;堆栈使用:无
;全局变量:无
; 说明:
; 返回某类型文件的数量.
*/
UINT8 FileNum(UINT8 filetype, UINT16* filenum)
{
    *filenum = 0;
    for (int i = 0; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filetype == filetype)
        {
            *filenum += 1;
        }
    }
    return *filenum>0?1:0;
}
/*
;功能    :浏览 某类型的第fileorder个文件的information.
;U8 	FileSearch(U8 filetype,U16 fileorder,U16 * filename,U8 * information);
;入口    :U8 fls_filetype,U16 fileorder;
;出口    :U16 * filename,U8 * information;
;堆栈使用:无
;全局变量:无
; 说明:
; 通过比较information[10],知道是否是要打开的文件.
*/
UINT8 FileSearch(UINT8 filetype, UINT16 fileorder, UINT16* filename, UINT8* information)
{
    for (int i = 0; i < FILE_NUM; i++)
    {
        if (fileinfos[i].filetype == filetype && fileorder == 1)
        {
            *filename = fileinfos[i].filename;
            memcpy(information, fileinfos[i].information, 10);
            return 1;
        }
        fileorder--;
        if (fileorder == 0)
        {
            break;
        }
    }
    return 0;
}
