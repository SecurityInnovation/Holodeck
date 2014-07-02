///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		TestFaults.cpp
///
/// DESCRIPTION: Contains methods for the class TestFaults.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- --------------------------------------------
/// 03/10/2003     Gavin Menezes	File Created.
///*************************************************************************
#include "StdAfx.h"
#include ".\testfaults.h"
#include "SpecialCases.h"
#include <sys/timeb.h>
#include <windows.h>
#using <mscorlib.dll>

using namespace System;
using namespace System::IO;
using namespace Microsoft::Win32;
using namespace TestingFunctionXML;
using namespace FaultsXMLFramework;
using namespace System::Collections;

extern FARPROC disableInterception = NULL;
extern FARPROC enableInterception = NULL;
TestFaults::TestFaults(void)
	{	
	faultNavigatorInstance = new FaultXMLNavigator( );
	functionNavigatorInstance = new FunctionXMLNavigator( );
	}

TestFaults::~TestFaults(void)
	{
	}

	///*************************************************************************
	/// Method:		getInterceptionDisablers
	/// Description: Enables/Disables Interception
	///
	/// Parameters: None.
	///	
	/// Return Value : None. 
	///*************************************************************************
void TestFaults::getInterceptionDisablers()
	{
	HMODULE hHeatHandle;
	hHeatHandle = GetModuleHandle("Heat4dll.dll");
	if (hHeatHandle == NULL)
		printf("ERROR Loading Module HEAT4DLL.DLL!\n");
	
	// Get function pointers...
	disableInterception = GetProcAddress(hHeatHandle, "disableInterceptionInCurrentThread");
	enableInterception = GetProcAddress(hHeatHandle, "enableInterceptionInCurrentThread");
	return;
	}


	///*************************************************************************
	/// Method:		GetStringNotContains
	/// Description: Mixes the values in the string
	///
	/// Parameters: A bunch of characters to mix up
	///	
	/// Return Value : Returns  characters 
	///*************************************************************************
char *TestFaults::GetStringNotContains(char *incomingString)
	{
	//go through each character in the string
	char *newString = new char[(int)strlen(incomingString)+1];
	for (int charCount=0; charCount<=(int)strlen(incomingString);charCount++)
		{
		int value =  char(incomingString[charCount]+1);
		newString[charCount] = char(value);		
		}
	//newString[strlen(newString)-5]='\0';
	return "Test";
	}

	///*************************************************************************
	/// Method:		GetStringContains
	/// Description: Mixes the values in the string
	///
	/// Parameters: A bunch of characters to mix up
	///	
	/// Return Value : Returns characters 
	///*************************************************************************
char *TestFaults::GetStringContains(char *incomingString)
	{
	char *newString=new char[strlen(GetStringNotContains(incomingString))];
	newString = GetStringNotContains(incomingString);
	char *resultingString=new char[strlen(incomingString)*3+1];
	strcpy(resultingString,newString);
	strcat(resultingString,incomingString);
	strcat(resultingString,newString);
	delete newString;
	return resultingString;
	}

	///*************************************************************************
	/// Method:		GetBinaryNotContains
	/// Description: Mixes the values in the string (values are binary numbers)
	///
	/// Parameters: A bunch of characters to mix up
	///	
	/// Return Value : Returns characters 
	///*************************************************************************
char *TestFaults::GetBinaryNotContains(char *incomingBinary){

	char *newString=new char[(int)(strlen(incomingBinary))+1];

	for (int i = 0; i<(int)strlen(incomingBinary);i++)
		{
		if (incomingBinary[i]=='0')
			{
			newString[i]='1';
			}
		else if(incomingBinary[i]=='1')
			{
			newString[i]='0';
			}

		}
	newString[strlen(newString)-5]='\0';
	return newString;
	}

	///*************************************************************************
	/// Method:		GetBinaryContains
	/// Description: Mixes the values in the string (values are binary numbers)
	///
	/// Parameters: A bunch of characters to mix up
	///	
	/// Return Value : Returns characters 
	///*************************************************************************
char *TestFaults::GetBinaryContains(char *incomingBinary){
	//similar to getStringContains but we add extra binary values
	char *newString="1";
	//strcpy(newString,incomingBinary);
	//newString=GetBinaryNotContains(newString);
	char *resultingString=new char[strlen(incomingBinary)*3+1];
	strcpy(resultingString,newString);
	strcpy(resultingString,incomingBinary);
	strcpy(resultingString,newString);
	return resultingString;
	}

	///*************************************************************************
	/// Method:		LoadFaultsXmlFile
	/// Description: Opens the faults.xml file and loads its contents/structure
	///			     to memory.
	///
	/// Parameters: None.
	///	
	/// Return Value : None.
	///*************************************************************************
