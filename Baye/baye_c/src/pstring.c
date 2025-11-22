#include "baye/compa.h"
/* 程序常量字符串文件 */
/* 提示信息 */
const U8 dFgtCtMove[] = "无法到达!";
const U8 dFgtNoAim[] = "没有目标";
const U8 dFgtErrAim[] = "命令无效";
const U8 dFgtWhrErr[] = "天候错误";
const U8 dFgtEthErr[] = "地形错误";
const U8 dFgtMpErr[] = "技能不足";

/* 地形信息 */
const U8 dFgtTrnInf[] = "草地平原山地森林村庄城池营寨河流";

/* 菜单数据 */
const U8 dFgtMnuCmd[] = "攻击计谋查看待机";
const U8 dFgtSysMnu[] = "回合结束全军撤退战斗动画移动速度敌军移动";
const U8 dFgtLookMnu[] = "不看观看";
const U8 dFgtMoveSpe[] = "快速慢速";

/* 帮助数据 */
const U8 dFgtGenTyp[] = "骑兵步兵弓兵水军极兵玄兵";
const U8 dFgtHlpGen[] = "等级:% |兵种:    |武力:%  |智力:%  |经验:%  |生命:%  |技力:%  |攻击:%   |防御:%   |兵力:%   |状态:";
const U8 dTerrInf0[] = "草地：适合各兵种作战之地形。无防御效果。";
const U8 dTerrInf1[] = "平原：适合各兵种作战之地形。无防御效果。";
const U8 dTerrInf2[] = "山地：适合步兵和弓箭兵作战之地形。步兵、弓箭兵防御上升30%。";
const U8 dTerrInf3[] = "森林：适合步兵和弓箭兵作战之地形。步兵、弓箭兵防御上升15%。";
const U8 dTerrInf4[] = "村庄：适合各兵种作战之地形。各兵种防御上升10%。";
const U8 dTerrInf5[] = "城池：适合各兵种作战之地形。各兵种防御上升50%。";
const U8 dTerrInf6[] = "营寨：适合各兵种作战之地形。各兵种防御上升20%。";
const U8 dTerrInf7[] = "河流：适合水军作战之地形。水军无防御效果。";

/* 战斗界面信息 */
const U8 dFgtGetExp[] = "  获得经验：";
const U8 dFgtArmsH[] = "兵力减少";
const U8 dFgtArmsA[] = "兵力增加";
const U8 dFgtProvH[] = "粮草减少";
const U8 dFgtJNFail[] = "计谋失败!";

/* 状态信息 */
const U8 dFgtInSta[] = " 陷入了 ";
const U8 dFgtState0[] = "正常";
const U8 dFgtState1[] = "混乱";
const U8 dFgtState2[] = "禁咒";
const U8 dFgtState3[] = "定身";
const U8 dFgtState4[] = "奇门";
const U8 dFgtState5[] = "遁甲";
const U8 dFgtState6[] = "石阵";
const U8 dFgtState7[] = "潜踪";
const U8 dFgtState[] = " 状态";
const U8 dFgtNoProv0[] = " 我军的粮食消耗完了。";
const U8 dFgtNoProv1[] = " 敌军的粮食消耗完了。";

/* 将领提示信息 */
const U8 dFgtLevUp0[] = "随着能力的提升，我也应该有更好的表现吧！";
const U8 dFgtLevUp1[] = "哈哈……，知道我的厉害了吧！哟哟哟……";
const U8 dFgtLevUp2[] = "早就说过，我是无敌的嘛！怎么就没人肯信呢？";
const U8 dFgtDead0[] = "就这点兵力，实在无法再战下去，只好先撤退了。";
const U8 dFgtDead1[] = "谋事在人，成事在天。我已尽力。时也？命也？";
const U8 dFgtDead2[] = "为什么受伤的总是我？我不甘心，我不甘心！";

/* 错误信息 */
const U8 dChoseKing[] = "势力形势图";
const U8 dGamConErr[] = "游戏环境变量初始化失败。请重试！";
const U8 dGamVarErr[] = "游戏引擎初始化失败。请重试！";

/* 预览信息 */
const U8 dDaysInf[] = "第    天";
const U8 dPowerCmp[] = "战场形势图";
const U8 dArmyInf[] = "军";
const U8 dMainGen[] = "将:";
const U8 dProvInf[] = "粮草:";
const U8 dNoView[] = "?????";
const U8 dReserve0[] = "将领分布";
const U8 dReserve1[] = "已获取敌人最新情报！可使用查找键观看。";
const U8 dReserve2[] = "保留序号";
const U8 dReserve3[] = "保留序号";
const U8 dReserve4[] = "保留序号";
const U8 dReserve5[] = "保留序号";
const U8 dReserve6[] = "保留序号";
const U8 dReserve7[] = "保留序号";
const U8 dReserve8[] = "保留序号";
const U8 dReserve9[] = "保留序号";

/* 存盘文件 */
const U8 dSaveFNam[] = "sango .sav";
const U8 dRecordInf[] = "              年";
const U8 dNullFNam[] = "空";
const U8 dWriting[] = "存储中...";
const U8 dReading[] = "读取中...";
const U8 dErrInf[] = "创建文件失败!";
const U8 dErrInf1[] = "打开文件失败!";