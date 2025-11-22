
baye.hooks.battleBuildAttackAttriutes = function(context) {
    /*
        脚本计算攻防示例
        context.index: 计算结果存放位置序号
        context.generalIndex: 战场将领序号
    */

    var result = baye.data.g_GenAtt[context.index];
    var pIndex = baye.data.g_FgtParam.GenArray[context.generalIndex] - 1;
    var person = baye.data.g_Persons[pIndex];

    // 地形对兵种的影响
    var terEffects = [
    //  草地, 平原, 山地, 森林, 村庄, 城池, 营寨, 河流
        [1, 1, 1, 1, 1, 1, 1, 1], // 骑兵
        [1, 1, 1, 1, 1, 1, 1, 1], // 步兵
        [1, 1, 1, 1, 1, 1, 1, 1], // 弓兵
        [1, 1, 1, 1, 1, 1, 1, 1], // 水兵
        [1, 1, 1, 1, 1, 1, 1, 1], // 极兵
        [1, 1, 1, 1, 1, 1, 1, 1], // 玄兵
    ];

    var AtkModulus = [1.0, 0.8, 0.9, 0.8, 1.3, 0.4];	/* 各兵种攻击系数 */
    var DfModulus =  [0.7, 1.2, 1.0, 1.1, 1.2, 0.6];	/* 各兵种防御系数 */
    var TerrDfModu = [1.0, 1.0, 1.3, 1.15, 1.1, 1.5, 1.2, 0.8];	/* 各种地形防御系数 */

    // 武力/等级/攻击系数影响攻击力
    var at = person.Force * (person.Level + 10) * AtkModulus[person.ArmsType];

    // 智力/等级/防御系数影响防御力
    var df = person.IQ * (person.Level + 10) * DfModulus[person.ArmsType];

    // 叠加地形对攻防的影响
    at = terEffects[person.ArmsType][result.ter] * at;

    df = terEffects[person.ArmsType][result.ter] * df;

    // 地形固有防御系数
    df *= TerrDfModu[result.ter];

    // 输出最终结果
    result.at = at;
    result.df = df;
};

baye.hooks.battleDrivePersonState = function(context) {
    var person = baye.getPersonByGeneralIndex(context.generalIndex);
    if (person.Arms > 0) person.Arms -= 1;
};

baye.hooks.countAttackHurt = function(context) {
    /*
        计算伤害示例

        查阅文档可知, 可以使用计算普通攻击伤害的钩子
            http://bgwp.oschina.io/baye-doc/script/index.html#baye-hooks-countattackhurt

        context.hurt: 计算结果存放位置序号

        如下是引擎默认算法:
    */

    var KeZhiMatrix = [
        [1.0, 1.2, 0.8, 1.0, 0.7, 1.3],
        [0.8, 1.0, 1.2, 1.0, 0.6, 1.2],
        [1.2, 0.8, 1.0, 1.0, 1.1, 1.2],
        [1.0, 1.0, 1.0, 1.0, 1.0, 1.0],
        [1.1, 1.3, 0.9, 1.0, 1.0, 1.5],
        [0.6, 0.6, 0.6, 0.6, 0.6, 0.6]
    ];

    var p0 = baye.data.g_GenAtt[0]; //攻击方数据
    var p1 = baye.data.g_GenAtt[1]; //防守方数据

    var person = baye.getPersonByGeneralIndex(p0.generalIndex);

    /* 基本伤害 hurt = (at / df) * arms / 8 */
    var hurt = p0.at / p1.df * (person.Arms >> 3);

    /* 相克加层 hurt *= modu */
    hurt *= KeZhiMatrix[p0.armsType][p1.armsType];
    context.hurt = hurt;
};

