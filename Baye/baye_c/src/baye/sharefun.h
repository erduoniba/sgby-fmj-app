#ifndef	SHAREFUN_H
#define	SHAREFUN_H

#include "sources.h"

CITYEDIT_C U8 AddPerson(U8 city,PersonID person);
CITYEDIT_C U8 DelPerson(U8 city,PersonID person);
CITYEDIT_C ToolID AddGoods(U8 city,ToolID goods);
CITYEDIT_C ToolID AddGoodsEx(U8 city, ToolID goods, U8 found);
CITYEDIT_C U8 DelGoods(U8 city,ToolID goods);
CITYEDIT_C U8 ClearOrderQueue(void);
CITYEDIT_C U8 SearchRoad(U8 sc,U8 xs,U8 ys,U8 ob,U8 xo,U8 yo);
CITYEDIT_C U8 AttackCityRoad(U8 sc,U8 xs,U8 ys,U8 ob,U8 xo,U8 yo);
CITYEDIT_C U8 GetKingCitys(PersonID king,U8 *cqueue);
CITYEDIT_C U8 GetPersonCity(PersonID person);
CITYEDIT_C U32 GetCityPersons(U8 city, PersonID *pqueue);
CITYEDIT_C U32 GetCityOutPersons(U8 city,PersonID *pqueue);
CITYEDIT_C U32 GetCityCaptives(U8 city,PersonID *pqueue);
CITYEDIT_C U32 GetEnemyPersons(PersonID king,PersonID *pqueue);
CITYEDIT_C U32 GetEnemySatraps(PersonID king,PersonID *squeue);
CITYEDIT_C U32 GetEnemyKing(PersonID king,PersonID *kqueue);
CITYEDIT_C U8 GetCitySet(CitySetType *pos);
CITYEDIT_C U8 ShowCityMap(CitySetType *pos);
CITYEDIT_C void ShowMapClear(void);
CITYEDIT_C U8 GetRoundEnemyCity(U8 city,U8 *cqueue);
CITYEDIT_C U32 GetKingPersons(PersonID king,PersonID *pqueue);
CITYEDIT_C U16 GetFood(U16 min,U16 max);
CITYEDIT_C U16 GetMoney(U16 min,U16 max);
CITYEDIT_C U16 GetArmy(U16 min,U16 max);
CITYEDIT_C U8 GetRoundSelfCity(U8 city,U8 *cqueue);

/*CITYCMD_C U8 AppointDrv(OrderType *Order);*/
CITYCMD_C U8 AssartDrv(OrderType *Order);
CITYCMD_C U8 NopDrv(OrderType *Order);
CITYCMD_C U8 AccractbusinessDrv(OrderType *Order);
CITYCMD_C U8 SearchDrv(OrderType *Order);
CITYCMD_C U8 FatherDrv(OrderType *Order);
CITYCMD_C U8 InspectionDrv(OrderType *Order);
CITYCMD_C U8 SurrenderDrv(OrderType *Order);
CITYCMD_C U8 BanishDrv(OrderType *Order);
CITYCMD_C U8 ExchangeDrv(OrderType *Order);
CITYCMD_C U8 TransportationDrv(OrderType *Order);
CITYCMD_C U8 MoveDrv(OrderType *Order);
CITYCMD_C U8 AlienateDrv(OrderType *Order);
CITYCMD_C U8 CanvassDrv(OrderType *Order);
CITYCMD_C U8 CounterespiongeDrv(OrderType *Order);
CITYCMD_C U8 RealienateDrv(OrderType *Order);
CITYCMD_C U8 InduceDrv(OrderType *Order);
/*CITYCMD_C U8 TributeDrv(OrderType *Order);*/
CITYCMD_C U8 ReconnoitreDrv(OrderType *Order);
CITYCMD_C U8 ConscriptionDrv(OrderType *Order);
CITYCMD_C U8 DepredateDrv(OrderType *Order);
CITYCMD_C U8 OrderExec(OrderType *Order);
CITYCMD_C void PolicyExec(void);

