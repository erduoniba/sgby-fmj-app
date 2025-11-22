#pragma once
#include "framework.h"

typedef struct tagMsgType
{
	UINT8 type;
	UINT16 param;
}MsgType, * PtrMsg;

#define		SYS_FLAG_TIMER_MASK				0x01			/* 定时 */

#define		DICT_WM_DUMMY		0x00		/* 无效消息 */
#define		DICT_WM_KEY			0x01		/* 按键值，只有行列信息 */

#define		DICT_WM_CHAR_ASC	0x02		/* 输入 ASCII码 */
#define		DICT_WM_CHAR_HZ		0x03		/* 输入汉字 */
#define		DICT_WM_CHAR_MATH	0x04		/* 数学功能键 */
#define		DICT_WM_CHAR_FUN	0x05		/* 输入功能键 */

#define		DICT_WM_TIMER		0x06		/* 定时到 */
#define		DICT_WM_COM			0x07		/* 串口接收到数据 */

#define		DICT_WM_POWER		0x08		/* 电源 */

#define		DICT_WM_ALERT		0x09		/* 闹钟到 ，行程到 */
#define		DICT_WM_COMMAND		0x0A		/* 界面相关 */

/*-----------------------------------------------------------------------------------------
*			WM_COMMAND  类消息 对应消息值定义
*-----------------------------------------------------------------------------------------*/
#define		CMD_CHN_INPUT_OPEN			1			/* 输入法打开 */
#define		CMD_CHN_INPUT_CLOSE			2			/* 输入法关闭 */
#define		CMD_RETURN_HOME				3			/* 返回主主界面 */
#define		CMD_SCR_NESTED				4			/* 界面重叠 */

typedef struct tagRandEnv
{
	UINT32 next;
	UINT16 randMax;
} RandEnvType, * PtrRandEnv;
