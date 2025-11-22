//
//  version.c
//  baye-ios
//
//  Created by loong on 17/7/1.
//
//

#include "baye/consdef.h"

#ifdef EMSCRIPTEN
#include "baye/version.h"
#else
#define BAYE_VERSION "2.0.0"
#endif

U8 g_engineVersion[32] = BAYE_VERSION;

