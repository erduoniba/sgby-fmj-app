/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：mem.h
*	文件标识：A600系统
*	摘    要：A600内存堆管理函数头文件
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.08.20   任新村      完成基本内容
*******************************************************************************************/
#ifndef		MEM_H
#define		MEM_H

#ifndef		DICTSYS_H
#include	"dictsys.h"
#endif

#define MEM_OK					0
#define MEM_MCB_ERROR           1

#define MCB_BLANK               'b'
#define MCB_USE                 'u'

#define MCB_END                 'e'
#define MCB_NORMAL              'n'

#define MCB_LENGTH              4				/*  内存控制块的大小 */

#define	MIN_BLK_BYTES			4				/*  分配的最小字节 */
#define	MIN_BLK_MASK			0x03
#define	MIN_BLK_NMASK			0xfffc


typedef struct	tagMCB
{
    U8      use_flag;               /*	使用标志 */ 
    U8      end_flag;               /*  结束标志 */
	U16 	len;					/*  分配的空间长度，不含MCB长度 */
}MCB;

#endif

