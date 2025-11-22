#ifndef	_FIGHT_H
#define	_FIGHT_H

/*======================================
  定义游戏战斗的常量
======================================*/

/*------------------------
  战斗常量相关定义
------------------------*/
#define	MAP_HEAD_LEN		16		/* 地图数据头长 */
#define	FGT_PLAMAX		10		/* 战斗时，每方最大将领个数 */
#define	FGTA_MAX		FGT_PLAMAX*2	/* 战斗队列最大组员 */
#define	MAX_LEVEL		g_engineConfig.maxLevel		/* 最大等级 */
#define	CITY_TIL		3		/* 城池地图块 */
#define	FGT_DF			0		/* 玩家方(前10个将)战斗模式——防御 */
#define	FGT_AT			1		/* 玩家方(前10个将)战斗模式——进攻 */
#define	FGT_AUTO		2		/* 电脑进攻电脑 */
#define	FGT_COMON		0		/* 战斗继续 */
#define	FGT_WON			1		/* 战斗胜利(前10个将) */
#define	FGT_LOSE		2		/* 战斗失败(前10个将) */
#define	FGT_EXPMAX		100		/* 最大经验值 */
#define	NO_MOV			1		/* 被定身时的移动力量 */
#define	JN_MAX			100		/* 最大技能序号 */
#define	MNU_EXIT		0xFF		/* 菜单中跳出 */

/*------------------------
  战斗界面相关定义
------------------------*/
#define	SCR_MAPWID		(SCR_WID/16)            /* 屏幕横向地图块个数 */
#define	SCR_MAPHGT		(SCR_HGT/16 - 1)		/* 屏幕纵向地图块个数 */
#define	SHOW_MX			(SCR_MAPWID / 2)        /* 当前将领屏幕地图块坐标 */
#define	SHOW_MY			(SCR_MAPHGT / 2)        /* 当前将领屏幕地图块坐标 */

#define FIGHT_MAP_BUFFER_LEN (256)  // (256/16)**2   256 is the max screen size
#define FIGHT_MAP_DATA_LEN (256*256)

/* 战斗特效播放坐标 */
#define	FGT_SPESX		((SCR_WID-130) / 2)
#define	FGT_SPESY		((SCR_HGT-64) / 2)
#define	KING_TX			WK_SX + 72
#define	KING_TY			WK_SY + 4
#define	KING_SX			WK_SX+16	/* 君主显示坐标 */
#define	KING_EX			KING_SX+35	/* 君主显示坐标 */
#define	KING_SY			WK_SY+13	/* 君主显示坐标 */
#define	CITY_SX			WK_SX+60	/* 城市缩略图显示坐标 */
#define	CITY_SY			WK_SY+20	/* 城市缩略图显示坐标 */

/* 状态栏 */
#define	STA_SX			WK_SX		/* 状态栏显示起始坐标 */
#define	STA_EX			WK_EX		/* 状态栏显示终止坐标 */
#define	STA_SY			WK_EY-15	/* 状态栏显示起始坐标 */
#define	STA_EY			WK_EY		/* 状态栏显示终止坐标 */
#define	STA_LCX			STA_SX+14	/* 粮草显示的坐标 */
#define	STA_LCY			STA_SY+3	/* 粮草显示的坐标 */
#define	STA_TQX			STA_SX+47	/* 天气图标显示的坐标 */
#define	STA_TQY			STA_SY+2	/* 天气图标显示的坐标 */
#define	STA_XXX			STA_SX+74	/* 信息显示坐标 */
#define	STA_XXY			STA_SY+2	/* 信息显示坐标 */

/* 帮助信息 */
#define	HLP_SX			WK_SX+4		/* 帮助显示框起始坐标 */
#define	HLP_EX			WK_EX-5		/* 帮助显示框终止坐标 */
#define	HLP_SY			WK_SY+4		/* 帮助显示框起始坐标 */
#define	HLP_EY			WK_EY-5		/* 帮助显示框终止坐标 */

