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
  *   FILE     fileiter.cpp
  *   VERSION  2.25
  */


#include <limits.h>
#include <fileiter.h>

#if defined(__CYGWIN__) || defined(__CYGWIN32__)
#include <sys/cygwin.h>
#endif

JM_NAMESPACE(JM)

// start with the operating system specific stuff:

#if (defined(__BORLANDC__) || defined(FI_WIN32_DIR) || defined(JM_MSC_VER)) && !defined(JM_NO_WIN32)

// platform is DOS or Windows
// directories are separated with '\\'
// and names are insensitive of case

const char* _fi_sep = "\\";
const char* _fi_sep_alt = "/";
#define FI_TRANSLATE(c) tolower(c)

#else

// platform is not DOS or Windows
// directories are separated with '/'
// and names are sensitive of case

const char* _fi_sep = "/";
const char* _fi_sep_alt = _fi_sep;
#define FI_TRANSLATE(c) c

#endif

#ifdef FI_WIN32_MAP

void mapfile::open(const char* file)
{
   JM_GUARD_STACK
#if defined(__CYGWIN__)||defined(__CYGWIN32__)
   char win32file[ MAX_PATH ];
   cygwin_conv_to_win32_path( file, win32file );
   hfile = CreateFile(win32file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
#else
   hfile = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
#endif
   if(hfile != INVALID_HANDLE_VALUE)
   {
      hmap = CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, 0);
      _first = (const char*)MapViewOfFile(hmap, FILE_MAP_READ, 0, 0, 0);
      _last = _first + GetFileSize(hfile, 0);
   }
}

void mapfile::close()
{
   JM_GUARD_STACK
   if(hfile != INVALID_HANDLE_VALUE)
   {
      UnmapViewOfFile((void*)_first);
      CloseHandle(hmap);
      CloseHandle(hfile);
      hmap = hfile = 0;
      _first = _last = 0;
   }
}

#elif !defined(JM_NO_STL)

mapfile_iterator& mapfile_iterator::operator = (const mapfile_iterator& i)
{
   JM_GUARD_STACK
   if(file && node)
      file->unlock(node);
   file = i.file;
   node = i.node;
   offset = i.offset;
   if(file)
      file->lock(node);
   return *this;
}

mapfile_iterator& mapfile_iterator::operator++ ()
{
   JM_GUARD_STACK
   if((++offset == mapfile::buf_size) && file)
   {
      ++node;
      offset = 0;
      file->lock(node);
      file->unlock(node-1);
   }
   return *this;
}

mapfile_iterator mapfile_iterator::operator++ (int)
{
   JM_GUARD_STACK
   mapfile_iterator temp(*this);
   if((++offset == mapfile::buf_size) && file)
   {
      ++node;
      offset = 0;
      file->lock(node);
      file->unlock(node-1);
   }
   return temp;
}

mapfile_iterator& mapfile_iterator::operator-- ()
{
   JM_GUARD_STACK
   if((offset == 0) && file)
   {
      --node;
      offset = mapfile::buf_size - 1;
      file->lock(node);
      file->unlock(node + 1);
   }
   else
      --offset;
   return *this;
}

mapfile_iterator mapfile_iterator::operator-- (int)
{
   JM_GUARD_STACK
   mapfile_iterator temp(*this);
   if((offset == 0) && file)
   {
      --node;
      offset = mapfile::buf_size - 1;
      file->lock(node);
      file->unlock(node + 1);
   }
   else
      --offset;
   return temp;
}

mapfile_iterator operator + (const mapfile_iterator& i, long off)
{
   JM_GUARD_STACK
   mapfile_iterator temp(i);
   temp += off;
   return temp;
}

mapfile_iterator operator - (const mapfile_iterator& i, long off)
{
   JM_GUARD_STACK
   mapfile_iterator temp(i);
   temp -= off;
   return temp;
}

mapfile::iterator mapfile::begin()const
{
   JM_GUARD_STACK
   return mapfile_iterator(this, 0);
}

mapfile::iterator mapfile::end()const
{
   JM_GUARD_STACK
   return mapfile_iterator(this, _size);
}

void mapfile::lock(pointer* node)const
{
   JM_GUARD_STACK
   assert(node >= _first);
   assert(node <= _last);
   if(node < _last)
   {
      if(*node == 0)
      {
         if(condemed.empty())
         {
            *node = new char[sizeof(int) + buf_size];
            *((int*)(*node)) = 1;
         }
         else
         {
            pointer* p = condemed.front();
            condemed.pop_front();
            *node = *p;
            *p = 0;
            *((int*)(*node)) = 1;
         }
         ::fseek(hfile, (node - _first) * buf_size, SEEK_SET);
         if(node == _last - 1)
            ::fread(*node + sizeof(int), _size % buf_size, 1, hfile);
         else
            ::fread(*node + sizeof(int), buf_size, 1, hfile);
      }
      else
      {
         if(*((int*)(*node)) == 0)
         {
            *((int*)(*node)) = 1;
            condemed.remove(node);
         }
         else
            ++(*((int*)(*node)));
      }
   }
}

