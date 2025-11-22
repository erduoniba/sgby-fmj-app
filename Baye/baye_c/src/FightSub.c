/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	FightSub.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		游戏引擎战斗子模块
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
#undef	FightSub
#define	FightSub
#include "baye/enghead.h"
#include "baye/data-bind.h"
#define		IN_FILE	1	/* 当前文件位置 */
#include "baye/script.h"
#include "baye/bind-objects.h"

/*本体函数声明*/
/*------------------------------------------*/
/* 其他功能函数 */
void FgtAtvShowNum(U8 x,U8 y,U16 number);
void FgtFormatStr(U8 *buf,U16 num);
U32 FgtGetExp(U16 hurt);
U32 FgtDrvWeiG(U8 aIdx);
void FgtShowSNum(U8 sym,U8 idx,U16 num);
void FgtGetMapDat(U8 x,U8 y);
PersonID TransIdxToGen1(U8 idx);
U8 FgtAtkAction(U8 aIdx);
U8 FgtJNAction(FGTCMD *pcmd);
U8 FgtLoadToMem2(U8 idx,U8 *buf);
void FgtResumeMp(U8 idx);
void FgtShowSNum2(U8 sym,U8 idx,U16 num);

/***********************************************************************
 * 说明:     驱动战斗命令
 * 输入参数: pcmd-命令接口
 * 返回值  : 获得的经验
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtDrvCmd(FGTCMD *pcmd)
{
    U8	idx,exp = 0;
    JLPOS	*pos;

    idx = pcmd ->sIdx;
    pos = &g_GenPos[idx];
    pos->active = STA_END;

    BuiltAtkAttr(0,idx);
    BuiltAtkAttr(1,pcmd ->aIdx);
    switch(pcmd->type)
    {
        case CMD_ATK:
            exp = FgtAtkAction(pcmd->aIdx);
            break;
            /*	case CMD_PK:
             exp = FgtPKAction(pcmd);
             break;*/
        case CMD_STGM:
            exp = FgtJNAction(pcmd);
            break;
        case MNU_EXIT:
            pos->active = STA_WAIT;
            pos->x = g_BakUpX;
            pos->y = g_BakUpY;
            FgtSetFocus(idx);
            if (g_engineConfig.fixFoodOverFlow) {
                return 0;
            }
        case CMD_REST:
            FgtResumeMp(idx);
            return 0;
    }
    return exp;
}
/***********************************************************************
 * 说明:     显示游戏的战斗帮助
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtShowHlp()
{
    U8	nidx,idx,x,y;
    U8	tmp,tbuf[10],tbuf2[30];
    U8	*pbuf,*pic;
    RECT	big,small;
    PersonType	*per;
    JLPOS	*pos;

    if (call_hook_a("fightWillShowHelp", NULL) == 0) {
        return;
    }
    /* 初始化界面 */
    gam_clrlcd(HLP_SX + 1,HLP_SY + 1,HLP_EX - 1,HLP_EY - 1);
    gam_rect(HLP_SX,HLP_SY,HLP_EX,HLP_EY);