baye.hooks.countAttackHurt = function(context) {
    /*
        计算伤害示例

        查阅文档可知, 可以使用计算普通攻击伤害的钩子
            http://bgwp.oschina.io/baye-doc/script/index.html#baye-hooks-countattackhurt

        context.hurt: 计算结果存放位置序号

        引入其它条件(如特殊道具)影响伤害输出:
    */

    var KeZhiMatrix = [
        [1.0, 1.2, 0.8, 1.0, 0.7, 1.3],
        [0.8, 1.0, 1.2, 1.0, 0.6, 1.2],
        [1.2, 0.8, 1.0, 1.0, 1.1, 1.2],
        [1.0, 1.0, 1.0, 1.0, 1.0, 1.0],
        [1.1, 1.3, 0.9, 1.0, 1.0, 1.5],
        [0.6, 0.6, 0.6, 0.6, 0.6, 0.6]
    ];

    var p0 = baye.data.g_GenAtt[0]; //攻击方数据
    var p1 = baye.data.g_GenAtt[1]; //防守方数据

    var person = baye.getPersonByGeneralIndex(p0.generalIndex);

    /* 基本伤害 hurt = (at / df) * arms / 8 */
    var hurt = p0.at / p1.df * (person.Arms >> 3);

    /* 相克加层 hurt *= modu */
    hurt *= KeZhiMatrix[p0.armsType][p1.armsType];

    // 查找攻击方是否携带有特殊道具
    // baye.data.g_GenAtt里面有20个位置, 前十个为攻城方, 后十个为守城方
    //
    // 首先需要判断攻击者是攻城方还是守城放

    var startIndex = p0.generalIndex < 10 ? 0 : 10;

    var hasNewBeeTool = false;
    var newBeeToolID = 5; //牛逼道具的ID

    // 查找攻击方有没有牛逼道具
    for (var ind = startIndex; ind < startIndex + 10; ind++) {
        if (baye.data.g_GenPos[ind].state != 8) { // 8死亡
            var p = baye.getPersonByGeneralIndex(ind);
            if (p.Tool1 == newBeeToolID || p.Tool2 == newBeeToolID) {
                hasNewBeeTool = true; // 找到牛逼道具
                break;
            }
        }
    }

    if (hasNewBeeTool) {
        // 攻击方团队有牛逼道具, 百倍伤害
        hurt *= 100;
    }
    context.hurt = hurt;
};

// 禁止年龄自动增长
baye.data.g_engineConfig.disableAgeGrow = 1;

// 定义数据表
baye.TOOL_AGE_TAB = [10, 10, 0, 10 /* ... */ ];

// 赏赐时加属性
baye.hooks.giveTool = function(ctx) {
    var up = baye.TOOL_AGE_TAB[ctx.toolIndex];
    if (up > 0) {
        var person = baye.data.g_Persons[ctx.personIndex];
        person.Age += up;
    }
    ctx.result = 1;
};

// 没收时加属性
baye.hooks.takeOffTool = function(ctx) {
    var up = baye.TOOL_AGE_TAB[ctx.toolIndex];
    if (up > 0) {
        var person = baye.data.g_Persons[ctx.personIndex];
        person.Age -= up;
    }
    ctx.result = 1;
};

baye.hooks.takeOffTool = function(ctx) {
    baye.say(ctx.personIndex, "你算哪根葱?\n" + baye.getToolName(ctx.toolIndex) + "岂是你想拿走就拿走?", function() {
        baye.alert("没收失败!");
        ctx.result = 0;
    });
};

baye.hooks.willGiveTool = function(c) {
    // 1号道具 + 2号道具 + 3000金钱 可合成 3号道具

    if (baye.getPersonName(c.personIndex) != "马腾") {
        return -1;
    }

    var person = baye.data.g_Persons[c.personIndex];
    var city = baye.data.g_Cities[c.cityIndex];

    // 检查道具是否满足条件
    if (person.Tool1 - 1 == 1 && c.toolIndex == 2) {

        // 限制条件
        if (city.Money < 10000) {
            baye.say(c.personIndex, "城池不够富足,无法开工!");
            return 0;
        }
        if (city.PeopleDevotion < 99) {
            baye.say(c.personIndex, "城民们对你的治理不太满意,无法开工!");
            return 0;
        }
        if (city.State != 0) {
            baye.say(c.personIndex, "城池灾害,无法开工!");
            return 0;
        }
        city.Money -= 3000; // 扣除3000钱
        baye.deleteToolInCity(c.cityIndex, c.toolIndex); // 删除材料道具
        baye.putToolInCity(c.cityIndex, 3); // 生产出新道具
        baye.say(c.personIndex, "全新[" + baye.getToolName(3) + "]已打造成功!");
        return 0;
    }
    return -1;
};

