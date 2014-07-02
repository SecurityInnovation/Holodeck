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
  *   FILE     re_coll.h
  *   VERSION  2.25
  *   This is an internal header file, do not include directly
  */
  
#ifndef RE_COLL_H
#define RE_COLL_H

#ifndef JM_CFG_H
#include <jm/jm_cfg.h>
#endif

#ifndef RE_STR_H
#include <re_str.h>
#endif

JM_NAMESPACE(JM)

#ifdef __BORLANDC__
   #if __BORLANDC__ == 0x530
    #pragma option push -a4 -b
   #elif __BORLANDC__ > 0x530
    #pragma option push -a8 -b
   #endif
#endif

JM_IX_DECL bool RE_CALL re_lookup_def_collate_name(re_str<char>& buf, const char* name);

void RE_CALL re_init_collate();
void RE_CALL re_free_collate();
void RE_CALL re_update_collate();
JM_IX_DECL bool RE_CALL _re_lookup_collate(re_str<char>& buf, const char* p);

inline bool RE_CALL re_lookup_collate(re_str<char>& buf, const char* first, const char* last)
{
   re_str<char> s(first, last);
   return _re_lookup_collate(buf, s.c_str());
}

#ifndef JM_NO_WCSTRING
JM_IX_DECL bool RE_CALL re_lookup_collate(re_str<wchar_t>& out, const wchar_t* first, const wchar_t* last);
#endif

#ifdef __BORLANDC__
 #if __BORLANDC__ > 0x520
  #pragma option pop
 #endif
#endif

JM_END_NAMESPACE

#endif

