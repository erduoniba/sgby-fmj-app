/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	cityedit.c
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
 *	1.0    2005-6-3 14:45  陈泽伟     基本的功能完成
 ***********************************************************************/

#include "baye/stdsys.h"
#include "baye/comm.h"
#undef	CITYCMD_C
#define	CITYCMD_C
#include "baye/enghead.h"
#include "baye/bind-objects.h"

/*U8 MainOrderMenu(void);
 U8 InteriorOrderMenu(void);
 U8 DiplomatismOrderMenu(void);
 U8 ArmamentOrderMenu(void);

 FAR U8 AddPerson(U8 city,U8 person);
 FAR U8 DelPerson(U8 city,U8 person);
 FAR U8 AddOrderHead(OrderType *Order);
 FAR U8 AddOrderEnd(OrderType *Order);
 FAR U8 DelOrder(OrderQueueType *Order);
 FAR U8 GetKingCitys(U8 king,U8 *cqueue);
 FAR U8 GetPersonCity(U8 person);
 FAR U8 GetCityPersons(U8 city,U8 *pqueue);
 FAR U8 GetCityCaptives(U8 city,U8 *pqueue);
 FAR U8 GetCityOutPersons(U8 city,U8 *pqueue);
 FAR U8 MenuComm(MenuType *menu);
 FAR U8 ShowPersonControl(U8 *person,U8 pcount,U8 x0,U8 y0,U8 x1,U8 y1);
 FAR U16 GetFood(U16 min,U16 max);
 FAR U16 GetMoney(U16 min,U16 max);
 FAR U16 GetArmy(U16 min,U16 max);
 FAR U8 GetCitySet(CitySetType *pos);
 FAR U8 SearchRoad(U8 sc,U8 xs,U8 ys,U8 ob,U8 xo,U8 yo);*/

/******************************************************************************
 * 函数名:AppointDrv
 * 说  明:“任命”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
/*U8 AppointDrv(OrderType *Order)
 {
	return(1);
 }*/