CITYCMDB_C U8 OrderMenu(void);
CITYCMDB_C U8 MainOrderMenu(U16 ind);
CITYCMDB_C U8 InteriorOrderMenu(void);
CITYCMDB_C U8 DiplomatismOrderMenu(void);
CITYCMDB_C U8 ArmamentOrderMenu(void);
CITYCMDB_C U8 CityCommon(U8 city,U8 cmd);
CITYCMDB_C U8 AppointMake(U8 city);
CITYCMDB_C U8 AssartMake(U8 city);
CITYCMDB_C U8 AccractbusinessMake(U8 city);
CITYCMDB_C U8 SearchMake(U8 city);
CITYCMDB_C U8 FatherMake(U8 city);
CITYCMDB_C U8 InspectionMake(U8 city);
CITYCMDB_C U8 SurrenderMake(U8 city);
CITYCMDB_C U8 KillMake(U8 city);
CITYCMDB_C U8 BanishMake(U8 city);
CITYCMDB_C void ShowDMsg(U8 city,PersonID person,U16 upval,U8 flag);
CITYCMDB_C U8 LargessMake(U8 city);


CITYCMDC_C U8 ExchangeMake(U8 city);
CITYCMDC_C U8 TreatMake(U8 city);
CITYCMDC_C U8 TransportationMake(U8 city);
CITYCMDC_C U8 MoveMake(U8 city);
CITYCMDC_C U8 AlienateMake(U8 city);
CITYCMDC_C U8 CanvassMake(U8 city);
CITYCMDC_C U8 CounterespiongeMake(U8 city);
CITYCMDC_C U8 RealienateMake(U8 city);
CITYCMDC_C U8 InduceMake(U8 city);
CITYCMDC_C U8 TributeMake(U8 city);
CITYCMDC_C U8 ReconnoitreMake(U8 city);
CITYCMDC_C U8 ConscriptionMake(U8 city);
CITYCMDC_C U8 DistributeMake(U8 city);

CITYCMDD_C U8 BattleMake(U8 city);
CITYCMDD_C U8 BattleDrv(OrderType *Order);
CITYCMDD_C U8 FightResultDeal(U8 city,U8 result);
CITYCMDD_C void HoldCaptive(PersonID person,U8 city);
CITYCMDD_C U8 LostEscape(PersonID person,U8 city);
CITYCMDD_C PersonID BeOccupied(PersonID person,U8 city);
CITYCMDD_C void KingOverDeal(PersonID king);
CITYCMDD_C void KingDeadNote(PersonID king);
CITYCMDD_C void NewKingNote(PersonID king);
CITYCMDD_C void WeightOverNote(PersonID king);
CITYCMDD_C void WonPersonNoet(PersonID person);
CITYCMDD_C PersonID TheLoserDeal(U8 city,PersonID *lqueue);
CITYCMDD_C U8 AddOrderHead(OrderType *Order);
CITYCMDD_C U8 AddOrderEnd(OrderType *Order);
CITYCMDD_C U8 AddFightOrder(OrderType *Order,PersonID *Fighters);
CITYCMDD_C U8 DelOrder(OrderType *Order);
CITYCMDD_C void BeNewKingNote(PersonID king);

CITYCMDE_C U8 ConfiscateMake(U8 city);
CITYCMDE_C U8 DepredateMake(U8 city);
CITYCMDE_C void ShowAttackNote(PersonID ap,U8 city);
CITYCMDE_C void ShowFightNote(PersonID ap,PersonID bc);
CITYCMDE_C void ShowFightWinNote(PersonID wp);
CITYCMDE_C void ShowFightLossNote(void);
CITYCMDE_C void ShowFightNoteFace(U8 idx);
CITYCMDE_C U8 IsMoney(U8 city,U8 order);
CITYCMDE_C void OrderConsumeMoney(U8 city,U8 order);
CITYCMDE_C U8 IsManual(PersonID person,U8 order);
CITYCMDE_C void OrderConsumeThew(PersonID person,U8 order);
CITYCMDE_C U8 GetWeekCity(U8 count,U8 *cqueue);

#define GOODS_PROP_COUNT 5
#define PERSON_PROP_COUNT 13

