#ifndef	_RESOURCE_H
#define	_RESOURCE_H

/*======================================
  资源管理头文件
======================================*/

typedef struct {
	U32	ResLen;			/* 资源串总长度 */
	U16	ResId;			/* 资源串id */
	U16	ItmCnt;			/* 资源项个数 */
	U16	ItmLen;			/* 资源项长度(0表示资源项不等长，后面会有资源项索引序列) */
	U8	ResKey;			/* 资源串加密钥匙(加密算法统一使用+ResKey) */
	U8	Reserved;		/* 保留 */
}RCHEAD;				/* 资源串头定义 */

#endif	/* _RESOURCE_H */
