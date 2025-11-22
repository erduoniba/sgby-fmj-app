var lcdWidth = 16*10;
var lcdHeight = 16*6;
var dotSize = 4;

function getLCD() {
    var canvas = document.getElementById('lcd');
    if (canvas.getContext === undefined) {
        alert("你的浏览器不支持HTML5");
    }
    var ctx = canvas.getContext('2d');
    return ctx;
}

function lcdBlur(blur) {
    var lcd = getLCD();
    if (blur) {
        lcd.imageSmoothingEnabled = true;
        $('#lcd').removeClass('no_blur')
    } else {
        lcd.imageSmoothingEnabled = false;
        $('#lcd').addClass('no_blur')
    }
}

function lcdInit()
{
    var width = 16*10;
    var height = 16*6;

    //分辨率
    switch (window.localStorage["baye/resolution"]) {
    case '0':
        width = 16*10;
        height = 16*6;
        break;
    case '1':
        width = 16*13;
        height = 16*8;
        break;
    }

    bayeResizeScreen(width, height);

    if (window.localStorage["baye/debug"] == '1') {
        _bayeSetDebug(1);
    }
    lcdBlur(false);
    baye_bridge_init();
}

function bayeResizeScreen(width, height) {
    lcdWidth = width;
    lcdHeight = height;
    var canvas = document.getElementById('lcd');
    canvas.width = width * dotSize;
    canvas.height = height * dotSize
    _bayeSetLcdSize(lcdWidth, lcdHeight);
}

function imagePixel(img, i)
{
    img.data[i] = 0;
    img.data[i+1] = 0;
    img.data[i+2] = 0;
    img.data[i+3] = 255;
}

function imageDot(img, x, y, lineSize)
{
    var ind = lineSize*y + x;
    imagePixel(img, ind*4);
}

function lcdSetDotSize(s)
{
    var canvas = document.getElementById('lcd');
    dotSize = s;
    canvas.width = lcdWidth * dotSize;
    canvas.height = lcdHeight * dotSize
}

function lcdFlushBuffer(buffer) {
    var lcd = getLCD();
    var w = lcdWidth*dotSize;
    var h = lcdHeight*dotSize

    var buffer_wrp = new Uint8ClampedArray(wasmMemory.buffer, buffer, w*h*4);
    var img = new ImageData(buffer_wrp, w, h);
    lcd.putImageData(img, 0, 0);
}

function sendKey(key) {
    if('undefined' === typeof Module || 'undefined' === typeof Module.asm){
        return;
	}
    _bayeSendKey(key);
}

var 		VK_PGUP =			0x20;
var 		VK_PGDN	=			0x21;
var 		VK_UP	=			0x22;
var 		VK_DOWN	=			0x23;
var 		VK_LEFT	=			0x24;
var 		VK_RIGHT=			0x25;
var 		VK_HELP	=			0x26;
var 		VK_ENTER=			0x27;
var 		VK_EXIT	=			0x28;

var 		VK_INSERT	=		0x30;
var 		VK_DEL		=		0x31;
var 		VK_MODIFY	=		0x32;
var 		VK_SEARCH	=		0x33;

function onKeyDown(e) {
    var event = e?e:window.event;

    switch (event.keyCode) {
        case 13:
            sendKey(VK_ENTER);
            break;
        case 72:
            sendKey(VK_HELP);
            break;
        case 70:
            sendKey(VK_SEARCH);
            break;
        case 83:
            sendKey(VK_SEARCH);
            break;
        case 32:
            sendKey(VK_EXIT);
            break;
        case 27:
            sendKey(VK_EXIT);
            break;
        case 38:
            sendKey(VK_UP);
            break;
        case 40:
            sendKey(VK_DOWN);
            break;
        case 37:
            sendKey(VK_LEFT);
            break;
        case 39:
            sendKey(VK_RIGHT);
            break;
    }
}

function bin2hex (s) {

  var i, l, o = "", n;

  s += "";

  for (i = 0, l = s.length; i < l; i++) {
    n = s.charCodeAt(i).toString(16)
    o += n.length < 2 ? "0" + n : n;
  }

  return o;
}

