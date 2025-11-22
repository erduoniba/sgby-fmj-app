// 关闭灾害提示
baye.data.g_engineConfig.promptCityDisaster = 0;
// 所有人物禁言
baye.data.g_engineConfig.disableAllPersonReport = 0;
// 关闭开场动画
baye.data.g_engineConfig.showStartMovie = 0;

// 压缩存档中的customData数据 0不压缩，1~9表示压缩等级
baye.data.g_engineConfig.compressCustomData = 5;

// 获取城池坐标
var pos = baye.data.g_CityPositions[0];
console.log(pos.x);
console.log(pos.y);

// 最大战斗天数, 65535 for unlimited
baye.data.g_FgtBoutMax = 10;

// 城池光标移动范围
baye.data.g_cityCursorRange = { top: 2, left: 2, bottom: 4, right: 4 };

// 屏幕前景色色号, 0~255。背景色固定为0号色
baye.data.g_paintColor = 255;

// 禁用战死
baye.data.g_engineConfig.disableFightToDeath = 1;

const BLACK = 255;
const GRAY = 200;

baye.data.theme = {
    landMapColor: GRAY, // 大陆地图颜色
    ownedCityColor: BLACK, // 已拥有城池颜色
    emptyCityColor: BLACK, // 空城池颜色
    otherCityColor: BLACK, // 其它势力城池颜色
    landCursorColor: BLACK, // 大陆地图光标颜色
    battleNoteColor: BLACK, // 战争提示图颜色
    kingHeadColor: BLACK, // 君主头像颜色
    personHeadColor: BLACK, // 人物头像颜色
    fightMoveRangeColor: BLACK, // 战场移动范围颜色
    fightMapColor: BLACK, // 战场地图颜色
};