// ==========================================

baye.createCustomData = function() {
    // 创建初始数据
    switch (baye.data.g_PIdx) {
        case 1: //时期1
        case 2: //时期2
        case 3: //时期2
        case 4: //时期4
    }
    return {
        personNames: [],
    };
};

baye.hooks.didOpenNewGame = function() {
    // 新开局时初始化自定义数据
    baye.data.customData = baye.createCustomData();
};

baye.hooks.didLoadGame = function() {
    // 读档后需要从存档读取自定义数据
    var dat = JSON.parse(baye.getCustomData());
    baye.data.customData = dat ? dat : baye.createCustomData();
};

baye.hooks.willSaveGame = function() {
    // 存档后前需要保存自定义数据
    var data = JSON.stringify(baye.data.customData);
    baye.setCustomData(data);
};

// ==========================================

baye.hooks.cityMakeCommand = function() {
    var items = ['继续', '取消'];

    baye.choose(4, 5, 40, 30, items, 1, function(ind) {
        console.log("Choosing: " + ind);
        return ind == 0 ? -1 : 0;
    });
};

baye.hooks.cityMakeCommand = function() {
    var items = range(20);

    baye.choosePerson(items, 0, function(ind) {
        console.log("Choosing: " + baye.getPersonName(ind));
    });
};


baye.hooks.cityMakeCommand = function() {
    var items = range(20);

    baye.chooseTool(items, 0, function(ind) {
        console.log("Choosing: " + baye.getToolName(ind));
    });
};

// ==========================================

// 配置道具表格列数量
baye.data.g_uiCfg.toolPropertyCount = 6;

// 配置表格列宽度
baye.data.g_uiCfg.toolPropertiesDisplayWitdh = [4, 6, 6, 6, 6, 6];

// 定义道具表格表头
var baye_tool_heads = ["类型", "加武力", "加智力", "加统率", "加移动", "变兵种"];

// 填写表头的钩子
baye.hooks.getToolPropertyTitle = function(c) {
    c.title = baye_tool_heads[c.propertyIndex];
    return 0;
};

// 填写单元格的钩子
baye.hooks.getToolPropertyValue = function(c) {

    var tool = baye.data.g_Tools[c.toolIndex];

    switch (baye_tool_heads[c.propertyIndex]) {
        case "类型":
            c.value = ["装备", "使用"][tool.useflag];
            break;
        case "加武力":
            c.value = tool.at; // 转成字符串
            break;
        case "加智力":
            c.value = tool.iq; // 转成字符串
            break;
        case "加统率":
            c.value = baye.TOOL_AGE_TAB[c.toolIndex];
            break;
        case "加移动":
            c.value = '' + tool.move; // 转成字符串
            break;
        case "变兵种":
            c.value = ['无', '水', '玄', '极', "骑兵", "步兵", "弓兵", "水兵", "极兵", "玄兵"][tool.arm];
            break;
    }
    return 0;
};


