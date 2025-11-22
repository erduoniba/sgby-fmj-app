
// 全局错误处理函数，捕获并显示JavaScript运行时错误
window.onerror = function(msg, url, line, col, error) {
   var extra = !col ? '' : '\ncolumn: ' + col;
   extra += !error ? '' : '\nerror: ' + error;
   alert("Error: " + msg + "\nurl: " + url + "\nline: " + line + extra);
   return false;
};

// 定义全局随机数生成函数，如果未定义则使用默认值0.5
if (window._bayeRand === undefined) {
    window._bayeRand = function() {
        return 0.5;
    };
}

// 重写Math.random函数，使用游戏引擎提供的随机数生成器
// 将结果限制在0-1之间，保持与标准Math.random行为一致
Math.random = function() {
    try {
        return _bayeRand() % 65536 / 65536;
    } catch {
        return 0;
    }
};

// 为String对象添加format方法，支持类似Python的字符串格式化
// 示例: "Hello {0}".format("World") => "Hello World"
if (!String.prototype.format) {
  String.prototype.format = function() {
    var args = arguments;
    return this.replace(/{(\d+)}/g, function(match, number) {
      return typeof args[number] != 'undefined'
        ? args[number]
        : match
      ;
    });
  };
}

// 为Array对象添加includes方法的polyfill
// 用于检查数组是否包含指定元素
if (!Array.prototype.includes) {
    Array.prototype.includes = function(searchElement, fromIndex) {
        return this.indexOf(searchElement, fromIndex) >= 0;
    }
}

// 游戏速度控制相关变量
var gameSpeedMultipleValue = 1.0;    // 游戏速度倍数值，默认1倍速
var originalSetInterval = null; // 保存原始的setInterval函数

// 定义游戏引擎中使用的数据类型常量
var    ValueTypeU8 = 0;        // 8位无符号整数
var    ValueTypeU16 = 1;       // 16位无符号整数
var    ValueTypeU32 = 2;       // 32位无符号整数
var    ValueTypeString = 3;     // 字符串类型
var    ValueTypeObject = 4;     // 对象类型
var    ValueTypeArray = 5;      // 数组类型
var    ValueTypeMethod = 6;     // 方法类型
var    ValueTypeGBKBuffer = 7;  // GBK编码的字符缓冲区

// 创建GBK编码解码器，用于处理中文字符
var gbkDecoder = new TextDecoder('GBK');
var gbkEncoder = new TextEncoder('GBK', { NONSTANDARD_allowLegacyEncoding: true });

// BayeObject类：游戏引擎与JavaScript之间的数据桥接基类
function BayeObject() {
}

// 重写toString方法，根据不同的数据类型返回对应的字符串表示
BayeObject.prototype.toString = function() {
    switch (this._type) {
    case ValueTypeU8:
        return 'U8(' + this.value + ')';
    case ValueTypeU16:
        return 'U16(' + this.value + ')';
    case ValueTypeU32:
        return 'U32(' + this.value + ')';
    case ValueTypeString:
        return 'String("' + this.value + '")';
    case ValueTypeObject:
        return 'Object';
    case ValueTypeArray:
        return 'Array[' + this.length + ']';
    case ValueTypeGBKBuffer:
        return 'GBKBuffer[' + this.length + ']';
    case ValueTypeMethod:
        return 'Method';
    }
};

// 将游戏引擎中的值转换为JavaScript对象
function baye_bridge_value(value) {
    return baye_bridge_valuedef(_Value_get_def(value), _Value_get_addr(value));
}

// 根据值的类型创建对应的属性描述器
// 用于定义对象的getter和setter，实现数据双向绑定
function baye_bridge_description_for_value(jvalue, type) {
    switch (type) {
        case ValueTypeU8:
        case ValueTypeU16:
        case ValueTypeU32:
        case ValueTypeString:
            return {
                get: function() {
                    return jvalue.value.value;
                },
                set: function(value) {
                    jvalue.value.value = value;
                }
            };
            break;
        case ValueTypeArray:
            return {
                get: function() {
                    return jvalue.value;
                },
                set: function(value) {
                    var jv = jvalue.value;
                    var length = jv.length;
                    for(var i = 0; i < length && i < value.length; i++) {
                        jv[i] = value[i]
                    }
                }
            };
            break;
        case ValueTypeGBKBuffer:
            return {
                get: function() {
                    var buffer = bayeU8Array(jvalue.value._addr, _bayeStrLen(jvalue.value._addr));
                    return gbkDecoder.decode(buffer);
                },
                set: function(value) {
                    var jv = jvalue.value;
                    var arr = gbkEncoder.encode('' + value);
                    var length = Math.min(jv.length - 1, arr.length);
                    for(var i = 0; i < length; i++) {
                        jv[i] = arr[i]
                    }
                    jv[length] = 0;
                }
            };
            break;
        case ValueTypeObject:
            return {
                get: function() {
                    return jvalue.value;
                },
                set: function(value) {
                    jvalue.value._baye_properties.forEach( name => {
                        if (value[name] !== undefined) {
                            jvalue.value[name] = value[name];
                        }
                    });
                }
            }
            break;
    }
}

// 封装Object.defineProperty，用于定义单个属性
function defineProperty(obj, p, desc) {
    Object.defineProperty(obj, p, desc);
}

// 封装Object.defineProperties，用于批量定义属性
function defineProperties(obj, desc) {
    Object.defineProperties(obj, desc);
}

// 创建延迟加载的值对象
// 只有在首次访问value属性时才会创建实际的对象
function baye_bridge_valuedef_lazy(def, addr) {
    var obj = {
        get value() {
            if (this._value == undefined) {
                this._value = baye_bridge_valuedef(def, addr);
            }
            return this._value;
        }
    };
    return obj;
}