/*------------------------
  兵种相关定义
------------------------*/
#define	ARMTYP_NUM		6		/* 兵种个数 */
#define	ARM_QIBING		0		/* 骑兵 */
#define	ARM_BUBING		1		/* 步兵 */
#define	ARM_GOBING		2		/* 弓箭兵 */
#define	ARM_SHUIJUN		3		/* 水军 */
#define	ARM_JIBING		4		/* 极兵 */
#define	ARM_XUANBING		5		/* 玄兵 */
#define	MOV_QIBING		5		/* 骑兵基本移动力 */
#define	MOV_BUBING		4		/* 步兵基本移动力 */
#define	MOV_GOBING		4		/* 弓箭兵基本移动力 */
#define	MOV_SHUIJUN		5		/* 水军基本移动力 */
#define	MOV_JIBING		6		/* 极兵基本移动力 */
#define	MOV_XUANBING		3		/* 玄兵基本移动力 */

/*------------------------
  战斗天气相关定义
------------------------*/
#define	WEATHER_FINE		1		/* 晴 */
#define	WEATHER_CLOUDY		2		/* 阴 */
#define	WEATHER_WIND		3		/* 风 */
#define	WEATHER_RAIN		4		/* 雨 */
#define	WEATHER_HAIL		5		/* 冰雹 */

/*------------------------
  战斗地形相关定义
------------------------*/
#define	TIL_WID			16		/* 地图块宽度 */
#define	TIL_HGT			16		/* 地图块高度 */
#define	TERRAIN_MAX		8		/* 地形种类个数 */
#define	TERRAIN_LEA		0		/* 草地 */
#define	TERRAIN_DENE		1		/* 平原 */
#define	TERRAIN_HILL		2		/* 山地 */
#define	TERRAIN_WOOD		3		/* 森林 */
#define	TERRAIN_THORP		4		/* 村庄 */
#define	TERRAIN_CITY		5		/* 城池 */
#define	TERRAIN_TENT		6		/* 营寨 */
#define	TERRAIN_RIVER		7		/* 河流 */
#define	TERN_DENES		0		/* 无效图块结束号 */
#define	TERN_LEAS		1		/* 平原结束号 */
#define	TERN_CITYS		2		/* 草地结束号 */
#define	TERN_THORPS		3		/* 城镇结束号 */
#define	TERN_WOODS		4		/* 村庄结束号 */
#define	TERN_HILLS		5		/* 森林结束号 */
#define	TERN_RIVERS		15		/* 山地结束号 */
#define	TERN_TENTS		41		/* 营寨地图块 */

/*------------------------
  战斗状态相关定义
------------------------*/
#define	STATE_ZC		0		/* 正常 */
#define	STATE_HL		1		/* 混乱 */
#define	STATE_JZ		2		/* 禁咒 */
#define	STATE_DS		3		/* 定身 */
#define	STATE_QM		4		/* 奇门 */
#define	STATE_DJ		5		/* 遁甲 */
#define	STATE_SZ		6		/* 石阵 */
#define	STATE_QZ		7		/* 潜踪 */
#define	STATE_SW		8		/* 死亡 */

/*------------------------
  行军算法相关定义
------------------------*/
#define	FGT_TILMAX		46		/* 地图块个数 */
#define	FGT_MVMAX		8		/* 最大移动力 */
#define	FGT_MRG			15		/* 最大移动力范围(FGT_MVMAX-1)*2 */
#define MOVCHI_LEN		(FGT_MVMAX-2)*4
#define	MOV_SPT			0x00		/* 移动将领所在点被扩充后填充的值 */
#define	MOV_OVER		0xFF		/* 超出地图范围的点 */
#define MOV_RSTD		0x80		/* 地况阻力基值 */
#define MOV_NOT			0xFE		/* 不可以移入的点 */
#define MOV_WKP			0xA0		/* 待激活点-被激活就可移入，没被激活就不可移入 */

/*------------------------
  命令执行标志
------------------------*/
#define	STA_WAIT		0		/* 等待命令 */
#define	STA_END			1		/* 命令结束 */
/* 命令类型 */
#define	CMD_ATK			0		/* 攻击命令 */
/*#define	CMD_PK			1	*/	/* 单挑命令 */
#define	CMD_STGM		1		/* 计谋命令 */
#define	CMD_LOOK		2		/* 查看命令 */
#define	CMD_REST		3		/* 休息命令 */

/*------------------------
  武器攻击范围相关定义
------------------------*/
#define	TOOL_ATT_RANGEUNIT		15
#define	TOOL_ATT_RANGE		TOOL_ATT_RANGEUNIT*TOOL_ATT_RANGEUNIT