function binarray2hex (arr) {

  var i, l, o = "", n;

  for (i = 0, l = arr.length; i < l; i++) {
    n = arr[i].toString(16)
    o += n.length < 2 ? "0" + n : n;
  }

  return o;
}

function clearLib(then) {
    window.localStorage.removeItem('baye//data/dat.lib');
    window.localStorage.removeItem('baye/libname');
    window.localStorage.removeItem('baye/libpath');
    libCacheClear(then);
}

function getLibName() {
    return window.localStorage['baye/libname'] || "Unnamed";
}

if (typeof(Storage) === "undefined") {
    alert("你的浏览器不支持存档");
}


var layoutType = 0;
var keypadWidth = 250;


function layoutKeyboard() {
    var w = window.innerWidth
    var h = window.innerHeight

    if (h / w > lcdHeight/lcdWidth) {
        var availableHeight = h - w * lcdHeight/lcdWidth;
        var isCompatLayout = (layoutType == 1 || layoutType == 2);

        var kbWidth = isCompatLayout ? keypadWidth : w;

        var ratio = availableHeight / 3 / kbWidth * 100;
        if (ratio > 30) {
            ratio = 30;
        }

        $(".dummy30").css("margin-top", ratio + "%");
        $(".keypad").removeAttr("style");

        if (isCompatLayout) {
            $(".keypad").css("width", keypadWidth);
            if (layoutType == 1) {
                $(".keypad").css("float", 'right');
            } else {
                $(".keypad").css("float", 'left');
            }
        }

        if (layoutType == 3) {
            $("#keypad1").hide();
            $("#keypad2").show();
        } else {
            $("#keypad2").hide();
            $("#keypad1").show();
        }
    }
}

function saveKeyboardLayout() {
    window.localStorage['baye.kb.layout'] = String(layoutType);
}

function loadKeyboardLayout() {
    layoutType = parseInt(window.localStorage['baye.kb.layout']);
    if (!layoutType) {
        layoutType = 3;
    }
    layoutKeyboard();
}

function switchLayout() {
    layoutType += 1;
    layoutType %= 4;
    saveKeyboardLayout();
    layoutKeyboard();
}

loadKeyboardLayout();

String.prototype.format = function(args) {
    var result = this;
    if (arguments.length > 0) {
        if (arguments.length == 1 && typeof (args) == "object") {
            for (var key in args) {
                if(args[key]!=undefined){
                    var reg = new RegExp("({" + key + "})", "g");
                    result = result.replace(reg, args[key]);
                }
            }
        }
        else {
            for (var i = 0; i < arguments.length; i++) {
                if (arguments[i] != undefined) {
                    var reg = new RegExp("({[" + i + "]})", "g");
                    result = result.replace(reg, arguments[i]);
                }
            }
        }
    }
    return result;
}

function ajaxGet(path, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', path, true);
    xhr.responseType = 'blob';

    xhr.onload = function(e) {
        if (this.status == 200 || this.status == 0) {
        var blob = this.response;
        callback(blob);
      }
    };

    xhr.send();
}

var dynLib = null;

function libCacheClear(then) {
    // 清除内存中的lib数据
    dynLib = null;
    // 清除IndexedDB中的lib数据
    var store = new IdbKvStore('baye');
    store.remove('lib', then);
}

function libCacheSet(data, then) {
    var store = new IdbKvStore('baye');
    store.set('lib', data, then);
}

function libCacheGet(ok, err) {
    var store = new IdbKvStore('baye')
    store.get('lib', function (e, value) {
        if (e) {
            console.log("no DB");
            err();
        } else if (value) {
            console.log(`lib found in DB, size=${value.length}`);
            dynLib = bin2hex(value);
            ok();
        } else {
            console.log("no lib in DB");
            err();
        }
    })
}

