#ifndef	_FSYS_H
#define	_FSYS_H
#include <stdio.h>

/*======================================
  文件系统再封装
======================================*/

/*------------------------
  文件结构体指针
  ------------------------*/
typedef	struct gam_FILE gam_FILE;				/* 文件指针类型 */

#ifndef SEEK_SET
#define	SEEK_SET	0		/* 从文件开头 */
#endif

#ifndef SEEK_CUR
#define	SEEK_CUR	1		/* 从文件指针的现行位置 */
#endif

#define	OPEN_R		1		/* 读模式 */
#define	OPEN_W		2		/* 写模式 */


/*------------------------
  函数声明
  ------------------------*/
FAR gam_FILE *gam_fopen(const U8 *fname,U8 pmode);
FAR U8 gam_fclose(gam_FILE *fhandle);
FAR U8 gam_fseek (gam_FILE *fhandle, U32 offset, U8 fromwhere);
FAR U32 gam_fread(U8 *buf, U8 size, U16 count, gam_FILE *fhandle);
FAR U32 gam_fwrite(U8 *buf, U32 size, U16 count, gam_FILE *fhandle);
FAR U32 gam_ftell(gam_FILE *fhandle);
FAR U8 *gam_fload(U8 *bptr, U32 addr, gam_FILE *fhandle);
FAR U8 *gam_freadall(gam_FILE *fhandle, U32 *outlen);
FAR U8 gam_frename(gam_FILE *fhandle,U8 *newname);
FAR U8 gam_fdelete(gam_FILE *fhandle);

#endif	/* _FSYS_H */
