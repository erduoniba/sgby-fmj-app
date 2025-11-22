/**
 * 视口管理器
 * 负责横竖屏检测和LCD尺寸调整
 */

// 检测设备方向
function isDevicePortrait() {
    return window.innerHeight > window.innerWidth;
}

function reloadLCD(){
    var height = window.innerHeight;
    var width = window.innerWidth;
    console.log(height,width);

    if (height > width) {
        console.log('竖屏状态');

        // 竖屏模式：让 CSS 媒体查询接管，清除所有内联样式
        $("#app").attr('style', '');
        $("#lcd").attr('style', '');

        lcdRotateMode = 0;  // 不旋转
    } else {
        console.log('横屏状态，取消缩放，与body大小一致');

        // 横屏模式：让游戏画面填满整个屏幕，与body大小一致
        // 直接使用屏幕的完整宽高，不进行任何缩放

        // 对app容器应用全屏尺寸，取消缩放
        $("#app").css({
            'width': width + 'px',
            'height': height + 'px',
            'position': 'fixed',
            'left': '0px',
            'top': '0px',
            'margin-left': '0px',
            'margin-top': '0px',
            'display': 'block',
            '-webkit-transform': 'none',
            'transform': 'none',
            'transform-origin': 'center center'
        });

        $("#lcd").css({
            'height': height + 'px',
            'width': width + 'px',
            '-webkit-transform': 'none',
            'transform': 'none',
            'transform-origin': 'left top',
            'position': 'absolute',
            'left': '0px',
            'top': '0px',
            'margin-left': '0px',
            'margin-top': '0px'
        });
        lcdRotateMode = 0;

        // 横屏模式下禁用用户缩放
        updateViewportScale(1.0, false);
    }

    // 更新滤镜层大小
    $("#filterOverlay").css({
        'width': '100%',
        'height': '100%'
    });
}

// 更新视口缩放设置
function updateViewportScale(scale, enableZoom) {
    var viewport = document.getElementById('viewport');
    if (!viewport) {
        viewport = document.querySelector('meta[name="viewport"]');
    }

    if (viewport) {
        if (enableZoom) {
            // 横屏模式：viewport 固定为 1.0，通过 CSS transform 实现缩放
            // 但是禁用用户手动缩放，因为我们用双指手势控制
            viewport.setAttribute('content',
                'width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no');
        } else {
            // 竖屏模式：禁用缩放
            viewport.setAttribute('content',
                'width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no, minimal-ui');
        }
    }
}

