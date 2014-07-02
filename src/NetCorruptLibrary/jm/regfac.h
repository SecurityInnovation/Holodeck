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
  *   FILE     regfac.h
  *   VERSION  2.25
  */

#ifndef REGFAC_H
#define REGFAC_H

#ifndef JM_CFG_H
#include <jm/jm_cfg.h>
#endif

#ifdef RE_LOCALE_CPP


#include <string>
#include <jm/re_str.h>
#include <jm/re_cls.h>
#include <list>
#include <map>

//
// class regfacet
//
// provides syntax data etc, customised versions
// can be installed in an instance of std::locale and imbue'd
// into a reg_expression for per-instance localisation.
//

JM_NAMESPACE(JM)

#ifdef __BORLANDC__
   #if __BORLANDC__ == 0x530
    #pragma option push -a4 -b
   #elif __BORLANDC__ > 0x530
    #pragma option push -a8 -b
   #endif
#endif

template <class charT>
class regfacet : public JM_STD::locale::facet
{
public:
   static JM_STD::locale::id id;
   regfacet(unsigned int i = 0);
   jm_uintfast32_t RE_CALL lookup_classname(const charT* first, const charT* last)const;
   bool RE_CALL lookup_collatename(re_str<charT>& s, const re_str<charT>& name)const;
   unsigned int RE_CALL syntax_type(charT)const;
   void RE_CALL update(const JM_STD::locale&)const;
   charT RE_CALL zero()const;
   charT RE_CALL ten()const;

protected:
   virtual jm_uintfast32_t RE_CALL do_lookup_classname(const charT* first, const charT* last)const = 0;
   virtual bool RE_CALL do_lookup_collatename(re_str<charT>& s, const re_str<charT>& name)const = 0;
   virtual unsigned int RE_CALL do_syntax_type(charT)const = 0;
   virtual void RE_CALL do_update(const JM_STD::locale&) = 0;

   // required by Rogue Wave, not part of standard:
   JM_STD::locale::id& get_id()const { return id; }
   ~regfacet(){}
};

JM_TEMPLATE_SPECIALISE
class JM_IX_DECL regfacet<char> : public JM_STD::locale::facet
{
public:
   typedef JM_STD::messages<char>::string_type string_type;
private:
   unsigned char syntax_map[256];
   string_type name;
   char _zero, _ten;
   JM_STD::map<JM_STD::string, unsigned long, JM_STD::less<JM_STD::string> > classes;
   JM_STD::map<re_str<char>, re_str<char>, JM_STD::less<re_str<char> > > collating_elements;
   regfacet(const regfacet&);

#ifdef RE_THREADS
   critical_section cs;
#endif

public:
   static JM_STD::locale::id id;
   regfacet(unsigned int i = 0);
   jm_uintfast32_t RE_CALL lookup_classname(const char* first, const char* last)const { return do_lookup_classname(first, last); }
   bool RE_CALL lookup_collatename(re_str<char>& s, const re_str<char>& name)const { return do_lookup_collatename(s, name); }
   unsigned int RE_CALL syntax_type(char c)const { return do_syntax_type(c); }
   void RE_CALL update(const JM_STD::locale& l)const { const_cast<regfacet<char>*>(this)->do_update(l); }
   char RE_CALL zero()const { return _zero; }
   char RE_CALL ten()const { return _ten; }

protected:
   virtual jm_uintfast32_t RE_CALL do_lookup_classname(const char* first, const char* last)const;
   virtual bool RE_CALL do_lookup_collatename(re_str<char>& s, const re_str<char>& name)const;
   virtual unsigned int RE_CALL do_syntax_type(char)const;
   virtual void RE_CALL do_update(const JM_STD::locale&);

   // required by Rogue Wave, not part of standard:
   JM_STD::locale::id& get_id()const { return id; }
   ~regfacet();

};

JM_TEMPLATE_SPECIALISE
class JM_IX_DECL regfacet<wchar_t> : public JM_STD::locale::facet
{
public:
   typedef JM_STD::messages<wchar_t>::string_type string_type;
private:
   JM_STD::messages<char>::string_type name;

   struct syntax_map
   {
      wchar_t c;
      unsigned int type;
   };

   JM_STD::list<syntax_map> syntax;
   wchar_t _zero, _ten;
   JM_STD::map<JM_STD::wstring, unsigned long, JM_STD::less<JM_STD::wstring> > classes;
   const JM_STD::locale* ploc;
   JM_STD::map<re_str<wchar_t>, re_str<wchar_t>, JM_STD::less<re_str<wchar_t> > > collating_elements;
   regfacet(const regfacet&);

#ifdef RE_THREADS
   critical_section cs;
#endif

public:
   static JM_STD::locale::id id;
   regfacet(unsigned int i = 0);
   jm_uintfast32_t RE_CALL lookup_classname(const wchar_t* first, const wchar_t* last)const { return do_lookup_classname(first, last); }
   bool RE_CALL lookup_collatename(re_str<wchar_t>& s, const re_str<wchar_t>& name)const { return do_lookup_collatename(s, name); }
   unsigned int RE_CALL syntax_type(wchar_t c)const { return do_syntax_type(c); }
   void RE_CALL update(const JM_STD::locale& l)const { const_cast<regfacet<wchar_t>*>(this)->do_update(l); }
   wchar_t RE_CALL zero()const { return _zero; }
   wchar_t RE_CALL ten()const { return _ten; }

protected:
   virtual jm_uintfast32_t RE_CALL do_lookup_classname(const wchar_t* first, const wchar_t* last)const;
   virtual bool RE_CALL do_lookup_collatename(re_str<wchar_t>& s, const re_str<wchar_t>& name)const;
   virtual unsigned int RE_CALL do_syntax_type(wchar_t)const;
   virtual void RE_CALL do_update(const JM_STD::locale&);

   // required by Rogue Wave, not part of standard:
   JM_STD::locale::id& get_id()const { return id; }
   ~regfacet();

};

#ifdef __BORLANDC__
 #if __BORLANDC__ > 0x520
  #pragma option pop
 #endif
#endif

JM_END_NAMESPACE

#endif

#endif

