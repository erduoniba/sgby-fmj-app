/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	varma.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		游戏变量定义文件
 **
 *移植性声明:
 *	1、符合标准：《游戏设计标准V1.0》
 *	2、兼容模式：本程序和界面无关，无兼容模式。
 *	3、当引擎的编译环境发生变化时，可修改其中的变量定义
 *修改历史：
 *	版本    日期     作者     改动内容和原因
 *   ----------------------------------------------------
 *	1.0    2005.5.16  高国军     基本的功能完成
 ***********************************************************************/
#define	_VAR_MAL_				/* 屏蔽对extern.h头文件的包含 */
#include "baye/stdsys.h"
#include "baye/comm.h"
#include "baye/enghead.h"

/*变量定义*/
/*------------------------------------------*/
U8	g_GamKeySound;		/* 按键声音 */
U16	g_GamKBState;		/* 键盘状态 */
U8	*g_CBnkPtr;		/* 常量页起始地址指针 */
RandEnvType	g_RandEnv;	/* 随机变量 */
gam_FILE	*g_LibFp;	/* 资源库文件指针 */
gam_FILE	*g_FontFp12;	/* 字库文件指针 */
gam_FILE	*g_FontsFp24[4];	/* 字库文件指针 */
gam_FILE	*g_FontsFp24En[2];	/* 字库文件指针 */

/*变量定义*/
/*------------------------------------------*/
/*comm variable define*/
U8	c_ReFlag;		/* 恢复标志 */
U8	c_Sx,c_Sy,c_Ex,c_Ey;	/* 字符串显示区域 */
U16	c_FontBnkS;		/* 汉字库存放的起始页号，由调用者初始化 */

/*变量定义*/
/*------------------------------------------*/
U8	*g_VisScr;	/* 虚拟屏幕指针 */
U8	g_RunErr;		/* 运行错误 */

/*变量定义*/
/*------------------------------------------*/
U8	*g_FightMapData;	/* 战斗地图 */
U8	*g_FightMap;		/* 战斗地图缓存 */
U8	*g_FightPath;		/* 战斗行军计算 */
U8	*g_FgtAtkRng;		/* 攻击范围 */
U16	g_TileId;		/* 当前地图所用tile的ID */
U16	g_EneTmpProv;		/* 战斗模块，敌人粮草临时变量 */
U8	g_MapWid;		/* 战斗地图宽度 */
U8	g_MapHgt;		/* 战斗地图高度 */
U8	g_FoucsX,g_FoucsY;	/* 光标的地图坐标 */
U8	g_MapSX,g_MapSY;	/* 当前地图显示的起始坐标 */
U8	g_PathSX,g_PathSY;	/* 行军范围地图起始坐标 */
U8	g_PUseSX,g_PUseSY;	/* 行军范围使用起始坐标 */
U8	g_BakUpX,g_BakUpY;	/* 将领坐标备份 */
U8	g_CityX,g_CityY;	/* 城市坐标 */
U8	g_FgtOver;		/* 战斗是否结束 */
U8	g_FgtWeather;		/* 战场天气情况 */
U16	g_FgtBoutCnt;		/* 战斗回合计数 */
U16	g_FgtBoutMax = 30;	/* 战斗最大回合数 */
U8	g_MainGenIdx;		/* 主将序号 */
U8	g_LookEnemy;		/* 是否观看敌人移动 */
U8	g_LookMovie;		/* 是否观看战斗动画 */
U8	g_MoveSpeed;		/* 战斗中移动速度 */
FGTJK	g_FgtParam;		/* 战斗模块接口参数 */
JLPOS	g_GenPos[FGTA_MAX];	/* 将领地图位置及基本属性 */
JLATT	g_GenAtt[2];		/* 攻击状态下的两个将领属性 */

U8  g_AutoUpdateMapXY;

/*变量定义*/
/*------------------------------------------*/
PersonID g_PlayerKing;		/*玩家君主*/
U16 g_YearDate;			/*当前日期*/
U8 g_MonthDate;			/*当前日期*/
U8 g_PIdx;			/*历史时期*/
PersonType g_Persons[PERSON_MAX];		/*存放人才属性指针*/
CityType g_Cities[256];	/*存放城市属性指针*/
PersonID g_PersonsQueue[PERSON_MAX];	/*人才队列*/
ToolID g_GoodsQueue[GOODS_MAX];	/*道具队列*/
OrderQueueType *g_OrderHead;	/*命令队列头指针*/
OrderQueueType *g_OrderEnd;	/*命令队列末指针*/
CitySetType g_CityPos;		/*当前城市地图显示位置结构*/
U8 g_FromSave = 0;

/*变量定义*/
/*------------------------------------------*/
U8 citymap[SHOWMAP_HS_MAX][SHOWMAP_WS_MAX];	/*当前显示城市地图*/
PersonID cavpdb;
U8 cavps;			/*战争俘虏临时变量*/
U8 g_MenuTouchComfirm = 0;		/* 恢复标志 */



//-------------------------------
static U32 pq_len = 0;

void GamSetPersonCount(U32 count)
{
    pq_len = count;
}

U32 GamGetPersonCount(void)
{
    return pq_len;
}
