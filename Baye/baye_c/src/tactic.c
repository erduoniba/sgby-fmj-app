/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	tactic.c
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
#undef	TACTIC_C
#define	TACTIC_C
#include "baye/enghead.h"
#include "touch.h"
#include "baye/script.h"

/*void ComputerTacticInterior(U8 city);
 void ComputerTacticDiplomatism(U8 city);
 void ComputerTacticArmament(U8 city);

 FAR U8 GetCitySet(CitySetType *pos);
 FAR U8 OrderMenu(void);
 FAR U8 ShowCityPro(U8 city);
 FAR U8 CityCommon(U8 city,U8 order);
 FAR U8 PersonUpDatadate(void);
 FAR U8 CitiesUpDataDate(void);
 FAR U8 RandEvents(void);
 FAR U8 EventStateDeal(void);
 FAR void PolicyExec(void);
 FAR U8 AddPerson(U8 city,U8 person);
 FAR U8 DelPerson(U8 city,U8 person);
 FAR U8 AddOrderHead(OrderType *Order);
 FAR U8 AddOrderEnd(OrderType *Order);
 FAR U8 GetCityPersons(U8 city,U8 *pqueue);
 FAR U8 GetCityCaptives(U8 city,U8 *pqueue);
 FAR U8 MenuComm(MenuType *menu);
 FAR U8 ShowPersonControl(U8 *person,U8 pcount,U8 x0,U8 y0,U8 x1,U8 y1);
 FAR U16 GetFood(U16 min,U16 max);
 FAR U16 GetMoney(U16 min,U16 max);
 FAR U16 GetArmy(U16 min,U16 max);
 FAR U8 SearchRoad(U8 sc,U8 xs,U8 ys,U8 ob,U8 xo,U8 yo);
 FAR void RevenueMoney(void);
 FAR void HarvestryFood(void);*/

/******************************************************************************
 * 函数名:PlayerTactic
 * 说  明:玩家策略控制
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-1 14:31	基本功能完成
 ******************************************************************************/
U8 PlayerTactic(void)
{
    U8 cset;
    U8 cral;

    while (1)
    {
        cset = GetCitySet(&g_CityPos);

        if (0xff == cset)
        {
            return(1);
        }
        else
        {
            if (g_Cities[cset].Belong != (g_PlayerKing + 1))
            {
                if (g_Cities[cset].Belong)
                {
                    ShowConstStrMsg(NOTE_STR3);
                }
                else
                {
                    ShowConstStrMsg(NOTE_STR5);
                }
                continue;
            }

            cral = OrderMenu();
            switch (cral)
            {
                case 0xff:
                    break;
                case 0xfe:
                    ShowCityPro(cset);
                    break;
                default:
                    CityCommon(cset,cral);
                    break;
            }
        }
    }
    return(0);
}

/******************************************************************************
 * 函数名:ComputerTactic
 * 说  明:产生电脑策略
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-17 15:37	基本功能完成
 ******************************************************************************/
/*君主产生“内政、协调”、“外交”、“军备”策略几率*/
/*const U8 KingTacticOddsIH[5] = {50,40,30,20,10};
 const U8 KingTacticOddsD[5] = {80,70,70,40,20};*/

void ComputerTactic(void)
{
    U32 i;
    U32 rnd;
    U8 TacticOddsIH[6],TacticOddsD[6];
    CityType *cptr;
    PersonID b;

    ResItemGetN(IFACE_CONID,KingTacticOddsIH,TacticOddsIH, sizeof(TacticOddsIH));
    ResItemGetN(IFACE_CONID,KingTacticOddsD,TacticOddsD, sizeof(TacticOddsD));

    if (g_engineConfig.aiLevelUpSpeed) {
        for (i = 0; i < PERSON_COUNT; i++)
        {
            rnd = gam_rand() % 100;
            if (rnd < g_engineConfig.aiLevelUpSpeed) {
                if (g_Persons[i].Belong != g_PlayerKing+1)
                {
                    LevelUp(&g_Persons[i]);
                }
            }
        }
    }

    for (i = 0;i < CITY_MAX;i ++)
    {
        cptr = &g_Cities[i];
        cptr->State = STATE_NORMAL;
        b = cptr->Belong;
        if (!(b))
            continue;
        if (b == (g_PlayerKing + 1))
            continue;

        cptr->AvoidCalamity += 1;
        if (cptr->AvoidCalamity > 100)
            cptr->AvoidCalamity = 100;

        if (cptr->Food < 100)
            cptr->Food = 500;
        if (cptr->Money > 10000)
            cptr->Money /= 2;
        /*添加电脑策略代码*/
        rnd = gam_rand() % 100;
        if (TacticOddsIH[g_Persons[b - 1].Character] > rnd)			/*执行内政、协调策略*/
        {
            ComputerTacticInterior(i);
            ComputerTacticHarmonize(i);
        }
        else if (TacticOddsD[g_Persons[b - 1].Character] > rnd)		/*执行外交策略*/
        {
            ComputerTacticDiplomatism(i);
        }
        else										/*执行军备策略*/
        {
            ComputerTacticArmament(i);
        }
    }
}