// 色盘，每个色号对应的32位色, 一共256个色号
baye.data.g_paintPalette = [
    0x00FFFFFF, // 0号色(背景色), 32位数据由透明度和RGB组成
    0x01FEFEFE,
    0x02FDFDFD,
    0x03FCFCFC,
    0x04FBFBFB,
    0x05FAFAFA,
    0x06F9F9F9,
    0x07F8F8F8,
    0x08F7F7F7,
    0x09F6F6F6,
    0x0AF5F5F5,
    0x0BF4F4F4,
    0x0CF3F3F3,
    0x0DF2F2F2,
    0x0EF1F1F1,
    0x0FF0F0F0,
    0x10EFEFEF,
    0x11EEEEEE,
    0x12EDEDED,
    0x13ECECEC,
    0x14EBEBEB,
    0x15EAEAEA,
    0x16E9E9E9,
    0x17E8E8E8,
    0x18E7E7E7,
    0x19E6E6E6,
    0x1AE5E5E5,
    0x1BE4E4E4,
    0x1CE3E3E3,
    0x1DE2E2E2,
    0x1EE1E1E1,
    0x1FE0E0E0,
    0x20DFDFDF,
    0x21DEDEDE,
    0x22DDDDDD,
    0x23DCDCDC,
    0x24DBDBDB,
    0x25DADADA,
    0x26D9D9D9,
    0x27D8D8D8,
    0x28D7D7D7,
    0x29D6D6D6,
    0x2AD5D5D5,
    0x2BD4D4D4,
    0x2CD3D3D3,
    0x2DD2D2D2,
    0x2ED1D1D1,
    0x2FD0D0D0,
    0x30CFCFCF,
    0x31CECECE,
    0x32CDCDCD,
    0x33CCCCCC,
    0x34CBCBCB,
    0x35CACACA,
    0x36C9C9C9,
    0x37C8C8C8,
    0x38C7C7C7,
    0x39C6C6C6,
    0x3AC5C5C5,
    0x3BC4C4C4,
    0x3CC3C3C3,
    0x3DC2C2C2,
    0x3EC1C1C1,
    0x3FC0C0C0,
    0x40BFBFBF,
    0x41BEBEBE,
    0x42BDBDBD,
    0x43BCBCBC,
    0x44BBBBBB,
    0x45BABABA,
    0x46B9B9B9,
    0x47B8B8B8,
    0x48B7B7B7,
    0x49B6B6B6,
    0x4AB5B5B5,
    0x4BB4B4B4,
    0x4CB3B3B3,
    0x4DB2B2B2,
    0x4EB1B1B1,
    0x4FB0B0B0,
    0x50AFAFAF,
    0x51AEAEAE,
    0x52ADADAD,
    0x53ACACAC,
    0x54ABABAB,
    0x55AAAAAA,
    0x56A9A9A9,
    0x57A8A8A8,
    0x58A7A7A7,
    0x59A6A6A6,
    0x5AA5A5A5,
    0x5BA4A4A4,
    0x5CA3A3A3,
    0x5DA2A2A2,
    0x5EA1A1A1,
    0x5FA0A0A0,
    0x609F9F9F,
    0x619E9E9E,
    0x629D9D9D,
    0x639C9C9C,
    0x649B9B9B,
    0x659A9A9A,
    0x66999999,
    0x67989898,
    0x68979797,
    0x69969696,
    0x6A959595,
    0x6B949494,
    0x6C939393,
    0x6D929292,
    0x6E919191,
    0x6F909090,
    0x708F8F8F,
    0x718E8E8E,
    0x728D8D8D,
    0x738C8C8C,
    0x748B8B8B,
    0x758A8A8A,
    0x76898989,
    0x77888888,
    0x78878787,
    0x79868686,
    0x7A858585,
    0x7B848484,
    0x7C838383,
    0x7D828282,
    0x7E818181,
    0x7F808080,
    0x807F7F7F,
    0x817E7E7E,
    0x827D7D7D,
    0x837C7C7C,
    0x847B7B7B,
    0x857A7A7A,
    0x86797979,
    0x87787878,
    0x88777777,
    0x89767676,
    0x8A757575,
    0x8B747474,
    0x8C737373,
    0x8D727272,
    0x8E717171,
    0x8F707070,
    0x906F6F6F,
    0x916E6E6E,
    0x926D6D6D,
    0x936C6C6C,
    0x946B6B6B,
    0x956A6A6A,
    0x96696969,
    0x97686868,
    0x98676767,
    0x99666666,
    0x9A656565,
    0x9B646464,
    0x9C636363,
    0x9D626262,
    0x9E616161,
    0x9F606060,
    0xA05F5F5F,
    0xA15E5E5E,
    0xA25D5D5D,
    0xA35C5C5C,
    0xA45B5B5B,
    0xA55A5A5A,
    0xA6595959,
    0xA7585858,
    0xA8575757,
    0xA9565656,
    0xAA555555,
    0xAB545454,
    0xAC535353,
    0xAD525252,
    0xAE515151,
    0xAF505050,
    0xB04F4F4F,
    0xB14E4E4E,
    0xB24D4D4D,
    0xB34C4C4C,
    0xB44B4B4B,
    0xB54A4A4A,
    0xB6494949,
    0xB7484848,
    0xB8474747,
    0xB9464646,
    0xBA454545,
    0xBB444444,
    0xBC434343,
    0xBD424242,
    0xBE414141,
    0xBF404040,
    0xC03F3F3F,
    0xC13E3E3E,
    0xC23D3D3D,
    0xC33C3C3C,
    0xC43B3B3B,
    0xC53A3A3A,
    0xC6393939,
    0xC7383838,
    0xC8373737,
    0xC9363636,
    0xCA353535,
    0xCB343434,
    0xCC333333,
    0xCD323232,
    0xCE313131,
    0xCF303030,
    0xD02F2F2F,
    0xD12E2E2E,
    0xD22D2D2D,
    0xD32C2C2C,
    0xD42B2B2B,
    0xD52A2A2A,
    0xD6292929,
    0xD7282828,
    0xD8272727,
    0xD9262626,
    0xDA252525,
    0xDB242424,
    0xDC232323,
    0xDD222222,
    0xDE212121,
    0xDF202020,
    0xE01F1F1F,
    0xE11E1E1E,
    0xE21D1D1D,
    0xE31C1C1C,
    0xE41B1B1B,
    0xE51A1A1A,
    0xE6191919,
    0xE7181818,
    0xE8171717,
    0xE9161616,
    0xEA151515,
    0xEB141414,
    0xEC131313,
    0xED121212,
    0xEE111111,
    0xEF101010,
    0xF00F0F0F,
    0xF10E0E0E,
    0xF20D0D0D,
    0xF30C0C0C,
    0xF40B0B0B,
    0xF50A0A0A,
    0xF6090909,
    0xF7080808,
    0xF8070707,
    0xF9060606,
    0xFA050505,
    0xFB040404,
    0xFC030303,
    0xFD020202,
    0xFE010101,
    0xFF000000, // 255号色
];

// 开启此选项后，引擎在往城中添加人物前先将该人物从所有城池中删除。
baye.data.g_engineConfig.checkRedundantOnAddPerson = 1;

