/***********************************************************************
 * 游戏速度控制头文件
 * 提供游戏速度倍数控制功能
 * 
 * 作者: Harry
 * 日期: 2025-08-13
 ***********************************************************************/

#ifndef BAYE_SPEED_H
#define BAYE_SPEED_H

#include "stdsys.h"

/* 速度倍数范围 */
#define SPEED_MIN 0.2f
#define SPEED_MAX 5.0f
#define SPEED_DEFAULT 1.0f

/* 获取当前游戏速度倍数 */
FAR float bayeGetGameSpeed(void);

/* 设置游戏速度倍数 */
FAR void bayeSetGameSpeed(float speed);

/* 获取调整后的时间间隔 */
FAR int bayeGetAdjustedInterval(int baseInterval);

#endif /* BAYE_SPEED_H */