/******************************************************************************
 * 函数名:ComputerTacticInterior
 * 说  明:产生城市内政电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:32	基本功能完成
 ******************************************************************************/
void ComputerTacticInterior(U8 city)
{
    PersonID *pqptr;
    U32 i,rnd;
    U32 pcount;
    OrderType order;
    U16 *c,cl,ca;
    U16 *f,fl,fa;
    U8 *PeopleDevotion;
    U32 *Population;
    CityType *cptr;

    cptr = &g_Cities[city];
    f = &cptr->Farming;
    fl = cptr->FarmingLimit;

    c = &cptr->Commerce;
    cl = cptr->CommerceLimit;

    PeopleDevotion = &cptr->PeopleDevotion;
    Population = &cptr->Population;

    pqptr = (PersonID*)SHARE_MEM;
    pcount = GetCityPersons(city,pqptr);
    for (i = 0;i < pcount;i ++)
    {
        rnd = gam_rand() % 5;
        switch (rnd)
        {
            case 0:		/*开垦*/
                fa = 200;
                *f += fa;
                if (*f > fl)
                {
                    *f = fl;
                }
                order.OrderId = ASSART;
                break;
            case 1:		/*招商*/
                ca = 200;
                *c += ca;
                if (*c > cl)
                {
                    *c = cl;
                }
                order.OrderId = ACCRACTBUSINESS;
                break;
            case 2:		/*搜寻*/
                order.OrderId = SEARCH;
                break;
            case 3:		/*出巡*/
                *PeopleDevotion += 4;
                if (*PeopleDevotion > 100)
                    *PeopleDevotion = 100;
                *Population += 100;
                if (*Population > cptr->PopulationLimit)
                    *Population = cptr->PopulationLimit;

                order.OrderId = INSPECTION;
                break;
            case 4:
                continue;
                break;
        }

        order.Person = pqptr[i];
        order.City = city;
        order.Object = PID(city);
        order.TimeCount = 0;
        if (AddOrderHead(&order)) {
            DelPerson(city,pqptr[i]);
        }
    }
}

/******************************************************************************
 * 函数名:ComputerTacticDiplomatism
 * 说  明:产生城市外交电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:34	基本功能完成
 ******************************************************************************/
void ComputerTacticDiplomatism(U8 city)
{
    PersonID *pqptr,*eqptr;
    U32 i,rnd;
    U32 pcount,ecount;
    OrderType order;

    pqptr = (PersonID*)SHARE_MEM;
    eqptr = pqptr + PERSON_COUNT;
    pcount = GetCityPersons(city,pqptr);
    for (i = 0;i < pcount;i ++)
    {
        rnd = gam_rand() % 8;
        ecount = GetCityCaptives(city,eqptr);
        if (ecount)
        {
            rnd = 0;
        }
        switch (rnd)
        {
            case 0:		/*招降*/
                /*ecount = GetCityCaptives(city,eqptr);*/
                if (ecount)
                {
                    rnd = gam_rand() % ecount;
                    g_Persons[eqptr[rnd]].Belong = g_Cities[city].Belong;
                }
                continue;
            case 1:		/*处斩*/
                /*ecount = GetCityCaptives(city,eqptr);*/
                if (ecount)
                {
                    rnd = gam_rand() % ecount;
                    DelPerson(city,eqptr[rnd]);
                }

                continue;
                break;
            case 2:		/*流放*/
                /*ecount = GetCityCaptives(city,eqptr);
                 if (ecount)
                 {
                 rnd = gam_rand() % ecount;
                 g_Persons[eqptr[rnd]].Belong = 0;
                 g_Persons[eqptr[rnd]].Arms = 0;
                 }
                 order.OrderId = BANISH;*/
                continue;
                break;
            case 3:		/*离间*/
                ecount = GetEnemyPersons(g_Cities[city].Belong,eqptr);
                if (ecount)
                {
                    rnd = gam_rand() % ecount;
                    order.Object = eqptr[rnd];
                }
                else
                {
                    continue;
                }
                order.OrderId = ALIENATE;
                break;
            case 4:		/*招揽*/
                ecount = GetEnemyPersons(g_Cities[city].Belong,eqptr);
                if (ecount)
                {
                    rnd = gam_rand() % ecount;
                    order.Object = eqptr[rnd];
                }
                else
                {
                    continue;
                }
                order.OrderId = CANVASS;
                break;
            case 5:		/*策反*/
                ecount = GetEnemySatraps(g_Cities[city].Belong,eqptr);
                if (ecount)
                {
                    rnd = gam_rand() % ecount;
                    order.Object = eqptr[rnd];
                }
                else
                {
                    continue;
                }
                order.OrderId = COUNTERESPIONAGE;
                break;
            case 6:		/*劝降*/
                ecount = GetEnemyKing(g_Cities[city].Belong,eqptr);
                if (ecount)
                {
                    rnd = gam_rand() % ecount;
                    order.Object = eqptr[rnd];
                }
                else
                {
                    continue;
                }
                order.OrderId = INDUCE;
                break;
            case 7:
                continue;
                break;
        }

        order.Person = pqptr[i];
        order.City = city;
        order.TimeCount = 0;
        if (AddOrderHead(&order)) {
            DelPerson(city,pqptr[i]);
        }
    }
}

