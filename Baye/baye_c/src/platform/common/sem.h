//
//  sem.h
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//

#ifndef __baye_ios__lock__
#define __baye_ios__lock__

#include <stdio.h>

typedef void* GAM_SEM;
typedef void* GAM_LOCK;

GAM_SEM gam_sem_create();
void gam_sem_delete(GAM_SEM sem);
void gam_sem_wait(GAM_SEM sem);
void gam_sem_signal(GAM_SEM sem);

GAM_LOCK gam_lock_create();
void gam_lock_delete(GAM_LOCK lck);
void gam_lock_lock(GAM_LOCK lck);
void gam_lock_unlock(GAM_LOCK lck);

#endif /* defined(__baye_ios__lock__) */
