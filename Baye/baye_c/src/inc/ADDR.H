/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：addr.h
*	文件标识：A600系统
*	摘    要：常用的头文件
*   注    意：该文件要在 dictsys.h 后面被包含
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.7.26   任新村      完成基本内容
*******************************************************************************************/
#ifndef		DICTADDR_H
#define		DICTADDR_H
/*******************************************************************************************/
#define		MEM_HEAP_START				0x2c00		/* 堆首地址 */
#define		MEM_HEAP_SIZE				0x1400		/* 堆大小 */

/*******************************************************************************************/
#define		SYS_DATA_LOG_BANK			0x9				/* 数据 bank 的逻辑 bank 号*/
#define		SYS_DATA_BANK_NUM			0x4				/* 数据 bank 的个数 */
#define		SYS_DATA_BANK_ADDR			0x9000			/* 数据 bank 的的地址*/

/*******************************************************************************************/
#if			(DICT_PHY_VER == VER_USR_DEBUG)
#define		SYS_PHY_START_ADDR		0xf80000		/* 目前 512K 开发板的程序空间起始地址 */

#elif		(DICT_PHY_VER == VER_RXC_DEBUG)
#define		SYS_PHY_START_ADDR		0xf00000		/* 目前 1M 开发板的程序空间起始地址 */

#elif		(DICT_PHY_VER == VER_RXC_DOWN)
#define		SYS_PHY_START_ADDR		0xf80000		/* 目前 512 开发板的程序空间起始地址 */

#elif		(DICT_PHY_VER == VER_RXC_RELEASE)
#define		SYS_PHY_START_ADDR		0xea0000  /*0xe00000*/		/* 目前 2M 开发板的程序空间起始地址 */
#endif
/*********************************************************************************************/
#define			DATA_BANK_SIZE			0x4000
#define			SHELL_DATA_ADDR			0x30000
#define			APP_DATA_START_ADDR		0x130000
/*********************************************************************************************/
#if			(DICT_PHY_VER == VER_RXC_RELEASE )
#define	FONT_DATA_BASE_ADDR			0x80
#else
#define	FONT_DATA_BASE_ADDR			0x40
#endif

/*********************************************************************************************/
#define		STUDY_PHY_DATA_ADDR		0x894000		/* 学习数据物理地址 */
#define		DATA_PHY_ADDR			0x7b0000		/* 资料地址 */

#define		DICT_SPL_DATA1			0x0400000		/* 简体词典数据 1*/
#define		DICT_SPL_DATA2			0x0400000		/* 简体词典数据 2*/

#endif
