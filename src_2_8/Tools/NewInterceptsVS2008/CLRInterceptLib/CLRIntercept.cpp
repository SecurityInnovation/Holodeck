//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRIntercept.cpp
//
// DESCRIPTION: Contains functions to modify a .NET binary for interception
//				of one or more methods in that binary.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 7 Apr 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <stdio.h>
#include "CLRIntercept.h"
#include "Registry.h"
#include "defines.h"
#include "PEFile.h"
#include <io.h>

using namespace SiUtils;


// Container for the list of methods to be intercepted
SiArray<CLRInterceptDescription> g_vidIntercepts;

// Container for the list of handler modules
SiArray<PEFile*> g_vpeHandlerModules;

// Container for the list of modules that have been intercepted
SiArray<CLRInterceptedModule> g_vimInterceptedModules;

// Module information for HeatDotNet.dll
PEFile* g_ppeHeatDotNetModule = NULL;

// Moudle information for DotNotReplacementLibrary.dll
PEFile* g_ppeDotNetReplacementLibraryModule = NULL;

// Unique ID for the generated method delegate and method info classes, see CLRInterceptMethod
DWORD g_dwMethodDelegateID = 0;

// Total checksum for all replacement libraries
DWORD g_dwReplacementLibraryChecksum = 0;

// Total number of intercepts processed so far
DWORD g_dwInterceptsProcessed = 0;


//*************************************************************************
// Function:	 CLRInitializeIntercepts
// Description: Initializes .NET interception.
//				
// Parameters:
//	None
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRInitializeIntercepts()
{
	Registry reg;
	SiString sInstalledPath;
	SiString sReplacementLibFolder;

	if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
		return false;
	if (!reg.Read(INSTALL_PATH_VALUE_NAME, sInstalledPath))
		return false;
	if (!reg.Read(PLUGIN_DLL_PATH_VALUE_NAME, sReplacementLibFolder))
		return false;

	// HeatDotNet.dll is in the same folder as the replacement libraries
	sInstalledPath += "\\";
	sInstalledPath += sReplacementLibFolder;

	// Find the location of HeatDotNet.dll
	char szHeatDotNetFullPath[MAX_PATH];
	if (!SearchPath(sInstalledPath, "HeatDotNet.dll", NULL, MAX_PATH, szHeatDotNetFullPath, NULL))
		return false;

	// Load HeatDotNet.dll module information
	g_ppeHeatDotNetModule = new PEFile();
	if (!g_ppeHeatDotNetModule)
		return false;

	if (!g_ppeHeatDotNetModule->load(szHeatDotNetFullPath, true))
	{
		delete g_ppeHeatDotNetModule;
		g_ppeHeatDotNetModule = NULL;
		return false;
	}

	g_dwReplacementLibraryChecksum += g_ppeHeatDotNetModule->getChecksum();

	// Find the location of DotNetReplacementLibrary.dll
	char szDotNetReplacementLibraryFullPath[MAX_PATH];
	if (!SearchPath(sInstalledPath, "DotNetReplacementLibrary.dll", NULL, MAX_PATH, szDotNetReplacementLibraryFullPath, NULL))
		return false;

	// Load DotNetReplacementLibrary.dll module information
	g_ppeDotNetReplacementLibraryModule = new PEFile();
	if (!g_ppeDotNetReplacementLibraryModule)
		return false;

	if (!g_ppeDotNetReplacementLibraryModule->load(szDotNetReplacementLibraryFullPath, true))
	{
		delete g_ppeDotNetReplacementLibraryModule;
		g_ppeDotNetReplacementLibraryModule = NULL;
		return false;
	}

	g_dwReplacementLibraryChecksum += g_ppeDotNetReplacementLibraryModule->getChecksum();

	// Initialization success
	return true;
}


//*************************************************************************
// Function:	 CLRDeinitializeIntercepts
// Description: Frees resources used by .NET interception.
//				
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void CLRDeinitializeIntercepts()
{
	// Free HeatDotNet.dll module information
	if (g_ppeHeatDotNetModule)
	{
		delete g_ppeHeatDotNetModule;
		g_ppeHeatDotNetModule = NULL;
	}

	// Free DotNetReplacementLibrary.dll module information
	if (g_ppeDotNetReplacementLibraryModule)
	{
		delete g_ppeDotNetReplacementLibraryModule;
		g_ppeDotNetReplacementLibraryModule = NULL;
	}

	// Free handler modules
	for (DWORD i=0; i<g_vpeHandlerModules.GetLength(); i++)
		delete g_vpeHandlerModules[i];
	g_vpeHandlerModules.Clear();

	g_vidIntercepts.Clear();
	g_vimInterceptedModules.Clear();

	g_dwInterceptsProcessed = 0;
	g_dwReplacementLibraryChecksum = 0;
}


//*************************************************************************
// Function:	 CLRAddInterceptHandlerModule
// Description: Adds an interception handler module for use in .NET
//				interception.  All public static methods in the
//				HeatInterceptHandlers namespace are interception handlers.
//				The method being intercepted is of the same name as the
//				handler method inside of the type name given by the
//				InterceptTypeName attribute applied to the handler class.
//
//				This function must be called during initialization in
//				order to guarantee interception of the desired method.
//				If the module being intercepted has already been loaded
//				before this function is called, the methods will not
//				be intercepted.
//				
// Parameters:
//	sModuleName - Path to the interception handler module
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRAddInterceptHandlerModule(const SiString& sModuleName)
{
	wchar_t wszFullPath[MAX_PATH];

	//If file cannot be located, then try and look for it in the Holodeck
	//plugins folder
	if (_access (sModuleName, 0) == -1)
	{
		Registry reg;
		SiString sInstalledPath;
		SiString sReplacementLibFolder;

		if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
			return false;
		if (!reg.Read(INSTALL_PATH_VALUE_NAME, sInstalledPath))
			return false;
		if (!reg.Read(PLUGIN_DLL_PATH_VALUE_NAME, sReplacementLibFolder))
			return false;

		// Handler DLLs are in the same folder as the replacement libraries
		sInstalledPath += "\\";
		sInstalledPath += sReplacementLibFolder;

		// Find the full path name of the module
		if (!SearchPathW(sInstalledPath, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, NULL))
			return false;
	}
	else
		wcscpy (wszFullPath, sModuleName);

	// Load the module information
	PEFile* ppeHandlerModule = new PEFile();
	if (!ppeHandlerModule)
		return false;

	if (!ppeHandlerModule->load(wszFullPath, true))
	{
		delete ppeHandlerModule;
		return false;
	}

	bool fAddedIntercept = false; // true if we found a function to intercept in this handler module

	SiArray<CLRTypeName> vTypes = ppeHandlerModule->enumerateTypes();
	for (DWORD i=0; i<vTypes.GetLength(); i++)
	{
		if ((vTypes[i].sNamespace == "HeatInterceptHandlers") ||
			(!strncmp((char*)vTypes[i].sNamespace, "HeatInterceptHandlers.", strlen("HeatIntercept."))))
		{
			// HeatInterceptHandlers namespace, check for the InterceptTypeName attribute
			CLRTypeName tnAttributeType;
			tnAttributeType.sNamespace = "HeatDotNet";
			tnAttributeType.sName = "InterceptTypeName";
			const CLRBlobHeapEntry* pAttr = ppeHandlerModule->getAttributeValue(vTypes[i], tnAttributeType);
			if (!pAttr)
				continue;

			// InterceptTypeName attribute takes a single string parameter.  This string
			// is found at offset 3 within the value data.
			SiString sFullTypeName = SiString((char*)&pAttr->pbData[3]);
			// Parse the type name into namespace and class name
			int dwLastDot = sFullTypeName.FindLast('.');
			int dwLastPlus = sFullTypeName.FindLast('+');
			CLRTypeName tnType;
			if (dwLastPlus != -1)
			{
				// Nested class
				sFullTypeName.SubString(tnType.sOuterClass, 0, dwLastPlus - 1);
				sFullTypeName.SubString(tnType.sName, dwLastPlus + 1);
			}
			else if (dwLastDot == -1)
			{
				// No '.' found, so namespace is empty
				tnType.sNamespace = SiString("");
				tnType.sName = sFullTypeName;
			}
			else
			{
				sFullTypeName.SubString(tnType.sNamespace, 0, dwLastDot - 1);
				sFullTypeName.SubString(tnType.sName, dwLastDot + 1);
			}

			// Enumerate methods in the type and add intercepts for the handler methods
			SiArray<CLRMemberDescription> vMethods = ppeHandlerModule->enumerateMethods(vTypes[i]);
			for (DWORD j=0; j<vMethods.GetLength(); j++)
			{
				// Only intercept methods with public static interception handler methods
				if ((vMethods[j].dwFlags & CLR_METHOD_ACCESS_MASK) != CLR_METHOD_ACCESS_PUBLIC)
					continue;
				if (!(vMethods[j].dwFlags & CLR_METHOD_CONTRACT_STATIC))
					continue;

				CLRInterceptDescription idIntercept;

				if (vMethods[j].sName == "ctor")
				{
					// Constructor method.  Fill in info about method to intercept
					idIntercept.mdInterceptMethod.tnType = tnType;
					idIntercept.mdInterceptMethod.sName = ".ctor";
					idIntercept.fConstructorIntercept = true;
					// Constructors are always instance methods
					idIntercept.mdInterceptMethod.sSignature = vMethods[j].sSignature.convertToInstanceMethod();
					idIntercept.mdInstanceInterceptMethod = idIntercept.mdInterceptMethod;
				}
				else
				{
					// Non-constructor method.  Fill in info about method to intercept
					idIntercept.mdInterceptMethod.tnType = tnType;
					idIntercept.mdInterceptMethod.sName = vMethods[j].sName;
					idIntercept.mdInterceptMethod.sSignature = vMethods[j].sSignature;
					idIntercept.fConstructorIntercept = false;
					// Method may be static or an instance, set up signatures for
					// both cases
					idIntercept.mdInterceptMethod.sSignature = vMethods[j].sSignature.convertToInstanceMethod();
					idIntercept.mdInstanceInterceptMethod = idIntercept.mdInterceptMethod;
					// Remove the "instance" flag for the non-instance signature form
					idIntercept.mdInterceptMethod.sSignature.setCallConv(
						idIntercept.mdInterceptMethod.sSignature.getCallConv() & (~CLR_CALLCONV_INSTANCE));
				}

				idIntercept.mdHandlerMethod = vMethods[j];
				idIntercept.ppeHandlerModule = ppeHandlerModule;

				g_vidIntercepts.Add(idIntercept);

//				printf("Intercepting %s.%s.%s\n", (char*)idIntercept.mdInterceptMethod.tnType.sNamespace,
//					(char*)idIntercept.mdInterceptMethod.tnType.sName, (char*)idIntercept.mdInterceptMethod.sName);

				fAddedIntercept = true;
			}
		}
	}

	if (!fAddedIntercept)
	{
		// No intercepts found in this module.  Free the resources used by the module information
		delete ppeHandlerModule;
		return true;
	}

	// Module contains intercept handlers.  Add the module to the handler module list
	g_vpeHandlerModules.Add(ppeHandlerModule);
	g_dwReplacementLibraryChecksum += ppeHandlerModule->getChecksum();
	return true;
}


