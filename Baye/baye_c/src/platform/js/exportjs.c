//
//  exportjs.c
//  baye-ios
//
//  Created by loong on 16/10/29.
//
//

#include <stdio.h>

#include	"baye/comm.h"
#include	"baye/bind-objects.h"
#include	"baye/data-bind.h"
#include	"baye/script.h"
#include	"baye/fundef.h"
#include	"baye/enghead.h"
#include	"baye/attribute.h"
#include    "touch.h"

void gam_setcustomdata(U8*data);
U8* gam_getcustomdata();
void baye_init_for_js(void);
U8 GamVarInit(void);

EMSCRIPTEN_KEEPALIVE
void bayeSendKey(int key)
{
    MsgType msg;
    msg.type = VM_CHAR_FUN;
    msg.param = key;
    GuiPushMsg(&msg);
}

EMSCRIPTEN_KEEPALIVE
void bayeSendTouchEvent(int event, int x, int y)
{
    touchSendTouchEvent((U16)event, (I16)x, (I16)y);
}

EMSCRIPTEN_KEEPALIVE
void bayeSetLcdSize(int width, int height)
{
    g_screenWidth = width;
    g_screenHeight = height;
    SysAdjustLCDBuffer(width, height);
    
    // 根据屏幕尺寸动态调整菜单按钮坐标
    // 原始分辨率是 160x96，现在根据实际尺寸计算缩放比例
    float scaleX = (float)width / 160.0f;
    float scaleY = (float)height / 96.0f;
    
    // 原始坐标（基于160x96分辨率）
    static const Rect originalMainMenuRects[4] = {
        {6, 45, 73, 63}, //新君登基
        {83, 45, 151, 63}, //重返沙场
        {6, 70, 73, 88}, //制作群组
        {83, 71, 151, 88}, //解甲归田
    };
    
    static const Rect originalPeriodMenuRects[4] = {
        {0, 24, 78, 56}, //董卓弄权
        {0, 62, 78, 93}, //曹操崛起
        {81, 24, 158, 56}, //赤壁之战
        {81, 62, 158, 93}, //三国鼎立
    };
    
    // 动态调整主菜单按钮坐标
    int i;
    for (i = 0; i < 4; i++) {
        g_engineConfig.mainMenuButtonRects[i].left = (I16)(originalMainMenuRects[i].left * scaleX);
        g_engineConfig.mainMenuButtonRects[i].top = (I16)(originalMainMenuRects[i].top * scaleY);
        g_engineConfig.mainMenuButtonRects[i].right = (I16)(originalMainMenuRects[i].right * scaleX);
        g_engineConfig.mainMenuButtonRects[i].bottom = (I16)(originalMainMenuRects[i].bottom * scaleY);
    }
    
    // 动态调整时期选择菜单按钮坐标
    for (i = 0; i < 4; i++) {
        g_engineConfig.periodMenuButtonRects[i].left = (I16)(originalPeriodMenuRects[i].left * scaleX);
        g_engineConfig.periodMenuButtonRects[i].top = (I16)(originalPeriodMenuRects[i].top * scaleY);
        g_engineConfig.periodMenuButtonRects[i].right = (I16)(originalPeriodMenuRects[i].right * scaleX);
        g_engineConfig.periodMenuButtonRects[i].bottom = (I16)(originalPeriodMenuRects[i].bottom * scaleY);
    }
    
    // 同样调整存档界面的坐标
    g_engineConfig.saveFaceListAnchor.x = (I16)(31 * scaleX);
    g_engineConfig.saveFaceListAnchor.y = (I16)(33 * scaleY);
    
    printf("屏幕尺寸调整为 %dx%d，缩放比例: %.2fx%.2f\n", width, height, scaleX, scaleY);
}

EMSCRIPTEN_KEEPALIVE
void bayeSetDebug(int debug)
{
    GamSetDebug(debug);
}

EMSCRIPTEN_KEEPALIVE
int bayeGetCurrentPeriod()
{
    return g_PIdx;
}

