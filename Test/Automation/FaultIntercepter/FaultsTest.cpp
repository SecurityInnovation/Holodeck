///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		FaultsTest.cpp
///
/// DESCRIPTION: Contains definitions for the class FaultsTest.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    -----------    ------------------------------------------
//	01/11/2003    Gavin Menezes    File Created.
///*************************************************************************

#include "StdAfx.h"
#include ".\faultstest.h"
#using <mscorlib.dll>

using namespace System;
using namespace System::IO;
using namespace System::Collections;

using namespace TestingFunctionXML;
using namespace FaultsXMLFramework;

///*************************************************************************
/// Method:		FaultsTest
/// Description: Constructor
///
/// Parameters:
///	none
///
///*************************************************************************

FaultsTest::FaultsTest(void)
	{
	faultNavigatorInstance = new FaultXMLNavigator( );
	functionNavigatorInstance = new FunctionXMLNavigator( );

	}
///*************************************************************************
/// Method:		~FaultsTest
/// Description: Destructor
///
///*************************************************************************
FaultsTest::~FaultsTest(void)
	{
	}
///*************************************************************************
/// Method:		LoadFaultsXmlFile
/// Description: Loada the XML Faults file into memory
///
/// Parameters:
///	faultsXMLFilePath - the path of the faults.xml file
///
/// Return Value: void
///*************************************************************************
void FaultsTest::LoadFaultsXmlFile(char *faultsXMLFilePath){
	faultNavigatorInstance->ValidateXmlDocument(faultsXMLFilePath);
	faultNavigatorInstance->parseXmlDocument(faultsXMLFilePath);
	Console::WriteLine("Faults file loaded!");
	}

///*************************************************************************
/// Method:		LoadFunctionsXmlFile
/// Description: Loads the XML Functions file into memory
///
/// Parameters:
///	xmlFileName - the path of the functions.xml file
///
/// Return Value: void
///*************************************************************************

void FaultsTest::LoadFunctionsXmlFile(char *functionsXMLFilePath){
	functionNavigatorInstance->ValidateXmlDocument(functionsXMLFilePath);
	functionNavigatorInstance->parseXmlDocument(functionsXMLFilePath);
	Console::WriteLine("Functions file loaded!\n");
	}