/******************************************************************************
 * 函数名:ComputerTacticHarmonize
 * 说  明:产生城市协调电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:34	基本功能完成
 ******************************************************************************/
void ComputerTacticHarmonize(U8 city)
{
    U8 *cqptr;
    PersonID *pqptr,*eqptr;
    U32 i,j,c,rnd;
    U32 pcount,ccount,ecount;
    OrderType order;
    CityType *cptr;

    cptr = &g_Cities[city];
    pqptr = (PersonID*)SHARE_MEM;
    eqptr = pqptr + PERSON_COUNT;
    cqptr = (U8*)(eqptr + PERSON_COUNT);
    pcount = GetCityPersons(city,pqptr);
    for (i = 0;i < pcount;i ++)
    {
        rnd = gam_rand() % 7;
        switch (rnd)
        {
            case 0:		/*治理*/
                cptr->State = STATE_NORMAL;
                cptr->AvoidCalamity += 4;
                if (cptr->AvoidCalamity > 100)
                    cptr->AvoidCalamity = 100;
                order.OrderId = FATHER;
                break;
            case 1:		/*赏赐*/
                continue;
                break;
            case 2:		/*没收*/
                continue;
                break;
            case 3:		/*交易*/
                /*order.OrderId = EXCHANGE;*/
                continue;
                break;
            case 4:		/*输送*/
                /*order.OrderId = TRANSPORTATION;*/
                continue;
                break;
            case 5:		/*移动*/
                if (i < 3)
                {
                    continue;
                }
                ccount = GetKingCitys(PID(cptr->Belong - 1),cqptr);
                if (ccount < 2)
                {
                    continue;
                }
                for (c = 0,j = 0;j < ccount;j ++)
                {
                    ecount = GetRoundEnemyCity(cqptr[j], NULL);
                    if (ecount)
                    {
                        c = j;
                        rnd = gam_rand() % 2;
                        if (rnd)
                        {
                            break;
                        }
                    }
                }
                order.Object = PID(cqptr[c]);
                order.OrderId = MOVE;
                break;
            case 6:
                continue;
                break;
        }

        order.Person = pqptr[i];
        order.City = city;
        order.TimeCount = 0;
        if (AddOrderHead(&order)) {
            DelPerson(city,pqptr[i]);
        }
    }
}

/******************************************************************************
 * 函数名:ComputerTacticArmament
 * 说  明:产生城市军备电脑策略
 *
 * 入口参数：要产生电脑策略的城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-20 15:35	基本功能完成
 ******************************************************************************/
void ComputerTacticArmament(U8 city)
{
    U8 *cqptr;
    PersonID *pqptr, fp[10];
    U32 i,j,rnd;
    U32 pcount,fpcount,fcount;
    OrderType order;
    PersonType *pptr;

    pqptr = (PersonID*)SHARE_MEM;
    cqptr = (U8*)(pqptr + PERSON_COUNT);
    pcount = GetCityPersons(city,pqptr);
    if (!pcount)
        return;
    
    PersonID pind = pqptr[gam_rand() % pcount];
    pptr = &g_Persons[pind];

    if (g_engineConfig.aiLevelUpSpeed == 0 && !(g_MonthDate % 3))
    {
        LevelUp(pptr);
    }
    for (i = 0;i < pcount;i ++)
    {
        rnd = gam_rand() % 9;
        switch (rnd)
        {
            case 0:
                order.OrderId = RECONNOITRE;
                continue;
                break;
            case 1:		/*征兵*/
            case 2:		/*分配*/
            case 3:		/*征兵*/
            case 4:		/*分配*/
            case 5:		/*分配*/
                pptr->Arms = PlcArmsMax(pind);
                /*g_Cities[city].MothballArms += ((U16) 20) * g_Cities[city].PeopleDevotion;*/

                order.OrderId = CONSCRIPTION;
                order.Arms = 0;
                continue;
                break;
                /*case 2:		分配*/
                /*armys = g_Persons[pqptr[i]].Level;
                 armys *= 10;
                 armys += g_Persons[pqptr[i]].Force + g_Persons[pqptr[i]].IQ;
                 armys *= 10;
                 g_Cities[city].MothballArms = g_Cities[city].MothballArms + g_Persons[pqptr[i]].Arms - armys;
                 g_Persons[pqptr[i]].Arms = armys;
                 order.OrderId = DISTRIBUTE;
                 continue;
                 break;*/
            case 6:		/*掠夺*/
                order.OrderId = DEPREDATE;
                continue;
                break;
            case 7:		/*出征*/
            {
                PersonID* pqptr = ((PersonID*)SHARE_MEM) + PERSON_MAX;

                if (i >= 1)
                    continue;

                fcount = GetRoundEnemyCity(city,cqptr);
                if (!fcount)
                {
                    continue;
                }
                rnd = gam_rand() % (fcount * 2);
                if (rnd >= fcount)
                    continue;
                /*rnd = gam_rand() % fcount;
                 order.Object = cqptr[rnd];*/
                order.Object = PID(GetWeekCity(fcount,cqptr));
                fpcount = GetCityPersons(city,pqptr);
                if (fpcount)
                {
                    PersonID t;

                    fcount = 0;
                    for (fcount = 0;fcount < fpcount - 1;fcount ++)
                    {
                        for (j = fcount + 1;j < fpcount;j ++)
                        {
                            /*添加武将兵力排序*/
                            if (g_Persons[pqptr[fcount]].Arms < g_Persons[pqptr[j]].Arms)
                            {
                                t = pqptr[j];
                                pqptr[j] = pqptr[fcount];
                                pqptr[fcount] = t;
                            }
                            if (!g_Persons[pqptr[pcount - 1]].Arms)
                                fpcount -= 1;
                        }
                    }

                    if (g_Persons[pqptr[0]].Arms < 1000)
                    {
                        continue;
                    }

                    if (fpcount < 4)
                    {
                        continue;
                    }
                    else
                    {
                        /*添加出征命今生成代码*/
                        order.OrderId = BATTLE;
                        gam_memset(fp,0,10*sizeof(PersonID));
                        fpcount -= 1;
                        if (fpcount > 10)
                        {
                            fpcount = 10;
                        }
                        for (j = 0;j < fpcount;j ++)
                        {
                            fp[j] = PID(pqptr[j] + 1);
                        }
                        /*order.Person = fpcount;*/
                        order.City = city;
                        order.Food = g_Cities[city].Food;
                        order.TimeCount = 0;
                        if (AddFightOrder(&order,fp)) {
                            for (j = 0;j < fpcount;j ++)
                            {
                                DelPerson(city,pqptr[j]);
                            }
                        }
                        return;
                    }
                }
                continue;
                break;
            }
            case 8:
                continue;
                break;
        }

        order.Person = pqptr[i];
        order.City = city;
        order.TimeCount = 0;
        if (AddOrderHead(&order)) {
            DelPerson(city,pqptr[i]);
        }
    }
}

