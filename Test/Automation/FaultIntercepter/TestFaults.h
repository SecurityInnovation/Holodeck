///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		TestFaults.h
///
/// DESCRIPTION: Contains definitions for the class TestFaults.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 03/10/2003     Gavin Menezes	File Created.
///*************************************************************************
#pragma once
#include <windows.h>

using namespace System;
using namespace System::IO;
using namespace System::Collections;


__gc public class TestFaults
{

private:
	
	FunctionXMLNavigator *functionNavigatorInstance;
    

public:
	FaultXMLNavigator * faultNavigatorInstance;
	TestFaults(void);
	~TestFaults(void);

	void LoadFaultsXmlFile();
	void LoadFunctionsXmlFile();
	bool TestFault(char *faultName,StreamWriter *pWriter,StreamWriter *DetailWriter);

	char *GetStringNotContains(char *incomingString);
	char *GetStringContains   (char *incomingString);
	char *GetBinaryContains   (char *incomingBinary);
	char *GetBinaryNotContains(char *incomingBinary);
	
	ArrayList *FillParamArray(String *functionName, ArrayList *matchParamsArray);
	void getInterceptionDisablers();
	bool CallFunction(FaultFunction *funcName, ArrayList *ParamArray, DWORD ReturnValue, DWORD ErrorCode,StreamWriter *DetailWriter);


};