baye.hooks.tacticStage4 = function() {
    var allPerson = baye.data.g_Persons;
    var allCity = baye.data.g_Cities;
	var maxLevel = baye.data.g_engineConfig.maxLevel;
    var playerKingId = baye.data.g_PlayerKing + 1;
	var mixLevel = 0.4 * maxLevel;

    for (var i = 0; i < allPerson.length; i++) { //征兵机制
        var p = allPerson[i];
        if (
            p.Belong > 0
            && p.Belong != playerKingId
            && p.Belong != 255
        ) {
			var chance = Math.random();
			var maxArms = p.Level*100 + p.Age*150 + p.Force*80 + p.IQ*70;
			var halfArms = p.Level*50 + p.Age*80 + p.Force*40 + p.IQ*30;
			if (p.Arms == 800) {
			 p.Arms = maxArms;
			}
			else if (chance < 0.7 && p.Arms >= halfArms) {
			 p.Arms = maxArms;
			}
			else if (chance < 0.9 && p.Arms <= halfArms) {
			 p.Arms = halfArms;
			}
		}
	}

	for (var a = 0; a < allPerson.length; a++) { //AI升级机制
        var p = allPerson[a];
        if (
            p.Belong > 0
            && p.Belong != playerKingId
            && p.Belong != 255
            && p.Level < maxLevel
        ) {
            var chance = Math.random();
            if (chance < 0.285) {
                p.Level = p.Level + 1;
            }
			else if  (chance < 0.325 && p.Force > 75) {
                p.Level = p.Level + 1;
            }
			else if  (chance < 0.325 && p.IQ > 75) {
                p.Level = p.Level + 1;
            }
        }
    }

	for (var b = 0; b < allCity.length; b++) { //AI内政完善
		var city = allCity[b];
		if (
			city.Belong > 0
			&& city.Belong != playerKingId
		) {
			var chance = Math.random();
			if (chance < 0.8 && city.Food < 1200) { //防缺粮机制1
			 city.Food = city.Food + 300;
			}
			else if (chance < 0.9 && city.Food < 600) { //防缺粮机制2
			 city.Food = city.Food + 500;
			}
			if (city.Food > 40000) { //AI粮草管控
			 city.Food = city.Food/1.5
			}
			if (city.Money > 25000) { //AI金钱管控
			 city.Money = city.Money/2.5
			}
			if (city.Commerce > 6000) { //AI商业管控
			 city.Commerce = city.Commerce/1.5
			}
			if (city.MothballArms > 0) { //AI后备兵力清空
			 city.MothballArms = 0;
			}
		}
	}

	for (var c = 0; c < allPerson.length; c++) {  //玩家自动获得经验机制
        var p = allPerson[c];
        if (
            p.Belong == playerKingId
            && p.Belong != 255
			&& p.Level < mixLevel
        ) {
			if (
            p.Experience >= 95
        ) {
             p.Level = p.Level + 1;
			 p.Experience = 0;
            }
			else if (
            p.Experience < 100
        ) {
             p.Experience = p.Experience + 5;
            }
        }
    }

}


baye.data.g_uiCfg.personPropertiesCount = 13;                          // 配置人物表格 列数量

baye.data.g_uiCfg.personPropertiesDisplayWitdh = [                     // 配置人物表格 列宽度
4, 4, 4, 4, 4, 4, 4, 4, 4, 10, 10, 8, 4,]

var baye_person_heads = [                                        // 定义人物表格 表头
"等级", "统率", "武力", "智力", "兵种", "兵力", "忠诚", "体力", "经验", "主道具", "副道具", "归属", "城池",]


baye.hooks.getPersonPropertyTitle = function(c) {                // 填写人物表头的钩子
    c.value = baye_person_heads[c.propertyIndex];
    return 0;
};

baye.hooks.getPersonPropertyValue = function(c) {                // 填写人物单元格的钩子
    var person = baye.data.g_Persons[c.personIndex];

    function toolName(id) {
        if (id == 0)
            return '';
        else
            return baye.getToolName(id - 1);
    }

    function belongName(id) {
        if (id == 0)
            return '无';
        if (id == 0xff)
            return '俘虏';
        if (id - 1 == c.personIndex)
            return '君主';
        return baye.getPersonName(id - 1)
    }

    function getPersonCityName(pindex) {
    // 根据人物序号查找所在城池
    // 算法: 去每个城里面看有没有这个人, 找到则返回该城市名称
        var cities = baye.data.g_Cities;
        for (var i = 0; i < cities.length; i++) {
            var city = cities[i];
            var start = city.PersonQueue;
            var end = city.PersonQueue + city.Persons;
            for (var p = start; p < end; p++) {
                if (baye.data.g_PersonsQueue[p] == pindex) {
                    return baye.getCityName(i);
                }
            }
        }
        return '-'
    }

    switch (baye_person_heads[c.propertyIndex]) {
        case "等级":
            if (person.Level == baye.maxLevel)
                c.value = 'MAX';
            else
                c.value = person.Level;
            break;
        case "统率":
            c.value = person.Age;
            break;
        case "武力":
            c.value = person.Force;
            break;
        case "智力":
            c.value = person.IQ;
            break;
        case "兵种":
            c.value = ["骑兵", "步兵", "弓兵", "水兵", "极兵", "玄兵"][person.ArmsType];
            break;
        case "兵力":
            c.value = person.Arms;
            break;
        case "忠诚":
            c.value = person.Devotion;
            break;
        case "体力":
            c.value = person.Thew;
            break;
        case "经验":
            c.value = person.Experience;
            break;
        case "主道具":
            c.value = toolName(person.Tool1);
            break;
        case "副道具":
            c.value = toolName(person.Tool2);
            break;
        case "归属":
            c.value = belongName(person.Belong);
            break;
        case "城池":
            c.value = getPersonCityName(c.personIndex);
            break;
    }
    return 0;
};

