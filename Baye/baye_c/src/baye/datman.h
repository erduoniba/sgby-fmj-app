#ifndef	_RESOURCE_H
#define	_RESOURCE_H

/*======================================
  资源管理头文件
======================================*/

typedef struct {
	U32	ResLen;			/* 资源串总长度 */
	U16	ResId;			/* 资源串id */
	U16	ItmCnt;			/* 资源项个数 */
	U32	ItmLen;			/* 资源项长度(0表示资源项不等长，后面会有资源项索引序列) */
	U8	ResKey;			/* 资源串加密钥匙(加密算法统一使用+ResKey) */
	U8	Reserved;		/* 保留 */
}RCHEAD;				/* 资源串头定义 */

typedef struct {
	U32	offset;			/* 资源项相对资源起始地址的偏移地址 */
	U32	rlen;			/* 资源项的数据长度 */
}RIDX;					/* 资源项索引结构 */

/*------------------------
  函数
  ------------------------*/
FAR U32 ResGetItemLen(U16 ResId,U16 idx);
FAR U8 *ResLoadToCon(U16 ResId,U16 idx,U8 *cbnk);
FAR U8 ResLoadToMem(U16 ResId,U16 idx,U8 *ptr);
FAR U8 ResItemGet(U16 ResId,U16 idx,U8 *ptr);
FAR U8 ResItemGetN(U16 ResId,U16 idx,U8 *ptr, U32 bufsize);
FAR U8 *ResLoadStringWithId(U16 ResId);

#endif	/* _RESOURCE_H */