/******************************************************************************
 * 函数名:ConditionUpdate
 * 说  明:环境更新
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
void ConditionUpdate(void)
{
    g_MonthDate += 1;		/*时间月累加*/
    if (g_MonthDate > 12)
    {
        g_YearDate += 1;		/*时间年累加*/
        g_MonthDate = 1;
        GoodsUpDatadate();
        PersonUpDatadate();
    }
    CitiesUpDataDate();
    EventStateDeal();
    RandEvents();
}

/******************************************************************************
 * 函数名:GameDevDrv
 * 说  明:游戏发展驱动
 *
 * 入口参数：无
 *
 * 出口参数：游戏命令
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-1 11:43	基本功能完成
 ******************************************************************************/
FAR U8 GameDevDrv(void)
{
    U8 ral;
    U8 skipUpdateSatrap = g_FromSave;

    while (1)
    {
        if (IsLoss())
        {
            /*提示玩家全军覆没*/
            ShowConstStrMsg(STR_GAMELOST);
            return(1);
        }
        if (IsWin())
        {
            /*提示玩家大获全胜*/
            ShowConstStrMsg(STR_GAMEWON);
            return(0);
        }

        if (skipUpdateSatrap) {
            skipUpdateSatrap = 0;
        } else {
            SetCitySatrap();
            call_hook_a("tacticStage1", NULL);
        }
        /*玩家策略*/
        do
        {
            if (call_hook_a("tacticStageUser", NULL) == -1)
                PlayerTactic();
            ral = FunctionMenu();
            if (!ral)
            {
                continue;
            }
            else if (1 == ral)
            {
                break;
            }
            else if (2 == ral)
            {
                return(2);
            }
        } while(1);

        call_hook_a("tacticStage2", NULL);
        /*提示电脑策略中*/
        ShowTacticNote();
        /*电脑策略*/
        ComputerTactic();
        call_hook_a("tacticStage3", NULL);

        /*命令队列执行*/
        PolicyExec();
        call_hook_a("tacticStage4", NULL);

        /*环境更新*/
        ConditionUpdate();
        call_hook_a("tacticStage5", NULL);
    }
}



/******************************************************************************
 * 函数名:IsWin
 * 说  明:检测玩家是否取胜
 *
 * 入口参数：无
 *
 * 出口参数：0 -- 否，1 -- 是
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-30 0:35	基本功能完成
 ******************************************************************************/
U8 IsWin(void)
{
    PersonID *ptr;
    ptr = (PersonID*)SHARE_MEM;
    if (GetEnemyKing(g_PlayerKing,ptr))
        return(0);
    else
        return(1);
}

/******************************************************************************
 * 函数名:IsLoss
 * 说  明:检测玩家是否失败
 *
 * 入口参数：无
 *
 * 出口参数：0 -- 否，1 -- 是
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-30 0:44	基本功能完成
 ******************************************************************************/
