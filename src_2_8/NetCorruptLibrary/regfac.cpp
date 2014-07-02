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
  *   FILE     regfac.cpp
  *   VERSION  2.25
  */

#include <jm/regfac.h>
#include <jm/re_str.h>
#include <jm/re_lst.h>
#include <jm/re_nls.h>
#include <jm/regex.h>
#include <jm/re_mss.h>
#include <jm/re_cls.h>

#ifdef RE_LOCALE_CPP

JM_NAMESPACE(JM)

JM_STD::string RE_CALL to_narrow(const JM_STD::wstring& is, const JM_STD::locale& l);

JM_STD::locale::id regfacet<char>::id;
JM_STD::locale::id regfacet<wchar_t>::id;

regfacet<char>::regfacet(unsigned int i)
   : JM_STD::locale::facet(i), name("xxxxxxxx")
{
}

regfacet<wchar_t>::regfacet(unsigned int i)
   : JM_STD::locale::facet(i), name("xxxxxxxx")
{
}

regfacet<char>::~regfacet()
{
}

regfacet<wchar_t>::~regfacet()
{
}

jm_uintfast32_t RE_CALL regfacet<char>::do_lookup_classname(const char* first, const char* last)const
{
   JM_GUARD_STACK
   unsigned int i;
   JM_STD::string s(first, last);

   JM_STD::map<JM_STD::string, unsigned long, JM_STD::less<JM_STD::string> >::const_iterator pos = classes.find(s);
   if(pos != classes.end())
      return (*pos).second;

   for(i = 0; i < re_classes_max; ++i)
   {
      if(s == re_char_class_names[i])
         return re_char_class_id[i];
   }
   return 0;
}

bool RE_CALL regfacet<char>::do_lookup_collatename(re_str<char>& s, const re_str<char>& name)const
{
   JM_GUARD_STACK
   JM_STD::map<re_str<char>, re_str<char>, JM_STD::less<re_str<char> > >::const_iterator pos = collating_elements.find(name);
   if(pos != collating_elements.end())
   {
      s = (*pos).second;
   }
   return re_lookup_def_collate_name(s, name.c_str());
}

unsigned int RE_CALL regfacet<char>::do_syntax_type(char c)const
{
   return syntax_map[(unsigned char)c];
}

void RE_CALL regfacet<char>::do_update(const JM_STD::locale& l)
{
   JM_GUARD_STACK
#ifdef RE_THREADS
   critical_section::rw_guard g(cs);
#endif
   if(l.name() != name)
   {
      name = l.name();
      JM_STD::messages<char>::catalog cat = regex_message_catalogue ? JM_USE_FACET(l, JM_STD::messages<char>).open(regex_message_catalogue, l) : -1;

      memset(syntax_map, syntax_char, 256);
      unsigned int i;
      for(i = 1; i < syntax_max; ++i)
      {
         string_type s = re_get_def_message(i+100);
         if((int)cat >= 0)
            s = JM_USE_FACET(l, JM_STD::messages<char>).get(cat, 0, i+100, s);
         for(unsigned int j = 0; j < s.size(); ++j)
         {
            syntax_map[s[j]] = (unsigned char)(i);
         }
      }

      // load any custom collate names:
      string_type c1, c2;
      i = 400;
      if((int)cat >= 0)
         c2 = JM_USE_FACET(l, JM_STD::messages<char>).get(cat, 0, i, c1);
      while(c2.size())
      {
         const char* p1, *p2, *p3, *p4;;
         p1 = c2.c_str();
         while(*p1 && JM_STD::isspace((char)*p1, l))++p1;
         p2 = p1;
         while(*p2 && !JM_STD::isspace((char)*p2, l))++p2;
         p3 = p2;
         while(*p3 && JM_STD::isspace((char)*p3, l))++p3;
         p4 = p3;
         while(*p4 && !JM_STD::isspace((char)*p4, l))++p4;
         collating_elements[re_str<char>(p1, p2)] = re_str<char>(p3, p4);
         
         ++i;
         c2 = JM_USE_FACET(l, JM_STD::messages<char>).get(cat, 0, i, c1);
      }

      re_str<char> n("zero");
      JM_STD::map<re_str<char>, re_str<char>, JM_STD::less<re_str<char> > >::const_iterator pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _zero = *(*pos).second.c_str();
      else
         _zero = '0';

      n = "ten";
      pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _ten = *(*pos).second.c_str();
      else
         _ten = 'a';

      JM_STD::string m;
      JM_STD::string s;
      if((int)cat >= 0)
         for(i = 0; i < re_classes_max; ++i)
         {
            s = JM_USE_FACET(l, JM_STD::messages<char>).get(cat, 0, i+300, m);
            if(s.size())
               classes[s] = i;
         }
      
      if((int)cat >= 0)
         JM_USE_FACET(l, JM_STD::messages<char>).close(cat);
   }
}

jm_uintfast32_t RE_CALL regfacet<wchar_t>::do_lookup_classname(const wchar_t* first, const wchar_t* last)const
{
   JM_GUARD_STACK
   JM_STD::wstring s(first, last);
   unsigned int i;

   JM_STD::map<JM_STD::wstring, unsigned long, JM_STD::less<JM_STD::wstring> >::const_iterator pos = classes.find(s);
   if(pos != classes.end())
      return (*pos).second;

   JM_STD::string ns = to_narrow(s, *ploc);
   for(i = 0; i < re_classes_max; ++i)
   {
      if(strcmp(re_char_class_names[i], ns.c_str()) == 0)
         return re_char_class_id[i];
   }
   return 0;
}

