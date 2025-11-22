#pragma once
#include "dictsys.h"
#include "keytable.h"
#include "debug.h"

#define __8D27 _00208000
#define __8D2A _0020813F
#define __8D2D _002082CD
#define __8D30 _00208375
#define __8D33 _002084B5
#define __8D36 _002084DC
#define __8D39 _002085A3
#define __8D3C _00208E01
#define __8D3F _00208E4C
#define __8D42 _00208F3B
#define __8D45 _002090AC
#define __8D48 _002091C1
#define __8D4B _002092D6
#define __8D4E _00209490
#define __8D51 _002094B2
#define __8D54 _00209591
#define __8D57 _00209700
#define __8D5A _002097DB
#define __8D5D _00209D89
#define __8D60 _0020A0EE
#define __8D63 _0020A23A
#define __8D66 _0020A28D
#define __8D69 _0020A2F8
#define __8D6C _0020A891
#define __8D6F _0020AFEC
#define __8D72 _0020B641
#define __8D75 _0020BB0F
#define __8D78 _00234000
#define __8D7B _00234D76
#define __8D7E _0023516C
#define __8D81 _00235BD4
#define __8D84 _00236061
#define __8D87 _0023675B
#define __8D8A _00236FD7
#define __8D8D _00237631
#define __8D90 _002377C6
#define __8D93 _00214000
#define __8D96 _00214BE8
#define __8D99 _00215199
#define __8D9C _00215227
#define __8D9F _0021686E
#define __8DA2 _00216EF3
#define __8DA5 _00217748
#define __8DA8 _00218000
#define __8DAB _00218196
#define __8DAE _0021828D
#define __8DB1 _00204000
#define __8DB4 _00204563
#define __8DB7 _002046A5

#define __8DBA _00204739
#define __8DBE _00244000
#define __8DC1 _002441C3
#define __8DC4 _00244573
#define __8DC7 _002446ED
#define __8DCA _00244914
#define __8DCD _00247BBE
#define __8DD0 _00247BFE
#define __8DD3 _0020C000
#define __8DD6 _0020C148
#define __8DD9 _0020C1FB
#define __8DDC _0020C61F
#define __8DDF _0020C8AB
#define __8DE2 _0020C9C5
#define __8DE5 _0020D0C5
#define __8DE8 _0020E7FE
#define __8DEB _0020E850
#define __8DEE _0020E8A9
#define __8DF1 _0020EA8B
#define __8DF4 _0020EB2D
#define __8DF7 _0020EED0
#define __8DFA _0020EFD5
#define __8DFD _0020F437
#define __8E00 _0021C000
#define __8E03 _0021CDE4
#define __8E06 _00224000
#define __8E09 _00227924
#define __8E0C _0023C000
#define __8E0F _0023C7F9
#define __8E12 _0023CD77
#define __8E15 _0023CFE5
#define __8E18 _0023D20C
#define __8E1B _0023D52A
#define __8E1E _0023DAA7
#define __8E21 _0023E1E4
#define __8E24 _0023E641
#define __8E27 _0023E7A1
#define __8E2A _0023EA2F
#define __8E2D _0023ECA9
#define __8E30 _0023F623
#define __8E33 _0023F994
#define __8E36 _00240000
#define __8E39 _002400C1
#define __8E3C _002401AE
#define __8E3F _00240841
#define __8E42 _00241598
#define __8E45 _002418BC
#define __8E48 _0024218A
#define __8E4B _00242399
#define __8E4E _00242DA4
#define __8E51 _0024314B
#define __8E54 _0024327E
#define __8E57 _0024330C
#define __8E5A _00243555
#define __8E5D _00228000
#define __8E60 _00229222
#define __8E63 _00229452
#define __8E66 _00229784
#define __8E69 _0022998D
#define __8E6C _0022A081
#define __8E6F _0022A28C
#define __8E72 _0022A31B
#define __8E75 _0022A8FA
#define __8E78 _0022AB72
#define __8E7B _0022AE7B
#define __8E7E _00220000
#define __8E81 _00220874
#define __8E84 _0022111F
#define __8E87 _00221FE3
#define __8E8A _002227EF
#define __8E8D _00223246
#define __8E90 _002235D8
#define __8E93 _0022C000
#define __8E96 _0022E873
#define __8E99 _0022EB3F
#define __8E9C _0022EE58
#define __8E9F _0022F42C
#define __8EA2 _0022F5CE
#define __8EA5 _00230000
#define __8EA8 _002319B4
#define __8EAB _002322CA