SHOWFACE_C void ShowGoodsPro(ToolID goods,U8 pro,U8 x,U8 y,U8 wid);
SHOWFACE_C void GetGoodsProStr(ToolID goods,U8 pro,U8 *str);
SHOWFACE_C U8 ShowGoodsProStr(U8 pro,U8 x,U8 y,U8 wid);
SHOWFACE_C ToolID ShowGoodsControl(ToolID *goods,ToolID gcount, ToolID init, U8 x0,U8 y0,U8 x1,U8 y1);
SHOWFACE_C void GetPersonName(PersonID person,U8 *str);
SHOWFACE_C void ShowPersonPro(PersonID person,U8 pro,U8 x,U8 y,U8 wid);
SHOWFACE_C void GetPersonProStr(PersonID person,U8 pro,U8 *str);
SHOWFACE_C U8 ShowPersonProStr(U8 pro,U8 x,U8 y,U8 wid);
SHOWFACE_C PersonID ShowPersonControl(PersonID *person,U32 pcount,PersonID initSelected,U8 x0,U8 y0,U8 x1,U8 y1);
SHOWFACE_C void ShowDateTime(void);
SHOWFACE_C void GetGoodsName(ToolID goods,U8 *str);
/*SHOWFACE_C U8 MenuComm(MenuType *menu);*/
SHOWFACE_C void GetCityName(U8 city,U8 *str);
SHOWFACE_C void GetCityProStr(U8 city,U8 pro,U8 *str);
SHOWFACE_C void GetCityState(U8 city,U8 *str);
SHOWFACE_C U8 ShowCityPro(U8 city);

TACTIC_C U8 PlayerTactic(void);
TACTIC_C void ComputerTactic(void);
TACTIC_C void ComputerTacticInterior(U8 city);
TACTIC_C void ComputerTacticDiplomatism(U8 city);
TACTIC_C void ComputerTacticHarmonize(U8 city);
TACTIC_C void ComputerTacticArmament(U8 city);
TACTIC_C void ConditionUpdate(void);
TACTIC_C U8 GameDevDrv(void);
TACTIC_C U8 IsWin(void);
TACTIC_C U8 IsLoss(void);
TACTIC_C void SetCitySatrap(void);
TACTIC_C U32 GetAllKings(PersonID *kings);
TACTIC_C void LoadPeriod(U8 period);
TACTIC_C U8 FunctionMenu(void);
TACTIC_C U32 NumOperate(U32 min,U32 max, U32 donum);
TACTIC_C void ShowTacticNote(void);


PACCOUNT_C void InitItem(U8 x0,U8 y0,U8 x1,U8 y1,PosItemType *positem);
PACCOUNT_C U8 AddItem(U8 wid,U8 hig,PosItemType *positem,U8 *x,U8 *y);
PACCOUNT_C U8 NextLineItem(PosItemType *positem);

 
INFDEAL_C U8 CitiesUpDataDate(void);
INFDEAL_C U8 RandEvents(void);
INFDEAL_C U8 EventStateDeal(void);
INFDEAL_C void RevenueMoney(void);
INFDEAL_C void HarvestryFood(void);
INFDEAL_C U8 PersonUpDatadate(void);
INFDEAL_C U8 GoodsUpDatadate(void);
INFDEAL_C U8 AddGoodsPerson(ToolID goods,PersonID person);
INFDEAL_C U8 DelGoodsPerson(ToolID goods,PersonID person);
INFDEAL_C void ShowConstStrMsg(U8 idx);
/*INFDEAL_C void ShowAttackMsg(U8 fs,U8 co);*/
INFDEAL_C void ShowPersonHead(U8 x,U8 y, PersonID id);
INFDEAL_C void ShowGReport(PersonID person,U8 *str);
FAR void ShowDialog(PersonID commander, PersonID reporter,U8 *str);
FAR void ShowDialogRandom(PersonID commander, PersonID reporter, U8 **str, U8 count);
INFDEAL_C void SetGoods(U8 city, ToolID goods);
INFDEAL_C void SetGoodsByIndex(ToolID goods);
INFDEAL_C ToolID GetCityPGoods(U8 city,ToolID *gqueue);
INFDEAL_C ToolID GetCityDispGoods(U8 city,ToolID *gqueue);
INFDEAL_C U32 GetPersonsCount(PersonID king);
INFDEAL_C U8 GetDirect(U8 sc,U8 oc);
INFDEAL_C void ReportCalamity(U8 city);
PersonID GamChoosePerson(PersonID* pqptr, U32 pcount, PersonID selected);
U16 GamChoose(U16 x, U16 y, U16 w, U16 h, U16 pIdx, U8* buf);

U8 GetArmType(PersonType* p);
void LevelUp(PersonType*p);
U32 OrderNeedMoney(U8 order);

U8 FgtGetGenTer(U8 idx);
U8 GamChooseCity(void);

void pushPaintColor(U8 color);
void popPaintColor();

#endif