baye.hooks.getFighterInfo = function(c) {
    var pp = baye.data.g_GenPos[c.generalIndex];
    var st = pp.extStates.map(function(v) {
        switch (v) {
            case 1:
                return "状态1"
            case 2:
                return "状态2"
            case 3:
                return "状态3"
        }
        return "其它" + v;
    });
    c.info = "额外状态:[" + st.join(",") + "]"
}

// 调试, 列出战场全部人物
function pa() {
    for (var i = 0; i < 20; i++) {
        var id = baye.data.g_FgtParam.GenArray[i];
        if (id) {
            console.log('' + i + ':' + baye.getPersonName(id-1));
        }
    }
}

function reset() {
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
}

// 调试, 移动指定任务到跟前来
function mv(i) {
    var pd = baye.data.g_GenPos[i];
    pd.x = baye.data.g_FoucsX;
    pd.y = baye.data.g_FoucsY;
}


// 战斗动画前播放自定义特效
baye.hooks.willShowPKAnimation = function() {
	if (baye.data.g_LookMovie) {
        var x = 0;
        var y = 0;
        /* 调用开场动画 resid: 3, itemindex: 0*/
        var resid = 3;
        var item = 0;
        var canskip = 0;
	    baye.playSPE(x, y, resid, item, canskip);
	}
	return -1;
}
function hexprint(x, n) {
    var r = '';
    for (var i = 0; i < n; i++) {
        r += sprintf('%x ', x[i]);
    }
    console.log(r);
}

// -------------- 显示版本 --------------
var libversion = "1.2.3.5";
var customData = {};

baye.hooks.showMainHelp = function() {
    baye.clearScreen();
    text = "版本信息\n";
    text += "引擎:" + baye.data.g_engineVersion + "\n";
    text += "数据:" + libversion + "\n";
    text += "开局:" + customData.libversion + "\n";
    baye.drawText(0, 0, text);
};

baye.hooks.didOpenNewGame = function() {
    // 开局时记住lib的版本
    customData.libversion = libversion;
};

baye.hooks.willSaveGame = function() {
    // 存档时写入自定义数据
    var data = JSON.stringify(customData);
    console.log('save:' + data);
    baye.setCustomData(data);
};

baye.hooks.didLoadGame = function() {
    // 读档后载入自定义数据
    var dat = JSON.parse(baye.getCustomData());
    console.log('load:'+baye.getCustomData());
    baye.data.customData = dat ? dat : {};
};

baye.hooks.showSkill = function(c) {c.result = 1;}


// ---------------------------------------

// 已知两点, 求直线方程
function resolveEquationWithPoints(x0, y0, x1, y1) {
    var res = {a: 0, b: 0, c: 0};
    res.a = y2 - y1;
    res.b = x1 - x2;
    res.c = x2*y1 - x1*y2;
    return res;
}

// 求点到直线的距离
function distanceFromPointToLine(x, y, line) {
    return Math.abs(line.a*x + line.b*y + line.c) / Math.sqrt(line.a*line.a + line.b*line.b);
}

// 两点间距离公式
function distanceOfTwoPoints(x0, y0, x1, y1) {
    var dx = x0 - x1;
    var dy = y0 - y1;
    return Math.sqrt(dx*dx + dy*dy);
}

