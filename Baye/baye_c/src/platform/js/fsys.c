//
//  fsys.c
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//

#include "baye/stdsys.h"
#include "baye/comm.h"
#include "baye/enghead.h"
#include <stdio.h>
#include <emscripten.h>

#define ROM_PATH "/rom/"
#define SAV_PATH "/data/"

struct gam_FILE {
    U8 (*fclose)(gam_FILE*fp);
    U8 (*fseek)(gam_FILE*fp, U32 offset, U8 fromwhere);
    U32 (*fread)(U8*buf, U8 size, U16 count, gam_FILE*fp);
    U32 (*fwrite)(U8 *buf, U32 size, U16 count, gam_FILE *fhandle);
    U32 (*ftell)(gam_FILE*fp);
};

static gam_FILE *rom_fopen(const U8 *fname,U8 pmode);
static gam_FILE *sav_fopen(const U8 *fname, U8 pmode);

FAR gam_FILE *gam_fopen(const U8 *fname,U8 pmode)
{
    static char rom_prefix[] = ROM_PATH;
    static char sav_prefix[] = SAV_PATH;

    if (strncmp(rom_prefix, (const char*)fname, sizeof(rom_prefix) - 1) == 0) {
        return rom_fopen(fname, pmode);
    } else
    if (strncmp(sav_prefix, (const char*)fname, sizeof(sav_prefix) - 1) == 0) {
        return sav_fopen(fname, pmode);
    }
    return NULL;
}

FAR U8 gam_fclose(gam_FILE *fp)
{
    return fp->fclose(fp);
}

FAR U8 gam_fseek (gam_FILE *fp, U32 offset, U8 fromwhere)
{
    return fp->fseek(fp, offset, fromwhere);
}

FAR U32 gam_fread(U8 *buf, U8 size, U16 count, gam_FILE *fp)
{
    return fp->fread(buf, size, count, fp);
}

FAR U32 gam_fwrite(U8 *buf, U32 size, U16 count, gam_FILE *fp)
{
    return fp->fwrite(buf, size, count, fp);
}

FAR U32 gam_ftell(gam_FILE *fp)
{
    return fp->ftell(fp);
}

FAR U8 *gam_fload(U8 *bptr, U32 addr, gam_FILE *fhandle)
{
    return bptr + addr;
}
/***********************************************************************
 * 说明:     增强功能函数—修改文件名
 * 输入参数: fhandle-文件句柄	newname-新的文件名
 * 返回值  : 0-操作成功		!0-出错代码
***********************************************************************/
//FAR U8 gam_frename(gam_FILE *fhandle,U8 *newname)
//{
//	return 0;
//}
/***********************************************************************
 * 说明:     增强功能函数—文件删除
 * 输入参数: fhandle-文件句柄
 * 返回值  : 0-操作成功		!0-出错代码
***********************************************************************/
//FAR U8 gam_fdelete(gam_FILE *fhandle)
//{
//    return 0;
//}


///*******************************************

static const U8 font[] =
#include "font.bin.c"
;
static const U8 font24_1[] =
#include "font24.1.bin.c"
;
static const U8 font24_2[] =
#include "font24.2.bin.c"
;
static const U8 font24_3[] =
#include "font24.3.bin.c"
;
static const U8 font24_4[] =
#include "font24.4.bin.c"
;
static const U8 font24_ascii_1[] =
#include "font24_ascii.1.c"
;
static const U8 font24_ascii_2[] =
#include "font24_ascii.2.c"
;
//static U8 dat[] =
//#include "dat.lib.c"
//;


typedef struct {
    gam_FILE base;
    U32 cur;
    U32 length;
    U8* data;
} rom_FILE;


static U8 rom_fclose(gam_FILE*fp) {
    gam_free(fp);
    return 0;
}

