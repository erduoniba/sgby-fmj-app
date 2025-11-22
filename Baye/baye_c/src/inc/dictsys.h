/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：dictsys.h
*	文件标识：A600系统
*	摘    要：A600系统数据类型和函数接口定义
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.7.26   任新村      完成基本内容
*******************************************************************************************/

#ifndef		DICTSYS_H
#define		DICTSYS_H

#ifndef		KEYTABLE_H
#include	"keytable.h"
#endif

//#define		COMPILER_2500AD
#define		GAM_PHY_SET				4980				/* 当前系统的机型 */

#define		VER_USR_DEBUG			0					/*  应用开发人员用 */
#define		VER_RXC_DEBUG			1
#define		VER_RXC_DOWN			2
#define		VER_RXC_RELEASE			3					/* 最终版本释放用 */

#define		DICT_PHY_VER			VER_RXC_DEBUG		/* 物理环境 */
#define		DICT_SYS_VER			0x0100				/* 版本号 */

/******************************************************************************************
*						字符内码类型定义
*******************************************************************************************/
#define		CHAR_SET_GB2312			0x01
#define		CHAR_SET_BIG5			0x02

#define		DICT_CHAR_SET			CHAR_SET_GB2312
/******************************************************************************************
*						系统数据类型定义
*******************************************************************************************/

typedef	signed char                    I8;
typedef signed short __attribute__((aligned(1))) I16;
typedef signed int __attribute__((aligned(1))) I32;

typedef		unsigned	char		U8;
typedef unsigned short __attribute__((aligned(1))) U16;
typedef unsigned int __attribute__((aligned(1))) U32;

struct detector{
    U8 a;
    U16 b;
    U32 c;
};
typedef int compiler_does_not_support_aligned1[sizeof(struct detector) == 7?0:-1];

typedef U8 SBUF[1024];

typedef U16 PersonID;
typedef U16 ToolID;
typedef U16 SkillID;
typedef I16 PT;

#define PID0 0
#define PID(n) (n)

#define TID(n) (n)
#define SID(n) (n)

//static inline ToolID toTid(U16 tid) { return (ToolID){ tid }; }
//static inline SkillID toSid(U16 tid) { return (SkillID){ tid }; }
//static inline PersonID toPid(U16 pid) { return (PersonID){ pid }; }

#define		TRUE		1
#define		true		1
#define		FALSE		0
#define		false		0
//#define		NULL		0

//#define		FAR			banked
#define		FAR

/******************************************************************************************
*						系统硬件信息定义
*******************************************************************************************/
#define		SCR_WIDTH	159				/* 屏幕宽 */
#define		SCR_HEIGHT	96				/* 屏幕高 */

#define		SCR_ROWS	6				/* 屏幕字符行数( 按8*16点阵算) */
#define		SCR_COLS	10				/* 列数 */

#define		MEM_HEAP_START		0x2c00	/* 堆首地址 */
#define		MEM_HEAP_SIZE		0x1400	/* 堆大小 */

#if		(DICT_PHY_VER == VER_USR_DEBUG)
#define		SYS_PHY_START_ADDR		0xf80000		/* 目前 512K 开发板的程序空间起始地址 */
#elif		(DICT_PHY_VER == VER_RXC_DEBUG)
#define		SYS_PHY_START_ADDR		0xf00000		/* 目前 1M 开发板的程序空间起始地址 */
#elif		(DICT_PHY_VER == VER_RXC_DOWN)
#define		SYS_PHY_START_ADDR		0xf80000		/* 目前 512 开发板的程序空间起始地址 */
#elif		(DICT_PHY_VER == VER_RXC_RELEASE)
#define		SYS_PHY_START_ADDR		0xed0000  /*0xe00000*/		/* 目前 2M 开发板的程序空间起始地址 */
#endif

#define		SYS_DATA_LOG_BANK			0x9				/* 数据 bank 的逻辑 bank 号*/
#define		SYS_DATA_BANK_NUM			0x4				/* 数据 bank 的个数 */
#define		SYS_DATA_BANK_ADDR			0x9000			/* 数据 bank 的的地址*/

/******************************************************************************************
*						消息定义
*******************************************************************************************/

typedef union MsgParam {
    U32 u32;
    I32 i32;
    struct {
        U16 p0;
        U16 p1;
    } u16;
    struct {
        I16 p0;
        I16 p1;
    } i16;
} MsgParam;

