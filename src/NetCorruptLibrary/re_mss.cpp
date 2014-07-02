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
  *   FILE     re_mss.cpp
  *   VERSION  2.25
  *   Message helper functions, for regular
  *   expression library.
  */

#include <jm/re_mss.h>
#include <jm/re_str.h>
#include <jm/re_nls.h>
#include <jm/regex.h>
#include <jm/re_thrd.h>
#include <locale.h>
#include <stdio.h>

#ifndef JM_NO_CAT
#include <nl_types.h>
#endif


#ifndef RE_MESSAGE_BASE
#define RE_MESSAGE_BASE 0
#endif


JM_NAMESPACE(JM)

#ifdef RE_LOCALE_W32
HINSTANCE hresmod = 0;
#elif !defined(JM_NO_CAT)
nl_catd message_cat = (nl_catd)-1;
#endif

unsigned int message_count = 0;
re_str<char>* mess_locale;
JM_IX_DECL const char* regex_message_catalogue = 0;

struct mss
{
   unsigned int id;
   const char* what;
};

JM_IX_DECL const char *re_default_error_messages[] =
{  "Success",             /* REG_NOERROR */
   "No match",             /* REG_NOMATCH */
   "Invalid regular expression",    /* REG_BADPAT */
   "Invalid collation character",      /* REG_ECOLLATE */
   "Invalid character class name",     /* REG_ECTYPE */
   "Trailing backslash",         /* REG_EESCAPE */
   "Invalid back reference",        /* REG_ESUBREG */
   "Unmatched [ or [^",       /* REG_EBRACK */
   "Unmatched ( or \\(",         /* REG_EPAREN */
   "Unmatched \\{",           /* REG_EBRACE */
   "Invalid content of \\{\\}",     /* REG_BADBR */
   "Invalid range end",       /* REG_ERANGE */
   "Memory exhausted",           /* REG_ESPACE */
   "Invalid preceding regular expression",   /* REG_BADRPT */
   "Premature end of regular expression", /* REG_EEND */
   "Regular expression too big",    /* REG_ESIZE */
   "Unmatched ) or \\)",         /* REG_ERPAREN */
   "Empty expression",           /* REG_EMPTY */
   "Unknown error",    /* REG_E_UNKNOWN */
   "",
   "",
   "",
};

#ifndef RE_LOCALE_CPP

JM_IX_DECL char* re_custom_error_messages[] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
};

#endif

const mss default_messages[] = {
                              { 100+ syntax_open_bracket, "(", },
                              { 100+ syntax_close_bracket, ")", },
                              { 100+ syntax_dollar, "$", },
                              { 100+ syntax_caret, "^", },
                              { 100+ syntax_dot, ".", },
                              { 100+ syntax_star, "*", },
                              { 100+ syntax_plus, "+", },
                              { 100+ syntax_question, "?", },
                              { 100+ syntax_open_set, "[", },
                              { 100+ syntax_close_set, "]", },
                              { 100+ syntax_or, "|", },
                              { 100+ syntax_slash, "\\", },
                              { 100+ syntax_hash, "#", },
                              { 100+ syntax_dash, "-", },
                              { 100+ syntax_open_brace, "{", },
                              { 100+ syntax_close_brace, "}", },
                              { 100+ syntax_digit, "0123456789", },
                              { 100+ syntax_b, "b", },
                              { 100+ syntax_B, "B", },
                              { 100+ syntax_left_word, "<", },
                              { 100+ syntax_right_word, ">", },
                              { 100+ syntax_w, "w", },
                              { 100+ syntax_W, "W", },
                              { 100+ syntax_start_buffer, "`A", },
                              { 100+ syntax_end_buffer, "'z", },
                              { 100+ syntax_newline, "\n", },
                              { 100+ syntax_comma, ",", },
                              { 100+ syntax_a, "a", },
                              { 100+ syntax_f, "f", },
                              { 100+ syntax_n, "n", },
                              { 100+ syntax_r, "r", },
                              { 100+ syntax_t, "t", },
                              { 100+ syntax_v, "v", },
                              { 100+ syntax_x, "x", },
                              { 100+ syntax_c, "c", },
                              { 100+ syntax_colon, ":", },
                              { 100+ syntax_equal, "=", },

                              { 100 + syntax_e, "e", },
                              { 100 + syntax_l, "l", },
                              { 100 + syntax_L, "L", },
                              { 100 + syntax_u, "u", },
                              { 100 + syntax_U, "U", },
                              { 100 + syntax_s, "s", },
                              { 100 + syntax_S, "S", },
                              { 100 + syntax_d, "d", },
                              { 100 + syntax_D, "D", },
                              { 100 + syntax_E, "E", },
                              { 100 + syntax_Q, "Q", },
                              { 100 + syntax_X, "X", },
                              { 100 + syntax_C, "C", },
                              { 100 + syntax_Z, "Z", },
                              { 100 + syntax_G, "G", },

                            { 0, "", },
                         };

#ifndef RE_LOCALE_CPP

JM_IX_DECL unsigned int RE_CALL re_get_default_message(char* buf, unsigned int len, unsigned int id)
{
   JM_GUARD_STACK
   const mss* pm = default_messages;
   while(pm->id)
   {
      if(pm->id == id)
      {
         unsigned int size = (unsigned int)(re_strlen(pm->what) + 1);
         if(size > len)
            return size;
         re_strcpy(buf, pm->what);
         return size;
      }
      ++pm;
   }
   if(buf && len)
      *buf = 0;
   return 1;
}