EMSCRIPTEN_KEEPALIVE
int bayeCityAddGoods(U8 cityIndex, U16 goodsIndex)
{
    return AddGoodsEx(cityIndex, goodsIndex, 1);
}

EMSCRIPTEN_KEEPALIVE
int bayeCityDelGoods(U8 cityIndex, U16 goodsIndex)
{
    return DelGoods(cityIndex, goodsIndex);
}

EMSCRIPTEN_KEEPALIVE
ValueType Value_get_type(Value*value) {
    return value->def->type;
}

EMSCRIPTEN_KEEPALIVE
ValueDef* Value_get_def(Value*value) {
    return value->def;
}

EMSCRIPTEN_KEEPALIVE
void* Value_get_addr(Value*value) {
    return (void*)value->offset;
}

EMSCRIPTEN_KEEPALIVE
U8 Value_get_u8_value(Value*value) {
    return *(U8*)value->offset;
}

EMSCRIPTEN_KEEPALIVE
void Value_set_u8_value(Value*value, U8 cvalue) {
    *(U8*)value->offset = cvalue;
}

EMSCRIPTEN_KEEPALIVE
U16 Value_get_u16_value(Value*value) {
    return *(U16*)value->offset;
}

EMSCRIPTEN_KEEPALIVE
void Value_set_u16_value(Value*value, U16 cvalue) {
    *(U16*)value->offset = cvalue;
}

EMSCRIPTEN_KEEPALIVE
U32 Value_get_u32_value(Value*value) {
    return *(U32*)value->offset;
}

EMSCRIPTEN_KEEPALIVE
void Value_set_u32_value(Value*value, U32 cvalue) {
    *(U32*)value->offset = cvalue;
}

EMSCRIPTEN_KEEPALIVE
U32 Object_get_field_count(Value*value) {
    return value->def->subdef.objDef->count;
}

EMSCRIPTEN_KEEPALIVE
Field* Object_get_field_by_index(Value*obj, U32 ind) {
    return &obj->def->subdef.objDef->fields[ind];
}

EMSCRIPTEN_KEEPALIVE
const char* Field_get_name(Field*field) {
    return field->name;
}

EMSCRIPTEN_KEEPALIVE
Value* Field_get_value(Field*field) {
    return &field->value;
}

EMSCRIPTEN_KEEPALIVE
ValueType Field_get_type(Field*field) {
    return field->value.def->type;
}

EMSCRIPTEN_KEEPALIVE
ValueType ValueDef_get_type(ValueDef*def) {
    return def->type;
}


EMSCRIPTEN_KEEPALIVE
U32 baye_get_u8_value(U8*value) {
    return *(U8*)value;
}

EMSCRIPTEN_KEEPALIVE
void baye_set_u8_value(U8*value, U8 cvalue) {
    *(U8*)value = cvalue;
}

EMSCRIPTEN_KEEPALIVE
U32 baye_get_u16_value(U16*value) {
    return *(U16*)value;
}

EMSCRIPTEN_KEEPALIVE
void baye_set_u16_value(U16*value, U16 cvalue) {
    *(U16*)value = cvalue;
}

EMSCRIPTEN_KEEPALIVE
U32 baye_get_u32_value(U32*value) {
    return *(U32*)value;
}

EMSCRIPTEN_KEEPALIVE
void baye_set_u32_value(U32*value, U32 cvalue) {
    *(U32*)value = cvalue;
}


EMSCRIPTEN_KEEPALIVE
U32 ValueDef_get_field_count(ValueDef*def) {
    return def->subdef.objDef->count;
}


EMSCRIPTEN_KEEPALIVE
Field *ValueDef_get_field_by_index(ValueDef*def, U32 ind) {
    return &def->subdef.objDef->fields[ind];
}

EMSCRIPTEN_KEEPALIVE
U32 ValueDef_get_array_length(ValueDef*def) {
    return def->size / def->subdef.arrDef->size;
}