#define __8EAF _00210000
#define __8EB2 _00210D5B
#define __8EB5 _002116EB
#define __8EB8 _00211ABD
#define __8EBB _00212986
#define __8EBE _00212EB4
#define __8EC1 _00238000
#define __8EC4 _00239068
#define __8EC7 _00239787
#define __8ECA _00239BE2
#define __8ECD _00239F29
#define __8ED0 _0023A1F2
#define __8ED3 _0023A326
#define __8ED6 _0023A4E0
#define __8ED9 _0023A4E6
#define __8EDC _0023A64B
#define __8EDF _0023A83D
#define __8EE2 _0023ACA7

#define __8EE6 _00200046
#define __8EE9 _00200942
#define __8EEC _002018C6
#define __8EEF _00201948
#define __8EF2 _00201F1E
#define __8EF5 _0020249B
#define __8EF8 _00203425
#define __8EFB _00218301
#define __8EFE _00219486
#define __8F01 _00219B0D
#define __8F04 _00219DF8
#define __8F07 _0021A1B4
#define __8F0A _0021A499
#define __8F0D _0021B0AE
#define __8F10 _0021B221
#define __8F13 _0020F532
#define __8F16 _0020F876
#define __8F19 _0020F975
#define __8F1C _0020FA30
#define __8F1F _0020FA67
#define __8F22 _0020FABF
#define __8F25 _00203591
#define __8F28 _0020378B
#define __8F2B _002038D3
#define __8F2E _002039BF
#define __8F31 _00203B8B
#define __8F34 _0023B486
#define __8F37 _0023B652
#define __8F3A _0023B73B
#define __8F3D _0023B762
#define __8F40 _0023B789
#define __8F43 _0023B88B

#define _SysMemcpy SysMemcpy
#define _SysMemcmp SysMemcmp
#define _DataBankSwitch DataBankSwitch
#define _GetDataBankNumber GetDataBankNumber

#define MEM_ERR        1    /*堆空间申请或释放失败*/
#define DLOSE_ERR      2    /*数据页面丢失*/
#define ILOSE_ERR      3    /*数据索引定位失败*/
#define DATA_ERR       4    /*数据错误*/
#define ACTOR_ERR      5    /*不可识别的角色类型*/
#define MAP_OVERFL     6    /*地图边界溢出*/
#define ID_ERR         7    /*创建/删除角色的id越界*/
#define CONTROL_ERR    8    /*当前的控制角色尚未被创建*/
#define NPC_OVERF      9    /*创建NPC角色时，资源耗竭*/
#define FACETO_ERR     10   /*不存在的角色方向*/
#define NPC_ERR        11   /*指定脚标的NPC不存在*/
#define NPC_RECBEUSE   12   /*指定脚标的NPC已经被占用*/
#define ENEMY_K_NODEF  13   /*发生战斗时却发现没有定义当前场景中可能出现的敌人种类*/
#define GOODS_NODEF    14   /*不可识别的道具类型*/
#define OVERFLOW_MAG   15   /*魔法链节点个数溢出*/
#define TEMPFILE_ERR   16   /*创建临时文件失败*/

#define DAT_GUDSCRIPT     1     /*剧情数据界*/
#define DAT_MAPDATA       2     /*地图文件界*/
#define DAT_ACTORRES      3     /*角色资源界*/
#define DAT_MGICRES       4     /*魔法资源界*/
#define DAT_SPECRES       5     /*特效资源界*/
#define DAT_GOODSRES      6     /*道具资源界*/
#define DAT_TILEDATA      7     /*地图块资源界*/
#define DAT_ACTORPIC      8     /*角色图片资源界*/
#define DAT_GOODSPIC      9     /*道具图片资源界*/
#define DAT_SPEPIC        10    /*特效图片资源界*/
#define DAT_SUNDRYPIC     11    /*杂类图片资源界*/
#define DAT_MAGICLINK     12    /*链资源界*/

