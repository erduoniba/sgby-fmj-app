#ifndef	_CONS_DEF_H
#define	_CONS_DEF_H
#include <inc/dictsys.h>

/*======================================
  定义游戏系统的常量
======================================*/

/*------------------------
  游戏系统参数
------------------------*/
#define	GAM_DEBUG_MODE		0		/* 游戏调试模式 */
#define	GAM_RELESE_MODE		1		/* 游戏释放模式 */
#define	GAM_VER			GAM_RELESE_MODE

/*------------------------
  引擎性能相关定义
------------------------*/
#define	TIMER_INT		50		/* 定时中断 */
#define	TIMER_DLY		1
#define	SHOW_DLYBASE		10

/*------------------------
  界面相关定义
------------------------*/
extern U16 g_screenWidth;
extern U16 g_screenHeight;
extern U16 g_scale;

#define	SCR_WID			g_screenWidth               /* 屏幕宽度 */
#define	SCR_HGT			g_screenHeight              /* 屏幕高度 */
#define	WK_SX			0                           /* 游戏工作区坐标 */
#define	WK_SY			0                           /* 游戏工作区坐标 */
#define	WK_EX			(SCR_WID - 1)               /* 游戏工作区坐标 */
#define	WK_EY			(SCR_HGT - 1)               /* 游戏工作区坐标 */
#define	SCR_LINE		((SCR_WID+7)/8)             /* 屏幕每行字节数 */
#define	LINE_BYTES		SCR_LINE                    /* 屏幕每行字节个数 */
#define	ASC_WID			6		/* Ascii字模宽度 */
#define	ASC_HGT			12		/* Ascii字模高度 */
#define	HZ_WID			12		/* HZ字模宽度 */
#define	HZ_HGT			12		/* HZ字模高度 */
#define	AX_SCALE		g_scale		// 坐标缩放

#define MAX_SCR_BUF_LEN (256*256*AX_SCALE*AX_SCALE)

/* 颜色定义 */
#define	COLOR_WHITE		0		/* 白色 */
#define	COLOR_BLACK		1		/* 黑色 */
#define	COLOR_BPP		1		/* Bits Per Pixel*/
#define	COLOR_DOBPP		8		/* Derivative Of Byte Per Pixel */

/*------------------------
  固定资源ID定义
------------------------*/
#define	IFACE_STRID		1		/* 引擎界面字符串资源id */
#define	IFACE_CONID		2		/* 引擎界面常量资源id */
#define	MAIN_SPE		3		/* 游戏开头动画 */
#define	FIGHT_TILE		4		/* 战斗地图tile */
#define	BING_PIC		5		/* 兵种图片 */
#define	MAKER_SPE		6		/* 开发组群 */
#define	STEP_PIC		7		/* 脚步图片 */
#define	STATE_PIC		8		/* 状态显示图片 */
#define	WEATHER_PIC		9		/* 天气图片 */
#define	SKL_RESID		10		/* 技能资源 */
#define	SKL_NAMID		11		/* 技能名字 */
#define	SKL_EXPID		12		/* 技能说明 */
#define	SKL_RNGID		13		/* 技能施展范围 */
#define	SPE_SKLID		14		/* 将领专长技能 */
#define	NUM_PICID		15		/* 数字图片ID */
#define	SPE_BACKPIC		16		/* 特效背景图片 */


