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
  *   FILE     cregex.cpp
  *   VERSION  2.25
  */



#include <cregex>
#include <regex>
#if !defined(JM_NO_STRING_H)
#include <map>
#include <list>
#include <fileiter.h>
#include <stdio.h>

JM_NAMESPACE(JM)

#ifdef __BORLANDC__
#if __BORLANDC__ < 0x530
//
// we need to instantiate the vector classes we use
// since declaring a reference to type doesn't seem to
// do the job...
JM_STD::vector<unsigned int> inst1;
JM_STD::vector<JM_STD::string> inst2;
#endif
#endif


template <class iterator>
JM_STD::string to_string(iterator i, iterator j)
{
   JM_GUARD_STACK
   JM_STD::string s;
   while(i != j)
   {
      s.append(1, *i);
      ++i;
   }
   return s;
}

inline JM_STD::string to_string(const char* i, const char* j)
{
   return JM_STD::string(i, j);
}



class RegExData
{
public:
   enum type
   {
      type_pc,
      type_pf,
      type_copy
   };
   regex e;
   cmatch m;
   reg_match<mapfile::iterator, regex::alloc_type> fm;
   type t;
   const char* pbase;
   unsigned line;
   mapfile::iterator fbase;
#if defined(JM_NO_DEFAULT_PARAM) || defined(JM_NO_TRICKY_DEFAULT_PARAM)
   JM_STD::map<int, JM_STD::string, JM_STD::less<int>, JM_STD::allocator<JM_STD::pair<const int, JM_STD::string> > > strings;
   JM_STD::map<int, int, JM_STD::less<int>, JM_STD::allocator<JM_STD::pair<const int, int> > > positions;
#else
   JM_STD::map<int, JM_STD::string> strings;
   JM_STD::map<int, int> positions;
#endif
   void update();
   void clean();
};

void RegExData::update()
{
   JM_GUARD_STACK
   strings.erase(strings.begin(), strings.end());
   positions.erase(positions.begin(), positions.end());
   if(t == type_pc)
   {
      for(unsigned int i = 0; i < m.size(); ++i)
      {
         if(m[i].matched)
         {
            strings[i] = JM_STD::string(m[i].first, m[i].second);
            positions[i] = (int)(m[i].first - pbase);
         }
      }
      line = m.line();
   }
   else
   {
      for(unsigned int i = 0; i < fm.size(); ++i)
      {
         if(fm[i].matched)
         {
            strings[i] = to_string(fm[i].first, fm[i].second);
            positions[i] = (int)(fm[i].first - fbase);
         }
      }
      line = fm.line();
   }
   t = type_copy;
}

void RegExData::clean()
{
   JM_GUARD_STACK
   fbase = mapfile::iterator();
   fm = reg_match<mapfile::iterator, regex::alloc_type>();
}

RegEx::RegEx()
{
   JM_GUARD_STACK
   pdata = new RegExData();
}

RegEx::RegEx(const RegEx& o)
{
   JM_GUARD_STACK
   pdata = new RegExData(*(o.pdata));
}

RegEx::~RegEx()
{
   JM_GUARD_STACK
   delete pdata;
}

RegEx::RegEx(const char* c, bool icase)
{
   JM_GUARD_STACK
   pdata = new RegExData();
   SetExpression(c, icase);
}

RegEx::RegEx(const JM_STD::string& s, bool icase)
{
   JM_GUARD_STACK
   pdata = new RegExData();
   SetExpression(s.c_str(), icase);
}

RegEx& RegEx::operator=(const RegEx& o)
{
   JM_GUARD_STACK
   *pdata = *(o.pdata);
   return *this;
}

RegEx& RegEx::operator=(const char* p)
{
   JM_GUARD_STACK
   SetExpression(p, false);
   return *this;
}

unsigned int RegEx::SetExpression(const char* p, bool icase)
{
   JM_GUARD_STACK
   jm_uintfast32_t f = icase ? regbase::normal | regbase::use_except | regbase::icase : regbase::normal | regbase::use_except;
   return pdata->e.set_expression(p, f);
}

JM_STD::string RegEx::Expression()const
{
   JM_GUARD_STACK
   return pdata->e.expression();
}

//
// now matching operators:
//
bool RegEx::Match(const char* p, unsigned int flags)
{
   JM_GUARD_STACK
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   if(query_match(p, end, pdata->m, pdata->e, flags))
   {
      pdata->update();
      return true;
   }
   return false;
}

bool RegEx::Search(const char* p, unsigned int flags)
{
   JM_GUARD_STACK
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   if(reg_search(p, end, pdata->m, pdata->e, flags))
   {
      pdata->update();
      return true;
   }
   return false;
}

struct pred1
{
   GrepCallback cb;
   RegEx* pe;
   pred1(GrepCallback c, RegEx* i) : cb(c), pe(i) {}
   bool operator()(const cmatch& m)
   {
      pe->pdata->m = m;
      return cb(*pe);
   }
};