//*************************************************************************
// Function:	 CLRAddInterceptHandlerModuleForCacheCheck
// Description: Adds an interception handler module for use in .NET
//				interception, but only for use in a cache check.  This
//				is significantly faster, as only the checksum needs to
//				be computed.
//				
// Parameters:
//	sModuleName - Path to the interception handler module
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRAddInterceptHandlerModuleForCacheCheck(const SiUtils::SiString& sFileName)
{
	Registry reg;
	SiString sInstalledPath;
	SiString sReplacementLibFolder;

	if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
		return false;
	if (!reg.Read(INSTALL_PATH_VALUE_NAME, sInstalledPath))
		return false;
	if (!reg.Read(PLUGIN_DLL_PATH_VALUE_NAME, sReplacementLibFolder))
		return false;

	// Handler DLLs are in the same folder as the replacement libraries
	sInstalledPath += "\\";
	sInstalledPath += sReplacementLibFolder;

	// Find the full path name of the module
	wchar_t wszFullPath[MAX_PATH];
	if (!SearchPathW(sInstalledPath, (wchar_t*)sFileName, NULL, MAX_PATH, wszFullPath, NULL))
		return false;

	// Load the module information
	PEFile* ppeHandlerModule = new PEFile();
	if (!ppeHandlerModule)
		return false;

	if (!ppeHandlerModule->load(wszFullPath, false))
	{
		delete ppeHandlerModule;
		return false;
	}

	g_dwReplacementLibraryChecksum += ppeHandlerModule->getChecksum();
	delete ppeHandlerModule;
	return true;
}


//*************************************************************************
// Function:	 CLRAllowModuleLoad
// Description: Some modules have precompiled native code present in the
//				Global Assembly Cache.  This precompiled native code will
//				be used instead of our intercepted methods unless we
//				deny loading the precompiled native binary.  This function
//				will determine if a given module name is the precompiled
//				native code version of a module we are intercepting.  If
//				so, the module load should be denied to force a JIT
//				compile of the newly intercepted methods.
//				
// Parameters:
//	sModuleName - Path to the module to check
//
// Return Value: true if the module is OK to load, false if the module
//	should not be loaded (if it is a precompiled native code version of
//	an intercepted module)
//*************************************************************************
bool CLRAllowModuleLoad(const SiString& sModuleName)
{
	wchar_t wszFullPath[MAX_PATH];
	wchar_t wszNativeImagesPath[MAX_PATH];
	wchar_t *wszFileName;

	if ((!g_ppeHeatDotNetModule) || (!g_ppeDotNetReplacementLibraryModule))
	{
		// Haven't initialized yet
		return true;
	}

	// Find the full path name of the module
	if (!SearchPathW(NULL, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, &wszFileName))
	{
		// Not a path to a known file
		return true;
	}

	if (!GetWindowsDirectoryW(wszNativeImagesPath, MAX_PATH))
	{
		// Couldn't get Windows directory
		return true;
	}

	// Construct the path to the native images directory from the path to the Windows directory
	if (wszNativeImagesPath[wcslen(wszNativeImagesPath) - 1] == '\\')
		wszNativeImagesPath[wcslen(wszNativeImagesPath) - 1] = 0;
	wcscat(wszNativeImagesPath, L"\\assembly\\nativeimages");

	// Is the module being loaded within the native images directory?
	if (_wcsnicmp(wszFullPath, wszNativeImagesPath, wcslen(wszNativeImagesPath)) != 0)
	{
		// Not a precompiled native version of a .NET module
		return true;
	}

	// The module is a precompiled native version of a .NET module.  Don't allow the native
	// precomiled version to be loaded.  This will force the CLR to JIT the methods in the
	// module, allowing our interception code to run.
	return false;
}


//*************************************************************************
// Function:	 CLRInterceptCreateDelegateType
// Description: Creates a delegate type to hold a function pointer to
//		a specific method.
//				
// Parameters:
//	ppeModule - Module containing method to create a delegate for
//  szConstructorDelegateName - Name of delegate type to create
//  mdInterceptMethod - Name and signature of method to intercept
//  dwDelegateConstructorToken - Receives the token of the constructor
//		method for the new delegate type
//  dwDelegateInvokeToken - Receives the token of the Invoke method
//		for the new delegate type
//	dwInterceptedTypeToken - Receives the token for the type containing
//		the intercepted method
//
// Return Value: true if the delegate type was successfully created, false
//		otherwise
//*************************************************************************
bool CLRInterceptCreateDelegateType(PEFile* ppeModule, char* szMethodDelegateName,
									CLRMemberDescription& mdInterceptMethod, DWORD& dwDelegateConstructorToken,
									DWORD& dwDelegateInvokeToken, DWORD dwInterceptedTypeToken)
{
	// Make sure System.MulticastDelegate is available
	CLRTypeName tnMulticastDelegate;
	tnMulticastDelegate.sNamespace = "System";
	tnMulticastDelegate.sName = "MulticastDelegate";
	if (ppeModule->findTypeToken(tnMulticastDelegate) == 0)
	{
		// System.MulticastDelegate does not exist yet, create a TypeRef
		DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
		if (dwAssemblyRef == 0)
			return false;

		if (ppeModule->createTypeRef(dwAssemblyRef, tnMulticastDelegate) == 0)
			return false;
	}

	// Create the delegate type.  Delegate types are derived from System.MulticastDelegate and have the following members:
	//		void .ctor(Object, IntPtr)
	//		System.IAsyncResult BeginInvoke(<Method parameters>, AsyncCallback, Object)
	//		void EndInvoke(System.IAsyncResult)
	//		void Invoke(<Method parameters>
	// None of these methods contain method code.  They are marked with the "runtime" flag, meaning that the .NET
	// runtime itself will implement these methods.
	CLRNewTypeDescription ntdDelegateType;
	ntdDelegateType.tnTypeName.sNamespace = "";
	ntdDelegateType.tnTypeName.sName = szMethodDelegateName;
	ntdDelegateType.tnTypeName.sOuterClass = ppeModule->getFullTypeName(dwInterceptedTypeToken);
	ntdDelegateType.tnBaseName.sNamespace = "System";
	ntdDelegateType.tnBaseName.sName = "MulticastDelegate";
	ntdDelegateType.dwFlags = CLR_TYPEDEF_VIS_PUBLIC | CLR_TYPEDEF_LAYOUT_AUTO | CLR_TYPEDEF_SEMANTICS_SEALED |
		CLR_TYPEDEF_STRING_ANSI;

	// Add the constructor method
	CLRNewMethodDescription nmdDelegateCtorMethod;
	nmdDelegateCtorMethod.dwImplFlags = CLR_METHODIMPL_CODETYPE_RUNTIME;
	nmdDelegateCtorMethod.mdMember.dwFlags = CLR_METHOD_ACCESS_PUBLIC | CLR_METHOD_CONTRACT_HIDEBYSIG |
		CLR_METHOD_IMPL_SPECIALNAME | CLR_METHOD_RESERVED_RTSPECIALNAME;
	nmdDelegateCtorMethod.mdMember.sName = ".ctor";
	nmdDelegateCtorMethod.mdMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	nmdDelegateCtorMethod.mdMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_VOID));
	nmdDelegateCtorMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_OBJECT));
	nmdDelegateCtorMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_INTPTR));
	ntdDelegateType.vmdMethods.Add(nmdDelegateCtorMethod);

	CLRTypeName tnIAsyncResult;
	tnIAsyncResult.sNamespace = "System";
	tnIAsyncResult.sName = "IAsyncResult";
	if (ppeModule->findTypeToken(tnIAsyncResult) == 0)
	{
		// Type does not exist yet, create a TypeRef
		DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
		if (dwAssemblyRef == 0)
			return false;

		if (ppeModule->createTypeRef(dwAssemblyRef, tnIAsyncResult) == 0)
			return false;
	}

	CLRTypeName tnAsyncCallback;
	tnAsyncCallback.sNamespace = "System";
	tnAsyncCallback.sName = "AsyncCallback";
	if (ppeModule->findTypeToken(tnAsyncCallback) == 0)
	{
		// Type does not exist yet, create a TypeRef
		DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
		if (dwAssemblyRef == 0)
			return false;

		if (ppeModule->createTypeRef(dwAssemblyRef, tnAsyncCallback) == 0)
			return false;
	}

	// Add the BeginInvoke method
	CLRNewMethodDescription nmdDelegateBeginInvokeMethod;
	nmdDelegateBeginInvokeMethod.dwImplFlags = CLR_METHODIMPL_CODETYPE_RUNTIME;
	nmdDelegateBeginInvokeMethod.mdMember.dwFlags = CLR_METHOD_ACCESS_PUBLIC | CLR_METHOD_CONTRACT_HIDEBYSIG |
		CLR_METHOD_CONTRACT_VIRTUAL | CLR_METHOD_VIRTUAL_NEWSLOT;
	nmdDelegateBeginInvokeMethod.mdMember.sName = "BeginInvoke";
	nmdDelegateBeginInvokeMethod.mdMember.sSignature = mdInterceptMethod.sSignature;
	nmdDelegateBeginInvokeMethod.mdMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	nmdDelegateBeginInvokeMethod.mdMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_CLASS, tnIAsyncResult));
	nmdDelegateBeginInvokeMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_CLASS, tnAsyncCallback));
	nmdDelegateBeginInvokeMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_OBJECT));
	ntdDelegateType.vmdMethods.Add(nmdDelegateBeginInvokeMethod);

	// Add the EndInvoke method
	CLRNewMethodDescription nmdDelegateEndInvokeMethod;
	nmdDelegateEndInvokeMethod.dwImplFlags = CLR_METHODIMPL_CODETYPE_RUNTIME;
	nmdDelegateEndInvokeMethod.mdMember.dwFlags = CLR_METHOD_ACCESS_PUBLIC | CLR_METHOD_CONTRACT_HIDEBYSIG |
		CLR_METHOD_CONTRACT_VIRTUAL | CLR_METHOD_VIRTUAL_NEWSLOT;
	nmdDelegateEndInvokeMethod.mdMember.sName = "EndInvoke";
	nmdDelegateEndInvokeMethod.mdMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	nmdDelegateEndInvokeMethod.mdMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_VOID));
	nmdDelegateEndInvokeMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_CLASS, tnIAsyncResult));
	ntdDelegateType.vmdMethods.Add(nmdDelegateEndInvokeMethod);

	// Add the Invoke method
	CLRNewMethodDescription nmdDelegateInvokeMethod;
	nmdDelegateInvokeMethod.dwImplFlags = CLR_METHODIMPL_CODETYPE_RUNTIME;
	nmdDelegateInvokeMethod.mdMember.dwFlags = CLR_METHOD_ACCESS_PUBLIC | CLR_METHOD_CONTRACT_HIDEBYSIG |
		CLR_METHOD_CONTRACT_VIRTUAL;
	nmdDelegateInvokeMethod.mdMember.sName = "Invoke";
	nmdDelegateInvokeMethod.mdMember.sSignature = mdInterceptMethod.sSignature;
	nmdDelegateInvokeMethod.mdMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	ntdDelegateType.vmdMethods.Add(nmdDelegateInvokeMethod);

	// Add the new constructor delegate type to the executable
	DWORD dwDelegateTypeToken = ppeModule->createType(ntdDelegateType);
	if (dwDelegateTypeToken == 0)
	{
		// Type creation failed
		return false;
	}

	// Make the delegate a nested type within the intercepted type
	if (!ppeModule->addNestedType(dwInterceptedTypeToken, dwDelegateTypeToken))
		return false;

	// Get a token for the constructor of the new delegate type
	nmdDelegateCtorMethod.mdMember.tnType = ntdDelegateType.tnTypeName;
	CLRMethod* pDelegateConstructorMethod = ppeModule->findMethod(nmdDelegateCtorMethod.mdMember, false);
	if (!pDelegateConstructorMethod)
	{
		// Couldn't find method
		return false;
	}
	// Construct a token using the returned method
	dwDelegateConstructorToken = CLR_TOKEN_METHOD | (pDelegateConstructorMethod->getMethodIndex() + 1);
	delete pDelegateConstructorMethod;

	// Get a token for Invoke method of the new delegate type
	nmdDelegateInvokeMethod.mdMember.tnType = ntdDelegateType.tnTypeName;
	CLRMethod* pDelegateInvokeMethod = ppeModule->findMethod(nmdDelegateInvokeMethod.mdMember, false);
	if (!pDelegateInvokeMethod)
	{
		// Couldn't find method
		return false;
	}
	// Construct a token using the returned method
	dwDelegateInvokeToken = CLR_TOKEN_METHOD | (pDelegateInvokeMethod->getMethodIndex() + 1);
	delete pDelegateInvokeMethod;

	return true;
}


