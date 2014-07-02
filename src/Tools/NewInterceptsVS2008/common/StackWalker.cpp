//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StackWalker.cpp
//
// DESCRIPTION: Contains implementation for the class StackWalker.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 Jan 2004		 Ady K		 File created.
//*************************************************************************

#include "StackWalker.h"

namespace Replacement
{
	StackWalker * StackWalker::sw = NULL;

	//*************************************************************************
	// Method:		StackWalker
	// Description: Constructor for the StackWalker class
	//
	// Parameters:
	//	hTargetThread - Handle of the thread for which the stack trace
	//					is required
	//
	// Return Value: None
	//*************************************************************************
	StackWalker::StackWalker()
	{
	}

	//*************************************************************************
	// Method:		~StackWalker
	// Description: Destructor for the StackWalker class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	StackWalker::~StackWalker()
	{
	}

	//*************************************************************************
	// Method:		getInstance
	// Description: Returns an instance of this class
	//
	// Parameters:
	//	None
	//
	// Return Value: An instance of this class
	//*************************************************************************
	StackWalker * StackWalker::getInstance()
	{
		if (sw == NULL)
			sw = new StackWalker();

		return sw;
	}

	//*************************************************************************
	// Method:		GetStackInfo
	// Description: Gets the call stack for the specified thread
	//
	// Parameters:
	//	hThread - the handle to the target thread
	//	threadContext - the context of the target thread
	//
	// Return Value: (FrameInfo *) An array containing stack trace data
	//*************************************************************************
	SiUtils::SiArray <FrameInfo *> StackWalker::GetStackInfo(HANDLE hThread, CONTEXT &threadContext)
	{
		//Clear the frame array of any previous data
		frameArray.Clear();

		DWORD imageType = IMAGE_FILE_MACHINE_I386;
		HANDLE hProcess = GetCurrentProcess();
		
		int frameNum;						// counts walked frames
		DWORD offsetFromSymbol;				// tells us how far from the symbol we were
		DWORD symOptions;					// symbol handler settings
		IMAGEHLP_SYMBOL *pSym = (IMAGEHLP_SYMBOL *) malloc( IMGSYMLEN + MAXNAMELEN );
		char undName[MAXNAMELEN];			// undecorated name
		char undFullName[MAXNAMELEN];		// undecorated name with all shenanigans
		IMAGEHLP_MODULE Module;
		IMAGEHLP_LINE Line;
		std::string symSearchPath;
		char *tt = 0, *p;

		STACKFRAME s;						// in/out stackframe
		memset( &s, '\0', sizeof s );

		tt = new char[TTBUFLEN];
		// build symbol search path from:
		symSearchPath = "";
		// current directory
		if (GetCurrentDirectory(TTBUFLEN, tt ))
			symSearchPath += tt + std::string( ";" );
		
		// dir with executable
		if (GetModuleFileName(0, tt, TTBUFLEN))
		{
			for ( p = tt + strlen( tt ) - 1; p >= tt; -- p )
			{
				// locate the rightmost path separator
				if ( *p == '\\' || *p == '/' || *p == ':' )
					break;
			}
			
			if ( p != tt )
			{
				if ( *p == ':' ) // we leave colons in place
					++ p;
				*p = '\0'; // eliminate the exe name and last path sep
				symSearchPath += tt + std::string( ";" );
			}
		}
		
		// environment variable _NT_SYMBOL_PATH
		if ( GetEnvironmentVariable( "_NT_SYMBOL_PATH", tt, TTBUFLEN ) )
			symSearchPath += tt + std::string( ";" );
		// environment variable _NT_ALTERNATE_SYMBOL_PATH
		if ( GetEnvironmentVariable( "_NT_ALTERNATE_SYMBOL_PATH", tt, TTBUFLEN ) )
			symSearchPath += tt + std::string( ";" );
		// environment variable SYSTEMROOT
		if ( GetEnvironmentVariable( "SYSTEMROOT", tt, TTBUFLEN ) )
			symSearchPath += tt + std::string( ";" );

		if ( symSearchPath.size() > 0 ) // if we added anything, we have a trailing semicolon
			symSearchPath = symSearchPath.substr( 0, symSearchPath.size() - 1 );

		strncpy( tt, symSearchPath.c_str(), TTBUFLEN );
		tt[TTBUFLEN - 1] = '\0'; // if strncpy() overruns, it doesn't add the null terminator

		// init symbol handler stuff (SymInitialize())
		if (!SymInitialize(hProcess, tt, false ))
			goto cleanup;

		// SymGetOptions()
		symOptions = SymGetOptions();
		symOptions |= SYMOPT_LOAD_LINES;
		symOptions &= ~SYMOPT_UNDNAME;
		SymSetOptions( symOptions );

		// Enumerate modules and tell imagehlp.dll about them.
		enumAndLoadModuleSymbols( hProcess, GetCurrentProcessId() );

		// init STACKFRAME for first call
		// Notes: AddrModeFlat is just an assumption. I hate VDM debugging.
		// Notes: will have to be #ifdef-ed for Alphas; MIPSes are dead anyway,
		// and good riddance.
		s.AddrPC.Offset = threadContext.Eip;
		s.AddrPC.Mode = AddrModeFlat;
		s.AddrFrame.Offset = threadContext.Ebp;
		s.AddrFrame.Mode = AddrModeFlat;

		memset( pSym, '\0', IMGSYMLEN + MAXNAMELEN );
		pSym->SizeOfStruct = IMGSYMLEN;
		pSym->MaxNameLength = MAXNAMELEN;

		memset( &Line, '\0', sizeof Line );
		Line.SizeOfStruct = sizeof Line;

		memset( &Module, '\0', sizeof Module );
		Module.SizeOfStruct = sizeof Module;

		offsetFromSymbol = 0;

		for ( frameNum = 0; ; ++ frameNum )
		{
			FrameInfo * frameInfo = new FrameInfo();

			if (!StackWalk( imageType, hProcess, hThread, &s, &threadContext, NULL,
				SymFunctionTableAccess, SymGetModuleBase, NULL ) )
				break;

			frameInfo->frameNumber = frameNum;
			frameInfo->IsWOWFarCall = s.Far;
			frameInfo->IsVirtualFrame = s.Virtual;
			frameInfo->Eip = s.AddrPC.Offset;				//if 0, the no symbols
			frameInfo->ReturnAddr = s.AddrReturn.Offset;
			frameInfo->FramePtr = s.AddrFrame.Offset;
			frameInfo->StackPtr = s.AddrStack.Offset;

			if (s.AddrPC.Offset == 0)
			{
				//printf( "(-nosymbols- PC == 0)\n" );
			}
			else
			{ // we seem to have a valid PC
				// show procedure info (SymGetSymFromAddr())
				if ( ! SymGetSymFromAddr( hProcess, s.AddrPC.Offset, &offsetFromSymbol, pSym ) )
				{
					/*if ( GetLastError() != 487 )
						printf( "SymGetSymFromAddr(): GetLastError() = %lu\n", GetLastError() );*/
				}
				else
				{
					// UnDecorateSymbolName()
					UnDecorateSymbolName( pSym->Name, undName, MAXNAMELEN, UNDNAME_NAME_ONLY );
					UnDecorateSymbolName( pSym->Name, undFullName, MAXNAMELEN, UNDNAME_COMPLETE );

					strcpy (frameInfo->undecoratedName, undName);
					strcpy (frameInfo->undecoratedFullName, undFullName);
					strcpy (frameInfo->signature, pSym->Name );
					frameInfo->offsetFromSymbol = offsetFromSymbol;
				}

				// show module info (SymGetModuleInfo())
				if (SymGetModuleInfo( hProcess, s.AddrPC.Offset, &Module ) )
				{	
					strcpy (frameInfo->ModuleName, Module.ModuleName);
					strcpy (frameInfo->ImageName, Module.ImageName);
					frameInfo->BaseOfImage = Module.BaseOfImage;

				}

			} // we seem to have a valid PC

			// no return address means no deeper stackframe
			if ( s.AddrReturn.Offset == 0 )
			{
				// avoid misunderstandings in the printf() following the loop
				SetLastError( 0 );
				break;
			}
			
			this->frameArray.Add (frameInfo);

		} // for ( frameNum )

	cleanup:
		// de-init symbol handler etc. (SymCleanup())
		SymCleanup( hProcess );
		free( pSym );
		delete [] tt;
		return frameArray;
	}

