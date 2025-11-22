/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	infdeal.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		封装引擎对系统显示程序调用的入口
 **
 *移植性声明:
 *	1、符合标准：《游戏设计标准V1.0》
 *	2、兼容模式：本程序和界面无关，无兼容模式。
 *	3、使用本封装程序的引擎要移植到其他环境中，系统的显示部分只需在此修改
 *修改历史：
 *	版本    日期     作者     改动内容和原因
 *   ----------------------------------------------------
 *	1.0    2005-6-29 14:45  陈泽伟     基本的功能完成
 ***********************************************************************/

#include "baye/stdsys.h"
#include "baye/comm.h"
#undef	INFDEAL_C
#define	INFDEAL_C
#include "baye/enghead.h"
#include "baye/script.h"
#include "baye/bind-objects.h"



/******************************************************************************
 * 函数名:CitiesUpDataDate
 * 说  明:城市发展
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR U8 CitiesUpDataDate(void)
{
    U32 i,j,s,rnd;
    PersonID *pqptr;
    U16 df,as,*food;
    U32 *Population;
    CityType *cptr;
    U32 c;

    if (6 == g_MonthDate || 10 == g_MonthDate)
    {
        HarvestryFood();
    }
    if (!(g_MonthDate % 3))
    {

        RevenueMoney();
    }

    for (i = 0;i < PERSON_COUNT;i ++)
    {
        g_Persons[i].Thew += THREW_RENEW;
        if (g_Persons[i].Thew > 100)
            g_Persons[i].Thew = 100;
    }

    pqptr = (PersonID*)SHARE_MEM;
    for (i = 0;i < CITY_MAX;i ++)
    {
        cptr = &g_Cities[i];
        if (cptr->Belong)
        {
            if (!(g_MonthDate % 3))
            {
                rnd = 1 + gam_rand() % 4;
                if (cptr->AvoidCalamity > rnd)
                    cptr->AvoidCalamity -= rnd;
            }
            if (cptr->Money > 30000)
                cptr->Money = 30000;

            Population = &cptr->Population;
            *Population += 50;
            if (*Population > cptr->PopulationLimit)
                *Population = cptr->PopulationLimit;

            df = cptr->MothballArms;
            c = GetCityPersons(i,pqptr);
            for (j = 0;j < c;j ++)
            {
                as = g_Persons[pqptr[j]].Arms;
                switch (g_Cities[i].State)
                {
                    case STATE_NORMAL:	/*正常*/
                    case STATE_FAMINE:	/*饥荒*/
                        break;
                    case STATE_DROUGHT:	/*旱灾*/
                        as = as - as / 4;
                        break;
                    case STATE_FLOOD:	/*水灾*/
                        as = as - as / 4;
                        break;
                    case STATE_REBELLION:	/*暴动*/
                        as /= 2;
                        break;
                }
                df += as;
                g_Persons[pqptr[j]].Arms = as;
            }
            if (g_engineConfig.ratioOfFoodToArmsPerMouth) {
                df /= g_engineConfig.ratioOfFoodToArmsPerMouth;
            } else {
                df /= 50;
            }

            food = &cptr->Food;
            if (*food > df)
            {
                *food -= df;
            }
            else
            {
                for (j = 0;j < c;j ++)
                {
                    g_Persons[pqptr[j]].Arms /= 2;
                }
                *food = 0;
                /*闹饥荒*/
                s = cptr->State;;
                cptr->State = STATE_FAMINE;
                if ((STATE_FAMINE != s) && (cptr->Belong == g_PlayerKing + 1))
                    ReportCalamity(i);
            }
            c = GetCityCaptives(i,pqptr);
            for (j = 0;j < c;j ++)
            {
                if (g_Persons[pqptr[j]].OldBelong == cptr->Belong)
                    g_Persons[pqptr[j]].Belong = cptr->Belong;
            }
        }
    }
    return(1);
}