//*************************************************************************
// Function:	 CLRGetTypeName
// Description: Gets the type name from a type signature
//
// Parameters:
//  sSig - Type signature
//
// Return Value: Type name
//*************************************************************************
CLRTypeName CLRGetTypeName(CLRTypeSignature& sSig)
{
	if ((sSig.getTypeCode() == CLR_DATATYPE_CLASS) || (sSig.getTypeCode() == CLR_DATATYPE_VALUETYPE))
		return sSig.getAssocTypeName();
	else if (sSig.getTypeCode() == CLR_DATATYPE_STRING)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "String";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_BOOLEAN)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Boolean";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_CHAR)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Char";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_SBYTE)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "SByte";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_BYTE)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Byte";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_INT16)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Int16";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_UINT16)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "UInt16";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_INT32)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Int32";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_UINT32)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "UInt32";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_INT64)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Int64";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_UINT64)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "UInt64";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_SINGLE)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Single";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_DOUBLE)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "Double";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_INTPTR)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "IntPtr";
		return tnType;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_UINTPTR)
	{
		CLRTypeName tnType;
		tnType.sNamespace = "System";
		tnType.sName = "UIntPtr";
		return tnType;
	}
	CLRTypeName tnType;
	tnType.sNamespace = "System";
	tnType.sName = "Void";
	return tnType;
}


//*************************************************************************
// Function:	 CLRConvertToType
// Description: Outputs IL code to convert an Object to a specific type
//
// Parameters:
//	ppeModule - Module containing the method
//  mMethod - Method to insert IL code into
//  sSig - Type to convert to
//  dwParamLocalIndex - Storage for local variable index if this parameter
//		is a reference parameter
//
// Return Value: true on success
//*************************************************************************
bool CLRConvertToType(PEFile* ppeModule, CLRMethod& mMethod, CLRTypeSignature& sSig, DWORD& dwParamLocalIndex)
{
	BYTE bOpcode;

	if ((sSig.getTypeCode() == CLR_DATATYPE_CLASS) || (sSig.getTypeCode() == CLR_DATATYPE_STRING))
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x74; // castclass
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_BYREF)
	{
		// Insert a local variable to store the current value
		dwParamLocalIndex = mMethod.getLocalVars().getParamCount();
		mMethod.getLocalVars().insertParamAtEnd(*sSig.getAssocType());

		CLRConvertToType(ppeModule, mMethod, *sSig.getAssocType(), dwParamLocalIndex);

		if (dwParamLocalIndex == 0)
		{
			bOpcode = 0x0a; // stloc.0
			if (!mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}
		else if (dwParamLocalIndex == 1)
		{
			bOpcode = 0x0b; // stloc.1
			if (!mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}
		else if (dwParamLocalIndex == 2)
		{
			bOpcode = 0x0c; // stloc.2
			if (!mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}
		else if (dwParamLocalIndex == 3)
		{
			bOpcode = 0x0d; // stloc.3
			if (!mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}
		else
		{
			bOpcode = 0x13; // stloc.s
			if (!mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
			bOpcode = (BYTE)dwParamLocalIndex;
			if (!mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}

		// Push a reference to the local on the stack
		bOpcode = 0x12; // ldloc.a
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		bOpcode = (BYTE)dwParamLocalIndex;
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_VALUETYPE)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x71; // ldobj
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;
	}
	else if ((sSig.getTypeCode() == CLR_DATATYPE_BOOLEAN) || (sSig.getTypeCode() == CLR_DATATYPE_SBYTE))
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x46; // ldind.i1
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if ((sSig.getTypeCode() == CLR_DATATYPE_CHAR) || (sSig.getTypeCode() == CLR_DATATYPE_UINT16))
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x49; // ldind.u2
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_BYTE)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x47; // ldind.u1
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_INT16)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x48; // ldind.i2
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_INT32)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x4a; // ldind.i4
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_UINT32)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x4b; // ldind.u4
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if ((sSig.getTypeCode() == CLR_DATATYPE_INT64) || (sSig.getTypeCode() == CLR_DATATYPE_UINT64))
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x4c; // ldind.i8
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_SINGLE)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x4e; // ldind.r4
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if (sSig.getTypeCode() == CLR_DATATYPE_DOUBLE)
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x4f; // ldind.r8
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if ((sSig.getTypeCode() == CLR_DATATYPE_INTPTR) || (sSig.getTypeCode() == CLR_DATATYPE_UINTPTR))
	{
		CLRTypeName tnType = CLRGetTypeName(sSig);
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x79; // unbox
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;

		bOpcode = 0x71; // ldobj
		if (!mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;
	}
	return true;
}