function loadLibFromUrl(url, then) {
    console.log("trying to load from DB");
    libCacheGet(then, () => {
        console.log("loading from " + url);
        ajaxGet(url, function(file){
            console.log("ajax ok");
            var reader = new FileReader();
            reader.onload = function() {
                libCacheSet(reader.result);
                dynLib = bin2hex(reader.result);
                console.log("read ok");
                then();
            }
            reader.readAsBinaryString(file);
        });
    });
}

function loadLib(files) {
    var reader = new FileReader();
    reader.onload = function() {
        libCacheSet(reader.result, () => {
            window.localStorage['baye/libname'] = '自定义';
            window.localStorage['baye/libpath'] = undefined;
            window.location.reload();
        });
    }
    reader.readAsBinaryString(files[0]);
}

function loadLibDefault(then) {
    var url = window.localStorage['baye/libpath'];
    if (!url) {
        alert("没有选择版本");
    } else {
        loadLibFromUrl(url, function(){
            then();
        });
    }
}

function bayeMain() {
    // 检查是否是三国霸业原版
    var libpath = window.localStorage['baye/libpath'];
    var libname = window.localStorage['baye/libname'];
    
    // 如果是三国霸业词典原版，根据分辨率选择不同的lib文件
    if (libname === '三国霸业-词典原版' || libpath === 'libs/dat-v2-mod.lib' || libpath === 'libs/dat-mod.lib') {
        var resolution = window.localStorage['baye/resolution'] || '0';
        var newLibPath;
        
        // 根据分辨率选择lib文件
        if (resolution === '1') {
            // 高清模式，使用v2版本
            newLibPath = 'libs/dat-v2-mod.lib';
            console.log('使用高清版本: libs/dat-v2-mod.lib');
        } else {
            // 原版模式，使用标准版本
            newLibPath = 'libs/dat-mod.lib';
            console.log('使用原版分辨率: libs/dat-mod.lib');
        }
        
        // 如果lib路径发生了变化，需要清除缓存
        if (libpath !== newLibPath) {
            console.log('检测到分辨率切换，清除lib缓存...');
            // 清除缓存的lib数据
            window.localStorage.removeItem('baye//data/dat.lib');
            // 清除IndexedDB中的缓存
            libCacheClear(function() {
                console.log('lib缓存已清除');
                // 更新lib路径
                window.localStorage['baye/libpath'] = newLibPath;
                // 重新加载lib
                loadLibDefault(_main);
            });
        } else {
            // lib路径没有变化，直接加载
            window.localStorage['baye/libpath'] = newLibPath;
            loadLibDefault(_main);
        }
    } else {
        // 非三国霸业原版，直接加载
        loadLibDefault(_main);
    }
}

function chooseLib(title, path, self_) {
    var self = $(self_);
    self.html("请稍候...");
    self.attr("disabled", "disabled");

    clearLib(() => {
		//http direct
		if (0 === path.indexOf('http')){
			location.href=path;
			return;
		}

        if (path && path.length > 0) {
            window.localStorage['baye/libname'] = title;
            window.localStorage['baye/libpath'] = path;
            // iOS WebView桥接
            if (window.webkit && window.webkit.messageHandlers && window.webkit.messageHandlers.chooseLib) {
                window.webkit.messageHandlers.chooseLib.postMessage({ path });
            }
            // Flutter WebView桥接
            else if (window.flutter_inappwebview && window.flutter_inappwebview.callHandler) {
                window.flutter_inappwebview.callHandler('chooseLib', { path });
            }
        }
        redirect();
    });
}


function loadDetail(id, path) {
    var e = $(id);
    if (e.is(":hidden")) {
        if (e.attr("data-loaded")) {
            e.show();
        } else {
            e.show();
            $.get(path, {}, function(text) {
                e.html(text.replace(/(?:\r\n|\r|\n)/g, '<br />'));
                e.attr("data-loaded", "1");
            }).fail(function(req){
                if (req.status == 404) {
                    e.html("待补充");
                    e.attr("data-loaded", "1");
                }
                else if (req.status == 0) {
                    e.html(req.responseText.replace(/(?:\r\n|\r|\n)/g, '<br />'));
                    e.attr("data-loaded", "1");
                }
                else {
                    e.html("错误: " + req.status);
                }
                e.show();
            });
        }
    } else {
        e.hide();
    }
}

