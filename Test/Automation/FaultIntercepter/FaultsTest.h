///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		FaultsTest.h
///
/// DESCRIPTION: Contains definitions for the class FaultsTest.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 01/11/2003     Gavin Menezes   File Created.
///*************************************************************************

#pragma once
#include <windows.h>
using namespace System;
//using namespace System::String;
using namespace System::IO;
 

__gc public class FaultsTest
{
	private: 
		FaultXMLNavigator * faultNavigatorInstance;
		FunctionXMLNavigator * functionNavigatorInstance;

	public:
		FaultsTest(void);
		~FaultsTest(void);

		bool TestFault(char * faultName);
		void LoadFunctionsXmlFile(char *functionsXMLFilePath);
		void LoadFaultsXmlFile(char *faultsXMLFilePath);

	    char *GetStringNotContains(char *incomingString);
		char *GetStringContains(char *incomingString);
		
		char *GetBinaryContains(char *incomingBinary);
		char *GetBinaryNotContains(char *incomingBinary);

		System::Collections::ArrayList *FillParamArray(char *functionName, System::Collections::ArrayList *matchParamArray);
		bool CallFunction(String *funcName, System::Collections::ArrayList *ParamArray, DWORD returnvalue, DWORD errorcode);
};
