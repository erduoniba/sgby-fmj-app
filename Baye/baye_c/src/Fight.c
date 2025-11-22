/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	Fight.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		游戏引擎战斗模块
 **
 *移植性声明:
 *	1、符合标准：《游戏设计标准V1.0》
 *	2、兼容模式：平移兼容|缩放兼容
 *修改历史：
 *	版本    日期     作者     改动内容和原因
 *   ----------------------------------------------------
 *	1.0    2005.5.16  高国军     基本的功能完成
 ***********************************************************************/
#include "baye/stdsys.h"
#include "baye/comm.h"
#undef	Fight
#define	Fight
#include "baye/enghead.h"
#include "baye/bind-objects.h"
#include "touch.h"
#define		IN_FILE	1	/* 当前文件位置 */

/*本体函数声明*/
/*------------------------------------------*/
U8 FgtIntVar(void);
U8 FgtIntMap(void);
void FgtIntScr(void);
void FgtInit(void);
U8 FgtGetBaseXY(U8 sym);
void FgtShowGen(U8 act);
void FgtShowCursor(void);
void FgtDealMan(void);
void FgtDealCmp(void);
U8 FgtGetControl(void);
U8 FgtGenMove(U8 idx);
void FgtAllRight(bool *flag);
U8 FgtGetFoucs(void (*chkcondition)(bool*flag));
void FgtMoveBack(bool *flag);
void FgtShowState(void);
void FgtShowInf(void);
void FgtGetPCmd(FGTCMD *pcmd);
U8 FgtExeCmd(FGTCMD *pcmd);
void FgtGetMPos(U8 idx,RECT *pRect);
U8 FgtCmdAimGet(U8 type,SkillID param,U8 idx);
void FgtShowAtRng(void);
void FgtCmdBack(bool *flag);
bool FgtChkRng(void);
void FgtRPicShowV(U16 id,U8 idx,U8 sx,U8 sy);
void FgtDealBout(void);
void FgtRefrashMap(void);
void FgtDrvState(void);
void FgtShowSNum(U8 sym,U8 idx,U16 num);
void FgtMapUnitShow(U8 tx,U8 ty,U8 flag);
void FgtShowGetExp(U8 exp);
void FgtShowChgSpe(U8 sfrm,U8 efrm,U8 x,U8 y);
void FgtChkAtkEnd(void);
PersonID TransIdxToGen2(U8 idx);
void FgtLoadToMem(U8 idx,U8 *buf);
U8 *FgtLoadToCon(U16 ResId,U8 idx);
void FgtShowFrame(void);
void FgtStrShowV(U8 x,U8 y,U8 *buf);
void FgtShowMap(U8 x,U8 y);
void FgtGetMapDat(U8 x,U8 y);
void FgtChkEnd(U8 flag);

/***********************************************************************
 * 说明:     战斗模块入口函数
 * 输入参数: 无
 * 返回值  : 0
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 GamFight(void)
{
    FgtInit();
    if (!g_FgtOver) {
        call_hook("enterBattle", NULL);
#define CHECK_OVER() if(g_FgtOver) break;
        while(!g_FgtOver)
        {
            call_hook("battleStage1", NULL);
            CHECK_OVER();
            FgtDealBout(); //时间步进，检查到30天，恢复将领激活状态，变天气，检查粮草是否耗尽，驱动人物技能状态
            CHECK_OVER();
            call_hook("battleStage2", NULL);
            CHECK_OVER();
            FgtDealMan(); // 玩家策略
            CHECK_OVER();
            call_hook("battleStage3", NULL);
            CHECK_OVER();
            FgtDealCmp(); // AI策略
            CHECK_OVER();
            call_hook("battleStage4", NULL);
            CHECK_OVER();
            CountProvUse(); // 处理粮草消耗
            CHECK_OVER();
            call_hook("battleStage5", NULL);
        }
        call_hook("exitBattle", NULL);
    }
    /* 战斗只通过g_FgtOver返回胜败，战后处理由外部完成 */
    return 0;
}
/***********************************************************************
 * 说明:     初始化战斗
 * 输入参数: way-进攻方向
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtInit(void)
{
    FgtIntVar();
    if(FGT_COMON != g_FgtOver)
        return;
    if(FGT_AUTO == g_FgtParam.Mode)
    {
        IF_HAS_HOOK("fightCountWinner") {
            if (CALL_HOOK_A() == 0 && FGT_COMON != g_FgtOver) {
                HOOK_LEAVE();
                return;
            }
        }
        FgtCountWon();
        return;
    }
    else
    {
        FgtIntMap();
        FgtIntScr();
    }
    FgtChkAtkEnd();
    FgtChkEnd(false);
    FgtChkEnd(true);
}
/***********************************************************************
 * 说明:     处理玩家的操作
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtDealCmp(void)
{
    U8	pTer;
    FGTCMD	fcmd;

    while(!g_FgtOver)
    {
        if(!FgtGetMCmd(&fcmd))
            break;
        pTer = FgtExeCmd(&fcmd);
        if(FGT_DF == g_FgtParam.Mode && TERRAIN_CITY == pTer)
            g_FgtOver = FGT_LOSE;
        else
            FgtChkEnd(true);
    }
}
/***********************************************************************
 * 说明:     处理玩家的操作
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtDealMan(void)
{
    U8	idx,pTer;
    FGTCMD	fcmd;

    for(pTer = 0;pTer < FGT_PLAMAX;pTer += 1)
    {
        if(STATE_SW == g_GenPos[pTer].state)
            continue;
        FgtSetFocus(pTer);			/* 敌人处理完后，将光标移回玩家主将 */
        break;
    }
