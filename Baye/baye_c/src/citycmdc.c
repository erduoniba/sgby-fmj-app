/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	citycmdc.c
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
 *	1.0    2005-7-12 11:17  陈泽伟     基本的功能完成
 ***********************************************************************/

#include "baye/stdsys.h"
#include "baye/comm.h"
#undef	CITYCMDC_C
#define	CITYCMDC_C
#include "baye/enghead.h"

/*FAR U8 AddPerson(U8 city,U8 person);
 FAR U8 DelPerson(U8 city,U8 person);
 FAR U8 AddOrderHead(OrderType *Order);
 FAR U8 AddOrderEnd(OrderType *Order);
 FAR U8 GetCityPersons(U8 city,U8 *pqueue);
 FAR U8 GetCityCaptives(U8 city,U8 *pqueue);
 FAR U8 GetEnemyPersons(U8 king,U8 *pqueue);
 FAR U8 GetEnemySatraps(U8 king,U8 *squeue);
 FAR U8 GetEnemyKing(U8 king,U8 *kqueue);
 FAR U8 MenuComm(MenuType *menu);
 FAR U8 ShowPersonControl(U8 *person,U8 pcount,0,U8 x0,U8 y0,U8 x1,U8 y1);
 FAR U16 GetFood(U16 min,U16 max);
 FAR U16 GetMoney(U16 min,U16 max);
 FAR U16 GetArmy(U16 min,U16 max);
 FAR U8 GetCitySet(CitySetType *pos);
 FAR U8 SearchRoad(U8 sc,U8 xs,U8 ys,U8 ob,U8 xo,U8 yo);*/

/******************************************************************************
 * 函数名:ExchangeMake
 * 说  明:“交易”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
FAR U8 ExchangeMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID p;
    U8 mstr[15];
    U16 exc;
    OrderType order;
    RECT pRect;
    CityType *cptr;


    pqptr = (PersonID*)SHARE_MEM;

    do
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            ShowConstStrMsg(STR_NOPERSON);
            break;
        }
        pcode = ShowPersonControl(pqptr,pcount,PID(0),WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);

        if (0xffff != pcode)
        {
            p = pqptr[pcode];
            if (!IsManual(p,EXCHANGE))
            {
                ShowConstStrMsg(NOTE_STR9);
                continue;
            }

            cptr = &g_Cities[city];

            /*menu.top = 0;
             menu.set = 0;
             menu.count = 2;
             menu.sx = WK_SX + WK_EX / 2;
             menu.sy = WK_SY + 10 + 4;
             menu.ex = WK_SX + WK_EX / 2 + ASC_WID * 4;
             menu.ey = WK_EY - 10;
             ResLoadToMem(STRING_CONST,ATRR_STR65,mstr);
             ResLoadToMem(STRING_CONST,ATRR_STR66,&mstr[5]);
             menu.menustr[0] = mstr;
             menu.menustr[1] = &mstr[5];
             pcount = MenuComm(&menu);*/
            ResLoadToMem(STRING_CONST,MENU_TSTR,mstr);
            pRect.sx = WK_SX + WK_EX / 2;
            pRect.ex = WK_SX + WK_EX / 2 + ASC_WID * 4;
            pRect.sy = WK_SY + 10 + 4;
            if ((WK_EY - (WK_SY + 10 + 4)) > (ASC_HGT * 2 + 4)) {
                pRect.ey = WK_SY + 10 + 4 + ASC_HGT * 2;
            } else {
                pRect.ey = WK_SY + 10 + 4 + ((WK_EY - (WK_SY + 10 + 4)) / ASC_HGT) * ASC_HGT;
            }
            pcount = PlcSplMenu(&pRect,0,mstr);

            if (0xffff == pcount)
            {
                continue;
            }
            else if (0 == pcount)
            {
                /*添加买入代码*/
                if (cptr->Money / 5)
                {
                    exc = GetFood(1,cptr->Money / 5);
                    if (0xffff != exc)
                    {
                        if (g_engineConfig.fixOverFlow16) {
                            ADD16(cptr->Food, exc);
                            exc *= 5;
                            ADD16(cptr->Money, -exc);
                        } else {
                            cptr->Food += exc;
                            exc *= 5;
                            cptr->Money -= exc;
                        }
                    }
                    else {
                        continue;
                    }
                }
            }
            else if (1 == pcount)
            {
                /*添加卖出代码*/
                if (cptr->Food)
                {
                    exc = GetFood(1,cptr->Food);
                    if (0xffff != exc)
                    {
                        if (g_engineConfig.fixOverFlow16) {
                            ADD16(cptr->Food, -exc);
                            exc *= 2;
                            ADD16(cptr->Money, exc);
                        } else {
                            cptr->Food -= exc;
                            exc *= 2;
                            cptr->Money += exc;
                        }
                        if (cptr->Money > 30000)
                            cptr->Money = 30000;
                    }
                    else {
                        continue;
                    }
                }
            }

            OrderConsumeThew(p,EXCHANGE);
            order.OrderId = EXCHANGE;
            order.Person = p;
            order.City = city;
            order.TimeCount = 0;
            if (AddOrderHead(&order)) {
                DelPerson(city,p);
            }
        }
        break;
    } while (1);

    return(1);
}