#define BUFFSIZE 1024

    pbuf = gam_malloc(BUFFSIZE);
    if(pbuf == NULL)
        return;
    nidx = FgtGetGenIdx(g_FoucsX,g_FoucsY);
    if(0xFF != nidx)
    {
        PersonID p;

        pos = &g_GenPos[nidx];
        /* 显示将领信息 */
        x = HLP_EX - 52;
        y = HLP_SY + 4;
        gam_rect(x,y,x + 50,y + 40);
        big.sx = HLP_SX + 4;
        big.sy = HLP_SY + 2;
        big.ex = HLP_EX - 2;
        big.ey = HLP_EY - 2;
        small.sx = x - 2;
        small.sy = y;
        small.ex = x + 50;
        small.ey = y + 42;
        BuiltAtkAttr(0,nidx);
        p = TransIdxToGen1(nidx);
        GetPersonName(p,tbuf);
        PlcMidShowStr(x + 26,y + 28,tbuf);
        pic = ResLoadToCon(GEN_HEADPIC1 + g_PIdx,1,g_CBnkPtr);
        gam_drawpic(GEN_HEADPIC1+g_PIdx, p, x + 13,y + 2, 1);

        IF_HAS_HOOK("getFighterInfo") {
            U8* info = pbuf;
            U8 generalIndex = nidx;

            BIND_GBKARR(info, BUFFSIZE);
            BIND_U8(&generalIndex);

            if (CALL_HOOK() == 0) {
                HOOK_LEAVE();
                goto tagShow;
            }
        }

        FgtLoadToMem2(dFgtHlpGen,pbuf);
        per = (PersonType *) (&g_Persons[p]);
        if(MAX_LEVEL > per->Level)
            FgtFormatStr(pbuf,per->Level);
        else
        {
            pbuf[5] = 'M';
            pbuf[6] = 'X';
        }
        FgtLoadToMem2(dFgtGenTyp,tbuf2);
        tmp = GetArmType(per) << 2;
        gam_memcpy(pbuf + 13,tbuf2 + tmp,4);
        FgtFormatStr(pbuf,per->Force);
        FgtFormatStr(pbuf,per->IQ);
        FgtFormatStr(pbuf,per->Experience);
        FgtFormatStr(pbuf,pos->hp);
        FgtFormatStr(pbuf,pos->mp);
        FgtFormatStr(pbuf,g_GenAtt[0].at);
        FgtFormatStr(pbuf,g_GenAtt[0].df);
        FgtFormatStr(pbuf,per->Arms);
        FgtLoadToMem2(dFgtState0 + pos->state,tbuf);
        gam_strcat(pbuf,tbuf);
tagShow:
        PlcStrShowS(&big,&small,pbuf);
    }
    else
    {
        /* 显示地形信息 */
        idx = FgtGetTerrain(g_FoucsX,g_FoucsY);
        IF_HAS_HOOK("getTerrainInfo") {
            U8* info = pbuf;
            U8 ter = idx;

            BIND_GBKARR(info, BUFFSIZE);
            BIND_U8(&ter);

            if (CALL_HOOK() == 0) {
                HOOK_LEAVE();
                goto tagShowT;
            }
        }
        FgtLoadToMem2(dTerrInf0 + idx,pbuf);
    tagShowT:
        c_Sx = HLP_SX + 4;
        c_Sy = HLP_SY + 4;
        c_Ex = HLP_EX - 4;
        c_Ey = HLP_EY - 4;
        GamStrShowS(c_Sx,c_Sy,pbuf);
    }

tagOut:
    GamDelay(0, 2);
    gam_free(pbuf);
}
/***********************************************************************
 * 说明:     驱动技能攻击命令
 * 输入参数: pcmd-执行命令结构
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
/* 技能的特效id */
static U8 dJNSpeId[JN_MAX] = {QIBING_SPE,QIBING_SPE,QIBING_SPE,QIBING_SPE,FIRE_SPE,WOOD_SPE,WOOD_SPE,BUBING_SPE,
    JIANBING_SPE,JIANBING_SPE,FIRE_SPE,WATER_SPE,BUMP_SPE,FENG_SPE,FENG_SPE,LIUYAN_SPE,YUAN_SPE,FIRE_SPE,WATER_SPE,FENG_SPE,0,
    0,ZHEN_SPE,XJING_SPE,0,0,0,0,YUAN_SPE};
