/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	showface.c
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
 *	1.0    2005-6-10  陈泽伟     基本的功能完成
 ***********************************************************************/

#include "baye/stdsys.h"
#include "baye/comm.h"
#undef	SHOWFACE_C
#define	SHOWFACE_C
#include "baye/enghead.h"
#include "touch.h"
#include "baye/bind-objects.h"

/*void GetCityProStr(U8 city,U8 pro,U8 *str);
 void GetPersonProStr(U8 person,U8 pro,U8 *str);
 void GetPersonName(U8 person,U8 *str);*/

typedef struct{
    U8 personPropertiesDisplayWitdh[256];
    U8 personPropertiesCount;
    U8 toolPropertiesDisplayWitdh[256];
    U8 toolPropertiesCount;
    U8 cityPropertiesCount;
} UICfg;

static UICfg cfg = {
    .personPropertiesDisplayWitdh = {8, 10, 4, 4, 4, 4, 4, 4, 4, 5, 4, 10, 10},
    .personPropertiesCount  = PERSON_PROP_COUNT,
    .toolPropertiesDisplayWitdh = {4, 6, 6, 6, 6},
    .toolPropertiesCount  = GOODS_PROP_COUNT,
    .cityPropertiesCount  = 11,
};


void bind_show_face(ObjectDef* global_def)
{
    ObjectDef* def = ObjectDef_new();
    ObjectDef_addFieldArray(def, "personPropertiesDisplayWitdh",
                            ValueTypeU8, &cfg.personPropertiesDisplayWitdh, sizeof(cfg.personPropertiesDisplayWitdh));
    ObjectDef_addFieldArray(def, "toolPropertiesDisplayWitdh",
                            ValueTypeU8, &cfg.toolPropertiesDisplayWitdh, sizeof(cfg.toolPropertiesDisplayWitdh));
    BIND_U8EX("personPropertiesCount", &cfg.personPropertiesCount);
    BIND_U8EX("toolPropertiesCount", &cfg.toolPropertiesCount);
    BIND_U8EX("cityPropertiesCount", &cfg.cityPropertiesCount);

    ObjectDef_addFieldF(global_def, "g_uiCfg", ValueTypeObject, 0, def, 0);
}



/******************************************************************************
 * 函数名:ShowGoodsPro
 * 说  明:显示道具属性
 *
 * 入口参数：person -- 道具编号，pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
void ShowGoodsPro(ToolID goods,U8 pro,U8 x,U8 y,U8 wid)
{
    U8 sx,sy;
    U8 str[128];
    U8 i;
    U8 *ptr = cfg.toolPropertiesDisplayWitdh;
    PosItemType positem;

    InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);

    if (AddItem(ASC_WID * 10 + 1,ASC_HGT,&positem,&sx,&sy))
    {
        GetGoodsName(goods,str);
        PlcMidShowStr(sx + ASC_WID * 10 / 2,sy,str);
        /*GamStrShowS(sx,sy,str);*/
    }
    else
    {
        return;
    }

    for (i = pro; i < cfg.toolPropertiesCount; i++)
    {
        if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
        {
            GetGoodsProStr(goods,i,str);
            PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
            /*GamStrShowS(sx,sy,str);*/
        }
        else
        {
            break;
        }
    }
}


/******************************************************************************
 * 函数名:GetGoodsProStr
 * 说  明:获取道具属性值字符串
 *
 * 入口参数：goods -- 道具编号，pro -- 属性编号，str -- 属性字符串
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 14:57	基本功能完成
 ******************************************************************************/
void GetGoodsProStr(ToolID goods,U8 pro,U8 *str)
{
    U32 idx = '\0';
    GOODS *gptr;

    str[0] = 0;
    IF_HAS_HOOK("getToolPropertyValue") {
        U8* value = str;
        BIND_U16EX("toolIndex", &goods);
        BIND_U8EX("propertyIndex", &pro);
        BIND_GBKARR(value, 128);

        if (CALL_HOOK() == 0) HOOK_RETURN();
    }

    gptr = (GOODS *) ResLoadToCon(GOODS_RESID,1,g_CBnkPtr);
    gptr = &gptr[goods];
    switch (pro)
    {
        case 0:		/*用法*/
            switch (gptr->useflag)
        {
            case 0:
                idx = GOODS_USEFLAG0;
                break;
            case 1:
                idx = GOODS_USEFLAG1;
                break;
        }
            ResLoadToMem(STRING_CONST,idx,str);
            break;
        case 1:		/*加武力*/
            gam_itoa(gptr->at,str,10);
            break;
        case 2:		/*加智力*/
            gam_itoa(gptr->iq,str,10);
            break;
        case 3:		/*加移动*/
            gam_itoa(gptr->move,str,10);
            break;
        case 4:		/*改兵种*/
            switch (gptr->arm)
        {
            case 0:		/*无*/
                idx = ATRR_STR70;
                break;
            case 1:		/*水兵*/
                idx = GOODS_ARM1;
                break;
            case 2:		/*玄兵*/
                idx = GOODS_ARM2;
                break;
            case 3:		/*极兵*/
                idx = GOODS_ARM3;
                break;
            default:
                idx = ATRR_STR11 + gptr->arm-4;
                break;
        }
            ResLoadToMem(STRING_CONST,idx,str);
            break;
    }
}