/******************************************************************************
 * 函数名:TreatMake
 * 说  明:“宴请”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
FAR U8 TreatMake(U8 city)
{
    U8 str[512];
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode = PID(0);
    PersonID p;
    PersonType *pptr;

    pqptr = (PersonID*)SHARE_MEM;
    pcount = GetCityPersons(city,pqptr);
    if (!pcount)
    {
        ShowConstStrMsg(STR_NOPERSON);
        return(1);
    }

    do
    {
        if (!IsMoney(city,TREAT))
        {
            /*金钱不足*/
            ShowConstStrMsg(NOTE_STR8);
            return(1);
        }

        ShowMapClear();
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            /*ShowConstStrMsg(STR_NOPERSON);*/
            return(1);
        }
        pcode = ShowPersonControl(pqptr,pcount,pcode,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff != pcode)
        {
            p = pqptr[pcode];
            pptr = &g_Persons[p];
            OrderConsumeMoney(city,TREAT);
            pptr->Thew += THREW_TREAT;
            if (pptr->Thew > 100)
                pptr->Thew = 100;
            if (p != g_PlayerKing)
            {
                ResLoadToMem(STRING_CONST,P_SAY_STR40,str);
                ShowMapClear();
                ShowGReport(p,str);
                if (pptr->Devotion < 100)
                    pptr->Devotion += 1;
            }
        }
        else
        {
            break;
        }
    } while (1);

    return(1);
}