U8 IsLoss(void)
{
    U8 *ptr;

    ptr = SHARE_MEM;
    if (GetKingCitys(g_PlayerKing,ptr))
        return(0);
    else
        return(1);
}


/******************************************************************************
 * 函数名:SetCitySatrap
 * 说  明:设置城中最高智力者为太守
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-30 0:47	基本功能完成
 ******************************************************************************/
void SetCitySatrap(void)
{
    U32 c;
    U32 i,pq,ps;
    U32 k,sp;

    for (c = 0;c < CITY_MAX;c ++)
    {
        k = g_Cities[c].Belong;
        if (k)
        {
            sp = 0;
            pq = g_Cities[c].PersonQueue;
            for (i = 0;i < g_Cities[c].Persons;i ++)
            {
                ps = g_PersonsQueue[pq + i];
                if (g_Persons[ps].Belong == k)
                {
                    if (k == ps + 1)
                    {
                        sp = ps + 1;
                        break;
                    }

                    if (sp)
                    {
                        if (g_Persons[sp - 1].IQ < g_Persons[ps].IQ)
                            sp = ps + 1;
                    }
                    else
                    {
                        sp = ps + 1;
                    }
                }
            }
            g_Cities[c].SatrapId = sp;
        }
        else
            g_Cities[c].SatrapId = PID0;
    }
}

/******************************************************************************
 * 函数名:GetAllKings
 * 说  明:取得现时期君主
 *
 * 入口参数：kings -- 君主队列指针
 *
 * 出口参数：君主个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 19:28	基本功能完成
 ******************************************************************************/
U32 GetAllKings(PersonID *kings)
{
    U8 c,i,count;
    U32 b;

    count = 0;
    for (c = 0;c < CITY_MAX;c ++)
    {
        b = g_Cities[c].Belong;
        if (b)
        {
            for (i = 0;i < count;i ++)
            {
                if (kings[i] == b - 1)
                    break;
            }
            if (i < count)
            {
                continue;
            }
            kings[count] = b - 1;
            count += 1;
        }
    }
    return(count);
}


/******************************************************************************
 * 函数名:LoadPeriod
 * 说  明:加载特定时期资料
 *
 * 入口参数：period -- 指定时期
 *
 * 出口参数：君主个数
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-29 23:01	基本功能完成
 ******************************************************************************/
static U8* loadCity(CityType*city, U8*raw)
{
    gam_memcpy(&city->State, raw, sizeof(city->State));
    raw += sizeof(city->State);
    gam_memcpy(&city->Belong, raw, sizeof(city->Belong));
    raw += sizeof(city->Belong);
    gam_memcpy(&city->SatrapId, raw, sizeof(city->SatrapId));
    raw += sizeof(city->SatrapId);
    gam_memcpy(&city->FarmingLimit, raw, sizeof(city->FarmingLimit));
    raw += sizeof(city->FarmingLimit);
    gam_memcpy(&city->Farming, raw, sizeof(city->Farming));
    raw += sizeof(city->Farming);
    gam_memcpy(&city->CommerceLimit, raw, sizeof(city->CommerceLimit));
    raw += sizeof(city->CommerceLimit);
    gam_memcpy(&city->Commerce, raw, sizeof(city->Commerce));
    raw += sizeof(city->Commerce);
    gam_memcpy(&city->PeopleDevotion, raw, sizeof(city->PeopleDevotion));
    raw += sizeof(city->PeopleDevotion);
    gam_memcpy(&city->AvoidCalamity, raw, sizeof(city->AvoidCalamity));
    raw += sizeof(city->AvoidCalamity);
    gam_memcpy(&city->PopulationLimit, raw, sizeof(city->PopulationLimit));
    raw += sizeof(city->PopulationLimit);
    gam_memcpy(&city->Population, raw, sizeof(city->Population));
    raw += sizeof(city->Population);
    gam_memcpy(&city->Money, raw, sizeof(city->Money));
    raw += sizeof(city->Money);
    gam_memcpy(&city->Food, raw, sizeof(city->Food));
    raw += sizeof(city->Food);
    gam_memcpy(&city->MothballArms, raw, sizeof(city->MothballArms));
    raw += sizeof(city->MothballArms);
    gam_memcpy(&city->PersonQueue, raw, sizeof(city->PersonQueue));
    raw += sizeof(city->PersonQueue);
    gam_memcpy(&city->Persons, raw, sizeof(city->Persons));
    raw += sizeof(city->Persons);
    gam_memcpy(&city->ToolQueue, raw, sizeof(city->ToolQueue));
    raw += sizeof(city->ToolQueue);
    gam_memcpy(&city->Tools, raw, sizeof(city->Tools));
    raw += sizeof(city->Tools);
    return raw;
}

U8* loadCities(CityType*cities, U8*raw)
{
    int i;
    for (i = 0; i < CITY_MAX; i++) {
        raw = loadCity(cities+i, raw);
    }
    return raw;
}

