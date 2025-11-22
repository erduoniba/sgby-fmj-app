/***********************************************************************
 * 游戏速度控制实现
 * 提供游戏速度倍数控制功能
 * 
 * 作者: Harry
 * 日期: 2025-08-13
 ***********************************************************************/

#include "baye/speed.h"
#include <math.h>

/* 全局速度倍数变量 */
static float g_GameSpeedMultiple = SPEED_DEFAULT;

/***********************************************************************
 * 说明:     获取当前游戏速度倍数
 * 输入参数: 无
 * 返回值  : 当前速度倍数
 ***********************************************************************/
FAR float bayeGetGameSpeed(void)
{
    return g_GameSpeedMultiple;
}

/***********************************************************************
 * 说明:     设置游戏速度倍数
 * 输入参数: speed - 速度倍数 (0.2 到 5.0)
 * 返回值  : 无
 ***********************************************************************/
FAR void bayeSetGameSpeed(float speed)
{
    /* 限制速度范围 */
    if (speed < SPEED_MIN) {
        speed = SPEED_MIN;
    } else if (speed > SPEED_MAX) {
        speed = SPEED_MAX;
    }
    
    g_GameSpeedMultiple = speed;
}

/***********************************************************************
 * 说明:     获取调整后的时间间隔
 * 输入参数: baseInterval - 基础时间间隔（毫秒）
 * 返回值  : 调整后的时间间隔
 ***********************************************************************/
FAR int bayeGetAdjustedInterval(int baseInterval)
{
    int adjustedInterval;
    
    /* 速度越快，时间间隔越短 */
    adjustedInterval = (int)(baseInterval / g_GameSpeedMultiple);
    
    /* 确保最小间隔，避免过快导致性能问题 */
    if (adjustedInterval < 1) {
        adjustedInterval = 1;
    }
    
    return adjustedInterval;
}