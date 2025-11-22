/**
 * 三国霸业VIP内购功能 - 农业和商业发展倍率 (V2版本 - 直接调用C函数)
 *
 * 此文件通过调用C层函数来设置倍率，倍率计算在C代码中完成
 */

(function() {
    'use strict';

    console.log('[VIP倍率V2] 模块脚本开始加载');

    /**
     * 从localStorage读取VIP设置并应用到C代码
     */
    function loadAndApplyVIPSettings() {
        try {
            // 读取农业倍率设置
            var agricultureMultiplier = localStorage.getItem('vip.agricultureMultiplier');
            if (agricultureMultiplier) {
                var multiplier = parseInt(agricultureMultiplier, 10);
                console.log('[VIP倍率V2] 读取农业倍率：' + multiplier + '倍');

                // 调用C函数设置倍率
                if (typeof Module !== 'undefined' && Module._SetAgricultureMultiplier) {
                    Module._SetAgricultureMultiplier(multiplier);
                    console.log('[VIP倍率V2] ✓ 已通过C函数设置农业倍率：' + multiplier + '倍');
                } else {
                    console.warn('[VIP倍率V2] C函数 _SetAgricultureMultiplier 不可用');
                }
            } else {
                console.log('[VIP倍率V2] localStorage中没有农业倍率，使用默认值1');
            }

            // 读取商业倍率设置
            var commerceMultiplier = localStorage.getItem('vip.commerceMultiplier');
            if (commerceMultiplier) {
                var multiplier = parseInt(commerceMultiplier, 10);
                console.log('[VIP倍率V2] 读取商业倍率：' + multiplier + '倍');

                // 调用C函数设置倍率
                if (typeof Module !== 'undefined' && Module._SetCommerceMultiplier) {
                    Module._SetCommerceMultiplier(multiplier);
                    console.log('[VIP倍率V2] ✓ 已通过C函数设置商业倍率：' + multiplier + '倍');
                } else {
                    console.warn('[VIP倍率V2] C函数 _SetCommerceMultiplier 不可用');
                }
            } else {
                console.log('[VIP倍率V2] localStorage中没有商业倍率，使用默认值1');
            }
        } catch (e) {
            console.error('[VIP倍率V2] 读取设置失败：' + e.message);
        }
    }

    /**
     * 暴露全局API - 设置农业倍率
     * iOS原生端可通过此API设置倍率
     */
    window.setAgricultureMultiplier = function(multiplier) {
        console.log('[VIP倍率V2] setAgricultureMultiplier被调用: ' + multiplier);

        if (typeof multiplier !== 'number' || multiplier < 1 || multiplier > 10) {
            console.error('[VIP倍率V2] 无效的农业倍率值: ' + multiplier);
            return false;
        }

        // 保存到localStorage
        localStorage.setItem('vip.agricultureMultiplier', multiplier.toString());

        // 调用C函数设置倍率
        if (typeof Module !== 'undefined' && Module._SetAgricultureMultiplier) {
            Module._SetAgricultureMultiplier(multiplier);
            console.log('[VIP倍率V2] ✓ 农业倍率已设置为: ' + multiplier + '倍');
            return true;
        } else {
            console.warn('[VIP倍率V2] C函数 _SetAgricultureMultiplier 不可用，已保存到localStorage');
            return false;
        }
    };

    /**
     * 暴露全局API - 设置商业倍率
     * iOS原生端可通过此API设置倍率
     */
    window.setCommerceMultiplier = function(multiplier) {
        console.log('[VIP倍率V2] setCommerceMultiplier被调用: ' + multiplier);

        if (typeof multiplier !== 'number' || multiplier < 1 || multiplier > 10) {
            console.error('[VIP倍率V2] 无效的商业倍率值: ' + multiplier);
            return false;
        }

        // 保存到localStorage
        localStorage.setItem('vip.commerceMultiplier', multiplier.toString());

        // 调用C函数设置倍率
        if (typeof Module !== 'undefined' && Module._SetCommerceMultiplier) {
            Module._SetCommerceMultiplier(multiplier);
            console.log('[VIP倍率V2] ✓ 商业倍率已设置为: ' + multiplier + '倍');
            return true;
        } else {
            console.warn('[VIP倍率V2] C函数 _SetCommerceMultiplier 不可用，已保存到localStorage');
            return false;
        }
    };

    /**
     * 暴露全局API - 获取农业倍率
     */
    window.getAgricultureMultiplier = function() {
        var value = localStorage.getItem('vip.agricultureMultiplier');
        return value ? parseInt(value, 10) : 1;
    };

    /**
     * 暴露全局API - 获取商业倍率
     */
    window.getCommerceMultiplier = function() {
        var value = localStorage.getItem('vip.commerceMultiplier');
        return value ? parseInt(value, 10) : 1;
    };

    // 等待Module初始化完成后加载设置
    if (typeof Module !== 'undefined' && Module.onRuntimeInitialized) {
        Module.onRuntimeInitialized = (function(original) {
            return function() {
                if (original) original();
                console.log('[VIP倍率V2] Module运行时已初始化，加载VIP设置');
                loadAndApplyVIPSettings();
            };
        })(Module.onRuntimeInitialized);
    } else {
        // Module还未定义，等待一段时间后再尝试
        var checkCount = 0;
        var checkInterval = setInterval(function() {
            checkCount++;
            if (typeof Module !== 'undefined' && Module._SetAgricultureMultiplier) {
                clearInterval(checkInterval);
                console.log('[VIP倍率V2] Module已就绪，加载VIP设置');
                loadAndApplyVIPSettings();
            } else if (checkCount > 50) {  // 最多等待5秒
                clearInterval(checkInterval);
                console.warn('[VIP倍率V2] Module初始化超时，VIP倍率功能可能不可用');
            }
        }, 100);
    }

    console.log('[VIP倍率V2] 模块脚本加载完成');
})();