/******************************************************************************
 * 函数名:AssartDrv
 * 说  明:“开垦”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 AssartDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

U8 NopDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:AccractbusinessDrv
 * 说  明:“招商”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 AccractbusinessDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:SearchDrv
 * 说  明:“搜寻”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 SearchDrv(OrderType *Order)
{
    SBUF str;
    SBUF astr;
    SearchCondition *infp;
    U32 sp;
    PersonID person, pb;
    U32 iq;
    PersonID p;
    U32 fo;
    U32 pcount;
    U32 rnd = '\0';
    U32 pss;
    U32 count;

    ShowMapClear();
    pss = gam_rand() % 4;
    person = Order->Person;
    pb = g_Persons[person].Belong;
    iq = g_Persons[person].IQ;

    if (g_engineDebug) pss = 1;

    switch (pss)
    {
        case 0:
            break;
        case 1:
            pss = 0;
            rnd = gam_rand() % 150;
            if (g_engineDebug) rnd = gam_rand() % 2;
            if (rnd < iq)
            {
                PersonID *pqptr = (PersonID*)SHARE_MEM;
                switch (rnd % 2)
                {
                    case 0:
                        pcount = GetCityOutPersons(Order->City,pqptr);
                        if (pcount)
                        {
                            sp = gam_rand() % pcount;
                            p = pqptr[sp];
                            count = p;
                            infp = (SearchCondition*)ResLoadToCon(GENERAL_CON,g_PIdx,g_CBnkPtr);
                            fo = infp[count].bole;
                            rnd = 0;
                            if (fo - 1 == person)
                            {
                                rnd = 1;
                                g_Persons[p].Belong = pb;
                                g_Persons[p].Devotion = 70 + gam_rand() % 30;

                                /*搜索到武将*/
                            }
                            else if (!fo)
                            {
                                rnd = gam_rand() % 110;
                                if (rnd < iq)
                                {
                                    rnd = 1;
                                    g_Persons[p].Belong = pb;
                                    g_Persons[p].Devotion = 70 + gam_rand() % 30;
                                }
                                else
                                {
                                    rnd = 2;
                                }
                            }

                            if (pb == g_PlayerKing + 1)
                            {
                                pss = 1;
                                if (!rnd)
                                {
                                    ResLoadToMem(STRING_CONST,STR_HEARPERSON,str);
                                    ShowGReport(person,str);
                                    break;
                                }
                                else
                                {
                                    ResLoadToMem(STRING_CONST,STR_GETGOODS,str);
                                    GetPersonName(p,astr);
                                    gam_strcat(str,astr);
                                }
                                ShowGReport(person,str);
                                if (rnd == 1)
                                {
                                    rnd = P_SAY_STR13 + gam_rand() % 4;
                                }
                                else if (rnd == 2)
                                {
                                    rnd = P_SAY_STR17 + gam_rand() % 4;
                                }
                                ResLoadToMem(STRING_CONST,rnd,str);
                                ShowGReport(p,str);
                            }
                        }
                        break;
                    case 1: {
                        ToolID *pqptr = (ToolID*)SHARE_MEM;
                        ToolID p, pcount;

                        pcount = GetCityDispGoods(Order->City,pqptr);
                        if (pcount)
                        {
                            sp = gam_rand() % pcount;
                            p = pqptr[sp];
                            count = p;
                            infp = (SearchCondition*)ResLoadToCon(GOODS_CON,g_PIdx,g_CBnkPtr);
                            fo = infp[count].bole;
                            /*rnd = 0;*/
                            if (!fo || (fo - 1 == person))
                            {
                                /*搜索到道具*/
                                /*rnd = 1;*/
                                if (pb == g_PlayerKing + 1)
                                {
                                    ResLoadToMem(STRING_CONST,STR_GETGOODS,str);
                                    GetGoodsName(p,astr);
                                    gam_strcat(str,astr);
                                    ShowGReport(person,str);
                                    pss = 1;
                                }
                                SetGoods(Order->City, p);
                            }
                        }
                        break;
                    }
                }
            }

            /*if (rnd)
             {
             pss = 1;
             if (pb == g_PlayerKing + 1)
             {
             ResLoadToMem(STRING_CONST,STR_GETNOTHING,str);
             ShowGReport(person,str);
             }
             }*/
            break;
        case 2:
            rnd = 10 + gam_rand() % (iq * 2);
            if (g_engineConfig.fixOverFlow16) {
                ADD16(g_Cities[Order->City].Money, rnd);
            } else {
                g_Cities[Order->City].Money += rnd;
            }
            break;
        case 3:
            rnd = 10 + gam_rand() % (iq * 2);
            if (g_engineConfig.fixOverFlow16) {
                ADD16(g_Cities[Order->City].Food, rnd);
            } else {
                g_Cities[Order->City].Food += rnd;
            }
            break;
    }
    while (pb == g_PlayerKing + 1)
    {
        if (1 == pss)
            break;

        if (!pss)
        {
            ResLoadToMem(STRING_CONST,STR_GETNOTHING,str);
        }
        else
        {
            ResLoadToMem(STRING_CONST,STR_GETSOME1 - 2 + pss,str);
            gam_itoa(rnd,astr,10);
            gam_strcat(str,astr);
        }
        ShowGReport(person,str);
        break;
    }

    AddPerson(Order->City,person);
    return(1);
}