void TestFaults::LoadFaultsXmlFile()
	{
	RegistryKey *LMachine = RegistryKey::OpenRemoteBaseKey(RegistryHive::LocalMachine, System::Environment::MachineName);            
	RegistryKey *SKey = LMachine->OpenSubKey( "SOFTWARE", true );
	RegistryKey *HKey =  SKey->OpenSubKey( "HolodeckEE", true);
	String *dbPath = dynamic_cast<String*>(HKey->GetValue("DtdPath"));
	String *dbName = dynamic_cast<String*>(HKey->GetValue("FunctionDatabaseFolder"));

	String *fullfaultsPath = String::Concat(dbPath,"\\",dbName,"\\");
	String *fullfunctionsPath = String::Concat(dbPath,"\\",dbName,"\\");
	fullfaultsPath = String::Concat(fullfaultsPath,"faults.xml");
	faultNavigatorInstance->ValidateXmlDocument(fullfaultsPath);
	faultNavigatorInstance->parseXmlDocument(fullfaultsPath);
	Console::WriteLine("Faults file loaded!");
	}

	///*************************************************************************
	/// Method:		LoadFunctionsXmlFile
	/// Description: Opens the functions.xml file and loads its contents/structure
	///			     to memory.
	///
	/// Parameters: None.
	///	
	/// Return Value : None.
	///*************************************************************************
void TestFaults::LoadFunctionsXmlFile()
	{
	RegistryKey *LMachine = RegistryKey::OpenRemoteBaseKey(RegistryHive::LocalMachine, System::Environment::MachineName);            
	RegistryKey *SKey = LMachine->OpenSubKey( "SOFTWARE", true );
	RegistryKey *HKey =  SKey->OpenSubKey( "HolodeckEE", true);
	String *dbPath = dynamic_cast<String*>(HKey->GetValue("DtdPath"));
	String *dbName = dynamic_cast<String*>(HKey->GetValue("FunctionDatabaseFolder"));

	String *fullfaultsPath = String::Concat(dbPath,"\\",dbName,"\\");
	String *fullfunctionsPath = String::Concat(dbPath,"\\",dbName,"\\");
	fullfunctionsPath = String::Concat(fullfunctionsPath,"functions.xml");

	functionNavigatorInstance->ValidateXmlDocument(fullfunctionsPath);
	functionNavigatorInstance->parseXmlDocument(fullfunctionsPath);
	Console::WriteLine("Functions file loaded!\n");
	}




	///*************************************************************************
	/// Method:		 TestFault
	/// Description: Initilizes all requirements to call the faults
	///
	/// Parameters: Name of the fault to set
	///				Log for the functions of the fault(pass/fail)
	///				Log for detailed information on each function
	///	
	/// Return Value : Boolean (pass/fail of fault)
	///*************************************************************************
