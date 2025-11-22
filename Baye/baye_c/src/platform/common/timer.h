//
//  timer.h
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//

#ifndef baye_ios_timer_h
#define baye_ios_timer_h

#include "baye/stdsys.h"


void gam_timer_init();
void gam_timer_set_callback(U8 n, void(*cb)());
int gam_timer_open(U8 n, int interval);
void gam_timer_close(U8 n);
U8 gam_check_timer_on(U8 n);
int gam_timer_interval(U8 n);
void gam_timer_set_interval(U8 n, int interval);

void gam_timer2_open(int interval, void(*callback)());

#endif
