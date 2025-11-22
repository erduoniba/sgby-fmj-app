#ifndef GRAPH_H
#define GRAPH_H

#define	NO_MASK		0		/* 不带mask数据时mask的值 */
#define	HV_MASK		1		/* 带mask数据时mask的值 */
typedef struct PictureHead
{
	U16 wid;		/*图片宽*/
	U16 hig;		/*图片高*/
	U16 count;		/*图片数*/
	U8 mask;		/*高4为bpp；(mask & 0x01)为1时为透明图；(mask & 0x02)为0时为横向图，为1时为纵向图*/
}PictureHeadType;
#define	PICHEAD_LEN	sizeof(PictureHeadType)

#endif
