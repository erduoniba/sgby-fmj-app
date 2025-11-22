#ifndef	_COMM_H
#define	_COMM_H

#include "inc/dictsys.h"

#ifndef BOOL
#define BOOL I8
#endif
/*======================================
  引擎程序相对于环境独立性
======================================*/

/*------------------------
  字库常量定义
  ------------------------*/
#define	GAM_FONT_BNK_NUM	40			/* 游戏字库占用4K的页面个数 */
#define	GAM_FONT_ASC_QU		0xA4			/* Ascii字模在游戏字库中存放的区 */

/*------------------------
  外部内存常量
  ------------------------*/
#define	EXTMEM_BNKNUM		4			/* 外部内存使用空间为内存第4页面(地址 16K~20K) */

#define	VS_PTR _VS_PTR
#define	SHARE_MEM _SHARE_MEM
#define FIGHTERS_IDX _FIGHTERS_IDX
#define FIGHTERS _FIGHTERS
#define	ORDERQUEUE _ORDERQUEUE

extern U8 *_VS_PTR;		/* 虚拟屏幕缓冲 */
extern U8 *_SHARE_MEM;     /* 共享临时内存 */
extern U8 *_FIGHTERS_IDX;      /* 出征武将队列索引(30个字节) */
extern U8 *_FIGHTERS;      /* 出征武将队列(30*10=300个字节) */
extern U8 *_ORDERQUEUE;        /* 命令队列(12*100=1200个字节) */

/*------------------------
  消息结构定义
  ------------------------*/
typedef	MsgType GMType;

/*------------------------
  消息类型定义
  ------------------------*/
#define		VM_DUMMY		0x00		/* 无效消息 */
#define		VM_KEY			0x01		/* 按键值，只有行列信息 */
#define		VM_CHAR_ASC		0x02		/* 输入 ASCII码 */
#define		VM_CHAR_HZ		0x03		/* 输入汉字 */
#define		VM_CHAR_MATH		0x04		/* 数学功能键 */
#define		VM_CHAR_FUN		0x05		/* 输入功能键 */
#define		VM_TIMER		0x06		/* 定时到 */
#define		VM_COM			0x07		/* 串口接收到数据 */
#define		VM_POWER		0x08		/* 电源 */
#define		VM_ALERT		0x09		/* 闹钟到 ，行程到 */
#define		VM_COMMAND		0x0a		/* 界面相关 */
#define		VM_TOUCH		0x10		/* 触控事件 */

/*------------------------
  转换后的扩展键  ---  功能键
  说明：功能键对应消息类型WM_CHAR_FUN,取消息值(param)的低字节
  ------------------------*/
#define		VK_PGUP				0x20
#define		VK_PGDN				0x21
#define		VK_UP				0x22
#define		VK_DOWN				0x23
#define		VK_LEFT				0x24
#define		VK_RIGHT			0x25
#define		VK_HELP				0x26
#define		VK_ENTER			0x27
#define		VK_EXIT				0x28

#define		VK_INSERT			0x30
#define		VK_DEL				0x31
#define		VK_MODIFY			0x32
#define		VK_SEARCH			0x33
#define		VK_SHIFT			0x34			/* shift */
#define		VK_SYM				0x35			/*  符号 */

/* 触控消息 */
#define		VT_TOUCH_DOWN			0x01
#define		VT_TOUCH_UP             0x02
#define		VT_TOUCH_MOVE			0x03
#define		VT_TOUCH_CANCEL			0x04

/*------------------------
  其它结构体定义
  ------------------------*/
typedef	struct {
	PT	sx;			/* 矩形左 */
	PT	sy;			/* 矩形上 */
	PT	ex;			/* 矩形右 */
	PT	ey;			/* 矩形下 */
}RECT;


/*------------------------
  函数
  ------------------------*/
static inline U8 GamMsgIsKey(GMType msg) {
  return (msg.type > 0 && msg.type <= VM_CHAR_FUN);
}
static inline U8 GamMsgIsKeyOrTouchUp(GMType msg) {
  return GamMsgIsKey(msg) || (msg.type == VM_TOUCH && msg.param == VT_TOUCH_UP);
}
static inline U8 GamMsgIsTimer0(GMType msg) {
  return (msg.type == VM_TIMER && msg.param == 0);
}
static inline U8 GamMsgIsTimer1(GMType msg) {
  return (msg.type == VM_TIMER && msg.param == 1);
}
FAR	U8      GamConInit(void);						/* 游戏系统环境初始化，根据不同环境调用不同的初始化函数 */
FAR	void	GamConRst(void);						/* 游戏系统环境恢复，根据不同环境调用不同的恢复函数 */
FAR	void	GamGetMsg(GMType *pMsg);					/* 封装系统的输入，并将其转换成游戏的消息机制 */
FAR void    GamGetLastMsg(GMType *pMsg);
FAR void    GamClearLastMsg(void);
FAR	U8      GamDelay(U16 dly, BOOL keyflag);					/* 1%秒延时 */
FAR	void	GamMsgBox(const U8 *buf,U8 delay);					/* 信息框 */
FAR	void	GamShowFrame(U8 *vscr);						/* 将虚拟屏幕show到屏幕上 */
FAR	void	GamPicShowS(PT x,PT y,PT wid,PT hgt,U8 *pic);			/* 显示图片到屏幕 */
FAR	void	GamPicShowV(PT x,PT y,PT wid,PT hgt,U8 *pic,U8 *vscr);		/* 显示图片到虚拟屏幕 */
FAR	void	GamMPicShowS(PT x,PT y,PT wid,PT hgt,U8 *pic);			/* 显示mask图片到屏幕 */
FAR	void	GamMPicShowV(PT x,PT y,PT wid,PT hgt,U8 *pic,U8 *vscr);		/* 显示mask图片到虚拟屏幕 */
FAR	U32     GamStrShowS(PT x,PT y,const U8 *str);					/* 显示12汉字到屏幕 */
FAR	void	GamAsciiS(PT x,PT y,U8 asc);					/* 显示6x12ASCII字符到屏幕 */
FAR	U32     GamStrShowV(PT x,PT y,U8 *str,U8 *vscr);			/* 显示12汉字到虚拟屏幕 */
FAR void    GamClearScreenV(PT l, PT t, PT r, PT b, U8*scr);

FAR void GamSetResourcePath(const U8* datPath, const U8*fontDir);
FAR void GamSetAltLibPath(const U8* datPath);
FAR void GamSetDataDir(const U8*dataDir);
FAR U8 GamSetFont(U16 font);
FAR U8 GamSetFontEn(U16 font);
FAR void GamClearFontCache();

#ifndef min
#define min(a, b) (a > b) ? b : a
#endif

#ifndef min
#define min(a, b) (a > b) ? b : a
#endif

#endif	/* _COMM_H */
