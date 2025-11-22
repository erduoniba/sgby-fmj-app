/***********************************************************************
 *Copyright (c)2005 , 东莞步步高教育电子分公司
 *All rights reserved.
 **
 文件名称：	FgtCount.c
 *文件标识：	步步高电子词典的游戏引擎模块
 *摘要：		游戏引擎战斗计算子模块
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
#undef	FgtCount
#define	FgtCount
#include "baye/enghead.h"
#include "baye/bind-objects.h"
#define		IN_FILE	1	/* 当前文件位置 */


/*本体函数声明*/
/*------------------------------------------*/
PersonID TransIdxToGen(U8 idx);
U16 FgtAllArms(U8 flag);
void CountBaseAttr(U8 i);

/***********************************************************************
 * 说明:     电脑攻击电脑,直接产生战斗结果
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtCountWon(void)
{
    U8	randv;
    U16	arms1,arms2;
    U16	prov1,prov2;


    randv = gam_rand() % 101;
    arms1 = FgtAllArms(0);
    if(!arms1)
        g_FgtOver = FGT_LOSE;
    arms2 = FgtAllArms(1);
    if(!arms2)
        g_FgtOver = FGT_WON;
    prov1 = g_FgtParam.MProvender;
    prov2 = g_FgtParam.EProvender;
    if(arms1 > arms2)
    {
        if((arms1 >> 1) > arms2)
            g_FgtOver = (randv < 30) + 1;		/* 兵力大于对方的两倍，有70%的机会获取胜利 */
        else if(prov1 > prov2)
            g_FgtOver = (randv < 40) + 1;		/* 兵力和兵粮都大于对方，有60%的机会获取胜利 */
        else
            g_FgtOver = (randv < 60) + 1;		/* 兵力大于对方，兵粮小于对方，有40%的机会获取胜利 */
    }
    else
    {
        if(arms1 < (arms2 >> 1))
            g_FgtOver = (randv > 2) + 1;		/* 兵力小于对方1/2时，有2%的机会获取胜利 */
        else if(prov1 > prov2)
            g_FgtOver = (randv > 30) + 1;		/* 兵力小于对方，兵粮大于对方，有30%的机会获取胜利 */
        else
            g_FgtOver = (randv > 10) + 1;		/* 兵力和兵粮都小于对方时，有10%的机会获取胜利 */
    }
}
/***********************************************************************
 * 说明:     初始化战斗对象
 * 输入参数: 无
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void CountInitGen(void)
{
    U8	i, cnt = 0;

    for(i = 0; i < FGTA_MAX; i += 1)
    {
        if(!(i % FGT_PLAMAX))
            cnt = 0;
        g_GenPos[i].state = STATE_ZC;
        if(g_FgtParam.GenArray[i] == 0)
        {
            cnt += 1;
            g_GenPos[i].state = STATE_SW;
            if(cnt == FGT_PLAMAX)
            {
                printf("fight over\n");
                g_FgtOver = FGT_LOSE - (i / FGT_PLAMAX);
                return;
            }
            else
                continue;
        }
        CountBaseAttr(i);
    }
}
/***********************************************************************
 * 说明:     计算将领的基本属性
 * 输入参数: pos-战斗属性	per-将领资料
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void CountBaseAttr(U8 i)
{
    U16	at,df,lev,thew;
    JLPOS	*pos;
    PersonType *per;
    PersonID p;

    pos = &g_GenPos[i];
    p = TransIdxToGen(i);
    per = &g_Persons[p];

    at = per->Force;
    df = per->IQ;
    lev = per->Level;
    thew = per->Thew;
    /* 计算将领的生命 */
    pos->hp = (at * 80 / 100 + df * 30 / 100 + lev) * thew / 100;

    /* 计算将领的技能点 */
    pos->mp = (df * 80 / 100 + (PlcExtract(at) >> 1) + lev) * thew / 100;

    /* 计算将领的移动力 */
    CountMoveP(i);

#if	(GAM_VER == GAM_DEBUG_MODE)
    pos->state = STATE_ZC;