/******************************************************************************
 * 函数名:ShowGoodsProStr
 * 说  明:显示道具属性字符串
 *
 * 入口参数：pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
 *
 * 出口参数：下一个显示属性编号
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
U8 ShowGoodsProStr(U8 pro,U8 x,U8 y,U8 wid)
{
    U8 sx,sy;
    U8 str[128];
    U8 *ptr = cfg.toolPropertiesDisplayWitdh;
    U8 i;
    PosItemType positem;

    InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);

    if (AddItem(ASC_WID * 10 + 1,ASC_HGT,&positem,&sx,&sy))
    {
        ResLoadToMem(STRING_CONST,GOODS_ATRR_STR0,str);
        PlcMidShowStr(sx + ASC_WID * 10 / 2,sy,str);
        /*GamStrShowS(sx,sy,str);*/
    }
    else
    {
        return(0);
    }

    for (i = pro; i < cfg.toolPropertiesCount; i ++)
    {
        if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
        {
            str[0] = 0;

            IF_HAS_HOOK("getToolPropertyTitle") {
                U8* title = str;
                BIND_U8EX("propertyIndex", &i);
                BIND_GBKARR(title, 128);
                CALL_HOOK();
            }
            if (str[0] == 0) {
                ResLoadToMem(STRING_CONST,GOODS_ATRR_STR1 + i,str);
            }
            PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
            /*GamStrShowS(sx,sy,str);*/
        }
        else
        {
            return(i);
        }
    }
    return(i);

}