typedef		struct	tagMsgType
{
	U8		type;
	U16		param;
	MsgParam param2;
}MsgType,*PtrMsg;

#define		WM_DUMMY		0x00		/* 无效消息 */
#define		WM_KEY			0x01		/* 按键值，只有行列信息 */

#define		WM_CHAR_ASC		0x02		/* 输入 ASCII码 */
#define		WM_CHAR_HZ		0x03		/* 输入汉字 */
#define		WM_CHAR_MATH	0x04			/* 数学功能键 */
#define		WM_CHAR_FUN		0x05		/* 输入功能键 */

#define		WM_TIMER		0x06		/* 定时到 */
#define		WM_COM			0x07		/* 串口接收到数据 */

#define		WM_POWER		0x08		/* 电源 */

#define		WM_ALERT		0x09		/* 闹钟到 ，行程到 */
#define		WM_COMMAND		0x0a		/* 界面相关 */	
/*-----------------------------------------------------------------------------------------
*			WM_COMMAND  类消息 对应消息值定义				
*-----------------------------------------------------------------------------------------*/
#define		CMD_CHN_INPUT_OPEN			1			/* 输入法打开 */
#define		CMD_CHN_INPUT_CLOSE			2			/* 输入法关闭 */

#define		CMD_RETURN_HOME				3			/* 返回主主界面 */
#define		CMD_SCR_NESTED				4			/* 界面重叠 */
/*-----------------------------------------------------------------------------------------
*			WM_COMMAND  类消息 对应消息值定义				
*-----------------------------------------------------------------------------------------*/
#define		PWR_OFF						1
/*-----------------------------------------------------------------------------------------
*			时间类结构及函数
*-----------------------------------------------------------------------------------------*/
typedef	struct	tagDate
{
	U8	day;			
	U8	month;
	U8	year;
	U8	wday;				/* 星期几 */
}TypeDate,*PtrDate;

typedef	struct	tagTime
{
	U8	second;
	U8	minute;
	U8	hour;
}TypeTime,*PtrTime;

#ifndef		TIME_C_LOCAL
FAR	U8	SysGetDate(PtrDate);
FAR	U8	SysGetTime(PtrTime);
FAR	U8	SysSetTime(U8*);
#endif

/******************************************************************************************
*						Gui结构类型定义
*******************************************************************************************/
#define			MENU_STYLE_ICON					0x01			/*  前面带图标 */
#define			MENU_STYLE_ICON_NMASK			0xfe
#define			MENU_STYLE_LOOP					0x02			/*  循环 */

typedef		struct	tagGuiMenuItemType
{
	U8		strLen;				/*  该项菜单字符串长度 */
	U8		*str;				/*  菜单项字符串 */		
	U8		*bmpData0;			/*  普通菜单项的图标 */
	U8		*bmpData1;			/*  选中菜单项的图标 */
}GuiMenuItemType, *PtrGuiMenuItemType;

typedef		struct	tagGuiMenu
{
	U8					itemNum;			/*	菜单项数 */
	U8					colNum;				/*  菜单列数 */
	U8					*colPos;			/*	菜单各列的相对位置 */
	U8					style;				/*  菜单风格 */
	PtrGuiMenuItemType	*menuItems ;		/*  菜单项 */
}GuiMenuType,*PtrGuiMenu;

typedef		struct		tagMenuEnv
{
	U8		x0;					/* 指定的屏幕显示区域坐标 */
	U8		x1;					
	U8		y0;
	U8		y1;
	U8		startDispItem;		/* 本屏显示的起始项 */

/********************************* 以上几项由用户填写，下面几项保留**********/
	U8		maxDispItem;		/* 指定的区域内最大的显示项数 */
	U8		dispItemNum;		/* 本屏幕实际显示的项数 */
	U8		nFocus;				/* 在按 Exit 时，菜单的焦点 */
}GuiMenuEnvType,*PtrGuiMenuEnv;

typedef		struct	tagRect
{
	U8		left;					/* 左 */
	U8		top;					/* 上 */
	U8		height;					/* 高 */ 
	U8		width;					/* 宽 */
}RectType,*PtrRect;