EMSCRIPTEN_KEEPALIVE
U32 ValueDef_get_size(ValueDef*def) {
    return def->size;
}

EMSCRIPTEN_KEEPALIVE
ValueDef* ValueDef_get_array_subdef(ValueDef*def) {
    return def->subdef.arrDef;
}

EMSCRIPTEN_KEEPALIVE
FAR const U8* bayeGetPersonName(U32 personIndex)
{
    static U8 name[32] = {0};
    name[0] = 0;
    GetPersonName(PID(personIndex), name);
    return name;
}

EMSCRIPTEN_KEEPALIVE
FAR const U8* bayeGetToolName(ToolID toolIndex)
{
    static U8 name[32] = {0};
    name[0] = 0;
    GetGoodsName(toolIndex, name);
    return name;
}

EMSCRIPTEN_KEEPALIVE
FAR const U8* bayeGetSkillName(U32 skillIndex)
{
    static U8 name[32] = {0};
    name[0] = 0;
    ResLoadToMem(SKL_NAMID, skillIndex+1, name);
    return name;
}

EMSCRIPTEN_KEEPALIVE
FAR const U8* bayeGetCityName(U32 cityIndex)
{
    static U8 name[32] = {0};
    name[0] = 0;
    GetCityName(cityIndex, name);
    return name;
}

EMSCRIPTEN_KEEPALIVE
FAR U32 bayeStrLen(const U8* s)
{
    return (U32)strlen((const char*)s);
}

Value* bind_get_global(void);

EMSCRIPTEN_KEEPALIVE
Value* bayeGetGlobal(void) {
    return bind_get_global();
}

EMSCRIPTEN_KEEPALIVE
U8* bayeGetCustomData(void) {
    return gam_getcustomdata();
}

EMSCRIPTEN_KEEPALIVE
void bayeSetCustomData(U8*data) {
    return gam_setcustomdata(data);
}

EMSCRIPTEN_KEEPALIVE
void bayeScriptInit(void) {
    script_init();
}

EMSCRIPTEN_KEEPALIVE
U32 bayeRand(void) {
    return gam_rand();
}

EMSCRIPTEN_KEEPALIVE
void bayeSRand(U32 seed) {
    gam_srand(seed);
}

EMSCRIPTEN_KEEPALIVE
U32 bayeGetSeed(void) {
    return gam_seed();
}

EMSCRIPTEN_KEEPALIVE
void bayeLevelUp(U8 person) {
    LevelUp(&g_Persons[person]);
}

EMSCRIPTEN_KEEPALIVE
U32 bayeOrderNeedMoney(U8 order) {
    return OrderNeedMoney(order);
}

EMSCRIPTEN_KEEPALIVE
void bayeOrderComsumeMoney(U8 city, U8 order) {
    OrderConsumeMoney(city, order);
}


EMSCRIPTEN_KEEPALIVE
U32 bayeFgtGetGenTer(U8 index) {
    return FgtGetGenTer(index);
}

EMSCRIPTEN_KEEPALIVE
void bayePutPersonInCity(U8 city, U32 person) {
    AddPerson(city, PID(person));
}

EMSCRIPTEN_KEEPALIVE
void bayePutToolInCity(U8 city, U16 tool, U8 hide) {
    AddGoodsEx(city, tool, !hide);
}


EMSCRIPTEN_KEEPALIVE
void bayeDeletePersonInCity(U8 city, U32 person) {
    DelPerson(city, PID(person));
}

EMSCRIPTEN_KEEPALIVE
void bayeDeleteToolInCity(U8 city, U16 tool) {
    DelGoods(city, tool);
}

EMSCRIPTEN_KEEPALIVE
U8 bayeGetArmType(U8 pIndex) {
    return GetArmType(&g_Persons[pIndex]);
}

EMSCRIPTEN_KEEPALIVE
U8* bayeGetGBKBuffer(void) {
    return g_asyncActionStringParam;
}