unsigned int RegEx::Grep(GrepCallback cb, const char* p, unsigned int flags)
{
   JM_GUARD_STACK
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   unsigned int result = reg_grep(pred1(cb, this), p, end, pdata->e, flags);
   if(result)
      pdata->update();
   return result;
}

struct pred2
{
   JM_STD::vector<JM_STD::string>& v;
   RegEx* pe;
   pred2(JM_STD::vector<JM_STD::string>& o, RegEx* e) : v(o), pe(e) {}
   bool operator()(const cmatch& m)
   {
      pe->pdata->m = m;
      v.push_back(JM_STD::string(m[0].first, m[0].second));
      return true;
   }
};


unsigned int RegEx::Grep(JM_STD::vector<JM_STD::string>& v, const char* p, unsigned int flags)
{
   JM_GUARD_STACK
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   unsigned int result = reg_grep(pred2(v, this), p, end, pdata->e, flags);
   if(result)
      pdata->update();
   return result;
}

struct pred3
{
   JM_STD::vector<unsigned int>& v;
   const char* base;
   RegEx* pe;
   pred3(JM_STD::vector<unsigned int>& o, const char* pb, RegEx* p) : v(o), base(pb), pe(p) {}
   bool operator()(const cmatch& m)
   {
      pe->pdata->m = m;
      v.push_back((unsigned int)(m[0].first - base));
      return true;
   }
};

unsigned int RegEx::Grep(JM_STD::vector<unsigned int>& v, const char* p, unsigned int flags)
{
   JM_GUARD_STACK
   pdata->t = RegExData::type_pc;
   pdata->pbase = p;
   const char* end = p;
   while(*end)++end;

   unsigned int result = reg_grep(pred3(v, p, this), p, end, pdata->e, flags);
   if(result)
      pdata->update();
   return result;
}

struct pred4
{
   GrepFileCallback cb;
   RegEx* pe;
   const char* file;
   bool ok;
   pred4(GrepFileCallback c, RegEx* i, const char* f) : cb(c), pe(i), file(f), ok(true) {}
   bool operator()(const reg_match<mapfile::iterator, regex::alloc_type>& m)
   {
      pe->pdata->t = RegExData::type_pf;
      pe->pdata->fm = m;
      pe->pdata->update();
      ok = cb(file, *pe);
      return ok;
   }
};

void BuildFileList(JM_STD::list<JM_STD::string>* pl, const char* files, bool recurse)
{
   JM_GUARD_STACK
   file_iterator start(files);
   file_iterator end;
   if(recurse)
   {
      // go through sub directories:
      char buf[MAX_PATH];
      strcpy(buf, start.root());
      if(*buf == 0)
      {
         strcpy(buf, ".");
         strcat(buf, directory_iterator::separator());
         strcat(buf, "*");
      }
      else
      {
         strcat(buf, directory_iterator::separator());
         strcat(buf, "*");
      }
      directory_iterator dstart(buf);
      directory_iterator dend;

      // now get the file mask bit of "files":
      const char* ptr = files;
      while(*ptr) ++ptr;
      while((ptr != files) && (*ptr != *directory_iterator::separator()) && (*ptr != '/'))--ptr;
      if(ptr != files) ++ptr;

      while(dstart != dend)
      {
         sprintf(buf, "%s%s%s", dstart.path(), directory_iterator::separator(), ptr);
         BuildFileList(pl, buf, recurse);
         ++dstart;
      }
   }
   while(start != end)
   {
      pl->push_back(*start);
      ++start;
   }
}


unsigned int RegEx::GrepFiles(GrepFileCallback cb, const char* files, bool recurse, unsigned int flags)
{
   JM_GUARD_STACK
   unsigned int result = 0;
   JM_STD::list<JM_STD::string> file_list;
   BuildFileList(&file_list, files, recurse);
   JM_STD::list<JM_STD::string>::iterator start, end;
   start = file_list.begin();
   end = file_list.end();

   while(start != end)
   {
      mapfile map((*start).c_str());
      pdata->t = RegExData::type_pf;
      pdata->fbase = map.begin();
      pred4 pred(cb, this, (*start).c_str());
      int r = reg_grep(pred, map.begin(), map.end(), pdata->e, flags);
      result += r;
      ++start;
      pdata->clean();
      if(pred.ok == false)
         return result;
   }

   return result;
}

unsigned int RegEx::FindFiles(FindFilesCallback cb, const char* files, bool recurse, unsigned int flags)
{
   JM_GUARD_STACK
   unsigned int result = 0;
   JM_STD::list<JM_STD::string> file_list;
   BuildFileList(&file_list, files, recurse);
   JM_STD::list<JM_STD::string>::iterator start, end;
   start = file_list.begin();
   end = file_list.end();

   while(start != end)
   {
      mapfile map((*start).c_str());
      pdata->t = RegExData::type_pf;
      pdata->fbase = map.begin();

      if(reg_search(map.begin(), map.end(), pdata->fm, pdata->e, flags))
      {
         ++result;
         if(false == cb((*start).c_str()))
            return result;
      }
      //pdata->update();
      ++start;
      pdata->clean();
   }

   return result;
}

