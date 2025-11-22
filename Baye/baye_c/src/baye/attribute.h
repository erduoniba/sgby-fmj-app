#ifndef ATTRIBUT_H
#define ATTRIBUT_H
#include "touch.h"

#define PERSON_MAX		2000	/*最大武将数*/
#define PERSON_COUNT	GamGetPersonCount()	/*武将数*/
#define CITY_MAX		g_engineConfig.citiesCount	/*最大城市数*/
#define GOODS_MAX		2000	/*最大道具数*/
#define PERSON_DEATH_AGE	90	/*武将寿命*/
#define PERSON_APPEAR_AGE	16	/*武将出现年龄*/

/*------------------------
  君主性格定义
  ------------------------*/
#define CHARACTER_PEACE		4	/*和平*/
#define CHARACTER_JUSTICE	3	/*大义*/
#define CHARACTER_DUPLICITY	2	/*奸诈*/
#define CHARACTER_CRAZY		1	/*狂人*/
#define CHARACTER_RASH		0	/*冒进*/

/*------------------------
  君主性格劝降机率
  ------------------------*/
#define PERSUADE_PEACE		15	/*和平*/
#define PERSUADE_JUSTICE	5	/*大义*/
#define PERSUADE_DUPLICITY	20	/*奸诈*/
#define PERSUADE_CRAZY		1	/*狂人*/
#define PERSUADE_RASH		10	/*冒进*/

/*------------------------
  性格定义
  ------------------------*/
#define CHARACTER_LOYALISM	4	/*忠义*/
#define CHARACTER_IDEAL		3	/*大志*/
#define CHARACTER_AVARICE	2	/*贪财*/
#define CHARACTER_DREAD		1	/*怕死*/
#define CHARACTER_TEMERITY	0	/*卤莽*/

/*------------------------
  性格离间机率
  ------------------------*/
#define ALIENATE_LOYALISM	5	/*忠义*/
#define ALIENATE_IDEAL		30	/*大志*/
#define ALIENATE_AVARICE	40	/*贪财*/
#define ALIENATE_DREAD		30	/*怕死*/
#define ALIENATE_TEMERITY	50	/*卤莽*/

/*------------------------
  性格招揽机率
  ------------------------*/
#define CANVASS_LOYALISM	5	/*忠义*/
#define CANVASS_IDEAL		20	/*大志*/
#define CANVASS_AVARICE		30	/*贪财*/
#define CANVASS_DREAD		40	/*怕死*/
#define CANVASS_TEMERITY	15	/*卤莽*/

/*------------------------
  性格策反机率
  ------------------------*/
#define COUNTERESPIONGE_LOYALISM	5	/*忠义*/
#define COUNTERESPIONGE_IDEAL		60	/*大志*/
#define COUNTERESPIONGE_AVARICE		20	/*贪财*/
#define COUNTERESPIONGE_DREAD		10	/*怕死*/
#define COUNTERESPIONGE_TEMERITY	30	/*卤莽*/

/*------------------------
  性格招降机率
  ------------------------*/
#define SURRENDER_LOYALISM	5	/*忠义*/
#define SURRENDER_IDEAL		20	/*大志*/
#define SURRENDER_AVARICE	30	/*贪财*/
#define SURRENDER_DREAD		60	/*怕死*/
#define SURRENDER_TEMERITY	15	/*卤莽*/

/*------------------------
  性格招降机率系数
  ------------------------*/
#define SURRENDER_LOYALISM_MOD	1	/*忠义*/
#define SURRENDER_IDEAL_MOD	3	/*大志*/
#define SURRENDER_AVARICE_MOD	4	/*贪财*/
#define SURRENDER_DREAD_MOD	5	/*怕死*/
#define SURRENDER_TEMERITY_MOD	2	/*卤莽*/

/*------------------------
  方向定义
  ------------------------*/
#define	DIRECT_N	0		/* 方向——北 */
#define	DIRECT_EN	1		/* 方向——东北 */
#define	DIRECT_E	2		/* 方向——东 */
#define	DIRECT_ES	3		/* 方向——东南 */
#define	DIRECT_S	4		/* 方向——南 */
#define	DIRECT_WS	5		/* 方向——西南 */
#define	DIRECT_W	6		/* 方向——西 */
#define	DIRECT_WN	7		/* 方向——西北 */


/*------------------------
  城市状态定义
  ------------------------*/
#define	STATE_NORMAL		0	/*正常*/
#define	STATE_FAMINE		1	/*饥荒*/
#define	STATE_DROUGHT		2	/*旱灾*/
#define	STATE_FLOOD		3	/*水灾*/
#define	STATE_REBELLION		4	/*暴动*/


#define THREW_RENEW		4	/*每月恢复体力*/
#define THREW_TREAT		50	/*宴请恢复体力*/

typedef struct {
	U8	idx_;			/* 道具序号 */
	U8	useflag;		/* 使用标志：是被使用还是被装备*/
	U8	atRange[30];		/* 攻击范围数据 */
    U8  changeAttackRange; 	/* 是否改变攻击范围 */
	U8	reserved[60-31];
	U8	at;			/* 对武力的加层 */
	U8	iq;			/* 对智力的加层 */
	U8	move;			/* 对移动力的加层 */
	U8	arm;			/* 对兵种的改变 */
}GOODS;

typedef struct Person				/*人才属性 (12 Bytes) */
{
	PersonID OldBelong;		/*编号*/
	PersonID Belong;			/*归属*/
	U8 Level;			/*等级*/
	U8 Force;			/*武力*/
	U8 IQ;				/*智力*/
	U8 Devotion;			/*忠诚*/
	U8 Character;			/*性格*/
	U8 Experience;			/*经验*/
	U8 Thew;			/*体力*/
	U8 ArmsType;			/*兵种*/
	U16 Arms;			/*兵力*/
	ToolID Equip[2];			/*装备*/
	U8 Age;				/*年龄*/
}PersonType;