/******************************************************************************
 * 函数名:TransportationMake
 * 说  明:“输送”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
FAR U8 TransportationMake(U8 city)
{
    PersonID *pqptr;
    U8 str[512];
    U32 pcount;
    PersonID pcode;
    PersonID p = PID0;
    U8 ocity;
    U8 odis;
    U8 xs,ys;
    OrderType order;

    pqptr = (PersonID*)SHARE_MEM;

    pcount = 0;
    while (1)
    {
        if (0 == pcount)
        {
            pcount = GetCityPersons(city,pqptr);
            if (!pcount)
            {
                ShowConstStrMsg(STR_NOPERSON);
                break;
            }
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff == pcode)
                break;

            p = pqptr[pcode];
            if (!IsManual(p,TRANSPORTATION))
            {
                ShowConstStrMsg(NOTE_STR9);
                pcount = 0;
                continue;
            }

            pcount = 1;
        }
        else if (1 == pcount)
        {
            order.Food = GetFood(0,g_Cities[city].Food);
            if (0xffff == order.Food)
                pcount = 0;
            else
                pcount = 2;
        }
        else if (2 == pcount)
        {
            order.Money = GetMoney(0,g_Cities[city].Money);
            if (0xffff == order.Money)
                pcount = 1;
            else
                pcount = 3;
        }
        else if (3 == pcount)
        {
            order.Arms = GetArmy(0,g_Cities[city].MothballArms);
            if (0xffff == order.Arms)
                pcount = 2;
            else
                pcount = 4;
        }
        else if (4 == pcount)
        {
            xs = g_CityPos.setx;
            ys = g_CityPos.sety;

            ResLoadToMem(STRING_CONST,STR_OBJ,str);
            ShowMapClear();
            ShowGReport(p,str);

            ocity = GetCitySet(&g_CityPos);
            if (0xff == ocity)
            {
                pcount = 3;
            }
            else if (ocity != city)
            {
                if (!(g_Cities[ocity].Belong))
                {
                    /*提示无人占领城池*/
                    ShowConstStrMsg(NOTE_STR5);
                    continue;
                }
                else if (g_Cities[ocity].Belong != g_Cities[city].Belong)
                {
                    /*提示敌方城池*/
                    ShowConstStrMsg(NOTE_STR3);
                    continue;
                }

                odis = SearchRoad(city,xs,ys,ocity,g_CityPos.setx,g_CityPos.sety);
                if (0xff != odis)
                {
                    OrderConsumeThew(p,TRANSPORTATION);

                    if (g_engineConfig.fixOverFlow16) {
                        ADD16(g_Cities[city].Food, -order.Food);
                        ADD16(g_Cities[city].Money, -order.Money);
                        ADD16(g_Cities[city].MothballArms, -order.Arms);
                    } else {
                        g_Cities[city].Food -= order.Food;
                        g_Cities[city].Money -= order.Money;
                        g_Cities[city].MothballArms -= order.Arms;
                    }

                    ResLoadToMem(STRING_CONST,P_SAY_STR31,str);
                    ShowGReport(p,str);
                    order.OrderId = TRANSPORTATION;
                    order.Person = p;
                    order.City = city;
                    order.Object = PID(ocity);
                    order.TimeCount = odis;
                    if (AddOrderHead(&order)) {
                        DelPerson(city,p);
                    }
                    break;
                }
                else
                {
                    /*提示无法到达*/
                    ShowConstStrMsg(NOTE_STR4);
                }
            }
        }
    }

    return(1);
}

/******************************************************************************
 * 函数名:MoveMake
 * 说  明:“移动”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 MoveMake(U8 city)
{
    PersonID *pqptr;
    U8 str[512];
    U32 pcount;
    PersonID pcode = PID0;
    PersonID p = PID0;
    U32 ocity;
    U32 odis;
    U32 xs,ys;
    OrderType order;

    pqptr = (PersonID*)SHARE_MEM;

    pcount = GetCityPersons(city,pqptr);
    if (!pcount)
    {// 没人
        ShowConstStrMsg(STR_NOPERSON);
        return 0;
    }

    while (1)
    {// 先选城池
        xs = g_CityPos.setx;
        ys = g_CityPos.sety;
        ResLoadToMem(STRING_CONST,STR_OBJ,str);
        ShowMapClear();
        ShowGReport(g_PlayerKing,str);
        ocity = GetCitySet(&g_CityPos);
        if (0xff == ocity)
        {//取消
            return 0;
        }
        else if (ocity != city)
        {
            if (!g_Cities[ocity].Belong)
            {
                ShowConstStrMsg(NOTE_STR5);
            }
            else if (g_Cities[ocity].Belong != g_Cities[city].Belong)
            {
                /*提示敌方城池*/
                ShowConstStrMsg(NOTE_STR3);
                continue;
            }


            odis = SearchRoad(city,xs,ys,ocity,g_CityPos.setx,g_CityPos.sety);
            if (0xff != odis)
            {
                break;
            }
            else
            {
                /*提示无法到达*/
                ShowConstStrMsg(NOTE_STR4);
            }
        }
    }

    while (1)
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            return 1;
        }
        ShowMapClear();
        pcode = ShowPersonControl(pqptr,pcount,pcode,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff == pcode)
            break;
        p = pqptr[pcode];
        if (!IsManual(p,MOVE))
        {
            ShowConstStrMsg(NOTE_STR9);
            pcount = 0;
            continue;
        }
        if (g_engineConfig.fixThewOverFlow) {
            OrderConsumeThew(p, MOVE);
        } else {
            OrderConsumeThew(p, TRANSPORTATION);
        }

        ResLoadToMem(STRING_CONST,P_SAY_STR31,str);
        ShowGReport(p,str);
        order.OrderId = MOVE;
        order.Person = p;
        order.City = city;
        order.Object = PID(ocity);
        order.TimeCount = odis;
        if (AddOrderHead(&order)) {
            DelPerson(city,p);
        }
    }

    return(1);
}

