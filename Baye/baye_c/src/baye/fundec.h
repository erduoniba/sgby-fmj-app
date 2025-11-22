#ifndef	_FUNDEC_H
#define	_FUNDEC_H
#include "fight.h"
#include "baye/attribute.h"

GamEng U8		GamRecordMan(U8 flag);

PublicFun U8		PlcMovie(U16 speid, U16 index,U8 startfrm,U8 endfrm,U8 keyflag,PT x,PT y);
PublicFun U16		PlcSplMenu(RECT *pRect,U16 pIdx,U8 *buf);
PublicFun U8		PlcExtract(U16 num);
PublicFun U16		PlcArmsMax(PersonID id);
PublicFun U16		PlcArmsMaxP(PersonType* p);
PublicFun void		PlcGraMsgBox(U8 *buf,U8 delay,U8 line);
PublicFun void		PlcMidShowStr(PT x,PT y,U8 *buf);
PublicFun void		PlcStrShowS(RECT *big,RECT *small,U8 *buf);
PublicFun void		PlcNumShow(PT x,PT y,U32 num,U8 dig,U8 *vs);
PublicFun void		PlcRPicShow(U16 id,U16 idx,PT x,PT y,U8 flag);
PublicFun void      PlcRPicShowEx(U16 id, U16 item, U16 idx,PT x,PT y,U8 flag);
Debug void		gamPrintNum(PT x,PT y, U32 num, U8 base,U8 sign);
Debug void		gamTraceP(U32 num);
Debug void		gamTracel(U32 num);
Debug void		gamPause();

Fight U8		GamFight(void);
Fight void		FgtSetFocus(U8 foc);
Fight void		FgtFrashGen(U8 idx,U8 cnt);

FightSub U8		FgtGenPIdx(U8 i);
FightSub U8		FgtMainMenu(void);
FightSub SKILLEF*FgtGetJNPtr(SkillID param);
FightSub U8		FgtGetTerrain(U8 x,U8 y);
FightSub U8		FgtGetGenIdx(U8 x,U8 y);
FightSub U8		FgtDrvCmd(FGTCMD *pcmd);
FightSub void		FgtDealEnd(U8 *genbak);
FightSub void		FgtShowHlp(void);
FightSub void		FgtChgWeather(void);
FightSub U8		FgtCheckIdx(void);

FgtCount void		CountMoveP(U8 i);
FgtCount U16		CountAtkHurt(void);
FgtCount U16		CountPlusSub(U16 *a,U16 v);
FgtCount U16		CountOverAdd(U16 *a,U16 v,U16 up);
FgtCount void		CountSklHurt(SkillID param,U16 *arms,U16 *prov, U8 origin, U8*state);
FgtCount void		FgtCountPath(U8 idx);
FgtCount void		BuiltAtkAttr(U8 idx,U8 pIdx);
FgtCount void		CountProvUse(void);
FgtCount void		CountInitGen(void);
FgtCount void		FgtCountWon(void);



FgtPkAi SkillID		FgtGetJNIdx(U8 idx,RECT *pRect);
FgtPkAi U8		FgtGetMCmd(FGTCMD *pcmd);
FgtPkAi U8		FgtJNChkAim(SkillID param,U8 same,U8 aidx,U8 sidx);
FgtPkAi void		FgtGetCmdRng(U8 type,SkillID sklid,U8 idx);
FgtPkAi void		FgtShowMvRng(void);
FgtPkAi void		FgtShowView(void);

U16 sqrt32(U32 a);

// VIP功能：农业和商业开发倍率设置和获取函数
void SetAgricultureMultiplier(U16 multiplier);
void SetCommerceMultiplier(U16 multiplier);
U16 GetAgricultureMultiplier(void);
U16 GetCommerceMultiplier(void);

#endif	/* _FUNDEC_H */