bool TestFaults::TestFault(char *faultName, StreamWriter *pWriter, StreamWriter *DetailWriter)
	{

	getInterceptionDisablers();

	if(disableInterception != NULL) disableInterception();
	String *path = System::Environment::CurrentDirectory;
	//Indicate when functions begins -->for testing
	Console::WriteLine("---------->TestFault Method Begins<----------\n");

	//If a fault is successful or not
	bool bSuccess = true;
	if(disableInterception != NULL) disableInterception();
	//Get an enumerator to the faults.xml tables in memory using a hashtable
	Hashtable *faultHashTable = new Hashtable();
	faultHashTable=dynamic_cast<Hashtable *>(faultNavigatorInstance->FaultTableByName->Clone());
	IDictionaryEnumerator *faultsEnum=faultHashTable->GetEnumerator();
	RegistryKey *LMachine = RegistryKey::OpenRemoteBaseKey(RegistryHive::LocalMachine, System::Environment::MachineName);            
	RegistryKey *SKey = LMachine->OpenSubKey( "SOFTWARE", true );
	RegistryKey *HKey =  SKey->OpenSubKey( "HolodeckEE", true);
	RegistryKey *IntercepterReadyKey = HKey->CreateSubKey("FaultIntercepterReady");
	Sleep(2000);

	RegistryKey *FaultIntercepterKey;
	if(disableInterception != NULL) disableInterception();
	Console::Write("\nWaiting to set fault");
	do	{
		if(disableInterception != NULL)disableInterception();
		Sleep(500);
		Console::Write(".");
		FaultIntercepterKey = HKey->OpenSubKey("FaultReady");
		}while(FaultIntercepterKey == NULL);

		Console::WriteLine("\nNow running fault...");
		Sleep(10000);
		HKey->DeleteSubKey("FaultIntercepterReady");

		while(faultsEnum->MoveNext())
			{
			//Initilize returnvalue and errorcodes for functions
			DWORD ReturnValue = 0;
			DWORD ErrorCode = 0;

			//get the return values and errorcodes for the fault. Convert to DWORD
			Fault *fault = dynamic_cast<Fault *>(faultsEnum->get_Value());
			
			ReturnValue = (Convert::ToInt32(fault->get_ReturnValue()));
			ErrorCode = (Convert::ToInt32(fault->get_ErrorCode()));

			//locate the specified fault name in the list of faults 
			//(DELETE THIS TO CYCLE THROUGH ALL FAULTS)
			if (System::String::Compare(fault->get_Name(),faultName) == 0)
				{
				DetailWriter->WriteLine("------>Detailed Information for {0} Fault<--------", fault->Name);			
				//Print the number of faults in the function
				Console::WriteLine(S"{0} Fault has {1} functions",fault->Name,__box(fault->Function->Count));

				//write number of faults in function to file
				pWriter->WriteLine("\n-------->   {0} Fault has {1} functions    <-------\n", fault->Name,__box(fault->Function->Count));

				//Get an array of all the functions
				for(int ifuncs=0;ifuncs<fault->Function->Count;ifuncs++)
					{pWriter->Flush();
				//get a pointer to each function in the fault
				FaultFunction *faultFunction=dynamic_cast<FaultFunction *>(fault->Function->get_Item(ifuncs));

				//Set the return values and errorcodes for the specific function
				if(faultFunction->get_OverrideErrorCode() != NULL)
					{
					String *ErrorCodeNoChange = faultFunction->get_OverrideErrorCode();
					if (String::Compare(ErrorCodeNoChange,"No Change") == 0)
						{
						ErrorCode = (Convert::ToInt32(fault->get_ErrorCode()));
						}
					else
						{
						ErrorCode = (Convert::ToInt32(faultFunction->get_OverrideErrorCode()));
						}
					}

				if(faultFunction->get_OverrideReturnValue() != NULL)
					{
					String *ReturnValueNoChange =faultFunction->get_OverrideReturnValue();
					if (String::Compare(ReturnValueNoChange,"No Change") == 0)
						{
						ReturnValue = (Convert::ToInt32(fault->get_ReturnValue()));
						}
					else 
						{
						ReturnValue = (Convert::ToInt32(faultFunction->get_OverrideReturnValue()));
						}
					}

				//get a pointer to a list of parameters 
				ArrayList *paramsToSendToCallFunction = FillParamArray(faultFunction->Name,faultFunction->get_MatchParams());
								
				//call the function 
				bool callfunctionPassFail = CallFunction(faultFunction,paramsToSendToCallFunction,ReturnValue,ErrorCode,DetailWriter);
				if (callfunctionPassFail == true)
					{
					//Console::WriteLine("The function {0} has Passed.",faultFunction->Name);
					if(disableInterception != NULL)disableInterception();
					pWriter->WriteLine("{0} has Passed!",faultFunction->Name);
					bSuccess=true;
					}
				else if(callfunctionPassFail == false)
					{	
					//Console::WriteLine("The function {0} has Failed.",faultFunction->Name);
					if(disableInterception != NULL)disableInterception();
					pWriter->WriteLine("{0} has Failed!",faultFunction->Name);
					bSuccess=false;
					}
					}
				}
			//	 }

			}

		Console::WriteLine("\n-------------->TestFault Method Ends<------------");
		pWriter->Flush();

		return bSuccess;	
	}



	///*************************************************************************
	/// Method:		 FillParamArray
	/// Description: Fills the parameter array of each function in a fault
	///
	/// Parameters: Name of the function in the fault
	///				An array list of match parameters				
	///	
	/// Return Value : A list of the parameters in the function
	///*************************************************************************