/******************************************************************************
 * 函数名:ShowGoodsControl
 * 说  明:显示、选择武将
 *
 * 入口参数：goods -- 道具队列，gcount -- 道具总数，x0 y0 x1 y1 -- 显示范围
 *
 * 出口参数：0xff -- 退出，其他值 -- 选中武将序号
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR ToolID ShowGoodsControlInner(ToolID *goods,ToolID gcount, ToolID init, U8 x0,U8 y0,U8 x1,U8 y1);
FAR ToolID ShowGoodsControl(ToolID *goods,ToolID gcount, ToolID init, U8 x0,U8 y0,U8 x1,U8 y1) {
    int prev = SysScrollingTimerOpen(0);
    ToolID rv = ShowGoodsControlInner(goods, gcount, init, x0, y0, x1, y1);
    SysScrollingTimerOpen(prev);
    return rv;
}
FAR ToolID ShowGoodsControlInner(ToolID *goods,ToolID gcount, ToolID init, U8 x0,U8 y0,U8 x1,U8 y1)
{
    U32 i,showflag,count,top,set;
    U8 spc,spcv[6];
    U8 wid;
    GMType Msg;
    Touch touch;
    U8 topWhenTouchDown = 0;
    U8 leftWhenTouchDown = 0;

    if (!gcount)
        return TID(0xffff);

    /*gam_clrlcd(x0,y0,x1,y1);*/

    wid = x1 - x0 + 1;
    count = (y1 - y0 + 1) / ASC_HGT - 1;
    if (count > gcount)
    {
        count = gcount;
    }
    y1 = y0 + count * ASC_HGT + ASC_HGT;

    Rect menuRect = {
        .left = x0,
        .top = y0,
        .right = x0 + ASC_WID * 10 - 1,
        .bottom = y1
    };

    gam_rect(x0,y0,x1,y1 + 1);
    x0 += 1;
    y0 += 1;
    x1 -= 1;

    top = init;
    top = limitValueInRange(top, 0, gcount-count);
    set = init;
    set = limitValueInRange(set, 0, gcount-1);
    spc = 0;
    spcv[0] = 0;
    showflag = 1;
    while (1)
    {
        if (showflag)
        {
            gam_clrlcd(x0,y0,x1,y0 + count * ASC_HGT + ASC_HGT - 1);
            spcv[spc + 1] = ShowGoodsProStr(spcv[spc],x0,y0,wid);
            for (i = 0;i < count;i ++)
            {
                if (i >= gcount)
                    break;

                ShowGoodsPro(goods[top + i],spcv[spc],x0,y0 + ASC_HGT * i + ASC_HGT,wid);
            }
            if (set >= top && set < top + count) {
                gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 10 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
            }
            showflag = 0;
        }

        GamGetMsg(&Msg);

        if (VM_CHAR_FUN == Msg.type)
        {
            switch (Msg.param)
            {
                case VK_UP:
                    if (set)
                    {
                        set -= 1;
                        if (set < top)
                        {
                            top = set;
                        }
                        showflag = 1;
                    }
                    break;
                case VK_DOWN:
                    if (set < gcount - 1)
                    {
                        set += 1;
                        if (set >= top + count)
                        {
                            top = set + 1 - count;
                        }
                        showflag = 1;
                    }
                    break;
                case VK_LEFT:
                    if (spcv[spc])
                    {
                        spc -= 1;
                        showflag = 1;
                    }
                    break;
                case VK_RIGHT:
                    if (spcv[spc + 1] < cfg.toolPropertiesCount)
                    {
                        spc += 1;
                        showflag = 1;
                    }
                    break;
                case VK_ENTER:
                    return TID(set);
                case VK_EXIT:
                    return TID(0xffff);
            }
        } else if (VM_TOUCH == Msg.type) {
            touchUpdate(&touch, Msg);
            switch (Msg.param) {
                case VT_TOUCH_DOWN:
                    topWhenTouchDown = top;
                    leftWhenTouchDown = spc;
                    I16 index = touchListViewItemIndexAtPoint(touch.currentX, touch.currentY, menuRect, 1+ASC_HGT, 1, top, gcount, ASC_HGT);
                    if (index >= 0 && set != index) {
                        set = index;
                        showflag = 1;
                    }
                    break;
                case VT_TOUCH_UP:
                {
                    if (!touch.completed || touch.moved) break;
                    I16 index = touchListViewItemIndexAtPoint(touch.currentX, touch.currentY, menuRect, 1+ASC_HGT, 1, top, gcount, ASC_HGT);
                    if (index < 0)
                    {
                        return TID(0xffff);
                    }
                    return TID(index);
                }
                case VT_TOUCH_MOVE:
                {
                    if (!touch.touched) break;
                    U8 zMax = 100;

                    Point p = touchListViewCalcTopLeftForMove(&touch, leftWhenTouchDown, zMax, 30, topWhenTouchDown, gcount - count, ASC_HGT);

                    if (spc != p.x || top != p.y) {
                        top = p.y;
                        if (p.x > spc) {
                            if (spcv[spc + 1] < cfg.toolPropertiesCount) {
                                spc = spc + 1;
                            }
                        } else {
                            spc = p.x;
                        }
                        showflag = 1;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }

    return TID(0xffff);
}


/******************************************************************************
 * 函数名:GetGoodsName
 * 说  明:获取道具名称
 *
 * 入口参数：goods -- 道具编号，str -- 武将名称
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-7-28 14:45	基本功能完成
 ******************************************************************************/
FAR void GetGoodsName(ToolID goods,U8 *str)
{
    /*GOODS *gptr;

     gptr = (GOODS *) ResLoadToCon(GOODS_RESID,1,g_CBnkPtr);
     gam_memcpy(str,gptr[goods].name,10);
     str[10] = 0;*/
    ResLoadToMem(GOODS_NAME,goods + 1,str);
}

/******************************************************************************
 * 函数名:GetPersonName
 * 说  明:获取武将名称
 *
 * 入口参数：person -- 武将编号，str -- 武将名称
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-10 10:16	基本功能完成
 ******************************************************************************/
FAR void GetPersonName(PersonID person,U8 *str)
{
    U8 l = '\0';

    IF_HAS_HOOK("getPersonName") {
        U16 personIndex = person;
        U8 name[12] = "";
        BIND_U16(&personIndex);

        BIND_GBKARR(name, sizeof(name));
        if (CALL_HOOK() == 0) {
            memcpy(str, name, sizeof(name));
            HOOK_RETURN();
        }
    }

    switch (g_PIdx)
    {
        case 1:
            l = GENERAL_NAME;
            break;
        case 2:
            l = GENERAL_NAME2;
            break;
        case 3:
            l = GENERAL_NAME3;
            break;
        case 4:
            l = GENERAL_NAME4;
            break;
    }
    ResLoadToMem(l,person + 1,str);
    /***********************************************************/
    /*dptr = ResLoadToCon(GENERAL_NAME,g_PIdx,g_CBnkPtr);
     pnt = person;
     pnt *= 8;
     gam_memcpy(str,&dptr[pnt],8);
     str[8] = 0;*/
    /***********************************************************/
}

/******************************************************************************
 * 函数名:ShowPersonPro
 * 说  明:显示武将属性
 *
 * 入口参数：person -- 武将编号，pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
void ShowPersonPro(PersonID person,U8 pro,U8 x,U8 y,U8 wid)
{
    U8 sx,sy;
    U8 str[128];
    U8 *ptr = cfg.personPropertiesDisplayWitdh;
    U8 i;
    PosItemType positem;

    InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);

    if (AddItem(ASC_WID * 8 + 1,ASC_HGT,&positem,&sx,&sy))
    {
        GetPersonName(person,str);
        PlcMidShowStr(sx + ASC_WID * 8 / 2,sy,str);
        /*GamStrShowS(sx,sy,str);*/
    }
    else
    {
        return;
    }

    for (i = pro;i < cfg.personPropertiesCount;i ++)
    {
        if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
        {
            GetPersonProStr(person,i,str);
            PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
            /*GamStrShowS(sx,sy,str);*/
        }
        else
        {
            break;
        }
    }
}

/******************************************************************************
 * 函数名:GetPersonProStr
 * 说  明:获取武将属性值字符串
 *
 * 入口参数：person -- 武将编号，pro -- 属性编号，str -- 属性字符串
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
void GetPersonProStr(PersonID person,U8 pro,U8 *str)
{
    U8 idx = '\0';
    PersonID b;

    IF_HAS_HOOK("getPersonPropertyValue") {
        U8* value = str;

        BIND_U16EX("personIndex", &person);
        BIND_U8EX("propertyIndex", &pro);
        BIND_GBKARR(value, 128);

        if (CALL_HOOK() == 0) HOOK_RETURN();
    }


    b = g_Persons[person].Belong;
    switch (pro)
    {
        case 0:		/*归属*/
            if (0xffff == b)
            {
                ResLoadToMem(STRING_CONST,ATRR_STR69,str);
            }
            else if (b == person + 1)
            {
                ResLoadToMem(STRING_CONST,ATRR_STR68,str);
            }
            else if (b)
            {
                GetPersonName(PID(b - 1),str);
            }
            else
            {
                ResLoadToMem(STRING_CONST,ATRR_STR67,str);
            }
            break;
        case 1:		/*所在城市*/
            GetCityName(GetPersonCity(person),str);
            break;
        case 2:		/*等级*/
            gam_itoa(g_Persons[person].Level,str,10);
            break;
        case 3:		/*武力*/
            gam_itoa(g_Persons[person].Force,str,10);
            break;
        case 4:		/*智力*/
            gam_itoa(g_Persons[person].IQ,str,10);
            break;
        case 5:		/*忠诚*/
            if (b == (person + 1))
            {
                ResLoadToMem(STRING_CONST,ATRR_STR71,str);
            }
            else if (b)
            {
                gam_itoa(g_Persons[person].Devotion,str,10);
            }
            else
            {
                ResLoadToMem(STRING_CONST,ATRR_STR71,str);
            }

            break;
        case 6:		/*经验*/
            gam_itoa(g_Persons[person].Experience,str,10);
            break;
        case 7:		/*体力*/
            gam_itoa(g_Persons[person].Thew,str,10);
            break;
        case 8:		/*兵种*/
        {
            switch (GetArmType(&g_Persons[person]))
            {
                case 0:
                    idx = ATRR_STR11;
                    break;
                case 1:
                    idx = ATRR_STR12;
                    break;
                case 2:
                    idx = ATRR_STR13;
                    break;
                case 3:
                    idx = ATRR_STR14;
                    break;
                case 4:
                    idx = ATRR_STR15;
                    break;
                case 5:
                    idx = ATRR_STR16;
                    break;
            }
            ResLoadToMem(STRING_CONST,idx,str);
            break;
        }
        case 9:		/*兵力*/
            gam_itoa(g_Persons[person].Arms,str,10);
            break;
        case 10:		/*年龄*/
            gam_itoa(g_Persons[person].Age,str,10);
            break;
        case 11:		/*道具一*/
            str[0] = 0;
            if (g_Persons[person].Equip[0])
                GetGoodsName(TID(g_Persons[person].Equip[0] - 1),str);
            break;
        case 12:		/*道具二*/
            str[0] = 0;
            if (g_Persons[person].Equip[1])
                GetGoodsName(TID(g_Persons[person].Equip[1] - 1),str);
            break;
    }
}