/******************************************************************************
 * 函数名:AlienateMake
 * 说  明:“离间”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 AlienateMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID pp;
    OrderType order;

    if (!IsMoney(city,ALIENATE))
    {
        /*金钱不足*/
        ShowConstStrMsg(NOTE_STR8);
        return(1);
    }

    pqptr = (PersonID*)SHARE_MEM;

    do
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            ShowConstStrMsg(STR_NOPERSON);
            break;
        }
        pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff != pcode)
        {
            if (!IsManual(pqptr[pcode],ALIENATE))
            {
                ShowConstStrMsg(NOTE_STR9);
                continue;
            }

            pp = pqptr[pcode];
            pcount = GetEnemyPersons(g_PlayerKing,pqptr);
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff != pcode)
            {
                if (g_engineConfig.fixAlienateComsumeThew) {
                    OrderConsumeThew(pp, ALIENATE);
                } else {
                    OrderConsumeThew(pqptr[pcode],ALIENATE);
                }

                OrderConsumeMoney(city,ALIENATE);
                order.OrderId = ALIENATE;
                order.Person = pp;
                order.City = city;
                order.Object = pqptr[pcode];
                order.TimeCount = 10;
                if (AddOrderHead(&order)) {
                    DelPerson(city,pp);
                }
            }
            else
            {
                continue;
            }
        }
        break;
    } while (1);

    return(1);
}

/******************************************************************************
 * 函数名:CanvassMake
 * 说  明:“招揽”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 CanvassMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID pp;
    OrderType order;

    if (!IsMoney(city,CANVASS))
    {
        /*金钱不足*/
        ShowConstStrMsg(NOTE_STR8);
        return(1);
    }

    pqptr = (PersonID*)SHARE_MEM;

    do
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            ShowConstStrMsg(STR_NOPERSON);
            break;
        }
        pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff != pcode)
        {
            pp = pqptr[pcode];
            if (!IsManual(pp,CANVASS))
            {
                ShowConstStrMsg(NOTE_STR9);
                continue;
            }

            pcount = GetEnemyPersons(g_PlayerKing,pqptr);
            ShowMapClear();
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff != pcode)
            {
                OrderConsumeThew(pp,CANVASS);

                OrderConsumeMoney(city,CANVASS);
                order.OrderId = CANVASS;
                order.Person = pp;
                order.City = city;
                order.Object = pqptr[pcode];
                order.TimeCount = 10;
                if (AddOrderHead(&order)) {
                    DelPerson(city,pp);
                }
            }
            else
            {
                continue;
            }
        }
        break;
    } while (1);

    return(1);
}

/******************************************************************************
 * 函数名:CounterespiongeMake
 * 说  明:“策反”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 CounterespiongeMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID pp;
    OrderType order;

    if (!IsMoney(city,COUNTERESPIONAGE))
    {
        /*金钱不足*/
        ShowConstStrMsg(NOTE_STR8);
        return(1);
    }

    pqptr = (PersonID*)SHARE_MEM;

    do
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            ShowConstStrMsg(STR_NOPERSON);
            break;
        }
        ShowMapClear();
        pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff != pcode)
        {
            pp = pqptr[pcode];
            if (!IsManual(pp,COUNTERESPIONAGE))
            {
                ShowConstStrMsg(NOTE_STR9);
                continue;
            }


            pcount = GetEnemySatraps(g_PlayerKing,pqptr);
            if (!pcount)
            {
                /*提示无策反对象*/
                break;
            }
            ShowMapClear();
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff != pcode)
            {
                OrderConsumeThew(pp,COUNTERESPIONAGE);
                OrderConsumeMoney(city,COUNTERESPIONAGE);
                order.OrderId = COUNTERESPIONAGE;
                order.Person = pp;
                order.City = city;
                order.Object = pqptr[pcode];
                order.TimeCount = 10;
                if (AddOrderHead(&order)) {
                    DelPerson(city,pp);
                }
            }
            else
            {
                continue;
            }
        }
        break;
    } while (1);

    return(1);
}

