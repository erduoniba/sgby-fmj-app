// System interface for the game
let canvas = null;
let ctx = null;
let keyDownListeners = [];
let keyUpListeners = [];
let intervalHandlers = [];

function sysDrawScreen(buffer, width, height) {
    if (!canvas) {
        canvas = document.createElement('canvas');
        canvas.width = width;
        canvas.height = height;
        document.body.appendChild(canvas);
        ctx = canvas.getContext('2d');
    }
    
    const imageData = new ImageData(new Uint8ClampedArray(buffer), width, height);
    ctx.putImageData(imageData, 0, 0);
}

function sysSetInterval(delay, handler) {
    const id = setInterval(handler, delay);
    intervalHandlers.push(id);
    return id;
}

function sysClearInterval(id) {
    clearInterval(id);
    const index = intervalHandlers.indexOf(id);
    if (index > -1) {
        intervalHandlers.splice(index, 1);
    }
}

;(function(global) {
    var gbkDecoder = new TextDecoder('GBK');
    var gbkEncoder = new TextEncoder('GBK', { NONSTANDARD_allowLegacyEncoding: true });

    var KEY_UP = 1
    var KEY_DOWN = 2
    var KEY_LEFT = 3
    var KEY_RIGHT = 4
    var KEY_PAGEUP = 5
    var KEY_PAGEDOWN = 6
    var KEY_ENTER = 7
    var KEY_CANCEL = 8
    var KEY_REPEAT = 9  // 添加重复上回合的按键常量

    function transCode(k) {
        switch (k) {
            case 13:
                return KEY_ENTER;
            case 32:
                return KEY_CANCEL;
            case 27:
                return KEY_CANCEL;
            case 38:
                return KEY_UP;
            case 40:
                return KEY_DOWN;
            case 37:
                return KEY_LEFT;
            case 39:
                return KEY_RIGHT;
            case 219:
                return KEY_PAGEUP;
            case 221:
                return KEY_PAGEDOWN;
            case 82:  // R键
                return 9;  // KEY_REPEAT
            default:
                return 255;
        }
    };

    function getLCD() {
        var canvas = document.getElementById('lcd');
        var ctx = canvas.getContext('2d');
        return ctx;
    }

    function imagePixel(img, i, color)
    {
        img.data[i] = color.r;
        img.data[i+1] = color.g;
        img.data[i+2] = color.b;
        img.data[i+3] = color.a;
    }

    function imageDot(img, x, y, lineSize, color)
    {
        var ind = lineSize*y + x;
        imagePixel(img, ind*4, color);
    }

    function getStorage(path) {
        if (path.startsWith("sav/")) {
            return window.localStorage
        } else {
            return fmj.rom;
        }
    }

    global.sysStorageGet = function(path) {
        return getStorage(path)[path];
    };

    global.sysStorageSet = function(path, value) {
        // iOS WebView桥接
        if (window.webkit && window.webkit.messageHandlers && window.webkit.messageHandlers.sysStorageSet) {
            window.webkit.messageHandlers.sysStorageSet.postMessage({ path, value });
        }
        // Flutter WebView桥接
        else if (window.flutter_inappwebview && window.flutter_inappwebview.callHandler) {
            window.flutter_inappwebview.callHandler('sysStorageSet', { path, value });
        }
        // 或者使用postMessage方式
        else if (window.parent && window.parent !== window) {
            window.parent.postMessage({ 
                type: 'sysStorageSet', 
                data: { path, value } 
            }, '*');
        }
        
        return getStorage(path)[path] = value;
    };

    global.sysStorageHas = function(path) {
        return getStorage(path)[path] != null;
    };

    global.sysGbkEncode = function(str) {
        if (!str) return new Uint8Array(0);
        
        try {
            return gbkEncoder.encode(str);
        } catch (error) {
            // Replace invalid characters and try again
            var cleaned = str.replace(/[\uFFFD\u0000-\u001F\u007F-\u009F\uD800-\uDFFF]/g, '?');
            if (!cleaned) return new Uint8Array(0);
            
            try {
                return gbkEncoder.encode(cleaned);
            } catch (e) {
                // Final fallback: ASCII only
                var ascii = str.replace(/[^\x20-\x7E]/g, '?');
                return ascii ? gbkEncoder.encode(ascii) : new Uint8Array(0);
            }
        }
    };

    global.sysGbkDecode = function(data) {
        return gbkDecoder.decode(new Int8Array(data));
    };

    global.sysRandom = Math.random;

    function call(f, a) {
        try {
            f(a);
        } catch (e) {
            throw new Error(e + "\n" + e.stack);
        }
    }

    global.sysAddKeyDownListener = function(callback) {
        $('body').keydown(function(e){
            call(callback, transCode(e.keyCode));
        });
    };

    global.sysAddKeyUpListener = function(callback) {
        $('body').keyup(function(e){
            call(callback, transCode(e.keyCode));
        });
    };

    global.sysSetInterval = function(interval, callback) {
        clearInterval(fmj.updateInterval);
        
        // 速度控制变量
        var speedMultiple = 1.0;
        var lastUpdateTime = 0;
        
        fmj.updateInterval = setInterval(function(){
            // 动态获取速度倍数
            if (typeof window !== 'undefined' && typeof window.gameSpeedMultiple === 'number') {
                var newSpeed = window.gameSpeedMultiple;
                if (newSpeed > 0.2 && newSpeed < 5.0) {
                    speedMultiple = newSpeed;
                }
            }
            
            var currentTime = Date.now();
            var expectedInterval = interval / speedMultiple;
            
            // 根据速度倍数控制执行频率
            if (currentTime - lastUpdateTime >= expectedInterval) {
                lastUpdateTime = currentTime;
                call(callback);
            }
        }, Math.min(interval / 5, 16)); // 使用较小的基础间隔确保精确控制
        
        return fmj.updateInterval;
    };

    global.sysDrawScreen = function(buffer, wid, hgt) {
        var lcd = getLCD();
        var w = wid;
        var h = hgt;

        var img = lcd.createImageData(wid, hgt);

        for (var y = 0; y < h; y += 1) {
            for (var x = 0; x < w; x += 1) {
                var ind = w*y + x;
                var pixel = buffer[ind];
                imageDot(img, x, y, w, pixel);
            }
        }
        lcd.imageSmoothingEnabled = false;
        lcd.putImageData(img, 0, 0);
    };

    global.sysExit = function() {
        console.log("Exit");
    };

    // 获取存档槽数量，从window变量中读取
    global.sysGetSaveSlotCount = function() {
        // 优先从 window.fmjSaveSlotCount 读取
        if (typeof window.fmjSaveSlotCount === 'number' && window.fmjSaveSlotCount > 0) {
            return window.fmjSaveSlotCount;
        }
        // 后备方案，从其他可能的变量名读取
        if (typeof window.saveSlotCount === 'number' && window.saveSlotCount > 0) {
            return window.saveSlotCount;
        }
        // 默认返回5个存档槽
        return 5;
    };

    // 游戏配置相关的系统函数
    global.sysGetWinMoneyMultiple = function() {
        return window.winMoneyMultiple || 1;
    };

    global.sysGetWinExpMultiple = function() {
        return window.winExpMultiple || 1;
    };

    global.sysGetWinItemMultiple = function() {
        return window.winItemMultiple || 1;
    };

    global.sysGetCombatProbability = function() {
        return window.combat_probability || 0;
    };

    global.sysGetMapContainerState = function() {
        return window.mapContainerState || false;
    };

    global.sysGetMagicReverse = function() {
        return window.magicReverse || false;
    };

    global.sysGetChoiceLibName = function() {
        return window.choiceLibName || 'FMJ';
    };

    // 外部回调函数
    global.sysShowMapBase64 = function(data) {
        if (typeof window !== 'undefined' && window.showMapBase64) {
            window.showMapBase64(data);
        }
    };

    global.sysUpdatePlayerPosition = function(x, y, mapX, mapY, mapWidth, mapHeight) {
        if (typeof window !== 'undefined' && window.updatePlayerPosition) {
            window.updatePlayerPosition(x, y, mapX, mapY, mapWidth, mapHeight);
        }
    };

    global.sysUpdateTreasureBoxes = function(treasureBoxesJson, mapWidth, mapHeight) {
        if (typeof window !== 'undefined' && window.updateTreasureBoxes) {
            try {
                window.updateTreasureBoxes(JSON.parse(treasureBoxesJson), mapWidth, mapHeight);
            } catch (e) {
                console.warn('Failed to update treasure boxes:', e);
            }
        }
    };

    global.sysInitializeFMJDevTools = function(gameInstance) {
        if (typeof window !== 'undefined' && window.FMJDevTools) {
            try {
                window.FMJDevTools.initialize(gameInstance);
            } catch (e) {
                console.warn('Failed to initialize FMJDevTools:', e);
            }
        }
    };
    
    // 宝箱映射Cookie管理函数
    global.sysInitBoxMapping = function() {
        if (window.boxMapping && window.boxMapping.init) {
            window.boxMapping.init();
        }
    };
    
    global.sysAddBoxMapping = function(boxKey, eventId) {
        if (window.boxMapping && window.boxMapping.add) {
            window.boxMapping.add(boxKey, eventId);
        }
    };
    
    global.sysGetBoxEventId = function(boxKey) {
        if (window.boxMapping && window.boxMapping.getEventId) {
            return window.boxMapping.getEventId(boxKey);
        }
        return null;
    };
    
    global.sysGetAllBoxMappings = function() {
        if (window.boxMapping && window.boxMapping.getAll) {
            return window.boxMapping.getAll();
        }
        return {};
    };
    
    global.sysLoadBoxMappingsIntoKotlin = function(callback) {
        if (window.boxMapping && window.boxMapping.getAll) {
            var mappings = window.boxMapping.getAll();
            for (var key in mappings) {
                if (mappings.hasOwnProperty(key)) {
                    callback(key, mappings[key]);
                }
            }
        }
    };

    global.fmj = {rom: {}};
})(this);


