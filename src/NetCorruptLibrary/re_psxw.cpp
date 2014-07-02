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
  *   FILE     re_psxw.cpp
  *   VERSION  2.25
  */

#include <jm/jm_cfg.h>

#ifndef JM_NO_WCSTRING

#include <jm/regex.h>
#include <jm/re_mss.h>

#ifndef JM_NO_WCHAR_H
#include <wchar.h>
#endif
#ifndef JM_NO_WCTYPE_H
#include <wctype.h>
#endif
#include <string.h>
#include <stdio.h>

#if defined(JM_NO_SWPRINTF) && !defined(JM_NO_WSPRINTF)
#include <widec.h>
#endif


unsigned int wmagic_value = 28631;

JM_IX_DECL int RE_CCALL regcompW(regex_tW* expression, const wchar_t* ptr, int f)
{
   JM_GUARD_STACK
   if(expression->re_magic != wmagic_value)
   {
#ifndef JM_NO_EXCEPTIONS
      expression->guts = 0;
      try{
#endif
      expression->guts = new wregex();
#ifndef JM_NO_EXCEPTIONS
      } catch(...)
      {
         delete (wregex*)(expression->guts);
         return REG_ESPACE;
      }
#endif
   }
   // set default flags:
   jm_uintfast32_t flags = (f & REG_EXTENDED) ? regbase::extended : regbase::basic;
   expression->eflags = (f & REG_NEWLINE) ? match_not_dot_newline : 0;

   // and translate those that are actually set:
   if(f & REG_NOCOLLATE)
      flags |= regbase::nocollate;

   if(f & REG_NOSUB)
      expression->eflags |= match_any;

   if(f & REG_NOSPEC)
      flags |= regbase::literal;
   if(f & REG_ICASE)
      flags |= regbase::icase;

   const wchar_t* p2;
   if(f & REG_PEND)
      p2 = expression->re_endp;
   else p2 = ptr + wcslen(ptr);

   int result;

#ifndef JM_NO_EXCEPTIONS
   try{
#endif
      expression->re_magic = wmagic_value;
      ((wregex*)(expression->guts))->set_expression(ptr, p2, flags);
      expression->re_nsub = ((wregex*)(expression->guts))->mark_count() - 1;
      result = ((wregex*)(expression->guts))->error_code();
#ifndef JM_NO_EXCEPTIONS
   } catch(...)
   {
      result = REG_E_UNKNOWN;
   }
#endif
   if(result)
      regfreeW(expression);
   return result;

}

const wchar_t* wnames[] = {L"REG_NOERROR", L"REG_NOMATCH", L"REG_BADPAT", L"REG_ECOLLATE",
                        L"REG_ECTYPE", L"REG_EESCAPE", L"REG_ESUBREG", L"REG_EBRACK",
                        L"REG_EPAREN", L"REG_EBRACE", L"REG_BADBR", L"REG_ERANGE",
                        L"REG_ESPACE", L"REG_BADRPT", L"REG_EMPTY", L"REG_E_UNKNOWN"};


JM_IX_DECL unsigned int RE_CCALL regerrorW(int code, const regex_tW* e, wchar_t* buf, unsigned int buf_size)
{
   JM_GUARD_STACK
   unsigned int result = 0;
   if(code & REG_ITOA)
   {
      code &= ~REG_ITOA;
      if(code <= REG_E_UNKNOWN)
      {
         result = (unsigned int)(wcslen(wnames[code]) + 1);
         if(buf_size >= result)
            wcscpy(buf, wnames[code]);
         return result;
      }
      return result;
   }
#if !defined(JM_NO_SWPRINTF) || !defined(JM_NO_WSPRINTF)
   if(code == REG_ATOI)
   {
      wchar_t localbuf[5];
      if(e == 0)
         return 0;
      for(int i = 0; i <= REG_E_UNKNOWN; ++i)
      {
         if(wcscmp(e->re_endp, wnames[i]) == 0)
         {
#if defined(JM_NO_SWPRINTF)
            wsprintf(localbuf, "%d", i);
#else
            swprintf(localbuf, L"%d", i);
#endif
            if(wcslen(localbuf) < buf_size)
               wcscpy(buf, localbuf);
            return (unsigned int)(wcslen(localbuf) + 1);
         }
      }
#if defined(JM_NO_SWPRINTF)
      wsprintf(localbuf, "%d", 0);
#else
      swprintf(localbuf, L"%d", 0);
#endif
      if(wcslen(localbuf) < buf_size)
         wcscpy(buf, localbuf);
      return (unsigned int)(wcslen(localbuf) + 1);
   }
#endif
   if(code <= REG_E_UNKNOWN)
   {
#ifdef RE_LOCALE_CPP
      JM_STD::string p = JM::re_get_error_str(code, JM_STD::locale());
      unsigned int len = JM::re_strwiden((wchar_t*)0, 0, p.c_str());
      if(len < buf_size)
      {
         JM::re_strwiden(buf, buf_size, p.c_str());
      }
      return len + 1;
#else
      const char* p = JM::re_get_error_str(code);
      unsigned int len = JM::re_strwiden((wchar_t*)0, 0, p);
      if(len < buf_size)
      {
         JM::re_strwiden(buf, buf_size, p);
      }
      return len + 1;
#endif
   }
   if(buf_size)
      *buf = 0;
   return 0;
}

JM_IX_DECL int RE_CCALL regexecW(const regex_tW* expression, const wchar_t* buf, unsigned int n, regmatch_t* array, int eflags)
{
   JM_GUARD_STACK
   bool result = false;
   jm_uintfast32_t flags = match_default | expression->eflags;
   const wchar_t* end;
   const wchar_t* start;
   wcmatch m;
   
   if(eflags & REG_NOTBOL)
      flags |= match_not_bol;
   if(eflags & REG_NOTEOL)
      flags |= match_not_eol;
   if(eflags & REG_STARTEND)
   {
      start = buf + array[0].rm_so;
      end = buf + array[0].rm_eo;
   }
   else
   {
      start = buf;
      end = buf + wcslen(buf);
   }

#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   if(expression->re_magic == wmagic_value)
   {
      result = reg_search(start, end, m, *(wregex*)(expression->guts), flags);
   }
   else
      return result;
#ifndef JM_NO_EXCEPTIONS
   } catch(...)
   {
      return REG_E_UNKNOWN;
   }
#endif

   if(result)
   {
      // extract what matched:
     unsigned int i;
      for(i = 0; (i < n) && (i < expression->re_nsub + 1); ++i)
      {
         array[i].rm_so = (regoff_t)((m[i].matched == false) ? -1 : (m[i].first - buf));
         array[i].rm_eo = (regoff_t)((m[i].matched == false) ? -1 : (m[i].second - buf));
      }
      // and set anything else to -1:
      for(i = expression->re_nsub + 1; i < n; ++i)
      {
         array[i].rm_so = -1;
         array[i].rm_eo = -1;
      }
      return 0;
   }
   return REG_NOMATCH;
}

JM_IX_DECL void RE_CCALL regfreeW(regex_tW* expression)
{
   JM_GUARD_STACK
   if(expression->re_magic == wmagic_value)
   {
      delete (wregex*)(expression->guts);
   }
   expression->re_magic = 0;
}

#endif