	//*************************************************************************
	// Method:		enumAndLoadModuleSymbols
	// Description: Enumerates and loads the module symbols in a given process
	//
	// Parameters:
	//	hProcess - the handle to the process
	//	pid - the ID of the process
	//
	// Return Value: (none)
	//*************************************************************************
	void StackWalker::enumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid )
	{
		ModuleList modules;
		ModuleListIter it;
		char *img, *mod;

		// fill in module list
		fillModuleList( modules, pid, hProcess );

		for ( it = modules.begin(); it != modules.end(); ++ it )
		{
			// unfortunately, SymLoadModule() wants writeable strings
			img = new char[(*it).imageName.size() + 1];
			strcpy( img, (*it).imageName.c_str() );
			mod = new char[(*it).moduleName.size() + 1];
			strcpy( mod, (*it).moduleName.c_str() );

			SymLoadModule( hProcess, 0, img, mod, (*it).baseAddress, (*it).size );
			/*if ( SymLoadModule( hProcess, 0, img, mod, (*it).baseAddress, (*it).size ) == 0 )
				printf( "Error %lu loading symbols for \"%s\"\n",
					GetLastError(), (*it).moduleName.c_str() );
			else
				printf( "Symbols loaded: \"%s\"\n", (*it).moduleName.c_str() );*/

			delete [] img;
			delete [] mod;
		}
	}


	//*************************************************************************
	// Method:		fillModuleList
	// Description: support function
	//
	//*************************************************************************
	bool StackWalker::fillModuleList( ModuleList& modules, DWORD pid, HANDLE hProcess )
	{
		// try toolhelp32 first
		if ( fillModuleListTH32( modules, pid ) )
			return true;
		// nope? try psapi, then
		return fillModuleListPSAPI( modules, pid, hProcess );
	}

	//*************************************************************************
	// Method:		fillModuleListTH32
	// Description: support function
	//
	//*************************************************************************
	bool StackWalker::fillModuleListTH32( ModuleList& modules, DWORD pid )
	{
		// CreateToolhelp32Snapshot()
		typedef HANDLE (__stdcall *tCT32S)( DWORD dwFlags, DWORD th32ProcessID );
		// Module32First()
		typedef BOOL (__stdcall *tM32F)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );
		// Module32Next()
		typedef BOOL (__stdcall *tM32N)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );

		// I think the DLL is called tlhelp32.dll on Win9X, so we try both
		const char *dllname[] = { "kernel32.dll", "tlhelp32.dll" };
		HINSTANCE hToolhelp;
		tCT32S pCT32S;
		tM32F pM32F;
		tM32N pM32N;

		HANDLE hSnap;
		MODULEENTRY32 me = { sizeof me };
		bool keepGoing;
		ModuleEntry e;
		int i;

		for ( i = 0; i < lenof( dllname ); ++ i )
		{
			hToolhelp = LoadLibrary( dllname[i] );
			if ( hToolhelp == 0 )
				continue;
			pCT32S = (tCT32S) GetProcAddress( hToolhelp, "CreateToolhelp32Snapshot" );
			pM32F = (tM32F) GetProcAddress( hToolhelp, "Module32First" );
			pM32N = (tM32N) GetProcAddress( hToolhelp, "Module32Next" );
			if ( pCT32S != 0 && pM32F != 0 && pM32N != 0 )
				break; // found the functions!
			FreeLibrary( hToolhelp );
			hToolhelp = 0;
		}

		if ( hToolhelp == 0 ) // nothing found?
			return false;

		hSnap = pCT32S( TH32CS_SNAPMODULE, pid );
		if ( hSnap == (HANDLE) -1 )
			return false;

		keepGoing = !!pM32F( hSnap, &me );
		while ( keepGoing )
		{
			// here, we have a filled-in MODULEENTRY32
			//printf( "%08lXh %6lu %-15.15s %s\n", me.modBaseAddr, me.modBaseSize, me.szModule, me.szExePath );
			e.imageName = me.szExePath;
			e.moduleName = me.szModule;
			e.baseAddress = (DWORD) me.modBaseAddr;
			e.size = me.modBaseSize;
			modules.push_back( e );
			keepGoing = !!pM32N( hSnap, &me );
		}

		CloseHandle( hSnap );

		FreeLibrary( hToolhelp );

		return modules.size() != 0;
	}

	//*************************************************************************
	// Method:		fillModuleListPSAPI
	// Description: support function
	//
	//*************************************************************************
	bool StackWalker::fillModuleListPSAPI( ModuleList& modules, DWORD pid, HANDLE hProcess )
	{
		// EnumProcessModules()
		typedef BOOL (__stdcall *tEPM)( HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded );
		// GetModuleFileNameEx()
		typedef DWORD (__stdcall *tGMFNE)( HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
		// GetModuleBaseName() -- redundant, as GMFNE() has the same prototype, but who cares?
		typedef DWORD (__stdcall *tGMBN)( HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
		// GetModuleInformation()
		typedef BOOL (__stdcall *tGMI)( HANDLE hProcess, HMODULE hModule, LPMODULEINFO pmi, DWORD nSize );

		HINSTANCE hPsapi;
		tEPM pEPM;
		tGMFNE pGMFNE;
		tGMBN pGMBN;
		tGMI pGMI;

		int i;
		ModuleEntry e;
		DWORD cbNeeded;
		MODULEINFO mi;
		HMODULE *hMods = 0;
		char *tt = 0;

		hPsapi = LoadLibrary( "psapi.dll" );
		if ( hPsapi == 0 )
			return false;

		modules.clear();

		pEPM = (tEPM) GetProcAddress( hPsapi, "EnumProcessModules" );
		pGMFNE = (tGMFNE) GetProcAddress( hPsapi, "GetModuleFileNameExA" );
		pGMBN = (tGMFNE) GetProcAddress( hPsapi, "GetModuleBaseNameA" );
		pGMI = (tGMI) GetProcAddress( hPsapi, "GetModuleInformation" );
		if ( pEPM == 0 || pGMFNE == 0 || pGMBN == 0 || pGMI == 0 )
		{
			// yuck. Some API is missing.
			FreeLibrary( hPsapi );
			return false;
		}

		hMods = new HMODULE[TTBUFLEN / sizeof HMODULE];
		tt = new char[TTBUFLEN];
		// not that this is a sample. Which means I can get away with
		// not checking for errors, but you cannot. :)

		if ( ! pEPM( hProcess, hMods, TTBUFLEN, &cbNeeded ) )
		{
			//printf( "EPM failed, GetLastError() = %lu\n", GetLastError() );
			goto cleanup;
		}

		if ( cbNeeded > TTBUFLEN )
		{
			//printf( "More than %lu module handles. Huh?\n", lenof( hMods ) );
			goto cleanup;
		}

		for ( i = 0; i < cbNeeded / sizeof hMods[0]; ++ i )
		{
			// for each module, get:
			// base address, size
			pGMI( hProcess, hMods[i], &mi, sizeof mi );
			e.baseAddress = (DWORD) mi.lpBaseOfDll;
			e.size = mi.SizeOfImage;
			// image file name
			tt[0] = '\0';
			pGMFNE( hProcess, hMods[i], tt, TTBUFLEN );
			e.imageName = tt;
			// module name
			tt[0] = '\0';
			pGMBN( hProcess, hMods[i], tt, TTBUFLEN );
			e.moduleName = tt;
			/*printf( "%08lXh %6lu %-15.15s %s\n", e.baseAddress,
				e.size, e.moduleName.c_str(), e.imageName.c_str() );*/

			modules.push_back( e );
		}

	cleanup:
		if ( hPsapi )
			FreeLibrary( hPsapi );
		delete [] tt;
		delete [] hMods;

		return modules.size() != 0;
	}
}