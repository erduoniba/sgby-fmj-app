/**
 * 重定向守卫
 * 检测是否需要跳转到主页，防止直接访问游戏页面
 */

(function() {
    'use strict';

    // 设置调试模式
    window.bayeDebugMode = window.location.href.match(/(localhost|debug=1)/);

    // 检查是否需要跳转到主页
    (function goHomeIfNeeded() {
        if (!window.bayeDebugMode) {
            var match = window.location.hash.match(/#([0-9]*)/i);
            if (match) {
                var ts = parseInt(match[1]);
                var now = new Date().getTime() / 1000;
                var dt = now - ts;
                if (ts && 0 <= dt && dt < 3600) {
                    return;
                }
            }
            window.location.href = "index.html";
        }
    })();
})();
