/*
 *
 * Copyright (c) 1998-9
 * Dr John Maddock
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Dr John Maddock makes no representations
 * about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 */

 /*
  *   FILE     re_thrd.cpp
  *   VERSION  2.25
  *   Thread synch helper functions, for regular
  *   expression library.
  */

#include <jm/re_thrd.h>  

JM_NAMESPACE(JM)

void RE_CALL re_init_threads()
{
   JM_GUARD_STACK
#ifdef JM_THREADS
   if(p_re_lock == 0)
      p_re_lock = new critical_section();
   cs_guard g(*p_re_lock);
   ++re_lock_count;
#endif
}

void RE_CALL re_free_threads()
{
   JM_GUARD_STACK
#ifdef JM_THREADS
   cs_guard g(*p_re_lock);
   --re_lock_count;
   if(re_lock_count == 0)
   {
      g.acquire(false);
      delete p_re_lock;
      p_re_lock = 0;
   }
#endif
}

#ifdef JM_THREADS

JM_IX_DECL critical_section* p_re_lock = 0;

JM_IX_DECL unsigned int re_lock_count = 0;

#endif

JM_END_NAMESPACE