typedef	struct	tagRandEnv
{
	U32 next 	;
 	U16 randMax	;
}	RandEnvType, *PtrRandEnv;

typedef	struct	tagQryBoxBmp
{
	U8		left;					/* 左 */
	U8		top;					/* 上 */
	U8		height;					/* 高 */ 
	U8		width;					/* 宽 */
	U8		*bmpData;				/* 位图数据 */
} QryBoxBmpType , *PtrQryBoxBmp ;

/******************************************************************************************
*						Gui  部分常量定义
*******************************************************************************************/
/******************************************************************************************/
#define		INPUT_NUM_ENABLE			0x01			/* 数字 */
#define		INPUT_ENG_ENABLE			0x02			/* 英文	*/
#define		INPUT_SYM_ENABLE			0x04			/* 符号 */
#define		INPUT_CHN_ENABLE			0x08			/* 汉字 */
#define		INPUT_ALL_ENABLE			0x0f			/* 全部 */

#define		SYS_NUM_KBD					0x01			/* 数字键盘 */
#define		SYS_ENG_KBD					0x02			/* 英文键盘 */
#define		SYS_CHN_KBD					0x04			/* 中文键盘 */
#define		SYS_SYM_KBD					0x08			/* 符号键盘 */

#define		SYS_CAPS_STATE				0x10			/* CAPS 状态 */
#define		SYS_CAPS_NSTATE				0xEF			/* 屏蔽 状态 */
#define		SYS_SHIFT_STATE				0x20			/* SHIFT 状态*/
#define		SYS_SHIFT_NSTATE			0xDF			/* 屏蔽 状态 */

/*-----------------------------------------------------------------------------------------
*							单选框
*-----------------------------------------------------------------------------------------*/
#define		SSB_UP_MARGIN					0x02	/* 单选框的上边线和文字的距离 */
#define		SSB_LEFT_MARGIN					0x04
#define		SSB_RIGHT_MARGIN				0x04
#define		SSB_BOTTOM_MARGIN				0x02
#define		SSB_SHADOW_WIDTH				0x02	/* 单选框阴影的宽度 */
/*-----------------------------------------------------------------------------------------
*							菜单
*-----------------------------------------------------------------------------------------*/

#define		SYS_ASC_WIDTH		8
#define		MENU_FONT_HEIGHT	16
#define		MENU_FONT_WIDTH		16
#define		MENU_ICON_WIDTH		16

/*-----------------------------------------------------------------------------------------
*							下载程序
*-----------------------------------------------------------------------------------------*/
typedef		struct		tagAppSwitchInfo
{
	U16		currentAppID;							/* 当前应用程序 ID */
	U16		heirAppID;								/* 下次要切换的ap 的ID */
	U32		downAppPhyAddr;							/* 如果要启动下载程序，其物理地址 */
	U8		currentAppAttr;							/* 属性 */
	U8		heirAppAttr;
}AppSwitchInfoType,*PtrAppSwitchInfo;
typedef	FAR		void	(*FarDownAppPtrType)(U16 para1 , U16 para2 );

#define		DWN_APP_START_BANK_ID		0xe0

/*-----------------------------------------------------------------------------------------
*							错误返回值
*-----------------------------------------------------------------------------------------*/
#define		GUI_ERR_UER_ABORT				0xff
#define		GUI_ERR_MEM_OVR					0xfe
#define		GUI_ERR_HOT_KEY					0xfd

/******************************************************************************************
*						系统Bios函数
*******************************************************************************************/
/*-----------------------------------------------------------------------------------------
*			屏幕显示函数
*-----------------------------------------------------------------------------------------*/
FAR void SysRectClear(PT x1,PT y1,PT x2,PT y2);
FAR void SysCircleClear(PT x0,PT y0,U8 r);
FAR void SysPartPicture(PT x,PT y,PT x0,PT y0,PT width,PT high,U8* BuffPoint);
FAR void SysLCDClear();

FAR void SysChinese(PT x,PT y,U16 Hz);
FAR void SysAscii(PT x,PT y,U8 asc);
FAR void SysPrintString(PT x,PT y,U8* str);
FAR void SysLine(PT x1,PT y1,PT x2,PT y2);
FAR void SysRect(PT x1,PT y1,PT x2,PT y2);
FAR void SysFillRect(PT x1,PT y1,PT x2,PT y2);
FAR void SysPutPixel(PT x,PT y,U8 data);