// 核心的数据类型转换函数
// 将游戏引擎中的数据定义和地址转换为JavaScript对象
// def: 数据类型定义
// addr: 内存地址
function baye_bridge_valuedef(def, addr) {
    var type = _ValueDef_get_type(def);
    var jsObj = new BayeObject();
    jsObj._def = def;
    jsObj._addr = addr;
    jsObj._type = type;

    switch (type) {
        case ValueTypeU8:
            defineProperty(jsObj, 'value', {
                get: function() {
                    return _baye_get_u8_value(this._addr);
                },
                set: function(value) {
                    if (value > 0xff) value = 0xff;
                    if (value < 0) value = 0;
                    return _baye_set_u8_value(this._addr, value);
                }
            });
            break;
        case ValueTypeU16:
            defineProperty(jsObj, 'value', {
                get: function() {
                    return _baye_get_u16_value(this._addr);
                },
                set: function(value) {
                    if (value > 0xffff) value = 0xffff;
                    if (value < 0) value = 0;
                    return _baye_set_u16_value(this._addr, value);
                }
            });
            break;
        case ValueTypeU32:
            defineProperty(jsObj, 'value', {
                get: function() {
                    return _baye_get_u32_value(this._addr);
                },
                set: function(value) {
                    return _baye_set_u32_value(this._addr, value);
                }
            });
            break;
        case ValueTypeString:
            defineProperty(jsObj, 'value', {
                get: function() {
                    // TODO:
                    return this._addr;
                }
            });
            break;
        case ValueTypeObject:
            return baye_bridge_obj(def, addr);
        case ValueTypeArray:
        case ValueTypeGBKBuffer:
            var length = _ValueDef_get_array_length(def);
            jsObj.length = length;
            var subdef = _ValueDef_get_array_subdef(def);
            var subsize = _ValueDef_get_size(subdef);
            var properties = {};
            for (var i = 0; i < length; i++) {
                var item_value = baye_bridge_valuedef_lazy(subdef, addr + subsize * i);
                var desc = baye_bridge_description_for_value(item_value, _ValueDef_get_type(subdef));
                properties[i] = desc;
            }
            defineProperties(jsObj, properties);
            break;
        case ValueTypeMethod:
            return function() {
            };
    }
    return jsObj;
}

function baye_bridge_obj(def, addr) {
    var jsObj = new BayeObject();
    var properties = {};

    var count = _ValueDef_get_field_count(def);
    var property_names = [];


    jsObj._def = def;
    jsObj._addr = addr;

    for (var i = 0; i < count; i++) {
        var field = _ValueDef_get_field_by_index(def, i);
        var cname = _Field_get_name(field);
        var name = UTF8ToString(cname);
        var field_value_addr = _Field_get_value(field);
        var value_def = _Value_get_def(field_value_addr);
        var value_offset = _Value_get_addr(field_value_addr);
        var field_value = baye_bridge_valuedef_lazy(value_def, addr + value_offset);

        var desc = baye_bridge_description_for_value(field_value, _Field_get_type(field));
        properties[name] = desc;
        property_names.push(name);
    }
    defineProperties(jsObj, properties);
    jsObj._baye_properties = property_names;
    return jsObj;
}

function bayeU8Array(caddr, length) {
    // 兼容新版本 Emscripten，使用 wasmMemory 而不是 Module.HEAPU8
    if (typeof wasmMemory !== 'undefined' && wasmMemory.buffer) {
        return new Uint8Array(wasmMemory.buffer, caddr, length);
    } else if (Module.HEAPU8) {
        return Module.HEAPU8.subarray(caddr, caddr+length);
    } else {
        console.error('无法访问 WebAssembly 内存');
        return new Uint8Array(0);
    }
}

function bayeWrapFunctionS(innerf) {
    return function() {
        var addr = innerf.apply(this, arguments);
        if (addr != 0) {
            return new TextDecoder('GBK').decode(bayeU8Array(addr, _bayeStrLen(addr)));
        }
        return null;
    };
}

function range(start, stop, step) {
    if (typeof stop == 'undefined') {
        // one param defined
        stop = start;
        start = 0;
    }

    if (typeof step == 'undefined') {
        step = 1;
    }

    if ((step > 0 && start >= stop) || (step < 0 && start <= stop)) {
        return [];
    }

    var result = [];
    for (var i = start; step > 0 ? i < stop : i > stop; i += step) {
        result.push(i);
    }

    return result;
}

