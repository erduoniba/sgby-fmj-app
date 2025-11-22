#ifndef	_COMPA_H
#define	_COMPA_H
#include "../inc/dictsys.h"
#include <stdlib.h>
#include <string.h>
#include "consdef.h"

#ifndef __cplusplus
#ifndef bool
typedef	char bool;
#endif
#endif
/*+++++++++++++++
 通用函数的宏替换
 +++++++++++++++*/
/* 随机函数 */
//#define	gam_srand(seed)		srand(seed)		/* 替换A系列系统中的随机初始化函数 */
//#define	gam_rand()		_gam_rand()			/* 替换A系列系统中的随机函数 */
int gam_rand(void);
void gam_srand(unsigned int seed);
int gam_seed(void);

/* 内存函数 */
#define	gam_memcpy(a,s,l)	memcpy(a,s,l)			/* 替换A系列系统中常驻的内存拷贝函数 */
#define	gam_memset(buf,val,len)	memset(buf,val,len)			/* 替换A系列系统中的内存填充函数 */

#ifdef MEM_DEBUG
#define gam_malloc(sz) _gam_malloc(sz, __func__, __LINE__)
#define gam_strdup(sz) _gam_strdup(sz, __func__, __LINE__)
#define gam_realloc(p, sz) _gam_realloc(p, sz, __func__, __LINE__)
void* _gam_malloc(size_t sz, const char*loc, int line);
char* _gam_strdup(const char* s, const char*loc, int line);
void* _gam_realloc(void*p, size_t sz, const char*loc, int line);
void gam_free(void*p);
void gam_print_gc();
void gam_gc_check_all();
#else
#define gam_malloc(sz) calloc(1, sz)
#define gam_strdup(s) (U8*)strdup((const char*)s)
#define gam_realloc realloc
#define gam_free free
#define gam_print_gc()
#define gam_gc_check_all()
#endif

/* 字符串函数 */
#define	gam_strcmp(str1,str2)	strcmp((const char*)str1,(const char*)str2)
#define	gam_strcat(str1,str2)	strcat((char*)str1,(const char*)str2)
#define	gam_strlen(str)		strlen((const char*)str)
#define	gam_itoa(i,str,bdh)	itoa(i,(char*)str,bdh)
#define	gam_ltoa(l,str,bdh)	ltoa(l,(char*)str,bdh)
#define	gam_atoi(str)		atoi((const char*)str)
#define	gam_atol(str)		atol((const char*)str)

/* 屏幕操作函数 */
#if 0
#define	gam_clrlcd(l,t,r,b)	SysLcdPartClear(l,t,r,b)		/* 清除屏幕矩形 */
#define	gam_clslcd()		SysLcdPartClear(0,0,SCR_WID-1,SCR_HGT-1)/* 清除整个屏幕 */
#define	gam_revlcd(l,t,r,b)	SysLcdReverse(l,t,r,b)			/* 反显屏幕 */
#define	gam_putpixel(x,y,c)	SysPutPixel(x,y,c)			/* 画点函数 */
#define	gam_line(l,t,r,b)	SysLine(l,t,r,b)			/* 显示直线 */
#define	gam_linec(l,t,r,b)	SysLineClear(l,t,r,b)			/* 消隐直线 */
#define	gam_rect(l,t,r,b)	SysRect(l,t,r,b)			/* 显示矩形 */
#define	gam_rectc(l,t,r,b)	SysRectClear(l,t,r,b)			/* 消隐矩形 */
#else
FAR void gam_clrlcd(PT x1,PT y1,PT x2,PT y2);
FAR void gam_clslcd(void);
FAR void gam_revlcd(PT x1,PT y1,PT x2,PT y2);
FAR void gam_line(PT x1,PT y1,PT x2,PT y2);
FAR void gam_rectc(PT x1,PT y1,PT x2,PT y2);
FAR void gam_rect(PT x1,PT y1,PT x2,PT y2);
FAR void gam_putpixel(PT x,PT y,U8 data);
FAR void gam_usescr(U8 scr);
#endif
#define	gam_getkey()		SysGetKey()				/* 有按键获取，无按键返回 */
#define gam_drawpic(id, ind, x, y, flag) PlcRPicShowEx(id, 0, ind+1, x, y, flag)

#define	gam_selectscr(scr)	SysSelectScreen(scr)
#define gam_copyscr(scr)    SysCopyScreen(scr)

#define gam_savscr() SysSaveScreen()
#define gam_restorescr() SysRestoreScreen()

#define gam_clrvscr         GamClearScreenV

U32	GamStrShow(PT x,PT y,const U8 *buf);

#ifndef HAVE_ITOA
#include "itoa.h"
#endif

FAR void logPicture(U8 wid,U8 hgt, U8* buffer);

#endif	/* _COMPA_H */