/******************************************************************************
 * 函数名:RandEvents
 * 说  明:随机事件
 *
 * 入口参数：
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR U8 RandEvents(void)
{
    U8 i;
    U8 *state;
    U8 rnd;
    PersonID ccb;

    for (i = 0;i < CITY_MAX;i ++)
    {
        ccb = g_Cities[i].Belong;
        if (ccb)
        {
            state = &g_Cities[i].State;
            rnd = gam_rand() % 100;
            switch (*state)
            {
                case STATE_NORMAL:
                    if (rnd > g_Cities[i].AvoidCalamity)
                    {
                        rnd = gam_rand() % 5;
                        switch (rnd)
                        {
                            case 0:		/*遭旱灾*/
                                *state = STATE_DROUGHT;
                                break;
                            case 1:		/*遭水灾*/
                                *state = STATE_FLOOD;
                                break;
                            case 2:		/*暴动*/
                                rnd = gam_rand() % 100;
                                if (rnd > g_Cities[i].PeopleDevotion)
                                    *state = STATE_REBELLION;
                                break;
                            default:continue;break;/*无灾害*/
                        }
                        if ((STATE_NORMAL != *state) && (ccb == g_PlayerKing + 1))
                        {
                            ReportCalamity(i);
                        }
                    }
                    break;
                case STATE_FAMINE:	/*饥荒*/
                    if (g_Cities[i].Food)
                        *state = STATE_NORMAL;
                    break;
                case STATE_DROUGHT:	/*旱灾*/
                case STATE_FLOOD:	/*水灾*/
                    if (rnd < g_Cities[i].AvoidCalamity)
                        *state = STATE_NORMAL;
                    break;
                case STATE_REBELLION:	/*暴动*/
                    if (rnd < g_Cities[i].PeopleDevotion)
                        *state = STATE_NORMAL;
                    break;
            }
        }
    }
    return(1);
}

/******************************************************************************
 * 函数名:EventStateDeal
 * 说  明:事件状态处理
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-12 15:21	基本功能完成
 ******************************************************************************/
FAR U8 EventStateDeal(void)
{
    U8 i;
    U8 *PeopleDevotion;
    U16 *Farming,*Commerce,*MothballArms,*Food,*Money;
    U32 *Population;

    for (i = 0;i < CITY_MAX;i ++)
    {
        PeopleDevotion = &g_Cities[i].PeopleDevotion;
        Farming = &g_Cities[i].Farming;
        Commerce = &g_Cities[i].Commerce;
        MothballArms = &g_Cities[i].MothballArms;
        Food = &g_Cities[i].Food;
        Money = &g_Cities[i].Money;
        Population = &g_Cities[i].Population;


        switch (g_Cities[i].State)
        {
            case STATE_NORMAL:	/*正常*/
                continue;
                break;
            case STATE_FAMINE:	/*饥荒*/
                *Commerce = *Commerce - *Commerce / 20;
                *PeopleDevotion = *PeopleDevotion - *PeopleDevotion / 20;
                *MothballArms /= 2;
                *Population = *Population - *Population / 4;
                break;
            case STATE_DROUGHT:	/*旱灾*/
                *Food = *Food - *Food / 20;
                *MothballArms = *MothballArms - *MothballArms / 4;
                *Population = *Population - *Population / 4;
                break;
            case STATE_FLOOD:	/*水灾*/
                *Food = *Food - *Food / 20;
                *Commerce = *Commerce - *Commerce / 10;
                *Money = *Money - *Money / 10;
                *MothballArms = *MothballArms - *MothballArms / 4;
                *Population = *Population - *Population / 4;
                break;
            case STATE_REBELLION:	/*暴动*/
                *Food = *Food - *Food / 20;
                *Commerce = *Commerce - *Commerce / 20;
                *Money = *Money - *Money / 20;
                *PeopleDevotion = *PeopleDevotion - *PeopleDevotion / 10;
                *MothballArms /= 2;
                break;
        }
        *Farming = *Farming - *Farming / 20;
    }
    return(1);
}

