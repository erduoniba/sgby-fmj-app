/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：shell.h
*	文件标识：A600系统主界面头文件
*	摘    要：主界面实现
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.10.07   任新村      完成基本内容
*******************************************************************************************/
#ifndef		SHELL_H

#ifndef		DICTSYS_H
#include	"dictsys.h"
#endif

/*****************************************************************************************/
typedef	FAR	int (*FarFunPtrType)(void * ,U16 para1 , U16 para2 );

typedef		struct	tagShellMenuArray			/* 子菜单结构 */
{
	U8		num;			/* 项数 */
	U8		**SubMenuStr;
}ShellMenuArrayType , *PtrShellMenuArrayType;

#define	APP_NAME_LEN		20
typedef		struct	tagAppRunInfo
{
	U16		bankTableAddr;			/* 程序起始的物理地址 */
	U32		dataAddr;				/* 数据段的物理存储地址 */
	U8		type;					/* 系统内部程序或下载程序 */
	U32		id;						/* 唯一id 号 */	
	U8		name[APP_NAME_LEN];		/* 名称 */
}AppRunInfoType, *PtrAppRunInfo ;

#define		SYS_FUN_TOTAL_NUM					10			/* 主功能总的项数 */

#define		SHELL_SUBMENU_ITEM_LEN		10
#define		SHELL_MAINMENU_ITEM_LEN		4

#define		MAIN_MENU_BMP_WIDTH			62
#define		MAIN_MENU_BMP_HEIGHT		16
#define		SHELL_DOWN_FILE_MAX_NUM		100
#define		SHELL_DOWN_FILE_TITLE_LEN	8

#ifndef		SHELL_C_LOCAL
FAR	void	ShellMenuInit();
#endif

#ifndef		APPINFO_C_LOCAL
FAR	U8	GuiSwitchApp(AppRunInfoType *pAppInfo , U16		para1, U16  para2 );
#endif

void   ShellSortMenu(U8 Data_filetype,U16 *sorted_array,U16 *filenum);
U8    ShellReadDwnTitle(U8  *des_prt,U32 in_file_py,U16 len,U8 filehandle);

extern	const	U8*		bmpMainMenu[] ; 
extern	const	U8		bmpFolder[];
extern	const	U8		strDwnFileOpenErr[] ;
extern	const	U8		cstShellFTAry[] ; 
extern	const	U8		strShellPleaseDwn[];


#endif

