/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	showfun.c
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
#undef	CITYCMDE_C
#define	CITYCMDE_C
#include "baye/enghead.h"
#include "baye/bind-objects.h"


/******************************************************************************
 * 函数名:ConfiscateMake
 * 说  明:“没收”命令生成
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
FAR U8 ConfiscateMake(U8 city)
{
    PersonID *pqptr;
    U8 str[512];
    U32 pcount;
    PersonID pcode = PID0, p;
    ToolID g;
    ToolID gq[3],*gptr;
    U16 c;

    pqptr = (PersonID*)SHARE_MEM;
    do
    {
        pcount = GetCityPersons(city,pqptr);
        if (!pcount)
        {
            ShowConstStrMsg(STR_NOPERSON);
            break;
        }
        /*gam_clrlcd(WK_SX,WK_SY,WK_EX,WK_EY);*/
        ShowMapClear();
        pcode = ShowPersonControl(pqptr,pcount,pcode,WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
        if (0xffff != pcode)
        {
            p = pqptr[pcode];
            c = 0;
            gptr = g_Persons[p].Equip;
            if (!gptr[0])
            {
                gptr[0] = gptr[1];
                gptr[1] = TID(0);
            }
            else
            {
                c = 1;
            }

            if (gptr[c])
            {
                c += 1;
            }
            gq[0] = TID(gptr[0] - 1);
            gq[1] = TID(gptr[1] - 1);
            if (c)
            {
                /*gam_clrlcd(WK_SX,WK_SY,WK_EX,WK_EY);*/
                ShowMapClear();
                g = ShowGoodsControl(gq, TID(c), TID(0), WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
                if (0xffff == g)
                {
                    continue;
                }
                else
                {
                    int ret = -1;

                    IF_HAS_HOOK("willTakeOffTool") {
                        BIND_U8EX("cityIndex", &city);
                        BIND_U16EX("personIndex", &p);
                        BIND_U16EX("toolIndex", &gq[g]);
                        ret = CALL_HOOK_A();
                    }

                    if (ret == 0) {
                        continue;
                    }

                    /*添加没收成功代码*/
                    if (p != g_PlayerKing)
                    {
                        ResLoadToMem(STRING_CONST,P_SAY_STR7 + (gam_rand() % 3),str);
                        ShowMapClear();
                        ShowGReport(p,str);
                        if (g_Persons[p].Devotion < 20)
                            g_Persons[p].Devotion = 0;
                        else
                            g_Persons[p].Devotion -= 20;
                    }
                    if (DelGoodsPerson(gq[g],p)) {
                        gptr[g] = TID(0);
                        ToolID index = AddGoods(city,gq[g]);
                        SetGoodsByIndex(TID(index - 1));
                    }
                }
            }
            else
            {
                /*提示该武将没道具*/
                ShowConstStrMsg(STR_NOGOODS);
                continue;
            }
        }
        else
        {
            return(1);
        }
    } while (1);
    return(1);
}

/******************************************************************************
 * 函数名:DepredateMake
 * 说  明:“掠夺”命令生成
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
FAR U8 DepredateMake(U8 city)
{
    U8 str[512];
    PersonID *pqptr;
    U32 pcount;
    PersonID pcode;
    PersonID p;
    OrderType order;

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
            p = pqptr[pcode];
            if (!IsManual(p,CONSCRIPTION))
            {
                ShowConstStrMsg(NOTE_STR9);
                continue;
            }
            OrderConsumeThew(p,CONSCRIPTION);

            ResLoadToMem(STRING_CONST,P_SAY_STR27,str);
            ShowGReport(p,str);

            order.OrderId = DEPREDATE;
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
 * 函数名:进攻提示
 * 说  明:ShowAttackNote
 *
 * 入口参数：ap -- 进攻方，city -- 被进攻城市
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-23 11:00	基本功能完成
 ******************************************************************************/