/* 技能播放的起始和终止针 */
static U8 dJNSpeSFrm[JN_MAX] = {11,11,11,11,0,0,0,10,9,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static U8 dJNSpeEFrm[JN_MAX] = {21,21,21,21,7,7,0,20,17,17,7,7,14,7,7,7,7,7,7,7,0,0,7,7,0,0,0,0,7};
/* 技能播放的坐标 */
static U8 dJNSpeSX[JN_MAX] = {0,0,0,0,33,33,33,0,0,0,33,33,0,33,33,33,33,33,33,33,0,0,33,33,0,0,0,0,33};
/* 技能背景模式-1为有背景 0为没背景 */
static U8 dJNMode[JN_MAX] = {1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void FgtInitArmsJNNum(void);
void bind_skill_num(ObjectDef*def);

FAR void FgtLoadJNConsts(void) {
    ResItemGetN(IFACE_CONID, kdJNSpeId, dJNSpeId, sizeof(dJNSpeId));
    ResItemGetN(IFACE_CONID, kdJNSpeSFrm, dJNSpeSFrm, sizeof(dJNSpeSFrm));
    ResItemGetN(IFACE_CONID, kdJNSpeEFrm, dJNSpeEFrm, sizeof(dJNSpeEFrm));
    ResItemGetN(IFACE_CONID, kdJNSpeSX, dJNSpeSX, sizeof(dJNSpeSX));
    ResItemGetN(IFACE_CONID, kdJNMode, dJNMode, sizeof(dJNMode));
    FgtInitArmsJNNum();
}

void bind_skill_var(ObjectDef* def)
{
    DEFADD_U8ARR(dJNSpeId, JN_MAX);
    DEFADD_U8ARR(dJNSpeSFrm, JN_MAX);
    DEFADD_U8ARR(dJNSpeEFrm, JN_MAX);
    DEFADD_U8ARR(dJNSpeSX, JN_MAX);
    DEFADD_U8ARR(dJNMode, JN_MAX);
    bind_skill_num(def);
}

U8 FgtGetGenTer(U8 idx);
bool FgtChkAkRng(U8 x,U8 y);

static U8 _CommonJNAction(SkillID param, U8 aim, U8 sIdx, U8 aIdx, U8 originIdx) {
    U16 arms, prov, up;
    U8 bidx, state, buf[25], *ptr;
    SKILLEF	*skl = (SKILLEF	*)FgtGetJNPtr(param);
    U8 stateChanged;

    BuiltAtkAttr(1, aIdx);

    state = skl->state;
    if (g_GenPos[aIdx].state == STATE_SW)
        state = STATE_SW;
    CountSklHurt(param, &arms, &prov, originIdx, &state);		/* 计算该技能的兵力和粮草伤害 */
    stateChanged = g_GenPos[aIdx].state != state;
    g_GenPos[aIdx].state = state;
    if(state == STATE_DS)		/* 定身状态时，设置将领的移动力为1 */
        g_GenPos[aIdx].move = NO_MOV;
    I32 rv = call_hook_a("willShowPKAnimation", NULL);
    param -= 1;
    if(dJNSpeId[param])
    {
        if(g_LookMovie && rv == -1)
        {
            if(dJNMode[param])
                PlcRPicShow(SPE_BACKPIC,1,FGT_SPESX,FGT_SPESY,false);
            PlcMovie(dJNSpeId[param],0,dJNSpeSFrm[param],dJNSpeEFrm[param],0,FGT_SPESX + dJNSpeSX[param],FGT_SPESY);
        }
    }
    if(arms)
    {
        if(aim & 1)
        {
            up = PlcArmsMax(TransIdxToGen1(aIdx));
            bidx = dFgtArmsA;
            arms = CountOverAdd(g_GenAtt[1].arms,arms,up);
        }
        else
        {
            bidx = dFgtArmsH;
            arms = CountPlusSub(g_GenAtt[1].arms,arms);
        }
        if(g_LookMovie)
        {
            FgtLoadToMem2(bidx,buf);
            GamStrShowS(FGT_SPESX + 40,FGT_SPESY + 2,buf);
            FgtAtvShowNum(FGT_SPESX + 40,FGT_SPESY + 40,arms);
        }
        else
            FgtShowSNum2((aim & 1) ? '+' : '-',aIdx,arms);
        GamDelay(SHOW_DLYBASE * 5,false);
    }
    if(stateChanged && state != STATE_ZC && state != STATE_SW)
    {
        IF_HAS_HOOK("showStateChanged") {
            BIND_IDX("generalIndex", &aIdx);
            BIND_U8(&state);
            if (CALL_HOOK_A() == 0) {
                HOOK_LEAVE();
                goto did;
            }
        }
        GetPersonName(TransIdxToGen1(aIdx),buf);
        up = gam_strlen(buf);
        ptr = buf + up;
        FgtLoadToMem2(dFgtInSta,ptr);
        FgtLoadToMem2(dFgtState0 + state,ptr + 8);
        FgtLoadToMem2(dFgtState,ptr + 8 + 4);
        GamMsgBox(buf,1);
    }
did:
    call_hook_a("didShowPKAnimation", NULL);
    return FgtGetExp(arms);
}

U8 FgtJNAction(FGTCMD *pcmd)
{
    U8	sIdx,aIdx;
    U8	aim,bidx,buf[25];
    U8	rnd,success = 0xff;
    U16	arms,prov = 0,*provp;
    SKILLEF	*skl;
    SkillID param;

    param = SID(pcmd->param);
    sIdx = pcmd->sIdx;
    aIdx = pcmd->aIdx;
    skl = FgtGetJNPtr(param);

    if (!g_engineDebug) {
        g_GenPos[sIdx].mp -= skl->useMp;
    }

    gam_memset(buf,' ',10);

    IF_HAS_HOOK("showSkill") {
        U8 ter = FgtGetGenTer(sIdx);
        BIND_U8(&ter);
        BIND_U16EX("skillId", &pcmd->param);
        BIND_U8EX("result", &success);
        CALL_HOOK();
    }

    if (success == 0xff){
        rnd = gam_rand() % (g_GenAtt[1].canny + 20);
        success = rnd <= (g_GenAtt[0].canny >> 1);
    }
    if (g_engineDebug) {
        success = 1;
    }
    if(!success)
    {	/* 施展计谋失败 */
        ResLoadToMem(SKL_NAMID,param,buf + 1);
        buf[5] = ' ';
        FgtLoadToMem2(dFgtJNFail,buf+7);
        PlcGraMsgBox(buf,1,0xFF);
        return 0;
    }

    if(!g_LookMovie)
    {
        ResLoadToMem(SKL_NAMID,param,buf+6);
        PlcGraMsgBox(buf,1,0);
    }
    aim = skl->aim;
    if(param == 22)		/* 天变 */
    {
        FgtChgWeather();
        return 8;
    }
    else if(param == 27)	/* 围攻 */
    {
        U32 exp = FgtDrvWeiG(aIdx);
        BuiltAtkAttr(0,sIdx);		/* 恢复技能施展者的战斗变量 */
        return exp;
    }
    else if(param == 30)	/* 侦察 */
    {
        g_EneTmpProv = g_FgtParam.EProvender;
        FgtLoadToMem2(dReserve1,SHARE_MEM);
        ShowGReport(TransIdxToGen1(sIdx),SHARE_MEM);
        GamShowFrame(g_VisScr);
        return 8;
    }

    U8 _;
    /* 驱动通用的技能 */
    CountSklHurt(param, &arms, &prov, aIdx, &_);		/* 计算该技能的兵力和粮草伤害 */

    if (prov) {
        bidx = dFgtProvH;
        arms = prov;
        if(sIdx < FGT_PLAMAX)
            provp = &g_FgtParam.EProvender;
        else
            provp = &g_FgtParam.MProvender;
        prov = CountPlusSub(provp,prov);

        if(g_LookMovie)
        {
            FgtLoadToMem2(dFgtProvH, buf);
            GamStrShowS(FGT_SPESX + 40, FGT_SPESY + 2, buf);
            FgtAtvShowNum(FGT_SPESX + 40, FGT_SPESY + 40, arms);
        }
        else
            FgtShowSNum2('-', aIdx, arms);
        GamDelay(SHOW_DLYBASE * 5,false);
    }

    arms = _CommonJNAction(param, aim, sIdx, aIdx, aIdx);
    if ((aim & 2)) {
        for(int i = 0;i < FGTA_MAX;i += 1)
        {
            JLPOS* pos = &g_GenPos[i];
            SkillID skidx = param;
            if (i == aIdx)
                continue;
            if (g_FgtParam.GenArray[i] == 0)
                continue;

            if (!(aim & 4)) {
                U8 same;

                if(STATE_SW == pos->state)
                    continue;

                if(!FgtChkAkRng(pos->x, pos->y))
                    continue;

                if((sIdx >= FGT_PLAMAX && i >= FGT_PLAMAX) || (sIdx < FGT_PLAMAX && i < FGT_PLAMAX))
                    same = true;
                else
                    same = false;

                if(!FgtJNChkAim(skidx, same, i, sIdx))
                    continue;
            }
            arms = add_16(arms, _CommonJNAction(param, aim, sIdx, i, aIdx));
        }
    }
    FgtSetFocus(sIdx);
    return (FgtGetExp(arms));
}
/***********************************************************************
 * 说明:     驱动围攻指令
 * 输入参数: aIdx-目标将领脚标
 * 返回值  : 获得的经验值
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
const U8 dJNWGModu[] = {0xFF,0,0,0xFF,1,0,0,1};
U32 FgtDrvWeiG(U8 aIdx)
{
    U32	i,exp,deltax,deltay;
    U32	sx,sy;

    i = 0;
    exp = 0;
    while(1)
    {
        if(i > 3) break;
        sx = g_GenPos[aIdx].x;
        sy = g_GenPos[aIdx].y;
        deltax = dJNWGModu[i * 2];
        deltay = dJNWGModu[i * 2 + 1];
        i += 1;
        if(!sx && deltax > 0x80)
            continue;
        if(!sy && deltay > 0x80)
            continue;
        sx += deltax;
        sy += deltay;
        deltax = FgtGetGenIdx(sx,sy);
        if(deltax == 0xFF || ((aIdx < FGT_PLAMAX && deltax < FGT_PLAMAX) || (aIdx > FGT_PLAMAX && deltax > FGT_PLAMAX)))
            continue;
        FgtFrashGen(deltax,2);
        BuiltAtkAttr(0,deltax);
        exp += FgtAtkAction(aIdx);
    }
    return (exp >> 1);
}
/***********************************************************************
 * 说明:     驱动普通攻击命令
 * 输入参数: pcmd-执行命令结构
 * 返回值  : 返回获得的经验
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
const U8 FgtSpeFrm[] = {11,10,9,10,9,10};
U8 FgtAtkAction(U8 aIdx)
{
    U8	sType,dead;
    U8	sFrm,eFrm;
    U16	hurt,speId;

    hurt = CountAtkHurt();
    speId = CountPlusSub(g_GenAtt[1].arms,hurt);
    dead = (hurt != speId);			/* dead = true 被攻击将领被击溃 */
    hurt = speId;

    I32 rv = call_hook_a("willShowPKAnimation", NULL);
    /* 动画播放 */
    if(g_LookMovie)
    {
        PlcRPicShow(SPE_BACKPIC,1,FGT_SPESX,FGT_SPESY,false);
        if(TERRAIN_RIVER == g_GenAtt[1].ter)
        {
            speId = SHUISHANG_SPE;
            sFrm = 0;
            eFrm = 4;
        }
        else
        {
            sType = g_GenAtt[0].armsType;
            sFrm = FgtSpeFrm[sType] * g_GenAtt[1].armsType;
            eFrm = sFrm + FgtSpeFrm[sType] - 1;
            speId = QIBING_SPE + sType;
        }
        if (rv == -1)
            PlcMovie(speId,0,sFrm,eFrm,0,FGT_SPESX,FGT_SPESY);
        FgtAtvShowNum(FGT_SPESX + 40,FGT_SPESY + 40,hurt);
    }
    else
        FgtShowSNum2('-',aIdx,hurt);
    GamDelay(SHOW_DLYBASE * 5,false);

    /* 若当前将领将对方击毙，要额外加经验 */
    sType = *g_GenAtt[0].level - *g_GenAtt[1].level;
    if(sType > 0xE0)
        sType = 24;
    else if(!sType)
        sType = 16;
    else
        sType = 8;
    call_hook_a("didShowPKAnimation", NULL);
    return (FgtGetExp(hurt) + dead * sType);
}
/***********************************************************************
 * 说明:     动画显示指定的数值
 * 输入参数: x,y-显示的屏幕位置	number-显示的数值
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtAtvShowNum(U8 x,U8 y,U16 number)
{
    U8	pLen,pbuf[10];
    U8	wid,hgt,idx;
    U8	i,j;
    U8  *pic;

    gam_itoa(number,pbuf,10);
    pLen = gam_strlen(pbuf);
    
    pic = ResLoadToCon(NUM_PICID,1,g_CBnkPtr);
    wid = ((PictureHeadType *)pic)->wid;
    hgt = ((PictureHeadType *)pic)->hig;

    for(i = 0;i < pLen;i += 1)
    {
        idx = pbuf[i] - '0';
        GamDelay(4,false);
        for(j = 0;j < hgt/2;j += 1)
        {
            gam_drawpic(NUM_PICID, idx, x, y-j, 1);
            GamDelay(1,false);
        }
        x += wid/2;
    }
}
/***********************************************************************
 * 说明:     将num格式化输出到buf中
 * 输入参数: buf-格式化字符串	num-待格式化的字符
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtFormatStr(U8 *buf,U16 num)
{
    U8	*ptr,tbuf[10];
    U16	i,len;

    len = gam_strlen(buf);
    ptr = (U8 *)NULL;
    for(i = 0;i < len;i += 1)
    {
        if(buf[i] == '%')
        {
            ptr = buf + i;
            break;
        }
    }
    if(NULL == ptr)
        return;
    gam_itoa(num,tbuf,10);
    len = gam_strlen(tbuf);
    gam_memcpy(ptr,tbuf,len);
}
/***********************************************************************
 * 说明:     获取战斗的经验
 * 输入参数: hurt-造成的伤害
 * 返回值  : 获得的经验
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U32 FgtGetExp(U16 hurt)
{
    U32	exp,lec;

    lec = *g_GenAtt[0].level - *g_GenAtt[1].level;		/* 等级差 */
    exp = PlcExtract(hurt) >> 2;
    if(lec > 0x80)		/* 攻击者比被攻击者的等级低 */
        exp -= lec;
    else
    {
        if(exp > lec)
            exp -= lec;
        else
            exp = 0;
    }
    exp += 2;
    return exp;
}
/***********************************************************************
 * 说明:     获取当前将领在地图上显示的图块序号
 * 输入参数: i-将领脚标
 * 返回值  : 图块序号
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtGenPIdx(U8 i)
{
    U8	idx,state;
    JLPOS	*pos;
    PersonID p;

    pos = &g_GenPos[i];
    p = TransIdxToGen1(i);
    state = pos->state;

    if(state == STATE_SZ || state == STATE_HL)
        idx = 14;
    else if(FgtGetTerrain(pos->x,pos->y) == TERRAIN_RIVER)
        idx = 12;
    else
    {
        idx = GetArmType(&g_Persons[p]);
        idx <<= 1;
    }
    /* 敌人的将领面向左 */
    if(i > FGT_PLAMAX - 1)
        idx += 16;

    return idx;
}
/***********************************************************************
 * 说明:     获取指定地点的地形序号
 * 输入参数: x,y-地图坐标
 * 返回值  : 地形序号
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtGetTerrain(U8 x,U8 y)
{
    U8	idx,*ptr;
    U16	offset;

    ptr = g_FightMapData;
    offset = g_MapWid * y + x;
    idx = ptr[offset];
    if(idx > TERN_RIVERS)
    {
        if(idx == TERN_TENTS)
            return TERRAIN_TENT;
        else
            return TERRAIN_RIVER;
    }
    else if(idx > TERN_HILLS)
        return TERRAIN_HILL;
    else if(idx > TERN_WOODS)
        return TERRAIN_WOOD;
    else if(idx > TERN_THORPS)
        return TERRAIN_THORP;
    else if(idx > TERN_CITYS)
        return TERRAIN_CITY;
    else if(idx >TERN_LEAS)
        return TERRAIN_LEA;
    else if(idx > TERN_DENES)
        return TERRAIN_DENE;
    return -1;
}
/***********************************************************************
 * 说明:     获取指定的技能指针
 * 输入参数: param-技能id
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR SKILLEF *FgtGetJNPtr(SkillID param)
{
    U16	poff;

    poff = sizeof(SKILLEF);
    poff *= param - 1;
    return (SKILLEF*)(ResLoadToCon(SKL_RESID,1,g_CBnkPtr) + poff);
}
/***********************************************************************
 * 说明:     获取指定地图坐标处的将领脚标
 * 输入参数: x,y-地图坐标
 * 返回值  : 将领脚标
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtGetGenIdx(U8 x,U8 y)
{
    U8	i;
    JLPOS	*pos;

    for(i = 0;i < FGTA_MAX;i += 1)
    {
        pos = &g_GenPos[i];
        if(STATE_SW == pos->state)
            continue;
        if(x == pos->x && y == pos->y)
            return i;
    }
    return 0xFF;
}
/***********************************************************************
 * 说明:     转变当前的战场天气
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtChgWeather(void)
{
    if (!g_engineConfig.batlleDisableWeatherChange)
        g_FgtWeather = gam_rand() % 5 + 1;
    PlcRPicShow(WEATHER_PIC1 - 1 + g_FgtWeather,1,FGT_SPESX,FGT_SPESY,true);
    PlcRPicShow(WEATHER_PIC,g_FgtWeather,STA_TQX,STA_TQY,true);		/* 显示天气 */
    PlcRPicShow(WEATHER_PIC,g_FgtWeather,STA_TQX,STA_TQY,false);		/* 显示天气 */
    GamDelay(SHOW_DLYBASE * 5,false);
}
/***********************************************************************
 * 说明:     处理战斗主菜单
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtMainMenu(void)
{
    U8	*var = NULL;
    U8	tmp,idx,mbuf[42];
    RECT	pRect,pRect2, pRectSubMenu;

    idx = WK_SY + 8 + HZ_HGT * 5;
    pRect.sx = WK_SX + 8;
    pRect.ex = WK_SX + 8 + (HZ_WID << 2);
    pRect.sy = WK_SY + 8;
    tmp = WK_EY - 20;
    if(WK_EY - 20 > idx)
        tmp = idx;
    pRect.ey = tmp;
    tmp = (WK_EX - WK_SX) >> 1;
    pRect2.sx = tmp - HZ_WID;
    pRect2.ex = tmp + HZ_WID;
    tmp = (WK_EY - WK_SY) >> 1;
    pRect2.sy = tmp - HZ_HGT;
    pRect2.ey = tmp + HZ_HGT;

    pRectSubMenu.sx = pRect.ex;
    pRectSubMenu.sy = pRect.sy + HZ_HGT;
    pRectSubMenu.ex = pRectSubMenu.sx + HZ_WID*4;
    pRectSubMenu.ey = pRectSubMenu.sy + HZ_HGT;
    const char* backStr = "\xc8\xab\xbe\xfc\xb3\xb7\xcd\xcb"; //全军撤退

    idx = 0;
    while(1)
    {
        U8 hooked = 0;

        FgtLoadToMem2(dFgtSysMnu,mbuf);
    tagMenu:
        IF_HAS_HOOK("fightOpenMainMenu") {
            idx = (U8)CALL_HOOK_A();
            hooked = 1;
        } else {
            idx = (U8)PlcSplMenu(&pRect,idx,mbuf);
        }
        switch(idx)
        {
            case 1:
                if (!hooked && ((U8)PlcSplMenu(&pRectSubMenu, 0, (U8*)backStr)) == MNU_EXIT) {
                    GamShowFrame(g_VisScr);
                    goto tagMenu;
                }
                g_FgtOver = FGT_LOSE;
            case 0:
                GamShowFrame(g_VisScr);
                return 0xFF;
            case MNU_EXIT:
                GamShowFrame(g_VisScr);
                return 1;
            case 2:
                var = &g_LookMovie;
                tmp = dFgtLookMnu;
                break;
            case 3:
                var = &g_MoveSpeed;
                tmp = dFgtMoveSpe;
                break;
            case 4:
                var = &g_LookEnemy;
                tmp = dFgtLookMnu;
                break;
        }
        if (var) {
            FgtLoadToMem2(tmp,mbuf);
            tmp = (U8)PlcSplMenu(&pRect2,*var,mbuf);
            if(tmp != MNU_EXIT)
                *var = tmp;
            GamShowFrame(g_VisScr);
        }
    }
}
/***********************************************************************
 * 说明:     若休息的话，每个回合恢复一些mp
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtResumeMp(U8 idx)
{
    U8	maxmp;
    PersonType *per;
    PersonID p;

    p = TransIdxToGen1(idx);
    per = &g_Persons[p];
    maxmp = (U8)(((U16)(per->IQ) * 80 / 100 + (PlcExtract(per->Force) >> 1) + per->Level) * per->Thew / 100);
    if(g_GenPos[idx].mp < maxmp)
        g_GenPos[idx].mp += 1;
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
void FgtShowSNum2(U8 sym,U8 idx,U16 num)
{
    I16	sx,sy;
    U8	tbuf[10];
    
    tbuf[0] = sym;
    gam_itoa(num,tbuf + 1,10);
    sx = FgtGetScrX(g_GenPos[idx].x);
    sy = FgtGetScrY(g_GenPos[idx].y);
    Rect rect = {0, 0, g_screenWidth, g_screenHeight};
    if (!touchIsPointInRect(sx, sy, rect)) {
        FgtSetFocus(idx);
        sx = FgtGetScrX(g_GenPos[idx].x);
        sy = FgtGetScrY(g_GenPos[idx].y);
    }
    GamStrShowS(sx,sy,tbuf);
}
/***********************************************************************
 * 说明:     检查被选中的将领是否为控制有效
 * 输入参数: 无
 * 返回值  : 
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U8 FgtCheckIdx(void)
{
    U8	i,state;
    JLPOS	*pos;
    
    for (i = 0; i < FGT_PLAMAX; i += 1)
    {
        pos = &g_GenPos[i];
        state = pos->state;
        if(STATE_SW == state)
            continue;
        if (pos->x != g_FoucsX || pos->y != g_FoucsY)
            continue;
        if (STA_WAIT != pos->active)
            return 0xFF;
        if (STATE_HL == state || STATE_SZ == state)
            return 0xFF;
        return i;
    }
    return 0xFF;
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
PersonID TransIdxToGen1(U8 idx)
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
U8 FgtLoadToMem2(U8 idx,U8 *buf)
{
    return ResLoadToMem(IFACE_STRID,idx,buf);
}