ArrayList *TestFaults::FillParamArray(String *functionName, ArrayList *matchParamsArray)
	{
	//Get an emumerator to the functions table
	Hashtable *functionHashTable=new Hashtable();
	functionHashTable=dynamic_cast<Hashtable *>(functionNavigatorInstance->FunctionTableByName->Clone()); 
	IDictionaryEnumerator *functionsEnum=functionHashTable->GetEnumerator();

	//The functionName variable is a string, We need a pointer to the function 
	//with that name from functions.xml fuile
	Function *function;
	while(functionsEnum->MoveNext())
		{ 
		function=dynamic_cast<Function *>(functionsEnum->get_Value());
		if (String::Compare(function->FunctionName,functionName) == 0)
			{
			break;
			}
		}
	ArrayList *arrayOfParameters = new ArrayList();
	
	//Get the parameters for the specified function
	for(int iParams=0;iParams<function->Parameter->Count;iParams++)
		{
		TestingFunctionXML::Parameter *functionParam = dynamic_cast<TestingFunctionXML::Parameter *>(function->get_Parameter()->get_Item(iParams));

		//Compare the type of parameter and set the AssignedValue of that parameter
		if (String::Compare(functionParam->get_CompareAs(), "PointerType") == 0)
			{
			functionParam->set_AssignedValue(NULL);  //NULL=0
			}
		else if (String::Compare(functionParam->get_CompareAs(), "StringType") == 0)
			{
			functionParam->set_AssignedValue(L"TEST");
			}
		else functionParam->set_AssignedValue(NULL);

		if(matchParamsArray!=0)
			{

			//For each parameter check if it has a matching match params
			for(int iMParams = 0;iMParams<matchParamsArray->Count;iMParams++)
				{
				FaultFunctionMatchParams *functionMatchParams=dynamic_cast <FaultFunctionMatchParams *>(matchParamsArray->get_Item(iMParams));
				ArrayList *matchParamArray = functionMatchParams->get_MatchParam();

				//Get a list of MatchParam values
				for(int iMParam=0;iMParam<matchParamArray->Count;iMParam++)
					{
					FaultFunctionMatchParams *functionMatchParam=dynamic_cast <FaultFunctionMatchParams *>(matchParamArray->get_Item(iMParam));
					if(String::Compare(functionParam->Name,functionMatchParam->Name)==0)
						{
						char *functionTestValue = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(functionMatchParam->TestValue).ToPointer();

						if(String::Compare(functionMatchParam->get_TestOperator(), "not contains") == 0)
							{									
							functionParam->set_AssignedValue(GetStringNotContains(functionTestValue));
							}
						else if (String::Compare(functionMatchParam->get_TestOperator(), "contains") == 0)
							{
							functionParam->set_AssignedValue(GetStringContains(functionTestValue));
							}
						else if(String::Compare(functionMatchParam->get_TestOperator(), "binary contains") == 0)
							{
							functionParam->set_AssignedValue(GetBinaryContains(functionTestValue));
							}
						else if (String::Compare(functionMatchParam->get_TestOperator(), "binary not contains") == 0)
							{
							functionParam->set_AssignedValue(GetBinaryNotContains(functionTestValue));
							}
						else if (String::Compare(functionMatchParam->get_TestOperator(),"=") == 0)
							{
							if (String::Compare(functionParam->get_CompareAs(), "StringType") == 0)
								{
								functionParam->set_AssignedValue(functionMatchParam->get_TestValue()); 
								}
							else
								{
								//functionParam->set_AssignedValue(((functionMatchParam->get_TestValue())));
								functionParam->set_AssignedValue((0));
								}
							}
						}//---String Compare
					}//--For IMParam
				}//--For IMParams
			}//--For !=0
		arrayOfParameters->Add(functionParam);
		}
	return arrayOfParameters;
	}




	///*************************************************************************
	/// Method:		 CallFunction
	/// Description: Calls the function and records the retuen value and error code
	///
	/// Parameters: Name of the ffunction to call
	///				A list of parameters 
	///				The original return value set for function in faults.xml
	///				The original error code set for function in faults.xml
	///				Log records the detailed information (errorcode/return value for function)
	///	
	/// Return Value : Boolean (pass/fail of fault)
	///*************************************************************************