FAR void ShowAttackNote(PersonID ap,U8 city)
{
    SBUF str,buf;

    ShowFightNoteFace(0);

    if (ap == g_PlayerKing)
    {
        ResLoadToMem(STRING_CONST,STR_OURS,str);
    }
    else
    {
        GetPersonName(ap,str);
    }
    ResLoadToMem(STRING_CONST,STR_ATTACKC,buf);
    gam_strcat(str,buf);
    GetCityName(city,buf);
    gam_strcat(str,buf);
    PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    GamDelay(100,false);
}


/******************************************************************************
 * 函数名:战斗提示
 * 说  明:ShowFightNote
 *
 * 入口参数：ap -- 进攻方，bc -- 被进攻方
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-23 11:19	基本功能完成
 ******************************************************************************/
FAR void ShowFightNote(PersonID ap,PersonID bc)
{
    SBUF str,buf;

    ShowFightNoteFace(1);

    GetPersonName(ap,str);
    ResLoadToMem(STRING_CONST,STR_FIGHT_NOTE1,buf);
    gam_strcat(str,buf);
    GetPersonName(bc,buf);
    gam_strcat(str,buf);
    ResLoadToMem(STRING_CONST,STR_FIGHT_NOTE2,buf);
    gam_strcat(str,buf);
    PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    GamDelay(300, g_engineConfig.responseNoteOfBettle);
}

/******************************************************************************
 * 函数名:战斗胜利提示
 * 说  明:ShowFightWinNote
 *
 * 入口参数：战胜方
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-23 11:27	基本功能完成
 ******************************************************************************/
FAR void ShowFightWinNote(PersonID wp)
{
    SBUF str,buf;

    ShowFightNoteFace(2);

    if (wp == g_PlayerKing)
    {
        ResLoadToMem(STRING_CONST,STR_OURS,str);
    }
    else
    {
        GetPersonName(wp,str);
    }
    ResLoadToMem(STRING_CONST,STR_WON,buf);
    gam_strcat(str,buf);
    PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    GamDelay(300, g_engineConfig.responseNoteOfBettle);
}

/******************************************************************************
 * 函数名:我方战斗失败提示
 * 说  明:ShowFightLossNote
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-23 11:32	基本功能完成
 ******************************************************************************/
