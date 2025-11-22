/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：keytable.h
*	文件标识：A600系统
*	摘    要：A600键盘扫描码表和转换后的键盘消息值
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.08.07   任新村      完成基本内容
*******************************************************************************************/
#ifndef		KEYTABLE_H
#define		KEYTABLE_H
/*******************************************************************************************
*				转换后的扩展键  ---  功能键
*		说明：功能键对应消息类型WM_CHAR_FUN,取消息值(param)的低字节
*
******************************************* ************************************************/
#define		CHAR_EC_DICT			0x01
#define		CHAR_CE_DICT			0x02
#define		CHAR_SPK				0x03
#define		CHAR_ON_OFF				0x04
#define		CHAR_DOWNLOAD			0x05
#define		CHAR_ADDRESS			0x06
#define		CHAR_HOME_MENU			0x07
#define		CHAR_TIME				0x08
#define		CHAR_USER_DEF			0x09

#define		CHAR_PGUP				0x20
#define		CHAR_PGDN				0x21
#define		CHAR_UP					0x22
#define		CHAR_DOWN				0x23
#define		CHAR_LEFT				0x24
#define		CHAR_RIGHT				0x25
#define		CHAR_HELP				0x26
#define		CHAR_ENTER				0x27
#define		CHAR_EXIT				0x28
#define		CHAR_ZY					0x29			/*中英数*/
#define		CHAR_INPUT				0x2a

#define		CHAR_INSERT				0x30
#define		CHAR_DEL				0x31
#define		CHAR_MODIFY				0x32
#define		CHAR_SEARCH				0x33
#define		CHAR_SHIFT				0x34			/* shift */
#define		CHAR_SYM				0x35			/*  符号 */

#define		CHAR_DUMMY				0xF0

/*******************************************************************************************
*							键盘扫描码
*		说明：对应消息类型WM_KEY,取消息值(param)的低字节
*
*******************************************************************************************/
#define		KEY_ON_OFF				0x00
#define		KEY_HOME_MENU			0x01
#define		KEY_EC_DICT				0x02
#define		KEY_CE_DICT				0x03
#define		KEY_ADDRESS				0x04
#define		KEY_TIME				0x05
#define		KEY_DOWNLOAD			0x06
#define		KEY_SPK					0x07

#define		KEY_NUM_1				0x08
#define		KEY_NUM_2				0x09
#define		KEY_NUM_3				0x0a
#define		KEY_NUM_4				0x0b
#define		KEY_NUM_5				0x0c
#define		KEY_NUM_6				0x0d
#define		KEY_NUM_7				0x0e
#define		KEY_NUM_8				0x0f

#define		KEY_Q					0x10
#define		KEY_W					0x11
#define		KEY_E					0x12
#define		KEY_R					0x13
#define		KEY_T					0x14
#define		KEY_Y					0x15
#define		KEY_U					0x16
#define		KEY_I					0x17

#define		KEY_A					0x18
#define		KEY_S					0x19
#define		KEY_D					0x1a
#define		KEY_F					0x1b
#define		KEY_G					0x1c
#define		KEY_H					0x1d
#define		KEY_J					0x1e
#define		KEY_K					0x1f

#define		KEY_INPUT				0x20
#define		KEY_Z					0x21
#define		KEY_X					0x22
#define		KEY_C					0x23
#define		KEY_V					0x24
#define		KEY_B					0x25
#define		KEY_N					0x26
#define		KEY_M					0x27

#define		KEY_ZY					0x28
#define		KEY_INSERT				0x29
#define		KEY_MODIFY				0x2a
#define		KEY_SEARCH				0x2b
#define		KEY_DEL					0x2c
#define		KEY_HELP				0x2d
#define		KEY_EXIT				0x2e
#define		KEY_ENTER				0x2f

#define		KEY_NUM_9				0x30
#define		KEY_NUM_0				0x31
#define		KEY_O					0x32
#define		KEY_P					0x33
#define		KEY_L					0x34			/*	L/+/-		*/
#define		KEY_UP					0x35
#define		KEY_SPACE				0x36
#define		KEY_LEFT				0x37

#define		KEY_DOWN				0x38
#define		KEY_RIGHT				0x39
#define		KEY_PGUP				0x3a
#define		KEY_PGDN				0x3b

#define		KEY_SHIFT				KEY_ZY
#define		KEY_DOT					KEY_T

#endif			/*  KEYTABLE_H  */
