#include <sem.h>
#include <stdlib.h>
#include <pthread.h>

GAM_LOCK gam_lock_create()
{
    pthread_mutex_t *lock = (pthread_mutex_t *)malloc(sizeof(*lock));
    pthread_mutex_init(lock, NULL);
    return (GAM_LOCK)lock;
}

void gam_lock_delete(GAM_LOCK lck)
{
    //pthread_mutex_destory((pthread_mutex_t *)lck);
    free(lck);
}

void gam_lock_lock(GAM_LOCK lck)
{
    pthread_mutex_lock((pthread_mutex_t *)lck);
}

void gam_lock_unlock(GAM_LOCK lck)
{
    pthread_mutex_unlock((pthread_mutex_t *)lck);
}

GAM_SEM gam_sem_create()
{
    GAM_SEM sem = gam_lock_create();
    gam_lock_lock(sem);
    return sem;
}

void gam_sem_delete(GAM_SEM sem)
{
    gam_lock_delete(sem);
}

void gam_sem_wait(GAM_SEM sem)
{
    gam_lock_lock(sem);
}

void gam_sem_signal(GAM_SEM sem)
{
    gam_lock_unlock(sem);
}
