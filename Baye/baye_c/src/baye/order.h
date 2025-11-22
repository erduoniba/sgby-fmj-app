#ifndef ORDER_H
#define ORDER_H

#define ORDER_INTERIOR		1		/*内政指令*/
#define NOP             0	/*什么也不做*/
#define ASSART			1	/*开垦*/
#define ACCRACTBUSINESS		2	/*招商*/
#define SEARCH			3	/*搜寻*/
#define FATHER			4	/*治理*/
#define INSPECTION		5	/*出巡*/
#define SURRENDER		6	/*招降*/
#define KILL			7	/*处斩*/
#define BANISH			8	/*流放*/
#define LARGESS			9	/*赏赐*/
#define CONFISCATE		10	/*没收*/
#define EXCHANGE		11	/*交易*/
#define TREAT			12	/*宴请*/
#define TRANSPORTATION		13	/*输送*/
#define MOVE			14	/*移动*/

#define ORDER_DIPLOMATISM	15		/*外交指令*/
#define ALIENATE		15	/*离间*/
#define CANVASS			16	/*招揽*/
#define COUNTERESPIONAGE	17	/*策反*/
#define REALIENATE		18	/*反间*/
#define INDUCE			19	/*劝降*/
/*#define TRIBUTE			20	朝贡*/

#define ORDER_ARMAMENT		23		/*军备指令*/
#define RECONNOITRE		23	/*侦察*/
#define CONSCRIPTION		24	/*征兵*/
#define DISTRIBUTE		25	/*分配*/
#define DEPREDATE		26	/*掠夺*/
#define BATTLE			27	/*出征*/



#define THEW_APPOINT		4	/*任命消耗体力*/
#define THEW_ASSART		4	/*开垦消耗体力*/
#define THEW_ACCRACTBUSINESS	4	/*招商消耗体力*/
#define THEW_SEARCH		4	/*搜寻消耗体力*/
#define THEW_FATHER		4	/*治理消耗体力*/
#define THEW_INSPECTION		4	/*出巡消耗体力*/
#define THEW_SURRENDER		4	/*招降消耗体力*/
#define THEW_KILL		4	/*处斩消耗体力*/
#define THEW_BANISH		4	/*流放消耗体力*/
#define THEW_LARGESS		4	/*赏赐消耗体力*/
#define THEW_CONFISCATE		4	/*没收消耗体力*/
#define THEW_EXCHANGE		4	/*交易消耗体力*/
#define THEW_TREAT		4	/*宴请消耗体力*/
#define THEW_TRANSPORTATION	4	/*输送消耗体力*/
#define THEW_MOVE		4	/*移动消耗体力*/
#define THEW_ALIENATE		4	/*离间消耗体力*/
#define THEW_CANVASS		4	/*招揽消耗体力*/
#define THEW_COUNTERESPIONAGE	4	/*策反消耗体力*/
#define THEW_REALIENATE		4	/*反间消耗体力*/
#define THEW_INDUCE		4	/*劝降消耗体力*/
#define THEW_TRIBUTE		4	/*朝贡消耗体力*/
#define THEW_RECONNOITRE	4	/*侦察消耗体力*/
#define THEW_CONSCRIPTION	4	/*征兵消耗体力*/
#define THEW_DISTRIBUTE		4	/*分配消耗体力*/
#define THEW_DEPREDATE		4	/*掠夺消耗体力*/
#define THEW_BATTLE		4	/*出征消耗体力*/


#define	ORDER_MAX		200	/*最大命令数*/
#define	FIGHT_ORDER_MAX		30	/*最大出征命令数*/


typedef struct OrderStruct	/*命令结构*/
{
	U8 OrderId;		/*命令编号*/
	PersonID Person;		/*执行武将*/
	U8 City;		/*所在城市*/
	PersonID Object;		/*目标*/
	U16 Arms;		/*士兵数量*/
	U16 Food;		/*粮食数量*/
	U16 Money;		/*金钱数量*/
	U8 Consume;		/*消耗时间*/
	U8 TimeCount;		/*执行累时*/
}OrderType;

typedef struct OrderQueue
{
	OrderType iOrder;
	struct OrderQueue *nOrder;
}OrderQueueType;

#endif