//*************************************************************************
// Function:	 CLRInterceptCreateMethodInterceptInfoType
// Description: Creates a type derived from MethodInterceptInfo that
//		holds both the "this" pointer and a delegate for an intercepted
//		method.  An instance of this class will be passed to the method
//		handler.
//
// Parameters:
//	ppeModule - Module containing method to create a MethodInterceptInfo
//		type for
//  szMethodInterceptInfoName - Name of type to create
//  pMethod - CLRMethod object for the method being intercepted
//  tnDelegateType - Name of delegate type for the method being intercepted
//  dwMethodInterceptInfoToken - Token of MethodInterceptInfo
//		constuctor, used for calling the base class
//  dwDelegateInvokeToken - Token of the Invoke method for the specific
//		delegate type for this method
//  dwMethodInfoConstructorToken - Receives the token for the
//		constructor method of the new ConstructorInterceptInfo type
//	dwInterceptedTypeToken - Receives the token for the type containing
//		the intercepted method
//
// Return Value: true if the type was successfully created, false
//		otherwise
//*************************************************************************
bool CLRInterceptCreateMethodInterceptInfoType(PEFile* ppeModule, char* szMethodInterceptInfoName,
											   CLRMethod* pMethod, CLRTypeName tnDelegateType,
											   DWORD dwMethodInterceptInfoToken,
											   DWORD dwDelegateInvokeToken, DWORD& dwMethodInfoConstructorToken,
											   DWORD dwInterceptedTypeToken)
{
	// Create the MethodInterceptInfo type.  This needs the following members:
	//		void .ctor(Object, <constructor delegate type>)
	//		void Invoke(Object[])
	//		<constructor delegate type> methodDelegate
	CLRNewTypeDescription ntdMethodInfoType;
	ntdMethodInfoType.tnTypeName.sNamespace = "";
	ntdMethodInfoType.tnTypeName.sName = szMethodInterceptInfoName;
	ntdMethodInfoType.tnTypeName.sOuterClass = ppeModule->getFullTypeName(dwInterceptedTypeToken);
	ntdMethodInfoType.tnBaseName.sNamespace = "HeatDotNet";
	ntdMethodInfoType.tnBaseName.sName = "MethodInterceptInfo";
	ntdMethodInfoType.dwFlags = CLR_TYPEDEF_VIS_PUBLIC | CLR_TYPEDEF_LAYOUT_AUTO | CLR_TYPEDEF_STRING_ANSI |
		CLR_TYPEDEF_TYPEIMPL_BEFOREFIELDINIT;

	// Set up structure for .ctor method
	CLRNewMethodDescription nmdMethodInfoCtorMethod;
	nmdMethodInfoCtorMethod.dwImplFlags = 0;
	nmdMethodInfoCtorMethod.mdMember.dwFlags = CLR_METHOD_ACCESS_PUBLIC | CLR_METHOD_CONTRACT_HIDEBYSIG |
		CLR_METHOD_IMPL_SPECIALNAME | CLR_METHOD_RESERVED_RTSPECIALNAME;
	nmdMethodInfoCtorMethod.mdMember.sName = ".ctor";
	nmdMethodInfoCtorMethod.mdMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	nmdMethodInfoCtorMethod.mdMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_VOID));
	nmdMethodInfoCtorMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_OBJECT));
	nmdMethodInfoCtorMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_CLASS, tnDelegateType));

	// Need to know the token for the methodDelegate field that will be added.  When createType is called it
	// will be added to the very end of the Field table, so just create a token based on this.
	DWORD dwMethodDelegateFieldToken = CLR_TOKEN_FIELD | (ppeModule->getMetadata().getTable(CLR_TABLE_FIELD).
		getRowCount() + 1);

	// Write out IL code for .ctor method:
	//		ldarg.0
	//		ldarg.1
	//		call ConstructorInterceptInfo..ctor
	//		ldarg.0
	//		ldarg.2
	//		stfld methodDelegate
	//		ret
	BYTE bOpcode = 0x02; // ldarg.0
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x03; // ldarg.1
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x28; // call ConstructorInterceptInfo..ctor
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd((BYTE*)&dwMethodInterceptInfoToken, 4))
		return false;

	bOpcode = 0x02; // ldarg.0
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x04; // ldarg.2
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x7d; // stfld methodDelegate
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd((BYTE*)&dwMethodDelegateFieldToken, 4))
		return false;

	bOpcode = 0x2a; // ret
	if (!nmdMethodInfoCtorMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	// Add .ctor method to the type
	ntdMethodInfoType.vmdMethods.Add(nmdMethodInfoCtorMethod);

	// Set up structure for Invoke method
	CLRNewMethodDescription nmdMethodInfoInvokeMethod;
	nmdMethodInfoInvokeMethod.dwImplFlags = 0;
	nmdMethodInfoInvokeMethod.mdMember.dwFlags = CLR_METHOD_ACCESS_PUBLIC | CLR_METHOD_CONTRACT_HIDEBYSIG |
		CLR_METHOD_CONTRACT_VIRTUAL;
	nmdMethodInfoInvokeMethod.mdMember.sName = "Invoke";
	nmdMethodInfoInvokeMethod.mdMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	nmdMethodInfoInvokeMethod.mdMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_OBJECT));
	nmdMethodInfoInvokeMethod.mdMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_VECTOR,
		CLRTypeSignature(CLR_DATATYPE_OBJECT)));

	// Make sure the stack is big enough for us to do our work
	if (nmdMethodInfoInvokeMethod.mMethod.getMaxStack() < (pMethod->getParamCount() + 1))
		nmdMethodInfoInvokeMethod.mMethod.setMaxStack(pMethod->getParamCount() + 1);

	// Write out code for the Invoke method:
	//		ldarg.0
	//		ldfld methodDelegate
	//		<push parameters>
	//		call <new delegate type>.Invoke
	//		ret
	// Equivalent C# code:
	//		methodDelegate.DynamicInvoke(paramObjs)

	bOpcode = 0x02; // ldarg.0
	if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x7b; // ldfld methodDelegate
	if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd((BYTE*)&dwMethodDelegateFieldToken, 4))
		return false;

	SiArray<CLRTypeSignature> vtsParamTypes = pMethod->getSignature().getParamTypes();
	DWORD* pParamLocalIndex = new DWORD[vtsParamTypes.GetLength()];
	for (DWORD i=0; i<vtsParamTypes.GetLength(); i++)
	{
		bOpcode = 0x03; // ldarg.1
		if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
			return false;

		if (i <= 9)
		{
			bOpcode = (BYTE)(0x16 + i); // ldc.i4.<i>
			if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}
		else
		{
			bOpcode = 0x1f; // ldc.i4.s
			if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
			bOpcode = (BYTE)i;
			if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}

		bOpcode = 0x9a; // ldelem.ref
		if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
			return false;

		if (!CLRConvertToType(ppeModule, nmdMethodInfoInvokeMethod.mMethod, vtsParamTypes[i], pParamLocalIndex[i]))
			return false;
	}

	bOpcode = 0x6f; // call <new delegate type>.Invoke
	if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd((BYTE*)&dwDelegateInvokeToken, 4))
		return false;

	// Store any reference parameters back into Object array
	for (DWORD i=0; i<vtsParamTypes.GetLength(); i++)
	{
		if (vtsParamTypes[i].getTypeCode() == CLR_DATATYPE_BYREF)
		{
			bOpcode = 0x03; // ldarg.1
			if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
				return false;

			if (i <= 9)
			{
				bOpcode = (BYTE)(0x16 + i); // ldc.i4.<i>
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else
			{
				bOpcode = 0x1f; // ldc.i4.s
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
				bOpcode = (BYTE)i;
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}

			if (pParamLocalIndex[i] == 0)
			{
				bOpcode = 0x06; // ldloc.0
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else if (pParamLocalIndex[i] == 1)
			{
				bOpcode = 0x07; // ldloc.1
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else if (pParamLocalIndex[i] == 2)
			{
				bOpcode = 0x08; // ldloc.2
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else if (pParamLocalIndex[i] == 3)
			{
				bOpcode = 0x09; // ldloc.3
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else
			{
				bOpcode = 0x11; // ldloc.s
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
				bOpcode = (BYTE)pParamLocalIndex[i];
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
			}

			if ((vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_VALUETYPE) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_BOOLEAN) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_CHAR) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_SBYTE) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_BYTE) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_INT16) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_UINT16) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_INT32) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_UINT32) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_INT64) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_UINT64) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_SINGLE) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_DOUBLE) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_INTPTR) ||
				(vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_UINTPTR))
			{
				CLRTypeName tnType = CLRGetTypeName(*vtsParamTypes[i].getAssocType());
				DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
				if (dwTypeToken == 0)
				{
					// Type does not exist yet, create a TypeRef
					DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
					if (dwAssemblyRef == 0)
						return false;

					dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
					if (dwTypeToken == 0)
						return false;
				}

				bOpcode = 0x8c; // box
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;
			}

			bOpcode = 0xa2; // stelem.ref
			if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
				return false;
		}
	}

	delete[] pParamLocalIndex;

	if (pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_VOID)
	{
		bOpcode = 0x14; // ldnull
		if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else if ((pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_VALUETYPE) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_BOOLEAN) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_CHAR) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_SBYTE) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_BYTE) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_INT16) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_UINT16) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_INT32) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_UINT32) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_INT64) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_UINT64) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_SINGLE) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_DOUBLE) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_INTPTR) ||
		(pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_UINTPTR))
	{
		CLRTypeName tnType = CLRGetTypeName(pMethod->getSignature().getReturnType());
		DWORD dwTypeToken = ppeModule->findTypeToken(tnType);
		if (dwTypeToken == 0)
		{
			// Type does not exist yet, create a TypeRef
			DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
			if (dwAssemblyRef == 0)
				return false;

			dwTypeToken = ppeModule->createTypeRef(dwAssemblyRef, tnType);
			if (dwTypeToken == 0)
				return false;
		}

		bOpcode = 0x8c; // box
		if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
			return false;
		if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd((BYTE*)&dwTypeToken, 4))
			return false;
	}

	bOpcode = 0x2a; // ret
	if (!nmdMethodInfoInvokeMethod.mMethod.insertILAtEnd(&bOpcode, 1))
		return false;

	// Add Invoke method to the type
	ntdMethodInfoType.vmdMethods.Add(nmdMethodInfoInvokeMethod);

	// Add the methodDelegate field to the type
	CLRMemberDescription mdMethodInfoDelegateMember;
	mdMethodInfoDelegateMember.dwFlags = CLR_FIELD_ACCESS_PRIVATE;
	mdMethodInfoDelegateMember.sName = "memberDelegate";
	mdMethodInfoDelegateMember.sSignature.setCallConv(CLR_CALLCONV_FIELD);
	mdMethodInfoDelegateMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_CLASS, tnDelegateType));
	ntdMethodInfoType.vmdFields.Add(mdMethodInfoDelegateMember);

	// Add the new ConstructorInterceptInfo type to the executable
	DWORD dwMethodInfoTypeToken = ppeModule->createType(ntdMethodInfoType);
	if (dwMethodInfoTypeToken == 0)
	{
		// Type creation failed
		return false;
	}

	// Make the new type a nested type within the intercepted type
	if (!ppeModule->addNestedType(dwInterceptedTypeToken, dwMethodInfoTypeToken))
		return false;

	// Get a token for the constructor of the new delegate type
	nmdMethodInfoCtorMethod.mdMember.tnType = ntdMethodInfoType.tnTypeName;
	CLRMethod* pMethodInfoConstructorMethod = ppeModule->findMethod(nmdMethodInfoCtorMethod.mdMember, false);
	if (!pMethodInfoConstructorMethod)
	{
		// Couldn't find method
		return false;
	}
	// Construct a token using the returned method
	dwMethodInfoConstructorToken = CLR_TOKEN_METHOD | (pMethodInfoConstructorMethod->getMethodIndex() + 1);
	delete pMethodInfoConstructorMethod;

	return true;
}