FAR void SysCopyScreen(U8*scr);
FAR void SysSaveScreen(void);
FAR void SysRestoreScreen(void);
FAR void SysSelectScreen(U8*scr);
FAR	void SysRestoreScreenEx();
FAR	void SysSaveScreenEx();
FAR void SysPicture(PT sX, PT sY, PT eX, PT eY, U8*pic , U8 flag, U8 scale);
FAR void SysPictureEx(PT sX, PT sY, PT eX, PT eY, U8*pic , U8 flag, U8 scale, U8 compat);
FAR void DecodePic(U8* dst, const U8*pic, PT w, PT h, U8 scale);
FAR void SysCircle(PT x0,PT y0,PT r);
FAR void SysFillCircle(PT x0,PT y0,PT r);

FAR void SysLcdPartClear(PT x1,PT y1,PT x2,PT y2);
FAR void SysLcdReverse(PT x1,PT y1,PT x2,PT y2);

FAR void SysChinese12(PT x,PT y,U16 Hz);
FAR void SysAscii12(PT x,PT y,U8 asc);
FAR void SysPrintString12(PT x,PT y,U8* str);
FAR void SysPictureFill(U8* BuffPoint);
FAR void SysAdjustLCDBuffer(int wid, int height);

/*-----------------------------------------------------------------------------------------
*			光标相关函数
*-----------------------------------------------------------------------------------------*/
FAR	U8 OpenCursorInit(U8 x, U8 y);
FAR void CursorInit(U8 width, U8 high);
FAR U8 OpenCursor(void);
FAR U8 CloseCursor(void);
FAR void MoveCursor(U8 x, U8 y);
FAR	void SysCursorReadParam(U8* ptr);
FAR	void SysCursorWriteParam(U8* ptr);

/*-----------------------------------------------------------------------------------------
*			时间相关函数
*-----------------------------------------------------------------------------------------*/
FAR U8 SysGetSecond(void);
FAR U8 SysGetMinute(void);
FAR U8 SysGetHour(void);
FAR	void SysGetDay(U16 *day);
FAR	void SysSetSecond(U8 sec);
FAR	void SysSetMinute(U8 min);
FAR	void SysSetHour(U8 hour);
FAR	void SysSetDay(U16 day);

FAR void SysTimer1Open(U8 times);
FAR void SysTimer1Close(void);
FAR int SysScrollingTimerOpen(int times);
FAR void SysScrollingTimerClose(void);
FAR U8 SysScrollingTimerOn(void);
FAR	U8 SysGetTimer1Number(void);

/*-----------------------------------------------------------------------------------------
*			Icon  操作函数
*-----------------------------------------------------------------------------------------*/
typedef		struct		tagSysIconEnv
{
	U8		iconSaveBuf[12];
}TypeSysIconEnv,*PtrSysIconEnv;
/*-----------------------------------------------------------------------------------------*/
FAR	void SysIconNumClear(void);	/*一次性清除四个7段数字及3个小数点*/
FAR	void SysIconSetLeftScrollBar(U8 percent);  /*设置Lcd左边的滚动条显示， 输入参数为显示的百分比（0~100）*/
FAR	void SysIconSetRightScrollBar(U8 percent);  /*设置Lcd右边的滚动条显示， 输入参数为显示的百分比（0~100）*/
FAR void SysIconAllSave(PtrSysIconEnv iconEnv);		/* 保存所有 Icon */
FAR void SysIconAllRestore(PtrSysIconEnv iconEnv);	/* 恢复所有 Icon */
FAR	void SysIconAllClear(void);						/*清除所有icon显示,系统的除外 */

FAR void SysIconLeftArrow(U8 data);
FAR void SysIconRightArrow(U8 data);
FAR void SysIconUpArrow(U8 data);
FAR void SysIconDownArrow(U8 data);
FAR void SysIconBattery(U8 data);