/******************************************************************************
 * 函数名:RevenueMoney
 * 说  明:各城市获得税收金钱
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-6 8:50	基本功能完成
 ******************************************************************************/
void RevenueMoney(void)
{
    U8 c;

    for (c = 0;c < CITY_MAX;c ++)
    {
        if (g_Cities[c].Money < 30000)
            g_Cities[c].Money += g_Cities[c].Commerce / 2;
    }
}

/******************************************************************************
 * 函数名:HarvestryFood
 * 说  明:各城市获得收获食物
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-6 8:50	基本功能完成
 ******************************************************************************/
void HarvestryFood(void)
{
    U8 c;

    for (c = 0;c < CITY_MAX;c ++)
    {
        if (g_Cities[c].Food < 30000) {
            if (g_engineConfig.fixOverFlow16) {
                ADD16(g_Cities[c].Food, g_Cities[c].Farming / 4);
            } else {
                g_Cities[c].Food += g_Cities[c].Farming / 4;
            }
        }
    }
}

/******************************************************************************
 * 函数名:GoodsUpDatadate
 * 说  明:更新道具信息
 *
 * 入口参数：无
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-4 10:43	基本功能完成
 ******************************************************************************/
FAR U8 GoodsUpDatadate(void)
{
    SearchCondition *ginf;	/* -- 道具信息指针*/
    U32 g;
    U16 t;
    U32 l = ResGetItemLen(GOODS_CON, g_PIdx);
    ginf = (SearchCondition*)ResLoadToCon(GOODS_CON,g_PIdx,g_CBnkPtr);

    for (g = 0,t = 0;t < l;g ++,t += 3)
    {
        if (ginf[t].birth == g_YearDate)
        {
            AddGoods(ginf[t + 2].city,TID(g));
        }
    }
    return 1;
}

/******************************************************************************
 * 函数名:PersonUpDatadate
 * 说  明:更新武将信息
 *
 * 入口参数：无
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-4 10:43	基本功能完成
 ******************************************************************************/
FAR U8 PersonUpDatadate(void)
{
    SearchCondition *pinf;	/* -- 武将信息指针*/
    U32 c;
    U32 p;

    if (!g_engineConfig.disableAgeGrow && 1 == g_MonthDate)
    {
        for (p = 0;p < PERSON_COUNT;p ++)
        {
            g_Persons[p].Age += 1;
        }
    }

    U32 l = ResGetItemLen(GENERAL_CON, g_PIdx) / sizeof(SearchCondition);
    pinf = (SearchCondition*)ResLoadToCon(GENERAL_CON,g_PIdx,g_CBnkPtr);

    for (p = 0;p < PERSON_COUNT && p < l;p ++)
    {
        if ((pinf[p].birth + PERSON_APPEAR_AGE) == g_YearDate)
        {
            c = pinf[p].city;

            if (!c)
            {
                c = gam_rand() % CITY_MAX;
            }

            if (g_engineConfig.fixCityOffset) {
                c -= 1;
            }

            if (!AddPerson(c,PID(p)))
                return(0);

            if (!g_engineConfig.disableAgeGrow) {
                g_Persons[p].Age = PERSON_APPEAR_AGE;
            }
        }
    }

    /*
     for (c = 0;c < CITY_MAX;c ++)
     {
     e = g_Cities[c].PersonQueue + g_Cities[c].Persons;
     for (p = g_Cities[c].PersonQueue;p < e;p ++)
     {
     g_Persons[p].Age += 1;
     if (PERSON_DEATH_AGE <= g_Persons[p].Age)
     {
     if (gam_rand() % 100 < 50)
     {
					if (!DelPerson(c,p))
					{
     return(0);
					}
     }
     }
     }
     }
     */
    return(1);
}