//*************************************************************************
// Function:	 CLRInterceptMethod
// Description: Intercepts a method.
//				
// Parameters:
//	ppeModule - Module containing method to intercept
//  pMethod - CLRMethod object representing method to intercept
//  mdInterceptMethod - Name and signature of method to intercept
//  mdHandlerMethod - Name and signature of handler method
//  dwHeatDotNetRefToken - TypeRef token for HeatDotNet.dll
//
// Return Value: true if the method was successfully intercepted, false
//		otherwise
//*************************************************************************
bool CLRInterceptMethod(PEFile* ppeModule, CLRMethod* pMethod, CLRMemberDescription& mdInterceptMethod,
						CLRMemberDescription& mdHandlerMethod, DWORD dwHeatDotNetRefToken)
{
	// Construct tokens for the intercepted method.  RIDs are one-indexed, so add one to the row index
	DWORD dwInterceptedMethodToken = CLR_TOKEN_METHOD + (pMethod->getMethodIndex() + 1);
	DWORD dwInterceptedTypeToken = CLR_TOKEN_TYPEDEF + (pMethod->getTypeIndex() + 1);

	// Get the member token for HeatDotNet.MethodInterceptInfo constructor
	// void HeatDotNet.MethodInterceptInfo..ctor(Object thisObj)
	CLRMemberDescription mdMethodInterceptInfoMember;
	mdMethodInterceptInfoMember.tnType.sNamespace = "HeatDotNet";
	mdMethodInterceptInfoMember.tnType.sName = "MethodInterceptInfo";
	mdMethodInterceptInfoMember.sName = ".ctor";
	mdMethodInterceptInfoMember.sSignature.setCallConv(CLR_CALLCONV_INSTANCE);
	mdMethodInterceptInfoMember.sSignature.setReturnType(CLR_DATATYPE_VOID);
	mdMethodInterceptInfoMember.sSignature.insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_OBJECT));
	mdMethodInterceptInfoMember.dwAssemblyRefToken = dwHeatDotNetRefToken;
	DWORD dwMethodInterceptInfoToken = ppeModule->findMemberRefToken(mdMethodInterceptInfoMember, true);
	if (dwMethodInterceptInfoToken == 0)
		return false;

	// Get the method token for the handler method
	DWORD dwHandlerToken = ppeModule->findMemberRefToken(mdHandlerMethod, true);
	if (dwHandlerToken == 0)
	{
		// Reference creation failed
		delete ppeModule;
		return false;
	}

	// Get the member token for HeatDotNet.Interception.InInterceptionHandler
	CLRMemberDescription mdInInterceptMember;
	mdInInterceptMember.tnType.sNamespace = "HeatDotNet";
	mdInInterceptMember.tnType.sName = "Interception";
	mdInInterceptMember.sName = "InInterceptionHandler";
	mdInInterceptMember.sSignature.setCallConv(CLR_CALLCONV_FIELD);
	mdInInterceptMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_INT32));
	mdInInterceptMember.dwAssemblyRefToken = dwHeatDotNetRefToken;
	DWORD dwInInterceptToken = ppeModule->findMemberRefToken(mdInInterceptMember, true);
	if (dwInInterceptToken == 0)
		return false;

	// Get the member token for HeatDotNet.Interception.ForceOriginalCode
	CLRMemberDescription mdForceOriginalMember;
	mdForceOriginalMember.tnType.sNamespace = "HeatDotNet";
	mdForceOriginalMember.tnType.sName = "Interception";
	mdForceOriginalMember.sName = "ForceOriginalCode";
	mdForceOriginalMember.sSignature.setCallConv(CLR_CALLCONV_FIELD);
	mdForceOriginalMember.sSignature.setReturnType(CLRTypeSignature(CLR_DATATYPE_INT32));
	mdForceOriginalMember.dwAssemblyRefToken = dwHeatDotNetRefToken;
	DWORD dwForceOriginalToken = ppeModule->findMemberRefToken(mdForceOriginalMember, true);
	if (dwForceOriginalToken == 0)
		return false;

	// We need to create a constructor delegate that can contain a "function pointer" to the real constructor
	// method.  This will be passed through another new class derived from HeatDotNet.ConstructorInterceptInfo.
	// The ConstructorInterceptInfo class will have the "this" object as well as an abstract Invoke method that
	// takes an Object array with the parameters to pass to the original constructor method.  Our derived
	// ConstructorInterceptInfo class will implement the abstract Invoke method to call the original constructor
	// with the parameters specified in the Object array.
	char szMethodDelegateName[64];
	char szMethodInterceptInfoName[64];
	sprintf(szMethodDelegateName, "$$HeatMethodDelegate$%d", g_dwMethodDelegateID);
	sprintf(szMethodInterceptInfoName, "$$HeatMethodInterceptInfo$%d", g_dwMethodDelegateID);
	g_dwMethodDelegateID++;

	DWORD dwDelegateConstructorToken, dwDelegateInvokeToken;
	DWORD dwMethodInfoConstructorToken;

	if (!CLRInterceptCreateDelegateType(ppeModule, szMethodDelegateName, mdInterceptMethod,
		dwDelegateConstructorToken, dwDelegateInvokeToken, dwInterceptedTypeToken))
		return false;

	CLRTypeName tnDelegateType;
	tnDelegateType.sNamespace = "";
	tnDelegateType.sName = szMethodDelegateName;
	tnDelegateType.sOuterClass = ppeModule->getFullTypeName(dwInterceptedTypeToken);

	if (!CLRInterceptCreateMethodInterceptInfoType(ppeModule, szMethodInterceptInfoName, pMethod,
		tnDelegateType, dwMethodInterceptInfoToken, dwDelegateInvokeToken, dwMethodInfoConstructorToken,
		dwInterceptedTypeToken))
		return false;

	// Create the IL code within the constructor for the interception process.  The IL code created is as follows:
	//
	//		ldsfld HeatDotNet.Intercepts.ForceOriginalCode
	//		brtrue.s ExecOriginalCode
	//		ldsfld HeatDotNet.Intercepts.InInterceptionHandler
	//		brtrue.s <original method code>
	//		ldsfld HeatDotNet.Intercepts.InInterceptionHandler
	//		ldc.i4.1
	//		add
	//		stsfld HeatDotNet.Intercepts.InInterceptionHandler
	//		ldarg.0 (or ldnull for static)
	//		ldarg.0 (or ldnull for static)
	//		ldftn <intercepted method>
	//		newobj <new constructor delegate type>
	//		newobj <new ConstructorInterceptInfo type>
	//		<push method parameters>
	//		call <handler method>
	//		ldsfld HeatDotNet.Intercepts.InInterceptionHandler
	//		ldc.i4.1
	//		sub
	//		stsfld HeatDotNet.Intercepts.InInterceptionHandler
	//		ret
	//	ExecOriginalCode:
	//		ldc.i4.0
	//		stsfld HeatDotNet.Intercepts.ForceOriginalCode
	//
	// Equivalent C# code (if no return value, will return value from handler function if one exists):
	//		if (HeatDotNet.Interception.ForceOriginalCode == 0)
	//		{
	//			if (HeatDotNet.Interception.InInterceptionHandler == 0)
	//			{
	//				HeatDotNet.Interception.InInterceptionHandler++;
	//				<Handler method>(new <new ConstructorInterceptInfo type>(this, <new constructor delegate type>(
	//					<original method>)), <Constructor parameters>);
	//				HeatDotNet.Interception.InInterceptionHandler--;
	//				return;
	//			}
	//		}
	//		else
	//			HeatDotNet.Interception.ForceOriginalCode = 0;
	//
	// This code will call the handler method, with the same parameters as passed to the original
	// method (including the implied "this" parameter if it exists).  It will increment
	// HeatDotNet.Intercepts.InInterceptionHandler while processing the handler method, and
	// check the value of this member when entering the function.  If InInterceptionHandler has
	// already been incremented, control is passed directly to the original method rather than
	// the handler method.  This allows a handler method to call the original method in the
	// normal fashion without any unexpected side effects.
	CLRMethod mCallCode, mIfInInterceptCode, mIfForceOriginal, mClearOriginal;

	// Make sure the stack is big enough for us to do our work
	if (pMethod->getMaxStack() < pMethod->getParamCount())
		pMethod->setMaxStack(pMethod->getParamCount());

	if (pMethod->getMaxStack() < 3)
		pMethod->setMaxStack(3);

	BYTE bOpcode = 0x7e; // ldsfld HeatDotNet.Intercepts.InInterceptionHandler
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwInInterceptToken, 4))
		return false;

	bOpcode = 0x17; // ldc.i4.1
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x58; // add
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x80; // stsfld HeatDotNet.Intercepts.InInterceptionHandler
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwInInterceptToken, 4))
		return false;

	// Store private reference parameters into a temporary Object
	SiArray<CLRTypeSignature> vtsParamTypes = pMethod->getSignature().getParamTypes();
	DWORD* pParamLocalIndex = new DWORD[vtsParamTypes.GetLength()];
	for (DWORD i=0; i<vtsParamTypes.GetLength(); i++)
	{
		if ((vtsParamTypes[i].getTypeCode() == CLR_DATATYPE_BYREF) &&
			vtsParamTypes[i].isPrivate())
		{
			// Insert a local variable to store the current value
			pParamLocalIndex[i] = pMethod->getLocalVars().getParamCount();
			pMethod->getLocalVars().insertParamAtEnd(CLRTypeSignature(CLR_DATATYPE_OBJECT));

			bOpcode = 0x0e; // ldarg.s uint8
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			DWORD index = i + (pMethod->getSignature().getCallConv() & CLR_CALLCONV_INSTANCE) ? 1 : 0;
			if (!mCallCode.insertILAtEnd((BYTE*)&index, 1))
				return false;

			DWORD dwTypeToken = vtsParamTypes[i].getAssocType()->getTypeToken();
			if (vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_VALUETYPE)
			{
				bOpcode = 0x71; // ldobj
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;

				// Private structure, box it into an Object
				bOpcode = 0x8c; // box
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;
			}
			else
			{
				bOpcode = 0x50; // ldind.ref
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}

			bOpcode = 0x13; // stloc.s
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			bOpcode = (BYTE)pParamLocalIndex[i];
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
		}
	}

	if (pMethod->getSignature().getCallConv() & CLR_CALLCONV_INSTANCE)
	{
		bOpcode = 0x02; // ldarg.0
		if (!mCallCode.insertILAtEnd(&bOpcode, 1))
			return false;

		bOpcode = 0x02; // ldarg.0
		if (!mCallCode.insertILAtEnd(&bOpcode, 1))
			return false;
	}
	else
	{
		bOpcode = 0x14; // ldnull
		if (!mCallCode.insertILAtEnd(&bOpcode, 1))
			return false;

		bOpcode = 0x14; // ldnull
		if (!mCallCode.insertILAtEnd(&bOpcode, 1))
			return false;
	}

	bOpcode = 0xfe; // ldftn <intercepted method>
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	bOpcode = 0x06;
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwInterceptedMethodToken, 4))
		return false;

	bOpcode = 0x73; // newobj <new constructor delegate type>..ctor
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwDelegateConstructorToken, 4))
		return false;

	bOpcode = 0x73; // newobj <new ConstructorInterceptInfo type>..ctor
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwMethodInfoConstructorToken, 4))
		return false;

	// Push on the method parameters.  Don't push the "this" parameter for instance methods, as it is included in the
	// newly created ConstructorInterceptInfo object.
	DWORD sigParamIndex = 0;
	for (DWORD j=(pMethod->getSignature().getCallConv() & CLR_CALLCONV_INSTANCE) ? 1 : 0; j<pMethod->getParamCount(); j++, sigParamIndex++)
	{
		if ((vtsParamTypes[sigParamIndex].getTypeCode() == CLR_DATATYPE_BYREF) &&
			vtsParamTypes[sigParamIndex].isPrivate())
		{
			// Push a reference to the local on the stack
			bOpcode = 0x12; // ldloc.a
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			bOpcode = (BYTE)pParamLocalIndex[sigParamIndex];
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
		}
		else
		{
			if (j == 0)
			{
				bOpcode = 0x02; // ldarg.0
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else if (j == 1)
			{
				bOpcode = 0x03; // ldarg.1
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else if (j == 2)
			{
				bOpcode = 0x04; // ldarg.2
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else if (j == 3)
			{
				bOpcode = 0x05; // ldarg.3
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else
			{
				bOpcode = 0x0e; // ldarg.s uint8
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&j, 1))
					return false;
			}

			if (vtsParamTypes[sigParamIndex].isPrivate() &&
				(vtsParamTypes[sigParamIndex].getTypeCode() == CLR_DATATYPE_VALUETYPE))
			{
				// Private structure, box it into an Object
				bOpcode = 0x8c; // box
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				DWORD dwTypeToken = pMethod->getSignature().getParamTypes()[sigParamIndex].getTypeToken();
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;
			}
		}
	}

	bOpcode = 0x28; // call <handler method>
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwHandlerToken, 4))
		return false;

	if (pMethod->getSignature().getReturnType().isPrivate())
	{
		DWORD dwTypeToken = pMethod->getSignature().getReturnType().getTypeToken();
		if (pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_VALUETYPE)
		{
			bOpcode = 0x79; // unbox
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
				return false;

			bOpcode = 0x71; // ldobj
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
				return false;
		}
		else if (pMethod->getSignature().getReturnType().getTypeCode() == CLR_DATATYPE_CLASS)
		{
			bOpcode = 0x74; // castclass
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
				return false;
		}
	}

	// Store private reference parameters back into the actual parameter
	for (DWORD i=0; i<vtsParamTypes.GetLength(); i++)
	{
		if ((vtsParamTypes[i].getTypeCode() == CLR_DATATYPE_BYREF) &&
			vtsParamTypes[i].isPrivate())
		{
			bOpcode = 0x0e; // ldarg.s uint8
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			DWORD index = i + (pMethod->getSignature().getCallConv() & CLR_CALLCONV_INSTANCE) ? 1 : 0;
			if (!mCallCode.insertILAtEnd((BYTE*)&index, 1))
				return false;

			bOpcode = 0x11; // ldloc.s
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;
			bOpcode = (BYTE)pParamLocalIndex[i];
			if (!mCallCode.insertILAtEnd(&bOpcode, 1))
				return false;

			DWORD dwTypeToken = vtsParamTypes[i].getAssocType()->getTypeToken();
			if (vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_VALUETYPE)
			{
				bOpcode = 0x79; // unbox
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;

				bOpcode = 0x71; // ldobj
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;

				bOpcode = 0x81; // stobj
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;
			}
			else if (vtsParamTypes[i].getAssocType()->getTypeCode() == CLR_DATATYPE_CLASS)
			{
				bOpcode = 0x74; // castclass
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
				if (!mCallCode.insertILAtEnd((BYTE*)&dwTypeToken, 4))
					return false;

				bOpcode = 0x51; // stind.ref
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}
			else
			{
				bOpcode = 0x51; // stind.ref
				if (!mCallCode.insertILAtEnd(&bOpcode, 1))
					return false;
			}
		}
	}
	delete[] pParamLocalIndex;

	bOpcode = 0x7e; // ldsfld HeatDotNet.Intercepts.InInterceptionHandler
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwInInterceptToken, 4))
		return false;

	bOpcode = 0x17; // ldc.i4.1
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x59; // sub
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x80; // stsfld HeatDotNet.Intercepts.InInterceptionHandler
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mCallCode.insertILAtEnd((BYTE*)&dwInInterceptToken, 4))
		return false;

	bOpcode = 0x2a; // ret
	if (!mCallCode.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x16; // ldc.i4.0
	if (!mClearOriginal.insertILAtEnd(&bOpcode, 1))
		return false;

	bOpcode = 0x80; // stsfld HeatDotNet.Intercepts.ForceOriginalCode
	if (!mClearOriginal.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mClearOriginal.insertILAtEnd((BYTE*)&dwForceOriginalToken, 4))
		return false;

	bOpcode = 0x7e; // ldsfld HeatDotNet.Intercepts.InInterceptionHandler
	if (!mIfInInterceptCode.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mIfInInterceptCode.insertILAtEnd((BYTE*)&dwInInterceptToken, 4))
		return false;

	bOpcode = 0x2d; // brtrue.s
	if (!mIfInInterceptCode.insertILAtEnd(&bOpcode, 1))
		return false;
	bOpcode = (BYTE)mCallCode.getILCodeSize() + (BYTE)mClearOriginal.getILCodeSize(); // Skip over call code if already intercepting
	if (!mIfInInterceptCode.insertILAtEnd(&bOpcode, 1))
		return false;

	// Add the handler method call code just after the brtrue.s instruction
	if (!mIfInInterceptCode.insertILAtEnd(mCallCode.getILCode(), mCallCode.getILCodeSize()))
		return false;

	bOpcode = 0x7e; // ldsfld HeatDotNet.Intercepts.ForceOriginalCode
	if (!mIfForceOriginal.insertILAtEnd(&bOpcode, 1))
		return false;
	if (!mIfForceOriginal.insertILAtEnd((BYTE*)&dwForceOriginalToken, 4))
		return false;

	bOpcode = 0x2d; // brtrue.s
	if (!mIfForceOriginal.insertILAtEnd(&bOpcode, 1))
		return false;
	bOpcode = (BYTE)mIfInInterceptCode.getILCodeSize(); // Skip over call code if forcing original code
	if (!mIfForceOriginal.insertILAtEnd(&bOpcode, 1))
		return false;

	if (!mIfForceOriginal.insertILAtEnd(mIfInInterceptCode.getILCode(), mIfInInterceptCode.getILCodeSize()))
		return false;

	if (!mIfForceOriginal.insertILAtEnd(mClearOriginal.getILCode(), mClearOriginal.getILCodeSize()))
		return false;

	// Add the IL code to the beginning of the intercepted method
	if (!pMethod->insertILAtStart(mIfForceOriginal.getILCode(), mIfForceOriginal.getILCodeSize()))
		return false;

	// Replace the original method with the new modified method
	ppeModule->replaceMethod(pMethod);
	return true;
}