FAR void SysIconScrollBar(U8 StartPosition,U8 Number,U8 state);
FAR void SysIconScrollBarDownArrow(U8 data);
FAR void SysIconScrollBarUpArrow(U8 data);
FAR void SysIconNumber1(U8 data);
FAR void SysIconNumber2(U8 data);
FAR void SysIconNumber3(U8 data);
FAR void SysIconNumber4(U8 data);
FAR void SysIconDot1(U8 data);
FAR void SysIconDot2(U8 data);
FAR void SysIconDot3(U8 data);
FAR void SysIconF1(U8 data);
FAR void SysIconF2(U8 data);
FAR void SysIconF3(U8 data);
FAR void SysIconF4(U8 data);
FAR void SysIconF5(U8 data);
FAR void SysIconF6(U8 data);
FAR void SysIconF7(U8 data);
FAR void SysIconF8(U8 data);
FAR void SysIconShift(U8 data);
FAR void SysIconCaps(U8 data);
FAR void SysIconRing(U8 data);
FAR void SysIconRingClock(U8 data);
FAR void SysIconSpeaker(U8 data);
FAR void SysIconBell(U8 data);
FAR void SysIconKey(U8 data);
FAR void SysIconNum(U8 data);
FAR void SysSpeBackSoundPlay(U16 tempvar);
FAR void Speech691_initial();
/*-----------------------------------------------------------------------------------------
*			串口函数
*-----------------------------------------------------------------------------------------*/
#define		BAUD_9600_BPS		0
#define		BAUD_19200_BPS		1
#define		BAUD_38400_BPS		2
#define		BAUD_57600_BPS		3
#define		BAUD_115200_BPS		4

#define		DISABLE_PARITY		0	/*无奇偶校验*/
#define		ODD_PARITY			1	/*奇校验*/
#define		EVEN_PARITY			2	/*偶校验*/
#define		RESERVE_PARITY		3	/*保留校验位*/

#define		UART_MODE			0	/*通信模式为串口通信*/
#define		IRDA_MODE			1	/*通信模式为红外通信*/

FAR	U8 	SysOpenCom(U8 mode,U8 Baud,U8 Parity);
U8 	SysReadCom(U8*str,U8 *size);
U8 	SysWriteCom( U8	len,U8 *buf);
FAR	U8	SysCloseCom(void);

/*-----------------------------------------------------------------------------------------
*			其他 Bios 函数
*-----------------------------------------------------------------------------------------*/
FAR void DirectReadData(U32 StarAddr,U8* BuffPoint,U8 count);
FAR void DirectWriteData(U32 StarAddr,U8* BuffPoint,U8 count);
void DataBankSwitch(U8 logicStartBank,U8 bankNumber,U16 physicalStartBank);
void GetDataBankNumber(U8 logicStartBank,U16* physicalBankNumber);
FAR	void SysClrPA0(void);
FAR	void SysSetPA0(void);
FAR	void SetAutoOffValue(U8	);
FAR void SysSetDownAppStartAddr(U32 startaddr);
FAR void SysGetDownAppStartAddr(U32* startaddr);
FAR void SysSetKeySound(U8 keySoundFlag);
FAR U8   SysGetKeySound(void);
FAR U8	 SysGetKey(void);
FAR void SysSetVolume(U8 volume);
FAR U8   SysGetVolume(void);
FAR	U8	 SetAutoPowerOffTimeOut(U8);
FAR U8 	 GetAutoPowerOffTimeOut();
FAR	void SysLCDVoltage(U8 voltage);		/*voltage: 0 - 63 */
FAR void OpenAlarm(void);
FAR void CloseAlarm(void);
FAR	void SysClearKeyBuffer(void);			/* 清除键盘缓冲区 */
FAR void	SysPlayMelody(U8 melodyNum);
FAR	void	SysStopMelody(void);

/******************************************************************************************
*						杂类函数
*******************************************************************************************/
#ifndef		DICTMEM_C_LOCAL
FAR	void	SysMemInit(U16 start,U16 len) ;
FAR	char*	SysMemAllocate(U16 len);
FAR	U8		SysMemFree(char *p);
#endif

#ifndef		DICTGUI_C_LOCAL
FAR	U16		SysRand(PtrRandEnv pRandEnv);
FAR	void	SysSrand(PtrRandEnv pRandEnv ,  U16 seed , U16  randMax);
#endif

void	SysMemcpy(U8 *dest , U8  *src , U16 len);
U8		SysMemcmp(U8 *dest , U8  *src , U16 len);	/*src 和 dest 相比 返回值分别为0x01 ,0x00 ,0xff*/
/******************************************************************************************
*						Gui函数
*******************************************************************************************/
#ifndef		DICTGUI_C_LOCAL
FAR	void	GuiMsgQueInit(void);
FAR	void	GuiSetInputFilter(U8	filter);		/* 键盘屏蔽属性 */
FAR	void	GuiSetKbdType(U8	type);				/* 键盘类型		*/