bool RE_CALL regfacet<wchar_t>::do_lookup_collatename(re_str<wchar_t>& s, const re_str<wchar_t>& name)const
{
   JM_GUARD_STACK
   JM_STD::map<re_str<wchar_t>, re_str<wchar_t>, JM_STD::less<re_str<wchar_t> > >::const_iterator pos = collating_elements.find(name);
   if(pos != collating_elements.end())
   {
      s = (*pos).second;
   }

   unsigned int len = re_strnarrow((char*)NULL, 0, name.c_str());
   auto_array<char> buf(new char[len]);
   re_strnarrow((char*)buf, len, name.c_str());
   re_str<char> t_out;
   bool result =  re_lookup_def_collate_name(t_out, (char*)buf);
   if(result)
   {
      len = re_strwiden((wchar_t*)0, 0, t_out);
      auto_array<wchar_t> wb(new wchar_t[len]);
      re_strwiden((wchar_t*)wb, len, t_out);
      s = (wchar_t*)wb;
   }
   return result;
}

unsigned int RE_CALL regfacet<wchar_t>::do_syntax_type(wchar_t c)const
{
   JM_GUARD_STACK
   JM_STD::list<syntax_map>::const_iterator i, j;
   i = syntax.begin();
   j = syntax.end();
   while(i != j)
   {
      if((*i).c == c)
         return (*i).type;
      ++i;
   }
   return 0;
}

void RE_CALL regfacet<wchar_t>::do_update(const JM_STD::locale& l)
{
   JM_GUARD_STACK
#ifdef RE_THREADS
   critical_section::rw_guard g(cs);
#endif
   if(l.name() != name)
   {
      ploc = &l;
      syntax_map m;
      name = l.name();
      JM_STD::messages<wchar_t>::catalog cat = regex_message_catalogue ? JM_USE_FACET(l, JM_STD::messages<wchar_t>).open(regex_message_catalogue, l) : -1;

      syntax.clear();
      unsigned int i;
      for(i = 1; i < syntax_max; ++i)
      {
         string_type s = re_get_def_message_w(i+100);
         if((int)cat >= 0)
            s = JM_USE_FACET(l, JM_STD::messages<wchar_t>).get(cat, 0, i+100, s);
         for(unsigned int j = 0; j < s.size(); ++j)
         {
            m.c = s[j];
            m.type = i;
            syntax.push_back(m);
         }
      }

      // load any custom collate names:
      string_type c1, c2;
      i = 400;
      if((int)cat >= 0)
         c2 = JM_USE_FACET(l, JM_STD::messages<wchar_t>).get(cat, 0, i, c1);
      while(c2.size())
      {
         const wchar_t* p1, *p2, *p3, *p4;;
         p1 = c2.c_str();
         while(*p1 && JM_STD::isspace((wchar_t)*p1, l))++p1;
         p2 = p1;
         while(*p2 && !JM_STD::isspace((wchar_t)*p2, l))++p2;
         p3 = p2;
         while(*p3 && JM_STD::isspace((wchar_t)*p3, l))++p3;
         p4 = p3;
         while(*p4 && !JM_STD::isspace((wchar_t)*p4, l))++p4;
         collating_elements[re_str<wchar_t>(p1, p2)] = re_str<wchar_t>(p3, p4);
         
         ++i;
         c2 = JM_USE_FACET(l, JM_STD::messages<wchar_t>).get(cat, 0, i, c1);
      }

      re_str<wchar_t> n(L"zero");
      JM_STD::map<re_str<wchar_t>, re_str<wchar_t>, JM_STD::less<re_str<wchar_t> > >::const_iterator pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _zero = *(*pos).second.c_str();
      else
         _zero = L'0';

      n = L"ten";
      pos = collating_elements.find(n);
      if(pos != collating_elements.end())
         _ten = *(*pos).second.c_str();
      else
         _ten = L'a';

      JM_STD::wstring ws;
      JM_STD::wstring s;
      if((int)cat >= 0)
         for(i = 0; i < re_classes_max; ++i)
         {
            s = JM_USE_FACET(l, JM_STD::messages<wchar_t>).get(cat, 0, i+300, ws);
            if(s.size())
               classes[s] = i;
         }

      if((int)cat >= 0)
         JM_USE_FACET(l, JM_STD::messages<wchar_t>).close(cat);
   }
}

JM_STD::string RE_CALL to_narrow(const JM_STD::wstring& is, const JM_STD::locale& l)
{
   JM_GUARD_STACK
   unsigned int bufsize = is.size() * 2;
   auto_array<char> t = new char[bufsize];
   typedef JM_STD::codecvt<wchar_t, char, JM_STDC::mbstate_t> cvt_type;
   const cvt_type& cvt = JM_USE_FACET(l, cvt_type);
   JM_STDC::mbstate_t state = 0;

   const wchar_t* next_in;
   char* next_out;
   while(true)
   {
      switch(cvt.out(state, is.c_str(), is.c_str() + is.size(), next_in, t.get(), t.get() + bufsize, next_out))
      {
      case JM_STD::codecvt_base::ok:
         return JM_STD::string(t.get(), next_out);
      case JM_STD::codecvt_base::partial:
         bufsize *= 2;
         t = auto_array<char>(new char[bufsize]);
         continue;
      case JM_STD::codecvt_base::error:
         // not much we can do here but guess:
      case JM_STD::codecvt_base::noconv:
         JM_STD::string out;
         for(unsigned i = 0; i < is.size(); ++i)
         {
            out.append(1, is[i]);
         }
         return out;
      }
   }
}

JM_END_NAMESPACE

#endif




