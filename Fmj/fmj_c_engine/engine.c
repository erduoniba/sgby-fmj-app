#include "engine.h"
#include "middle.h"
#include <stdlib.h>
#include <string.h>

UINT8 MCU_memory_dummy[0x8000];
UINT8 MCU_memory[0x10000];

//UINT8 main(void* para1, void* para2)
void _00200046()
{
    // 初始化所有变量
    _00200247();
    // 播放游戏启动特效
    _00200B96();
    while (MCU_memory[0x1935] == 0x00)
    {
        _5054:
        // 引擎菜单处理
        _00200C79();
        __8D93();
        if (MCU_memory[0x1935] == 0xFD)
        {
            _506D:
            MCU_memory[0x1935] = 0x00;
            __8EE9(0x02);
        }
    }
    _5082:
    _002000C4();
    __8EE9(0x07);
    SysMemInit(0x2C00, 0x1400);
}

void _002000C4()
{
    UINT8 _17C1[0x14];
    UINT8 _17BC[6];
    UINT8 _742E[] = "RunErr:";
    if (MCU_memory[0x1935] == 0x00)
    {
        return;
    }
    if (MCU_memory[0x1935] == 0xFE || MCU_memory[0x1935] == 0xFD || MCU_memory[0x1935] == 0xFF)
    {
        MCU_memory[0x1935] = 0x00;
        return;
    }
    fillmem(_17C1, 0x0014, 0x00);
    _SysMemcpy(_17C1, _742E, 0x0007);
    _itoa(MCU_memory[0x1935], _17BC, 0x000A);
    strcat(_17C1, _17BC);
    SysPrintString(0x00, 0x00, _17C1);
    __8F16();
}

void _00200247()
{
    UINT8 _17CC;
    GuiInit();
    FlashInit();
    SysMemInit(0x2C00, 0x1400);
    SysIconAllClear();
    *(UINT16*)(MCU_memory+0x1933) = GuiGetKbdState();
    GuiSetInputFilter(0x03);
    GuiSetKbdType(0x03);
    MCU_memory[0x192E] = SysGetKeySound();
    SysSetKeySound(0x00);
    SysTimer1Open(0x32);
    _DataBankSwitch(0x04, 0x01, 0x0004);
    _GetDataBankNumber(0x09, (UINT16*)(MCU_memory + 0x1931));
    _SysMemcpy(MCU_memory+0x1938, MCU_memory + 0x9000 +3, 0x0008); // _9003指向游戏名
    MCU_memory[0x1940] = 0x00;
    MCU_memory[0x1A94] = 0x00;
    MCU_memory[0x1935] = 0x00;
    UINT8 _17CD = 0x19;
    SysSrand((PtrRandEnv)(MCU_memory + 0x1928), SysGetSecond(), 2000);
    for (_17CC = 0x00; _17CC<0x03; _17CC++)
    {
        _5423:
        fillmem(MCU_memory+0x1988+_17CC*0x0019, _17CD, 0x00);
    }
    for (_17CC = 0x00; _17CC<40; _17CC++)
    {
        _54BB:
        *(UINT16*)(MCU_memory+0x19D3+(_17CC<<1)) = 0x0000;
    }
    _17CD = 0x33;
    for (_17CC = 0x00; _17CC<0x03; _17CC++)
    {
        _551B:
        fillmem(MCU_memory+0x1826+_17CC*0x0033, _17CD, 0x00);
    }
_5592:
    // 可能出现的敌人的种类
    fillmem(MCU_memory+0x18E4, 0x0008, 0x00);
    *(UINT16*)(MCU_memory + 0x1AA9) = (UINT16)(SysMemAllocate(302) - MCU_memory);
    if (*(UINT16*)(MCU_memory+0x1AA9) == 0x0000)
    {
        _5628:
        MCU_memory[0x1935] = MEM_ERR;
    }
    else
    {
        _5630:
        fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1AA9), 300, 0x00);
    }
    _5668:
    *(UINT16*)(MCU_memory + 0x1AA7) = (UINT16)(SysMemAllocate(0x0100) - MCU_memory);
    if (*(UINT16*)(MCU_memory+0x1AA7) == 0x0000)
    {
        _56B2:
        MCU_memory[0x1935] = MEM_ERR;
    }
    else
    {
        _56BA:
        fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1AA7), 0x0100, 0x00);
    }
    _56F2:
    *(UINT16*)(MCU_memory + 0x1936) = (UINT16)(SysMemAllocate(0x0794) - MCU_memory);
    if (*(UINT16*)(MCU_memory+0x1936) == 0x0000)
    {
        _573C:
        MCU_memory[0x1935] = MEM_ERR;
    }
    _5741:
    *(UINT16*)(MCU_memory+0x192F) = 0x4A10;
    _17CD = 0x34;
    for (_17CC = 0x00; _17CC<0x03; _17CC++)
    {
        _5772:
        *(UINT16*)(MCU_memory + 0x181E + (_17CC<<1)) = 0x4C70+_17CC*_17CD;
        fillmem(MCU_memory + *(UINT16*)(MCU_memory + 0x181E + (_17CC<<1)), _17CD, 0x00);
    }
    _584E:
    *(UINT16*)(MCU_memory+0x1824) = 0x4C70+_17CC*_17CD;
    fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1824), _17CD, 0x00);
    MCU_memory[0x1AAC] = 0x01;
    MCU_memory[0x1AAB] = 0x01;
    MCU_memory[0x1A96] = 0x00; // 不遇怪
    UINT32 _7436 = 0x00000000;
    *(UINT32*)(MCU_memory+0x1A8F) = _7436; // 玩家拥有的金钱
    MCU_memory[0x1A95] = 0x00; // 背包中的道具种类数
    MCU_memory[0x1A93] = 0x00;
    MCU_memory[0x1A97] = 0x00; // 音乐号码
    MCU_memory[0x1A98] = 0x00; // 音乐持续次数
    MCU_memory[0x1AAD] = 0x01; // 音乐开
    MCU_memory[0x194D] = 0xFF;
    MCU_memory[0x1A99] = 0x00; // 关闭自动连续行走
    MCU_memory[0x197B] = 0x00; // 取消显示主角在当前地图的坐标功能
    // 默认能存档
    MCU_memory[0x1A9B] = 0x01;
    MCU_memory[0x1A9C] = 0x00; // 定时触发事件
    *(UINT16*)(MCU_memory + 0x1A9D) = 0x0000; // 定时触发事件的定时时间
    MCU_memory[0x1A9F] = 0x00;
}

void _00200942(UINT8 _17CF)
{
    if (MCU_memory[0x1935])
    {
        return;
    }
    if (_17CF&0x01)
    {
        for (UINT8 _17CE = 0x00; _17CE<0x28; _17CE++)
        {
            if (*(UINT16*)(MCU_memory+0x19D3+(_17CE<<1)) == 0x0000)
            {
                continue;
            }
            __8DDF(_17CE);
        }
    }
    if (_17CF&0x02)
    {
        for (UINT8 _17CE = 0x00; _17CE<0x03; _17CE++)
        {
            // 角色类型
            if (MCU_memory[0x1988+_17CE*0x0019] == 0x00)
            {
                continue;
            }
            //      角色类型
            __8DDC(MCU_memory[0x1988+_17CE*0x0019]);
        }
    }
    if (_17CF&0x04)
    {
        __8DBE();
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1936)) == 0x00)
        {
            MCU_memory[0x1935] = MEM_ERR;
        }
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1AA7)) == 0x00)
        {
            MCU_memory[0x1935] = MEM_ERR;
        }
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1AA9)) == 0x00)
        {
            MCU_memory[0x1935] = MEM_ERR;
        }
        SysTimer1Close();
        SysStopMelody();
        SysSetKeySound(MCU_memory[0x192E]);
        GuiSetKbdState(*(UINT16*)(MCU_memory+0x1933));
    }
}

// 播放游戏启动特效
void _00200B96()
{
	fillmem(MCU_memory + *(UINT16*)(MCU_memory + 0x1936), 0x0780, 0x00);
    // 播放特效
    __8DE5(0x01, 0xF7, 0x01, 0x00, 0x00);
    fillmem(MCU_memory + *(UINT16*)(MCU_memory + 0x1936), 0x0780, 0x00);
    // 播放特效
    __8DE5(0x01, 0xF8, 0x01, 0x00, 0x00);
}

// 引擎菜单处理
void _00200C79()
{
    const UINT32 _7452 = 0x00000000;
    while (MCU_memory[0x1935] == 0x00)
    {
        // 引擎菜单选择
        switch (_00200DF1())
        {
        case 0x0000: // _5CA1 新的征程
            // 删除文件类型为0x17的文件
            __8DBE();
            MCU_memory[0x194D] = 0xFF;
            // 清空屏幕缓冲
            fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780, 0xAA);
            // 清空EVENT
            fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1AA9), 0x012C, 0x00);
            MCU_memory[0x1A96] = 0x00; // 不遇怪
            *(UINT32*)(MCU_memory+0x1A8F) = _7452; // 玩家拥有的金钱
            MCU_memory[0x1A95] = 0x00; // 背包中的道具种类数
            MCU_memory[0x1AAD] = 0x01; // 音乐开
            // 从剧情1-1开始
            _0020239E(0x01, 0x01);
            return;
        case 0x0001: // _5D60 再续前缘
            //
            if (__8EEF() != 0x00)
            {
                return;
            }
            break;
        case 0x0002: // _5D79
            // 从剧情0-1开始
            _0020239E(0x00, 0x01);
            break;
        case 0x0003: // _5D96
            // 从剧情0-2开始
            _0020239E(0x00, 0x02);
            break;
        case 0x0004: // _5DB3
            // 从剧情0-3开始
            _0020239E(0x00, 0x03);
            break;
        case 0x0005: // _5DD0
            // 从剧情0-4开始
            _0020239E(0x00, 0x04);
            break;
        }
    }
}

// 引擎菜单选择界面
UINT8 _00200DF1()
{
    UINT8* _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6; // 当前选择的选项下标
    UINT8 _17C5;
    MsgType _17C2;
    fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780, 0x00);
    // 返回指向背景图片数据的指针
    _17CA = __8F0A(DAT_SUNDRYPIC, 0x02, 0x0E);
    // 计算背景图片居中时的x坐标
    _17C9 = (0x009F- _17CA[0x02])/0x0002;
    // 计算背景图片居中时的y坐标
    _17C8 = (0x60- _17CA[0x03])/0x02;
    // 背景图片居中显示
    __8F07(DAT_SUNDRYPIC, 0x02, 0x0E, 0x00, _17C9, _17C8);
    __8E33();
    _17C6 = 0x00;
    // 获取最大选项下标
    _17C7 = _002010D5();
    while (MCU_memory[0x1935] == 0x00)
    {
        _5F31:
        _17C5 = __8DE5(0x01, 0x00FA+_17C6, 0x03, 0x00, 0x00);
        if (_17C5 == 0xFF)
        {
            _5FA9:
            continue;
        }
        _5FAC:
        _17C2.type = DICT_WM_KEY;
        _17C2.param = _17C5;
        if (GuiTranslateMsg(&_17C2) == 0x00)
        {
            _6026:
            continue;
        }
        _6029:
        if (_17C2.type != DICT_WM_CHAR_FUN)
        {
            _6035:
            continue;
        }
        _6038:
        switch (_17C2.param)
        {
        case CHAR_UP: // _6062
            if (_17C6 == 0x00)
            {
            _606B:
                _17C6 = _17C7;
            }
            else
            {
            _6076:
                _17C6--;
            }
            break;
        case CHAR_DOWN: // _6084
            if (_17C6 == _17C7)
            {
            _6092:
                _17C6 = 0x00;
            }
            else
            {
            _609B:
                _17C6++;
            }
            break;
        case CHAR_ENTER: // _60A9
            return _17C6;
        case CHAR_EXIT: // _60B0
            MCU_memory[0x1935] = 0xFE;
            return 0xFF;
        }
    }
    return 0xFF;
}

UINT8 _002010D5()
{
    UINT8 _17CF = 0x00;
    for (; _17CF<0x06; _17CF++)
    {
        _6108:
        __8F0A(DAT_SPECRES, 0x01, 0x00FA+_17CF);
        if (MCU_memory[0x1935])
        {
            _6156:
            break;
        }
    }
    _615C:
    MCU_memory[0x1935] = 0x00;
    return _17CF-0x01;
}

// 返回存档位置标号，返回值0,1,2
UINT8 _0020117E(UINT8 _17CF)
{
    UINT8 _7466[3][4] = {{0x07,0x1B,0x2E,0x44},{0x07,0x1B,0x2E,0x44},{0x07,0x1B,0x2E,0x44}};
    UINT8 _7472[3][4] = {{0x19,0x19,0x19,0x1C},{0x30,0x30,0x30,0x33},{0x47,0x47,0x47,0x4A}};
    UINT8 _747E[0x0A] = "空档案";
    UINT8 x;
    UINT8 y;
    MsgType _17AD;
    UINT8 _17BE[0x0F];
    UINT8 _17BB[0x03];
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    UINT8* _17B7 = __8F0A(DAT_SUNDRYPIC, 0x02, _17CF);
    UINT8 _17BA = _17B7[0x02];
    UINT8 _17B9 = _17B7[0x03];
    UINT8 _17B6 = (0x009F-_17BA)/0x0002;
    UINT8 _17B5 = (0x60-_17B9)/0x02;
    __8F04(DAT_SUNDRYPIC, 0x02, _17CF, 0x00, _17B6, _17B5);
    for (UINT8 _17B2 = 0x00; _17B2<0x03; _17B2++)
    {
        _62C2:
        if (__8DC4(_17B2, _17BB, _17BE) == 0x00)
        {
            _630B:
            _SysMemcpy(_17BE, _747E, 0x000A);
        }
        else
        {
            _6347:
            for (UINT8 _17B1=0x00; _17B1<0x03; _17B1++)
            {
                _6368:
                if (_17BB[_17B1] == 0x00)
                {
                    _6388:
                    continue;
                }
                _638B:
                x = _7466[_17B2][_17B1]+_17B6;
                y = _7472[_17B2][_17B1]+_17B5-0x03;
                __8F04(DAT_SUNDRYPIC, 0x01, _17BB[_17B1], 0x06, x, y);
            }
        }
        _6494:
        x = _7466[_17B2][0x03]+_17B6;
        y = _7472[_17B2][0x03]+_17B5;
        SysPrintString(x, y, _17BE);
        if (MCU_memory[0x1935])
        {
            _655E:
            return 0xFF;
        }
    }
    _6566:
    SysTimer1Close();
    UINT8 _17B0 = 0x00;
    x = _7466[_17B0][0x03]+_17B6-0x01;
    y = _7472[_17B0][0x03]+_17B5-0x01;
    SysLcdReverse(x, y, x+0x52, y+0x11);
    while (MCU_memory[0x1935] == 0x00)
    {
        _6653:
        if (GuiGetMsg(&_17AD) == 0x00)
        {
            _6688:
            continue;
        }
        _668B:
        if (GuiTranslateMsg(&_17AD) == 0x00)
        {
            _66C0:
            continue;
        }
        _66C3:
        if (_17AD.type == DICT_WM_COMMAND)
        {
            _66CF:
            if (_17AD.param == CMD_RETURN_HOME)
            {
                _66EA:
                MCU_memory[0x1935] = 0xFE;
            }
        }
        _66EF:
        if (_17AD.type != DICT_WM_CHAR_FUN)
        {
            _66FB:
            continue;
        }
        _66FE:
        switch (_17AD.param)
        {
        case CHAR_DOWN: // _6728
            _17B0++;
            break;
        case CHAR_UP: // _6734
            _17B0--;
            break;
        case CHAR_ENTER: // _6740
            SysTimer1Open(0x32);
            return _17B0;
        case CHAR_EXIT: // _6754
            SysTimer1Open(0x32);
            return 0xFE;
        }
        _6766:
        if (_17B0 > 0xF0)
        {
            _6777:
            _17B0 = 0x02;
        }
        _677D:
        if (_17B0 > 0x02)
        {
            _678E:
            _17B0 = 0x00;
        }
        _6794:
        SysLcdReverse(x, y, x+0x52, y+0x11);
        x = _7466[_17B0][0x03]+_17B6-0x01;
        y = _7472[_17B0][0x03]+_17B5-0x01;
        SysLcdReverse(x, y, x+0x52, y+0x11);
    }
    return MCU_memory[0x1935];
}

UINT8 _002018C0()
{
    return 0x01;
}

UINT8 _002018C6()
{
    UINT8 _17CE;
    UINT8 _17CF = _0020117E(0x0F);
    if (_17CF < 0x03)
    {
        _68EB:
        _17CE = __8DCA(_17CF);
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        return _17CE;
    }
    _6930:
    return 0x00;
}

UINT8 _00201948()
{
    UINT8 _17CF;
    UINT8 _17CE;
    UINT8 _17CD;
    while (MCU_memory[0x1935] == 0x00)
    {
        _6960:
        _17CF = _0020117E(0x10);
        if (_17CF < 0x03)
        {
            _6975:
            __8DBE();
            if (__8DC7(_17CF) != 0x00)
            {
                _6997:
                _17CE = MCU_memory[0x1AAC];
                _17CD = MCU_memory[0x1AAB];
                // 从剧情0-5开始
                _0020239E(0x00, 0x05);
                MCU_memory[0x1AAC] = _17CE;
                MCU_memory[0x1AAB] = _17CD;
                SysStopMelody();
                MCU_memory[0x201C] = 0x00;
                fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780, 0xAA);
                __8DBA();
                _00201A53();
                _00201C6C();
                _00201EB2();
                MCU_memory[0x194D] = _17CF;
                return 0x01;
            }
        }
        else
        {
            _6A38:
            return 0x00;
        }
    }
    return MCU_memory[0x1935];
}

void _00201A53()
{
    UINT8 _17C7[6];
    if (MCU_memory[0x1935])
    {
        _6A6B:
        return;
    }
    _6A6E:
    _17C7[0] = DAT_ACTORPIC;
    for (UINT8 _17CD = 0x00; _17CD<0x28; _17CD++)
    {
        _6AAC:
        if (*(UINT16*)(MCU_memory+0x19D3+(_17CD<<1)) == 0x0000)
        {
            continue;
        }
    _6AE6:
        // 角色类型
        _17C7[1] = MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CD << 1))];
        _17C7[2] = MCU_memory[0x194E + _17CD];
        __8F10(_17C7);
        if (MCU_memory[0x1935])
        {
            _6BA1:
            return;
        }
    _6BA4:
        // 角色图片资源数据的bank偏移
        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CD<<1)) + 0x0017] = _17C7[3];
        // 角色图片资源数据映射到0x9000后，相对于0x9000的偏移量
        *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory+0x19D3+(_17CD<<1)) + 0x0018) = *(UINT16*)(_17C7+4);
    }
}

void _00201C6C()
{
    UINT8 _17CC;
    UINT8* _17CA;
    UINT8 _17C4[6];
    if (MCU_memory[0x1935])
    {
        _6C84:
        return;
    }
    _6C87:
    _17C4[0] = DAT_ACTORPIC;
    _17C4[1] = 0x01;
    for (UINT8 _17CD = 0x00; _17CD<0x03; _17CD++)
    {
        _6CE0:
        // 角色资源ID
        _17CC = MCU_memory[0x1988+_17CD*0x0019+0x01];
        if (_17CC == 0x00)
        {
            _6D1F:
            continue;
        }
        _6D22:
        //                          玩家角色 角色资源ID
        _17CA = __8F0A(DAT_ACTORRES, 0x01, _17CC);
        if (MCU_memory[0x1935])
        {
            _6D5E:
            return;
        }
        _6D61:
        // 角色图片编号
        _17C4[2] = _17CA[0x0016];
        __8F10(_17C4);
        if (MCU_memory[0x1935])
        {
            _6DD9:
            return;
        }
        _6DDC:
        // 角色图片数据的bank偏移
        MCU_memory[0x1988+_17CD*0x0019+0x000A] = _17C4[3];
        // 角色图片数据射到0x9000后，相对于0x9000的偏移量
        *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x000B) = *(UINT16*)(_17C4+4);
    }
}

void _00201EB2()
{
    if (MCU_memory[0x1935])
    {
        _6ECA:
        return;
    }
_6ECD:
    //      当前地图的章       当前地图的节    屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
    __8DD6(MCU_memory[0x1979], MCU_memory[0x197A], MCU_memory[0x197C], MCU_memory[0x197D]);
}

UINT8 _00201F1E(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1)
{
    const UINT8 _7495[] = "是";
    const UINT8 _7498[] = "否";
    MsgType _17CC;
    UINT8 _17C8[3];
    SysLcdPartClear(0x1B, 0x0F, 0x89, 0x51);
    SysRect(0x1C, 0x10, 0x84, 0x4C);
    SysFillRect(0x20, 0x4D, 0x88, 0x50);
    SysFillRect(0x85, 0x14, 0x88, 0x50);
    SysRect(0x2C, 0x34, 0x46, 0x46);
    SysRect(0x5C, 0x34, 0x74, 0x46);
    _SysMemcpy(_17C8, _7495, 0x0003);
    SysPrintString(0x30, 0x36, _17C8);
    _SysMemcpy(_17C8, _7498, 0x0003);
    SysPrintString(0x60, 0x36, _17C8);
    SysPrintString(0x22, 0x18, _17D1);
    if (_17CF)
    {
        _7170:
        SysLcdReverse(0x5B, 0x33, 0x75, 0x47);
    }
    else
    {
        _719F:
        SysLcdReverse(0x2B, 0x33, 0x47, 0x47);
    }
    _71CB:
    while (1)
    {
        _71D2:
        if (GuiGetMsg(&_17CC) == 0x00)
        {
            _7207:
            continue;
        }
        _720A:
        if (GuiTranslateMsg(&_17CC) == 0x00)
        {
            _723F:
            continue;
        }
        _7242:
        if (_17CC.type == DICT_WM_COMMAND)
        {
            _724E:
            if (_17CC.param == CMD_RETURN_HOME)
            {
                _7269:
                MCU_memory[0x1935] = 0xFE;
                return 0x01;
            }
        }
        _7273:
        if (_17CC.param == CHAR_LEFT)
        {
            _72BF:
            _17CF = 0x00;
            SysLcdReverse(0x2B, 0x33, 0x47, 0x47);
            SysLcdReverse(0x5B, 0x33, 0x75, 0x47);
        }
        else if (_17CC.param == CHAR_RIGHT)
        {
            _7320:
            _17CF = 0x01;
            SysLcdReverse(0x5B, 0x33, 0x75, 0x47);
            SysLcdReverse(0x2B, 0x33, 0x47, 0x47);
        }
        else if (_17CC.param == CHAR_ENTER)
        {
            _7381:
            return _17CF;
        }
    }
}

// 从剧情_17CF-_17D0开始
void _0020239E(UINT8 _17CF, UINT8 _17D0)
{
    __8F0A(DAT_GUDSCRIPT, _17CF, _17D0);
    if (MCU_memory[0x1935])
    {
        _73D4:
        MCU_memory[0x1935] = 0x00;
        return;
    }
    _73DC:
    MCU_memory[0x1AAC] = _17CF; // 当前章
    MCU_memory[0x1AAB] = _17D0; // 当前节
    // 缓存GUT数据，并初始化GUT数据起始位置和GUT命令偏移量数据
    __8DBA();
    // 执行对应偏移量的指令
    __8DB1(*(UINT16*)(MCU_memory+0x1AA0));
}

void _0020249B(UINT8 _17CF)
{
    if (MCU_memory[0x1935])
    {
        _74A6:
        return;
    }
    _74A9:
    if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))] == 0x04)
    { // 场景对象
        _74DC:
        _002024FE(_17CF);
    }
    else
    { // NPC角色
        _74E6:
        _00202C6D(_17CF);
    }
}

void _002024FE(UINT8 _17CF)
{
    UINT8 _17C8;
    UINT8 _17C7;
    // _17C6、_17C5未初始化？？？
    UINT8 _17C6=0;
    UINT8 _17C5=0;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 width;
    UINT8 height;
    UINT8* _17BE;
    UINT8* _17BC;
    UINT8 _17BB;
    UINT8 _17BA;
    UINT8 _17B9;
    UINT8 x1;
    UINT8 y1;
    UINT16 _17B4;
    UINT8 _17B3;
    UINT8 _17B2;
    UINT8 _17B1;
    UINT8 _17B0;
    UINT16 _17AE;
    UINT16 _17AC;
    if (_17C6 == 0x04 && _17C5 == 0x02)
    {
        _7529:
        __8D96();
    }
    _7534:
    _17C8 = MCU_memory[0x05+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))];
    _17C7 = MCU_memory[0x06+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))];
    //          屏幕左上角定位到地图的x处
    _17C8 -= MCU_memory[0x197C];
    //          屏幕左上角定位到地图的y处
    _17C7 -= MCU_memory[0x197D];
    if (_17C8 > 0x08 && _17C8 < 0x80)
    {
        _75CF:
        return;
    }
    _75D2:
    if (_17C7 > 0x80)
    {
        _75E3:
        return;
    }
    _75E6:
    _17C6 = _17C8;
    _17C5 = _17C7;
    _17BE = _00202F9F(_17CF, &_17BB, &_17BA, &_17AE, &_17B9);
    _17B3 = _17BB>>0x03;
    if (_17BB & 0x07)
    {
        _76B4:
        _17B3++;
    }
    _76BD:
    _17B2 = _17BA>>0x03;
    if (_17BA & 0x07)
    {
        _76D7:
        _17B2++;
    }
    _76E0:
    _17B1 = _17BB>>0x04;
    if (_17BB & 0x0F)
    {
        _76FA:
        _17B1++;
    }
    _7703:
    _17B0 = _17BA>>0x04;
    if (_17BA & 0x0F)
    {
        _771D:
        _17B0++;
    }
    _7726:
    if (_17C8 < 0x80)
    {
        _7732:
        _17C4 = 0x00;
    }
    else
    {
        _773B:
        _17C4 = 0x00-_17C8;
    }
    _7746:
    if (_17C4 >= _17B1)
    {
        _7754:
        return;
    }
    _7757:
    if (_17C7 >= 0x06+_17B0-0x01)
    {
        _776F:
        return;
    }
    _7772:
    if (_17B0 > _17C7+0x01)
    {
        _778C:
        _17C3 = _17B0-(_17C7+0x01);
    }
    else
    {
        _77A3:
        _17C3 = 0x00;
    }
    _77A9:
    if (_17C3 >= _17B0)
    {
        _77B7:
        return;
    }
    _77BA:
    if (_17B1 > 0x09-_17C8)
    {
        _77D4:
        width = 0x09-_17C8;
    }
    else
    {
        _77E2:
        width = _17B1-_17C4;
    }
    _77EF:
    if (0x06 < _17C7+0x01)
    {
        _7802:
        height = _17B0-(_17C7+0x01-0x06);
    }
    else
    {
        _781C:
        height = _17B0-_17C3;
    }
    _7829:
    _17C4 <<= 4;
    _17C3 <<= 4;
    if (_17C8 < 0x80)
    {
        _7853:
        x1 = (_17C8<<4)+0x08;
    }
    else
    {
        _7868:
        x1 = 0x08;
    }
    _786E:
    if (_17C7+0x01 < 0x06)
    {
        _787D:
        y1 = _17C7+0x01- height;
    }
    else
    {
        _7890:
        y1 = 0x06- height;
    }
    _789B:
    width <<= 4;
    if (width +_17C4 > _17BB)
    {
        _78C2:
        width = _17BB-_17C4;
    }
    _78CF:
    height <<= 4;
    y1 <<= 4;
    if (_17BA & 0x0F)
    {
        _78F8:
        if (_17C3+ height > _17BA && _17C3 != 0x00)
        {
            _7919:
            _17C3 = _17BA- height;
        }
        _7926:
        if (_17C3 == 0x00)
        {
        _792F:
            //   当前TILE数据单元的高
            y1 = MCU_memory[0x1984]+ y1 - (_17BA & 0x0F);
            //                                 当前TILE数据单元的高
            height = (_17BA & 0x0F) + height - MCU_memory[0x1984];
        }
    }
    _796D:
    _17BC = MCU_memory+0x4000;
    _17B4 = width >> 0x03;
    if (width &0x07)
    {
        _7992:
        _17B4++;
    }
    _799D:
    _17B4 *= _17B9;
    _17BE = _17BE + _17B3 * _17C3 * _17B9 +_17C4/0x08*_17B9;
    _17AC = _17B3*_17B9;
    for (UINT8 _17C0=0x00; _17C0< height; _17C0++)
    {
        _7AB7:
        _SysMemcpy(_17BC, _17BE, _17B4);
        _17BE+=_17AC;
        _17BC+=_17B4;
    }
    _7B51:
    _17BC = MCU_memory+0x4000;
    if (_17B9 == 0x02)
    {
        _7B68:
        __8DA8(x1, y1, width, height, _17BC);
    }
    else
    {
        _7BAD:
        SysPictureDummy(x1, y1, x1 + width - 0x01, y1 + height - 0x01, _17BC, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
    }
    _7C21:
    if (_17C6+_17B3 > 0x04 && _17C6 <= 0x04 && _17C5 == 0x02)
    {
        _7C51:
        __8D96();
    }
}

void _00202C6D(UINT8 _17CF)
{
    UINT8 y2;
    UINT8 x2;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8* _17C2;
    UINT8 width; // 宽
    UINT8 height; // 高
    UINT8 _17BF;
    UINT16 _17BD;
    // x坐标
    UINT8 x1 = MCU_memory[0x05+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))];
    // y坐标
    UINT8 y1 = MCU_memory[0x06+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))];
    //      屏幕左上角定位到地图的x处  屏幕左上角定位到地图的y处
    if (x1 < MCU_memory[0x197C] || y1 < MCU_memory[0x197D])
    {
        _7D00:
        return;
    }
_7D03:
    //      屏幕左上角定位到地图的x处
    x1 -= MCU_memory[0x197C];
    //      屏幕左上角定位到地图的y处
    y1 -= MCU_memory[0x197D];
    if (x1 >= 0x09 || y1 > 0x06)
    {
        _7D40:
        return;
    }
    _7D43:
    _17C5 = x1;
    _17C4 = y1;
    _17C2 = _00202F9F(_17CF, &width, &height, &_17BD, &_17BF);
    //        当前TILE数据单元的宽
    x1 = x1 * MCU_memory[0x1983] + 0x08;
    if (y1 == 0x00)
    {
        _7E13:
        _17C2 += (_17BD>>0x0001);
        height >>= 0x01;
    }
_7E63:
    //               当前TILE数据单元的高
    _17C6 = height - MCU_memory[0x1984];
    if (y1 == 0x06)
    {
        _7E7F:
        height >>= 0x01;
    }
_7E8E:
    //        当前TILE数据单元的高
    y1 = y1 * MCU_memory[0x1984] - _17C6;
    if (_17BF == 0x02)
    {
        _7EB2:
        __8DA8(x1, y1, width, height, _17C2);
    }
    else
    {
        _7EF7:
        x2 = x1 + width -0x01;
        y2 = y1 + height -0x01;
        SysPictureDummy(x1, y1, x2, y2, _17C2, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
    }
    _7F6B:
    if (_17C5 == 0x04 && _17C4 == 0x02)
    {
        _7F83:
        __8D96();
    }
}

// 返回指向图片开始地址
// DAT_ACTORPIC                     宽           高       单张图片字节数     像素位宽
UINT8* _00202F9F(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2, UINT16* _17D4, UINT8* _17D6)
{
    UINT16 _17C9;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    // 切换到角色图片资源数据的bank
    UINT16 _17CD = (MCU_memory[0x17 + *(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1))] << 0x0002) + *(UINT16*)(MCU_memory + 0x1931);
    _DataBankSwitch(0x09, 0x04, _17CD);
    UINT8* _17C7 = MCU_memory + 0x9000;
    if (_17C7[0] != 'A' || _17C7[1] != 'C' || _17C7[2] != 'P')
    {
        _80D7:
        MCU_memory[0x1935] = DLOSE_ERR;
        return _17C7;
    }
_80EA:
    // 指向角色图片资源数据
    _17C7 += *(UINT16*)(MCU_memory+0x18+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1)));
    _17C6 = _17C7[2]; // 宽
    _17C5 = _17C7[3]; // 高
    _17C4 = _17C7[5];
    _17C7 += 0x0006;
    if (_17C6 & 0x07)
    {
        _81F6:
        _17C9 = (_17C6>>0x03)+0x01;
    }
    else
    {
        _8218:
        _17C9 = (_17C6>>0x03);
    }
    _8234:
    _17C9 *= _17C5;
    _17C9 *= _17C4;
    //                      面相
    UINT16 _17CB = _17C9*(MCU_memory[0x02+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))]-0x01);
    _17CB *= 0x0003;
    //                      脚步
    UINT16 _17C3 = _17C9*(MCU_memory[0x03+*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))]);
    if (_17C3 == 0x03)
    {
        _8323:
        _17C3 = 0x01;
    }
    _8329:
    _17CB += _17C9*_17C3;
    _17C7 += _17CB;
    *_17D0 = _17C6;
    *_17D2 = _17C5;
    *_17D4 = _17C9;
    *_17D6 = _17C4;
    return _17C7;
}

void _00203425(UINT8* _17D0, UINT8 _17D2, UINT8 _17D3, UINT8 _17D4)
{
    UINT8 _17CB[2];
    UINT8 _17C8[3];
    UINT8 _17CD = (UINT8)strlen(_17D0);
    __8E75(0x02, _17CD, _17CB);
    SysPrintString(_17CB[0], _17CB[1], _17D0);
    _17C8[0] = _17D2;
    _17C8[1] = _17D3;
    _17C8[2] = _17D4;
    __8ECA(_17C8);
    __8DD3(0x02, 0x01);
}

// BUY "t0 t1 ... tsum"
void _00203591()
{
    UINT8 _1775[0x5A];
    UINT8 _1774 = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("BUY");
    while (0x01)
    {
        _85CF:
        if (MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)]+0x01 == 0x01)
        {
            _85F7:
            break;
        }
        _85FA:
        _1775[_1774*0x0003+0x0001] = MCU_memory[*(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2)];
        *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
        _1775[_1774*0x0003] = MCU_memory[*(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2)];
        *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
        LOG(" %d", _1775[_1774 * 0x0003] * 100 + _1775[_1774 * 0x0003 + 0x0001]);
        _1774++;
    }
    LOG("\n");
    _8725:
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    __8EC4(0x00, _1774, _1775);
}

// TESTMONEY money lessadr
void _0020378B()
{
    UINT16 money;
    UINT16 lessadr;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    money = *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0004;
    lessadr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("TESTMONEY %d %d\n", money, lessadr);
    //          玩家拥有的金钱
    if (money > *(UINT16*)(MCU_memory+0x1A8F))
    {
        _88AD:
        *(UINT16*)(MCU_memory+0x1AA0) = lessadr;
    }
}

// CALLCHAPTER m n
void _002038D3()
{
    UINT8 m;
    UINT8 n;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("CALLCHAPTER %d %d\n", m, n);
    __8F40(m, n);
}

// DELETEGOODS type index erradr
void _002039BF()
{
    UINT8 type;
    UINT8 index;
    UINT16 erradr;
    UINT8 _17C7[3];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    index = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    erradr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("DELETEGOODS %d %d %d\n", type, index, erradr);
    _17C7[0] = type;
    _17C7[1] = index;
    _17C7[2] = 0x63;
    if (__8ECD(_17C7) == 0x00)
    {
        _8B65:
        *(UINT16*)(MCU_memory+0x1AA0) = erradr;
    }
}

// MESSAGE "..."
void _00203B8B()
{
    UINT8* _17CC;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _17CC = MCU_memory + *(UINT16*)(MCU_memory+0x1AA0);
    *(UINT16*)(MCU_memory+0x1AA0) += (UINT16)strlen(MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2))+0x0001;
    LOG("MESSAGE %s\n", _17CC + *(UINT16*)(MCU_memory + 0x1AA2));
    GuiMsgBox(_17CC + *(UINT16*)(MCU_memory + 0x1AA2), 0x0000);
}

// 执行对应偏移量的指令
UINT8 _00204000(UINT16 _17D0)
{
    const UINT8 _8B6F[] = "错误指令...";
    UINT16 _17CE; // GUT章节对应的Bank
    //                                          装载当前GUT章节数据
    *(UINT16*)(MCU_memory + 0x1AA2) = (UINT16)(__8F0A(DAT_GUDSCRIPT, MCU_memory[0x1AAC], MCU_memory[0x1AAB]) - MCU_memory);
    if (MCU_memory[0x1935])
    {
        _504B:
        return 0x03;
    }
_5050:
    // 记录GUT章节对应的Bank
    _GetDataBankNumber(0x09, &_17CE);
    // 指向当前GUT数据的起始位置
    *(UINT16*)(MCU_memory+0x1AA2) += 0x0018;
    // 当前待执行GUT命令的偏移量
    *(UINT16*)(MCU_memory+0x1AA0) = _17D0;
    MCU_memory[0x1AA4] = 0x01;
    while (0x01)
    {
    _50BF:
        // 切换到当前GUT章节对应的Bank
        _DataBankSwitch(0x09, 0x04, _17CE);
        if (MCU_memory[0x1AA4] != 0x01)
        {
            _50F2:
            break;
        }
    _50F5:
        // 如果当前待执行GUT命令的偏移量<章节数据大小
        if (*(UINT16*)(MCU_memory+0x1AA0) < *(UINT16*)(MCU_memory+0x1AAE))
        {
            _5119:
            switch (MCU_memory[*(UINT16*)(MCU_memory+0x1AA0) + *(UINT16*)(MCU_memory+0x1AA2)])
            {
            case MUSIC: // _5153
                __8EDC();
                break;
            case LOADMAP: // _5161
                _0020484C();
                break;
            case CREATEACTOR: // _5167
                __8D27();
                break;
            case DELETENPC: // _5175
                __8D2D();
                break;
            //case MAPEVENT:
            //    break;
            //case ACTOREVENT:
            //    break;
            case MOVE: // _5183
                __8D30();
                break;
            case ACTORMOVE: // _5191
                __8D4E();
                break;
            //case ACTORSPEED:
            //    break;
            case _CALLBACK: // _519F
                __8D33();
                break;
            case GOTO: // _51AD
                __8D36();
                break;
            case IF: // _51BB
                __8D4B();
                break;
            case SET: // _51C9
                __8D3F();
                break;
            case SAY: // _51D7
                __8D39();
                break;
            case STARTCHAPTER: // _51E5
                __8D51();
                _GetDataBankNumber(0x09, &_17CE);
                break;
            case SCREENR: // _5218
                __8D3C();
                break;
            case SCREENS: // _5226
                _00204A08();
                break;
            //case SCREENA:
            //    break;
            //case EVENT:
            //    break;
            //case MONEY:
            //    break;
            case GAMEOVER: // _522C
                __8ED6();
                break;
            case IFCMP: // _523A
                __8D42();
                break;
            case ADD: // _5248
                __8D45();
                break;
            case SUB: // _5256
                __8D48();
                break;
            case SETCONTROLID: // _5264
                _00204B52();
                break;
            //case GUTEVENT:
            // GUTEVENT n label
            // 当角色触发到地图事件n时，跳转到label下执行
            //    break;
            case SETEVENT: // _526A
                __8D54();
                break;
            case CLREVENT: // _5278
                __8D57();
                break;
            case BUY: // _5286
                __8F25();
                break;
            case FACETOFACE: // _5294
                __8D5A();
                break;
            case MOVIE: // _52A2
                __8EDF();
                break;
            case CHOICE: // _52B0
                __8EE2();
                break;
            case CREATEBOX: // _52BE
                _00204C97();
                break;
            case DELETEBOX: // _52C4
                _00204E3D();
                break;
            case GAINGOODS: // _52CA
                _00204E49();
                break;
            case INITFIGHT: // _52D0
                _00205121();
                break;
            case FIGHTENABLE: // _52D6
                _0020541C();
                break;
            case FIGHTDISENABLE: // _52DC
                _00205443();
                break;
            case CREATENPC: // _52E2
                __8D2A();
                break;
            case ENTERFIGHT: // _52F0
                _0020546A();
                break;
            case DELETEACTOR: // _52F6
                _00204BE5();
                break;
            case GAINMONEY: // _52FC
                _00205B30();
                break;
            case USEMONEY: // _5302
                _00205C82();
                break;
            case SETMONEY: // _5308
                _00205DD4();
                break;
            case LEARNMAGIC: // _530E
                _00205ED6();
                break;
            case SALE: // _5314
                _0020608A();
                break;
            case NPCMOVEMOD: // _531A
                _002060B7();
                break;
            case MESSAGE: // _5320
                __8F31();
                break;
            case DELETEGOODS: // _532E
                __8F2E();
                break;
            case RESUMEACTORHP: // _533C
                _002062BB();
                break;
            case ACTORLAYERUP: // _5342
                __8ED3();
                break;
            case BOXOPEN: // _5350
                __8ED9();
                break;
            case DELALLNPC: // _535E
                _0020663B();
                break;
            case NPCSTEP: // _5364
                __8D5D();
                break;
            case SETSCENENAME: // _5372
                __8D60();
                break;
            case SHOWSCENENAME: // _5380
                __8D63();
                break;
            case SHOWSCREEN: // _538E
                __8D66();
                break;
            case USEGOODS: // _539C
                __8F34();
                break;
            case ATTRIBTEST: // _53AA
                _0020666A();
                break;
            case ATTRIBSET: // _53B0
                __8D69();
                break;
            case ATTRIBADD: // _53BE
                __8D6C();
                break;
            case SHOWGUT: // _53CC
                _00206D95();
                break;
            case USEGOODSNUM: // _53D2
                _00206F18();
                break;
            case RANDRADE: // _53D8
                _002071F7();
                break;
            case MENU: // _53DE
                __8D72();
                break;
            case TESTMONEY: // _53EC
                __8F28();
                break;
            case CALLCHAPTER: // _53FA
                __8F2B();
                break;
            case DISCMP: // _5408
                _00207376();
                break;
            case RETURN: // _540E
                _0020758A();
                break;
            case TIMEMSG: // _5414
                _00207590();
                break;
            case DISABLESAVE: // _541A
                _002076D3();
                break;
            case ENABLESAVE: // _5420
                _002076FA();
                break;
            case GAMESAVE: // _5426
                _00207721();
                break;
            case SETEVENTTIMER: // _542C
                __8F37();
                break;
            case ENABLESHOWPOS: // _543A
                __8F3A();
                break;
            case DISABLESHOWPOS: // _5448
                __8F3D();
                break;
            case SETTO: // _5456
                __8D75();
                break;
            case TESTGOODSNUM: // _5464
                _0020774E();
                break;
            default: // _546A
                SysPrintString(0x0A, 0x46, _8B6F);
                return 0x01;
                break;
            }
            _549F:
            if (MCU_memory[0x1935])
            {
                _54A7:
                *(UINT16*)(MCU_memory+0x1AA0) -= MCU_memory[0x1AB0];
                *(UINT16*)(MCU_memory+0x1AA0) -= MCU_memory[0x1AB0];
                *(UINT16*)(MCU_memory+0x1AA0) -= 0x0003;
                if (MCU_memory[0x1935] == 0xFE || MCU_memory[0x1935] == 0xFD || MCU_memory[0x1935] == 0xFF)
                {
                    void;
                }
                _553D:
                return 0x02;
            }
        }
        else
        {
            _5545:
            break;
        }
    }
    _554B:
    return 0x00;
}

UINT8 _00204563(UINT8 _17CF)
{
    UINT16 _17CB;
    UINT16 _17C9;
    UINT8 _17C8;
    if (_17CF+0x01 == 0x01)
    {
        _5585:
        return 0x01;
    }
    _558A:
    if (_17CF > MCU_memory[0x1AB0])
    {
        _55A0:
        return 0x01;
    }
    _55A5:
    _17C9 = *(UINT16*)(MCU_memory+0x1AA0);
    _17C8 = MCU_memory[0x1AA4];
    _17CB = *(UINT16*)(MCU_memory+ 0x1AB1 +((_17CF-0x01)<<1));
    if (_17CB+0x0001 == 0x0001)
    {
        _5634:
        return 0x00;
    }
    _5639:
    if (__8DB1(_17CB) == 0x00)
    {
        _566C:
        *(UINT16*)(MCU_memory+0x1AA0) = _17C9;
        MCU_memory[0x1AA4] = _17C8;
        return 0x00;
    }
    _568D:
    return 0x02;
}

void _002046A5()
{
    __8DBA();
    fillmem(MCU_memory + *(UINT16*)(MCU_memory + 0x1AA7) + 0x00C8, 0x0038, 0x00);
    __8EE9(0x01);
}

// 缓存GUT数据，并初始化GUT数据起始位置和GUT命令偏移量数据
void _00204739()
{
    // 指向当前GUT数据的起始位置
    *(UINT16*)(MCU_memory + 0x1AA2) = (UINT16)(__8F0A(DAT_GUDSCRIPT, MCU_memory[0x1AAC], MCU_memory[0x1AAB]) - MCU_memory);
    if (MCU_memory[0x1935])
    {
        _5784:
        return;
    }
    _5787:
    *(UINT16*)(MCU_memory+0x1AA2) += 0x0018;
    _SysMemcpy(MCU_memory + 0x1AAE, MCU_memory + *(UINT16*)(MCU_memory+0x1AA2), 0x0201);
    // 当前待执行GUT命令的偏移量
    *(UINT16*)(MCU_memory+0x1AA0) = MCU_memory[0x1AB0] *0x0002+0x0003;
}

// LOADMAP m n x y
void _0020484C()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    UINT8 m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    UINT8 n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    UINT8 x = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    UINT8 y = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("LOADMAP %d %d %d %d\n", m, n, x, y);
    __8DD6(m, n, x, y);
    __8D96();
    __8D99();
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// SCREENS x y
void _00204A08()
{
    UINT8 x;
    UINT8 y;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    x = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    y = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SCREENS %d %d\n", x, y);
    MCU_memory[0x197C] = x; // 屏幕左上角定位到地图的x处
    MCU_memory[0x197D] = y; // 屏幕左上角定位到地图的y处
    // 加载要显示的地图块数据
    __8DA5();
    __8DA2();
    __8D96();
    __8D99();
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// SETCONTROLID actor
void _00204B52()
{
    UINT8 actor;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SETCONTROLID %d\n", actor);
    __8DF1(actor);
}

// DELETEACTOR actor
void _00204BE5()
{
    UINT8 _17CF;
    UINT8 actor;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("DELETEACTOR %d\n", actor);
    _17CF = __8DFD(actor);
    if (_17CF == 0x00)
    {
        _5C74:
        return;
    }
    _5C77:
    __8DDC(actor);
}

// CREATEBOX id boxindex x y
void _00204C97()
{
    UINT8 id = 0x00;
    UINT8 boxindex = 0x00;
    UINT8 x = 0x00;
    UINT8 y = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    boxindex = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    x = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    y = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("CREATEBOX %d %d %d %d\n", id, boxindex, x, y);
    __8DD9(id - 0x01, 0x04, boxindex, x, y);
}

// DELETEBOX id
void _00204E3D()
{
    LOG("DELETEBOX->");
    __8D2D();
}

// GAINGOODS type index
void _00204E49()
{
    const UINT8 _8B7B[] = "已满载！";
    const UINT8 _8B84[] = "获得:";
    UINT8 _17B7[20];
    UINT8* _17B5;
    UINT8 type;
    UINT8 index;
    UINT8 _17B0[3];
    UINT8* _17AE;
    type = 0x00;
    index = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    index = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("GAINGOODS %d %d\n", type, index);
    _17B0[0] = type; // 道具类型
    _17B0[1] = index; // Idx
    _17B0[2] = 0x01; // 道具数量
    if (__8ECA(_17B0) == 0x00)
    {
        _5F99:
        _SysMemcpy(_17B7, _8B7B, 0x000A);
    }
    else
    {
        _5F96:
        _17B5 = __8F0A(DAT_GOODSRES, type, index);
        if (MCU_memory[0x1935])
        {
            _6013:
            return;
        }
        _6016:
        _17AE = _17B5;
        _SysMemcpy(_17B7, _8B84, 0x0006);
        // 复制道具名称
        _SysMemcpy(_17B7+0x05, _17AE + 0x0006, 0x000C);
    }
    _60DA:
    GuiMsgBox(_17B7, 0x0064);
}

// INITFIGHT f0 f1 f2 f3 f4 f5 f6 f7 scrb scrl scrr
void _00205121()
{
    UINT8 _17BF[13];
    UINT16 _17A7[11];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _SysMemcpy((UINT8*)_17A7, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0016);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0016;
    LOG("INITFIGHT %d %d %d %d %d %d %d %d %d %d %d\n",
        _17A7[0], _17A7[1], _17A7[2], _17A7[3], _17A7[4], _17A7[5], _17A7[6], _17A7[7],
        _17A7[8], _17A7[9], _17A7[10]);
    _17BF[0] = 0x01;
    _17BF[4] = 0x00; // 可能出现的敌人种类数量
    for (UINT8 _17A6=0x00; _17A6<0x08; _17A6++)
    {
        _6240:
        if (_17A7[_17A6]+0x0001 != 0x0001)
        {
            _6284:
            _17BF[5+_17BF[4]] = (UINT8)_17A7[_17A6];
            _17BF[4] = _17BF[4] + 0x01;
        }
    }
    _632C:
    _17BF[1] = (UINT8)_17A7[8]; // 大战斗背景
    _17BF[3] = (UINT8)_17A7[9]; // 左下角图
    _17BF[2] = (UINT8)_17A7[10]; // 右上角图
    __8E6C(_17BF);
}

// FIGHTENABLE
void _0020541C()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("FIGHTENABLE\n");
    MCU_memory[0x1A96] = 0x01; // 遇怪
}

// FIGHTDISENABLE
void _00205443()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("FIGHTDISENABLE\n");
    MCU_memory[0x1A96] = 0x00; // 不遇怪
}

// ENTERFIGHT roundmax f0 f1 f2 scrb scrl scrr evtrnd0 evtrnd1 evtrnd2 evt0 evt1 evt2 lossto winto
void _0020546A()
{
    UINT16 roundmax;
    UINT16 lossto;
    UINT16 winto;
    UINT16 f[3];
    UINT16 evtrnd[3];
    UINT16 evt[3];
    UINT8 _17AC;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    roundmax = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)f, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0006);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0006;
    MCU_memory[0x18DD] = MCU_memory[*(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2)]; // 大战斗背景
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    MCU_memory[0x18DF] = MCU_memory[*(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2)]; // 左下角图
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    MCU_memory[0x18DE] = MCU_memory[*(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2)]; // 右上角图
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)evtrnd, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0006);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0006;
    _SysMemcpy((UINT8*)evt, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0006);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0006;
    _SysMemcpy((UINT8*)&lossto, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&winto, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("ENTERFIGHT %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", roundmax, f[0], f[1], f[2],
        MCU_memory[0x18DD], MCU_memory[0x18DF], MCU_memory[0x18DE], evtrnd[0], evtrnd[1], evtrnd[2],
        evt[0], evt[1], evt[2], lossto, winto);
    MCU_memory[0x191D] = (UINT8)roundmax; // 限定最多回合数
    for (UINT8 _17AD = 0x00; _17AD<0x03; _17AD++)
    {
    _6934:
        MCU_memory[0x1826+_17AD*0x0033+0x01] = (UINT8)f[_17AD]; // 敌人角色资源ID
    }
    _69BD:
    for (UINT8 _17AD = 0x00; _17AD<0x03; _17AD++)
    {
        _69DE:
        MCU_memory[0x191F+_17AD] = (UINT8)evtrnd[_17AD]; // 战斗触发事件回合
    }
    _6A43:
    for (UINT8 _17AD = 0x00; _17AD<0x03; _17AD++)
    {
        _6A64:
        MCU_memory[0x1922+_17AD] = (UINT8)evt[_17AD]; // 相应的事件号
    }
    _6AC9:
    _17AC = __8E00(0x00); // 指定敌人战斗
    if (_17AC == 0x02)
    {
        _6AE6:
        *(UINT16*)(MCU_memory+0x1AA0) = lossto; // 战斗失败时跳转到的地址标号
    }
    else if (_17AC == 0x01)
    {
        _6B0A:
        *(UINT16*)(MCU_memory+0x1AA0) = winto; // 战斗胜利时跳转到的地址标号
    }
}

// GAINMONEY money
void _00205B30()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    UINT32 money = *(UINT32*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0004;
    LOG("GAINMONEY %d\n", money);
    UINT32 _17C6 = money;
    UINT32 _8B8A = 0x0001019F;
    UINT32 _8B8E = 0x0001019F;
    //  玩家拥有的金钱
    if (*(UINT32*)(MCU_memory+0x1A8F)+ money > _8B8A);
    {
    _6C1B:
        //              玩家拥有的金钱
        _17C6 = _8B8E-*(UINT32*)(MCU_memory+0x1A8F);
    }
_6C46:
    // 玩家拥有的金钱
    *(UINT32*)(MCU_memory+0x1A8F) += _17C6;
}

// USEMONEY money
void _00205C82()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    UINT32 money = *(UINT32*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0004;
    LOG("USEMONEY %d\n", money);
    UINT32 _17C6 = money;
    //  玩家拥有的金钱
    if (*(UINT32*)(MCU_memory+0x1A8F) > money)
    {
    _6D62:
        // 玩家拥有的金钱
        *(UINT32*)(MCU_memory+0x1A8F) -= money;
    }
    else
    {
        UINT32 _8B92 = 0x00000000;
        _6D90:
        _17C6 = *(UINT32*)(MCU_memory+0x1A8F); // 玩家拥有的金钱
        *(UINT32*)(MCU_memory+0x1A8F) = _8B92; // 玩家拥有的金钱
    }
}

// SETMONEY money
void _00205DD4()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    UINT32 money = *(UINT32*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0004;
    LOG("SETMONEY %d\n", money);
    UINT32 _8B96 = 0x0001019F;
    UINT32 _8B9A = 0x0001019F;
    if (money > _8B96)
    {
        _6E8F:
        *(UINT32*)(MCU_memory+0x1A8F) = _8B9A; // 玩家拥有的金钱
    }
    else
    {
        _6EA5:
        *(UINT32*)(MCU_memory+0x1A8F) = money; // 玩家拥有的金钱
    }
}

// LEARNMAGIC actor type index
void _00205ED6()
{
    UINT8 type;
    UINT8 index;
    UINT8 actor;
    UINT8 _17CB;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    index = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("LEARNMAGIC %d %d %d\n", actor, type, index);
    for (_17CB=0x00; _17CB<0x03; _17CB++)
    {
        _6FF6:
        // 角色资源ID
        if (MCU_memory[_17CB*0x0019+0x1988+0x01] == actor)
        {
            _7032:
            break;
        }
    }
    _7038:
    if (_17CB < 0x03)
    {
        _7044:
        __8E96(_17CB, type, index);
    }
    else
    {
        _7074:
        MCU_memory[0x1935] = ID_ERR;
    }
}

// SALE
void _0020608A()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("SALE\n");
    __8D8A();
}

// NPCMOVEMOD id mode
void _002060B7()
{
    UINT8 id = 0x00;
    UINT8 mode = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    mode = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("NPCMOVEMOD %d %d\n", id, mode);
    if (*(UINT16*)(0x19D3+MCU_memory+((id -0x01)<<1)) != 0x0000)
    {
        _71B2:
        if (mode +0x01 == 0x01)
        {
        _71C1:
            // 动作
            MCU_memory[*(UINT16*)(0x19D3+MCU_memory+((id -0x01)<<1))+0x0004] = 0x00;
        }
        else
        {
            _7209:
            if (mode == 0x01)
            {
            _7215:
                // 动作
                MCU_memory[*(UINT16*)(0x19D3+MCU_memory+((id -0x01)<<1))+0x0004] = 0x03;
            }
            else
            {
            _725D:
                // 动作
                MCU_memory[*(UINT16*)(0x19D3+MCU_memory+((id -0x01)<<1))+0x0004] = 0x04;
            }
        }
    }
    else
    {
        _72A5:
        MCU_memory[0x1935] = NPC_ERR;
    }
}

// RESUMEACTORHP id rade
void _002062BB()
{
    UINT8 _17CD;
    UINT8 id;
    UINT16 _17C6;
    UINT16 _17C4;
    UINT16 _17CA = 0x0000;
    UINT16 rade = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    rade = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("RESUMEACTORHP %d %d\n", id, rade);
    if (rade <= 0x0064 && rade >= 0x0001)
    {
        _73E2:
        for (_17CD=0x00; _17CD<0x03; _17CD++)
        {
            _7403:
            // 角色资源ID
            if (MCU_memory[0x1988+_17CD*0x0019+0x01] == id)
            {
                break;
            }
        }
        _7445:
        if (_17CD >= 0x03)
        {
            _7451:
            MCU_memory[0x1935] = CONTROL_ERR;
            return;
        }
        _7459:
        // 指向角色属性
        _17C4 = *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x11);
        //      hpMax
        _17C6 = *(UINT16*)(MCU_memory+_17C4+0x06)* rade /0x0064;
        if (_17C6+0x0001 == 0x0001)
        {
            _7502:
            _17C6 = 0x0001;
        }
    _750D:
        // hp += _17C6
        *(UINT16*)(MCU_memory+_17C4+0x08) = *(UINT16*)(MCU_memory + _17C4 + 0x08) + _17C6;
        // hp > hpMax
        if (*(UINT16*)(MCU_memory + _17C4 + 0x08) > *(UINT16*)(MCU_memory+_17C4+0x06))
        {
        _75D1:
            // hp = hpMax
            *(UINT16*)(MCU_memory+_17C4+0x0008) = *(UINT16*)(MCU_memory + _17C4 + 0x06);
        }
    }
}

// DELALLNPC
void _0020663B()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("DELALLNPC\n");
    __8EE9(0x01);
}

// ATTRIBTEST actor type attrval lowaddr higaddr
void _0020666A()
{
    UINT8 _17CD;
    UINT8 actor;
    UINT8 type;
    UINT16 attrval;
    UINT16 _17C7;
    UINT16 lowaddr;
    UINT16 higaddr;
    UINT16 _17C1;
    UINT16 _17BF;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    type = *(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    attrval = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    lowaddr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    higaddr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("ATTRIBTEST %d %d %d %d %d\n", actor, type, attrval, lowaddr, higaddr);
    for (_17CD=0x00; _17CD<0x03; _17CD++)
    {
        _7875:
        // 角色资源ID
        if (MCU_memory[0x1988+_17CD*0x0019+0x01] == actor)
        {
            _78B1:
            break;
        }
    }
    _78B7:
    if (_17CD < 0x03)
    {
        _78C3:
        // 指向角色属性
        _17C1 = *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x11);
        // 指向角色装备
        _17BF = *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x13);
    }
    else
    {
        _7950:
        MCU_memory[0x1935] = ID_ERR;
    }
    _7955:
    _17C7 = 0x0000;
    switch (type)
    {
    case 0x0000: // _797F
        // 等级
        _17C7 = MCU_memory[_17C1];
        break;
    case 0x0001: // _79A3
        // 攻击
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x0E);
        break;
    case 0x0002: // _79C9
        // 防御
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x10);
        break;
    case 0x0003: // _79EF
        // 身法
        _17C7 = MCU_memory[_17C1+0x16];
        break;
    case 0x0004: // _7A13
        // hp
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x08);
        break;
    case 0x0005: // _7A39
        // mp
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x0C);
        break;
    case 0x0006: // _7A5F
        // 当前经验
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x12);
        break;
    case 0x0007: // _7A85
        // 灵力
        _17C7 = MCU_memory[_17C1+0x17];
        break;
    case 0x0008: // _7AA9
        // 幸运
        _17C7 = MCU_memory[_17C1+0x18];
        break;
    case 0x0009: // _7ACD
        // 攻击的异常回合数
        _17C7 = MCU_memory[_17C1+0x19];
        break;
    case 0x000A: // _7AF1
        // 对特殊状态的免疫
        _17C7 = MCU_memory[_17C1+0x01];
        break;
    case 0x000B: // _7B15
        // 普通攻击可能产生异常状态
        _17C7 = MCU_memory[_17C1+0x02];
        break;
    case 0x000C: // _7B39
        // 合体法术
        _17C7 = MCU_memory[_17C1+0x03];
        break;
    case 0x000D: // _7B5D
        // 每回合变化生命
        _17C7 = MCU_memory[_17C1+0x04];
        break;
    case 0x000E: // _7B81
        // 每回合变化真气
        _17C7 = MCU_memory[_17C1+0x05];
        break;
    case 0x000F: // _7BA5
        // 头戴
        _17C7 = MCU_memory[_17BF];
        break;
    case 0x0010: // _7BC9
        // 身穿
        _17C7 = MCU_memory[_17BF+0x01];
        break;
    case 0x0011: // _7BED
        // 肩披
        _17C7 = MCU_memory[_17BF+0x02];
        break;
    case 0x0012: // _7C11
        // 护腕
        _17C7 = MCU_memory[_17BF+0x03];
        break;
    case 0x0013: // _7C35
        // 手持
        _17C7 = MCU_memory[_17BF+0x04];
        break;
    case 0x0014: // _7C59
        // 脚蹬
        _17C7 = MCU_memory[_17BF+0x05];
        break;
    case 0x0015: // _7C7D
        // 佩戴1
        _17C7 = MCU_memory[_17BF+0x06];
        break;
    case 0x0016: // _7CA1
        // 佩戴2
        _17C7 = MCU_memory[_17BF+0x07];
        break;
    case 0x0017: // _7CC5
        // hpMax
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x06);
        break;
    case 0x0018: // _7CEB
        // mpMax
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x0A);
        break;
    default: // _7D11
        MCU_memory[0x1935] = DATA_ERR;
        break;
    }
    _7D16:
    if (_17C7 < attrval)
    {
        _7D34:
        *(UINT16*)(MCU_memory+0x1AA0) = lowaddr;
    }
    else if (_17C7 > attrval)
    {
        _7D6F:
        *(UINT16*)(MCU_memory+0x1AA0) = higaddr;
    }
}

// SHOWGUT toppic botpic "......"
void _00206D95()
{
    UINT8 toppic;
    UINT8 botpic;
    UINT8* _17CA;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    toppic = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    botpic = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _17CA = MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2);
    *(UINT16*)(MCU_memory+0x1AA0) += (UINT16)strlen(_17CA)+0x0001;
    LOG("SHOWGUT %d %d %s\n", toppic, botpic, _17CA);
    __8D6F(toppic, botpic, _17CA);
}

// USEGOODSNUM type index num erradr
void _00206F18()
{
    UINT8 type;
    UINT8 index;
    UINT8 num;
    UINT16 erradr;
    UINT8 _17C5[3];
    UINT16 _17C3;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    index = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    num = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    erradr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("USEGOODSNUM %d %d %d %d\n", type, index, num, erradr);
    UINT8 _17CD = __8ED0(type, index);
    if (_17CD == 0xFF)
    {
        _80AE:
        *(UINT16*)(MCU_memory+0x1AA0) = erradr;
    }
    else
    {
        _80C9:
        _17C3 = *(UINT16*)(MCU_memory+0x1988+MCU_memory[0x1A94]*0x0019+0x17);
        if (MCU_memory[_17CD*0x0003+_17C3+0x02] < num)
        {
            _814C:
            *(UINT16*)(MCU_memory+0x1AA0) = erradr;
        }
        else
        {
            _8164:
            _17C5[0] = type;
            _17C5[1] = index;
            _17C5[2] = num;
            __8ECD(_17C5);
        }
    }
}

// RANDRADE rade trueadr
void _002071F7()
{
    UINT16 rade;
    UINT16 trueadr;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    rade = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    UINT16 _17CC = SysRand((PtrRandEnv)(MCU_memory + 0x1928)) % 0x03E8;
    trueadr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("RANDRADE %d %d\n", rade, trueadr);
    if (_17CC < rade)
    {
        _8350:
        *(UINT16*)(MCU_memory+0x1AA0) = trueadr;
    }
}

// DISCMP n m lowaddr higaddr
void _00207376()
{
    UINT8 n;
    UINT8 m;
    UINT16 lowaddr;
    UINT16 higaddr;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    lowaddr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    higaddr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("DISCMP %d %d %d %d\n", n, m, lowaddr, higaddr);
    if (MCU_memory[*(UINT16*)(MCU_memory+0x1AA7)+n] < m)
    {
        _8520:
        *(UINT16*)(MCU_memory+0x1AA0) = lowaddr;
    }
    else if (MCU_memory[*(UINT16*)(MCU_memory+0x1AA7)+n] > m)
    {
        _8564:
        *(UINT16*)(MCU_memory+0x1AA0) = higaddr;
    }
}

// RETURN
void _0020758A()
{
    MCU_memory[0x1AA4] = 0x02;
    LOG("RETURN\n");
}

// TIMEMSG time "......"
void _00207590()
{
    UINT8* _17CC;
    UINT8 time;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    time = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _17CC = MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2);
    *(UINT16*)(MCU_memory+0x1AA0) += (UINT16)strlen(_17CC)+0x0001;
    LOG("TIMEMSG %d %s\n", time, _17CC);
    GuiMsgBox(_17CC, time);
}

// DISABLESAVE
void _002076D3()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("DISABLESAVE\n");
    // 不能存档
    MCU_memory[0x1A9B] = 0x00;
}

// ENABLESAVE
void _002076FA()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("ENABLESAVE\n");
    // 能存档
    MCU_memory[0x1A9B] = 0x01;
}

// GAMESAVE
void _00207721()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("GAMESAVE\n");
    __8EEC();
}

// TESTGOODSNUM type index num adrequ adrmore
void _0020774E()
{
    UINT8 type;
    UINT8 index;
    UINT8 num;
    UINT16 adrequ;
    UINT16 adrmore;
    UINT16 _17C4;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    index = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    num = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    adrequ = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    adrmore = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("TESTGOODSNUM %d %d %d %d %d\n", type, index, num, adrequ, adrmore);
    UINT8 _17CD = __8ED0(type, index);
    if (_17CD == 0xFF)
    {
        _894B:
        return;
    }
    _894E:
    _17C4 = *(UINT16*)(MCU_memory+0x1988+MCU_memory[0x1A94]*0x0019+0x17);
    if (MCU_memory[_17C4+_17CD*0x0003+0x02] > num)
    {
        _89D6:
        *(UINT16*)(MCU_memory+0x1AA0) = adrmore;
    }
    else if (MCU_memory[_17C4+_17CD*0x0003+0x02] == num)
    {
        _8A2D:
        *(UINT16*)(MCU_memory+0x1AA0) = adrequ;
    }
}

// CREATEACTOR actor x y
void _00208000()
{
    UINT8 actor;
    UINT8 x;
    UINT8 y;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    x = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    y = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("CREATEACTOR %d %d %d\n", actor, x, y);
    __8DD9(0x00, 0x01, actor, x, y);
}

// CREATENPC id npc x y
void _0020813F()
{
    UINT8 npc;
    UINT8 id;
    UINT8 x;
    UINT8 y;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    npc = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    x = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    y = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("CREATENPC %d %d %d %d\n", id, npc, x, y);
    __8DD9(id - 0x01, 0x02, npc, x, y);
}

// DELETENPC id
void _002082CD()
{
    UINT8 id;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("DELETENPC %d\n", id);
    __8D9C(id -0x01);
    __8DDF(id -0x01);
}

// MOVE id x y
void _00208375()
{
    UINT8 id;
    UINT8 x;
    UINT8 y;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    x = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    y = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("MOVE %d %d %d\n", id, x, y);
    __8DE2(id - 0x01, x, y);
}

// CALLBACK
void _002084B5()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("CALLBACK\n");
    MCU_memory[0x1AA4] = 0x00;
}

// GOTO label
void _002084DC()
{
    UINT16 label = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    label = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("GOTO %d\n", label);
    *(UINT16*)(MCU_memory+0x1AA0) = label;
}

// SAY pos "......"
void _002085A3()
{
    MsgType _17CA;
    UINT8 _17C8;
    UINT8 _17B2[22];
    UINT8* _17B0;
    UINT8 _17C9 = 0x00;
    UINT8 pos = 0x00;
    UINT16 _17AD = 0x0000; // 字符串长度
    UINT16 _17AB = 0x0000;
    UINT16 _17A9;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    pos = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _17B0 = MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2);
    LOG("SAY %d %s\n", pos, _17B0);
    while (0x01)
    {
        _5675:
        *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
        if (_17B0[_17AD]+0x01 == 0x01)
        {
            _56C0:
            break;
        }
        _56C3:
        _17AD += 0x0001;
    }
    _56D6:
    while (0x01)
    {
        _56DD:
        if (pos +0x01 == 0x01)
        { // 不带贴图，可以显示15个半角字符
            _56EC:
            for (_17C8=0x00; _17C8<0x0F; _17AB+=0x0001,_17C8++)
            {
                _573B:
                if (_17B0[_17AB] > 0x80)
                {
                    _5767:
                    _17AB+=0x0001;
                    _17C8++;
                }
                else if (_17B0[_17AB]+0x01 == 0x01)
                {
                    _57AD:
                    break;
                }
            }
        }
        else
        { // 带贴图，可以显示11个半角字符
            _57B6:
            for (_17C8=0x00; _17C8<0x0B; _17AB+=0x0001,_17C8++)
            {
                _5805:
                if (_17B0[_17AB] > 0x80)
                {
                    _5831:
                    _17AB+=0x0001;
                    _17C8++;
                }
                else if (_17B0[_17AB]+0x01 == 0x01)
                {
                    _5877:
                    break;
                }
            }
        }
        _587D:
        _SysMemcpy(_17B2, _17AB - _17C8 + _17B0, _17C8);
        _17B2[_17C8] = 0x00;
        if (pos +0x01 == 0x01)
        { // 不带贴图
            _5935:
            SysLcdPartClear(0x09, 0x37, 0x95, 0x5F);
            SysRect(0x09, 0x37, 0x95, 0x5F);
            SysPrintString(0x0E, 0x3A, _17B2);
        }
        else
        { // 带贴图
            _59D2:
            SysLcdPartClear(0x09, 0x32, 0x95, 0x5F);
            SysRect(0x09, 0x32, 0x95, 0x5F);
            SysLine(0x26, 0x32, 0x2C, 0x38);
            SysLine(0x2D, 0x38, 0x95, 0x38);
            _GetDataBankNumber(0x09, &_17A9);
            __8F04(DAT_SUNDRYPIC, 0x01, pos, 0x00, 0x0D, 0x2E);
            if (MCU_memory[0x1935])
            {
                _5AFF:
                return;
            }
            _5B02:
            _DataBankSwitch(0x09, 0x04, _17A9);
            SysPrintString(0x2E, 0x3A, _17B2);
        }
    _5B5C:
        // 第二行可以显示15个半角字符
        for (_17C8=0x00; _17C8<0x0F; _17AB+=0x0001,_17C8++)
        {
            _5BAB:
            if (_17B0[_17AB] > 0x80)
            {
                _5BD7:
                _17AB+=0x0001;
                _17C8++;
            }
            else if (_17B0[_17AB]+0x01 == 0x01)
            {
                _5C1D:
                break;
            }
        }
        _5C23:
        _SysMemcpy(_17B2, _17AB - _17C8 + _17B0, _17C8);
        _17B2[_17C8] = 0x00;
        // 显示第二行的字符串
        SysPrintString(0x0E, 0x4D, _17B2);
        while (0x01)
        {
            _5D05:
            if (GuiGetMsg(&_17CA) == 0x00)
            {
                _5D3A:
                continue;
            }
            _5D3D:
            if (_17CA.type == DICT_WM_KEY)
            {
                _5D49:
                break;
            }
        }
        _5D4F:
        if  (_17B0[_17AB]+0x01 == 0x01)
        { // 对话输出完成，恢复画面，退出函数
            _5D79:
            __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
            break;
        }
        _5DA4:
        if (_17AB >= _17AD)
        { // 对话输出完成，恢复画面，退出函数
            _5DC2:
            __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
            break;
        }
    }
}

// SCREENR
void _00208E01()
{
    UINT16 _17CE = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0003;
    LOG("SCREENR\n");
}

// SET n m
void _00208E4C()
{
    UINT8 n;
    UINT8 m;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SET %d %d\n", n, m);
    MCU_memory[n + *(UINT16*)(MCU_memory + 0x1AA7)] = m;
}

// IFCMP n m label
void _00208F3B()
{
    UINT8 n;
    UINT8 m;
    UINT16 label;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    label = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("IFCMP %d %d %d\n", n, m, label);
    if (MCU_memory[n +*(UINT16*)(MCU_memory+0x1AA7)] == m)
    {
        _6086:
        *(UINT16*)(MCU_memory+0x1AA0) = label;
    }
}

// ADD n m
void _002090AC()
{
    UINT8 n;
    UINT8 m;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("ADD %d %d\n", n, m);
    MCU_memory[n +*(UINT16*)(MCU_memory+0x1AA7)] = MCU_memory[n + *(UINT16*)(MCU_memory + 0x1AA7)] + m;
}

// SUB n m
void _002091C1()
{
    UINT8 n;
    UINT8 m;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SUB %d %d\n", n, m);
    MCU_memory[n +*(UINT16*)(MCU_memory+0x1AA7)] = MCU_memory[n + *(UINT16*)(MCU_memory + 0x1AA7)] - m;
}

// IF n label
void _002092D6()
{
    UINT16 n = 0x0000;
    UINT16 label = 0x0000;
    UINT8 _17C9;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    label = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("IF %d %d\n", n, label);
    _17C9 = MCU_memory[n /0x0008+*(UINT16*)(MCU_memory+0x1AA9)];
    _17C9 &= (1 << (n % 0x0008));
    if (_17C9)
    {
        _646A:
        *(UINT16*)(MCU_memory+0x1AA0) = label;
    }
}

// ACTORMOVE
void _00209490()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x000D;
    LOG("ACTORMOVE\n");
}

// STARTCHAPTER m n
void _002094B2()
{
    UINT8 m;
    UINT8 n;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("STARTCHAPTER %d %d\n", m, n);
    MCU_memory[0x1AAC] = m; // 当前GUT的章
    MCU_memory[0x1AAB] = n; // 当前GUT的节
    __8DB7();
}

// SETEVENT n
void _00209591()
{
    UINT16 _17CC;
    UINT16 n = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _17CC = *(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2);
    n = *(UINT16*)(MCU_memory+_17CC);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SETEVENT %d\n", n);
    _17CC = MCU_memory[n/0x0008+*(UINT16*)(MCU_memory+0x1AA9)];
    _17CC |= (1<<(n%0x0008));
    MCU_memory[n/0x0008+*(UINT16*)(MCU_memory+0x1AA9)] = (UINT8)_17CC;
}

// CLREVENT n
void _00209700()
{
    UINT16 n = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("CLREVENT %d\n", n);
    __8D90(n);
}

// FACETOFACE n m
void _002097DB()
{
    UINT8 n;
    UINT8 m;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT16 _17C6;
    UINT16 _17C4;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    n = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    m = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("FACETOFACE %d %d\n", n, m);
    if (n+0x01 == 0x01)
    {
        _689F:
        //      屏幕左上角定位到地图的x处  角色x坐标
        _17CB = MCU_memory[0x197C] + MCU_memory[0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x05];
        //      屏幕左上角定位到地图的y处  角色y坐标
        _17CA = MCU_memory[0x197D] + MCU_memory[0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x06];
        // 指向角色面相
        _17C6 = 0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x0002;
    }
    else
    {
    _697F:
        // x坐标
        _17CB = MCU_memory[*(UINT16*)(((n-0x01)<<1)+0x19D3+MCU_memory)+0x05];
        // y坐标
        _17CA = MCU_memory[*(UINT16*)(((n-0x01)<<1)+0x19D3+MCU_memory)+0x06];
        // 指向面相
        _17C6 = *(UINT16*)(((n - 0x01) << 1) + 0x19D3 + MCU_memory) + 0x0002;
    }
    _6A3B:
    if (m+0x01 == 0x01)
    {
        _6A4A:
        //      屏幕左上角定位到地图的x处  角色x坐标
        _17C9 = MCU_memory[0x197C] + MCU_memory[0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x05];
        //      屏幕左上角定位到地图的y处 角色y坐标
        _17C8 = MCU_memory[0x197D] + MCU_memory[0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x06];
        // 指向角色面相
        _17C4 = 0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x0002;
    }
    else
    {
    _6B2A:
        // x坐标
        _17C9 = MCU_memory[*(UINT16*)(((m-0x01)<<1)+0x19D3+MCU_memory)+0x05];
        // y坐标
        _17C8 = MCU_memory[*(UINT16*)(((m-0x01)<<1)+0x19D3+MCU_memory)+0x06];
        // 指向面相
        _17C4 = *(UINT16*)(((m - 0x01) << 1) + 0x19D3 + MCU_memory) + 0x0002;
    }
    _6BE6:
    if (_17CB < _17C9)
    {
        _6BF4:
        MCU_memory[_17C6] = 0x02; // 角色面相右
        MCU_memory[_17C4] = 0x04; // 角色面相左
    }
    else if (_17CB > _17C9)
    {
        _6C3C:
        MCU_memory[_17C6] = 0x04; // 角色面相左
        MCU_memory[_17C4] = 0x02; // 角色面相右
    }
    _6C6E:
    if (_17CA < _17C8)
    {
        _6C7C:
        MCU_memory[_17C6] = 0x03; // 角色面相下
        MCU_memory[_17C4] = 0x01; // 角色面相上
    }
    else if (_17CA > _17C8)
    {
        _6CC4:
        MCU_memory[_17C6] = 0x01; // 角色面相上
        MCU_memory[_17C4] = 0x03; // 角色面相下
    }
    _6CF6:
    if (n)
    {
        _6CFF:
        __8D9C(n-0x01);
        __8EF5(n-0x01);
    }
    _6D23:
    if (m)
    {
        _6D2C:
        __8D9C(m-0x01);
        __8EF5(m-0x01);
    }
    _6D50:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// NPCSTEP id faceto step
void _00209D89()
{
    UINT8 id;
    UINT8 faceto;
    UINT8 step;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    faceto = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    step = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("NPCSTEP %d %d %d\n", id, faceto, step);
    if (id == 0x00)
    {
        _6E89:
        // 角色面相
        MCU_memory[0x1988+MCU_memory[0x1A94]*0x0019+0x0002] = faceto % 0x04 + 0x01;
        // 角色脚步
        MCU_memory[0x1988+MCU_memory[0x1A94]*0x0019+0x0003] = step % 0x03;
        //      角色x坐标                                               角色y坐标
        __8D9F(MCU_memory[0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x05], MCU_memory[0x1988+MCU_memory[0x1A94]*0x0019+0x06]);
        __8D96();
    }
    else if (id <= 0x28)
    {
    _6FE2:
        // 面相
        MCU_memory[*(UINT16*)(((id -0x01)<<1)+0x19D3+MCU_memory)+0x0002] = faceto % 0x04 + 0x01;
        // 脚步
        MCU_memory[*(UINT16*)(((id -0x01)<<1)+0x19D3+MCU_memory)+0x0003] = step % 0x03;
        __8D9C(id -1);
        __8EF5(id -1);
    }
    _70B5:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// SETSCENENAME "..."
void _0020A0EE()
{
    UINT8* _17CB;
    UINT8 _17CD = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _17CB = MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2);
    *(UINT16*)(MCU_memory+0x1AA0) += (UINT16)strlen(_17CB)+0x0001;
    LOG("SETSCENENAME %s\n", _17CB);
    //          当前地图的名称
    _SysMemcpy(MCU_memory + 0x1942, _17CB, (UINT16)strlen(_17CB)+0x0001);
    MCU_memory[0x194C] = 0x00; // 地图名称数组的最后一个字节强制赋0，防止后续操作越界
}

// SHOWSCENENAME
void _0020A23A()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("SHOWSCENENAME\n");
    // 显示当前地图的名称
    GuiMsgBox(MCU_memory + 0x1942, 0x0064);
}

// SHOWSCREEN
void _0020A28D()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("SHOWSCREEN\n");
    __8DA2();
    __8D96();
    __8D99();
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// ATTRIBSET actor type attrval
void _0020A2F8()
{
    UINT8 _17CD;
    UINT8 actor;
    UINT8 type;
    UINT16 attrval;
    UINT16 _17C6;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    attrval = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("ATTRIBSET %d %d %d\n", actor, type, attrval);
    for (_17CD=0x00; _17CD<0x03; _17CD++)
    {
        _7435:
        // 角色资源ID
        if (MCU_memory[0x1988+_17CD*0x0019+0x01] == actor)
        {
            _7471:
            break;
        }
    }
    _7477:
    if (_17CD < 0x03)
    {
        _7483:
        // 指向角色属性
        _17C6 = *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x11);
    }
    else
    {
        _74CB:
        MCU_memory[0x1935] = ID_ERR;
    }
    _74D0:
    switch (type)
    {
    case 0x0000:  // _74F1
        // 等级
        MCU_memory[_17C6] = (UINT8)attrval;
        break;
    case 0x0001:  // _7518
        // 攻击
        *(UINT16*)(MCU_memory+_17C6+0x000E) = attrval;
        break;
    case 0x0002:  // _7551
        // 防御
        *(UINT16*)(MCU_memory+_17C6+0x0010) = attrval;
        break;
    case 0x0003:  // _758A
        // 身法
        MCU_memory[_17C6+0x0016] = (UINT8)attrval;
        break;
    case 0x0004:  // _75BE
        // hp
        *(UINT16*)(MCU_memory+_17C6+0x0008) = attrval;
        break;
    case 0x0005:  // _75F7
        // mp
        *(UINT16*)(MCU_memory+_17C6+0x000C) = attrval;
        break;
    case 0x0006:  // _7630
        // 当前经验
        *(UINT16*)(MCU_memory+_17C6+0x0012) = attrval;
        break;
    case 0x0007:  // _7669
        // 灵力
        MCU_memory[_17C6+0x0017] = (UINT8)attrval;
        break;
    case 0x0008:  // _769D
        // 幸运
        MCU_memory[_17C6+0x0018] = (UINT8)attrval;
        break;
    case 0x0009:  // _76D1
        // 攻击的异常回合数
        MCU_memory[_17C6+0x0019] = (UINT8)attrval;
        break;
    case 0x000A:  // _7705
        // 对特殊状态的免疫
        MCU_memory[_17C6+0x0001] = (UINT8)attrval;
        break;
    case 0x000B:  // _7739
        // 普通攻击可能产生异常状态
        MCU_memory[_17C6+0x0002] = (UINT8)attrval;
        break;
    case 0x000C:  // _776D
        // 合体法术
        MCU_memory[_17C6+0x0003] = (UINT8)attrval;
        break;
    case 0x000D:  // _77A1
        // 每回合变化生命
        MCU_memory[_17C6+0x0004] = (UINT8)attrval;
        break;
    case 0x000E:  // _77D5
        // 每回合变化真气
        MCU_memory[_17C6+0x0005] = (UINT8)attrval;
        break;
    case 0x000F:  // _7809
        // hpMax
        *(UINT16*)(MCU_memory+_17C6+0x0006) = attrval;
        break;
    case 0x0010:  // _7842
        // mpMax
        *(UINT16*)(MCU_memory+_17C6+0x000A) = attrval;
        break;
    default:  // _787B
        MCU_memory[0x1935] = DATA_ERR;
        break;
    }
}

// ATTRIBADD actor type attrval
void _0020A891()
{
    UINT8 _17CD;
    UINT8 actor;
    UINT8 type;
    UINT16 attrval;
    UINT8* _17C6;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    actor = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    attrval = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("ATTRIBADD %d %d %d\n", actor, type, attrval);
    for (_17CD=0x00; _17CD<0x03; _17CD++)
    {
        _79CE:
        // 角色资源ID
        if (MCU_memory[0x1988+_17CD*0x0019+0x01] == actor)
        {
            _7A0A:
            break;
        }
    }
    _7A10:
    if (_17CD < 0x03)
    {
        _7A1C:
        // 指向角色属性
        _17C6 = MCU_memory + *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x11);
    }
    else
    {
        _7A64:
        MCU_memory[0x1935] = ID_ERR;
        return;
    }
    _7A69:
    switch (type)
    {
    case 0x0000:  // _7A8A
        // 等级
        _17C6[0x00] += attrval;
        break;
    case 0x0001:  // _7AEA
        // 攻击
        *(UINT16*)(_17C6+0x000E) += attrval;
        break;
    case 0x0002:  // _7B56
        // 防御
        *(UINT16*)(_17C6+0x0010) += attrval;
        break;
    case 0x0003:  // _7BC2
        // 身法
        _17C6[0x0016] += attrval;
        break;
    case 0x0004:  // _7C2F
        // hp
        *(UINT16*)(_17C6+0x0008) += attrval;
        break;
    case 0x0005:  // _7C9B
        // mp
        *(UINT16*)(_17C6+0x000C) += attrval;
        break;
    case 0x0006:  // _7D07
        // 当前经验
        *(UINT16*)(_17C6+0x0012) += attrval;
        break;
    case 0x0007:  // _7D73
        // 灵力
        _17C6[0x0017] += attrval;
        break;
    case 0x0008:  // _7DE0
        // 幸运
        _17C6[0x0018] += attrval;
        break;
    case 0x0009:  // _7E4D
        // 攻击的异常回合数
        _17C6[0x0019] += attrval;
        break;
    case 0x000A:  // _7EBA
        // hpMax
        *(UINT16*)(_17C6+0x0006) += attrval;
        break;
    case 0x000B:  // _7F26
        // mpMax
        *(UINT16*)(_17C6+0x000A) += attrval;
        break;
    default:  // _7F92
        MCU_memory[0x1935] = DATA_ERR;
        break;
    }
    _7F97:
    __8E87(_17C6, 0x01);
}

void _0020AFEC(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1)
{
    UINT8* _17CA;
    UINT16 _17C8;
    UINT16 _17C6;
    UINT16 _17C4;
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    SysTimer1Close();
    SysTimer1Open(0x0A);
    MCU_memory[0x191C] = 0x00;
    _GetDataBankNumber(0x09, &_17C4);
    _17CA = __8F0A(DAT_SUNDRYPIC, 0x05, _17CF);
    _17CF = MCU_memory[0x1935];
    if (MCU_memory[0x1935])
    {
        _80B8:
        MCU_memory[0x1935] = 0x00;
    }
    else
    {
        _80C0:
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), _17CA, 0x0280);
    }
    _80F9:
    _17CA = __8F0A(DAT_SUNDRYPIC, 0x05, _17D0);
    _17D0 = MCU_memory[0x1935];
    if (MCU_memory[0x1935])
    {
        _813C:
        MCU_memory[0x1935] = 0x00;
    }
    else
    {
        _8144:
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x1936) + 0x03E8, _17CA, 0x0280);
    }
    _81A8:
    _DataBankSwitch(0x09, 0x04, _17C4);
    _17C8 = 0x0000;
    _17C6 = (UINT16)strlen(_17D1)-0x0050;
    _0020B31C(_17CF, _17D0, 0x52, _17D1);
    while (_17C8 < _17C6)
    {
        _8271:
        if (MCU_memory[0x191C])
        {
            _8279:
            break;
        }
        _827C:
        _17C8 += 0x0014;
        _0020B31C(_17CF, _17D0, 0x10, _17D1+_17C8);
    }
    _82F3:
    SysTimer1Close();
    SysTimer1Open(0x32);
}

void _0020B31C(UINT8 _17CF, UINT8 _17D0, UINT8 y, UINT8* _17D2)
{
    UINT8 _17B8[0x14];
    UINT8 _17CC = 0x00;
    _SysMemcpy(_17B8, _17D2, 0x0014);
    _17D2 += 0x0014;
    while (y)
    {
        _83BA:
        y -= 0x02;
        SysPrintString(0x00, y, _17B8);
        _0020B4AF(0x01, _17CF);
        SysPrintString(0x00, y + 0x10, _17D2);
        _0020B4AF(0x00, _17D0);
        __8DD3(0x01, 0x01);
        if (MCU_memory[0x191C])
        {
            _8498:
            return;
        }
    }
}

void _0020B4AF(UINT8 _17CF, UINT8 _17D0)
{
    UINT8 y1;
    UINT8* _17C8;
    if (_17D0)
    {
        _84CB:
        if (_17CF)
        {
            _84D4:
            y1 = 0x00;
        }
        else
        {
            _84DD:
            y1 = 0x50;
        }
        _84E3:
        SysLcdPartClear(0x00, y1, 0x9E, y1 +0x0F);
    }
    else
    {
        _8521:
        if (_17CF)
        {
            _852A:
            _17C8 = MCU_memory + *(UINT16*)(MCU_memory+0x1936);
            y1 = 0x00;
        }
        else
        {
            _8548:
            _17C8 = MCU_memory + *(UINT16*)(MCU_memory+0x1936)+0x03E8;
            y1 = 0x60- _17C8[0x03];
        }
        _858D:
        SysPicture(0x00, y1, _17C8[0x02] - 0x01, _17C8[0x03] + y1 - 0x01, _17C8 + 0x0006, 0x00);
    }
}

// MENU choice "......"
void _0020B641()
{
    UINT8 choice;
    UINT8* _17CB;
    UINT8* _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    choice = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _17CB = MCU_memory +*(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2);
    *(UINT16*)(MCU_memory+0x1AA0) += (UINT16)strlen(_17CB)+0x0001;
    LOG("MENU %d %s\n", choice, _17CB);
    _17C7 = 0x00;
    _17C8 = 0x00;
    _17C4 = 0x00;
    _17C6 = 0x00;
    _8755:
    while (_17CB[_17C6])
    {
        _8778:
        if (_17CB[_17C6] == 0x20)
        {
            _879E:
            if (_17C8 < _17C7)
            {
                _87AC:
                _17C8 = _17C7;
            }
            _87B4:
            if (_17C7)
            {
                _87BD:
                _17C4++;
            }
            _87C8:
            _17C7 = 0x00;
            _17C6++;
        }
        else
        {
            _87DC:
            _17C7++;
            _17C6++;
        }
    }
    _87F5:
    if (_17C8 < _17C7)
    {
        _8803:
        _17C8 = _17C7;
    }
    _880B:
    if (_17C7)
    {
        _8814:
        _17C4++;
    }
    _881F:
    if (_17C4 == 0x00)
    {
        _8828:
        return;
    }
    _882B:
    _17C9 = SysMemAllocate(_17C8*_17C4+0x0A);
    if (_17C9 == 0x0000)
    {
        _8885:
        MCU_memory[0x1935] = MEM_ERR;
        return;
    }
    _888D:
    fillmem(_17C9, _17C8 * _17C4, 0x20);
    _17C7 = 0x00;
    _17C6 = 0x00;
    _17C5 = 0x00;
    _88EB:
    while (_17CB[_17C6])
    {
        _890E:
        _17C9[_17C5] = _17CB[_17C6];
        if (_17CB[_17C6] == 0x20)
        {
            _8980:
            if (_17C7)
            {
                _8989:
                _17C5 += _17C8-_17C7;
                _17C7 = 0x00;
            }
            _89A1:
            _17C6++;
        }
        else
        {
            _89AF:
            _17C7++;
            _17C6++;
            _17C5++;
        }
    }
    _89D3:
    _17C9[_17C5] = 0x00;
    _17C6 = 0x00;
    MCU_memory[choice +*(UINT16*)(MCU_memory+0x1AA7)] = __8EB2(0x50 - _17C8 * 0x04, 0x0A, _17C8, _17C4, _17C9, 0x00, &_17C6) + 0x01;
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory + 0x1936));
    if (SysMemFree(_17C9) == 0x00)
    {
        _8AF9:
        MCU_memory[0x1935] = MEM_ERR;
    }
}

// SETTO n1 n2
void _0020BB0F()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    UINT8 n1 = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    UINT8 n2 = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SETTO %d %d\n", n1, n2);
    MCU_memory[n2 +*(UINT16*)(MCU_memory+0x1AA7)] = MCU_memory[n1 + *(UINT16*)(MCU_memory + 0x1AA7)];
}

void _0020C000(UINT8 _17CF, UINT8 _17D0)
{
    MsgType _17CC;
    UINT8 _17CB = 0x00;
    while (MCU_memory[0x1935] == 0x00)
    {
        _5021:
        if (GuiGetMsg(&_17CC) == 0x00)
        {
            _5056:
            continue;
        }
        _5059:
        if (GuiTranslateMsg(&_17CC) == 0x00)
        {
            _508E:
            continue;
        }
        _5091:
        if (_17CC.type == DICT_WM_COMMAND)
        {
            _509D:
            if (_17CC.param == CMD_RETURN_HOME)
            {
                _50B8:
                MCU_memory[0x1935] = 0xFE;
            }
        }
        _50BD:
        if (_17CC.type == DICT_WM_CHAR_FUN)
        {
            _50C9:
            if (_17CC.param == CHAR_EXIT)
            {
                _50E4:
                if (MCU_memory[0x191C] == 0x00)
                {
                    _50EC:
                    MCU_memory[0x191C] = 0x01;
                }
            }
        }
        _50F1:
        if (_17D0)
        {
            _50FA:
            if (_17CC.type == DICT_WM_CHAR_FUN)
            {
                _5106:
                return;
            }
        }
        _5109:
        if (_17CC.type == DICT_WM_TIMER)
        {
            _5115:
            _17CB++;
        }
        _511E:
        if (_17CB > _17CF)
        {
            _5131:
            return;
        }
    }
}

void _0020C148(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2)
{
    // 装载地图
    __8EFE(_17CF, _17D0);
    if (MCU_memory[0x1935])
    {
        _5179:
        return;
    }
    _517C:
    MCU_memory[0x197C] = _17D1; // 屏幕左上角定位到地图的x处
    MCU_memory[0x197D] = _17D2; // 屏幕左上角定位到地图的y处
    // 加载要显示的地图块数据
    __8DA5();
    __8DA2();
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

void _0020C1FB(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3)
{
    UINT8 _17CD;
    if (MCU_memory[0x1935])
    {
        _5216:
        return;
    }
    _5219:
    switch (_17D0)
    {
    case 0x0001:  // _523A 玩家角色
        if (__8DC1(_17D1) != 0x00)
        {
            _5251:
            break;
        }
        _5254:
        if (MCU_memory[0x1935])
        {
            _525C:
            return;
        }
        _525F:
        _17CD = __8DEE();
        if (MCU_memory[0x1935])
        {
            _5276:
            return;
        }
        _5279:
        __8EFB(_17CD, 0x01, _17D1);
        if (MCU_memory[0x1935])
        {
            _52AC:
            return;
        }
        _52AF:
        // 角色x坐标
        MCU_memory[0x1988+_17CD*0x0019+0x0005] = _17D2;
        // 角色y坐标
        MCU_memory[0x1988+_17CD*0x0019+0x0006] = _17D3;
        // 指向背包中的道具
        *(UINT16*)(MCU_memory+0x1988+_17CD*0x0019+0x0017) = *(UINT16*)(MCU_memory+0x192F);
        // 角色资源ID
        if (MCU_memory[0x1988 + MCU_memory[0x1A94] * 0x0019 + 0x01] == MCU_memory[0x1988+_17CD*0x0019+0x01])
        {
            _5412:
            __8D96();
        }
        _541D:
        break;
    case 0x0002:  // _5420 NPC角色
    case 0x0004:  // _5420 场景对象
        _0020E969(_17CF);
        __8EFB(_17CF, _17D0, _17D1);
        if (MCU_memory[0x1935])
        {
            _545C:
            return;
        }
    _545F:
        // x坐标
        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0005] = _17D2;
        // y坐标
        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0006] = _17D3;
        // x坐标
        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0007] = _17D2;
        // y坐标
        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0008] = _17D3;
        __8EF5(_17CF);
        break;
    case 0x0003:  // _5581 敌人角色
        //          敌人角色 ID
        __8EFB(_17CF, 0x03, _17D1);
        if (MCU_memory[0x1935])
        {
            _55B4:
            return;
        }
        _55B7:
        __8E03(_17CF, 0x00);
        return;
    default:  // _55DE
        MCU_memory[0x1935] = ACTOR_ERR;
        return;
    }
    _55E6:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

void _0020C61F(UINT8 _17CF)
{
    UINT8 _17CE;
    for (_17CE=0x00; _17CE<0x03; _17CE++)
    {
        _5653:
        // 角色资源ID
        if (MCU_memory[0x1988+_17CE*0x0019+0x01] == _17CF)
        {
            _568F:
            break;
        }
    }
    _5695:
    if (_17CE < 0x03)
    {
        _56A1:
        // 角色资源ID
        MCU_memory[0x1988+_17CE*0x0019+0x0001] = 0x00;
        // 释放指向角色姓名的内存空间
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1988+_17CE*0x0019+0x0F)) == 0x00)
        {
            _574C:
            MCU_memory[0x1935] = MEM_ERR;
            return;
        }
        _5754:
        // 释放指向角色属性的内存空间
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1988+_17CE*0x0019+0x11)) == 0x00)
        {
            _57B6:
            MCU_memory[0x1935] = MEM_ERR;
            return;
        }
        _57BE:
        // 释放指向角色装备的内存空间
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1988+_17CE*0x0019+0x13)) == 0x00)
        {
            _5820:
            MCU_memory[0x1935] = MEM_ERR;
            return;
        }
        _5828:
        // 释放指向角色魔法链数据的内存空间
        if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x1988+_17CE*0x0019+0x15)) == 0x00)
        {
            _588A:
            MCU_memory[0x1935] = MEM_ERR;
            return;
        }
    }
    else
    {
        _5895:
        MCU_memory[0x1935] = ID_ERR;
    }
}

void _0020C8AB(UINT8 _17CF)
{
    if (_17CF < 0x28)
    {
        _58CA:
        if (*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) != 0x0000)
        {
            _5917:
            if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))) == 0x00)
            {
                _5966:
                MCU_memory[0x1935] = MEM_ERR;
            }
            else
            {
                _596E:
                *(UINT16*)(MCU_memory+0x19D3+(_17CF<<1)) = 0x0000;
            }
        }
        else
        {
            _59A7:
            MCU_memory[0x1935] = NPC_ERR;
        }
    }
    else
    {
        _59AF:
        MCU_memory[0x1935] = ID_ERR;
    }
}

void _0020C9C5(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    if (MCU_memory[0x1935])
    {
        _59E0:
        return;
    }
    _59E3:
    if (_17CF < 0x28)
    {
        _59EF:
        if (*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1)) != 0x0000)
        {
        _5A26:
            // x坐标
            if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x05] > _17D0)
            {
            _5A60:
                // 面相左
                MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0002] = 0x04;
            }
            // x坐标
            else if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x05] < _17D0)
            {
                _5ADA:
                // 面相右
                MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0002] = 0x02;
            }
        _5B1C:
            // x坐标
            while (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x05] != _17D0)
            {
                _5B51:
                // x坐标
                if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x05] > _17D0)
                {
                    _5B8B:
                    // x坐标
                    MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) + 0x0005] = MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) + 0x05] - 0x01;
                }
                else
                {
                    _5C04:
                    // x坐标
                    if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x05] < _17D0)
                    {
                        _5C39:
                        // x坐标
                        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0005] = MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) + 0x05] + 0x01;
                    }
                }
                _5CAF:
                _0020CF6D(_17CF);
            }
        _5CB9:
            // y坐标
            if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x06] > _17D1)
            {
                _5CF3:
                // 面相上
                MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0002] = 0x01;
            }
            // y坐标
            else if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x06] < _17D1)
            {
            _5D6D:
                // 面相下
                MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0002] = 0x03;
            }
            _5DAF:
            // y坐标
            while (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x06] != _17D1)
            {
                _5DE4:
                // y坐标
                if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x06] > _17D1)
                {
                    _5E1E:
                    // y坐标
                    MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0006] = MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) + 0x06] - 0x01;
                }
                // y坐标
                else if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x06] < _17D1)
                {
                    _5ECC:
                    // y坐标
                    MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0006] = MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) + 0x06] + 0x01;
                }
                _5F42:
                _0020CF6D(_17CF);
            }
            _5F4C:
            return;
        }
        _5F4F:
        MCU_memory[0x1935] = NPC_ERR;
    }
    else
    {
        _5F57:
        MCU_memory[0x1935] = ID_ERR;
    }
}

void _0020CF6D(UINT8 _17CF)
{
    // 脚步
    MCU_memory[*(UINT16*)(MCU_memory + 0x19D3 + (_17CF << 1)) + 0x0003]++;
    if (MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x03] > 0x03)
    {
        _6029:
        MCU_memory[*(UINT16*)(MCU_memory+0x19D3+(_17CF<<1))+0x0003] = 0x00;
    }
    _606B:
    __8DA2();
    __8D96();
    __8D99();
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// _17CF:特效类型。_17D2:特效序号。_17D1:bit0代表是否支持按键跳过动画,bit1代表是否清空背景。_17D2:特效x坐标。_17D3:特效y坐标。
UINT8 _0020D0C5(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3)
{
    UINT8* _17C7; // 一组指针，指向特效中所有图片数据
    UINT8* _17C5=0;
    UINT8* _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT8 _17BF;
    UINT16 width;
    UINT16 height;
    UINT16 _17B9;
    UINT16 _17B7; // 最大一张图片所有信息占有字节数
    UINT16 _17B5;
    UINT16 _17B3;
    UINT8 _17B1;
    UINT8 _17B0;
    UINT8 _17AF;
    UINT8 _17AE;
    UINT8 x1;
    UINT8 y1;
    UINT8 _17A9;
    UINT8* _17A7;
    UINT8* _17A5;
    UINT8* _17A2;
    if (MCU_memory[0x1935])
    {
        _60E0:
        return 0xFF;
    }
    _60E5:
    _17B3 = 0x0000;
    _17C7 = MCU_memory+0x4800;
    _17B1 = 0x01;
    _17B0 = 0x01;
    _17B7 = 0x0000;
    _17C3 = __8F0A(DAT_SPECRES, _17CF, _17D0);
    if (MCU_memory[0x1935])
    {
        _614C:
        MCU_memory[0x1935] = 0x00;
        _17C3 = __8F0A(DAT_SPECRES, _17CF, 0x01);
        if (MCU_memory[0x1935])
        {
            _618D:
            return 0xFF;
        }
    }
_6192:
    // 特效帧数
    _17C2 = _17C3[0x0002];
    // 最大图号
    _17C1 = _17C3[0x0003];
    // 起始帧
    _17C0 = _17C3[0x0004];
    // 终止帧
    _17BF = _17C3[0x0005];
    _17C3 += 0x0006;
    // 存储所有帧的nShow
    _17A7 = SysMemAllocate(_17C2+0x0A);
    if (_17A7 == NULL)
    {
        _62AE:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
_62B8:
    // 存储所有帧的Show
    _17A5 = SysMemAllocate(_17C2+0x0A);
    if (_17A5 == NULL)
    {
        _6309:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
_6313:
    // 指向帧信息
    _17A2 = _17C3;
    // 特效帧数
    _17B3 = _17C2;
    // 指向图片资源，帧信息占五个字节：x,y,Show,nShow,图号
    _17C3 += _17B3*0x0005;
    for (UINT8 _17B2=0x00; _17B2<=_17BF-_17C0; _17B2++)
    {
        _63AD:
        _17A7[_17B2] = _17A2[(_17B2 + _17C0) * 0x0005 + 0x03]; // nShow
        _17A5[_17B2] = _17A2[(_17B2 + _17C0) * 0x0005 + 0x02]; // Show
    }
    _6480:
    for (UINT8 _17B2=0x00; _17B2<_17C1; _17B2++)
    {// 遍历所有图片
    _64A3:
        // 宽
        width = _17C3[0x0002];
        // 高
        height = _17C3[0x0003];
        // 透明编码
        _17B5 = _17C3[0x0005];
        _17B9 = width /0x0008;
        if (width %0x0008 != 0x0000)
        {
            _6589:
            _17B9++;
        }
        _6599:
        _17B9 *= height;
        _17B9 *= _17B5;
        _17B9 += 0x0006; // 一张图片所有信息占有字节数
        *(UINT16*)((_17B2 << 1) + _17C7) = (UINT16)(_17C3 - MCU_memory);
        _17C3 += _17B9;
        if (_17B7 < _17B9)
        {
            _668C:
            _17B7 = _17B9;
        }
    }
    _66A5:
    if ((_17D1 & 0x02) != 0x02)
    {
        _66B3:
        _17C5 = MCU_memory + 0x4000;
        fillmem(_17C5, _17B7, 0x00);
    }
    _66FA:
    _17AF = 0x00;
    _17A9 = _17AF;
    while (0x01)
    {
        _670F:
        if ((_17D1 & 0x01) == 0x01)
        {
            _671D:
            _17AE = SysGetKey();
            if (_17AE != 0xFF)
            {
                _6738:
                if (SysMemFree(_17A7) == 0x00)
                {
                    _676B:
                    MCU_memory[0x1935] = MEM_ERR;
                    return 0xFF;
                }
                _6775:
                if (SysMemFree(_17A5) == 0x00)
                {
                    _67A8:
                    MCU_memory[0x1935] = MEM_ERR;
                    return 0xFF;
                }
                return _17AE;
            }
        }
        _67B9:
        for (UINT8 _17B2=0x00; _17B2<=_17AF; _17B2++)
        {
            _67DE:
            if (_17A5[_17B2] == 0x01)
            { // Show==0x01
                _6804:
                if ((_17D1 & 0x02) != 0x02)
                {
                _6812:
                    // 特效图片的宽
                    width = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0002];
                    // 特效图片的高
                    height = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0003];
                    // 图片的x坐标
                    x1 = _17A2[(_17B2+_17C0)*0x0005]+_17D2;
                    // 图片的y坐标
                    y1 = _17A2[(_17B2+_17C0)*0x0005+0x01]+_17D3;
                    if (y1 >= 0x80)
                    {
                        _69C8:
                        height -= (0xFF^ y1)+0x01;
                        y1 = 0x00;
                    }
                    _6A00:
                    height = (height + y1) <= 0x0060 ? height : (0x60 - y1);
                    if (height < 0x0080)
                    {
                        _6A91:
                        if (height +0x0001>0x0001)
                        {
                            _6ABE:
                            SysPictureDummy(x1, y1, width + x1 - 0x0001, height + y1 - 0x0001, _17C5, MCU_memory+*(UINT16*)(MCU_memory+0x1936), 0x00);
                        }
                    }
                }
                _6B98:
                _17B1 = 0x01;
            }
            _6B9E:
            if (_17A5[_17B2]+0x01 != 0x01)
            {
                _6BC7:
                _17A5[_17B2]--; // Show--
            }
        }
        _6BED:
        if (_17B1 == 0x01)
        {
            _6BF9:
            if ((_17D1 & 0x02) == 0x02)
            {
                _6C07:
                __8DE8();
            }
            _6C12:
            for (UINT8 _17B2=0x00; _17B2<=_17A9; _17B2++)
            {
                _6C37:
                if (_17A5[_17B2]+0x01 != 0x01)
                {
                    _6C60:
                    // 特效图片的宽
                    width = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0002];
                    // 特效图片的高
                    height = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0003];
                    // 透明编码。1不透明，2透明。
                    _17B5 = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0005];
                    // 图片的x坐标
                    x1 = _17A2[(_17B2+_17C0)*0x0005]+_17D2;
                    // 图片的y坐标
                    y1 = _17A2[(_17B2+_17C0)*0x0005+0x01]+_17D3;
                    if (_17B5 == 0x0002)
                    { // 含透明信息的图片
                        _6EB7:
                        __8DA8(x1, y1, (UINT8)width, (UINT8)height, MCU_memory + *(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0006);
                    }
                    else
                    { // 不含透明信息的图片
                        _6F80:
                        SysPictureDummy(x1, y1, width + x1 - 0x0001, height + y1 - 0x0001, MCU_memory + *(UINT16*)((_17A2[(_17B2 + _17C0) * 0x0005 + 0x04] << 1) + _17C7) + 0x0006, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
                    }
                }
            }
        }
        _70E5:
        if (_17B0 == 0x01)
        {
            _70F1:
            for (UINT8 _17B2=_17A9+0x01; _17B2<=_17AF; _17B2++)
            {
                _711B:
                if (_17A5[_17B2]+0x01 != 0x01)
                {
                    _7144:
                    // 特效图片的宽
                    width = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0002];
                    // 特效图片的高
                    height = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0003];
                    // 透明编码。1不透明，2透明。
                    _17B5 = MCU_memory[*(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0005];
                    // 图片的x坐标
                    x1 = _17A2[(_17B2+_17C0)*0x0005]+_17D2;
                    // 图片的y坐标
                    y1 = _17A2[(_17B2+_17C0)*0x0005+0x01]+_17D3;
                    if (_17B5 == 0x0002)
                    { // 含透明信息的图片
                        _739B:
                        __8DA8(x1, y1, (UINT8)width, (UINT8)height, MCU_memory + *(UINT16*)((_17A2[(_17B2+_17C0)*0x0005+0x04]<<1)+_17C7)+0x0006);
                    }
                    else
                    { // 不含透明信息的图片
                        _7464:
                        SysPictureDummy(x1, y1, width + x1 - 0x0001, height + y1 - 0x0001, MCU_memory + *(UINT16*)((_17A2[(_17B2 + _17C0) * 0x0005 + 0x04] << 1) + _17C7) + 0x0006, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
                    }
                }
            }
            _75C9:
            _17A9 = _17AF;
        }
        _75D1:
        if (_17B0 == 0x01 || _17B1 == 0x01)
        {
            _75E9:
            __8DAE(MCU_memory+*(UINT16*)(MCU_memory+0x1936));
            _17B0 = 0x00;
            _17B1 = 0x00;
        }
        else
        {
            _7620:
            for (UINT8 _17AA=0x00; _17AA<0x00FF; _17AA++)
            {
                void;
            }
        }
        _7654:
        if (_17A7[_17AF] >= 0x01)
        {
            _767A:
            _17A7[_17AF]--;
        }
        _769D:
        while (_17A7[_17AF] <= 0x01)
        {
            _76C5:
            if (_17AF+_17C0 < _17BF)
            {
                _76D8:
                _17AF++;
                _17B0 = 0x01;
            }
            else
            {
                break;
            }
        }
        _76EA:
        if (_17AF+_17C0 >= _17BF)
        {
            UINT8 _17B2;
            _76FD:
            for (_17B2=_17A9+0x01; _17B2<=_17AF; _17B2++)
            {
                _7722:
                if (_17A5[_17B2] > 0x01)
                {
                    _774D:
                    break;
                }
            }
            _7753:
            if (_17B2 > _17AF)
            {
                _7766:
                break;
            }
        }
    }
    _776C:
    if (SysMemFree(_17A7) == 0x00)
    {
        _779F:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
    _77A9:
    if (SysMemFree(_17A5) == 0x00)
    {
        _77DC:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
    _77E6:
    return 0xFF;
}

void _0020E7FE()
{
    SysPictureDummy(0x00, 0x00, 0x9E, 0x5F, MCU_memory + 0x4000, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
}

// 播放预定音乐
void _0020E850()
{
    if (MCU_memory[0x1935])
    {
        _7858:
        return;
    }
    // 如果音乐关
    if (MCU_memory[0x1AAD] == 0x00)
    {
        _7863:
        return;
    }
_7866:
    // 如果音乐持续次数为0 或 音乐号码为0
    if (MCU_memory[0x1A98] == 0x00 || MCU_memory[0x1A97] == 0x00)
    {
        _7876:
        return;
    }
    _7879:
    if (MCU_memory[0x201C] == 0x00)
    {
        _7881:
        MCU_memory[0x201C] = 0x01;
        //  音乐持续次数
        if (MCU_memory[0x1A98] != 0xFF)
        {
            _7891:
            MCU_memory[0x1A98]--; // 音乐持续次数
        }
    _789A:
        //              音乐号码
        SysPlayMelody(MCU_memory[0x1A97]);
    }
}

// 添加玩家角色
UINT8 _0020E8A9()
{
    UINT8 _17CF;
    for(_17CF=0x00; _17CF<0x03; _17CF++)
    {
        _78DA:
        // 如果第_17CF角色资源ID==0
        if (MCU_memory[_17CF*0x0019+0x1988+0x01]+0x01 == 0x01)
        {
            _7917:
            break;
        }
    }
    _791D:
    if (_17CF >= 0x03)
    {
        _7929:
        MCU_memory[0x1935] = ID_ERR;
        return 0xFF;
    }
    _7933:
    if (__8DF4(_17CF) == 0x00)
    {
        _794A:
        return 0xFF;
    }
    _794F:
    return _17CF;
}

void _0020E969(UINT8 _17CF)
{
    if (_17CF > 0x28)
    {
        _7988:
        MCU_memory[0x1935] = NPC_OVERF;
        return;
    }
    _7995:
    if (*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3) != 0x0000)
    {
        _79CC:
        MCU_memory[0x1935] = NPC_RECBEUSE;
        return;
    }
    _79D4:
    *(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) = (UINT16)(SysMemAllocate(0x0020) - MCU_memory);
    if (*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3) == 0x0000)
    {
        _7A75:
        MCU_memory[0x1935] = MEM_ERR;
    }
}

void _0020EA8B(UINT8 _17CF)
{
    UINT8 _17CE;
    for (_17CE=0x00; _17CE<0x03; _17CE++)
    {
        _7ABF:
        // 角色资源ID
        if (MCU_memory[_17CE*0x0019+0x1988+0x01] == _17CF)
        {
            _7AFB:
            break;
        }
    }
    _7B01:
    if (_17CE < 0x03)
    {
        _7B0D:
        MCU_memory[0x1A94] = _17CE;
    }
    else
    {
        _7B17:
        MCU_memory[0x1935] = ID_ERR;
    }
}

UINT8 _0020EB2D(UINT8 _17CF)
{
    // 申请角色姓名内存空间
    *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x000F) = (UINT16)(SysMemAllocate(0x000E) - MCU_memory);
    if (*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x0F) == 0x0000)
    {
        _7C14:
        MCU_memory[0x1935] = MEM_ERR;
        return 0x00;
    }
    _7C1E:
    // 申请角色属性内存空间
    *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0011) = (UINT16)(SysMemAllocate(0x001C) - MCU_memory);
    if (*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11) == 0x0000)
    {
        _7CF2:
        MCU_memory[0x1935] = MEM_ERR;
        return 0x00;
    }
    _7CFC:
    // 申请角色装备内存空间
    *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0013) = (UINT16)(SysMemAllocate(0x000A) - MCU_memory);
    if (*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x13) == 0x0000)
    {
        _7DD0:
        MCU_memory[0x1935] = MEM_ERR;
        return 0x00;
    }
    _7DDA:
    // 申请角色魔法链数据内存空间
    *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0015) = (UINT16)(SysMemAllocate(0x0068) - MCU_memory);
    if (*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x15) == 0x0000)
    {
        _7EAE:
        MCU_memory[0x1935] = MEM_ERR;
        return 0x00;
    }
    _7EB8:
    return 0x01;
}

void _0020EED0()
{
    const UINT8 _8523[] = " 档案储存中… ";
    UINT8 _17BF[0x0F];
    UINT8 _17BC[2];
    _SysMemcpy(_17BF, _8523, 0x000F);
    UINT8 _17BE = (UINT8)strlen(_17BF);
    __8E75(0x02, _17BE, _17BC);
    SysPrintString(_17BC[0], _17BC[1], _17BF);
}

UINT8 _0020EFD5(UINT8 _17CF)
{
    UINT8 _17CC;
    UINT8 datalength;
    UINT8 information[0x0A];
    UINT8 _17B7[10];
    UINT8 filehandle;
    UINT16 filename;
    UINT16 filenum;
    UINT32 filelength;
    if (MCU_memory[0x1935])
    {
        _7FF0:
        return 0x00;
    }
    _7FF5:
    // 角色资源ID
    if (MCU_memory[_17CF*0x0019+0x1988+0x01] == 0x00)
    {
        _802C:
        return 0x00;
    }
    _8031:
    fillmem(information, 0x000A, 0x00);
    // 获取游戏名字符数
    datalength = (UINT8)strlen(MCU_memory+0x1938);
    // 复制游戏名到 information
    strcpy(information, MCU_memory+0x1938);
    //                          角色资源ID
    information[datalength] = MCU_memory[_17CF * 0x0019 + 0x1988 + 0x01] + 0x64 + 0x30;
    _17CC = FileNum(0x17, &filenum);
    for (UINT16 _17B4 = 0x0001; _17B4<= filenum; _17B4++)
    {
        _81AE:
        _17CC = FileSearch(0x17, _17B4, &filename, _17B7);
        if (strcmp(information, _17B7) != 0x0000)
        {
            _8260:
            continue;
        }
        _8263:
        _17CC = FileOpen(filename, 0x17, ReadAndWrite, &filehandle, &filelength);
        if (_17CC == 0x00)
        {
            _82DB:
            return 0x00;
        }
        _82E0:
        _17CC = FileDel(filehandle);
    }
    _82F6:
    datalength = __8DD0();
    filelength *= datalength;
    _17CC = FileCreat(0x17, filelength, information, &filename, &filehandle);
    if (_17CC == 0x00)
    {
        _83A6:
        return 0x00;
    }
    _83AB:
    __8DCD(_17CF, MCU_memory+0x1CAF);
    _17CC = FileWrite(filehandle, datalength, MCU_memory+0x1CAF);
    _17CC = FileClose(filehandle);
    return 0x01;
}

UINT8 _0020F437(UINT8 _17CF)
{
    UINT8 _17CE;
    for (_17CE=0x00; _17CE<0x03; _17CE++)
    {
        _846B:
        // 角色资源ID
        if (MCU_memory[_17CE*0x0019+0x1988+0x01] == _17CF)
        {
            _84A7:
            break;
        }
    }
    _84AD:
    if (_17CE >= 0x03)
    {
        _84B9:
        MCU_memory[0x1935] = ID_ERR;
        return 0x00;
    }
    _84C3:
    if (__8DFA(_17CE) == 0x00)
    {
        _84DA:
        if (__8DFA(_17CE) == 0x00)
        {
            _84F1:
            MCU_memory[0x1935] = TEMPFILE_ERR;
            return 0x00;
        }
    }
    _84FB:
    return 0x01;
}

// _17CF:x,_17D0:y,_17D1:数值,_17D5进制,_17D6:
void _0020F532(UINT8 _17CF, UINT8 _17D0, INT32 _17D1, UINT8 _17D5, UINT8 _17D6)
{
    UINT8 _8B22[] = "0123456789ABCDEF";
    INT32 _8B33 = 0x00000000;
    UINT32 _8B37 = 0x00000000;
    UINT32 _8B3B = 0x00000000;
    UINT32 _8B3F = 0x00000001;
    UINT32 _8B43 = 0x00000001;
    UINT32 _17C7;
    UINT16 _17C5;
    UINT16 _17C3;
    UINT8 _17AF[20];
    if (_17D6 == 0x01)
    {
        _8551:
        if (_17D1 < _8B33)
        {
            _8575:
            SysAscii(_17CF, _17D0, '-');
            _17CF += 0x08;
            _17D1 = -_17D1;
        }
    }
    _85D3:
    _17C5 = 0x0000;
    for (;;)
    {
        _85DC:
        _17C7 = _17D1/(UINT32)_17D5;
        if (_17C7 > _8B37)
        {
            _862F:
            _17AF[_17C5] = _17D1 - _17C7 * (UINT32)_17D5;
            _17C5++;
            _17D1 = _17C7;
        }
        else
        {
            break;
        }
    }
    _86F7:
    _17AF[_17C5] = (UINT8)_17D1;
    _17C5++;
    for (_17C7=_8B3B; _17C7<(UINT32)_17C5; _17C7+=_8B3F)
    {
        _87BF:
        _17C3 = _17AF[_17C5 - (_17C7 + _8B43)];
        SysAscii(_17CF, _17D0, _8B22[_17C3]);
        _17CF += 0x08;
    }
}

void _0020F876()
{
    MsgType _17CB;
    do
    {
        _8886:
        _17CB.type = DICT_WM_KEY;
        _17CB.param = SysGetKey();
        if (GuiTranslateMsg(&_17CB) && _17CB.type == DICT_WM_CHAR_FUN
         && (_17CB.param == CHAR_ENTER || _17CB.param == CHAR_EXIT))
        {
            _8957:
            break;
        }
    } while (0x01);
}

void _0020F975(UINT32 _17D0)
{
    for (UINT8 _17CE=0x00; _17CE<0x08; _17CE++)
    {
        _89A8:
        SysAscii(0x08 * _17CE, 0x00, 0x20);
    }
    _89E3:
    __8F13(0x00, 0x00, _17D0, 0x10, 0x00);
}

void _0020FA30(UINT32 _17D0)
{
    __8F19(_17D0);
    __8F16();
}

void _0020FA67(UINT8 _17CF, UINT8 _17D0, UINT32 _17D1, UINT8 _17D5, UINT8 _17D6)
{
    __8F13(_17CF, _17D0, _17D1, _17D5, _17D6);
}

void _0020FABF(UINT8 _17CF, UINT8 _17D0, UINT32 _17D1, UINT8 _17D5, UINT8 _17D6)
{
    __8F13(_17CF, _17D0, _17D1, _17D5, _17D6);
    __8F16();
}

// 选择魔法界面
UINT8 _00210000(UINT16 _17D0, UINT8 _17D2, UINT8 _17D3)
{
    /*
                        
                ███  
            ██      █
            ██      █
            ██      █
            █████  
          █  ████  
        █  █          
      █  █            
    █  █              
  █  █                
█  █                  
    */
    const UINT8 _8799[] = {0x00,0x0B,0x00,0xEB,0x03,0x11,0x03,0x10,0x03,0x11,0x03,0xE3,0x05,0xEB,0x0A,0x0E,0x14,0x02,0x28,0x02,0x50,0x0F,0xA0,0x08};
    /*
    █    
  █  █  
█      █
█  █  █
█  █  █
█      █
  █  █  
    █    
    */
    const UINT8 _87B4[] = {0x24,0x50,0x88,0xA8,0xA8,0x88,0x50,0x20};
    /*
    █    
  ███  
█████
█████
█████
█████
  ███  
    █    
    */
    const UINT8 _87BD[] = {0x24,0x70,0xF8,0xF8,0xF8,0xF8,0x70,0x20};
    const UINT8 _87C6[] = "耗真气:";
    MsgType _17CC;
    UINT8 _17C0[0x08];
    UINT8 _17A0[0x20];
    UINT8 _179F;
    UINT8 _179E;
    UINT8 _179D;
    UINT8* _179B;
    UINT16 _1799;
    UINT8 _1798;
    UINT8 _1797;
    UINT8* _1795;
    if (_17D2+0x01 == 0x01)
    {
        _501F:
        return 0xFF;
    }
    _5024:
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    _GetDataBankNumber(0x09, &_1799);
    _SysMemcpy(_17A0, _8799, 0x0020);
    if (_17D3+0x01 >= _17D2)
    {
        _50C7:
        _17D3 = _17D2-0x01;
        if (_17D3+0x01 == 0x01)
        {
            _50E1:
            _179E = 0x00;
        }
        else
        {
            _50EA:
            _179E = _17D3-0x01;
        }
    }
    else
    {
        _50F8:
        _179E = _17D3;
    }
    _5100:
    _179D = _17D3;
    if (_179D == _179E)
    {
        _5116:
        SysPicture(0x63, 0x09, 0x6E, 0x14, _17A0, 0x00);
    }
    else
    {
        _515A:
        SysPicture(0x63, 0x19, 0x6E, 0x24, _17A0, 0x00);
    }
    _519B:
    _179F = 0x03;
    SysRect(0x0A, 0x04, 0x93, 0x27);
    SysRect(0x0A, 0x29, 0x93, 0x4C);
    _SysMemcpy(_17A0, _87B4, 0x0020);
    SysPicture(0x84, 0x0F, 0x88, 0x16, _17A0, 0x00);
    SysPicture(0x84, 0x17, 0x88, 0x1E, _17A0, 0x00);
    _SysMemcpy(_17C0, _87C6, 0x0008);
    SysPrintString(0x0A, 0x4E, _17C0);
    _1798 = 0x00;
    _1797 = 0xFF;
    while (0x01)
    {
        _5362:
        if ((_179F & 0x01) == 0x01)
        {
            _5370:
            _DataBankSwitch(0x09, 0x04, _1799);
            SysLcdPartClear(0x0E, 0x06, 0x5E, 0x26);
            _179B = __8F0A(DAT_MGICRES, MCU_memory[_179E * 0x0002 + _17D0], MCU_memory[_179E*0x0002+_17D0+0x01]);
            if (MCU_memory[0x1935])
            {
                _546C:
                return 0xFF;
            }
            _5471:
            _1795 = _179B;
            // 显示法术名称
            SysPrintString(0x0E, 0x06, _1795+0x0006);
            if (_179E+0x01 < _17D2)
            {
                _54D5:
                _179B = __8F0A(DAT_MGICRES, MCU_memory[(_179E + 0x01) * 0x0002 + _17D0], MCU_memory[(_179E + 0x01) * 0x0002 + _17D0 + 0x01]);
                if (MCU_memory[0x1935])
                {
                    _5583:
                    return 0xFF;
                }
                _5588:
                _1795 = _179B;
                // 显示法术名称
                SysPrintString(0x0E, 0x16, _1795+0x0006);
            }
            _55DB:
            if (_179E+0x01 > 0x01)
            {
                _55EF:
                _SysMemcpy(_17A0, _87BD, 0x0020);
            }
            else
            {
                _562B:
                _SysMemcpy(_17A0, _87B4, 0x0020);
            }
            _5664:
            SysPicture(0x84, 0x07, 0x88, 0x0E, _17A0, 0x00);
            if (_179E+0x02 < _17D2)
            {
                _56C6:
                _SysMemcpy(_17A0, _87BD, 0x0020);
            }
            else
            {
                _5702:
                _SysMemcpy(_17A0, _87B4, 0x0020);
            }
            _573B:
            SysPicture(0x84, 0x1F, 0x88, 0x26, _17A0, 0x00);
            _179F &= 0xFE;
        }
        _5796:
        if ((_179F & 0x02) == 0x02)
        {
            _57A4:
            SysLcdPartClear(0x0B, 0x2A, 0x92, 0x4B);
            _179B = __8F0A(DAT_MGICRES, MCU_memory[_179D * 0x0002 + _17D0], MCU_memory[_179D*0x0002+_17D0+0x01]);
            if (MCU_memory[0x1935])
            {
                _5880:
                return 0xFF;
            }
            _5885:
            _1795 = _179B;
            //                                              魔法描述
            _1798 = __8EC7(0x0C, 0x2B, 0x91, 0x4A, _1798, _1795+0x001A);
            SysLcdPartClear(0x42, 0x50, 0x9E, 0x5F);
            //                  耗费真气
            __8F1F(0x42, 0x50, (UINT32)_1795[0x04], 0x0A, 0x01);
            _179F &= 0xFD;
        }
        _597F:
        if (GuiGetMsg(&_17CC) == 0x00)
        {
            _59B4:
            continue;
        }
        _59B7:
        if (GuiTranslateMsg(&_17CC) == 0x00)
        {
            _59EC:
            continue;
        }
        _59EF:
        if (_17CC.type == DICT_WM_CHAR_FUN)
        {
            _5A20:
            switch (_17CC.param)
            {
            case CHAR_UP: // _5A4A
            case CHAR_LEFT: // _5A4A
                if (_1798 > 0x01)
                {
                    _5A5E:
                    _179D--;
                    if (_179D < _179E)
                    {
                        _5A77:
                        _179E = _179D;
                        _179F |= 0x01;
                    }
                    else
                    {
                        _5A8C:
                        SysLcdPartClear(0x63, 0x09, 0x6E, 0x24);
                        _SysMemcpy(_17A0, _8799, 0x0020);
                        SysPicture(0x63, 0x09, 0x6E, 0x14, _17A0, 0x00);
                    }
                    _5B32:
                    _1798 = 0x00;
                    _1797 = 0xFF;
                    _179F |= 0x02;
                }
                break;
            case CHAR_RIGHT: // _5B4B
            case CHAR_DOWN: // _5B4B
                if (_179D+0x01 < _17D2)
                {
                    _5B5C:
                    _179D++;
                    if (_179D > _179E+0x01)
                    {
                        _5B81:
                        _179E = _179D-0x01;
                        _179F |= 0x01;
                    }
                    else
                    {
                        _5B99:
                        SysLcdPartClear(0x63, 0x09, 0x6E, 0x24);
                        _SysMemcpy(_17A0, _8799, 0x0020);
                        SysPicture(0x63, 0x19, 0x6E, 0x24, _17A0, 0x00);
                    }
                    _5C3F:
                    _1798 = 0x00;
                    _1797 = 0xFF;
                    _179F |= 0x02;
                }
                break;
            case CHAR_PGUP: // _5C58
                if (_1798 > 0x01)
                {
                    _5C64:
                    _1797 = _1798;
                    _1798 -= 0x02;
                    _179F |= 0x02;
                }
                break;
            case CHAR_PGDN: // _5C89
                if (_1798 >= 0x01)
                {
                    _5C95:
                    if (_1798 != _1797)
                    {
                        _5CA3:
                        _1797 = _1798;
                        _179F |= 0x02;
                    }
                }
                break;
            case CHAR_ENTER: // _5CB8
                _DataBankSwitch(0x09, 0x04, _1799);
                return _179D;
                break;
            case CHAR_EXIT: // _5CEA
                _DataBankSwitch(0x09, 0x04, _1799);
                return 0xFF;
                break;
            }
        }
        else if (_17CC.type == DICT_WM_COMMAND)
        {
            _5D1D:
            if (_17CC.param == CMD_RETURN_HOME)
            {
                _5D38:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
}

UINT8 _00210D5B(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8* _17D3, UINT8 _17D5, UINT8* _17D6)
{
    /*
                
                
      █        
    ███      
  █████    
███████  
                
                
    */
    UINT8 _87EE[9] = {0x00,0x00,0x10,0x38,0x7C,0xFE,0x00,0x00,0x00 };
    /*
                
                
███████  
  █████    
    ███      
      █        
                
                
    */
    UINT8 _87F7[9] = {0x00,0x00,0xFE,0x7C,0x38,0x10,0x00,0x00,0x00 };
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17B8[0x09];
    MsgType _17B5;
    _17CB = _17D1*0x08;
    if (_17CB > 0x009C-_17CF)
    {
        _5DBB:
        return 0xFF;
    }
    _5DC0:
    if (_17CF < 0x01)
    {
        _5DCC:
        _17CF = 0x01;
    }
    _5DD2:
    if (_17D0 < 0x01)
    {
        _5DDE:
        _17D0 = 0x01;
    }
    _5DE4:
    _17C8 = _17CF+_17CB+0x04;
    if (_17D0 > 0x32)
    {
        _5E05:
        _17D0 = 0x32;
    }
    _5E0B:
    if (_17D2 > 0x03)
    {
        _5E1C:
        if (_17D0 > 0x2A)
        {
            _5E2D:
            _17D0 = 0x2A;
        }
    }
    _5E33:
    _17CA = (0x5F-_17D0-0x04)/0x10;
    if (_17D2 > _17CA)
    {
        _5E5D:
        _17C5 = _17CA*0x10+_17D0+0x04;
        if (_17D2-_17CA > 0x01)
        {
            _5E8A:
            _17CA--;
        }
        else
        {
            _5E98:
            _17C5 += 0x08;
            if (_17C5 > 0x5E)
            {
                _5EB4:
                _17CA--;
                _17C5 = _17CA*0x10+_17D0+0x10+0x04;
            }
        }
    }
    else
    {
        _5EDC:
        _17CA = _17D2;
        _17C5 = _17CA*0x10+_17D0+0x04;
    }
    _5EFB:
    if (_17D5+0x01 > _17D2)
    {
        _5F11:
        _17D5 = _17D2;
        if (_17D5 > _17CA-0x01)
        {
            _5F33:
            _17C4 = _17D5-(_17CA-0x01);
        }
        else
        {
            _5F4A:
            _17C4 = 0x00;
        }
    }
    else
    {
        _5F53:
        if (_17D2 - _17D5 > _17CA-0x01)
        {
            _5F7A:
            _17C4 = _17D5;
        }
        else if (_17D2 > _17CA)
        {
            _5F98:
            _17C4 = _17D2-(_17CA-0x01);
        }
        else
        {
            _5FAF:
            _17C4 = 0x00;
        }
    }
    _5FB5:
    if (*_17D6+_17CA > _17D5)
    {
        _5FD8:
        if (*_17D6 <= _17D5)
        {
            _5FF3:
            _17C4 = *_17D6;
        }
    }
    _6006:
    _17C3 = _17D5;
    _17C2 = 0x10;
    SysLcdPartClear(_17CF - 0x01, _17D0 - 0x01, _17C8 + 0x01, _17C5+0x01);
    SysRect(_17CF, _17D0, _17C8, _17C5);
    _17C9 = _17CF+0x02;
    _17C8 -= 0x02;
    _17C7 = _17D0+0x02;
    _17C5 -= 0x02;
    _17D0 += 0x02;
    while (0x01)
    {
        _60DE:
        if ((_17C2 & 0x10) == 0x10)
        {
            _60EC:
            if (_17C4 >= 0x01)
            {
                _60F8:
                _17C7 = _17D0+0x08;
            }
            else
            {
                _6106:
                _17C7 = _17D0;
            }
            _610E:
            _17C6 = _17CA*0x10+_17C7;
            SysLcdPartClear(_17C9, _17D0, _17C8, _17C5);
            _17CB = _17D1*_17C4;
            __8EC7(_17C9, _17C7, _17C8, _17C6, _17C4, _17D3);
            if (_17C4 >= 0x01)
            {
                _61BC:
                _SysMemcpy(_17B8, _87EE, 0x0009);
                _17CB = (_17D1 - 0x01) * 0x04 + _17C9;
                SysPicture(_17CB, _17D0, _17CB + 0x07, _17D0 + 0x07, _17B8, 0x00);
            }
            _626D:
            if (_17C4+_17CA < _17D2)
            {
                _6280:
                _SysMemcpy(_17B8, _87F7, 0x0009);
                _17CB = (_17D1 - 0x01) * 0x04 + _17C9;
                SysPicture(_17CB, _17C5 - 0x08, _17CB + 0x07, _17C5 - 0x08 + 0x07, _17B8, 0x00);
            }
            _633F:
            _17CB = (_17C3-_17C4)*0x10+_17C7;
            SysLcdReverse(_17C9, _17CB, _17C8, _17CB+0x0F);
            _17C2 = 0x00;
        }
        _6397:
        if (GuiGetMsg(&_17B5) == 0x00)
        {
            _63CC:
            continue;
        }
        _63CF:
        if (GuiTranslateMsg(&_17B5) == 0x00)
        {
            _6404:
            continue;
        }
        _6407:
        if (_17B5.type == DICT_WM_CHAR_FUN)
        {
            _6413:
            switch (_17B5.param)
            {
            case CHAR_UP: // _643D
            case CHAR_LEFT: // _643D
                if (_17C3 >= 0x01)
                {
                    _6449:
                    if (_17C3-0x01 < _17C4)
                    {
                        _645A:
                        _17C4--;
                        _17C2 = 0x10;
                    }
                    else
                    {
                        _646E:
                        _17CB = (_17C3-_17C4)*0x10+_17C7;
                        SysLcdReverse(_17C9, _17CB, _17C8, _17CB+0x0F);
                        SysLcdReverse(_17C9, _17CB - 0x10, _17C8, _17CB-0x10+0x0F);
                    }
                    _6505:
                    _17C3--;
                }
                else
                {
                    _6513:
                    _17C3 = _17D2-0x01;
                    if (_17D2 > _17CA)
                    {
                        _6531:
                        _17C4 = _17D2-_17CA;
                    }
                    else
                    {
                        _6541:
                        _17C4 = 0x00;
                    }
                    _6547:
                    _17C2 = 0x10;
                }
                break;
            case CHAR_DOWN: // _6550
            case CHAR_RIGHT: // _6550
                if (_17C3 < _17D2-0x01)
                {
                    _6565:
                    _17CB = _17C3+0x01-_17C4+0x01;
                    if (_17CB > _17CA)
                    {
                        _658B:
                        _17C4++;
                        _17C2 = 0x10;
                    }
                    else
                    {
                        _659F:
                        _17CB = (_17C3-_17C4)*0x10+_17C7;
                        SysLcdReverse(_17C9, _17CB, _17C8, _17CB+0x0F);
                        SysLcdReverse(_17C9, _17CB + 0x10, _17C8, _17CB+0x10+0x0F);
                    }
                    _6636:
                    _17C3++;
                }
                else
                {
                    _6644:
                    _17C3 = 0x00;
                    _17C4 = 0x00;
                    _17C2 = 0x10;
                }
                break;
            case CHAR_ENTER: // _6659
                *_17D6 = _17C4;
                return _17C3;
                break;
            case CHAR_EXIT: // _667E
                *_17D6 = _17C4;
                return 0xFF;
                break;
            }
        }
        else if (_17B5.type == DICT_WM_COMMAND)
        {
            _66B0:
            if (_17B5.param == CMD_RETURN_HOME)
            {
                _66CB:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
}

// _17D0:指向背包中的道具，_17D2:背包中的道具种类数
UINT8 _002116EB(UINT8* _17D0, UINT8 _17D2, UINT8* _17D3, UINT8 _17D5, UINT8 _17D6, UINT8* _17D7)
{
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8* _17C6;
    UINT8 _17C5;
    UINT8* _17C3;
    _17C3 = SysMemAllocate(_17D2*0x03+0x0A);
    if (_17C3 == NULL)
    {
        _6753:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
    _675D:
    _17C6 = SysMemAllocate(_17D2+0x0A);
    if (_17C6 == NULL)
    {
        _67AE:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
    _67B8:
    _17C8 = 0x00;
    _17C5 = 0x00;
    // 遍历背包中的道具
    for (UINT8 _17CB=0x00; _17CB<_17D2; _17CB++)
    {
        _67E7:
        for (_17CA=0x00; _17CA<_17D5; _17CA++)
        {
            _680A:
            if (_17D0[_17CB * 0x0003] == _17D3[_17CA])
            {
                _686F:
                break;
            }
        }
        _6875:
        if (_17CA < _17D5)
        {
            _6883:
            _SysMemcpy(_17C8 * 0x0003 + _17C3, _17CB * 0x0003 + _17D0, 0x0003);
            _17C6[_17C8] = _17CB;
            if (_17CB <= _17D6)
            {
                _699B:
                _17C5 = _17C8;
            }
            _69A3:
            _17C8++;
        }
    }
    _69B1:
    _17C9 = __8EC1(0x03, _17C8, _17C3, _17C5, _17D7);
    if (SysMemFree(_17C3) == 0x00)
    {
        _6A31:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
    _6A3B:
    if (SysMemFree(_17C6) == 0x00)
    {
        _6A6E:
        MCU_memory[0x1935] = MEM_ERR;
        return 0xFF;
    }
    _6A78:
    if (_17C9 == 0xFF)
    {
        _6A84:
        return 0xFF;
    }
    _6A89:
    return _17C6[_17C9];
}

void _00211ABD(UINT8* _17D0, UINT8 _17D2)
{
    const UINT8 _8818[] = "等级";
    const UINT8 _881D[] = "生命";
    const UINT8 _8822[] = "真气";
    const UINT8 _8827[] = "攻击力";
    const UINT8 _882E[] = "防御力";
    const UINT8 _8835[] = "经验值";
    const UINT8 _883C[] = "身法";
    const UINT8 _8841[] = "灵力";
    const UINT8 _8846[] = "幸运";
    const UINT8 _884B[] = "免疫";
    const UINT8 _8850[] = "MAX";
    const UINT8 _8854[] = "/";
    const UINT8 _8856[] = "/";
    const UINT8 _8858[] = "毒";
    const UINT8 _885B[] = "乱";
    const UINT8 _885E[] = "封";
    const UINT8 _8861[] = "眠";
    const UINT8 _8864[] = "无";
    UINT8 _17C0[0x07];
    UINT8 x;
    UINT16 _17BB;
    SysLcdPartClear(0x25, 0x00, 0x9E, 0x5F);
    SysLine(0x26, 0x0A, 0x26, 0x56);
    if (_17D2%0x02+0x01 == 0x01)
    { // 属性第一页
        _6B43:
        _SysMemcpy(_17C0, _8818, 0x0005);
        SysPrintString(0x2B, 0x05, _17C0);
        _SysMemcpy(_17C0, _881D, 0x0005);
        SysPrintString(0x2B, 0x17, _17C0);
        _SysMemcpy(_17C0, _8822, 0x0005);
        SysPrintString(0x2B, 0x29, _17C0);
        _SysMemcpy(_17C0, _8827, 0x0007);
        SysPrintString(0x2B, 0x3B, _17C0);
        _SysMemcpy(_17C0, _882E, 0x0007);
        SysPrintString(0x2B, 0x4D, _17C0);
        // 等级
        if (_17D0[0] < 0xF0)
        {
        _6D71:
            //                  等级
            __8F1F(0x62, 0x05, (UINT32)_17D0[0], 0x0A, 0x01);
        }
        else
        {
            _6DBD:
            _SysMemcpy(_17C0, _8850, 0x0007);
            SysPrintString(0x62, 0x05, _17C0);
        }
        _6E28:
        _SysMemcpy(_17C0, _8854, 0x0003);
        //                  hp
        __8F1F(0x62, 0x17, (UINT32)*(UINT16*)(_17D0 + 0x08), 0x0A, 0x01);
        SysPrintString(0x7C, 0x17, _17C0);
        //                  hpMax
        __8F1F(0x85, 0x17, (UINT32)*(UINT16*)(_17D0 + 0x06), 0x0A, 0x01);
        //                  mp
        __8F1F(0x62, 0x29, (UINT32)*(UINT16*)(_17D0 + 0x0C), 0x0A, 0x01);
        SysPrintString(0x7C, 0x29, _17C0);
        //                  mpMax
        __8F1F(0x85, 0x29, (UINT32)*(UINT16*)(_17D0 + 0x0A), 0x0A, 0x01);
        //                  攻击
        __8F1F(0x62, 0x3B, (UINT32)*(UINT16*)(_17D0 + 0x0E), 0x0A, 0x01);
        //                  防御
        __8F1F(0x62, 0x4D, (UINT32)*(UINT16*)(_17D0 + 0x10), 0x0A, 0x01);
    }
    else
    { // 属性第二页
        _7112:
        _SysMemcpy(_17C0, _8835, 0x0007);
        SysPrintString(0x2B, 0x05, _17C0);
        _SysMemcpy(_17C0, _883C, 0x0005);
        SysPrintString(0x2B, 0x17, _17C0);
        _SysMemcpy(_17C0, _8841, 0x0007);
        SysPrintString(0x2B, 0x29, _17C0);
        _SysMemcpy(_17C0, _8846, 0x0005);
        SysPrintString(0x2B, 0x3B, _17C0);
        _SysMemcpy(_17C0, _884B, 0x0005);
        SysPrintString(0x2B, 0x4D, _17C0);
        // 当前经验
        _17BB = *(UINT16*)(_17D0+0x12);
        UINT8 _17BF;
        // _17BB为0跳出循环
        for (_17BF=0x00; _17BB/=0x0A; _17BF++)
        {
            void;
        }
        _739F:
        _SysMemcpy(_17C0, _8856, 0x0003);
        //                  当前经验
        __8F01(0x62, 0x07, (UINT32)*(UINT16*)(_17D0 + 0x12), 0x00);
        SysPrintString(0x7A - (0x04 - _17BF) * 0x04, 0x05, _17C0);
        //                                              升级经验
        __8F01(0x0083 - (0x04 - _17BF) * 0x04, 0x0C, (UINT32)*(UINT16*)(_17D0 + 0x14), 0x00);
        //                  身法
        __8F1F(0x62, 0x17, (UINT32)_17D0[0x16], 0x0A, 0x01);
        //                  灵力
        __8F1F(0x62, 0x29, (UINT32)_17D0[0x17], 0x0A, 0x01);
        //                  幸运
        __8F1F(0x62, 0x3B, (UINT32)_17D0[0x18], 0x0A, 0x01);
        _17BF = 0x00;
        // 对毒的免疫
        if (_17D0[0x01]&0x08)
        {
            _7612:
            _SysMemcpy(_17C0, _8858, 0x0003);
            _17BF++;
        }
        _7656:
        // 对乱的免疫
        if (_17D0[0x01]&0x04)
        {
            _766C:
            _SysMemcpy(_17C0 + _17BF * 0x03, _885B, 0x0003);
            _17BF++;
        }
        _76E0:
        // 对封的免疫
        if (_17D0[0x01]&0x02)
        {
            _76F6:
            _SysMemcpy(_17C0 + _17BF * 0x03, _885E, 0x0003);
            _17BF++;
        }
        _776A:
        // 对眠的免疫
        if (_17D0[0x01]&0x01)
        {
            _7780:
            _SysMemcpy(_17C0 + _17BF * 0x03, _8861, 0x0003);
            _17BF++;
        }
        _77F4:
        for (UINT8 _17BE=0x00; _17BE<_17BF; _17BE++)
        {
            _7817:
            x = _17BE * 0x13 + 0x62 -(_17BF>>0x02)*0x0E;
            SysPrintString(x, 0x4D, _17C0 + _17BE * 0x03);
            if (_17BE >= 0x01)
            {
                _78B8:
                SysLine(x - 0x02, 0x4D, x - 0x02, 0x5C);
            }
        }
        _7901:
        // 没有对特殊效果免疫的能力
        if (_17BF == 0x00)
        {
            _790A:
            _SysMemcpy(_17C0, _8864, 0x0003);
            SysPrintString(0x62, 0x4D, _17C0);
        }
    }
}

void _00212986()
{
    /*
█            
██          
███        
████      
█████    
██████  
███████
██████  
█████    
████      
███        
██          
█            
              
    */
    UINT8 _8867[] = {0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00};
    UINT8 _17B4[0x0E];
    UINT8 _17B3;
    UINT8 _17B2;
    UINT8 _17AE[3];
    UINT8 _17AD;
    UINT8 _17AC;
    MsgType _17A8;
    SysLcdPartClear(0x00, 0x00, 0x24, 0x5F);
    _17AD = 0x00;
    for (UINT8 _17AB=0x00; _17AB<0x03; _17AB++)
    {
        _79E9:
        // 角色资源ID
        if (MCU_memory[_17AB*0x0019+0x1988+0x01] >= 0x01)
        {
            _7A23:
            _17AE[_17AD] = _17AB;
            //                          角色资源ID
            __8F04(DAT_SUNDRYPIC, 0x01, MCU_memory[_17AB * 0x0019 + 0x1988 + 0x01], 0x00, 0x08, _17AD*0x20+0x02);
            _17AD++;
        }
    }
    _7ADB:
    if (_17AD < 0x01)
    {
        _7AE7:
        return;
    }
    _7AEA:
    _SysMemcpy(_17B4, _8867, 0x0E);
    _17B3 = 0x02;
    _17AC = 0x00;
    _17B2 = 0x00;
    while (0x01)
    {
        _7B3C:
        if (_17B3 == 0x01)
        {
            _7B48:
            //      指向角色属性
            __8EB8(MCU_memory + *(UINT16*)(MCU_memory + _17AE[_17AC] * 0x0019 + 0x1988 + 0x11), _17B2);
            _17B3 = 0x00;
        }
        else if (_17B3 == 0x02)
        {
            _7BF1:
            //      指向角色属性
            __8EB8(MCU_memory + *(UINT16*)(MCU_memory + _17AE[_17AC] * 0x0019 + 0x1988 + 0x11), _17B2);
            SysLcdPartClear(0x01, 0x02, 0x07, 0x5F);
            SysPicture(0x01, _17AC * 0x20 + 0x0C, 0x07, _17AC * 0x20 + 0x0C + 0x0D, _17B4, 0x00);
            _17B3 = 0x00;
        }
        _7D23:
        if (GuiGetMsg(&_17A8) == 0x00)
        {
            _7D58:
            continue;
        }
        _7D5B:
        if (GuiTranslateMsg(&_17A8) == 0x00)
        {
            _7D90:
            continue;
        }
        _7D93:
        if (_17A8.type == DICT_WM_CHAR_FUN)
        {
            _7DC4:
            switch (_17A8.param)
            {
            case CHAR_LEFT: // _7DEE
            case CHAR_UP: // _7DEE
                if (_17AD > 0x01)
                {
                    _7DFF:
                    if (_17AC < 0x01)
                    {
                        _7E0B:
                        _17AC = _17AD;
                    }
                    _7E13:
                    _17AC--;
                    _17B3 = 0x02;
                }
                break;
            case CHAR_RIGHT: // _7E27
            case CHAR_DOWN: // _7E27
                if (_17AD > 0x01)
                {
                    _7E38:
                    _17AC++;
                    _17AC %= _17AD;
                    _17B3 = 0x02;
                }
                break;
            case CHAR_PGUP: // _7E5D
            case CHAR_PGDN: // _7E5D
                _17B2++;
                _17B3 = 0x01;
                break;
            case CHAR_EXIT: // _7E71
                return;
                break;
            }
        }
        else if (_17A8.type == DICT_WM_COMMAND)
        {
            _7E7A:
            if (_17A8.param == CMD_RETURN_HOME)
            {
                _7E95:
                MCU_memory[0x1935] = 0xFE;
                return;
            }
        }
    }
}

void _00212EB4()
{
    const UINT8 _8891[] = "属性魔法物品系统";
    const UINT8 _88A3[] = "状态穿戴";
    const UINT8 _88AD[] = "使用装备";
    const UINT8 _88B7[] = "读入进度存储进度游戏设置结束游戏";
    const UINT8 _88D9[] = "音乐开音乐关";
    const UINT8 _88E7[] = "金钱：";
    const UINT8 _8912[] = "目前不能存档!";
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C5;
    UINT8 _17C2[3];
    UINT8 _17C1;
    UINT8 _17A1[30];
    UINT8 _178F[0x12];
    UINT8 _1785[0x0A];
    UINT8 _1763[0x22];
    UINT8 _1759[0x0A];
    UINT8 _1751[0x08];
    UINT8 _1743[0x0E];
    UINT8 _1742;
    _SysMemcpy(_178F, _8891, 0x0012);
    _SysMemcpy(_1785, _88AD, 0x000A);
    _SysMemcpy(_1763, _88B7, 0x0022);
    _SysMemcpy(_1759, _88A3, 0x000A);
    _SysMemcpy(_1743, _88D9, 0x000E);
    _SysMemcpy(_1751, _88E7, 0x0008);
    _17C5 = 0x00;
    for (UINT8 _17C1=0x00; _17C1<0x03; _17C1++)
    {
        _8041:
        // 角色资源ID
        if (MCU_memory[_17C1*0x0019+0x1988+0x01] >= 0x01)
        {
            _807B:
            _17C2[_17C5] = _17C1;
            //                                  指向角色姓名
            _SysMemcpy(_17A1 + _17C5 * 0x0A, MCU_memory + *(UINT16*)(MCU_memory + _17C1 * 0x0019 + 0x1988 + 0x0F), 0x000A);
            _17C5++;
        }
    }
    _816A:
    for (_17C1=0x00; _17C1<_17C5*0x0A; _17C1++)
    {
        _8198:
        if (_17A1[_17C1]+0x01 == 0x01)
        {
            _81BE:
            for (; _17C1%0x0A>=0x01; _17C1++)
            {
                _81E0:
                _17A1[_17C1] = 0x20;
            }
        }
    }
    _8214:
    _17A1[_17C1] = 0x00;
    _17C9 = 0x00;
    _17CB = 0x00;
    while (0x01)
    {
        _8255:
        SysLcdPartClear(0x09, 0x03, 0x65, 0x17);
        SysRect(0x0A, 0x04, 0x64, 0x16);
        SysPrintString(0x0B, 0x05, _1751);
        //                  玩家拥有的金钱
        __8F1F(0x33, 0x05, *(UINT32*)(MCU_memory + 0x1A8F), 0x0A, 0x01);
        _17CB = __8EB2(0x0A, 0x18, 0x04, 0x04, _178F, _17CB, &_17C9);
        switch (_17CB)
        {
        case 0x0000: // _8390
            _17C8 = 0x00;
            _17CA = __8EB2(0x28, 0x11, 0x04, 0x02, _1759, 0x00, &_17C8);
            if (_17CA+0x01 == 0x01)
            {
                _83FC:
                __8EBB();
            }
            else if (_17CA == 0x01)
            {
                _8416:
                __8EA5();
            }
            break;
        case 0x0001: // _8424
            if (_17C5 > 0x01)
            {
                _8435:
                _17C8 = 0x00;
                _17CA = __8EB2(0x28, 0x1E, 0x0A, _17C5, _17A1, 0x00, &_17C8);
            }
            else
            {
                _8497:
                _17CA = 0x00;
            }
            _849D:
            if (_17CA != 0xFF)
            {
                _84A9:
                __8D81(_17C2[_17CA]);
            }
            break;
        case 0x0002: // _84D2
            _17C8 = 0x00;
            _17CA = __8EB2(0x28, 0x28, 0x04, 0x02, _1785, 0x00, &_17C8);
            if (_17CA == 0x0000)
            {
                _8560:
                if (__8D7B() == 0x01)
                {
                    _8573:
                    return;
                }
            }
            else if (_17CA == 0x0001)
            {
                _8579:
                __8D78();
            }
            break;
        case 0x0003: // _858A
            _17C8 = 0x00;
            _1742 = __8EB2(0x28, 0x1E, 0x08, 0x04, _1763, 0x00, &_17C8);
            switch (_1742)
            {
            case 0x0000: // _8608
                // 读入进度
                __8EEF();
                return;
                break;
            case 0x0001: // _8616
                // 存储进度
                if (MCU_memory[0x1A9B])
                {
                    // 能存档时的处理
                    _861E:
                    __8EEC();
                }
                else
                {
                    // 不能存档时的处理
                    _862C:
                    _SysMemcpy(_17A1, _8912, 0x000F);
                    GuiMsgBox(_17A1, 0x00C8);
                }
                return;
                break;
            case 0x0002: // _869E
                // 游戏设置
                _17C8 = 0x00;
                _1742 = __8EB2(0x50, 0x32, 0x06, 0x02, _1743, 0x00, &_17C8);
                if (_1742+0x01 == 0x01)
                {
                    _870A:
                    MCU_memory[0x1AAD] = 0x01; // 音乐开
                    MCU_memory[0x201C] = 0x00;
                    __8DEB();
                }
                else if (_1742 == 0x01)
                {
                    _872E:
                    MCU_memory[0x1AAD] = 0x00; // 音乐关
                    SysStopMelody();
                }
                break;
            case 0x0003: // _8741
                // 结束游戏
                MCU_memory[0x1935] = 0xFD;
                break;
            }
            break;
        case 0x00FF: // _874C
            return;
            break;
        }
        if (MCU_memory[0x1935])
        {
            _875A:
            return;
        }
        _875D:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory + 0x1936));
    }
}

void _00214000()
{
    const UINT8 _8AA0[] = "Ver";
    MsgType _17CB;
    UINT8 _17CA;
    UINT8 _17C9; // 按键缓存
    UINT8 _17C6[3]; // 循环队列，缓存输入的字符
    UINT8 _17C5; // 循环队列下标
    if (MCU_memory[0x1935])
    {
        _5018:
        return;
    }
    _501B:
    _17C5 = 0x00; // 循环队列下标
    _17CA = 0x00;
    _17C9 = 0x00; // 清空按键缓存
    while (MCU_memory[0x1935] == 0x00)
    {
        _5035:
        if (GuiGetMsg(&_17CB) == 0x00)
        {
            _506A:
            continue;
        }
        _506D:
        if (GuiTranslateMsg(&_17CB) == 0x00)
        {
            _50A2:
            continue;
        }
        _50A5:
        if (_17CB.type == DICT_WM_COMMAND)
        {
            _50B1:
            if (_17CB.param == CMD_RETURN_HOME)
            {
                _50CC:
                MCU_memory[0x1935] = 0xFE;
            }
        }
        _50D1:
        // 播放预定音乐
        __8DEB();
        if (_17CB.type == DICT_WM_CHAR_ASC)
        {
            _511F:
            if (_17CB.param == ' ')
            {
            _513A:
                // 切换自动连续行走标志
                MCU_memory[0x1A99] = !MCU_memory[0x1A99];
            }
            _5143:
            _17C9 = 0x00; // 清空按键缓存
            _17C6[_17C5] = (UINT8)_17CB.param;
            _17C5++; // 循环队列下标
            if (_17C5 == 0x03) // 循环队列下标
            {
                _51AF:
                if (_SysMemcmp(_17C6, _8AA0, 0x0003) == 0x00)
                { // 如果输入的字符串为Ver，则显示引擎版本信息
                    _51F2:
                    _00216CA7();
                }
                _51F5:
                _17C5 = 0x00; // 循环队列下标
            }
        }
        else if (_17CB.type == DICT_WM_CHAR_FUN)
        {
        _51FE:
            // 如果按下的按键和按键缓存相同
            if (_17CB.param == _17C9)
            {
                _5223:
                _17C9 = 0x00; // 清空按键缓存
            }
            else
            {
                _522C:
                _17C9 = (UINT8)_17CB.param; // 按键缓存
            }
            goto _523D;
        }
        else if (_17CB.type == DICT_WM_TIMER)
        {
            _523D:
            _17CA = _00214347(&_17C9);
            MCU_memory[0x1A93] = MCU_memory[0x1A93]|0x40|0x20;
            _00215865();
            //  定时触发事件的定时时间
            if (*(UINT16*)(MCU_memory+0x1A9D) != 0x0000)
            {
            _528D:
                // 定时触发事件的定时时间
                *(UINT16*)(MCU_memory+0x1A9D) -= 0x0001;
                //  定时触发事件的定时时间
                if (*(UINT16*)(MCU_memory+0x1A9D) == 0x0000)
                {
                _52C8:
                    // 定时触发事件的定时时间
                    *(UINT16*)(MCU_memory+0x1A9D) = 0x0000;
                    //      定时触发事件
                    __8DB4(MCU_memory[0x1A9C]);
                }
            }
        }
        else
        {
            _52E1:
            continue;
        }
        _52E4:
        _00214B98();
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        if (_17CA)
        {
            _5318:
            __8DB4(_17CA);
            _17C9 = 0x00; // 清空按键缓存
            _17CA = 0x00;
        }
    }
}

// *_17D0按键缓存
UINT8 _00214347(UINT8* _17D0)
{
    UINT8 _17CB = 0x00; // 是否按下方向按键
    UINT8 _17CA = 0x01; // 默认需要更新地图
    UINT8 _17C9 = 0x00; // 事件号
    UINT8 _17C8;
    switch (*_17D0)
    {
    case CHAR_LEFT: // _5395
        // 角色面相设置为左
        MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x0002] = 0x04;
        if (MCU_memory[0x197C]) // 屏幕左上角定位到地图的x处
        {
        _53E5:
            //              屏幕左上角定位到地图的x处   屏幕左上角定位到地图的y处
            _17C9 = _00215648(MCU_memory[0x197C] + 0x03, MCU_memory[0x197D]+0x03);
            //                                                 屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            if (MCU_memory[0x1A59 +0x0006] > 0x80 && _0021554E(MCU_memory[0x197C]+0x03, MCU_memory[0x197D]+0x03) == 0x00)
            {
                _5485:
                MCU_memory[0x197C]--; // 屏幕左上角定位到地图的x处
                MCU_memory[0x1A9F] = 0x01; // 玩家行走方向左
            }
            else
            {
                _54BC:
                _17CA = 0x00; // 已走到地图最左边，不需要更新地图
            }
        }
        _54C2:
        _17CB = 0x01; // 按下方向按键
        break;
    case CHAR_RIGHT: // _54CB
        // 角色面相设置为右
        MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x0002] = 0x02;
        //  屏幕左上角定位到地图的x处 < 地图宽-9
        if (MCU_memory[0x197C] < MCU_memory[0x197E]-0x09)
        {
        _552E:
            //                屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            _17C9 = _00215648(MCU_memory[0x197C] + 0x05, MCU_memory[0x197D]+0x03);
            //                                                 屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            if (MCU_memory[0x1A59 +0x000A] > 0x80 && _0021554E(MCU_memory[0x197C]+0x05, MCU_memory[0x197D]+0x03) == 0x00)
            {
                _55CE:
                MCU_memory[0x197C]++; // 屏幕左上角定位到地图的x处
                MCU_memory[0x1A9F] = 0x02; // 玩家行走方向右
            }
            else
            {
                _5605:
                _17CA = 0x00; // 已走到地图最右边，不需要更新地图
            }
        }
        _560B:
        _17CB = 0x01; // 按下方向按键
        break;
    case CHAR_DOWN: // _5614
        // 角色面相设置为下
        MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x0002] = 0x03;
        // 屏幕左上角定位到地图的y处 < 地图高-6
        if (MCU_memory[0x197D] < MCU_memory[0x197F] - 0x06)
        {
        _5677:
            //                屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            _17C9 = _00215648(MCU_memory[0x197C] + 0x04, MCU_memory[0x197D]+0x04);
            //                                                 屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            if (MCU_memory[0x1A6B +0x0008] > 0x80 && _0021554E(MCU_memory[0x197C]+0x04, MCU_memory[0x197D]+0x04) == 0x00)
            {
                _5717:
                MCU_memory[0x197D]++; // 屏幕左上角定位到地图的y处
                MCU_memory[0x1A9F] = 0x04; // 玩家行走方向下
            }
            else
            {
                _574E:
                _17CA = 0x00; // 已走到地图最下边，不需要更新地图
            }
        }
        _5754:
        _17CB = 0x01; // 按下方向按键
        break;
    case CHAR_UP: // _575D
        // 角色面相设置为上
        MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x0002] = 0x01;
        if (MCU_memory[0x197D]) // 屏幕左上角定位到地图的y处
        {
        _57AD:
            //                屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            _17C9 = _00215648(MCU_memory[0x197C] + 0x04, MCU_memory[0x197D]+0x02);
            //                                                 屏幕左上角定位到地图的x处 屏幕左上角定位到地图的y处
            if (MCU_memory[0x1A47 +0x0008] > 0x80 && _0021554E(MCU_memory[0x197C]+0x04, MCU_memory[0x197D]+0x02) == 0x00)
            {
                _584D:
                MCU_memory[0x197D]--; // 屏幕左上角定位到地图的y处
                MCU_memory[0x1A9F] = 0x03; // 玩家行走方向上
            }
            else
            {
                _5884:
                _17CA = 0x00; // 已走到地图最上边，不需要更新地图
            }
        }
        _588A:
        _17CB = 0x01; // 按下方向按键
        break;
    case CHAR_ENTER: // _5893
        _17C9 = _00215346(); // 色对面需要处理的事件
        *_17D0 = 0x00; // 角色下一步不行动
        return _17C9;
        break;
    case CHAR_EXIT: // _58BA
        *_17D0 = 0x00; // 角色下一步不行动
        MCU_memory[0x1A9A] = 0x00;
        __8EBE();
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        if (MCU_memory[0x1A9A])
        {
            _5913:
            __8F40(0xFF, MCU_memory[0x1A9A]);
        }
        _593E:
        return 0x00;
        break;
    case CHAR_SEARCH: // _5943 todo
        _17C8 = 0x06; // 下面跳到剧情0-6
        break;
    case CHAR_INSERT: // _594C todo
        _17C8 = 0x07; // 下面跳到剧情0-7
        break;
    case CHAR_MODIFY: // _5955 todo
        _17C8 = 0x08; // 下面跳到剧情0-8
        break;
    case CHAR_DEL: // _595E todo
        _17C8 = 0x09; // 下面跳到剧情0-9
        break;
    default: // _5967
        *_17D0 = 0x00; // 角色下一步不行动
        return 0x00;
        break;
    }
_5985:
    // 如果按下了方向按键
    if (_17CB)
    {
        _598E:
        if (_17CA)
        { // 需要更新地图
            _5997:
            // 加载要显示的地图块数据
            __8DA5();
            MCU_memory[0x1A93] |= 0x80;
            _002167B3();
        }
        _59AD:
        // 角色脚步
        MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x0003]++;
        if (MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x03] > 0x03)
        {
            _5A62:
            MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x0003] = 0x00;
        }
        _5AAA:
        //      角色x坐标                                               角色y坐标
        __8D9F(MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x05], MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x06]);
        if (MCU_memory[0x1A99] == 0x00)
        { // 自动连续行走是关闭的
            _5B3A:
            *_17D0 = 0x00; // 角色下一步不行动
        }
    }
    else
    {
    _5B56:
        // 跳到剧情0-_17C8
        __8F43(_17C8);
        *_17D0 = 0x00; // 角色下一步不行动
    }
    _5B7E:
    return _17C9;
}

void _00214B98()
{
    if (MCU_memory[0x1A93]&0x80)
    {
        _5BA2:
        __8DA2();
    }
    _5BAD:
    if (MCU_memory[0x1A93]&0x40)
    {
        _5BB7:
        __8D96();
    }
    _5BC2:
    if (MCU_memory[0x1A93]&0x20)
    {
        _5BCC:
        __8D99();
    }
    _5BD7:
    if (MCU_memory[0x197B])
    { // 打开显示主角在当前地图的坐标功能
        _5BDF:
        _00216E26();
    }
    _5BE2:
    MCU_memory[0x1A93] = 0x00;
}

// DAT_ACTORPIC
void _00214BE8()
{
    UINT16 _17CC;
    UINT16 _17CA;
    UINT16 _17C8;
    UINT8 width;
    UINT8 height;
    UINT8 x1;
    UINT8 y1;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT8 _17BF;
    if (MCU_memory[0x1935])
    {
        _5C00:
        return;
    }
    _5C03:
    // 角色资源ID
    if (MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x01] == 0x00)
    {
        _5C39:
        return;
    }
    _5C3C:
    // 角色图片数据的bank偏移
    _17CC = (MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x0A] << 0x02) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17CC);
    UINT8* _17C6 = MCU_memory + 0x9000;
    if (_17C6[0] != 'A' || _17C6[1] != 'C' || _17C6[2] != 'P')
    {
        _5D67:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
    _5D6F:
    // 角色图片数据射到0x9000后，相对于0x9000的偏移量
    _17C6 += *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x0B);
    width = _17C6[2]; // 宽
    height = _17C6[3]; // 高
    _17C1 = _17C6[5];
    _17C6 += 0x0006;
    if (width &0x07)
    {
        _5E81:
        _17C8 = (width >>0x03)+0x01;
    }
    else
    {
        _5EA3:
        _17C8 = (width >>0x03);
    }
    _5EBF:
    _17C8 *= height; // 一张角色图所占字节数
    //              角色面相
    _17CA = _17C8*(MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x02]-0x01);
    _17CA *= 0x0003;
    // 角色脚步
    _17C0 = MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x03];
    if (_17C0 == 0x03)
    {
        _5F97:
        _17C0 =0x01;
    }
    _5F9D:
    _17CA += _17C8*_17C0;
    _17CA *= _17C1;
    _17C6 += _17CA;
    //               当前TILE数据单元的高
    _17BF = height - MCU_memory[0x1984];
    //      角色x坐标                                              当前TILE数据单元的宽
    x1 = MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x05] * MCU_memory[0x1983]+0x08;
    //      角色y坐标                                              当前TILE数据单元的高
    y1 = MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x06] * MCU_memory[0x1984]-_17BF;
    if (_17C1 == 0x02)
    {
        _60CF:
        __8DA8(x1, y1, width, height, _17C6);
    }
    else
    {
        _6114:
        SysPictureDummy(x1, y1, x1 + width - 0x01, y1 + height - 0x01, _17C6, MCU_memory + *(UINT16*)(MCU_memory + 0x1936), 0x00);
    }
}

void _00215199()
{
    for (UINT8 _17CF=0x00; _17CF<0x28; _17CF++)
    {
        _61CA:
        if (*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3) == 0x0000)
        {
            _6201:
            continue;
        }
        _6204:
        __8EF5(_17CF);
    }
}

void _00215227(UINT8 _17CF)
{
    //              x坐标                                                             屏幕左上角定位到地图的x处
    UINT8 _17CD = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x05] - MCU_memory[0x197C];
    //              y坐标                                                             屏幕左上角定位到地图的y处
    UINT8 _17CC = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x06] - MCU_memory[0x197D];
    __8D9F(_17CD, _17CC);
    if (_17CD == 0x04)
    {
        _62EA:
        if (_17CC == 0x02 || _17CC == 0x04)
        {
            _6302:
            __8D96();
        }
    }
    _630D:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// 返回角色对面需要处理的事件
UINT8 _00215346()
{
    UINT8 _17CE;
    //            屏幕左上角定位到地图的x处    角色x坐标
    UINT8 _17CD = MCU_memory[0x197C] + MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x05];
    //            屏幕左上角定位到地图的y处    角色y坐标
    UINT8 _17CC = MCU_memory[0x197D] + MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x06];
    // 角色面相
    switch (MCU_memory[MCU_memory[0x1A94]*0x0019+0x1988+0x02])
    {
    case 0x0004: // _6426 左
        if (_17CD+0x01 != 0x01)
        {
            _6435:
            _17CD--;
        }
        else
        {
            _6441:
            return 0x00;
        }
        _6446:
        break;
    case 0x0002: // _6449 右
        //                  当前TILE数据单元的宽
        if (_17CD + 0x01 != MCU_memory[0x1983])
        {
            _6465:
            _17CD++;
        }
        else
        {
            _6471:
            return 0x00;
        }
        break;
    case 0x0001: // _6479 上
        if (_17CC+0x01 != 0x01)
        {
            _6488:
            _17CC--;
        }
        else
        {
            _6494:
            return 0x00;
        }
        break;
    case 0x0003: // _649C 下
        //                  当前TILE数据单元的高
        if (_17CC + 0x01 != MCU_memory[0x1984])
        {
            _64B8:
            _17CC++;
        }
        else
        {
            _64C4:
            return 0x00;
        }
        break;
    }
_64CC:
    // 返回地图指定坐标上的事件号
    _17CE = _00215648(_17CD, _17CC);
    // 如果无事件，或返回值大于80
    if (_17CE+0x01 == 0x01 || _17CE > 0x50)
    {
        _6510:
        _17CE = _0021554E(_17CD, _17CC);
    }
    _6534:
    return _17CE;
}

// 返回给定坐标处的NPC（下标）
UINT8 _0021554E(UINT8 _17CF, UINT8 _17D0)
{
    for (UINT8 _17CE=0x00; _17CE<0x28; _17CE++)
    {
        _6582:
        if (*(UINT16*)(MCU_memory+(_17CE<<1)+0x19D3) != 0x0000)
        {
        _65B9:
            // x坐标
            if (MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x19D3)+0x05] == _17CF)
            {
            _65EE:
                // y坐标
                if (MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x19D3)+0x06] == _17D0)
                {
                    _6623:
                    return _17CE+0x01;
                }
            }
        }
    }
    return 0x00;
}

// 返回地图指定坐标上的事件号，如果存在，返回的事件号+40
UINT8 _00215648(UINT8 _17CF, UINT8 _17D0)
{
    // 当前地图数据的bank偏移
    UINT16 _17CB = MCU_memory[0x1980];
    _17CB <<= 0x0002;
    _17CB += *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17CB);
    UINT8* _17CD = MCU_memory + 0x9000;
    // 当前地图数据映射到0x9000后，相对于0x9000的偏移量
    _17CD += *(UINT16*)(MCU_memory+0x1981);
    _17CD += 0x0012;
    UINT8* _17C1 = _17CD;
    UINT16 _17C9 = _17CF;
    UINT16 _17C7 = _17D0;
    UINT16 _17C5 = MCU_memory[0x197E];// 地图宽
    UINT16 _17C3 = _17C7*_17C5+_17C9;
    if (_17C1[_17C3*0x0002+0x01]+0x01 != 0x01)
    {
        _6811:
        return _17C1[_17C3*0x0002+0x01]+0x28;
    }
    else
    {
        _684D:
        return 0x00;
    }
}

// 处理地图上NPC的动作
void _00215865()
{
    UINT16 _17CE;
    UINT8* _17CC;
    UINT8* _17CA;
    if (MCU_memory[0x1935])
    {
        _687D:
        return;
    }
_6880:
    // 当前地图数据的bank偏移
    _17CE = MCU_memory[0x1980];
    _17CE <<= 0x0002;
    _17CE += *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17CE);
    _17CC = MCU_memory + 0x9000;
    // 当前地图数据映射到0x9000后，相对于0x9000的偏移量
    _17CC += *(UINT16*)(MCU_memory+0x1981);
    _17CC += 0x0012;
    _17CA = _17CC;
    for (UINT8 _17C9=0x00; _17C9<0x28; _17C9++)
    {
        _69A4:
        if (*(UINT16*)(MCU_memory+(_17C9<<1)+0x19D3) == 0x0000)
        {
            _69DB:
            continue;
        }
    _69DE:
        // 动作
        switch (MCU_memory[*(UINT16*)(MCU_memory+(_17C9<<1)+0x19D3)+0x04])
        {
        case 0x0003: // _6A26
            _00215A79(_17C9);
            break;
        case 0x0001: // _6A30
        case 0x0002: // _6A30
            _00215EDE(_17C9, _17CA);
            break;
        case 0x0004: // _6A58
            _002165FB(_17C9);
            break;
        case 0x0000: // _6A62
        default:
            break;
        }
    }
}

void _00215A79(UINT8 _17CF)
{
    UINT16 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    // 图片
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0016] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x16] - 0x01;
    if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x16] == 0x00)
    {
        _6B32:
        _17CD = SysRand((PtrRandEnv)(MCU_memory + 0x1928)) % 0x00C8;
        _17CC = _17CD%0x04+0x01;
        // 面相
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0002] = _17CC;
        _17CD %= 0x05;
        if (_17CC == 0x04)
        {
        _6BDA:
            //                                                              x坐标
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0007] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x05] - _17CD;
        }
        else if (_17CC == 0x02)
        {
            _6C61:
            //                                                              x坐标
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0007] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x05] + _17CD;
        }
        else if (_17CC == 0x03)
        {
            _6CE8:
            //                                                              y坐标
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0008] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x06] + _17CD;
        }
        else if (_17CC == 0x01)
        {
            _6D6F:
            //                                                              y坐标
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0008] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x06] - _17CD;
        }
        _6DE7:
        //      x坐标                                                              屏幕左上角定位到地图的x处
        _17CB = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x05] -MCU_memory[0x197C];
        //      y坐标                                                              屏幕左上角定位到地图的y处
        _17CA = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x06] -MCU_memory[0x197D];
        __8D9F(_17CB, _17CA);
        // 动作
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0004] = 0x02;
    }
}

void _00215EDE(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    //  x坐标
    if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x05] == MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x07]
        // y坐标
        && MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x06] == MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x08]
        && MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x07] <= 0xE0
        && MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x08] <= 0xE0)
    {
    _7034:
        // y坐标
        _17CC = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x06];
        _17CC *= MCU_memory[0x197E]; // 地图宽
        // x坐标
        _17CC += MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x05];
        // x坐标
        _17C9 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x05];
        // y坐标
        _17C8 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x06];
        // 面相
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x02] == 0x04)
        { // 左
            _7177:
            _17CC--;
            _17C9--;
        }
        else if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x02] == 0x02)
        { // 右
            _71DB:
            _17CC++;
            _17C9++;
        }
        else if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x02] == 0x03)
        { // 下
            _723F:
            _17CC += MCU_memory[0x197E]; // 地图宽
            _17C8++;
        }
        else
        { // 上
            _7279:
            _17CC -= MCU_memory[0x197E]; // 地图宽
            _17C8--;
        }
        _72B0:
        if (_17D0[_17CC*0x0002] >= 0x80)
        {
            _72F1:
            //      屏幕左上角定位到地图的x处  角色x坐标
            _17CB = MCU_memory[0x197C]+ MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x05];
            //      屏幕左上角定位到地图的y处  角色y坐标
            _17CA = MCU_memory[0x197D]+ MCU_memory[MCU_memory[0x1A94] * 0x0019 + 0x1988 + 0x06];
            if (_17C9 != _17CB || _17C8 != _17CA)
            {
                _7392:
                if (_0021554E(_17C9, _17C8) == 0x00)
                {
                    _73BD:
                    _002165FB(_17CF);
                    // x坐标
                    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0005] = _17C9;
                    // y坐标
                    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0006] = _17C8;
                    return;
                }
            }
        }
    }
_744F:
    //                                                              x坐标
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0007] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x05];
    //                                                              y坐标
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0008] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x06];
    // 动作
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0004] = 0x03;
    // 角色图片 = 角色延时
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0016] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x15];
}

void _002165FB(UINT8 _17CF)
{
    //              x坐标                                                             屏幕左上角定位到地图的x处
    UINT8 _17CD = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x05] - MCU_memory[0x197C];
    //              y坐标                                                             屏幕左上角定位到地图的y处
    UINT8 _17CC = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x06] - MCU_memory[0x197D];
    __8D9F(_17CD, _17CC);
    // 脚步
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0003] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x03] + 0x01;
    if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x03] > 0x03)
    {
        _7760:
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0003] = 0x00;
    }
}

void _002167B3()
{
    if (MCU_memory[0x1935])
    {
        _77CB:
        return;
    }
    _77CE:
    if (MCU_memory[0x1A96] == 0x00)
    { // 不遇怪
        _77D6:
        return;
    }
    _77D9:
    if (SysRand((PtrRandEnv)(MCU_memory + 0x1928)) % 0x0064 > 0x0003)
    { // 不遇怪机率97%
        _7835:
        return;
    }
    _7838:
    if (__8E00(0x01) == 0x02)
    {
        _784D:
        __8E09();
        MCU_memory[0x1935] = 0xFD;
    }
}

void _0021686E(UINT8 _17CF, UINT8 _17D0)
{
    UINT16 _17CB;
    if (MCU_memory[0x1935])
    {
        _7889:
        return;
    }
    _788C:
    if (_17CF >= 0x09 || _17D0 > 0x06)
    {
        _78A9:
        return;
    }
    _78AC:
    _GetDataBankNumber(0x09, &_17CB);
    // 当前TILE数据的bank偏移
    UINT16 _17CD = MCU_memory[0x1985];
    _17CD <<= 0x0002;
    _17CD += *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17CD);
    if (_17D0)
    {
        _795C:
        _002169E0(_17CF, _17D0-0x01);
    }
    _797D:
    if (_17D0 != 0x06)
    {
        _7989:
        _002169E0(_17CF, _17D0);
    }
    _79A7:
    _DataBankSwitch(0x09, 0x04, _17CB);
}

void _002169E0(UINT8 _17CF, UINT8 _17D0)
{
    //            当前TILE数据单元的宽
    UINT8 _17C6 = MCU_memory[0x1983]>>0x03;
    //       当前TILE数据单元的高
    _17C6 *= MCU_memory[0x1984];
    //            当前TILE数据单元的宽
    UINT8 _17C3 = MCU_memory[0x1983]-0x01;
    //            当前TILE数据单元的高
    UINT8 _17C2 = MCU_memory[0x1984]-0x01;
    UINT8* _17C9 = MCU_memory + 0x9000;
    // 当前TILE数据映射到0x9000后，相对于0x9000的偏移量
    _17C9 += *(UINT16*)(MCU_memory + 0x1986);
    _17C9 += 0x0004;
    UINT8 _17C7 = *_17C9;
    _17C9++;
    UINT8 _17C8 = *_17C9;
    _17C9++;
    UINT8 x1 = (_17C3+0x01)*_17CF+0x08;
    //               当前TILE数据单元的高
    UINT8 y1 = _17D0*MCU_memory[0x1984]+0x00;
    UINT16 _17C0 = MCU_memory[_17CF*0x0002+ _17D0 * 0x0009 * 0x0002 + 0x1A23];
    if (_17C0 > 0x0080)
    {
        _7BB4:
        _17C0 -= 0x0080;
    }
    _7BD7:
    _17C0 *= _17C6;
    SysPictureDummy(x1, y1, x1 + _17C3, y1 + _17C2, _17C0 + _17C9, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
}

void _00216CA7()
{
    //" 引擎代号：110       引擎版本：Ver 1.1   引擎设计：            高国军  陈泽伟        步步高游戏组           2005年1月   "
    // 上面字符串内容按位取反得到_8AF0里面的内容
    UINT8 _8AF0[] = {0xDF,0x2D,0x02,0x38,0x19,0x4B,0x05,0x45,0x3A,0x5C,0x45,0xCE,0xCE,0xCF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0x2D,0x02,0x38,0x19,0x4F,0x19,0x4E,0x41,0x5C,0x45,0xA9,0x9A,0x8D,0xDF,0xCE,0xD1,0xCE,0xDF,0xDF,0xDF,0x2D,0x02,0x38,0x19,0x36,0x17,0x43,0x39,0x5C,0x45,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0x47,0x20,0x46,0x05,0x41,0x03,0xDF,0xDF,0x4C,0x3D,0x2B,0x0C,0x31,0x4F,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0x4D,0x42,0x4D,0x42,0x47,0x20,0x2C,0x31,0x30,0x48,0x28,0x16,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xDF,0xCD,0xCF,0xCF,0xCA,0x3B,0x15,0xCE,0x2B,0x3D,0xDF,0xDF,0xDF,0x00};
    UINT8 _17CE;
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    for (_17CE=0x00; _8AF0[_17CE]!=0x00; _17CE++)
    {
        _7D22:
        MCU_memory[_17CE+0x1CAF] = _8AF0[_17CE] ^ 0xFF;
    }
    _7D6D:
    MCU_memory[_17CE+0x1CAF] = 0x00;
    SysPrintString(0x00, 0x00, MCU_memory+0x1CAF);
    SysRect(0x00, 0x00, 0x9E, 0x5F);
    __8DD3(0x14, 0x00);
}

void _00216E26()
{
    __8F07(DAT_SUNDRYPIC, 0x02, 0x11, 0x00, 0x0A, 0x03);
    //                  屏幕左上角定位到地图的x处
    __8F01(0x0F, 0x08, (UINT32)MCU_memory[0x197C] + 0x04, 0x01);
    //                  屏幕左上角定位到地图的y处
    __8F01(0x22, 0x08, (UINT32)MCU_memory[0x197D] + 0x03, 0x01);
}

void _00216EF3()
{
    UINT8* _17CA;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT8 _17BF;
    UINT8 _17BE;
    UINT8 x1;
    UINT8 y1;
    UINT16 _17BA;
    UINT16 _17B8;
    if (MCU_memory[0x1935])
    {
        _7F0B:
        return;
    }
_7F0E:
    //       当前TILE数据单元的宽
    _17C2 = (MCU_memory[0x1983] >> 0x03) * 0x10;
    // 当前TILE数据的bank偏移
    _17BA = (MCU_memory[0x1985] << 0x0002) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17BA);
    // 当前TILE数据映射到0x9000后，相对于0x9000的偏移量
    _17CA = MCU_memory + 0x9000 + *(UINT16*)(MCU_memory+0x1986) + 0x0004;
    _17C3 = *_17CA; // 当前TILE块中数据单元个数
    _17CA++;
    _17C4 = *_17CA;
    _17CA++;
    _DataBankSwitch(0x04, 0x01, 0x0005);
    x1 = 0x08;
    y1 = 0x00;
    if (MCU_memory[0x1A9F])
    {
        _8292:
        _17C6 = 0x00;
        _17C5 = 0x00;
        _17C1 = 0x00;
        _17C0 = 0x00;
        _17BF = 0x00;
        _17BE = 0x00;
        switch (MCU_memory[0x1A9F])
        {
        case 0x0001: // _82D6
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x1936) + 0x0002, MCU_memory + 0x4000, 0x077E);
            _17C8 = 0x06;
            _17C5 = 0x10;
            _17BE = 0x01;
            break;
        case 0x0002: // _834C
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory + 0x4002, 0x077E);
            _17C8 = 0x06;
            _17C5 = 0x10;
            x1 = 0x88;
            _17C1 = 0x08;
            _17BE = 0x01;
            break;
        case 0x0003: // _83A3
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x1936) + 0x0140, MCU_memory + 0x4000, 0x0640);
            _17C8 = 0x09;
            _17C6 = 0x10;
            _17BF = 0x01;
            break;
        case 0x0004: // _8419
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory + 0x4140, 0x0640);
            _17C8 = 0x09;
            _17C6 = 0x10;
            _17C0 = 0x05;
            y1 = 0x50;
            _17BF = 0x01;
            break;
        }
    _8470:
        // 屏幕左右添加多条竖线
        for (UINT16 _17B6=0x0000; _17B6<0x0780;_17B6+=0x0014)
        {
            _84BD:
            MCU_memory[_17B6+*(UINT16*)(MCU_memory+0x1936)] = 0xAA;
            MCU_memory[_17B6+0x0013+*(UINT16*)(MCU_memory+0x1936)] = 0xAA;
        }
        _851D:
        for (UINT8 _17C9=0x00; _17C9<_17C8; _17C9++)
        {
            _8540:
            _17C7 = MCU_memory[_17C1*0x0002+ _17C0 * 0x0009 * 0x0002 + 0x1A23] & 0x7F;
            _17B8 = _17C7 * _17C2;
            SysPictureDummy(x1, y1, x1 + 0x10 - 0x01, y1 + 0x10 - 0x01, _17CA + _17B8, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
            x1 += _17C6;
            y1 += _17C5;
            _17C1 += _17BF;
            _17C0 += _17BE;
        }
    }
    else
    {
        _80B5:
        for (UINT8 _17C9=0x00; _17C9<0x06; _17C9++)
        {
            _80D6:
            x1 = 0x08;
            y1 = _17C9*0x10+0x00;
            for (UINT8 _17C8=0x00; _17C8<0x09; _17C8++)
            {
                _810F:
                _17C7 = MCU_memory[_17C8*0x0002+ _17C9 * 0x0009 * 0x0002 + 0x1A23];
                if (_17C7 > 0x80)
                {
                    _819D:
                    _17C7 -= 0x80;
                }
                LOG("%d ", _17C7);
                _81A8:
                _17B8 = _17C7 * _17C2;
                SysPictureDummy(x1, y1, x1 + 0x10 - 0x01, y1 + 0x10 - 0x01, _17CA + _17B8, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
                x1 += 0x10;
            }
            LOG("\n");
        }
    }
    _86D7:
    MCU_memory[0x1A9F] = 0x00;
    _SysMemcpy(MCU_memory + 0x4000, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780);
    _DataBankSwitch(0x04, 0x01, 0x0004);
}

// 加载要显示的地图块数据
void _00217748()
{
    UINT8* _17CD;
    UINT8 _17CA;
    UINT16 _17C8;
    UINT16 _17C6;
    UINT8* _17C4;
    if (MCU_memory[0x1935])
    {
        _8760:
        return;
    }
_8763:
    //  地图宽-屏幕左上角定位到地图的x处 < 9 || 地图高-屏幕左上角定位到地图的y处 < 6
    if (MCU_memory[0x197E]-MCU_memory[0x197C] < 0x09 || MCU_memory[0x197F]-MCU_memory[0x197D] < 0x06)
    {
        _8799:
        MCU_memory[0x1935] = MAP_OVERFL;
        return;
    }
_87A1:
    //      当前地图数据的bank偏移
    _17C8 = (MCU_memory[0x1980] << 0x0002) +*(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17C8);
    // 指向当前显示的地图块数据缓存
    _17C4 = MCU_memory + 0x1A23;
    // 当前地图数据映射到0x9000后，相对于0x9000的偏移量
    _17CD = MCU_memory + 0x9000 + *(UINT16*)(MCU_memory+0x1981) + 0x0012;
    for (UINT8 _17CC=0x00; _17CC<0x06; _17CC++)
    {
    _88C2:
        //        屏幕左上角定位到地图的y处      地图宽              屏幕左上角定位到地图的x处
        _17C6 = ((MCU_memory[0x197D] + _17CC) * MCU_memory[0x197E] + MCU_memory[0x197C])*0x0002;
        _17CA = _17CC*0x09;
        for (UINT8 _17CB=0x00; _17CB<0x09; _17CB++)
        {
            _897A:
            _17C4[(_17CA+_17CB)*0x0002] = _17CD[_17C6];
            _17C4[(_17CA+_17CB)*0x0002+0x0001] = _17CD[_17C6 + 0x0001];
            _17C6 = _17C6+0x0002;
        }
    }
}

void _00218000(UINT8 x, UINT8 y, UINT8 width, UINT8 height, UINT8* _17D3)
{
    UINT8 _52F1[8] = { 0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE };
    UINT8 _52F9[8] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
    UINT8 A;
    UINT8 X;
    UINT8 Y;
    UINT16 _E9;
    UINT16 _EB;
    UINT8 _F0;
    UINT8 _F1;
    UINT8 _F2;
    UINT8 _F4;
    UINT8 _F5;
    UINT8 x1 = x;
    UINT8 y1 = y;
    UINT8 w = width;
    UINT8 h = height;
    UINT8* pic = _17D3;
    UINT8* screen_buffer = MCU_memory + *(UINT16*)(MCU_memory+0x1936);
    UINT8 _C8;
    UINT8 _F3 = 0x00;
    if (y1 &0x80)
    {
        // 如果y1为负数
        _5030:
        // _F3为y1的绝对值
        _F3 = (y1 ^0xFF)+0x01;
        if (_F3 >= h)
        {
            _503D:
            return;
        }
        _503E:
        _E9 = w;
        if (_E9&0x07)
        {
            _5046:
            _E9 += 0x08;
        }
        _504D:
        _E9 >>= 3;
        _E9 <<= 1;
        _EB = _F3;
        X = 0x00;
        do {
            _505F:
            if (_EB&0x01)
            {
                _5065:
                pic += _E9;
            }
            _5072:
            _E9 <<= 1;
            _EB >>= 1;
            X++;
        } while (X < 0x07);
        y1 = 0x00;
    }
    _5081:
    _C8 = x1 &0x07;

    // _E9 = y1*(160/8)+(x1 >>3)
    _E9 = y1;
    _EB = y1;
    _E9 <<= 4;
    _EB <<= 2;
    _E9 += x1 >>3;
    _E9 += _EB;

    screen_buffer += _E9;
    Y = 0x00;
    _F0 = Y;
    _F4 = Y;
    _F1 = screen_buffer[Y];
    _F2 = Y;
    X = _C8;
    for (;;) {
    _50E3:
        Y = _F0;
        _F5 = pic[Y];
        _F0 = Y;
        for (;;) {
        _50EB:
            if (_F5&0xC0)
            {
                _50F1:
                if ((_F5 & 0xC0) == 0x40)
                {// b01 黑
                    _50F5:
                    _F1 |= _52F9[X];
                }
            }
            else
            {// b00 白
                _50FF:
                _F1 &= _52F1[X];
            }
            _5106:
            X++;
            if (X == 0x08)
            {
                _510B:
                Y = _F2;
                screen_buffer[Y] = _F1;
                Y++;
                _F1 = screen_buffer[Y];
                _F2 = Y;
                X = 0x00;
            }
            _511A:
            _F4++;
            if (_F4 == 0x04)
            {
                break;
            }
            _5122:
            _F5 <<= 2;
        }
        _512B:
        _F4 = 0x00;
        _F0++;
        if ((_F0<<2) < w)
        {
            continue;
        }
        _513E:
        if (X)
        {
            _5142:
            Y = _F2;
            screen_buffer[Y] = _F1;
        }
        _5148:
        _F3++;
        if (h == _F3)
        {
            _514E:
            break;
        }
        _5150:
        y1++;
        if (0x5F < y1)
        {
            _5158:
            return;
        }
        _5159:
        screen_buffer += 0x0014;
        _F1 = screen_buffer[0];
        _F0 = w &0x07;
        A = w >>3;
        Y = _F0;
        if (Y)
        {
            _517B:
            A++;
        }
        _517E:
        pic += A<<1;
        _F0 = 0x00;
        _F2 = 0x00;
        X = _C8;
    }
}

void _00218196(UINT8 A, UINT8 _17D0, UINT8 _17D1, UINT8* _17D2, UINT8* _17D4)
{
    UINT8 _52F1[8] = { 0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE };
    UINT8 _52F9[8] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
    UINT8 X;
    UINT8 Y;
    UINT8 _F0;
    UINT8 _F1;
    UINT8 _F2;
    UINT8 _F3;
    UINT8 _F4;
    UINT8 _F5;
    UINT8 __C2 = A;
    UINT8 _C3 = _17D0;
    UINT8 _C8 = _17D1;
    UINT8 _C0 = _17D1;
    UINT8* _C4 = _17D2;
    UINT8* _C6 = _17D4;
    UINT8 _C1 = 0x00;
    Y = 0x00;
    _F0 = Y;
    _F3 = Y;
    _F4 = Y;
    _F1 = _C6[Y];
    _F2 = Y;
    X = _C8;
    _51CD:
    for (;;) {
        Y = _F0;
        _F5 = _C4[Y];
        _F0 = Y;
        _51D5:
        for (;;) {
            if ((_F5 & 0xC0) == 0x40)
            {
            _51DF:
                _F1 |= _52F9[X];
            }
            else if ((_F5 & 0xC0) == 0x00)
            {
            _51E9:
                _F1 &= _52F1[X];
            }
            _51F0:
            X++;
            if (X == 0x08)
            {
                _51F5:
                Y = _F2;
                _C6[Y] = _F1;
                Y++;
                _F1 = _C6[Y];
                _F2 = Y;
                X = 0x00;
            }
            _5204:
            _F4++;
            if (_F4 == 0x04)
            {
                break;
            }
            _520C:
            _F5 <<= 2;
        }
        _5215:
        _F4 = 0x00;
        _F0++;
        if ((_F0<<2) < __C2)
        {
            continue;
        }
        _5228:
        if (X)
        {
            _522C:
            _C6[_F2] = _F1;
        }
        _5232:
        _F3++;
        if (_C3 == _F3)
        {
            _528C:
            break;
        }
        _523A:
        _C1++;
        if (((__C2 + _C8) & 0x07) == 0x00)
        {
            _5245:
            A = __C2;
        }
        else
        {
            _524A:
            A = 0x08+__C2;
        }
        _524F:
        _C6 += A>>3;
        _F1 = _C6[0x00];
        _F0 = __C2&0x07;
        A = __C2 >> 3;
        Y = _F0;
        if (Y)
        {
            _5272:
            A++;
        }
        _5275:
        _C4 += A<<1;
        _F0 = 0x00;
        _F2 = 0x00;
        X = _C8;
    }
}

void _0021828D(UINT8* _17D0)
{
    SysPicture(0x00, 0x00, 0x9E, 0x5F, _17D0, 0x00);
}

void _00218301(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    UINT8* _17CD;
    if (MCU_memory[0x1935])
    {
        _531C:
        return;
    }
    _531F:
    _17CD = _0021A499(DAT_ACTORRES, _17D0, _17D1);
    if (_17D0 == 0x01)
    {
        // 玩家角色
        _5359:
        _002183F7(_17CF, _17CD);
    }
    else if (_17D0 == 0x02 || _17D0 == 0x04)
    {
        // NPC角色、场景对象
        _5399:
        _00218BBB(_17CF, _17CD);
    }
    else
    {
        // 敌人角色
        _53C1:
        _00218EA7(_17CF, _17CD);
    }
}

void _002183F7(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17C5[6];
    UINT8 _17C4;
    UINT8 _17C3;
    UINT16 _17C1;
    UINT16 _17BF;
    if (MCU_memory[0x1935])
    {
        _5412:
        return;
    }
    _5415:
    // 复制角色基础信息
    _SysMemcpy(MCU_memory + _17CF * 0x0019 + 0x1988, _17D0, 0x000A);
    // 复制角色姓名
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0F), _17D0 + 0x000A, 0x000C);
    // 角色图片编号
    _17C4 = _17D0[0x0016];
    // 角色魔法链
    _17C3 = _17D0[0x0017];
    MCU_memory[_17CF*0x0019+0x1988+0x000D] = _17C3;
    // 复制角色装备
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x13), _17D0 + 0x0018, 0x0008);
    // 复制角色属性
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), _17D0 + 0x0018+ 0x0008, 0x001A);
    _17C5[0x00] = DAT_ACTORPIC;
    _17C5[0x01] = 0x01;
    _17C5[0x02] = _17C4;
    MCU_memory[_17CF+0x1976] = _17C4; // 角色位对应的角色编号
    _0021A678(_17C5);
    if (MCU_memory[0x1935])
    {
        _5807:
        return;
    }
    _580A:
    // 角色图片数据的bank偏移
    MCU_memory[_17CF*0x0019+0x1988+0x000A] = _17C5[3];
    // 角色图片数据射到0x9000后，相对于0x9000的偏移量
    *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x000B) = *(UINT16*)(_17C5+4);
    if (_17C3 == 0x00)
    {
        _58D5:
        return;
    }
    _58D8:
    _17C5[0x00] = DAT_MAGICLINK;
    _17C5[0x01] = 0x01;
    _17C5[0x02] = _17C3;
    _0021A678(_17C5);
    if (MCU_memory[0x1935])
    {
        _5956:
        return;
    }
    _5959:
    // 角色学会                                 角色学会
    MCU_memory[_17CF*0x0019+0x1988+0x000E] = MCU_memory[_17CF * 0x0019 + 0x1988 + 0x09];
    _17C1 = MCU_memory[_17CF*0x0019+0x1988+0x09]*0x02;
    _17BF = (_17C5[3] << 0x0002) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17BF);
    if (_0021B0AE(DAT_MAGICLINK) != 0x00)
    {
        _5AB4:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
    _5ABC:
    _17D0 = MCU_memory + 0x9000 + *(UINT16*)(_17C5 + 4) + 0x0003;
    // 复制角色魔法链数据
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x15), _17D0, _17C1);
}

void _00218BBB(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17C7[6];
    if (MCU_memory[0x1935])
    {
        _5BD6:
        return;
    }
    _5BD9:
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3), _17D0, 0x0016);
    // 角色图片 = 角色延时
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0016] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3) + 0x15];
    _17D0 += 0x0016;
    _17C7[0x00] = DAT_ACTORPIC;
    // 角色类型
    _17C7[0x01] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x19D3)];
    // 角色图片编号
    _17C7[0x02] = *_17D0;
    MCU_memory[_17CF+0x194E] = *_17D0;
    _0021A678(_17C7);
    if (MCU_memory[0x1935])
    {
        _5DDF:
        return;
    }
_5DE2:
    // 角色图片资源数据的bank偏移
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0017] = _17C7[3];
    // 角色图片资源数据映射到0x9000后，相对于0x9000的偏移量
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+(_17CF<<1)+0x19D3)+0x0018) = *(UINT16*)(_17C7+4);
}

void _00218EA7(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17C7[6];
    UINT8 _17C6;
    UINT8 _17C5;
    UINT16 _17C3;
    UINT16 _17C1;
    if (MCU_memory[0x1935])
    {
        _5EC2:
        return;
    }
    _5EC5:
    // 复制敌人角色数据
    _SysMemcpy(MCU_memory + _17CF * 0x0033 + 0x1826, _17D0, 0x002E);
    _17D0 += 0x002E;
    // 敌人角色图片
    _17C6 = _17D0[0];
    // 敌人角色魔法链
    _17C5 = _17D0[0x0001];
    // 敌人角色图片ID
    MCU_memory[_17CF*0x0033+0x1826+0x0005] = _17C6;
    _17C7[0x00] = DAT_ACTORPIC;
    _17C7[0x01] = 0x03;
    _17C7[0x02] = _17C6;
    _0021A678(_17C7);
    if (MCU_memory[0x1935])
    {
        _6067:
        return;
    }
_606A:
    // 敌人角色图片数据的bank偏移
    MCU_memory[_17CF*0x0033+0x1826+0x002E] = _17C7[3];
    // 敌人角色图片数据映射到0x9000后，相对于0x9000的偏移量
    *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x002F) = *(UINT16*)(_17C7 + 4);
    if (_17C5 == 0x00)
    {
        _6135:
        return;
    }
    _6138:
    _17C7[0x00] = DAT_MAGICLINK;
    _17C7[0x01] = 0x01;
    _17C7[0x02] = _17C5;
    _0021A678(_17C7);
    if (MCU_memory[0x1935])
    {
        _61B6:
        return;
    }
_61B9:
    //      敌人角色学会
    _17C3 = MCU_memory[_17CF*0x0033+0x1826+0x02]*0x02;
    // 指向敌人角色魔法链数据
    *(UINT16*)(MCU_memory + _17CF * 0x0033 + 0x1826 + 0x0031) = (UINT16)(SysMemAllocate(_17C3 + 0x0004) - MCU_memory);
    if (*(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x31) == 0x0000)
    {
        _62E7:
        MCU_memory[0x1935] = MEM_ERR;
        return;
    }
    _62EF:
    _17C1 = (_17C7[3] << 0x0002) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17C1);
    if (_0021B0AE(DAT_MAGICLINK) != 0x00)
    {
        _637F:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
    _6387:
    _17D0 = MCU_memory + 0x9000 + *(UINT16*)(_17C7 + 4) + 0x0003;
    //         复制敌人角色魔法链数据
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0033 + 0x1826 + 0x0031), _17D0, _17C3);
}

// 装载地图
void _00219486(UINT8 _17CF, UINT8 _17D0)
{
    UINT8 _17C7[6];
    UINT16 _17C5;
    UINT8* _17C3;
    UINT8 _17C2;
    if (MCU_memory[0x1935])
    {
        _64A1:
        return;
    }
    _64A4:
    MCU_memory[0x1979] = _17CF; // 当前地图的类型
    MCU_memory[0x197A] = _17D0; // 当前地图的序号
    _17C7[0x00] = DAT_MAPDATA;
    _17C7[0x01] = _17CF;
    _17C7[0x02] = _17D0;
    _0021A678(_17C7);
    if (MCU_memory[0x1935])
    {
        _6532:
        return;
    }
    _6535:
    MCU_memory[0x1980] = _17C7[3]; // 当前地图数据的bank偏移
    *(UINT16*)(MCU_memory+0x1981) = *(UINT16*)(_17C7+4); // 当前地图数据映射到0x9000后，相对于0x9000的偏移量
    // 当前地图数据的bank偏移
    _17C5 = (MCU_memory[0x1980] << 0x0002) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17C5);
    _17C3 = MCU_memory + 0x9000;
    if (_17C3[0] != 'M' || _17C3[1] != 'A' || _17C3[2] != 'P')
    {
        _668E:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
    _6696:
    _17C3 += *(UINT16*)(MCU_memory+0x1981); // 当前地图数据映射到0x9000后，相对于0x9000的偏移量
    if (_17C3[0] != _17CF || _17C3[1] != _17D0)
    {
        _6713:
        MCU_memory[0x1935] = DATA_ERR;
        return;
    }
    _671B:
    _17C2 = _17C3[2];
    if (_17C3[3] != 0x00)
    {
    _676C:
        //          当前地图的名称
        _SysMemcpy(MCU_memory+0x1942, _17C3 + 0x0003, 0x000A);
    }
    _67CE:
    _17C3 += 0x0010;
    MCU_memory[0x197E] = _17C3[0]; // 地图宽
    MCU_memory[0x197F] = _17C3[1]; // 地图高
    _17C7[0x00] = DAT_TILEDATA;
    _17C7[0x01] = 0x01;
    _17C7[0x02] = _17C2;
    _0021A678(_17C7);
    if (MCU_memory[0x1935])
    {
        _68E3:
        return;
    }
    _68E6:
    MCU_memory[0x1985] = _17C7[3]; // 当前TILE数据的bank偏移
    *(UINT16*)(MCU_memory+0x1986) = *(UINT16*)(_17C7+4); // 当前TILE数据映射到0x9000后，相对于0x9000的偏移量
    // 当前TILE数据的bank偏移
    _17C5 = (MCU_memory[0x1985] << 0x0002) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17C5);
    _17C3 = MCU_memory + 0x9000;
    if (_17C3[0] != 'T' || _17C3[1] != 'I' || _17C3[2] != 'L')
    {
        _6A3F:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
_6A47:
    // 当前TILE数据映射到0x9000后，相对于0x9000的偏移量
    _17C3 += *(UINT16*)(MCU_memory+0x1986);
    MCU_memory[0x1983] = _17C3[2]; // 当前TILE数据单元的宽
    MCU_memory[0x1984] = _17C3[3]; // 当前TILE数据单元的高
}

// _17CF:x,_17D0:y,_17D1:数值,_17D5:0-小字体，1-正常字体
void _00219B0D(UINT8 x, UINT8 y, UINT32 _17D1, UINT8 _17D5)
{
    UINT8 _8269[10][7] = {
        { 0x60, 0x90, 0x90, 0x90, 0x90, 0x90, 0x60 },
        { 0x40, 0xC0, 0x40, 0x40, 0x40, 0x40, 0xE0 },
        { 0x60, 0x90, 0x10, 0x20, 0x40, 0x80, 0xF0 },
        { 0xE0, 0x10, 0x10, 0xE0, 0x10, 0x10, 0xE0 },
        { 0x20, 0x60, 0xA0, 0xA0, 0xA0, 0xF0, 0x20 },
        { 0xF0, 0x80, 0xE0, 0x10, 0x10, 0x90, 0x60 },
        { 0x60, 0x90, 0x80, 0xE0, 0x90, 0x90, 0x60 },
        { 0xF0, 0x10, 0x10, 0x20, 0x40, 0x40, 0x40 },
        { 0x60, 0x90, 0x90, 0x60, 0x90, 0x90, 0x60 },
        { 0x60, 0x90, 0x90, 0x70, 0x10, 0x90, 0x60 }
};
    UINT32 _82AF = 0x0000000A;
    UINT32 _82B3 = 0x00000000;
    UINT32 _82B7 = 0x0000000A;
    UINT32 _17C7;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17BA[10];
    UINT8 _17B0[10];
    if (MCU_memory[0x1935])
    {
        _6B28:
        return;
    }
    _6B2B:
    _17C4 = 0x00;
    for (;;) {
        _6B31:
        _17C7 = _17D1/_82AF;
        if (_17C7 > _82B3)
        {
            _6B83:
            _17BA[_17C4] = _17D1 - _17C7 * _82B7;
            _17C4++;
            _17D1 = _17C7;
            continue;
        }
        else
        {
            _6C3A:
            _17BA[_17C4] = (UINT8)_17D1;
            _17C4++;
            for (UINT8 _17C6=0x00; _17C6<_17C4; _17C6++)
            {
                _6CA6:
                _17C5 = _17BA[_17C4-(_17C6+0x01)];
                if (_17D5)
                {
                    _6CDA:
                    _0021A1B4(DAT_SUNDRYPIC, 0x02, 0x05, _17C5, x, y);
                }
                else
                {
                    _6D11:
                    _SysMemcpy(_17B0, _8269[_17C5], 0x0007);
                    SysPicture(x, y, x + 0x03, y + 0x06, _17B0, 0x00);
                }
                _6DD9:
                x += 0x05;
            }
            _6DE7:
            return;
        }
    }
}

void _00219DF8(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 x1, UINT8 y1)
{
    UINT8* _17CD;
    UINT8* _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 x2;
    UINT8 y2;
    UINT8 _17C4;
    if (MCU_memory[0x1935])
    {
        _6E13:
        return;
    }
    _6E16:
    _17CB = _0021A499(_17CF, _17D0, _17D1);
    if (MCU_memory[0x1935])
    {
        _6E4E:
        MCU_memory[0x1935] = 0x00;
        return;
    }
    _6E56:
    _17CA = _17CB[2]; // 宽
    _17C9 = _17CB[3]; // 高
    _17C7 = _17CB[4];
    _17C8 = _17CB[5];
    if (_17C7 > _17D2)
    {
        _6F09:
        x2 = x1 +_17CA-0x01;
        y2 = y1 +_17C9-0x01;
        UINT16 _17C2 = _17CA/0x08;
        if (_17CA%0x08)
        {
            _6F55:
            _17C2++;
        }
        _6F65:
        _17C2 *= _17C9;
        _17C2 *= _17C8;
        _17C2 *= _17D2;
        _17CB += 0x0006;
        _17CB += _17C2;
        if (_17C8 == 0x01)
        {
            _7033:
            SysPicture(x1, y1, x2, y2, _17CB, 0x00);
        }
        else
        {
            _707D:
            _17CD = MCU_memory+0x4800;
            SysSaveScreen(x1, y1, x2, y2, _17CD);
            _17C4 = x1 %0x08;
            __8DAB(_17CA, _17C9, _17C4, _17CB, _17CD);
            _17C4 = (x2 +0x01)%0x08;
            if (_17C4)
            {
                _713F:
                x2 = x2 -_17C4+0x08;
            }
            _714F:
            x1 -= _17C4;
            SysPicture(x1, y1, x2, y2, _17CD, 0x00);
        }
    }
}

// _17D3:x，_17D4:y
void _0021A1B4(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 x1, UINT8 y1)
{
    UINT8* _17CB;
    UINT8 width;
    UINT8 height;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT16 _17C5;
    if (MCU_memory[0x1935])
    {
        _71CF:
        return;
    }
    _71D2:
    _17CB = _0021A499(_17CF, _17D0, _17D1);
    if (MCU_memory[0x1935])
    {
        _720A:
        MCU_memory[0x1935] = 0x00;
        return;
    }
    _7212:
    width = _17CB[2]; // 宽
    height = _17CB[3]; // 高
    _17C7 = _17CB[4]; // 图片个数
    _17C8 = _17CB[5]; // 透明编码。1不透明，2透明。
    if (_17C7 > _17D2)
    {
        _72C5:
        _17C5 = width /0x08;
        if (width %0x08)
        {
            _72F1:
            _17C5++;
        }
        _7301:
        _17C5 *= height;
        _17C5 *= _17C8;
        _17C5 *= _17D2;
        _17CB += 0x0006;
        _17CB += _17C5;
        if (_17C8 == 0x02)
        { // 含透明信息的图片
            _73CF:
            __8DA8(x1, y1, width, height, _17CB);
        }
        else
        { // 不含透明信息的图片
            _7414:
            SysPictureDummy(x1, y1, x1 + width - 0x01, y1 + height - 0x01, _17CB, MCU_memory +*(UINT16*)(MCU_memory+0x1936), 0x00);
        }
    }
}

// 返回指向所需数据的指针
UINT8* _0021A499(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    UINT8 _17C7[6];
    UINT16 _17C5;
    UINT8* _17C3;
    UINT8* _17CD = NULL;
    if (MCU_memory[0x1935])
    {
        _74BD:
        return _17CD;
    }
    _74CB:
    _17C7[0x00] = _17CF;
    _17C7[0x01] = _17D0;
    _17C7[0x02] = _17D1;
    _0021A678(_17C7);
    if (MCU_memory[0x1935])
    {
        _754D:
        return _17CD;
    }
    _755B:
    _17C5 = (_17C7[3] << 0x0002) + *(UINT16*)(MCU_memory + 0x1931);
    _DataBankSwitch(0x09, 0x04, _17C5);
    if (_0021B0AE(_17CF) != 0x00)
    {
        _75ED:
        MCU_memory[0x1935] = DLOSE_ERR;
        return _17CD;
    }
    _7600:
    _17C3 = MCU_memory + 0x9000 + *(UINT16*)(_17C7 + 4);
    _17CD = _17C3;
    return _17CD;
}

// 找到给定标记的实际偏移量_17D0[3]:偏移多少个0x4000,*(UINT16*)(_17D0+0x0004):偏移的字节
void _0021A678(UINT8* _17D0)
{
    UINT8* _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT16 _17C8;
    UINT16 _17C6;
    UINT16 _17C4;
    UINT16 _17C2;
    if (MCU_memory[0x1935])
    {
        _7690:
        return;
    }
    _7693:
    _17CA = 0x00;
    _DataBankSwitch(0x09, 0x04, *(UINT16*)(MCU_memory+0x1931));
    _17CC = MCU_memory + 0x9000;
    if (_17CC[0] != 'L' || _17CC[1] != 'I' || _17CC[2] != 'B')
    {
        _773B:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
_7743:
    // _17C6得到所有资源的个数，每个资源占3字节
    _17C6 = (_17CC[0x000C + 0x0001] << 0x0008) + _17CC[0x000C];
    _17CC += 0x0010;
    _17C6 /= 0x0003;
    if (_17C6%0x0002 != 0x0000)
    {
        _7875:
        _17C4 = (_17C6+0x0001)>>0x0001;
    }
    else
    {
        _78A6:
        _17C4 = _17C6>>0x0001;
    }
    // 二分法查找资源标记
    _78C7:
    _17C8 = 0x00;
    while (0x01)
    {
        _78D7:
        if (_17CC[0] == _17D0[0])
        {
            // 找到资源标识则退出循环
            _7908:
            break;
        }
        _790E:
        if (_17CC[0] < _17D0[0])
        {
            _793F:
            _17C2 = _17C4*0x0003;
            _17CC += _17C2;
            _17C8 += _17C2;
        }
        else
        {
            _79B7:
            _17C2 = _17C4*0x0003;
            _17CC -= _17C2;
            _17C8 -= _17C2;
        }
        _7A2C:
        if (_17C4 == 0x0000)
        {
            // 没找到
            _7A47:
            MCU_memory[0x1935] = ILOSE_ERR;
            return;
        }
        _7A4F:
        if (_17C4 == 0x0001)
        {
            _7A6A:
            _17CA = 0x01;
        }
        _7A70:
        if (_17C4%0x0002 != 0x0000 && _17CA == 0x00)
        {
            _7A9F:
            _17C4 = (_17C4+0x0001)>>0x0001;
        }
        else
        {
            _7AD0:
            _17C4 >>= 0x0001;
        }
    }
    _7AF4:
    _17CC = MCU_memory + 0x9000 + 0x0010;
    _17CB = 0x00;
    _17C6 = _17C8;
    // 从上个标记往右找
    for (_17C8=_17C6+0x0002; ; _17C8 += 0x0003)
    {
        _7B92:
        if (_17CC[_17C8 - 0x0002] != _17D0[0])
        {
            // 往右没找到
            _7BEF:
            break;
        }
        _7BF2:
        if ((_17CC[_17C8] == _17D0[0x02]) && (_17CC[_17C8 - 0x0001] == _17D0[0x01]))
        {
            // 找到匹配的标签
            _7C97:
            _17CB = 0x01;
            break;
        }
    }
    _7CA3:
    if (_17CB == 0x00)
    {
        // 往右没找到则往左找
        _7CAC:
        if (_17C6+0x0001 == 0x0001)
        {
            _7CD4:
            MCU_memory[0x1935] = ILOSE_ERR;
            return;
        }

        _7CDC:
        // 从上个标记往左找
        for (_17C8=_17C6-0x0001; ; _17C8-=0x0003)
        {
            _7D28:
            if (_17CC[_17C8 - 0x0002] != _17D0[0])
            {
                // 往左没找到
                _7D85:
                break;
            }
            _7D88:
            if (_17CC[_17C8] == _17D0[0x02] && _17CC[_17C8-0x0001] == _17D0[0x01])
            {
                // 找到匹配的标签
                _7E2D:
                _17CB = 0x01;
                break;
            }
            _7E36:
            if (_17CC[_17C8 - 0x0002] != _17D0[0])
            {
                _7E93:
                break;
            }
        }
        _7E99:
        if (_17CB == 0x00)
        {
            _7EA2:
            MCU_memory[0x1935] = ILOSE_ERR;
            return;
        }
    }
    // 根据地址表计算数据所在地址
    _7EAA:
    _17CC = MCU_memory + 0x9000 + 0x2000; // 0xB000;
    _17D0[0x0003] = _17CC[_17C8 - 0x0002];
    *(UINT16*)(_17D0+0x0004) = (_17CC[_17C8] << 0x0008)+ _17CC[_17C8 - 0x0001];
    static UINT8 type[3] = { 0 };
    if ((type[0] != _17D0[0]) || (type[1] != _17D0[1]) || (type[2] != _17D0[2]))
    {
        char *p = "";
        switch (_17D0[0])
        {
            case DAT_GUDSCRIPT:
                p = "剧情数据界";
                break;
            case DAT_MAPDATA:
                p = "地图文件界";
                break;
            case DAT_ACTORRES:
                p = "角色资源界";
                break;
            case DAT_MGICRES:
                p = "魔法资源界";
                break;
            case DAT_SPECRES:
                p = "特效资源界";
                break;
            case DAT_GOODSRES:
                p = "道具资源界";
                break;
            case DAT_TILEDATA:
                p = "地图块资源界";
                break;
            case DAT_ACTORPIC:
                p = "角色图片资源界";
                break;
            case DAT_GOODSPIC:
                p = "道具图片资源界";
                break;
            case DAT_SPEPIC:
                p = "特效图片资源界";
                break;
            case DAT_SUNDRYPIC:
                p = "杂类图片资源界";
                break;
            case DAT_MAGICLINK:
                p = "链资源界";
                break;
        }
        LOG("find[%s]: %d-%d-%d address: 0x%06X\n", p, _17D0[0], _17D0[1], _17D0[2],
            _17D0[3] * 0x4000 + *(UINT16*)(_17D0 + 0x0004));
        if (_17D0[0] == 11 && _17D0[1] == 3 && _17D0[2] == 1)
        {
            printf("debug\n");
        }
        type[0] = _17D0[0];
        type[1] = _17D0[1];
        type[2] = _17D0[2];
    }
}

// 判断数据是那种类型的，与_17CF（取值范围[1,12]）匹配则返回0x00
UINT8 _0021B0AE(UINT8 _17CF)
{
    const UINT8 _8243[]="GUTMAPARSMRSSRSGRSTILACPGDPGGJSUNMLR";
    UINT8* _17CC;
    if (MCU_memory[0x1935])
    {
        _80C9:
        return 0xFF;
    }
    _80CE:
    if (_17CF > 0x0C || _17CF+0x01 == 0x01)
    {
        _80EB:
        return 0xFF;
    }
    _80F0:
    _17CC = MCU_memory + 0x9000;
    if (_17CC[0] != _8243[(_17CF-0x01)*0x03] || _17CC[1] != _8243[(_17CF-0x01)*0x03+0x0001] || _17CC[2] != _8243[(_17CF-0x01)*0x03+0x0002])
    {
        _8204:
        return 0xFF;
    }
    _8209:
    return 0x00;
}

void _0021B221(UINT8* _17D0)
{
    _0021A678(_17D0);
}

UINT8 _0021C000(UINT8 _17CF)
{
    UINT8 _17CE; // 战斗结果
    if (MCU_memory[0x1935])
    {
        _501B:
        return 0xFF;
    }
    _5020:
    _17CE = 0x00;
    MCU_memory[0x1927] = 0x00;
    MCU_memory[0x1925] = _17CF;
    if (_17CF)
    {
        _503B:
        _0021EA2D(); // 随机普通敌人
    }
    _503E:
    _0021C8AF(); // 初始化战斗前的各项数据
    _0021CD12(); // 显示敌人角色
    __8E54();
    _0021F60F();
    while (_17CE == 0x00)
    {
        _505B:
        _0021EBE5(); // 战斗触发事件回合处理
        _0021C16E();
        if (MCU_memory[0x1927])
        {
            _5069:
            _17CE = MCU_memory[0x1927];
            break;
        }
        _5073:
        _17CE = __8E06();
        // 设定了限定最多回合数，且当前回合数==限定最多回合数
        if (MCU_memory[0x191D] && MCU_memory[0x191E] == MCU_memory[0x191D])
        {
            _5096:
            _17CE = 0x03;
            break;
        }
    _509F:
        // 当前回合数加1
        MCU_memory[0x191E]++;
    }
    _50A5:
    if (_17CE == 0x01)
    { // 战斗胜利
        _50B1:
        __8E93();
    }
    _50BC:
    if (_17CE != 0x02)
    { // 战斗没有失败
        _50C8:
        fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780, 0xAA);
        __8DA2();
        __8D96();
        __8D99();
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
    }
    _5149:
    __8E9F();
    return _17CE;
}

void _0021C16E()
{
    UINT8 _17CF;
    UINT8 _17CE;
    if (MCU_memory[0x1935])
    {
        _5186:
        return;
    }
    _5189:
    if (MCU_memory[0x191C] == 0x00)
    {
        _5191:
        if (MCU_memory[*(UINT16*)(MCU_memory+0x181E)] == 0x06)
        {
            _51A7:
            return;
        }
    }
    _51AA:
    _17CE = 0x00;
    while (_17CE < 0x03)
    {
        _51BC:
        // 玩家角色资源ID
        if (MCU_memory[_17CE*0x0019+0x1988+0x01] == 0x00)
        { // 玩家角色资源ID为0代表此位置没有玩家角色
            _51F3:
            _17CE++;
            continue;
        }
        _51FF:
        switch (_0021C307(_17CE))
        {
        case 0x0000: // _5223 普通攻击
            if (_0021D208(_17CE) == 0x00)
            {
                _5232:
                _17CE++;
            }
            break;
        case 0x0001: // _523E 魔法
            if (_0021D3AF(_17CE) == 0x00)
            {
                _524D:
                _17CE++;
            }
            break;
        case 0x0002: // _5259 选项
            if (_0021D9DC(_17CE) == 0x00)
            {
                _5268:
                _17CF = MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)];
                if (_17CF == 0x06 || _17CF == 0x07)
                {
                    _52AF:
                    return;
                }
                _52B5:
                _17CE++;
            }
            break;
        case 0x0003: // _52C1
            if (_0021DDBA(_17CE) == 0x00)
            {
                _52D0:
                return;
            }
            break;
        case 0x0006: // _52D6
            _17CE++;
            break;
        case 0x0005: // _52E2
            _17CE = _0021F445(_17CE);
            break;
        default: // _52F0
            return;
            break;
        }
    }
}

UINT8 _0021C307(UINT8 _17CF)
{
    const UINT8 _8724[] = { 0x40,0x60,0x80 };
    const UINT8 _8727[] = { 0x34,0x30,0x28 };
    UINT8 _17C6[4];
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT8* _17BE;
    if (MCU_memory[0x1935])
    {
        _5322:
        return 0xFF;
    }
    _5327:
    _17C5 = 0x00;
    fillmem(_17C6, 0x0004, 0x00);
    // 角色资源ID
    if (MCU_memory[_17CF*0x0019+0x1988+0x01] == 0x00)
    {
        _539F:
        return 0x06;
    }
_53A4:
    //  hp
    if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x08) == 0x0000)
    {
        _53FA:
        return 0x06;
    }
    _53FF:
    if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x04] != 0xFF)
    {
        _5444:
        return 0x06;
    }
    _5449:
    if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x06] != 0xFF)
    {
        _548E:
        return 0x06;
    }
    _5493:
    _17C4 = 0x00;
    _17C3 = 0x00;
    _17C1 = _8724[_17CF]+0x05;
    _17C0 = _8727[_17CF]-0x07;
    _17BE = MCU_memory + 0x4100;
    __8E78(0x00, 0x00);
    __8E42(_17CF, _17C3);
    SysSaveScreen(_17C1, _17C0, _17C1 + 0x0F, _17C0 + 0x0F, _17BE);
    _0021F6C9(_17C1, _17C0, 0x00);
    while (MCU_memory[0x1935] == 0x00 && MCU_memory[0x1927] == 0x00)
    {
        MsgType _17CA;
        _55C1:
        if (GuiGetMsg(&_17CA) == 0x00)
        {
            _55F6:
            continue;
        }
        _55F9:
        if (GuiTranslateMsg(&_17CA) == 0x00)
        {
            _562E:
            continue;
        }
        _5631:
        if (_17CA.type == DICT_WM_COMMAND)
        {
            _563D:
            if (_17CA.param == CMD_RETURN_HOME)
            {
                _5658:
                MCU_memory[0x1935] = 0xFE;
            }
        }
        _565D:
        if (_17CA.type == DICT_WM_CHAR_FUN)
        {
            _56A0:
            switch (_17CA.param)
            {
            case CHAR_UP:  // _56CA
                _17C3 = 0x00;
                break;
            case CHAR_DOWN:  // _56D3
                _17C3 = 0x02;
                break;
            case CHAR_LEFT:  // _56DC
                if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x05] == 0xFF)
                {
                    _5721:
                    _17C3 = 0x01;
                }
                break;
            case CHAR_RIGHT:  // _572A
                if (MCU_memory[0x1926] > 0x01)
                { // 玩家人数 > 1
                    _573A:
                    _17C3 = 0x03;
                }
                break;
            case CHAR_EXIT:  // _5743
                return 0x05;
                break;
            case CHAR_ENTER:  // _5748
                return _17C3;
                break;
            }
            _574F:
            __8E42(_17CF, _17C3);
            _0021F6C9(_17C1, _17C0, 0x00);
            continue;
        }
        else if (_17CA.type == DICT_WM_CHAR_ASC)
        {
            _579B:
            switch (_17CA.param)
            {
            case 'A':  // _57C5
            case 'a':  // _57C5
                break;
            case 'D':  // _57C8
            case 'd':  // _57C8
                break;
            case 'R':  // _57CB
            case 'r':  // _57CB
                if (MCU_memory[*(UINT16*)(MCU_memory+0x181E)] == 0x06)
                {
                    _57E1:
                    MCU_memory[0x191C] = 0x00;
                }
                _57E6:
                return 0x04;
                break;
            case 'E':  // _57EB
            case 'e':  // _57EB
                break;
            case 'W':  // _57EE
            case 'w':  // _57EE
                break;
            }
        }
        else if (_17CA.type != DICT_WM_TIMER)
        {
            _569D:
            continue;
        }
        _57F1:
        _17C4 = !_17C4;
        if (_17C4 == 0x00)
        {
            _5805:
            _17C2 = 0x01;
        }
        else
        {
            _580E:
            _17C2 = 0x00;
        }
        _5814:
        SysRestoreScreen(_17C1, _17C0, _17C1 + 0x0F, _17C0 + 0x0F, _17BE);
        _0021F6C9(_17C1, _17C0, _17C2);
    }
    _5897:
    return 0xFF;
}

void _0021C8AF()
{
    UINT8 _17CC;
    if (MCU_memory[0x1935])
    {
        _58C7:
        return;
    }
    _58CA:
    if (MCU_memory[0x18DD] == MCU_memory[0x18DE])
    {
    _58E2:
        // 右上角图ID
        MCU_memory[0x18DE] = MCU_memory[0x18DE] + 0x1E;
    }
    _5904:
    if (MCU_memory[0x18DD] == MCU_memory[0x18DF])
    {
    _591C:
        // 左下角图ID
        MCU_memory[0x18DF] = MCU_memory[0x18DF] + 0x6E;
    }
    _593E:
    _17CC = 0x05;
    MCU_memory[0x1926] = 0x00; // 玩家人数
    for (UINT8 _17CD=0x00; _17CD<0x03; _17CD++)
    {
        _596A:
        // 角色资源ID
        if (MCU_memory[_17CD*0x0019+0x1988+0x01])
        {
            _59A1:
            MCU_memory[0x1926]++; // 玩家人数加1
        }
        _59A4:
        fillmem(MCU_memory + _17CD * 0x0007 + 0x18EC, 0x0007, 0xFF);
        MCU_memory[_17CD+0x1916] = 0x00;
        MCU_memory[_17CD+0x1919] = 0x00;
        // 重置玩家增减益
        fillmem(MCU_memory + _17CD * 0x0005 + 0x1800, _17CC, 0x00);
    }
    _5AE1:
    for (UINT8 _17CD=0x00; _17CD<0x03; _17CD++)
    {
        _5B02:
        fillmem(MCU_memory + _17CD * 0x0007 + 0x1901, 0x0007, 0xFF);
        // 重置敌人增减益
        fillmem(MCU_memory + _17CD * 0x0005 + 0x180F, _17CC, 0x00);
    }
    _5BF5:
    for (UINT8 _17CD=0x00; _17CD<0x0A; _17CD++)
    {
        _5C16:
        MCU_memory[_17CD*0x0003+0x18BF] = 0x00;
        MCU_memory[_17CD*0x0003+0x18BF+0x0001] = 0x00;
        MCU_memory[_17CD*0x0003+0x18BF+0x0002] = 0x00;
    }
_5CE7:
    // 待增加角色经验值
    *(UINT16*)(MCU_memory+0x18E0) = 0x0000;
    // 待增加角色金钱
    *(UINT16*)(MCU_memory+0x18E2) = 0x0000;
    MCU_memory[0x191E] = 0x00; // 当前回合数
    MCU_memory[0x191C] = 0x01;
}

void _0021CD12()
{
    UINT8 _17CE;
    if (MCU_memory[0x1935])
    {
        _5D2A:
        return;
    }
    _5D2D:
    for (UINT8 _17CF=0x00; _17CF<0x03; _17CF++)
    {
        _5D4E:
        // 敌人角色资源ID
        _17CE = MCU_memory[_17CF*0x0033+0x1826+0x01];
        if (_17CE == 0x00)
        {
            _5D8D:
            continue;
        }
    _5D90:
        //          敌人角色 ID
        __8DD9(_17CF, 0x03, _17CE, 0x00, 0x00);
        if (MCU_memory[0x1935])
        {
            _5DCD:
            return;
        }
    }
}

void _0021CDE4(UINT8 _17CF, UINT8 _17D0)
{
    const UINT8 _872A[] = { 0x0C,0x2C,0x52 }; // 敌人图片x坐标
    const UINT8 _872D[] = { 0x19,0x0E,0x0B }; // 敌人图片y坐标
    UINT8* _17CB;
    UINT8 width;
    UINT8 height;
    UINT8 _17C8;
    UINT8 x1;
    UINT8 y1;
    UINT16 _17C4;
    if (MCU_memory[0x1935])
    {
        _5DFF:
        return;
    }
_5E02:
    //      敌人角色图片数据的bank偏移
    _17C4 = (MCU_memory[_17CF * 0x0033 + 0x1826 + 0x2E] << 0x002) + *(UINT16*)(MCU_memory+0x1931);
    _DataBankSwitch(0x09, 0x04, _17C4);
    if (__8F0D(DAT_ACTORPIC) != 0x00)
    {
        _5EC8:
        MCU_memory[0x1935] = DLOSE_ERR;
        return;
    }
_5ED0:
    //                          敌人角色图片数据映射到0x9000后，相对于0x9000的偏移量
    _17CB = MCU_memory+0x9000 + *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x2F);
    width = _17CB[2];
    height = _17CB[3];
    _17C8 = _17CB[5];
    _17CB += 0x0006;
    UINT16 _17C2 = width >>0x03;
    if (width &0x07)
    {
        _600A:
        _17C2 += 0x0001;
    }
    _602D:
    _17C2 *= height;
    _17C2 *= _17C8;
    _17C2 *= _17D0;
    _17CB += _17C2;
    x1 = _872A[_17CF];
    y1 = _872D[_17CF];
    x1 -= width /0x06;
    y1 -= height / 0x08;
    if (_17C8 == 0x02)
    { // 显示透明图片
        _613E:
        __8DA8(x1, y1, width, height, _17CB);
        return;
    }
    _6183:
    SysPictureDummy(x1, y1, x1 + width - 0x01, y1 + height - 0x01, _17CB, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x00);
}

// 普通攻击
UINT8 _0021D208(UINT8 _17CF)
{
    if (MCU_memory[0x1935])
    {
        _6223:
        return 0xFF;
    }
    _6228:
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x01;
    // 如果普通攻击可能产生XX异常状态
    if (MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0002]&0x10)
    {
        _62A2:
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0xAA;
        return 0x00;
    }
    _62E9:
    while (MCU_memory[0x1935] == 0x00)
    {
        _62F1:
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = _0021E331(0x02, _17CF, 0x00);
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01] == 0xFF)
        {
            _638F:
            return 0xFF;
        }
        else
        {
            _6394:
            return 0x00;
        }
    }
    return MCU_memory[0x1935];
}

// 选择魔法攻击
UINT8 _0021D3AF(UINT8 _17CF)
{
    UINT8 _8788[] = "真气不足!";
    UINT16 _17CB;
    UINT8* _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8* _17C5=NULL;
    UINT8 _17C4;
    UINT8 _17BA[0x0A];
    if (MCU_memory[0x1935])
    {
        _63CA:
        return 0xFF;
    }
    _63CF:
    _17C7 = 0x00;
    // 指向角色魔法链数据
    _17CB = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x15);
    while (MCU_memory[0x1935] == 0x00)
    {
        _6422:
        while (MCU_memory[0x1935] == 0x00)
        {
            _642A:
            //                      角色学会
            _17C7 = __8EAF(_17CB, MCU_memory[_17CF * 0x0019 + 0x1988 + 0x0E], _17C7);
            if (_17C7 == 0xFF)
            {
                _64A9:
                return _17C7;
            }
        _64B0:
            //                          第_17CB个角色选择的魔法类型         第_17CB个角色选择的魔法Idx
            _17C5 = __8F0A(DAT_MGICRES, MCU_memory[_17CB + _17C7 * 0x0002], MCU_memory[_17CB+_17C7*0x0002+0x01]);
            if (MCU_memory[0x1935])
            {
                _6558:
                return 0xFF;
            }
            _655D:
            _17C9 = _17C5;
            //  如果耗费真气>第_17CF个角色的MP
            if (_17C9[0x04] > *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0C))
            {
                _65EC:
                _SysMemcpy(_17BA, _8788, 0x000A);
                SysTimer1Close();
                // 显示"真气不足!"消息框
                GuiMsgBox(_17BA, 0x0064);
                SysTimer1Open(0x32);
            }
            else
            {
                _6676:
                break;
            }
        }
    _667C:
        // 第_17CB个角色选择使用魔法
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x02;
        // 第_17CB个角色选择的魔法类型
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003] = MCU_memory[_17CB + _17C7 * 0x0002];
        // 第_17CB个角色选择的魔法Idx
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003+0x0001] = MCU_memory[_17CB + _17C7 * 0x0002 + 0x01];
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x0003, _17C5, 0x001A);
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003+0x03] >= 0x80)
        {
            _6871:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0xAA;
            return 0x00;
        }
        else
        {
            _68BB:
            _17C8 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003];
            if (_17C8 == 0x01 || _17C8 == 0x05)
            {
                _690F:
                _17C4 = 0x02;
            }
            else
            {
                _6918:
                _17C4 = 0x01;
            }
            _691E:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = _0021E331(_17C4, _17CF, 0x01);
            if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] == 0xFF)
            {
                _69BE:
                continue;
            }
            else
            {
                _69C1:
                return 0x00;
            }
        }
    }
    return MCU_memory[0x1935];
}

UINT8 _0021D9DC(UINT8 _17CF)
{
    const UINT8 _8792[] = "围攻道具防御逃跑状态";
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17B6[0x16];
    if (MCU_memory[0x1935])
    {
        _69F7:
        return 0xFF;
    }
    _69FC:
    _17CD = 0x00;
    _17CC = 0x00;
    _SysMemcpy(_17B6, _8792, 0x0016);
    __8E33();
    while (MCU_memory[0x1935] == 0x00)
    {
        _6A54:
        _17CD = __8EB2(0x0A, 0x05, 0x05, 0x05, _17B6, _17CD, &_17CC);
        switch (_17CD)
        {
        case 0x0000: // _6ACE
            MCU_memory[0x191C] = 0x00;
            for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
            {
                _6AF4:
                // 角色资源ID
                if (MCU_memory[_17CE*0x0019+0x1988+0x01] == 0x00)
                {
                    _6B2B:
                    continue;
                }
                _6B2E:
                MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)] = 0x06;
                // 普通攻击可能产生异常状态
                if (MCU_memory[*(UINT16*)(MCU_memory+_17CE*0x0019+0x1988+0x11)+0x02]&0x10)
                {
                    _6BA8:
                    MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)+0x0001] = 0xAA;
                }
                else
                {
                    _6BED:
                    MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)+0x0001] = 0x00;
                }
            }
            _6C32:
            return 0x00;
            break;
        case 0x0001: // _6C37
            if (_0021ECE3(_17CF) != 0x00)
            {
                _6C46:
                return 0x00;
            }
            break;
        case 0x0002: // _6C51
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x05;
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0002] = 0x08;
            return 0x00;
            break;
        case 0x0003: // _6CCD
            for (UINT8 _17CE=_17CF; _17CE<0x03; _17CE++)
            {
                _6CF0:
                // 角色资源ID
                if (MCU_memory[_17CE*0x0019+0x1988+0x01] == 0x00)
                {
                    _6D27:
                    continue;
                }
                _6D2A:
                MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)] = 0x07;
            }
            _6D62:
            return 0x00;
            break;
        case 0x0004: // _6D67
            __8E45(_17CF);
            break;
        default: // _6D79
            return 0xFF;
            break;
        }
        _6D7E:
        __8DAE(MCU_memory + 0x4000);
    }
    return MCU_memory[0x1935];
}

UINT8 _0021DDBA(UINT8 _17CF)
{
    UINT8* _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8* _17C6;
    if (MCU_memory[0x1935])
    {
        _6DD5:
        return 0xFF;
    }
_6DDA:
    // 合体法术
    _17CA = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x03];
    if (_17CA)
    {
        _6E25:
        //                  攻击型
        _17CB = __8F0A(DAT_MGICRES, 0x01, _17CA);
        if (MCU_memory[0x1935])
        {
            _6E61:
            return 0xFF;
        }
        _6E66:
        _17C6 = _17CB;
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x04;
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003] = 0x01;
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003+0x0001] = _17CA;
        // 复制法术数据
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x0003, _17CB, 0x001A);
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003+0x03] >= 0x80)
        {
            _6FFD:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0xAA;
        }
        else
        {
            _7042:
            _17C9 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003];
            if (_17C9 == 0x01 || _17C9 == 0x05)
            {
                _7096:
                _17C8 = 0x02;
            }
            else
            {
                _709F:
                _17C8 = 0x01;
            }
            _70A5:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = _0021E331(_17C8, _17CF, 0x01);
            if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01] == 0xFF)
            {
                _7145:
                return 0xFF;
            }
        }
    }
    else
    {
        _714D:
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x03;
        // 普通攻击可能产生异常状态
        if (MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x02]&0x10)
        {
            _71C7:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0xAA;
        }
        else
        {
            _720C:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = _0021E331(0x02, _17CF, 0x03);
            if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01] == 0xFF)
            {
                _72AA:
                return 0xFF;
            }
        }
    }
    _72AF:
    for (UINT8 _17CA=0x00; _17CA<0x03; _17CA++)
    {
        _72D0:
        if (_17CA == _17CF)
        {
            _72DE:
            continue;
        }
        _72E1:
        MCU_memory[*(UINT16*)(MCU_memory+(_17CA<<1)+0x181E)] = 0xAA;
    }
    _7319:
    return 0x00;
}

UINT8 _0021E331(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    const UINT8 _8724[] = { 0x40,0x60,0x80 };
    const UINT8 _8727[] = { 0x34,0x30,0x28 };
    const UINT8 _872A[] = { 0x0C,0x2C,0x52 };
    const UINT8 _872D[] = { 0x19,0x0E,0x0B };
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8* _17BF;
    UINT8* _17BD;
    if (MCU_memory[0x1935])
    {
        _734C:
        return 0xFF;
    }
    _7351:
    _17C5 = 0x00;
    _17C6 = 0x00;
    _17C8 = 0x01;
    if (_17CF == 0x02)
    {
        _736F:
        if (__8E66(_17C6, &_17C5) == 0x01)
        {
            _73A8:
            return _17C5;
        }
        _73AF:
        _17C2 = _872A[_17C5]+0x04;
        _17C1 = _872D[_17C5]-0x0B;
    }
    else
    {
        _73F6:
        _17C5 = _17D0;
        if (__8E60(_17C6, &_17C5) == 0x01)
        {
            _7437:
            return _17C5;
        }
        _743E:
        _17C2 = _8724[_17C5]+0x05;
        _17C1 = _8727[_17C5]-0x07;
    }
    _7482:
    _17BF = MCU_memory + 0x4100;
    _17BD = MCU_memory + 0x4200;
    _17C4 = _8724[_17D0]+0x05;
    _17C3 = _8727[_17D0]-0x07;
    __8E42(_17D0, _17D1);
    SysSaveScreen(_17C4, _17C3, _17C4 + 0x0F, _17C3 + 0x0F, _17BF);
    SysSaveScreen(_17C2, _17C1, _17C2 + 0x0F, _17C1 + 0x0F, _17BD);
    _0021F6C9(_17C4, _17C3, 0x00);
    while (MCU_memory[0x1935] == 0x00)
    {
        MsgType _17CA;
        _75DD:
        if (GuiGetMsg(&_17CA) == 0x00)
        {
            _7612:
            continue;
        }
        _7615:
        if (GuiTranslateMsg(&_17CA) == 0x00)
        {
            _764A:
            continue;
        }
        _764D:
        if (_17CA.type == DICT_WM_COMMAND)
        {
            _7659:
            if (_17CA.param == CMD_RETURN_HOME)
            {
                _7674:
                MCU_memory[0x1935] = 0xFE;
            }
        }
        _7679:
        if (_17CA.type == DICT_WM_CHAR_FUN)
        {
            _76AA:
            switch (_17CA.param)
            {
            case CHAR_LEFT: //_76D4
                _17C6 = 0x2D;
                break;
            case CHAR_RIGHT: //_76DD
                _17C6 = 0x2B;
                break;
            case CHAR_EXIT: //_76E6
                return 0xFF;
                break;
            case CHAR_ENTER: //_76EB
                return _17C5;
                break;
            }
            _76F2:
            if (_17CF == 0x01)
            {
                _76FE:
                __8E60(_17C6, &_17C5);
                _17C7 = _17C5;
                _17C2 = _8724[_17C5]+0x05;
                _17C1 = _8727[_17C5]-0x07;
            }
            else
            {
                _777C:
                __8E66(_17C6, &_17C5);
                _17C7 = _17D0;
                _17C2 = _872A[_17C5]+0x04;
                _17C1 = _872D[_17C5]-0x0B;
            }
            _77F7:
            __8E42(_17C7, _17D1);
            SysSaveScreen(_17C2, _17C1, _17C2 + 0x0F, _17C1 + 0x0F, _17BD);
            _0021F6C9(_17C4, _17C3, 0x00);
        }
        else if (_17CA.type == DICT_WM_TIMER)
        {
            _789B:
            SysRestoreScreen(_17C4, _17C3, _17C4 + 0x0F, _17C3 + 0x0F, _17BF);
            SysRestoreScreen(_17C2, _17C1, _17C2 + 0x0F, _17C1 + 0x0F, _17BD);
            _17C8 = !_17C8;
            if (_17C8)
            {
                _795F:
                _17C9 = 0x01;
            }
            else
            {
                _7968:
                _17C9 = 0x00;
            }
            _796E:
            _0021F6C9(_17C4, _17C3, _17C9+0x00);
            if (_17CF == 0x01)
            {
                _79A2:
                _0021F6C9(_17C2, _17C1, _17C9+0x02);
            }
            else
            {
                _79CD:
                __8F04(DAT_SUNDRYPIC, 0x02, 0x03, _17C9 + 0x00, _17C2, _17C1);
            }
        }
    }
    return MCU_memory[0x1935];
}

void _0021EA2D()
{
    UINT8 _17CD; // 可能出现的敌人有多少种
    UINT8 _17CC;
    UINT16 _17CB;
    if (MCU_memory[0x1935])
    {
        _7A45:
        return;
    }
    _7A48:
    _17CD = __8E6F();
    if (_17CD == 0x00)
    {
        _7A60:
        MCU_memory[0x1935] = ENEMY_K_NODEF;
        return;
    }
_7A68:
    // 出现的敌人的个数
    _17CC = SysRand((PtrRandEnv)(MCU_memory + 0x1928)) % 0x0003 + 0x0001;
    LOG("出现%d个敌人，敌人ID：", _17CC);
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _7ACF:
        if (_17CE < _17CC)
        {
        _7ADD:
            // 随机敌人
            _17CB = SysRand((PtrRandEnv)(MCU_memory + 0x1928)) % _17CD;
            // 敌人角色资源ID = 可能出现的敌人角色资源ID
            MCU_memory[_17CE*0x0033+0x1826+0x0001] = MCU_memory[_17CB + 0x18E4];
            LOG(" %d", MCU_memory[_17CB + 0x18E4]);
        }
        else
        {
            _7B83:
            // 敌人角色资源ID
            MCU_memory[_17CE*0x0033+0x1826+0x0001] = 0x00;
        }
    }
    LOG("\n");
    _7BCF:
    MCU_memory[0x191D] = 0x00; // 未限定最多回合数
}

void _0021EBE5()
{
    if (MCU_memory[0x1935])
    {
        _7BFD:
        return;
    }
    _7C00:
    if (MCU_memory[0x1925])
    { // 与随机敌人战斗
        _7C08:
        return;
    }
    _7C0B:
    for (UINT8 _17CF=0x00; _17CF<0x03; _17CF++)
    {
        _7C2C:
        if (MCU_memory[0x1935])
        {
            _7C34:
            return;
        }
    _7C37:
        // 当前回合数 == 某NPC战斗触发事件回合
        if (MCU_memory[0x191E] == MCU_memory[_17CF+0x191F])
        {
            _7C5F:
            __8E78(0x00, 0x00);
            __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
            // 处理对应NPC战斗触发事件相应的事件
            __8DB4(MCU_memory[_17CF+0x1922]);
        }
    }
}

UINT8 _0021ECE3(UINT8 _17CF)
{
    const UINT8 _87CC[] = "装备投掷使用";
    UINT8 _17C7[4];
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17B3[0x0E];
    UINT8 _17B2;
    UINT8* _17B0;
    UINT8* _17AE;
    UINT8* _17AC;
    // 如果背包中的道具种类==0
    if (MCU_memory[0x1A95] == 0x00)
    {
        _7CFE:
        return 0x00;
    }
    _7D03:
    _17C2 = 0x00;
    _17C1 = 0x00;
    _SysMemcpy(_17B3, _87CC, 0x000E);
    while (0x01)
    {
        _7D4F:
        _17C2 = __8EB2(0x1E, 0x0F, 0x04, 0x03, _17B3, _17C2, &_17C1);
        if (_17C2 == 0xFF)
        {
            _7DB4:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x00;
            return 0x00;
        }
        _7DEE:
        if (_17C2 == 0x00)
        {
            _7DF7:
            __8D78();
            return 0x00;
        }
        else if (_17C2 == 0x01)
        {
            _7E16:
            _17C7[0x00] = 0x07;
            _17C7[0x01] = 0x08;
            _17C5 = 0x02;
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x09;
            _17B2 = 0x02;
        }
        else
        {
            _7E90:
            _17C7[0x00] = 0x09;
            _17C7[0x01] = 0x0A;
            _17C7[0x02] = 0x0B;
            _17C7[0x03] = 0x0C;
            _17C5 = 0x04;
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x08;
            _17B2 = 0x01;
        }
        _7F3D:
        _17C6 = 0x00;
        _17C4 = 0x00;
        _17B0 = MCU_memory + *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x17);
        SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
        _17C6 = __8EB5(_17B0, MCU_memory[0x1A95], _17C7, _17C5, _17C6, &_17C4);
        if (_17C6 == 0xFF)
        {
            _8031:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x00;
            return 0x00;
        }
        _806B:
        _17AE = __8F0A(DAT_GOODSRES, _17B0[_17C6 * 0x0003], _17B0[_17C6*0x0003+0x01]);
        _17AC = MCU_memory + *(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x001D;
        // 道具类型
        _17AC[0] = _17AE[0x00];
        // 道具Idx
        _17AC[1] = _17AE[0x01];
        // 道具持续回合
        _17AC[2] = _17AE[0x04];
        // 复制道具名称
        _SysMemcpy(_17AC + 0x0003, _17AE + 0x0006, 0x000C);
        // 复制道具属性
        _SysMemcpy(_17AC + 0x000F, _17AE + 0x0016, 0x0008);
        _17C3 = (UINT8)*(UINT16*)(_17AC+0x000F+0x06);
        if (_17C3 > 0x0F)
        {
            _8342:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0xAA;
        }
        else
        {
            _8387:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = _0021E331(_17B2, _17CF, 0x02);
        }
        _83F4:
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01] != 0xFF)
        {
            _8427:
            break;
        }
    }
    _842D:
    return 0x01;
}

UINT8 _0021F445(UINT8 _17CF)
{
    if (_17CF)
    {
        _8461:
        _17CF--;
        if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x06] == 0xFF)
        {
            _84AF:
            return _17CF;
        }
        _84B6:
        if (_17CF)
        {
            _84BF:
            _17CF--;
            if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x06] == 0xFF)
            {
                _850D:
                return _17CF;
            }
        }
    }
    _8514:
    return _17CF;
}

UINT8 _0021F52E(UINT8* _17D0)
{
    const UINT8 _87DA[] = "123";
    const UINT8 _87DE[] = "456";
    const UINT8 _87E2[] = "789";
    if (_SysMemcmp(_17D0, _87DA, 0x0004) == 0x00)
    {
        _856F:
        return 0x01;
    }
    _8577:
    if (_SysMemcmp(_17D0, _87DE, 0x0004) == 0x00)
    {
        _85B8:
        return 0x02;
    }
    _85C0:
    if (_SysMemcmp(_17D0, _87E2, 0x0004) == 0x00)
    {
        _8601:
        return 0x03;
    }
    _8609:
    return 0x00;
}

void _0021F60F()
{
    for (UINT8 _17CF=0x00; _17CF<0x03; _17CF++)
    {
        _8640:
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x02] == 0x08)
        {
            _8673:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0002] = 0x00;
        }
    }
}

void _0021F6C9(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    if (MCU_memory[0x1935])
    {
        _86D4:
        return;
    }
    _86D7:
    __8F04(DAT_SUNDRYPIC, 0x02, 0x04, _17D1, _17CF, _17D0);
}

UINT8 _00220000(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2)
{
    UINT8 _17C4[8];
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8* _17C0;
    // 角色资源ID
    if (MCU_memory[_17D2*0x0019+0x1988+0x01] == 0x00)
    {
        _504A:
        return 0x00;
    }
    _504F:
    _17C3 = 0x00;
    // 指向角色属性
    _17C0 = MCU_memory + *(UINT16*)(MCU_memory+_17D2*0x0019+0x1988+0x11);
    for (UINT8 _17CC=0x00; _17CC<0x08; _17CC+=2)
    {
        _50BD:
        _17C4[_17CC] = _17D0[_17CC / 0x02] >> 0x0008;
        _17C4[_17CC+0x01] = (UINT8) _17D0[_17CC / 0x02];
    }
    _519C:
    switch (_17CF)
    {
    case 0x0001: // _51BD
        // hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _51E5:
            break;
        }
    _51E8:
        // hp
        if (*(UINT16*)(_17C0 + 0x08) <= _17D0[0x00])
        {
            _523F:
            *(UINT16*)(_17C0+0x0008) = 0x0000;
            _17C3 = 0x01;
        }
        else
        {
            _527B:
            *(UINT16*)(_17C0+0x0008) = *(UINT16*)(_17C0 + 0x08)- _17D0[0x00];
        }
    _5316:
        // mp
        if (*(UINT16*)(_17C0 + 0x0C) <= _17D0[0x01])
        {
            _536D:
            *(UINT16*)(_17C0+0x000C) = 0x0000;
        }
        else
        {
            _53A3:
            *(UINT16*)(_17C0+0x000C) = *(UINT16*)(_17C0 + 0x0C)- _17D0[0x01];
        }
        _543E:
        _17C2 = _17C4[7] & 0xF0;
        _17C2 >>= 0x04;
        if (_17C4[4] || _17C4[5] || _17C4[6])
        {
            _5472:
            if (__8E51(_17D2, 0x01) != 0x00)
            {
                _54A0:
                __8E8D(_17D2, _17C4+4, '-', _17C2);
            }
        }
        _54F1:
        _17C4[7] = _17C4[7] & 0x0F;
        if (_17C4[7])
        {
            _5521:
            if (__8E51(_17D2, 0x01) != 0x00)
            {
                _554F:
                __8E27(_17D2, _17C4[7], 0x01, _17C2);
            }
        }
        break;
    case 0x0002: // _558C
        // hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _55B4:
            break;
        }
        _55B7:
        _17C2 = _17C4[7] &0xF0;
        _17C2 >>= 0x04;
        __8E8D(_17D2, _17C4+4, '+', _17C2);
        break;
    case 0x0003: // _5624
        // hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _564C:
            break;
        }
        _564F:
        *(UINT16*)(_17C0+0x0008) = *(UINT16*)(_17C0 + 0x08)+ _17D0[0x00];
        _17C4[7] = _17C4[7] & 0x0F;
        if (_17C4[7])
        {
            _570A:
            __8E2A(_17D2, _17C4[7], 0x01);
        }
        break;
    case 0x0004: // _5740
        // hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _5768:
            break;
        }
    _576B:
        //  hp                                      hpMax
        *(UINT16*)(_17C0 + 0x0008) = _17D0[0x00] * (*(UINT16*)(_17C0 + 0x06)) / 0x0064;
        MCU_memory[0x1926]++; // 玩家人数加1
        break;
    default: //_5813
        break;
    }
    _5816:
    __8E87(_17C0, 0x01);
    return _17C3;
}

UINT8 _00220874(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2)
{
    UINT8 _17C4[8];
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8* _17C0;
    // 敌人角色资源ID
    if (MCU_memory[_17D2*0x0033+0x1826+0x01] == 0x00)
    {
        _58BE:
        return 0x00;
    }
    _58C3:
    _17C3 = 0x00;
    // 指向敌人角色等级开始的数据
    _17C0 = MCU_memory + _17D2 * 0x0033 + 0x1826 + 0x0012;
    for (UINT8 _17CC=0x00; _17CC<0x08; _17CC+=2)
    {
        _5948:
        _17C4[_17CC] = _17D0[_17CC / 0x02] >> 0x0008;
        _17C4[_17CC+0x01] = (UINT8)_17D0[_17CC / 0x02];
    }
    _5A27:
    switch (_17CF)
    {
    case 0x0001: //_5A48
        // 敌人角色hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _5A70:
            break;
        }
        _5A73:
        // 敌人角色hp
        if (*(UINT16*)(_17C0 + 0x08) <= _17D0[0x00])
        {
            _5ACA:
            *(UINT16*)(_17C0+0x0008) = 0x0000;
            _17C3 = 0x01;
        }
        else
        {
            _5B06:
            *(UINT16*)(_17C0+0x0008) = *(UINT16*)(_17C0 + 0x08)- _17D0[0x00];
        }
        _5BA1:
        // 敌人角色mp
        if (*(UINT16*)(_17C0 + 0x0C) <= _17D0[0x01])
        {
            _5BF8:
            *(UINT16*)(_17C0+0x000C) = 0x0000;
        }
        else
        {
            _5C2E:
            *(UINT16*)(_17C0+0x000C) = *(UINT16*)(_17C0 + 0x0C)- _17D0[0x01];
        }
        _5CC9:
        _17C2 = _17C4[7] &0xF0;
        _17C2 >>= 0x04;
        if (_17C4[4] || _17C4[5] || _17C4[6])
        {
            _5CFD:
            if (__8E51(_17D2, 0x02) != 0x00)
            {
                _5D2B:
                __8E90(_17D2, _17C4+4, 0x2D, _17C2);
            }
        }
        _5D7C:
        _17C4[7] = _17C4[7] & 0x0F;
        if (_17C4[7])
        {
            _5DAC:
            if (__8E51(_17D2, 0x02) != 0x00)
            {
                _5DDA:
                __8E27(_17D2, _17C4[7], 0x02, _17C2);
            }
        }
        break;
    case 0x0002: //_5E17
        // 敌人角色hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _5E3F:
            break;
        }
        _5E42:
        _17C2 = _17C4[7] &0xF0;
        _17C2 >>= 0x04;
        __8E90(_17D2, _17C4+4, 0x2B, _17C2);
        break;
    case 0x0003: //_5EAF
        // 敌人角色hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _5ED7:
            break;
        }
        _5EDA:
        // 敌人角色hp
        *(UINT16*)(_17C0+0x0008) = *(UINT16*)(_17C0 + 0x08)+ _17D0[0x00];
        _17C4[7] = _17C4[7] & 0x0F;
        if (_17C4[7])
        {
            _5F95:
            __8E2A(_17D2, _17C4[7], 0x02);
        }
        break;
    case 0x0004: //_5FCB
        // 敌人角色hp
        if (*(UINT16*)(_17C0+0x08) == 0x0000)
        {
            _5FF3:
            break;
        }
        _5FF6:
        // 敌人角色hp                               敌人角色hpMax
        *(UINT16*)(_17C0+0x0008) = _17D0[0x00] * (*(UINT16*)(_17C0 + 0x06)) / 0x0064;
        __8E2A(_17D2, 0xFF, 0x02);
        break;
    default: // _60BE
        break;
    }
    _60C1:
    __8E87(_17C0, 0x02);
    return _17C3;
}

void _0022111F(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2, UINT8* _17D4)
{
    UINT8 _17CA;
    UINT8 _17C2[8];
    UINT8 _17BA[8];
    if (MCU_memory[0x1935])
    {
        _613A:
        return;
    }
    _613D:
    for (UINT8 _17CB=0x00; _17CB<0x08; _17CB+=2)
    {
        _6160:
        _17C2[_17CB] = _17D0[_17CB / 0x02] >> 0x0008;
        _17C2[_17CB+0x01] = (UINT8)_17D0[_17CB / 0x02];
        _17BA[_17CB] = _17D2[_17CB / 0x02] >> 0x0008;
        _17BA[_17CB+0x01] = (UINT8)_17D2[_17CB / 0x02];
    }
    _6309:
    switch (_17CF)
    {
    case 0x0007: // _632A
        _17CA = 0x00;
        // 攻击
        *(UINT16*)(_17D4+0x000E) = *(UINT16*)(_17D4 + 0x0E)- _17C2[2];
        *(UINT16*)(_17D4+0x000E) = *(UINT16*)(_17D4 + 0x0E)+ _17BA[2];
        // 普通攻击可能产生异常状态
        *(UINT16*)(_17D4+0x0002) = _17BA[7];
        break;
    case 0x0001: // _646D
    case 0x0002: // _646D
    case 0x0003: // _646D
    case 0x0004: // _646D
    case 0x0005: // _646D
        _17CA = 0x01;
        // 防御
        *(UINT16*)(_17D4+0x0010) = *(UINT16*)(_17D4 + 0x10)- _17C2[3];
        *(UINT16*)(_17D4+0x0010) = *(UINT16*)(_17D4 + 0x10)+ _17BA[3];
        break;
    case 0x0006: // _6580
        _17CA = 0x02;
        //        每回合变化生命
        _00221F23(_17D4 + 0x0004, _17C2[0], 0x00);
        _00221F23(_17D4 + 0x0004, _17BA[0], 0x01);
        //        每回合变化真气
        _00221F23(_17D4 + 0x0005, _17C2[1], 0x00);
        _00221F23(_17D4 + 0x0005, _17BA[1], 0x01);
        // 合体法术
        _17D4[0x0003] = _17BA[7];
        break;
    case 0x0008: // _6719
    case 0x000C: // _6719
        _17CA = 0xFF;
        break;
    case 0x0009: // _6722
        _17CA = 0xFF;
        // hp
        *(UINT16*)(_17D4+0x0008) = *(UINT16*)(_17D4 + 0x08)+ *(UINT16*)(_17D2);
        // mp
        *(UINT16*)(_17D4+0x000C) = *(UINT16*)(_17D4 + 0x0C)+ *(UINT16*)(_17D2 + 0x02);
        break;
    case 0x000A: // _683F
        _17CA = 0xFF;
        // hp
        *(UINT16*)(_17D4+0x0008) = *(UINT16*)(_17D4 + 0x06) * _17BA[0] / 0x0064;
        break;
    case 0x000B: // _68CE
        _17CA = 0x03;
        break;
    default: // _68D7
        MCU_memory[0x1935] = GOODS_NODEF;
        break;
    }
    _68DF:
    _00221979(_17CA, _17C2, _17BA, _17D4);
    __8E87(_17D4, 0x01);
}

void _00221979(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2, UINT8* _17D4)
{
    const UINT8 _8A9A[] = { 0x00,0x01,0xFF,0x03,0x04,0x05,0x06,0xFF };
    const UINT8 _8AA2[] = { 0x00,0x01,0x02,0xFF,0x04,0x05,0x06,0xFF };
    const UINT8 _8AAA[] = { 0xFF,0xFF,0x02,0x03,0x04,0x05,0x06,0xFF };
    const UINT8 _8AB2[] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0xFF };
    const UINT8* _8ABA[] = { _8A9A,_8AA2,_8AAA,_8AB2 };
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _6994:
        return;
    }
    _6997:
    if (_17CF > 0x03)
    {
        _69A8:
        return;
    }
    _69AB:
    for (UINT8 _17CC=0x00; _17CC<0x08; _17CC++)
    {
        _69CC:
        _17CB = _8ABA[_17CF][_17CC];
        if (_17CB == 0xFF)
        {
            _6A2C:
            continue;
        }
        _6A2F:
        _00221AE9(_17CB, _17D0[_17CB], _17D4, 0x00);
        _00221AE9(_17CB, _17D2[_17CB], _17D4, 0x01);
    }
}

// 切换装备时的属性变动
void _00221AE9(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT8 _17D3)
{
    UINT8* _17CB=NULL;
    UINT8* _17C9=NULL;
    UINT8 _17C8;
    if (MCU_memory[0x1935])
    {
        _6B04:
        return;
    }
    _6B07:
    _17C8 = 0x00;
    if (_17CF < 0x04)
    {
        _6B19:
        _17C8 = 0x01;
    }
    _6B1F:
    switch (_17CF)
    {
    case 0x0000: // _6B40
        // 指向hpMax
        _17CB = _17D1 + 0x0006;
        break;
    case 0x0001: // _6B7C
        // 指向mpMax
        _17CB = _17D1 + 0x000A;
        break;
    case 0x0002: // _6BB8
        // 指向攻击
        _17CB = _17D1 + 0x000E;
        break;
    case 0x0003: // _6BF4
        // 指向防御
        _17CB = _17D1 + 0x0010;
        break;
    case 0x0004: // _6C30
        // 指向身法
        _17C9 = _17D1 + 0x0016;
        break;
    case 0x0005: // _6C6C
        // 指向灵力
        _17C9 = _17D1 + 0x0017;
        break;
    case 0x0006: // _6CA8
        // 指向幸运
        _17C9 = _17D1 + 0x0018;
        break;
    }
    _6CE4:
    if (_17C8)
    {
        _6CED:
        if (_17D0 >= 0x80)
        {
            _6CF9:
            _17D0 -= 0x80;
            _17D3 = !_17D3;
        }
        _6D0F:
        if (_17D3)
        {
            _6D18:
            *(UINT16*)(_17CB) = *(UINT16*)(_17CB) + _17D0;
        }
        else
        {
            _6D85:
            if (*(UINT16*)(_17CB) > _17D0)
            {
                _6DB3:
                *(UINT16*)(_17CB) = *(UINT16*)(_17CB) - _17D0;
            }
            else
            {
                _6E18:
                *(UINT16*)(_17CB) = 0x0000;
            }
        }
    }
    else
    {
        _6E41:
        if (_17D0 >= 0x80)
        {
            _6E4D:
            _17D0 -= 0x80;
            _17D3 = !_17D3;
        }
        _6E63:
        if (_17D3)
        {
            _6E6C:
            *_17C9 = *_17C9 + _17D0;
        }
        else
        {
            _6EA3:
            if (*_17C9 > _17D0)
            {
                _6EC2:
                *_17C9 = *_17C9 - _17D0;
            }
            else
            {
                _6EF9:
                *_17C9 = 0x00;
            }
        }
    }
}

void _00221F23(UINT8* _17D0, UINT8 _17D2, UINT8 _17D3)
{
    if (MCU_memory[0x1935])
    {
        _6F3B:
        return;
    }
    _6F3E:
    if (_17D2 >= 0x80)
    {
        _6F4A:
        _17D2 -= 0x80;
        _17D3 = !_17D3;
    }
    _6F60:
    if (_17D3)
    {
        _6F69:
        *_17D0 = *_17D0 + _17D2;
    }
    else
    {
        _6F9F:
        *_17D0 = *_17D0 - _17D2;
    }
}

void _00221FE3(UINT8* _17D0, UINT8 _17D2)
{
    UINT8* _17CC;
    UINT8* _17CA;
    if (MCU_memory[0x1935])
    {
        _6FFB:
        return;
    }
    _6FFE:
    if (_17D2 == 0x01) // 玩家角色
    {
        _700A:
        _17CC = _17D0;
        // hpMax
        if (*(UINT16*)(_17CC+0x06) > 0x03E7)
        {
            _704D:
            *(UINT16*)(_17CC+0x0006) = 0x03E7;
        }
    _7080:
        // hp
        if (*(UINT16*)(_17CC+0x08) > 0x03E7)
        {
            _70AD:
            *(UINT16*)(_17CC+0x0008) = 0x03E7;
        }
    _70E0:
        // hp > hpMax
        if (*(UINT16*)(_17CC + 0x08) > *(UINT16*)(_17CC+0x06))
        {
            _713B:
            *(UINT16*)(_17CC+0x0008) = *(UINT16*)(_17CC + 0x06);
        }
    _7194:
        // mpMax
        if (*(UINT16*)(_17CC+0x0A) > 0x03E7)
        {
            _71C1:
            *(UINT16*)(_17CC+0x000A) = 0x03E7;
        }
    _71F4:
        // mp
        if (*(UINT16*)(_17CC+0x0C) > 0x03E7)
        {
            _7221:
            *(UINT16*)(_17CC+0x000C) = 0x03E7;
        }
    _7254:
        // mp > mpMax
        if (*(UINT16*)(_17CC + 0x0C) > *(UINT16*)(_17CC+0x0A))
        {
            _72AF:
            *(UINT16*)(_17CC+0x000C) = *(UINT16*)(_17CC + 0x0A);
        }
    _7308:
        // 攻击
        if (*(UINT16*)(_17CC+0x0E) > 0x03E7)
        {
            _7335:
            *(UINT16*)(_17CC+0x000E) = 0x03E7;
        }
    _7368:
        // 防御
        if (*(UINT16*)(_17CC+0x10) > 0x03E7)
        {
            _7395:
            *(UINT16*)(_17CC+0x0010) = 0x03E7;
        }
    _73C8:
        // 身法
        if (_17CC[0x16] > 0x63)
        {
            _73E5:
            _17CC[0x0016] = 0x63;
        }
    _740B:
        // 灵力
        if (_17CC[0x17] > 0x63)
        {
            _7428:
            _17CC[0x0017] = 0x63;
        }
    _744E:
        // 幸运
        if (_17CC[0x18] > 0x63)
        {
            _746B:
            _17CC[0x0018] = 0x63;
        }
    }
    else // 敌人角色
    {
        _7494:
        _17CA = _17D0;
        // 身法
        if (_17CA[0x01] > 0x63)
        {
            _74C7:
            _17CA[0x0001] = 0x63;
        }
    _74ED:
        // 灵力
        if (_17CA[0x02] > 0x63)
        {
            _750A:
            _17CA[0x0002] = 0x63;
        }
    _7530:
        // 智商
        if (_17CA[0x03] > 0x63)
        {
            _754D:
            _17CA[0x0003] = 0x63;
        }
    _7573:
        // 幸运
        if (_17CA[0x04] > 0x63)
        {
            _7590:
            _17CA[0x0004] = 0x63;
        }
    _75B6:
        // hp > hpMax
        if (*(UINT16*)(_17CA + 0x08) > *(UINT16*)(_17CA+0x06))
        {
            _7611:
            *(UINT16*)(_17CA+0x0008) = *(UINT16*)(_17CA + 0x06);
        }
    _766A:
        // mp > mpMax
        if (*(UINT16*)(_17CA + 0x0C) > *(UINT16*)(_17CA+0x0A))
        {
            _76C5:
            *(UINT16*)(_17CA+0x000C) = *(UINT16*)(_17CA + 0x0A);
        }
    _771E:
        // 攻击
        if (*(UINT16*)(_17CA+0x0E) > 0x03E7)
        {
            _774B:
            *(UINT16*)(_17CA+0x000E) = 0x03E7;
        }
    _777E:
        // 防御
        if (*(UINT16*)(_17CA+0x10) > 0x03E7)
        {
            _77AB:
            *(UINT16*)(_17CA+0x0010) = 0x03E7;
        }
    }
}

void _002227EF(UINT8* _17D0, UINT8* _17D2, UINT8* _17D4, UINT8 _17D6)
{
    if (MCU_memory[0x1935])
    {
        _7807:
        return;
    }
    _780A:
    if (_17D6 == 0x2B)
    {
    _7816:
        //                  hpMax
        *(UINT16*)(_17D4) = *(UINT16*)(_17D0)+ *(UINT16*)(_17D2);
        //                          hp
        *(UINT16*)(_17D4+0x0002) = *(UINT16*)(_17D0 + 0x02)+ *(UINT16*)(_17D2 + 0x02);
        *(UINT16*)(_17D4+0x0002) = *(UINT16*)(_17D4);
        //                          mpMax
        *(UINT16*)(_17D4+0x0004) = *(UINT16*)(_17D0 + 0x04)+ *(UINT16*)(_17D2 + 0x04);
        //                          mp
        *(UINT16*)(_17D4+0x0006) = *(UINT16*)(_17D0 + 0x06)+ *(UINT16*)(_17D2 + 0x06);
        *(UINT16*)(_17D4+0x0006) = *(UINT16*)(_17D4 + 0x04);
        //                          攻击
        *(UINT16*)(_17D4+0x0008) = *(UINT16*)(_17D0 + 0x08)+ *(UINT16*)(_17D2 + 0x08);
        //                          防御
        *(UINT16*)(_17D4+0x000A) = *(UINT16*)(_17D0 + 0x0A)+ *(UINT16*)(_17D2 + 0x0A);
        //              身法
        _17D4[0x0010] = _17D0[0x10]+ _17D2[0x10];
        //              灵力
        _17D4[0x0011] = _17D0[0x11]+ _17D2[0x11];
        //              幸运
        _17D4[0x0012] = _17D0[0x12]+ _17D2[0x12];
    }
    else
    {
    _7CF1:
        //                  hpMax
        *(UINT16*)(_17D4) = *(UINT16*)(_17D0)- *(UINT16*)(_17D2);
        //                          hp
        *(UINT16*)(_17D4+0x0002) = *(UINT16*)(_17D0 + 0x02)- *(UINT16*)(_17D2 + 0x02);
        //                          mpMax
        *(UINT16*)(_17D4+0x0004) = *(UINT16*)(_17D0 + 0x04)- *(UINT16*)(_17D2 + 0x04);
        //                          mp
        *(UINT16*)(_17D4+0x0006) = *(UINT16*)(_17D0 + 0x06)- *(UINT16*)(_17D2 + 0x06);
        *(UINT16*)(_17D4+0x0006) = *(UINT16*)(_17D4 + 0x04);
        //                          攻击
        *(UINT16*)(_17D4+0x0008) = *(UINT16*)(_17D0 + 0x08)- *(UINT16*)(_17D2 + 0x08);
        //                          防御
        *(UINT16*)(_17D4+0x000A) = *(UINT16*)(_17D0 + 0x0A)- *(UINT16*)(_17D2 + 0x0A);
        //                          当前经验
        *(UINT16*)(_17D4+0x000C) = *(UINT16*)(_17D0 + 0x0C);
        *(UINT16*)(_17D4+0x000E) = *(UINT16*)(_17D2 + 0x0E);
        //              身法
        _17D4[0x0010] = _17D0[0x10]- _17D2[0x10];
        //              灵力
        _17D4[0x0011] = _17D0[0x11]- _17D2[0x11];
        //              幸运
        _17D4[0x0012] = _17D0[0x12]- _17D2[0x12];
    }
}

void _00223246(UINT8 _17CF, UINT8* _17D0, UINT8 _17D2, UINT8 _17D3)
{
    UINT8 _17CD = 0x00;
    // 玩家角色攻击
    UINT16 _17CB = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0E);
    // 玩家角色防御
    UINT16 _17C9 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x10);
    // 玩家角色身法
    UINT16 _17C7 = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x16];
    if (_17D0[0x00])
    {
        _8364:
        _17CB = _00223988(_17CB, _17D0[0x00], _17D2);
        // 玩家攻击增减益
        *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800) = _17CB;
        _17CD += 0x40;
    }
    _8408:
    if (_17D0[0x01])
    {
        _841C:
        _17C9 = _00223988(_17C9, _17D0[0x01], _17D2);
        // 玩家防御增减益
        *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800+0x0002) = _17C9;
        _17CD += 0x20;
    }
    _84CD:
    if (_17D0[0x02])
    {
        _84E1:
        _17C7 = _00223988(_17C7, _17D0[0x02], _17D2);
        // 玩家身法增减益
        MCU_memory[_17CF*0x0005+0x1800+0x0004] = (UINT8)_17C7;
        _17CD += 0x10;
    }
    _8595:
    __8E27(_17CF, _17CD, 0x01, _17D3);
}

void _002235D8(UINT8 _17CF, UINT8* _17D0, UINT8 _17D2, UINT8 _17D3)
{
    UINT8 _17CD = 0x00;
    UINT16 _17CB = 0x0000;
    UINT16 _17C9 = 0x0000;
    UINT16 _17C7 = 0x0000;
    if (_17D0[0x00])
    {
    _8620:
        // 敌人角色攻击
        _17CB = *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x0E);
        _17CB = _00223988(_17CB, _17D0[0x00], _17D2);
        _17CD += 0x40;
    }
    _86CA:
    if (_17D0[0x01])
    {
    _86DE:
        // 敌人角色防御
        _17C9 = *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x10);
        _17C9 = _00223988(_17C9, _17D0[0x01], _17D2);
        _17CD += 0x20;
    }
    _8788:
    if (_17D0[0x02])
    {
    _879C:
        // 敌人角色身法
        _17C7 = MCU_memory[_17CF*0x0033+0x1826+0x0012+0x01];
        _17C7 = _00223988(_17C7, _17D0[0x02], _17D2);
        _17CD += 0x10;
    }
    _8844:
    __8E27(_17CF, _17CD, 0x02, _17D3);
    // 敌人攻击增减益
    *(UINT16*)(MCU_memory+_17CF*0x0005+0x180F) = _17CB;
    // 敌人防御增减益
    *(UINT16*)(MCU_memory+_17CF*0x0005+0x180F+0x0002) = _17C9;
    // 敌人身法增减益
    MCU_memory[_17CF * 0x0005 + 0x180F + 0x0004] = (UINT8)_17C7;
}

// 根据符号_17D3，返回真实_17D0值
UINT16 _00223988(UINT16 _17D0, UINT8 _17D2, UINT8 _17D3)
{
    const float _8ADE = 100.0;  // 0x42C80000
    float _17CC = (float)_17D0;
    _17CC = _17CC*(float)_17D2/_8ADE;
    if (_17D3 == '-')
    {
        _8A06:
        _17D0 = 0x0000-_17D0;
    }
    _8A29:
    return _17D0;
}

UINT8 _00224000()
{
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C4[0x06];
    if (MCU_memory[0x1935])
    {
        _5018:
        return 0xFF;
    }
    _501D:
    fillmem(_17C4, 0x0006, 0xFF);
    __8E0C(_17C4);
    __8E54();
    for (UINT8 _17CD=0x00; _17CD<0x06; _17CD++)
    {
        _50AF:
        _17CC = _17C4[_17CD];
        if (_17CC == 0xFF)
        {
            _50DA:
            continue;
        }
        _50DD:
        if (_17CC < 0x80)
        {
            _50E9:
            _17CA = 0x01;
        }
        else
        {
            _50F2:
            _17CA = 0x02;
        }
        _50F8:
        _17CB = _00226B9F(_17CA);
        if (_17CB)
        {
            _510C:
            return _17CB;
        }
        _5113:
        _17CB = __8E0F(_17CC);
        if (_17CB)
        {
            _512F:
            return _17CB;
        }
        _5136:
        if (_17CC < 0x80)
        {
            _5142:
            // 角色资源ID                                           hp
            if (MCU_memory[_17CC*0x0019+0x1988+0x01] == 0x0000 || *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CC*0x0019+0x1988+0x11)+0x08) == 0x0000)
            {
                _51CF:
                continue;
            }
            _51D2:
            __8E18(_17CC);
            if (MCU_memory[_17CC * 0x0007 + 0x18EC +0x06] == 0xFF)
            {
                _5229:
                if (MCU_memory[_17CC * 0x0007 + 0x18EC +0x04] != 0xFF)
                {
                    _526E:
                    __8E1E(_17CC, _17CA);
                }
                else
                {
                    _5297:
                    _0022447C(_17CC);
                }
            }
            _529E:
            if (MCU_memory[_17CC+0x1919])
            {
                _52BE:
                _00227A7C();
                return 0x03;
            }
        }
        else
        {
            _52C9:
            _17CC -= 0x80;
            // 敌人角色资源ID
            if (MCU_memory[_17CC*0x0033+0x1826+0x01] == 0x00)
            {
                _530B:
                continue;
            }
            _530E:
            if (MCU_memory[_17CC * 0x0007 + 0x1901 +0x06] == 0xFF)
            {
                _5356:
                if (MCU_memory[_17CC * 0x0007 + 0x1901 +0x04] != 0xFF)
                {
                    _539B:
                    __8E1E(_17CC, _17CA);
                }
                else
                {
                    _53C4:
                    _00224635(_17CC);
                }
            }
        }
        _53CB:
        if (MCU_memory[0x1935])
        {
            _53D3:
            return 0xFF;
        }
        _53D8:
        __8E21(_17CC, _17CA);
        __8E1B(_17CC, _17CA);
    }
    _5427:
    _17CB = _00226B9F(0x02);
    if (_17CB)
    {
        _5439:
        return _17CB;
    }
    _5440:
    _17CB = _00226B9F(0x01);
    if (_17CB)
    {
        _5452:
        return _17CB;
    }
    _5459:
    __8E24();
    return 0x00;
}

void _0022447C(UINT8 _17CF)
{
    switch (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)])
    {
    case 0x0001: // _54C7
    case 0x0006: // _54C7
        __8E5D(0x01, _17CF);
        _00226400(_17CF);
        break;
    case 0x0002: // _54F5
        __8E5D(0x02, _17CF);
        _00224E75(_17CF);
        break;
    case 0x0003: // _5523
        __8E5D(0x03, _17CF);
        _00226D37(_17CF);
        break;
    case 0x0004: // _5551
        __8E5D(0x03, _17CF);
        _00224E75(_17CF);
        break;
    case 0x0007: // _557F
        __8E15(_17CF);
        __8E5D(0x07, _17CF);
        break;
    case 0x0008: // _55B5
        __8E5D(0x08, _17CF);
        __8E3C(_17CF);
        break;
    case 0x0009: // _55EB
        __8E5D(0x09, _17CF);
        __8E3F(_17CF);
        break;
    case 0x0005: // _5621
    case 0x00AA: // _5621
    defalut: // _5621
        break;
    }
}

void _00224635(UINT8 _17CF)
{
    UINT8 _17CD;
    _0022475F(_17CF);
    _0022773B();
    if (0x0001 == MCU_memory[*(UINT16*)(MCU_memory+0x1824)])
    {
        _568D:
        __8E69(0x01, _17CF);
        _002264FA(_17CF);
    }
    else if (0x0002 == MCU_memory[*(UINT16*)(MCU_memory+0x1824)])
    {
        _56BB:
        _002259AC(_17CF);
    }
    _56C5:
    _17CD = 0x00;
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _56EC:
        if (MCU_memory[_17CE+0x1916])
        {
            _570C:
            MCU_memory[_17CE+0x1916] = 0x00;
            _17CD = 0x01;
        }
    }
    _573A:
    if (_17CD)
    {
        _5743:
        __8E24();
    }
}

void _0022475F(UINT8 _17CF)
{
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8* _17C8;
    UINT8 _17C7;
    UINT16 _17C5;
    UINT16 _17C3;
    UINT16 _17C1;
    _17C5 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    // 敌人角色智商
    _17C7 = 0x64-MCU_memory[_17CF*0x0033+0x1826+0x0012+0x03];
    // 敌人角色mp
    _17C3 = *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x0C);
    // 敌人角色魔法链数据
    _17C1 = *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x31);
    // 敌人角色学会
    _17CA = MCU_memory[_17CF*0x0033+0x1826+0x02];
    _17C5 %= 0x0064;
    if (_17C5 < _17C7)
    {
        _58F8:
        MCU_memory[*(UINT16*)(MCU_memory+0x1824)] = 0x01;
    }
    else
    {
        _5913:
        if (_17C1 != 0x0000 && _17CA)
        {
            _5937:
            _17CB = _17C5%_17CA;
            _17C8 = __8F0A(DAT_MGICRES, MCU_memory[_17CB * 0x0002 + _17C1], MCU_memory[_17CB*0x0002+_17C1+0x01]);
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)] = 0x02;
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003] = MCU_memory[_17CB * 0x0002 + _17C1];
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003+0x0001] = MCU_memory[_17CB * 0x0002 + _17C1 + 0x01];
            // 复制法术属性
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x1824) + 0x0003, _17C8, 0x001A);
            if (_17C3 < MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003+0x04] || MCU_memory[_17CF*0x0007+0x1901+0x05] != 0xFF)
            {
                _5BAF:
                MCU_memory[*(UINT16*)(MCU_memory+0x1824)] = 0x01;
            }
            else
            {
                _5BCA:
                if (MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003+0x03] >= 0x80)
                { // 法术全体效果，且持续回合>0
                    _5BED:
                    MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0001] = 0xAA;
                    return;
                }
            }
        }
        else
        {
            _5C18:
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)] = 0x01;
        }
    }
    _5C30:
    if (MCU_memory[*(UINT16*)(MCU_memory+0x1824)] == 0x01 || MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003] == 0x01)
    {
        _5C69:
        _17CC = _17C5%0x0003;
        // 角色资源ID                               hp
        if (MCU_memory[_17CC*0x0019+0x1988+0x01] && *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CC*0x0019+0x1988+0x11)+0x08) != 0x0000)
        {
            _5D12:
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0001] = _17CC;
        }
        else
        {
            _5D3C:
            __8E63(0x2B, &_17CC);
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0001] = _17CC;
        }
    }
    else
    {
        _5D93:
        _17CC = _17C5%0x0003;
        // 敌人角色资源ID
        if (MCU_memory[_17CC*0x0033+0x1826+0x01])
        {
            _5DE6:
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0001] = _17CC;
        }
        else
        {
            _5E10:
            __8E66(0x2B, &_17CC);
            MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0001] = _17CC;
        }
    }
}

void _00224E75(UINT8 _17CF)
{
    UINT8 _17CA;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT16 _17B8[0x04];
    UINT8* _17B6;
    UINT16 _17B0[3];
    UINT16 _17AA[3];
    UINT8* _17A8;
    if (MCU_memory[0x1935])
    {
        _5E90:
        return;
    }
    _5E93:
    _17CA = 0x00;
    _17C4 = 0x00;
    _17C3 = 0x00;
    _17C1 = 0x00;
    _17C0 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    _17A8 = MCU_memory +*(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x0003;
    // mp
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x000C) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0C)- _17A8[0x04];
    _17B6 = _17A8+0x0012;
    _SysMemcpy((UINT8*)_17B8, _17B6, 0x0008);
    _17C6 = _17A8[0];
    switch (_17C6)
    {
    case 0x0001: // _60DE
        _17C2 = 0x02;
        _17C4 = 0x01;
        break;
    case 0x0003: // _60ED
    case 0x0002: // _60ED
    case 0x0004: // _60ED
        _17C2 = 0x01;
        _17C3 = 0x01;
        break;
    case 0x0005: // _60FC
        _17C2 = 0x02;
        _17CA = 0x01;
        break;
    }
    _610B:
    __8E33();
    _17C5 = _17A8[0x05];
    if (_17C5 > 0x00C8)
    {
        _6149:
        _17C8 = 0x00;
        _17C7 = 0x00;
    }
    else
    {
        _6158:
        __8E7B(_17C0, &_17C8, &_17C7, _17C2);
    }
    _61CA:
    __8DE5(0x02, _17C5, 0x02, _17C8, _17C7);
    if (_17CA)
    {
        _620A:
        __8EA2(_17CF);
        return;
    }
    else if (_17C4)
    {
        _6225:
        _17C3 = __8E2D(_17CF, _17B8, _17B0, _17AA);
        if (_17C0 != 0xAA)
        {
            _6286:
            _17B0[0] = _17B0[_17C0];
            _17AA[0] = _17AA[_17C0];
        }
        _6340:
        if (_17B0[0] || _17B0[1] || _17B0[2])
        {
            _6391:
            __8E72(0x2D, _17B0, _17C0, 0x02, 0x00);
        }
        _63CF:
        if (_17AA[0] || _17AA[1] || _17AA[2])
        {
            _6420:
            __8E72(0x2D, _17AA, _17C0, 0x02, 0x00);
        }
        _645E:
        if (_17C0 == 0xAA)
        {
            _646A:
            for (UINT8 _17C9=0x00; _17C9<0x03; _17C9++)
            {
                _648B:
                if (_17B0[_17C9] == 0xFFFF)
                {
                    _64C2:
                    _17B0[_17C9] = 0x0000;
                }
                _6501:
                if (_17AA[_17C9] == 0xFFFF)
                {
                    _6538:
                    _17AA[_17C9] = 0x0000;
                }
                _6577:
                _17B8[0x00] = _17B0[_17C9];
                _17B8[0x01] = _17AA[_17C9];
                if (__8E81(_17C6, _17B8, _17C9) != 0x00)
                {
                    _6671:
                    if (_17C1 == 0x00)
                    {
                        _667A:
                        _17C1 = 0x01;
                    }
                }
            }
        }
        else
        {
            _6686:
            if (_17B0[_17C0] == 0xFFFF)
            {
                _66BD:
                _17B0[_17C0] = 0x0000;
            }
            _66FC:
            if (_17AA[_17C0] == 0xFFFF)
            {
                _6733:
                _17AA[_17C0] = 0x0000;
            }
            _6772:
            _17B8[0x00] = _17B0[_17C0];
            _17B8[0x01] = _17AA[_17C0];
            _17C1 = __8E39(_17C6, _17B8, _17C0);
        }
        _6868:
        if (_17C3)
        {
        _6871:
            __8E30(_17CF, _17B0, _17AA);
            _17C3 = 0x00;
        }
    }
    _68B6:
    if (_17C3)
    {
        _68BF:
        if (_17C6 != 0x04 && _17B8 != NULL)
        {
            _68E6:
            __8E72(0x2B, _17B8, _17C0, 0x01, 0x01);
        }
        _6924:
        __8E36(_17C6, _17B8, _17C0);
    }
    _695A:
    if (_17C0 == 0xAA)
    {
        _6966:
        __8E54();
    }
    else
    {
        _6974:
        __8E57(_17C0);
    }
    _6983:
    if (_17C1)
    {
        _698C:
        __8E5A(_17C0);
    }
}

void _002259AC(UINT8 _17CF)
{
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT16 _17BA[0x04];
    UINT8* _17B8;
    UINT16 _17B2[3];
    UINT16 _17AC[3];
    _17C3 = 0x00;
    _17C2 = 0x00;
    _17C5 = MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x01];
    _17B8 = MCU_memory + *(UINT16*)(MCU_memory+0x1824)+0x0003+0x0012;
    _SysMemcpy((UINT8*)_17BA, _17B8, 0x0008);
    // 敌人角色mp
    *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x000C) = *(UINT16*)(MCU_memory + _17CF * 0x0033 + 0x1826 + 0x0012 + 0x0C)- MCU_memory[*(UINT16*)(MCU_memory + 0x1824) + 0x0003 + 0x04];
    _17C7 = MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003];
    switch (_17C7)
    {
    case 0x0001: // _6BB0
        _17C4 = 0x01;
        _17C3 = 0x01;
        break;
    case 0x0003: // _6BBF
    case 0x0002: // _6BBF
    case 0x0004: // _6BBF
        _17C4 = 0x02;
        _17C2 = 0x01;
        break;
    case 0x0005: // _6BCE
        break;
    }
    _6BD1:
    __8E69(0x02, _17CF);
    __8E33();
    __8E7B(_17C5, &_17C9, &_17C8, _17C4);
    _17C6 = MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x0003+0x05];
    __8DE5(0x02, _17C6, 0x02, _17C9, _17C8);
    if (_17C3)
    {
        _6CD1:
        _17C2 = __8E4B(_17CF, _17BA, _17B2, _17AC);
        if (_17C5 != 0xAA)
        {
            _6D32:
            _17B2[0] = _17B2[_17C5];
            _17AC[0] = _17AC[_17C5];
        }
        _6DEC:
        if (_17B2[0] || _17B2[1] || _17B2[2])
        {
            _6E3D:
            __8E72(0x2D, _17B2, _17C5, 0x01, 0x00);
        }
        _6E7B:
        if (_17AC[0] || _17AC[1] || _17AC[2])
        {
            _6ECC:
            __8E72(0x2D, _17AC, _17C5, 0x01, 0x00);
        }
        _6F0A:
        if (_17C5 == 0xAA)
        {
            _6F16:
            for (UINT8 _17CA=0x00; _17CA<0x03; _17CA++)
            {
                _6F37:
                if (_17B2[_17CA] == 0xFFFF)
                {
                    _6F6E:
                    _17B2[_17CA] = 0x0000;
                }
                _6FAD:
                if (_17AC[_17CA] == 0xFFFF)
                {
                    _6FE4:
                    _17AC[_17CA] = 0x0000;
                }
                _701A:
                _17BA[0x00] = _17B2[_17CA];
                _17BA[0x01] = _17AC[_17CA];
                __8E7E(_17C7, _17BA, _17CA);
            }
            _710D:
            __8E54();
        }
        else
        {
            _711B:
            if (_17B2[_17C5] == 0xFFFF)
            {
                _7152:
                _17B2[_17C5] = 0x0000;
            }
            _7191:
            if (_17AC[_17C5] == 0xFFFF)
            {
                _71C8:
                _17AC[_17C5] = 0x0000;
            }
            _71FE:
            _17BA[0x00] = _17B2[_17C5];
            _17BA[0x01] = _17AC[_17C5];
            __8E36(_17C7, _17BA, _17C5);
            __8E57(_17C5);
        }
        _72FD:
        if (_17C2)
        {
            _7306:
            __8E4E(_17CF, _17B2, _17AC);
            _17C2 = 0x00;
        }
    }
    _734B:
    if (_17C2)
    {
        _7354:
        if (_17C7 != 0x04)
        {
            _7360:
            if (_17BA[0x00] != 0x0000)
            {
                _737B:
                __8E72(0x2B, _17BA, _17C5, 0x02, 0x01);
            }
        }
        _73B9:
        __8E39(_17C7, _17BA, _17C5);
    }
}

void _00226400(UINT8 _17CF)
{
    if (MCU_memory[0x1935])
    {
        _741B:
        return;
    }
_741E:
    //                  玩家角色攻击                                                                          玩家攻击增减益
    _002270A5(_17CF, *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0E)+ *(UINT16*)(MCU_memory + _17CF * 0x0005 + 0x1800));
}

void _002264FA(UINT8 _17CF)
{
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT16 _17C6; // todo 应该和上面的变量组成结构体
    UINT16 _17C4;
    UINT16 _17C2;
    UINT16 _17C0;
    UINT16 _17BE;
    UINT16 _17BC;
    if (MCU_memory[0x1935])
    {
        _7515:
        return;
    }
    _7518:
    if (_17CF > 0x03)
    {
        _7524:
        return;
    }
    _752C:
    _17CC = MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x01];
    // 指向角色属性
    _17BC = *(UINT16*)(MCU_memory+_17CC*0x0019+0x1988+0x11);
    _17BE = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    // 玩家身法=玩家身法增减益+玩家角色身法+50
    _17C8 = MCU_memory[_17CC*0x0005+0x1800+0x04]+ MCU_memory[_17BC + 0x16] + 0x32;
    // 敌人身法=敌人身法增减益+敌人角色身法
    _17C9 = MCU_memory[_17CF*0x0005+0x180F+0x04]+ MCU_memory[_17CF * 0x0033 + 0x1826 + 0x0012 + 0x01];
    // 如果玩家身法>敌人身法
    if (_17C8 > _17C9)
    {
        _7697:
        _17C8 = _17C8-_17C9;
    }
    else
    {
        _76A7:
        _17C8 = 0x0A;
    }
    _76AD:
    if (_17BE%0x00C8 < _17C8) // MISS
    {
        _76D5:
        _17C6 = 0xFFFF;
        __8E72(0x2D, &_17C6, _17CC, 0x01, 0x01);
    }
    else
    {
        _7737:
        // 敌人攻击=敌人角色攻击+敌人攻击增减益
        _17C4 = *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x0E) + *(UINT16*)(MCU_memory + _17CF * 0x0005 + 0x180F);
        // 玩家防御=玩家角色防御+玩家防御增减益
        _17C2 = *(UINT16*)(MCU_memory+_17BC+0x10) + *(UINT16*)(MCU_memory+_17CC*0x0005+0x1800+0x02);
        // 玩家hp
        _17C0 = *(UINT16*)(MCU_memory+_17BC+0x08);
        _17C6 = _17C4/((_17C2>>0x0003)+0x0001);
        _17C6 += _17BE%((_17C4>>0x0004)+0x0001);
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CC<<1)+0x181E)] == 0x05 || MCU_memory[_17CC+0x1916])
        {
            _7964:
            _17C6 >>= 0x0001;
        }
    _7985:
        // 如果最终伤害>玩家hp
        if (_17C6 > _17C0)
        {
        _79A8:
            // hp
            *(UINT16*)(MCU_memory+_17BC+0x0008) = 0x0000;
        }
        else
        {
        _79DE:
            // hp=hp-最终伤害
            *(UINT16*)(MCU_memory+_17BC+0x0008) = *(UINT16*)(MCU_memory + _17BC + 0x08) - _17C6;
        }
        _7A47:
        __8E57(_17CC);
        __8E72(0x2D, &_17C6, _17CC, 0x01, 0x01);
        // 敌人角色普通攻击可产生[bit3:毒,bit2:乱,bit1:封,bit0:眠]
        _17CB = MCU_memory[_17CF*0x0033+0x1826+0x04]&0x0F;
        if (_17CB)
        {
            _7AEB:
            // 敌人角色普通攻击产生的效果持续回合
            _17CA = MCU_memory[_17CF*0x0033+0x1826+0x0012+0x05];
            if (__8E51(_17CC, 0x01) != 0x00)
            {
                _7B5C:
                __8E27(_17CC, _17CB, 0x01, _17CA);
            }
        }
    }
}

UINT8 _00226B9F(UINT8 _17CF)
{
    UINT8 _17CD;
    if (MCU_memory[0x1935])
    {
        _7BBA:
        return 0xFF;
    }
    _7BBF:
    if (_17CF == 0x02)
    {
        _7BCB:
        _17CD = 0x03;
        for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
        {
            _7BF2:
            // 角色资源ID                                       hp
            if (MCU_memory[_17CE*0x0019+0x1988+0x01] == 0x00 || *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CE*0x0019+0x1988+0x11)+0x08) == 0x0000)
            {
                _7C7F:
                _17CD--;
            }
        }
        _7C8B:
        if (_17CD)
        {
            _7C94:
            return 0x00;
        }
        else
        {
            _7C9C:
            return 0x02;
        }
    }
    else
    {
        _7CA4:
        _17CD = 0x03;
        for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
        {
            _7CCB:
            // 敌人角色资源ID
            if (MCU_memory[_17CE*0x0033+0x1826+0x01] == 0x00)
            {
                _7D02:
                _17CD--;
            }
        }
        _7D0E:
        if (_17CD)
        {
            _7D17:
            return 0x00;
        }
        else
        {
            _7D1F:
            return 0x01;
        }
    }
}

void _00226D37(UINT8 _17CF)
{
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT16 _17C2;
    UINT16 _17C0;
    if (MCU_memory[0x1935])
    {
        _7D52:
        return;
    }
    _7D55:
    _17C7 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    __8E7B(_17C7, &_17C9, &_17C8, 0x02);
    __8E33();
    //          角色资源ID
    _17C4 = (MCU_memory[_17CF*0x0019+0x1988+0x01]-0x01)<<0x01;
    _17C4 %= 0x0F;
    _17C4 += 0x00F0;
    if (_17C7 == 0xAA)
    {
        _7E81:
        _17C4++;
    }
    _7E8A:
    __8DE5(0x02, _17C4, 0x02, _17C9, _17C8);
    _17C2 = 0x0000;
    _17C5 = 0x00;
    _17CA = _17CF;
    while (_17C5 < 0x03)
    {
        _7EE4:
        _17C5++;
        // 角色资源ID
        if (MCU_memory[_17CA*0x0019+0x1988+0x01] == 0x00)
        {
            _7F24:
            continue;
        }
        _7F27:
        if (_17CA > 0x02)
        {
            _7F38:
            _17CA = 0x00;
        }
    _7F3E:
        // 攻击
        _17C0 = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CA * 0x0019 + 0x1988 + 0x11) + 0x0E);
        _17C6 = _17CA-_17CF;
        if (_17C6 > 0xF0)
        {
            _7FAD:
            _17C6 = 0x03-_17C6;
        }
        _7FB8:
        _17C0 >>= _17C6;
        _17C2 += _17C0;
        //          玩家攻击增减益
        _17C2 += (*(UINT16*)(MCU_memory+_17CA*0x0005+0x1800)>>_17C6);
        _17CA++;
    }
    _806F:
    _002270A5(_17CF, _17C2);
}

void _002270A5(UINT8 _17CF, UINT16 _17D0)
{
    UINT8 _17CB;
    UINT16 _17C4[3];
    UINT8 _17CA = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    if (_17CA == 0xAA)
    {
        _80F3:
        for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
        {
            _8114:
            _0022726A(_17CF, _17CC, _17D0, _17C4);
        }
        _8153:
        _17CB = 0x00;
    }
    else
    {
        _815C:
        _0022726A(_17CF, _17CA, _17D0, _17C4);
        _17C4[0] = _17C4[_17CA];
        _17CB = 0x01;
    }
    _81FB:
    __8E72(0x2D, _17C4, _17CA, 0x02, _17CB);
    __8E48(_17CF);
    __8E5A(_17CA);
}

void _0022726A(UINT8 _17CF, UINT8 _17D0, UINT16 _17D1, UINT16* _17D3)
{
    UINT16 _17CB;
    UINT16 _17C9;
    UINT16 _17C7;
    UINT16 _17C5;
    UINT16 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT16 _17BF;
    if (MCU_memory[0x1935])
    {
        _8285:
        return;
    }
    _8288:
    // 敌人角色资源ID
    if (MCU_memory[_17D0*0x0033+0x1826+0x01] == 0x00)
    {
        _82BF:
        return;
    }
    _82C2:
    _17C5 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    // 指向敌人角色等级开始的数据
    _17BF = _17D0 * 0x0033 + 0x1826 + 0x0012;
    // 敌人身法=敌人身法增减益+敌人角色身法+50
    _17C2 = MCU_memory[_17D0*0x0005+0x180F+0x04]+ MCU_memory[_17BF + 0x01] + 0x32;
    // 玩家身法=玩家身法增益+玩家角色身法
    _17C1 = MCU_memory[_17CF*0x0005+0x1800+0x04]+ MCU_memory[*(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x16];
    // 如果敌人身法>玩家身法
    if (_17C2 > _17C1)
    {
        _8431:
        _17C2 -= _17C1;
    }
    else
    {
        _8441:
        _17C2 = 0x0A;
    }
    _8447:
    if (_17C5%0x00C8 < _17C2) // MISS
    {
        _846F:
        _17D3[_17D0] = 0xFFFF;
    }
    else
    {
        _84AB:
        // 敌人防御=敌人角色防御+敌人防御增减益
        _17C9 = *(UINT16*)(MCU_memory+_17BF+0x10) + *(UINT16*)(MCU_memory+_17D0*0x0005+0x180F+0x02);
        // 敌人角色hp
        _17C7 = *(UINT16*)(MCU_memory+_17BF+0x08);
        _17C3 = _17C9>>0x0003;
        if (_17C3 == 0x0000)
        {
            _8582:
            _17C3 = 0x0001;
        }
        _858D:
        _17CB = _17D1/_17C3;
        _17C3 = _17D1>>0x0002;
        if (_17C3 == 0x0000)
        {
            _85ED:
            _17C3 = 0x000A;
        }
        _85F8:
        _17CB += _17C5%_17C3;
        // 如果敌人角色hp>真实伤害
        if (_17C7 > _17CB)
        {
            _864F:
            // 敌人角色hp=敌人角色hp-真实伤害
            *(UINT16*)(MCU_memory+_17BF+0x0008) = *(UINT16*)(MCU_memory + _17BF + 0x08) - _17CB;
        }
        else
        {
            _86BB:
            // 敌人角色hp=0
            *(UINT16*)(MCU_memory+_17BF+0x0008) = 0x0000;
        }
        _86EE:
        _17D3[_17D0] = _17CB;
    }
}

void _0022773B()
{
    UINT8 _17CF = MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x01];
    if (_17CF == 0xAA)
    {
        _8769:
        for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
        {
            _878A:
            _002277CD(_17CE);
            __8E57(_17CE);
        }
    }
    else
    {
        _87A6:
        _002277CD(_17CF);
        __8E57(_17CF);
    }
}

void _002277CD(UINT8 _17CF)
{
    UINT16 _17CC;
    if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x04] != 0xFF)
    {
        _8825:
        return;
    }
    _8828:
    if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x06] != 0xFF)
    {
        _886D:
        return;
    }
    _8870:
    if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] == 0x05)
    {
        _88A3:
        return;
    }
    _88A6:
    _17CC = SysRand((PtrRandEnv)(MCU_memory + 0x1928)) % 0x0064;
    if (_17CC > 0x5A)
    {
        _88EE:
        MCU_memory[_17CF+0x1916] = 0x01;
    }
}

void _00227924()
{
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8* _17CA = __8F0A(DAT_SUNDRYPIC, 0x02, 0x0D);
    _17CD = _17CA[0x02];
    _17CC = _17CA[0x03];
    __8F07(DAT_SUNDRYPIC, 0x02, 0x0D, 0x00, (0x009F - _17CD) >> 0x0001, (0x60 - _17CC) >> 0x01);
    __8E33();
    while (MCU_memory[0x1935] == 0x00)
    {
        _8A1F:
        if (__8DE5(0x01, 0xF9, 0x03, 0x00, 0x00) != 0xFF)
        {
            _8A62:
            break;
        }
    }
    _8A68:
    _00227A7C();
}

void _00227A7C()
{
    UINT16 _17CE;
    for (UINT8 _17CD=0x00; _17CD<0x03; _17CD++)
    {
        _8AAD:
        // 指向角色属性
        _17CE = *(UINT16*)(MCU_memory+_17CD*0x0019+0x1988+0x11);
        // hp
        if (*(UINT16*)(MCU_memory+_17CE+0x08) == 0x0000)
        {
            _8B1A:
            *(UINT16*)(MCU_memory + _17CE + 0x0008) = 0x0001;
        }
    }
}

void _00228000(UINT8 _17CF, UINT8 _17D0)
{
    const UINT8 _84BF[0x24] = { 0x02,0x03,0x03,0x04,0x04,0xFF,0x00,0x00,0x05,0x06,0x07,0xFF,0x00,0x00,0x00,0x0A,0xFF,0xFF,0x00,0x00,0x00,0x05,0xFF,0xFF,0x00,0x00,0x05,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    const UINT8 _84E3[0x24] = { 0x0A,0x03,0x03,0x0A,0x05,0xFF,0x0A,0x0A,0x32,0x50,0x01,0xFF,0x03,0x03,0x03,0x14,0xFF,0xFF,0x0A,0x0A,0x23,0x01,0xFF,0xFF,0x0A,0x0A,0x32,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    const UINT8 _8507[0x6C] = { 0x40,0x40,0x40,0x40,0x40,0xFF,0x3E,0x3C,0x3C,0x3C,0x3C,0xFF,0x42,0x44,0x46,0x46,0xFF,0xFF,0x3D,0x3A,0x38,0x38,0xFF,0xFF,0x3C,0x38,0x38,0x38,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x60,0x60,0x60,0x60,0x60,0xFF,0x5E,0x5C,0x5C,0x5C,0x5C,0xFF,0x63,0x66,0x69,0x6C,0xFF,0xFF,0x5D,0x5A,0x58,0x58,0xFF,0xFF,0x5C,0x58,0x58,0x58,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x80,0x80,0x80,0x80,0xFF,0x7E,0x7C,0x7C,0x7C,0x7C,0xFF,0x82,0x84,0x86,0x86,0xFF,0xFF,0x7E,0x7C,0x7C,0x7C,0x7C,0xFF,0x7C,0x78,0x78,0x78,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    const UINT8 _8573[0x6C] = { 0x34,0x34,0x34,0x34,0x34,0xFF,0x33,0x31,0x31,0x31,0x31,0xFF,0x36,0x38,0x3C,0x3C,0xFF,0xFF,0x33,0x32,0x31,0x31,0xFF,0xFF,0x33,0x31,0x31,0x31,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x30,0x30,0x30,0x30,0x30,0xFF,0x2F,0x2D,0x2D,0x2D,0x2D,0xFF,0x33,0x36,0x39,0x3C,0xFF,0xFF,0x2F,0x2E,0x2D,0x2D,0xFF,0xFF,0x2F,0x2D,0x2D,0x2D,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x28,0x28,0x28,0x28,0x28,0xFF,0x27,0x26,0x26,0x26,0x26,0xFF,0x2A,0x2C,0x2E,0x2E,0xFF,0xFF,0x27,0x26,0x26,0x26,0xFF,0xFF,0x27,0x26,0x26,0x26,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    const UINT8 _85DF[0x6A] = { 0x00,0xE6,0xE2,0xCD,0xD2,0xFF,0x00,0x02,0x02,0xF2,0xF6,0xFF,0x00,0x24,0x24,0x14,0x18,0xFF,0x00,0x02,0x02,0xF2,0xF6,0xFF,0x00,0xFF,0xFD,0xFD,0xFD,0xFD,0x00,0xCD,0xC7,0xB4,0xB8,0xFF,0x00,0xF3,0xED,0xD2,0xD6,0xFF,0x00,0x06,0x06,0xF6,0xFA,0xFF,0x00,0xF3,0xED,0xD2,0xD6,0xFF,0x00,0xFF,0xFD,0xFD,0xFD,0xFD,0x00,0xAA,0xA3,0x91,0x95,0xFF,0x00,0xD3,0xCC,0xB6,0xBA,0xFF,0x00,0xE6,0xE2,0xD5,0xD9,0xFF,0x00,0xD3,0xCC,0xB6,0xBA,0xFF,0x00,0xFF,0xFD,0xFD,0xFD,0xFD,0x00,0xF6,0xF2,0xE6,0xEA,0xFF,0x00,0xE8,0xE8,0xE0,0xE6,0xFF,0x00,0xEC,0xEC,0xDD };
    const UINT8 _8639[0x6A] = { 0x00,0xF6,0xF2,0xE6,0xEA,0xFF,0x00,0xE8,0xE8,0xE0,0xE6,0xFF,0x00,0xEC,0xEC,0xDD,0xE0,0xFF,0x00,0xE8,0xE8,0xE0,0xE6,0xFF,0x00,0xFF,0xFD,0xFD,0xFD,0xFD,0x00,0xFA,0xF6,0xEA,0xED,0xFF,0x00,0xF0,0xEC,0xE2,0xE6,0xFF,0x00,0xE4,0xE4,0xDB,0xE2,0xFF,0x00,0xF0,0xEC,0xE2,0xE6,0xFF,0x00,0xFF,0xFD,0xFD,0xFD,0xFD,0x00,0xFD,0xFB,0xF2,0xF6,0xFF,0x00,0xF3,0xEF,0xEB,0xEF,0xFF,0x00,0xEB,0xE7,0xE0,0xE4,0xFF,0x00,0xF3,0xEF,0xEB,0xEF,0xFF,0x00,0xFF,0xFD,0xFD,0xFD,0xFD,0x01,0x02,0x04,0x07,0x01,0x00,0x02,0x00,0x03,0x00,0x04,0x00,0x07,0x00,0x08,0x00 };
    const UINT8 _8693[] = { 0x01,0x02,0x04 };
    const UINT8 _8696 = 0x07;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    if (MCU_memory[0x1935])
    {
        _501B:
        return;
    }
    _501E:
    _17C2 = 0x00;
    SysTimer1Close();
    SysTimer1Open(0x01);
    _17C3 = 0x00;
    _17C4 = _8693[_17D0];
    _17C6 = MCU_memory[*(UINT16*)(MCU_memory+(_17D0<<1)+0x181E)+0x01];
    // 角色资源ID
    _17C5 = MCU_memory[_17D0*0x0019+0x1988+0x01];
    switch (_17CF)
    {
    case 0x0001: // _50E7
        if (_17C6 == 0xAA)
        {
            _50F3:
            _17C6 = 0x03;
        }
        _50F9:
        _17CF = 0x00;
        _17C7 = 0x05;
        break;
    case 0x0002: // _5108
        _17CF = 0x01;
        _17C2 = MCU_memory[*(UINT16*)(MCU_memory+(_17D0<<1)+0x181E)+0x0003+0x05];
        if (_17C2 > 0x00C8)
        {
            _5169:
            _17C7 = 0x03;
            _17C3 = 0x01;
        }
        else
        {
            _5178:
            _17C7 = 0x05;
        }
        break;
    case 0x0003: // _5181
        _00228694(_17D0);
        return;
        break;
    case 0x0004: // _518B
        _17C2 = MCU_memory[*(UINT16*)(MCU_memory+(_17D0<<1)+0x181E)+0x0003+0x01];
        if (_17C2 > 0x00E6)
        {
            _51E6:
            _17CF = 0x01;
            _17C7 = 0x03;
        }
        else
        {
            _51F8:
            _00228694(_17D0);
            return;
        }
        break;
    case 0x0007: // _5202
        if (MCU_memory[_17D0+0x1919])
        {
            _5222:
            _00228C7F();
            return;
        }
        else
        {
            _5228:
            _17CF = 0x02;
            _17C7 = 0x04;
        }
        break;
    case 0x0008: // _5237
        _17CF = 0x03;
        _17C7 = 0x04;
        break;
    case 0x0009: // _5246
        _17CF = 0x04;
        _17C7 = 0x04;
        break;
    case 0x0005: // _5255
        _17CF = 0x05;
        break;
    }
    _525E:
    _0022AB72(_17C4, 0x00);
    __8E33();
    for (UINT8 _17CC=0x00; _17CC<_17C7; _17CC++)
    {
        _52A8:
        _17C9 = _84E3[_17CC+ _17CF * 0x0006];
        _17C8 = _84BF[_17CC+ _17CF * 0x0006];
        _17CB = _8507[_17CC+ _17D0 * 0x0024+ _17CF * 0x0006];
        _17CA = _8573[_17CC+ _17D0 * 0x0024+ _17CF * 0x0006];
        if (_17CF == 0x00)
        {
            _544D:
            _17CB += _85DF[_17CC+ _17D0 * 0x001E + _17C6 * 0x0006];
            _17CA += _8639[_17CC+ _17D0 * 0x001E + _17C6 * 0x0006];
        }
        _5555:
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
        __8F07(DAT_SUNDRYPIC, 0x03, _17C5, _17C8, _17CB, _17CA);
        if (MCU_memory[0x1935])
        {
            _55D1:
            return;
        }
        _55D4:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        __8DD3(_17C9, 0x00);
    }
    _5623:
    if (_17C2 > 0x00E6 || _17C3)
    {
        _5648:
        _0022AB72(_8696, 0x00);
    }
    _566B:
    SysTimer1Close();
    SysTimer1Open(0x32);
}

void _00228694(UINT8 _17CF)
{
    const UINT16 _86B7 = 0x0000;
    const UINT16 _86D2 = 0x0303;
    const UINT16 _86DB = 0x4842;
    const UINT16 _872C = 0x2C32;
    UINT8 _17CA;
    UINT16 _17C2[3];
    UINT16 _17BC[3];
    UINT16 _17B6[3];
    UINT16 _17B4;
    UINT8 _17B3;
    for (UINT8 _17C9=0x00; _17C9<0x03; _17C9++)
    {
        _56C8:
        _17CA = _17C9+_17CF;
        if (_17CA > 0x02)
        {
            _56E6:
            _17CA %= 0x03;
        }
        _56F5:
        // 角色资源ID
        if (MCU_memory[_17CA*0x0019+0x1988+0x01])
        {
        _572C:
            // hp
            if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CA*0x0019+0x1988+0x11)+0x08) != 0x0000)
            {
                _5782:
                _17B6[_17CA] = _17C9 * 0x0009 + _86B7;
                _17B4 = _86D2;
                _17C2[_17CA] = _17CF * 0x001B + _86DB + _17C9 * 0x0009;
                _17BC[_17CA] = _17CF * 0x001B + _872C + _17C9 * 0x0009;
            }
        }
    }
    _5945:
    _0022AB72(0x07, 0x00);
    __8E33();
    for (UINT8 _17C9=0x00; _17C9<0x09; _17C9++)
    {
        _598B:
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
        for (UINT8 _17C8=0x00; _17C8<0x03; _17C8++)
        {
            _59E2:
            // 角色资源ID
            if (MCU_memory[_17C8*0x0019+0x1988+0x01])
            {
            _5A19:
                // hp
                if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C8*0x0019+0x1988+0x11)+0x08) != 0x0000)
                {
                    _5A6F:
                    // 角色资源ID
                    _17B3 = MCU_memory[_17C8*0x0019+0x1988+0x01];
                    __8F07(DAT_SUNDRYPIC, 0x03, _17B3,
                        MCU_memory[_17C9 + _17B6[_17C8]],
                        MCU_memory[_17C9 + _17C2[_17C8]],
                        MCU_memory[_17C9 + _17BC[_17C8]]);
                }
            }
            _5BD7:
            if (MCU_memory[0x1935])
            {
                _5BDF:
                return;
            }
        }
        _5BE5:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        __8DD3(MCU_memory[_17C9 + _17B4], 0x00);
    }
    _5C56:
    SysTimer1Close();
    SysTimer1Open(0x32);
}

void _00228C7F()
{
    const UINT16 _877D = 0x4241;
    const UINT16 _878F = 0x3C38;
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17C9;
    _0022AB72(0x07, 0x00);
    __8E33();
    for (UINT8 _17CB=0x00; _17CB<0x06; _17CB++)
    {
        _5CD5:
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
        for (UINT8 _17CA=0x00; _17CA<0x03; _17CA++)
        {
            _5D2C:
            // 角色资源ID
            if (MCU_memory[_17CA*0x0019+0x1988+0x01])
            {
            _5D63:
                // hp
                if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CA*0x0019+0x1988+0x11)+0x08) != 0x0000)
                {
                    _5DB9:
                    // 角色资源ID
                    _17C9 = MCU_memory[_17CA*0x0019+0x1988+0x01];
                    _17CD = MCU_memory[_17CB+ _17CA * 0x0006 + _877D];
                    _17CC = MCU_memory[_17CB+ _17CA * 0x0006 + _878F];
                    if (_17CD > 0x008C || _17CC > 0x4B)
                    {
                        _5EBA:
                        continue;
                    }
                    _5EBD:
                    __8F07(DAT_SUNDRYPIC, 0x03, _17C9, 0x00, _17CD, _17CC);
                }
            }
            _5EF9:
            if (MCU_memory[0x1935])
            {
                _5F01:
                return;
            }
        }
        _5F07:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        __8DD3(0x05, 0x00);
    }
    _5F54:
    __8DD3(0x1E, 0x00);
    SysTimer1Close();
    SysTimer1Open(0x32);
}

void _00228F9F(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3)
{
    UINT8 _17CE;
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _5FBA:
        return;
    }
    _5FBD:
    if (_17D1 == 0x02)
    {
        _5FC9:
        // 敌人角色资源ID
        if (MCU_memory[_17D0*0x0033+0x1826+0x01] == 0x00)
        {
            _6000:
            return;
        }
        _6003:
        _17CE = DAT_ACTORPIC;
        _17CD = 0x03;
        // 敌人角色魔法链
        _17CC = MCU_memory[_17D0*0x0033+0x1826+0x05];
        _17CB = 0x00;
    }
    else
    {
        _604E:
        // 角色资源ID
        if (MCU_memory[_17D0*0x0019+0x1988+0x01] == 0x00)
        {
            _6085:
            return;
        }
        _6088:
        _17CE = DAT_SUNDRYPIC;
        _17CD = 0x03;
        // 角色资源ID
        _17CC = MCU_memory[_17D0*0x0019+0x1988+0x01];
        _17CB = MCU_memory[*(UINT16*)(MCU_memory+(_17D0<<1)+0x181E)+0x02];
        if (_17CF == 0x2D)
        {
        _6105:
            // hp
            if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17D0*0x0019+0x1988+0x11)+0x08) == 0x0000)
            {
                _615B:
                _17CB = 0x0B;
            }
            else
            {
                _6164:
                if (MCU_memory[_17D0+0x1916])
                {
                    _6184:
                    _17CB = 0x08;
                }
                else
                {
                    _618D:
                    if (MCU_memory[*(UINT16*)(MCU_memory+(_17D0<<1)+0x181E)] == 0x05)
                    {
                        _61C0:
                        _17CB = 0x08;
                    }
                    else
                    {
                        _61C9:
                        _17CB = 0x09;
                    }
                }
            }
        }
    }
    _61CF:
    __8F07(_17CE, _17CD, _17CC, _17CB, _17D2, _17D3);
}

UINT8 _00229222(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _623D:
        return 0xFF;
    }
    _6242:
    _17CC = 0x00;
    _17CB = 0x00;
    _17CD = *_17D0;
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _6282:
        if (_17CF == 0x2D)
        {
            _628E:
            _17CD--;
            if (_17CD > 0xF0)
            {
                _62A8:
                _17CD = 0x02;
            }
            _62AE:
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01])
            {
                _62E5:
                *_17D0 = _17CD;
                return 0x00;
            }
        }
        else if (_17CF == 0x2B)
        {
            _6314:
            _17CD++;
            if (_17CD > 0x02)
            {
                _632E:
                _17CD = 0x00;
            }
            _6334:
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01])
            {
                _636B:
                *_17D0 = _17CD;
                return 0x00;
            }
        }
        else
        {
            _638E:
            _17CD = *_17D0+_17CE;
            _17CD %= 0x03;
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01])
            {
                _63EC:
                _17CB++;
                if (_17CB == 0x01)
                {
                    _6401:
                    _17CC = _17CD;
                }
            }
        }
    }
    _640C:
    *_17D0 = _17CC;
    if (_17CB == 0x00)
    {
        _6430:
        return 0xAA;
    }
    return _17CB;
}

UINT8 _00229452(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _646D:
        return 0xFF;
    }
    _6472:
    _17CC = 0x00;
    _17CB = 0x00;
    _17CD = *_17D0;
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _64B2:
        if (_17CF == 0x2D)
        {
            _64BE:
            _17CD--;
            if (_17CD > 0xF0)
            {
                _64D8:
                _17CD = 0x02;
            }
            _64DE:
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01])
            {
            _6515:
                // hp
                if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CD*0x0019+0x1988+0x11)+0x08) != 0x0000)
                {
                    _656B:
                    *_17D0 = _17CD;
                    return 0x00;
                }
            }
        }
        else if (_17CF == 0x2B)
        {
            _659A:
            _17CD++;
            if (_17CD > 0x02)
            {
                _65B4:
                _17CD = 0x00;
            }
            _65BA:
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01])
            {
            _65F1:
                // hp
                if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CD*0x0019+0x1988+0x11)+0x08) != 0x0000)
                {
                    _6647:
                    *_17D0 = _17CD;
                    return 0x00;
                }
            }
        }
        else
        {
            _666A:
            _17CD = *_17D0+_17CE;
            _17CD %= 0x03;
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01])
            {
            _66C8:
                // hp
                if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CD*0x0019+0x1988+0x11)+0x08) != 0x0000)
                {
                    _671E:
                    _17CB++;
                    if (_17CB == 0x01)
                    {
                        _6733:
                        _17CC = _17CD;
                    }
                }
            }
        }
    }
    _673E:
    *_17D0 = _17CC;
    if (_17CB == 0x00)
    {
        _6762:
        return 0xAA;
    }
    return _17CB;
}

UINT8 _00229784(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _679F:
        return 0xFF;
    }
    _67A4:
    _17CD = *_17D0;
    _17CC = 0x00;
    _17CB = 0x00;
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _67E4:
        if (_17CF == 0x2D)
        {
            _67F0:
            _17CD--;
            if (_17CD > 0xF0)
            {
                _680A:
                _17CD = 0x02;
            }
            _6810:
            // 敌人角色资源ID
            if (MCU_memory[_17CD*0x0033+0x1826+0x01])
            {
                _6847:
                *_17D0 = _17CD;
                return 0x00;
            }
        }
        else if (_17CF == 0x2B)
        {
            _6876:
            _17CD++;
            if (_17CD > 0x02)
            {
                _6890:
                _17CD = 0x00;
            }
            _6896:
            // 敌人角色资源ID
            if (MCU_memory[_17CD*0x0033+0x1826+0x01])
            {
                _68CD:
                *_17D0 = _17CD;
                return 0x00;
            }
        }
        else
        {
            _68F0:
            // 敌人角色资源ID
            if (MCU_memory[_17CE*0x0033+0x1826+0x01])
            {
                _6927:
                _17CB++;
                if (_17CB == 0x01)
                {
                    _693C:
                    _17CC = _17CE;
                }
            }
        }
    }
    _6947:
    *_17D0 = _17CC;
    if (_17CB == 0x00)
    {
        _696B:
        return 0xAA;
    }
    else
    {
        _6973:
        return _17CB;
    }
}

// 敌人战斗动画
void _0022998D(UINT8 _17CF, UINT8 _17D0)
{
    const UINT8 _84B3[] = { 0x40,0x60,0x80 };
    const UINT8 _84B6[] = { 0x34,0x30,0x28 };
    const UINT8 _8693[] = { 0x01,0x02,0x04 };
    const UINT8 _87A1[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x04,0x05,0x06 };
    const UINT8 _87AD[] = { 0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x0A,0x0A,0x0F,0x0A,0x14 };
    const UINT8 _87B9[] = { 0x0A,0x0C,0x0E,0x0E,0x0E,0x0E,0x2A,0x2C,0x2E,0x2E,0x2E,0x2E,0x50,0x52,0x54,0x54,0x54,0x54 };
    const UINT8 _87CB[] = { 0x17,0x19,0x1B,0x1B,0x1B,0x1B,0x0C,0x0E,0x10,0x10,0x10,0x10,0x09,0x0B,0x0D,0x0D,0x0D,0x0D };
    const UINT8 _87DD[] = { 0x0C,0x0B,0x0A,0x40,0x42,0x44,0x2C,0x2B,0x2A,0x40,0x42,0x44,0x52,0x51,0x50,0x40,0x42,0x44,0x0D,0x0E,0x0F,0x0F,0x0F,0x0F,0x0C,0x0B,0x0A,0x60,0x62,0x64,0x2C,0x2B,0x2A,0x60,0x62,0x64,0x52,0x51,0x50,0x60,0x62,0x64,0x2D,0x2E,0x2F,0x2F,0x2F,0x2F,0x0C,0x0B,0x0A,0x80,0x82,0x84,0x2C,0x2B,0x2A,0x80,0x82,0x84,0x52,0x51,0x50,0x80,0x82,0x84,0x53,0x54,0x55,0x55,0x55,0x55,0x19,0x18,0x17,0x34,0x36,0x38,0x0E,0x0D,0x0C,0x34,0x36,0x38,0x0B,0x0A,0x09,0x34,0x36,0x38,0x1A,0x1B,0x1C,0x1C,0x1C,0x1C,0x19,0x18,0x17,0x30,0x32,0x34,0x0E,0x0D,0x0C,0x30,0x32,0x34,0x0B,0x0A,0x09,0x30,0x32,0x34,0x0F,0x10,0x11,0x11,0x11,0x11,0x19,0x18,0x17,0x28,0x2A,0x2C,0x0E,0x0D,0x0C,0x28,0x2A,0x2C,0x0B,0x0A,0x09,0x28,0x2A,0x2C,0x0C,0x0D,0x0E,0x0E,0x0E,0x0E,0x01,0x02,0x04,0x07,0x01,0x02,0x04,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    const UINT8 _8825[] = { 0x19,0x18,0x17,0x34,0x36,0x38,0x0E,0x0D,0x0C,0x34,0x36,0x38,0x0B,0x0A,0x09,0x34,0x36,0x38,0x1A,0x1B,0x1C,0x1C,0x1C,0x1C,0x19,0x18,0x17,0x30,0x32,0x34,0x0E,0x0D,0x0C,0x30,0x32,0x34,0x0B,0x0A,0x09,0x30,0x32,0x34,0x0F,0x10,0x11,0x11,0x11,0x11,0x19,0x18,0x17,0x28,0x2A,0x2C,0x0E,0x0D,0x0C,0x28,0x2A,0x2C,0x0B,0x0A,0x09,0x28,0x2A,0x2C,0x0C,0x0D,0x0E,0x0E,0x0E,0x0E,0x01,0x02,0x04,0x07,0x01,0x02,0x04,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8* _17C4 = _87DD; // 初始化，以便解除编译报错
    UINT8* _17C2 = _8825; // 初始化，以便解除编译报错
    UINT8* _17C0;
    UINT8 _17BF;
    UINT8 _17BE;
    UINT8 _17BD;
    UINT8 _17BC;
    UINT8 _17BB;
    UINT8 _17BA;
    UINT8 _17B9;
    UINT8 _17B8;
    UINT8 _17B7;
    UINT8 _17B6;
    UINT8 _17B5;
    UINT8 _17B4;
    UINT8 _17B3;
    if (MCU_memory[0x1935])
    {
        _69A8:
        return;
    }
    _69AB:
    if (_17D0 >= 0x06)
    {
        _69B7:
        return;
    }
    _69BA:
    SysTimer1Close();
    SysTimer1Open(0x01);
    if (_17D0 > 0x02)
    {
        _69E3:
        _17BA = 0x03;
        _17D0 -= 0x03;
    }
    else
    {
        _69F7:
        _17BA = _17D0;
    }
    _69FF:
    // 敌人角色魔法链
    _17CA = MCU_memory[_17D0*0x0033+0x1826+0x05];
    _17C9 = MCU_memory[*(UINT16*)(MCU_memory+0x1824)+0x01];
    _17C6 = _17C9-0x01;
    if (_17C6 < 0xF0)
    {
        _6A5E:
        _17B4 = DAT_SUNDRYPIC;
        _17B3 = 0x03;
        // 角色资源ID
        _17BC = MCU_memory[_17C6*0x0019+0x1988+0x01];
        _17BB = MCU_memory[*(UINT16*)(MCU_memory+(_17C6<<1)+0x181E)+0x02];
        _17B6 = _84B3[_17C6];
        _17B5 = _84B6[_17C6];
    }
    else
    {
        _6B10:
        _17B4 = DAT_SUNDRYPIC;
        _17B3 = 0x04;
        _17BC = MCU_memory[0x18DF];
        _17BB = 0x00;
        _17B6 = 0x00;
        _17B5 = 0x3D;
    }
    _6B35:
    _17C0 = __8F0A(DAT_ACTORPIC, 0x03, _17CA);
    _17B8 = _17C0[0x02];
    _17B7 = _17C0[0x03];
    _17CB = _17C0[0x04];
    _17B9 = _17CB-0x01;
    if (_17CB > 0x02)
    {
        _6BC1:
        _17BE = 0x01;
    }
    else
    {
        _6BCA:
        _17BE = 0x00;
    }
    _6BD0:
    if (0x0001 == _17CF)
    {
        _6C01:
        _17CF = 0x00;
        if (_17BA == 0x03)
        {
            _6C13:
            _17C9 = _17D0;
        }
        _6C1B:
        _17C4 = _17BA*0x0006+ _17C9 * 0x0018 + _87DD;
        _17C2 = _17BA*0x0006+ _17C9 * 0x0018 + _8825;
        _17CB = 0x06;
    }
    else if (0x0002 == _17CF)
    {
        _6CDE:
        _17CF = 0x01;
        _17C4 = _17D0*0x0006+_87B9;
        _17C2 = _17D0*0x0006+_87CB;
    }
    _6D41:
    _0022AB72(0x00, _8693[_17D0]);
    __8E33();
    for (UINT8 _17CC=0x00; _17CC<_17CB; _17CC++)
    {
        _6DA2:
        _17BF = _87A1[_17CC+ _17BE * 0x0006];
        if (_17BF > _17B9)
        {
            _6E04:
            _17BF = _17B9;
        }
        _6E0C:
        _17C8 = _17C4[_17CC];
        _17C7 = _17C2[_17CC];
        if (_17CF == 0x00 && _17BA != 0x03 && _17CC > 0x02)
        {
            _6E76:
            if (_17B8 > 0x20)
            {
                _6E87:
                _17C8 -= _17B8>>0x01;
            }
            else
            {
                _6EA2:
                _17C8 -= _17B8;
            }
            _6EAF:
            _17C7 -= _17B7/0x04;
            if (_17C8 > 0xB0)
            {
                _6ED8:
                _17C8 = _17CC*0x02;
            }
        }
        _6EE7:
        _17BD = _87AD[_17CC+ _17BE * 0x0006];
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
        __8F07(0x08, 0x03, _17CA, _17BF, _17C8, _17C7);
        if (_17CF == 0x00 && _17BC != 0x00)
        {
            _6FBC:
            __8F07(_17B4, _17B3, _17BC, _17BB, _17B6, _17B5);
        }
        _6FFE:
        if (MCU_memory[0x1935])
        {
            _7006:
            return;
        }
        _7009:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        __8DD3(_17BD, 0x00);
    }
    _7058:
    SysTimer1Close();
    SysTimer1Open(0x32);
}

void _0022A081(UINT8* _17D0)
{
    if (MCU_memory[0x1935])
    {
        _7099:
        return;
    }
    MCU_memory[0x1A96] = _17D0[0x00]; // 遇怪开关
    if (MCU_memory[0x1A96] == 0x00)
    { // 不遇怪
        _70B6:
        return;
    }
    _70B9:
    MCU_memory[0x18DD] = _17D0[0x01]; // 大战斗背景
    MCU_memory[0x18DE] = _17D0[0x02]; // 右上角图
    MCU_memory[0x18DF] = _17D0[0x03]; // 左下角图
    _0022A190(_17D0[0x04], _17D0+0x0005);
}

void _0022A190(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CA;
    if (MCU_memory[0x1935])
    {
        _71AB:
        return;
    }
    _71AE:
    if (_17CF >= 0x08)
    {
        _71BA:
        _17CF = 0x08;
    }
    _71C0:
    for (_17CA=0x00; _17CA<_17CF; _17CA++,_17D0++)
    {
    _7219:
        // 可能出现的敌人的种类
        MCU_memory[_17CA+0x18E4] = *_17D0;
    }
    _7256:
    MCU_memory[_17CA+0x18E4] = 0x00; // fixme 存在数组越界
}

UINT8 _0022A28C()
{
    UINT8 _17CF;
    if (MCU_memory[0x1935])
    {
        _72A4:
        return 0xFF;
    }
    _72A9:
    _17CF = 0x00;
    for (UINT8 _17CE=0x00; _17CE<0x08; _17CE++)
    {
    _72D2:
        // 可能出现的敌人的种类 == 0
        if (MCU_memory[_17CE+0x18E4] == 0x00)
        {
            _72F2:
            break;
        }
        _72F5:
        _17CF++;
    }
    _7301:
    return _17CF;
}

UINT8 _0022A31B(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2, UINT8 _17D3, UINT8 _17D4)
{
    const UINT8 _886D[] = { 0x01,0x02,0x04 };
    const UINT8 _8870 = 0x07;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17CB;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT16 _17C5;
    if (MCU_memory[0x1935])
    {
        _7336:
        return 0xFF;
    }
    _733B:
    _17CA = 0x00;
    _17C9 = 0x00;
    _17CB = 0x00;
    _17C8 = 0x00;
    _17C7 = 0x00;
    if (_17D2 == 0xAA) // 群体伤害
    {
        _7365:
         if (_17D3 == 0x02)
         {
            _7371:
            _17CA = 0x03;
            _17C7 = _8870;
         }
         else
         {
            _7381:
            _17CA = 0x03;
            _17C8 = _8870;
         }
    }
    else
    {
        _7391:
         if (_17D3 == 0x02)
        {
            _739D:
            _17C7 = _886D[_17D2];
        }
        else
        {
            _73BF:
            _17C8 = _886D[_17D2];
        }
    }
    _73DE:
    _0022AB72(_17C8, _17C7);
    __8E33();
    _0022B054(_17CF, _17D2, _17D3);
    __8E33();
    SysTimer1Close();
    SysTimer1Open(0x01);
    while (_17CB < 0x06)
    {
        _745B:
        if (_17D2 == 0xAA)
        {
            _7467:
            for (UINT8 _17CC=0x00; _17CC<_17CA; _17CC++)
            {
                _748A:
                if (_17D4)
                {
                    _7493:
                    _17C5 = _17D0[0];
                }
                else
                {
                    _74B7:
                    _17C5 = _17D0[_17CC];
                }
                _74EC:
                if (_0022A682(_17CF, _17C5, _17CC, _17D3, _17CB*0x02) != 0x00)
                {
                    _7537:
                    if (_17CB == 0x00)
                    {
                        _7540:
                        _17C9++;
                    }
                }
            }
        }
        else
        {
            _754F:
            _0022A682(_17CF, _17D0[0], _17D2, _17D3, _17CB * 0x02);
            _17C9 = 0x01;
        }
        _75B7:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
        __8DD3(0x05, 0x01);
        if (MCU_memory[0x1935])
        {
            _7609:
            return 0xFF;
        }
        _760E:
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
        _17CB++;
    }
    _7650:
    SysTimer1Close();
    SysTimer1Open(0x32);
    return _17C9;
}

UINT8 _0022A682(UINT8 _17CF, UINT16 _17D0, UINT8 _17D2, UINT8 _17D3, UINT8 _17D4)
{
    const UINT8 _84B3[] = { 0x40,0x60,0x80 };
    const UINT8 _84B6[] = { 0x34,0x30,0x28 };
    const UINT8 _84B9[] = { 0x0C,0x2C,0x52 };
    const UINT8 _84BC[] = { 0x19,0x0E,0x0B };
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    if (_17D3 == 0x02)
    {
        _76A1:
        // 敌人角色资源ID
        if (MCU_memory[_17D2*0x0033+0x1826+0x01] == 0x00)
        {
            _76D8:
            return 0x00;
        }
        _76DD:
        _17CD = _84B9[_17D2]-0x04;
        _17CC = _84BC[_17D2]+0x06;
    }
    else
    {
        _7724:
        // 角色资源ID
        if (MCU_memory[_17D2*0x0019+0x1988+0x01] == 0x00)
        {
            _775B:
            return 0x00;
        }
        _7760:
        _17CD = _84B3[_17D2]-0x04;
        _17CC = _84B6[_17D2]+0x06;
    }
    _77A4:
    if (_17CF == 0x2D)
    {
        _77B0:
        _17CA = 0x07;
        _17CB = _17CC-0x02;
    }
    else
    {
        _77C4:
        _17CA = 0x06;
        _17CB = _17CC;
    }
    _77D2:
    _17CC -= _17D4;
    _17CB -= _17D4;
    if (_17D0 != 0x0000)
    {
        _7807:
        if (_17D0 == 0xFFFF) // MISS
        {
        _7822:
            // 显示MISS图片
            __8F07(DAT_SUNDRYPIC, 0x02, 0x12, 0x00, _17CD, _17CC);
        }
        else
        {
            _785F:
            __8F07(DAT_SUNDRYPIC, 0x02, 0x03, 0x00, _17CD, _17CC);
            // 显示伤害数值
            __8F01(_17CD + 0x07, _17CB, (UINT32)_17D0, 0x01);
        }
    }
    _78E2:
    return 0x01;
}

void _0022A8FA(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1)
{
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8* _17C8 = __8F0A(DAT_SUNDRYPIC, 0x02, 0x08);
    UINT8 _17C7 = _17C8[0x02];
    UINT8 _17C6 = _17C8[0x03];
    _17CC = 0x18-(_17C6>>0x02);
    if (_17CF == 0x01)
    {
        _7989:
        _17CC += _17C6+0x04;
    }
    else if (_17CF == 0x02)
    {
        _79A8:
        _17CC += (_17C6+0x04)>>0x01;
    }
    _79C1:
    _17CA = _17CC+_17C6;
    _17D0 <<= 0x02;
    _17CD = 0x50-_17D0-_17C7-0x02;
    _17CB = 0x50+_17D0+0x02;
    SysLcdPartClear(_17CD, _17CC, _17CB, _17CA);
    __8F04(DAT_SUNDRYPIC, 0x02, 0x08, 0x00, _17CD, _17CC);
    __8F04(DAT_SUNDRYPIC, 0x02, 0x08, 0x01, _17CB, _17CC);
    _17CD += _17C7;
    _17CD--;
    _17CC++;
    _17CA -= 0x02;
    SysRect(_17CD, _17CC, _17CB, _17CA);
    _17D1[0x0000] = _17CD + 0x02;
    _17D1[0x0001] = _17CC + 0x03;
}

void _0022AB72(UINT8 _17CF, UINT8 _17D0)
{
    const UINT8 _84B3[] = { 0x40,0x60,0x80 };
    const UINT8 _84B6[] = { 0x34,0x30,0x28 };
    const UINT8 _8871[] = { 0x01,0x02,0x04 };
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _7B8D:
        return;
    }
_7B90:
    //                          大战斗背景
    __8F07(DAT_SUNDRYPIC, 0x04, MCU_memory[0x18DD], 0x00, 0x00, 0x00);
    //                          右上角图
    __8F07(DAT_SUNDRYPIC, 0x04, MCU_memory[0x18DE], 0x00, 0x42, 0x00);
    //                          左下角图
    __8F07(DAT_SUNDRYPIC, 0x04, MCU_memory[0x18DF], 0x00, 0x00, 0x3D);
    for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
    {
        _7C6E:
        // 角色资源ID
        if (MCU_memory[_17CC*0x0019+0x1988+0x01] == 0x00)
        {
            _7CA5:
            continue;
        }
        _7CA8:
        if (_8871[_17CC]&_17CF)
        {
            _7CCC:
            continue;
        }
        _7CCF:
        _17CB = MCU_memory[*(UINT16*)(MCU_memory+(_17CC<<1)+0x181E)+0x02];
        //                          角色资源ID
        __8F07(DAT_SUNDRYPIC, 0x03, MCU_memory[_17CC * 0x0019 + 0x1988 + 0x01], _17CB, _84B3[_17CC], _84B6[_17CC]);
        if (MCU_memory[0x1935])
        {
            _7DB0:
            return;
        }
    }
    _7DB6:
    for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
    {
        _7DD7:
        // 敌人角色资源ID
        if (MCU_memory[_17CC*0x0033+0x1826+0x01] == 0x00)
        {
            _7E0E:
            continue;
        }
        _7E11:
        if (_8871[_17CC]&_17D0)
        {
            _7E35:
            continue;
        }
        _7E38:
        __8E03(_17CC, 0x00);
        if (MCU_memory[0x1935])
        {
            _7E64:
            return;
        }
    }
}

void _0022AE7B(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2, UINT8 _17D4)
{
    const UINT8 _84AF = 0x28;
    const UINT8 _84B0 = 0x0C;
    const UINT8 _84B1 = 0x5A;
    const UINT8 _84B2 = 0x2E;
    const UINT8 _84B3[] = { 0x40,0x60,0x80 };
    const UINT8 _84B6[] = { 0x34,0x30,0x28 };
    const UINT8 _84B9[] = { 0x0C,0x2C,0x52 };
    const UINT8 _84BC[] = { 0x19,0x0E,0x0B };
    if (_17D4 == 0x02)
    {
        _7E9A:
        if (_17CF == 0xAA)
        {
            _7EA6:
            *_17D0 = _84AF + 0xC4;
            *_17D2 = _84B0 + 0xCE;
        }
        else
        {
            _7EF3:
            *_17D0 = _84B9[_17CF] + 0xC4;
            *_17D2 = _84BC[_17CF] + 0xCE;
        }
    }
    else
    {
        _7F70:
        if (_17CF == 0xAA)
        {
            _7F7C:
            *_17D0 = _84B1 + 0xC4;
            *_17D2 = _84B2 + 0xCE;
        }
        else
        {
            _7FC9:
            *_17D0 = _84B3[_17CF] + 0xC4;
            *_17D2 = _84B6[_17CF] + 0xCE;
        }
    }
}

void _0022B054(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    const UINT8 _84B3[] = { 0x40,0x60,0x80 };
    const UINT8 _84B6[] = { 0x34,0x30,0x28 };
    const UINT8 _84B9[] = { 0x0C,0x2C,0x52 };
    const UINT8 _84BC[] = { 0x19,0x0E,0x0B };
    UINT8 _17CD;
    UINT8 _17CB;
    const UINT8* _17C9;
    const UINT8* _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    if (MCU_memory[0x1935])
    {
        _806F:
        return;
    }
    _8072:
    _17CD = 0x00;
    _17C5 = 0x01;
    _17C4 = 0x00;
    if (_17D1 == 0x02)
    {
        _8090:
        _17C9 = _84B9;
        _17C7 = _84BC;
        _17CB = 0x03;
        _17C6 = 0xFE;
    }
    else
    {
        _80C5:
        _17C9 = _84B3;
        _17C7 = _84B6;
        _17CB = 0x03;
        _17C6 = 0x02;
    }
    _80F7:
    if (_17D0 == 0xAA)
    {
        _8103:
        while (_17C5 < 0x03)
        {
            _810F:
            if (_17CF == 0x2D)
            {
                _811B:
                _17C4 = _17C5*_17C6;
            }
            _812C:
            for (UINT8 _17CC=0x00; _17CC<_17CB; _17CC++)
            {
                _814F:
                _00228F9F(_17CF, _17CC, _17D1, _17C9[_17CC] + _17C4, _17C7[_17CC]+_17C4);
            }
            _81CB:
            __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
            _17C5++;
        }
        _8235:
        for (UINT8 _17CC=0x00; _17CC<_17CB; _17CC++)
        {
            _8258:
            _00228F9F(_17CF, _17CC, _17D1, _17C9[_17CC] - 0x02, _17C7[_17CC]);
        }
        _82CD:
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
    }
    else
    {
        _82F8:
        while (_17C5 < 0x03)
        {
            _8304:
            if (_17CF == 0x2D)
            {
                _8310:
                _17C4 = _17C5*_17C6;
            }
            _8321:
            _00228F9F(_17CF, _17D0, _17D1, _17C9[_17D0] + _17C4, _17C7[_17D0]+_17C4);
            __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
            _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
            _17C5++;
        }
        _8404:
        _00228F9F(_17CF, _17D0, _17D1, _17C9[_17D0] - 0x02, _17C7[_17D0]);
        __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
    }
}

void _0022C000()
{
    UINT16 _17CA;
    UINT16 _17C8;
    UINT16 _17C6;
    if (MCU_memory[0x1935])
    {
        _5018:
        return;
    }
    _501B:
    __8E78(0x00, 0x00);
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
    UINT16 _17CC = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    _17CA = _17CC%0x000A+0x000F;
    _0022C757();
    // 显示获得的道具名称和数量
    _0022CC0F();
    // 玩家拥有的金钱 += 待增加角色金钱
    *(UINT32*)(MCU_memory+0x1A8F) += *(UINT16*)(MCU_memory+0x18E2);
    for (UINT8 _17C5=0x00; _17C5<0x03; _17C5++)
    {
        _5116:
        // 角色资源ID
        if (MCU_memory[_17C5*0x0019+0x1988+0x01] == 0x00)
        {
            _514D:
            continue;
        }
    _5150:
        // hp == 0
        if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x08) == 0x0000)
        {
        _51A6:
            // hp = 1
            *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x0008) = 0x0001;
            continue;
        }
    _520B:
        //                                                                                      hpMax                                                                           hp
        _17C8 = (*(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11) + 0x06) -*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x08))*_17CA/0x0064;
        //                                                                                      mpMax                                                                           mp
        _17C6 = (*(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11) + 0x0A) -*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x0C))*_17CA/0x0064;
        // hp
        *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x0008) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11) + 0x08) + _17C8;
        // mp
        *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x000C) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11) + 0x0C) + _17C6;
        //      指向角色属性
        __8E87(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11), 0x01);
        // 当前经验 += 待增加角色经验值
        *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x0012) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11) + 0x12) + *(UINT16*)(MCU_memory + 0x18E0);
        // 当前经验 > 升级经验
        if (*(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17C5 * 0x0019 + 0x1988 + 0x11) + 0x12) > *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C5*0x0019+0x1988+0x11)+0x14))
        {
            _573C:
            _0022F05D(_17C5);
        }
    }
}

void _0022C757()
{
    const UINT8 _8ACD[] = "获得经验";
    const UINT8 _8AD6[] = "战斗获得 ";
    const UINT8 _8AE0[] = "钱";
    UINT8 _17A6[0x28];
    UINT8 _179E[8];
    UINT8 _179D;
    UINT8 _179B[2];
    if (MCU_memory[0x1935])
    {
        _576F:
        return;
    }
    _5772:
    fillmem(_17A6, 0x0028, 0x00);
    //              待增加角色经验值
    _itoa(*(UINT16*)(MCU_memory+0x18E0), _179E, 0x000A);
    _179D = (UINT8)strlen(_179E);
    _SysMemcpy(_17A6, _8ACD, 0x0009);
    _179D = 0x05-_179D;
    _0022D1F7(_17A6, _179D);
    strcat(_17A6, _179E);
    _179D = (UINT8)strlen(_17A6);
    __8E75(0x00, _179D, _179B);
    SysPrintString(_179B[0], _179B[1], _17A6);
    fillmem(_17A6, 0x0028, 0x00);
    //              待增加角色金钱
    _itoa(*(UINT16*)(MCU_memory+0x18E2), _179E, 0x000A);
    _179D = (UINT8)strlen(_179E);
    _SysMemcpy(_17A6, _8AD6, 0x0009);
    _179D = 0x05-_179D;
    _0022D1F7(_17A6, _179D);
    strcat(_17A6, _179E);
    _SysMemcpy(_17A6 + 0x000E, _8AE0, 0x0003);
    _179D = (UINT8)strlen(_17A6);
    __8E75(0x01, _179D, _179B);
    SysPrintString(_179B[0], _179B[1], _17A6);
    __8DD3(0x05, 0x01);
}

void _0022CC0F()
{
    const UINT8 _8AE3[] = "得到 ";
    const UINT8 _8AE9[] = " x";
    UINT8 _17A3[0x28];
    UINT8 _179B[8];
    UINT8 _179A;
    UINT8 _1799;
    UINT8 _1798;
    UINT8 _1797;
    UINT8* _1795;
    if (MCU_memory[0x1935])
    {
        _5C27:
        return;
    }
    _5C2A:
    for (UINT8 _17CB=0x00; _17CB<0x0A; _17CB++)
    {
        _5C4B:
        // 得到的道具类型
        _1799 = MCU_memory[_17CB*0x0003+0x18BF];
        if (_1799 == 0x00)
        {
            _5C8A:
            continue;
        }
        _5C8D:
        // 得到的道具的Idx
        _1798 = MCU_memory[_17CB*0x0003+0x18BF+0x01];
        // 得到的道具数量
        _1797 = MCU_memory[_17CB*0x0003+0x18BF+0x02];
        _1795 = __8F0A(DAT_GOODSRES, _1799, _1798);
        if (MCU_memory[0x1935])
        {
            _5D37:
            return;
        }
        fillmem(_17A3, 0x0028, 0x00);
        _SysMemcpy(_17A3, _8AE3, 0x0005);
        // 道具名称长度
        _179A = (UINT8)strlen(_1795+0x0006);
        // 复制道具名称
        _SysMemcpy(_17A3 + 0x0005, _1795 + 0x0006, _179A);
        _SysMemcpy(_17A3 + 0x0005 + _179A, _8AE9, 0x0002);
        _itoa(_1797, _179B, 0x000A);
        strcat(_17A3, _179B);
        __8EF8(_17A3, _1799, _1798, _1797);
        if (MCU_memory[0x1935])
        {
            _5FBF:
            return;
        }
    }
}

void _0022CFD6(UINT8 _17CF)
{
    const UINT8 _8AEC[] = "修行提升";
    UINT8* _17CD;
    UINT8 _17B9[0x12];
    UINT8 _17B8;
    UINT8 _17B6[2];
    if (MCU_memory[0x1935])
    {
        _5FF1:
        return;
    }
    _5FF4:
    // 获取角色姓名字符串长度          指向角色姓名
    _17B8 = (UINT8)strlen(MCU_memory + *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x0F));
    // 指向角色姓名
    _17CD = MCU_memory + *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x0F);
    _SysMemcpy(_17B9, _17CD, _17B8);
    _17CD += _17B8;
    _SysMemcpy(_17CD, _8AEC, 0x0009);
    _17B8 = (UINT8)strlen(_17B9);
    __8E75(0x02, _17B8, _17B6);
    SysPrintString(_17B6[0], _17B6[1], _17B9);
}

void _0022D1F7(UINT8* _17D0, UINT8 _17D2)
{
    UINT8 _17CD;
    if (MCU_memory[0x1935])
    {
        _620F:
        return;
    }
    _6212:
    if (_17D2 == 0x00)
    {
        _621B:
        return;
    }
    _621E:
    _17CD = (UINT8)strlen(_17D0);
    fillmem(_17D0 + _17CD, _17D2, 0x20);
    fillmem(_17D0 + _17CD + _17D2, 0x0001, 0x00);
}

void _0022D34B(UINT8 _17CF, UINT8* _17D0)
{
    const UINT8 _8AF5[] = { 0x25,0x38,0x56,0x69,0x25,0x38,0x56,0x69,0x2F,0x60,0x2F,0x60,0x2F,0x60,0x2F,0x60,0x2F,0x60 };
    const UINT8 _8B07[] = { 0x09,0x09,0x09,0x09,0x15,0x15,0x15,0x15,0x21,0x21,0x2D,0x2D,0x39,0x39,0x45,0x45,0x51,0x51 };
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8* _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    const UINT8* _17C6;
    const UINT8* _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT16 _17C0;
    if (MCU_memory[0x1935])
    {
        _6366:
        return;
    }
    _6369:
    _17CA = __8F0A(DAT_SUNDRYPIC, 0x02, 0x09);
    _17CD = _17CA[0x02];
    _17CC = _17CA[0x03];
    _17C9 = (0x009F-_17CD)/0x0002;
    _17C8 = (0x60-_17CC)/0x02;
    _17C6 = _8AF5;
    _17C4 = _8B07;
    __8F07(DAT_SUNDRYPIC, 0x02, 0x09, 0x00, _17C9, _17C8);
    for (UINT8 _17CE=0x00; _17CE<0x12; _17CE++)
    {
        _6488:
        _17C3 = _17C6[_17CE]+_17C9;
        _17C2 = _17C4[_17CE]+_17C8;
        switch (_17CE)
        {
        case 0x0000: // _64F7
            _17C0 = *(UINT16*)(_17D0+0x02);
            break;
        case 0x0001: // _651C
            _17C0 = *(UINT16*)(_17D0);
            break;
        case 0x0002: // _6541
            // hp
            _17C0 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x08);
            break;
        case 0x0003: // _6595
            // hpMax
            _17C0 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x06);
            break;
        case 0x0004: // _65E9
            _17C0 = *(UINT16*)(_17D0+0x06);
            break;
        case 0x0005: // _660E
            _17C0 = *(UINT16*)(_17D0+0x04);
            break;
        case 0x0006: // _6633
            // mp
            _17C0 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0C);
            break;
        case 0x0007: // _6687
            // mpMax
            _17C0 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0A);
            break;
        case 0x0008: // _66DB
            _17C0 = *(UINT16*)(_17D0+0x08);
            break;
        case 0x0009: // _6700
            // 攻击
            _17C0 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0E);
            break;
        case 0x000A: // _6754
            _17C0 = *(UINT16*)(_17D0+0x0A);
            break;
        case 0x000B: // _6779
            // 防御
            _17C0 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x10);
            break;
        case 0x000C: // _67CD
            _17C0 = _17D0[0x10];
            break;
        case 0x000D: // _67F0
            // 身法
            _17C0 = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x16];
            break;
        case 0x000E: // _6842
            _17C0 = _17D0[0x11];
            break;
        case 0x000F: // _6865
            // 灵力
            _17C0 = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x17];
            break;
        case 0x0010: // _68B7
            _17C0 = _17D0[0x12];
            break;
        case 0x0011: // _68DA
            // 幸运
            _17C0 = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x18];
            break;
        }
        _692C:
        __8F01(_17C3, _17C2, (UINT32)_17C0, 0x01);
    }
    _696B:
    __8DD3(0x02, 0x01);
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
    __8DD3(0x03, 0x01);
}

UINT8 _0022D9E8(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT16 _17C6;
    UINT16 _17C4;
    UINT16 _17C2;
    UINT16 _17C0;
    UINT8* _17BE;
    UINT8 _17AA[20];
    UINT8* _17A8;
    if (MCU_memory[0x1935])
    {
        _6A03:
        return 0x00;
    }
    _6A08:
    _17C4 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    _SysMemcpy(_17D0, MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0006, 0x0013);
    //              角色学会
    _17D0[0x0013] = MCU_memory[_17CF * 0x0019 + 0x1988 + 0x09];
    //                                  角色资源ID
    _17BE = __8F0A(DAT_MAGICLINK, 0x02, MCU_memory[_17CF*0x0019+0x1988+0x01]);
    // 等级
    _17CA = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)];
    _17C9 = _17BE[0x02];
    _17C6 = _17CA-0x01;
    _17C6 *= 0x0014;
    _17A8 = _17BE+0x0004+_17C6;
    __8E8A(_17D0, _17A8, _17AA, 0x2D);
    _17C8 = _0022E655(_17CF, _17C9, _17A8, &_17C0);
    _17A8 = _17A8 +_17C8*0x14;
    __8E8A(_17AA, _17A8, _17AA, 0x2B);
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0006, _17AA, 0x0013);
    // 当前经验
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0012) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0012) - _17C0;
    // 等级
    MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)] = MCU_memory[*(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11)] + _17C8;
    // 升级经验
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0014) = *(UINT16*)(_17A8 + 0x0E);
    //      hpMax
    _17C2 = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x06) -*(UINT16*)(_17D0);
    _17C2 = _17C4%_17C2;
    _17C8 = _17C2>>0x0003;
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0006) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x06) + _17C8;
    //      mpMax
    _17C2 = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0A) -*(UINT16*)(_17D0+0x04);
    _17C2 = _17C4%_17C2;
    _17C8 = _17C2>>0x0003;
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x000A) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0A) + _17C8;
    // hp = hpMax
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0008) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x06);
    // mp = mpMax
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x000C) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0A);
    //      指向角色属性
    __8E87(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), 0x01);
    if (_17CA >= _17C9)
    {
    _7585:
        // 等级
        MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)] = 0xF3;
        // 升级经验
        *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0014) = 0x0000;
    }
    _762F:
    return _17A8[0x13];
}

UINT8 _0022E655(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT16* _17D3)
{
    *_17D3 = 0x0000;
    // 当前经验
    UINT16 _17C9 = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x12);
    // 等级
    UINT8 _17CC = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)];
    UINT8 _17CB = _17CC;
    while (_17C9 > *(UINT16*)(_17D1+0x0E))
    {
        _7760:
        if (_17CB >= _17D0)
        {
            _776E:
            return _17CB-_17CC;
        }
        _777A:
        _17C9 -= *(UINT16*)(_17D1+0x0E);
        *_17D3 = *(UINT16*)(_17D1 + 0x0E);
        _17D1 = _17D1 +0x0014;
        _17CB++;
    }
    _7854:
    return _17CB-_17CC;
}

void _0022E873(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    UINT8 _17CA;
    UINT8 _17BC[0x0C];
    UINT16 _17BA;
    UINT8* _17C8;
    if (MCU_memory[0x1935])
    {
        _788E:
        return;
    }
    _7891:
    // 角色学会
    _17CA = MCU_memory[_17CF*0x0019+0x1988+0x0E];
    // 指向魔法链数据
    _17BA = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x15);
    MCU_memory[_17CA*0x0002+_17BA] = _17D0;
    MCU_memory[_17CA*0x0002+_17BA+0x0001] = _17D1;
    // 角色学会
    MCU_memory[_17CF * 0x0019 + 0x1988 + 0x000E]++;
    _17C8 = __8F0A(DAT_MGICRES, _17D0, _17D1);
    // 复制法术名称
    _SysMemcpy(_17BC, _17C8 + 0x0006, 0x000C);
    //      指向姓名
    __8E99(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0F), _17BC);
}

void _0022EB3F(UINT8* _17D0, UINT8* _17D2)
{
    const UINT8 _8B61[] = { 0x00,0x00 };
    const UINT8 _8B63[] = { 0x06,0x2B };
    UINT8* _17C6 = __8F0A(DAT_SUNDRYPIC, 0x02, 0x0A);
    UINT8 _17CA = _17C6[0x02];
    UINT8 _17C9 = _17C6[0x03];
    UINT8 _17CC = (0x009F-_17CA)>>0x0001;
    UINT8 _17CB = (0x60-_17C9)>>0x01;
    SysLcdPartClear(_17CC - 0x03, _17CB - 0x03, _17CC + _17CA + 0x03, _17CB+_17C9+0x03);
    __8F04(DAT_SUNDRYPIC, 0x02, 0x0A, 0x00, _17CC, _17CB);
    const UINT8* _17C4 = _8B61;
    const UINT8* _17C2 = _8B63;
    UINT8 _17C8 = (UINT8)strlen(_17D0);
    _17C8 *= 0x08;
    SysPrintString((0x009F - _17C8) >> 0x0001, _17C2[0x00] + _17CB, _17D0);
    _17C8 = (UINT8)strlen(_17D2);
    _17C8 *= 0x08;
    SysPrintString((0x009F - _17C8) >> 0x0001, _17C2[0x01] + _17CB, _17D2);
    __8DD3(0x03, 0x01);
}

void _0022EE58(UINT8 _17CF, UINT8 _17D0)
{
    UINT8* _17CB;
    UINT8* _17C9;
    UINT16 _17C7;
    UINT8 _17C6;
    if (MCU_memory[0x1935])
    {
        _7E73:
        return;
    }
    _7E76:
    for (_17C6=0x00; _17C6<0x03; _17C6++)
    {
        _7E97:
        // 角色资源ID
        if (MCU_memory[_17C6*0x0019+0x1988+0x01] == _17CF)
        {
            _7ED3:
            break;
        }
    }
    _7ED9:
    _17CB = __8F0A(DAT_MAGICLINK, 0x02, _17CF);
    if (_17D0 > _17CB[0x02])
    {
        _7F30:
        _17D0 = _17CB[0x02];
    }
    _7F44:
    _17C7 = _17D0-0x02;
    _17C7 *= 0x0014;
    _17C9 = _17CB+0x0004+_17C7;
    // 当前经验
    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17C6*0x0019+0x1988+0x11)+0x0012) = *(UINT16*)(_17C9 + 0x0E) + 0x0005;
    _0022F05D(_17C6);
}

void _0022F05D(UINT8 _17CF)
{
    UINT8 _17BA[20];
    UINT8 _17B9;
    //                          角色资源ID
    __8F0A(DAT_MAGICLINK, 0x02, MCU_memory[_17CF*0x0019+0x1988+0x01]);
    if (MCU_memory[0x1935])
    {
        _80CF:
        MCU_memory[0x1935] = 0x00;
        return;
    }
_80D7:
    // 升级经验
    if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x14) == 0x0000)
    {
        _812D:
        return;
    }
    _8130:
    _0022CFD6(_17CF);
    _17B9 = _0022D9E8(_17CF, _17BA);
    _0022D34B(_17CF, _17BA);
    if (_17BA[13] < _17B9)
    {
        _8199:
        _0022F1D7(_17CF, _17BA[13], _17B9);
    }
}

void _0022F1D7(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    UINT16 _17CB;
    UINT8* _17C9;
    if (MCU_memory[0x1935])
    {
        _81F2:
        return;
    }
    _81F5:
    if (_17D1 == _17D0)
    {
        _8203:
        return;
    }
    _8206:
    //                                  角色魔法链
    _17C9 = __8F0A(DAT_MAGICLINK, 0x01, MCU_memory[_17CF*0x0019+0x1988+0x0D]);
    if (_17C9[0x02] < _17D1)
    {
        _8282:
        MCU_memory[0x1935] = OVERFLOW_MAG;
        return;
    }
    _828A:
    _GetDataBankNumber(0x09, &_17CB);
    for (UINT8 _17CD=_17D0; _17CD<_17D1; _17CD++)
    {
        _82D4:
        __8E96(_17CF, _17C9[_17CD * 0x0002 + 0x0003], _17C9[_17CD*0x0002+0x0003+0x01]);
        _DataBankSwitch(0x09, 0x04, _17CB);
    }
    _83D0:
    // 角色学会
    MCU_memory[_17CF*0x0019+0x1988+0x0009] = _17D1;
}

void _0022F42C()
{
    for (UINT8 _17CD=0x00; _17CD<0x03; _17CD++)
    {
        _845D:
        // 敌人角色资源ID
        if (MCU_memory[_17CD*0x0033+0x1826+0x01] == 0x00)
        {
            _8494:
            continue;
        }
        _8497:
        // 指向敌人角色魔法链数据
        if (*(UINT16*)(MCU_memory+_17CD*0x0033+0x1826+0x31) != 0x0000)
        {
            _84E1:
            if (SysMemFree(MCU_memory + *(UINT16*)(MCU_memory+_17CD*0x0033+0x1826+0x31)) == 0x00)
            {
                _8543:
                MCU_memory[0x1935] = MEM_ERR;
            }
        }
    _8548:
        // 重置角色敌人数据
        fillmem(MCU_memory + _17CD * 0x0033 + 0x1826, 0x0033, 0x00);
    }
}

void _0022F5CE(UINT8 _17CF)
{
    if (MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x0003] == 0x0001)
    {
        _863C:
        _0022F65A(_17CF);
    }
}

void _0022F65A(UINT8 _17CF)
{
    const UINT8 _8B65[] = "偷得 ";
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _179C[0x28];
    UINT16 _179A;
    UINT8* _1798;
    UINT16 _1796;
    _17CA = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    // 敌人角色学会
    if (MCU_memory[_17CA*0x0033+0x1826+0x0028+0x02] == 0x00)
    {
        _86E0:
        return;
    }
    _86E3:
    _179A = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    // 幸运
    _17C9 = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x18];
    // 敌人角色幸运
    _17C8 = MCU_memory[_17CF*0x0033+0x1826+0x0012+0x04];
    if (_17C9 > _17C8)
    {
        _87AC:
        _17C7 = 0x03;
    }
    else
    {
        _87B5:
        _17C7 = 0x02;
    }
    _87BB:
    _17C9 = _179A%_17C9+0x0001;
    _17C8 = _179A%_17C8;
    _17C7 = _179A%_17C7;
    // 指向敌人角色携带物品1
    _1796 = _17CA * 0x0033 + 0x1826 + 0x0028;
    if (_17C9 > _17C8)
    {
        _8891:
        if (_17C7)
        {
        _889A:
            // 敌人角色携带物品1数量减1
            MCU_memory[_1796+0x0002] = MCU_memory[_1796 + 0x02] - 0x01;
            // 偷得的道具的类型
            _17C6 = MCU_memory[_1796];
            // 偷得的道具的Idx
            _17C5 = MCU_memory[_1796+0x01];
            _1798 = __8F0A(DAT_GOODSRES, _17C6, _17C5);
            if (MCU_memory[0x1935])
            {
                _893F:
                return;
            }
            _8942:
            fillmem(_179C, 0x0028, 0x00);
            _SysMemcpy(_179C, _8B65, 0x0005);
            // 道具名称长度
            _17C4 = (UINT8)strlen(_1798+0x0006);
            // 复制道具名称
            _SysMemcpy(_179C + 0x0005, _1798 + 0x0006, _17C4);
            __8EF8(_179C, _17C6, _17C5, 0x01);
        }
    }
}

void _00230000()
{
    /*
                    ███
        ██    █████████████
      █████                █████
      ██      ███    ███      ██
        █    █                      ██
        █    ███████████
                █        ██
              ████████████████
              ██      ████  ██████
                    ██████      ███
              ████    ███
  ████████        ███
    ████              ██
                  ██████
                    ████

              █        ██      █
        ███████  ██    ██
      ██    █        ██  █
      ████████████████
  █████████████████
        ███████  ██  ██
        ██  ████  ██  ██
        █  ██    █    █  ██
        ███████    ████
        ███████    ███
          █████        ███
  █████        ██████████
        ███    ██  ██    ███████
      ██            █          █████
                                      ██
    */
    const UINT8 _8861[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x0C,0xFF,0xF8,0x1F,0x00,0xF8,0x18,0xE7,0x18,0x09,0x00,0x18,0x09,0xFF,0xC0,0x00,0x86,0x00,0x01,0xFF,0xFE,0x01,0x8F,0x7E,0x00,0x3F,0x1C,0x01,0xE7,0x00,0x7F,0x87,0x00,0x3C,0x06,0x00,0x00,0x7E,0x00,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0C,0x40,0x0F,0xEC,0xC0,0x19,0x0D,0x00,0x1F,0xFF,0xE0,0x7F,0xFF,0xC0,0x0F,0xED,0x80,0x0D,0xED,0x80,0x0B,0x25,0x80,0x0F,0xE7,0x80,0x0F,0xE7,0x00,0x07,0xC3,0x80,0x7C,0x3F,0xF0,0x0E,0x6C,0xFE,0x18,0x10,0x7C,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    const UINT8 _8971[] = { 0x03,0x03,0x03,0x1E,0x14,0x3B,0x32,0x41,0x4F,0x3D,0x6C,0x2E,0x6A,0x09,0x4E,0x02 };
    const UINT8 _8983[] = {0x06,0x06,0x05,0x03,0x07,0x02,0x04,0x01};
    const UINT8 _898B[8][5] = { "装饰", "装饰", "护腕" , "脚蹬" , "手持" , "身穿" , "肩披" , "头戴" };
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17C8[3];
    UINT8 _17C7;
    UINT8 _17B7[8]; // todo
    UINT8 _16A7[0x010F];
    UINT8 _16A6;
    UINT8 _16A5;
    UINT8 _16A4;
    UINT8 _16A3;
    UINT8 _169E[0x05];
    UINT16 _169C;
    UINT8* _169A;
    UINT8 _1699;
    UINT8* _1697=0;
    MsgType _1694;
    _17C7 = 0x00;
    for (UINT8 _16A6=0x00; _16A6<0x03; _16A6++)
    {
        _5044:
        // 角色资源ID
        if (MCU_memory[_16A6*0x0019+0x1988+0x01] >= 0x01)
        {
            _507E:
            _17C8[_17C7] = _16A6;
            _17C7++;
        }
    }
    _50E5:
    _17CB = 0x00;
    _17CC = 0x00;
    _SysMemcpy(_16A7, _8861, 0x010F);
    _1699 = 0x00;
    _16A3 = 0xFF;
    _16A5 = 0x00;
    _16A4 = 0x02;
    while (0x01)
    {
        _5163:
        if (_16A4)
        {
            _516C:
            // 指向角色装备
            _169C = *(UINT16*)(MCU_memory+ _17C8[_17CB] *0x0019+0x1988+0x13);
            // 佩戴1
            _17B7[0] = MCU_memory[_169C + 0x06];
            // 佩戴2
            _17B7[1] = MCU_memory[_169C + 0x07];
            // 护腕
            _17B7[2] = MCU_memory[_169C + 0x03];
            // 脚蹬
            _17B7[3] = MCU_memory[_169C + 0x05];
            // 手持
            _17B7[4] = MCU_memory[_169C + 0x04];
            // 身穿
            _17B7[5] = MCU_memory[_169C + 0x01];
            // 肩披
            _17B7[6] = MCU_memory[_169C + 0x02];
            // 头戴
            _17B7[7] = MCU_memory[_169C];
            if (_16A4 == 0x02 || _16A5 == 0x01)
            {
                _5482:
                SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
                SysPicture(0x87, 0x00, 0x9E, 0x2B, _16A7, 0x00);
                //                          角色资源ID
                __8F04(DAT_SUNDRYPIC, 0x01, MCU_memory[_17C8[_17CB] * 0x0019 + 0x1988 + 0x01], 0x00, 0x2C, 0x0C);
                for (UINT8 _16A6=0x00; _16A6<0x08; _16A6++)
                {
                    _5603:
                    if (_17B7[_16A6]+0x01 == 0x01)
                    {
                        _5632:
                        continue;
                    }
                    _5635:
                    _169A = __8F0A(DAT_GOODSRES, _8983[_16A6], _17B7[_16A6]);
                    if (MCU_memory[0x1935])
                    {
                        _56CE:
                        __8F1C((UINT32)0xEEE0);
                        return;
                    }
                    _56FA:
                    _1697 = _169A;
                    __8F04(DAT_GOODSPIC, _8983[_16A6], _1697[0x05], 0x00, _8971[_16A6 * 0x02], _8971[_16A6*0x02+0x0001]);
                    if (MCU_memory[0x1935])
                    {
                        _5824:
                        __8F1C((UINT32)0xEEE1);
                        return;
                    }
                }
                _5853:
                _16A5 = 0x00;
            }
            _5859:
            _SysMemcpy(_169E, _898B[_17CC], 0x0005);
            SysPrintString(0x78, 0x50, _169E);
            //                                      指向角色姓名
            SysPrintString(0x1E, 0x28, MCU_memory + *(UINT16*)(MCU_memory+ _17C8[_17CB] *0x0019+0x1988+0x0F));
            _16A6 = _17CC*0x02;
            SysRect(_8971[_16A6], _8971[_16A6 + 0x01], _8971[_16A6] + 0x18, _8971[_16A6+0x01]+0x18);
            _16A4 = 0x00;
        }
        _5AF2:
        if (GuiGetMsg(&_1694) == 0x00)
        {
            _5B27:
            continue;
        }
        _5B2A:
        if (GuiTranslateMsg(&_1694) == 0x00)
        {
            _5B5F:
            continue;
        }
        _5B62:
        if (_1694.type == DICT_WM_CHAR_FUN)
        {
            _5B93:
            switch (_1694.param)
            {
            case CHAR_LEFT: // _5BBD
                if (_17CB >= 0x01)
                {
                    _5BD6:
                    _17CB--;
                    _16A4 = 0x02;
                }
                break;
            case CHAR_RIGHT: // _5C06
                if (_17CB < _17C7-0x01)
                {
                    _5C35:
                    _17CB++;
                    _16A4 = 0x02;
                }
                break;
            case CHAR_UP: // _5C65
                if (_17CC >= 0x01)
                {
                    _5C7E:
                    _16A6 = _17CC*0x02;
                    SysLcdPartClear(_8971[_16A6], _8971[_16A6 + 0x01], _8971[_16A6] + 0x18, _8971[_16A6+0x01]+0x18);
                    if (_17B7[_17CC] >= 0x01)
                    {
                        _5DDB:
                        _169A = __8F0A(DAT_GOODSRES, _8983[_17CC], _17B7[_17CC]);
                        if (MCU_memory[0x1935])
                        {
                            _5E8E:
                            return;
                        }
                        _5E91:
                        _1697 = _169A;
                        __8F04(DAT_GOODSPIC, _8983[_17CC], _1697[0x05], 0x00, _8971[_17CC * 0x02], _8971[_17CC*0x02+0x01]);
                    }
                    _5FDA:
                    _17CC--;
                    _1699 = 0x00;
                    _16A3 = 0xFF;
                    _16A4 = 0x01;
                }
                break;
            case CHAR_DOWN: // _6016
                if (_17CC < 0x07)
                {
                    _602F:
                    _16A6 = _17CC*0x02;
                    SysLcdPartClear(_8971[_16A6], _8971[_16A6 + 0x01], _8971[_16A6] + 0x18, _8971[_16A6+0x01]+0x18);
                    if (_17B7[_17CC] >= 0x01)
                    {
                        _618C:
                        _169A = __8F0A(DAT_GOODSRES, _8983[_17CC], _17B7[_17CC]);
                        _1697 = _169A;
                        if (MCU_memory[0x1935])
                        {
                            _6255:
                            return;
                        }
                        _6258:
                        __8F04(DAT_GOODSPIC, _8983[_17CC], _1697[0x05], 0x00, _8971[_17CC * 0x02], _8971[_17CC*0x02+0x01]);
                    }
                    _638B:
                    _17CC++;
                    _1699 = 0x00;
                    _16A3 = 0xFF;
                    _16A4 = 0x01;
                }
                break;
            case CHAR_PGUP: // _63C7
                if (_1699 > 0x01)
                {
                    _63D8:
                    SysLcdPartClear(0x08, 0x1C, 0x96, 0x41);
                    SysRect(0x09, 0x1D, 0x95, 0x40);
                    _16A3 = _1699;
                    _1699 -= 0x02;
                    _1699 = __8EC7(0x0B, 0x1E, 0x96, 0x41, _1699, _1697+0x001E);
                }
                break;
            case CHAR_PGDN: // _64B2
                if (_1699 >= 0x01)
                {
                    _64BE:
                    if (_1699 != _16A3)
                    {
                        _64CC:
                        SysLcdPartClear(0x08, 0x1C, 0x96, 0x41);
                        SysRect(0x09, 0x1D, 0x95, 0x40);
                        _16A3 = _1699;
                        _1699 = __8EC7(0x0B, 0x1E, 0x96, 0x41, _1699, _1697+0x001E);
                    }
                }
                break;
            case CHAR_ENTER: // _659B
                if (_16A5+0x01 == 0x01)
                {
                    _65AA:
                    if (_17B7[_17CC]+0x01 != 0x01 && _1699+0x01 == 0x01)
                    {
                        _65F5:
                        _169A = __8F0A(DAT_GOODSRES, _8983[_17CC], _17B7[_17CC]);
                        if (MCU_memory[0x1935])
                        {
                            _66A8:
                            return;
                        }
                        _66AB:
                        _1697 = _169A;
                        SysLcdPartClear(0x08, 0x0A, 0x5A, 0x1C);
                        SysRect(0x09, 0x0B, 0x5A, 0x1C);
                        // 显示道具名称
                        SysPrintString(0x0B, 0x0C, _1697+0x0006);
                        SysLcdPartClear(0x08, 0x1C, 0x96, 0x41);
                        SysRect(0x09, 0x1D, 0x95, 0x40);
                        // 显示道具说明
                        _1699 = __8EC7(0x0B, 0x1E, 0x96, 0x41, _1699, _1697+0x001E);
                        _16A5 = 0x01;
                    }
                    else
                    {
                        _6823:
                        _16A4 = __8EA8(_17C8[_17CB], _17CC);
                        if (MCU_memory[0x1935])
                        {
                            _68B5:
                            return;
                        }
                        _68B8:
                        _16A4 = 0x02;
                    }
                }
                else
                {
                    _68C1:
                    _16A4 = __8EA8(_17C8[_17CB], _17CC);
                    if (MCU_memory[0x1935])
                    {
                        _6953:
                        return;
                    }
                    _6956:
                    _16A4 = 0x02;
                    _16A5 = 0x00;
                    if (_16A4)
                    {
                        _696B:
                        _1699 = 0x00;
                    }
                }
                break;
            case CHAR_EXIT: // _6974
                return;
                break;
            }
        }
        else if (_1694.type = DICT_WM_COMMAND)
        {
            _697A:
            if (_1694.param == CMD_RETURN_HOME)
            {
                _6995:
                MCU_memory[0x1935] = 0xFE;
                return;
            }
        }
    }
}

UINT8 _002319B4(UINT8 _17CF, UINT8 _17D0)
{
    const UINT8 _8983[] = {0x06,0x06,0x05,0x03,0x07,0x02,0x04,0x01};
    const UINT8 _89F3[] = "不能装备！";
    UINT8 _17CC;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT16 _17C7;
    UINT8 _17C6;
    UINT8* _17C4;
    UINT8 _17A6[0x11];
    UINT8* _17A4;
    UINT8 _17A1[3];
    UINT8* _179F;
    UINT16 _179D;
    // 如果背包中的道具种类==0
    if (MCU_memory[0x1A95]+0x01 == 0x01)
    {
        _69D5:
        return 0x00;
    }
    _69DA:
    // 指向角色装备
    _179D = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x13);
    switch (_17D0)
    {
    case 0x0000: // _6A40
        // 佩戴1
        _17C7 = _179D + 0x0006;
        break;
    case 0x0001: // _6A7C
        // 佩戴2
        _17C7 = _179D + 0x0007;
        break;
    case 0x0002: // _6AB8
        // 护腕
        _17C7 = _179D + 0x0003;
        break;
    case 0x0003: // _6AF4
        // 脚蹬
        _17C7 = _179D + 0x0005;
        break;
    case 0x0004: // _6B30
        // 手持
        _17C7 = _179D + 0x0004;
        break;
    case 0x0005: // _6B6C
        // 身穿
        _17C7 = _179D + 0x0001;
        break;
    case 0x0006: // _6BA8
        // 肩披
        _17C7 = _179D + 0x0002;
        break;
    case 0x0007: // _6BE4
        // 头戴
        _17C7 = _179D;
        break;
    }
    _6C13:
    _17A4 = MCU_memory + *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    _17C9 = _8983[_17D0];
    _17CC = 0x00;
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    _17CA = __8EB5(_17A4, MCU_memory[0x1A95], &_17C9, 0x01, 0x00, &_17CC);
    if (_17CA != 0xFF)
    {
        _6D31:
        _17C4 = __8F0A(DAT_GOODSRES, _17A4[_17CA * 0x0003], _17A4[_17CA*0x0003+0x01]);
        if (MCU_memory[0x1935])
        {
            _6DD9:
            return 0x00;
        }
        _6DDE:
        _179F = _17C4;
        // 装备使能判断，下面代码判断装备能否装备在指定主角身上
        _17CC = _179F[0x03]&(0x01 << (MCU_memory[_17CF * 0x0019 + 0x1988 + 0x01] - 0x01));
        if (_17CC == 0x00)
        {
            _6E55:
            _SysMemcpy(_17A6, _89F3, 0x0011);
            GuiMsgBox(_17A6, 0x0064);
        }
        else
        {
            _6EC7:
            if (MCU_memory[_17C7] != _17A4[_17CA*0x0003+0x01])
            {
                _6F1E:
                _17CC = __8EAB(_17CF, _17D0, _17CA * 0x0003 + _17A4);
                if (MCU_memory[0x1935])
                {
                    _6F9C:
                    return 0x00;
                }
            }
            else
            {
                _6FA4:
                _17CC = 0xFF;
            }
            _6FAA:
            if (_17CC == 0x00)
            {
                _6FB3:
                _17C6 = MCU_memory[_17C7];
                MCU_memory[_17C7] = _17A4[_17CA * 0x0003 + 0x01];
                _17A1[0] = _17A4[_17CA * 0x0003];
                _17A1[1] = _17A4[_17CA * 0x0003 + 0x01];
                _17A1[2] = 0x01;
                __8ECD(_17A1);
                __8D8D(_17A1[0], _17A1[1], 0x01);
                if (_17C6 >= 0x01)
                {
                    _7154:
                    _17A1[0] = _8983[_17D0];
                    _17A1[1] = _17C6;
                    _17A1[2] = 0x01;
                    __8ECA(_17A1);
                    _17C4 = __8F0A(DAT_GOODSRES, _17A1[0], _17A1[1]);
                    if (MCU_memory[0x1935])
                    {
                        _7239:
                        return 0x00;
                    }
                    _723E:
                    _179F = _17C4;
                    if (*(UINT16*)(_179F+0x84) != 0x0000)
                    {
                        _727C:
                        __8D90(*(UINT16*)(_179F+0x84));
                    }
                }
            }
        }
    }
    _72B2:
    return 0x00;
}

// _17D1:待装备道具
UINT8 _002322CA(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1)
{
    const UINT8 _8853[] = { 0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00 };
    const UINT8 _8983[] = {0x06,0x06,0x05,0x03,0x07,0x02,0x04,0x01};
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8* _17C7=NULL;
    UINT8* _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17B4[0x0E];
    UINT8 _17B3;
    UINT8 _17B2;
    UINT8 _17B1;
    UINT8 _17B0;
    UINT8 _17AF;
    UINT8 _17AE;
    UINT8* _17AC;
    UINT8 _17AA[2];
    UINT8 _17A2[0x08]; // 待装备道具属性
    UINT16 _179A[0x04]; // 已装备道具属性
    UINT8 _1792[0x08];
    UINT8* _178D;
    UINT8* _178B;
    UINT8 _1771[0x1A]; // 角色属性
    MsgType _176E;
    //                  指向角色属性
    _SysMemcpy(_1771, MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), 0x001A);
    // 指向角色装备
    _178B = MCU_memory + *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x13);
    _17AC = &_17B3;
    // 攻击的异常回合数
    _17AA[0] = _1771[0x19];
    switch (_17D0)
    {
    case 0x0000: // _742D
        // 指向佩戴1
        _17C7 = _178B + 0x0006;
        break;
    case 0x0001: // _7469
        // 指向佩戴2
        _17C7 = _178B + 0x0007;
        break;
    case 0x0002: // _74A5
        // 指向护腕
        _17C7 = _178B + 0x0003;
        _17AC = &_17AE;
        break;
    case 0x0003: // _750F
        // 指向脚蹬
        _17C7 = _178B + 0x0005;
        _17AC = &_17AF;
        break;
    case 0x0004: // _7579
        // 指向手持
        _17C7 = _178B + 0x0004;
        break;
    case 0x0005: // _75B5
        // 指向身穿
        _17C7 = _178B + 0x0001;
        _17AC = &_17B0;
        break;
    case 0x0006: // _761F
        // 指向肩披
        _17C7 = _178B + 0x0002;
        _17AC = &_17B1;
        break;
    case 0x0007: // _7689
        // 指向头戴
        _17C7 = _178B;
        _17AC = &_17B2;
        break;
    }
    _76E6:
    _17AE = 0x00;
    _17AF = 0x00;
    _17B0 = 0x00;
    _17B1 = 0x00;
    _17B2 = 0x00;
    if (_178B[0x03])
    {
        // 护腕类
        UINT8 _8985 = 0x05;
        _7782:
        _17C5 = __8F0A(DAT_GOODSRES, _8985, _178B[0x03]);
        if (MCU_memory[0x1935])
        {
            _77D3:
            return 0xFF;
        }
        _77D8:
        _178D = _17C5;
        // 护腕类抗性
        _17AE = *(UINT16*)(_178D + 0x0016 + 0x06);
    }
    _7843:
    if (_178B[0x05])
    {
        // 鞋类
        UINT8 _8986 = 0x03;
        _7858:
        _17C5 = __8F0A(DAT_GOODSRES, _8986, _178B[0x05]);
        if (MCU_memory[0x1935])
        {
            _78A9:
            return 0xFF;
        }
        _78AE:
        _178D = _17C5;
        // 鞋类抗性
        _17AF = *(UINT16*)(_178D + 0x0016 + 0x06);
    }
    _7919:
    if (_178B[0x01])
    {
        // 衣类
        UINT8 _8988 = 0x02;
        _792E:
        _17C5 = __8F0A(DAT_GOODSRES, _8988, _178B[0x01]);
        if (MCU_memory[0x1935])
        {
            _797F:
            return 0xFF;
        }
        _7984:
        _178D = _17C5;
        // 衣类抗性
        _17B0 = *(UINT16*)(_178D + 0x0016 + 0x06);
    }
    _79EF:
    if (_178B[0x02])
    {
        // 护甲类
        UINT8 _8989 = 0x04;
        _7A04:
        _17C5 = __8F0A(DAT_GOODSRES, _8989, _178B[0x02]);
        if (MCU_memory[0x1935])
        {
            _7A55:
            return 0xFF;
        }
        _7A5A:
        _178D = _17C5;
        // 护甲类抗性
        _17B1 = *(UINT16*)(_178D + 0x0016 + 0x06);
    }
    _7AC5:
    if (_178B[0x00])
    {
        // 冠类
        UINT8 _898A = 0x01;
        _7ADA:
        _17C5 = __8F0A(DAT_GOODSRES, _898A, _178B[0x00]);
        if (MCU_memory[0x1935])
        {
            _7B2B:
            return 0xFF;
        }
        _7B30:
        _178D = _17C5;
        // 冠类抗性
        _17B2 = *(UINT16*)(_178D + 0x0016 + 0x06);
    }
    _7B9B:
    SysLcdPartClear(0x00, 0x00, 0x24, 0x5F);
    if (*_17C7 >= 0x01)
    {
        _7BDF:
        _17C5 = __8F0A(DAT_GOODSRES, _8983[_17D0], *_17C7);
        if (MCU_memory[0x1935])
        {
            _7C48:
            return 0xFF;
        }
        _7C4D:
        _178D = _17C5;
        // 复制已装备道具属性
        _SysMemcpy(_179A, _178D + 0x0016, 0x0008);
        //                                  道具图号
        __8F04(DAT_GOODSPIC, _8983[_17D0], _178D[0x05], 0x00, 0x0B, 0x05);
        _17C3 = 0x01;
    }
    else
    {
        _7D4E:
        _17C3 = 0x00;
        _179A[0x00] = 0x0000;
        _179A[0x01] = 0x0000;
        _179A[0x02] = 0x0000;
        _179A[0x03] = 0x0000;
    }
    _7DF4:
    _17C5 = __8F0A(DAT_GOODSRES, _17D1[0x00], _17D1[0x01]);
    if (MCU_memory[0x1935])
    {
        _7E50:
        return 0xFF;
    }
    _7E55:
    _178D = _17C5;
    // 复制待装备道具属性
    _SysMemcpy(_17A2, _178D + 0x0016, 0x0008);
    if (_17D0 == 0x04)
    {
        _7EEC:
        // 道具持续回合
        _17AA[1] = _178D[0x04];
    }
    else
    {
        _7F20:
        _17AA[1] = _17AA[0];
    }
    _7F45:
    //                                  道具图号
    __8F04(DAT_GOODSPIC, _17D1[0x00], _178D[0x05], 0x00, 0x0B, 0x23-(0x01-_17C3)*0x18);
    //                          角色资源ID
    __8F04(DAT_SUNDRYPIC, 0x01, MCU_memory[_17CF * 0x0019 + 0x1988 + 0x01], 0x00, 0x08, 0x3C);
    _SysMemcpy(_17B4, _8853, 0x000E);
    SysPicture(0x01, 0x28 - (0x01 - _17C3) * 0x18, 0x07, 0x34 - (0x01 - _17C3) * 0x18, _17B4, 0x00);
    _17C9 = 0x00;
    _17CA = 0x01;
    _17C4 = 0x02;
    while (0x01)
    {
        _80F0:
        if (_17C4 == 0x01)
        {
            _80FC:
            __8EB8(_1771, _17C9);
            _17C4 = 0x00;
        }
        else if (_17C4 == 0x02)
        {
            _8159:
            __8E84(_17D1[0x00], _179A, _17A2, _1771);
            _17AC[0] = _17A2[6];
            // 整体抗性汇总
            _17CB = _17AE;
            _17CB |= _17AF;
            _17CB |= _17B0;
            _17CB |= _17B1;
            _17CB |= _17B2;
            _1771[1] = _17CB;
            _1771[0x19] = _17AA[_17CA];
            __8EB8(_1771, _17C9);
            _17C4 = 0x00;
        }
        _82D4:
        if (GuiGetMsg(&_176E) == 0x00)
        {
            _8309:
            continue;
        }
        _830C:
        if (GuiTranslateMsg(&_176E) == 0x00)
        {
            _8341:
            continue;
        }
        _8344:
        if (DICT_WM_CHAR_FUN == _176E.type)
        {
            _8375:
            switch (_176E.param)
            {
            case CHAR_UP: // _839F
                if (_17C3 == 0x01)
                {
                    _83AB:
                    if (_17CA == 0x01)
                    {
                        _83B7:
                        SysLcdPartClear(0x01, 0x07, 0x0A, 0x34);
                        SysPicture(0x01, 0x0A, 0x07, 0x16, _17B4, 0x00);
                        _SysMemcpy(_1792, _17A2, 0x0008);
                        _SysMemcpy(_17A2, _179A, 0x0008);
                        _SysMemcpy(_179A, _1792, 0x0008);
                        _17CA = 0x00;
                        _17C4 = 0x02;
                    }
                }
                break;
            case CHAR_DOWN: // _8571
                if (_17C3 == 0x01)
                {
                    _857D:
                    if (_17CA+0x01 == 0x01)
                    {
                        _858C:
                        SysLcdPartClear(0x01, 0x07, 0x0A, 0x34);
                        SysPicture(0x01, 0x28, 0x07, 0x34, _17B4, 0x00);
                        _SysMemcpy(_1792, _17A2, 0x0008);
                        _SysMemcpy(_17A2, _179A, 0x0008);
                        _SysMemcpy(_179A, _1792, 0x0008);
                        _17CA = 0x01;
                        _17C4 = 0x02;
                    }
                }
                break;
            case CHAR_PGUP: // _8746
            case CHAR_PGDN: // _8746
                _17C9++;
                _17C4 = 0x01;
                break;
            case CHAR_ENTER: // _875A
                if (_17CA == 0x01)
                {
                    _8766:
                    //          指向角色属性
                    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), _1771, 0x001A);
                    return 0x00;
                }
                _8802:
                return 0xFF;
                break;
            case CHAR_EXIT: // _880A
                return 0xFF;
                break;
            }
        }
        else if (DICT_WM_COMMAND == _176E.type)
        {
            _8815:
            if (_176E.param == CMD_RETURN_HOME)
            {
                _8830:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
}

void _00234000()
{
    const UINT8 _889F[] = "不能装备！";
    const UINT8 _88C6[] = "已装备！";
    UINT8* _17CA;
    UINT8 _17C7[3];
    UINT8* _17C5;
    UINT16 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT8 _17BC[3];
    UINT8 _17BB;
    UINT8 _17BA;
    UINT8 _1790[30];
    UINT8 _178F;
    UINT8 _178E;
    UINT8 _178D;
    UINT8* _178B;
    UINT8 _176D[0x11];
    UINT16 _176B;
    UINT8 _176A;
    UINT8 _1761[9];
    _1761[0] = 0x06;
    _1761[1] = 0x06;
    _1761[2] = 0x05;
    _1761[3] = 0x03;
    _1761[4] = 0x07;
    _1761[5] = 0x02;
    _1761[6] = 0x04;
    _1761[7] = 0x01;
    _1761[8] = 0x0E;
    _17CA = MCU_memory + *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    _17C1 = 0x00;
    _17C2 = 0x00;
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    while (0x01)
    {
        _518E:
        _17C2 = __8EB5(_17CA, MCU_memory[0x1A95], _1761, 0x08, _17C2, &_17C1);
        if (_17C2 == 0xFF)
        {
            _51FB:
            return;
        }
        _5201:
        _178B = __8F0A(DAT_GOODSRES, _17CA[_17C2 * 0x0003], _17CA[_17C2*0x0003+0x01]);
        if (MCU_memory[0x1935])
        {
            _52A9:
            return;
        }
        _52AC:
        _17C5 = _178B;
        _17C0 = 0x00;
        for (_17BB=0x00; _17BB<0x03; _17BB++)
        {
            _52E9:
            // 装备使能判断，下面代码判断装备能否装备在指定主角身上
            _17BA = _17C5[0x03]&(0x01 << (MCU_memory[_17BB * 0x0019 + 0x1988 + 0x01] - 0x01));
            if (_17BA)
            {
                _534A:
                _17BC[_17C0] = _17BB;
                //                                  指向角色姓名
                _SysMemcpy(_1790 + _17C0 * 0x0A, MCU_memory +*(UINT16*)(MCU_memory + _17BB * 0x0019 + 0x1988 + 0x0F), 0x000C);
                _17C0++;
            }
        }
        _5439:
        for (_17BB=0x00; _17BB<_17C0*0x0A; _17BB++)
        {
            _5467:
            if (_1790[_17BB]+0x01 == 0x01)
            {
                _548D:
                for (; _17BB%0x0A>=0x01; _17BB++)
                {
                    _54AF:
                    _1790[_17BB] = 0x20;
                }
            }
        }
        _54E3:
        if (_17C0+0x01 == 0x01)
        {
            _54F2:
            _SysMemcpy(_176D, _889F, 0x0011);
            GuiMsgBox(_176D, 0x0064);
            continue;
        }
        _5564:
        _178F = 0x00;
        if (_17C0 > 0x01)
        {
            _557B:
            _178E = 0x00;
            _178F = __8EB2(0x30, 0x0F, 0x0A, _17C0, _1790, 0x00, &_178E);
            if (_178F == 0xFF)
            {
                _55E6:
                SysLcdPartClear(0x2F, 0x0E, 0x82, 0x46);
                continue;
            }
        }
        _561D:
        // 指向角色装备
        _17C3 = *(UINT16*)(MCU_memory+ _17BC[_178F] *0x0019+0x1988+0x13);
        switch (_17CA[_17C2*0x0003])
        {
        case 0x0006: // _56D3
            if (MCU_memory[_17C3+0x07] >= 0x01)
            {
                _56EB:
                _176B = _17C3 + 0x0006;
                _178D = 0x00;
            }
            else
            {
                _572D:
                _176B = _17C3 + 0x0007;
                _178D = 0x01;
            }
            break;
        case 0x0005: // _576F
            _176B = _17C3 + 0x0003;
            _178D = 0x02;
            break;
        case 0x0003: // _57B1
            _176B = _17C3 + 0x0005;
            _178D = 0x03;
            break;
        case 0x0007: // _57F3
            _176B = _17C3 + 0x0004;
            _178D = 0x04;
            break;
        case 0x0002: // _5835
            _176B = _17C3 + 0x0001;
            _178D = 0x05;
            break;
        case 0x0004: // _5877
            _176B = _17C3 + 0x0002;
            _178D = 0x06;
            break;
        case 0x0001: // _58B9
            _176B = _17C3;
            _178D = 0x07;
            break;
        }
        _58EE:
        if (MCU_memory[_176B] != _17CA[_17C2*0x0003+0x01])
        {
            _5945:
            _17BB = __8EAB(_17BC[_178F], _178D, _17C2 * 0x0003 + _17CA);
            SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
        }
        else
        {
            _5A11:
            _SysMemcpy(_176D, _88C6, 0x0011);
            GuiMsgBox(_176D, 0x0064);
            _17BB = 0xFF;
            SysLcdPartClear(0x2F, 0x0E, 0x82, 0x46);
        }
        _5ABA:
        if (_17BB == 0x00)
        {
            _5AC3:
            _176A = MCU_memory[_176B];
            MCU_memory[_176B] = _17CA[_17C2 * 0x0003 + 0x01];
            _17C7[0] = _17CA[_17C2 * 0x0003];
            _17C7[1] = _17CA[_17C2 * 0x0003 + 0x01];
            _17C7[2] = 0x01;
            __8ECD(_17C7);
            _00237631(_17C7[0], _17C7[1], 0x01);
            if (_176A >= 0x01)
            {
                _5C5C:
                _17C7[0] = _1761[_178D];
                _17C7[1] = _176A;
                _17C7[2] = 0x01;
                __8ECA(_17C7);
                _00237631(_17C7[0], _17C7[1], 0x00);
            }
            _5D2E:
            SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
        }
    }
}

UINT8 _00234D76()
{
    const UINT8 _88CF[] = "战斗中才能使用！";
    UINT8 _17C7[7];
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17B3[0x11];
    UINT8* _17B1;
    _17C7[0] = 0x08;
    _17C7[1] = 0x09;
    _17C7[2] = 0x0A;
    _17C7[3] = 0x0B;
    _17C7[4] = 0x0C;
    _17C7[5] = 0x0D;
    _17C7[6] = 0x0E;
    // 指向第MCU_memory[0x1A94]个角色背包中的道具
    _17B1 = MCU_memory + *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    _17C5 = 0x00;
    _17C6 = 0x00;
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    while (0x01)
    {
        _5ECE:
        _17C6 = __8EB5(_17B1, MCU_memory[0x1A95], _17C7, 0x07, _17C6, &_17C5);
        if (_17C6 == 0xFF)
        {
            _5F3B:
            return 0x00;
        }
    _5F40:
        //                      暗器类                         兴奋剂
        if (_17B1[_17C6*0x0003] == 0x08 || _17B1[_17C6*0x0003] == 0x0C)
        {
            _5FBA:
            _SysMemcpy(_17B3, _88CF, 0x0011);
            GuiMsgBox(_17B3, 0x0064);
        }
        //                          剧情类
        else if (_17B1[_17C6*0x0003] == 0x0E)
        {
            _6069:
            MCU_memory[0x1A9A] = _17B1[_17C6*0x0003+0x01];
            return 0x01;
        }
        else
        {
            _60A9:
            if (__8D7E(_17C6 * 0x0003 + _17B1) == 0x01)
            {
                _6118:
                return 0x01;
            }
            _611D:
            SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
        }
    }
    _6154:
    return 0x00;
}

UINT8 _0023516C(UINT8* _17D0)
{
    const UINT8 _88E0[] = "无效！";
    UINT8 _17CB;
    UINT8* _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C3[3];
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17AD[0x08];
    UINT16 _17A5[0x04];
    UINT16 _179D[0x04];
    UINT8* _179B;
    UINT8 _1798[3];
    UINT8* _1796=NULL;
    MsgType _1793;
    if (_17D0[0x00] == 0x0D)
    {
        _6193:
        _17C8 = __8DB4(0xFF);
        if (_17C8 == 0x01)
        {
            _61B0:
            _SysMemcpy(_17AD, _88E0, 0x0008);
            GuiMsgBox(_17AD, 0x0064);
            return 0x00;
        }
        _6224:
        return 0x01;
    }
    _6229:
    _17C9 = __8F0A(DAT_GOODSRES, _17D0[0x00], _17D0[0x01]);
    if (MCU_memory[0x1935])
    {
        _6285:
        return 0x01;
    }
    _628A:
    SysLcdPartClear(0x00, 0x00, 0x24, 0x5F);
    _179B = _17C9;
    // 复制道具属性
    _SysMemcpy((UINT8*)_17A5, _179B + 0x0016, 0x0008);
    __8F04(DAT_GOODSPIC, _179B[0x00], _179B[0x05], 0x00, 0x0B, 0x0A);
    _17C7 = _17D0[0x02];
    __8F1F(0x0F, 0x22, (UINT32)_17C7, 0x0A, 0x01);
    _179D[0] = 0x0000;
    _179D[1] = 0x0000;
    _179D[2] = 0x0000;
    _179D[3] = 0x0000;
    _17C2 = 0x00;
    for (UINT8 _17C6=0x00; _17C6<0x03; _17C6++)
    {
        _64B3:
        // 角色资源ID
        if (MCU_memory[_17C6*0x0019+0x1988+0x01] >= 0x01)
        {
            _64ED:
            _17C3[_17C2] = _17C6;
            _17C2++;
        }
    }
    _652B:
    _17C1 = 0x00;
    _17CB = 0x00;
    _17C8 = 0x01;
    while (0x01)
    {
        _6544:
        if (_17C8 == 0x01)
        {
            _6550:
            // 指向角色属性
            _1796 = MCU_memory + *(UINT16*)(MCU_memory+ _17C3[_17C1] *0x0019+0x1988+0x11);
            SysLcdPartClear(0x04, 0x3C, 0x24, 0x5E);
            //                          角色资源ID
            __8F04(DAT_SUNDRYPIC, 0x01, MCU_memory[_17C3[_17C1] * 0x0019 + 0x1988 + 0x01], 0x00, 0x08, 0x3C);
            __8EB8(_1796, _17CB);
            _17C8 = 0x00;
        }
        _66A3:
        if (GuiGetMsg(&_1793) == 0x00)
        {
            _66D8:
            continue;
        }
        _66DB:
        if (GuiTranslateMsg(&_1793) == 0x00)
        {
            _6710:
            continue;
        }
        _6713:
        if (_1793.type == DICT_WM_CHAR_FUN)
        {
            _6744:
            switch (_1793.param)
            {
            case CHAR_LEFT: // _676E
                if (_17C1 >= 0x01)
                {
                    _677A:
                    _17C1--;
                    _17C8 = 0x01;
                }
                break;
            case CHAR_RIGHT: // _678E
                if (_17C1 < _17C2-0x01)
                {
                    _67A3:
                    _17C1++;
                    _17C8 = 0x01;
                }
                break;
            case CHAR_PGUP: // _67B7
            case CHAR_PGDN: // _67B7
                _17CB++;
                _17C8 = 0x01;
                break;
            case CHAR_ENTER: // _67CB
                if (_17C7 >= 0x01)
                {
                    _67D7:
                    if ((_17A5[3] & 0x0010) == 0x0010 && (_17D0[0x00] == 0x09 || _17D0[0x00] == 0x0B))
                    {
                        _682C:
                        for (UINT8 _17C6=0x00; _17C6<_17C2; _17C6++)
                        {
                            _684F:
                            // 指向角色属性
                            _1796 = MCU_memory + *(UINT16*)(MCU_memory+ _17C3[_17C6] *0x0019+0x1988+0x11);
                            __8E84(_17D0[0x00], _179D, _17A5, _1796);
                        }
                        _6916:
                        // 指向角色属性
                        _1796 = MCU_memory + *(UINT16*)(MCU_memory+ _17C3[_17C1] *0x0019+0x1988+0x11);
                    }
                    else
                    {
                        _697D:
                        if (_17D0[0x00] != 0x0A)
                        {
                            _6994:
                            __8E84(_17D0[0x00], _179D, _17A5, _1796);
                        }
                    }
                    _69F4:
                    _1798[0] = _17D0[0x00];
                    _1798[1] = _17D0[0x01];
                    _1798[2] = 0x01;
                    __8ECD(_1798);
                    _00237631(_1798[0], _1798[1], 0x01);
                    _17C7--;
                    SysLcdPartClear(0x0F, 0x22, 0x24, 0x3C);
                    __8F1F(0x0F, 0x22, (UINT32)_17C7, 0x0A, 0x01);
                    if (_17C7+0x01 == 0x01)
                    {
                        _6B49:
                        SysLcdPartClear(0x00, 0x00, 0x24, 0x3C);
                    }
                    _6B75:
                    _17C8 = 0x01;
                }
                else
                {
                    _6B7E:
                    return 0x00;
                }
                break;
            case CHAR_EXIT: // _6B86
                return 0x00;
                break;
            }
        }
        else if (_1793.type == DICT_WM_COMMAND)
        {
            _6B91:
            if (_1793.param == CMD_RETURN_HOME)
            {
                _6BAC:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
    _6BBC:
    return 0x00;
}

void _00235BD4(UINT8 _17CF)
{
    const UINT8 _88FF[] = "真气不足！";
    const UINT8 _890A[] = "此处无法使用！";
    UINT8 _17B9[0x10];
    UINT8 _17B8;
    UINT8* _17B6;
    UINT8 _17B5;
    UINT8 _17B4;
    UINT16 _17AA;
    UINT8* _17A8;
    // 指向角色魔法链数据
    _17AA = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x15);
    _17B8 = 0x00;
    while (0x01)
    {
        _6C39:
        //                      角色学会
        _17B8 = __8EAF(_17AA, MCU_memory[_17CF * 0x0019 + 0x1988 + 0x0E], _17B8);
        if (_17B8 == 0xFF)
        {
            _6CB8:
            return;
        }
        _6CBB:
        if (MCU_memory[_17B8*0x0002+_17AA] == 0x03)
        {
            _6CF8:
            _17B6 = __8F0A(DAT_MGICRES, MCU_memory[_17B8 * 0x0002 + _17AA], MCU_memory[_17B8*0x0002+_17AA+0x01]);
            if (MCU_memory[0x1935])
            {
                _6DA0:
                return;
            }
            _6DA3:
            _17A8 = _17B6;
            // 耗费真气
            _17B4 = _17A8[0x04];
            // mp
            if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x0C) >= _17B4)
            {
                _6E25:
                _17B5 = __8D84(_17B8 * 0x0002 + _17AA);
                if (_17B5 != 0xFF)
                {
                _6E9C:
                    // mp
                    *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x000C) = *(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0C) - _17B4;
                }
            }
            else
            {
                _6F6C:
                _SysMemcpy(_17B9, _88FF, 0x000C);
                GuiMsgBox(_17B9, 0x0064);
            }
        }
        else
        {
            _6FDE:
            _SysMemcpy(_17B9, _890A, 0x0010);
            GuiMsgBox(_17B9, 0x0064);
        }
    }
}

UINT8 _00236061(UINT16 _17D0)
{
    UINT8 _17BF;
    UINT8 _17BE;
    UINT8 _17BB[3];
    UINT8 _17BA;
    UINT8 _17B9;
    UINT8 _17B7;
    UINT8* _17B5;
    UINT16 _17AD[0x04];
    MsgType _17AA;
    UINT8* _17A8;
    _17B5 = __8F0A(DAT_MGICRES, MCU_memory[_17D0], MCU_memory[_17D0+0x01]);
    if (MCU_memory[0x1935])
    {
        _70CD:
        return 0xFF;
    }
    _70D2:
    _17A8 = _17B5;
    // 复制法术属性
    _SysMemcpy((UINT8*)_17AD, _17A8 + 0x0012, 0x0008);
    // 法术效果
    _17B7 = _17A8[0x03];
    SysLcdPartClear(0x00, 0x00, 0x24, 0x5F);
    //                                  法术名称
    __8EC7(0x04, 0x05, 0x24, 0x36, 0x00, _17A8+0x0006);
    _17BA = 0x00;
    for (UINT8 _17B8=0x00; _17B8<0x03; _17B8++)
    {
        _7210:
        // 角色资源ID
        if (MCU_memory[_17B8*0x0019+0x1988+0x01] >= 0x01)
        {
            _724A:
            _17BB[_17BA] = _17B8;
            _17BA++;
        }
    }
    _7288:
    if (_17BA < 0x01)
    {
        _7294:
        return 0xFF;
    }
    _7299:
    _17BF = 0x02;
    _17B9 = 0x00;
    _17BE = 0x00;
    while (0x01)
    {
        _72B2:
        if (_17BF == 0x01)
        {
            _72BE:
            //                  指向角色属性
            __8EB8(MCU_memory + *(UINT16*)(MCU_memory + _17BB[_17B9] * 0x0019 + 0x1988 + 0x11), _17BE);
            _17BF = 0x00;
        }
        else if (_17BF == 0x02)
        {
            _7367:
            SysLcdPartClear(0x00, 0x3C, 0x24, 0x5F);
            //                          角色资源ID
            __8F04(DAT_SUNDRYPIC, 0x01, MCU_memory[_17BB[_17B9] * 0x0019 + 0x1988 + 0x01], 0x00, 0x08, 0x3C);
            //                  指向角色属性
            __8EB8(MCU_memory + *(UINT16*)(MCU_memory + _17BB[_17B9] * 0x0019 + 0x1988 + 0x11), _17BE);
            _17BF = 0x00;
        }
        _74BA:
        if (GuiGetMsg(&_17AA) == 0x00)
        {
            _74EF:
            continue;
        }
        _74F2:
        if (GuiTranslateMsg(&_17AA) == 0x00)
        {
            _7527:
            continue;
        }
        _752A:
        if (_17AA.type == DICT_WM_CHAR_FUN)
        {
            _755B:
            switch (_17AA.param)
            {
            case CHAR_LEFT: // _7585
                if (_17BA > 0x01)
                {
                    _7596:
                    if (_17B9 < 0x01)
                    {
                        _75A2:
                        _17B9 = _17BA;
                    }
                    _75AA:
                    _17B9--;
                    _17BF = 0x02;
                }
                break;
            case CHAR_RIGHT: // _75BE
                if (_17BA > 0x01)
                {
                    _75CF:
                    _17B9++;
                    _17B9 %= _17BA;
                    _17BF = 0x02;
                }
                break;
            case CHAR_PGUP: // _75F4
            case CHAR_PGDN: // _75F4
                _17BE++;
                _17BF = 0x01;
                break;
            case CHAR_ENTER: // _7608
                if (_17B7 >= 0x80)
                {
                    _7614:
                    for (UINT8 _17B8=0x00; _17B8<_17BA; _17B8++)
                    {
                        _7637:
                        __8E7E(MCU_memory[_17D0], _17AD, _17BB[_17B8]);
                        if (MCU_memory[0x1935])
                        {
                            _769F:
                            return 0xFF;
                        }
                    }
                }
                else
                {
                    _76AA:
                    __8E7E(MCU_memory[_17D0], _17AD, _17BB[_17B9]);
                }
                _770A:
                return 0x00;
                break;
            case CHAR_EXIT: // _7712
                return 0xFF;
                break;
            }
        }
        else if (_17AA.type == DICT_WM_COMMAND)
        {
            _771D:
            if (_17AA.param == CMD_RETURN_HOME)
            {
                _7738:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
}

UINT8 _0023675B(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT32 _17D3)
{
    const UINT8 _8931[] = "金钱：";
    const UINT8 _8938[] = ":\0卖出个数  ：";
    const UINT8 _893A[] = "卖出个数  ：";
    const UINT8 _8947[] = "买入个数  ：";
    const UINT32 _8954 = 0x0001019F;
    const UINT32 _8958 = 0x0001019F;
    const UINT8 _895C[] = "金钱不足！";
    const UINT32 _8967 = 0x0001019F;
    const UINT32 _896B = 0x0001019F;
    UINT8 _17BB[0x0C];
    // 背包中可能会达到的道具数量，总道具数量不大于99
    UINT8 _17BA;
    // 购买/出售的道具数量
    UINT8 _17B9;
    UINT8 _17B8;
    MsgType _17A9;
    UINT32 _17B0 = *(UINT32*)(MCU_memory+0x1A8F); // 玩家拥有的金钱
    // 玩家可能拥有的金钱，不大于0x0001019F
    UINT32 _17AC = _17B0;
    UINT32 _17B4 = _17D3;
    _17BA = _17D0;
    _17B9 = 0x00;
    SysLcdPartClear(0x0C, 0x15, 0x93, 0x4B);
    SysRect(0x0D, 0x16, 0x92, 0x4A);
    _SysMemcpy(_17BB, _8931, 0x000C);
    SysPrintString(0x0F, 0x18, _17BB);
    _SysMemcpy(_17BB, _17D1, 0x000C);
    SysPrintString(0x0F, 0x28, _17BB);
    _SysMemcpy(_17BB, _8938, 0x000C);
    SysPrintString(0x5D, 0x28, _17BB);
    if (_17CF == 0x01)
    {
        // 出售道具
        _799E:
        _SysMemcpy(_17BB, _893A, 0x0010);
        SysPrintString(0x0F, 0x38, _17BB);
    }
    else
    {
        // 购买道具
        _7A0C:
        _SysMemcpy(_17BB, _8947, 0x0010);
        SysPrintString(0x0F, 0x38, _17BB);
    }
    _7A77:
    // 是否需要刷新界面
    _17B8 = 0x01;
    while (0x01)
    {
        _7A84:
        if (_17B8 == 0x01)
        {
            // 刷新界面显示
            _7A90:
            SysLcdPartClear(0x40, 0x18, 0x8C, 0x27);
            SysLcdPartClear(0x6D, 0x28, 0x91, 0x49);
            __8F1F(0x40, 0x18, _17AC, 0x0A, 0x01);
            __8F1F(0x6D, 0x28, (UINT32)_17BA, 0x0A, 0x01);
            __8F1F(0x6D, 0x38, (UINT32)_17B9, 0x0A, 0x01);
            _17B8 = 0x00;
        }
        _7BA6:
        if (GuiGetMsg(&_17A9) == 0x00)
        {
            _7BDB:
            continue;
        }
        _7BDE:
        if (GuiTranslateMsg(&_17A9) == 0x00)
        {
            _7C13:
            continue;
        }
        _7C16:
        if (_17A9.type == DICT_WM_CHAR_FUN)
        {
            _7C22:
            if (_17A9.param == CHAR_UP)
            {
                _7C3D:
                if (_17CF == 0x01)
                {
                    _7C49:
                    if (_17B9 >= 0x01)
                    {
                        _7C55:
                        _17B9--;
                        _17BA++;
                        _17B0 -= _17B4;
                        _17AC = _17B0 > _8954 ? _8958 : _17B0;
                        _17B8 = 0x01;
                    }
                }
                else
                {
                    _7D01:
                    if (_17AC >= _17B4)
                    {
                        _7D23:
                        if (_17BA < 0x63)
                        {
                            _7D2F:
                            _17BA++;
                            _17B9++;
                            _17AC -= _17B4;
                            _17B8 = 0x01;
                        }
                    }
                    else
                    {
                        _7D83:
                        _SysMemcpy(_17BB, _895C, 0x000C);
                        GuiMsgBox(_17BB, 0x0064);
                    }
                }
            }
            else if (_17A9.param == CHAR_DOWN)
            {
                _7E10:
                if (_17CF == 0x01)
                {
                    _7E1C:
                    if (_17BA >= 0x01)
                    {
                        _7E28:
                        _17BA--;
                        _17B9++;
                        _17B0 += _17B4;
                        _17AC = _17B0 > _8967 ? _896B : _17B0;
                        _17B8 = 0x01;
                    }
                }
                else
                {
                    _7ED4:
                    if (_17B9 >= 0x01)
                    {
                        _7EE0:
                        _17BA--;
                        _17B9--;
                        _17AC += _17B4;
                        _17B8 = 0x01;
                    }
                }
            }
            else if (_17A9.param == CHAR_ENTER)
            {
                _7F4F:
                if (_17B9+0x01 == 0x01)
                {
                    _7F5E:
                    return 0xFF;
                }
                return _17B9;
            }
            else if (_17A9.param == CHAR_EXIT)
            {
                _7F88:
                return 0xFF;
            }
        }
        else if (_17A9.type == DICT_WM_COMMAND)
        {
            _7F9C:
            if (_17A9.param == CMD_RETURN_HOME)
            {
                _7FB7:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
}

void _00236FD7()
{
    const UINT8 _896F[] = "没携带物品！";
    const UINT8 _897C[] = "不可卖物品！";
    const UINT32 _8989 = 0x0001019F;
    const UINT32 _898D = 0x0001019F;
    UINT32 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8* _17C1;
    UINT8 _17C0;
    UINT8 _17BF;
    UINT8* _17BD;
    UINT8* _17BB;
    UINT8 _17B8[3];
    UINT8 _17A9[0x0E];
    // 如果背包中的道具种类==0
    if (MCU_memory[0x1A95]+0x01 == 0x01)
    {
        _7FF5:
        _SysMemcpy(_17A9, _896F, 0x000E);
        GuiMsgBox(_17A9, 0x0064);
        return;
    }
    _8067:
    _17BB = MCU_memory + *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    _17C4 = 0x00;
    _17C5 = 0x00;
    while (0x01)
    {
        _80F2:
        _17C5 = __8EC1(0x03, MCU_memory[0x1A95], _17BB, _17C5, &_17C4);
        if(_17C5 == 0xFF)
        {
            _814C:
            break;
        }
        _8152:
        _17C1 = __8F0A(DAT_GOODSRES, _17BB[_17C5 * 0x0003], _17BB[_17C5*0x0003+0x01]);
        if (MCU_memory[0x1935])
        {
            _81FA:
            return;
        }
        _81FD:
        _17BD = _17C1;
        if (*(UINT16*)(_17BD+0x84) >= 0x0001)
        {
            _823B:
            _SysMemcpy(_17A9, _897C, 0x000E);
            GuiMsgBox(_17A9, 0x0064);
            continue;
        }
        _82AD:
        _17C0 = _17BB[_17C5*0x0003+0x02];
        // 道具当价
        _17C6 = (UINT32)*(UINT16*)(_17BD+0x14);
        // 出售道具
        _17BF = __8D87(0x01, _17C0, _17BD + 0x0006, _17C6);
        if (_17BF != 0xFF)
        {
            _8390:
            _17C6 *= (UINT32)_17BF;
            // 玩家拥有的金钱
            *(UINT32*)(MCU_memory+0x1A8F) += _17C6;
            // 玩家拥有的金钱
            if (*(UINT32*)(MCU_memory+0x1A8F) > _8989)
            {
            _8409:
                // 玩家拥有的金钱
                *(UINT32*)(MCU_memory+0x1A8F) = _898D;
            }
            _841C:
            if (_17BF == _17BB[_17C5*0x0003+0x02])
            {
                _845F:
                SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
            }
            else
            {
                _8496:
                SysLcdPartClear(0x0C, 0x15, 0x93, 0x4B);
            }
            _84CA:
            _17B8[0] = _17BB[_17C5 * 0x0003];
            _17B8[1] = _17BB[_17C5 * 0x0003 + 0x01];
            _17B8[2] = _17BF;
            __8ECD(_17B8);
        }
        else
        {
            _85C1:
            SysLcdPartClear(0x0C, 0x15, 0x93, 0x4B);
        }
    }
    _85F8:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

void _00237631(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    UINT16 _17CB;
    UINT8* _17C9 = __8F0A(DAT_GOODSRES, _17CF, _17D0);
    if (MCU_memory[0x1935])
    {
        _8682:
        return;
    }
    _8685:
    _17CB = *(UINT16*)(_17C9+0x84);
    if (_17CB != 0x0000)
    {
        _86C3:
        if (_17D1)
        {
            _86CC:
            MCU_memory[(_17CB>>0x0003)+*(UINT16*)(MCU_memory+0x1AA9)] |= 0x0001 << (_17CB % 0x0008);
            return;
        }
        _878C:
        __8D90(_17CB);
    }
}

void _002377C6(UINT16 _17D0)
{
    UINT16 _17CD = _17D0>>0x0003;
    UINT16 _17CB = _17D0%0x0008;
    UINT8 _17CA = 0x0001<<_17CB;
    _17CA ^= 0xFF;
    MCU_memory[_17CD+*(UINT16*)(MCU_memory+0x1AA9)] = MCU_memory[_17CD + *(UINT16*)(MCU_memory + 0x1AA9)] & _17CA;
}

UINT8 _00238000(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT8 _17D3, UINT8* _17D4)
{
    UINT8 _8438[0x0E] = { 0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00 };
    UINT8 _8446[] = "数量：";
    UINT8 _844D[] = "价：";
    UINT8 _8452[] = "名：";
    MsgType _17C8;
    UINT8 _17C3[0x05];
    UINT8 _17BE[0x05];
    UINT8 _17B7[0x07];
    UINT8 _17A9[0x0E];
    UINT8 _17A8;
    UINT8 _17A7;
    UINT8 _17A6;
    UINT8 _17A5;
    UINT8 _17A4;
    UINT8 _17A3;
    UINT8 _17A2;
    UINT8* _17A0;
    UINT8 _179E;
    UINT8* _179C;
    if (_17D0+0x01 == 0x01)
    {
        _5022:
        return 0xFF;
    }
    _5027:
    _17D0--;
    if (_17D3 > _17D0)
    {
        _5043:
        _17D3 = _17D0;
    }
    _504B:
    if (*_17D4+0x03 > _17D0)
    {
        _506C:
        if (_17D0 <= 0x03)
        {
            _507A:
            *_17D4 = 0x00;
        }
        else
        {
            _5096:
            *_17D4 = _17D0 - 0x03;
        }
    }
    _50BC:
    if (*_17D4 > _17D3)
    {
        _50DA:
        _17D3 = *_17D4;
    }
    _50ED:
    _17A8 = *_17D4;
    _17A7 = _17D3;
    _17A6 = 0x00;
    _179E = 0x11;
    SysLine(0x00, 0x05, 0x05, 0x00);
    SysLine(0x05, 0x00, 0x27, 0x00);
    SysLine(0x00, 0x05, 0x00, 0x5F);
    SysLine(0x28, 0x15, 0x28, 0x5F);
    SysLine(0x29, 0x14, 0x29, 0x5F);
    SysLine(0x00, 0x5F, 0x27, 0x5F);
    SysLine(0x27, 0x00, 0x3A, 0x13);
    SysLine(0x27, 0x01, 0x39, 0x13);
    SysLine(0x3A, 0x13, 0x8C, 0x13);
    SysLine(0x2A, 0x14, 0x8C, 0x14);
    SysLine(0x36, 0x00, 0x8C, 0x00);
    SysLine(0x2A, 0x3A, 0x8C, 0x3A);
    SysRect(0x2A, 0x15, 0x9E, 0x39);
    SysRect(0x2A, 0x3B, 0x9E, 0x5F);
    _SysMemcpy(_17B7, _8446, 0x0007);
    _SysMemcpy(_17C3, _844D, 0x0005);
    _SysMemcpy(_17BE, _8452, 0x0005);
    _SysMemcpy(_17A9, _8438, 0x000E);
    SysPrintString(0x2E, 0x17, _17BE);
    SysPrintString(0x2E, 0x28, _17C3);
    SysPicture(0x04, (_17A7 - _17A8) * 0x17 + 0x08, 0x0A, (_17A7 - _17A8) * 0x17 + 0x14, _17A9, 0x00);
    while (0x01)
    {
        _556E:
        if ((_179E & 0x10) == 0x10)
        {
            _557C:
            for (UINT8 _179F=0x00; _179F<0x04; _179F++)
            {
                _559D:
                if (_17A8+_179F <= _17D0)
                {
                    _55B2:
                    _17A0 = __8F0A(DAT_GOODSRES, _17D1[(_17A8 + _179F) * 0x0003], _17D1[(_17A8+_179F)*0x0003+0x01]);
                    if (MCU_memory[0x1935])
                    {
                        _5664:
                        return 0xFF;
                    }
                    _5669:
                    _179C = _17A0;
                    _17A0 = __8F0A(DAT_GOODSPIC, _17D1[(_17A8 + _179F) * 0x0003], _179C[0x05]);
                    if (MCU_memory[0x1935])
                    {
                        _5707:
                        return 0xFF;
                    }
                    _570C:
                    _17A5 = _17A0[0x0002];
                    _17A4 = _17A0[0x0003];
                    _17A3 = 0x18-_17A5;
                    _17A3 >>= 0x01;
                    _17A2 = 0x18-_17A4;
                    _17A2 >>= 0x01;
                    _17A0 = _17A0+0x0006;
                    SysLcdPartClear(0x0E, _179F * 0x17 + 0x01, 0x25, _179F*0x17+0x18);
                    SysPicture(_17A3 + 0x0E, _17A2 + 0x01+ _179F * 0x17, 0x0D + _17A3 + _17A5, _17A2 + _17A4+ _179F * 0x17, _17A0, 0x00);
                }
            }
            _58AC:
            _179E &= 0x01;
        }
        _58B6:
        if ((_179E & 0x01) == 0x01)
        {
            _58C4:
            _17A0 = __8F0A(DAT_GOODSRES, _17D1[_17A7 * 0x0003], _17D1[_17A7*0x0003+0x01]);
            if (MCU_memory[0x1935])
            {
                _596C:
                return 0xFF;
            }
            _5971:
            _179C = _17A0;
            SysLcdPartClear(0x4A, 0x17, 0x9B, 0x37);
            // 显示道具名称
            SysPrintString(0x4A, 0x17, _179C+0x0006);
            if ((_17CF&0x01)+0x01 == 0x01)
            {
                _5A09:
                // 道具买价
                __8F1F(0x4A, 0x29, (UINT32)* (UINT16*)(_179C + 0x12), 0x0A, 0x01);
            }
            else
            {
                _5A6C:
                // 道具当价
                __8F1F(0x4A, 0x29, (UINT32)* (UINT16*)(_179C + 0x14), 0x0A, 0x01);
            }
            _5ACC:
            if ((_17CF & 0x02) == 0x02)
            {
                _5ADA:
                SysLcdPartClear(0x3D, 0x01, 0x9E, 0x12);
                SysPrintString(0x3C, 0x02, _17B7);
                __8F1F(0x6E, 0x03, (UINT32)_17D1[_17A7 * 0x0003 + 0x02], 0x0A, 0x01);
            }
            _5BAF:
            SysLcdPartClear(0x2E, 0x3D, 0x9D, 0x5E);
            // xxx道具说明
            _17A6 = __8EC7(0x2E, 0x3D, 0x9D, 0x5E, _17A6, _179C+0x001E);
            _179E &= 0x10;
        }
        _5C49:
        if (GuiGetMsg(&_17C8) == 0x00)
        {
            _5C7E:
            continue;
        }
        _5C81:
        if (GuiTranslateMsg(&_17C8) == 0x00)
        {
            _5CB6:
            continue;
        }
        _5CB9:
        if (_17C8.type == DICT_WM_CHAR_FUN)
        {
            _5CC5:
            if (_17C8.param == CHAR_PGDN)
            {
                _5CE0:
                _179E = 0x01;
            }
            else if (_17C8.param == CHAR_PGUP)
            {
                _5D04:
                if (_17A6 > 0x01)
                {
                    _5D10:
                    _17A6 -= 0x02;
                    _179E = 0x01;
                }
                else if (_17A6 == 0x01)
                {
                    _5D35:
                    _17A6 = 0x00;
                    _179E = 0x01;
                }
            }
            else if (_17C8.param == CHAR_DOWN)
            {
                _5D5F:
                SysLcdPartClear(0x04, 0x07, 0x0A, 0x5E);
                if (_17A7-_17A8 < 0x03 && _17A7 < _17D0)
                {
                    _5DAA:
                    _17A7++;
                    _17A6 = 0x00;
                    _179E = 0x01;
                }
                else if (_17A7-_17A8 == 0x03 && _17A7 < _17D0)
                {
                    _5DE1:
                    _17A7++;
                    _17A8++;
                    _17A6 = 0x00;
                    _179E = 0x11;
                }
                _5DFF:
                SysPicture(0x04, (_17A7 - _17A8) * 0x17 + 0x08, 0x0A, (_17A7 - _17A8) * 0x17 + 0x14, _17A9, 0x00);
            }
            else if (_17C8.param == CHAR_UP)
            {
                _5E90:
                SysLcdPartClear(0x04, 0x07, 0x0A, 0x5E);
                if (_17A7-_17A8+0x01 > 0x01)
                {
                    _5ED5:
                    _17A7--;
                    _17A6 = 0x00;
                    _179E = 0x01;
                }
                else if (_17A7-_17A8+0x01 == 0x01)
                {
                    _5F01:
                    if (_17A7-_17A8+0x01 >= 0x01 && _17A7)
                    {
                        _5F0F:
                        _17A7--;
                        _17A8--;
                        _17A6 = 0x00;
                        _179E = 0x11;
                    }
                }
                _5F2D:
                SysPicture(0x04, (_17A7 - _17A8) * 0x17 + 0x08, 0x0A, (_17A7 - _17A8) * 0x17 + 0x14, _17A9, 0x00);
            }
            else if (_17C8.param == CHAR_ENTER)
            {
                _5FBE:
                *_17D4 = _17A8;
                return _17A7;
            }
            else if (_17C8.param == CHAR_EXIT)
            {
                _5FFE:
                *_17D4 = _17A8;
                return 0xFF;
            }
        }
        else if (_17C8.type == DICT_WM_COMMAND)
        {
            _602D:
            if (_17C8.param == CMD_RETURN_HOME)
            {
                _6048:
                MCU_memory[0x1935] = 0xFE;
                return 0xFF;
            }
        }
    }
}

void _00239068(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1)
{
    const UINT8 _845E[] = "金钱不足！";
    const UINT8 _8469[] = "已满载！";
    const UINT8 _8457[] = "金钱：";
    UINT8 _17B9[0x0C];
    UINT8 _17B1[0x07];
    UINT8 _17B0;
    UINT8 _17AF;
    UINT8 _17AE;
    UINT8* _17AC;
    UINT8 _17AB;
    UINT8* _17A9;
    UINT8 _17A6[3];
    UINT16 _17A4;
    UINT32 _17A0;
    _17A4 = *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    SysLcdPartClear(0x00, 0x00, 0x9E, 0x5F);
    _17AF = 0x00;
    _17B0 = 0x00;
    while (0x01)
    {
        _6106:
        SysLcdPartClear(0x3D, 0x01, 0x9E, 0x12);
        _SysMemcpy(_17B1, _8457, 0x0007);
        SysPrintString(0x03C, 0x02, _17B1);
        __8F1F(0x6E, 0x03, 0x1A, 0x0A, 0x01);
        _17B0 = __8EC1(_17CF, _17D0, _17D1, _17B0, &_17AF);
        if (_17B0 == 0xFF)
        {
            _6239:
            return;
        }
        else
        {
            _623F:
            _17AC = __8F0A(DAT_GOODSRES, _17D1[_17B0 * 0x0003], _17D1[_17B0*0x0003+0x01]);
            if (MCU_memory[0x1935])
            {
                _62E7:
                return;
            }
            _62EA:
            _17A9 = _17AC;
            // 道具买价 > 玩家拥有的金钱
            if ((UINT32)*(UINT16*)(_17A9+0x12) > *(UINT32*)(MCU_memory+0x1A8F))
            {
                _6330:
                // 道具买价大于玩家拥有的金钱
                _SysMemcpy(_17B9, _845E, 0x000C);
                GuiMsgBox(_17B9, 0x0064);
            }
            else
            {
                _63A2:
                _17AE = __8ED0(_17D1[_17B0 * 0x0003], _17D1[_17B0*0x0003+0x01]);
                if (_17AE == 0xFF)
                {
                _6444:
                    // 如果背包中的道具种类>=200
                    if (MCU_memory[0x1A95] >= 0x00C8)
                    {
                        _645D:
                        _SysMemcpy(_17B9, _8469, 0x000A);
                        GuiMsgBox(_17B9, 0x0064);
                    }
                }
                _64CF:
                // 道具买价
                _17A0 = (UINT32) * (UINT16*)(_17A9 + 0x12);
                if (_17AE != 0xFF)
                {
                    _650E:
                    _17AB = MCU_memory[_17AE*0x0003+_17A4+0x02];
                }
                else
                {
                    _654A:
                    _17AB = 0x00;
                }
                _6550:
                _17AB = __8D87(0x00, _17AB, _17A9 + 0x0006, _17A0);
                if (_17AB != 0xFF)
                {
                    _65C7:
                    _17A0 *= (UINT32)_17AB;
                    // 玩家拥有的金钱
                    *(UINT32*)(MCU_memory+0x1A8F) -= _17A0;
                    _17A6[0] = _17D1[_17B0 * 0x0003];
                    _17A6[1] = _17D1[_17B0 * 0x0003 + 0x01];
                    _17A6[2] = _17AB;
                    __8ECA(_17A6);
                }
                _6717:
                SysLcdPartClear(0x0C, 0x15, 0x93, 0x4B);
            }
        }
    }
    _674E:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

UINT8 _00239787(UINT8 x, UINT8 y, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3, UINT8* _17D4)
{
    UINT8 _17CC;
    UINT8 _17BF[0x0D];
    UINT8 _17BE;
    UINT8 _17BC;
    UINT8 _17A7[0x14];
    UINT8 _17A6;
    UINT8 _17A5;
    _17A6 = (_17D1-x+0x01)/0x08-0x01;
    if (_17A6+0x01 == 0x01)
    {
        _67C5:
        return 0x00;
    }
    _67CA:
    _17A5 = (_17D2-y+0x01)/0x10;
    if (_17A5+0x01 == 0x01)
    {
        _67F2:
        return 0x00;
    }
    _67F7:
    _17BE = 0x00;
    for (_17CC=0x00; _17CC<0x0D; _17CC++)
    {
        _681E:
        for (UINT8 _17BD=0x00; _17BD<=_17A6; _17BD++)
        {
            _6843:
            if (_17D4[_17BE] > 0x80)
            {
                _686E:
                if (_17BD+0x01 > _17A6)
                {
                    _6884:
                    break;
                }
                _6887:
                _17BE++;
                _17BD++;
            }
            else if (_17D4[_17BE]+0x01 == 0x01)
            {
                _68C5:
                break;
            }
            _68C8:
            _17BE++;
        }
        _68D4:
        _17BF[_17CC] = _17BE;
        if (_17D4[_17BE]+0x01 == 0x01)
        {
            _692D:
            break;
        }
    }
    _6933:
    if (_17D3 >= _17CC)
    {
        _6941:
        if (_17A5 >= _17CC+0x01)
        {
            _6956:
            _17D3 = 0x00;
        }
        else
        {
            _695F:
            _17D3 = _17CC+0x01-_17A5;
        }
    }
    _696F:
    for (_17BC=0x00; _17BC<_17A5; _17BC++)
    {
        _6992:
        if (_17BC+_17D3 > _17CC)
        {
            _69AA:
            return _17CC;
        }
        if (_17BC+_17D3+0x01 > 0x01)
        {
            _69CA:
            _SysMemcpy(_17A7, _17BF[_17BC + _17D3 - 0x01]+ _17D4, 0x0014);
            _17A7[_17BF[_17BC + _17D3] -_17BF[_17BC+_17D3-0x01]] = 0x00;
        }
        else
        {
            _6ACF:
            _SysMemcpy(_17A7, _17D4, 0x0014);
            _17A7[_17BF[_17BC + _17D3]] = 0x00;
        }
        _6B5D:
        SysPrintString(x, _17BC * 0x10 + y, _17A7);
    }
    _6BAA:
    if (_17BC == _17A5)
    {
        _6BB8:
        _17BC--;
    }
    _6BC3:
    return _17BC+_17D3;
}

// 背包增加道具
UINT8 _00239BE2(UINT8* _17D0)
{
    UINT16 _17CC = *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    // 获得背包种指定道具的下标
    UINT8 _17CE = __8ED0(_17D0[0x00], _17D0[0x01]);
    if (_17CE != 0xFF)
    {
    _6C8C:
        // 如果增加道具后，道具数量小于99
        if (_17D0[0x02]+ MCU_memory[_17CE * 0x0003 + _17CC + 0x02] < 0x63)
        {
        _6CE1:
            // 道具数量增加对应数量
            MCU_memory[_17CE*0x0003+_17CC+0x0002] = MCU_memory[_17CE * 0x0003 + _17CC + 0x02] + _17D0[0x02];
        }
        else
        {
        _6D83:
            // 道具数量等于99个
            MCU_memory[_17CE*0x0003+_17CC+0x0002] = 0x63;
        }
        _6DCF:
        return 0x02;
    }
    else
    {
    _6DD7:
        // 如果背包中的道具种类<200
        if (MCU_memory[0x1A95] < 0x00C8)
        {
            _6DF0:
            MCU_memory[MCU_memory[0x1A95]*0x0003+_17CC] = _17D0[0x00]; // 道具类型
            MCU_memory[MCU_memory[0x1A95]*0x0003+_17CC+0x0001] = _17D0[0x01]; // 道具Idx
            MCU_memory[MCU_memory[0x1A95]*0x0003+_17CC+0x0002] = _17D0[0x02]; // 道具数量
            MCU_memory[0x1A95]++; // 背包中道具种类加1
            return 0x01;
        }
    }
    _6F11:
    return 0x00;
}

UINT8 _00239F29(UINT8* _17D0)
{
    UINT16 _17C9 = *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    UINT8 _17CB = __8ED0(_17D0[0x00], _17D0[0x01]);
    if (_17CB == 0xFF)
    {
        _6FD3:
        return 0x00;
    }
    _6FDB:
    if (MCU_memory[_17CB * 0x0003 + _17C9 + 0x02] > _17D0[0x02])
    {
        _7036:
        MCU_memory[_17CB*0x0003+_17C9+0x0002] = MCU_memory[_17CB * 0x0003 + _17C9 + 0x02]- _17D0[0x02];
        return 0x01;
    }
    else
    {
    _70E1:
        // 如果背包中的道具种类==1
        if (MCU_memory[0x1A95] == 0x01)
        {
        _70EC:
            MCU_memory[0x1A95] = 0x00; // 背包中的道具种类数
        }
        else
        {
        _70F4:
            MCU_memory[0x1A95]--; // 背包中的道具种类数
            _SysMemcpy(MCU_memory+_17CB * 0x0003 + _17C9, MCU_memory+MCU_memory[0x1A95] * 0x0003 + _17C9, 0x0003);
        }
        _71DA:
        return 0x02;
    }
}

// _17CF:道具类型，_17D0:道具Idx
// 返回背包中指定道具的下标
UINT8 _0023A1F2(UINT8 _17CF, UINT8 _17D0)
{
    UINT16 _17CC;
    // 背包中的道具种类==0
    if (MCU_memory[0x1A95]+0x01 == 0x01)
    {
        _7213:
        return 0xFF; // 未找到道具
    }
    _7218:
    _17CC = *(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x17);
    // 遍历背包中的道具
    for (UINT8 _17CE=0x00; _17CE<MCU_memory[0x1A95]; _17CE++)
    {
        _727E:
        if (_17CF == MCU_memory[_17CE*0x0003+_17CC])
        {
            _72C1:
            if (_17D0 == MCU_memory[_17CE*0x0003+_17CC+0x01])
            {
                _7304:
                return _17CE;
            }
        }
    }
    _730E:
    return 0xFF; // 未找到道具
}

// ACTORLAYERUP id level
void _0023A326()
{
    UINT16 id = 0x0000;
    UINT16 level = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _SysMemcpy((UINT8*)&id, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&level, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("ACTORLAYERUP %d %d\n", id, level);
    __8E9C((UINT8)id, (UINT8)level);
}

// GAMEOVER
void _0023A4E0()
{
    MCU_memory[0x1935] = 0xFD;
    LOG("GAMEOVER\n");
}

// BOXOPEN id
void _0023A4E6()
{
    UINT8 id = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    id = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("BOXOPEN %d\n", id);
    if (id >= 0x01 && id <= 0x28)
    {
        _7579:
        if (*(UINT16*)(MCU_memory+((id -0x01)<<1)+0x19D3) == 0x0000)
        {
            _75B3:
            MCU_memory[0x1935] = NPC_ERR;
            return;
        }
    _75BB:
        // 脚步
        MCU_memory[*(UINT16*)(MCU_memory+((id -0x01)<<1)+0x19D3)+0x0003] = 0x01;
        __8EF5(id -0x01);
    }
    _7612:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

// MUSIC n	timers
void _0023A64B()
{
    UINT16 n = 0x0000;
    UINT16 timers = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _SysMemcpy((UINT8*)&n, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&timers, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("MUSIC %d %d\n", n, timers);
    MCU_memory[0x1A97] = (UINT8)n; // 音乐号码
    MCU_memory[0x1A98] = (UINT8)timers; // 音乐持续次数
    //  音乐持续次数
    if (MCU_memory[0x1A98]+0x01 == 0x01)
    {
        _77EA:
        MCU_memory[0x1A98] = 0xFF; // 音乐持续次数
    }
_77EF:
    //  如果音乐号码为0 或 音乐关
    if (MCU_memory[0x1A97]+0x01 == 0x01 || MCU_memory[0x1AAD]+0x01 == 0x01)
    {
    _780B:
        MCU_memory[0x1A98] = 0x00; // 音乐持续次数
        SysStopMelody();
    }
    else
    {
    _781E:
        //              音乐号码
        SysPlayMelody(MCU_memory[0x1A97]);
    }
}

// MOVIE type team x y ctl
void _0023A83D()
{
    UINT16 type = 0x0000;
    UINT16 team = 0x0000;
    UINT16 x = 0x0000;
    UINT16 y = 0x0000;
    UINT16 ctl = 0x0000;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    _SysMemcpy((UINT8*)&type, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&team, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&x, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&y, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    _SysMemcpy((UINT8*)&ctl, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("MOVIE %d %d %d %d %d\n", type, team, x, y, ctl);
    __8E33();
    if ((ctl & 0x0002) == 0x0000)
    { // 背景会变
        _7BC0:
        fillmem(MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780, 0x00);
    }
    _7BF8:
    __8DE5((UINT8)type, (UINT8)team, (UINT8)ctl, (UINT8)x, (UINT8)y);
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory+0x1936), MCU_memory+0x4000, 0x0780);
}

// CHOICE "..." "..." adress
void _0023ACA7()
{
    MsgType _17C9;
    UINT8 _17B4[20];
    UINT8 _179F[20];
    UINT8 _179D[2];
    UINT8 _179C;
    UINT8 _179B = 0x00;
    UINT8 _179A;
    UINT8 _1799;
    UINT8 _1798;
    UINT16 adress;
    _179D[0] = 0x00;
    _179D[1] = 0x00;
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    while (0x01)
    {
        _7D1B:
        _SysMemcpy(_179F+ _179D[0], MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0001);
        *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
        if (_179F[_179D[0]]+0x01 == 0x01)
        {
            _7E28:
            break;
        }
        _7E2B:
        _179D[0]++;
    }
    _7E44:
    _179F[0x0013] = 0x00;
    while (0x01)
    {
        _7E89:
        _SysMemcpy(_17B4+ _179D[1], MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0001);
        *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
        if (_17B4[_179D[1]]+0x01 == 0x01)
        {
            _7F96:
            break;
        }
        _7F99:
        _179D[1]++;
    }
    _7FB2:
    _17B4[0x0013] = 0x00;
    if (_179D[1] < _179D[0])
    {
        _800A:
        _179C = _179D[0]*0x08+0x06;
    }
    else
    {
        _8021:
        _179C = _179D[1]*0x08+0x06;
    }
    _8035:
    _SysMemcpy((UINT8*)&adress, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2), 0x0002);
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("CHOICE %s %s %d\n", _179F, _17B4, adress);
    _179A = (0x00A0-_179C)/0x0002;
    _1799 = _179A+_179C;
    SysLcdPartClear(_179A, 0x1C, _1799, 0x46);
    SysRect(_179A + 0x01, 0x1E, _1799 - 0x01, 0x44);
    SysPrintString(_179A + 0x03, 0x20, _179F);
    SysPrintString(_179A + 0x03, 0x32, _17B4);
    _1798 = _179B*0x12+0x1E+0x02;
    SysLcdReverse(_179A + 0x03, _1798, _1799 - 0x03, _1798+0x10);
    while (0x01)
    {
        _8266:
        if (GuiGetMsg(&_17C9) == 0x00)
        {
            _829B:
            continue;
        }
        _829E:
        if (GuiTranslateMsg(&_17C9) == 0x00)
        {
            _82D3:
            continue;
        }
        _82D6:
        if (_17C9.type == DICT_WM_CHAR_FUN)
        {
            _82E2:
            switch (_17C9.param)
            {
            case CHAR_LEFT: // _830C
            case CHAR_RIGHT: // _830C
            case CHAR_DOWN: // _830C
            case CHAR_UP: // _830C
                SysLcdReverse(_179A + 0x03, _1798, _1799 - 0x03, _1798+0x10);
                _179B++;
                _179B %= 0x0002;
                _1798 = _179B*0x12+0x1E+0x02;
                SysLcdReverse(_179A + 0x03, _1798, _1799 - 0x03, _1798+0x10);
                break;
            case CHAR_ENTER: // _83D8
                if (_179B == 0x01)
                {
                    _83E4:
                    *(UINT16*)(MCU_memory+0x1AA0) = adress;
                }
                _83F9:
                __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
                return;
            }
        }
    }
}

// USEGOODS type index erradr
void _0023B486()
{
    UINT8 type;
    UINT8 index;
    UINT16 erradr;
    UINT8 _17C7[3];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    type = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    index = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    erradr = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("USEGOODS %d %d %d\n", type, index, erradr);
    _17C7[0] = type;
    _17C7[1] = index;
    _17C7[2] = 0x01;
    if (__8ECD(_17C7) == 0x00)
    {
        _862C:
        *(UINT16*)(MCU_memory+0x1AA0) = erradr;
    }
}

// SETEVENTTIMER event timer
void _0023B652()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    // 定时触发事件
    MCU_memory[0x1A9C] = MCU_memory[*(UINT16*)(MCU_memory+0x1AA0)+*(UINT16*)(MCU_memory+0x1AA2)];
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    // 定时触发事件的定时时间
    *(UINT16*)(MCU_memory+0x1A9D) = *(UINT16*)(MCU_memory+ *(UINT16*)(MCU_memory + 0x1AA0) + *(UINT16*)(MCU_memory + 0x1AA2));
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0002;
    LOG("SETEVENTTIMER %d %d\n", MCU_memory[0x1A9C], *(UINT16*)(MCU_memory + 0x1A9D));
}

// ENABLESHOWPOS
void _0023B73B()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("ENABLESHOWPOS\n");
    MCU_memory[0x197B] = 0x01; // 打开显示主角在当前地图的坐标功能
}

// DISABLESHOWPOS
void _0023B762()
{
    *(UINT16*)(MCU_memory+0x1AA0) += 0x0001;
    LOG("DISABLESHOWPOS\n");
    MCU_memory[0x197B] = 0x00; // 取消显示主角在当前地图的坐标功能
}

// __8F40
UINT8 _0023B789(UINT8 _17CF, UINT8 _17D0)
{
    UINT8 _17CE = MCU_memory[0x1AAC];
    UINT8 _17CD = MCU_memory[0x1AAB];
    UINT16 _17CB = *(UINT16*)(MCU_memory+0x1AA0);
    MCU_memory[0x1AAC] = _17CF;
    MCU_memory[0x1AAB] = _17D0;
    __8DBA();
    if (MCU_memory[0x1935])
    {
        _87E0:
        MCU_memory[0x1AAC] = _17CE;
        MCU_memory[0x1AAB] = _17CD;
        return 0x01;
    }
    _87F3:
    __8DB1(*(UINT16*)(MCU_memory+0x1AA0));
    if (MCU_memory[0x1935])
    {
        _8823:
        return 0x02;
    }
    _8828:
    if (MCU_memory[0x1AA4] == 0x02)
    {
        _8833:
        MCU_memory[0x1AAC] = _17CE;
        MCU_memory[0x1AAB] = _17CD;
        __8DBA();
        if (MCU_memory[0x1935])
        {
            _8854:
            return 0x01;
        }
        _8859:
        *(UINT16*)(MCU_memory+0x1AA0) = _17CB;
        MCU_memory[0x1AA4] = 0x01;
    }
    _8873:
    return 0x00;
}

void _0023B88B(UINT8 _17CF)
{
    if (__8F40(0x00, _17CF) == 0x01)
    {
        _88BC:
        MCU_memory[0x1935] = 0x00;
    }
}

void _0023C000(UINT8* _17D0)
{
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C2[0x06];
    UINT16 _17C0;
    UINT8 _17B4[0x0C];
    if (MCU_memory[0x1935])
    {
        _5018:
        return;
    }
    _501B:
    for (UINT8 _17CD=0x00; _17CD<0x06; _17CD++)
    {
        _503C:
        _17C2[_17CD] = 0x00;
        _17B4[_17CD*0x0002+0x0001] = 0x00;
        _17B4[_17CD*0x0002] = 0xFF;
        // 身法
        _17C0 = MCU_memory[*(UINT16*)(MCU_memory+_17CD*0x0019+0x1988+0x11)+0x16];
        if (_17CD < 0x03)
        {
            _514A:
            // 角色资源ID
            if (MCU_memory[_17CD*0x0019+0x1988+0x01] == 0x00)
            {
                _5181:
                continue;
            }
            _5184:
            _17CA = MCU_memory[*(UINT16*)(MCU_memory+(_17CD<<1)+0x181E)+0x001D];
            //       玩家身法增减益
            _17C0 += MCU_memory[_17CD*0x0005+0x1800+0x04];
            _17B4[_17CD*0x0002+0x0001] = (UINT8)_17C0;
            _17B4[_17CD*0x0002] = _17CD;
            _17C9 = MCU_memory[*(UINT16*)(MCU_memory+(_17CD<<1)+0x181E)];
            _17C8 = MCU_memory[*(UINT16*)(MCU_memory+(_17CD<<1)+0x181E)+0x0003];
            if (_17C9 == 0x02 && (_17C8 == 0x03 || _17C8 == 0x04))
            {
                _533C:
                _17B4[_17CD*0x0002+0x0001] = (_17C0 >> 0x0001) + _17C0;
            }
            else if (_17C9 == 0x08 && (_17CA == 0x0A || _17CA == 0x09))
            {
                _53E8:
                _17B4[_17CD*0x0002+0x0001] = (_17C0 >> 0x0001) + _17C0;
            }
        }
        else
        {
            _5470:
            _17CB = _17CD-0x03;
            // 敌人角色资源ID
            if (MCU_memory[_17CB*0x0033+0x1826+0x01] == 0x00)
            {
                _54B2:
                continue;
            }
            _54B5:
            _17B4[_17CD*0x0002] = _17CB + 0x80;
            // 敌人身法=敌人角色身法+敌人身法增减益
            _17B4[_17CD*0x0002+0x0001] = MCU_memory[_17CB * 0x0033 + 0x1826 + 0x0012 + 0x01]+ MCU_memory[_17CB * 0x0005 + 0x180F + 0x04];
        }
    }
    _55CA:
    for (UINT8 _17CD=0x00; _17CD<0x06; _17CD++)
    {
        _55EB:
        for (_17CC=_17CD;_17CC+0x01>0x01; _17CC--)
        {
            _5616:
            _17C2[_17CC] = _17C2[_17CC - 0x01];
            _17D0[_17CC] = _17D0[_17CC - 0x01];
            if (_17C2[_17CC - 0x01] >= _17B4[_17CD*0x0002+0x0001])
            {
                _5719:
                break;
            }
        }
        _571F:
        _17C2[_17CC] = _17B4[_17CD * 0x0002 + 0x0001];
        _17D0[_17CC] = _17B4[_17CD * 0x0002];
    }
}

UINT8 _0023C7F9(UINT8 _17CF)
{
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17C8[3];
    if (MCU_memory[0x1935])
    {
        _5814:
        return 0x01;
    }
    _5819:
    _17CB = 0x00;
    if (_17CF < 0x80)
    {
        _582B:
        _17CC = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
        if (_17CC != 0xAA && _17CC > 0x02)
        {
            _5877:
            _17CC = 0x00;
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0x00;
            if (__8E66(0x2B, &_17CC) == 0xAA)
            {
                _58F6:
                return 0x01;
            }
        }
        _58FB:
        switch (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)])
        {
        case 0x0002: // _5943
            // mp
            if (*(UINT16*)(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x0C) < MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003+0x04])
            {
                _59E5:
                _17CB = 0x01;
            }
            _59EB:
            if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x05] != 0xFF)
            {
                _5A30:
                _17CB = 0x01;
            }
            break;
        case 0x0008: // _5A39
        case 0x0009: // _5A39
            _17C8[0] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x001D];
            _17C8[1] = MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x001D + 0x01];
            _17C8[2] = 0x01;
            if (__8ECD(_17C8) == 0x00)
            {
                _5B3B:
                _17CB = 0x01;
            }
            break;
        case 0x0000: // _5B44
            _17CB = 0x01;
            break;
        }
        _5B4D:
        if (_17CB)
        {
            _5B56:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] = 0x01;
            // 普通攻击可能产生异常状态
            if (MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x02] > 0x0F)
            {
                _5BD6:
                MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0xAA;
                _17CC = 0xAA;
            }
            else
            {
                _5C21:
                MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0x00;
                _17CC = 0x00;
            }
        }
        _5C69:
        if (_17CC == 0xAA || _0023EBC0(_17CF) != 0x00)
        {
            _5C84:
            return 0x00;
        }
        else
        {
            _5C8C:
            if (_17CC > 0x03)
            {
                _5C9D:
                _17CC = 0x00;
            }
            _5CA3:
            // 敌人角色资源ID
            if (MCU_memory[_17CC*0x0033+0x1826+0x01])
            {
                _5CDA:
                return 0x00;
            }
            _5CDF:
            if (__8E66(0x2B, &_17CC) == 0xAA)
            {
                _5D16:
                return 0x01;
            }
            _5D1B:
            MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = _17CC;
        }
    }
    _5D5F:
    return 0x00;
}

void _0023CD77(UINT16 _17D0)
{
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _5D8F:
        return;
    }
    _5D92:
    for (_17CB=0x00; _17CB<0x0A; _17CB++)
    {
        _5DB3:
        if (MCU_memory[_17CB*0x0003+0x18BF] == 0x00)
        {
            _5DEA:
            break;
        }
    _5DED:
        //                                          物品类型                                                            物品ID
        if (MCU_memory[_17CB * 0x0003 + 0x18BF] == MCU_memory[_17D0] && MCU_memory[_17CB * 0x0003 + 0x18BF + 0x01] == MCU_memory[_17D0+0x01])
        {
            _5E93:
            break;
        }
    }
    _5E99:
    if (_17CB > 0x08)
    {
        _5EAA:
        return;
    }
    _5EAD:
    MCU_memory[_17CB*0x0003+0x18BF] = MCU_memory[_17D0];
    MCU_memory[_17CB*0x0003+0x18BF+0x0001] = MCU_memory[_17D0 + 0x01];
    MCU_memory[_17CB * 0x0003 + 0x18BF + 0x0002]++; // 物品数量加1
}

void _0023CFE5(UINT8 _17CF)
{
    UINT8 _17CD;
    UINT8 _17CC;
    UINT8 _17CB;
    UINT8 _17CA;
    UINT16 _17C8;
    if (MCU_memory[0x1935])
    {
        _6000:
        return;
    }
    _6003:
    if (MCU_memory[0x1925] == 0x00)
    { // 与指定敌人战斗
        _600B:
        MCU_memory[_17CF+0x1919] = 0x00;
    }
    else
    {
        _6033:
        _17C8 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
        // 幸运
        _17CD = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x18];
        // 身法
        _17CC = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x16];
        // 敌人角色幸运
        _17CB = MCU_memory[0x1826+0x0012+0x04];
        // 敌人角色身法
        _17CA = MCU_memory[0x1826+0x0012+0x01];
        _17CD = _17C8%(_17CD+0x01);
        _17CC = _17C8%(_17CC+0x01);
        _17CB = _17C8%(_17CB+0x01);
        _17CA = _17C8%(_17CA+0x01);
        MCU_memory[_17CF+0x1919] = _17CD + _17CC > _17CB + _17CA ? 0x01 : 0x00;
    }
}

void _0023D20C(UINT8 _17CF)
{
    UINT16 _17CB;
    UINT16 _17C9;
    UINT16 _17C7;
    UINT16 _17C5;
    if (MCU_memory[0x1935])
    {
        _6227:
        return;
    }
    _622A:
    // 指向角色属性
    _17C5 = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    // 每回合变化生命
    _17C7 = MCU_memory[_17C5+0x04];
    if (_17C7 != 0x0000)
    {
    _62AB:
        //      hp
        _17CB = *(UINT16*)(MCU_memory+_17C5+0x08)+_17C7;
        //      hpMax
        _17C9 = *(UINT16*)(MCU_memory+_17C5+0x06);
        if (_17CB > _17C9)
        {
            _6324:
            _17CB = _17C9;
        }
    _633A:
        // hp
        *(UINT16*)(MCU_memory+_17C5+0x0008) = _17CB;
        __8E72(0x2B, &_17C7, _17CF, 0x01, 0x01);
    }
_63C4:
    // 每回合变化真气
    _17C7 = MCU_memory[_17C5+0x05];
    if (_17C7 != 0x0000)
    {
    _6400:
        //      mp
        _17CB = *(UINT16*)(MCU_memory+_17C5+0x0C)+_17C7;
        //      mpMax
        _17C9 = *(UINT16*)(MCU_memory+_17C5+0x0A);
        if (_17CB > _17C9)
        {
            _6479:
            _17CB = _17C9;
        }
    _648F:
        // mp
        *(UINT16*)(MCU_memory+_17C5+0x000C) = _17CB;
        __8E72(0x2B, &_17C7, _17CF, 0x01, 0x01);
    }
}

void _0023D52A(UINT8 _17CF, UINT8 _17D0)
{
    if (_17D0 == 0x01)
    {
        _6549:
        for (UINT8 _17CA=0x00; _17CA<0x07; _17CA++)
        {
            _656A:
            if (MCU_memory[_17CA+ _17CF * 0x0007 + 0x18EC] == 0xFF)
            {
                _65BD:
                continue;
            }
            _65C0:
            MCU_memory[_17CA+ _17CF * 0x0007 + 0x18EC]--;
            if (MCU_memory[_17CA+ _17CF * 0x0007 + 0x18EC] == 0x00)
            {
                _6660:
                if (_17CA == 0x00)
                {
                _6669:
                    // 重置玩家攻击增减益
                    *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800) = 0x0000;
                }
                else if (_17CA == 0x01)
                {
                _66C1:
                    // 重置玩家防御增减益
                    *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800+0x0002) = 0x0000;
                }
                else if (_17CA == 0x02)
                {
                _6726:
                    // 重置玩家身法增减益
                    MCU_memory[_17CF*0x0005+0x1800+0x0004] = 0x00;
                }
                _676F:
                MCU_memory[_17CA+ _17CF * 0x0007 + 0x18EC] = 0xFF;
                if (_17CA == 0x04)
                {
                    _67D0:
                    MCU_memory[*(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x0001] = 0x00;
                }
            }
        }
    }
    else
    {
        _6818:
        for (UINT8 _17CA=0x00; _17CA<0x07; _17CA++)
        {
            _6839:
            if (MCU_memory[_17CA+ _17CF * 0x0007 + 0x1901] == 0xFF)
            {
                _688C:
                continue;
            }
            _688F:
            MCU_memory[_17CA+ _17CF * 0x0007 + 0x1901]--;
            if (MCU_memory[_17CA+ _17CF * 0x0007 + 0x1901] == 0x00)
            {
                _692F:
                if (_17CA == 0x00)
                {
                _6938:
                    // 重置敌人攻击增减益
                    *(UINT16*)(MCU_memory+_17CF*0x0005+0x180F) = 0x0000;
                }
                else if (_17CA == 0x01)
                {
                    _6990:
                    // 重置敌人防御增减益
                    *(UINT16*)(MCU_memory+_17CF*0x0005+0x180F+0x0002) = 0x0000;
                }
                else if (_17CA == 0x02)
                {
                    _69F5:
                    // 重置敌人身法增减益
                    MCU_memory[_17CF*0x0005+0x180F+0x0004] = 0x00;
                }
                _6A3E:
                MCU_memory[_17CA+ _17CF * 0x0007 + 0x1901] = 0xFF;
            }
        }
    }
}

void _0023DAA7(UINT8 _17CF, UINT8 _17D0)
{
    UINT16 _17CB;
    UINT16 _17C9;
    UINT16 _17C7;
    UINT16 _17C5;
    UINT16 _17C3;
    UINT16 _17C1;
    UINT16 _17BF;
    if (MCU_memory[0x1935])
    {
        _6AC2:
        return;
    }
    _6AC5:
    _17C5 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    if (_17D0 == 0x01)
    { // 玩家受到伤害
        _6B02:
        // 指向玩家角色属性
        _17C1 = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0x04;
        __8E5D(0x01, _17CF);
        // 玩家攻击=玩家角色攻击+玩家攻击增减益
        _17CB = *(UINT16*)(MCU_memory+_17C1+0x0E) + *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800);
        // 玩家防御=玩家角色防御+玩家防御增减益
        _17C9 = *(UINT16*)(MCU_memory+_17C1+0x10) + *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800+0x02);
        // 玩家hp
        _17C7 = *(UINT16*)(MCU_memory+_17C1+0x08);
        _17C3 = _17CB/((_17C9>>0x0002)+0x0001);
        _17C3 += _17C5%((_17CB>>0x0004)+0x0001);
        // 如果真实伤害>玩家hp
        if (_17C3 > _17C7)
        {
        _6D7B:
            // 玩家hp
            *(UINT16*)(MCU_memory+_17C1+0x0008) = 0x0000;
        }
        else
        {
        _6DB1:
            // 玩家hp
            *(UINT16*)(MCU_memory+_17C1+0x0008) = *(UINT16*)(MCU_memory + _17C1 + 0x08) - _17C3;
        }
        _6E1A:
        __8E72(0x2D, &_17C3, _17CF, 0x01, 0x01);
        __8E57(_17CF);
    }
    else
    { // 敌人受到伤害
        _6E80:
        // 指向敌人角色等级开始的数据
        _17BF = _17CF * 0x0033 + 0x1826 + 0x0012;
        __8E69(0x01, 0x03+_17CF);
        // 敌人攻击=敌人角色攻击+敌人攻击增减益
        _17CB = *(UINT16*)(MCU_memory+_17BF+0x0E) + *(UINT16*)(MCU_memory+_17CF*0x0005+0x180F);
        // 敌人防御=敌人角色防御+敌人防御增减益
        _17C9 = *(UINT16*)(MCU_memory+_17BF+0x10) + *(UINT16*)(MCU_memory+_17CF*0x0005+0x180F+0x02);
        // 敌人hp
        _17C7 = *(UINT16*)(MCU_memory+_17BF+0x08);
        _17C3 = _17CB/((_17C9>>0x0003)+0x0001);
        _17C3 += _17C5%((_17CB>>0x0002)+0x0001);
        // 如果敌人hp>真实伤害
        if (_17C7 > _17C3)
        {
            _70D1:
            // 敌人hp
            *(UINT16*)(MCU_memory+_17BF+0x0008) = *(UINT16*)(MCU_memory + _17BF + 0x08) - _17C3;
        }
        else
        {
            _713D:
            // 敌人hp
            *(UINT16*)(MCU_memory+_17BF+0x0008) = 0x0000;
        }
        _7170:
        __8E72(0x2D, &_17C3, _17CF, 0x02, 0x01);
        __8E5A(_17CF);
    }
}

void _0023E1E4(UINT8 _17CF, UINT8 _17D0)
{
    UINT16 _17CB;
    UINT16 _17C9;
    UINT16 _17C7;
    if (MCU_memory[0x1935])
    {
        _71FF:
        return;
    }
    _7202:
    // 指向角色属性
    _17C7 = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    if (_17D0 == 0x01)
    {
        _7253:
        if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x03] == 0xFF)
        {
            _7298:
            return;
        }
        _729B:
        // 角色资源ID                                       hp
        if (MCU_memory[_17CF*0x0019+0x1988+0x01] == 0x00 || *(UINT16*)(MCU_memory+_17C7+0x08) == 0x0000)
        {
            _72FA:
            return;
        }
    _72FD:
        // hp
        _17C9 = *(UINT16*)(MCU_memory+_17C7+0x08);
        _17CB = (_17C9>>0x0002);
        // hp
        *(UINT16*)(MCU_memory+_17C7+0x0008) = *(UINT16*)(MCU_memory + _17C7 + 0x08) - _17CB;
        __8E72(0x2D, &_17CB, _17CF, 0x01, 0x01);
        __8E57(_17CF);
    }
    else
    {
        _7410:
        if (MCU_memory[_17CF * 0x0007 + 0x1901 +0x03] == 0xFF)
        {
            _7455:
            return;
        }
        _7458:
        // 敌人角色资源ID
        if (MCU_memory[_17CF*0x0033+0x1826+0x01] == 0x00)
        {
            _748F:
            return;
        }
        _7492:
        // 敌人角色hp
        _17C9 = *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x08);
        _17CB = (_17C9>>0x0002);
        // 敌人角色hp
        *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x0008) = *(UINT16*)(MCU_memory + _17CF * 0x0033 + 0x1826 + 0x0012 + 0x08) - _17CB;
        __8E72(0x2D, &_17CB, _17CF, 0x02, 0x01);
        __8E5A(_17CF);
    }
}

void _0023E641()
{
    UINT8 _17CB[0x03];
    if (MCU_memory[0x1935])
    {
        _7659:
        return;
    }
    _765C:
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _767D:
        _17CB[_17CE] = MCU_memory[*(UINT16*)(MCU_memory + (_17CE << 1) + 0x181E)];
        MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)] = 0x01;
    }
    _770B:
    __8E54();
    for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
    {
        _7737:
        MCU_memory[*(UINT16*)(MCU_memory+(_17CE<<1)+0x181E)] = _17CB[_17CE];
    }
}

void _0023E7A1(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2)
{
    const UINT8 _89CB[0x07] = {
        0x40, // 玩家攻击增减益更新
        0x20, // 玩家防御增减益更新
        0x10, // 玩家身法增减益更新
        0x08,
        0x04,
        0x02,
        0x01,
    };
    UINT16 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    if (MCU_memory[0x1935])
    {
        _77BC:
        return;
    }
    _77BF:
    if (_17D1 == 0x01)
    {
        _77CB:
        _17C8 = _17CF*0x0007+0x18EC;
        // 对特殊状态的免疫
        _17C7 = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x01];
        _17C4 = 0x01;
    }
    else
    {
        _7843:
        _17C8 = _17CF*0x0007+0x1901;
        // 敌人角色免疫特殊状态[bit3:毒,bit2:乱,bit1:封,bit0:眠]
        _17C7 = MCU_memory[_17CF*0x0033+0x1826+0x03];
        _17C4 = 0x02;
    }
    _78AC:
    for (UINT8 _17CA=0x00; _17CA<0x07; _17CA++)
    {
        _78CD:
        if (_89CB[_17CA]&_17D0)
        {
            _78F1:
            if ((_89CB[_17CA] & _17C7) == 0x00)
            {
                _7915:
                __8E7B(_17CF, &_17C6, &_17C5, _17C4);
                __8DE5(0x01, _17CA + 0x00F0, 0x02, _17C6, _17C5);
                MCU_memory[_17CA+_17C8] = _17D2;
            }
        }
    }
}

void _0023EA2F(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1)
{
    const UINT8 _89CB[0x07] = { 0x40,0x20,0x10,0x08,0x04,0x02,0x01 };
    UINT16 _17CC;
    UINT8 _17CB;
    if (MCU_memory[0x1935])
    {
        _7A4A:
        return;
    }
    _7A4D:
    _17CB = 0x00;
    if (_17D1 == 0x01)
    {
        _7A5F:
        _17CC = _17CF*0x0007+0x18EC;
        _17CB = 0x01;
    }
    else
    {
        _7A95:
        _17CC = _17CF*0x0007+0x1901;
    }
    _7AC2:
    for (UINT8 _17CE=0x00; _17CE<0x07; _17CE++)
    {
        _7AE3:
        if (_89CB[_17CE]&_17D0)
        {
            _7B07:
            if (_17CB)
            {
                _7B10:
                if (_17CE == 0x04)
                {
                    _7B1C:
                    if (MCU_memory[_17CE+_17CC] != 0xFF)
                    {
                        _7B42:
                        MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0001] = 0x00;
                    }
                }
            }
            _7B84:
            MCU_memory[_17CE+_17CC] = 0xFF;
        }
    }
}

UINT8 _0023EBC0(UINT8 _17CF)
{
    UINT8 _17CE;
    if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] == 0x0002)
    {
        _7C2B:
        _17CE = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0003];
        if (_17CE == 0x01 || _17CE == 0x05)
        {
            _7C7F:
            return 0x00;
        }
        else
        {
            _7C87:
            return 0x01;
        }
    }
    else if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] == 0x0008)
    {
        _7C8C:
        return 0x01;
    }
    _7C91:
    return 0x00;
}

UINT8 _0023ECA9(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2, UINT16* _17D4)
{
    UINT16 _17C9;
    UINT16 _17C7;
    UINT16 _17C5;
    UINT16 _17C3;
    UINT16 _17C1;
    UINT16 _17BF;
    UINT8 _17CB = 0x00;
    fillmem((UINT8*)_17D2, 0x0006, 0x00);
    fillmem((UINT8*)_17D4, 0x0006, 0x00);
    if (_17D0[0x00] > 0x0000) // todo
    {
        _7D8C:
        _17D0[0x00] = 0x0000 - _17D0[0x00];
        _17CB = 0x01;
    }
    _7DFA:
    if (_17D0[0x01] > 0x0000)
    {
        _7E26:
        _17D0[0x01] = 0x0000 - _17D0[0x01];
        _17CB = 0x01;
    }
    _7EA1:
    // 指向角色属性
    _17C1 = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    // 玩家身法=玩家身法增减益+玩家角色身法
    _17C5 = MCU_memory[_17CF*0x0005+0x1800+0x04]+ MCU_memory[*(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11) + 0x16];
    for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
    {
        _7F91:
        // 敌人角色资源ID
        if (MCU_memory[_17CC*0x0033+0x1826+0x01] == 0x00)
        {
            _7FC8:
            continue;
        }
        _7FCB:
        _17C7 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
        // 指向敌人角色等级开始的数据
        _17BF = _17CC * 0x0033 + 0x1826 + 0x0012;
        // 敌人身法=敌人身法增减益+敌人角色身法+20
        _17C3 = MCU_memory[_17CC*0x0005+0x180F+0x04]+ MCU_memory[_17BF + 0x01] + 0x14;
        // 如果敌人身法>玩家身法
        if (_17C3 > _17C5)
        {
            _80DA:
            _17C3 -= _17C5;
        }
        else
        {
            _8103:
            _17C3 = 0x0000;
        }
        _810C:
        if (_17C7%0x0064 < _17C3)
        {
            _8135:
            if (_17D0[0x00] != 0x0000)
            {
                _815C:
                _17D2[_17CC] = 0xFFFF; // MISS
            }
            _8195:
            if (_17D0[0x01] != 0x0000)
            {
                _81BC:
                _17D4[_17CC] = 0xFFFF; // MISS
            }
            _81F5:
            continue;
        }
        _81F8:
        _17C9 = _17D0[0x00];
        //       灵力
        _17C9 += MCU_memory[_17C1+0x17]*(_17C9 >> 0x0006);
        //       敌人角色灵力
        _17C9 -= MCU_memory[_17BF+0x02]*(_17C9 >> 0x0006);
        _17C9 += (_17C7%_17C9)>>0x0004;
        // 如果魔法伤害>敌人角色hp
        if (_17C9 > *(UINT16*)(MCU_memory+_17BF+0x08))
        {
        _8361:
            // 实际伤害=敌人角色hp
            _17D2[_17CC] = *(UINT16*)(MCU_memory + _17BF + 0x08);
        }
        else
        {
            _83C3:
            _17D2[_17CC] = _17C9;
        }
        _83FF:
        _17C9 = _17D0[0x01];
        //       灵力
        _17C9 += MCU_memory[_17C1+0x17]* _17C9 >> 0x0006;
        //       敌人角色灵力
        _17C9 -= MCU_memory[_17BF+0x02]* _17C9 >> 0x0006;
        _17C9 += (_17C7%_17C9)>>0x0004;
        //          敌人角色mp
        if (_17C9 > *(UINT16*)(MCU_memory+_17BF+0x0C))
        {
            _8568:
            _17D4[_17CC] = *(UINT16*)(MCU_memory + _17BF + 0x0C);
        }
        else
        {
            _85CA:
            _17D4[_17CC] = _17C9;
        }
    }
    _8609:
    return _17CB;
}

void _0023F623(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2)
{
    UINT16 _17CA = 0x0000;
    UINT16 _17C8 = 0x0000;
    // 指向角色属性
    UINT8* _17C6 = MCU_memory + *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
    {
        _86AE:
        // 敌人角色资源ID
        if (MCU_memory[_17CC*0x0033+0x1826+0x01] == 0x00)
        {
            _86E5:
            continue;
        }
        _86E8:
        _17CA += _17D0[_17CC];
        _17C8 += _17D2[_17CC];
    }
    _8775:
    if (_17CA-0x0000 != 0x0000)
    {
        _879D:
        __8E72(0x2B, &_17CA, _17CF, 0x01, 0x01);
        // hp
        *(UINT16*)(_17C6+0x0008) = *(UINT16*)(_17C6 + 0x08) + _17CA;
    }
    _885A:
    if (_17C8-0x0000 != 0x0000)
    {
        _8882:
        __8E72(0x2B, &_17C8, _17CF, 0x01, 0x01);
        // mp
        *(UINT16*)(_17C6+0x000C) = *(UINT16*)(_17C6 + 0x0C) + _17C8;
    }
    _893F:
    __8E87(_17C6, 0x01);
}

void _0023F994()
{
    _SysMemcpy(MCU_memory+0x4000, MCU_memory + *(UINT16*)(MCU_memory+0x1936), 0x0780);
}

void _00240000(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2)
{
    if (_17D2 == 0xAA)
    {
        _501F:
        for (UINT8 _17CE=0x00; _17CE<0x03; _17CE++)
        {
            _5040:
            __8E7E(_17CF, _17D0, _17CE);
        }
    }
    else
    {
        _507A:
        __8E7E(_17CF, _17D0, _17D2);
    }
}

UINT8 _002400C1(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2)
{
    UINT8 _17CE = 0x00;
    if (_17D2 == 0xAA)
    {
        _50E6:
        for (UINT8 _17CD=0x00; _17CD<0x03; _17CD++)
        {
            _5107:
            if (__8E81(_17CF, _17D0, _17CD) != 0x00)
            {
                _5145:
                if (_17CE == 0x00)
                {
                    _514E:
                    _17CE = 0x01;
                }
            }
        }
    }
    else
    {
        _515A:
        _17CE = __8E81(_17CF, _17D0, _17D2);
    }
    _5194:
    return _17CE;
}

void _002401AE(UINT8 _17CF)
{
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    UINT8 _17C0;
    UINT8 _17BF;
    UINT8 _17BE;
    UINT8 _17BD;
    UINT16 _17B5[0x04];
    UINT16 _17AD[4];
    UINT16 _17AB;
    if (MCU_memory[0x1935])
    {
        _51C9:
        return;
    }
    _51CC:
    __8E33();
    fillmem((UINT8*)_17B5, 0x0008, 0x00);
    _SysMemcpy((UINT8*)_17AD, MCU_memory + *(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x001D + 0x000F, 0x0008);
    _17C5 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    _17C4 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x001D];
    if (_17C4 == 0x09)
    {
        _533D:
        _17C9 = _17AD[2] >>0x0008;
        _17C8 = (UINT8)_17AD[2];
        __8E7B(_17C5, &_17C7, &_17C6, 0x01);
        __8DE5(_17C9, _17C8, 0x02, _17C7, _17C6);
        if (_17AD[0] != 0x0000)
        {
            _542E:
            __8E72(0x2B, _17AD, _17C5, 0x01, 0x01);
        }
        _546C:
        if (_17AD[1] != 0x0000)
        {
            _5487:
            __8E72(0x2B, &_17AD[1], _17C5, 0x01, 0x01);
        }
    }
    _54F4:
    if (_17C5 == 0xAA)
    {
        _5500:
        _17BE = 0x03;
    }
    else
    {
        _5509:
        _17BE = 0x01;
    }
    _550F:
    _17C0 = _17AD[1] >> 0x0008;
    _17C1 = (UINT8)_17AD[1];
    _17C2 = _17AD[2] >> 0x0008;
    _17BD = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x001D+0x02];
    for (UINT8 _17CA=0x00; _17CA<_17BE; _17CA++)
    {
        _560E:
        _17BF = _17CA+_17C5;
        _17BF %= 0x03;
        // 角色资源ID
        if (MCU_memory[_17BF*0x0019+0x1988+0x01] == 0x00)
        {
            _5661:
            continue;
        }
        _5664:
        //                                      指向角色属性
        __8E84(_17C4, _17B5, _17AD, MCU_memory + *(UINT16*)(MCU_memory+_17BF*0x0019+0x1988+0x11));
        _17C3 = _17AD[3] &0x0F;
        if (_17C3)
        {
            _56FC:
            __8E2A(_17BF, _17C3, 0x01);
        }
        _5727:
        if (_17C4 == 0x0A)
        {
        _5733:
            // hp
            _17AB = *(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17BF*0x0019+0x1988+0x11)+0x08);
            __8E72(0x2B, &_17AB, _17BF, 0x01, 0x01);
        }
        else if (_17C4 == 0x0C)
        {
            _57E7:
            __8E8D(_17BF, &_17C0, '+', _17BD);
        }
    }
    _5825:
    __8E54();
}

void _00240841(UINT8 _17CF)
{
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT16 _17BA[4];
    UINT16 _17B8;
    __8E33();
    _17C7 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    _17CA = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x001D];
    _SysMemcpy((UINT8*)_17BA, MCU_memory + *(UINT16*)(MCU_memory + (_17CF << 1) + 0x181E) + 0x001D + 0x000F, 0x0008);
    if (_17CA == 0x08)
    {
        _5974:
        _17C9 = _17BA[2] >>0x0008;
        _17C8 = (UINT8)_17BA[2];
        if (_17C8)
        {
            _59AA:
            __8E7B(_17C7, &_17C5, &_17C4, 0x02);
            __8DE5(_17C9, _17C8, 0x02, _17C5, _17C4);
        }
        _5A53:
        if (_17BA[0] != 0x0000)
        {
            _5A6E:
            _00241014(_17CF, _17BA[0], 0x01);
        }
        _5AAE:
        if (_17BA[1] != 0x0000)
        {
            _5AC9:
            _00241014(_17CF, _17BA[1], 0x02);
        }
        _5B09:
        //      指向角色属性
        __8E87(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), 0x01);
    }
    else
    {
        _5B7F:
        _17B8 = 0x0000;
        if ((_17BA[3] & 0x00FF) != 0x0000)
        {
            _5BAF:
            _17B8 |= 0x2000;
        }
        _5BD1:
        if ((_17BA[3] & 0xFF00) != 0x0000)
        {
            _5BF8:
            _17B8 |= 0x1000;
        }
        _5C1A:
        if ((_17BA[2] & 0x00FF) != 0x0000)
        {
            _5C41:
            _17B8 |= 0x0800;
        }
        _5C63:
        if ((_17BA[2] & 0xFF00) != 0x0000)
        {
            _5C8A:
            _17B8 |= 0x0400;
        }
        _5CAC:
        if ((_17BA[0] & 0x00FF) != 0x0000)
        {
            _5CD3:
            _17B8 |= 0x0200;
        }
        _5CF5:
        if ((_17BA[0] & 0xFF00) != 0x0000)
        {
            _5D1C:
            _17B8 |= 0x0100;
        }
        _5D3E:
        if ((_17BA[1] & 0x00FF) != 0x0000)
        {
            _5D65:
            _17B8 |= 0x0080;
        }
        _5D87:
        _17B8 += (_17BA[1] &0x7F00)>>0x0008;
        if (_17C7 == 0xAA)
        {
            _5DD0:
            for (UINT8 _17C6=0x00; _17C6<0x03; _17C6++)
            {
                _5DF1:
                _00241375(_17C6, _17B8, 0x01);
            }
        }
        else
        {
            _5E21:
            _00241375(_17C7, _17B8, 0x01);
        }
        _5E4B:
        __8E72(0x2D, &_17B8, _17C7, 0x02, 0x01);
    }
    _5E9F:
    _17C3 = _17BA[3] &0x000F;
    if (_17C3)
    {
        _5EC5:
        _17C2 = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x001D+0x02];
        if (_17C7 == 0xAA)
        {
            _5F0D:
            for (UINT8 _17C6=0x00; _17C6<0x03; _17C6++)
            {
                _5F2E:
                if (__8E51(_17C7, 0x02) != 0x00)
                {
                    _5F5C:
                    __8E27(_17C6, _17C3, 0x02, _17C2);
                }
            }
        }
        else
        {
            _5F94:
            if (__8E51(_17C7, 0x02) != 0x00)
            {
                _5FC2:
                __8E27(_17C7, _17C3, 0x02, _17C2);
            }
        }
    }
    _5FF4:
    __8E5A(_17C7);
}

void _00241014(UINT8 _17CF, UINT16 _17D0, UINT8 _17D2)
{
    UINT8 _17CA;
    UINT16 _17C6;
    UINT16 _17C8 = 0x0000;
    UINT8 _17CB = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
    if (_17D0 > 0x8000)
    {
        _607F:
        _17D0 = 0x0000-_17D0;
        _17C8 = _17D0;
    }
    _60B8:
    if (_17CB == 0xAA)
    {
        _60C4:
        for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
        {
            _60E5:
            _00241375(_17CC, _17D0, _17D2);
        }
    }
    else
    {
        _6117:
        _00241375(_17CB, _17D0, _17D2);
    }
    _6143:
    _17CA = __8E72(0x2D, &_17D0, _17CB, 0x02, 0x01);
    if (_17C8 != 0x0000)
    {
        _61B8:
        _17C8 = _17C8*_17CA;
        // 指向角色属性
        _17C6 = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
        if (_17D2 == 0x01)
        {
        _622C:
            // hp
            *(UINT16*)(MCU_memory+_17C6+0x0008) = *(UINT16*)(MCU_memory + _17C6 + 0x08) + _17C8;
        }
        else
        {
        _6298:
            // mp
            *(UINT16*)(MCU_memory+_17C6+0x000C) = *(UINT16*)(MCU_memory + _17C6 + 0x0C) + _17C8;
        }
        _6301:
        __8E72(0x2B, &_17C8, _17CF, 0x01, 0x01);
        __8E57(_17CF);
    }
}

void _00241375(UINT8 _17CF, UINT16 _17D0, UINT8 _17D2)
{
    // 指向敌人角色等级开始的数据
    UINT16 _17CB = _17CF * 0x0033 + 0x1826 + 0x0012;
    if (_17D2 == 0x01)
    {
        _63F0:
        // 敌人角色hp
        if (*(UINT16*)(MCU_memory+_17CB+0x08) < _17D0)
        {
            _641B:
            *(UINT16*)(MCU_memory+_17CB+0x0008) = 0x0000;
        }
        else
        {
            _6451:
            *(UINT16*)(MCU_memory+_17CB+0x0008) = *(UINT16*)(MCU_memory + _17CB + 0x08) - _17D0;
        }
    }
    else
    {
        _64BD:
        // 敌人角色mp
        if (*(UINT16*)(MCU_memory+_17CB+0x0C) < _17D0)
        {
            _64E8:
            *(UINT16*)(MCU_memory+_17CB+0x000C) = 0x0000;
        }
        else
        {
            _651E:
            *(UINT16*)(MCU_memory+_17CB+0x000C) = *(UINT16*)(MCU_memory + _17CB + 0x0C) - _17D0;
        }
    }
}

void _00241598(UINT8 _17CF, UINT8 _17D0)
{
    UINT8 _8A2E = 0x07;
    UINT8 _8A2F = 0x31;
    UINT8 _8A30 = 0x3E;
    UINT8 _8A31 = 0x42;
    UINT8 _8A36 = 0x02;
    UINT8 _8A3B = 0xFD;
    UINT8 _17CC;
    UINT16 _17CA;
    if (MCU_memory[0x1935])
    {
        _65B3:
        return;
    }
    _65B6:
    // 角色资源ID
    _17CC = MCU_memory[_17CF*0x0019+0x1988+0x01];
    __8F07(DAT_SUNDRYPIC, 0x02, 0x01, _17D0, _8A2E, _8A30);
    __8F07(DAT_SUNDRYPIC, 0x02, 0x02, 0x00, _8A2F, _8A31);
    __8F07(DAT_SUNDRYPIC, 0x01, _17CC, 0x07, _8A36 + _8A2F, _8A31+ _8A3B);
    // 指向角色属性
    _17CA = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    // hp
    _00241806(0x00, *(UINT16*)(MCU_memory+_17CA+0x08));
    // hpMax
    _00241806(0x01, *(UINT16*)(MCU_memory+_17CA+0x06));
    // mp
    _00241806(0x02, *(UINT16*)(MCU_memory+_17CA+0x0C));
    // mpMax
    _00241806(0x03, *(UINT16*)(MCU_memory+_17CA+0x0A));
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

void _00241806(UINT8 _17CF, UINT16 _17D0)
{
    const UINT8 _8A2F = 0x31;
    const UINT8 _8A31 = 0x42;
    const UINT8 _8A32[] = { 0x1E,0x37,0x1E,0x37,0x02 };
    const UINT8 _8A37[] = { 0x06,0x06,0x11,0x11,0xFD };
    __8F01(_8A32[_17CF] + _8A2F, _8A37[_17CF] + _8A31, (UINT32)_17D0, 0x01);
}

void _002418BC(UINT8 _17CF)
{
    UINT8 _17CE;
    MsgType _17CB;
    if (MCU_memory[0x1935])
    {
        _68D7:
        return;
    }
    _68DA:
    _00241A3F(_17CF);
    while (MCU_memory[0x1935] == 0x00)
    {
        _68E9:
        if (GuiGetMsg(&_17CB) == 0x00)
        {
            _691E:
            continue;
        }
        _6921:
        if (GuiTranslateMsg(&_17CB) == 0x00)
        {
            _6956:
            continue;
        }
        _6959:
        if (_17CB.type == DICT_WM_COMMAND)
        {
            _6965:
            if (_17CB.param == CMD_RETURN_HOME)
            {
                _6980:
                MCU_memory[0x1935] = 0xFE;
            }
        }
        _6985:
        if (_17CB.type == DICT_WM_CHAR_FUN)
        {
            _6994:
            if (_17CB.param == CHAR_LEFT)
            {
                _69E0:
                _17CE = 0x2D;
            }
            else if (_17CB.param == CHAR_RIGHT)
            {
                _69E9:
                _17CE = 0x2B;
            }
            else if (_17CB.param == CHAR_EXIT)
            {
                _69F2:
                break;
            }
            _69F5:
            __8E60(_17CE, &_17CF);
            _00241A3F(_17CF);
        }
    }
}

void _00241A3F(UINT8 _17CF)
{
    const UINT8 _8A3C[0x13] = { 0x07,0x32,0x57,0x32,0x57,0x09,0x19,0x29,0x39,0x49,0x58,0x68,0x0A,0x1A,0x2A,0x3A,0x4A,0x5A,0x6A };
    const UINT8 _8A4F[0x13] = { 0x04,0x09,0x09,0x15,0x15,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x39,0x39,0x39,0x39,0x39,0x39,0x39 };
    const UINT8 _8A62[] = { 0x08,0x04,0x02,0x01 };
    UINT8 _17CB;
    UINT8 _17CA;
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8* _17C6;
    UINT8 _17C5;
    UINT8 _17C4;
    UINT8 _17C3;
    UINT8 _17C2;
    UINT8 _17C1;
    INT16 _17BF; // 应为有符号数
    UINT16 _17BD;
    if (MCU_memory[0x1935])
    {
        _6A5A:
        return;
    }
    _6A5D:
    // 角色资源ID
    if (MCU_memory[_17CF*0x0019+0x1988+0x01] == 0x00)
    {
        _6A94:
        return;
    }
    _6A97:
    // 角色资源ID
    _17C3 = MCU_memory[_17CF*0x0019+0x1988+0x01];
    _17C6 = __8F0A(DAT_SUNDRYPIC, 0x02, 0x0B);
    _17C9 = _17C6[0x02];
    _17C8 = _17C6[0x03];
    _17CB = (UINT8)((0x009F-_17C9)/0x0002);
    _17CA = (0x60-_17C8)/0x02;
    __8F07(DAT_SUNDRYPIC, 0x02, 0x0B, 0x00, _17CB, _17CA);
    __8F07(DAT_SUNDRYPIC, 0x01, _17C3, 0x00, _8A3C[0] + _17CB, _17CA + _8A4F[0]);
    // 指向角色属性
    _17BD = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    for (UINT8 _17CC=0x01; _17CC<0x05; _17CC++)
    {
        _6C57:
        if (_17CC == 0x01)
        {
        _6C63:
            // hp
            _17BF = *(UINT16*)(MCU_memory+_17BD+0x08);
        }
        else if (_17CC == 0x02)
        {
        _6C95:
            // 幸运
            _17BF = MCU_memory[_17BD+0x18];
        }
        else if (_17CC == 0x03)
        {
        _6CC5:
            // 攻击
            _17BF = *(UINT16*)(MCU_memory+_17BD+0x0E);
        }
        else
        {
        _6CEB:
            // 身法
            _17BF = MCU_memory[_17BD+0x16];
        }
        _6D0C:
        __8F01(_8A3C[_17CC] + _17CB, _8A4F[_17CC] + _17CA, (UINT32)_17BF, 0x01);
    }
    _6D93:
    _17C5 = 0x0C;
    // 对特殊状态的免疫
    _17C1 = MCU_memory[_17BD+0x01];
    for (UINT8 _17CC=0x00; _17CC<0x07; _17CC++)
    {
        _6DCE:
        if (_17CC == 0x02)
        {
        _6DDA:
            //  玩家身法增减益
            _17BF = MCU_memory[_17CF*0x0005+0x1800+0x04];
            if (_17BF == 0x0000)
            {
                _6E38:
                _17C2 = 0x04;
            }
            else if (_17BF > 0x0080)
            {
                _6E61:
                _17C2 = 0x01;
            }
            else
            {
                _6E6A:
                _17C2 = 0x00;
            }
            _6E70:
            _17C4 = 0x00;
        }
        else if (_17CC < 0x02)
        {
            _6E85:
            if (_17CC == 0x00)
            {
            _6E8E:
                // 玩家攻击增减益
                _17BF = *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800);
            }
            else
            {
            _6ED6:
                // 玩家防御增减益
                _17BF = *(UINT16*)(MCU_memory+_17CF*0x0005+0x1800+0x02);
            }
            _6F1B:
            if (_17BF == 0x0000)
            {
                _6F36:
                _17C2 = 0x04;
            }
            else if (_17BF > 0x0000)
            {
                _6F5F:
                _17C2 = 0x01;
            }
            else // todo
            {
                _6F68:
                _17C2 = 0x00;
            }
            _6F6E:
            _17C4 = 0x00;
        }
        else
        {
            _6F77:
            _17C4 = _8A62[_17CC-0x03]&_17C1;
            if (_17C4)
            {
                _6FA6:
                _17C2 = 0x03;
            }
            else
            {
                _6FAF:
                _17C2 = 0x02;
            }
        }
        _6FB5:
        __8F07(DAT_SUNDRYPIC, 0x02, 0x0C, _17C2, _8A3C[_17CC + 0x0005] + _17CB, _8A4F[_17CC+0x0005]+_17CA);
        if (_17C4 == 0x00)
        {
            _7040:
            _17BF = MCU_memory[_17CC+ _17CF * 0x0007 + 0x18EC];
            if (_17BF == 0x00FF)
            {
                _70B7:
                _17BF = 0x0000;
            }
            _70C0:
            __8F01(_8A3C[_17C5 + _17CC] + _17CB, _8A4F[_17C5 + _17CC] + _17CA, (UINT32)_17BF, 0x01);
        }
    }
    _7151:
    __8DAE(MCU_memory + *(UINT16*)(MCU_memory+0x1936));
}

void _0024218A(UINT8 _17CF)
{
    UINT8 _17CD;
    UINT8 _17CB;
    //              普通攻击可能产生异常状态
    UINT8 _17CE = MCU_memory[*(UINT16*)(MCU_memory+_17CF+0x0019+0x1988+0x11)+0x02]&0x0F;
    if (_17CE)
    {
        _71EA:
        _17CB = MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x01];
        // 攻击的异常回合数
        _17CD = MCU_memory[*(UINT16*)(MCU_memory+_17CF+0x0019+0x1988+0x11)+0x19];
        if (_17CB == 0xAA)
        {
            _7267:
            for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
            {
                _7288:
                // 敌人角色资源ID
                if (MCU_memory[_17CC*0x0033+0x1826+0x01] == 0x00)
                {
                    _72BF:
                    continue;
                }
                _72C2:
                if (__8E51(_17CC, 0x02) != 0x00)
                {
                    _72F0:
                    __8E27(_17CC, _17CE, 0x02, _17CD);
                }
            }
        }
        else
        {
            _7328:
            if (__8E51(_17CB, 0x02) != 0x00)
            {
                _7356:
                __8E27(_17CB, _17CE, 0x02, _17CD);
            }
        }
    }
}

UINT8 _00242399(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2, UINT16* _17D4)
{
    UINT16 _17C9;
    UINT16 _17C7;
    UINT8 _17C5;
    UINT16 _17C3;
    UINT16 _17C1;
    UINT16 _17BF;
    UINT8 _17CB = 0x00;
    fillmem((UINT8*)_17D2, 0x0006, 0x00);
    fillmem((UINT8*)_17D4, 0x0006, 0x00);
    // 法术hp伤害
    if (_17D0[0x00] > 0x8000)
    {
        _747C:
        _17D0[0x00] = 0x0000 - _17D0[0x00];
        _17CB = 0x01;
    }
_74EA:
    // 法术mp伤害
    if (_17D0[0x01] > 0x8000)
    {
        _7516:
        _17D0[0x01] = 0x0000 - _17D0[0x01];
        _17CB = 0x01;
    }
    _7591:
    // 指向敌人角色等级开始的数据
    _17BF = _17CF * 0x0033 + 0x1826 + 0x0012;
    // 敌人身法=敌人身法增减益+敌人角色身法
    _17C5 = MCU_memory[_17CF*0x0005+0x180F+0x04]+ MCU_memory[_17BF + 0x01];
    for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
    {
        _766A:
        // 指向角色属性
        _17C1 = *(UINT16*)(MCU_memory+_17CC*0x0019+0x1988+0x11);
        // 角色资源ID
        if (MCU_memory[_17CC*0x0019+0x1988+0x01] == 0x00)
        {
            _76E6:
            continue;
        }
    _76E9:
        // hp
        if (*(UINT16*)(MCU_memory+_17C1+0x08) == 0x0000)
        {
            _7711:
            continue;
        }
        _7714:
        _17C7 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
        // 玩家身法=玩家身法增减益+玩家角色身法+20
        _17C3 = MCU_memory[_17CC*0x0005+0x1800+0x04]+ MCU_memory[_17C1 + 0x16] + 0x14;
        // 如果玩家身法>敌人身法
        if (_17C3 > _17C5)
        {
            _77C7:
            _17C3 -= _17C5;
        }
        else
        {
            _77F0:
            _17C3 = 0x0000;
        }
        _77F9:
        if (_17C7%0x0064 < _17C3)
        {
            _7822:
            if (_17D0[0x00] != 0x0000)
            {
                _7849:
                _17D2[_17CC] = 0xFFFF; // MISS
            }
            _7882:
            if (_17D0[0x01] != 0x0000)
            {
                _78A9:
                _17D4[_17CC] = 0xFFFF; // MISS
            }
            _78E2:
            continue;
        }
        _78E5:
        _17C9 = _17D0[0x00]; // 法术hp伤害
        //          敌人角色灵力
        _17C9 += MCU_memory[_17BF+0x02]*(_17C9 >> 0x0006);
        //       灵力
        _17C9 -= MCU_memory[_17C1+0x17]*(_17C9 >> 0x0006);
        _17C9 += (_17C7%_17C9)>>0x0004;
        if (MCU_memory[*(UINT16*)(MCU_memory+(_17CC<<1)+0x181E)] == 0x05 || MCU_memory[_17CC+0x1916])
        {
            _7A69:
            _17C9 -= _17C9>>0x0002;
        }
    _7AAA:
        //              hp
        if (_17C9 > *(UINT16*)(MCU_memory+_17C1+0x08))
        {
            _7AE2:
            _17D2[_17CC] = *(UINT16*)(MCU_memory + _17C1 + 0x08);
        }
        else
        {
            _7B44:
            _17D2[_17CC] = _17C9;
        }
        _7B80:
        _17C9 = _17D0[0x01];
        //          敌人角色灵力
        _17C9 -= MCU_memory[_17BF+0x02]*(_17C9 >> 0x0006);
        //       灵力
        _17C9 += MCU_memory[_17C1+0x17]*(_17C9 >> 0x0006);
        _17C9 += (_17C7%_17C9)>>0x0004;
        //          mp
        if (_17C9 > *(UINT16*)(MCU_memory+_17C1+0x0C))
        {
            _7CE9:
            _17D4[_17CC] = *(UINT16*)(MCU_memory + _17C1 + 0x0C);
        }
        else
        {
            _7D4B:
            _17D4[_17CC] = _17C9;
        }
    }
    _7D8A:
    return _17CB;
}

void _00242DA4(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2)
{
    UINT16 _17CA = 0x0000;
    UINT16 _17C8 = 0x0000;
    // 指向敌人角色等级开始的数据
    UINT8* _17C6 = MCU_memory + _17CF * 0x0033 + 0x1826 + 0x0012;
    for (UINT8 _17CC=0x00; _17CC<0x03; _17CC++)
    {
    _7E46:
        // hp
        if (*(UINT16*)(MCU_memory+*(UINT16*)(MCU_memory+_17CC*0x0019+0x1988+0x11)+0x08) == 0x0000)
        {
            _7E9C:
            continue;
        }
        _7E9F:
        _17CA += _17D0[_17CC];
        _17C8 += _17D2[_17CC];
    }
    _7F2C:
    if (_17CA-0x0000 != 0x0000)
    {
        _7F54:
        __8E72(0x2B, &_17CA, _17CF, 0x02, 0x01);
        // 敌人角色hp
        *(UINT16*)(_17C6+0x0008) = *(UINT16*)(_17C6 + 0x08) + _17CA;
    }
    _8011:
    if (_17C8-0x0000 != 0x0000)
    {
        _8039:
        __8E72(0x2B, &_17C8, _17CF, 0x02, 0x01);
        // 敌人角色mp
        *(UINT16*)(_17C6+0x000C) = *(UINT16*)(_17C6 + 0x0C) + _17C8;
    }
    _80F6:
    __8E87(_17C6, 0x02);
}

UINT8 _0024314B(UINT8 _17CF, UINT8 _17D0)
{
    UINT8 _17CE;
    UINT8 _17CD;
    UINT16 _17CB;
    if (_17D0 == 0x01)
    {
    _816A:
        // 幸运
        _17CE = MCU_memory[*(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11)+0x18];
    }
    else
    {
        _81AF:
        // 敌人角色幸运
        _17CE = MCU_memory[_17CF*0x0033+0x1826+0x0012+0x04];
    }
    _81F2:
    _17CB = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
    _17CD = _17CB%0x005A;
    if (_17CE < _17CD || _17CD > 0x3C)
    {
        _825E:
        return 0x01;
    }
    else
    {
        _8266:
        return 0x00;
    }
}

void _0024327E()
{
    for (UINT8 _17CF=0x00; _17CF<0x03; _17CF++)
    {
        _82AF:
        // 角色资源ID
        if (MCU_memory[_17CF*0x0019+0x1988+0x01] == 0x00)
        {
            _82E6:
            continue;
        }
        _82E9:
        __8E57(_17CF);
    }
}

void _0024330C(UINT8 _17CF)
{
    UINT8 _17CC;
    UINT16 _17CA;
    if (MCU_memory[0x1935])
    {
        _8327:
        return;
    }
    _832A:
    if (_17CF > 0x03)
    {
        _833B:
        return;
    }
    _833E:
    // 指向角色属性
    _17CA = *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x11);
    // hp == 0
    if (*(UINT16*)(MCU_memory+_17CA+0x08) == 0x0000)
    {
        _83AB:
        _17CC = 0x0B;
        __8E2A(_17CF, 0xFF, 0x01);
        MCU_memory[0x1926]--; // 玩家人数减1
    }
    else
    {
        _83E6:
        if (MCU_memory[_17CF * 0x0007 + 0x18EC +0x06] != 0xFF)
        {
            _842B:
            _17CC = 0x0A;
        }
        else
        {
        _8434:
            // hp                                       hpMax
            if (*(UINT16*)(MCU_memory + _17CA + 0x08) < *(UINT16*)(MCU_memory+_17CA+0x06)>>0x0002)
            {
                _8495:
                _17CC = 0x0A;
            }
            else
            {
                _849E:
                if (MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)] == 0x05 || MCU_memory[_17CF+0x1916])
                {
                    _84F1:
                    _17CC = 0x08;
                }
                else
                {
                    _84FA:
                    _17CC = 0x00;
                }
            }
        }
    }
    _8500:
    MCU_memory[*(UINT16*)(MCU_memory+(_17CF<<1)+0x181E)+0x0002] = _17CC;
}

void _00243555(UINT8 _17CF)
{
    UINT8 _17CE;
    if (MCU_memory[0x1935])
    {
        _8570:
        return;
    }
    _8573:
    _17CE = 0x00;
    if (_17CF == 0xAA)
    {
        _8585:
        for (_17CE=0x00; _17CE<0x03; _17CE++)
        {
            _85A6:
            _002435CB(_17CE);
        }
    }
    else
    {
        _85B3:
        _002435CB(_17CF);
    }
}

void _002435CB(UINT8 _17CF)
{
    UINT16 _17CB;
    UINT16 _17C9;
    UINT16 _17C7;
    UINT8 _17C6;
    UINT8 _17C5;
    if (MCU_memory[0x1935])
    {
        _85E6:
        return;
    }
    _85E9:
    // 敌人角色资源ID
    if (MCU_memory[_17CF*0x0033+0x1826+0x01] == 0x00)
    {
        _8620:
        return;
    }
    _8623:
    // 敌人角色hp
    if (*(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x0012+0x08) != 0x0000)
    {
        _867A:
        return;
    }
    _867D:
    // 指向敌人角色等级开始的数据
    _17C9 = _17CF * 0x0033 + 0x1826 + 0x0012;
    // 待增加角色经验值 += 敌人角色经验
    *(UINT16*)(MCU_memory+0x18E0) += *(UINT16*)(MCU_memory+_17C9+0x14);
    // 待增加角色金钱 += 敌人角色带钱
    *(UINT16*)(MCU_memory+0x18E2) += *(UINT16*)(MCU_memory+_17C9+0x12);
    // 指向敌人角色携带物品2
    _17CB = _17CF * 0x0033 + 0x1826 + 0x002B;
    // 敌人角色携带物品2数量
    if (MCU_memory[_17CB+0x02])
    {
        _87AC:
        _17C7 = SysRand((PtrRandEnv)(MCU_memory + 0x1928));
        // 幸运
        _17C6 = MCU_memory[*(UINT16*)(MCU_memory+MCU_memory[0x1A94]*0x0019+0x1988+0x11)+0x18];
        // 敌人角色幸运
        _17C5 = MCU_memory[_17C9+0x04];
        _17C6 = _17C7%_17C6+0x0001;
        _17C5 = _17C7%_17C5;
        if (_17C6 > _17C5)
        {
            _888E:
            if (_17C7%0x0004 != 0x0000)
            {
                _88B4:
                __8E12(_17CB);
            }
        }
    }
    _88DD:
    _002438F5(_17CF);
}

void _002438F5(UINT8 _17CF)
{
    UINT8* _17CB;
    if (MCU_memory[0x1935])
    {
        _8910:
        return;
    }
    _8913:
    // 指向敌人角色魔法链数据
    _17CB = MCU_memory + *(UINT16*)(MCU_memory+_17CF*0x0033+0x1826+0x31);
    if (_17CB != 0x0000)
    {
        _8973:
        if (SysMemFree(_17CB) == 0x00)
        {
            _89A6:
            MCU_memory[0x1935] = MEM_ERR;
        }
    }
_89AB:
    // 重置敌人角色数据
    fillmem(MCU_memory + _17CF * 0x0033 + 0x1826, 0x0033, 0x00);
}

// 删除文件类型为0x17的文件
UINT8 _00244000()
{
    UINT8 filehandle;
    UINT8 information[10];
    UINT16 filename;
    UINT16 filenum;
    UINT32 filelength;
    UINT8 _17CD = FileNum(0x17, &filenum);
    if (filenum -0x0000 == 0x0000)
    {
        _506B:
        return 0x01;
    }
    _5070:
    for (UINT16 _17C0=0x0001; _17C0<= filenum; _17C0++)
    {
        _50B1:
        _17CD = FileSearch(0x17, _17C0, &filename, information);
        _17CD = FileOpen(filename, 0x17, ReadAndWrite, &filehandle, &filelength);
        if (_17CD == 0x00)
        {
            _5190:
            return 0x00;
        }
        _5195:
        _17CD = FileDel(filehandle);
    }
    _51AB:
    return 0x01;
}

UINT8 _002441C3(UINT8 _17CF)
{
    UINT8 _17CA;
    UINT8 filehandle;
    UINT8 datalength;
    UINT16 filename;
    UINT32 filelength;
    if (MCU_memory[0x1935])
    {
        _51DE:
        return 0x00;
    }
    _51E3:
    datalength = _002461F9();
    if (_00246255(_17CF + 0x0064, &filename, 0x17) != 0x00)
    {
        _5241:
        _17CA = FileOpen(filename, 0x17, ReadOnly, &filehandle, &filelength);
        if (_17CA)
        {
            _52B9:
            _17CA = FileRead(filehandle, datalength, MCU_memory+0x1CAF);
            FileClose(filehandle);
            _17CF = __8DEE();
            if (_17CF == 0xFF)
            {
                _531A:
                return 0x00;
            }
            _531F:
            _00245A87(_17CF, MCU_memory + 0x1CAF);
            return 0x01;
        }
    }
    _5346:
    if (MCU_memory[0x194D] == 0xFF)
    {
        _5351:
        return 0x00;
    }
    _5356:
    if (_00246255(MCU_memory[0x194D] * 0x02 + 0x01, &filename, 0x09) != 0x00)
    {
        _53B3:
        _17CA = FileOpen(filename, 0x09, ReadOnly, &filehandle, &filelength);
        if (_17CA)
        {
            _542B:
            filelength = (UINT32)datalength;
            filelength *= _17CF-0x01;
            _17CA = FileSeek(filehandle, filelength, FromTop);
            _17CA = FileRead(filehandle, datalength, MCU_memory+0x1CAF);
            FileClose(filehandle);
            _17CF = __8DEE();
            if (MCU_memory[0x1CAF] == 0xFF || MCU_memory[0x1CAF] == 0xE0 || _17CF == 0xFF)
            {
                _552F:
                return 0x00;
            }
            _5534:
            _00245A87(_17CF, MCU_memory + 0x1CAF);
            return 0x01;
        }
    }
    _555B:
    return 0x00;
}

UINT8 _00244573(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2)
{
    UINT8 _17CA;
    UINT8 filehandle;
    UINT16 filename;
    UINT32 filelength;
    if (_00246255(_17CF * 0x02, &filename, 0x09) == 0x00)
    {
        _55E1:
        return 0x00;
    }
    _55E6:
    _17CA = FileOpen(filename, 0x09, ReadOnly, &filehandle, &filelength);
    if (_17CA)
    {
        _565E:
        FileRead(filehandle, 0x0C, _17D2);
        FileRead(filehandle, 0x03, _17D0);
        _17CA = FileClose(filehandle);
    }
    _56D5:
    return 0x01;
}

UINT8 _002446ED(UINT8 _17CF)
{
    const UINT8 _8C05[0x0D] = { 0xFF,0xFF,0xFF,0xFF,0x2D,0xFF,0xFF,0x5A,0xEA,0xEA,0xEA,0xEA,0x68 };
    UINT8 filehandle;
    UINT8* bufadd;
    UINT16 filename;
    UINT32 filelength;
    if (MCU_memory[0x1935])
    {
        _5708:
        return 0x00;
    }
    _570D:
    if (_00246255(_17CF * 0x02, &filename, 0x09) == 0x00)
    {
        _5768:
        return 0x00;
    }
    _576D:
    __8EE9(0x03);
    if (MCU_memory[0x1935])
    {
        _5782:
        return 0x00;
    }
    _5787:
    bufadd = MCU_memory+0x1CAF;
    if (FileOpen(filename, 0x09, ReadOnly, &filehandle, &filelength))
    {
        _5812:
        for (UINT8 _17CA=0x00; _17CA<0x0D; _17CA++)
        {
            _5833:
            FileRead(filehandle, _8C05[_17CA], bufadd);
            _0024649F(_17CA, bufadd);
            if (MCU_memory[0x1935])
            {
                _58B3:
                break;
            }
        }
        _58B9:
        FileClose(filehandle);
    }
    _58C8:
    if (_002457C3(_17CF, bufadd) == 0x00)
    {
        _58F7:
        return 0x00;
    }
    _58FC:
    return 0x01;
}

UINT8 _00244914(UINT8 _17CF)
{
    const UINT8 _8C12[] = "覆盖原进度？";
    const UINT32 _8C1F = 0x00000BB8;
    UINT8 _17CA;
    UINT8 filehandle;
    UINT8 datalength;
    UINT8 information[0x0F];
    UINT8* bufadd;
    UINT8 _17B5;
    UINT16 filename;
    UINT32 filelength;
    if (_00246255(_17CF * 0x02, &filename, 0x09) != 0x00)
    {
        _5982:
        _SysMemcpy(information, _8C12, 0x000F);
        _17B5 = __8EF2(0x00, 0x00, information);
        if (_17B5 == 0x00 || _17B5 == 0xFE)
        {
            _5A08:
            _17CA = FileOpen(filename, 0x09, ReadAndWrite, &filehandle, &filelength);
            _17CA = FileDel(filehandle);
        }
        else
        {
            _5A8D:
            return 0x00;
        }
    }
    _5A92:
    __8DF7();
    bufadd = MCU_memory+0x1CAF;
    fillmem(information, 0x000F, 0x00);
    // 获取游戏名字符数
    datalength = (UINT8)strlen(MCU_memory+0x1938);
    // 复制游戏名到 information
    strcpy(information, MCU_memory+0x1938);
    information[datalength] = _17CF * 0x02 + 0x30;
    filelength = _8C1F;
    _17CA = FileCreat(0x09, filelength, information, &filename, &filehandle);
    if (_17CA == 0x00)
    {
        _5C29:
        return 0x00;
    }
    _5C2E:
    for (UINT8 _17C7=0x00; _17C7<0x10; _17C7++)
    {
        _5C4F:
        if (_00246F44(_17C7, bufadd, &datalength) == 0x00)
        {
            _5C8E:
            break;
        }
        _5C91:
        _17CA = FileWrite(filehandle, datalength, bufadd);
    }
    _5CCE:
    _17CA = FileClose(filehandle);
    if (_00244D2D(_17CF, bufadd) == 0x00)
    {
        _5D10:
        return 0x00;
    }
    _5D15:
    return 0x01;
}

UINT8 _00244D2D(UINT8 _17CF, UINT8* _17D0)
{
    const UINT32 _8C23 = 0x00000019;
    UINT8 _17CA;
    UINT8 information[0x0F];
    UINT8 _17BA;
    UINT8 _17B9;
    UINT8 datalength;
    UINT8 _17B7;
    UINT8 _17B5;
    UINT8 _179C[0x19];
    UINT8 _179B;
    UINT8 _1799;
    UINT16 filename;
    UINT16 _1793;
    UINT32 _178F;
    UINT32 _178B;
    fillmem(_179C, 0x0019, 0x00);
    UINT8 _17B6 = 0x00;
    if (MCU_memory[0x194D] != 0xFF)
    {
        _5D8C:
        if (_00246255(MCU_memory[0x194D] * 0x02 + 0x01, &filename, 0x09) != 0x00)
        {
            _5DE9:
            _17B6 = 0x01;
            _17CA = FileOpen(filename, 0x09, ReadAndWrite, &_17B7, &_178F);
        }
    }
    _5E5E:
    if (MCU_memory[0x194D] != _17CF)
    {
        _5E6B:
        if (_00246255(_17CF * 0x02 + 0x01, &filename, 0x09) != 0x00)
        {
            _5EC9:
            _17CA = FileOpen(filename, 0x09, ReadAndWrite, &_17BA, &_178F);
            _17CA = FileDel(_17BA);
        }
    }
    _5F4B:
    fillmem(information, 0x000F, 0x00);
    // 获取游戏名字符数
    _17B5 = (UINT8)strlen(MCU_memory+0x1938);
    // 复制游戏名到 information
    strcpy(information, MCU_memory+0x1938);
    information[_17B5] = _17CF * 0x02 + 0x01 + 0x30;
    datalength = _002461F9();
    _178F = (UINT32)datalength;
    _178F *= _8C23;
    _17CA = FileCreat(0x09, _178F, information, &filename, &_17BA);
    if (_17CA == 0x00)
    {
        _6105:
        if (_17B6)
        {
            _610E:
            FileClose(_17B7);
        }
        _611D:
        return 0x00;
    }
    _6122:
    for (UINT8 _179A=0x00; _179A<0x03; _179A++)
    {
        _6143:
        // 角色资源ID
        _179B = MCU_memory[_179A*0x0019+0x1988+0x01];
        if (_179B)
        {
            _6182:
            _179C[_179B-0x01] = 0x01;
            _00245E6C(_179A, _17D0);
            UINT32 _178B = (UINT32)datalength;
            _178B *= (UINT32)(_179B-0x01);
            _17CA = FileSeek(_17BA, _178B, FromTop);
            _17CA = FileWrite(_17BA, datalength, _17D0);
        }
    }
    _62A2:
    _17CA = FileNum(0x17, &_1793);
    if (_1793-0x0000 != 0x0000)
    {
        _62FD:
        for (UINT16 fileorder =0x0001; fileorder <=_1793; fileorder++)
        {
            _633E:
            _17CA = FileSearch(0x17, fileorder, &filename, information);
            _17CA = FileOpen(filename, 0x17, ReadAndWrite, &_17B9, &_178B);
            _17CA = FileRead(_17B9, datalength, _17D0);
            _17CA = FileClose(_17B9);
            if (_17D0[0] == 0xFF || _17D0[0] == 0xE0)
            {
                _648F:
                continue;
            }
            _6492:
            _1799 = _17D0[0x01]-0x01;
            if (_179C[_1799])
            {
                _64C8:
                continue;
            }
            _64CB:
            _179C[_1799] = 0x01;
            UINT32 _178B = (UINT32)_1799;
            _178B *= (UINT32)datalength;
            _17CA = FileSeek(_17BA, _178B, FromTop);
            _17CA = FileWrite(_17BA, datalength, _17D0);
        }
    }
    _65C0:
    if (_17B6)
    {
        _65C9:
        for (UINT8 _179A=0x00; _179A<=0x19; _179A++)
        {
            _65EA:
            if (_179C[_179A])
            {
                _660A:
                continue;
            }
            _660D:
            _178B = (UINT32)datalength;
            _178B *= (UINT32)_179A;
            _17CA = FileSeek(_17B7, _178B, FromTop);
            _17CA = FileRead(_17B7, datalength, _17D0);
            _17CA = FileSeek(_17BA, _178B, FromTop);
            _17CA = FileWrite(_17BA, datalength, _17D0);
        }
    }
    _6748:
    if (_17B6 == 0x00)
    {
        _6751:
        MCU_memory[0x194D] = _17CF;
    }
    else
    {
        _675B:
        if (MCU_memory[0x194D] == _17CF)
        {
            _6768:
            _17CA = FileDel(_17B7);
        }
        else
        {
            _677E:
            MCU_memory[0x194D] = _17CF;
            _17CA = FileClose(_17B7);
        }
    }
    _6798:
    _17CA = FileClose(_17BA);
    return 0x01;
}

UINT8 _002457C3(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17C9;
    UINT8 filehandle;
    UINT8 _17C7;
    UINT16 filename;
    UINT32 filelength;
    UINT32 _17BD;
    if (_00246255(_17CF * 0x02 + 0x01, &filename, 0x09) == 0x00)
    {
        _6834:
        return 0x00;
    }
    _6839:
    _17C9 = FileOpen(filename, 0x09, ReadAndWrite, &filehandle, &filelength);
    if (_17C9 == 0x00)
    {
        _68B1:
        return 0x00;
    }
    _68B6:
    _17C7 = _002461F9();
    for (UINT8 _17CA=0x00; _17CA<0x03; _17CA++)
    {
        _68DE:
        // 角色资源ID
        if (MCU_memory[_17CA*0x0019+0x1988+0x01] == 0x00)
        {
            _6915:
            continue;
        }
        _6918:
        // 角色资源ID
        _17BD = MCU_memory[_17CA*0x0019+0x1988+0x01]-0x01;
        _17BD *= _17C7;
        _17C9 = FileSeek(filehandle, _17BD, FromTop);
        _17C9 = FileRead(filehandle, _17C7, _17D0);
        if (__8DF4(_17CA) == 0x00)
        {
            _6A24:
            FileClose(filehandle);
            return 0x00;
        }
        _6A38:
        _00245A87(_17CA, _17D0);
    }
    _6A60:
    FileClose(filehandle);
    return 0x01;
}

UINT8 _00245A87(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CA = 0x0F;
    // 复制角色基础信息
    _SysMemcpy(MCU_memory + _17CF * 0x0019 + 0x1988, _17D0, _17CA);
    // 复制角色姓名
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0F), _17CA + _17D0, 0x000C);
    _17CA += 0x0C;
    // 复制角色属性
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), _17CA + _17D0, 0x001A);
    _17CA += 0x1A;
    // 复制角色装备
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x13), _17CA + _17D0, 0x0008);
    _17CA += 0x08;
    // 复制角色魔法链数据
    _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x15), _17CA + _17D0, 0x0064);
    // 指向背包中的道具
    *(UINT16*)(MCU_memory+_17CF*0x0019+0x1988+0x0017) = *(UINT16*)(MCU_memory+0x192F);
    return 0x01;
}

UINT8 _00245E6C(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17CA = 0x0F;
    // 复制角色基础信息
    _SysMemcpy(_17D0, MCU_memory + _17CF * 0x0019 + 0x1988, _17CA);
    // 复制角色姓名
    _SysMemcpy(_17CA + _17D0, MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x0F), 0x000C);
    _17CA += 0x0C;
    // 复制角色属性
    _SysMemcpy(_17CA + _17D0, MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x11), 0x001A);
    _17CA += 0x1A;
    // 复制角色装备
    _SysMemcpy(_17CA + _17D0, MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x13), 0x0008);
    _17CA += 0x08;
    // 复制角色魔法链数据
    _SysMemcpy(_17CA + _17D0, MCU_memory + *(UINT16*)(MCU_memory + _17CF * 0x0019 + 0x1988 + 0x15), 0x0064);
    return 0x01;
}

// 返回角色全部数据需要的字节长度
UINT8 _002461F9()
{
    UINT8 _17CF = 0x0F; // 角色基础信息数据长度
    _17CF += 0x0C; // 角色姓名数据长度
    _17CF += 0x1A; // 角色属性数据长度
    _17CF += 0x08; // 角色装备数据长度
    _17CF += 0x64; // 角色魔法链数据长度
    return _17CF;
}

// 查找存档文件，找到返回1，没找到返回0
UINT8 _00246255(UINT8 _17CF, UINT16* filename, UINT8 filetype)
{
    UINT8 _17B8[10];
    UINT16 filenum;
    UINT8 _17C2[0x0A];
    fillmem(_17C2, 0x000A, 0x00);
    // 获取游戏名字符数
    UINT8 _17CC = (UINT8)strlen(MCU_memory+0x1938);
    // 复制游戏名到 _17C2
    strcpy(_17C2, MCU_memory+0x1938);
    _17C2[_17CC] = _17CF + 0x30;
    UINT8 _17CD = FileNum(filetype, &filenum);
    if (filenum -0x0000 == 0x0000)
    {
        _739D:
        return 0x00;
    }
    _73A2:
    for (UINT16 fileorder =0x0001; fileorder <= filenum; fileorder++)
    {
        _73E3:
        _17CD = FileSearch(filetype, fileorder, filename, _17B8);
        if (strcmp(_17C2, _17B8) == 0x0000)
        {
            _747F:
            return 0x01;
        }
    }
    _7487:
    return 0x00;
}

UINT8 _0024649F(UINT8 _17CF, UINT8* _17D0)
{
    UINT8 _17C9;
    UINT8 _17C7;
    UINT8 _17C5;
    UINT8 _17C6 = 0x00;
    switch (_17CF)
    {
    case 0x0000: // _74D9
        //          当前地图的名称
        _SysMemcpy(MCU_memory+0x1942, _17D0, 0x000C);
        // 角色1资源ID
        MCU_memory[0x1988+0x0001] = _17D0[0x0C];
        // 角色2资源ID
        MCU_memory[0x19A1+0x0001] = _17D0[0x0D];
        // 角色3资源ID
        MCU_memory[0x19BA+0x0001] = _17D0[0x0E];
        MCU_memory[0x1A94] = _17D0[0x0F];
        MCU_memory[0x1A95] = _17D0[0x10]; // 背包中的道具种类数
        MCU_memory[0x1A96] = _17D0[0x11]; // 遇怪开关
        MCU_memory[0x1AAB] = _17D0[0x12];
        MCU_memory[0x1AAC] = _17D0[0x13];
        MCU_memory[0x1AA4] = _17D0[0x14];
        *(UINT16*)(MCU_memory+0x1AA0) = _17D0[0x15];
        *(UINT16*)(MCU_memory+0x1AA0) <<= 0x0008;
        *(UINT16*)(MCU_memory+0x1AA0) += _17D0[0x16];
        _17C9 = _17D0[0x17];
        _17C9 <<= 0x0008;
        _17C9 += _17D0[0x18];
        *(UINT16*)(MCU_memory+0x1AA2) = _17C9;
        MCU_memory[0x18DD] = _17D0[0x19]; // 大战斗背景
        MCU_memory[0x18DE] = _17D0[0x1A]; // 右上角图
        MCU_memory[0x18DF] = _17D0[0x1B]; // 左下角图
        //          玩家拥有的金钱
        _SysMemcpy(MCU_memory + 0x1A8F, _17D0 + 0x001C, 0x0004);
        // 可能出现的敌人的种类
        _SysMemcpy(MCU_memory + 0x18E4, _17D0 + 0x0020, 0x0008);
        _SysMemcpy(MCU_memory + 0x197C, _17D0 + 0x0028, 0x000C);
        MCU_memory[0x1A97] = _17D0[0x34]; // 音乐号码
        MCU_memory[0x1A98] = _17D0[0x35]; // 音乐持续次数
        MCU_memory[0x1AAD] = _17D0[0x36]; // 音乐开关状态
        _SysMemcpy(MCU_memory+0x194E, _17D0 + 0x0037, 0x0028);
        _SysMemcpy(MCU_memory+0x1976, _17D0 + 0x0037 + 0x0028, 0x0003);
        MCU_memory[0x1979] = _17D0[0x62]; // 当前地图的类型
        MCU_memory[0x197A] = _17D0[0x63]; // 当前地图的序号
        MCU_memory[0x1A9B] = _17D0[0x64]; // 是否能够存档
        MCU_memory[0x1A9C] = _17D0[0x65]; // 定时触发事件
        // 定时触发事件的定时时间
        *(UINT16*)(MCU_memory+0x1A9D) = _17D0[0x66];
        *(UINT16*)(MCU_memory + 0x1A9D) <<= 0x0008;
        *(UINT16*)(MCU_memory+0x1A9D) += _17D0[0x67];
        MCU_memory[0x197B] = _17D0[0x68]; // 是否显示主角在当前地图的坐标功能
        break;
    case 0x0001: // _7AF7
        _SysMemcpy(MCU_memory + 0x1AAE, _17D0, 0x0201);
        break;
    case 0x0002: // _7B47
        _SysMemcpy(MCU_memory+*(UINT16*)(MCU_memory+0x1AA7), _17D0, 0x00FF);
        break;
    case 0x0003: // _7B83
        _SysMemcpy(MCU_memory+*(UINT16*)(MCU_memory+0x1AA9), _17D0, 0x00FF);
        break;
    case 0x0004: // _7BBF
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x1AA9) + 0x00FF, _17D0, 0x002D);
        break;
    case 0x0005: // _7C26
        //          指向背包中的道具
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x192F), _17D0, 0x00FF);
        break;
    case 0x0006: // _7C78
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x192F) + 0x02FD, _17D0, 0x00FF);
        break;
    case 0x0007: // _7CDF
        _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + 0x192F) + 0x02FD, _17D0, 0x005A); // fixme 根据另一行代码推测应该是0x05FA
        break;
    case 0x0008: // _7D46
        _17C7 = 0x00;
        _17C6 = 0x01;
        break;
    case 0x0009: // _7D55
        _17C7 = 0x09;
        _17C6 = 0x01;
        break;
    case 0x000A: // _7D64
        _17C7 = 0x12;
        _17C6 = 0x01;
        break;
    case 0x000B: // _7D73
        _17C7 = 0x1B;
        _17C6 = 0x01;
        break;
    case 0x000C: // _7D82
        _17C7 = 0x24;
        _17C6 = 0x01;
        break;
    }
    _7D91:
    if (_17C6)
    {
        _7D9A:
        _17C5 = 0x1A;
        for (UINT8 _17C8=_17C7; _17C8<_17C7+0x09; _17C8++)
        {
            _7DCC:
            if (_17D0[0])
            {
                _7DE0:
                *(UINT16*)(MCU_memory + (_17C8 << 1) + 0x19D3) = (UINT16)(SysMemAllocate(_17C5 + 0x02) - MCU_memory);
                if (*(UINT16*)(MCU_memory+(_17C8<<1)+0x19D3) == 0x0000)
                {
                    _7E86:
                    MCU_memory[0x1935] = MEM_ERR;
                    break;
                }
                _7E8E:
                _SysMemcpy(MCU_memory + *(UINT16*)(MCU_memory + (_17C8 << 1) + 0x19D3), _17D0, _17C5);
            }
            _7EFC:
            _17D0 += _17C5;
        }
    }
    _7F2C:
    return 0x01;
}

UINT8 _00246F44(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2)
{
    UINT8 _17C9;
    UINT8 _17C8;
    UINT8 _17C7 = 0x00;
    fillmem(_17D0, 0x00FF, 0x00);
    switch (_17CF)
    {
    case 0x0000: // _7FB7
        //                  当前地图的名称
        _SysMemcpy(_17D0, MCU_memory+0x1942, 0x000C);
        // 角色1资源ID
        _17D0[0x000C] = MCU_memory[0x1988 + 0x01];
        // 角色2资源ID
        _17D0[0x000D] = MCU_memory[0x19A1 + 0x01];
        // 角色3资源ID
        _17D0[0x000E] = MCU_memory[0x19BA + 0x01];
        _17D0[0x000F] = MCU_memory[0x1A94]; // 显示第几个角色
        _17D0[0x0010] = MCU_memory[0x1A95]; // 身上携带的道具种类数
        _17D0[0x0011] = MCU_memory[0x1A96]; // 遇怪开关
        _17D0[0x0012] = MCU_memory[0x1AAB]; // 当前GUT的节
        _17D0[0x0013] = MCU_memory[0x1AAC]; // 当前GUT的章
        _17D0[0x0014] = MCU_memory[0x1AA4];
        // 到当前要执行的GUT指令的偏移量
        _17D0[0x0015] = *(UINT16*)(MCU_memory + 0x1AA0) >> 0x0008;
        _17D0[0x0016] = (UINT8) *(UINT16*)(MCU_memory + 0x1AA0);
        // 指向当前GUT数据的起始位置
        _17D0[0x0017] = *(UINT16*)(MCU_memory + 0x1AA2) >> 0x0008;
        _17D0[0x0018] = (UINT8) *(UINT16*)(MCU_memory + 0x1AA2);
        _17D0[0x0019] = MCU_memory[0x18DD]; // 大战斗背景
        _17D0[0x001A] = MCU_memory[0x18DE]; // 右上角图
        _17D0[0x001B] = MCU_memory[0x18DF]; // 左下角图
        //                          玩家拥有的金钱
        _SysMemcpy(_17D0 + 0x001C, MCU_memory + 0x1A8F, 0x0004);
        // 可能出现的敌人的种类
        _SysMemcpy(_17D0 + 0x0020, MCU_memory + 0x18E4, 0x0008);
        _SysMemcpy(_17D0 + 0x0028, MCU_memory + 0x197C, 0x000C);
        _17D0[0x0034] = MCU_memory[0x1A97]; // 音乐号码
        _17D0[0x0035] = MCU_memory[0x1A98]; // 音乐持续次数
        _17D0[0x0036] = MCU_memory[0x1AAD]; // 音乐开关状态
        _SysMemcpy(_17D0 + 0x0037, MCU_memory+0x194E, 0x0028);
        _SysMemcpy(_17D0 + 0x0037 + 0x0028, MCU_memory+0x1976, 0x0003);
        _17D0[0x0062] = MCU_memory[0x1979]; // 当前地图的类型
        _17D0[0x0063] = MCU_memory[0x197A]; // 当前地图的序号
        _17D0[0x0064] = MCU_memory[0x1A9B]; // 是否能够存档
        _17D0[0x0065] = MCU_memory[0x1A9C]; // 定时触发事件
        // 定时触发事件的定时时间
        _17D0[0x0066] = *(UINT16*)(MCU_memory + 0x1A9D) >> 0x0008;
        _17D0[0x0067] = (UINT8) *(UINT16*)(MCU_memory + 0x1A9D);
        _17D0[0x0068] = MCU_memory[0x197B]; // 是否显示主角在当前地图的坐标功能
        *_17D2 = 0xFF;
        break;
    case 0x0001: // _871B
        _SysMemcpy(_17D0, MCU_memory + 0x1AAE, 0x0201);
        *_17D2 = 0xFF;
        break;
    case 0x0002: // _8784
        _SysMemcpy(_17D0, MCU_memory+*(UINT16*)(MCU_memory+0x1AA7), 0x00FF);
        *_17D2 = 0xFF;
        break;
    case 0x0003: // _87D9
        _SysMemcpy(_17D0, MCU_memory+*(UINT16*)(MCU_memory+0x1AA9), 0x00FF);
        *_17D2 = 0xFF;
        break;
    case 0x0004: // _882E
        _SysMemcpy(_17D0, MCU_memory + *(UINT16*)(MCU_memory + 0x1AA9) + 0x00FF, 0x002D);
        *_17D2 = 0x2D;
        break;
    case 0x0005: // _88AE
        //                  指向背包中的道具
        _SysMemcpy(_17D0, MCU_memory + *(UINT16*)(MCU_memory + 0x192F), 0x00FF);
        *_17D2 = 0xFF;
        break;
    case 0x0006: // _8919
        _SysMemcpy(_17D0, MCU_memory + *(UINT16*)(MCU_memory + 0x192F) + 0x02FD, 0x00FF);
        *_17D2 = 0xFF;
        break;
    case 0x0007: // _8999
        *_17D2 = 0x5A;
        _SysMemcpy(_17D0, MCU_memory + *(UINT16*)(MCU_memory + 0x192F) + 0x05FA, 0x005A);
        break;
    case 0x0008: // _8A19
        _17C9 = 0x00;
        _17C7 = 0x01;
        break;
    case 0x0009: // _8A28
        _17C9 = 0x09;
        _17C7 = 0x01;
        break;
    case 0x000A: // _8A37
        _17C9 = 0x12;
        _17C7 = 0x01;
        break;
    case 0x000B: // _8A46
        _17C9 = 0x1B;
        _17C7 = 0x01;
        break;
    case 0x000C: // _8A55
        _17C9 = 0x24;
        break;
    default: // _8A5E
        return 0x00;
        break;
    }
    _8A63:
    if (_17C7)
    {
        _8A6C:
        _17C8 = 0x00;
        for (UINT8 _17CA=_17C9; _17CA<_17C9+0x09; _17CA++)
        {
            _8A9E:
            if (_17CA >= 0x28)
            {
                _8AAA:
                break;
            }
            _8AAD:
            if (*(UINT16*)(MCU_memory+(_17CA<<1)+0x19D3) != 0x0000)
            {
                _8AE4:
                _SysMemcpy(_17D0 + _17C8, MCU_memory + *(UINT16*)(MCU_memory + (_17CA << 1) + 0x19D3), 0x001A);
            }
            _8B7D:
            _17C8 += 0x1A;
        }
        _8B8B:
        *_17D2 = _17C8;
    }
    _8BA6:
    return 0x01;
}

UINT8 _00247BBE(UINT8 _17CF, UINT8* _17D0)
{
    return _00245E6C(_17CF, _17D0);
}

UINT8 _00247BFE()
{
    return _002461F9();
}