bool TestFaults::CallFunction(FaultFunction *thisfault, ArrayList *ParamArray, DWORD returnValue, DWORD errorCode,StreamWriter *DetailWriter)
	{
	if(disableInterception != NULL)disableInterception();

	//print function name to screen
	Console::WriteLine(thisfault->Name);
	DetailWriter->WriteLine(thisfault->Name);

	//Initilize all function pointers to NULL
	Function0 functionPtr0 = NULL;
	Function1 functionPtr1 = NULL;
	Function2 functionPtr2 = NULL;
	Function3 functionPtr3 = NULL;
	Function4 functionPtr4 = NULL;
	Function5 functionPtr5 = NULL;
	Function6 functionPtr6 = NULL;
	Function7 functionPtr7 = NULL;
	Function8 functionPtr8 = NULL;
	Function9 functionPtr9   = NULL;
	Function10 functionPtr10 = NULL;
	Function11 functionPtr11 = NULL;
	Function12 functionPtr12 = NULL;

	//initliize real returnvalues and real errorcodes from function pointers
	DWORD realError=0;
	DWORD realReturn=0;
	String *funcName = thisfault->Name;

	//Getting the hashtable for functions.xml
	Hashtable *FunctionTable = new Hashtable();
	FunctionTable = dynamic_cast<Hashtable *>(functionNavigatorInstance->FunctionTableByName->Clone());
	IDictionaryEnumerator *fEnum = FunctionTable->GetEnumerator();
	Function *callFunc;
	bool functionfoundFlag = false;

	//Getting a pointer to the required function name
	//if the function does not exist in functions.xml then return fail
	while (fEnum->MoveNext())
		{
		callFunc=dynamic_cast<Function *>(fEnum->get_Value());
		if(String::Compare(callFunc->get_FunctionName(),funcName) == 0){functionfoundFlag = true; break;}
		}

	if (functionfoundFlag == false)
		{
		Console::WriteLine("Function not in functions.xml\n"); 
		DetailWriter->WriteLine("Function not in functions.xml\n"); return false;
		}

	//Initilize voidParamArray and get the name of dll file to load for the function
	DWORD *voidParamArray = new DWORD[13];

	if(disableInterception != NULL)disableInterception();

	String *lpFileName = callFunc->get_OriginalDll(); 	
	//LPCTSTR lpFileName = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(lpFileName).ToPointer( );
	HMODULE hModule = LoadLibrary((char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(lpFileName).ToPointer( ));
	//HMODULE hModule = GetModuleHandle(lpFileName);

	//If the dll file cannot be loaded then the function fails 
	if ( hModule == NULL )
		{
		Console::WriteLine( "Failed to load module.\n" );
		return false;
		}

	//initilize a variable to hold the parameter information for the fuction
	TestingFunctionXML::Parameter *paramTemp = new TestingFunctionXML::Parameter();

	/////////////////////////////CHECK RESOURCE/////////////////////////////////////////////

	if(thisfault->get_CheckResource()!=NULL)
		{
		for (int icheck = 0; icheck <thisfault->CheckResource->Count;icheck++)
			{

			FaultFunctionCheckResource *thisfaultCheckRe = dynamic_cast<FaultFunctionCheckResource *>(thisfault->CheckResource->get_Item(icheck));
			//check Resource
			//0 - NAEXISTS
			//1 - EXISTS
			//2 - NOTEXISTS
			//3 - CREATE

			if(String::Compare(thisfaultCheckRe->Exists,"0") == 0)
				{//Not Applicable
				int paramindex = System::Convert::ToInt16(thisfaultCheckRe->ParamIndex,10);
				Console::WriteLine("CheckResource->ParamIndex:{0}   Resource:{1}",System::Convert::ToString(paramindex),thisfaultCheckRe->Exists);
				//if resource is 0 then do nothing
				}

			if(String::Compare(thisfaultCheckRe->Exists,"1") == 0||String::Compare(paramTemp->Name, "lpExistingFileName")==0)
				{
				//not exists
				int paramindex = System::Convert::ToInt16(thisfaultCheckRe->ParamIndex,10);
				Console::WriteLine("CheckResource->ParamIndex:{0}   Resource:{1}",System::Convert::ToString(paramindex),thisfaultCheckRe->Exists);
				paramTemp = dynamic_cast<TestingFunctionXML::Parameter *>(ParamArray->get_Item(paramindex));
				if(String::Compare(paramTemp->Name, "lpNewFileName") == 0)
					{
					if(paramTemp->get_AssignedValue() != NULL)
						{
						//verify that it does exists
						try{
							FileStream *exists = new FileStream(paramTemp->get_AssignedValue(),FileMode::Open,FileAccess::ReadWrite);  
							exists->Close();
							}catch(System::IO::FileNotFoundException *err)
							{
							Console::WriteLine("Resource does not exist!\n");	
							}
						}
					}
				}

			if(String::Compare(thisfaultCheckRe->Exists,"2") == 0)
				{//notexists
				int paramindex = System::Convert::ToInt16(thisfaultCheckRe->ParamIndex,10);
				Console::WriteLine("CheckResource->ParamIndex:{0}   Resource:{1}",System::Convert::ToString(paramindex),thisfaultCheckRe->Exists);
				paramTemp = dynamic_cast<TestingFunctionXML::Parameter *>(ParamArray->get_Item(paramindex));
				if(String::Compare(paramTemp->Name, "lpNewFileName") == 0 ||String::Compare(paramTemp->Name, "lpExistingFileName")==0)
					{
					if(paramTemp->get_AssignedValue() != NULL)
						{
						//verify that it does exists
						try{
							FileStream *exists = new FileStream(paramTemp->get_AssignedValue(),FileMode::Open,FileAccess::ReadWrite);  
							exists->Close();
							}catch(System::IO::FileNotFoundException *err)
							{
							Console::WriteLine("Resource does not exist - made it exist!\n");	
							FileStream *makeitexist = new FileStream(paramTemp->get_AssignedValue(),FileMode::CreateNew ,FileAccess::ReadWrite );  
							makeitexist->Close();
							}
						}
					}
				}

			if(String::Compare(thisfaultCheckRe->Exists,"3") == 0)
				{//create
				int paramindex = System::Convert::ToInt16(thisfaultCheckRe->ParamIndex,10);
				Console::WriteLine("CheckResource->ParamIndex:{0}   Resource:{1}",System::Convert::ToString(paramindex),thisfaultCheckRe->Exists);
				paramTemp = dynamic_cast<TestingFunctionXML::Parameter *>(ParamArray->get_Item(paramindex));
				if(String::Compare(paramTemp->Name, "lpNewFileName") == 0||String::Compare(paramTemp->Name, "lpExistingFileName")==0)
					{
					if(paramTemp->get_AssignedValue() != NULL)
						{
						//create the resource or file
						try{
							Console::WriteLine("Resource Created!\n");	
							FileStream *makeitexist = new FileStream(paramTemp->get_AssignedValue(),FileMode::CreateNew ,FileAccess::ReadWrite);  
							makeitexist->Close();
							}
					catch(System::IO::FileNotFoundException *err)
						{

						}
						}
					}
				}
			}
		}

	/////////////////////////////ALLOCATION GT//////////////////////////////////////////////
	if(thisfault->Allocation!=NULL){

		FileStream *FileStreamer;
		FileStreamer = new FileStream("c:\\TEST.txt", FileMode::OpenOrCreate, FileAccess::ReadWrite );
		StreamWriter * pWriter = new StreamWriter(FileStreamer);
		pWriter->Close();
		if(String::Compare(thisfault->Allocation,"GT") == 0)
			{
			DWORD   INT_VAL = 20;
			HANDLE GENERIC_HANDLE = &INT_VAL;

			//Following are special case functions for Allocation GT
			if(String::Compare(thisfault->get_Name(),"ReplaceFileA") == 0)
				{
				LPCSTR filetoreplace = "TEST.TXT";
				LPCSTR filereplacement= "TEST2.TXT";
				LPCSTR filebackup= "TESTBackup.TXT";
				DWORD dwRet = 20;
				if(enableInterception != NULL)enableInterception();
				dwRet = ReplaceFileA(filetoreplace, filereplacement,filebackup ,NULL ,NULL ,NULL );
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&(dwRet==returnValue));
				}
			if(String::Compare(thisfault->get_Name(),"ReplaceFileW") == 0)
				{
				LPCWSTR filetoreplace = L"TEST.TXT";
				LPCWSTR filereplacement= L"TEST2.TXT";
				LPCWSTR filebackup= L"TESTBackup.TXT";
				DWORD dwRet = 20;
				if(enableInterception != NULL)enableInterception();
				dwRet = ReplaceFileW(filetoreplace, filereplacement,filebackup ,NULL ,NULL ,NULL );
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&(dwRet==returnValue));
				}
			if(String::Compare(thisfault->get_Name(),"CopyFileA") == 0)
				{
				LPCSTR filetocopy = "TEST.TXT";
				LPCSTR filecopyto = "TEST2.TXT";
				DWORD dwRet = 20;
				if(enableInterception != NULL)enableInterception();				 
				dwRet = CopyFileA(filetocopy, filecopyto, true);	
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&(dwRet==returnValue));
				}
			if(String::Compare(thisfault->get_Name(),"CopyFileW") == 0)
				{
				LPCWSTR filetocopy = L"TEST.TXT";
				LPCWSTR filecopyto = L"TEST2.TXT";
				DWORD dwRet = 20;
				if(enableInterception != NULL)enableInterception();
				dwRet = CopyFileW(filetocopy, filecopyto, true);	
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&(dwRet==returnValue));
				}

			if(String::Compare(thisfault->get_Name(),"CopyFileExA") == 0)
				{
				LPCSTR filetocopy = "TEST.TXT";
				LPCSTR filecopyto = "TEST2.TXT";
				DWORD dwRet = 20;
				if(enableInterception != NULL)enableInterception();
				dwRet = CopyFileExA(filetocopy, filecopyto, NULL,NULL,false, 1);	
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&(dwRet==returnValue));
				}

			if(String::Compare(thisfault->get_Name(),"CopyFileExW") == 0)
				{
				LPCWSTR filetocopy = L"TEST.TXT";
				LPCWSTR filecopyto = L"TEST2.TXT";
				BOOL dwRet = 0;
				if(enableInterception != NULL)enableInterception();
				dwRet = CopyFileExW(filetocopy, filecopyto, NULL, NULL, false, 1);	
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&(dwRet==returnValue));
				}

			if(String::Compare(thisfault->get_Name(),"GlobalReAlloc") == 0)
				{
				HGLOBAL dwRet = 0;
				if(enableInterception != NULL)enableInterception();
				dwRet = GlobalReAlloc(GlobalAlloc(GMEM_MOVEABLE, 2), 2, GMEM_MODIFY);
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&((DWORD)(dwRet) == returnValue));
				}

			if(String::Compare(thisfault->get_Name(),"LocalReAlloc") == 0)
				{
				HLOCAL dwRet = 0;
				if(enableInterception != NULL)enableInterception();
				dwRet = LocalReAlloc(GENERIC_HANDLE, INT_VAL, INT_VAL);
				DWORD copyError = GetLastError();
				return ((copyError==errorCode)&&((DWORD)(dwRet) == returnValue));
				}	

			if(String::Compare(thisfault->get_Name(),"HeapReAlloc") == 0)
				{
				LPVOID dwRet = 0;
			CHAR * local_pointer = NULL;
			if(enableInterception != NULL)enableInterception();
			dwRet = HeapReAlloc(GetProcessHeap(), INT_VAL, local_pointer, INT_VAL);
			DWORD copyError = GetLastError();
			return ((copyError==errorCode)&&((DWORD)(dwRet) == returnValue));
				}

			}
		}