/******************************************************************************
 * 函数名:FatherDrv
 * 说  明:“治理”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 FatherDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:InspectionDrv
 * 说  明:“出巡”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 InspectionDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:SurrenderDrv
 * 说  明:“招降”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 SurrenderDrv(OrderType *Order)
{
    U8 randint;
    U8 rade;
    U8 dev;
    PersonID p, ob;
    U8 flag;
    SBUF str;
    PersonType *pptr;

    p = Order->Person;
    ob = Order->Object;
    pptr = &g_Persons[ob];
    flag = 0;
    do
    {
        if (pptr->Belong == g_Persons[p].Belong)
        {
            AddPerson(Order->City,p);
            return(1);
        }

        if (g_engineDebug) {
            goto tagSuccess;
        }

        rade = g_Persons[p].IQ - pptr->IQ;
        rade += 50;
        randint = gam_rand() % 100;
        if (randint > rade)
            break;

        /*randint = gam_rand() % 100;
         if (randint < g_Persons[ob].Devotion)
         break;*/

        switch (pptr->Character)
        {
            case CHARACTER_LOYALISM:
                rade = SURRENDER_LOYALISM_MOD;
                break;
            case CHARACTER_IDEAL:
                rade = SURRENDER_IDEAL_MOD;
                break;
            case CHARACTER_AVARICE:
                rade = SURRENDER_AVARICE_MOD;
                break;
            case CHARACTER_DREAD:
                rade = SURRENDER_DREAD_MOD;
                break;
            case CHARACTER_TEMERITY:
                rade = SURRENDER_TEMERITY_MOD;
                break;
        }
        dev = pptr->Devotion;
        pptr->Devotion = dev - dev /10;
        if (dev > 60)
            break;

        rade = dev / rade;
        randint = gam_rand() % 100;
        if (randint < rade)
            break;

    tagSuccess:
        /*添加招降成功代码*/
        pptr->Belong = g_Persons[p].Belong;
        pptr->Devotion = 40 + gam_rand() % 40;
        flag = 1;
    }while (0);

    if (g_Persons[p].Belong == g_PlayerKing + 1)
    {
        if (flag)
        {
            WonPersonNoet(ob);
        }
        else
        {
            ResLoadToMem(STRING_CONST,P_SAY_STR1 + (gam_rand() % 3),str);
            ShowMapClear();
            ShowGReport(ob,str);
        }
    }

    AddPerson(Order->City,p);
    return(1);
}

/******************************************************************************
 * 函数名:ExchangeDrv
 * 说  明:“交易”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 ExchangeDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:TransportationDrv
 * 说  明:“输送”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:30	基本功能完成
 ******************************************************************************/