#define MUSIC             0
#define LOADMAP           1
#define CREATEACTOR       2
#define DELETENPC         3
#define MAPEVENT          4
#define ACTOREVENT        5
#define MOVE              6
#define ACTORMOVE         7
#define ACTORSPEED        8
#define _CALLBACK         9
#define GOTO              10
#define IF                11
#define SET               12
#define SAY               13
#define STARTCHAPTER      14
#define SCREENR           15
#define SCREENS           16
#define SCREENA           17
#define EVENT             18
#define MONEY             19
#define GAMEOVER          20
#define IFCMP             21
#define ADD               22
#define SUB               23
#define SETCONTROLID      24
#define GUTEVENT          25
#define SETEVENT          26
#define CLREVENT          27
#define BUY               28
#define FACETOFACE        29
#define MOVIE             30
#define CHOICE            31
#define CREATEBOX         32
#define DELETEBOX         33
#define GAINGOODS         34
#define INITFIGHT         35
#define FIGHTENABLE       36
#define FIGHTDISENABLE    37
#define CREATENPC         38
#define ENTERFIGHT        39
#define DELETEACTOR       40
#define GAINMONEY         41
#define USEMONEY          42
#define SETMONEY          43
#define LEARNMAGIC        44
#define SALE              45
#define NPCMOVEMOD        46
#define MESSAGE           47
#define DELETEGOODS       48
#define RESUMEACTORHP     49
#define ACTORLAYERUP      50
#define BOXOPEN           51
#define DELALLNPC         52
#define NPCSTEP           53
#define SETSCENENAME      54
#define SHOWSCENENAME     55
#define SHOWSCREEN        56
#define USEGOODS          57
#define ATTRIBTEST        58
#define ATTRIBSET         59
#define ATTRIBADD         60
#define SHOWGUT           61
#define USEGOODSNUM       62
#define RANDRADE          63
#define MENU              64
#define TESTMONEY         65
#define CALLCHAPTER       66
#define DISCMP            67
#define RETURN            68
#define TIMEMSG           69
#define DISABLESAVE       70
#define ENABLESAVE        71
#define GAMESAVE          72
#define SETEVENTTIMER     73
#define ENABLESHOWPOS     74
#define DISABLESHOWPOS    75
#define SETTO             76
#define TESTGOODSNUM      77
#define SETFIGHTMISS      78
#define SETARMSTOSS       79

extern UINT8 MCU_memory[0x10000];