/******************************************************************************
 * 函数名:AddGoodsPerson
 * 说  明:添加道具属性到武将
 *
 * 入口参数：无
 *
 * 出口参数：0 --装备，1 --  使用，0xff -- 无法装备
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR U8 AddGoodsPerson(ToolID goods,PersonID person)
{
    GOODS *gptr;

    gptr = (GOODS *) ResLoadToCon(GOODS_RESID,1,g_CBnkPtr);
    U8 arm = gptr[goods].arm;

    IF_HAS_HOOK("giveTool") {
        U16 personIndex = person;
        U16 toolIndex = goods;
        U8 result = 0;

        BIND_U16(&personIndex);
        BIND_U16(&toolIndex);
        BIND_U8(&result);

        if (CALL_HOOK_A() == 0 && result == 0) {
            HOOK_RETURN(0xff);
        }
    }

    if (gptr[goods].useflag) {
        switch (arm)
        {
            case 0:
                break;
            case 1:		/*水兵*/
                g_Persons[person].ArmsType = ARM_SHUIJUN;
                break;
            case 2:		/*玄兵*/
                if (g_Persons[person].IQ > 105)
                {
                    g_Persons[person].ArmsType = ARM_XUANBING;
                    break;
                }
                return(0xff);
            case 3:		/*极兵*/
                if (g_Persons[person].Force > 105)
                {
                    g_Persons[person].ArmsType = ARM_JIBING;
                    break;
                }
                return(0xff);
            default:
                g_Persons[person].ArmsType = arm-4;
                break;
        }
    }

    g_Persons[person].Force += gptr[goods].at;
    g_Persons[person].IQ += gptr[goods].iq;
    return(gptr[goods].useflag);
}

/******************************************************************************
 * 函数名:DelGoodsPerson
 * 说  明:从武将删除道具属性
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR U8 DelGoodsPerson(ToolID goods,PersonID person)
{
    GOODS *gptr;

    gptr = (GOODS *) ResLoadToCon(GOODS_RESID,1,g_CBnkPtr);

    IF_HAS_HOOK("takeOffTool") {
        U16 personIndex = person;
        U16 toolIndex = goods;
        U8 result = 0;

        BIND_U16(&personIndex);
        BIND_U16(&toolIndex);
        BIND_U8(&result);

        if (CALL_HOOK_A() == 0 && result == 0) {
            HOOK_RETURN(0);
        }
    }

    g_Persons[person].Force -= gptr[goods].at;
    g_Persons[person].IQ -= gptr[goods].iq;
    return 1;
}

/******************************************************************************
 * 函数名:ShowConstStrMsg
 * 说  明:显示常量字符串信息
 *
 * 入口参数：idx -- 常量字符串编号
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-29 0:00	基本功能完成
 ******************************************************************************/
FAR void ShowConstStrMsg(U8 idx)
{
    U8 str[20];
    ResLoadToMem(STRING_CONST,idx,str);
    GamMsgBox(str,2);
}

/******************************************************************************
 * 函数名:ShowAttackMsg
 * 说  明:显示攻城信息
 *
 * 入口参数：fs -- 进攻方君主，co -- 防守方城池
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-29 0:00	基本功能完成
 ******************************************************************************/
/*FAR void ShowAttackMsg(U8 fs,U8 co)
 {
	SBUF str,buf;

	if (fs == g_PlayerKing)
	{
 ResLoadToMem(STRING_CONST,STR_OURS,str);
	}
	else
	{
 GetPersonName(fs,str);
	}
	ResLoadToMem(STRING_CONST,STR_ATTACKC,buf);
	gam_strcat(str,buf);
	GetCityName(co,buf);
	gam_strcat(str,buf);

	GamMsgBox(str,2);
 }*/