U8 TransportationDrv(OrderType *Order)
{
    PersonID ob;
    U8 rpi;
    SBUF str;

    if (gam_rand() % 100 > 20)
    {
        ob = Order->Object;

        if (g_engineConfig.fixOverFlow16) {
            ADD16(g_Cities[ob].Food, Order->Food);
            ADD16(g_Cities[ob].Money, Order->Money);
            ADD16(g_Cities[ob].MothballArms, Order->Arms);
        } else {
            g_Cities[ob].Food += Order->Food;
            g_Cities[ob].Money += Order->Money;
            g_Cities[ob].MothballArms += Order->Arms;
        }
        rpi = STR_RP9;
    }
    else
    {
        rpi = STR_RP10;
    }
    ResLoadToMem(STRING_CONST,rpi,str);
    ShowGReport(Order->Person,str);


    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:MoveDrv
 * 说  明:“移动”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 MoveDrv(OrderType *Order)
{
    PersonID pb;
    PersonID *b;
    U32 ob;

    ob = Order->Object;
    b = &g_Cities[ob].Belong;
    pb = g_Persons[Order->Person].Belong;
    if (!(*b))
    {
        *b = pb;
        /*g_Cities[ob].SatrapId = Order->Person + 1;*/
    }
    else if ((*b) != pb)
    {
        ob = Order->City;
    }

    AddPerson(ob,Order->Person);
    return(1);
}


/******************************************************************************
 * 函数名:AlienateDrv
 * 说  明:“离间”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 AlienateDrv(OrderType *Order)
{
    U8 randint;
    U8 rade;
    PersonID ob;
    static const char* sucessDialogs[] = {
        /* 主多疑而不用，吾当如何处之！ */
        "\xd6\xf7\xb6\xe0\xd2\xc9\xb6\xf8\xb2\xbb\xd3\xc3\xa3\xac\xce\xe1\xb5\xb1\xc8\xe7\xba\xce\xb4\xa6\xd6\xae\xa3\xa1",
        /* 赏罚不明，何以服众！ */
        "\xc9\xcd\xb7\xa3\xb2\xbb\xc3\xf7\xa3\xac\xba\xce\xd2\xd4\xb7\xfe\xd6\xda\xa3\xa1",
        /* 优柔寡断，非明主之像！ */
        "\xd3\xc5\xc8\xe1\xb9\xd1\xb6\xcf\xa3\xac\xb7\xc7\xc3\xf7\xd6\xf7\xd6\xae\xcf\xf1\xa3\xa1",
        /* 鼠目寸光，岂为明主邪！ */
        "\xca\xf3\xc4\xbf\xb4\xe7\xb9\xe2\xa3\xac\xc6\xf1\xce\xaa\xc3\xf7\xd6\xf7\xd0\xb0\xa3\xa1",
    };

    static const char* failDialogs[] = {
        /* 天选之子，吾誓死守护！ */
        "\xcc\xec\xd1\xa1\xd6\xae\xd7\xd3\xa3\xac\xce\xe1\xca\xc4\xcb\xc0\xca\xd8\xbb\xa4\xa3\xa1",
        /* 鼠辈安敢行此卑鄙之事! */
        "\xca\xf3\xb1\xb2\xb0\xb2\xb8\xd2\xd0\xd0\xb4\xcb\xb1\xb0\xb1\xc9\xd6\xae\xca\xc2!",
        /* 疏不间亲，阁下自重！ */
        "\xca\xe8\xb2\xbb\xbc\xe4\xc7\xd7\xa3\xac\xb8\xf3\xcf\xc2\xd7\xd4\xd6\xd8\xa3\xa1",
    };

    ob = Order->Object;

    do
    {
        rade = g_Persons[Order->Person].IQ - g_Persons[ob].IQ;
        rade += 50;
        randint = gam_rand() % 100;
        if (randint > rade)
            break;

        randint = gam_rand() % 100;
        if (randint < g_Persons[ob].Devotion)
            break;

        switch (g_Persons[ob].Character)
        {
            case CHARACTER_LOYALISM:
                rade = ALIENATE_LOYALISM;
                break;
            case CHARACTER_IDEAL:
                rade = ALIENATE_IDEAL;
                break;
            case CHARACTER_AVARICE:
                rade = ALIENATE_AVARICE;
                break;
            case CHARACTER_DREAD:
                rade = ALIENATE_DREAD;
                break;
            case CHARACTER_TEMERITY:
                rade = ALIENATE_TEMERITY;
                break;
        }
        randint = gam_rand() % 100;
        if (randint > rade)
            break;


        /*添加离间成功代码*/
        if (g_Persons[ob].Devotion > 4)
            g_Persons[ob].Devotion -= 4;
        else
            g_Persons[ob].Devotion = 0;
        U8 devo = g_Persons[ob].Devotion;
        U8 ind = 0;
        if (devo > 80) {
            ind = 0;
        } else if (devo > 60) {
            ind = 1;
        } else if (devo > 40) {
            ind = 2;
        } else {
            ind = 3;
        }
        ShowDialog(Order->Person, ob, (U8*)sucessDialogs[ind]);
        goto tagRet;
    }while (0);

    {
        U8 devo = g_Persons[ob].Devotion;
        U8 ind = 0;
        if (devo > 80) {
            ind = 0;
        } else if (devo > 60) {
            ind = 1;
        } else {
            ind = 2;
        }
        ShowDialog(Order->Person, ob, (U8*)failDialogs[ind]);
    }
