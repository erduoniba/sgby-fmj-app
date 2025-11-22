enum {
    /* 程序常量数据文件 */
    /* 兵种攻击范围(按兵种排序) */
    dFgtAtRange = 1,
    /* 兵种策略(按兵种排序) */
    dFgtJNArray,
    /* 各种地形对不同兵种的移动力影响 */
    dFgtLandR,
    /* 各种地形对不同兵种的战力量影响 草地 平原 山地 森林 村庄 城池 营寨 河流 */
    dFgtLandF,
    
    /* 战斗初始化坐标 攻击方*8+防守方 */
    dFgtIntPos,
    
    /* 城市数组 */
    dCityPos,
    
    /* 道具属性显示宽度 */
    GOODS_PRO_WID,
    /* 武将属性显示宽度 */
    PERSON_PRO_WID,
    /* 无用数据 */
    DirectP,
    /* 指令消耗体力		任 开 招 搜 治 出 招 处 流 赏 没 交 宴 输 移 离 招 策 反 劝 无 无 无 侦 征 分 掠 出 */
    ConsumeThew,
    /* 指令消耗金钱		 任  开  招  搜  治  出  招  处  流  赏  没  交  宴  输  移  离  招  策  反  劝  无  无  无  侦  征  分  掠  出 */
    ConsumeMoney,
    /* 君主产生“内政、协调” 策略几率 */
    KingTacticOddsIH,
    /* 君主产生“外交”、“军备”策略几率 */
    KingTacticOddsD,
    /* city map */
    C_MAP,
    /* 城市战斗地图ID */
    dCityMapId,

    /* 引擎参数配置 */
    dEngineConfig,

    /*兵种移动力*/
    dFgtIntMove,

    /*相克系数*/
    dSubduModu,

    /*各兵种攻击系数*/
    dAtkModulus,
    /*各兵种防御系数*/
    dDfModulus,
    /*各地形防御系数*/
    dTerrDfModu,

    kdJNSpeId,
    /* 技能播放的起始和终止针 */
    kdJNSpeSFrm,
    kdJNSpeEFrm,
    /* 技能播放的坐标 */
    kdJNSpeSX,
    /* 技能背景模式-1为有背景 0为没背景 */
    kdJNMode,
};