function generateHtml(json, container) {
    var tpl = $("#item_temp").html();
    let html = "";
  
    for (let i in json) {
      html += tpl.format({
        title: json[i]["title"],
        libpath: json[i]["path"],
        descid: i,
        descpath: json[i]["desc"] ? json[i]["desc"] : json[i]["path"] + '.about'
      });
    }
    $(container).html(html);
  }
function loadLibLists(container) {
    $.ajax({
         type:"GET",
         url:"libs.json?ver=2023111705",
         dataType:"json",
     }).success(function(json) {
        generateHtml(json, container);
    }).error(function(xhr, status, error) {
        var errorMsg = "加载版本列表失败";
        if (xhr.status === 404) {
            errorMsg += ": 找不到版本文件";
        } else if (xhr.status === 0) {
            const json = JSON.parse(xhr.responseText)
            generateHtml(json, container);
            return;
        } else {
            errorMsg += ": " + error;
        }
        $(container).html('<div class="error">' + errorMsg + '</div>');
    });
}

function redirect(page) {
    var uiKind = window.localStorage['baye/uiKind'];

    switch (uiKind) {
        case "mobile":
            isMobile = true;
            break;
        case "desktop":
            isMobile = false;
            break;
        default:
            isMobile = "detect";
            break;
    }

    if (isMobile == "detect") {
        isMobile = navigator.userAgent.match(/(iPhone|iPod|Android|ios|Mobile|ARM)/i);
    }

    if(isMobile){
        var defaultMPage = "m.html";
        switch (window.localStorage['baye/mpage']) {
        case '0':
            defaultMPage = "m.html"
            break;
        case '1':
            defaultMPage = "m-old.html"
            break;
        case '2':
            defaultMPage = "m-ges.html"
            break;
        case '3':
            defaultMPage = "m-ktouch.html"
            break;
        }
        console.log('page:' + page);
        console.log('defpage:' + defaultMPage);
        page = page || defaultMPage;
    } else {
        page = "pc.html";
    }
    var now = new Date().getTime() / 1000;
    var name = getLibName();
    var hash = isMobile ? "#" + now : "";
    window.location.href = page + "?name=" + name + hash;
}

function goHome() {
    window.location.href = "index.html";
}

function disablePageScroll() {
    document.body.addEventListener('touchmove', function(event) {
        event.preventDefault();
    }, {
        passive: false,
        capture: false
    });
    window.onscroll = function() {  window.scrollTo(0, 0); }
}