static U8 rom_fseek(gam_FILE*fp, U32 offset, U8 fromwhere) {
    rom_FILE* _fp = (rom_FILE*)fp;

    if (fromwhere == SEEK_SET) {
        if (offset < _fp->length) {
            _fp->cur = offset;
            return 0;
        } else {
            return -1;
        }
    } else if(fromwhere == SEEK_CUR) {
        U32 loc = _fp->cur + offset;
        if (loc < _fp->length) {
            _fp->cur = loc;
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

static U32 rom_fread(U8*buf, U8 size, U16 count, gam_FILE*fp) {
    rom_FILE* _fp = (rom_FILE*)fp;

    U32 remain = _fp->length - _fp->cur;
    U32 l = MIN(size*count, remain);
    if (l > 0) {
        gam_memcpy(buf, _fp->data + _fp->cur, l);
        _fp->cur += l;
    }
    return l/size;
}

static U32 rom_fwrite(U8 *buf, U32 size, U16 count, gam_FILE *fhandle) {
    return 0;
}

static U32 rom_ftell(gam_FILE*fp) {
    rom_FILE* _fp = (rom_FILE*)fp;
    return _fp->cur;
}

static void rom_finit(rom_FILE *fp) {
    fp->base.fclose = rom_fclose;
    fp->base.fseek = rom_fseek;
    fp->base.fread = rom_fread;
    fp->base.fwrite = rom_fwrite;
    fp->base.ftell = rom_ftell;
    fp->cur = 0;
    fp->length = 0;
    fp->data = NULL;
}

static rom_FILE *rom_fnew() {
    rom_FILE* fp = gam_malloc(sizeof(rom_FILE));
    rom_finit(fp);
    return fp;
}

static gam_FILE *rom_fopen(const U8 *fname,U8 pmode) {
    if (pmode != 'r') {
        return NULL;
    }

    if (0 == gam_strcmp(fname, "/rom/fonts/font.bin")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font;
        fp->length = sizeof(font);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/fonts/font24.cn.1")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font24_1;
        fp->length = sizeof(font24_1);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/fonts/font24.cn.2")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font24_2;
        fp->length = sizeof(font24_2);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/fonts/font24.cn.3")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font24_3;
        fp->length = sizeof(font24_3);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/fonts/font24.cn.4")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font24_4;
        fp->length = sizeof(font24_4);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/fonts/font24.en.1")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font24_ascii_1;
        fp->length = sizeof(font24_ascii_1);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/fonts/font24.en.2")) {
        rom_FILE* fp = rom_fnew();
        fp->data = (U8*)font24_ascii_2;
        fp->length = sizeof(font24_ascii_2);
        return (gam_FILE*)fp;
    } else if (0 == gam_strcmp(fname, "/rom/dat.lib")) {
//        rom_FILE* fp = rom_fnew();
//        fp->data = dat;
//        fp->length = sizeof(dat);
//        return (gam_FILE*)fp;
        return NULL;
    } else {
        return NULL;
    }
}

// ------------------- save fs --------------------------

typedef struct {
    rom_FILE base;
    U8* fname;
    U32 alloced;
} sav_write_FILE;

static U8* getValue(const U8*key) {

    int value = EM_ASM_INT({
        var key = UTF8ToString($0);
        var value = "";
        var filename = "baye/" + key;

        if (window.bayeLoadFileContent) {
            value = window.bayeLoadFileContent(filename);
        } else {
            value = window.localStorage[filename];
        }

        if (value) {
            var buffer = Module._bayeAlloc(value.length+1);
            Module.stringToUTF8(value, buffer, value.length+1);
            return buffer;
        }
        return 0;
    }, (int)key);

    return (U8*)value;
}

static void setValue(const U8*key, const U8*value) {
    EM_ASM_({
        var key = UTF8ToString($0);
        var value = UTF8ToString($1);
        var filename = "baye/" + key;

        if (window.bayeSaveFileContent) {
            window.bayeSaveFileContent(filename, value);
        } else {
            window.localStorage[filename] = value;
        }
    }, (int)key, (int)value);
}

static U8* hex_encode(U8*s, U32 length) {
    U8* buffer = (U8*)gam_malloc(length * 2 + 1);
    buffer[length * 2] = 0;
    for (int i = 0; i < length; i++) {
        sprintf((char*)buffer+i*2, "%02X", s[i]);
    }
    return buffer;
}

static U8* hex_decode(U8*s, U32*length) {
    U32 l = strlen((char*)s);

    if (l % 2) return NULL;

    U8* buffer = gam_malloc(l/2 + 1);

    for (int i = 0; i < l; i += 2) {
        U32 c = 0;
        if (sscanf((char*)s+i, "%02X", &c) != 1) {
            gam_free(buffer);
            return NULL;
        }
        buffer[i/2] = (U8)c;
    }
    *length = l/2;
    return buffer;
}

static U8 sav_fclose(gam_FILE*fp_) {
    rom_FILE*fp = (rom_FILE*)fp_;
    gam_free(fp->data);
    gam_free(fp);
    return 0;
}

static U8 sav_fclose_w(gam_FILE*fp_) {
    sav_write_FILE*fp = (sav_write_FILE*)fp_;

    U8* hex_data = hex_encode(fp->base.data, fp->base.cur);
    gam_free(fp->base.data);

    setValue(fp->fname, hex_data);
    gam_free(hex_data);
    gam_free(fp->fname);
    gam_free(fp);
    return 0;
}

static U32 sav_fwrite(U8 *buf, U32 size, U16 count, gam_FILE *fp_) {
    sav_write_FILE*fp = (sav_write_FILE*)fp_;

    U32 len = size * count;

    if (fp->base.cur + len > fp->alloced) {
        U32 new_len = fp->base.cur + len + 1024;
        fp->base.data = gam_realloc(fp->base.data, new_len);
        fp->alloced = new_len;
    }
    memcpy(fp->base.data + fp->base.cur, buf, len);
    fp->base.cur += len;
    return count;
}

static void sav_finit_w(sav_write_FILE* fp, const U8*fname) {
    rom_finit((rom_FILE*)fp);
    fp->base.data = gam_malloc(1024);
    fp->alloced = 1024;
    fp->base.base.fclose = sav_fclose_w;
    fp->base.base.fwrite = sav_fwrite;
    fp->fname = (U8*)gam_strdup((char*)fname);
}

static gam_FILE *sav_fopen(const U8 *fname, U8 pmode) {
    if (pmode == 'r') {
        U8* data = getValue(fname);
        if (data) {
            U32 length = 0;
            U8 * f_data = hex_decode(data, &length);
            gam_free(data);

            if (f_data) {
                rom_FILE* fp = rom_fnew();
                fp->base.fclose = sav_fclose;
                fp->data = f_data;
                fp->length = length;
                return (gam_FILE*)fp;
            }
        }
        return NULL;
    } else if (pmode == 'w') {
        sav_write_FILE* fp = (sav_write_FILE*)gam_malloc(sizeof(sav_write_FILE));
        sav_finit_w(fp, fname);
        return (gam_FILE*)fp;
    } else {
        return NULL;
    }
}

