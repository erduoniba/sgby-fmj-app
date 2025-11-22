/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	paccount.c
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
 *	1.0    2005-6-1 15:35  陈泽伟     基本的功能完成
 ***********************************************************************/

#include "baye/stdsys.h"
#include "baye/comm.h"
#undef	PACCOUNT_C
#define	PACCOUNT_C
#include "baye/enghead.h"

/******************************************************************************
 * 函数名:InitItem
 * 说  明:初始化坐标计算结构体
 *
 * 入口参数：x0,y0,x1,y1 -- 坐标范围，positem -- 待初始化的坐标计算结构体
 *
 * 出口参数：无
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-1 15:46	基本功能完成
 ******************************************************************************/
FAR void InitItem(U8 x0,U8 y0,U8 x1,U8 y1,PosItemType *positem)
{
    positem->x0 = x0;
    positem->y0 = y0;
    positem->x1 = x1;
    positem->y1 = y1;
    positem->sx = x0;
    positem->sy = y0;
    positem->ex = x0;
    positem->ey = y0;
}

/******************************************************************************
 * 函数名:AddItem
 * 说  明:添加一个显示项计算坐标
 *
 * 入口参数：wid,hig -- 显示项大小，positem -- 坐标计算结构体，
 *		x,y -- 返回坐标
 * 出口参数：1 -- 成功，0 -- 无法添加（已容纳不下）
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005-6-1 15:46	基本功能完成
 ******************************************************************************/
FAR U8 AddItem(U8 wid,U8 hig,PosItemType *positem,U8 *x,U8 *y)
{
    if (positem->sy + hig - 1 > positem->y1)
        return(0);

    if (positem->ex + wid - 1 > positem->x1)
    {
        return(0);
        /*if (positem->ey + hig > positem->y1)
         {
         return(0);
         }
         else
         {
         if (positem->x0 + wid - 1 > positem->x1)
         return(0);

         positem->sx = positem->x0;
         positem->ex = positem->sx + wid - 1;
         positem->sy = positem->ey + 1;
         positem->ey = positem->sy + hig - 1;
         }*/
    }
    else
    {
        positem->sx = positem->ex + 1;
        positem->ex = positem->sx + wid - 1;
        if (positem->sy + hig - 1 > positem->ey)
            positem->ey = positem->sy + hig - 1;
    }

    *x = positem->sx;
    *y = positem->sy;

    return(1);
}

/******************************************************************************
 * 函数名:NextLineItem
 * 说  明:计算下一行坐标
 *
 * 入口参数：positem -- 坐标计算结构体
 *
 * 出口参数：1 -- 成功，0 -- 无法添加（已容纳不下）
 *
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		陈泽伟		2005/5/18 11:26AM	基本功能完成
 ******************************************************************************/
FAR U8 NextLineItem(PosItemType *positem)
{
    if (positem->ey == positem->y1)
        return(0);
    
    positem->sx = positem->x0;
    positem->ex = positem->sx;
    positem->sy = positem->ey + 1;
    positem->ey = positem->sy;
    
    return(1);
}
