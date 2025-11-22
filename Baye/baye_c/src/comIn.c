/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	comIn.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		封装系统输入及其它和显示无关的功能函数
 **
 *移植性声明:
 *	1、符合标准：《游戏设计标准V1.0》
 *	2、兼容模式：本程序和界面无关，无兼容模式。
 *	3、使用本封装程序的引擎要移植到其他环境中，系统的输入部分只需在此修改
 *修改历史：
 *	版本    日期     作者     改动内容和原因
 *   ----------------------------------------------------
 *	1.0    2005.5.16  高国军     基本的功能完成
 ***********************************************************************/
/* 声明本文件可以直接调用系统底层函数宏 */
#define		_BE_CHANGED_
#include "baye/stdsys.h"
#include "baye/comm.h"
#include "baye/enghead.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void GamLoadEngineConfig(void);

/* 当前所在文件 */
#define		IN_FILE		20

static const U8* g_fontDir = NULL;
static const U8* datFilePath = NULL;
static const U8* altDatFilePath = NULL;
static U8 initialized = 0;

static GMType	lastMsg = {0};

/***********************************************************************
 * 说明:     初始化游戏引擎所在的机型环境
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 GamConInit(void)
{
    U8 i;
    if (initialized) {
        return 0;
    }
    initialized = 1;
    /*系统环境初始化*/
    GuiInit();						/*Gui OS 初始化，使用前一定要调用 */
    FlashInit();						/*初始化flash文件系统变量*/
    SysMemInit(MEM_HEAP_START,MEM_HEAP_SIZE);		/*初始化堆*/
    SysIconAllClear();					/*清空所有Icon*/
    g_GamKBState=GuiGetKbdState();				/*取键盘状态 */
    GuiSetInputFilter(INPUT_ENG_ENABLE|INPUT_NUM_ENABLE);	/*只响应英文键盘*/
    GuiSetKbdType(SYS_ENG_KBD|SYS_NUM_KBD);			/*设置新的键盘状态*/
    g_GamKeySound=SysGetKeySound();				/*备份按键声音设定*/
    SysSetKeySound(false);					/*关闭按键声音*/
    SysTimer1Open(TIMER_INT);				/*设置定时器事件触发*/
    DataBankSwitch(4,1,EXTMEM_BNKNUM);			/*将当前的第4个页面切换成内存-增加可用的内存空间*/

    {
        U8 path[256];
        sprintf((char*)path, "%s/font.bin", (char*)g_fontDir);
        /*文件指针初始化*/
        g_FontFp12 = gam_fopen(path,'r');
        if(NULL == g_FontFp12) {
            printf("Open %s failed\n", path);
            return 1;
        }
        for (i = 1; i <= 4; i++) {
            sprintf((char*)path, "%s/font24.cn.%d", (char*)g_fontDir, i);
            g_FontsFp24[i-1] = gam_fopen((U8*)path, 'r');
            if(NULL == g_FontsFp24[i-1]) {
                printf("Open %s failed\n", path);
            }
        }
        for (i = 1; i <= 2; i++) {
            sprintf((char*)path, "%s/font24.en.%d", (char*)g_fontDir, i);
            g_FontsFp24En[i-1] = gam_fopen((U8*)path, 'r');
            if(NULL == g_FontsFp24En[i-1]) {
                printf("Open %s failed\n", path);
            }
        }
    }
    GamSetFont(0);
    GamSetFontEn(0);

    if (altDatFilePath) {
        g_LibFp = gam_fopen(altDatFilePath,'r');			/*打开自定义资源库文件*/
    }

    if(NULL == g_LibFp) {
        g_LibFp = gam_fopen(datFilePath,'r');			/*打开资源库文件*/
    } else {
        printf("Using custom lib\n");
    }

    if(NULL == g_LibFp) {
        printf("Can not load lib\n");
        printf("tried:\n");
        printf("%s\n", altDatFilePath);
        printf("%s\n", datFilePath);
        return 1;
    }
    g_CBnkPtr = gam_freadall(g_LibFp, NULL);				/*常量页面指针*/

    {
        extern U32	GetResStartAddr(U16 id);
        U32 magic = GetResStartAddr(17);
        U32 version;

        if (magic == 0xffffffff) {
            version = 0;
        } else {
            version = ((magic & 0xff00) >> 8) + 1;
        }
        printf("lib version=%d magic=0x%x\n", version, magic);

        if (version != 1) {
            printf("Unsupported lib version: %d\n", version);
            abort();
        }
        g_scale = magic & 0xf;
        printf("g_scale=%d\n", g_scale);

        void vs_ptr_init(void);
        void screen_buffer_init(void);

        vs_ptr_init();
        screen_buffer_init();


#ifdef __EMSCRIPTEN__
        EM_ASM_INT ({
            if (window.lcdSetDotSize) {
                window.lcdSetDotSize($0)
            }
        }, g_scale);
#endif
    }

    /*随机函数初始化*/
    i = SysGetSecond();
    gam_srand(i);						/*初始化随机数产生队列*/

    /*汉字显示初始化*/
    GetDataBankNumber(9,&c_FontBnkS);			/*获取字库起始地址*/
    c_ReFlag = true;
    c_Sx = WK_SX;
    c_Sy = WK_SY;
    c_Ex = WK_EX;
    c_Ey = WK_EY;

    /*虚拟屏幕缓冲指针*/
    g_VisScr = VS_PTR;
    gam_memset(g_VisScr,0,MAX_SCR_BUF_LEN);

    /* 从lib读取引擎参数 */
    GamLoadEngineConfig();
    return 0;
}
/***********************************************************************
 * 说明:     恢复系统参数
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void GamConRst(void)
{
    SysTimer1Close();
    gam_fclose(g_LibFp);
    SysSetKeySound(g_GamKeySound);		/* 恢复按键声音设定 */
    GuiSetKbdState(g_GamKBState);		/* 恢复键盘状态 */
}
/***********************************************************************
 * 说明:     获取系统消息，并转换为游戏的消息体制
 * 输入参数: pMsg-消息指针
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void GamGetMsg(GMType *pMsg)
{
#if GAM_VER==GAM_DEBUG_MODE
    if(g_RunErr!=NONE_ERR)
        gamTraceP(0xFF000000+g_RunErr);
#endif
    while(1)
    {
        if(!GuiGetMsg((PtrMsg)pMsg))		continue;
        if(!GuiTranslateMsg((PtrMsg)pMsg))	continue;
        break;
    }
    lastMsg = *pMsg;
}
/***********************************************************************
 * 说明:     延时程序
 * 输入参数: dly-延时秒(0-有键按下时才返回)	keyflag-延时过程中按键可否返回
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 GamDelay(U16 dly, BOOL keyflag)
{
    U8	tInt;
    GMType	pMsg;

    int tprev = SysScrollingTimerOpen(0);
    tInt = SysGetTimer1Number();
    SysTimer1Close();
    if(!dly) {
        do {
            GamGetMsg(&pMsg);
        } while (pMsg.type == VM_TIMER);
    }
    else
    {
        SysTimer1Open(TIMER_DLY);
        while(1)
        {
            if(!dly) break;
            GamGetMsg(&pMsg);
            if(VM_TIMER == pMsg.type && pMsg.param == 1)
                dly -= 1;
            else if (GamMsgIsTimer0(pMsg))
                continue;
            else
            {
                if(keyflag == true) break;
                if(keyflag == 2) {
                    if (VM_CHAR_FUN == pMsg.type)
                        break;
                    if (pMsg.type == VM_TOUCH && pMsg.param == VT_TOUCH_UP)
                        break;
                }
            }
            pMsg.param = 0;
        }
    }
    if(tInt)
    {
        SysTimer1Close();
        SysTimer1Open(tInt);
    }
    SysScrollingTimerOpen(tprev);
    return (U8)pMsg.param;
}

FAR void GamSetResourcePath(const U8* datPath, const U8*fontDir)
{
    datFilePath = (U8*)gam_strdup(datPath);
    g_fontDir = (U8*)gam_strdup(fontDir ? (const char*)fontDir : (const char*)"/font");
}

FAR void GamSetAltLibPath(const U8* datPath)
{
    altDatFilePath = (U8*)gam_strdup((char*)datPath);
}

FAR void GamClearLastMsg(void)
{
    lastMsg.type = 0;
}

FAR void GamGetLastMsg(GMType *pMsg)
{
    *pMsg = lastMsg;
}

static unsigned int seed = 0;

int gam_rand(void)
{
#ifdef _WIN32
    return rand();
#else
    return rand_r(&seed);
#endif
}

void gam_srand(unsigned int seed_)
{
#ifdef _WIN32
    return srand(seed_);
#else
    seed = seed_;
#endif
}

int gam_seed(void)
{
    return seed;
}

#ifdef MEM_DEBUG
typedef unsigned char protect_buf[128];

typedef struct gchead {
    struct gchead* prev;
    struct gchead* next;
    char loc[64];
    int line;
    size_t size;
    protect_buf pbuf;
} gchead;

typedef struct {
    protect_buf pbuf;
} gctail;

struct {
    gchead head;
    int alloc_count;
    int dealloc_count;
} gcinfo = {0};

static void gc_add_node(gchead*node) {
    if (!gcinfo.head.next) {
        gcinfo.head.next = &gcinfo.head;
        gcinfo.head.prev = &gcinfo.head;
    }
    node->prev = &gcinfo.head;
    node->next = gcinfo.head.next;

    node->prev->next = node;
    node->next->prev = node;
    gcinfo.alloc_count++;
}

static void gc_rm_node(gchead*node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    gcinfo.dealloc_count++;
}

static void protect_buf_init(protect_buf buf) {
    memset(buf, 0xcc, sizeof(protect_buf));
}

static int check_protect_buf(protect_buf buf) {
    for (int i = 0; i < sizeof(protect_buf); i++) {
        if (buf[i] != 0xcc) {
            return 1;
        }
    }
    return 0;
}

static void check_node(gchead*head) {
    gctail* tail = (gctail*)(((char*)(head+1)) + head->size);
    if (check_protect_buf(head->pbuf) || check_protect_buf(tail->pbuf)) {
        head->loc[sizeof(head->loc)-1] = 0;
        printf("Heap memory overflow detected: alloced at %s:%d size:%zu\n", head->loc, head->line, head->size);
        abort();
    }
}

char* _gam_strdup(const char* s, const char*loc, int line)
{
    size_t l = strlen(s);
    char* p = _gam_malloc(l+1, loc, line);
    memcpy(p, s, l);
    p[l] = 0;
    return p;
}

void* _gam_malloc(size_t sz, const char*loc, int line)
{
    gchead* head = malloc(sizeof(gchead) + sz + sizeof(protect_buf));
    gctail* tail = (gctail*)(((char*)(head+1))+sz);
    head->size = sz;
    head->line = line;
    strncpy(head->loc, loc, sizeof(head->loc));
    protect_buf_init(head->pbuf);
    protect_buf_init(tail->pbuf);
    gc_add_node(head);
    return &head[1];
}

void* _gam_realloc(void*p, size_t sz, const char*loc, int line)
{
    gchead* head = ((gchead*)p) - 1;
    void*newp = _gam_malloc(sz, loc, line);
    memcpy(newp, p, min(sz, head->size));
    gam_free(p);
    return newp;
}

void gam_free(void*p)
{
    gchead* head = ((gchead*)p) - 1;
    check_node(head);
    gc_rm_node(head);
    free(head);
}

void gam_print_gc()
{
    printf("================ Allocation summary ================ \n");
    for (gchead* head = gcinfo.head.next;head != &gcinfo.head; head=head->next)
    {
        printf("%s:%d\n", head->loc, head->line);
    }
    printf("alloced:%d\n", gcinfo.alloc_count);
    printf("dealloced:%d\n", gcinfo.dealloc_count);
    printf("remain:%d\n", gcinfo.alloc_count-gcinfo.dealloc_count);
}

void gam_gc_check_all()
{
    for (gchead* head = gcinfo.head.next;head != &gcinfo.head; head=head->next)
    {
        check_node(head);
    }
    printf("All check OK\n");
}

#endif

