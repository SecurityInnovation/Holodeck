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
  *   FILE     re_psx.cpp
  *   VERSION  2.25
  */
  
#include <jm/regex.h>
#include <jm/re_mss.h>

#include <stdio.h>

unsigned int magic_value = 25631;

JM_IX_DECL int RE_CCALL regcompA(regex_tA* expression, const char* ptr, int f)
{
   JM_GUARD_STACK
   if(expression->re_magic != magic_value)
   {
#ifndef JM_NO_EXCEPTIONS
      expression->guts = 0;
      try{
#endif
      expression->guts = new regex();
#ifndef JM_NO_EXCEPTIONS
      } catch(...)
      {
         delete (regex*)(expression->guts);
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

   const char* p2;
   if(f & REG_PEND)
      p2 = expression->re_endp;
   else p2 = ptr + strlen(ptr);

   int result;

#ifndef JM_NO_EXCEPTIONS
   try{
#endif
      expression->re_magic = magic_value;
      ((regex*)(expression->guts))->set_expression(ptr, p2, flags);
      expression->re_nsub = ((regex*)(expression->guts))->mark_count() - 1;
      result = ((regex*)(expression->guts))->error_code();
#ifndef JM_NO_EXCEPTIONS
   } catch(...)
   {
      result = REG_E_UNKNOWN;
   }
#endif
   if(result)
      regfreeA(expression);
   return result;

}

const char* names[] = {"REG_NOERROR", "REG_NOMATCH", "REG_BADPAT", "REG_ECOLLATE",
                        "REG_ECTYPE", "REG_EESCAPE", "REG_ESUBREG", "REG_EBRACK",
                        "REG_EPAREN", "REG_EBRACE", "REG_BADBR", "REG_ERANGE",
                        "REG_ESPACE", "REG_BADRPT", "REG_EMPTY", "REG_E_UNKNOWN"};


JM_IX_DECL unsigned int RE_CCALL regerrorA(int code, const regex_tA* e, char* buf, unsigned int buf_size)
{
   JM_GUARD_STACK
   unsigned int result = 0;
   if(code & REG_ITOA)
   {
      code &= ~REG_ITOA;
      if(code <= REG_E_UNKNOWN)
      {
         result = (unsigned int)(strlen(names[code]) + 1);
         if(buf_size >= result)
            strcpy(buf, names[code]);
         return result;
      }
      return result;
   }
   if(code == REG_ATOI)
   {
      char localbuf[5];
      if(e == 0)
         return 0;
      for(int i = 0; i <= REG_E_UNKNOWN; ++i)
      {
         if(strcmp(e->re_endp, names[i]) == 0)
         {
            sprintf(localbuf, "%d", i);
            if(strlen(localbuf) < buf_size)
               strcpy(buf, localbuf);
            return (unsigned int)(strlen(localbuf) + 1);
         }
      }
      sprintf(localbuf, "%d", 0);
      if(strlen(localbuf) < buf_size)
         strcpy(buf, localbuf);
      return (unsigned int)(strlen(localbuf) + 1);
   }
   if(code <= REG_E_UNKNOWN)
   {
#ifdef RE_LOCALE_CPP
      JM_STD::string p = JM::re_get_error_str(code, JM_STD::locale());
      unsigned int len = p.size();
      if(len < buf_size)
      {
         strcpy(buf, p.c_str());
      }
      return len + 1;
#else
      const char* p = JM::re_get_error_str(code);
      unsigned int len = (unsigned int)(strlen(p));
      if(len < buf_size)
      {
         strcpy(buf, p);
      }
      return len + 1;
#endif
   }
   if(buf_size)
      *buf = 0;
   return 0;
}

JM_IX_DECL int RE_CCALL regexecA(const regex_tA* expression, const char* buf, unsigned int n, regmatch_t* array, int eflags)
{
   JM_GUARD_STACK
   bool result = false;
   jm_uintfast32_t flags = match_default | expression->eflags;
   const char* end;
   const char* start;
   cmatch m;
   
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
      end = buf + strlen(buf);
   }

#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   if(expression->re_magic == magic_value)
   {
      result = reg_search(start, end, m, *(regex*)(expression->guts), flags);
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

JM_IX_DECL void RE_CCALL regfreeA(regex_tA* expression)
{
   JM_GUARD_STACK
   if(expression->re_magic == magic_value)
   {
      delete (regex*)(expression->guts);
   }
   expression->re_magic = 0;
}