::std::string RegEx::Merge(const ::std::string& in, const ::std::string& fmt,
                    bool copy, unsigned int flags)
{
   JM_STD::string result;
   string_out_iterator<JM_STD::string> i(result);
   reg_merge(i, in.begin(), in.end(), pdata->e, fmt.c_str(), copy, flags);
   return result;
}

::std::string RegEx::Merge(const char* in, const char* fmt,
                    bool copy, unsigned int flags)
{
   JM_STD::string result;
   string_out_iterator<JM_STD::string> i(result);
   reg_merge(i, in, in + strlen(in), pdata->e, fmt, copy, flags);
   return result;
}


//
// now operators for returning what matched in more detail:
//
unsigned int RegEx::Position(int i)const
{
   JM_GUARD_STACK
   switch(pdata->t)
   {
   case RegExData::type_pc:
      return (unsigned int)(pdata->m[i].matched ? pdata->m[i].first - pdata->pbase : (unsigned int)-1);
   case RegExData::type_pf:
      return (unsigned int)(pdata->fm[i].matched ? pdata->fm[i].first - pdata->fbase : (unsigned int)-1);
   case RegExData::type_copy:
      {
      JM_STD::map<int, int, JM_STD::less<int> >::iterator pos = pdata->positions.find(i);
      if(pos == pdata->positions.end())
         return (unsigned int)-1;
      return (*pos).second;
      }
   }
   return (unsigned int)-1;
}

unsigned int RegEx::Line()const
{
   JM_GUARD_STACK
   switch(pdata->t)
   {
   case RegExData::type_pc:
      return pdata->m[0].matched ? pdata->m.line() : (unsigned int)-1;
   case RegExData::type_pf:
      return pdata->fm[0].matched ? pdata->fm.line() : (unsigned int)-1;
   case RegExData::type_copy:
      {
         return pdata->line;
      }
   }
   return (unsigned int)-1;
}

unsigned int RegEx::Marks()const
{
   JM_GUARD_STACK
   return pdata->e.mark_count();
}


unsigned int RegEx::Length(int i)const
{
   JM_GUARD_STACK
   switch(pdata->t)
   {
   case RegExData::type_pc:
      return (unsigned int)(pdata->m[i].matched ? pdata->m[i].second - pdata->m[i].first : 0);
   case RegExData::type_pf:
      return (unsigned int)(pdata->fm[i].matched ? pdata->fm[i].second - pdata->fm[i].first : 0);
   case RegExData::type_copy:
      {
      JM_STD::map<int, JM_STD::string, JM_STD::less<int> >::iterator pos = pdata->strings.find(i);
      if(pos == pdata->strings.end())
         return 0;
      return (unsigned int)((*pos).second.size());
      }
   }
   return 0;
}

JM_STD::string RegEx::What(int i)const
{
   JM_GUARD_STACK
   JM_STD::string result;
   switch(pdata->t)
   {
   case RegExData::type_pc:
      if(pdata->m[i].matched) 
         result.assign(pdata->m[i].first, pdata->m[i].second);
      break;
   case RegExData::type_pf:
      if(pdata->m[i].matched) 
         result.assign(to_string(pdata->m[i].first, pdata->m[i].second));
      break;
   case RegExData::type_copy:
      {
      JM_STD::map<int, JM_STD::string, JM_STD::less<int> >::iterator pos = pdata->strings.find(i);
      if(pos != pdata->strings.end())
         result = (*pos).second;
      break;
      }
   }
   return result;
}


JM_END_NAMESPACE

#if defined(__BORLANDC__) && (__BORLANDC__ >= 0x550) && (__BORLANDC__ <= 0x551) && !defined(_RWSTD_COMPILE_INSTANTIATE)
//
// this is an ugly hack to work around an ugly problem:
// by default this file will produce unresolved externals during
// linking unless _RWSTD_COMPILE_INSTANTIATE is defined (Borland bug).
// However if _RWSTD_COMPILE_INSTANTIATE is defined then we get separate
// copies of basic_string's static data in the RTL and this DLL, this messes
// with basic_string's memory management and results in run-time crashes,
// Oh sweet joy of Catch 22....
//
namespace std{
template<> template<>
basic_string<char>&
basic_string<char>::replace<const char*>(char* f1, char* f2, const char* i1, const char* i2)
{
   unsigned insert_pos = f1 - begin();
   unsigned remove_len = f2 - f1;
   unsigned insert_len = i2 - i1;
   unsigned org_size = size();
   if(insert_len > remove_len)
   {
      append(insert_len-remove_len, ' ');
      std::copy_backward(begin() + insert_pos + remove_len, begin() + org_size, end());
      std::copy(i1, i2, begin() + insert_pos);
   }
   else
   {
      std::copy(begin() + insert_pos + remove_len, begin() + org_size, begin() + insert_pos + insert_len);
      std::copy(i1, i2, begin() + insert_pos);
      erase(size() + insert_len - remove_len);
   }
   return *this;
}
}
#endif

#endif







