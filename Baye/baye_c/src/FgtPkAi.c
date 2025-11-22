/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	FightSub.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		游戏引擎战斗子模块-单挑和人工智能
 **
 *移植性声明:
 *	1、符合标准：《游戏设计标准V1.0》
 *	2、兼容模式：平移兼容|缩放兼容
 *修改历史：
 *	版本    日期     作者     改动内容和原因
 *   ----------------------------------------------------
 *	1.0    2005.5.16  高国军     基本的功能完成
 ***********************************************************************/
#include <stdlib.h>
#include "baye/stdsys.h"
#include "baye/comm.h"
#include "baye/enghead.h"
#include "baye/bind-objects.h"
#include "baye/script.h"
#include "touch.h"
#define		IN_FILE	1	/* 当前文件位置 */

/*本体函数声明*/
/*------------------------------------------*/
/* 声明abs函数以避免包含问题 */
extern int abs(int);

U8 FgtGetMCmdNear(FGTCMD *pcmd);
bool FgtJiNeng(FGTCMD *pcmd, U8 force);
bool FgtAtkCmd(FGTCMD *pcmd);
void FgtCmpMove(U8 idx);
void FgtMakeSklNam(SBUF sbuf, SkillID *sklbuf);
void FgtGetSklBuf(U8 gid,SkillID *buf);
U8 FgtCanUse(SkillID param,U8 idx);
bool FgtChkAkRng(U8 x,U8 y);
U8 FgtCntInterval(U8 x1,U8 y1,U8 x2,U8 y2);
U8 FgtGetGenTer(U8 idx);
void FgtGetHurtMax(U16 *h,U8 *a);
void FgtGetAtkRng(U8 idx,U8 x,U8 y);
void FgtGetAimPos(U8 *aimx,U8 *aimy);
U8 FgtGetNearGen(void);
void FgtLoadToMem3(U8 idx,U8 *buf);
void FgtViewForce(U8 pForce,U8 pSIdx);
PersonID TransIdxToGen3(U8 idx);
U8 FgtStatGen(U8 flag);
static void AdvancedCmdRng(U8 type,SkillID param,U8 idx);

/***********************************************************************
 * 说明:     获取计算机控制方的战斗命令
 * 输入参数: pcmd-战斗命令
 * 返回值  :
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FgtPkAi U8		FgtGetMCmd(FGTCMD *pcmd)
{
    U8	jb;
    U8	sx,sy;

    jb = FgtGetMCmdNear(pcmd);
    if(0xFF == jb)
        return 0;
    else
    {
        if(CMD_ATK == pcmd->type || CMD_STGM == pcmd->type)
        {
            jb = pcmd->aIdx;
            FgtSetFocus(jb);
            sx = FgtGetScrX(g_GenPos[jb].x);
            sy = FgtGetScrY(g_GenPos[jb].y);
            PlcMovie(STACHG_SPE,0,12,17,0,sx,sy);
        }
        return 1;
    }
}
/***********************************************************************
 * 说明:     获取计算机控制方的战斗命令
 * 输入参数: pcmd-战斗命令
 * 返回值  :
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGetMCmdNear(FGTCMD *pcmd)
{
    U8	i;

    i = FgtGetNearGen();
    if(0xFF != i)
    {
        pcmd->sIdx = i;
        /* 显示选中的将领移动范围 */
        FgtCountPath(i);
        if(g_LookEnemy)
        {
            FgtSetFocus(i);
            FgtShowMvRng();
            FgtFrashGen(i,1);
            GamShowFrame(g_VisScr);
            if(g_MoveSpeed)
                GamDelay(SHOW_DLYBASE * 5,false);
        }

        IF_HAS_HOOK("aiFightCommand") {
            BIND_U8EX("type", &pcmd->type);
            BIND_U16EX("skillId", &pcmd->param);
            BIND_U8EX("sIdx", &pcmd->sIdx);
            BIND_U8EX("aIdx", &pcmd->aIdx);

            if (CALL_HOOK() == 0) HOOK_RETURN(i);
        }
        
        /* 将领移动 */
        FgtCmpMove(i);
        if(g_LookEnemy)
        {
            FgtSetFocus(i);
            if(g_MoveSpeed)
                GamDelay(SHOW_DLYBASE * 5,false);
        }
        if (g_engineConfig.aiAttackMethod == 1) {
            FGTCMD atk;
            memcpy(&atk, pcmd, sizeof(atk));
            if(FgtAtkCmd(&atk)) {
                if (!FgtJiNeng(pcmd, 0)) {
                    memcpy(pcmd, &atk, sizeof(atk));
                }
            } else if(!FgtJiNeng(pcmd, 1)) {
                pcmd->type = CMD_REST;		/* 仍没目标，只好休息了 */
            }
        } else {
            if(!FgtJiNeng(pcmd, 0) && !FgtAtkCmd(pcmd))
                pcmd->type = CMD_REST;		/* 仍没目标，只好休息了 */
        }
    }
    return i;
}