FAR	U8      GuiSendMsg(PtrMsg	pMsg);
FAR	U8      GuiPushMsg(PtrMsg pMsg);
FAR	U8		GuiGetMsg(PtrMsg pMsg);
FAR	U8		GuiTranslateMsg(PtrMsg	pMsg);		/* 转换扫描码为字符，或从输入法得到汉字等 */
FAR	U8		GuiInit(void);
FAR	void	GuiKeyEmuInit(void);

FAR	U16		GuiGetKbdState(void);					/* 取键盘状态 */
FAR	void	GuiSetKbdState(U16 state);			/* 恢复键盘状态 */

FAR	U8		GuiStartDownApp(void );
FAR	U8		GuiLoadDownApp(U32		phyAppAddr );
FAR	void	SysPrintNum(U8	x,U8	y,long num, U8 base,U8	sign);
				/* 打印一个数，坐标为(x,y) , base 为进制，sign 为符号  */
FAR	U16		SysGetVer(void) ;
FAR	U8		AppPowerOffHelper();
FAR	U8		GuiDownAppHelp(U8 *helpAddr);
FAR	void	GuiEnterPswCheck();
FAR	void	GuiLeavePswCheck();
#endif

#ifndef		APPINFO_C_LOCAL
#define		ID_GUI_NULL_HELP		0xffff
FAR	void	GuiHelp(U16		id );

FAR	U8		GuiTimeDjsMsg(void);			/* 倒计时 */
FAR	U8		GuiTimeJnrMsg(void);			/* 纪念日 */
FAR	void	GuiTimeDayIntRefresh(void);			/* 天中断刷新 */

FAR	U8		GuiSetKjDisp();				/* 开机显示 */
FAR	void	GuiHwSelfCheck();			
FAR	U8		GuiStudyEveryDayRecite();		/* 每天背单词 */
FAR	void	GuiInitAppOnReset(U8	index);	/* 所有应用程序开机初始化 */
FAR	U8		GuiStudyAbhsReciteAwake();		/* 爱宾好斯 */
#endif

#ifndef		GUIMENU_C_LOCAL
FAR	U8		GuiMenu(U8	sel,PtrGuiMenu pMenu ,PtrGuiMenuEnv	pMenuEnv );
FAR	U8		GuiSingleSelBox(U8	origin,PtrGuiMenu pMenu ,PtrGuiMenuEnv	pSglSelBoxEnv );
FAR	U8		GuiQueryBox(U8	sel , U8	infoType , U8	*infoData );
FAR	U8		GuiMsgBox( U8*  strMsg, U16		nTimeout);
FAR	U8		GuiMsgBoxHelper( U8*  strMsg, U16	nTimeout,U8	bSaveScr );
FAR	void	GuiSetCurHelpId(U16		id);
FAR	U16		GuiGetCurHelpId(void);
#endif

#define		SysCls()		(SysLcdPartClear(0,0,SCR_WIDTH-1,SCR_HEIGHT - 1 ))

/******************************************************************************************
*						调试用函数
*******************************************************************************************/
#ifndef		DICTGUI_C_LOCAL
FAR	void	Pause();						/* 暂停，按键后继续 */			
FAR	void	TRACEL(U32	num);				/* 显示一个U32的数在屏幕左上角 */
FAR	void	TRACELP(U32	num);				/* 显示并暂停 */
FAR	void	TraceNum(U8	x,U8	y,long num, U8 base,U8	sign);
				/* 打印一个数，坐标为(x,y) , base 为进制，sign 为符号  */
FAR	void	TraceNumP(U8	x,U8	y,long num, U8 base,U8	sign);	/* 打印并暂停 */
#endif
/******************************************************************************************
*						编译参数
*******************************************************************************************/

#ifdef	COMPILER_2500AD
	.asm
	.CHIP	6502
	.LINKLIST
	.SYMBOLS
	.DEBUG	ASM
	.LIST	ON
	.optimize	off
	.endasm
#endif	

/*******************************************************************************************/

#endif
