#include "inc/DictAfx.h"

/*********************************************************************************************
 *                        程序及字库等地址
 **********************************************************************************************/
const U8 MT_DataAddrL = 0x00;
const U8 MT_DataAddrM = 0x70;

const U8 ProgramSizeH =	(SYS_PHY_START_ADDR >> 20) & 0x0f;			/* 程序起始地址 */
const U8 ProgramSizeL = (SYS_PHY_START_ADDR >> 12) & 0xff;
const U8 MT_DataAddrH = (SYS_PHY_START_ADDR >> 16) & 0xff +1 ;

/*********************************************************************************************/

const U8  FontDataBaseAddr = FONT_DATA_BASE_ADDR ;				/* 字库输入法地址 */
const U32 ChnInputDataAddr = (FONT_DATA_BASE_ADDR<<16) + 0x4CCE0;

/*********************************************************************************************/
#if			(DICT_PHY_VER == VER_RXC_RELEASE )

const	U32	StudyDataAddr = STUDY_PHY_DATA_ADDR;				/* 学习数据物理地址 */
const	U32	DataBinAddr = DATA_PHY_ADDR;						/* 资料地址 */

#endif

/*********************************************************************************************
 *                        LCD 显示缓冲区
 **********************************************************************************************/
#define LCD_RAMMAPADDRESSMOVE	0x01

#if		(LCD_RAMMAPADDRESSMOVE == 0x01)
const U8  Lcd_RamMapAddressMove = 0x01;
const U16 Lcd_RamMapOldAddress = 0x0400;
const U16 Lcd_RamMapNewAddress = 0x1000;

#elif    	(LCD_RAMMAPADDRESSMOVE == 0x00)
const U8  Lcd_RamMapAddressMove = 0x00;
const U16 Lcd_RamMapOldAddress = 0x0400;
const U16 Lcd_RamMapNewAddress = 0x0400;
#endif