U8* loadPerson(PersonType*person, U8*raw)
{
    gam_memcpy(&person->OldBelong, raw, sizeof(person->OldBelong));
    raw += sizeof(person->OldBelong);
    gam_memcpy(&person->Belong, raw, sizeof(person->Belong));
    raw += sizeof(person->Belong);
    gam_memcpy(&person->Level, raw, sizeof(person->Level));
    raw += sizeof(person->Level);
    gam_memcpy(&person->Force, raw, sizeof(person->Force));
    raw += sizeof(person->Force);
    gam_memcpy(&person->IQ, raw, sizeof(person->IQ));
    raw += sizeof(person->IQ);
    gam_memcpy(&person->Devotion, raw, sizeof(person->Devotion));
    raw += sizeof(person->Devotion);
    gam_memcpy(&person->Character, raw, sizeof(person->Character));
    raw += sizeof(person->Character);
    gam_memcpy(&person->Experience, raw, sizeof(person->Experience));
    raw += sizeof(person->Experience);
    gam_memcpy(&person->Thew, raw, sizeof(person->Thew));
    raw += sizeof(person->Thew);
    gam_memcpy(&person->ArmsType, raw, sizeof(person->ArmsType));
    raw += sizeof(person->ArmsType);
    gam_memcpy(&person->Arms, raw, sizeof(person->Arms));
    raw += sizeof(person->Arms);
    gam_memcpy(&person->Equip, raw, sizeof(person->Equip));
    raw += sizeof(person->Equip);
    gam_memcpy(&person->Age, raw, sizeof(person->Age));
    raw += sizeof(person->Age);
    return raw;
}

U8* loadPersons(PersonType*person, U8*raw, U16 length)
{
    int i;
    U16 pcount = length/sizeof(PersonType);
    for (i = 0; i < pcount; i++) {
        raw = loadPerson(person+i, raw);
    }
    return raw;
}

void LoadPeriod(U8 period)
{
    U32 i;
    U8 *ptr;
    
    ptr = ResLoadToCon(CITY_RESID,period,g_CBnkPtr);
    ptr = loadCities(g_Cities, ptr);
    g_YearDate = *((U16 *) ptr);
    
    U32 length = ResGetItemLen(GENERAL_RESID, period);
    U16 pcount = length / sizeof(PersonType);
    GamSetPersonCount(pcount);
    ptr = ResLoadToCon(GENERAL_RESID,period,g_CBnkPtr);
    loadPersons(g_Persons, ptr, length);
    
    ResItemGetN(GENERAL_QUEUE, period, (U8*)g_PersonsQueue, sizeof(PersonID)*pcount);
    
    ResItemGetN(GOODS_QUEUE, period, (U8*)g_GoodsQueue, GOODS_MAX);
    
    gam_memset(FIGHTERS_IDX,0,FIGHT_ORDER_MAX);
    gam_memset((U8 *)ORDERQUEUE,0xff,(U16)ORDER_MAX * sizeof(OrderType));
    
    g_PIdx = period;
    
    for (i = 0;i < CITY_MAX;i ++)
        g_Cities[i].State = 0;
    for (i = 0;i < PERSON_COUNT;i ++)
    {
        g_Persons[i].Thew = 100;
        g_Persons[i].Arms = 100;
        g_Persons[i].OldBelong = PID0;
    }
    
    g_MonthDate = 1;
}


/******************************************************************************
 * 函数名:FunctionMenu
 * 说  明:功能菜单
 *
 * 入口参数：无
 *
 * 出口参数：
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-13 15:33	基本功能完成
 ******************************************************************************/
U8 FunctionMenu(void)
{
    /*U8 mstr[30];
     MenuType menu;
     
     menu.top = 0;
     menu.set = 0;
     menu.count = 3;
     menu.sx = WK_SX + WK_EX / 3;
     menu.sy = WK_SY + 10;
     menu.ex = WK_SX + WK_EX / 3 + ASC_WID * 8;
     menu.ey = WK_EY - 10;
     ResLoadToMem(STRING_CONST,FUNC_TEND,mstr);
     ResLoadToMem(STRING_CONST,FUNC_SAVE,&mstr[10]);
     ResLoadToMem(STRING_CONST,FUNC_QUIT,&mstr[20]);
     menu.menustr[0] = mstr;
     menu.menustr[1] = &mstr[10];
     menu.menustr[2] = &mstr[20];
     
     switch (MenuComm(&menu))
     {
     case 0:
     return(1);
     case 1:
     GamRecordMan(0);
     return(0);
     case 2:
     return(2);
     case 0xff:
     return(0);
     }*/
    
    RECT pRect, pRectSubMenu;
    U8 mstr[30];
    ResLoadToMem(STRING_CONST,MENU_FUNCSTR,mstr);
    pRect.sx = WK_SX + WK_EX / 3;
    pRect.ex = WK_SX + WK_EX / 3 + ASC_WID * 8;
    pRect.sy = WK_SY + 10;
    if ((WK_EY - (WK_SY + 10)) > (ASC_HGT * 3 + 4)) {
        pRect.ey = WK_SY + 10 + ASC_HGT * 3;
    } else {
        pRect.ey = WK_SY + 10 + ((WK_EY - (WK_SY + 10)) / ASC_HGT) * ASC_HGT;
    }

    pRectSubMenu.sx = pRect.sx;
    pRectSubMenu.sy = pRect.ey + HZ_HGT;
    pRectSubMenu.ex = pRectSubMenu.sx + HZ_WID*4;
    pRectSubMenu.ey = pRectSubMenu.sy + HZ_HGT;
    const char* exitStr = "\xc8\xb7\xb6\xa8\xcd\xcb\xb3\xf6"; //确定退出
    U8 choosing = 0;

    while (1) {
        U8 hooked = 0;
        IF_HAS_HOOK("mainSystemMenu") {
            choosing = (U8)CALL_HOOK_A();
            hooked = 1;
        } else {
            choosing = (U8)PlcSplMenu(&pRect, choosing, mstr);
        }

        switch (choosing) {
            case 0:
                return(1);
            case 1:
                GamRecordMan(0);
                return(0);
            case 2:
                if (!hooked && ((U8)PlcSplMenu(&pRectSubMenu, 0, (U8*)exitStr)) == MNU_EXIT) {
                    ShowMapClear();
                    continue;
                }
                return(2);
            case 0xff:
                return(0);
        }
    }
    return 0;
}

