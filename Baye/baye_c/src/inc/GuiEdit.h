/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：dictsys.h
*	文件标识：A600系统
*	摘    要：Editor 结构 
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	 
*******************************************************************************************/
#ifndef __GUIEDITOR_H
#define __GUIEDITOR_H

#include  "dictsys.h"
#include  "stdlib.h"

#define  far            banked

#define		NULLTYPE		                     0
#define 	TEXT_FIELD			                 19    
#define 	TEXT_FIELD_ONELINE		             20
 
#define         DispMode                        0x2
#define         EditMode                        0x1
#define         SpUserMode                      0x0
/*
  My type define                        
*/
typedef    unsigned int 	WORD;
typedef    unsigned char  	BYTE;
/*
   编辑字段的结构
*/
typedef struct {
		const char *title;
		BYTE imetype;
		BYTE inputtype;
		unsigned char type;
		int offset;
		int size;
	       } EDITFIELD;

/*
编辑区域的结构
*/
typedef struct {
		int x,y;
		int tx;
		int twidth;
		int ewidth;
		int lines;
		WORD *exitkeylist;
	} EDITAREA;
/*
显示区域的结构
*/
typedef	struct{
		int sx,sy;
		unsigned char fw,fh;
		int width;
		int dwidth;
		int lines;
		int ml;
		int tx;					
		BYTE wordwrap;
	} DISAREA;
/*
字段的信息结构
*/
typedef	struct{	 
		int	*fno;	 		 	 
		int	*pos;					 
	} FIELDINFO;
	
extern far WORD Editor(EDITAREA *es, EDITFIELD *field,
		char *b, int bsize, int *sl, int *cx, int *cy,char mode);	
extern far char *memcpy(char *dst, char *src, int len);
extern far char *memset(char *str, char c, int len);
extern far char *memmove(char *dst, const char *src, int len);		
#endif   









		