#endif
}
/***********************************************************************
 * 说明:     计算将领的移动力
 * 输入参数: i - 将领脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
/* 兵种起始移动力 */
static U8 FgtIntMove[6]={MOV_QIBING,MOV_BUBING,MOV_GOBING,MOV_SHUIJUN,MOV_JIBING,MOV_XUANBING};
FAR void CountMoveP(U8 i)
{
    U8	lp,arm;
    GOODS	*ptr;
    JLPOS	*pos;
    PersonType *per;
    PersonID p;

    pos = &g_GenPos[i];

    IF_HAS_HOOK("countMove") {
        BIND_U8EX("generalIndex", &i);
        if (CALL_HOOK() == 0) {
            HOOK_LEAVE();
            return;
        }
    }

    if (pos->state == STATE_DS) {
        pos->move = 1;
        return;
    }

    p = TransIdxToGen(i);
    per = &g_Persons[p];

    arm = GetArmType(per);
    pos->move = FgtIntMove[arm];
    for(lp = 0; lp < 2; lp += 1)
    {	/* 累加马匹移动力 */
        ToolID t;
        t = per->Equip[lp];
        if(!t)
            continue;
        ptr = (GOODS *)(ResLoadToCon(GOODS_RESID,1,g_CBnkPtr) + (U16)(t - 1) * sizeof(GOODS));
        if(NULL == ptr)
            continue;
        if(ptr->move != 0)
            pos->move += ptr->move;
    }
    if(pos->move > FGT_MVMAX)
        pos->move = FGT_MVMAX;
}
/***********************************************************************
 * 说明:     计算普通攻击的兵力伤害
 * 输入参数: sIdx-源脚标		aIdx-目标脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
/* 相克系数 */
static float SubduModu[ARMTYP_NUM][ARMTYP_NUM] = {
    {1.0,1.2,0.8,1.0,0.7,1.3},{0.8,1.0,1.2,1.0,0.6,1.2},{1.2,0.8,1.0,1.0,1.1,1.2},
    {1.0,1.0,1.0,1.0,1.0,1.0},{1.1,1.3,0.9,1.0,1.0,1.5},{0.6,0.6,0.6,0.6,0.6,0.6}};
FAR U16 CountAtkHurt(void)
{
    U16	hurt;

    IF_HAS_HOOK("countAttackHurt") {
        BIND_U16(&hurt);
        
        if (CALL_HOOK() == 0) {
            HOOK_RETURN(hurt);
        }
    }
    /* 基本伤害 hurt = (at / df) * arms / 8 */
    hurt = (float)(g_GenAtt[0].at) / g_GenAtt[1].df * ((*g_GenAtt[0].arms) >> 3);
    /* 相克加层 hurt *= modu */
    hurt *= SubduModu[g_GenAtt[0].armsType][g_GenAtt[1].armsType];

    return (U16)(hurt + 10);	/* +10是为了防止双方兵力<4时，出现的平局状态 */
}
/***********************************************************************
 * 说明:     构造参加战斗的两个将领的数据
 * 输入参数: idx-g_GenAtt的脚标	pIdx-g_GenPos的脚标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
static float AtkModulus[] ={1.0,0.8,0.9,0.8,1.3,0.4};	/* 各兵种攻击系数 */
static float DfModulus[] ={0.7,1.2,1.0,1.1,1.2,0.6};	/* 各兵种防御系数 */
static float TerrDfModu[] = {1.0,1.0,1.3,1.15,1.1,1.5,1.2,0.8};	/* 各种地形防御系数 */

static U16 calcAt(I8 mModu, U16 at) {
    if (0 <= mModu && mModu <= 3) {
        // 兼容原版计算方式
        return at >> mModu;
    } else
    {
        // 优化计算方式
        if (mModu > 99) mModu = 99;
        if (mModu < -99) mModu = -99;
        return at - (at * (U32)mModu / 100);
    }
}