// 开局选择时期
baye.hooks.loadPeriod = function() {
    baye.centerChoose(100, 80, ["简单", "困难"], 0, function(idx) {
        console.log('chosen: ' + idx);
        if (idx == baye.None) {
            return 0; // 0 返回主界面， 1 继续选择君主
        }
        baye.loadPeriod(1); // 加载时期1数据
        // 可以在这里进一步做其它初始化配置
        return 1; // 继续选择君主
    });
}

// 菜单空闲时每隔大约半秒多调用一次onMenuIdle
baye.hooks.onMenuIdle = function(ctx) {
    baye.drawText(0, 0, "selected:" + ctx.index);
    var x = 8*ctx.index;
    var y = 20;
    var w = 8, h = 8;
    baye.revertRect(x, y, x+w-1, y+h-1);
}

// 战场系统菜单
baye.hooks.fightOpenMainMenu = function() {
    baye.centerChoose(60, 30, ["回合结束", "全军撤退"], 0, function(idx) {
        // 返回值被当做原系统菜单的选择项
        return idx;
    });
}

// 主系統菜单
baye.hooks.mainSystemMenu = function() {
    baye.centerChoose(60, 30, ["策略结束", "存储进度", "退出游戏"], 0, function(idx) {
        // 返回值被当做原系统菜单的选择项
        return idx;
    });
}

// 战场移动将领后动作选择菜单
baye.hooks.fightChooseAction = function(ctx) {
    console.log("person = " + ctx.index);
    baye.centerChoose(30, 60, ["攻击", "计谋", "查看", "待机"], 0, function(idx) {
        // 返回值被当做原始菜单的选择项
        return idx;
    });
}

// 战场技能选择菜单
baye.hooks.fightChooseSkill = function(ctx) {
    console.log("person = " + ctx.index);
    var skills = [2, 3, 4];
    var skillNames = skills.map(x => baye.getSkillName(x)); // 注意有些浏览器没有map函数

    baye.centerChoose(30, 60, skillNames, 0, function(idx) {
        if (idx == baye.None) {
            // 取消选择
            return baye.None;
        } else {
            // 返回技能ID
            return skills[idx];
        }
    });
}

// 设置是否开启像素模糊化, 高DPI时，有些情况可能开启blur会好看点
baye.blurScreen(true);







var currentFont = 0;

function setFont(font) {
    if (baye.setFont(font) == baye.OK) {
        currentFont = font;
    }
}

baye.hooks.willCloseMenu = function(ctx) {
    baye.hooks.willChangeMenuSelection = null;
};

baye.hooks.showMainHelp = function() {
    baye.hooks.willChangeMenuSelection = function(ctx) {
        var oldFont = currentFont; // 备份当前字体
        setFont(ctx.index);
        baye.clearRect(0, 0, 60, 30)
        baye.drawText(0, 0, "字体预览")
        setFont(oldFont); // 预览完恢复当前字体
    };
    baye.centerChoose(28, 50, ["默认", "仿宋", "黑体", "楷体", "宋体"], currentFont, function(idx) {
        if (idx != baye.None) {
            setFont(idx);
        }
    });
}

// 游戏主菜单, 未测试
baye.hooks.chooseGameEntry = function() {
    baye.centerChoose(100, 50, ["新开局", "重返沙场", "退出"], 0, function(idx) {
        if (idx == 2) {
            return 3;
        } else {
            return idx;
        }
    });
}

// 开局选君主
baye.hooks.chooseActor = function() {
    var ids = [1, 2, 3];
    var names = ids.map(x => baye.getPersonName(x));
    baye.centerChoose(100, 50, names, 0, function(idx) {
        if (idx == baye.None) {
            return baye.None;
        }
        return ids[idx];
    });
}