/******************************************************************************
 * 函数名:RealienateMake
 * 说  明:“反间”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 RealienateMake(U8 city)
{
    return(1);
}

/******************************************************************************
 * 函数名:InduceMake
 * 说  明:“劝降”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 InduceMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID pp;
    OrderType order;

    if (!IsMoney(city,INDUCE))
    {
        /*金钱不足*/
        ShowConstStrMsg(NOTE_STR8);
        return(1);
    }

    pqptr = (PersonID*)SHARE_MEM;

    do
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            ShowConstStrMsg(STR_NOPERSON);
            break;
        }
        pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff != pcode)
        {
            pp = pqptr[pcode];
            if (!IsManual(pp,INDUCE))
            {
                ShowConstStrMsg(NOTE_STR9);
                continue;
            }

            pcount = GetEnemyKing(g_PlayerKing,pqptr);
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff != pcode)
            {
                OrderConsumeThew(pp,INDUCE);

                OrderConsumeMoney(city,INDUCE);
                order.OrderId = INDUCE;
                order.Person = pp;
                order.City = city;
                order.Object = pqptr[pcode];
                order.TimeCount = 10;
                if (AddOrderHead(&order)) {
                    DelPerson(city,pp);
                }
            }
            else
            {
                continue;
            }
        }
        break;
    } while (1);

    return(1);
}

/******************************************************************************
 * 函数名:TributeMake
 * 说  明:“朝贡”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
/*FAR U8 TributeMake(U8 city)
 {
	return(1);
 }
 */



/******************************************************************************
 * 函数名:ReconnoitreMake
 * 说  明:“侦察”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 ReconnoitreMake(U8 city)
{
    PersonID *pqptr;
    U8 str[512];
    U32 pcount;
    PersonID pcode;
    PersonID p = PID0;
    U32 ocity;
    OrderType order;
    
    if (!IsMoney(city,RECONNOITRE))
    {
        /*金钱不足*/
        ShowConstStrMsg(NOTE_STR8);
        return(1);
    }
    
    pqptr = (PersonID*)SHARE_MEM;
    
    pcount = 0;
    while (1)
    {
        if (0 == pcount)	/*选择武将*/
        {
            pcount = GetCityPersons(city,pqptr);
            if (!pcount)
            {
                ShowConstStrMsg(STR_NOPERSON);
                break;
            }
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff == pcode)
                break;
            p = pqptr[pcode];
            if (!IsManual(p,RECONNOITRE))
            {
                ShowConstStrMsg(NOTE_STR9);
                pcount = 0;
                continue;
            }
            
            pcount = 1;
        }
        else if (1 == pcount)	/*选择目标城池*/
        {
            ResLoadToMem(STRING_CONST,STR_OBJ,str);
            ShowMapClear();
            ShowGReport(p,str);
            
            ocity = GetCitySet(&g_CityPos);
            if (0xff == ocity)
            {
                pcount = 0;
            }
            else if (ocity != city)
            {
                if (g_Cities[ocity].Belong == g_Cities[city].Belong)
                {
                    /*提示我方城池*/
                    ShowConstStrMsg(NOTE_STR7);
                    continue;
                }
                else
                {
                    ShowCityPro(ocity);
                    OrderConsumeThew(p,RECONNOITRE);
                    
                    OrderConsumeMoney(city,RECONNOITRE);
                    order.OrderId = RECONNOITRE;
                    order.Person = p;
                    order.City = city;
                    order.Object = PID(ocity);
                    order.TimeCount = 1;
                    if (AddOrderHead(&order)) {
                        DelPerson(city,p);
                    }
                    break;
                }
            }
        }
    }
    
    return(1);
}