function baye_bridge_init() {
    /**
     * 简单的Promise实现，用于处理异步操作
     * @param {Function} fn 执行器函数，接收resolve和reject两个参数
     */
    function Promise(fn) {
        _this = this
        var done = false;
        fn(function(x){
            if (typeof _this.onfulfilled == 'function') {
                if (!done) {
                    done = true
                    _this.onfulfilled(x)
                }
            }
        }, function(e){
            if (typeof _this.onrejected == "function") {
                if (!done) {
                    done = true
                    _this.onrejected(e)
                }
            }
        });
    }

    /**
     * 添加Promise成功和失败的回调函数
     * @param {Function} onfulfilled 成功时的回调函数
     * @param {Function} onrejected 失败时的回调函数
     */
    Promise.prototype.then = function(onfulfilled, onrejected) {
        this.onfulfilled = onfulfilled
        this.onrejected = onrejected
    };

    /**
     * 将普通函数包装成返回Promise的异步函数
     * @param {Function} f 要包装的函数
     * @returns {Function} 返回一个新函数，调用时返回Promise
     */
    function wrapAsync(f) {
        return function() {
            var args = Array.from(arguments);
            var _this = this;
            return Promise(function(resolve, reject){
                args.push(resolve);
                f.apply(_this, args);
            });
        }
    }

    /**
     * 截断字符串到指定长度，支持中文字符
     * @param {string} s 要截断的字符串
     * @param {number} n 目标长度
     * @param {boolean} pad 是否用空格填充到目标长度
     * @returns {string} 截断后的字符串
     */
    function truncate(s, n, pad) {
        var l = 0;
        var c = 0;
        for (var i = 0; i < s.length; i++) {
            var cs = s.charCodeAt(i) > 256 ? 2 : 1;
            if (l + cs <= n) {
                l += cs;
                c += 1;
            } else {
                break;
            }
        }
        var rv = s.slice(0, c);
        if (pad) {
            while (l < n) {
                rv += ' ';
                l += 1;
            }
        }
        return rv;
    }

    window.Promise = Promise;

    if (window.baye === undefined) {
        window.baye = {};
    }
    baye.debug = {};
    
    // 内存管理辅助函数 - 统一的内存分配和释放接口
    baye.memory = {
        // 统计分配的内存块数量
        allocatedBlocks: 0,
        
        // 安全分配内存
        alloc: function(size) {
            if (typeof Module._bayeAlloc === 'function') {
                var buffer = Module._bayeAlloc(size);
                if (buffer !== 0) {
                    this.allocatedBlocks++;
                    console.debug('Memory allocated:', buffer, 'size:', size, 'total blocks:', this.allocatedBlocks);
                }
                return buffer;
            } else {
                console.error('Module._bayeAlloc not available');
                return 0;
            }
        },
        
        // 安全释放内存
        free: function(buffer) {
            if (buffer === 0) return true;
            
            if (typeof Module._free === 'function') {
                Module._free(buffer);
                this.allocatedBlocks--;
                console.debug('Memory freed:', buffer, 'remaining blocks:', this.allocatedBlocks);
                return true;
            } else if (typeof _free === 'function') {
                _free(buffer);
                this.allocatedBlocks--;
                console.debug('Memory freed via global _free:', buffer, 'remaining blocks:', this.allocatedBlocks);
                return true;
            } else {
                console.warn('Memory free function not available, potential memory leak for buffer:', buffer);
                return false;
            }
        },
        
        // 获取内存使用统计
        getStats: function() {
            return {
                allocatedBlocks: this.allocatedBlocks
            };
        }
    };


    /**
     * 获取指定索引的人物名称
     * @param {number} index 人物索引
     * @returns {string} 人物名称
     */
    baye.getPersonName = bayeWrapFunctionS(_bayeGetPersonName);

    /**
     * 获取指定索引的道具名称
     * @param {number} index 道具索引
     * @returns {string} 道具名称
     */
    baye.getToolName = bayeWrapFunctionS(_bayeGetToolName);

    /**
     * 获取指定索引的技能名称
     * @param {number} index 技能索引
     * @returns {string} 技能名称
     */
    baye.getSkillName = bayeWrapFunctionS(_bayeGetSkillName);

    /**
     * 获取指定索引的城市名称
     * @param {number} index 城市索引
     * @returns {string} 城市名称
     */
    baye.getCityName = bayeWrapFunctionS(_bayeGetCityName);

    /**
     * 获取游戏中的人物总数
     * @returns {number} 人物总数
     */
    baye.getPersonCount = _bayeGetPersonCount;

    baye._cbs = [];

    /**
     * 添加异步操作的回调函数到回调栈
     * @param {Function} cb 回调函数，如果为空则使用默认回调
     */
    baye.pushCallback = function(cb) {
        if (baye.data.g_asyncActionID > 0)
            baye._cbs.push(cb ? cb : function(x){ return x; });
    }

    /**
     * 执行并移除回调栈顶部的回调函数
     * @returns {*} 回调函数的返回值
     */
    baye.callCallback = function() {
        var rv = baye._cbs.pop()();
        baye.pushCallback(baye.callback);
        return rv;
    }

    /**
     * 执行指定名称的钩子函数
     * @param {string} name 钩子函数名称
     * @param {*} context 钩子函数的上下文
     * @returns {*} 钩子函数的返回值
     */
    baye.callHook = function(name, context) {
        var rv = window.baye.hooks[name](context);
        baye.pushCallback(baye.callback);
        return rv;
    };

    /**
     * 获取自定义数据
     * @returns {string|null} 自定义数据字符串，如果没有则返回null
     */
    baye.getCustomData = function() {
        var cstr = _bayeGetCustomData();
        if (cstr == 0) return null;
        return UTF8ToString(cstr);
    }

    /**
     * 设置自定义数据
     * @param {string} data 要保存的自定义数据
     */
    baye.setCustomData = function(data) {
        var length = lengthBytesUTF8(data) + 1;
        var buffer = baye.memory.alloc(length);
        if (buffer === 0) {
            console.error('Failed to allocate memory for setCustomData');
            return;
        }
        
        stringToUTF8(data, buffer, length);
        _bayeSetCustomData(buffer);
        
        // 释放内存
        if (!baye.memory.free(buffer)) {
            console.warn('Failed to free memory in setCustomData, potential memory leak');
        }
    }

    /**
     * 显示警告对话框
     * @param {string} msg 要显示的消息
     * @param {Function} then 对话框关闭后的回调函数
     */
    baye.alert = function(msg, then){
        baye.data.g_asyncActionID = 1;
        baye.data.g_asyncActionParams[0] = 3;
        baye.data.g_asyncActionStringParam = msg;
        baye.callback = then;
    };

    /**
     * Promise风格的alert函数
     * @param {string} msg 要显示的消息
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncAlert = wrapAsync(baye.alert);

    /**
     * 显示人物对话
     * @param {number} personIndex 说话的人物索引
     * @param {string} msg 对话内容
     * @param {Function} then 对话结束后的回调函数
     */
    baye.say = function(personIndex, msg, then){
        baye.data.g_asyncActionID = 2;
        baye.data.g_asyncActionParams[0] = personIndex;
        baye.data.g_asyncActionStringParam = msg;
        baye.callback = then;
    };

    /**
     * Promise风格的say函数
     * @param {number} personIndex 说话的人物索引
     * @param {string} msg 对话内容
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncSay = wrapAsync(baye.say);

    /**
     * 游戏延时函数
     * @param {number} ticks 延时的时钟周期数
     * @param {number} flag 延时标志
     * @param {Function} then 延时结束后的回调函数
     */
    baye.delay = function(ticks, flag, then){
        baye.data.g_asyncActionID = 7;
        baye.data.g_asyncActionParams[0] = ticks;
        baye.data.g_asyncActionParams[1] = flag;
        setcb0(then);
    };

    /**
     * Promise风格的delay函数
     * @param {number} ticks 延时的时钟周期数
     * @param {number} flag 延时标志
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncDelay = wrapAsync(baye.delay);

    /**
     * 播放特效动画
     * @param {number} x 特效显示的X坐标
     * @param {number} y 特效显示的Y坐标
     * @param {number} speid 特效ID
     * @param {number} index 特效索引
     * @param {number} flag 特效标志
     * @param {Function} then 特效播放完成后的回调函数
     */
    baye.playSPE = function(x, y, speid, index, flag, then){
        baye.data.g_asyncActionID = 8;
        baye.data.g_asyncActionParams[0] = speid;
        baye.data.g_asyncActionParams[1] = index;
        baye.data.g_asyncActionParams[2] = x;
        baye.data.g_asyncActionParams[3] = y;
        baye.data.g_asyncActionParams[4] = flag;
        setcb0(then);
    };

    /**
     * Promise风格的playSPE函数
     * @param {number} x 特效显示的X坐标
     * @param {number} y 特效显示的Y坐标
     * @param {number} speid 特效ID
     * @param {number} index 特效索引
     * @param {number} flag 特效标志
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncPlaySPE = wrapAsync(baye.playSPE);

    /**
     * 获取数值输入
     * @param {number} min 最小值
     * @param {number} max 最大值
     * @param {Function} then 输入完成后的回调函数
     */
    baye.getNumber = function(min, max, then) {
        baye.data.g_asyncActionID = 9;
        baye.data.g_asyncActionParams[0] = min;
        baye.data.g_asyncActionParams[1] = max;
        baye.data.g_asyncActionParams[2] = max;
        setcb0(then);
    };

    /**
     * Promise风格的getNumber函数
     * @param {number} min 最小值
     * @param {number} max 最大值
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncGetNumber = wrapAsync(baye.getNumber);

    /**
     * 获取数值输入（带初始值）
     * @param {number} init 初始值
     * @param {number} min 最小值
     * @param {number} max 最大值
     * @param {Function} then 输入完成后的回调函数
     */
    baye.getNumber2 = function(init, min, max, then) {
        baye.data.g_asyncActionID = 9;
        baye.data.g_asyncActionParams[0] = min;
        baye.data.g_asyncActionParams[1] = max;
        baye.data.g_asyncActionParams[2] = init;
        setcb0(then);
    };

    /**
     * Promise风格的getNumber2函数
     * @param {number} init 初始值
     * @param {number} min 最小值
     * @param {number} max 最大值
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncGetNumber2 = wrapAsync(baye.getNumber2);

    /**
     * 进入战斗场景
     * @param {Function} then 战斗结束后的回调函数
     */
    baye.enterBattle = function(then) {
        baye.data.g_asyncActionID = 10;
        setcb0(then);
    };

    /**
     * Promise风格的enterBattle函数
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncEnterBattle = wrapAsync(baye.enterBattle);

    function setcb0(then) {
        if (then) {
            baye.callback = function() {
                return then(baye.data.g_asyncActionParams[0]);
            };
        } else {
            baye.callback = undefined;
        }
    }

    /**
     * 显示选择菜单
     * @param {number} x 菜单显示的X坐标
     * @param {number} y 菜单显示的Y坐标
     * @param {number} w 菜单宽度
     * @param {number} h 菜单高度
     * @param {string[]} items 选项列表
     * @param {number} init 初始选中项
     * @param {Function} then 选择完成后的回调函数
     */
    baye.choose = function(x, y, w, h, items, init, then){
        baye.data.g_asyncActionID = 3;
        baye.data.g_asyncActionParams[0] = x;
        baye.data.g_asyncActionParams[1] = y;
        baye.data.g_asyncActionParams[2] = w;
        baye.data.g_asyncActionParams[3] = h;
        baye.data.g_asyncActionParams[4] = init;

        var n = Math.floor(w / 6);
        var s = "";

        for (var i = 0; i < items.length; i++) {
            s += truncate(items[i], n, true);
        }

        baye.data.g_asyncActionStringParam = s;
        setcb0(then);
    };

    /**
     * Promise风格的choose函数
     * @param {number} x 菜单显示的X坐标
     * @param {number} y 菜单显示的Y坐标
     * @param {number} w 菜单宽度
     * @param {number} h 菜单高度
     * @param {string[]} items 选项列表
     * @param {number} init 初始选中项
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncChoose = wrapAsync(baye.choose);

    /**
     * 在屏幕中央显示选择菜单
     * @param {number} w 菜单宽度
     * @param {number} h 菜单高度
     * @param {string[]} items 选项列表
     * @param {number} init 初始选中项
     * @param {Function} then 选择完成后的回调函数
     */
    baye.centerChoose = function(w, h, items, init, then) {
        var x = (baye.data.g_screenWidth - w) / 2;
        var y = (baye.data.g_screenHeight - h) / 2;

        return baye.choose(x, y, w, h, items, init, then);
    };

    /**
     * Promise风格的centerChoose函数
     * @param {number} w 菜单宽度
     * @param {number} h 菜单高度
     * @param {string[]} items 选项列表
     * @param {number} init 初始选中项
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncCenterChoose = wrapAsync(baye.centerChoose);

    /**
     * 显示人物选择界面
     * @param {number[]} items 可选人物索引列表
     * @param {number} init 初始选中的人物索引
     * @param {Function} then 选择完成后的回调函数
     */
    baye.choosePerson = function(items, init, then) {
        baye.data.g_asyncActionID = 4;
        baye.data.g_asyncActionParams[0] = items.length;
        baye.data.g_asyncActionParams[1] = init;

        for (var i = 0; i < items.length; i++) {
            baye.data.g_asyncActionU16ParamArray[i] = items[i];
        }
        setcb0(then);
    };

    /**
     * Promise风格的choosePerson函数
     * @param {number[]} items 可选人物索引列表
     * @param {number} init 初始选中的人物索引
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncChoosePerson = wrapAsync(baye.choosePerson);

    /**
     * 显示道具选择界面
     * @param {number[]} items 可选道具索引列表
     * @param {number} init 初始选中的道具索引
     * @param {Function} then 选择完成后的回调函数
     */
    baye.chooseTool = function(items, init, then) {
        baye.data.g_asyncActionID = 5;
        baye.data.g_asyncActionParams[0] = items.length;
        baye.data.g_asyncActionParams[1] = init;

        for (var i = 0; i < items.length; i++) {
            baye.data.g_asyncActionU16ParamArray[i] = items[i];
        }
        setcb0(then);
    };

    /**
     * Promise风格的chooseTool函数
     * @param {number[]} items 可选道具索引列表
     * @param {number} init 初始选中的道具索引
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncChooseTool = wrapAsync(baye.chooseTool);

    /**
     * 显示城市选择界面
     * @param {Function} then 选择完成后的回调函数
     */
    baye.chooseCity = function(then) {
        baye.data.g_asyncActionID = 6;
        setcb0(then);
    };

    /**
     * Promise风格的chooseCity函数
     * @returns {Promise} 返回Promise对象
     */
    baye.asyncChooseCity = wrapAsync(baye.chooseCity);

    /**
     * 创建战斗场景
     * @param {number} city 战斗发生的城市索引
     * @param {Function} then 战斗创建完成后的回调函数
     */
    baye.makeBattle = function(city, then) {
        baye.data.g_asyncActionParams[0] = city;
        baye.data.g_asyncActionID = 11;
        setcb0(then);
    };

    /**
     * 执行城市指令
     * @param {number} city 目标城市索引
     * @param {number} cmd 要执行的指令
     * @param {Function} then 指令执行完成后的回调函数
     */
    baye.makeCommand = function(city, cmd, then) {
        baye.data.g_asyncActionParams[0] = city;
        baye.data.g_asyncActionParams[1] = cmd;
        baye.data.g_asyncActionID = 12;
        setcb0(then);
    };

    // 处理系统消息
    // then: 消息处理完成的回调函数
    // 返回消息对象，包含类型、参数等信息
    baye.sysMessage = function(then){
        baye.data.g_asyncActionID = 13;
        setcb0(function() {
            var msg = {
                "type": baye.data.g_asyncActionParams[0],
                "param": baye.data.g_asyncActionParams[1],
                "param2": {
                    "i32": baye.data.g_asyncActionParams[2],
                    "i16": {
                        "p0": baye.data.g_asyncActionParams[3],
                        "p1": baye.data.g_asyncActionParams[4],
                    }
                }
            };
            return then(msg);
        });
    };

    // 根据人物名称获取人物对象
    // name: 人物名称
    // 返回: 找到的人物对象，未找到返回undefined
    baye.getPersonByName = function(name) {
        var all = baye.data.g_Persons;
        for (var i = 0; i < all.length; i++) {
            if (baye.getPersonName(i) == name) {
                return all[i];
            }
        }
    };

    // 根据人物名称获取人物ID
    // name: 人物名称
    // 返回: 找到的人物ID，未找到返回undefined
    baye.getPersonIdByName = function(name) {
        var all = baye.data.g_Persons;
        for (var i = 0; i < all.length; i++) {
            if (baye.getPersonName(i) == name) {
                return i;
            }
        }
    };


    // 根据人物查找所在城市
    // person: 人物ID
    // 返回: 人物所在的城市ID，未找到返回0xff
    baye.getCityByPerson = function(person) {
        const cities = baye.data.g_Cities;
        for (var c = 0; c < cities.length; c++) {
            const j = cities[c].PersonQueue;
            for (var i = 0; i < cities[c].Persons; i++) {
                if (baye.data.g_PersonsQueue[j+i] == person)
                    return c;
            }
        }
        return 0xff;
    }


    baye.getCityByName = function(name) {
        var all = baye.data.g_Cities;
        for (var i = 0; i < all.length; i++) {
            if (baye.getCityName(i) == name) {
                return all[i];
            }
        }
    };

    baye.getFighterIndexByName = function(name) {
        var all = baye.data.g_FgtParam.GenArray;
        for (var i = 0; i < all.length; i++) {
            var index = all[i] - 1;
            if (index >= 0 && baye.getPersonName(index) == name) {
                return i;
            }
        }
    };

    baye.getFighterPositionByName = function(name) {
        var idx = baye.getFighterIndexByName(name);
        return baye.data.g_GenPos[idx];
    };

    baye.getPersonNameByID = function(id) {
        switch (id) {
        case 0:
            return "-";
        case 0xff:
            return "俘虏";
        default:
            return baye.getPersonName(id - 1);
        }
    };

    baye.printCity = function(i) {
        var city = baye.data.g_Cities[i];
        var people = baye.data.g_Persons;
        var queue = baye.data.g_PersonsQueue;

        var belong = baye.getPersonNameByID(city.Belong);

        console.log("--------" + baye.getCityName(i) + "--------");
        console.log("id: " + i);
        console.log("归属: " + belong);
        console.log("-");
        for (var qi = 0; qi  < city.Persons; qi++) {
            var pind = queue[city.PersonQueue + qi];
            var person = people[pind];
            var name = baye.getPersonName(pind);
            var belong = baye.getPersonNameByID(person.Belong);
            console.log(sprintf("%-10s 归属:%-10s", name, belong));
        }
        console.log("-");
        var queue = baye.data.g_GoodsQueue;
        var tools = baye.data.g_Tools;
        for (var qi = 0; qi  < city.Tools; qi++) {
            var tindex = queue[city.ToolQueue + qi];
            var tool = tools[pind];
            var name = baye.getToolName(pind);
            console.log(name);
        }
    };

    baye.printPeople = function () {
        for (var i = 0; i < 250; i++) {
            var p = baye.data.g_Persons[i];
            if (p.Level > 0) {
                console.log(sprintf('index: %03d name: %-08s 归属:%-08s', i, baye.getPersonName(i), baye.getPersonNameByID(p.Belong)));
            }
        }
    };

    baye.printAllCities = function() {
        var cities = baye.data.g_Cities;

        for (var i = 0; i < cities.length; i++) {
            baye.printCity(i);
        }
    };

    baye.getTerrainByGeneralIndex = function(index) {
        return _bayeFgtGetGenTer(index);
    };

    baye.putPersonInCity = function(city, person) {
        return _bayePutPersonInCity(city, person);
    };

    baye.putToolInCity = function(city, tool, hide) {
        return _bayePutToolInCity(city, tool, hide ? 1 : 0);
    };

    baye.deletePersonInCity = function(city, person) {
        return _bayeDeletePersonInCity(city, person);
    };

    baye.deleteToolInCity = function(city, tool) {
        return _bayeDeleteToolInCity(city, tool);
    };

    baye.getPersonByGeneralIndex = function(gIndex) {
        var pid = baye.data.g_FgtParam.GenArray[gIndex];
        return baye.data.g_Persons[pid - 1];
    };

    baye.moveHere = function(name) {
        var i = baye.getFighterIndexByName(name);
        var pd = baye.data.g_GenPos[i];
        pd.x = baye.data.g_FoucsX;
        pd.y = baye.data.g_FoucsY;
    };

    baye.getArmType = function(pindex) {
        return _bayeGetArmType(pindex);
    };

    baye.drawText = function (x, y, text, scr) {
        var gbkPtr = _bayeGetGBKBuffer();
        baye.data.g_asyncActionStringParam = text;
        return _bayeLcdDrawText(gbkPtr, x, y, scr);
    };

    baye.drawImage = function(x, y, resid, resitem, picIndex, scr) {
        _bayeLcdDrawImage(resid, resitem, picIndex, x, y, scr == 1 ? 0 : 1);
    };

    baye.clearRect = function(left, top, right, bottom, scr) {
        _bayeLcdClearRect(left, top, right, bottom, scr);
    };

    baye.revertRect = function(left, top, right, bottom, scr) {
        _bayeLcdRevertRect(left, top, right, bottom, scr);
    };

    baye.drawLine = function(startX, startY, endX, endY) {
        _bayeLcdDrawLine(startX, startY, endX, endY, 1);
    };

    baye.drawRect = function(left, top, right, bottom, scr) {
        _bayeLcdDrawRect(left, top, right, bottom, 1, scr);
    };

    baye.drawDot = function(x, y, color) {
        _bayeLcdDot(x, y, color);
    };

    baye.clearScreen = function() {
        baye.clearRect(0, 0, baye.data.g_screenWidth, baye.data.g_screenHeight);
    };

    baye.resizeScreen = function(width, height) {
        bayeResizeScreen(width, height);
    };

    baye.patchNames = function() {
        var l = baye.data.g_Persons.length;
        for (var i = 0; i < l; i++) {
            baye.data.g_Persons[i].name = baye.getPersonName(i);
        }

        l = baye.data.g_Tools.length;
        for (var i = 0; i < l; i++) {
            baye.data.g_Tools[i].name = baye.getToolName(i);
        }

        l = baye.data.g_Skills.length;
        for (var i = 0; i < l; i++) {
            baye.data.g_Skills[i].name = baye.getSkillName(i);
        }
    };

    baye.saveScreen = _bayeSaveScreen;

    baye.restoreScreen = _bayeRestoreScreen;
    baye.setFont = _bayeSetFont;
    baye.setFontEn = _bayeSetFontEn;
    baye.clearFontCache = _bayeClearFontCache;

    //计算将领在屏幕的像素位置
    baye.getFighterXY = function (index) {
        var ox = baye.data.g_GenPos[index].x - baye.data.g_MapSX;
        var oy = baye.data.g_GenPos[index].y - baye.data.g_MapSY;
        return {
            x: ox * 16,
            y: oy * 16
        };
    };
    baye.loadPeriod = _bayeLoadPeriod;
    baye.sendKey = _bayeSendKey;

    baye.None = 0xffff;
    baye.OK = 0;

    baye.VM_KEY =       0x05;		/* 按键 */
    baye.VM_TIMER =     0x06;		/* 定时到 */
    baye.VM_TOUCH =     0x10;		/* 触控事件 */

    baye.VK_PGUP =      0x20;
    baye.VK_PGDN =      0x21;
    baye.VK_UP =        0x22;
    baye.VK_DOWN =      0x23;
    baye.VK_LEFT =      0x24;
    baye.VK_RIGHT =     0x25;
    baye.VK_HELP =      0x26;
    baye.VK_ENTER =     0x27;
    baye.VK_EXIT =      0x28;
    baye.VK_INSERT =    0x30;
    baye.VK_DEL =       0x31;
    baye.VK_MODIFY =    0x32;
    baye.VK_SEARCH =    0x33;

    baye.VT_TOUCH_DOWN =    0x01;
    baye.VT_TOUCH_UP =      0x02;
    baye.VT_TOUCH_MOVE =    0x03;
    baye.VT_TOUCH_CANCEL =  0x04;

    baye.blurScreen = lcdBlur;

    // for debug
    baye.debug = {};

    baye.debug.pa = function () {
        for (var i = 0; i < 20; i++) {
            var id = baye.data.g_FgtParam.GenArray[i];
            if (id) {
                console.log('' + i + ':' + baye.getPersonName(id-1));
            }
        }
    };

    baye.debug.reset = function () {
        baye.data.g_LookMovie = 0;
        for (var i = 0; i < 10; i++) {
            var id = baye.data.g_FgtParam.GenArray[i];
            if (id) {
                baye.data.g_GenPos[i].active = 0;
                baye.data.g_GenPos[i].hp = 100;
                baye.data.g_GenPos[i].mp = 100;
                baye.data.g_Persons[id-1].Arms = 10000;
            }
        }
    };

    // 调试, 移动指定任务到跟中来
    baye.debug.mv = function (i) {
        var pd = baye.data.g_GenPos[i];
        pd.x = baye.data.g_FoucsX;
        pd.y = baye.data.g_FoucsY;
    };

    // 游戏速度控制API
    baye.setGameSpeed = function(multiple) {
        if (typeof multiple !== 'number' || multiple <= 0) {
            console.warn('游戏速度倍数必须是大于0的数字');
            return false;
        }
        
        // 限制速度范围，避免极端值影响游戏体验
        if (multiple > 5.0) {
            multiple = 5.0;
        } else if (multiple < 0.2) {
            multiple = 0.2;
        }
        
        gameSpeedMultipleValue = multiple;
        
        // 调用 C 语言的速度控制函数
        if (typeof Module !== 'undefined' && Module._bayeSetSpeed) {
            Module._bayeSetSpeed(multiple);
            console.log('游戏速度已设置为: ' + multiple + '倍');
        } else {
            console.warn('WebAssembly 速度控制函数不可用，请重新编译');
        }
        
        return true;
    };

    baye.getGameSpeed = function() {
        // 如果 C 函数可用，从 C 获取速度
        if (typeof Module !== 'undefined' && Module._bayeGetSpeed) {
            gameSpeedMultipleValue = Module._bayeGetSpeed();
        }
        return gameSpeedMultipleValue;
    };

    // 暴露全局API
    window.gameSpeedMultiple = function(multiple) {
        return baye.setGameSpeed(multiple);
    };
    
    window.getGameSpeed = function() {
        return baye.getGameSpeed();
    };
    
    // 保护速度控制函数不被覆盖
    try {
        Object.defineProperty(window, 'gameSpeedMultiple', {
            value: function(multiple) {
                return baye.setGameSpeed(multiple);
            },
            writable: false,
            configurable: false
        });
    } catch(e) {
        // 如果定义失败，使用备用方案
        console.warn('无法保护 gameSpeedMultiple 函数:', e);
    }
    
    // 添加备用的速度控制函数
    window.setGameSpeed = window.setGameSpeed || function(multiple) {
        return baye.setGameSpeed(multiple);
    };
    
    // 确保 Module 对象上也有速度控制函数
    if (typeof Module !== 'undefined') {
        Module.setGameSpeed = function(multiple) {
            return baye.setGameSpeed(multiple);
        };
        Module.getGameSpeed = function() {
            return baye.getGameSpeed();
        };
    }
    
    // 游戏显示方向控制API
    // 保存原始的reloadLCD函数
    if (typeof window.originalReloadLCD === 'undefined' && typeof reloadLCD === 'function') {
        window.originalReloadLCD = reloadLCD;
        console.log('已保存原始reloadLCD函数');
    }
    
    // 霸业游戏分辨率设置API
    window.setBayeResolution = function(resolution) {
        console.log('setBayeResolution被调用，分辨率：' + resolution);
        
        // 保存分辨率设置到localStorage
        localStorage.setItem('baye/resolution', resolution);
        
        try {
            // 根据分辨率值设置游戏画面尺寸
            var width, height;
            switch (resolution) {
                case '0':
                    width = 16 * 10;  // 160px - 词典分辨率
                    height = 16 * 6;  // 96px
                    console.log('设置为词典分辨率: ' + width + 'x' + height);
                    break;
                case '1':
                    width = 16 * 13;  // 208px - 高清分辨率
                    height = 16 * 8;  // 128px
                    console.log('设置为高清分辨率: ' + width + 'x' + height);
                    break;
                default:
                    width = 16 * 10;
                    height = 16 * 6;
                    console.log('使用默认词典分辨率: ' + width + 'x' + height);
                    break;
            }
            
            // 调用游戏引擎的分辨率设置函数
            if (typeof bayeResizeScreen === 'function') {
                bayeResizeScreen(width, height);
                console.log('bayeResizeScreen调用成功');
            } else {
                console.warn('bayeResizeScreen函数不可用');
            }
            
            // 如果当前已有reloadLCD函数，重新调用以应用新的分辨率
            if (typeof reloadLCD === 'function') {
                reloadLCD();
                console.log('reloadLCD调用成功，应用新分辨率');
            }
            
            return true;
            
        } catch (e) {
            console.error('setBayeResolution设置失败:', e);
            return false;
        }
    };
    
    // 获取当前分辨率设置的API
    window.getBayeResolution = function() {
        return localStorage.getItem('baye/resolution') || '0';
    };
    
    // 霸业游戏方向设置API
    window.setBayeOrientation = function(orientation) {
        console.log('setBayeOrientation被调用，方向：' + orientation);
        
        // 保存用户设置的方向到localStorage
        localStorage.setItem('bayeDisplayOrientation', orientation);
        
        try {
            // 重写reloadLCD函数
            window.reloadLCD = function() {
                var userOrientation = localStorage.getItem('bayeDisplayOrientation') || 'portrait';
                console.log('自定义reloadLCD执行，用户设置方向：' + userOrientation);
                
                var height = window.innerHeight;
                var width = window.innerWidth;
                
                if (userOrientation === 'landscape') {
                    // 横屏模式：使用原有的横屏逻辑
                    console.log('执行横屏模式布局');
                    if (window.originalReloadLCD) {
                        window.originalReloadLCD.call(this);
                    } else {
                        // 如果原始函数不可用，执行默认横屏逻辑
                        $("#app").css({
                            'width': width + 'px',
                            'height': height + 'px'
                        });

                        if (height > width) {
                            console.log('竖屏状态 - 横屏模式');
                            $("#lcd").css({
                                'height': width + 'px',
                                'width': height + 'px',
                                'transform-origin': 'left top',
                                'transform': 'rotate(-90deg) translateX(-' + height + 'px)',
                                'position': 'relative',
                                'left': '0px',
                                'top': '0px',
                                'margin-left': '0px',
                                'margin-top': '0px'
                            });
                            window.lcdRotateMode = 2;
                        } else {
                            console.log('横屏状态 - 横屏模式');

                            // 计算保持 160:96 比例的尺寸
                            var gameRatio = 160 / 96;
                            var appWidth, appHeight;

                            if (height * gameRatio <= width) {
                                appHeight = height;
                                appWidth = appHeight * gameRatio;
                            } else {
                                appWidth = width;
                                appHeight = appWidth / gameRatio;
                            }

                            // 对app容器设置尺寸，不应用缩放
                            $("#app").css({
                                'width': appWidth + 'px',
                                'height': appHeight + 'px',
                                'position': 'fixed',
                                'left': '50%',
                                'top': '50%',
                                'margin-left': (-appWidth / 2) + 'px',
                                'margin-top': (-appHeight / 2) + 'px',
                                'display': 'block',
                                '-webkit-transform': 'none',
                                'transform': 'none',
                                'transform-origin': 'center center'
                            });

                            $("#lcd").css({
                                'height': appHeight + 'px',
                                'width': appWidth + 'px',
                                '-webkit-transform': 'none',
                                'transform': 'none',
                                'transform-origin': 'left top',
                                'position': 'absolute',
                                'left': '0px',
                                'top': '0px',
                                'margin-left': '0px',
                                'margin-top': '0px'
                            });
                            window.lcdRotateMode = 0;
                        }
                    }
                } else {
                    // 竖屏模式：自定义布局逻辑
                    console.log('执行竖屏模式布局');
                    
                    // 更新app容器大小
                    $("#app").css({
                        'width': width + 'px',
                        'height': height + 'px'
                    });
                    
                    // 计算游戏画面尺寸，保持160:96比例
                    var gameAspectRatio = 160 / 96; // ≈ 1.67
                    var maxGameWidth = width;
                    var maxGameHeight = height * 0.4; // 使用屏幕高度的40%
                    
                    var gameWidth, gameHeight;
                    
                    // 按宽度计算高度
                    gameWidth = maxGameWidth;
                    gameHeight = gameWidth / gameAspectRatio;
                    
                    // 如果高度超出限制，则按高度计算宽度
                    if (gameHeight > maxGameHeight) {
                        gameHeight = maxGameHeight;
                        gameWidth = gameHeight * gameAspectRatio;
                    }
                    
                    console.log('计算得到的游戏尺寸：' + gameWidth + 'x' + gameHeight);
                    
                    // 设置app容器：位于屏幕上方，水平居中
                    $("#app").css({
                        'width': gameWidth + 'px',
                        'height': gameHeight + 'px',
                        'position': 'relative',
                        'left': '50%',
                        'top': '0px',
                        'margin-left': '-' + (gameWidth/2) + 'px',
                        'margin-top': '0px',
                        'display': 'block'
                    });
                    
                    // 设置LCD：填满app容器，无旋转
                    $("#lcd").css({
                        'width': gameWidth + 'px',
                        'height': gameHeight + 'px',
                        'transform-origin': 'left top',
                        'transform': 'rotate(0deg)',
                        '-webkit-transform': 'rotate(0deg)',
                        'position': 'relative',
                        'left': '0px',
                        'top': '0px',
                        'margin-left': '0px',
                        'margin-top': '0px'
                    });
                    
                    // 更新滤镜层
                    $("#filterOverlay").css({
                        'width': '100%',
                        'height': '100%'
                    });
                    
                    // 设置为无旋转模式
                    window.lcdRotateMode = 0;
                    console.log('竖屏模式设置完成，lcdRotateMode:', window.lcdRotateMode);
                }
                
                // 更新滤镜层大小（通用处理）
                $("#filterOverlay").css({
                    'width': '100%',
                    'height': '100%'
                });
            };
            
            // 立即执行一次reloadLCD（如果页面元素已存在）
            if (document.getElementById('app') && document.getElementById('lcd')) {
                reloadLCD();
            }
            
            console.log('setBayeOrientation设置完成');
            return true;
            
        } catch (e) {
            console.error('setBayeOrientation设置失败:', e);
            return false;
        }
    };
}

