

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
  *   FILE     re_coll.cpp
  *   VERSION  2.25
  */

#include <jm/re_nls.h>
#include <jm/re_str.h>
#include <jm/re_mss.h>
#include <jm/re_lst.h>
#include <jm/re_coll.h>

#include <locale.h>
#include <stdio.h>
#include <ctype.h>

JM_NAMESPACE(JM)

//
// these are the POSIX collating names:
//
const char* def_coll_names[] = {
"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "alert", "backspace", "tab", "newline", 
"vertical-tab", "form-feed", "carriage-return", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", 
"SYN", "ETB", "CAN", "EM", "SUB", "ESC", "IS4", "IS3", "IS2", "IS1", "space", "exclamation-mark", 
"quotation-mark", "number-sign", "dollar-sign", "percent-sign", "ampersand", "apostrophe", 
"left-parenthesis", "right-parenthesis", "asterisk", "plus-sign", "comma", "hyphen", 
"period", "slash", "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", 
"colon", "semicolon", "less-than-sign", "equals-sign", "greater-than-sign", 
"question-mark", "commercial-at", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", 
"Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "left-square-bracket", "backslash", 
"right-square-bracket", "circumflex", "underscore", "grave-accent", "a", "b", "c", "d", "e", "f", 
"g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "left-curly-bracket", 
"vertical-line", "right-curly-bracket", "tilde", "DEL", "", 
};

// these multi-character collating elements
// should keep most Western-European locales
// happy - we should really localise these a
// little more - but this will have to do for
// now:

const char* def_multi_coll[] = {
   "ae",
   "Ae",
   "AE",
   "ch",
   "Ch",
   "CH",
   "ll",
   "Ll",
   "LL",
   "ss",
   "Ss",
   "SS",
   "nj",
   "Nj",
   "NJ",
   "dz",
   "Dz",
   "DZ",
   "lj",
   "Lj",
   "LJ",
   "",
};



JM_IX_DECL bool RE_CALL re_lookup_def_collate_name(re_str<char>& buf, const char* name)
{
   JM_GUARD_STACK
   unsigned int i = 0;
   while(*def_coll_names[i])
   {
      if(re_strcmp(def_coll_names[i], name) == 0)
      {
         buf = (char)i;
         return true;
      }
      ++i;
   }
   i = 0;
   while(*def_multi_coll[i])
   {
      if(re_strcmp(def_multi_coll[i], name) == 0)
      {
         buf = def_multi_coll[i];
         return true;
      }
      ++i;
   }
   return false;
}

#ifndef RE_LOCALE_CPP

struct collate_name
{
   re_str<char> name;
   re_str<char> value;
   collate_name(){}
   collate_name(const char* p1, const char* p2, const char* p3, const char* p4)
      : name(p1, p2), value(p3, p4) {}
};

re_str<char>* re_coll_name;
re_list<collate_name, jm_def_alloc>* pcoll_names;
unsigned int collate_count = 0;

void RE_CALL re_init_collate()
{
   JM_GUARD_STACK
   if(collate_count == 0)
   {
      re_coll_name = new re_str<char>("xxxxxxxx");
      pcoll_names = new re_list<collate_name, jm_def_alloc>();
   }
   ++collate_count;
}

void RE_CALL re_free_collate()
{
   JM_GUARD_STACK
   if(--collate_count == 0)
   {
      delete re_coll_name;
      delete pcoll_names;
   }
}

void RE_CALL re_update_collate()
{
   JM_GUARD_STACK
   if(*re_coll_name != get_global_locale_name(LC_COLLATE))
   {
      *re_coll_name = get_global_locale_name(LC_COLLATE);
      char buf[256];
      unsigned int i = 400;
      re_get_message(buf, 256, i);
      while(*buf)
      {
         char* p1, *p2, *p3, *p4;;
         p1 = buf;
         while(*p1 && isspace(*p1))++p1;
         p2 = p1;
         while(*p2 && !isspace(*p2))++p2;
         p3 = p2;
         while(*p3 && isspace(*p3))++p3;
         p4 = p3;
         while(*p4 && !isspace(*p4))++p4;
         pcoll_names->add(collate_name(p1, p2, p3, p4));
         ++i;
         re_get_message(buf, 256, i);
      }
   }
}

JM_IX_DECL bool RE_CALL _re_lookup_collate(re_str<char>& buf, const char* p)
{
   JM_GUARD_STACK
   re_list<collate_name, jm_def_alloc>::iterator first, last;
   first = pcoll_names->begin();
   last = pcoll_names->end();
   while(first != last)
   {
      if((*first).name == p)
      {
         buf = (*first).value;
         return true;
      }
      ++first;
   }

   bool result = re_lookup_def_collate_name(buf, p);
   if((result == 0) && (re_strlen(p) == 1))
   {
      result = true;
      buf = *p;
   }
   return result;
}

#ifndef JM_NO_WCSTRING
JM_IX_DECL bool RE_CALL re_lookup_collate(re_str<wchar_t>& out, const wchar_t* first, const wchar_t* last)
{
   JM_GUARD_STACK
   re_str<wchar_t> s(first, last);
   unsigned int len = re_strnarrow((char*)NULL, 0, s.c_str());
   auto_array<char> buf(new char[len]);
   re_strnarrow((char*)buf, len, s.c_str());
   re_str<char> t_out;
   bool result =  _re_lookup_collate(t_out, (char*)buf);
   if(result)
   {
      len = re_strwiden((wchar_t*)0, 0, t_out);
      auto_array<wchar_t> wb(new wchar_t[len]);
      re_strwiden((wchar_t*)wb, len, t_out);
      out = (wchar_t*)wb;
   }
   return result;
}
#endif


#endif

JM_END_NAMESPACE