void mapfile::unlock(pointer* node)const
{
   JM_GUARD_STACK
   assert(node >= _first);
   assert(node <= _last);
   if(node < _last)
   {
      if(--(*((int*)(*node))) == 0)
      {
         condemed.push_back(node);
      }
   }
}

long int get_file_length(FILE* hfile)
{
   JM_GUARD_STACK
   long int result;
   ::fseek(hfile, 0, SEEK_END);
   result = ::ftell(hfile);
   ::fseek(hfile, 0, SEEK_SET);
   return result;
}


void mapfile::open(const char* file)
{
   JM_GUARD_STACK
   hfile = ::fopen(file, "rb");
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   if(hfile != 0)
   {
      _size = get_file_length(hfile);
      long cnodes = (_size + buf_size - 1) / buf_size;

      // check that number of nodes is not too high:
      if(cnodes > (long)((INT_MAX) / sizeof(pointer*)))
      {
         ::fclose(hfile);
         hfile = 0;
         _size = 0;
         return;
      }

      _first = new pointer[(int)cnodes];
      _last = _first + cnodes;
      memset(_first, 0, cnodes*sizeof(pointer));
   }
#ifndef JM_NO_EXCEPTIONS
   }catch(...)
   { close(); throw; }
#endif
}

void mapfile::close()
{
   JM_GUARD_STACK
   if(hfile != 0)
   {
      pointer* p = _first;
      while(p != _last)
      {
         if(*p)
            delete[] *p;
         ++p;
      }
      delete[] _first;
      _size = 0;
      _first = _last = 0;
      ::fclose(hfile);
      hfile = 0;
      condemed.erase(condemed.begin(), condemed.end());
   }
}


#endif


file_iterator::file_iterator()
{
   JM_GUARD_STACK
   _root = _path = 0;
   ref = 0;
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   _root = new char[MAX_PATH];
   _path = new char[MAX_PATH];
   ptr = _path;
   *_path = 0;
   *_root = 0;
   ref = new file_iterator_ref();
   ref->hf = _fi_invalid_handle;
   ref->count = 1;
#ifndef JM_NO_EXCEPTIONS
   } 
   catch(...)
   {
      delete[] _root;
      delete[] _path;
      delete ref;
      throw;
   }
#endif
}

file_iterator::file_iterator(const char* wild)
{
   JM_GUARD_STACK
   _root = _path = 0;
   ref = 0;
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   _root = new char[MAX_PATH];
   _path = new char[MAX_PATH];
   strcpy(_root, wild);
   ptr = _root;
   while(*ptr)++ptr;
   while((ptr > _root) && (*ptr != *_fi_sep) && (*ptr != *_fi_sep_alt))--ptr;
   #if 0
   *ptr = 0;
   strcpy(_path, _root);
   if(*_path == 0)
      strcpy(_path, ".");
   strcat(_path, _fi_sep);
   ptr = _path + strlen(_path);
   #else
   if((ptr == _root) && ( (*ptr== *_fi_sep) || (*ptr==*_fi_sep_alt) ) )
   {
     _root[1]='\0';
     strcpy(_path, _root);
     ptr = _path + strlen(_path);
   }
   else
   {
     *ptr = 0;
     strcpy(_path, _root);
     if(*_path == 0)
       strcpy(_path, ".");
     strcat(_path, _fi_sep);
     ptr = _path + strlen(_path);
   }
   #endif

   ref = new file_iterator_ref();
   ref->hf = FindFirstFile(wild, &(ref->_data));
   ref->count = 1;

   if(ref->hf == _fi_invalid_handle)
   {
      *_path = 0;
      ptr = _path;
   }
   else
   {
      strcpy(ptr, ref->_data.cFileName);
      if(ref->_data.dwFileAttributes & _fi_dir)
         next();
   }
#ifndef JM_NO_EXCEPTIONS
   } 
   catch(...)
   {
      delete[] _root;
      delete[] _path;
      delete ref;
      throw;
   }
#endif
}

file_iterator::file_iterator(const file_iterator& other)
{
   JM_GUARD_STACK
   _root = _path = 0;
   ref = 0;
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   _root = new char[MAX_PATH];
   _path = new char[MAX_PATH];
   strcpy(_root, other._root);
   strcpy(_path, other._path);
   ptr = _path + (other.ptr - other._path);
   ref = other.ref;
#ifndef JM_NO_EXCEPTIONS
   } 
   catch(...)
   {
      delete[] _root;
      delete[] _path;
      throw;
   }
