#include "baye/compa.h"

const U8 ATRR_STR0[]="状态:";
const U8 ATRR_STR1[]="归属:";
const U8 ATRR_STR2[]="太守:";
const U8 ATRR_STR3[]="农业:";
const U8 ATRR_STR4[]="商业:";
const U8 ATRR_STR5[]="民忠:";
const U8 ATRR_STR6[]="防灾:";
const U8 ATRR_STR7[]="人口:";
const U8 ATRR_STR8[]="金钱:";
const U8 ATRR_STR9[]="粮食:";
const U8 ATRR_STR10[]="后备兵力:";

const U8 CITY_STATE_STR1[]="正常";
const U8 CITY_STATE_STR2[]="饥荒";
const U8 CITY_STATE_STR3[]="旱灾";
const U8 CITY_STATE_STR4[]="水灾";
const U8 CITY_STATE_STR5[]="暴动";

const U8 ATRR_STR11[]="骑兵";
const U8 ATRR_STR12[]="步兵";
const U8 ATRR_STR13[]="弓兵";
const U8 ATRR_STR14[]="水兵";
const U8 ATRR_STR15[]="极兵";
const U8 ATRR_STR16[]="玄兵";

const U8 ATRR_STR17[]=" 姓  名 ";
const U8 ATRR_STR18[]=" 归  属 ";
const U8 ATRR_STR19[]=" 所在城市 ";
const U8 ATRR_STR20[]="等级";
const U8 ATRR_STR21[]="武力";
const U8 ATRR_STR22[]="智力";
const U8 ATRR_STR23[]="忠诚";
const U8 ATRR_STR24[]="经验";
const U8 ATRR_STR25[]="体力";
const U8 ATRR_STR26[]="兵种";
const U8 ATRR_STR27[]="兵力";
const U8 ATRR_STR28[]="年龄";
const U8 ATRR_GOODS1[]="  道具一  ";
const U8 ATRR_GOODS2[]="  道具二  ";

const U8 MENU_MAINSTR[]="内政外交军备状况";
const U8 MENU_INTERIORSTR[]="开垦招商搜寻治理出巡招降处斩流放赏赐没收交易宴请输送移动";
const U8 MENU_DIPLOMATISMSTR[]="离间招揽策反反间劝降";
const U8 MENU_ARMAMENTSTR[]="侦察征兵分配掠夺出征";

const U8 ATRR_STR60[]="  粮食操作";
const U8 ATRR_STR61[]="  金钱操作";
const U8 ATRR_STR62[]="  士兵操作";
const U8 ATRR_STR63[]="操作值:";
const U8 ATRR_STR64[]="最大值:";

const U8 MENU_TSTR[]="买入卖出";

const U8 ATRR_STR67[]="在野";
const U8 ATRR_STR68[]="君主";
const U8 ATRR_STR69[]="俘虏";
const U8 ATRR_STR70[]="无";
const U8 ATRR_STR71[]="--";

const U8 NOTE_STR1[]="已达农业上限";
const U8 NOTE_STR2[]="已达商业上限";
const U8 NOTE_STR3[]="敌方城池";
const U8 NOTE_STR4[]="无法到达";
const U8 NOTE_STR5[]="无人占领城池";
const U8 NOTE_STR6[]="无兵可分配";
const U8 NOTE_STR7[]="我方城池";
const U8 NOTE_STR8[]="无足够金钱";
const U8 NOTE_STR9[]="该将体力不足";
const U8 NOTE_STR10[]="防灾已达上限";

const U8 DATE_TIME_STR1[]="公元";
const U8 DATE_TIME_STR2[]="年";
const U8 DATE_TIME_STR3[]="月  ";

const U8 GOODS_ATRR_STR0[]="  名  称  ";
const U8 GOODS_ATRR_STR1[]="用法";
const U8 GOODS_ATRR_STR2[]="武力�";
const U8 GOODS_ATRR_STR3[]="智力�";
const U8 GOODS_ATRR_STR4[]="移动�";
const U8 GOODS_ATRR_STR5[]="兵种�";
const U8 GOODS_USEFLAG0[]="装备";
const U8 GOODS_USEFLAG1[]="使用";
const U8 GOODS_ARM1[]="水兵";
const U8 GOODS_ARM2[]="玄兵";
const U8 GOODS_ARM3[]="极兵";

const U8 ALLP_O_STR[]=" 姓  名 命令目标时间";
const U8 NONE_O_STR[]="         --  --  -- ";