// 自定义一个简易交互式网格菜单
function gridChoose(
    x, y, w, h, // 外框
    item_w, item_h, // 单个选项高宽
    total,    // 选项总数
    selected, // 初始选中 
    drawItem, // 绘制单个选项
    callback  // 选择或取消后的回调
) {
    var cols = Math.floor(w / item_w);
    var rows = Math.floor(total / cols);
    var display_rows = Math.min(rows, Math.floor(h / item_h));
    var display_idx_max = cols * display_rows;

    if (display_idx_max == 0) {
        // 显示不下，当退出处理
        return callback(baye.None);
    }

    function itemRect(idx) {
        var col = idx % cols;
        var row = Math.floor(idx / cols);
        return {
            x: col * item_w,
            y: row * item_h,
            w: item_w,
            h: item_h,
        };
    }

    function revertDisplayItem(idx) {
        var rect = itemRect(idx);
        baye.revertRect(rect.x, rect.y, rect.x + rect.w - 1, rect.y + rect.h - 1);
    }

    function itemAtPoint(x0, y0) {
        if (x0 < x || x0 >= x + cols * item_w) {
            return baye.None;
        }
        if (y0 < y || y0 >= y + display_rows * item_h) {
            return baye.None;
        }
        var col = Math.floor((x0 - x) / item_w);
        var row = Math.floor((y0 - y) / item_h);
        var idx = row * cols + col;
        if (idx >= display_idx_max) {
            return baye.None;
        } else {
            return idx;
        }
    }

    // 处理系统消息
    function processNextMessage() {
        baye.sysMessage(msg => {
            switch (msg.type) {
                // 按键消息
                case baye.VM_KEY: {
                    switch (msg.param) {
                        case baye.VK_UP: {
                            revertDisplayItem(selected);
                            if (selected > 0) {
                                selected--;
                            }
                            revertDisplayItem(selected);
                            break;
                        }
                        case baye.VK_DOWN: {
                            revertDisplayItem(selected);
                            if (selected < display_idx_max - 1) {
                                selected++;
                            }
                            revertDisplayItem(selected);
                            break;
                        }
                        case baye.VK_LEFT: {
                            break;
                        }
                        case baye.VK_RIGHT: {
                            break;
                        }
                        case baye.VK_ENTER: {
                            return callback(selected);
                        }
                        case baye.VK_EXIT: {
                            return callback(baye.None);
                        }
                    }
                    break
                }
                // 触控消息。 这里简易处理, 实际要体验更好需要考虑更多细节
                case baye.VM_TOUCH: {
                    switch (msg.param) {
                        case baye.VT_TOUCH_DOWN: {break;}
                        case baye.VT_TOUCH_UP: {
                            // 触控抬起时，计算触控点在哪个item上, 选中该item
                            var x = msg.param2.i16.p0;
                            var y = msg.param2.i16.p1;
                            var idx = itemAtPoint(x, y);
                            if (idx == baye.None) {
                                // 点击其他区域返回
                                return callback(baye.None);
                            } else {
                                // 如果已经高亮, 则确认选择
                                if (idx == selected) {
                                    return callback(idx);
                                }
                                revertDisplayItem(selected);
                                selected = idx;
                                revertDisplayItem(selected);
                            }
                            break;
                        }
                        case baye.VT_TOUCH_MOVE: {break;}
                        case baye.VT_TOUCH_CANCEL: {break;}
                    }
                    break
                }
                // 定时器消息
                case baye.VM_TIMER: {
                    break
                }
            }
            processNextMessage();
        })
    }

    baye.clearRect(x, y, w, h);
    for (var col = 0; col < cols; col++) {
        for (var row = 0; row < display_rows; row++) {
            var idx = row * cols + col;
            var x1 = x + col * item_w;
            var y1 = y + row * item_h;
            drawItem(x1, y1, idx);
        }
    }
    revertDisplayItem(selected);
    processNextMessage();
}


baye.hooks.showMainHelp = function() {
    gridChoose(0, 0, 120, 150, 36, 36, 9, 0, function(x, y, idx) {
        baye.drawRect(x, y, x + 35, y + 35, 1);
        baye.drawText(x, y, "ITEM" + idx);
    }, function(idx) {
        console.log("选中了" + idx);
    });
}



// 判断技能目标是否满足条件
baye.hooks.canUseSkill = function(ctx) {
    var skill = ctx.skillIndex; // 技能号
    var attacker = ctx.attackerIndex; // 攻击者
    var target = ctx.targetIndex; // 目标
    var same = ctx.same; // 是否同阵营

    return true; // 所有技能都可以使用
}

// 战场地形图单元绘制
baye.hooks.drawMapUnit = function(ctx) {
    var x = ctx.x;
    var y = ctx.y;
    var scr = !ctx.flag;
    var tile = ctx.tile;
    var pic = baye.data.g_FightMap[tile];

    var color = baye.data.g_paintColor;
    baye.data.g_paintColor = 170; // 地图色稍浅
    baye.drawImage(x, y, baye.data.g_TileId, 0, pic, scr);
    baye.data.g_paintColor = color;
}

// 战场人物绘制
baye.hooks.drawOneGeneral = function(ctx) {
    /*
        BIND_U8EX("frame", &act);
        BIND_U8EX("index", &i);
        BIND_U8EX("pic", &idx);
        BIND_U8EX("x", &sx);
        BIND_U8EX("y", &sy);
    */
    var x = ctx.x;
    var y = ctx.y;
    var pic = ctx.pic;

    var color = baye.data.g_paintColor;
    baye.data.g_paintColor = 200;
    baye.drawImage(x, y, 4, 0, pic, 1);
    baye.data.g_paintColor = color;
}

