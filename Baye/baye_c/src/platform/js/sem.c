//
//  sem.c
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//


#include "sem.h"
#include <stdlib.h>
#include <emscripten.h>
#include <baye/compa.h>

EM_JS(GAM_SEM, gam_sem_create, (), {
  if (!Module.sems) {
    Module.sems = []
  }
  for (var i = 0;; i++) {
    if (Module.sems[i] == undefined) {
      Module.sems[i] = {
        cnt: 0,
      };
      return i;
    }
  }
});

EM_JS(void, gam_sem_delete, (GAM_SEM semid), {
  Module.sems[semid] = undefined;
});


EM_JS(void, gam_sem_signal, (GAM_SEM semid), {
  var sem = Module.sems[semid];
  if (!sem) {
    console.warn('[gam_sem_signal] semaphore ' + semid + ' is undefined or deleted');
    return;
  }
  sem.cnt += 1;
  if (sem.signal) {
    sem.signal();
    sem.signal = null;
  }
});

EM_JS(void, gam_sem_wait, (GAM_SEM semid), {
  return Asyncify.handleSleep(function (wakeUp) {
    var sem = Module.sems[semid];
    if (!sem) {
      console.warn('[gam_sem_wait] semaphore ' + semid + ' is undefined or deleted');
      setTimeout(wakeUp, 0);
      return;
    }
    function signal() {
        sem.cnt -= 1;
        setTimeout(wakeUp, 0);
    }
    if (sem.cnt > 0) {
        signal();
    } else {
        sem.signal = signal;
    }
  });
});

GAM_LOCK gam_lock_create() {
    return NULL;
}
void gam_lock_delete(GAM_LOCK lck) {}
void gam_lock_lock(GAM_LOCK lck) {}
void gam_lock_unlock(GAM_LOCK lck) {}