/******************************************************************************
 * 函数名:ShowPersonProStr
 * 说  明:显示武将属性字符串
 *
 * 入口参数：pro -- 起始属性编号，x,y -- 显示坐标，wid -- 显示宽度
 *
 * 出口参数：下一个显示属性编号
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
U8 ShowPersonProStr(U8 pro,U8 x,U8 y,U8 wid)
{
    U8 sx,sy;
    U8 str[128];
    U8 *ptr = cfg.personPropertiesDisplayWitdh;
    U8 i;
    PosItemType positem;

    InitItem(x,y,x + wid - 1,y + ASC_HGT,&positem);

    if (AddItem(ASC_WID * 8 + 1,ASC_HGT,&positem,&sx,&sy))
    {
        ResLoadToMem(STRING_CONST,ATRR_STR17,str);
        PlcMidShowStr(sx + ASC_WID * 8 / 2,sy,str);
        /*GamStrShowS(sx,sy,str);*/
    }
    else
    {
        return(0);
    }

    for (i = pro; i < cfg.personPropertiesCount; i++)
    {
        if (AddItem(ASC_WID * ptr[i] + 1,ASC_HGT,&positem,&sx,&sy))
        {
            str[0] = 0;

            IF_HAS_HOOK("getPersonPropertyTitle") {
                U8 column = i;
                U8* value = str;

                BIND_U8EX("propertyIndex", &column);
                BIND_GBKARR(value, 128);
                
                CALL_HOOK();
            }

            if (str[0] == 0) ResLoadToMem(STRING_CONST,ATRR_STR18 + i,str);

            PlcMidShowStr(sx + ASC_WID * ptr[i] / 2,sy,str);
            /*GamStrShowS(sx,sy,str);*/
        }
        else
        {
            return(i);
        }
    }
    return(i);

}

