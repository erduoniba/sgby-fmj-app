#include	"baye/stdsys.h"
#include	"baye/comm.h"
#undef	Debug
#define	Debug
#include	"baye/fundec.h"
#include <stdio.h>

FAR void	gamTraceP(U32	num)
{
    printf("gamTraceP %d\n", num);
    fflush(stdin);
    exit(0);
}
FAR void	gamTracel(U32	num)
{
    U8		i ;

    for(i=0;i<8;i = i+1)
        SysAscii(8*i,0,' ');

    gamPrintNum(0,0,num,16,0);
}
/****************************************************************************************
 * 函数名   : FAR	void		gamPrintNum(U8	x,U8	y,long num, U8 base,U8	sign)
 * 说  明   : 数字打印函数
 * 输入参数 : U8	x,U8	y ：	坐标
 long num	:		数字
 U8 base		:		进制
 U8	sign	:		符号
 *
 * 返回值  :
 * 修改历史:
 *		姓名		日期			说明
 *		----		----			-----------
 *		任新村		2002.08.21		基本功能完成
 ****************************************************************************************/
const	U8	gamNumConvTable[]="0123456789ABCDEF";
FAR void	gamPrintNum(PT	x,PT	y, U32 num, U8 base,U8	sign)
{
    U32		n;
    U32		count ;
    int		index;
    U8		toPrint[20];

    if ( (sign == 1) && ((long)num <  0) )
    {
        SysAscii(x,y,'-');
        x = x+8 ;
        num = -num;
    }

    count = 0;
    while ((n = num / base) > 0) {
        toPrint[count] = (num - (n*base));
        count = count + 1;
        num = n ;
    }
    toPrint[count] = num;
    count = count + 1 ;

    for (n = 0; n < count; n=n+1){
        index = (int)(toPrint[count-(n+1)]) ;
        SysAscii(x,y,gamNumConvTable[index] );
        x = x+8 ;
    }
}
FAR void	gamPause()
{
    MsgType gMsg;

    do
    {
        gMsg.type=WM_KEY;
        gMsg.param = SysGetKey();
        if(!GuiTranslateMsg(&gMsg))	continue;
        if(gMsg.type!=WM_CHAR_FUN)	continue;
        if(gMsg.param==CHAR_ENTER || gMsg.param==CHAR_EXIT)
            break;
    }while(1);
}