const U8 STR_KING[]="君主 ";
const U8 STR_DEAD[]=" 遭劫";
const U8 STR_MAKENEWKING[]="请拥立新君";
const U8 STR_BEKING[]=" 成为君主";
const U8 STR_BEKINGED[]=" 被策反成为君主";
const U8 STR_OVER[]=" 势力灭亡";
const U8 STR_NOPERSON[]="城中无空闲武将";
const U8 STR_NOGOODS[]="该将无道具";
const U8 STR_CNOGOODS[]="城中无道具";
const U8 STR_FULLGOODS[]="该将道具已满";
const U8 STR_NOCAPTIVES[]="城中无俘虏";
const U8 STR_GAMELOST[]="我军全军覆没";
const U8 STR_GAMEWON[]="我军大获全胜";
const U8 STR_WON[]=" 战斗胜利";
const U8 STR_LOST[]=" 战斗失败";
const U8 STR_OBJ[]="选择目标";
const U8 STR_ARMOUT[]="部队已出发";
const U8 STR_WONP[]=" 我愿加入，为主公效力！";
const U8 STR_NOFIGHTER[]="无可出征武将";
const U8 STR_ATTACKC[]="军队进攻";
const U8 STR_OURS[]="我方";
const U8 STR_GETGOODS[]="城中找到 ";
const U8 STR_HEARPERSON[]="听说城中有位贤者，可惜臣未能访到。";
const U8 STR_GETNOTHING[]="什么也没找到……";
const U8 STR_GETSOME1[]="城中找到金钱 ";
const U8 STR_GETSOME2[]="城中找到粮食 ";
const U8 STR_FIGHT_NOTE1[]="军 vs ";
const U8 STR_FIGHT_NOTE2[]="军";
const U8 STR_CAV_NOTE1[]=" 成为俘虏";
const U8 STR_CAV_NOTE2[]=" 等成为俘虏";
const U8 STR_PERSONOVER[]=" 战死";
const U8 STR_CANNOTUSE[]="该将条件无法使用";
const U8 STR_INDUCENOTE[]=" 势力归降 ";

const U8 STR_RP1[]="  出现  ";
const U8 STR_RP2[]="，须尽快治理。";
const U8 STR_RP3[]=" 农业开发度变为 ";
const U8 STR_RP4[]=" (+";
const U8 STR_RP5[]=")。";
const U8 STR_RP6[]=" 商业开发度变为 ";
const U8 STR_RP7[]=" 民忠变为 ";
const U8 STR_RP8[]=" 城市状态正常，防灾变为 ";
const U8 STR_RP9[]="物资运送完毕。";
const U8 STR_RP10[]="途中遇到山贼，所输送物质被抢劫一空！";

const U8 MENU_FUNCSTR[]="策略结束存储进度结束游戏";

const U8 P_SAY_STR1[]="哼！我岂是贪生怕死之徒！";
const U8 P_SAY_STR2[]="你还是少费唇舌，我是不会背信弃义的！";
const U8 P_SAY_STR3[]="就凭你要我归降，笑话！";
const U8 P_SAY_STR4[]="哈……，来吧，十八年后又是一条好汉！";
const U8 P_SAY_STR5[]="死，我岂会恐惧？";
const U8 P_SAY_STR6[]="天命吾身踏黄泉，定起万军夺阴巢！";
const U8 P_SAY_STR7[]="一片忠诚，竟遭此待遇……";
const U8 P_SAY_STR8[]="长此以往，人心难留……";
const U8 P_SAY_STR9[]="主公此举自有道理，我却无法理解……";
const U8 P_SAY_STR10[]="我决不会辜负主公对我的期望！";
const U8 P_SAY_STR11[]="请期待我日后的表现！";
const U8 P_SAY_STR12[]="臣定鞠躬尽瘁，以报答主公之恩！";

const U8 P_SAY_STR13[]="某愿效死忠。";
const U8 P_SAY_STR14[]="在下不才，蒙明公不弃，愿效犬马之劳。";
const U8 P_SAY_STR15[]="某虽不才，愿为主公效劳。";
const U8 P_SAY_STR16[]="请主公期待我的表现吧。";
const U8 P_SAY_STR17[]="只凭你我交情，不足以讨论天下大事，请回吧。";
const U8 P_SAY_STR18[]="什么鸡鸣狗盗之徒，也想让我为你效力？";
const U8 P_SAY_STR19[]="某的能力还不足以担当重任，足下另请高明吧。";
const U8 P_SAY_STR20[]="比在下高明之士众多，明公何必舍良才择朽木？";

const U8 P_SAY_STR21[]="此事便交给我，有利的消息我都会带回来。";
const U8 P_SAY_STR22[]="属下这就动身，主公就静待佳音吧。";
const U8 P_SAY_STR23[]="属下会留心各种消息，以期有所得。";
const U8 P_SAY_STR24[]="我去城中访贤，各位费心留守了。";
const U8 P_SAY_STR25[]="好的人才，要亲自去找才行。";
const U8 P_SAY_STR26[]="为了今后大业，城中看看也非坏事。";

const U8 P_SAY_STR27[]="我虽不愿如此，但也是不得已。";

const U8 P_SAY_STR28[]="掠得金钱 ";
const U8 P_SAY_STR29[]="；夺得粮食 ";
const U8 P_SAY_STR30[]="。";

const U8 P_SAY_STR31[]="马上出发。";

const U8 P_SAY_STR32[]="此事不必再说，战场上分高低！";
const U8 P_SAY_STR33[]="你快回去备好兵马，我到战场上再答复你。";
const U8 P_SAY_STR34[]="笑话，该投降的是你们吧！";
const U8 P_SAY_STR35[]="大胆！竟敢小看我，滚回去喜好脖子！";

const U8 P_SAY_STR36[]="事到如今，也只好降了……";
const U8 P_SAY_STR37[]="此乃天命，请明公善待我的部属和百姓。";
const U8 P_SAY_STR38[]="为了避免生灵涂炭，降也是无可奈何……";
const U8 P_SAY_STR39[]="我乃竭诚投降，请明公勿疑！";

const U8 P_SAY_STR40[]="主公恩情，永铭于心……";
const U8 P_SAY_STR41[]="凭我的三寸不烂之舌，定让他回心转意。";

const U8 TACTIC_STR[]=" 策略中……";

