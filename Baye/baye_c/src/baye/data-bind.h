//
//  data-bind.h
//  baye-ios
//
//  Created by loong on 16/9/16.
//
//

#ifndef data_bind_h
#define data_bind_h

#include <baye/compa.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

typedef struct ObjectDef ObjectDef;
typedef struct ValueDef ValueDef;
typedef struct Object Object;
typedef struct Value Value;

typedef void(*Object_deinit_t)(Object*);
typedef void(*Value_deinit_t)(Value*);

typedef enum ValueType {
    ValueTypeU8 = 0,
    ValueTypeU16,
    ValueTypeU32,
    ValueTypeString,
    ValueTypeObject,
    ValueTypeArray,
    ValueTypeGBKArray = 7,
} ValueType;

#define ValueFlagRead       (1 << 0)
#define ValueFlagWrite      (1 << 1)
#define ValueFlagConst      (1 << 2)
#define ValueFlagRelative   (1 << 3)

#define ValueFlagRW (ValueFlagRead | ValueFlagWrite)
#define ValueFlagRO ValueFlagRead

typedef struct ValueDef {
    ValueType type;
    U32 size;
    union {
        ObjectDef *objDef;
        ValueDef *arrDef;
    }subdef;
} ValueDef;

typedef struct Value {
    ValueDef *def;
    U32 offset;
} Value;

typedef struct Field {
    const char *name;
    Value value;
} Field;

typedef struct ObjectDef
{
    U16 count;
    U16 alloced;
    U32 size;
    Field* fields;
} ObjectDef;

ObjectDef* ObjectDef_new(void);

void ObjectDef_free(ObjectDef* def);
void ObjectDef_addField(ObjectDef* def, Field* field);


extern ValueDef _U8_def;
extern ValueDef _U16_def;
extern ValueDef _U32_def;
extern ValueDef _str_def;


void ObjectDef_addField(ObjectDef* def, Field* field);
void ObjectDef_addFieldF(ObjectDef* def, const char*name, ValueType t, void* ptr, void* subdef, U32 arrLen);
void ObjectDef_addFieldArray(ObjectDef* def, const char*name, ValueType t, void* ptr, U32 arrLen);
void ObjectDef_addFieldGBKArray(ObjectDef* def, const char*name, void* ptr, U32 arrLen);

#define ObjectDef_addFieldU8(def, name, ptr) ObjectDef_addFieldF(def, name, ValueTypeU8, ptr, 0, 0)
#define ObjectDef_addFieldU16(def, name, ptr) ObjectDef_addFieldF(def, name, ValueTypeU16, ptr, 0, 0)
#define ObjectDef_addFieldU32(def, name, ptr) ObjectDef_addFieldF(def, name, ValueTypeU32, ptr, 0, 0)

#define ObjectDef_addFieldArray_DEF(name, t, len) ObjectDef_addFieldArray(def, #name, t, name, len)

#define ObjectDef_addFieldU8_DEF(name) ObjectDef_addFieldU8(def, #name, name)
#define ObjectDef_addFieldU16_DEF(name) ObjectDef_addFieldU16(def, #name, name)
#define ObjectDef_addFieldU32_DEF(name) ObjectDef_addFieldU32(def, #name, name)

#define BIND_U8EX(name, ptr) ObjectDef_addFieldF(def, name, ValueTypeU8, ptr, 0, 0)
#define BIND_U8(name) BIND_U8EX(#name, name)

#define BIND_U32EX(name, ptr) ObjectDef_addFieldU32(def, name, ptr)
#define BIND_U32(name) ObjectDef_addFieldU32(def, #name, name)

#define BIND_U16EX(name, ptr) ObjectDef_addFieldU16(def, name, ptr)
#define BIND_U16(name) ObjectDef_addFieldU16(def, #name, name)

#define BIND_U8ARR(name, len) ObjectDef_addFieldArray_DEF(name, ValueTypeU8, len)
#define BIND_U16ARR(name, len) ObjectDef_addFieldArray_DEF(name, ValueTypeU16, len)
#define BIND_GBKARR(name, len) ObjectDef_addFieldGBKArray(def, #name, name, len)

#define HOOK_RETURN(args...) {HOOK_LEAVE(); return args; }
#define HOOK_LEAVE() Value_ObjectValue_free(__v)

#define IF_HAS_HOOK(name) \
    if (has_hook(name)) \
        for (const char* _hook_name = name; _hook_name; _hook_name=0) \
            for(Value *__v = Value_ObjectValue_new(); __v; Value_ObjectValue_free(__v), __v=0) \
                for (ObjectDef *def = __v->def->subdef.objDef; def; def=0)

#define CALL_HOOK() call_hook(_hook_name, __v)
#define CALL_HOOK_S() call_hook_s(_hook_name, __v)
#define CALL_HOOK_A() call_hook_a(_hook_name, __v)

#define BIND_ID BIND_U16
#define BIND_IDX BIND_U16EX


Value* Value_ObjectValue_new(void);
void Value_ObjectValue_free(Value*value);

#endif /* data_bind_h */