/******************************************************************************
 * 函数名:ShowPersonHead
 * 说  明:显示武将头像（占空间51*41）
 *
 * 入口参数：x,y -- 显示坐标，id -- 武将编号
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-5 10:38	基本功能完成
 ******************************************************************************/
FAR void ShowPersonHead(U8 x,U8 y, PersonID id)
{
    U8 tbuf[14];
    pushPaintColor(g_engineConfig.theme.personHeadColor);
    gam_drawpic(GEN_HEADPIC1 + g_PIdx, id, x + 13,y + 2, 1);
    popPaintColor();
    GetPersonName(id, tbuf);
    PlcMidShowStr(x + 26,y + 28,tbuf);
}

/******************************************************************************
 * 函数名:ShowGReport
 * 说  明:显示武将报告
 *
 * 入口参数：person -- 武将编号，str -- 报告内容
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-11 14:26	基本功能完成
 ******************************************************************************/
FAR void ShowGReport(PersonID person, U8 *str)
{
    RECT	big,small;

    if (g_engineConfig.disableAllPersonReport) {
        return;
    }

    gam_clrlcd(WK_SX + 4,WK_EY - 4 - 44 - 4,WK_EX - 5,WK_EY - 4);
    gam_rect(WK_SX + 4,WK_EY - 4 - 44 - 4,WK_EX - 5,WK_EY - 5);
    big.sx = WK_SX + 8;
    big.sy = WK_EY - 4 - 44 - 4;
    big.ex = WK_EX - 7;
    big.ey = WK_EY - 7;
    small.sx = WK_SX + 8 + 2;
    small.sy = WK_EY - 4 - 44 + 2;
    small.ex = WK_SX + 8 + 54;
    small.ey = WK_EY - 4 - 44 - 4 + 4 + 44;

    ShowPersonHead(WK_SX + 8,WK_EY - 4 - 44 - 4 + 4, person);
    PlcStrShowS(&big,&small,str);
    GamDelay(300, 2);
}

/******************************************************************************
 * 函数名:SetGoods
 * 说  明:设定指定道具为已发现道具
 *
 * 入口参数：指定道具
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 11:22	基本功能完成
 ******************************************************************************/
FAR void SetGoods(U8 city, ToolID goods)
{
    U32 qc;
    U32 i = g_Cities[city].ToolQueue;

    qc = i + g_Cities[city].Tools;
    for (;i < qc;i++)
    {
        if ((g_GoodsQueue[i] & 0x7fff) == goods && !(g_GoodsQueue[i] & 0x8000))
        {
            g_GoodsQueue[i] |= 0x8000;
            break;
        }
    }
}

/******************************************************************************
 * 函数名:SetGoodsByIndex
 * 说  明:设定指定道具为已发现道具
 *
 * 入口参数：指定道具队列索引
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 11:22	基本功能完成
 ******************************************************************************/
FAR void SetGoodsByIndex(ToolID index) {

    if (index == 0xffff) return;

    g_GoodsQueue[index] |= 0x8000;
}


/******************************************************************************
 * 函数名:GetCityPGoods
 * 说  明:取得城市已发现物品
 *
 * 入口参数：city -- 指定城池，gqueue -- 周围城池队列指针
 *
 * 出口参数：城池个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 11:36	基本功能完成
 ******************************************************************************/
FAR ToolID GetCityPGoods(U8 city,ToolID *gqueue)
{
    U32 i,j;
    U32 count;
    
    count = 0;
    j = g_Cities[city].ToolQueue;
    for (i = 0;i < g_Cities[city].Tools;i ++,j ++)
    {
        if (g_GoodsQueue[j] & 0x8000)
        {
            gqueue[count] = TID(g_GoodsQueue[j] & 0x7fff);
            count += 1;
        }
    }
    return TID(count);
}


/******************************************************************************
 * 函数名:GetCityDispGoods
 * 说  明:取得城市中未发现道具
 *
 * 入口参数：city -- 指定城池，gqueue -- 道具队列指针
 *
 * 出口参数：道具个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 11:38	基本功能完成
 ******************************************************************************/
