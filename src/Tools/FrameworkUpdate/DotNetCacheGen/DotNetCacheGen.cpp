//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DotNetCacheGen.cpp
//
// DESCRIPTION: File containing the entry point function (WinMain) for the
//	.NET cache generator.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 May 2003		 R. Wagner	 File created.
//*************************************************************************

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

#include <windows.h>
#include <vcclr.h>
#include "SiString.h"
#include "Registry.h"
#include "CLRIntercept.h"
#include "DotNetCacheStatusForm.h"
#include "defines.h"
#include "StringConverter.h"
#include "PEFile.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections;
using namespace DotNetCacheGen;
using namespace SiUtils;


// This is the entry point for this application
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Holodeck::StringConverter *converter = Holodeck::StringConverter::GetInstance();

	//Check if any command line arguments have been specified.
	//If so then we must read the replacement names from the specified file
	bool IsCommandLine = false;
	String * cmdLineLibrary = (new String(lpCmdLine))->Replace("\"", "")->Trim();

	// If command line starts has the "/onfail" switch, write to the specific file if the
	// operation fails.  This is for InstallShield, since it is incapable of detecting the
	// process exit code.
	String* fileToWriteOnFailure = NULL;
	if (cmdLineLibrary->StartsWith("/onfail"))
	{
		fileToWriteOnFailure = cmdLineLibrary->Substring(strlen("/onfail") + 1);
		// No custom replacement names
		cmdLineLibrary = "";
	}
	
	if (System::IO::File::Exists (cmdLineLibrary))
		IsCommandLine = true;
	else
		IsCommandLine = false;

	// Check to make sure the .NET cache path is set
	Registry reg;
	if (!reg.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
	{
		::MessageBox(NULL, "Could not access Holodeck registry settings.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
		if (fileToWriteOnFailure != NULL)
			File::OpenWrite(fileToWriteOnFailure)->Close();
		return 0;
	}
	SiString cacheFolder;
	if (!reg.Read(DOT_NET_CACHE_PATH_VALUE, cacheFolder))
	{
		// Path not set, give it a default value
		reg.Write(DOT_NET_CACHE_PATH_VALUE, "DotNetCache");
	}

	//Initialize Intercepts
	if (!CLRInitializeIntercepts())
	{
		::MessageBox(NULL, "Could not access .NET replacement libraries.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
		if (fileToWriteOnFailure != NULL)
			File::OpenWrite(fileToWriteOnFailure)->Close();
		return 0;
	}

	//Cache check for user generated dotnet replacement libraries
	Registry registry;
	SiString sReplacementKey = ROOT_PATH;
	sReplacementKey += "\\";
	sReplacementKey += DOTNET_REPLACEMENT_DLLS_KEY;

	SiArray<SiString> interceptedDLLs;
	Hashtable * userReplacementLibArray = new Hashtable();
	//Open the "HolodeckEE" Key
	if (registry.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
	{
		interceptedDLLs = registry.GetValueNames (registry.GetCurrentKey(), DOTNET_REPLACEMENT_DLLS_KEY);
		//Open the "DotNetReplacementDlls" key
		registry.OpenKey (INSTALLED_ROOT_KEY, sReplacementKey);
		for (unsigned int j=0; j<interceptedDLLs.GetLength(); j++)
		{
			SiString iDLL = interceptedDLLs[j];
			SiString oDLL;
			registry.Read (iDLL, oDLL);
			String * interceptedDll = (char *) iDLL;
			String * originalDll = (char *) oDLL;
			if (CLRAddInterceptHandlerModuleForCacheCheck((char *) iDLL))
				userReplacementLibArray->Add (interceptedDll, originalDll);	
			else
			{
				interceptedDLLs.Remove(j);
				j--;
			}
		}
	}

	if (!IsCommandLine)
	{
		//Cache check for Holodeck dotnet replacement libraries
		if (!CLRAddInterceptHandlerModuleForCacheCheck("MSCorLibReplacementLibrary35.dll"))
		{
			::MessageBox(NULL, "Could not access .NET replacement libraries.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
			if (fileToWriteOnFailure != NULL)
				File::OpenWrite(fileToWriteOnFailure)->Close();
			return 0;
		}

		if (!CLRAddInterceptHandlerModuleForCacheCheck("SystemReplacementLibrary35.dll"))
		{
			::MessageBox(NULL, "Could not access .NET replacement libraries.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
			if (fileToWriteOnFailure != NULL)
				File::OpenWrite(fileToWriteOnFailure)->Close();
			return 0;
		}

		if (!CLRAddInterceptHandlerModuleForCacheCheck("SystemServiceProcessReplacementLibrary35.dll"))
		{
			::MessageBox(NULL, "Could not access .NET replacement libraries.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
			if (fileToWriteOnFailure != NULL)
				File::OpenWrite(fileToWriteOnFailure)->Close();
			return 0;
		}

		if (!CLRAddInterceptHandlerModuleForCacheCheck("SystemXMLReplacementLibrary35.dll"))
		{
			::MessageBox(NULL, "Could not access .NET replacement libraries.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
			if (fileToWriteOnFailure != NULL)
				File::OpenWrite(fileToWriteOnFailure)->Close();
			return 0;
		}
	}
	else
	{
		//Call CLRAddInterceptHandlerModuleForCacheCheck on the lib passed thru the command line
		SiString wszName;
		converter->ConvertToSiString(cmdLineLibrary, &wszName);
		if (!CLRAddInterceptHandlerModuleForCacheCheck(wszName))
		{
			::MessageBox(NULL, "Could not access .NET replacement libraries.  Please reinstall Holodeck.", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
			if (fileToWriteOnFailure != NULL)
				File::OpenWrite(fileToWriteOnFailure)->Close();
			return 0;
		}
	}

	wchar_t windowsDir[MAX_PATH];
	GetWindowsDirectoryW(windowsDir, MAX_PATH);
	String* frameworkDir = Path::Combine(new String(windowsDir), S"Microsoft.NET\\Framework");
	String* frameworkVersions __gc[] = Directory::GetDirectories(frameworkDir);

	ArrayList* genList = new ArrayList();
	SiString wszModuleName;
	String* moduleName;
	SiString interceptedModule;

	// Look for the intercepted modules for all installed versions of the .NET runtime
	for (int i=0; i<frameworkVersions->Count; i++)
	{
		moduleName = Path::Combine(frameworkVersions[i], "mscorlib.dll");
		if (File::Exists(moduleName))
		{
			converter->ConvertToSiString(moduleName, &wszModuleName);
			interceptedModule = CLRCheckInterceptedCache(wszModuleName);
			if (interceptedModule == "")
			{
				// Module not cached yet
				genList->Add(moduleName);
			}
		}

		moduleName = Path::Combine(frameworkVersions[i], "System.dll");
		if (File::Exists(moduleName))
		{
			converter->ConvertToSiString(moduleName, &wszModuleName);
			interceptedModule = CLRCheckInterceptedCache(wszModuleName);
			if (interceptedModule == "")
			{
				// Module not cached yet
				genList->Add(moduleName);
			}
		}

		moduleName = Path::Combine(frameworkVersions[i], "System.ServiceProcess.dll");
		if (File::Exists(moduleName))
		{
			converter->ConvertToSiString(moduleName, &wszModuleName);
			interceptedModule = CLRCheckInterceptedCache(wszModuleName);
			if (interceptedModule == "")
			{
				// Module not cached yet
				genList->Add(moduleName);
			}
		}

		moduleName = Path::Combine(frameworkVersions[i], "System.XML.dll");
		if (File::Exists(moduleName))
		{
			converter->ConvertToSiString(moduleName, &wszModuleName);
			interceptedModule = CLRCheckInterceptedCache(wszModuleName);
			if (interceptedModule == "")
			{
				// Module not cached yet
				genList->Add(moduleName);
			}
		}
	}

	for (int j=0; j< userReplacementLibArray->Count; j++)
	{
		String * keyName = (char *) interceptedDLLs[j];
		String * originalModule =  dynamic_cast <String *> (userReplacementLibArray->Item[keyName]);
		if (File::Exists(originalModule))
		{
			converter->ConvertToSiString(originalModule, &wszModuleName);
			interceptedModule = CLRCheckInterceptedCacheForAllModules(wszModuleName);
			if (interceptedModule == "")
			{
				// Module not cached yet
				genList->Add(originalModule);
			}
		}
	}

	CLRDeinitializeIntercepts();

	if (genList->Count == 0)
		return 0;

//	DotNetCacheStatusForm* form = new DotNetCacheStatusForm(genList, IsCommandLine, cmdLineLibrary);
//	form->ShowDialog();
	Application::Run(new DotNetCacheStatusForm(genList, IsCommandLine, cmdLineLibrary));
	return 0;
}