//*************************************************************************
// Function:	 CLRInterceptSecondaryModule
// Description: Intercepts all methods in a non-primary module of a
//				multi-module assembly that have a handler method set up
//				using the AddInterceptHandlerModule function.  This process
//				creates a new executable file containing the modified
//				methods.  This new module should be loaded instead of the
//				original for interception to take place.
//				
// Parameters:
//	sModuleName - Path to the module being intercepted
//  pPrimaryModule - Pointer to primary module
//
// Return value: Path to the new, modified module with interception in
//		place.  Empty string if there is no interception.
//*************************************************************************
SiString CLRInterceptSecondaryModule(const SiString& sModuleName, PEFile* pPrimaryModule)
{
	wchar_t wszFullPath[MAX_PATH];
	wchar_t wszNewDLLPath[MAX_PATH];
	wchar_t *wszFileName;

	if ((!g_ppeHeatDotNetModule) || (!g_ppeDotNetReplacementLibraryModule))
	{
		// Haven't initialized yet
		CLRSkipModuleAndUpdateStatus();
		return "";
	}

	// Find the full path name of the module
	if (!SearchPathW(NULL, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, &wszFileName))
	{
		CLRSkipModuleAndUpdateStatus();
		return "";
	}

	// See if it is a module that we have already intercepted.
	for (DWORD i=0; i<g_vimInterceptedModules.GetLength(); i++)
	{
		if (g_vimInterceptedModules[i].sModuleFullPath == wszFullPath)
		{
			// This module has already been  intercepted.  Return the replacement path.
			CLRSkipModuleAndUpdateStatus();
			return g_vimInterceptedModules[i].sModuleReplacementPath;
		}
	}

	// Load the module information
	PEFile* ppeModule = new PEFile();
	if (!ppeModule)
	{
		CLRSkipModuleAndUpdateStatus();
		return "";
	}

	char szFullPath[MAX_PATH];
	sprintf(szFullPath, "%S", wszFullPath);
	if (!ppeModule->load(szFullPath, true))
	{
		CLRSkipModuleAndUpdateStatus();
		delete ppeModule;
		return "";
	}

	// Compute the checksum for the original module
	DWORD dwOrigChecksum = ppeModule->getChecksum();

	if (!ppeModule->isManagedExecutable())
	{
		// Native executable.  Won't have any .NET methods.
		CLRSkipModuleAndUpdateStatus();
		delete ppeModule;
		return "";
	}

	// Add a module reference to the primary module
	DWORD dwPrimaryModuleRefToken = ppeModule->createAssemblyRef(*pPrimaryModule);
	if (dwPrimaryModuleRefToken == 0)
	{
		// Reference creation failed
		CLRSkipModuleAndUpdateStatus();
		delete ppeModule;
		return "";
	}

	// Create a TypeRef for System.Array if one doesn't already exist
	CLRTypeName tnArray;
	tnArray.sNamespace = "System";
	tnArray.sName = "Array";
	if (ppeModule->findTypeToken(tnArray) == 0)
	{
		// Type does not exist yet, create a TypeRef
		DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
		if (dwAssemblyRef == 0)
		{
			// No mscorlib reference
			CLRSkipModuleAndUpdateStatus();
			delete ppeModule;
			return "";
		}

		if (ppeModule->createTypeRef(dwAssemblyRef, tnArray) == 0)
		{
			// TypeRef creation failed
			CLRSkipModuleAndUpdateStatus();
			delete ppeModule;
			return "";
		}
	}

	// Keep a flag of whether or not we have actually intercepted a function in this module
	bool fInterceptedMethods = false;

	SiArray<PEFile*> mergeList;

	// Intercept the methods marked for interception
	for (DWORD i=0; i<g_vidIntercepts.GetLength(); i++, g_dwInterceptsProcessed++)
	{
		// Try to find the method in this module
		CLRMethod* pMethod = ppeModule->findMethod(g_vidIntercepts[i].mdInterceptMethod, true);
		if (!pMethod)
		{
			// Try the instance method form
			pMethod = ppeModule->findMethod(g_vidIntercepts[i].mdInstanceInterceptMethod, true);
			if (!pMethod)
			{
				// Method not in the module
				continue;
			}
		}

		CLRMemberDescription mdHandlerMethod = g_vidIntercepts[i].mdHandlerMethod;

		mdHandlerMethod.dwAssemblyRefToken = dwPrimaryModuleRefToken;

//		printf("Modifying %s.%s.%s\n", (char*)g_vidIntercepts[i].mdInterceptMethod.tnType.sNamespace,
//			(char*)g_vidIntercepts[i].mdInterceptMethod.tnType.sName, (char*)g_vidIntercepts[i].mdInterceptMethod.sName);

		if (CLRInterceptMethod(ppeModule, pMethod, g_vidIntercepts[i].mdInterceptMethod, mdHandlerMethod, dwPrimaryModuleRefToken))
			fInterceptedMethods = true;

		delete pMethod;
	}

	// Save the new modified metadata
	if (!ppeModule->saveMetadata())
	{
		// Metadata save failed.
		delete ppeModule;
		return "";
	}

	Registry reg;
	SiString sInstalledPath;
	SiString sCacheFolder;

	// Get the path of the cache folder
	if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
		return "";
	if (!reg.Read(INSTALL_PATH_VALUE_NAME, sInstalledPath))
		return "";
	if (!reg.Read(DOT_NET_CACHE_PATH_VALUE, sCacheFolder))
		return "";

	sInstalledPath += "\\";
	sInstalledPath += sCacheFolder;

	// Make sure the folder exists
	SHCreateDirectoryExW(NULL, (wchar_t*)sInstalledPath, NULL);

	// Get the path to the cached intercepted module
	swprintf(wszNewDLLPath, L"%s\\%s.%X.%X.netmodule", (wchar_t*)sInstalledPath, wszFileName, dwOrigChecksum,
		g_dwReplacementLibraryChecksum);

	// We have our modified module path.  Save the module.
	char szNewDLLPath[MAX_PATH];
	sprintf(szNewDLLPath, "%S", wszNewDLLPath);
	if (!ppeModule->save(szNewDLLPath))
	{
		// Module save failed
		delete ppeModule;
		return "";
	}

	// Save information about the module
	CLRInterceptedModule imInterceptedModule;
	imInterceptedModule.sModuleFileName = wszFileName;
	imInterceptedModule.sModuleFullPath = wszFullPath;
	imInterceptedModule.sModuleReplacementPath = wszNewDLLPath;
	g_vimInterceptedModules.Add(imInterceptedModule);

//	printf("Modified binary at %S", wszNewDLLPath);
	return wszNewDLLPath;
}


