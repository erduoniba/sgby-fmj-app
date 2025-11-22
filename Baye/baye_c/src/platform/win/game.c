//
//  main.c
//  baye-ios
//
//  Created by loong on 16/6/29.
//
//

#include "inc/dictsys.h"
#include "baye/comm.h"
#include "baye/bind-objects.h"
#include "baye/data-bind.h"
#include "baye/script.h"

#include "../js/exportjs.c"

#include <pthread.h>

FAR U8 GamConInit(void);
FAR void GamBaYeEng(void);

static void baye_init_for_win(void) {
    GamSetResourcePath((U8*)"dat.lib", (U8*)".\\");
    // GamSetAltLibPath((U8*)"dat.lib");
    GamSetDataDir((U8*)".");
    if (GamConInit()) {
        printf("init res failed!\n");
        exit(0);
    }
}

void baye_init_for_js(void) {
}

static void* bayeMain(void*_)
{
    baye_init_for_win();
    GamBaYeEng();
    exit(0);
    return NULL;
}

void bayeStart()
{
    pthread_t t;
    pthread_create(&t, NULL, bayeMain, NULL);
}
