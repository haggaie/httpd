/* TODO add heading */

#ifndef UTIL_TM_H
#define UTIL_TM_H

#if defined(_TM)
# define SYNC_BEGIN(lock) __tm_atomic {
# define SYNC_END(lock) }
# define INIT_SYNC(lock) (void)0
# define TM_CALLABLE __attribute__((tm_callable))
# define TM_PURE __attribute__((tm_pure))
# define DECLARE_SYNC(lock) 

/*# define ATOMIC_DEC32(x) (_Pragma ("omp atomic") --(*(x)))
  # define ATOMIC_INC32(x) (_Pragma ("omp atomic") ++(*(x))) */
TM_CALLABLE inline apr_uint32_t ATOMIC_DEC32(apr_uint32_t* p) {
    apr_uint32_t res;
     __tm_atomic { res = --*p; }
    return res;
}

TM_CALLABLE inline apr_uint32_t ATOMIC_INC32(apr_uint32_t* p) {
    apr_uint32_t res;
     __tm_atomic { res = ++*p; }
    return res;
}

TM_CALLABLE inline apr_uint32_t ATOMIC_READ32(const apr_uint32_t* p) {
    apr_uint32_t res;
     __tm_atomic { res = *p; }
    return res;
}

# define ATOMIC_SET32(x,y) __tm_atomic { (*(x) = (y)); }
# define MEMCMP             tm_memcmp

TM_CALLABLE
inline int tm_memcmp(const void *s1, const void *s2, size_t n)
{
    const char  * p1 = s1,
                * p2 = s2;
    int i;
    for (i = 0; i < n; ++i)
    {
        if (p1[i] < p2[i]) return -1;
        else if (p1[i] > p2[i]) return 1;
    }
    return 0;
}

#else
/* Being compiled without TM */

#error No _TM defined

# define TM_CALLABLE
# define TM_PURE
# include "apr_thread_mutex.h"
# define SYNC_BEGIN(lock)  if (lock) { apr_thread_mutex_lock(lock); }
# define SYNC_END(lock)    if (lock) { apr_thread_mutex_unlock(lock); }
# define INIT_SYNC(lock)   apr_thread_mutex_create(&lock, APR_THREAD_MUTEX_DEFAULT, p)
# define DECLARE_SYNC(lock) apr_thread_mutex_t *lock

# define ATOMIC_DEC32(x)   apr_atomic_dec32(x)
# define ATOMIC_INC32(x)   apr_atomic_inc32(x)
# define ATOMIC_SET32(x,y)   apr_atomic_set32(x,y)
# define ATOMIC_READ32(x)   apr_atomic_read32(x)
# define MEMCMP             memcmp
#endif

#endif /* UTIL_TM_H */