////////////////////////////////////PARAMETER CHECKS/////////////////////////////////////////
//Cycle through all the parameters
	for ( int paramCount = 0; paramCount < ParamArray->Count; paramCount++ )
	{
	paramTemp = dynamic_cast<TestingFunctionXML::Parameter *>(ParamArray->get_Item(paramCount));
	if ( paramTemp->get_CompareAs() == S"StringType" )
		{			
		if( !paramTemp->get_AssignedValue()->Empty)
			{
			voidParamArray[paramCount] = Convert::ToInt32(paramTemp->get_AssignedValue(),10);
			}
		else{
			voidParamArray[paramCount] = 0;
			}
		}
	else
		{
		voidParamArray[paramCount] = 0;
		//voidParamArray[paramCount] = (VOID*)paramTemp->get_AssignedValue();
		//voidParamArray[paramCount] = (DWORD)convertGCStringToNative((dynamic_cast<String *>(paramTemp->get_AssignedValue())));
		//voidParamArray[paramCount] = Convert::ToInt32(paramTemp->get_AssignedValue());
		}
	}

char*procAddressFunctionName = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(callFunc->FunctionName).ToPointer();
SpecialCases *Sp_cases = new SpecialCases();

///try{

if(enableInterception != NULL)enableInterception();
////////////////////////////////////RUN THE SPECIFIED FUNCTION///////////////////////////////
switch ( ParamArray->Count )
	{
	case 0:
		functionPtr0 = (Function0)GetProcAddress( hModule, procAddressFunctionName );
		if (!functionPtr0){	FreeLibrary(hModule);}
		else{realReturn = (DWORD)functionPtr0();
		break;
			}
	case 1:
		functionPtr1 = (Function1)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr1((VOID*)voidParamArray[0]);
		break;	

		//**SPECIAL***//
	case 2:
		functionPtr2 = (Function2)GetProcAddress( hModule, procAddressFunctionName );
		if(String::Compare(callFunc->FunctionName,"FindFirstFileA") == 0 || String::Compare(callFunc->FunctionName,"FindFirstFileW")==0){
			realReturn = Sp_cases->Sp_FindFirstFileW();
			}
		else if(String::Compare(callFunc->FunctionName,"FindNextFileA") == 0 || String::Compare(callFunc->FunctionName,"FindNextFileW")==0){
			realReturn = Sp_cases->Sp_FindNextFileA(); 
			}
		else if(String::Compare(callFunc->FunctionName,"LoadModule") == 0) {
			realReturn = Sp_cases->Sp_LoadModule(); 
			}
		else{
			realReturn = (DWORD)functionPtr2((VOID*)voidParamArray[0],(VOID*)voidParamArray[1]);
			}
	break;
	//**SPECIAL***//
	case 3:
		functionPtr3 = (Function3)GetProcAddress( hModule, procAddressFunctionName );
		if(String::Compare(callFunc->FunctionName,"HeapAlloc") == 0){
			realReturn = Sp_cases->Sp_HeapAlloc();
			}
		else {								   
			realReturn = (DWORD)functionPtr3( (VOID*)voidParamArray[0], (VOID*)voidParamArray[1], (VOID*)voidParamArray[2] );
			}
	break;
	case 4:
		functionPtr4 = (Function4)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr4( (VOID*)voidParamArray[0], (VOID*)voidParamArray[1], (VOID*)voidParamArray[2],(VOID*)voidParamArray[3] );
		break;
		//**SPECIAL***//
	case 5:
		functionPtr5 = (Function5)GetProcAddress( hModule, procAddressFunctionName );
		if(String::Compare(callFunc->FunctionName,"UnlockFileEx")==0){	
			realReturn = Sp_cases->Sp_UnlockFileEx(); 
			}
		else if(String::Compare(callFunc->FunctionName,"WriteFileEx") == 0){
			realReturn = Sp_cases->Sp_WriteFileEx(); 
			}
		else if(String::Compare(callFunc->FunctionName,"ReadFileEx") == 0){
			realReturn = Sp_cases->Sp_ReadFileEx(); 
			}
		else{
			realReturn = (DWORD)functionPtr5( (VOID*)voidParamArray[0], (VOID*)voidParamArray[1], (VOID*)voidParamArray[2],(VOID*)voidParamArray[3],(VOID*)voidParamArray[4] );
			}
	break;
	case 6:
		functionPtr6 = (Function6)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr6( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5]);
		break;
	case 7:
		functionPtr7 = (Function7)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr7( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6] );
		break;
	case 8:
		functionPtr8 = (Function8)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr8((VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7] );
		break;
	case 9:
		functionPtr9 = (Function9)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr9( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8] );
		break;
	case 10:
		functionPtr10 = (Function10)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr10( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8],(VOID*)&voidParamArray[9] );
		break;
	case 11:
		functionPtr11 = (Function11)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr11( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8],(VOID*)&voidParamArray[9],(VOID*)&voidParamArray[10] );
		break;
	case 12:
		functionPtr12 = (Function12)GetProcAddress( hModule, procAddressFunctionName );
		realReturn = (DWORD)functionPtr12( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8],(VOID*)&voidParamArray[9],(VOID*)&voidParamArray[10],(VOID*)&voidParamArray[11] );
		break;
	}


if(disableInterception != NULL)disableInterception();
realError = GetLastError(); 

int realret=realReturn;
int realerr=realError;

Console::WriteLine("ReturnValue: {0}   ErrorCode:{1}",Convert::ToString((int)returnValue),Convert::ToString((int)errorCode));
Console::WriteLine("RealReturn:{0}     RealError:{1}\n\n", Convert::ToString(realret),Convert::ToString(realerr));
DetailWriter->Flush();
DetailWriter->WriteLine("ReturnValue: {0}   ErrorCode:{1}",Convert::ToString((int)returnValue),Convert::ToString((int)errorCode));
DetailWriter->Flush();
DetailWriter->WriteLine("RealReturn:{0}     RealError:{1}\n\n", Convert::ToString(realret),Convert::ToString(realerr));
DetailWriter->Flush();

if(enableInterception != NULL) enableInterception();
return ((realError==errorCode));//&&(realReturn==returnValue));
	}