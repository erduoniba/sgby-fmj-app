//
//  bind-objects.c
//  baye-ios
//
//  Created by loong on 16/9/17.
//
//

#include "baye/bind-objects.h"
#include "baye/extern.h"
#include "baye/datman.h"
#include "baye/comm.h"
#include "baye/consdef.h"

extern U8 g_engineVersion[32];

void bind_skill_var(ObjectDef* def);
void bind_show_face(ObjectDef* def);

DEC_U8ARR_DEF(2);
DEC_U8ARR_DEF(5);
DEC_U8ARR_DEF(6);
DEC_U8ARR_DEF(8);
DEC_U16ARR_DEF(FGTA_MAX);

DEC_U16ARR_DEF(16);
DEC_U16ARR_DEF(2);

static Value g_var = {0};
static U8 g_errorString[128] = {0};

U16 g_asyncActionID = 0;
U32 g_asyncActionParams[32] = {0};
U8 g_asyncActionStringParam[1024*10] = {0};


void bind_clear_error_string(void) {
    g_errorString[0] = 0;
}

U8* bind_get_error_string(void) {
    return g_errorString;
}

void bind_init(void) {
    static ObjectDef* def = NULL;
    static ValueDef vref;

    typedef struct {
        U8 x;
        U8 y;
    } CityPos;

    CityPos* cityPos;

    cityPos = (CityPos*)ResLoadToCon(IFACE_CONID, dCityPos, g_CBnkPtr);

    if (def == NULL) {
        def = ObjectDef_new();
    }

    vref.size = 0;
    vref.subdef.objDef = def;
    vref.type = ValueTypeObject;

    g_var.def = &vref;
    g_var.offset = 0;

    bind_skill_var(def);
    bind_show_face(def);

    DEFADD_GBKARR(g_engineVersion, sizeof(g_engineVersion));
    DEFADDF(c_ReFlag, U8);
    DEFADDF(c_Sx, U8);
    DEFADDF(c_Sy, U8);
    DEFADDF(c_Ex, U8);
    DEFADDF(c_Ey, U8);
    DEFADDF(g_screenWidth, U16);
    DEFADDF(g_screenHeight, U16);
    DEFADDF(g_scale, U16);
    DEFADDF(g_FlipDrawing, U8);
    DEFADDF(g_paintColor, U8);
    DEFADD_U32ARR(g_paintPalette, 256);
    DEFADDF(g_MenuTouchComfirm, U8);

    DEFADD_GBKARR(g_errorString, sizeof(g_errorString));
    {
        // FOR AYNC CALL
        BIND_U8(&g_asyncActionID);
        DEFADD_U32ARR(g_asyncActionParams, 32);
        DEFADD_GBKARR(g_asyncActionStringParam, sizeof(g_asyncActionStringParam));

        U8* g_asyncActionStringParamArray = g_asyncActionStringParam;
        DEFADD_U8ARR(g_asyncActionStringParamArray, sizeof(g_asyncActionStringParam));

        U16* g_asyncActionU16ParamArray = (U16*)g_asyncActionStringParam;
        DEFADD_U8ARR(g_asyncActionU16ParamArray, sizeof(g_asyncActionStringParam)/sizeof(U16));
    }

    DEFADD_GBKARR(g_errorString, sizeof(g_errorString));

    DEFADD_U8ARR(g_VisScr, MAX_SCR_BUF_LEN);

    DEFADD_U8ARR(g_FightMapData, FIGHT_MAP_DATA_LEN);
    DEFADD_U8ARR(g_FightMap, FIGHT_MAP_BUFFER_LEN);
    DEFADD_U8ARR(g_FightPath, FGT_MRG*FGT_MRG + 25);
    DEFADD_U8ARR(g_FgtAtkRng, MAX_ATT_RANGE + 5);

    DEFADDF(g_TileId, U16);
    DEFADDF(g_EneTmpProv, U16);

    DEFADDF(g_MapWid, U8);
    DEFADDF(g_MapHgt, U8);

    DEFADDF(g_FoucsX, U8);
    DEFADDF(g_FoucsY, U8);

    DEFADDF(g_MapSX, U8);
    DEFADDF(g_MapSY, U8);

    DEFADDF(g_PathSX, U8);
    DEFADDF(g_PathSY, U8);

    DEFADDF(g_PUseSX, U8);
    DEFADDF(g_PUseSY, U8);

    DEFADDF(g_BakUpX, U8);
    DEFADDF(g_BakUpY, U8);

    DEFADDF(g_CityX, U8);
    DEFADDF(g_CityY, U8);

    DEFADDF(g_FgtOver, U8);

    DEFADDF(g_FgtWeather, U8);
    DEFADDF(g_FgtBoutCnt, U16);
    DEFADDF(g_FgtBoutMax, U16);
    DEFADDF(g_MainGenIdx, U8);
    DEFADDF(g_LookEnemy, U8);
    DEFADDF(g_LookMovie, U8);
    DEFADDF(g_MoveSpeed, U8);

    DEFADDF(g_PlayerKing, U16);
    DEFADDF(g_YearDate, U16);

    DEFADDF(g_MonthDate, U8);
    DEFADDF(g_PIdx, U8);
    DEFADDF(g_AutoUpdateMapXY, U8);

    DEFADD_U16ARR(g_PersonsQueue, PERSON_MAX);
    DEFADD_U16ARR(&g_GoodsQueue, GOODS_MAX);


    {
#define _ST PersonType
        static Field _fields[] = {
            _FIELD_RW(OldBelong, U16),
            _FIELD_RW(Belong, U16),
            _FIELD_RW(Level, U8),
            _FIELD_RW(Force, U8),
            _FIELD_RW(IQ, U8),
            _FIELD_RW(Devotion, U8),
            _FIELD_RW(Character, U8),
            _FIELD_RW(Experience, U8),
            _FIELD_RW(Thew, U8),
            _FIELD_RW(ArmsType, U8),
            _FIELD_RW(Arms, U16),
            _FIELD("Tool1", _ST, Equip[0], U16),
            _FIELD("Tool2", _ST, Equip[1], U16),
            _U16ARR_FIELD(Equip, 2),
            _FIELD_RW(Age, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_Persons", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * PERSON_MAX;
        arrfield.value.offset = (U32)g_Persons;

        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST CityType
        static Field _fields[] = {
            _FIELD_RW(State, U8),
            _FIELD_RW(Belong, U16),
            _FIELD_RW(SatrapId, U16),
            _FIELD_RW(FarmingLimit, U16),
            _FIELD_RW(Farming, U16),
            _FIELD_RW(CommerceLimit, U16),
            _FIELD_RW(Commerce, U16),
            _FIELD_RW(PeopleDevotion, U8),
            _FIELD_RW(AvoidCalamity, U8),
            _FIELD_RW(PopulationLimit, U32),
            _FIELD_RW(Population, U32),
            _FIELD_RW(Money, U16),
            _FIELD_RW(Food, U16),
            _FIELD_RW(MothballArms, U16),
            _FIELD_RW(PersonQueue, U16),
            _FIELD_RW(Persons, U16),
            _FIELD_RW(ToolQueue, U16),
            _FIELD_RW(Tools, U16),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_Cities", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * CITY_MAX;
        arrfield.value.offset = (U32)g_Cities;

        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST CityPos
        static Field _fields[] = {
            _FIELD_RW(x, U8),
            _FIELD_RW(y, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_CityPositions", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * CITY_MAX;
        arrfield.value.offset = (U32)cityPos;
        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST GOODS
        static Field _fields[] = {
            _FIELD_RO(useflag, U8),
            _FIELD_RO(changeAttackRange, U8),
            //_FIELD_RO(atRange, Binary),// TODO
            _FIELD_RO(at, U8),
            _FIELD_RO(iq, U8),
            _FIELD_RO(move, U8),
            _FIELD_RO(arm, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_Tools", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * GOODS_MAX;
        arrfield.value.offset = (U32)ResLoadToCon(GOODS_RESID, 1, g_CBnkPtr);

        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST SKILLEF

        static Field _fields[] = {
            _FIELD_RO(aim, U8),
            _FIELD_RO(state, U8),
            _FIELD_RO(power, U16),
            _FIELD_RO(destroy, U16),
            _FIELD_RO(useMp, U8),
            _U8ARR_FIELD(weather, 5),
            _U8ARR_FIELD(eland, 8),
            _U8ARR_FIELD(oland, 8),
            _U8ARR_FIELD(earm, 6),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_Skills", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * 2000; // TODO
        arrfield.value.offset = (U32)ResLoadToCon(SKL_RESID, 1, g_CBnkPtr);
        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST JLATT
        static Field _fields[] = {
            _FIELD_RW(canny, U8),
            _FIELD_RW(ter, U8),
            _FIELD_RW(bile, U8),
            _FIELD_RW(armsType, U8),
            _FIELD_RW(at, U16),
            _FIELD_RW(df, U16),
            _FIELD_RW(generalIndex, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_GenAtt", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * 2;
        arrfield.value.offset = (U32)g_GenAtt;
        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST JLPOS
        static Field _fields[] = {
            _FIELD_RW(x, U8),
            _FIELD_RW(y, U8),
            _FIELD_RW(hp, U16),
            _FIELD_RW(mp, U16),
            _FIELD_RW(move, U8),
            _FIELD_RW(active, U8),
            _FIELD_RW(state, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_GenPos", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * FGTA_MAX;
        arrfield.value.offset = (U32)g_GenPos;
        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST FGTJK
        static Field _fields[] = {
            _FIELD_RW(Mode, U8),
            _FIELD_RW(Way, U8),
            _FIELD_RW(CityIndex, U8),
            _FIELD_RW(MapId, U16),
            _FIELD_RW(MProvender, U16),
            _FIELD_RW(EProvender, U16),
            _U16ARR_FIELD(GenArray, FGTA_MAX),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static Field field = {"g_FgtParam", {.def=&_value_def, .offset=0}};
        field.value.offset = (U32)&g_FgtParam;
        ObjectDef_addField(def, &field);
    }
    {
#define _ST EngineConfig
        static Field _fields[] = {
            _FIELD_RW(enableToolAttackRange, U8),
            _FIELD_RW(fixCityOffset, U8),
            _FIELD_RW(fixThewOverFlow, U8),
            _FIELD_RW(fixFoodOverFlow, U8),
            _FIELD_RW(fixOverFlow16, U8),
            _FIELD_RW(fixConsumeMoney, U8),
            _FIELD_RW(fixFightMoveOutRange, U8),
            _FIELD_RW(enable16bitConsumeMoney, U8),
            _FIELD_RW(enableScript, U8),
            _FIELD_RW(fixAlienateComsumeThew, U8),
            _FIELD_RW(disableSL, U8),
            _FIELD_RW(aiLevelUpSpeed, U8),
            _FIELD_RW(disableAgeGrow, U8),
            _FIELD_RW(enableCustomRatio, U8),
            _FIELD_RW(ratioOfArmsToLevel, U16),
            _FIELD_RW(ratioOfArmsToAge, U8),
            _FIELD_RW(ratioOfArmsToIQ, U8),
            _FIELD_RW(ratioOfArmsToForce, U8),
            _FIELD_RW(ratioOfAttToForce, U8),
            _FIELD_RW(ratioOfAttToIQ, U8),
            _FIELD_RW(ratioOfAttToAge, U8),
            _FIELD_RW(ratioOfDefenceToForce, U8),
            _FIELD_RW(ratioOfDefenceToIQ, U8),
            _FIELD_RW(ratioOfDefenceToAge, U8),
            _FIELD_RW(ratioOfFoodToArmsPerMouth, U16),
            _FIELD_RW(ratioOfFoodToArmsPerDay, U16),
            _FIELD_RW(armsPerDevotion, U8),
            _FIELD_RW(armsPerMoney, U8),
            _FIELD_RW(maxLevel, U8),
            _FIELD_RW(responseNoteOfBettle, U8),
            _FIELD_RW(aiDefenceMode, U8),
            _FIELD_RW(aiAttackMethod, U8),
            _U16ARR_FIELD(mainMenuButtonRects, 16),
            _U16ARR_FIELD(periodMenuButtonRects, 16),
            _U16ARR_FIELD(saveFaceListAnchor, 2),
            _FIELD_RW(citiesCount, U8),
            _FIELD_RW(cityMapWidth, U8),
            _FIELD_RW(cityMapHeight, U8),
            _FIELD_RW(disableExpGrowing, U8),
            _FIELD_RW(hideRightBorder, U8),
            _FIELD_RW(promptCityDisaster, U8),
            _FIELD_RW(disableAllPersonReport, U8),
            _FIELD_RW(showStartMovie, U8),
            _FIELD_RW(compressCustomData, U8),
            _FIELD_RW(checkRedundantOnAddPerson, U8),
            _FIELD_RW(disableFightToDeath, U8),
            _FIELD_RW(useCustomFont, U8),
            _FIELD_RW(useCustomFontEn, U8),
            _FIELD_RW(cacheCustomFont, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static Field field = {"g_engineConfig", {.def=&_value_def, .offset=0}};
        field.value.offset = (U32)&g_engineConfig;
        ObjectDef_addField(def, &field);
    }
    {
#define _ST Theme
        static Field _fields[] = {
            _FIELD_RW(landMapColor, U8),
            _FIELD_RW(ownedCityColor, U8),
            _FIELD_RW(emptyCityColor, U8),
            _FIELD_RW(otherCityColor, U8),
            _FIELD_RW(landCursorColor, U8),
            _FIELD_RW(battleNoteColor, U8),
            _FIELD_RW(kingHeadColor, U8),
            _FIELD_RW(personHeadColor, U8),
            _FIELD_RW(fightMoveRangeColor, U8),
            _FIELD_RW(fightMapColor, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static Field field = {"theme", {.def=&_value_def, .offset=0}};
        field.value.offset = (U32)&g_engineConfig.theme;
        ObjectDef_addField(def, &field);
    }
    {
        extern Rect g_cityCursorRange;
#define _ST Rect
        static Field _fields[] = {
            _FIELD_RW(top, U16),
            _FIELD_RW(left, U16),
            _FIELD_RW(right, U16),
            _FIELD_RW(bottom, U16),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static Field field = {"g_cityCursorRange", {.def=&_value_def, .offset=0}};
        field.value.offset = (U32)&g_cityCursorRange;
        ObjectDef_addField(def, &field);
    }
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
#undef _ST
        static ValueDef arrdef = { .type=ValueTypeArray, .size=0, .subdef.arrDef=&_value_def };
        static Field arrfield = {"g_OrderQueue", {.def=&arrdef, .offset=0}};
        arrdef.size = _value_def.size * ORDER_MAX;
        arrfield.value.offset = (U32)ORDERQUEUE;
        ObjectDef_addField(def, &arrfield);
    }
    {
#define _ST CitySetType
        static Field _fields[] = {
            _FIELD_RW(x, U8),
            _FIELD_RW(y, U8),
            _FIELD_RW(setx, U8),
            _FIELD_RW(sety, U8),
        };

        static ObjectDef _obj_def = {
            AL(_fields), 0, sizeof(_ST), _fields
        };

        static ValueDef _value_def = {
            .type = ValueTypeObject,
            .size = sizeof(_ST),
            .subdef.objDef = &_obj_def,
        };
#undef _ST
        static Field field = {"g_CityPos", {.def=&_value_def, .offset=0}};
        field.value.offset = (U32)&g_CityPos;
        ObjectDef_addField(def, &field);
    }

    DEFADD_U8ARR(FIGHTERS_IDX, 30);
    DEFADD_U8ARR(FIGHTERS, 600);
}

Value* bind_get_global(void) {
    if (g_var.def == NULL) {
        bind_init();
    }
    return &g_var;
}