//    if (g_engineDebug) {
//        g_FgtOver = FGT_WON;
//        return;
//    }
    while(!g_FgtOver)
    {
        idx = FgtGetControl();
        if (idx == 0xff)
            break;
        if(FgtGenMove(idx))
            continue;
        fcmd.sIdx = idx;
        FgtGetPCmd(&fcmd);
        pTer = FgtExeCmd(&fcmd);
        if(FGT_AT == g_FgtParam.Mode && pTer == TERRAIN_CITY)
            g_FgtOver = FGT_WON;
        else
            FgtChkEnd(false);
    }
}
/***********************************************************************
 * 说明:     玩家生成命令
 * 输入参数: pcmd-命令结构
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtGetPCmd(FGTCMD *pcmd)
{
    U32	idx,aIdx = 0;
    U32	lflag,type,param = 0;
    RECT	pRect;
    SBUF buf;

    lflag = true;
    idx = pcmd->sIdx;
    type = 0;
    while(lflag)
    {
        /* 选择命令 */
        FgtShowFrame();
        FgtGetMPos(idx,&pRect);
        FgtLoadToMem(dFgtMnuCmd,buf);
        IF_HAS_HOOK("fightChooseAction") {
            BIND_U32EX("index", &idx);
            type = (U8)CALL_HOOK_A();
        } else {
            type = (U8)PlcSplMenu(&pRect,type,buf);
        }
        pcmd->type = type;
        param = 0;
        /* 获取命令的目标 */
        switch(type)
        {
            case CMD_LOOK:
                FgtShowHlp();
                param = 0xFFFF;
                break;
            case CMD_STGM:
                if(g_GenPos[idx].state == STATE_JZ)
                {
                    /* 若当前将领为禁咒状态，就直接break */
                    param = 0xFFFF;
                    break;
                }
                IF_HAS_HOOK("fightChooseSkill") {
                    BIND_U32EX("index", &idx);
                    param = CALL_HOOK_A();
                } else {
                    param = FgtGetJNIdx(idx,&pRect);
                }
                break;
            case CMD_REST:
            case MNU_EXIT:
                return;
        }
        if(param != 0xFFFF)
        {
            FgtGetCmdRng(type,SID(param),idx);
            aIdx = FgtCmdAimGet(type,SID(param),idx);
            if(aIdx != 0xFF)
                lflag = false;
        }
    }
    pcmd->param = param;
    pcmd->aIdx = aIdx;
}
/***********************************************************************
 * 说明:     获取命令执行的对象
 * 输入参数: type-命令类型	param-命令参数	idx-施展者的脚标
 * 返回值  : 0xFF-退出		其它-将领序号
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtCmdAimGet(U8 type,SkillID param,U8 idx)
{
    U8	pidx,same;
    U8	naim[10],err[10];

    if(param == 22 || param == 30)		/* 天变和谍报-不需要选择目标 */
        return idx;
    FgtLoadToMem(dFgtNoAim,naim);
    FgtLoadToMem(dFgtErrAim,err);
    FgtShowAtRng();
    while(1)
    {
        FgtShowFrame();
        if(FgtGetFoucs(FgtCmdBack) == 0xFF)
        {
            FgtSetFocus(idx);
            return 0xFF;
        }

        pidx = FgtGetGenIdx(g_FoucsX,g_FoucsY);
        if(0xFF == pidx)
        {
            FgtStrShowV(STA_XXX,STA_XXY,naim);
            continue;
        }
        if(FgtChkRng())
        {
            same = (idx < FGT_PLAMAX && pidx < FGT_PLAMAX) || (idx > FGT_PLAMAX && pidx > FGT_PLAMAX);
            if(CMD_STGM == type)
            {	/* 若是施展计谋 */
                if(FgtJNChkAim(param,same,pidx,idx))		/* 且符合施展条件-目标的类型、地形 */
                    break;
            }
            else
                if(!same) break;
        }
        FgtStrShowV(STA_XXX,STA_XXY,err);
    }
    return pidx;
}
/***********************************************************************
 * 说明:     选择命令执行目标的背景
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtCmdBack(bool *flag)
{
    if(*flag)
        FgtShowAtRng();
    else
        FgtAllRight(flag);
}
/***********************************************************************
 * 说明:     显示攻击的目标范围
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowAtRng(void)
{
    U8	rng;
    U8	x,y,sx,sy;

    /* g_FgtAtkRng中的数据结构：
     [0]：范围的尺寸
     [1]：范围左上角在地图上的起始列坐标
     [2]：范围左上角在地图上的起始行坐标
     [3]~[max]：范围数据 */

    rng = g_FgtAtkRng[0];
    for(y = 0;y < rng;y += 1)
    {
        sy = y + g_FgtAtkRng[2];
        if(sy < g_MapSY || sy >= g_MapSY + SCR_MAPHGT)
            continue;
        sy = FgtGetScrY(sy);
        for(x = 0;x < rng;x += 1)
        {
            sx = x + g_FgtAtkRng[1];
            if(sx < g_MapSX || sx >= g_MapSX + SCR_MAPWID)
                continue;
            if(g_FgtAtkRng[3 + x + y * rng] != 1)
                continue;
            sx = FgtGetScrX(sx);
            FgtRPicShowV(STEP_PIC,2,sx,sy);
        }
    }
}
/***********************************************************************
 * 说明:     获取菜单显示的起始坐标
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtGetMPos(U8 idx,RECT *pRect)
{
    U8	sx,sy,tmp;
    U8	rsy,rey;

    tmp = (WK_EY - WK_SY - 24) / HZ_HGT;
    if(tmp < 4)
        tmp = 6;
    else
        tmp = 0;
    sx = FgtGetScrX(g_GenPos[idx].x) + TIL_WID + 2;
    if(sx + ASC_WID * SKILL_NAMELEN > WK_EX - tmp)
        sx = sx - TIL_WID - tmp  - ASC_WID * SKILL_NAMELEN;
    pRect->sx = sx;
    pRect->ex = sx + ASC_WID * SKILL_NAMELEN;

    if((WK_EY - WK_SY - 24) / HZ_HGT < 4)
    {
        rsy = WK_SY + 4;
        rey = WK_EY - 18;
    }
    else
    {
        sy = FgtGetScrY(g_GenPos[idx].y);
        if((WK_EY - sy - 24) / HZ_HGT < 4)
        {
            sy = WK_EY - 18;
            rey = WK_EY - 18;
            rsy = sy - HZ_HGT * 4;
        }
        else
        {
            sy += 4;
            rsy = sy;
            rey = sy + HZ_HGT * 4;
        }
    }
    pRect->sy = rsy;
    pRect->ey = rey;
}
/***********************************************************************
 * 说明:     执行玩家生成的命令
 * 输入参数: pcmd-执行命令结构
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtExeCmd(FGTCMD *pcmd)
{
    U8	idx,exp;

    idx = pcmd->sIdx;
    exp = FgtDrvCmd(pcmd);
    if(exp)
    {
        FgtShowMap(g_MapSX,g_MapSY);
        FgtShowGen(0);
        if (!g_engineConfig.disableExpGrowing) {
            *g_GenAtt[0].exp += exp;
            if(idx < FGT_PLAMAX)
                FgtShowGetExp(exp);
        }
        FgtChkAtkEnd();
    }
    return (FgtGetTerrain(g_GenPos[idx].x,g_GenPos[idx].y));
}
/***********************************************************************
 * 说明:     获取将领移动目标位置
 * 输入参数: idx-被移动将领序号
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGenMove(U8 idx)
{
    U8	x,y;
    U8	buf[20];
    JLPOS	*pos;

    FgtCountPath(idx);

    pos = &g_GenPos[idx];
    if(NO_MOV == pos->move)		/* 被定身，不可移动 */
        goto tagRet;

    gam_memset(buf,0,20);
    FgtLoadToMem(dFgtCtMove,buf);
    FgtShowMvRng();
    while(1)
    {
        FgtShowFrame();
        if(FgtGetFoucs(FgtMoveBack) == 0xFF)
        {
            FgtSetFocus(idx);
            return 1;
        }
        x = g_FoucsX - g_PathSX + g_PUseSX;
        y = g_FoucsY - g_PathSY + g_PUseSY;

        U8 cannotMoveTo;

        if (g_engineConfig.fixFightMoveOutRange) {
            cannotMoveTo = x >= FGT_MRG || y >= FGT_MRG || g_FightPath[y * FGT_MRG + x] > MOV_RSTD;
        } else {
            cannotMoveTo = x > FGT_MRG || y > FGT_MRG || g_FightPath[y * FGT_MRG + x] > MOV_RSTD;
        }

        if(cannotMoveTo)
            FgtStrShowV(STA_SX+74,STA_SY+2,buf);
        else
            break;
    }