//*************************************************************************
// Function:	 CLRInterceptModule
// Description: Intercepts all methods in a module that have a handler
//				method set up using the AddInterceptHandlerModule
//				function.  This process creates a new executable file
//				containing the modified methods.  This new module should
//				be loaded instead of the original for interception to
//				take place.
//				
// Parameters:
//	sModuleName - Path to the module being intercepted
//
// Return value: Path to the new, modified module with interception in
//		place.  Empty string if there is no interception.
//*************************************************************************
SiString CLRInterceptModule(const SiString& sModuleName)
{
	wchar_t wszFullPath[MAX_PATH];
	wchar_t wszNewDLLPath[MAX_PATH];
	wchar_t *wszFileName;

	if ((!g_ppeHeatDotNetModule) || (!g_ppeDotNetReplacementLibraryModule))
	{
		// Haven't initialized yet
		CLRSkipModuleAndUpdateStatus();
		return "";
	}

	// Find the full path name of the module
	if (!SearchPathW(NULL, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, &wszFileName))
	{
		CLRSkipModuleAndUpdateStatus();
		return "";
	}

	// See if it is a module that we have already intercepted.
	for (DWORD i=0; i<g_vimInterceptedModules.GetLength(); i++)
	{
		if (g_vimInterceptedModules[i].sModuleFullPath == wszFullPath)
		{
			// This module has already been  intercepted.  Return the replacement path.
			CLRSkipModuleAndUpdateStatus();
			return g_vimInterceptedModules[i].sModuleReplacementPath;
		}
	}

	// Load the module information
	PEFile* ppeModule = new PEFile();
	if (!ppeModule)
	{
		CLRSkipModuleAndUpdateStatus();
		return "";
	}

	char szFullPath[MAX_PATH];
	sprintf(szFullPath, "%S", wszFullPath);
	if (!ppeModule->load(szFullPath, true))
	{
		CLRSkipModuleAndUpdateStatus();
		delete ppeModule;
		return "";
	}

	// Compute the checksum for the original module
	DWORD dwOrigChecksum = ppeModule->getChecksum();

	if (!ppeModule->isManagedExecutable())
	{
		// Native executable.  Won't have any .NET methods.
		CLRSkipModuleAndUpdateStatus();
		delete ppeModule;
		return "";
	}

	// Add an assembly reference to HeatDotNet.dll
	DWORD dwHeatDotNetRefToken = ppeModule->createAssemblyRef(*g_ppeHeatDotNetModule);
	if (dwHeatDotNetRefToken == 0)
	{
		// Reference creation failed
		CLRSkipModuleAndUpdateStatus();
		delete ppeModule;
		return "";
	}

	// Create a TypeRef for System.Array if one doesn't already exist
	CLRTypeName tnArray;
	tnArray.sNamespace = "System";
	tnArray.sName = "Array";
	if (ppeModule->findTypeToken(tnArray) == 0)
	{
		// Type does not exist yet, create a TypeRef
		DWORD dwAssemblyRef = ppeModule->findAssemblyRef("mscorlib");
		if (dwAssemblyRef == 0)
		{
			// No mscorlib reference
			CLRSkipModuleAndUpdateStatus();
			delete ppeModule;
			return "";
		}

		if (ppeModule->createTypeRef(dwAssemblyRef, tnArray) == 0)
		{
			// TypeRef creation failed
			CLRSkipModuleAndUpdateStatus();
			delete ppeModule;
			return "";
		}
	}

	// Keep an array of AssemblyRef tokens for all of the handler modules.  We will fill in this
	// array as we add AssemblyRefs to the current module.
	DWORD* pdwHandlerAssemblyRefTokens = new DWORD[g_vpeHandlerModules.GetLength()];
	memset(pdwHandlerAssemblyRefTokens, 0, sizeof(DWORD) * g_vpeHandlerModules.GetLength());

	// Keep a flag of whether or not we have actually intercepted a function in this module
	bool fInterceptedMethods = false;

	SiArray<PEFile*> mergeList;

	// Intercept the methods marked for interception
	for (DWORD i=0; i<g_vidIntercepts.GetLength(); i++, g_dwInterceptsProcessed++)
	{
		// Try to find the method in this module
		CLRMethod* pMethod = ppeModule->findMethod(g_vidIntercepts[i].mdInterceptMethod, true);
		if (!pMethod)
		{
			// Try the instance method form
			pMethod = ppeModule->findMethod(g_vidIntercepts[i].mdInstanceInterceptMethod, true);
			if (!pMethod)
			{
				// Method not in the module
				continue;
			}
		}

		CLRMemberDescription mdHandlerMethod = g_vidIntercepts[i].mdHandlerMethod;

		// Try and find the AssemblyRef token for the handler module, which will be present if the
		// handler module has been used already.
		for (DWORD j=0; j<g_vpeHandlerModules.GetLength(); j++)
		{
			if (g_vpeHandlerModules[j] == g_vidIntercepts[i].ppeHandlerModule)
			{
				if (pdwHandlerAssemblyRefTokens[j] != 0)
				{
					// Module already referenced
					mdHandlerMethod.dwAssemblyRefToken = pdwHandlerAssemblyRefTokens[j];
				}
				else
				{
					// Module not referenced.  Add the reference and get the metadata token.
					DWORD dwAssemblyRefToken = ppeModule->createAssemblyRef(*g_vidIntercepts[i].ppeHandlerModule);
					if (dwAssemblyRefToken == 0)
					{
						// Reference creation failed
						delete ppeModule;
						return "";
					}

					mergeList.Add(g_vidIntercepts[i].ppeHandlerModule);

					// Set the reference token for the current method
					mdHandlerMethod.dwAssemblyRefToken = dwAssemblyRefToken;

					// Add the reference token to the AssemblyRef token array
					pdwHandlerAssemblyRefTokens[j] = dwAssemblyRefToken;
				}
				break;
			}
		}

//		printf("Modifying %s.%s.%s\n", (char*)g_vidIntercepts[i].mdInterceptMethod.tnType.sNamespace,
//			(char*)g_vidIntercepts[i].mdInterceptMethod.tnType.sName, (char*)g_vidIntercepts[i].mdInterceptMethod.sName);

		if (CLRInterceptMethod(ppeModule, pMethod, g_vidIntercepts[i].mdInterceptMethod, mdHandlerMethod, dwHeatDotNetRefToken))
			fInterceptedMethods = true;

		delete pMethod;
	}

	mergeList.Add(g_ppeDotNetReplacementLibraryModule);
	mergeList.Add(g_ppeHeatDotNetModule);

	for (DWORD i=0; i<mergeList.GetLength(); i++)
	{
		if (!ppeModule->merge(*mergeList[i]))
		{
			// Merge failed
			delete ppeModule;
			return "";
		}
	}

	// Save the new modified metadata
	if (!ppeModule->saveMetadata())
	{
		// Metadata save failed.
		delete ppeModule;
		return "";
	}

	Registry reg;
	SiString sInstalledPath;
	SiString sCacheFolder;

	// Get the path of the cache folder
	if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
		return "";
	if (!reg.Read(INSTALL_PATH_VALUE_NAME, sInstalledPath))
		return "";
	if (!reg.Read(DOT_NET_CACHE_PATH_VALUE, sCacheFolder))
		return "";

	sInstalledPath += "\\";
	sInstalledPath += sCacheFolder;

	// Make sure the folder exists
	SHCreateDirectoryExW(NULL, (wchar_t*)sInstalledPath, NULL);

	// Get the path to the cached intercepted module
	swprintf(wszNewDLLPath, L"%s\\%s.%X.%X.dll", (wchar_t*)sInstalledPath, wszFileName, dwOrigChecksum,
		g_dwReplacementLibraryChecksum);

	// We have our modified module path.  Save the module.
	char szNewDLLPath[MAX_PATH];
	sprintf(szNewDLLPath, "%S", wszNewDLLPath);
	if (!ppeModule->save(szNewDLLPath))
	{
		// Module save failed
		delete ppeModule;
		return "";
	}

	// Save information about the module
	CLRInterceptedModule imInterceptedModule;
	imInterceptedModule.sModuleFileName = wszFileName;
	imInterceptedModule.sModuleFullPath = wszFullPath;
	imInterceptedModule.sModuleReplacementPath = wszNewDLLPath;
	g_vimInterceptedModules.Add(imInterceptedModule);

	// Intercept each module in a multi-module assembly
	for (DWORD i=0; i<ppeModule->getMetadata().getTable(CLR_TABLE_FILE).getRowCount(); i++)
	{
		if (ppeModule->getMetadata().getTable(CLR_TABLE_FILE)[i][CLR_FILE_FLAGS_COLINDEX] == 0) // Contains metadata?
		{
			// Overwrite file name part of the path with the module name
			wcscpy(wszFileName, (wchar_t*)ppeModule->getMetadata().getTable(CLR_TABLE_FILE)[i].getString(CLR_FILE_NAME_COLINDEX));
			// Intercept the module
			CLRInterceptSecondaryModule(wszFullPath, ppeModule);
		}
	}

//	printf("Modified binary at %S", wszNewDLLPath);
	return wszNewDLLPath;
}


