#pragma once
#include "keytable.h"
#include "dictsys.h"
#include "debug.h"

#define MEM_OK					0
#define MEM_MCB_ERROR           1

#define MCB_BLANK               'b'
#define MCB_USE                 'u'

#define MCB_END                 'e'
#define MCB_NORMAL              'n'

#define MCB_LENGTH              4				/*  内存控制块的大小 */

#define	MIN_BLK_BYTES			4				/*  分配的最小字节 */
#define	MIN_BLK_MASK			0x03
#define	MIN_BLK_NMASK			0xfffc


typedef struct	tagMCB
{
    UINT8      use_flag;               /*	使用标志 */
    UINT8      end_flag;               /*  结束标志 */
    UINT16 	len;					/*  分配的空间长度，不含MCB长度 */
}MCB;

void fillmem(UINT8*, UINT16, UINT8);

void SysPrintString(UINT8 x, UINT8 y, const UINT8* str);
void SysLine(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2);
void SysRect(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2);
void SysFillRect(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2);
void SysSaveScreen(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* BuffPoint);
void SysRestoreScreen(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* BuffPoint);
void SysPicture(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* BuffPoint, UINT8 flag);
void SysLcdPartClear(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2);
void SysAscii(UINT8 x, UINT8 y, UINT8 asc);
void SysLcdReverse(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2);
void SysPictureDummy(UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8* pic, UINT8* Screen, UINT8 flag);
UINT8 SysGetSecond();
void SysTimer1Open(UINT8 times);
void SysTimer1Close();
void SysIconAllClear(void);						/*清除所有icon显示,系统的除外 */
void DataBankSwitch(UINT8 logicStartBank, UINT8 bankNumber, UINT16 physicalStartBank);
void GetDataBankNumber(UINT8 logicStartBank, UINT16* physicalBankNumber);
void SysSetKeySound(UINT8 keySoundFlag);
UINT8   SysGetKeySound();
UINT8	 SysGetKey();
void SysPlayMelody(UINT8 melodyNum);
void SysStopMelody();
void SysMemInit(UINT16 start, UINT16 len);
char* SysMemAllocate(UINT16 len);
UINT8 SysMemFree(char* p);
UINT16 SysRand(PtrRandEnv pRandEnv);
void SysSrand(PtrRandEnv pRandEnv, UINT16 seed, UINT16 randMax);
void SysMemcpy(UINT8* dest, const UINT8* src, UINT16 len);
UINT8 SysMemcmp(UINT8* dest, const UINT8* src, UINT16 len);
void GuiSetInputFilter(UINT8 filter); /* 键盘屏蔽属性 */
void GuiSetKbdType(UINT8 type); /* 键盘类型 */
UINT8 GuiGetMsg(PtrMsg pMsg);
UINT8 GuiTranslateMsg(PtrMsg pMsg); /* 转换扫描码为字符，或从输入法得到汉字等 */
UINT8 GuiInit(void);
UINT16 GuiGetKbdState(); /* 取键盘状态 */
void GuiSetKbdState(UINT16 state); /* 恢复键盘状态 */
UINT8 GuiMsgBox(UINT8* strMsg, UINT16 nTimeout);

#define NoOpen			0x00
#define	ReadOnly		0x01
#define	ReadAndWrite	0x02
#define FromTop		0x01
#define	FromCurrent	0x02
#define	FromEnd		0x03
typedef struct _FileInfo
{
    UINT16 filename;
    UINT8 information[10];
    UINT8 filetype;
    HANDLE hFile;
} FileInfo;
// filename值为0代表没有这个文件。filehandle值与filename相同
// information[10]为字符串的文件名称
#define FILE_NUM 16
extern FileInfo fileinfos[FILE_NUM];

UINT8 FileCreat(UINT8 filetype, UINT32 filelength, UINT8* information, UINT16* filename, UINT8* filehandle);
UINT8 FileOpen(UINT16 filename, UINT8 filetype, UINT8 openmode, UINT8* filehandle, UINT32* filelength);
UINT8 FileDel(UINT8 filehandle);
UINT8 FileWrite(UINT8 filehandle, UINT8 datalength, UINT8* bufadd);
UINT8 FileClose(UINT8 filehandle);
UINT8 FileRead(UINT8 filehandle, UINT8 datalength, UINT8* bufadd);
UINT8 FileSeek(UINT8 filehandle, UINT32 fileoffset, UINT8 origin);
void FlashInit();
UINT8 FileNum(UINT8 filetype, UINT16* filenum);
// information[10]
UINT8 FileSearch(UINT8 filetype, UINT16 fileorder, UINT16* filename, UINT8* information);