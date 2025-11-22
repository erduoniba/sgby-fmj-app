#ifndef		DICTHELP_H
#define		DICTHELP_H

#ifndef		DICTSYS_H
#include	"dictsys.h"
#endif

#define  HELP_ID_01  00 /*帮助-主界面*/
#define  HELP_ID_02  01 /*帮助-菜单选择（适用于所有选择、确认、跳出三种功能的界面）*/
#define  HELP_ID_03  02 /*帮助-单词输入*/
#define  HELP_ID_04  03 /*帮助-单词选词*/
#define  HELP_ID_05  04 /*帮助-生/近选词*/
#define  HELP_ID_06  05 /*(删)*/
#define  HELP_ID_07  06 /*帮助-解释界面（生/近/单词解释界面）*/
#define  HELP_ID_08  07 /*帮助-跳查界面*/
#define  HELP_ID_09  08 /*帮助-跳查解释*/
#define  HELP_ID_10  09 /*帮助-属性解释*/
#define  HELP_ID_11  10 /*帮助-汉英词典*/
#define  HELP_ID_12  11 /*帮助-汉英解释*/
#define  HELP_ID_13  12 /*帮助-背诵设置（用于背诵中单词设置界面）*/
#define  HELP_ID_14  13 /*帮助-单词浏览*/
#define  HELP_ID_15  14 /*帮助-单词测验（快乐背诵）*/
#define  HELP_ID_16  15 /*(删)*/
#define  HELP_ID_17  16 /*帮助-输入单词（艾宾浩斯初记/复习/我爱背诵/疯狂背诵）*/
#define  HELP_ID_18  17 /*帮助-轻松测试（轻松背诵）*/
#define  HELP_ID_19  18 /*帮助-菜单选择（疯狂背诵）*/
#define  HELP_ID_20  19 /*帮助-普通计算*/
#define  HELP_ID_21  20 /*帮助-科学计算*/
#define  HELP_ID_22  21 /*帮助-单位换算*/
#define  HELP_ID_23  22 /*帮助-汇率设定*/
#define  HELP_ID_24  23 /*帮助-汇率计算*/
#define  HELP_ID_25  24 /*帮助-公式计算*/
#define  HELP_ID_26  25 /*帮助-编辑（名片与个人资料编辑通用）*/
#define  HELP_ID_27  26 /*帮助-浏览*/
#define  HELP_ID_28  27 /*帮助-目录*/
#define  HELP_ID_29  28 /*帮助-名片查找*/
#define  HELP_ID_30  29 /*帮助-日记编辑（日记浏览与个人资料通用）*/
#define  HELP_ID_31  30 /*帮助-浏览（日记浏览与个人资料通用）*/
#define  HELP_ID_32  31 /*帮助-电子签名编辑*/
#define  HELP_ID_33  32 /*(删)*/
#define  HELP_ID_34  33 /*帮助-课程表*/
#define  HELP_ID_35  34 /*帮助-课程编辑*/
#define  HELP_ID_36  35 /*帮助-浏览*/
#define  HELP_ID_37  36 /*帮助-万年历*/
#define  HELP_ID_38  37 /*帮助-时间设置*/
#define  HELP_ID_39  38 /*帮助-闹铃时间设置*/
#define  HELP_ID_40  39 /*帮助-闹铃周表*/
#define  HELP_ID_41  40 /*帮助-铃声类型*/
#define  HELP_ID_42  41 /*帮助-秒表*/
#define  HELP_ID_43  42 /*帮助-城市列表（世界时间城市目录）*/
#define  HELP_ID_44  43 /*帮助-自定义纪念日*/
#define  HELP_ID_45  44 /*帮助-目录（补充自定义纪念日/日程）*/
#define  HELP_ID_46  45 /*帮助-日程编辑*/
#define  HELP_ID_47  46 /*帮助-浏览*/
#define  HELP_ID_48  47 /*帮助-倒计时*/
#define  HELP_ID_49  48 /*帮助-下载说明*/
#define  HELP_ID_50  49 /*帮助-文章浏览（电子图书/电子资料浏览）*/
#define  HELP_ID_51  50 /*帮助-书签*/
#define  HELP_ID_52  51 /*帮助-字词搜索*/
#define  HELP_ID_53  52 /*帮助-页面查找*/
#define  HELP_ID_54  53 /*帮助-设置（屏幕调整、滚屏延时、声音大小）*/
#define  HELP_ID_55  54 /*帮助-密码设置*/
#define  HELP_ID_56  55 /*帮助-容量检查*/
#define  HELP_ID_57  56 /*帮助-下载数据列表（7号更改）*/
#define  HELP_ID_58  57 /*(删)*/
#define  HELP_ID_59  58 /*帮助-浏览（厂商简介/开发群/产品简介）*/
#define  HELP_ID_60  59 /*帮助-闪存碎片整理*/
#define  HELP_ID_61  60 /*(删)*/
#define  HELP_ID_62  61 /*帮助-开机动画*/
#define  HELP_ID_63  62 /*帮助-菜单（补充电子图书/电子资料）*/
#define  HELP_ID_64  63 /*帮助-文件属性（补充电子图书）*/
#define  HELP_ID_65  64 /*(删)*/
#define  HELP_ID_66  65 /*帮助-两机通讯*/
#define  HELP_ID_67  66 /*帮助-自动关机（补充系统）*/
#define  HELP_ID_68  67 /*帮助-开机设置（补充系统）*/
#define  HELP_ID_69  68 /*(删)*/
#define  HELP_ID_70  69 /*帮助-版本信息（补充系统）*/
#define  HELP_ID_71  70 /*帮助-例句（补充英汉词典）*/
#define  HELP_ID_72  71 /*帮助-浏览世界时间（补充世界时间）*/
#define  HELP_ID_73  72 /*帮助-单词复习（补充我爱背诵）*/
#define  HELP_ID_74  73 /*(删)*/
#define  HELP_ID_75  74 /*帮助-复习笔记（补充疯狂背诵）*/
#define  HELP_ID_76  75 /*帮助-学习记录（补充疯狂背诵）*/
#define  HELP_ID_77  76 /*(删)*/
#define  HELP_ID_78  77 /*(删)*/
#define  HELP_ID_79  78 /*帮助-查找界面（补充电子图书查找界面）*/
#define  HELP_ID_80  79 /*帮助-其他数据（7号补充资源管理器）*/
#define  HELP_ID_81  80 /*帮助-浏览（6号补充世界时间）*/
#define  HELP_ID_82  81 /*帮助-课程表命名（补充课程表）*/
#define  HELP_ID_83  82 /*(删)*/
#define  HELP_ID_84  83 /*(删)*/
#define  HELP_ID_85  84 /*帮助-智能拼音*/
#define  HELP_ID_86  85 /*帮助-关键字*/
#define  HELP_ID_87  86 /*帮助-下载类（28号更改）*/
#define  HELP_ID_88  87 /*帮助-文件类型目录*/
#define  HELP_ID_89  88 /*帮助-姓氏选择*/
#define  HELP_ID_90  89 /*帮助-课程浏览*/
#define  HELP_ID_91  90 /*帮助-糊模查询（补充英汉词典）*/
#define  HELP_ID_92  91 /*帮助-属性选词（补充英汉词典）*/
#define  HELP_ID_93  92 /*帮助-系统收录节日(补充纪念日)*/
#define  HELP_ID_94  93 /*帮助-开机设置提醒(补充纪念日)*/
#define  HELP_ID_95  94 /*帮助-纪念日浏览(补充纪念日)*/
#define  HELP_ID_96  95 /*帮助-日程浏览(安排日程安排)*/

#endif