///*************************************************************************
/// Method:		convertGCStringToNative
/// Description: 
///
/// Parameters:
///	
///
/// 
///*************************************************************************
char *convertGCStringToNative( String * nativeString )
	{
	return (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi( nativeString ).ToPointer( );
	}

///*************************************************************************
/// Method:		GetStringNotContains
/// Description: 
///
/// Parameters:
///	
///
/// 
///*************************************************************************
char *FaultsTest::GetStringNotContains(char *incomingString){
	//go through each character in the string
	char *newString=new char[(int)strlen(incomingString)+1];
	for (int charCount=0; charCount<=(int)strlen(incomingString);charCount++){
		int value =  char(incomingString[charCount]+1);
		newString[charCount]=char(value);		
		}
newString[strlen(newString)-5]='\0';
return newString;
	}


///*************************************************************************
/// Method:		GetStringContains
/// Description: 
///
/// Parameters:
///	
///
/// 
///*************************************************************************
char *FaultsTest::GetStringContains(char *incomingString)
	{
	char *newString=new char[strlen(GetStringNotContains(incomingString))];//new char[strlen(incomingString)];
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
/// Description: 
///
/// Parameters:
///	
///
/// 
///*************************************************************************
char *FaultsTest::GetBinaryNotContains(char* incomingBinary){

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
/// Description: 
///
/// Parameters:
///	
///
/// 
///*************************************************************************
char *FaultsTest::GetBinaryContains(char * incomingBinary){
	//similar to getStringContains
	char *newString=new char[strlen(incomingBinary)+1];

	strcpy(newString,incomingBinary);

	newString=GetBinaryNotContains(newString);

	char *resultingString=new char[strlen(incomingBinary)*3+1];

	strcpy(resultingString,newString);
	strcpy(resultingString,incomingBinary);
	strcpy(resultingString,newString);
	return resultingString;
	}

///*************************************************************************
/// Method:		TestFault
/// Description: Tests the fault name specified
///
/// Parameters:
///	faultName - The name of the fault in faults.xml to be tested
///
/// Return Value: true if successful, false otherwise
///*************************************************************************
bool FaultsTest::TestFault(char *faultName){
	//OUTPUT PASSFAIL TO A FILE
	FileStream *FileStreamer;
	FileStreamer = new FileStream("FAULTSTEST.TXT", FileMode::Create  , FileAccess::Write);
	StreamWriter * pWriter = new StreamWriter(FileStreamer);

	Console::WriteLine("---------->TestFault Method Begins<----------\n");

	//Load the Faults xml file in memory using FaultsXMLFramework
	LoadFaultsXmlFile("faults.xml");
	//Load the Functions xml file in memory using FunctionsXMLFramework
	LoadFunctionsXmlFile("functions.xml");

	bool bSuccess=true;

	Hashtable *myHT = new Hashtable();
	myHT=dynamic_cast<Hashtable *>(faultNavigatorInstance->FaultTableByName->Clone());
	IDictionaryEnumerator *faultsEnum=myHT->GetEnumerator();
	Console::WriteLine("OUTPUT FILE:FAULTTEST.TXT\n");

	while (faultsEnum->MoveNext()){
		DWORD ReturnValue=0;
		DWORD ErrorCode=0;

		Fault *fault=dynamic_cast<Fault *>(faultsEnum->get_Value());
		ReturnValue=(Convert::ToInt32(fault->get_ReturnValue()));
		ErrorCode=(Convert::ToInt32(fault->get_ErrorCode()));

		if (System::String::Compare(fault->get_Name(),faultName)==0)
			{
			ArrayList *funcList=fault->Function;
			Console::WriteLine(S"{0} Fault has {1} functions",fault->Name,__box(funcList->Count));
			pWriter->WriteLine("\n-------->   {0} Fault has {1} functions    <-------\n", fault->Name,__box(funcList->Count));
			//cycle through all functions 
			for(int ifuncs=0;ifuncs<funcList->Count;ifuncs++)
				{
				FaultFunction *thisfunc=dynamic_cast<FaultFunction *>(funcList->get_Item(ifuncs));
					{
					//****Search for a specific function--->TEST
					//if (System::String::Compare(thisfunc->get_Name(),"WSARecv")==0)
					//	{
					if(thisfunc->get_OverrideErrorCode()!=NULL){
						ErrorCode=(Convert::ToInt32(thisfunc->get_OverrideErrorCode()));
						}
				if(thisfunc->get_OverrideReturnValue()!=NULL){
					ReturnValue=(Convert::ToInt32(thisfunc->get_OverrideReturnValue()));
					}

			ArrayList *matchArray=thisfunc->get_MatchParams();

			ArrayList *paramsToSendToCallFunction = FillParamArray(convertGCStringToNative(thisfunc->Name),matchArray);

			if (CallFunction(thisfunc->get_Name(),paramsToSendToCallFunction,ReturnValue,ErrorCode) == true)
				{
				//Console::WriteLine("The function {0} has Passed.",thisfunc->Name);
				pWriter->WriteLine("{0} has Passed!",thisfunc->Name);
				bSuccess=true;
				}
			else 
				{	
				//Console::WriteLine("The function {0} has Failed.",thisfunc->Name);
				pWriter->WriteLine("{0} has Failed!",thisfunc->Name);
				bSuccess=false;
				}

					}
				//}
			}
			}
		}

Console::WriteLine("\n---------->TestFault Method Ends<------------\n");
pWriter->Close();
return bSuccess;	
	}

///*************************************************************************
/// Method:		 FillParamArray
/// Description: Fills the parameter array with values from functions.xml
///
/// Parameters:
///	faultName - The name of the fault in faults.xml to be tested
///
///*************************************************************************
ArrayList *FaultsTest::FillParamArray(char *functionName,ArrayList *matchParamArray){
	//Console::WriteLine("\n---------->FillParamArray Method Begins<---------");

	Hashtable *funcHash=new Hashtable();
	funcHash=dynamic_cast<Hashtable *>(functionNavigatorInstance->FunctionTableByName->Clone()); 
	IDictionaryEnumerator *functionsEnum=funcHash->GetEnumerator();

	ArrayList *arrayOfParams=new ArrayList();

	while(functionsEnum->MoveNext()){
		Function *function=dynamic_cast<Function *>(functionsEnum->get_Value());
		//find the specific function name
		if(function->get_FunctionName()->StartsWith(functionName)){

			ArrayList *functionParams=function->get_Parameter();

			//Console::WriteLine("The function {0} has {1} parameters",function->FunctionName ,__box(functionParams->Count));

			//declare a pointer to function parameter to be used by match params and update of parameter values
			TestingFunctionXML::Parameter *thisfuncparam;

			//cycle through all parameters one by one
			for (int i=0;i<functionParams->Count;i++)
				{
				thisfuncparam=dynamic_cast<TestingFunctionXML::Parameter *>(function->get_Parameter()->get_Item(i));

				if (String::Compare(thisfuncparam->get_CompareAs(), "PointerType") == 0)
					{
					thisfuncparam->set_AssignedValue(0);  //NULL=0
					}
				else if (String::Compare(thisfuncparam->get_CompareAs(), "StringType") == 0)
					{
					thisfuncparam->set_AssignedValue(S"TEST");
					}
				else thisfuncparam->set_AssignedValue(0);

				//chech for matchparamarray and see if it contains a "\\." value
				if(matchParamArray->Count!=0)
					{
					/******////MATCHPARAMS BEGINS HERE-->NEED TO BE TAKEN FROM AN ARRAY IN AN ARRAY
					for (int j = 0; j < matchParamArray->Count; j++)
						{
						FaultFunctionMatchParams *matchParam = dynamic_cast <FaultFunctionMatchParams *>(matchParamArray->get_Item(j));
						ArrayList *newarray=matchParam->get_MatchParam();

						/****////MATCHPARAM BEGINS HERE
						for (int k = 0; k < newarray->Count; k++)
							{
							FaultFunctionMatchParams *mParam = dynamic_cast <FaultFunctionMatchParams *>(newarray->get_Item(k));
							if (Convert::ToInt16(thisfuncparam->get_Index()) == Convert::ToInt16(mParam->get_ID())){ 
								if(String::Compare(mParam->get_TestOperator(), "not contains") == 0)
									{									
									thisfuncparam->set_AssignedValue(GetStringNotContains(convertGCStringToNative(mParam->TestValue)));
									}
								else if (String::Compare(mParam->get_TestOperator(), "contains") == 0)
									{
									thisfuncparam->set_AssignedValue(GetStringContains(convertGCStringToNative(mParam->TestValue)));
									}
								else if(String::Compare(mParam->get_TestOperator(), "binary contains") == 0)
									{
									thisfuncparam->set_AssignedValue(GetBinaryContains(convertGCStringToNative(mParam->TestValue)));

									}
								else if (String::Compare(mParam->get_TestOperator(), "binary not contains") == 0)
									{
									thisfuncparam->set_AssignedValue(GetBinaryNotContains(convertGCStringToNative(mParam->TestValue)));
									}
								else if (String::Compare(mParam->get_TestOperator(),"=") == 0)
									{
									if (String::Compare(thisfuncparam->get_CompareAs(), "StringType") == 0)
										{
										thisfuncparam->set_AssignedValue(mParam->get_TestValue()); 
										}
									else
										{
										//thisfuncparam->set_AssignedValue((VOID*)((mParam->get_TestValue())));
										thisfuncparam->set_AssignedValue(("TESTING"));
										}
									}
								}
							}//end for all matchparam
						}//end for all matchparams 
					}//end matchparamaeeay !=0
				arrayOfParams->Add(thisfuncparam);
				}//end for all params
			}//end if starts with func name
		}//end while
return arrayOfParams;
	}//end fillparam method

///*************************************************************************
/// Method:		 CallFunction
/// Description: 
///
/// Parameters:
///	
///
///*************************************************************************
bool FaultsTest::CallFunction(String *funcName, ArrayList *ParamArray, DWORD returnValue, DWORD errorCode){
	//Console::WriteLine("\n---------->CallFunction Begins<------------\n");
	
	Function0 functionPtr0 = NULL;
	Function1 functionPtr1 = NULL;
	Function2 functionPtr2 = NULL;
	Function3 functionPtr3 = NULL;
	Function4 functionPtr4 = NULL;
	Function5 functionPtr5 = NULL;
	Function6 functionPtr6 = NULL;
	Function7 functionPtr7 = NULL;
	Function8 functionPtr8 = NULL;
	Function9 functionPtr9 = NULL;
	Function10 functionPtr10 = NULL;
	Function11 functionPtr11 = NULL;
	Function12 functionPtr12 = NULL;

	int ret=returnValue;
	int err=errorCode;
bool functionfoundFlag=false;
	Console::WriteLine("Function: {0}  ",funcName);
	Console::WriteLine("ReturnValue: {0}    ErrorCode:{1}\n",Convert::ToString(ret),Convert::ToString(err));

	DWORD realError=0;
	DWORD realReturn=0;

	Hashtable *myFunctionTable = new Hashtable();
	myFunctionTable=dynamic_cast<Hashtable *>(functionNavigatorInstance->FunctionTableByName->Clone());
	IDictionaryEnumerator *fEnum=myFunctionTable->GetEnumerator();
	Function *callFunc;

//Search for the functionName in the functions file
	while (fEnum->MoveNext()){
		callFunc=dynamic_cast<Function *>(fEnum->get_Value());
		if(String::Compare(callFunc->get_FunctionName(),funcName)==0){functionfoundFlag=true;break;}
		
		}
if (functionfoundFlag==true){
Console::WriteLine( callFunc->get_FunctionName() );

DWORD *voidParamArray = new DWORD[13];
String *lpFileName = callFunc->get_OriginalDll(); 	
HMODULE hModule = LoadLibrary( convertGCStringToNative( lpFileName ) );

if ( hModule == NULL )
	{
	Console::WriteLine( "Failed to load module.\n" );
	return false;
	}	

TestingFunctionXML::Parameter *paramTemp =new TestingFunctionXML::Parameter();

for ( int paramCount = 0; paramCount < ParamArray->Count; paramCount++ )
	{
	paramTemp = dynamic_cast<TestingFunctionXML::Parameter *>(ParamArray->get_Item( paramCount ));

	Console::WriteLine("Param{0}:{1}",Convert::ToString(paramTemp->get_Index()),paramTemp->get_AssignedValue());


	if ( paramTemp->get_CompareAs() != S"StringType" )
		{
		if( !paramTemp->get_AssignedValue()->Empty){
			voidParamArray[paramCount] = Convert::ToInt32(paramTemp->get_AssignedValue(),10);
			}
		else{
			char *mychar=new char[];
			voidParamArray[paramCount] = DWORD(mychar);}
		}
	else
		{
		//voidParamArray[paramCount]=NULL;
		//voidParamArray[paramCount] = (DWORD)convertGCStringToNative((dynamic_cast<String *>(paramTemp->get_AssignedValue())));
					voidParamArray[paramCount] = Convert::ToInt32(paramTemp->get_AssignedValue(),10);

		}
	}
	
Console::WriteLine(ParamArray->Count);

try{

	switch ( ParamArray->Count )
		{
		case 0:
			functionPtr0 = (Function0)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			if (!functionPtr0){	FreeLibrary(hModule);}
			else{realReturn = (DWORD)functionPtr0();
			break;}
		case 1:
			functionPtr1 = (Function1)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			if(!functionPtr1){
			realReturn = (DWORD)functionPtr1((VOID*)voidParamArray[0]);
			break;
		case 2:
			functionPtr2 = (Function2)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr2((VOID*)voidParamArray[0],(VOID*)voidParamArray[1]);
			break;
		case 3:
			functionPtr3 = (Function3)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr3( (VOID*)voidParamArray[0], (VOID*)voidParamArray[1], (VOID*)voidParamArray[2] );
			break;
		case 4:
			functionPtr4 = (Function4)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr4( (VOID*)voidParamArray[0], (VOID*)voidParamArray[1], (VOID*)voidParamArray[2],(VOID*)voidParamArray[3] );
			break;
		case 5:
			functionPtr5 = (Function5)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr5( (VOID*)voidParamArray[0], (VOID*)voidParamArray[1], (VOID*)voidParamArray[2],(VOID*)voidParamArray[3],(VOID*)voidParamArray[4] );
			break;
		case 6:
			functionPtr6 = (Function6)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr6( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5]);
			break;
		case 7:
			functionPtr7 = (Function7)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr7( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6] );
			break;
		case 8:
			functionPtr8 = (Function8)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr8((VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7] );
			break;
		case 9:
			functionPtr9 = (Function9)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr9( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8] );
			break;
		case 10:
			functionPtr10 = (Function10)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr10( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8],(VOID*)&voidParamArray[9] );
			break;
		case 11:
			functionPtr11 = (Function11)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr11( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8],(VOID*)&voidParamArray[9],(VOID*)&voidParamArray[10] );
			break;
		case 12:
			functionPtr12 = (Function12)GetProcAddress( hModule, convertGCStringToNative( callFunc->FunctionName ) );
			realReturn = (DWORD)functionPtr12( (VOID*)&voidParamArray[0], (VOID*)&voidParamArray[1], (VOID*)&voidParamArray[2],(VOID*)&voidParamArray[3],(VOID*)&voidParamArray[4],(VOID*)&voidParamArray[5],(VOID*)&voidParamArray[6],(VOID*)&voidParamArray[7],(VOID*)&voidParamArray[8],(VOID*)&voidParamArray[9],(VOID*)&voidParamArray[10],(VOID*)&voidParamArray[11] );
			break;
	}
		}
	}catch(System::NullReferenceException  *err){} 



	realError = GetLastError();
	//	Console::WriteLine("\n---------->Call Function Ends<------------\n");
	//}catch(System::NullReferenceException *eeee){}
FreeLibrary(hModule);
int realret=realReturn;
int realerr=realError;

Console::WriteLine("\nRealReturn:{0}     RealError:{1}\n\n", Convert::ToString(realret),Convert::ToString(realerr));

return ((realError==errorCode)&&(realReturn==returnValue));
}else{Console::WriteLine("The function could not be found in functions.xml"); return false;}	



}