FAR void BuiltAtkAttr(U8 idx,U8 pIdx)
{
    U8	pTer;
    PersonID pGen;
    I8  mModu;
    U8	*mptr;
    JLATT	*pAtk;
    PersonType	*pTyp;

    pGen = TransIdxToGen(pIdx);
    if(pGen > PERSON_COUNT - 1)
        return;
    pTyp = (PersonType *)(&g_Persons[pGen]);
    pAtk = (JLATT *)(&g_GenAtt[idx]);
    pAtk->arms = &(pTyp->Arms);
    pAtk->exp = &(pTyp->Experience);
    pAtk->level = &(pTyp->Level);
    pAtk->generalIndex= pIdx;
    pAtk->canny = pTyp->IQ + pTyp->Level + 5;
    pTer = FgtGetTerrain(g_GenPos[pIdx].x,g_GenPos[pIdx].y);
    pAtk->ter = pTer;
    pAtk->bile = 0;
    pAtk->armsType = GetArmType(pTyp);
    pAtk->arms = &(pTyp->Arms);
    pAtk->exp = &(pTyp->Experience);

    IF_HAS_HOOK("battleBuildAttackAttriutes") {
        BIND_U8EX("index", &idx);
        BIND_U8EX("generalIndex", &pIdx);
        if (CALL_HOOK() == 0) {
            HOOK_RETURN();
        }
    }

    /* 获取地形影响 */
    mptr = ResLoadToCon(IFACE_CONID,dFgtLandF,g_CBnkPtr);
    mptr += pAtk->armsType * TERRAIN_MAX;
    mModu = (I8)mptr[pTer];

    U16 atRatio = pTyp->Force;
    U16 dfRatio = pTyp->IQ;

    if (g_engineConfig.enableCustomRatio) {
        atRatio = 0;
        atRatio += pTyp->IQ * g_engineConfig.ratioOfAttToIQ / 10;
        atRatio += pTyp->Force * g_engineConfig.ratioOfAttToForce / 10;
        atRatio += pTyp->Age * g_engineConfig.ratioOfAttToAge / 10;

        dfRatio = 0;
        dfRatio += pTyp->IQ * g_engineConfig.ratioOfDefenceToIQ / 10;
        dfRatio += pTyp->Force * g_engineConfig.ratioOfDefenceToForce / 10;
        dfRatio += pTyp->Age * g_engineConfig.ratioOfDefenceToAge / 10;
    }
    
    U16 at = (U16)(atRatio * (pTyp->Level + 10) * AtkModulus[pAtk->armsType]);
    U16 df = (U16)(dfRatio * (pTyp->Level + 10) * DfModulus[pAtk->armsType]);

    pAtk->at = calcAt(mModu, at);
    pAtk->df = calcAt(mModu, df);
    pAtk->df *= TerrDfModu[pTer];
}
/***********************************************************************
 * 说明:     计算兵粮的消耗
 * 输入参数: 无
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void CountProvUse(void)
{
    U8	i;
    U16	arms,*armsp;
    U8 ratio = 3;

    if (g_engineConfig.ratioOfFoodToArmsPerDay) {
        ratio = g_engineConfig.ratioOfFoodToArmsPerDay;
    }

    for(i = 0;i < 2;i += 1)
    {
        arms = FgtAllArms(i);
        arms = PlcExtract(arms) / ratio;
        if(i)
            armsp = &g_FgtParam.EProvender;
        else
            armsp = &g_FgtParam.MProvender;

        if(arms > *armsp)
            *armsp = 0;
        else
            *armsp -= arms;
    }
}
/***********************************************************************
 * 说明:     非负减法-将一个数减去另一个数，不够减补0
 * 输入参数: a-被减数		v-减数
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U16 CountPlusSub(U16 *a,U16 v)
{
    U16	tmp;

    tmp = *a;
    if(tmp > v)
        tmp -= v;
    else
    {
        v = tmp;
        tmp = 0;
    }
    *a = tmp;

    return 	(U16)v;
}
/***********************************************************************
 * 说明:     不溢出的加法
 * 输入参数: a-加数	v-被加数	up-上限
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR U16 CountOverAdd(U16 *a,U16 v,U16 up)
{
    U16	tmp;

    tmp = *a;
    if(tmp + v > up)
    {
        v = up - tmp;
        tmp = up;
    }
    else
        tmp += v;
    *a = tmp;

    return (U16)v;
}
/***********************************************************************
 * 说明:     计算技能的破坏力
 * 输入参数: skl-技能指针	arms-兵力	prov-粮草
 * 返回值  : 0-操作成功		!0-错误代码
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void CountSklHurt(SkillID param, U16 *arms, U16 *prov, U8 origin, U8*state)
{
    U8	i,eTyp;
    U8	oTer,eTer;
    U16	cnt,count[2];
    SKILLEF	*skl;

    IF_HAS_HOOK("countSkillHurt") {
        BIND_U16EX("skillId", &param);
        BIND_U16EX("hurt", arms);
        BIND_U8EX("origin", &origin);
        BIND_U8(state);
        BIND_U16(prov);

        if (CALL_HOOK() == 0) {
            HOOK_RETURN();
        }
    }

    skl = (SKILLEF	*)FgtGetJNPtr(param);

    count[0] = skl->power;
    count[1] = skl->destroy;
    oTer = g_GenAtt[0].ter;
    eTyp = g_GenAtt[1].armsType;
    eTer = g_GenAtt[1].ter;
    for(i = 0;i < 2;i += 1)
    {
        cnt = count[i];
        if(!cnt) continue;
        cnt *= (float)skl->weather[g_FgtWeather - 1] / 100;
        cnt *= (float)skl->earm[eTyp] / 100;
        cnt *= (float)skl->eland[eTer] / 100;
        cnt *= (float)skl->oland[oTer] / 100;
        count[i] = cnt;
    }
    *arms = count[0];
    *prov = count[1];
}
/***********************************************************************
 * 说明:     获取总兵力
 * 输入参数: flag-0前10个将的一方
 * 返回值  : 总兵力
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
U16 FgtAllArms(U8 flag)
{
    U8	i,idx,start;
    U16	rev;
    PersonID p;

    start = flag * FGT_PLAMAX;
    rev = 0;
    for(i = 0;i < FGT_PLAMAX;i += 1)
    {
        idx = i + start;
        if(STATE_SW == g_GenPos[idx].state)
            continue;
        p = TransIdxToGen(idx);
        rev += g_Persons[p].Arms;
    }
    return (U16) rev;
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
PersonID TransIdxToGen(U8 idx)
{
    if(idx > FGTA_MAX - 1)
        return PID(0xFFFF);
    return PID(g_FgtParam.GenArray[idx] - 1);
}


/*===========================================================================================================*/
/* 行军算法函数 */
void FgtGetRelief(U8 idx);
void FgtTransMove(U8 idx);
void FgtSetDifPot(U8 idx);
void FgtSetSamPot(U8 idx);
void FgtUnfurlTree(U8 idx,U8 *buf,U8 *len);

