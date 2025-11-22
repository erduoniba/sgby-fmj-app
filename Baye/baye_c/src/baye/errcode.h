#ifndef	_ERR_CODE_H
#define	_ERR_CODE_H

/*======================================
  出错管理头文件
======================================*/

/*------------------------
  错误结构
  ------------------------*/
typedef	struct {
	U8	errcode;		/* 错误代码 */
	U8	erroccur;		/* 错误发生位置(错误放生时所在的文件) */
	U16	errresid;		/* 出错资源串id */
	U16	erritmid;		/* 出错资源项id */
}ERRINF;				/* 出错信息结构定义 */

/*------------------------
  错误类型
  ------------------------*/
#define	WARNING_LEVEL		0			/* 产生警告消息的错误等级-警告的错误将暂停程序的执行 */
#define	TERM_LEVEL		WARNING_LEVEL+50	/* 产生致命消息的错误等级-致命的错误将终止程序的执行 */
#define NONE_ERR		0			/* 运行错误清零 */
/*警告错误*/
#define	MAP_LOAD_ERR		1			/* 地图载入错误 */
#define	TIL_LOAD_ERR		2			/* 地图块载入错误 */

#endif	/* _ERR_CODE_H */