bool FgtJiNengGetAim(FGTCMD *pcmd, PersonType*per, SkillID skidx, U8 idx)
{
    U8 i;
    JLPOS		*pos;
    bool	same;
    U16	arms;
    PersonID id;

    /* 搜索目标 */
    FgtGetCmdRng(CMD_STGM,skidx,idx);
    for(i = 0;i < FGTA_MAX;i += 1)
    {
        pos = &g_GenPos[i];
        if(STATE_SW == pos->state)
            continue;
        if(!FgtChkAkRng(pos->x,pos->y))
            continue;
        if(i >= FGT_PLAMAX)
            same = true;
        else
            same = false;
        if(!FgtJNChkAim(skidx,same,i,idx))
            continue;
        if(skidx == 17 || skidx == 29)
        {
            id = PID(g_FgtParam.GenArray[i] - 1);
            arms = PlcArmsMax(id) >> 1;
            arms += arms >> 1;		/* arms = max * 3/4 */
            /* 若施展的计谋为恢复的，目标将领的兵力至少要损失1/4 */
            if(per->Arms > arms)
                continue;
        }
        else
        {	/* 施展计谋成功 */
            pcmd->type = CMD_STGM;
            pcmd->aIdx = i;
            pcmd->param = skidx;
            return true;
        }
    }
    return false;
}

