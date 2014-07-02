//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StackWalker.h
//
// DESCRIPTION: Contains definition for the class StackWalker
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 Jan 2004		 Ady K		 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <psapi.h>
#include <math.h>

#include <string>
#include <vector>
#include <stdio.h>
#include "SiArray.h"

#include <Tlhelp32.h.>

#define lenof(a) (sizeof(a) / sizeof((a)[0]))
#define MAXNAMELEN 1024 // max name length for found symbols
#define IMGSYMLEN ( sizeof IMAGEHLP_SYMBOL )
#define TTBUFLEN 65536 // for a temp buffer

// imagehlp.h must be compiled with packing to eight-byte-boundaries,
// but does nothing to enforce that, so we do!
#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )

namespace Replacement
{
	//*************************************************************************
	// Class:		FrameInfo
	// Description: Storage class for CallStack data
	//
	//*************************************************************************	
	class FrameInfo
	{
		public:
		FrameInfo() {};
		~FrameInfo() {};

		int frameNumber;
		BOOL IsWOWFarCall;
		BOOL IsVirtualFrame;
		DWORD64 Eip;
		DWORD64 ReturnAddr;
		DWORD64 FramePtr;
		DWORD64 StackPtr;
		char undecoratedName[1024];
		char undecoratedFullName[1024];
		char signature[1024];
		DWORD offsetFromSymbol;

		char ModuleName[32];
		char ImageName[256];
		DWORD64 BaseOfImage;
	};

	//*************************************************************************
	// Structure:		ModuleEntry
	// Description: Storage structure for CallStack data
	//*************************************************************************	
	struct ModuleEntry
	{
		std::string imageName;
		std::string moduleName;
		DWORD baseAddress;
		DWORD size;
	};

	typedef std::vector< ModuleEntry > ModuleList;
	typedef ModuleList::iterator ModuleListIter;


	//*************************************************************************
	// Class:			StackWalker
	// Description: Main class which performs the stack trace
	//*************************************************************************	
	class StackWalker
	{
		public:
			StackWalker();
			~StackWalker();

			bool WalkStack();
			SiUtils::SiArray <FrameInfo *> frameArray;
			static StackWalker * getInstance();
			SiUtils::SiArray <FrameInfo *> GetStackInfo(HANDLE hThread, CONTEXT &threadContext);
		
		private:
			static StackWalker * sw;			
			void enumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid );
			bool fillModuleList( ModuleList& modules, DWORD pid, HANDLE hProcess );
			bool fillModuleListTH32( ModuleList& modules, DWORD pid );
			bool fillModuleListPSAPI( ModuleList& modules, DWORD pid, HANDLE hProcess );
	};
}