void _00200046();
void _002000C4();
void _00200247();
void _00200942(UINT8 _17CF);
// 播放游戏开始动画
void _00200B96();
void _00200C79();
// 游戏开始时的菜单，返回选择了哪项
UINT8 _00200DF1();
UINT8 _002010D5();
// 读取进度界面
UINT8 _0020117E(UINT8 _17CF);
UINT8 _002018C0();
UINT8 _002018C6();
UINT8 _00201948();
void _00201A53();
void _00201C6C();
void _00201EB2();
UINT8 _00201F1E(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1);
void _0020239E(UINT8 _17CF, UINT8 _17D0);
void _0020249B(UINT8 _17CF);
void _002024FE(UINT8 _17CF);
void _00202C6D(UINT8 _17CF);
UINT8* _00202F9F(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2, UINT16* _17D4, UINT8* _17D6);
void _00203425(UINT8* _17D0, UINT8 _17D2, UINT8 _17D3, UINT8 _17D4);
// BUY "t0 t1 ... tsum"
// 买卖，t0 ... tsum为物品类型，每种以空格分开，在""中(最多20种)
void _00203591();
// TESTMONEY money lessadr
// 检测玩家金钱，如果金钱少于money，即跳到lessadr处执行
void _0020378B();
// CALLCHAPTER m n
// 调用号为n，类型为m的脚本，当前脚本执行位置被保存，所有资源不初始化；当被调用脚本CALLBACK时，返回调用位置继续执行。（注意：调用前后资源都不进行初始化，被调用脚本也会影响调用后的资源状况）
void _002038D3();
// DELETEGOODS type index erradr
// 删除物品链中的物品（type，index），如果物品链中无此物品即跳到erradr处执行
void _002039BF();
// MESSAGE "..."
// 用消息框显示"..."中信息（每行最多显示8个汉字，最多可显示5行），等待按键才返回。
void _00203B8B();
UINT8 _00204000(UINT16 _17D0);
// 处理对应NPC战斗触发事件相应的事件
UINT8 _00204563(UINT8 _17CF);
void _002046A5();
void _00204739();
// LOADMAP m n x y
// 载入号码n,类型m的地图，初始位置（x，y）
void _0020484C();
// SCREENS x y
// 屏幕左上角定位到地图的（x，y）处
void _00204A08();
// SETCONTROLID actor
// 设置角色号actor为当前控制(默认的被控制角色——游戏的主人翁的id=1)
void _00204B52();
// DELETEACTOR actor
// 主角角色离队，从角色队列中删除指定主角号码actor的角色
void _00204BE5();
// CREATEBOX id boxindex x y
// 创建一个宝箱，宝箱号码boxindex，位置为（x，y），id为操作号
void _00204C97();
// DELETEBOX id
// 删除操作号为id的宝箱
void _00204E3D();
// GAINGOODS type index
// 获得物品，类型为type，号码为index
void _00204E49();
// INITFIGHT f0 f1 f2 f3 f4 f5 f6 f7 scrb scrl scrr
// 初始化随即战斗参数并开启随机战斗，f0-f7为可能出现的敌人的种类，scrb为大战斗背景，scrl为左下角图，scrr为右上角图
void _00205121();
// FIGHTENABLE
// 开启随机战斗
void _0020541C();
// FIGHTDISENABLE
// 关闭随机战斗
void _00205443();
// ENTERFIGHT roundmax f0 f1 f2 scrb scrl scrr evtrnd0 evtrnd1 evtrnd2 evt0 evt1 evt2 lossto winto
// 进入战斗，roundmax为限定最多回合数（0为无限），f0-f2为敌人，evtrnd0-evtrnd2为战斗触发事件回合，evt0-evt2为相应的事件号,lossto为战斗失败时跳转到的地址标号,winto为战斗胜利时跳转到的地址标号,scrb为大战斗背景，scrl为左下角图，scrr为右上角图
void _0020546A();
// GAINMONEY money
// 得到数目为money的钱
void _00205B30();
// USEMONEY money
// 用去（丢失）数目为money的钱
void _00205C82();
// SETMONEY money
// 设置现钱数目为money
void _00205DD4();
// LEARNMAGIC actor type index
// 号码actor的主角学会法术类型为type，序号为index的法术
void _00205ED6();
// SALE
void _0020608A();
// NPCMOVEMOD id mode
// 配角的运动模式，id为操作号，mode为 1 时随机运动，为 0 时原地不动,为 4 激活状态——只换图片，不改变位置（适合动态的物品对象）
void _002060B7();
// RESUMEACTORHP id rade
// 恢复角色id生命rade%(一般用于剧情战斗失败,而游戏继续)
void _002062BB();
// DELALLNPC
// 删除当前场景所有NPC
void _0020663B();
// ATTRIBTEST actor type attrval lowaddr higaddr
// 检测号码actor的主角的type（0-级别，1-攻击力，2-防御力，3-身法，4-生命，5-真气当前值，6-当前经验值，7-灵力，8-幸运，9-攻击的异常回合数，10-对特殊状态的免疫，11-普通攻击可能产生异常状态，12-合体法术，13-每回合变化生命，14-每回合变化真气，15-头戴，16-身穿，17-肩披，18-护腕，19-手持，20-脚蹬，21-佩戴1，22-佩戴2，23-生命上限，24-真气上限）属性，属性值小于attrval跳到lowaddr执行脚本，属性值大于attrval跳到higaddr执行脚本，等于不跳转。
void _0020666A();
// SHOWGUT toppic botpic "......"
// 显示字幕，toppic和botpic为字幕"......"的上下加修饰的图片号
void _00206D95();
// USEGOODSNUM type index num erradr
// 使用物品链中的num个物品（type，index），如果物品链中无此物品或该物品个数小于num，即跳到erradr处执行且物品链中物品不变
void _00206F18();
// RANDRADE rade trueadr
// 产生1000份之rade的随机机率，如果命中，即跳到erradr处执行
void _002071F7();
// DISCMP n m lowaddr higaddr
// 脚标号为n变量与数m，小于数m跳到lowaddr执行脚本，大于数m跳到higaddr执行脚本，等于不跳转。
void _00207376();
// RETURN
// 返回到调用位置的下一句(和CALLBACKDE的区别除了返回区别外，还有一个重要的是本语句会恢复原脚本的事件跳转表等脚本信息)
void _0020758A();
// TIMEMSG time "......"
// 用消息框显示"......"中信息（每行最多显示8个汉字，最多可显示5行），延时time*10毫秒，如果time为0则要按键才显示结束。
void _00207590();
// DISABLESAVE
// 禁止存档功能
void _002076D3();
// ENABLESAVE
// 允许存档功能
void _002076FA();
// GAMESAVE
// 调用存档功能
void _00207721();
// TESTGOODSNUM type index num adrequ adrmore
// 检测物品链中的物品（type，index）的个数，如果物品链中无此物品或该物品个数小于num，即不跳转；如果个数等于num，跳到adrequ处执行；如果个数大于num，跳到adrmore处执行。
void _0020774E();
// CREATEACTOR actor x y
// 创建主角号码actor，位置为（x，y）
void _00208000();
// CREATENPC id npc x y
// 创建配角号码npc，位置为（x，y），id为操作号
void _0020813F();
// DELETENPC id
// 删除操作号为id的配角
void _002082CD();
// MOVE id x y
// 将操作号为id的NPC角色移动到（x，y）处——主角不可以移动
void _00208375();
// CALLBACK
// 退出脚本
void _002084B5();
// GOTO label
// 跳转到脚本地址标号为label处运行
void _002084DC();
// SAY pos "......"
// 将""中的内容打印到屏幕上，pos为说话的附带贴图号，0为没有贴图
void _002085A3();
// SCREENR
void _00208E01();
// SET n m
// 脚标号为n的变量内容为m
void _00208E4C();
// IFCMP n m label
// 脚标号为n变量与数m相等就跳转到脚本地址标号为label处运行
void _00208F3B();
// ADD n m
// 脚标号为n的变量与数m相加，结果存到脚标号为n的变量中
void _002090AC();
// SUB n m
// 脚标号为n的变量减去数m，结果存到脚标号为n的变量中
void _002091C1();
// IF n label
// 如果事件标志为1，跳转到脚本地址标号为label处运行
void _002092D6();
// ACTORMOVE
void _00209490();
// STARTCHAPTER m n
// 载入号为n，类型为m的脚本，当前脚本结束
void _002094B2();
// SETEVENT n
// 将事件n标志设置为1
void _00209591();
// CLREVENT n
// 将事件n标志设置为0
void _00209700();
// FACETOFACE n m
// 角色n与角色m面相对，0为主角
void _002097DB();
// NPCSTEP id faceto step
// id角色，0为主角,faceto 面向，step 脚步
void _00209D89();
// SETSCENENAME "..."
// 设置场景名称，五个汉字以内。
void _0020A0EE();
// SHOWSCENENAME
// 显示场景名称
void _0020A23A();
// SHOWSCREEN
// 刷新屏幕
void _0020A28D();
// ATTRIBSET actor type attrval
// 设置号码actor的主角的type（0-级别，1-攻击力，2-防御力，3-身法，4-生命，5-真气当前值，6-当前经验值，7-灵力，8-幸运，9-攻击的异常回合数，10-对特殊状态的免疫，11-普通攻击可能产生异常状态，12-合体法术，13-每回合变化生命，14-每回合变化真气，15-生命上限，16-真气上限）属性，属性值等于attrval。
void _0020A2F8();
// ATTRIBADD actor type attrval
// 号码actor的主角的type（0-级别，1-攻击力，2-防御力，3-身法，4-生命，5-真气当前值，6-当前经验值，7-灵力，8-幸运，9-攻击的异常回合数，10-生命上限，11-真气上限）属性值加上attrval。
void _0020A891();
void _0020AFEC(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1);
void _0020B31C(UINT8 _17CF, UINT8 _17D0, UINT8 y, UINT8* _17D2);
void _0020B4AF(UINT8 _17CF, UINT8 _17D0);
// MENU choice "......"
// 多项选择菜单，"......"为菜单项字符，项与项间以空格分开（如"离开 继续前进 瞬移"），项数不限；选择结果存放在脚标号为choice的变量中，0为跳出没选择，其他值为选中项序号
void _0020B641();
// SETTO n1 n2
// 将变量n1的值赋给变量n2
void _0020BB0F();
void _0020C000(UINT8 _17CF, UINT8 _17D0);
void _0020C148(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2);
// _17D0:角色类型（玩家角色、NPC角色、敌人角色、场景对象），_17D1:角色id
void _0020C1FB(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3);
void _0020C61F(UINT8 _17CF);
void _0020C8AB(UINT8 _17CF);
void _0020C9C5(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
void _0020CF6D(UINT8 _17CF);
UINT8 _0020D0C5(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3);
void _0020E7FE();
void _0020E850();
UINT8 _0020E8A9();
void _0020E969(UINT8 _17CF);
void _0020EA8B(UINT8 _17CF);
UINT8 _0020EB2D(UINT8 _17CF);
void _0020EED0();
UINT8 _0020EFD5(UINT8 _17CF);
UINT8 _0020F437(UINT8 _17CF);
void _0020F532(UINT8 _17CF, UINT8 _17D0, INT32 _17D1, UINT8 _17D5, UINT8 _17D6);
void _0020F876();
void _0020F975(UINT32 _17D0);
void _0020FA30(UINT32 _17D0);
void _0020FA67(UINT8 _17CF, UINT8 _17D0, UINT32 _17D1, UINT8 _17D5, UINT8 _17D6);
void _0020FABF(UINT8 _17CF, UINT8 _17D0, UINT32 _17D1, UINT8 _17D5, UINT8 _17D6);
// 魔法详细信息界面
UINT8 _00210000(UINT16 _17D0, UINT8 _17D2, UINT8 _17D3);
// 游戏中主菜单
UINT8 _00210D5B(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8* _17D3, UINT8 _17D5, UINT8* _17D6);
UINT8 _002116EB(UINT8* _17D0, UINT8 _17D2, UINT8* _17D3, UINT8 _17D5, UINT8 _17D6, UINT8* _17D7);
// 显示角色属性
void _00211ABD(UINT8* _17D0, UINT8 _17D2);
void _00212986();
void _00212EB4();
void _00214000();
UINT8 _00214347(UINT8* _17D0);
void _00214B98();
void _00214BE8();
void _00215199();
void _00215227(UINT8 _17CF);
UINT8 _00215346();
UINT8 _0021554E(UINT8 _17CF, UINT8 _17D0);
UINT8 _00215648(UINT8 _17CF, UINT8 _17D0);
void _00215865();
void _00215A79(UINT8 _17CF);
void _00215EDE(UINT8 _17CF, UINT8* _17D0);
void _002165FB(UINT8 _17CF);
void _002167B3();
void _0021686E(UINT8 _17CF, UINT8 _17D0);
void _002169E0(UINT8 _17CF, UINT8 _17D0);
// 引擎版本显示
void _00216CA7();
// 打开显示主角在当前地图的坐标功能
void _00216E26();
void _00216EF3();
// 装载当前显示的地图数据到内存
void _00217748();
void _00218000(UINT8 x, UINT8 y, UINT8 width, UINT8 height, UINT8* _17D3);
void _00218196(UINT8 A, UINT8 _17D0, UINT8 _17D1, UINT8* _17D2, UINT8* _17D4);
void _0021828D(UINT8* _17D0);
// _17D0:角色类型（玩家角色、NPC角色、敌人角色、场景对象），_17D1:角色id
void _00218301(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
void _002183F7(UINT8 _17CF, UINT8* _17D0);
void _00218BBB(UINT8 _17CF, UINT8* _17D0);
void _00218EA7(UINT8 _17CF, UINT8* _17D0);
void _00219486(UINT8 _17CF, UINT8 _17D0);
void _00219B0D(UINT8 x, UINT8 y, UINT32 _17D1, UINT8 _17D5);
void _00219DF8(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 x1, UINT8 y1);
void _0021A1B4(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 x1, UINT8 y1);
UINT8* _0021A499(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
// _17D0指向6字节数据空间。找到给定标记的实际偏移量_17D0[3]:偏移多少个0x4000,*(UINT16*)(_17D0+0x0004):偏移的字节
void _0021A678(UINT8* _17D0);
// 判断数据是那种类型的，与_17CF（取值范围[1,12]）匹配则返回0x00
UINT8 _0021B0AE(UINT8 _17CF);
void _0021B221(UINT8* _17D0);
// _17CF:0与指定敌人战斗,1与随机敌人战斗
// 返回战斗结果，1战斗胜利，2战斗失败
UINT8 _0021C000(UINT8 _17CF);
void _0021C16E();
// _17CF第几个玩家角色
UINT8 _0021C307(UINT8 _17CF);
// 初始化战斗前的各项数据
void _0021C8AF();
// 显示敌人角色
void _0021CD12();
// _17CF敌人序号[0,2],_17D0图片序号
void _0021CDE4(UINT8 _17CF, UINT8 _17D0);
UINT8 _0021D208(UINT8 _17CF);
UINT8 _0021D3AF(UINT8 _17CF);
UINT8 _0021D9DC(UINT8 _17CF);
UINT8 _0021DDBA(UINT8 _17CF);
UINT8 _0021E331(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
// 随机普通敌人
void _0021EA2D();
// 战斗触发事件回合处理
void _0021EBE5();
UINT8 _0021ECE3(UINT8 _17CF);
UINT8 _0021F445(UINT8 _17CF);
UINT8 _0021F52E(UINT8* _17D0);
void _0021F60F();
void _0021F6C9(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
UINT8 _00220000(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2);
UINT8 _00220874(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2);
void _0022111F(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2, UINT8* _17D4);
void _00221979(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2, UINT8* _17D4);
void _00221AE9(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT8 _17D3);
void _00221F23(UINT8* _17D0, UINT8 _17D2, UINT8 _17D3);
void _00221FE3(UINT8* _17D0, UINT8 _17D2);
void _002227EF(UINT8* _17D0, UINT8* _17D2, UINT8* _17D4, UINT8 _17D6);
void _00223246(UINT8 _17CF, UINT8* _17D0, UINT8 _17D2, UINT8 _17D3);
void _002235D8(UINT8 _17CF, UINT8* _17D0, UINT8 _17D2, UINT8 _17D3);
UINT16 _00223988(UINT16 _17D0, UINT8 _17D2, UINT8 _17D3);
// 进行一个回合的战斗
UINT8 _00224000();
void _0022447C(UINT8 _17CF);
void _00224635(UINT8 _17CF);
void _0022475F(UINT8 _17CF);
void _00224E75(UINT8 _17CF);
void _002259AC(UINT8 _17CF);
void _00226400(UINT8 _17CF);
void _002264FA(UINT8 _17CF);
UINT8 _00226B9F(UINT8 _17CF);
void _00226D37(UINT8 _17CF);
void _002270A5(UINT8 _17CF, UINT16 _17D0);
void _0022726A(UINT8 _17CF, UINT8 _17D0, UINT16 _17D1, UINT16* _17D3);
void _0022773B();
void _002277CD(UINT8 _17CF);
void _00227924();
void _00227A7C();
void _00228000(UINT8 _17CF, UINT8 _17D0);
void _00228694(UINT8 _17CF);
void _00228C7F();
void _00228F9F(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3);
UINT8 _00229222(UINT8 _17CF, UINT8* _17D0);
UINT8 _00229452(UINT8 _17CF, UINT8* _17D0);
UINT8 _00229784(UINT8 _17CF, UINT8* _17D0);
void _0022998D(UINT8 _17CF, UINT8 _17D0);
void _0022A081(UINT8* _17D0);
// 设置可能出现的敌人种类。_17CF:可能出现的敌人种类数量,_17D0:可能出现的敌人种类列表
void _0022A190(UINT8 _17CF, UINT8* _17D0);
// 返回可能出现的敌人有多少种
UINT8 _0022A28C();
UINT8 _0022A31B(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2, UINT8 _17D3, UINT8 _17D4);
UINT8 _0022A682(UINT8 _17CF, UINT16 _17D0, UINT8 _17D2, UINT8 _17D3, UINT8 _17D4);
void _0022A8FA(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1);
void _0022AB72(UINT8 _17CF, UINT8 _17D0);
void _0022AE7B(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2, UINT8 _17D4);
void _0022B054(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
void _0022C000();
void _0022C757();
// 显示获得的道具名称和数量
void _0022CC0F();
void _0022CFD6(UINT8 _17CF);
void _0022D1F7(UINT8* _17D0, UINT8 _17D2);
void _0022D34B(UINT8 _17CF, UINT8* _17D0);
UINT8 _0022D9E8(UINT8 _17CF, UINT8* _17D0);
UINT8 _0022E655(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT16* _17D3);
void _0022E873(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
void _0022EB3F(UINT8* _17D0, UINT8* _17D2);
void _0022EE58(UINT8 _17CF, UINT8 _17D0);
// 角色升级
void _0022F05D(UINT8 _17CF);
void _0022F1D7(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
void _0022F42C();
void _0022F5CE(UINT8 _17CF);
// 显示偷得的道具名称
void _0022F65A(UINT8 _17CF);
// 角色穿戴界面
void _00230000();
UINT8 _002319B4(UINT8 _17CF, UINT8 _17D0);
// 准备穿戴装备的装备对比界面
UINT8 _002322CA(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1);
void _00234000();
UINT8 _00234D76();
UINT8 _0023516C(UINT8* _17D0);
void _00235BD4(UINT8 _17CF);
UINT8 _00236061(UINT16 _17D0);
// 购买/出售道具
// _17CF:0x00-购买道具,0x01-出售道具
// _17D0:背包中道具数量（总道具数量不大于99）
// _17D1:道具名称
// _17D3:道具买价/当价
// 返回购买/出售的道具数量，0xFF代表取消
UINT8 _0023675B(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT32 _17D3);
void _00236FD7();
void _00237631(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
void _002377C6(UINT16 _17D0);
// 装备列表界面
UINT8 _00238000(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1, UINT8 _17D3, UINT8* _17D4);
void _00239068(UINT8 _17CF, UINT8 _17D0, UINT8* _17D1);
UINT8 _00239787(UINT8 x, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2, UINT8 _17D3, UINT8* _17D4);
UINT8 _00239BE2(UINT8* _17D0);
UINT8 _00239F29(UINT8* _17D0);
UINT8 _0023A1F2(UINT8 _17CF, UINT8 _17D0);
// ACTORLAYERUP id level
// 角色id升到第level级,如果现级别大于或等于level则不升级
void _0023A326();
// GAMEOVER
// 游戏结束
void _0023A4E0();
// BOXOPEN id
// 使宝箱为打开状态（显示第二张图片）
void _0023A4E6();
// MUSIC n	timers
// 播放号码为n(为0时停止播放)音乐,持续次数为timers(为0时循环播放)
void _0023A64B();
// MOVIE type team x y ctl
// 播放动画，type和team为参数,(x,y)动画的相对屏幕左上角坐标的偏移,ctl为1时按键动画结束,ctl为2时背景不变,ctl为3时按键动画结束及背景不变
void _0023A83D();
// CHOICE "..." "..." adress
// 选择框，选择第一个不跳转，选择第二个脚本跳转到地址adress处(""中的字符数为9个汉字或19个ASCII字符）
void _0023ACA7();
// USEGOODS type index erradr
// 使用物品链中的一个物品（type，index），如果物品链中无此物品即跳到erradr处执行
void _0023B486();
// SETEVENTTIMER event timer
// 定时触发事件event，定时时间为timer*0.5秒
void _0023B652();
// ENABLESHOWPOS
// 打开显示主角在当前地图的坐标功能
void _0023B73B();
// DISABLESHOWPOS
// 取消显示主角在当前地图的坐标功能
void _0023B762();
UINT8 _0023B789(UINT8 _17CF, UINT8 _17D0);
void _0023B88B(UINT8 _17CF);
void _0023C000(UINT8* _17D0);
UINT8 _0023C7F9(UINT8 _17CF);
// _17D0物品数量加1
void _0023CD77(UINT16 _17D0);
void _0023CFE5(UINT8 _17CF);
void _0023D20C(UINT8 _17CF);
void _0023D52A(UINT8 _17CF, UINT8 _17D0);
void _0023DAA7(UINT8 _17CF, UINT8 _17D0);
void _0023E1E4(UINT8 _17CF, UINT8 _17D0);
void _0023E641();
void _0023E7A1(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1, UINT8 _17D2);
void _0023EA2F(UINT8 _17CF, UINT8 _17D0, UINT8 _17D1);
UINT8 _0023EBC0(UINT8 _17CF);
UINT8 _0023ECA9(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2, UINT16* _17D4);
void _0023F623(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2);
void _0023F994();
void _00240000(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2);
UINT8 _002400C1(UINT8 _17CF, UINT16* _17D0, UINT8 _17D2);
void _002401AE(UINT8 _17CF);
void _00240841(UINT8 _17CF);
void _00241014(UINT8 _17CF, UINT16 _17D0, UINT8 _17D2);
void _00241375(UINT8 _17CF, UINT16 _17D0, UINT8 _17D2);
void _00241598(UINT8 _17CF, UINT8 _17D0);
void _00241806(UINT8 _17CF, UINT16 _17D0);
void _002418BC(UINT8 _17CF);
void _00241A3F(UINT8 _17CF);
void _0024218A(UINT8 _17CF);
UINT8 _00242399(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2, UINT16* _17D4);
void _00242DA4(UINT8 _17CF, UINT16* _17D0, UINT16* _17D2);
UINT8 _0024314B(UINT8 _17CF, UINT8 _17D0);
void _0024327E();
void _0024330C(UINT8 _17CF);
void _00243555(UINT8 _17CF);
void _002435CB(UINT8 _17CF);
void _002438F5(UINT8 _17CF);
UINT8 _00244000();
UINT8 _002441C3(UINT8 _17CF);
UINT8 _00244573(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2);
UINT8 _002446ED(UINT8 _17CF);
UINT8 _00244914(UINT8 _17CF);
UINT8 _00244D2D(UINT8 _17CF, UINT8* _17D0);
UINT8 _002457C3(UINT8 _17CF, UINT8* _17D0);
UINT8 _00245A87(UINT8 _17CF, UINT8* _17D0);
UINT8 _00245E6C(UINT8 _17CF, UINT8* _17D0);
UINT8 _002461F9();
UINT8 _00246255(UINT8 _17CF, UINT16* filename, UINT8 filetype);
UINT8 _0024649F(UINT8 _17CF, UINT8* _17D0);
UINT8 _00246F44(UINT8 _17CF, UINT8* _17D0, UINT8* _17D2);
UINT8 _00247BBE(UINT8 _17CF, UINT8* _17D0);
UINT8 _00247BFE();