/***********************************************************************
 * 说明:     根据行军算法计算路径
 * 输入参数: idx-被移动将领序号
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
FAR void FgtCountPath(U8 idx)
{
    U8	chi1[MOVCHI_LEN],chi2[MOVCHI_LEN];
    U8	chi1P,chi2P;
    U8	i,lp,tmp,sidx;
    U8	*ap,*sptr,*aptr;
    
    IF_HAS_HOOK("countMoveRange") {
        BIND_U8EX("generalIndex", &idx);
        g_PUseSX = 0;
        g_PUseSY = 0;
        g_PathSX = g_GenPos[idx].x - 7;
        g_PathSY = g_GenPos[idx].y - 7;
        if (CALL_HOOK() == 0) {
            HOOK_LEAVE();
            return;
        }
    }

    CountMoveP(idx);
    FgtGetRelief(idx);
    FgtTransMove(idx);

    /* 对行军树进行扩展 */
    sidx = (g_GenPos[idx].move - 1) * (FGT_MRG + 1);
    chi1[0] = sidx;
    chi1P = 1;
    chi2P = 0;
    lp = chi1P;
    ap = &chi2P;
    sptr = chi1;
    aptr = chi2;
    while(lp)
    {
        for(i = 0; i < lp; i += 1)
        {
            tmp = sptr[i];
            if(g_FightPath[tmp] < 2)
                continue;
            FgtUnfurlTree(tmp,aptr,ap);
        }
        if (sptr == chi1)
        {
            chi1P = 0;
            sptr = chi2;
            aptr = chi1;
            lp = chi2P;
            ap = &chi1P;
        }
        else
        {
            chi2P = 0;
            sptr = chi1;
            aptr = chi2;
            lp = chi1P;
            ap = &chi2P;
        }
    }
    /* 将自己影射成特殊值MOV_SPT */
    g_FightPath[sidx] = MOV_SPT;
    /* 影射我放将领的位置 */
    FgtSetSamPot(idx);
    /* 若为奇门状态，需要重新影射敌方将领 */
    if(g_GenPos[idx].state == STATE_QM)
        FgtSetDifPot(idx);
}
/***********************************************************************
 * 说明:     获取行军范围内的地貌情况
 * 输入参数: idx-执行移动的将领序号
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 *	       高国军	       2005.9.19       修改了敌人瞬间移动的问题
 ***********************************************************************/