/******************************************************************************
 * 函数名:ShowPersonControl
 * 说  明:显示、选择武将
 *
 * 入口参数：person -- 武将队列，pcount -- 武将总数, initSelected --初始选中序号，x0 y0 x1 y1 -- 显示范围
 *
 * 出口参数：0xff -- 退出，其他值 -- 选中武将序号
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR PersonID ShowPersonControlInner(PersonID *person,U32 pcount,PersonID initSelected,U8 x0,U8 y0,U8 x1,U8 y1);
FAR PersonID ShowPersonControl(PersonID *person,U32 pcount,PersonID initSelected,U8 x0,U8 y0,U8 x1,U8 y1) {
    int prev = SysScrollingTimerOpen(5);
    PersonID id = ShowPersonControlInner(person, pcount, initSelected, x0, y0, x1, y1);
    SysScrollingTimerOpen(prev);
    return id;
}
FAR PersonID ShowPersonControlInner(PersonID *person,U32 pcount,PersonID initSelected,U8 x0,U8 y0,U8 x1,U8 y1)
{
    U32 i,showflag,count,top,set;
    U8 spc,spcv[7];
    U8 wid;
    GMType Msg;
    Touch touch;
    U8 topWhenTouchDown = 0;
    U8 leftWhenTouchDown = 0;

    if (!pcount)
        return(PID(0xffff));

    /*gam_clrlcd(x0,y0,x1,y1);*/

    wid = x1 - x0 + 1 - 2;
    count = (y1 - y0 + 1 - 2) / ASC_HGT - 1;
    if (count > pcount)
    {
        count = pcount;
    }
    y1 = y0 + count * ASC_HGT + ASC_HGT;
    gam_rect(x0,y0,x1,y1 + 1);

    Rect menuRect = {
        .left = x0,
        .top = y0,
        .right = x0 + ASC_WID * 10 - 1,
        .bottom = y1
    };

    x0 += 1;
    y0 += 1;
    x1 -= 1;

    top = initSelected;
    top = limitValueInRange(top, 0, pcount-count);
    set = initSelected;
    set = limitValueInRange(set, 0, pcount-1);
    spc = 0;
    spcv[0] = 0;
    showflag = 1;
    while (1)
    {
        if (showflag)
        {
            gam_clrlcd(x0,y0,x1,y1);
            spcv[spc + 1] = ShowPersonProStr(spcv[spc],x0,y0,wid);
            for (i = 0;i < count;i ++)
            {
                if (i >= pcount)
                    break;

                ShowPersonPro(person[top + i],spcv[spc],x0,y0 + ASC_HGT * i + ASC_HGT,wid);
            }
            if (set >= top && set < top + count) {
                gam_revlcd(x0,y0 + (set - top + 1) * ASC_HGT,x0 + ASC_WID * 8 - 1,y0 + (set - top + 1) * ASC_HGT + ASC_HGT - 1);
            }
            showflag = 0;
        }

        GamGetMsg(&Msg);

        if (VM_CHAR_FUN == Msg.type)
        {
            touchUpdate(&touch, Msg);
            switch (Msg.param)
            {
                case VK_SEARCH:
                case VK_PGUP:
                    if (set)
                    {
                        if (set < count)
                            set = 0;
                        else
                            set -= (count - 1);
                        top = set;
                        showflag = 1;
                    }
                    break;
                case VK_UP:
                    if (set)
                    {
                        set -= 1;
                        if (set < top)
                        {
                            top = set;
                        }
                        showflag = 1;
                    }
                    break;
                case VK_HELP:
                case VK_PGDN:
                    if (set < pcount - 1)
                    {
                        set += (count - 1);
                        if (set >= pcount - 1)
                            set = pcount - 1;
                        showflag = 1;

                        top = set;
                        if (top > pcount - count) {
                            top = pcount - count;
                        }
                    }
                    break;
                case VK_DOWN:
                    if (set < pcount - 1)
                    {
                        set += 1;
                        if (set >= top + count)
                        {
                            top = set + 1 - count;
                        }
                        showflag = 1;
                    }
                    break;
                case VK_LEFT:
                    if (spcv[spc])
                    {
                        spc -= 1;
                        showflag = 1;
                    }
                    break;
                case VK_RIGHT:
                    if (spcv[spc + 1] < cfg.personPropertiesCount)
                    {
                        spc += 1;
                        showflag = 1;
                    }
                    break;
                case VK_ENTER:
                    return(PID(set));
                case VK_EXIT:
                    return(PID(0xffff));
            }
        } else if (VM_TOUCH == Msg.type) {
            touchUpdate(&touch, Msg);
            switch (Msg.param) {
                case VT_TOUCH_DOWN:
                    topWhenTouchDown = top;
                    leftWhenTouchDown = spc;
                    I16 index = touchListViewItemIndexAtPoint(touch.currentX, touch.currentY, menuRect, 1+ASC_HGT, 1, top, pcount, ASC_HGT);
                    if (index >= 0 && set != index) {
                        set = index;
                        showflag = 1;
                    }
                    break;
                case VT_TOUCH_UP:
                {
                    if (!touch.completed || touch.moved) break;
                    I16 index = touchListViewItemIndexAtPoint(touch.currentX, touch.currentY, menuRect, 1+ASC_HGT, 1, top, pcount, ASC_HGT);
                    if (index < 0)
                    {
                        return PID(0xffff);
                    }
                    return PID(index);
                }
                case VT_TOUCH_MOVE:
                {
                    if (!touch.touched) break;
moveView:
                    {
                        U8 xMax = 100;

                        Point p = touchListViewCalcTopLeftForMove(&touch, leftWhenTouchDown, xMax, 30, topWhenTouchDown, pcount - count, ASC_HGT);

                        touchUpdateViewState(
                            &touch,
                            3,
                            pointState(p.y, 0, pcount - count)
                        );
                        if (spc != p.x || top != p.y) {
                            top = p.y;
                            if (p.x > spc) {
                                if (spcv[spc + 1] < cfg.personPropertiesCount) {
                                    spc = spc + 1;
                                }
                            } else {
                                spc = p.x;
                            }
                            showflag = 1;
                        }
                        break;
                    }
                }
                default:
                    break;
            }
        } else if (GamMsgIsTimer0(Msg)) {
            if (touchUpdate(&touch, Msg)) {
                goto moveView;
            }
        }
    }

    return PID(0xffff);
}