typedef struct City				/*城市属性(30 Bytes)*/
{
	U8 State;
	PersonID Belong;			/*归属*/
	PersonID SatrapId;			/*太守编号*/
	U16 FarmingLimit;		/*农业上限*/
	U16 Farming;			/*农业开发度*/
	U16 CommerceLimit;		/*商业上限*/
	U16 Commerce;			/*商业开发度*/
	U8 PeopleDevotion;		/*民忠*/
	U8 AvoidCalamity;		/*防灾*/
	U32 PopulationLimit;		/*人口上限*/
	U32 Population;			/*人口*/
	U16 Money;			/*金钱*/
	U16 Food;			/*粮食*/
	U16 MothballArms;		/*后备兵力*/
	PersonID PersonQueue;			/*人才队列*/
	PersonID Persons;			/*人才数*/
	ToolID ToolQueue;			/*道具队列*/
	ToolID Tools;			/*道具数*/
}CityType;

typedef struct {
    U8 birth;
    PersonID bole;
    U8 city;
} SearchCondition;

typedef struct {
  U8 landMapColor;
  U8 ownedCityColor;
  U8 emptyCityColor;
  U8 otherCityColor;
  U8 landCursorColor;
  U8 battleNoteColor;
  U8 kingHeadColor;
  U8 personHeadColor;

  U8 fightMoveRangeColor;
  U8 fightMapColor;
} Theme;

typedef struct {
    U8 enableToolAttackRange; //启用"道具改变攻击范围"
    U8 fixCityOffset;    //纠正城市偏移
    U8 fixThewOverFlow;  //修复体力溢出bug
    U8 fixFoodOverFlow;  //修复出征刷粮草
    U8 fixOverFlow16;    //修复多处16位溢出
    U8 fixConsumeMoney;  //启用招降收费
    U8 fixFightMoveOutRange; //修复战场瞬移
    U8 enable16bitConsumeMoney;   //启用扩展金钱消耗(16位)
    U8 enableScript;              //启用脚本
    U8 fixAlienateComsumeThew; //修复离间减小敌将体力问题
    U8 disableSL;              //禁SL
    U8 aiLevelUpSpeed;            //AI升级速度 (0~100, 0使用原版默认策略)

    U8 disableAgeGrow;         //禁用年龄增长
    U8 enableCustomRatio;      //启用自定义参数计算, 下面两线之间的参数总开关
    // ---------------------------
    U16 ratioOfArmsToLevel;    //带兵量和等级的关系, 默认100  (X*R)
    U8 ratioOfArmsToAge;      //带兵量和年龄的关系, 默认0  (X*R)
    U8 ratioOfArmsToIQ;        //带兵量和智力的关系, 默认10  (X*R)
    U8 ratioOfArmsToForce;     //带兵量和武力的关系, 默认10  (X*R)

    U8 ratioOfAttToForce;      //武力对攻击的影响, 默认10  (X*R/10)
    U8 ratioOfAttToIQ;          //智力对攻击的影响, 默认0  (X*R/10)
    U8 ratioOfAttToAge;         //年龄对攻击的影响, 默认0  (X*R/10)

    U8 ratioOfDefenceToForce; //武力对防御的影响, 默认0  (X*R/10)
    U8 ratioOfDefenceToIQ;    //智力对防御的影响, 默认10  (X*R/10)
    U8 ratioOfDefenceToAge;   //年龄对防御的影响, 默认0  (X*R/10)
    // ---------------------------
    U16 ratioOfFoodToArmsPerMouth; // 市政兵力和粮耗(默认50，越大粮耗越少)
    U16 ratioOfFoodToArmsPerDay; //战场兵力和粮耗参数(默认3，越大粮耗越少)
    U8 armsPerDevotion;        //征兵量和民忠的关系(默认20)
    U8 armsPerMoney;           //每个金钱能购买的士兵数(默认10)
    U8 maxLevel;                //最大等级
    U8 responseNoteOfBettle;   //"战斗提示"允许按键跳过(默认0，填2允许)
    U8 aiDefenceMode;          // AI行军策略，默认0追主将，1守城
    U8 aiAttackMethod;          // AI攻击时选择技能/普攻的算法(0原版，1优化版)

    U8 batlleDisableWeatherChange;    // 禁止战场天气变化
    Rect mainMenuButtonRects[4];   // 主界面4个按钮的边框位置
    Rect periodMenuButtonRects[4]; // 时期界面4个按钮的边框位置
    Point saveFaceListAnchor;      // 存档界面list左上角坐标
    U8 citiesCount;     //城池个数
    U8 cityMapWidth;    // 主地图宽
    U8 cityMapHeight;    // 主地图高
    
    U8 disableExpGrowing;
    U8 hideRightBorder;
    U8 promptCityDisaster; // 提示城池灾害
    U8 disableAllPersonReport; // 禁用所有人物报告对话框
    U8 showStartMovie; // 显示开场动画
    U8 compressCustomData; // 压缩扩展存档数据
    U8 checkRedundantOnAddPerson; // 往城池添加人物时检查重复
    U8 disableFightToDeath; // 战斗中不死亡
    U8 useCustomFont;       // 使用自定义字体
    U8 useCustomFontEn;     // 英文和数字使用自定义字体
    U8 cacheCustomFont;     // 缓存自定义字体

    Theme theme; // 主题
} EngineConfig;

extern EngineConfig g_engineConfig;

extern U8 g_engineDebug;

void GamSetDebug(U8 enabled);

U16 add_16(U16 dst, int n);

#define ADD16(d, n) d = add_16(d, n)

#endif