void FgtGetRelief(U8 idx)
{
    U8	*ptr;
    U8	x,y;
    U8	mx,my;			/* 地图影射起始坐标 */
    U8	bx,by,bw,bh;	/* 行军buf使用范围 */
    U8	move;
    U16	moff,boff;

    /* 将行军buf先初始化为越界 */
    gam_memset(g_FightPath,MOV_OVER,FGT_MRG*FGT_MRG);

    /* 计算地图影射坐标 */
    x = g_GenPos[idx].x + 1;
    y = g_GenPos[idx].y + 1;
    move = g_GenPos[idx].move;
    if(x >= move)
    {
        mx = x - move;
        bx = 0;
    }
    else
    {
        mx = 0;
        bx = move - x;
    }

    if(y >= move)
    {
        my = y - move;
        by = 0;
    }
    else
    {
        my = 0;
        by = move - y;
    }

    bw = move - bx;
    if(g_MapWid - x > move)
        bw += move - 1;
    else
        bw += g_MapWid - x;

    bh = move - by;
    if(g_MapHgt - y > move)
        bh += move - 1;
    else
        bh += g_MapHgt - y;

    /* 影射地图 */
    ptr = g_FightMapData;
    moff = my;
    moff *= g_MapWid;
    moff += mx;
    boff = by;
    boff *= FGT_MRG;
    boff += bx;
    for(y = 0; y < bh; y += 1)
    {
        gam_memcpy(g_FightPath + boff,ptr + moff,bw);
        moff += g_MapWid;
        boff += FGT_MRG;
    }
    g_PathSX = mx;
    g_PathSY = my;
    g_PUseSX = bx;
    g_PUseSY = by;
}
/***********************************************************************
 * 说明:     将当前的地形转换为移动力消耗，并影射将领阻挡
 * 输入参数: idx-执行移动的将领序号
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtTransMove(U8 idx)
{
    U8	i,type;
    U8	x,lp;
    U8	*mptr = NULL;
    
    U8  mbuf[256];

    /* 获取被操作的兵种 */
    type = GetArmType(&g_Persons[g_FgtParam.GenArray[idx] - 1]);

    IF_HAS_HOOK("countLandResistance") {
        U8* result = mbuf;
        BIND_U8EX("generalIndex", &idx);
        BIND_U8ARR(result, sizeof(mbuf));
        if (CALL_HOOK() == 0) {
            mptr = mbuf;
        }
    }
    
    if (mptr == NULL) {
        mptr = ResLoadToCon(IFACE_CONID,dFgtLandR,g_CBnkPtr);
        mptr += type * FGT_TILMAX;
    }
    
    /* 将地况转换成移动力消耗 */
    for(lp = 0; lp < FGT_MRG*FGT_MRG; lp += 1)
    {
        i = g_FightPath[lp];
        if(MOV_OVER == i)
            continue;
        g_FightPath[lp] = mptr[i];
    }

    /* 影射敌方将领阻力 */
    FgtSetDifPot(idx);
    /* 设置移动力 */
    x = g_GenPos[idx].move-1;
    lp = x * (FGT_MRG + 1);
    g_FightPath[lp] = g_GenPos[idx].move;
}
/***********************************************************************
 * 说明:     设置我方将领的阻挡
 * 输入参数: x,y-敌方将领行军范围内的坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtSetSamPot(U8 idx)
{
    U8	i,lps;
    U8	sx,sy;
    U8	x,y,off;
    JLPOS	*pos;

    /* 设置循环的起始点 */
    if (idx < FGT_PLAMAX)
        lps = 0;
    else
        lps = FGT_PLAMAX;

    /* 设置阻力 */
    sx = g_PathSX - g_PUseSX;
    sy = g_PathSY - g_PUseSY;
    for(i = lps; i < lps + FGT_PLAMAX; i += 1)
    {
        pos = &g_GenPos[i];
        if(STATE_SW == pos->state)
            continue;
        if(idx == i)
            continue;
        x = pos->x - sx;
        y = pos->y - sy;
        if(x > FGT_MRG || y > FGT_MRG)
            continue;
        off = y * FGT_MRG + x;
        if(off < FGT_MRG * FGT_MRG)
            g_FightPath[off] = MOV_NOT;
    }
}
/***********************************************************************
 * 说明:     设置敌方将领的阻挡
 * 输入参数: x,y-敌方将领行军范围内的坐标
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
void FgtSetDifPot(U8 idx)
{
    U8	i,x,y;
    U8	sx,sy;
    U8	lps,poff;
    JLPOS	*pos;

    /* 设置循环的起始点 */
    if (idx < FGT_PLAMAX)
        lps = FGT_PLAMAX;
    else
        lps = 0;

    /* 设置阻力 */
    sx = g_PathSX - g_PUseSX;
    sy = g_PathSY - g_PUseSY;
    for(i = lps; i < lps + FGT_PLAMAX; i += 1)
    {
        pos = &g_GenPos[i];
        if(STATE_SW == pos->state)
            continue;
        x = pos->x - sx;
        y = pos->y - sy;
        if(x > FGT_MRG || y > FGT_MRG)
            continue;
        poff = y;
        poff = poff * FGT_MRG + x;
        if(poff > FGT_MRG * FGT_MRG -1)		/* add 2005-8-25 2:46下午 容错，上面g_PathSX和g_PUseSX的使用混淆。 */
            continue;
        g_FightPath[poff] = MOV_NOT;
        /* 若当前将领是奇门状态，就不需要设置待激活点了 */
        if(g_GenPos[idx].state == STATE_QM)
            continue;
        /* 将该将领的上下左右都设置为待激活点 */
        if(x && poff > 1)
            g_FightPath[poff - 1] |= MOV_WKP;
        if(x < FGT_MRG-1 && poff < FGT_MRG * FGT_MRG -1)
            g_FightPath[poff + 1] |= MOV_WKP;
        if(y && poff > FGT_MRG - 1)
            g_FightPath[poff - FGT_MRG] |= MOV_WKP;
        if(y < FGT_MRG-1 && poff < FGT_MRG * (FGT_MRG -1) - 1)
            g_FightPath[poff + FGT_MRG] |= MOV_WKP;
    }
}
/***********************************************************************
 * 说明:     展开行军树
 * 输入参数: idx-节点序号	buf-目标buf		len-目标buf中的节点个数
 * 返回值  : 无
 * 修改历史:
 *               姓名            日期             说明
 *             ------          ----------      -------------
 *             高国军          2005.5.16       完成基本功能
 ***********************************************************************/
