//
//  itoa.c
//  baye-ios
//
//  Created by loong on 15/8/12.
//
//

// ref: http://baike.baidu.com/link?url=bvt24BXRjzFt3zMMXh8xIItmVdbXikNWtdKV-TMPwJlxq5AlGZUWNuS2a0Ri3AzT8eLnlR9bFjTgQ1lsKAIsPa

#include "baye/itoa.h"

char* ltoa(long num, char*str, int radix)
{/*索引表*/
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;/*中间变量*/
    int i=0,j,k;
    /*确定unum的值*/
    if(radix==10&&num<0)/*十进制负数*/
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num;/*其他情况*/
    /*转换*/
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    } while(unum);
    str[i]='\0';
    /*逆序*/
    if(str[0]=='-')k=1;/*十进制负数*/
    else k=0;
    char temp;
    for(j=k;j<=(i-1)/2;j++)
    {
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
    return str;
}

char* itoa(int num, char*str, int radix)
{
    return ltoa(num, str, radix);
}