FAR void ShowFightLossNote(void)
{
    SBUF str,buf;

    ShowFightNoteFace(3);

    ResLoadToMem(STRING_CONST,STR_OURS,str);

    ResLoadToMem(STRING_CONST,STR_LOST,buf);
    gam_strcat(str,buf);
    PlcMidShowStr(WK_SX + (WK_EX  - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2 + 64 + 2,str);
    GamDelay(300, g_engineConfig.responseNoteOfBettle);
}

/******************************************************************************
 * 函数名:ShowFightNoteFace
 * 说  明:
 *
 * 入口参数：图片序号
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
void ShowFightNoteFace(U8 idx)
{
    gam_clrlcd(WK_SX + (WK_EX - 120 - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2,WK_SX + (WK_EX - 120 - WK_SX) / 2 + 120,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 84);
    gam_rect(WK_SX + (WK_EX - 120 - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2,WK_SX + (WK_EX - 120 - WK_SX) / 2 + 120,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 84);
    pushPaintColor(g_engineConfig.theme.battleNoteColor);
    gam_drawpic(FIGHT_NOTE_ICON, idx, WK_SX + (WK_EX - 84 - WK_SX) / 2,WK_SY + (WK_EY - 84 - WK_SY) / 2 + 2, 1);
    popPaintColor();
}

/******************************************************************************
 * 函数名:needMoney
 * 说  明:指定命令需要消耗的金钱
 *
 * 入口参数：order -- 指定命令
 *
 * 出口参数：金钱
 ******************************************************************************/
static U16 needMoney(U8 order) {

    U8 *ptr = ResLoadToCon(IFACE_CONID,ConsumeMoney,g_CBnkPtr);

    if (g_engineConfig.enable16bitConsumeMoney) {
        return ((U16*)ptr)[order];
    } else {
        return ptr[order];
    }
}

/******************************************************************************
 * 函数名:IsMoney
 * 说  明:判断指定城市是否有足够的金钱执行指定命令
 *
 * 入口参数：city -- 指定城市，order -- 指定命令
 *
 * 出口参数：1 -- 足够，0 -- 不足
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 21:44	基本功能完成
 ******************************************************************************/
FAR U8 IsMoney(U8 city,U8 order)
{
    if (g_engineDebug) return 1;

    if (g_Cities[city].Money >= needMoney(order))
        return(1);
    else
        return(0);
}

/******************************************************************************
 * 函数名:OrderConsumeMoney
 * 说  明:指定城市消耗金钱执行指定命令
 *
 * 入口参数：city -- 指定成都市，order -- 指定命令
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 21:49	基本功能完成
 ******************************************************************************/
FAR void OrderConsumeMoney(U8 city,U8 order)
{
    U16 consume = needMoney(order);

    if (g_engineConfig.fixOverFlow16) {
        ADD16(g_Cities[city].Money, -consume);
    } else {
        g_Cities[city].Money -= consume;
    }
}

/******************************************************************************
 * 函数名:IsManual
 * 说  明:判断指定武将是否有足够的体力执行指定命令
 *
 * 入口参数：person -- 指定武将，order -- 指定命令
 *
 * 出口参数：1 -- 足够，0 -- 不足
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 20:18	基本功能完成
 ******************************************************************************/
FAR U8 IsManual(PersonID person,U8 order)
{
    U8 *ptr;
    
    if (g_engineDebug) return 1;

    ptr = ResLoadToCon(IFACE_CONID,ConsumeThew,g_CBnkPtr);
    if (g_Persons[person].Thew >= ptr[order])
        return(1);
    else
        return(0);
}

/******************************************************************************
 * 函数名:OrderConsumeThew
 * 说  明:指定武将消耗体力执行指定命令
 *
 * 入口参数：person -- 指定武将，order -- 指定命令
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 20:18	基本功能完成
 ******************************************************************************/
FAR void OrderConsumeThew(PersonID person,U8 order)
{
    U8 *ptr;
    
    ptr = ResLoadToCon(IFACE_CONID,ConsumeThew,g_CBnkPtr);
    if (g_engineConfig.fixOverFlow16) {
        ADD16(g_Persons[person].Thew, -ptr[order]);
    } else {
        g_Persons[person].Thew -= ptr[order];
    }
}

/******************************************************************************
 * 函数名:GetWeekCity
 * 说  明:取得最弱的城市
 *
 * 入口参数：count -- 城市个数，cqueue -- 城市队列指针
 *
 * 出口参数：最弱的城市
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-8-29 16:20	基本功能完成
 ******************************************************************************/
FAR U8 GetWeekCity(U8 count,U8 *cqueue)
{
    U32 c, i, o, cw = 0;
    PersonID p;
    U16 armst,armsw;
    CityType *cptr;
    OrderType *inode;
    
    inode = (OrderType *) ORDERQUEUE;
    armsw = 0xffff;
    for (c = 0;c < count;c ++)
    {
        cptr = &g_Cities[cqueue[c]];
        armst = 0;
        for (i = 0;i < cptr->Persons;i ++)
        {
            p = g_PersonsQueue[cptr->PersonQueue + i];
            if (g_Persons[p].Belong == cptr->Belong)
            {
                armst += g_Persons[p].Arms;
            }
        }
        for (i = 0;i < ORDER_MAX;i ++)
        {
            o = inode[i].OrderId;
            if (0xff == o)
                continue;
            
            if ((o != MOVE) && (o != BATTLE) && inode[i].City == cqueue[c])
            {
                armst += g_Persons[inode[i].Person].Arms;
            }
        }
        if (armsw > armst)
        {
            armsw = armst;
            cw = c;
        }
    }
    return(cqueue[cw]);
}

U32 OrderNeedMoney(U8 order)
{
    return needMoney(order);
}