/******************************************************************************
 * 函数名:GetCityName
 * 说  明:获取武将名称
 *
 * 入口参数：city -- 城市编号，str -- 城市名称
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-10 10:18	基本功能完成
 ******************************************************************************/
FAR void GetCityName(U8 city,U8 *str)
{
    /*U8 *dptr;
     U16 cnt;

     dptr = ResLoadToCon(CITY_NAME,g_PIdx,g_CBnkPtr);
     cnt = city;
     cnt *= 10;
     gam_memcpy(str,&dptr[cnt],10);
     str[10] = 0;*/

    ResLoadToMem(CITY_NAME,city + 1,str);
}

/******************************************************************************
 * 函数名:GetCityProStr
 * 说  明:获取城市属性字符串
 *
 * 入口参数：city -- 城市编号，pro -- 属性编号，str -- 属性字符串
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-10 9:47	基本功能完成
 ******************************************************************************/
void GetCityProStr(U8 city,U8 pro,U8 *str)
{
    U8 strbuf[20];
    str[0] = '\0';

    IF_HAS_HOOK("getCityPropertyDisplay") {
        U8* display = str;

        BIND_U8EX("cityIndex", &city);
        BIND_U8EX("propertyIndex", &pro);
        BIND_GBKARR(display, 128);
        if (CALL_HOOK() == 0) {
            HOOK_RETURN();
        }
    }

    switch (pro)
    {
        case 0:		/*归属*/
            ResLoadToMem(STRING_CONST,ATRR_STR1,str);
            if (g_Cities[city].Belong)
            {
                GetPersonName(PID(g_Cities[city].Belong - 1),&str[5]);
            }
            else
            {
                ResLoadToMem(STRING_CONST,ATRR_STR70,&str[5]);
            }
            break;
        case 1:		/*太守*/
            ResLoadToMem(STRING_CONST,ATRR_STR2,str);
            if (g_Cities[city].SatrapId)
            {
                GetPersonName(PID(g_Cities[city].SatrapId - 1),&str[5]);
            }
            else
            {
                ResLoadToMem(STRING_CONST,ATRR_STR70,&str[5]);
            }
            break;
        case 2:		/*农业*/
            ResLoadToMem(STRING_CONST,ATRR_STR3,str);
            gam_itoa(g_Cities[city].Farming,strbuf,10);
            gam_strcat(str,strbuf);
            str[gam_strlen(str) + 1] = '\0';
            str[gam_strlen(str)] = '/';
            // VIP功能：显示应用倍率后的上限
            {
                U32 limit_vip = (U32)g_Cities[city].FarmingLimit * GetAgricultureMultiplier();
                if (limit_vip > 65535) limit_vip = 65535;
                gam_itoa(limit_vip,strbuf,10);
            }
            gam_strcat(str,strbuf);
            break;
        case 3:		/*商业*/
            ResLoadToMem(STRING_CONST,ATRR_STR4,str);
            gam_itoa(g_Cities[city].Commerce,strbuf,10);
            gam_strcat(str,strbuf);
            str[gam_strlen(str) + 1] = '\0';
            str[gam_strlen(str)] = '/';
            // VIP功能：显示应用倍率后的上限
            {
                U32 limit_vip = (U32)g_Cities[city].CommerceLimit * GetCommerceMultiplier();
                if (limit_vip > 65535) limit_vip = 65535;
                gam_itoa(limit_vip,strbuf,10);
            }
            gam_strcat(str,strbuf);/**/
            break;
        case 4:		/*民忠*/
            ResLoadToMem(STRING_CONST,ATRR_STR5,str);
            gam_itoa(g_Cities[city].PeopleDevotion,strbuf,10);
            gam_strcat(str,strbuf);
            break;
        case 5:		/*防灾*/
            ResLoadToMem(STRING_CONST,ATRR_STR6,str);
            gam_itoa(g_Cities[city].AvoidCalamity,strbuf,10);
            gam_strcat(str,strbuf);
            break;
        case 6:		/*人口*/
            ResLoadToMem(STRING_CONST,ATRR_STR7,str);
            gam_ltoa(g_Cities[city].Population,strbuf,10);
            gam_strcat(str,strbuf);
            break;
        case 7:		/*金钱*/
            ResLoadToMem(STRING_CONST,ATRR_STR8,str);
            gam_itoa(g_Cities[city].Money,strbuf,10);
            gam_strcat(str,strbuf);
            break;
        case 8:		/*粮食*/
            ResLoadToMem(STRING_CONST,ATRR_STR9,str);
            gam_itoa(g_Cities[city].Food,strbuf,10);
            gam_strcat(str,strbuf);
            break;
        case 9:		/*后备兵力*/
            ResLoadToMem(STRING_CONST,ATRR_STR10,str);
            gam_itoa(g_Cities[city].MothballArms,strbuf,10);
            gam_strcat(str,strbuf);
            break;
        case 10:	/*城市状态*/
            ResLoadToMem(STRING_CONST,ATRR_STR0,str);
            GetCityState(city,strbuf);
            gam_strcat(str,strbuf);
            break;
    }
}