/******************************************************************************
 * 函数名:ConscriptionMake
 * 说  明:“征兵”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 ConscriptionMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID p;
    U16 arms,armsm;
    OrderType order;
    
    pqptr = (PersonID*)SHARE_MEM;
    pcount = GetCityPersons(city,pqptr);
    if (!pcount)
    {
        ShowConstStrMsg(STR_NOPERSON);
        return(1);
    }
    
    do
    {
        if (!IsMoney(city,CONSCRIPTION))
        {
            /*金钱不足*/
            ShowConstStrMsg(NOTE_STR8);
            return(1);
        }
        
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            /*ShowConstStrMsg(STR_NOPERSON);*/
            break;
        }
        do
        {
            ShowMapClear();
            pcode = ShowPersonControl(pqptr,pcount,PID0,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff != pcode)
            {
                p = pqptr[pcode];
                if (!IsManual(p,CONSCRIPTION))
                {
                    ShowConstStrMsg(NOTE_STR9);
                    continue;
                }
                arms = g_Cities[city].PeopleDevotion;
                arms *= g_engineConfig.armsPerDevotion;
                armsm = min(g_Cities[city].Money * g_engineConfig.armsPerMoney, 0xfffe);
                if (arms > armsm)
                {
                    arms = armsm;
                }
                
                arms = GetArmy(0,arms);
                if (0xffff != arms)
                {
                    ADD16(g_Cities[city].MothballArms, arms);
                    OrderConsumeThew(p,CONSCRIPTION);
                    /*添加消耗金钱（固定数目）代码*/
                    /*OrderConsumeMoney(city,CONSCRIPTION);*/
                    armsm = arms / g_engineConfig.armsPerMoney;
                    ADD16(g_Cities[city].Money, -armsm);
                    order.OrderId = CONSCRIPTION;
                    order.Person = p;
                    order.City = city;
                    order.TimeCount = 0;
                    if (AddOrderHead(&order)) {
                        DelPerson(city,p);
                    }
                }
                else
                {
                    continue;
                }
            }
            else
            {
                return(1);
            }
            break;
        } while (1);
    } while (1);
    
    return(1);
}

/******************************************************************************
 * 函数名:DistributeMake
 * 说  明:“分配”命令生成
 *
 * 入口参数：所在城市编号
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
FAR U8 DistributeMake(U8 city)
{
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode = PID0;
    PersonID p;
    U32 armys;
    
    pqptr = (PersonID*)SHARE_MEM;
    
    pcount = GetCityPersons(city,pqptr);
    if (!pcount)
    {
        ShowConstStrMsg(STR_NOPERSON);
        return(1);
    }
    pcount = 0;
    while (1)
    {
        if (0 == pcount)
        {
            pcount = GetCityPersons(city,pqptr);
            if (!pcount)
            {
                /*ShowConstStrMsg(STR_NOPERSON);*/
                break;
            }
            /*gam_clrlcd(WK_SX,WK_SY,WK_EX,WK_EY);*/
            ShowMapClear();
            pcode = ShowPersonControl(pqptr,pcount,pcode,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
            if (0xffff == pcode)
                break;
            
            p = pqptr[pcode];
            
            pcount = 1;
        }
        else if (1 == pcount)
        {
            p = pqptr[pcode];
            /*武将最大统率兵力*/
            armys = PlcArmsMax(p);
            /*----------------*/
            if (armys > (g_Cities[city].MothballArms + g_Persons[p].Arms))
                armys = g_Cities[city].MothballArms + g_Persons[p].Arms;

            if (!armys)
            {
                ShowConstStrMsg(NOTE_STR6);
                pcount = 0;
                continue;
            }
            
            armys = GetArmy(0,armys);
            if (0xffff == armys)
            {
                pcount = 0;
            }
            else
            {
                g_Cities[city].MothballArms = g_Cities[city].MothballArms + g_Persons[p].Arms - armys;
                g_Persons[p].Arms = armys;
            }
            pcount = 0;
        }
    }
    
    return(1);
}
