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
  *   FILE     regex.cpp
  *   VERSION  2.25
  */


#include <regex.h>
#include <jm/re_mss.h>



JM_NAMESPACE(JM)

#if !defined(JM_NO_EXCEPTIONS)

const char* bad_expression::what() const throw()
{
   JM_GUARD_STACK
#ifdef RE_LOCALE_CPP
   return code.c_str();
#else
   return re_get_error_str(code);
#endif
}

#endif

#ifdef RE_LOCALE_CPP
JM_STD::locale RE_CALL regbase::imbue(const JM_STD::locale& l)
{
   JM_GUARD_STACK
   JM_STD::locale t(locale_inst);
   locale_inst = l;
   return t;
}
#endif

regbase::regbase()
{
   JM_GUARD_STACK
   _flags = regbase::failbit;
   code = REG_EMPTY;
}

regbase::regbase(const regbase& b) :
#ifdef RE_LOCALE_CPP
   locale_inst(b.locale_inst),
#endif
   _flags(b._flags), code(b.code)
{
}


void RE_CALL regbase::fail(unsigned int err)
{
   JM_GUARD_STACK
   code = err;
   if(code)
   {
      _flags |= failbit;
#ifndef JM_NO_EXCEPTIONS
#ifdef RE_LOCALE_CPP
      if(_flags & use_except)
      {
         throw bad_expression(re_get_error_str(err, locale_inst));
      }
#else
      if(_flags & use_except)
         throw bad_expression(code);
#endif
#endif
   }
   else
      _flags &= ~(regbase::failbit);
}



JM_END_NAMESPACE

#ifdef JM_USE_VCL
#if __BORLANDC__ <= 0x530
#include <vcl.h>
#endif

int WINAPI DllEntryPoint(HINSTANCE , unsigned long , void*)
{
   return 1;
}
#endif