FAR ToolID GetCityDispGoods(U8 city,ToolID *gqueue)
{
    U32 i,j;
    U32 count;
    
    count = 0;
    j = g_Cities[city].ToolQueue;
    for (i = 0;i < g_Cities[city].Tools;i ++,j ++)
    {
        if (!(g_GoodsQueue[j] & 0x8000))
        {
            gqueue[count] = g_GoodsQueue[j];
            count += 1;
        }
    }
    return TID(count);
}


/******************************************************************************
 * 函数名:GetOrderPersonsCount
 * 说  明:取得指定势力的武将数
 *
 * 入口参数：指定势力君主编号
 *
 * 出口参数：武将个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 16:15	基本功能完成
 ******************************************************************************/
FAR U32 GetPersonsCount(PersonID king)
{
    U8 i,j;
    U16 fpc;
    U8 *fp;
    OrderType *op;
    U32 count;
    
    count = 0;
    op = (OrderType *) ORDERQUEUE;
    for (i = 0;i < ORDER_MAX;i ++)
    {
        if (0xff == op[i].OrderId)
            continue;
        
        if (g_Cities[op[i].City].Belong != king + 1)
            continue;
        
        if (BATTLE == op[i].OrderId)
        {
            fpc = op[i].Person;
            fpc *= 10;
            fp = FIGHTERS +  fpc;
            for (j = 0;j < 10;j ++)
            {
                if (fp[j])
                    count += 1;
            }
        }
        else
        {
            count += 1;
        }
    }
    fp = SHARE_MEM;
    count += GetKingPersons(king,(PersonID*)fp);
    return(count);
}

/******************************************************************************
 * 函数名:GetDirect
 * 说  明:取得目标方向
 *
 * 入口参数：sc -- 源城市编号，oc -- 目的城市编号
 *
 * 出口参数：方向编号
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 17:19	基本功能完成
 ******************************************************************************/
FAR U8 GetDirect(U8 sc,U8 oc)
{
    U8 *clnk;
    U8 i;
    U16 clnkcount;
    
    clnk = ResLoadToCon(CITY_LINKR,1,g_CBnkPtr);
    clnkcount = sc;
    clnkcount *= 16;
    clnk += clnkcount;
    
    for (i = 0;i < 8;i ++)
    {
        if (clnk[i] == oc + 1)
            break;
    }
    return(i);
}


/******************************************************************************
 * 函数名:ReportCalamity
 * 说  明:报告城市灾难
 *
 * 入口参数：报告对象城市
 *
 * 出口参数：0xffff -- 退出，其他值 -- 操作值
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 23:49	基本功能完成
 ******************************************************************************/
FAR void ReportCalamity(U8 city)
{
    SBUF rstr,astr;
    U32 p;
    
    p = g_Cities[city].SatrapId;
    if (p && g_engineConfig.promptCityDisaster)
    {
        GetCityName(city,rstr);
        ResLoadToMem(STRING_CONST,STR_RP1,astr);
        gam_strcat(rstr,astr);
        GetCityState(city,astr);
        gam_strcat(rstr,astr);
        ResLoadToMem(STRING_CONST,STR_RP2,astr);
        gam_strcat(rstr,astr);
        ShowMapClear();
        ShowGReport(PID(p - 1),rstr);
    }
}

FAR void ShowDialog(PersonID commander, PersonID reporter,U8 *str)
{
    U32 belong = g_Persons[commander].Belong;
    if (belong && belong - 1 == g_PlayerKing) {
        ShowGReport(reporter, str);
    }
}

FAR void ShowDialogRandom(PersonID commander, PersonID reporter, U8 **str, U8 count)
{
    U8 rnd = gam_rand() % count;
    ShowDialog(commander, reporter, str[rnd]);
}