tagRet:
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:CanvassDrv
 * 说  明:“招揽”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 CanvassDrv(OrderType *Order)
{
    U8 randint;
    U8 rade;
    U8 c;
    PersonID o,p;

    o = Order->Object;
    p = Order->Person;
    do
    {
        rade = g_Persons[p].IQ - g_Persons[o].IQ;
        randint = gam_rand() % 100;
        if ((randint + 100) > (rade + 100))
            break;

        randint = gam_rand() % 100;
        if (randint < g_Persons[o].Devotion)
            break;

        switch (g_Persons[o].Character)
        {
            case CHARACTER_LOYALISM:
                rade = CANVASS_LOYALISM;
                break;
            case CHARACTER_IDEAL:
                rade = CANVASS_IDEAL;
                break;
            case CHARACTER_AVARICE:
                rade = CANVASS_AVARICE;
                break;
            case CHARACTER_DREAD:
                rade = CANVASS_DREAD;
                break;
            case CHARACTER_TEMERITY:
                rade = CANVASS_TEMERITY;
                break;
        }
        randint = gam_rand() % 100;
        if (randint > rade)
            break;

        c = GetPersonCity(o);
        if (0xff == c)
            break;
        /*添加招揽成功代码*/
        DelPerson(c,o);
        AddPerson(Order->City,o);
        g_Persons[o].Belong = g_Persons[p].Belong;
        g_Persons[o].Devotion = 40 + gam_rand() % 40;
        {
            static const char* dialogs[] = {
                /* 良禽择木而栖，贤臣择主而仕 */
                "\xc1\xbc\xc7\xdd\xd4\xf1\xc4\xbe\xb6\xf8\xc6\xdc\xa3\xac\xcf\xcd\xb3\xbc\xd4\xf1\xd6\xf7\xb6\xf8\xca\xcb",
                /* 伏处一方，唯待明主，其在君乎? */
                "\xb7\xfc\xb4\xa6\xd2\xbb\xb7\xbd\xa3\xac\xce\xa8\xb4\xfd\xc3\xf7\xd6\xf7\xa3\xac\xc6\xe4\xd4\xda\xbe\xfd\xba\xf5?",
                /* 闻君贤名久矣，愿为君牵马坠镫！ */
                "\xce\xc5\xbe\xfd\xcf\xcd\xc3\xfb\xbe\xc3\xd2\xd3\xa3\xac\xd4\xb8\xce\xaa\xbe\xfd\xc7\xa3\xc2\xed\xd7\xb9\xef\xeb\xa3\xa1",
                /* 固所愿也，不敢请尔! */
                "\xb9\xcc\xcb\xf9\xd4\xb8\xd2\xb2\xa3\xac\xb2\xbb\xb8\xd2\xc7\xeb\xb6\xfb!",
                /* 赴汤蹈火，在所不辞！ */
                "\xb8\xb0\xcc\xc0\xb5\xb8\xbb\xf0\xa3\xac\xd4\xda\xcb\xf9\xb2\xbb\xb4\xc7\xa3\xa1",
            };
            ShowDialogRandom(Order->Person, o, (U8 **)dialogs, sizeof(dialogs)/sizeof(*dialogs));
        }
        goto ret;
    }while (0);
    {
        static const char* dialogs[] = {
            /* 燕雀安知鸿鹄之志哉! */
            "\xd1\xe0\xc8\xb8\xb0\xb2\xd6\xaa\xba\xe8\xf0\xc0\xd6\xae\xd6\xbe\xd4\xd5!",
            /* 无能之辈，焉敢如此！ */
            "\xce\xde\xc4\xdc\xd6\xae\xb1\xb2\xa3\xac\xd1\xc9\xb8\xd2\xc8\xe7\xb4\xcb\xa3\xa1",
            /* 忠臣不仕二主！ */
            "\xd6\xd2\xb3\xbc\xb2\xbb\xca\xcb\xb6\xfe\xd6\xf7\xa3\xa1",
        };
        ShowDialogRandom(Order->Person, o, (U8 **)dialogs, sizeof(dialogs)/sizeof(*dialogs));
    }
ret:
    AddPerson(Order->City,p);
    return(1);
}