#define	QIBING_SPE		19		/* 骑兵攻击特效 */
#define	BUBING_SPE		20		/* 步兵攻击特效 */
#define	JIANBING_SPE		21		/* 弓兵攻击特效 */
#define	SHUIJUN_SPE		22		/* 水军攻击特效 */
#define	JIBING_SPE		23		/* 极兵攻击特效 */
#define	XUANBING_SPE		24		/* 玄兵攻击特效 */
#define	SHUISHANG_SPE		25		/* 水面攻击特效 */
#define	MSGBOX_PIC		26		/* 消息框背景图片 */
#define	STACHG_SPE		27		/* 升级了/死亡了-特效 */
#define	WEATHER_PIC1		28		/* 天气图片-晴 */
#define	WEATHER_PIC2		29		/* 天气图片-阴 */
#define	WEATHER_PIC3		30		/* 天气图片-风 */
#define	WEATHER_PIC4		31		/* 天气图片-雨 */
#define	WEATHER_PIC5		32		/* 天气图片-冰 */
#define	DAYS_PIC		33		/* 天数-背景图片 */
#define	SPESTA_PIC		34		/* 特殊状态-禁咒、定身图片 */
#define	FIRE_SPE		35		/* 火攻烈火火箭 */
#define	WATER_SPE		36		/* 水淹海啸 */
#define	WOOD_SPE		37		/* 滚木|礌石 */
#define	BUMP_SPE		38		/* 撞击 */
#define	FENG_SPE		39		/* 封|定 */
#define	LIUYAN_SPE		40		/* 流言 */
#define	YUAN_SPE		41		/* 援兵|援军 */
#define	ZHEN_SPE		42		/* 石阵 */
#define	XJING_SPE		43		/* 陷阱 */
#define	MAIN_PIC		44		/* 主菜单背景图片 */
#define	YEAR_PIC		45		/* 历史时期背景图片 */
#define	SAVE_PIC		46		/* 存档管理界面背景图 */
#define	CITY_PIC		47		/* 城市缩略图 */
#define	GEN_HEADPIC1		(48-1)		/* 将领头像图片-董卓弄权 */
#define	GEN_HEADPIC2		(49-1)		/* 将领头像图片-曹操崛起 */
#define	GEN_HEADPIC3		(50-1)		/* 将领头像图片-赤壁之战 */
#define	GEN_HEADPIC4		(51-1)		/* 将领头像图片-三足鼎立 */

/*------------------------
  固定资源ID定义
------------------------*/
#define CITYMAP_TILE		54		/* 城市地图tile */
#define CITY_ICON		55		/* 城市图标 */
#define CITY_POS_ICON		56		/* 城市选择光标 */
#define CITY_RESID		57		/* 城市资源 */
#define CITY_NAME		58		/* 城市名称 */
#define CITY_LINKR		59		/* 右向城市连接 */
#define CITY_LINKL		60		/* 左向城市连接 */
#define GENERAL_RESID		61		/* 武将资源 */
#define GENERAL_NAME		62		/* 武将名称一 */
#define GENERAL_CON		63		/* 武将出现条件 */
#define STRING_CONST		64		/* 字符串常量 */
#define GENERAL_QUEUE		65		/* 武将队列 */
#define GOODS_RESID		66		/* 道具资源 */
#define GOODS_CON		67		/* 道具出现条件 */
#define GOODS_QUEUE		68		/* 道具队列 */
#define MAPFACE_ICON		69		/* 显示界面武将及城池图标 */
#define GENERAL_NAME2		70		/* 武将名称二 */
#define GENERAL_NAME3		71		/* 武将名称三 */
#define GENERAL_NAME4		72		/* 武将名称四 */
#define GOODS_NAME		73		/* 道具名称 */
#define GOODS_INF		74		/* 道具说明 */
#define TACTIC_ICON		75		/* 战略图 */
#define	FIGHT_NOTE_ICON		76		/* 兵戎提示图 */

#define	ENGINE_SCRIPT		77		/* 脚本 */

/* 菜单ICON */
#define	MAIN_ICON1		100		/* 主菜单icon1 */
#define	MAIN_ICON2		101		/* 主菜单icon2 */
#define	MAIN_ICON3		102		/* 主菜单icon3 */
#define	MAIN_ICON4		103		/* 主菜单icon4 */
#define	YEAR_ICON1		104		/* 年限icon1 */
#define	YEAR_ICON2		105		/* 年限icon2 */
#define	YEAR_ICON3		106		/* 年限icon3 */
#define	YEAR_ICON4		107		/* 年限icon4 */

/* 地图开始序号 */
#define	FIGHT_MAP		110		/* 战斗测试地图 */


#endif	/* _CONS_DEF_H */