#endif
   ++(ref->count);
}

file_iterator& file_iterator::operator=(const file_iterator& other)
{
   JM_GUARD_STACK
   strcpy(_root, other._root);
   strcpy(_path, other._path);
   ptr = _path + (other.ptr - other._path);
   if(--(ref->count) == 0)
   {
      if(ref->hf != _fi_invalid_handle)
         FindClose(ref->hf);
      delete ref;
   }
   ref = other.ref;
   ++(ref->count);
   return *this;
}


file_iterator::~file_iterator()
{
   JM_GUARD_STACK
   delete[] _root;
   delete[] _path;
   if(--(ref->count) == 0)
   {
      if(ref->hf != _fi_invalid_handle)
         FindClose(ref->hf);
      delete ref;
   }
}

file_iterator file_iterator::operator++(int)
{
   JM_GUARD_STACK
   file_iterator temp(*this);
   next();
   return temp;
}


void file_iterator::next()
{
   JM_GUARD_STACK
   if(ref->hf != _fi_invalid_handle)
   {
      bool cont = true;
      while(cont)
      {
         cont = FindNextFile(ref->hf, &(ref->_data));
         if(cont && ((ref->_data.dwFileAttributes & _fi_dir) == 0))
            break;
      }
      if(!cont)
      {
         // end of sequence
         FindClose(ref->hf);
         ref->hf = _fi_invalid_handle;
         *_path = 0;
         ptr = _path;
      }
      else
         strcpy(ptr, ref->_data.cFileName);
   }
}



directory_iterator::directory_iterator()
{
   JM_GUARD_STACK
   _root = _path = 0;
   ref = 0;
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   _root = new char[MAX_PATH];
   _path = new char[MAX_PATH];
   ptr = _path;
   *_path = 0;
   *_root = 0;
   ref = new file_iterator_ref();
   ref->hf = _fi_invalid_handle;
   ref->count = 1;
#ifndef JM_NO_EXCEPTIONS
   } 
   catch(...)
   {
      delete[] _root;
      delete[] _path;
      delete ref;
      throw;
   }
#endif
}

directory_iterator::directory_iterator(const char* wild)
{
   JM_GUARD_STACK
   _root = _path = 0;
   ref = 0;
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   _root = new char[MAX_PATH];
   _path = new char[MAX_PATH];
   strcpy(_root, wild);
   ptr = _root;
   while(*ptr)++ptr;
   while((ptr > _root) && (*ptr != *_fi_sep) && (*ptr != *_fi_sep_alt))--ptr;
   #if 0
   *ptr = 0;
   strcpy(_path, _root);
   if(*_path == 0)
      strcpy(_path, ".");
   strcat(_path, _fi_sep);
   ptr = _path + strlen(_path);
   #else
   if((ptr == _root) && ( (*ptr== *_fi_sep) || (*ptr==*_fi_sep_alt) ) )
   {
     _root[1]='\0';
     strcpy(_path, _root);
     ptr = _path + strlen(_path);
   }
   else
   {
     *ptr = 0;
     strcpy(_path, _root);
     if(*_path == 0)
       strcpy(_path, ".");
     strcat(_path, _fi_sep);
     ptr = _path + strlen(_path);
   }
   #endif
   ref = new file_iterator_ref();
   ref->count = 1;
   ref->hf = FindFirstFile(wild, &(ref->_data));
   if(ref->hf == _fi_invalid_handle)
   {
      *_path = 0;
      ptr = _path;
   }
   else
   {
      strcpy(ptr, ref->_data.cFileName);
      if(((ref->_data.dwFileAttributes & _fi_dir) == 0) || (strcmp(ref->_data.cFileName, ".") == 0) || (strcmp(ref->_data.cFileName, "..") == 0))
         next();
   }
#ifndef JM_NO_EXCEPTIONS
   } 
   catch(...)
   {
      delete[] _root;
      delete[] _path;
      delete ref;
      throw;
   }
#endif
}

directory_iterator::~directory_iterator()
{
   JM_GUARD_STACK
   delete[] _root;
   delete[] _path;
   if(--(ref->count) == 0)
   {
      if(ref->hf != _fi_invalid_handle)
         FindClose(ref->hf);
      delete ref;
   }
}

directory_iterator::directory_iterator(const directory_iterator& other)
{
   JM_GUARD_STACK
   _root = _path = 0;
   ref = 0;
#ifndef JM_NO_EXCEPTIONS
   try{
#endif
   _root = new char[MAX_PATH];
   _path = new char[MAX_PATH];
   strcpy(_root, other._root);
   strcpy(_path, other._path);
   ptr = _path + (other.ptr - other._path);
   ref = other.ref;
#ifndef JM_NO_EXCEPTIONS
   } 
   catch(...)
   {
      delete[] _root;
      delete[] _path;
      throw;
   }
#endif
   ++(ref->count);
}