function touchPadInit(elementID) {
    var activeTouch = null;
    var originX = 0;
    var originY = 0;
    var lastX = 0;
    var lastY = 0;
    var touchMoved = false;
    var xMoved = 0;
    var yMoved = 0;
    var previousMovingTime = 0;
    var previousX = 0;
    var previousY = 0;
//    var normalBackgroundColor = "#fff";
    function convertTouch(touch) {
        switch (lcdRotateMode) {
        case 0:
            break;
        case 1:
            return {
                x: touch.screenY,
                y: window.screen.width-touch.screenX,
            };
        case 2:
            return {
                x: window.screen.height-touch.screenY,
                y: touch.screenX,
            };
        }
        return {
            x: touch.screenX,
            y: touch.screenY,
        };
    }

    function raiseKey(key) {
        sendKey(key);
    }

    function resetTouch() {
        activeTouch = null;
        touchMoved = false;
        xMoved = 0;
        yMoved = 0;
        previousX = 0;
        previousY = 0;
        // todo: reset color
    }

    function touchBegan(event) {
        if (activeTouch || event.targetTouches.length < 1) {
            return;
        }
        activeTouch = event.targetTouches[0];
        previousMovingTime = event.timeStamp;
        var touch = convertTouch(activeTouch);
        previousX = lastX = originX = touch.x;
        previousY = lastY = originY = touch.y;
        touchMoved = false;
        // todo: color
    }

    function find(touches, touch) {
         for (var i in touches) {
            if (touch.identifier == touches[i].identifier) {
                return touches[i];
            }
         }
         return null;
    }

    function touchEnded(event) {
        if (activeTouch && find(event.changedTouches, activeTouch)) {
            resetTouch();
        }
    }

    function processPointMove(point,
                        previousPoint,
                        previousStayPoint,
                        dT,
                        stepMax,
                        stepMin,
                        speedMax,
                        speedThreshold,
                        vkUp,
                        vkDown)
    {
        var speed = (point - previousPoint) / dT;
        var dP = point - previousStayPoint;


        speed = Math.abs(speed);
        if (speed > speedThreshold) {
            speed -= speedThreshold;
        }
        else {
            speed = 0;
        }

        speed = Math.pow(speed, 3);
        speedMax = Math.pow(speedMax, 3);
        speed = Math.min(speed, speedMax);

        var step = stepMax - speed / speedMax * (stepMax - stepMin);
        var count = Math.floor(Math.abs(dP) / step);
        if (count > 0) {
            for (var i = 0; i < count; i++) {
                raiseKey( dP < 0 ? vkUp : vkDown);
            }
            return point;
        }
        return previousStayPoint;
    }

    function touchMove(event) {
        if (activeTouch) {
            var newTouch = find(event.changedTouches, activeTouch);
            if (!newTouch) {
                return;
            }

            var touch = convertTouch(newTouch);
            var x = touch.x;
            var y = touch.y;

            var dt = event.timeStamp - previousMovingTime;
            previousMovingTime = event.timeStamp;

            var dX = x - previousX;
            var dY = y - previousY;
            var speedX = dX / dt;
            var speedY = dX / dt;
            var ratio = Math.abs(speedX / speedY);

            lastY = processPointMove(y, previousY, lastY, dt, 30, 0.3, 2000, 800, VK_UP, VK_DOWN);
            lastX = processPointMove(x, previousX, lastX, dt, 30, 8, 1000, 500, VK_LEFT, VK_RIGHT);

            previousX = x;
            previousY = y;

            touchMoved = true;
        }
    }

    var element = document.getElementById(elementID);

    element.addEventListener("touchstart", touchBegan);
    element.addEventListener("touchmove", touchMove);
    element.addEventListener("touchend", touchEnded);
    element.addEventListener("touchcancel", touchEnded);
    disablePageScroll();
}

var lcdRotateMode = 0;

function touchScreenInit(lcdID) {
    var lcd = document.getElementById(lcdID);

    var activeTouch = null;

    var VT_TOUCH_DOWN = 1
    var VT_TOUCH_UP = 2
    var VT_TOUCH_MOVE = 3
    var VT_TOUCH_CANCEL = 4

    function raiseTouchEvent(key, touch) {
        var rect = lcd.getBoundingClientRect();

        var webX = touch.clientX - rect.left;
        var webY = touch.clientY - rect.top;

        var gameX = webX / rect.width * lcdWidth;
        var gameY = webY / rect.height * lcdHeight;

        switch (lcdRotateMode) {
        case 0:
            break;
        case 1:
            gameX = webY / rect.height * lcdWidth;
            gameY = (rect.width - webX) / rect.width * lcdHeight;
            break;
        case 2:
            gameX = (rect.height - webY) / rect.height * lcdWidth;
            gameY = webX / rect.width * lcdHeight;
            break;
        }
        if (window.bayeDebugMode) {
            var html = "";
            html += sprintf('canvas:(%.1f,%.1f)<br>', rect.left, rect.top);
            html += sprintf('client:(%.1f,%.1f)<br>', touch.clientX, touch.clientY);
            html += sprintf('web:(%.1f,%.1f)<br>', webX, webY);
            html += sprintf('game:(%.1f,%.1f)<br>', gameX, gameY);
            $('#info').html(html);
        }
        _bayeSendTouchEvent(key, gameX, gameY);
    }

    function resetTouch() {
        activeTouch = null;
    }

    function touchBegan(event) {
        if (activeTouch || event.targetTouches.length < 1) {
            return;
        }
        activeTouch = event.targetTouches[0];
        raiseTouchEvent(VT_TOUCH_DOWN, activeTouch);
    }

    function find(touches, touch) {
         for (var i in touches) {
            if (touch.identifier == touches[i].identifier) {
                return touches[i];
            }
         }
         return null;
    }

    function touchEnded(event) {
        if (activeTouch) {
            var touch = find(event.changedTouches, activeTouch);
            if (!touch) {
                return;
            }
            raiseTouchEvent(VT_TOUCH_UP, touch);
            resetTouch();
        }
    }

    function touchMove(event) {
        if (activeTouch) {
            var touch = find(event.changedTouches, activeTouch);
            if (!touch) {
                return;
            }
            raiseTouchEvent(VT_TOUCH_MOVE, touch);
        }
    }

    function touchCanceled(event) {
        if (activeTouch) {
            var touch = find(event.changedTouches, activeTouch);
            if (!touch) {
                return;
            }
            raiseTouchEvent(VT_TOUCH_CANCEL, touch);
            resetTouch();
        }
    }
    lcd.addEventListener("touchstart", touchBegan);
    lcd.addEventListener("touchmove", touchMove);
    lcd.addEventListener("touchend", touchEnded);
    lcd.addEventListener("touchcancel", touchCanceled);
    disablePageScroll();
}

