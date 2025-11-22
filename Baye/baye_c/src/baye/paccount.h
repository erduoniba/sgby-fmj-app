#ifndef PACCOUNT_H
#define PACCOUNT_H

/*-------------------------------
  特效资源结构定义
  魔法特效序号200以后为幻身魔法特效
  序号，到239共40个。
  魔法特效序号239以后为玩家角色合体
  普通攻击效果所用
  过场特效序号239以后为异常状态特效
  动画所用，249以后为主菜单动画所用

---------------------------------*/

typedef struct {/*特效数据结构（5 byte）*/
	U8 x;		/*该图片在屏幕上输出的坐标*/
	U8 y;		/*该图片在屏幕上输出的坐标*/
	U8 cdelay;	/*本张图片显示延时*/
	U8 ndelay;	/*下一张图片在本张图片显示多长时间后显示*/
	U8 picIdx;	/*本桢所用图片序号*/
	}SPEUNIT;

typedef struct {		/*特效图片组结构（4 byte）*/
	U8	 type;			/*特效类型*/
	U8	 idx;			/*特效序号(1~239为任意分配的序号)*/
	U8	 count;			/*桢数*/
	U8	 picmax;		/*最大图片序号*/
	U8	 startfrm;		/*播放起始帧*/
	U8	 endfrm;		/*播放终止帧*/
	/*SPEUNIT  spe[252];*/		/*桢数据项*/
	/*U8	 *dat[252];*/		/*图片数据区*/
	}SPERES;
	
	
typedef struct PosItem
{
	U8 x0;
	U8 y0;
	U8 x1;
	U8 y1;
	U8 sx;
	U8 sy;
	U8 ex;
	U8 ey;
}PosItemType;

typedef struct MenuStruct
{
	U8 sx;
	U8 sy;
	U8 ex;
	U8 ey;
	U8 top;
	U8 set;
	U8 count;
	U8 *menustr[20];
}MenuType;

typedef struct CitySet
{
	U8 x;
	U8 y;
	U8 setx;
	U8 sety;
}CitySetType;

#define CITYMAP_W	g_engineConfig.cityMapWidth /*地图宽*/
#define CITYMAP_H	g_engineConfig.cityMapHeight /*地图高*/
#define CITYMAP_TIL_W	16	/*地图块大小*/
#define CITYMAP_TIL_H	16	/*地图块大小*/
#define CITY_ICON_W	8	/*城市图标大小*/
#define CITY_ICON_H	8	/*城市图标大小*/
#define SHOWMAP_WS	((WK_EX-WK_SX+1+1)/CITYMAP_TIL_W-2)	/*屏幕显示地图块数*/
#define SHOWMAP_HS	((WK_EY-WK_SY+1)/CITYMAP_TIL_H)	/*屏幕显示地图块数*/

#define SHOWMAP_WS_MAX	30	/*屏幕最大支持显示地图块数*/
#define SHOWMAP_HS_MAX	30	/*屏幕最大支持显示地图块数*/

FAR void InitItem(U8 x0,U8 y0,U8 x1,U8 y1,PosItemType *positem);
FAR U8 AddItem(U8 wid,U8 hig,PosItemType *positem,U8 *x,U8 *y);
FAR U8 NextLineItem(PosItemType *positem);

#endif
