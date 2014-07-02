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
  *   FILE     re_cls.cpp
  *   VERSION  2.25
  *   Class type lookup functions, for regular
  *   expression library.
  */

#include <jm/jm_cfg.h>
#include <jm/re_mss.h>
#include <jm/re_str.h>
#include <jm/re_nls.h>
#include <jm/regex.h>
#include <jm/re_thrd.h>
#include <jm/re_lst.h>
#include <jm/re_cls.h>
#include <locale.h>
#include <stdio.h>


JM_NAMESPACE(JM)

jm_uintfast32_t re_char_class_id[] = {
   char_class_alnum,
   char_class_alpha,
   char_class_cntrl,
   char_class_digit,
   char_class_graph,
   char_class_lower,
   char_class_print,
   char_class_punct,
   char_class_space,
   char_class_upper,
   char_class_xdigit,
   char_class_blank,
   char_class_word,
   char_class_unicode,
};

const char* re_char_class_names[] = {
"alnum",
"alpha",
"cntrl",
"digit",
"graph",
"lower",
"print",
"punct",
"space",
"upper",
"xdigit",
"blank",
"word",
"unicode",
};

#ifndef RE_LOCALE_CPP

re_str<char>* re_cls_name;
re_str<char>* pclasses;
unsigned int classes_count = 0;

void RE_CALL re_init_classes()
{
   JM_GUARD_STACK
   if(classes_count == 0)
   {
      re_cls_name = new re_str<char>("xxxxxxxx");
      pclasses = new re_str<char>[re_classes_max];
   }
   ++classes_count;
}

void RE_CALL re_free_classes()
{
   JM_GUARD_STACK
   if(--classes_count == 0)
   {
      delete re_cls_name;
      delete[] pclasses;
   }
}

void RE_CALL re_update_classes()
{
   JM_GUARD_STACK
   if(*re_cls_name != get_global_locale_name(LC_CTYPE))
   {
      *re_cls_name = get_global_locale_name(LC_CTYPE);
      char buf[256];
      unsigned int i;
      for(i = 0; i < re_classes_max; ++i)
      {
         re_get_message(buf, 256, i+300);
         pclasses[i] = buf;
      }
   }
}

JM_IX_DECL jm_uintfast32_t RE_CALL _re_lookup_class(const char* p)
{
   JM_GUARD_STACK
   unsigned int i;
   for(i = 0; i < re_classes_max; ++i)
   {
      if(pclasses[i] == p)
      {
         return re_char_class_id[i];
      }
   }
   for(i = 0; i < re_classes_max; ++i)
   {
      if(strcmp(re_char_class_names[i], p) == 0)
      {
         return re_char_class_id[i];
      }
   }
   return 0;
}

#else
#endif

JM_END_NAMESPACE


