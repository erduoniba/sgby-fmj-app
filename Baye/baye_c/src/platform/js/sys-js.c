//
//  sys.c
//  baye-ios
//
//  Created by loong on 16/11/8.
//
//


#include "baye/compa.h"
#include "baye/comm.h"
#include <emscripten.h>

FAR U8 SysGetSecond(void)
{
    return (U8)EM_ASM_INT({
               var now = new Date();
               return now.getTime();
           }, 0);
}