/*------------------------
  攻击范围相关定义
------------------------*/
#define	ATT_RANGEUNIT		5
#define	ATT_RANGE		ATT_RANGEUNIT*ATT_RANGEUNIT

#define	MAX_ATT_RANGEUNIT TOOL_ATT_RANGEUNIT
#define	MAX_ATT_RANGE	MAX_ATT_RANGEUNIT*MAX_ATT_RANGEUNIT
/*------------------------
  技能相关定义
------------------------*/
#define	SKILL_NMAX		10		/* 每个将领最多可学会技能个数 */
#define SKILL_NAMELEN		4		/* 技能名字长度 */
#define SKILL_INFLEN		50		/* 技能说明长度 */
#define SKILL_RANGEUNIT		9		/* 技能施展范围单元 */
#define SKILL_RANGE		SKILL_RANGEUNIT*SKILL_RANGEUNIT
/* 施展对象 */
#define AIM_ENEMY		0		/* 敌方为施展目标 */
#define AIM_OWNER		1		/* 我方为施展目标 */
/* 特殊技能序号 */


/*------------------------
  战斗相关结构体定义
------------------------*/
typedef struct {
	U8	Mode;				/* 玩家的战斗模式FGT_DF|FGT_AT */
	U8	Way;				/* 战斗进攻方向 */
	U16	MapId;				/* 城市战斗地图 */
	U16	MProvender;			/* 玩家粮草 */
	U16	EProvender;			/* 敌人粮草 */
    PersonID	GenArray[FGTA_MAX];		/* 将领队列 */
    U8	CityIndex;			/* 战斗城市ID */
}FGTJK;						/* 战斗接口 */

typedef	struct {
	U8	aim;				/* 施展目标 */
	U8	state;				/* 技能对目标状态的影响 */
	U16	power;				/* 对兵力的基本伤害 */
	U16	destroy;			/* 对粮草的基本伤害 */
	U8	useMp;				/* 消耗技能点 */
	U8	weather[5];			/* 天气效果（0为不可施展 1-100%效果）*/
	U8	eland[8];			/* 敌人所在地形效果（0为不可施展 1-100%效果）*/
	U8	oland[8];			/* 我方所在地形效果（0为不可施展 1-100%效果）*/
	U8	earm[6];			/* 敌人兵种加层（0为不可施展 1-100%效果）*/
}SKILLEF;

typedef	struct {
	U8	name[SKILL_NAMELEN];		/* 技能名称 */
	U8	inf[SKILL_INFLEN];		/* 技能说明 */
	SKILLEF	ef;
	U8	range[SKILL_RANGE];		/* 攻击范围9*9方格以内的有效点 */
}SKILL;						/* 技能结构定义 */

typedef	struct {
	U8	x;				/* 地图坐标x */
	U8	y;				/* 地图坐标y */
	U16	hp;				/* 生命 */
	U16	mp;				/* 技能点 */
	U8	move;				/* 移动力 */
	U8	active;				/* 将领执行命令能力 */
	U8	state;				/* 状态 */
}JLPOS;						/* 将领在地图上的位置 */

typedef	struct {
	U8	*level;				/* 将领等级 */
	U8	canny;				/* 中计谋的可能性 */
	U8	ter;				/* 将领所在地形 */
	U8	bile;				/* 愤怒值 */
	U8	armsType;			/* 兵种 */
	U8	*exp;				/* 将领的经验值 */
	U16	*arms;				/* 兵力 */
	U16	at;				/* 攻击力 */
	U16	df;				/* 防御力 */
	U8	generalIndex;   /* 将领战场序号 (新增) */
}JLATT;						/* 执行攻击或被攻击的将领属性 */

typedef	struct {
	U8	type;				/* 命令类型 */
    U16 param;				/* 命令参数 */
	U8	sIdx;				/* 命令执行者脚标(GenArray[sIdx]) */
	U8	aIdx;				/* 命令结果承受者脚标(GenArray[aIdx]) */
}FGTCMD;					/* 战斗命令结构体 */
	
/*------------------------
  宏
------------------------*/
#define	FgtGetScrX(tx)		((tx - g_MapSX) << 4) + WK_SX
#define	FgtGetScrY(ty)		((ty - g_MapSY) << 4) + WK_SY

FAR void FgtLoadConsts(void);

#endif	/* _FIGHT_H */