/***********************************************************************
 * 说明:     产生将领的技能
 * 输入参数: pcmd-命令结构
 * 返回值  : true-操作成功	false-操作失败
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
bool FgtJiNeng(FGTCMD *pcmd, U8 force)
{
    U8	idx,skidx;
    SkillID sklbuf[SKILL_NMAX + 1];
    U16	arms,ranv;
    PersonType	*per;
    PersonID id;
    SkillID sklid;

    idx = pcmd->sIdx;
    if(STATE_JZ == g_GenPos[idx].state)
        return false;
    id = TransIdxToGen3(idx);
    per = &g_Persons[id];
    ranv = gam_rand();

    if (force == 0) {
        /* 智力越高的，施展技能的可能性越高 */
        if(ranv % 150 > per->IQ)
            return false;
        /* 兵力剩余越少，施展技能的可能性越高(玄兵除外) */
        if(GetArmType(per) != ARM_XUANBING)
        {
            arms = PlcArmsMax(id);
            arms += arms >> 1;		/* arms = max * 1.5 */
            if(gam_rand() % arms < per->Arms)
                return false;
        }
    }

    FgtGetSklBuf(idx, sklbuf);

    if (force == 0 && g_engineConfig.aiAttackMethod == 0) {
        /* 获取要施展的技能 */
        skidx = ranv % gam_strlen(sklbuf);
        sklid = sklbuf[skidx];
        /* 自己是否符合施展的条件 */
        if(FgtCanUse(sklid,idx))
            return false;
        return FgtJiNengGetAim(pcmd, per, sklid, idx);
    } else {
        /* 查找最佳技能 */
        U8 count = gam_strlen(sklbuf);
        while (count > 0) {
            // 序号越高, 可能性越高
            U16 rand = gam_rand() % (count * count);
            U8 ind = sqrt32(rand) % count;

            sklid = sklbuf[ind];
            if(sklid != 30 && FgtCanUse(sklid,idx) == 0) {
                if (FgtJiNengGetAim(pcmd, per, sklid, idx)) {
                    return true;
                }
            }
            for (;ind < count - 1; ind++) {
                sklbuf[ind] = sklbuf[ind+1];
            }
            count -= 1;
        }
    }
    return false;
}
/***********************************************************************
 * 说明:     产生将领的攻击命令
 * 输入参数: pcmd-战斗命令结构
 * 返回值  : true-操作成功	false-操作失败
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
bool FgtAtkCmd(FGTCMD *pcmd)
{
    U8	idx,aidx;
    U16	hurt;

    idx = pcmd->sIdx;
    FgtGetCmdRng(CMD_ATK,SID(0),idx);
    BuiltAtkAttr(0,idx);
    FgtGetHurtMax(&hurt,&aidx);
    /* 产生攻击的命令 */
    if(aidx != 0xFF)
    {
        pcmd->type = CMD_ATK;
        pcmd->aIdx = aidx;
        return true;
    }
    return false;
}
/***********************************************************************
 * 说明:     移动敌人的将领
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtCmpMove(U8 idx)
{
    U8	aimx,aimy,aidx;
    U8	x,y,ox,oy,pTer;
    U8	min,delta;
    U8	ax,ay,bx,by;
    bool	flag;
    U16	hurtmax,hurt;

    /* 若当前将领是太守，不进行移动 */
    if(FgtGetGenTer(idx) == TERRAIN_CITY)
        return;

    FgtGetAimPos(&aimx,&aimy);
    /* 获取移动的目标点 */
    ox = g_GenPos[idx].x;
    oy = g_GenPos[idx].y;
    min = 0xFF;
    hurtmax = 0;
    ax = ox;
    ay = oy;
    for(x = g_PUseSX;x < FGT_MRG;x += 1)
        for(y = g_PUseSY;y < FGT_MRG;y += 1)
        {
            if(g_FightPath[y * FGT_MRG + x] >= MOV_RSTD)
                continue;
            bx = x - g_PUseSX + g_PathSX;
            by = y - g_PUseSY + g_PathSY;
            pTer = FgtGetTerrain(bx,by);
            /* 若电脑为进攻模式，且能进入城池，利马儿执行 */
            if(TERRAIN_CITY == pTer && (FGT_DF == g_FgtParam.Mode || g_engineConfig.aiDefenceMode == 1))
            {
                g_GenPos[idx].x = bx;
                g_GenPos[idx].y = by;
                FgtSetFocus(idx);
                return;
            }
            delta = FgtCntInterval(bx,by,aimx,aimy);

            /* 取伤害最大或距离目标点最近，或有防御加层但有攻击目标的地形优先考虑 */
            FgtGetAtkRng(idx,bx,by);
            FgtGetHurtMax(&hurt,&aidx);
            /* 决定是否更新移动目标点 */
            if(!hurtmax)
            {	/* 没有能造成伤害的情况下，以趋近目标优先移动 */
                hurtmax = hurt;
                if(hurtmax || min > delta)
                    flag = true;
                else if(min == delta)		/* 有距离相同的点，随机更新 */
                    flag = gam_rand() % 10;
                else
                    flag = false;
            }
            else if(hurt < hurtmax)
                flag = false;
            else if(hurt > hurtmax)
                flag = true;
            else
            {	/* 造成的伤害等于最大伤害，取地形信息 */
                if(TERRAIN_HILL == pTer || TERRAIN_WOOD == pTer ||
                   TERRAIN_THORP == pTer || TERRAIN_CITY == pTer || TERRAIN_TENT == pTer)
                    flag = true;
                else
                {
                    if (g_engineConfig.aiDefenceMode == 1) {
                        if(FgtCntInterval(bx,by,g_CityX,g_CityY) < FgtCntInterval(ax,ay,g_CityX,g_CityY))
                            flag = true;		/* 趋向于靠近城池 */
                        else
                            flag = false;
                    } else {
                        if(FgtCntInterval(bx,by,ox,oy) > FgtCntInterval(ax,ay,ox,oy))
                            flag = true;		/* 相同攻击伤害(不为0)时，动比静要好，并且要大动 */
                        else
                            flag = false;
                    }
                }
            }
            if(flag)
            {	/* 更新优先点 */
                min = delta;
                hurtmax = hurt;
                ax = bx;
                ay = by;
            }
        }
    if(min != 0xFF)
    {
        g_GenPos[idx].x = ax;
        g_GenPos[idx].y = ay;
    }
}
/***********************************************************************
 * 说明:     获取技能的序号
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR SkillID FgtGetJNIdx(U8 idx,RECT *pRect)
{
    U8	rngb,inf[10];
    SkillID param;
    SkillID buf[SKILL_NMAX + 1];
    SBUF sbuf;

    FgtGetSklBuf(idx, buf);
    FgtMakeSklNam(sbuf, buf);
    rngb = gam_strlen(sbuf) / SKILL_NAMELEN;
    pRect->sx += 4;
    pRect->ex += 4;
    if(pRect->ex > WK_EX - SKILL_NAMELEN * ASC_WID - 4)
    {
        pRect->sx -= 24;
        pRect->ex -= 24;
    }
    pRect->sy += HZ_HGT / 2;
    pRect->ey -= HZ_HGT / 2;
    if(rngb < 3)
        pRect->ey = pRect->sy + rngb * HZ_HGT;
    while(1)
    {
        U32 midx;

        rngb = 0;
        midx = (U8)PlcSplMenu(pRect,0,sbuf);
        if(MNU_EXIT == midx)
            return SID(0xFFFF);
        param = buf[midx];
        rngb = FgtCanUse(param,idx);
        if(!rngb) break;
        FgtLoadToMem3(rngb,inf);
        GamStrShowS(STA_XXX,STA_XXY,inf);
    }
    return param;
}
/***********************************************************************
 * 说明:     确定将领本身该技能是否可用
 * 输入参数: param-技能参数	idx-将领序号
 * 返回值  : 0-可用		!0-错误参数
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtCanUse(SkillID param,U8 idx)
{
    U8	terrain;
    SKILLEF	*skl;
    JLPOS	*pos;

    pos = &g_GenPos[idx];
    terrain = FgtGetGenTer(idx);
    skl = FgtGetJNPtr(param);
    if(skl->useMp > pos->mp)
        return dFgtMpErr;		/*技能点不足*/
    if(!skl->oland[terrain])
        return dFgtEthErr;		/*地形错误*/
    if(!skl->weather[g_FgtWeather-1])
        return dFgtWhrErr;		/*天候错误*/
    return 0;
}
/***********************************************************************
 * 说明:     获取技能序号缓冲
 * 输入参数: id-将领ID		buf-技能名字缓冲
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 dArmsJNNum[]={3,4,3,3,4,9};
void FgtInitArmsJNNum(void)
{
    U8* ptr = ResLoadToCon(IFACE_CONID, dFgtJNArray, g_CBnkPtr);
    for (int i = 0; i < sizeof(dArmsJNNum); i++) {
        U8 n = 0;
        for(; n < SKILL_NMAX; n++) {
            if (ptr[i*SKILL_NMAX+n] == 0) break;
        }
        dArmsJNNum[i] = n;
    }
}

#define sizeof_dArmsJNNum sizeof(dArmsJNNum)

void bind_skill_num(ObjectDef*def)
{
    DEFADD_U8ARR(dArmsJNNum, sizeof_dArmsJNNum);
}


void FgtGetSklBuf(U8 gid, SkillID*buf)
{
    U32	type,len;
    SkillID	*sklbuf,*ptr;
    PersonID id = TransIdxToGen3(gid);

    gam_memset(buf,0,SKILL_NMAX + 1);
    /* 构造技能缓冲 */
    sklbuf = buf;
    ptr = (SkillID*)ResLoadToCon(SPE_SKLID,g_PIdx,g_CBnkPtr);
    if(ptr[id])		/* 特有技能 */
    {
        *sklbuf = ptr[id];
        sklbuf += 1;
    }
    if(g_Persons[id].Belong == id + 1)	/* 君主技能 */
    {
        *sklbuf = SID(30);
        sklbuf += 1;
    }
    U8* jnptr = ResLoadToCon(IFACE_CONID,dFgtJNArray,g_CBnkPtr);
    type = GetArmType(&g_Persons[id]);
    len = ((float)dArmsJNNum[type] * g_Persons[id].Level / (MAX_LEVEL + 1)) + 1;
    jnptr += type * SKILL_NMAX;

    for (U8 i = 0; i < len; i++) {
        sklbuf[i] = jnptr[i];
    }

    IF_HAS_HOOK("getSkillIds") {
        SkillID* skillIds = buf;
        BIND_U8EX("generalIndex", &gid);
        BIND_U16ARR(skillIds, 10);
        CALL_HOOK();
    }
}
/***********************************************************************
 * 说明:     构造技能名字缓冲
 * 输入参数: 无
 * 返回值  : true-正确
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtMakeSklNam(SBUF sbuf, SkillID *sklbuf)
{
    U32	i;
    U8*ptr;
    SkillID tmp;

    ptr = sbuf;
    for(i = 0;i < SKILL_NMAX;i += 1)
    {
        tmp = sklbuf[i];
        if(!tmp) break;
        ResLoadToMem(SKL_NAMID,tmp,ptr);
        ptr += SKILL_NAMELEN;
    }
}
/***********************************************************************
 * 说明:     检测目标承受该技能的条件是否满足
 * 输入参数: param-计谋序号	idx-施展者脚标	pidx-目标脚标
 * 返回值  : 0-不满足		!0-满足
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtJNChkAim(SkillID param,U8 same, U8 aidx, U8 sidx)
{
    U8	terrain,type;
    SKILLEF *skl;

    IF_HAS_HOOK("canUseSkill") {
        BIND_U8EX("skillIndex", &param);
        BIND_U8EX("same", &same);
        BIND_U8EX("attackerIndex", &aidx);
        BIND_U8EX("targetIndex", &sidx);
        HOOK_RETURN(CALL_HOOK());
    }

    type = GetArmType(&g_Persons[g_FgtParam.GenArray[aidx] - 1]);
    terrain = FgtGetGenTer(aidx);
    skl = (SKILLEF	*)FgtGetJNPtr(param);
    /* 检测目标兵种 */
    if(!skl->earm[type])
        return 0;
    /* 检测目标的地形 */
    if(!skl->eland[terrain])
        return 0;
    /* 施展对象是否符合 */
    if(((skl->aim & 1) && !same) || (!(skl->aim & 1) && same))
        return 0;
    return 1;
}
/***********************************************************************
 * 说明:     获取指定将领脚下的地况
 * 输入参数: idx-将领脚标
 * 返回值  : 地况
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGetGenTer(U8 idx)
{
    return (FgtGetTerrain(g_GenPos[idx].x,g_GenPos[idx].y));
}
/***********************************************************************
 * 说明:     获取最大的攻击对象和攻击值
 * 输入参数:
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtGetHurtMax(U16 *h,U8 *a)
{
    U8	i,aidx;
    U16	max,hurt;
    JLPOS	*pos;

    aidx = 0xFF;
    max = 0;
    for(i = 0;i < FGT_PLAMAX;i += 1)
    {
        pos = &g_GenPos[i];
        if(STATE_SW == pos->state)
            continue;
        if(!FgtChkAkRng(pos->x,pos->y))
            continue;
        if(i == g_MainGenIdx)
        {
            aidx = i;
            max = 0xFFFF;
            break;
        }
        BuiltAtkAttr(1,i);
        hurt = CountAtkHurt();
        if(max < hurt)
        {
            max = hurt;
            aidx = i;
        }
    }
    *h = max;
    *a = aidx;
}
/***********************************************************************
 * 说明:     检查指定的将领的是否在有效范围内
 * 输入参数: x,y-将领在地图中的坐标
 * 返回值  : true-是	false-否
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
bool FgtChkAkRng(U8 x,U8 y)
{
    U8	rng;

    rng = g_FgtAtkRng[0];
    x -= g_FgtAtkRng[1];
    y -= g_FgtAtkRng[2];
    if(y >= rng || x >= rng)
        return false;
    return (g_FgtAtkRng[(U16)y * rng + x + 3] == 1);
}
/***********************************************************************
 * 说明:     获取指定将领移动到指定位置时的攻击范围
 * 输入参数: idx-将领脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtGetAtkRng(U8 idx,U8 x,U8 y)
{
    U8	bakx,baky;
    JLPOS	*pos;

    pos = &g_GenPos[idx];
    bakx = pos->x;
    baky = pos->y;
    pos->x = x;
    pos->y = y;
    BuiltAtkAttr(0,idx);
    FgtGetCmdRng(CMD_ATK,SID(0),idx);
    pos->x = bakx;
    pos->y = baky;
}

static U8 bits_to_bytes(U8* dst, U8* src, U16 w, U16 h, U8 orMode) {
    U16 lineLen = (w + 7) / 8;

    for (U16 row = 0; row < h; row ++) {
        for (U16 col = 0; col < w; col++) {
            U8 col_ind = col / 8;
            U8 col_offset = col % 8;
            U8 v = src[lineLen*row + col_ind] & (0x80 >> col_offset);

            if (v || !orMode) {
                dst[w*row + col] = !!v;
            }
        }
    }
    return 0;
}

#if 0
static void convert(U8*data, U8 from, U8 to) {
    if (from == to) {
        return;
    }

    for (U8 row=to-1;; row--) {
        for (U8 col=to-1;; col--) {
            U16 new, oldrow, oldcol;
            new = row * to + col;

            oldrow = row - (to - from) / 2;
            oldcol = col - (to - from) / 2;

            if (oldrow >= from || oldcol >= from) {
                data[new] = 0;
            } else {
                data[new] = data[oldrow*from + oldcol];
            }
            if (col == 0) break;
        }
        if (row == 0) break;
    }
}
#endif

/***********************************************************************
 * 说明:     获取命令的攻击范围
 * 输入参数: type-命令类型	param-命令参数	idx-命令执行者
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtGetCmdRng(U8 type,SkillID sklid,U8 idx)
{
    U8 rngb = 0,*ptr;
    U8 tool_rng_data[TOOL_ATT_RANGEUNIT*TOOL_ATT_RANGEUNIT] = {0};

    switch(type)
    {
        case CMD_ATK:{
            U16 offset = ATT_RANGE * (U16)GetArmType(&g_Persons[g_FgtParam.GenArray[idx] - 1]);
            rngb = ATT_RANGEUNIT;
            ptr = ResLoadToCon(IFACE_CONID,dFgtAtRange,g_CBnkPtr) + offset;

            if (g_engineConfig.enableToolAttackRange) {
                for (U8 i = 0; i < 2; i++) {
                    ToolID tid = g_Persons[g_FgtParam.GenArray[idx] - 1].Equip[i];
                    if (tid) {
                        U32 tind = tid - 1;
                        U32 offset = sizeof(GOODS) * tind;
                        GOODS *tool = (GOODS *)(ResLoadToCon(GOODS_RESID, 1, g_CBnkPtr) + offset);
                        if (tool->changeAttackRange) {
                            bits_to_bytes(tool_rng_data, tool->atRange, TOOL_ATT_RANGEUNIT, TOOL_ATT_RANGEUNIT, 1);
                            rngb = TOOL_ATT_RANGEUNIT;
                            ptr = tool_rng_data;
                        }
                    }
                }
            }
            break;
        }
            /*		case CMD_PK:
             offset = ATT_RANGE * ARM_QIBING;
             id = IFACE_CONID;
             item = dFgtAtRange;
             rngb = ATT_RANGEUNIT;
             break;*/
        case CMD_STGM: {
            U32 offset = (sklid - 1) * SKILL_RANGE;
            rngb = SKILL_RANGEUNIT;
            ptr = ResLoadToCon(SKL_RNGID, 1, g_CBnkPtr) + offset;
            break;
        }
    }
    gam_memcpy(g_FgtAtkRng + 3,ptr,rngb * rngb);
    g_FgtAtkRng[0] = rngb;
    g_FgtAtkRng[1] = (U8)(g_GenPos[idx].x - (rngb >> 1));
    g_FgtAtkRng[2] = (U8)(g_GenPos[idx].y - (rngb >> 1));

    if (g_engineConfig.enableScript) {
//        convert(g_FgtAtkRng+3, rngb, TOOL_ATT_RANGEUNIT);
//        rngb = TOOL_ATT_RANGEUNIT;
//        g_FgtAtkRng[0] = rngb;
//        g_FgtAtkRng[1] = (U8)(g_GenPos[idx].x - (rngb >> 1));
//        g_FgtAtkRng[2] = (U8)(g_GenPos[idx].y - (rngb >> 1));
        AdvancedCmdRng(type, sklid, idx);
    }
}
/***********************************************************************
 * 说明:     获取要驱动的将领序号
 * 输入参数: 无
 * 返回值  : 0xFF-驱动完成	!0xFF-将领序号
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtGetNearGen(void)
{
    U8	i,state;
    U8	ax,ay;
    U8	min,idx;
    JLPOS	*pos;

    min = 0xFF;
    idx = 0xFF;
    FgtGetAimPos(&ax,&ay);
    for(i = FGT_PLAMAX;i < FGTA_MAX;i += 1)
    {
        pos = &g_GenPos[i];
        state = pos->state;
        if(STATE_SW == state || STATE_HL == state || STATE_SZ == state)
            continue;
        if(STA_END == pos->active)
            continue;
        state = FgtCntInterval(pos->x,pos->y,ax,ay);
        if(min > state)
        {
            min = state;
            idx = i;
        }
    }
    return idx;
}
/***********************************************************************
 * 说明:     获取目标点坐标
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtGetAimPos(U8 *aimx,U8 *aimy)
{
    /* 设定目标点坐标 */
    if(FGT_AT == g_FgtParam.Mode && g_engineConfig.aiDefenceMode == 0)
    {
        *aimx = g_GenPos[g_MainGenIdx].x;
        *aimy = g_GenPos[g_MainGenIdx].y;
    }
    else
    {
        *aimx = g_CityX;
        *aimy = g_CityY;
    }
}
/***********************************************************************
 * 说明:     计算两个坐标点之间的距离
 * 输入参数: x1,y1-点1		x2,y2-点2
 * 返回值  : 距离
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtCntInterval(U8 x1,U8 y1,U8 x2,U8 y2)
{
    U8	dx,dy;

    dx = x1 - x2;
    if(x1 < x2)
        dx = 0 - dx;
    dy = y1 - y2;
    if(y1 < y2)
        dy = 0 - dy;

    return (U8)(dx + dy);
}
/***********************************************************************
 * 说明:     将领的可移动范围影射到当前的屏幕上
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtShowMvRng(void)
{
    U8	x,y,dx,dy;
    U8	tmp;
    U8	sx,sy;

    dx = g_MapSX + g_PUseSX - g_PathSX;
    dy = g_MapSY + g_PUseSY - g_PathSY;
    pushPaintColor(g_engineConfig.theme.fightMoveRangeColor);
    for(x = g_PUseSX;x < FGT_MRG;x += 1)
        for(y = g_PUseSY;y < FGT_MRG;y += 1)
        {
            /* 将x,y转换成屏幕地图坐标 */
            sx = x - dx;
            sy = y - dy;
            /* 若x,y超出屏幕的显示范围 */
            if(sx >= SCR_MAPWID || sy >= SCR_MAPHGT)
                continue;
            tmp = g_FightPath[y * FGT_MRG + x];
            /* 不覆盖将领本身 && 在移动范围内 */
            if(tmp != MOV_SPT && tmp < MOV_RSTD)
            {
                sx <<= 4;
                sy <<= 4;
                sx += WK_SX;
                sy += WK_SY;
                gam_drawpic(STEP_PIC, 0, sx, sy, 0);
            }
        }
    popPaintColor();
}
/***********************************************************************
 * 说明:     显示战场上的形势
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtShowViewInner(void);
FAR void FgtShowView(void) {
    int prev = SysScrollingTimerOpen(0);
    FgtShowViewInner();
    SysScrollingTimerOpen(prev);
}
FAR void FgtShowViewInner(void)
{
    U8	pForce,pSIdx,pPcnt;		/* 当前将领显示起始序号 每页显示将领个数 */
    U8	sx,sy,idx,pcolor;
    U8	key,tbuf[20];
    JLPOS	*pos;
    Touch touch;

    gam_clslcd();
    sx = (SCR_WID / 2 - g_MapWid) / 2 + WK_SX;
    sy = (SCR_HGT - g_MapHgt) / 2 + WK_SY + 20;
    FgtLoadToMem3(dPowerCmp,tbuf);
    GamStrShowS(WK_SX + 5,WK_SY + 3,tbuf);		/* 战场形势图 */
    FgtLoadToMem3(dDaysInf,tbuf);
    gam_ltoa(g_FgtBoutCnt,tbuf + 3,10);
    tbuf[gam_strlen(tbuf)] = ' ';
    GamStrShowS(WK_SX + 5,WK_SY + 5 + HZ_HGT,tbuf);	/* 进行日期 */
    FgtLoadToMem3(dReserve0,tbuf);
    GamStrShowS((SCR_WID / 2 - 4 * HZ_WID) / 2 + WK_SX,sy - (HZ_HGT + 3),tbuf);	/* 将领位置 */

    pPcnt = (WK_EY - (HZ_HGT * 2 + 6)) / HZ_HGT;
    FgtViewForce(1,0);
    gam_rect(sx - 1,sy - 1,sx + g_MapWid,sy + g_MapHgt);
    pcolor = COLOR_BLACK;
    pForce = 1;
    pSIdx = 0;

    if (call_hook_a("didShowFightSituation", NULL) == 0) {
        return;
    }
    
    while(1)
    {

        for(idx = 0;idx < FGTA_MAX;idx += 1)
        {
            pos = (JLPOS *)&g_GenPos[idx];
            if(STATE_SW == pos->state)
                continue;
            gam_putpixel(sx + pos->x,sy + pos->y,pcolor);
        }
        if(COLOR_BLACK == pcolor)
            pcolor = COLOR_WHITE;
        else
            pcolor = COLOR_BLACK;
        call_hook("didRefreshFightSituation", NULL);
        key = GamDelay(50,true);
        if(!key)
            continue;
        MsgType msg;
        GamGetLastMsg(&msg);
    tagProcessMessage:
        if (VM_CHAR_FUN == msg.type) {
            switch(msg.param)
            {
                case VK_ENTER:
                case VK_EXIT:
                    return;
                case VK_LEFT:
                case VK_RIGHT:
                    pForce = !pForce;
                    pSIdx = 0;
                    break;
                case VK_UP:
                    if(pSIdx)
                        pSIdx -= pPcnt;
                    break;
                case VK_DOWN:
                    if(FgtStatGen(pForce) < pPcnt)
                        break;
                    if(pSIdx < FgtStatGen(pForce) - pPcnt)
                        pSIdx += pPcnt;
                    break;
            }
        } else if (VM_TOUCH == msg.type) {
            touchUpdate(&touch, msg);
            if (msg.param == VT_TOUCH_UP) {
                if (touch.completed) {
                    if (touch.moved) {
                        I16 dx = touch.currentX - touch.startX;
                        I16 dy = touch.currentY - touch.startY;
                        U8 up = dy < 0;
#define THRESHOLD 20
                        dx = abs(dx);
                        dy = abs(dy);
                        if (dx > THRESHOLD) {
                            msg.type = VM_CHAR_FUN;
                            msg.param = VK_LEFT;
                            goto tagProcessMessage;
                        } else if (dy > THRESHOLD) {
                            msg.type = VM_CHAR_FUN;
                            msg.param = up ? VK_DOWN : VK_UP;
                            goto tagProcessMessage;
                        }
                    } else {
                        return;
                    }
                }
            }
        }
        FgtViewForce(pForce,pSIdx);
    }
}
/***********************************************************************
 * 说明:     显示势力情报
 * 输入参数: pForce-交战的一方	pSIdx-开始序号
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtViewForce(U8 pForce,U8 pSIdx)
{
    U8	i,pPCnt,pGIdx;
    U8	tmp,tbuf[20];
    U16	provender;
    PersonID p;
    
    pGIdx = pForce * FGT_PLAMAX;
    gam_clrlcd(WK_SX + SCR_WID / 2,WK_SY,WK_EX,WK_EY);
    gam_rect(WK_SX + SCR_WID / 2,WK_SY,WK_EX,WK_EY);
    gam_line(WK_SX + SCR_WID / 2,WK_SY + HZ_HGT + 2,WK_EX,WK_SY + HZ_HGT + 2);
    gam_line(WK_SX + SCR_WID / 2,WK_SY + HZ_HGT * 2 + 4,WK_EX,WK_SY + HZ_HGT * 2 + 4);
    p = TransIdxToGen3(pGIdx);
    GetPersonName(PID(g_Persons[p].Belong - 1),tbuf);
    i = gam_strlen(tbuf);
    tbuf[i] = ' ';
    FgtLoadToMem3(dArmyInf,tbuf + i);
    i = SCR_WID / 2 + SCR_WID / 4 - (i + 2) * ASC_WID / 2;
    GamStrShowS(WK_SX + i,WK_SY + 2,tbuf);				/* 显示军团势力 */
    FgtLoadToMem3(dProvInf,tbuf);
    if(pForce)
    {
        if(!g_EneTmpProv)
            FgtLoadToMem3(dNoView,tbuf + 5);
        else
            gam_ltoa(g_EneTmpProv,tbuf + 5,10);
    }
    else
        gam_ltoa(g_FgtParam.MProvender,tbuf + 5,10);
    GamStrShowS(WK_SX + SCR_WID / 2 + 3,WK_SY + HZ_HGT + 4,tbuf);	/* 显示粮草 */
    
    pPCnt = (WK_EY - (HZ_HGT * 2 + 6)) / HZ_HGT;
    pSIdx += pGIdx;
    for(i = 0;i < pPCnt;i += 1)
    {
        tmp = i + pSIdx;
        if(tmp - pGIdx >= FGT_PLAMAX)
            break;
        p = g_FgtParam.GenArray[tmp];
        if(!p)
            break;
        p -= 1;
        provender = g_Persons[p].Arms;
        GetPersonName(p,tbuf);
        tmp = gam_strlen(tbuf);
        gam_memset(tbuf + tmp,' ',20 - tmp);
        gam_ltoa(provender,tbuf + 8,10);
        GamStrShowS(WK_SX + SCR_WID / 2 + 3,WK_SY + HZ_HGT * (2 + i) + 6,tbuf);	/* 显示将领 */
    }
    
}
/***********************************************************************
 * 说明:     获取上战场将领的个数
 * 输入参数: flag-指定战斗的一方
 * 返回值  : 将领个数
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U8 FgtStatGen(U8 flag)
{
    U8	i,pSIdx;
    
    pSIdx = flag * FGT_PLAMAX;
    for(i = 0;i < FGT_PLAMAX;i += 1)
    {		
        if(!g_FgtParam.GenArray[i + pSIdx])
            break;
    }
    return i;
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
void FgtLoadToMem3(U8 idx,U8 *buf)
{
    ResLoadToMem(IFACE_STRID,idx,buf);
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
PersonID TransIdxToGen3(U8 idx)
{
    return PID(g_FgtParam.GenArray[idx] - 1);
}

static void AdvancedCmdRng(U8 type, SkillID param, U8 idx) {
    IF_HAS_HOOK("calcAttackRange") {
        BuiltAtkAttr(0, idx);

        U8 ter = FgtGetTerrain(g_GenPos[idx].x, g_GenPos[idx].y);
        U16 personIndex = g_FgtParam.GenArray[idx] - 1;
        U8*range = g_FgtAtkRng+3;
        U8 rangeSize = g_FgtAtkRng[0];
        SkillID* skillId = &param;

        BIND_U8(&ter);
        BIND_U16(&personIndex);
        BIND_U8(&type);
        BIND_U16(skillId);
        BIND_U8(&rangeSize);
        BIND_U8ARR(range, TOOL_ATT_RANGE);

        if (CALL_HOOK() == 0) {
            g_FgtAtkRng[0] = rangeSize;
            g_FgtAtkRng[1] = (U8)(g_GenPos[idx].x - (rangeSize >> 1));
            g_FgtAtkRng[2] = (U8)(g_GenPos[idx].y - (rangeSize >> 1));
        }
    }
}