//*************************************************************************
// Function:	 CLRIsModuleManaged
// Description: Intercepts all methods in a module that have a handler
//				method set up using the AddInterceptHandlerModule
//				function.  This process creates a new executable file
//				containing the modified methods.  This new module should
//				be loaded instead of the original for interception to
//				take place.
//				
// Parameters:
//	sModuleName - Path to the module being intercepted
//
// Return value: Path to the new, modified module with interception in
//		place.  Empty string if there is no interception.
//*************************************************************************
bool CLRIsModuleManaged(const SiString& sModuleName)
{
	wchar_t wszFullPath[MAX_PATH];

	// Find the full path name of the module
	if (!SearchPathW(NULL, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, NULL))
		return false;

	// Load the module information
	PEFile* ppeModule = new PEFile();
	if (!ppeModule)
		return false;

	char szFullPath[MAX_PATH];
	sprintf(szFullPath, "%S", wszFullPath);
	if (!ppeModule->load(szFullPath, false))
	{
		delete ppeModule;
		return false;
	}

	if (!ppeModule->isManagedExecutable())
	{
		// Native executable.  Won't have any .NET methods.
		delete ppeModule;
		return false;
	}

	delete ppeModule;
	return true;
}


//*************************************************************************
// Function:	 CLRCheckInterceptedCache
// Description: Checks the instrumented binary cache to see if this module
//				has pre-generated intercepts.  This cached module should
//				be loaded instead of the original for interception to
//				take place.
//				
// Parameters:
//	sModuleName - Path to the module being intercepted
//
// Return value: Path to the cached module with interception in
//		place.  Empty string if there is no interception.
//*************************************************************************
SiUtils::SiString CLRCheckInterceptedCache(const SiUtils::SiString& sModuleName)
{
	Registry reg;
	SiString sInstalledPath;
	SiString sCacheFolder;

	// Find the full path name of the module
	wchar_t wszFullPath[MAX_PATH];
	wchar_t *wszFileName;
	if (!SearchPathW(NULL, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, &wszFileName))
		return "";

	// See if it is a module that we have already intercepted.
	for (DWORD i=0; i<g_vimInterceptedModules.GetLength(); i++)
	{
		if (g_vimInterceptedModules[i].sModuleFullPath == wszFullPath)
		{
			// This module has already been  intercepted.  Return the replacement path.
			CLRSkipModuleAndUpdateStatus();
			return g_vimInterceptedModules[i].sModuleReplacementPath;
		}
	}

	// Load the module information
	PEFile* ppeModule = new PEFile();
	if (!ppeModule)
		return "";

	char szFullPath[MAX_PATH];
	sprintf(szFullPath, "%S", wszFullPath);
	if (!ppeModule->load(szFullPath, false))
	{
		delete ppeModule;
		return "";
	}
	
	// Get the checksum of the module
	DWORD dwModuleChecksum = ppeModule->getChecksum();
	delete ppeModule;

	// Get the path of the cache folder
	if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
		return "";
	if (!reg.Read(INSTALL_PATH_VALUE_NAME, sInstalledPath))
		return "";
	if (!reg.Read(DOT_NET_CACHE_PATH_VALUE, sCacheFolder))
		return "";

	sInstalledPath += "\\";
	sInstalledPath += sCacheFolder;

	// Get the path to the cached intercepted module
	wchar_t wszNewDLLPath[MAX_PATH];
	swprintf(wszNewDLLPath, L"%s\\%s.%X.%X.dll", (wchar_t*)sInstalledPath, wszFileName, dwModuleChecksum,
		g_dwReplacementLibraryChecksum);

	if (PathFileExistsW(wszNewDLLPath))
	{
		// Module has a cached intercepted module.  Save information about the module.
		CLRInterceptedModule imInterceptedModule;
		imInterceptedModule.sModuleFileName = wszFileName;
		imInterceptedModule.sModuleFullPath = wszFullPath;
		imInterceptedModule.sModuleReplacementPath = wszNewDLLPath;
		g_vimInterceptedModules.Add(imInterceptedModule);
		return wszNewDLLPath;
	}

	// Try looking for a ".netmodule"
	swprintf(wszNewDLLPath, L"%s\\%s.%X.%X.netmodule", (wchar_t*)sInstalledPath, wszFileName, dwModuleChecksum,
		g_dwReplacementLibraryChecksum);

	if (PathFileExistsW(wszNewDLLPath))
	{
		// Module has a cached intercepted module.  Save information about the module.
		CLRInterceptedModule imInterceptedModule;
		imInterceptedModule.sModuleFileName = wszFileName;
		imInterceptedModule.sModuleFullPath = wszFullPath;
		imInterceptedModule.sModuleReplacementPath = wszNewDLLPath;
		g_vimInterceptedModules.Add(imInterceptedModule);
		return wszNewDLLPath;
	}

	// Module not cached
	return "";
}


//*************************************************************************
// Function:	 CLRCheckInterceptedCacheForAllModules
// Description: Checks the instrumented binary cache to see if this module
//				has pre-generated intercepts, and checks for consistency
//				of any secondary modules in a multi-module assembly.
//				
// Parameters:
//	sModuleName - Path to the module being intercepted
//
// Return value: Path to the cached module with interception in
//		place.  Empty string if there is no interception or secondary
//		modules are stale.
//*************************************************************************
SiString CLRCheckInterceptedCacheForAllModules(const SiString& sModuleName)
{
	SiString sRetVal = CLRCheckInterceptedCache(sModuleName);
	if (sRetVal.GetLength() == 0)
	{
		// Blank string returned, assembly not yet intercepted
		return sRetVal;
	}

	// Module exists, set up check for the secondary modules

	// Find the full path name of the module
	wchar_t wszFullPath[MAX_PATH];
	wchar_t *wszFileName;
	if (!SearchPathW(NULL, (wchar_t*)sModuleName, NULL, MAX_PATH, wszFullPath, &wszFileName))
		return "";

	// Load the module information
	PEFile* ppeModule = new PEFile();
	if (!ppeModule)
		return "";

	char szFullPath[MAX_PATH];
	sprintf(szFullPath, "%S", wszFullPath);
	if (!ppeModule->load(szFullPath, true))
	{
		delete ppeModule;
		return "";
	}

	// Enumerate secondary modules and check the intercept cache for each
	SiString sMainModuleFullPath = wszFullPath;
	for (DWORD i=0; i<ppeModule->getMetadata().getTable(CLR_TABLE_FILE).getRowCount(); i++)
	{
		if (ppeModule->getMetadata().getTable(CLR_TABLE_FILE)[i][CLR_FILE_FLAGS_COLINDEX] == 0) // Contains metadata?
		{
			// Overwrite file name part of the path with the module name
			wcscpy(wszFileName, (wchar_t*)ppeModule->getMetadata().getTable(CLR_TABLE_FILE)[i].getString(CLR_FILE_NAME_COLINDEX));
			// Check the module
			if (CLRCheckInterceptedCache(wszFullPath).GetLength() == 0)
			{
				// Blank string returned, so secondary module is not intercepted or stale.  Remove main assembly
				// from intercept list to force regeneration.
				for (DWORD j=0; j<g_vimInterceptedModules.GetLength(); j++)
				{
					if (g_vimInterceptedModules[i].sModuleFullPath == (wchar_t*)sMainModuleFullPath)
					{
						g_vimInterceptedModules.Remove(i);
						break;
					}
				}

				// Return empty string as the assembly is no longer has a valid cache entry.
				delete ppeModule;
				return "";
			}
		}
	}

	delete ppeModule;
	return sRetVal;
}


//*************************************************************************
// Function:	 CLRGetInterceptCount
// Description: Gets the number of intercepted functions
//				
// Parameters:
//	None
//
// Return value: Number of intercepted functions
//*************************************************************************
DWORD CLRGetInterceptCount()
{
	return g_vidIntercepts.GetLength();
}


//*************************************************************************
// Function:	 CLRGetInterceptModuleStatus
// Description: Gets the total number of intercepted functions processed
//				so far.
//				
// Parameters:
//	None
//
// Return value: Number of intercepted functions processed
//*************************************************************************
DWORD CLRGetInterceptModuleStatus()
{
	return g_dwInterceptsProcessed;
}


//*************************************************************************
// Function:	 CLRSkipModuleAndUpdateStatus
// Description: Updates the number of intercepted functions processed
//				when skipping the processing of a module.
//				
// Parameters:
//	None
//
// Return value: None
//*************************************************************************
void CLRSkipModuleAndUpdateStatus()
{
	g_dwInterceptsProcessed += CLRGetInterceptCount();
}