JM_IX_DECL unsigned int RE_CALL _re_get_message(char* buf, unsigned int len, unsigned int id)
{
   JM_GUARD_STACK
   // get the customised message if any:
   #ifdef RE_LOCALE_W32
   if(len < 255)
      return 255;
   unsigned int size = 0;
   if(hresmod)
      size = LoadString(hresmod, RE_MESSAGE_BASE + id, buf, 255);
   if(size)
      return size;
   #elif !defined(JM_NO_CAT)
   if(message_cat != (nl_catd)-1)
   {
      const char* m = catgets(message_cat, 0, id, NULL);
      if(m)
      {
         unsigned int size = re_strlen(m) + 1;
         if(size > len)
            return size;
         re_strcpy(buf, m);
         return size;
      }
   }
   #endif

   //
   // now get the default message if any:
   return re_get_default_message(buf, len, id);
}

JM_IX_DECL void RE_CALL re_message_init()
{
   JM_GUARD_STACK
   if(message_count == 0)
   {
      mess_locale = new re_str<char>("xxxxxxxxxxxxxxxx");
   }
   ++message_count;
}

#if !defined(LC_MESSAGES) && defined(JM_NO_CAT)
#define LC_MESSAGES LC_CTYPE
#endif

JM_IX_DECL void RE_CALL re_message_update()
{
   JM_GUARD_STACK
   //
   // called whenever the global locale changes:
   //
   re_str<char> l(get_global_locale_name(LC_MESSAGES));
   if(*mess_locale != l)
   {
      *mess_locale = l;
#ifdef RE_LOCALE_W32
      if(hresmod)
         FreeLibrary(hresmod);
      if(regex_message_catalogue)
         hresmod = LoadLibraryA(regex_message_catalogue);
#elif !defined(JM_NO_CAT)
      if(message_cat != (nl_catd)-1)
      {
         catclose(message_cat);
         message_cat = (nl_catd)-1;
      }
      if(regex_message_catalogue)
         message_cat = catopen(regex_message_catalogue, 0);
#endif
      for(int i = 0; i < REG_E_UNKNOWN; ++i)
      {
         if(re_custom_error_messages[i])
         {
            re_strfree(re_custom_error_messages[i]);
            re_custom_error_messages[i] = 0;
         }
      }
   }
}

JM_IX_DECL void RE_CALL re_message_free()
{
   JM_GUARD_STACK
   --message_count;
   if(message_count == 0)
   {
#ifdef RE_LOCALE_W32
      if(hresmod)
         FreeLibrary(hresmod);
#elif !defined(JM_NO_CAT)
      if(message_cat != (nl_catd)-1)
         catclose(message_cat);
#endif
      delete mess_locale;
      for(int i = 0; i < REG_E_UNKNOWN; ++i)
      {
         if(re_custom_error_messages[i])
         {
            re_strfree(re_custom_error_messages[i]);
            re_custom_error_messages[i] = 0;
         }
      }
   }
}

JM_IX_DECL const char* RE_CALL re_get_error_str(unsigned int id)
{
   JM_GUARD_STACK
#ifdef JM_THREADS
   cs_guard g(*p_re_lock);
#endif
   if(re_custom_error_messages[id] == 0)
   {
      char buf[256];
      _re_get_message(buf, 256, id + 200);
      if(*buf)
      {
         re_custom_error_messages[id] = re_strdup(buf);
         return re_custom_error_messages[id];
      }
      return re_default_error_messages[id];
   }
   return re_custom_error_messages[id];
}

#endif

#ifdef RE_LOCALE_CPP

JM_IX_DECL JM_STD::messages<char>::string_type RE_CALL re_get_def_message(unsigned int i)
{
   JM_GUARD_STACK
   const mss* p = default_messages;
   while(p->id)
   {
      if(p->id == i)
         return p->what;
      ++p;
   }

   return JM_STD::messages<char>::string_type();
}

#ifndef JM_NO_WCSTRING
JM_IX_DECL JM_STD::messages<wchar_t>::string_type RE_CALL re_get_def_message_w(unsigned int i)
{
   JM_GUARD_STACK
   const mss* p = default_messages;
   const char* pb = 0;
   while(p->id)
   {
      if(p->id == i)
      {
         pb = p->what;
         break;
      }
      ++p;
   }
   if(pb == 0)
      return JM_STD::messages<wchar_t>::string_type();

   unsigned int size = re_strwiden((wchar_t*)0, 0, pb);
   auto_array<wchar_t> buf(new wchar_t[size]);
   re_strwiden((wchar_t*)buf, size, pb);
   JM_STD::messages<wchar_t>::string_type s((wchar_t*)buf);
   return s;
}
#endif

JM_IX_DECL JM_STD::string RE_CALL re_get_error_str(unsigned int id, const JM_STD::locale& l)
{
   JM_GUARD_STACK
   JM_STD::string s = re_default_error_messages[id];
   JM_STD::messages<char>::catalog cat = regex_message_catalogue ? JM_USE_FACET(l, JM_STD::messages<char>).open(regex_message_catalogue, l) : -1;
   if((int)cat >= 0)
   {
      s = JM_USE_FACET(l, JM_STD::messages<char>).get(cat, 0, id+200, s);
      JM_USE_FACET(l, JM_STD::messages<char>).close(cat);
   }
   return s;
}

#endif

JM_END_NAMESPACE






