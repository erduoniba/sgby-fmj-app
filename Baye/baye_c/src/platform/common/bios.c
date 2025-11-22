//
//  bios.c
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//

#include <stdio.h>
#include "baye/compa.h"
#include "baye/comm.h"
#include "baye/consdef.h"
#include "baye/order.h"

#pragma mark - bios

void DataBankSwitch(U8 logicStartBank,U8 bankNumber,U16 physicalStartBank)
{
}

void GetDataBankNumber(U8 logicStartBank,U16* physicalBankNumber)
{
}

#pragma mark - flash
U8 *_VS_PTR;		/* 虚拟屏幕缓冲 */
U8 *_SHARE_MEM;     /* 共享临时内存 */
U8 *_FIGHTERS_IDX;      /* 出征武将队列索引(30个字节) */
U8 *_FIGHTERS;      /* 出征武将队列(30*10=300个字节) */
U8 *_ORDERQUEUE;        /* 命令队列(12*100=1200个字节) */


static void _shm_init()
{
    _SHARE_MEM = gam_malloc(202400);
    _FIGHTERS_IDX = gam_malloc(30);
    _FIGHTERS = gam_malloc(600);
    _ORDERQUEUE = gam_malloc(sizeof(OrderType) * ORDER_MAX);
}

void FlashInit(void)
{
    _shm_init();
}

void vs_ptr_init(void)
{
    _VS_PTR = gam_malloc(MAX_SCR_BUF_LEN);
}

void ResetFlash(void)
{
}