/* 树展开的方向 */
const U8 FgtUnfurlDrt[4] = {-FGT_MRG,FGT_MRG,-1,1};
void FgtUnfurlTree(U8 idx,U8 *buf,U8 *len)
{
    U8	power,plen,resistance;
    U8	i,poff,tmp;
    
    plen = *len;
    power = g_FightPath[idx];
    for (i = 0; i < 4; i += 1)
    {
        tmp = 0;
        poff = idx + FgtUnfurlDrt[i];
        if(poff > FGT_MRG * FGT_MRG -1)
            continue;
        resistance = g_FightPath[poff];
        if (resistance < MOV_RSTD)
            continue;
        if (resistance == MOV_NOT)
            continue;
        if (resistance == MOV_OVER)
            continue;
        /* 待激活点 */
        if (resistance > MOV_WKP)
        {
            tmp = resistance - MOV_WKP;
            if(power > tmp)
                tmp = 1;
            else
                tmp = 0;
        }
        /* 正常阻力点 */		
        if (!tmp)
        {
            tmp = power + MOV_RSTD;
            if(tmp > resistance)
                tmp -= resistance;
            else
                tmp = 0;
        }
        /* 增加新节点 */
        if (tmp)
        {
            g_FightPath[poff] = tmp;
            buf[plen] = poff;
            plen += 1;
        }
    }
    *len = plen;
}

FAR void FgtLoadJNConsts(void);

FAR void FgtLoadConsts(void) {
    ResItemGetN(IFACE_CONID, dFgtIntMove, FgtIntMove, sizeof(FgtIntMove));
    ResItemGetN(IFACE_CONID, dSubduModu, (U8*)SubduModu, sizeof(SubduModu));
    ResItemGetN(IFACE_CONID, dAtkModulus, (U8*)AtkModulus, sizeof(AtkModulus));
    ResItemGetN(IFACE_CONID, dDfModulus, (U8*)DfModulus, sizeof(DfModulus));
    ResItemGetN(IFACE_CONID, dTerrDfModu, (U8*)TerrDfModu, sizeof(TerrDfModu));
    FgtLoadJNConsts();
}
