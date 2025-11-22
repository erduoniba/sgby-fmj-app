//
//  script.c
//  baye-ios
//
//  Created by loong on 16/9/17.
//
//

#include "baye/script.h"
#include "baye/bind-objects.h"
#include "baye/consdef.h"
#include "baye/datman.h"
#include "baye/extern.h"
#include "baye/sharefun.h"
#include "baye/comm.h"
#include "baye/enghead.h"
#include <emscripten.h>

void script_init(void)
{
    EM_ASM({
        if (window.baye == undefined) {
            window.baye = {};
        }
        if (window.baye.hooks == undefined) {
            window.baye.hooks = {};
        }

        window.baye.data = baye_bridge_value(_bayeGetGlobal());
    });

    U8* script = ResLoadStringWithId(ENGINE_SCRIPT);
    if (script) {
        EM_ASM_INT({
            if (window.baye.preScriptInit) {
                window.baye.preScriptInit();
            }
            var script = UTF8ToString($0);
            eval(script);
        }, script);
    }
}

int call_hook_s(const char* name, Value* context)
{
    return EM_ASM_INT({
        var name = UTF8ToString($0);
        var rv = 0;

        if (window.baye == undefined || window.baye.hooks == undefined || window.baye.hooks[name] == undefined) {
            rv = -1;
        } else {
            var cContext = $1;
            if (cContext != 0) {
                var jsContext = baye_bridge_value(cContext);
                rv = baye.callHook(name, jsContext);
            } else {
                rv = baye.callHook(name, undefined);
            }
        }
        return rv;
    }, name, context);
}

static void js_callback(int *rv) {
    *rv = EM_ASM_INT({
        return baye.callCallback($0);
    }, *rv);
}

int call_hook_a(const char* name, Value* context)
{
    int rv = call_hook_s(name, context);

    while (g_asyncActionID) {
        U16 action = g_asyncActionID;
        g_asyncActionID = 0;

        switch (action) {
            case 1: // alert
                GamMsgBox(g_asyncActionStringParam, g_asyncActionParams[0]);
                js_callback(&rv);
                break;
            case 2: // say
                ShowGReport(PID(g_asyncActionParams[0]), g_asyncActionStringParam);
                js_callback(&rv);
                break;
            case 3: // menu
            {
                U16 x = g_asyncActionParams[0];
                U16 y = g_asyncActionParams[1];
                U16 w = g_asyncActionParams[2];
                U16 h = g_asyncActionParams[3];
                U16 init = g_asyncActionParams[4];
                g_asyncActionParams[0] = GamChoose(x, y, w, h, init, g_asyncActionStringParam);
                js_callback(&rv);
                break;
            }
            case 4: // Choose person
            {
                U16 count = g_asyncActionParams[0];
                U16 init = g_asyncActionParams[1];
                g_asyncActionParams[0] = GamChoosePerson((U16*)g_asyncActionStringParam, count, init);
                js_callback(&rv);
                break;
            }
            case 5: // Choose tool
            {
                U16 count = g_asyncActionParams[0];
                U16 init = g_asyncActionParams[1];
                g_asyncActionParams[0] = ShowGoodsControl((U16*)g_asyncActionStringParam,
                                                          count, init, WK_SX + 4,WK_SY + 2,WK_EX - 4,WK_EY - 2);
                js_callback(&rv);
                break;
            }
            case 6: // Choose city
                g_asyncActionParams[0] = GamChooseCity();
                js_callback(&rv);
                break;
            case 7: // delay
            {
                U16 count = g_asyncActionParams[0];
                U8 flag = g_asyncActionParams[1];
                g_asyncActionParams[0] = GamDelay(count, flag);
                js_callback(&rv);
                break;
            }
            case 8: // spe
            {
                U16 speid = g_asyncActionParams[0];
                U16 index = g_asyncActionParams[1];
                U16 x = g_asyncActionParams[2];
                U16 y = g_asyncActionParams[3];
                U16 flag = g_asyncActionParams[4];
                g_asyncActionParams[0] = PlcMovie(speid, index, 0, -1, flag, x, y);
                js_callback(&rv);
                break;
            }
            case 9: // get num
            {
                U32 max = g_asyncActionParams[1];
                U32 min = g_asyncActionParams[0];
                U32 init = g_asyncActionParams[2];
                g_asyncActionParams[0] = NumOperate(min, max, init);
                js_callback(&rv);
                break;
            }
            case 10: // enter battle
            {
//                FAR U8 GamFight(void);
                GamFight();
                js_callback(&rv);
                break;
            }
                
            case 11: // makeBattle
            {
                U8 city = g_asyncActionParams[0];
                BattleMake(city);
                js_callback(&rv);
                break;
            }
                
            case 12: // makeCommand
            {
                U8 city = g_asyncActionParams[0];
                U8 cmd = g_asyncActionParams[1];
                CityCommon(city, cmd);
                js_callback(&rv);
                break;
            }
            case 13: // sysMessage
            {
                GMType msg;
                GamGetMsg(&msg);
                g_asyncActionParams[0] = msg.type;
                g_asyncActionParams[1] = msg.param;
                g_asyncActionParams[2] = msg.param2.i32;
                g_asyncActionParams[3] = msg.param2.i16.p0;
                g_asyncActionParams[4] = msg.param2.i16.p1;
                js_callback(&rv);
                break;
            }
            default:
                break;
        }
    }
    return rv;
}

int has_hook(const char* name) {
    return EM_ASM_INT({
        var name = UTF8ToString($0);
        if (window.baye && window.baye.hooks && window.baye.hooks[name]) {
            return 1;
        } else {
            return 0;
        }
    }, name);
}

int call_script(const char*script) {
    return EM_ASM_INT({
        var scr = UTF8ToString($0);
        eval(scr);
    }, script);
}