/******************************************************************************
 * 函数名:CounterespiongeDrv
 * 说  明:“策反”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 CounterespiongeDrv(OrderType *Order)
{
    U32 randint;
    U32 rade;
    PersonID *pqptr;
    U32 i,pcount,c;
    PersonID o;

    o = Order->Object;
    do
    {
        rade = g_Persons[Order->Person].IQ - g_Persons[o].IQ;
        rade += 50;
        randint = gam_rand() % 100;
        if (randint > rade)
            break;

        randint = gam_rand() % 100;
        if (randint < g_Persons[o].Devotion)
            break;

        switch (g_Persons[o].Character)
        {
            case CHARACTER_LOYALISM:
                rade = COUNTERESPIONGE_LOYALISM;
                break;
            case CHARACTER_IDEAL:
                rade = COUNTERESPIONGE_IDEAL;
                break;
            case CHARACTER_AVARICE:
                rade = COUNTERESPIONGE_AVARICE;
                break;
            case CHARACTER_DREAD:
                rade = COUNTERESPIONGE_DREAD;
                break;
            case CHARACTER_TEMERITY:
                rade = COUNTERESPIONGE_TEMERITY;
                break;
        }
        randint = gam_rand() % 100;
        if (randint > rade)
            break;

        c = GetPersonCity(o);
        if (0xff == c)
            break;
        /*添加策反成功代码*/
        g_Persons[o].Belong = PID(o + 1);
        pqptr = (PersonID*)SHARE_MEM;
        pcount = GetCityPersons(c,pqptr);
        g_Cities[c].Belong = PID(o + 1);
        for (i = 0;i < pcount;i ++)
        {
            g_Persons[pqptr[i]].Belong = PID(o + 1);
        }
        {
            static const char* dialogs[] = {
                /* 王侯将相，宁有种乎！ */
                "\xcd\xf5\xba\xee\xbd\xab\xcf\xe0\xa3\xac\xc4\xfe\xd3\xd0\xd6\xd6\xba\xf5\xa3\xa1",
                /* 揭竿而起，为民请命！ */
                "\xbd\xd2\xb8\xcd\xb6\xf8\xc6\xf0\xa3\xac\xce\xaa\xc3\xf1\xc7\xeb\xc3\xfc\xa3\xa1",
                /* 积粮聚兵，逐鹿天下！ */
                "\xbb\xfd\xc1\xb8\xbe\xdb\xb1\xf8\xa3\xac\xd6\xf0\xc2\xb9\xcc\xec\xcf\xc2\xa3\xa1",
            };
            ShowDialogRandom(Order->Person, o, (U8 **)dialogs, sizeof(dialogs)/sizeof(*dialogs));
        }
        BeNewKingNote(o);
    }while (0);
    {
        static const char* dialogs[] = {
            /* 君臣相得，岂屑小所可趁也！ */
            "\xbe\xfd\xb3\xbc\xcf\xe0\xb5\xc3\xa3\xac\xc6\xf1\xd0\xbc\xd0\xa1\xcb\xf9\xbf\xc9\xb3\xc3\xd2\xb2\xa3\xa1",
            /* 赤胆忠心，誓佐明主定天下！ */
            "\xb3\xe0\xb5\xa8\xd6\xd2\xd0\xc4\xa3\xac\xca\xc4\xd7\xf4\xc3\xf7\xd6\xf7\xb6\xa8\xcc\xec\xcf\xc2\xa3\xa1",
        };
        ShowDialogRandom(Order->Person, o, (U8 **)dialogs, sizeof(dialogs)/sizeof(*dialogs));
    }
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:RealienateDrv
 * 说  明:“反间”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 RealienateDrv(OrderType *Order)
{
    return(1);
}