/******************************************************************************
 * 函数名:NumOperate
 * 说  明:数字操作
 *
 * 入口参数：min -- 最小值限制，max -- 最大值限制
 *
 * 出口参数：0xffff -- 退出，其他值 -- 操作值
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-23 11:43	基本功能完成
 ******************************************************************************/
FAR U32 NumOperateInner(U32 min,U32 max, U32 donum);
FAR U32 NumOperate(U32 min,U32 max, U32 donum) {
    int prev = SysScrollingTimerOpen(0);
    U32 rv = NumOperateInner(min, max,  donum);
    SysScrollingTimerOpen(prev);
    return rv;
}
FAR U32 NumOperateInner(U32 min,U32 max, U32 donum)
{
    U8 str[32];
    U8 showflag,i,bit,maxbit;
    U32 tnum,num;
    GMType Msg;

    Touch touch = {0};
    U32 valueWhenTouchDown = 0;

    if (g_engineDebug) {
        max = 10000;
    }

    U8 left = (WK_SX + WK_EX - WK_SX  - ASC_WID * 12) / 2;
    U8 top = (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT;

    U8 btnTop = top + ASC_HGT*2 + 3;
    U8 btnW = 30;
    U8 btnH = ASC_HGT + 2;


    Rect okButton = MakeRect(left - 10, btnTop, btnW, btnH);
    Rect maxButton = MakeRect(okButton.right+2, btnTop, btnW, btnH);
    Rect cancelButton = MakeRect(maxButton.right+2, btnTop, btnW, btnH);

    touchDrawButton(okButton, "\xc8\xb7\xb6\xa8");
    touchDrawButton(maxButton, "\xd7\xee\xb4\xf3");
    touchDrawButton(cancelButton, "\xc8\xa1\xcf\xfb");

    num = 1;
    
    ResLoadToMem(STRING_CONST,ATRR_STR63,str);
    GamStrShowS(left, top, str);
    ResLoadToMem(STRING_CONST,ATRR_STR64,str);
    gam_ltoa(max,&str[7],10);
    GamStrShowS(left, top + ASC_HGT, str);

    maxbit = gam_strlen(&str[7]) - 1;
    bit = maxbit;

    U8 mini_startX = (WK_SX + WK_EX - WK_SX  - ASC_WID * 12) / 2 + ASC_WID * 7;
    U8 mini_startY = (WK_SY + WK_EY - WK_SY - ASC_HGT * 3) / 2 + ASC_HGT;
    U8 enlarged_col_width = SCR_WID / (maxbit + 1);
    U8 enlarged_startY = mini_startY-1;
    static U8 config_show_enlarged = 0;
    showflag = config_show_enlarged ? 2 : 1;
    
    gam_savscr();

    while (1)
    {
        if (showflag)
        {
            U8 show_enlarged = config_show_enlarged && maxbit > 0;
            tnum = donum;
            if (showflag == 2) {
                if (show_enlarged) {
                    gam_clrlcd(0, enlarged_startY, SCR_WID, enlarged_startY+ASC_HGT+2);
                } else {
                    gam_restorescr();
                }
            }

            for (i = maxbit;(U8)(i + 1) >= 1;i --) {
                if (show_enlarged) {
                    // enlarged version
                    gam_rect(enlarged_col_width*i, enlarged_startY, enlarged_col_width*(i+1), enlarged_startY+ASC_HGT+2);
                    U8 x = enlarged_col_width * i + enlarged_col_width / 2 - ASC_WID/2;
                    GamAsciiS(x, enlarged_startY+1, (tnum % 10) + '0');

                } else {
                    GamAsciiS(mini_startX + ASC_WID * i, mini_startY, (tnum % 10) + '0');
                }
                tnum /= 10;
            }

            if (!show_enlarged) {
                gam_revlcd(mini_startX + ASC_WID * bit, mini_startY, mini_startX + ASC_WID * bit + ASC_WID - 1, mini_startY + ASC_HGT - 1);
            }
            showflag = 0;
        }
        
        GamGetMsg(&Msg);
        if (VM_CHAR_FUN == Msg.type)
        {
            switch (Msg.param)
            {
                case VK_UP:
                    if (donum + num <= max)
                    {
                        donum += num;
                        showflag = 1;
                    }
                    break;
                case VK_DOWN:
                    if (donum >= num && donum - num >= min)
                    {
                        donum -= num;
                        showflag = 1;
                    }
                    break;
                case VK_LEFT:
                    if (bit)
                    {
                        bit -= 1;
                        num *= 10;
                        showflag = 1;
                    }
                    break;
                case VK_RIGHT:
                    if (bit < maxbit)
                    {
                        bit += 1;
                        num /= 10;
                        showflag = 1;
                    }
                    break;
                case VK_ENTER:
                    return(donum);
                    break;
                case VK_HELP:
                    if (donum != max) {
                        donum = max;
                    } else {
                        donum = min;
                    }
                    showflag = 1;
                    break;
                case VK_EXIT:
                    return(0xffffffff);
            }
        }
        else if (VM_TOUCH == Msg.type) {
            touchUpdate(&touch, Msg);
            switch (Msg.param) {
                case VT_TOUCH_DOWN:
                    valueWhenTouchDown = donum;
                    break;
                case VT_TOUCH_UP:
                {
                    if (!touch.completed || touch.moved) break;
                    I16 x = touch.currentX, y = touch.currentY;
                    if (touchIsPointInRect(x, y, okButton)) {
                        return donum;
                    }
                    if (touchIsPointInRect(x, y, cancelButton)) {
                        return 0xffff;
                    }
                    if (touchIsPointInRect(x, y, maxButton)) {
                        donum = donum == max ? min : max;
                        showflag = 1;
                        break;
                    }
                    // else
                    showflag = 2;
                    config_show_enlarged = !config_show_enlarged;
                    break;
                }
                case VT_TOUCH_MOVE:
                {
                    I32 factors[] = {1, 10, 100, 1000, 10000, 100000};
                    I16 dy = touch.currentY - touch.startY;
                    I16 dunit = - dy / 10;
                    U16 factor = SCR_WID - limitValueInRange(touch.startX, 0, SCR_WID-1) - 1;
                    U16 ind = (maxbit+1) * factor / SCR_WID;
                    I32 dvalue = dunit * factors[ind];
                    donum = limitValueInRange(valueWhenTouchDown + dvalue, min, max);
                    showflag = 1;
                    break;
                }
            }
        }
    }
}

/******************************************************************************
 * 函数名:ShowTacticNote
 * 说  明:提示电脑策略中
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-19 23:49	基本功能完成
 ******************************************************************************/
void ShowTacticNote(void)
{
    U8 str[512], astr[512];
    PersonID *kq;
    U32 i,kc;
    
    ShowMapClear();
    kq = (PersonID*)SHARE_MEM;
    kc = GetEnemyKing(g_PlayerKing,kq);
    for (i = 0;i < kc;i ++)
    {
        GetPersonName(kq[i],str);
        ResLoadToMem(STRING_CONST,TACTIC_STR,astr);
        gam_strcat(str,astr);
        gam_clrlcd(WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2,WK_EY - 2 - ASC_HGT - 2,WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2 + ASC_WID * 19 + 4,WK_EY - 2);
        gam_rect(WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2,WK_EY - 2 - ASC_HGT - 2,WK_SX + WK_EX / 2 - (WK_SX + ASC_WID * 19 + 4) / 2 + ASC_WID * 19 + 4,WK_EY - 2);
        GamStrShowS(WK_SX + (WK_EX - (WK_SX + ASC_WID * 19)) / 2,WK_EY - 2 - ASC_HGT,str);
        GamDelay(5,false);
    }
    ShowMapClear();
}

U8 GetArmType(PersonType* p)
{
    U8 armType = p->ArmsType;

    for (int i = 0; i < 2; i++) {
        ToolID tid = p->Equip[i];
        if (tid) {
            GOODS *tool = (GOODS *)(ResLoadToCon(GOODS_RESID, 1, g_CBnkPtr) + (U16)(tid - 1) * sizeof(GOODS));
            if (tool == NULL) continue;
            switch (tool->arm)
            {
                case 0:		/*无*/
                    break;
                case 1:		/*水兵*/
                    armType = ARM_SHUIJUN;
                    break;
                case 2:		/*玄兵*/
                    armType = ARM_XUANBING;
                    break;
                case 3:		/*极兵*/
                    armType = ARM_JIBING;
                    break;
                default:
                    armType = tool->arm - 4;
                    break;
            }
        }
    }
    return armType;
}

void LevelUp(PersonType*p)
{
    p->Level++;
    if (p->Level > MAX_LEVEL)
        p->Level = MAX_LEVEL;
}
