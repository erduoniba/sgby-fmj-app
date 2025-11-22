/**
 * 游戏初始化模块
 * 负责游戏启动、Module配置和事件监听
 */

// 游戏启动事件处理
window.bayeStart = function() {
    console.log('🎮 游戏启动事件触发: bayeStart');

    // 触发自定义事件，方便其他系统监听
    if (typeof window.CustomEvent !== 'undefined') {
        var event = new CustomEvent('bayeGameStart', {
            detail: {
                timestamp: new Date(),
                source: 'bayeStart'
            }
        });
        window.dispatchEvent(event);
    }

    // 输出游戏数据状态
    if (typeof window.baye !== 'undefined' && window.baye.data) {
        console.log('✅ 游戏数据已就绪，城市数量:', window.baye.data.g_Cities ? window.baye.data.g_Cities.length : 'undefined');
    } else {
        console.log('⚠️ 游戏数据尚未准备就绪');
    }
};

// Module 已在 lcd.js 中初始化，这里只需要添加 postRun
Module.postRun = function (){
    lcdInit();
    touchScreenInit("lcd");

    // 清除可能导致问题的方向设置，使用自动检测
    localStorage.removeItem('bayeDisplayOrientation');
    console.log('使用自动检测的横竖屏模式');
    reloadLCD();

    //判断手机横竖屏状态：
    window.addEventListener("onorientationchange" in window ? "orientationchange" : "resize", function() {
        reloadLCD();
    }, false);

    $(window).resize(reloadLCD);

    if (bayeDebugMode) {
        $("#info").show();
    } else {
        $("#info").hide();
    }
    bayeMain();
};
