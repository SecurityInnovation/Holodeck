
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
  *   FILE     re_strw.cpp
  *   VERSION  2.25
  */

#include <jm/re_str.h>
#ifdef JM_PLATFORM_W32
#include <windows.h>
#endif

JM_NAMESPACE(JM)

#if !defined(JM_NO_WCSTRING) || defined(JM_PLATFORM_W32)

JM_IX_DECL unsigned int RE_CALL _re_strnarrow(char *s1, unsigned int len, const wchar_t *s2)
{
   JM_GUARD_STACK
#ifdef JM_PLATFORM_W32
   unsigned int size = WideCharToMultiByte(CP_ACP, 0, s2, -1, s1, 0, NULL, NULL);
   if(size > len)
      return size;
   return WideCharToMultiByte(CP_ACP, 0, s2, -1, s1, len, NULL, NULL);
#else
   unsigned int size = re_strlen(s2) + 1;
   if(size > len)
      return size;
   return wcstombs(s1, s2, len);
#endif
}

JM_IX_DECL unsigned int RE_CALL _re_strwiden(wchar_t *s1, unsigned int len, const char *s2)
{
   JM_GUARD_STACK
#ifdef JM_PLATFORM_W32
   unsigned int size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,   s2, -1, s1, 0);
   if(size > len)
      return size;
   return MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s2, -1, s1, len);
#else
   unsigned int size = re_strlen(s2) + 1;
   if(size > len)
      return size;
   size = mbstowcs(s1, s2, len);
   s1[size] = 0;
   return size + 1;
#endif
}

#endif




JM_END_NAMESPACE
