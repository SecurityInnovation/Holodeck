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
  *   FILE     re_kmp.h
  *   VERSION  2.25
  *   Knuth-Morris-Pratt search.
  */


#ifndef _RE_KMP_H
#define _RE_KMP_H

#ifdef JM_CFG_H
#include <jm/jm_cfg.h>
#endif


JM_NAMESPACE(JM)

#ifdef __BORLANDC__
   #if __BORLANDC__ == 0x530
    #pragma option push -a4 -b
   #elif __BORLANDC__ > 0x530
    #pragma option push -a8 -b
   #endif
#endif

template <class charT>
struct kmp_info
{
   unsigned int size;
   unsigned int len;
   const charT* pstr;
   int kmp_next[1];
};

template <class charT, class Allocator>
void kmp_free(kmp_info<charT>* pinfo, Allocator a)
{
   typedef JM_MAYBE_TYPENAME REBIND_TYPE(char, Allocator) atype;
   atype(a).deallocate((char*)pinfo, pinfo->size);
}

template <class iterator, class charT, class Trans, class Allocator>
kmp_info<charT>* kmp_compile(iterator first, iterator last, charT, Trans translate, Allocator a
#ifdef RE_LOCALE_CPP
                             , const JM_STD::locale& l
#endif
                             ) 
{    
   typedef JM_MAYBE_TYPENAME REBIND_TYPE(char, Allocator) atype;
   int i, j, m;
   i = 0;
   m = 0;
   JM_DISTANCE(first, last, m);
   ++m;
   unsigned int size = sizeof(kmp_info<charT>) + sizeof(int)*m + sizeof(charT)*m;
   --m;
   //
   // allocate struct and fill it in:
   //
   kmp_info<charT>* pinfo = (kmp_info<charT>*)atype(a).allocate(size);
   pinfo->size = size;
   pinfo->len = m;
   charT* p = (charT*)((char*)pinfo + sizeof(kmp_info<charT>) + sizeof(int)*(m+1));
   pinfo->pstr = p;
   while(first != last)
   {
      *p = translate(*first MAYBE_PASS_LOCALE(l));
      ++first;
      ++p;
   }
   *p = 0;
   //
   // finally do regular kmp compile:
   //
   j = pinfo->kmp_next[0] = -1;
   while (i < m) 
   {
      while ((j > -1) && (pinfo->pstr[i] != pinfo->pstr[j])) 
         j = pinfo->kmp_next[j];
      ++i;
      ++j;
      if (pinfo->pstr[i] == pinfo->pstr[j]) 
         pinfo->kmp_next[i] = pinfo->kmp_next[j];
      else 
         pinfo->kmp_next[i] = j;
   }

   return pinfo;
}

#ifdef __BORLANDC__
 #if __BORLANDC__ > 0x520
  #pragma option pop
 #endif
#endif

JM_END_NAMESPACE   // namespace regex

#endif   // _RE_KMP_H





