/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：dictsys.h
*	文件标识：A600系统
*	摘    要：A600系统数据类型和函数接口定义
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.7.26   任新村      完成基本内容
*******************************************************************************************/
#ifndef		SYSRESV_H
#define		SYSRESV_H

#ifndef		DICTSYS_H
#include	"DictSys.h"
#endif

/****************************************************************************************
*								系统保留宏定义
****************************************************************************************/
#define		MAX_MSG_NUM		8			/*	消息队列长度 */
#define		MAX_KEY_NUM		64			/*  按键个数 */
#define		MSG_QUE_MASK	0x07
/****************************************************************************************
*								系统保留函数	
****************************************************************************************/
extern		U8		g_ApoState;


#ifndef		PROCSWCH_C_LOCAL
FAR	void	GuiSetDefaultHeirApp();
#endif

FAR U8		LowBatteryDetection();
FAR	void	SysChnInputInit();
FAR	U8		SysChnInput(PtrMsg pMsg);
FAR	void	SysChnInputOpen(void);
FAR	void	SysChnInputClose(void);
FAR	U8		SysSymInput(PtrMsg	pMsg);
FAR	void	SysSymInputOpen(void);
FAR	void	SysSymInputClose(void);
FAR	void	SysHalt(void);
FAR	void	SysSleep(void);
FAR U8		SysGetKey();
FAR	void	SysHalt();

FAR	U8		AppAlarmOut();
FAR	U8		AppScheduleOut();
FAR	void	TimeRefreshTodayInfo();		/* 天中断 */

#define		PSW_START			0
#define		PSW_GAME			1
#define		PSW_DATA			2

FAR U8		CheckOldPWMain(U8 index);

FAR	U8		SetGetHaveSetValue(U8	index);
#define		GUI_PWON_TYPE_KJHM		4		/* 开机画面	*/
#define		GUI_PWON_TYPE_GJHM		5		/* 关机画面 */
		
/****************************************************************************************
*								自检函数
****************************************************************************************/
FAR  void SysLCDFill();			/* Lcd全部点亮函数 */
FAR  void SysLcdSelfTest();		/*   Lcd自检函数 */
FAR	 void BuzzerSelfCheck();
/************************************************************************************
*					应用程序初始化
************************************************************************************/
#define		APP_INIT_ON_RESET_NUM		7		/* 实际调用的个数 */

FAR	void	AppDictInitOnReset(void);		
FAR	void	AppStudyInitOnReset(void);
FAR	void	AppCalcInitOnReset(void);
FAR	void	AppMemoInitOnReset(void);
FAR	void	AppTimeInitOnReset(void);
FAR	void	AppGameInitOnReset(void);
FAR	void	AppDownLoadInitOnReset(void);
FAR	void	AppDataInitOnReset(void);
FAR	void	AppSysSetInitOnReset(void);
/************************************************************************************
*					程序  ID 及顺序属性
************************************************************************************/
#define		GUI_APP_TYPE_NORMAL			0x01		/* 固化程序 */
#define		GUI_APP_TYPE_DWN_PRG		0x02		/* 下载程序 */
#define		GUI_APP_TYPE_DWN_DATA		0x03		/* 下载数据 */
#define		GUI_APP_TYPE_HOT_APP		0x04		/* 热键程序 */

#define		GUI_APP_ID_DOWN_PRG			0xe000		/* 下载程序	*/
#define		GUI_APP_ID_ON_OFF			0xff02		/* 关机		*/
#define		GUI_APP_ID_HOME				0xff01		/* 主目录	*/

#define		GUI_APP_ID_EC_DICT			0x0000
#define		GUI_APP_ID_CE_DCIT			0x0001
#define		GUI_APP_ID_DOWN_LOAD		0x0500
#define		GUI_APP_ID_CARD				0x0700

#define		HOME_ID_DATA				3			/* 模块在 主目录顺序号 */
#define		HOME_ID_GAME				4
#define		HOME_ID_MEMO				7

/****************************************************************************************
*								键盘状态	
****************************************************************************************/
#if	0
#define		SYS_NUM_INPUT_STATE			SYS_NUM_KBD			/* 数字键盘 */
#define		SYS_ENG_INPUT_STATE			SYS_ENG_KBD			/* 英文键盘 */
#define		SYS_CHN_INPUT_STATE			SYS_CHN_KBD			/* 中文键盘 */
#define		SYS_SYM_INPUT_STATE			SYS_SYM_KBD			/* 符号键盘 */
#endif

#define		SYS_INPUT_STATE_MASK		0x0f			/* 键盘类型检测 */
#define		SYS_INPUT_STATE_NMASK		0xf0			/* 键盘类型检测 */

/****************************************************************************************
*								系统中断标志位	
****************************************************************************************/
#define		SYS_FLAG_TIMER_MASK				0x00000001			/* 定时 */
#define		SYS_FLAG_TIMER_NMASK			0xfffffffe

#define		SYS_FLAG_ALARM_MASK				0x00000002			/* 闹钟 */
#define		SYS_FLAG_ALARM_NMASK			0xfffffffd

#define		SYS_FLAG_SCHED_MASK				0x00000004			/* 行程 */
#define		SYS_FLAG_SCHED_NMASK			0xfffffffb

#define		SYS_FLAG_POWER_OFF_MASK			0x00000008			/* 开关机状态 0 - 开机 1-关机 */
#define		SYS_FLAG_POWER_OFF_NMASK		0xfffffff7

#define		SYS_FLAG_ZDGJ_MASK				0x00000010			/* 自动关机 */
#define		SYS_FLAG_ZDGJ_NMASK				0xffffffef

#define		SYS_FLAG_QZGJ_MASK				0x00000020			/* 强制关机 */
#define		SYS_FLAG_QZGJ_NMASK				0xffffffdf

#define		SYS_FLAG_LOW_POWER_MASK			0x00000040			/* 低电检测 */
#define		SYS_FLAG_LOW_POWER_NMASK		0xffffffbf

#define		SYS_FLAG_DAY_INT_MASK			0x00000080			/* 天中断 */
#define		SYS_FLAG_DAY_INT_NMASK			0xffffff7f

#define		SYS_FLAG_COM_MASK				0x02000000			/* 串口 */
#define		SYS_FLAG_COM_NMASK				0xfdffffff

#define		SYS_FLAG_DELAY_POWER_OFF_MASK	0x00010000			/* 延迟关机 */
#define		SYS_FLAG_DELAY_POWER_OFF_NMASK	0xfffeffff

#define		SYS_FLAG_NO_APO_MASK			0x00020000			/* 不自动关机 */
#define		SYS_FLAG_NO_APO_NMASK			0xfffdffff

#define		SYS_FLAG_ENTER_PSW_MASK			0x00040000			/* 进入密码 */
#define		SYS_FLAG_ENTER_PSW_NMASK		0xfffbffff

#endif