window.onerror = function(msg, url, line, col, error) {
    clearInterval(fmj.updateInterval);
    alert(msg + " at " + line);
    return false;
};

function enableDebug() {
    var core = window['fmj.core'].fmj
    function foreachPlayer(f) {
        var players = core.game.playerList.toArray();
        for (var i = 0; i < players.length; i++) {
            f(players[i]);
        }
    }
    $('body').keydown(function(e){
        switch(e.key) {
            case "f": {
                core.combat.Combat.Companion.ForceWin();
                console.log("Forced win");
                break;
            }
            case "d": {
                core.combat.Combat.Companion.globalDisableFighting_0 = true;
                console.log("disabled random fight");
                break;
            }
            case "e": {
                core.combat.Combat.Companion.globalDisableFighting_0 = false;
                console.log("enabled random fight");
                break;
            }
            case "1": {
                foreachPlayer(function(p) {
                    p.hp = 999;
                });
                break;
            }
            case "2": {
                foreachPlayer(function(p) {
                    p.mp = 999;
                });
                break;
            }
            case "3": {
                foreachPlayer(function(p) {
                    p.debuff.reset();
                });
                break;
            }
        }
    });

    window.events = core.script.ScriptResources.globalEvents;
    window.core = core;
    window.printGoods = function() {
        var e = core.lib.DatLib.ResType;
        for (var type = 1; type <= 14; type++) {
            for (var index = 1; index < 200; index++) {
                var goods = core.lib.DatLib.Companion.getRes_2et8c9$(e.GRS, type, index, true);
                if (goods == null) {
                    break;
                }
                console.log("type=" + type + " index=" + index + " " + goods.name_dx74sj$_0 + " " + goods.description_yw9j1y$_0);
            }
        }
    };
}
