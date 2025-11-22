//
//  bind-objects.h
//  baye-ios
//
//  Created by loong on 16/9/17.
//
//

#ifndef bind_objects_h
#define bind_objects_h

#include <stdio.h>
#include "baye/script.h"
#include "baye/fight.h"
#include "baye/attribute.h"

void bind_init(void);

void bind_clear_error_string(void);

U8* bind_get_error_string(void);

extern U16 g_asyncActionID;
extern U32 g_asyncActionParams[32];
extern U8 g_asyncActionStringParam[1024*10];

#ifndef offsetof
#define offsetof __builtin_offsetof
#endif

#define AL(a) sizeof(a)/sizeof(*a)


// static
#ifdef __EMSCRIPTEN__
#define _FIELD(name, ST, field, t) {({TEQ(((ST*)0)->field, t); name;}), {.def=&_##t##_def, .offset=offsetof(ST, field)}}
#else
#define _FIELD(name, ST, field, t) {name, {.def=&_##t##_def, .offset=offsetof(ST, field)}}
#endif

#define FIELD_RW(ST, field, t) _FIELD(#field, ST, field, t)
#define _FIELD_RW(field, t) FIELD_RW(_ST, field, t)
#define _FIELD_RO _FIELD_RW

#define _U8ARR_FIELD(field, l) {#field, {&U8_a##l##_def, offsetof(_ST, field)}}

#define DEC_U8ARR_DEF(n) \
        static ValueDef U8_a##n##_def = {\
            ValueTypeArray, n, .subdef.arrDef = &_U8_def\
        }

#define _U16ARR_FIELD(field, l) {#field, {&U16_a##l##_def, offsetof(_ST, field)}}

#define DEC_U16ARR_DEF(n) \
    static ValueDef U16_a##n##_def = {\
        ValueTypeArray, sizeof(U16)*n, .subdef.arrDef = &_U16_def\
    }

#define _BEGIN_SDEF(name) static Field name##_fields[] = {
#define _END_SDEF(name, p) }; \
\
            static ObjectDef name##_obj_def = { \
                AL(name##_fields), 0, sizeof(_ST), name##_fields \
            }; \
\
            static ValueDef name##_value_def = { \
                .type = ValueTypeObject, \
                .size = sizeof(_ST), \
                .subdef.objDef = &name##_obj_def, \
            }; \
            static Value name = {.def = &name##_value_def}; \
            name.offset = (U32)p;

#define TEQ(a, b) typedef int _[sizeof(a) == sizeof(b) ? 1 : -1];

// #### dynamic add
#define DEFADD_U8ARR(name, n) ObjectDef_addFieldArray(def, #name, ValueTypeU8, name, n);
#define DEFADD_GBKARR(name, n) ObjectDef_addFieldGBKArray(def, #name, name, n);
#define DEFADDF(name, t) ({ TEQ(name, t); ObjectDef_addFieldF(def, #name, ValueType##t, &name, 0, 0); })

#define DEFADD_U16ARR(name, n) ObjectDef_addFieldArray(def, #name, ValueTypeU16, name, n);
#define DEFADD_U32ARR(name, n) ObjectDef_addFieldArray(def, #name, ValueTypeU32, name, n);

#define HOOK_showSkill "showSkill"

#endif /* bind_objects_h */