FAR void GetCityState(U8 city,U8 *str)
{
    U8 state = '\0';
    
    switch (g_Cities[city].State)
    {
        case STATE_NORMAL:	/*正常*/
            state = CITY_STATE_STR1;
            break;
        case STATE_FAMINE:	/*饥荒*/
            state = CITY_STATE_STR2;
            break;
        case STATE_DROUGHT:	/*旱灾*/
            state = CITY_STATE_STR3;
            break;
        case STATE_FLOOD:	/*水灾*/
            state = CITY_STATE_STR4;
            break;
        case STATE_REBELLION:	/*暴动*/
            state = CITY_STATE_STR5;
            break;
    }
    ResLoadToMem(STRING_CONST,state,str);
}

/******************************************************************************
 * 函数名:ShowCityPro
 * 说  明:显示城市属性
 *
 * 入口参数：city -- 城市编号
 *
 * 出口参数：0 -- 退出，1 -- 确认
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-9 17:19	基本功能完成
 ******************************************************************************/
FAR U8 ShowCityProInner(U8 city);
FAR U8 ShowCityPro(U8 city) {
    int prev = SysScrollingTimerOpen(0);
    U8 rv = ShowCityProInner(city);
    SysScrollingTimerOpen(prev);
    return rv;
}
FAR U8 ShowCityProInner(U8 city)
{
    U8 showflag,showtop,i;
    U8 str[128];
    GMType Msg;
    Touch touch = {0};

    IF_HAS_HOOK("willShowCityInfo") {
        BIND_U8(&city);
        if (CALL_HOOK_A() == 0) {
            HOOK_RETURN(0);
        }
    }

    showtop = 0;
    showflag = 1;
    while (1)
    {
        if (showflag)
        {
            gam_clrlcd((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX - 5,WK_SY + 2,(WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX + ASC_WID * 15 + 5,WK_SY + 2 + (ASC_HGT + 1) * ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)));
            gam_rect((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX - 5 - 1,WK_SY + 2 - 1,(WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX + ASC_WID * 15 + 5 + 1,WK_SY + 2 + (ASC_HGT + 1) * ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) + 1);
            GetCityName(city,str);
            GamStrShowS((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX,WK_SY + 2,str);
            for (i = 0;i < ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1;i ++)
            {
                if (i >= cfg.cityPropertiesCount) break;
                GetCityProStr(city,showtop + i,str);
                GamStrShowS((WK_EX - WK_SX - ASC_WID * 15) / 2 + WK_SX,WK_SY + 2 + ASC_HGT + (ASC_HGT + 1) * i,str);
            }
            showflag = 0;
        }
        
        GamGetMsg(&Msg);
    tagHandleMsg:
        if (VM_CHAR_FUN == Msg.type)
        {
            switch (Msg.param)
            {
                case VK_UP:
                case VK_LEFT:
                    if (showtop)
                    {
                        showtop -= ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1;
                        showflag = 1;
                    }
                    break;
                case VK_DOWN:
                case VK_RIGHT:
                    if (showtop + ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1 < cfg.cityPropertiesCount)
                    {
                        showtop += ((WK_EY - 1 - 2 - (WK_SY + 2)) / (ASC_HGT + 1)) - 1;
                        showflag = 1;
                    }
                    break;
                case VK_ENTER:
                    return(1);
                case VK_EXIT:
                    return(0);
            }
        } else if (VM_TOUCH == Msg.type) {
            touchUpdate(&touch, Msg);

            if (Msg.param == VT_TOUCH_UP) {
                if (!touch.completed) continue;

                if (touch.moved) {
                    I16 dy = touch.currentY - touch.startY;
                    U8 up = dy < 0;
                    dy = abs(dy);
                    if (dy > 30) {
                        Msg.type = VM_CHAR_FUN;
                        Msg.param = up ? VK_DOWN : VK_UP;
                        goto tagHandleMsg;
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    return(0);
}