// 玩家城池被进攻时选择出战钩子
baye.hooks.meetFight = function({city}) {
    console.log("meet fight at " + city);
    const items = ["出战", "谈判"];
    function choose() {
        baye.centerChoose(40, 30, items, 0, index => {
            switch(items[index]) {
                case "出战": {
                    // 显示选择将领界面
                    return 1;
                }
                case "谈判": {
                    // 把战场上的敌军放回其归属地
                    return 0;
                }
                default: {
                    // 不能取消
                    choose();
                }
            }
        });
    }
    choose();
}

// 战场状态栏被点击
baye.hooks.fightStatusBarTouched = function({x, y}) {
    console.log(sprintf("status bar is touched at (%d,%d)", x, y));
    if (x < 30) {
        baye.sendKey(baye.VK_EXIT);
    } else if (x < 60){
        baye.sendKey(baye.VK_SEARCH);
    } else {
        baye.sendKey(baye.VK_HELP);
    }
    return 0;
}


// 自定义字体
function configFont(fontSize, fontName) {
    var fontId;
    // 设置引擎字体号（0号对应分辨率是12x12, 1-4号是24x24, 5号是48x48）
    switch (fontSize) {
        case 48:
            fontId = 5;
            break;
        case 24:
            fontId = 1;
            break;
        case 12:
            fontId = 0;
            break;
        default:
            throw "Unsupported font size: " + fontSize
    }
    baye.setFont(fontId);
    // 清除字体缓存
    baye.clearFontCache();
    // 开启完全自定义字体
    baye.data.g_engineConfig.useCustomFont = 1;
    // 开启完全自定义英文字体
    baye.data.g_engineConfig.useCustomFontEn = 1;
    // 设置引擎获取字模的钩子
    baye.hooks.fontImageForChar = function (ctx) {
        ctx.zmCode = getFontImage(ctx.code);
    }
    // 实现从页面渲染获取字模
    function getFontImage (c) {
        var ch;
        var w;
        var h;

        let gbkdecoder = new TextDecoder('gbk');
        var canvas = document.createElement("canvas");
        canvas.width = fontSize;
        canvas.height = fontSize;
        var ctx = canvas.getContext("2d");

        if (c < 256) {
            w = Math.ceil(fontSize / 2);
            h = fontSize;
            ch = String.fromCharCode(c);
            // 如果数字切边，可在这里微调
            ctx.font = (fontSize-2) + "px '" + fontName + "'";

            // 处理一些符号被切边
            var x_scale = {
                "%": 0.7, // 如果是百分号横向压缩至70%
                "@": 0.5,
                "#": 0.8,
            }[ch] || 1;
            var y_scale = 1;
            ctx.setTransform(x_scale, 0, 0, y_scale, 0, 0);
        } else {
            w = fontSize;
            h = fontSize;
            ch = gbkdecoder.decode(new Uint8Array([c >> 8, c & 0xff]));
            ctx.font = (fontSize-2) + "px '" + fontName + "'";
        }
        ctx.textAlign = "left";
        ctx.textBaseline = "top";
        ctx.fillStyle = "#000000";
        ctx.fillRect(0, 0, w, h);
        ctx.fillStyle = "#FFFFFF";
        ctx.fillText(ch, 0, 0);
        var data = ctx.getImageData(0, 0, w, h).data;
        var bs = Math.ceil(w / 8);
        var zm = new Array(bs * h);

        for (var i = 0; i < zm.length; i++)zm[i] = 0;

        //console.log(sprintf("%s %sx%s len=%s", ch, w, h, zm.length));

        for (var i = 0; i < h; i++) {
            var line = sprintf("%02d:", i);
            for (var j = 0; j < w; j++) {
                if (data[i * w * 4 + j * 4]) {
                    zm[parseInt(j / 8) + i * bs] += 0x80 >> (j % 8);
                    line += "*";
                } else {
                    line += "_";
                }
            }
            //console.log(line);
        }
        return zm;
    }
}

function loadNetFont(url) {
    // 加载联网字体
    const myFontName = 'BayeFace';
    const font = new FontFace(myFontName, 'url(' + url + ')', { style: 'normal', weight: 400 });
    document.fonts.add(font);
    font.load().then(() => {
        // 加载完成，重新配置引擎字体
        configFont(48, myFontName);
        console.log("加载字体完成");
    },
    (err) => {
        console.error("加载字体失败");
    });
}