EMSCRIPTEN_KEEPALIVE
void bayeLcdDrawImage(U16 resid, U16 item, U16 index, I32 x, I32 y, U8 scr) {
    return PlcRPicShowEx(resid, item, index+1, x, y, scr);
}

EMSCRIPTEN_KEEPALIVE
U32 bayeLcdDrawText(U8*text, I32 x, I32 y, U8 scr) {
    gam_usescr(scr);
    return GamStrShow(x, y, text);
}

/* 清除屏幕矩形 */
EMSCRIPTEN_KEEPALIVE
void bayeLcdClearRect(I32 left, I32 top, I32 right, I32 bottom, U8 scr) {
    gam_usescr(scr);
    SysLcdPartClear(left, top, right, bottom);
}

/* 反显屏幕 */
EMSCRIPTEN_KEEPALIVE
void bayeLcdRevertRect(I32 left, I32 top, I32 right, I32 bottom, U8 scr) {
    gam_usescr(scr);
    SysLcdReverse(left, top, right, bottom);
}

/* 画点函数 */
EMSCRIPTEN_KEEPALIVE
void bayeLcdDot(I32 x, I32 y, U8 color, U8 scr) {
    gam_usescr(scr);
    SysPutPixel(x, y, color);
}

/* 显示直线 */
EMSCRIPTEN_KEEPALIVE
void bayeLcdDrawLine(I32 sx, I32 sy, I32 ex, I32 ey, U8 color, U8 scr) {
    gam_usescr(scr);
    if (color) {
        SysLine(sx, sy, ex, ey);
    } else {
        // TODO:
        // gam_linec(sx, sy, ex, ey);
        printf("clear line not implemented\n");
    }
}

/* 显示矩形 */
EMSCRIPTEN_KEEPALIVE
void bayeLcdDrawRect(I32 sx, I32 sy, I32 ex, I32 ey, U8 color, U8 scr) {
    gam_usescr(scr);
    if (color) {
        SysRect(sx, sy, ex, ey);
    } else {
        SysRectClear(sx, sy, ex, ey);
    }
}

/* 初始化全局环境 */
EMSCRIPTEN_KEEPALIVE
void bayeGameEnvInit(void) {
    baye_init_for_js();
    GamVarInit();
}


EMSCRIPTEN_KEEPALIVE
void bayeSaveScreen(void) {
    SysSaveScreen();
}

EMSCRIPTEN_KEEPALIVE
void bayeRestoreScreen(void) {
    SysRestoreScreen();
}

EMSCRIPTEN_KEEPALIVE
U16 bayeGetPersonCount(void) {
    return GamGetPersonCount();
}

EMSCRIPTEN_KEEPALIVE
void* bayeAlloc(U32 sz) {
    return gam_malloc(sz);
}

EMSCRIPTEN_KEEPALIVE
void bayePrintGC() {
    gam_print_gc();
}

EMSCRIPTEN_KEEPALIVE
void bayeGCCheckAll() {
    gam_gc_check_all();
}

EMSCRIPTEN_KEEPALIVE
void bayeLoadPeriod(U8 period) {
    LoadPeriod(period);
}

EMSCRIPTEN_KEEPALIVE
U8 bayeSetFont(U16 font) {
    return GamSetFont(font);
}

EMSCRIPTEN_KEEPALIVE
U8 bayeSetFontEn(U16 font) {
    return GamSetFontEn(font);
}

EMSCRIPTEN_KEEPALIVE
void bayeClearFontCache() {
    GamClearFontCache();
}

/* 游戏速度控制函数 */
#include "../../baye/speed.h"

EMSCRIPTEN_KEEPALIVE
float bayeGetSpeed(void) {
    return bayeGetGameSpeed();
}

EMSCRIPTEN_KEEPALIVE
void bayeSetSpeed(float speed) {
    bayeSetGameSpeed(speed);
    
    /* 输出日志到 JavaScript 控制台 */
    EM_ASM({
        console.log('C: 游戏速度已设置为: ' + $0 + '倍');
    }, speed);
}