/******************************************************************************
 * 函数名:InduceDrv
 * 说  明:“劝降”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 InduceDrv(OrderType *Order)
{
    SBUF str, astr;
    U32 randint;
    U32 rade;
    U8 wf;
    U8 *cqptr;
    PersonID *pqptr;
    U32 i,j,count,pcount;
    PersonID p,o,pb;

    p = Order->Person;
    o = Order->Object;
    wf = 0;
    cqptr = SHARE_MEM;
    do
    {
        if (o == g_PlayerKing)
            break;
        pb = g_Persons[p].Belong;
        if (pb == g_PlayerKing + 1 && g_engineDebug)
            goto suc;
        if (GetKingCitys(PID(pb - 1),cqptr) < (GetKingCitys(PID(g_Persons[o].Belong - 1),cqptr) * 2))
            break;

        rade = g_Persons[p].IQ - g_Persons[o].IQ;
        rade += 50;
        randint = gam_rand() % 100;
        if (randint > rade)
            break;

        /*randint = gam_rand() % 100;
         if (randint < g_Persons[Order->Object].Devotion)
         break;*/

        switch (g_Persons[o].Character)
        {
            case CHARACTER_PEACE:
                rade = PERSUADE_PEACE;
                break;
            case CHARACTER_JUSTICE:
                rade = PERSUADE_JUSTICE;
                break;
            case CHARACTER_DUPLICITY:
                rade = PERSUADE_DUPLICITY;
                break;
            case CHARACTER_CRAZY:
                rade = PERSUADE_CRAZY;
                break;
            case CHARACTER_RASH:
                rade = PERSUADE_RASH;
                break;
        }
        randint = gam_rand() % 100;
        if (randint > rade)
            break;
    suc:

        /*添加劝降成功代码*/
        pqptr =(PersonID*)(SHARE_MEM + CITY_MAX);
        count = GetKingCitys(o,cqptr);
        for (i = 0;i < count;i ++)
        {
            pcount = GetCityPersons(cqptr[i],pqptr);
            g_Cities[cqptr[i]].Belong = pb;
            for (j = 0;j < pcount;j ++)
            {
                g_Persons[pqptr[j]].Belong = pb;
            }
        }

        // 处理不在城中的人员
        for (i = 0;i < PERSON_COUNT;i ++)
        {
            PersonID belong = g_Persons[i].Belong;
            if (!belong) continue;

            belong -= 1;
            if (belong == o) {
                if (belong == i) {
                    g_Persons[i].Belong = pb;
                } else {
                    g_Persons[i].Belong = 0;
                }
            }
        }
        wf = 1;
    }while (0);

    if (pb == g_PlayerKing + 1)
    {
        if (wf)
        {
            wf = P_SAY_STR36;
        }
        else
        {
            wf = P_SAY_STR32;
        }
        ResLoadToMem(STRING_CONST,wf + (gam_rand() % 4),str);
        ShowMapClear();
        ShowGReport(o,str);
    }
    else if (wf)
    {
        GetPersonName(o,str);
        ResLoadToMem(STRING_CONST,STR_INDUCENOTE,astr);
        gam_strcat(str,astr);
        GetPersonName(PID(pb - 1),astr);
        gam_strcat(str,astr);
        GamMsgBox(str,2);
    }
    
    
    AddPerson(Order->City,p);
    return(1);
}

/******************************************************************************
 * 函数名:TributeDrv
 * 说  明:“朝贡”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
/*U8 TributeDrv(OrderType *Order)
 {
	return(1);
 }*/

/******************************************************************************
 * 函数名:ReconnoitreDrv
 * 说  明:“侦察”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 ReconnoitreDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:ConscriptionDrv
 * 说  明:“征兵”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 ConscriptionDrv(OrderType *Order)
{
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:DepredateDrv
 * 说  明:“掠夺”命令驱动
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 17:31	基本功能完成
 ******************************************************************************/
U8 DepredateDrv(OrderType *Order)
{
    SBUF str,astr;
    U16 valf,valm;
    CityType *cptr;
    
    cptr = &g_Cities[Order->City];
    cptr->PeopleDevotion /= 2;
    cptr->Farming /= 2;
    cptr->Commerce /= 2;
    valf = g_Persons[Order->Person].IQ + g_Persons[Order->Person].Force;
    valm = valf * 2;
    valf *= 5;
    if (g_engineConfig.fixOverFlow16) {
        ADD16(cptr->Food, valf);
        ADD16(cptr->Money, valm);
    } else {
        cptr->Food += valf;
        cptr->Money += valm;
    }
    if (g_Persons[Order->Person].Belong == g_PlayerKing + 1)
    {
        ResLoadToMem(STRING_CONST,P_SAY_STR28,str);
        gam_itoa(valm,astr,10);
        gam_strcat(str,astr);
        ResLoadToMem(STRING_CONST,P_SAY_STR29,astr);
        gam_strcat(str,astr);
        gam_itoa(valf,astr,10);
        gam_strcat(str,astr);
        ResLoadToMem(STRING_CONST,P_SAY_STR30,astr);
        gam_strcat(str,astr);
        ShowGReport(Order->Person,str);
    }
    
    AddPerson(Order->City,Order->Person);
    return(1);
}

/******************************************************************************
 * 函数名:OrderExec
 * 说  明:执行命令
 *
 * 入口参数：Order -- 待执行命令
 *
 * 出口参数：1 -- 成功，0 -- 失败
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-3 16:55	基本功能完成
 ******************************************************************************/