tagRet:
    g_BakUpX = pos->x;
    g_BakUpY = pos->y;
    pos->x = g_FoucsX;
    pos->y = g_FoucsY;
    FgtSetFocus(idx);
    return 0;
}
/***********************************************************************
 * 说明:     将领移动时的背景显示
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtMoveBack(bool *flag)
{
    if(*flag)
        FgtShowMvRng();
    else
        FgtAllRight(flag);
}
/***********************************************************************
 * 说明:     获取被操作将领的序号
 * 输入参数: 无
 * 返回值  : !0xff-操作序号	0xff-结束操作
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGetControl(void)
{
    U8	idx;

    FgtShowMap(g_MapSX,g_MapSY);
    FgtShowState();
    FgtShowInf();
    while(1)
    {
        if(!FgtGetFoucs(FgtAllRight))
        {
            idx = FgtCheckIdx();
            if(idx != 0xFF)
                return idx;
            else
                continue;
        }
        if(FgtMainMenu() == 0xFF)
            return 0xFF;
    }
}
/***********************************************************************
 * 说明:     获取光标位置
 * 输入参数: chkcondition-检测函数
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGetFoucsInner(void (*chkcondition)(bool*flag));
U8 FgtGetFoucs(void (*chkcondition)(bool*flag)) {
    int prev = SysScrollingTimerOpen(5);
    U8 rv = FgtGetFoucsInner(chkcondition);
    SysScrollingTimerOpen(prev);
    return rv;
}
U8 FgtGetFoucsInner(void (*chkcondition)(bool*flag))
{
    bool	flag,tflag;
    GMType	pMsg;
    Touch touch = {0};
    U8 leftWhenTouchDown = 0;
    U8 topWhenTouchDown = 0;

    U8 btnTop = SCR_HGT - 16;
    U8 btnH = 16;

    Rect backButton = MakeRect(0, btnTop, 32, btnH);
    Rect searchButton = MakeRect(backButton.right, btnTop, 28, btnH);
    Rect infoButton = MakeRect(searchButton.right, btnTop, SCR_WID - searchButton.right, btnH);
    Rect mapRect = MakeRect(0, 0, SCR_WID, btnTop);
    Rect statusBarRect = MakeRect(0, btnTop, SCR_WID, btnH);

    flag = false;
    while(1)
    {
        GamGetMsg(&pMsg);
        if(VM_CHAR_FUN == pMsg.type)
        {
            if(VK_ENTER == pMsg.param)
                return 0;
            if(VK_EXIT == pMsg.param)
                return 0xFF;
            FgtMapUnitShow(g_FoucsX,g_FoucsY,0);
        }
    tagHandleMsg:
        switch(pMsg.type)
        {
            case VM_CHAR_FUN:
                touchUpdate(&touch, pMsg);
                g_AutoUpdateMapXY = true;
                switch(pMsg.param)
                {
                    case VK_UP:
                        if (g_FoucsY)
                            g_FoucsY -= 1;
                        break;
                    case VK_DOWN:
                        if (g_FoucsY < g_MapHgt-1)
                            g_FoucsY += 1;
                        break;
                    case VK_LEFT:
                        if (g_FoucsX)
                            g_FoucsX -= 1;
                        break;
                    case VK_RIGHT:
                        if (g_FoucsX < g_MapWid-1)
                            g_FoucsX += 1;
                        break;
                    case VK_HELP:
                        FgtShowHlp();
                        g_AutoUpdateMapXY = false;
                        break;
                    case VK_SEARCH:
                        FgtShowView();
                        g_AutoUpdateMapXY = false;
                        break;
                    default:
                        g_AutoUpdateMapXY = false;
                        break;
                }
                tflag = false;
                (*chkcondition)(&tflag);
                break;
            case VM_TIMER:
            {
                switch(pMsg.param) {
                    case 0:
                        if (touchUpdate(&touch, pMsg)) {
                            goto moveView;
                        } else {
                            continue;
                        }
                    case 1:
                        flag = !flag;
                        FgtShowMap(g_MapSX,g_MapSY);
                        break;
                }
                break;
            }
            case VM_TOUCH:
            {
                touchUpdate(&touch, pMsg);
                switch (pMsg.param) {
                    case VT_TOUCH_DOWN:
                        leftWhenTouchDown = g_MapSX;
                        topWhenTouchDown = g_MapSY;
                        break;
                    case VT_TOUCH_UP:
                    {
                        if (!touch.completed || touch.moved) break;

                        I16 x = touch.currentX, y = touch.currentY;

                        if (touchIsPointInRect(x, y, mapRect)) {
                            U8 col = x / 16;
                            U8 row = y / 16;
                            U8 toFocusX = g_MapSX + col;
                            U8 toFocusY = g_MapSY + row;
                            if (toFocusX == g_FoucsX && toFocusY == g_FoucsY) {
                                return 0;
                            }
                            Rect scrRect = MakeRect(g_MapSX, g_MapSY, SCR_MAPWID, SCR_MAPHGT);
                            if (touchIsPointInRect(g_FoucsX, g_FoucsY, scrRect)) {
                                FgtMapUnitShow(g_FoucsX,g_FoucsY,0);
                            }
                            g_FoucsX = toFocusX;
                            g_FoucsY = toFocusY;
                            tflag = false;
                            g_AutoUpdateMapXY = true; // 避免画面整体刷新
                            (*chkcondition)(&tflag);
                            break;
                        }

                        if (touchIsPointInRect(x, y, statusBarRect)) {
                            int ret = -1;
                            IF_HAS_HOOK("fightStatusBarTouched") {
                                BIND_U16(&x);
                                BIND_U16(&y);
                                ret = CALL_HOOK_A();
                            }
                            if (ret == 0) {
                                break;
                            }
                        }

                        if (touchIsPointInRect(x, y, backButton)) {
                            return 0xFF;
                        }
                        if (touchIsPointInRect(x, y, infoButton)) {
                            pMsg.type = VM_CHAR_FUN;
                            pMsg.param = VK_HELP;
                            goto tagHandleMsg;
                        }
                        if (touchIsPointInRect(x, y, searchButton)) {
                            pMsg.type = VM_CHAR_FUN;
                            pMsg.param = VK_SEARCH;
                            goto tagHandleMsg;
                        }
                        break;
                    }
                    case VT_TOUCH_MOVE:
                    {
                        Point p;
                        if (!touch.touched) break;
moveView:
                        p = touchListViewCalcTopLeftForMove(&touch,
                                                                  leftWhenTouchDown, g_MapWid-SCR_MAPWID, 16,
                                                                  topWhenTouchDown, g_MapHgt-SCR_MAPHGT, 16);
                        touchUpdateViewState(
                            &touch,
                            pointState(p.x, 0, g_MapWid-SCR_MAPWID),
                            pointState(p.y, 0, g_MapHgt-SCR_MAPHGT)
                        );
                        if (p.x != g_MapSX || p.y != g_MapSY) {
                            g_MapSX = p.x;
                            g_MapSY = p.y;
                            g_AutoUpdateMapXY = false;
                            tflag = false;
                            (*chkcondition)(&tflag);
                        } else {
                            continue;
                        }
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
        }
        FgtShowGen(flag);
        tflag = true;
        (*chkcondition)(&tflag);
        FgtShowCursor();
        FgtShowFrame();
    }
}
/***********************************************************************
 * 说明:     显示当前光标所在的位置
 * 输入参数: flag-显示还是消除
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowCursor(void)
{
    Rect scrRect = MakeRect(g_MapSX, g_MapSY, SCR_MAPWID, SCR_MAPHGT);
    if (touchIsPointInRect(g_FoucsX, g_FoucsY, scrRect)) {
        U8	x,y;
        x = FgtGetScrX(g_FoucsX);
        y = FgtGetScrY(g_FoucsY);
        FgtRPicShowV(STEP_PIC,5,x,y);
    }
}

void FgtLoadMap(void) {
    if (call_hook("loadFightMap", NULL) == -1) {
        /* 获取地图的基本信息 */
        U8 *ptr = FgtLoadToCon(g_FgtParam.MapId,1);
        g_MapWid = (U8) *ptr;
        g_MapHgt = (U8) *(ptr+2);
        ptr += 16;
        memcpy(g_FightMapData, ptr, g_MapWid*g_MapHgt);
    }
}
/***********************************************************************
 * 说明:     初始化战斗地图
 * 输入参数: mode-玩家战斗模式	way-进攻方向	mapid-地图id
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtIntMap(void)
{
    U8	i,sx,sy;
    U8	tmp,way;
    U16	lp,cx,cy;
    U8	*ptr,*tptr;
    /*	g_TileId = (U16) *(ptr+4);*/
    g_TileId = FIGHT_TILE;

    FgtLoadMap();

    ptr = g_FightMapData;

    lp = 0;
    while(CITY_TIL != ptr[lp])	/* 注意：若地图中没城市地图块，可能造成死循环 */
        lp += 1;
    g_CityX = lp % g_MapWid;
    g_CityY = lp / g_MapWid;
    cx = g_CityX - 2;
    cy = g_CityY - 2;

    /* 获取进攻方起始坐标 */
    sx = FgtGetBaseXY('x');
    sy = FgtGetBaseXY('y');

    /* 初始化将领坐标 */
    tmp = 8;
    way = g_FgtParam.Way;
    if (FGT_AT == g_FgtParam.Mode)
    {
        tmp = cx;
        cx = sx;
        sx = tmp;
        tmp = cy;
        cy = sy;
        sy = tmp;
        tmp = way;
        way = 8;
    }
    /* 初始化坐标 */
    ptr = FgtLoadToCon(IFACE_CONID,dFgtIntPos);
    tptr = ptr + way * 20;
    ptr += tmp * 20;
    for (i = 0; i < FGT_PLAMAX; i += 1)
    {
        tmp = i << 1;
        g_GenPos[i].x = cx + ptr[tmp];
        g_GenPos[i].y = cy + ptr[tmp + 1];
        g_GenPos[i+FGT_PLAMAX].x = sx + tptr[tmp];
        g_GenPos[i+FGT_PLAMAX].y = sy + tptr[tmp + 1];
    }
    return 0;
}
/***********************************************************************
 * 说明:     初始化战斗变量
 * 输入参数: mode-玩家的战斗方式	way-进攻方向
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtIntVar(void)
{
    g_FgtWeather = WEATHER_WIND;
    g_FgtOver = FGT_COMON;
    g_FgtBoutCnt = 0;
    if(FGT_AT == g_FgtParam.Mode)
        g_MainGenIdx = 0;
    else
        g_MainGenIdx = FGT_PLAMAX;
    /* 初始化将领基本属性 */
    CountInitGen();
    return 0;
}
/***********************************************************************
 * 说明:     获取攻击方初始化的基准坐标
 * 输入参数: way-进攻方向
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGetBaseXY(U8 sym)
{
    U8	x,y;

    x = g_MapWid - 5;
    y = g_MapHgt - 5;
    switch(g_FgtParam.Way)
    {
        case DIRECT_ES:
            break;
        case DIRECT_N:
            x = (g_MapWid>>1)-2;
            y = 0;
            break;
        case DIRECT_EN:
            y = 2;
            break;
        case DIRECT_E:
            y = (g_MapHgt >> 1) - 2;
            break;
        case DIRECT_S:
            x = (g_MapWid >> 1) - 2;
            break;
        case DIRECT_WS:
            x = 2;
            break;
        case DIRECT_W:
            x = 2;
            y = (g_MapHgt >> 1) - 2;
            break;
        case DIRECT_WN:
            x = 0;
            y = 0;
            break;
    }
    if('x' == sym)
        return x;
    else
        return y;
}
/***********************************************************************
 * 说明:     初始化战斗界面
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtIntScr(void)
{
    FgtShowState();
    FgtSetFocus(FGT_PLAMAX);
    GamDelay(SHOW_DLYBASE * 10,false);
    FgtSetFocus(0);
    GamDelay(SHOW_DLYBASE * 10,false);
}

void FgtShowStateBackground() {
    gam_clrvscr(STA_SX, STA_SY, STA_EX, STA_EY, g_VisScr);
    FgtRPicShowV(STATE_PIC,1,STA_SX,STA_SY);
}

/***********************************************************************
 * 说明:     显示战斗状态
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowState()
{
    FgtShowStateBackground();			/* 显示背景图 */
    PlcNumShow(STA_LCX,STA_LCY,g_FgtParam.MProvender,3,g_VisScr);	/* 显示粮草 */
    FgtRPicShowV(WEATHER_PIC,g_FgtWeather,STA_TQX,STA_TQY);		/* 显示天气 */
    call_hook("didShowFightStateBar", NULL);
}
/***********************************************************************
 * 说明:     检查光标所处位置是否在指定范围内的有效区域
 * 输入参数: 无
 * 返回值  : true-是	false-否
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
bool FgtChkRng(void)
{
    U8	x,y,rng;

    rng = g_FgtAtkRng[0];
    x = g_FoucsX - g_FgtAtkRng[1];
    y = g_FoucsY - g_FgtAtkRng[2];
    if(y >= rng || x >= rng)
        return false;
    return (g_FgtAtkRng[(U16)y * rng + x + 3] == 1);
}
/***********************************************************************
 * 说明:     处理回合开始时公共的对象
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtDealBout(void)
{
    U8	idx,numb[5],xof;
    U8	tbuf[30];

    g_FgtBoutCnt += 1;	/* 战斗天数增加 */
    g_EneTmpProv = 0;	/* 敌人粮草临时变量 */
    if(g_FgtBoutCnt > g_FgtBoutMax)
    {
        if(g_FgtParam.Mode == FGT_AT)
            g_FgtOver = FGT_LOSE;
        else
            g_FgtOver = FGT_WON;
        return;
    }
    /* 恢复将领的激活状态 */
    for(idx = 0;idx < FGTA_MAX;idx += 1)
        g_GenPos[idx].active = STA_WAIT;

    /* 转变天气 */
    FgtChgWeather();
    PlcRPicShow(DAYS_PIC,1,FGT_SPESX + 46,FGT_SPESY + 18,true);
    gam_itoa(g_FgtBoutCnt,numb,10);
    xof = 64 - (gam_strlen(numb) - 1) * ASC_WID / 2;
    GamStrShowS(FGT_SPESX + xof,FGT_SPESY + 22,numb);
    GamDelay(SHOW_DLYBASE * 5,false);

    if(g_FgtParam.EProvender == 0)
    {
        g_FgtOver = FGT_WON;
        idx = dFgtNoProv1;
    }
    if(g_FgtParam.MProvender == 0)
    {
        g_FgtOver = FGT_LOSE;
        idx = dFgtNoProv0;
    }
    if(g_FgtOver)
    {
        FgtLoadToMem(idx,tbuf);
        GamMsgBox(tbuf,1);
    }

    FgtDrvState();
}
/***********************************************************************
 * 说明:     刷新地图的显示
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtRefrashMap(void)
{
    if (g_AutoUpdateMapXY) {
        U8 changed = false;
        if (g_FoucsX < g_MapSX) {
            g_MapSX = g_FoucsX;
            changed = true;
        }
        if (g_FoucsY < g_MapSY) {
            g_MapSY = g_FoucsY;
            changed = true;
        }
        if (g_FoucsX >= g_MapSX + SCR_MAPWID)
        {
            g_MapSX = g_FoucsX - SCR_MAPWID + 1;
            changed = true;
        }
        if (g_FoucsY >= g_MapSY + SCR_MAPHGT)
        {
            g_MapSY = g_FoucsY - SCR_MAPHGT + 1;
            changed = true;
        }
        if (!changed)
            return;
    }
    FgtShowMap(g_MapSX,g_MapSY);
}
/***********************************************************************
 * 说明:     总是返回正确
 * 输入参数: 无
 * 返回值  : true-正确
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtAllRight(bool *flag)
{
    if(*flag) return;

    FgtRefrashMap();
    FgtShowState();
    FgtShowInf();
}
/***********************************************************************
 * 说明:     驱动特殊状态
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtDrvState(void)
{
    U8	i,state,usehook = 0;
    U16	arms;
    bool	rec,update;
    PersonType	*per;
    PersonID tmp;

    if(g_FgtOver)
        return;
    for(i = 0;i < FGTA_MAX;i += 1)
    {
        state = g_GenPos[i].state;
        if(STATE_SW == state)
            continue;
        tmp = TransIdxToGen2(i);
        per = &g_Persons[tmp];
        rec = (gam_rand() % 60) < (per->IQ >> 1);
        arms = per->Arms;
        update = false;

        IF_HAS_HOOK("battleDrivePersonState") {
            BIND_U8EX("generalIndex", &i);
            BIND_U8(&update);
            if(CALL_HOOK() == 0) {
                usehook = 1;
            }
        }

        if (!usehook)
        switch(state)
        {
            case STATE_ZC:		/* 正常 */
            case STATE_DJ:		/* 遁甲 */
                break;
            case STATE_SZ:		/* 石阵 */
                per->Arms -= (arms >> 3);
            case STATE_HL:		/* 混乱 */
            case STATE_JZ:		/* 禁咒 */
            case STATE_DS:		/* 定身 */
                if(rec)
                {
                    g_GenPos[i].state = STATE_ZC;
                    update = true;
                }
                break;
            case STATE_QM:		/* 奇门 */
            case STATE_QZ:		/* 潜踪 */
                if(!rec)
                {
                    g_GenPos[i].state = STATE_ZC;
                    update = true;
                }
                break;
        }
        if(arms != per->Arms)
        {
            I32 hurt = per->Arms - arms;
            FgtSetFocus(i);
            if (hurt < 0) {
                FgtShowSNum('-', i, -hurt);	/* 在地图上显示数字 */
            } else {
                FgtShowSNum('+', i, hurt);	/* 在地图上显示数字 */
            }
        }
        if(update)
        {
            FgtSetFocus(i);
            FgtFrashGen(i,4);		/* 刷新将领 */
        }
    }
}
/***********************************************************************
 * 说明:     将屏幕的焦点聚集到指定的将领身上
 * 输入参数: foc-将领序号
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtSetFocus(U8 foc)
{
    U8	sx,sy;

    sx = g_GenPos[foc].x;
    sy = g_GenPos[foc].y;
    g_FoucsX = sx;
    g_FoucsY = sy;
    /* 初始化显示地图 */
    if (sx < SHOW_MX)
        sx = 0;
    else
        sx -= SHOW_MX;
    if (sy < SHOW_MY)
        sy = 0;
    else
        sy -= SHOW_MY;
    FgtShowMap(sx,sy);
    FgtShowGen(0);
    FgtShowInf();
    FgtShowFrame();
}
/***********************************************************************
 * 说明:     在地图上显示小数字
 * 输入参数: sym-'+','-'	idx-将领脚标	num-显示的数字
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowSNum(U8 sym,U8 idx,U16 num)
{
    U8	sx,sy;
    U8	tbuf[10];

    tbuf[0] = sym;
    gam_itoa(num,tbuf + 1,10);
    sx = FgtGetScrX(g_GenPos[idx].x);
    sy = FgtGetScrY(g_GenPos[idx].y);
    GamStrShowS(sx,sy,tbuf);
    GamDelay(SHOW_DLYBASE * 5,false);
}
/***********************************************************************
 * 说明:     闪烁指定的将领
 * 输入参数: idx-将领脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtFrashGen(U8 idx,U8 cnt)
{
    U8	i;

    FgtShowGen(0);
    i = 0;
    while(1)
    {
        i += 1;
        FgtShowFrame();
        if(i > cnt) break;
        GamDelay(SHOW_DLYBASE,false);
        FgtMapUnitShow(g_GenPos[idx].x,g_GenPos[idx].y,1);
        GamDelay(SHOW_DLYBASE,false);
    }
}
/***********************************************************************
 * 说明:     显示当前地图显示范围内可见的将领
 * 输入参数: act-将领动作
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowGen(U8 act)
{
    U8	i,idx,pIdx;
    U8	sx,sy,tx,ty,state;
    JLPOS	*pos;

    IF_HAS_HOOK("drawGenerals") {
        BIND_U8EX("frame", &act);
        if (CALL_HOOK() == 0) {
            HOOK_LEAVE();
            return;
        }
    }

    /* 显示可见的将领 */
    for (i = 0; i < FGTA_MAX; i += 1)
    {
        pos = &g_GenPos[i];
        state = pos->state;
        if (STATE_SW == state)
            continue;
        tx = pos->x;
        ty = pos->y;
        if(tx >= g_MapSX && tx < g_MapSX + SCR_MAPWID &&
           ty >= g_MapSY && ty < g_MapSY + SCR_MAPHGT)
        {
            idx = FgtGenPIdx(i);
            if(STA_WAIT == pos->active)
                idx += act;
            sx = FgtGetScrX(tx);
            sy = FgtGetScrY(ty);
            /* 显示将领 */
            FgtMapUnitShow(tx,ty,0);

            IF_HAS_HOOK("drawOneGeneral") {
                BIND_U8EX("frame", &act);
                BIND_U8EX("index", &i);
                BIND_U8EX("pic", &idx);
                BIND_U8EX("x", &sx);
                BIND_U8EX("y", &sy);
                CALL_HOOK();
            } else {
                FgtRPicShowV(BING_PIC,idx + 1,sx,sy);
                /* 显示异常状态 */
                if(state == STATE_JZ)
                    pIdx = 2;
                else if(state == STATE_DS)
                    pIdx = 4;
                else
                    pIdx = 0;
                if(pIdx)
                {
                    pIdx += act;
                    FgtRPicShowV(SPESTA_PIC,pIdx + 1,sx,sy);
                }
            }
        }
    }
}
/***********************************************************************
 * 说明:     显示地图块单元
 * 输入参数: tx,ty-地图坐标	flag-true显示到屏幕 false显示到内存
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtMapUnitShow(U8 tx,U8 ty,U8 flag)
{
    U8	tile;

    tile = (ty - g_MapSY) * SCR_MAPWID + tx - g_MapSX;
    tx = FgtGetScrX(tx);
    ty = FgtGetScrY(ty);

    IF_HAS_HOOK("drawMapUnit") {
        BIND_U8EX("x", &tx);
        BIND_U8EX("y", &ty);
        BIND_U8EX("tile", &tile);
        BIND_U8EX("flag", &flag);
        CALL_HOOK();
    } else {
        pushPaintColor(g_engineConfig.theme.fightMapColor);
        gam_drawpic(g_TileId, g_FightMap[tile], tx, ty, flag);
        popPaintColor();
    }
}
/***********************************************************************
 * 说明:     显示获取的经验值
 * 输入参数: exp-获取的经验值
 * 返回值  : true-是	false-否
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowGetExp(U8 exp)
{
    U8	sbuf[20],numb[5];

    /* 显示获得的经验 */
    FgtLoadToMem(dFgtGetExp,sbuf);
    gam_itoa(exp,numb,10);
    gam_strcat(sbuf,numb);
    PlcGraMsgBox(sbuf,1,0xFF);
}
/***********************************************************************
 * 说明:     检查并驱动命令的结束（该升的升，该死的死）
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtChkAtkEnd(void)
{
    U8	i,idx;
    U8	x,y,*buf;
    PersonType	*per;
    JLPOS		*pos;
    PersonID p;

    buf = SHARE_MEM;
    for(i = 0;i < FGTA_MAX; i += 1)
    {
        pos = &g_GenPos[i];
        if(STATE_SW == pos->state)
            continue;
        p = TransIdxToGen2(i);
        per = &g_Persons[p];
        x = pos->x;
        y = pos->y;
        idx = gam_rand() % 3;
        /* 升的 */
        if(per->Experience > FGT_EXPMAX - 1)
        {
            FgtSetFocus(i);
            per->Experience -= FGT_EXPMAX;
            LevelUp(per);
            FgtShowChgSpe(0,5,x,y);
            FgtLoadToMem(dFgtLevUp0 + idx,buf);
            ShowGReport(p,buf);
        }
        /* 死的 */
        if(!pos->hp || !per->Arms)
        {
            FgtSetFocus(i);
            pos->state = STATE_SW;
            FgtShowChgSpe(6,11,x,y);
            FgtLoadToMem(dFgtDead0 + idx,buf);
            ShowGReport(p,buf);
        }
    }
}
/***********************************************************************
 * 说明:     显示变身动画
 * 输入参数: sfrm,efrm-播放参数	x,y-播放坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowChgSpe(U8 sfrm,U8 efrm,U8 x,U8 y)
{
    if(x < g_MapSX || x >= g_MapSX + SCR_MAPWID ||
       y < g_MapSY || y >= g_MapSY + SCR_MAPHGT)
        return;
    x = FgtGetScrX(x);
    y = FgtGetScrY(y);
    PlcMovie(STACHG_SPE,0,sfrm,efrm,0,x,y);
}
/***********************************************************************
 * 说明:     显示地图
 * 输入参数: x,y-显示地图的起始坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowMap(U8 x,U8 y)
{
    U8	i;
    U16	count;

    FgtGetMapDat(x,y);
    /* 显示地图 */
    for(i = 0; i < SCR_MAPWID * SCR_MAPHGT; i += 1)
    {
        y = ((i / SCR_MAPWID) << 4) + WK_SX;
        x = ((i % SCR_MAPWID) << 4) + WK_SY;

        IF_HAS_HOOK("drawMapUnit") {
            U8 flag = 0;
            BIND_U8EX("x", &x);
            BIND_U8EX("y", &y);
            BIND_U8EX("tile", &i);
            BIND_U8EX("flag", &flag);
            CALL_HOOK();
        } else {
            count = g_FightMap[i];
            pushPaintColor(g_engineConfig.theme.fightMapColor);
            gam_drawpic(g_TileId, count, x, y, 0);
            popPaintColor();
        }
    }
}
/***********************************************************************
 * 说明:     获取指定位置的地图数据
 * 输入参数: x,y-显示地图的起始坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtGetMapDat(U8 x,U8 y)
{
    U8	i,*ptr,*maptr;
    U16	count;

    /* 入口参数检查 */
    if(g_MapWid < x + SCR_MAPWID)
        x = g_MapWid - SCR_MAPWID;
    if(g_MapHgt < y + SCR_MAPHGT)
        y = g_MapHgt - SCR_MAPHGT;

    g_MapSX=x;
    g_MapSY=y;
    /* 获取要显示的地图数据指针 */
    ptr = g_FightMapData;
    /* 获取要显示的地图数据 */
    maptr = g_FightMap;
    count = g_MapWid;
    ptr += y * count;
    ptr += x;
    for(i = 0; i < SCR_MAPHGT; i += 1)
    {
        gam_memcpy(maptr,ptr,SCR_MAPWID);
        maptr += SCR_MAPWID;
        ptr += g_MapWid;
    }
}
/***********************************************************************
 * 说明:     显示当前位置的信息
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowInf(void)
{
    U32	idx;
    SBUF buf, sbuf;
    
    gam_clrvscr(STA_XXX,STA_XXY,STA_XXX + HZ_WID * 4,STA_XXY + HZ_HGT,g_VisScr);
    idx = FgtGetGenIdx(g_FoucsX,g_FoucsY);
    if(0xFF == idx)
    {
        idx = FgtGetTerrain(g_FoucsX,g_FoucsY) << 2;
        FgtLoadToMem(dFgtTrnInf,buf);
        gam_memcpy(sbuf,buf+idx,4);
        sbuf[4]=0;
    }
    else
    {
        PersonID p;
        p = TransIdxToGen2(idx);
        GetPersonName(p,sbuf);
    }
    FgtStrShowV(STA_XXX,STA_XXY,sbuf);
    call_hook_s("didRefreshFightStateBar", NULL);
#if	(GAM_VER == GAM_DEBUG_MODE)
    itoa(g_FoucsX,buf,10);
    FgtStrShowV(STA_XXX + 30,STA_XXY,buf);
    itoa(g_FoucsY,buf,10);
    FgtStrShowV(STA_XXX + 50,STA_XXY,buf);
#endif
}
/***********************************************************************
 * 说明:     检查战斗是否结束
 * 输入参数: flag-
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtChkEnd(U8 flag)
{
    U8	i,start;
    
    if(g_FgtOver)
        return;
    
    /* 若主将已死 */
    if(STATE_SW ==g_GenPos[g_MainGenIdx].state)
    {
        if(FGT_PLAMAX == g_MainGenIdx)
            g_FgtOver = FGT_WON;
        else
            g_FgtOver = FGT_LOSE;
        return;
    }
    
    if(flag)
    {
        start = 0;
        g_FgtOver = FGT_LOSE;
    }
    else
    {
        start = FGT_PLAMAX;
        g_FgtOver = FGT_WON;
    }
    /* 全员死亡 */
    for(i = start;i < FGTA_MAX; i += 1)
    {
        if(STATE_SW == g_GenPos[i].state)
            continue;
        g_FgtOver = FGT_COMON;
        return;
    }
}
/***********************************************************************
 * 说明:     将将领的控制脚标转换成将领id
 * 输入参数: idx-控制脚标
 * 返回值  : 将领id
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
PersonID TransIdxToGen2(U8 idx)
{
    return PID(g_FgtParam.GenArray[idx] - 1);
}
/***********************************************************************
 * 说明:     资源管理函数的本体调用
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtLoadToMem(U8 idx,U8 *buf)
{
    ResLoadToMem(IFACE_STRID,idx,buf);
}
/***********************************************************************
 * 说明:     初始化游戏引擎所在的机型环境
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 *FgtLoadToCon(U16 ResId,U8 idx)
{
    return (ResLoadToCon(ResId,idx,g_CBnkPtr));
}
/***********************************************************************
 * 说明:     刷新屏幕函数的本体调用
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtShowFrame(void)
{
    GamShowFrame(g_VisScr);
}
/***********************************************************************
 * 说明:     显示字符串到屏幕函数的本体调用
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtStrShowV(U8 x,U8 y,U8 *buf)
{
    GamStrShowV(x,y,buf,g_VisScr);
}
/***********************************************************************
 * 说明:     封装长调用函数PlcRPicShow
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtRPicShowV(U16 id,U8 idx,U8 sx,U8 sy)
{
    PlcRPicShow(id,idx,sx,sy,false);
}
