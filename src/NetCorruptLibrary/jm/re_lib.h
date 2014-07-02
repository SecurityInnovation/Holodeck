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
  *   FILE     re_lib.h
  *   VERSION  2.25
  *   Automatic library file inclusion.
  */


#ifndef RE_LIB_H
#define RE_LIB_H


#if defined(_MSC_VER) && !defined(RE_BUILD_DLL)
#ifdef __SGI_STL_PORT
   #ifdef _DLL
      #ifdef _DEBUG
         #pragma comment(lib, "vc6-stlport-re200dl.lib")
      #else // DEBUG
         #pragma comment(lib, "vc6-stlport-re200l.lib")
      #endif // _DEBUG
   #else // _DLL
      #ifdef _MT
         #ifdef _DEBUG
            #pragma comment(lib, "vc6-stlport-re200dm.lib")
         #else //_DEBUG
            #pragma comment(lib, "vc6-stlport-re200m.lib")
         #endif //_DEBUG
      #else //_MT
         #ifdef _DEBUG
            #pragma comment(lib, "vc6-stlport-re200d.lib")
         #else //_DEBUG
            #pragma comment(lib, "vc6-stlport-re200.lib")
         #endif //_DEBUG
      #endif //_MT
   #endif //_DLL
#else
   #ifdef _DLL
      #ifdef _DEBUG
         #pragma comment(lib, "vc6-re200dl.lib")
      #else // DEBUG
         #pragma comment(lib, "vc6-re200l.lib")
      #endif // _DEBUG
   #else // _DLL
      #ifdef _MT
         #ifdef _DEBUG
            #pragma comment(lib, "vc6-re200dm.lib")
         #else //_DEBUG
            #pragma comment(lib, "vc6-re200m.lib")
         #endif //_DEBUG
      #else //_MT
         #ifdef _DEBUG
            #pragma comment(lib, "vc6-re200d.lib")
         #else //_DEBUG
            #pragma comment(lib, "vc6-re200.lib")
         #endif //_DEBUG
      #endif //_MT
   #endif //_DLL
#endif // __SGI_STL_PORT
#endif //BOOST_MSVC


#if defined(__BORLANDC__) && !defined(RE_BUILD_DLL)
   
   #if __BORLANDC__ < 0x550
   
   #ifdef RE_USE_VCL
   
      #ifdef _RTLDLL
         #pragma comment(lib, "bcb4re200lv.lib")
      #else
         #pragma comment(lib, "bcb4re200v.lib")
      #endif
   
   #else // VCL
   
   #ifdef _RTLDLL
      #ifdef __MT__
         #pragma comment(lib, "bcb4re200lm.lib")
      #else // __MT__
         #pragma comment(lib, "bcb4re200l.lib")
      #endif // __MT__
   #else //_RTLDLL
      #ifdef __MT__
         #pragma comment(lib, "bcb4re200m.lib")
      #else // __MT__
         #pragma comment(lib, "bcb4re200.lib")
      #endif // __MT__
   #endif // _RTLDLL
   
   #endif // VCL
   
   #else // C++ Builder 5:

   #ifdef RE_USE_VCL
   
      #ifdef _RTLDLL
         #pragma comment(lib, "bcb5re200lv.lib")
      #else
         #pragma comment(lib, "bcb5re200v.lib")
      #endif
   
   #else // VCL
   
   #ifdef _RTLDLL
      #ifdef __MT__
         #pragma comment(lib, "bcb5re200lm.lib")
      #else // __MT__
         #pragma comment(lib, "bcb5re200l.lib")
      #endif // __MT__
   #else //_RTLDLL
      #ifdef __MT__
         #pragma comment(lib, "bcb5re200m.lib")
      #else // __MT__
         #pragma comment(lib, "bcb5re200.lib")
      #endif // __MT__
   #endif // _RTLDLL
   
   #endif // VCL
   
   #endif   
   
#endif //__BORLANDC__

#endif // RE_LIB_H