U8 OrderExec(OrderType *Order)
{
    {
#define _ST OrderType
        static Field _fields[] = {
            _FIELD_RW(OrderId, U8),
            _FIELD_RW(Person, U16),
            _FIELD_RW(City, U8),
            _FIELD_RW(Object, U16),
            _FIELD_RW(Arms, U16),
            _FIELD_RW(Food, U16),
            _FIELD_RW(Money, U16),
            _FIELD_RW(Consume, U8),
            _FIELD_RW(TimeCount, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
        static Value context = {.def = &_value_def };
#undef _ST

        context.offset = (U32)Order;
        if (call_hook_a("willExecuteOrder", &context) == 0){
            return 0;
        }
    }
    switch (Order->OrderId)
    {
            /*内政指令*/
        case NOP:
            NopDrv(Order);
            break;
        case ASSART:		/*开垦*/
            AssartDrv(Order);
            break;
        case ACCRACTBUSINESS:	/*招商*/
            AccractbusinessDrv(Order);
            break;
        case SEARCH:		/*搜寻*/
            SearchDrv(Order);
            break;
        case FATHER:		/*治理*/
            FatherDrv(Order);
            break;
        case INSPECTION:	/*出巡*/
            InspectionDrv(Order);
            break;
        case SURRENDER:		/*招降*/
            SurrenderDrv(Order);
            break;
        case KILL:		/*处斩*/
            break;
        case BANISH:		/*流放*/
            break;
        case LARGESS:		/*赏赐*/
            break;
        case CONFISCATE:	/*没收*/
            break;
        case EXCHANGE:		/*交易*/
            ExchangeDrv(Order);
            break;
        case TREAT:		/*宴请*/
            break;
        case TRANSPORTATION:	/*输送*/
            TransportationDrv(Order);
            break;
        case MOVE:		/*移动*/
            MoveDrv(Order);
            break;
            
            /*外交指令*/
        case ALIENATE:		/*离间*/
            AlienateDrv(Order);
            break;
        case CANVASS:		/*招揽*/
            CanvassDrv(Order);
            break;
        case COUNTERESPIONAGE:	/*策反*/
            CounterespiongeDrv(Order);
            break;
        case REALIENATE:	/*反间*/
            RealienateDrv(Order);
            break;
        case INDUCE:		/*劝降*/
            InduceDrv(Order);
            break;
            /*case TRIBUTE:		朝贡
             TributeDrv(Order);
             break;	*/
            
            /*军备指令*/
        case RECONNOITRE:	/*侦察*/
            ReconnoitreDrv(Order);
            break;
        case CONSCRIPTION:	/*征兵*/
            ConscriptionDrv(Order);
            break;
        case DISTRIBUTE:	/*分配*/
            break;
        case DEPREDATE:		/*掠夺*/
            DepredateDrv(Order);
            break;
        case BATTLE:		/*出征*/
            BattleDrv(Order);
            break;
    }
    return(1);
}

/******************************************************************************
 * 函数名:PolicyExec
 * 说  明:命令队列执行
 *
 * 入口参数：无
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-17 11:22	基本功能完成
 ******************************************************************************/
FAR void PolicyExec(void)
{
    /*OrderQueueType *inode,*nnode;
     OrderType eOrder;*/
    
    U8 i;
    OrderType *io;
    
    io = (OrderType *) ORDERQUEUE;
    for (i = 0;i < ORDER_MAX;i ++)
    {
        if (0xff == io[i].OrderId)
            continue;
        
        
        OrderExec(&io[i]);
        DelOrder(&io[i]);
    }
    
    /*inode = g_OrderHead;
     while (1)
     {
     if (NULL == inode)
     break;
     
     nnode = inode->nOrder;
     if (!(inode->iOrder.TimeCount))
     {
     gam_memcpy((U8 *) &eOrder,(U8 *) &(inode->iOrder),sizeof(OrderType));
     DelOrder(inode);
     OrderExec(&eOrder);
     }
     else
     {
     inode->iOrder.TimeCount -= 1;
     }
     inode = nnode;
     }*/
}