directory_iterator& directory_iterator::operator=(const directory_iterator& other)
{
   JM_GUARD_STACK
   strcpy(_root, other._root);
   strcpy(_path, other._path);
   ptr = _path + (other.ptr - other._path);
   if(--(ref->count) == 0)
   {
      if(ref->hf != _fi_invalid_handle)
         FindClose(ref->hf);
      delete ref;
   }
   ref = other.ref;
   ++(ref->count);
   return *this;
}

directory_iterator directory_iterator::operator++(int)
{
   JM_GUARD_STACK
   directory_iterator temp(*this);
   next();
   return temp;
}

void directory_iterator::next()
{
   JM_GUARD_STACK
   if(ref->hf != _fi_invalid_handle)
   {
      bool cont = true;
      while(cont)
      {
         cont = FindNextFile(ref->hf, &(ref->_data));
         if(cont && (ref->_data.dwFileAttributes & _fi_dir))
         {
            if(strcmp(ref->_data.cFileName, ".") && strcmp(ref->_data.cFileName, ".."))
               break;
         }
      }
      if(!cont)
      {
         // end of sequence
         FindClose(ref->hf);
         ref->hf = _fi_invalid_handle;
         *_path = 0;
         ptr = _path;
      }
      else
         strcpy(ptr, ref->_data.cFileName);
   }
}


#ifdef FI_POSIX_DIR

struct _fi_priv_data
{
   char root[MAX_PATH];
   char* mask;
   DIR* d;
   _fi_priv_data(const char* p);
};

_fi_priv_data::_fi_priv_data(const char* p)
{
   JM_GUARD_STACK
   strcpy(root, p);
   mask = root;
   while(*mask) ++mask;
   while((mask > root) && (*mask != *_fi_sep) && (*mask != *_fi_sep_alt)) --mask;
   if(mask == root && ((*mask== *_fi_sep) || (*mask == *_fi_sep_alt)) )
   {
      root[1] = '\0';
      strcpy(root+2, p+1);
      mask = root+2;
   }
   else if(mask == root)
   {
      root[0] = '.';
      root[1] = '\0';
      strcpy(root+2, p);
      mask = root+2;
   }
   else
   {
      *mask = 0;
      ++mask;
   }
}

bool iswild(const char* mask, const char* name)
{
   JM_GUARD_STACK
   while(*mask && *name)
   {
      switch(*mask)
      {
      case '?':
         ++name;
         ++mask;
         continue;
      case '*':
         ++mask;
         if(*mask == 0)
            return true;
         while(*name)
         {
            if(iswild(mask, name))
               return true;
            ++name;
         }
         return false;
      case '.':
         if(0 == *name)
         {
            ++mask;
            continue;
         }
         // fall through:
      default:
         if(FI_TRANSLATE(*mask) != FI_TRANSLATE(*name))
            return false;
         ++mask;
         ++name;
         continue;
      }
   }
   if(*mask != *name)
      return false;
   return true;
}

unsigned _fi_attributes(const char* root, const char* name)
{
   JM_GUARD_STACK
   char buf[MAX_PATH];
   if( ( (root[0] == *_fi_sep) || (root[0] == *_fi_sep_alt) ) && (root[1] == '\0') )
      sprintf(buf, "%s%s", root, name);
   else
      sprintf(buf, "%s%s%s", root, _fi_sep, name);
   DIR* d = opendir(buf);
   if(d)
   {
      closedir(d);
      return _fi_dir;
   }
   return 0;
}

_fi_find_handle _fi_FindFirstFile(const char* lpFileName, _fi_find_data* lpFindFileData)
{
   JM_GUARD_STACK
   _fi_find_handle dat = new _fi_priv_data(lpFileName);

   DIR* h = opendir(dat->root);
   dat->d = h;
   if(h != NULL)
   {
      if(_fi_FindNextFile(dat, lpFindFileData))
         return dat;
   }
   delete dat;
   return NULL;
}

bool _fi_FindNextFile(_fi_find_handle dat, _fi_find_data* lpFindFileData)
{
   JM_GUARD_STACK
   dirent* d;
   do
   {
      d = readdir(dat->d);
   } while(d && !iswild(dat->mask, d->d_name));

   if(d)
   {
      strcpy(lpFindFileData->cFileName, d->d_name);
      lpFindFileData->dwFileAttributes = _fi_attributes(dat->root, d->d_name);
      return true;
   }
   return false;
}

bool _fi_FindClose(_fi_find_handle dat)
{
   JM_GUARD_STACK
   closedir(dat->d);
   delete dat;
   return true;
}

#endif

JM_END_NAMESPACE