// --------- Engine callbacks ---------

function bayeFlushLcdBuffer(buffer) {
    lcdFlushBuffer(buffer);
}

function bayeStart() {
    _bayeSetLcdSize(lcdWidth, lcdHeight);
}

function bayeExit() {
    goHome();
}

function bayeLoadFileContent(filename) {
    console.log("Loading " + filename);
    if (filename == 'baye//data/dat.lib') {
        console.log("libsize: " + dynLib.length);
        return dynLib;
    } else {
        var data = window.localStorage[filename];
        if (data
            && data.length > 100
            && window.localStorage[filename + '.lib']
            && window.localStorage[filename + '.lib'] != window.localStorage['baye/libpath']) {
            return data.substring(0, 20);
        }
        return data;
    }
}

function bayeBindTap(selector) {
    function tap(){
        var action = $(this).attr("ontap");
        $(this).bind( "tap", function(e){
            eval(action);
            e.stopPropagation();
        });
    }
    $(selector).each(tap);
}

function bayeSaveFileContent(filename, content) {
    console.log("Saving " + filename);
    window.localStorage[filename + '.lib'] = window.localStorage['baye/libpath'];
    window.localStorage[filename + '.name'] = window.localStorage['baye/libname'];
    window.localStorage[filename] = content;

    // iOS WebView桥接
    if (window.webkit && window.webkit.messageHandlers && window.webkit.messageHandlers.sysStorageSet) {
        window.webkit.messageHandlers.sysStorageSet.postMessage({ path: filename, value: content });
    } 
    // Flutter WebView桥接
    else if (window.flutter_inappwebview && window.flutter_inappwebview.callHandler) {
        window.flutter_inappwebview.callHandler('sysStorageSet', { path: filename, value: content });
    }
    // 或者使用postMessage方式
    else if (window.parent && window.parent !== window) {
        window.parent.postMessage({ 
            type: 'sysStorageSet', 
            data: { path: filename, value: content } 
        }, '*');
    }
}

// 初始化 Module 对象（如果不存在）
if (typeof Module === 'undefined') {
    Module = {};
}

// 配置文件定位函数
Module.locateFile = function(path, prefix) {
    // 处理 WebAssembly 相关文件的路径定位
    if (path.endsWith('.wasm') || path.endsWith('.wasm.map')) {
        // 优先使用 js/ 目录下的文件
        return 'js/' + path;
    }
    // 其他文件使用默认路径
    return prefix + path;
};

Module.noInitialRun = true;

baye = {
    preScriptInit: function() {
        if (baye.data.g_scale > 2) {
            baye.blurScreen(true);
        }
    },
};
