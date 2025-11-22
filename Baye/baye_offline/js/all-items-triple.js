/**
 * 三国霸业全物品X3功能
 * 当用户点击iOS设置中的"添加到存档"按钮时，
 * 给当前玩家的第一个城池添加游戏中所有物品各3份
 */

(function() {
    'use strict';

    /**
     * 实现全物品功能
     * 将游戏中所有物品各添加3份到玩家的第一个城池
     * @returns {Object} 返回执行结果对象 {success: boolean, message: string}
     */
    function addAllItemsTriple() {
        console.log('[全物品] ===== 开始执行全物品功能 =====');

        // 检查游戏引擎是否已初始化
        if (typeof baye === 'undefined' || !baye.data) {
            console.error('[全物品] 错误：游戏引擎未初始化');
            return {
                success: false,
                message: '游戏尚未加载，请进入游戏后再试'
            };
        }

        // 检查是否有JavaScript桥接函数
        if (typeof _bayeCityAddGoods !== 'function') {
            console.error('[全物品] 错误：bayeCityAddGoods函数不可用');
            return {
                success: false,
                message: '游戏功能不可用，请重新加载游戏'
            };
        }

        try {
            var playerKing = baye.data.g_PlayerKing;
            var cities = baye.data.g_Cities;
            var allTools = baye.data.g_Tools; // 游戏中的所有物品

            console.log('[全物品] 玩家君主ID: ' + playerKing);
            console.log('[全物品] 游戏物品总数: ' + allTools.length);

            // 查找玩家的第一个城池
            var targetCityIndex = -1;
            var targetCityName = '';

            for (var cityIndex = 0; cityIndex < cities.length; cityIndex++) {
                var city = cities[cityIndex];

                // 找到玩家拥有的第一个城池
                if (city.Belong === (playerKing + 1)) {
                    targetCityIndex = cityIndex;
                    targetCityName = baye.getCityName(cityIndex);
                    break;
                }
            }

            if (targetCityIndex === -1) {
                console.error('[全物品] 错误：未找到玩家拥有的城池');
                return {
                    success: false,
                    message: '未找到您拥有的城池，请先占领一座城池'
                };
            }

            var addedItems = 0;
            var failedItems = 0;
            var skippedItems = 0;

            // 遍历游戏中的所有物品，每个添加1份
            for (var i = 0; i < allTools.length; i++) {
                var itemName = baye.getToolName(i);

                // 过滤无效物品（名称为空或null）
                if (!itemName || itemName.trim() === '') {
                    skippedItems++;
                    // console.log('[全物品] ⊘ 跳过无效物品 (ID: ' + i + ')');
                    continue;
                }

                // 添加物品
                var result = _bayeCityAddGoods(targetCityIndex, i);
                if (result) {
                    addedItems++;
                    console.log('[全物品] ✓ 成功添加: ' + itemName + ' (ID: ' + i + ')');
                } else {
                    failedItems++;
                    console.log('[全物品] × 添加失败: ' + itemName + ' (ID: ' + i + ')');
                }
            }

            console.log('[全物品] ========== 执行完成 ==========');
            console.log('[全物品] 物品总数: ' + allTools.length);
            console.log('[全物品] 成功添加: ' + addedItems + ' 个');
            console.log('[全物品] 跳过无效: ' + skippedItems + ' 个');
            console.log('[全物品] 添加失败: ' + failedItems + ' 个');

            // 返回成功结果
            var message = '全物品功能执行成功！' +
                         '请进入游戏查看物品栏';

            return {
                success: true,
                message: message
            };

        } catch (e) {
            console.error('[全物品] 执行时出错: ' + e.message);
            console.error('[全物品] 错误堆栈: ' + e.stack);
            return {
                success: false,
                message: '执行失败: ' + e.message
            };
        }
    }

    /**
     * 暴露全局API
     * iOS端可以通过 window.addAllItemsTriple() 调用
     */
    window.addAllItemsTriple = function() {
        console.log('[全物品] 全局API被调用');
        return addAllItemsTriple();
    };

    console.log('[全物品] 模块加载完成');
})();
