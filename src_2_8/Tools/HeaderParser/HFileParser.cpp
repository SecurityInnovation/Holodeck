//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HFileParser.cpp
//
// DESCRIPTION: Contains implemntation for the class HFileParser
//
//========================================================================
// Modification History
//
// Date         SCR  Name							Purpose
// -----------  ---  -----------					-----------------------
// 05 Oct 2003		 P.Singh, P.Patel, A.Jauhari	File created.
//*************************************************************************

#include "hfileparser.h"

namespace HeaderParser
{
	//*************************************************************************
	// Method:		HFileParser
	// Description: Constructor for the HFileParser class
	//
	// Parameters:
	//	szFilePath - full path to .h file to be parsed
	//
	// Return Value: None
	//*************************************************************************
	HFileParser::HFileParser(String *szFilePath)
	{
		szFile = szFilePath;
		szFunction = "";
		funcList = new ArrayList();
		bFiltFunc = false;
	}

	//*************************************************************************
	// Method:		HFileParser
	// Description: Constructor for the HFileParser class
	//
	// Parameters:
	//	szFilePath - full path to .h file to be parsed
	//	funcFilterFilePath - full path to text file containing functions to be parsed
	//
	// Return Value: None
	//*************************************************************************
	HFileParser::HFileParser(String *szFilePath, String *funcFilterFilePath)
	{
		szFile = szFilePath;
		funcFilterFile = funcFilterFilePath;
		szFunction = "";
		funcList = new ArrayList();
		selFuncs = new ArrayList();
		bFiltFunc = true;
	}

	//*************************************************************************
	// Method:		HFileParser
	// Description: Constructor for the HFileParser class
	//
	// Parameters:
	//	szFilePath - full path to .h file to be parsed
	//	funcsToParse - an array of function names to be parsed
	//
	// Return Value: None
	//*************************************************************************
	HFileParser::HFileParser(String *szFilePath, ArrayList *funcsToParse)
	{
		szFile = szFilePath;
		szFunction = "";
		funcList = new ArrayList();

		//Create an arraylist of InterceptedFunction types for parser
		ArrayList *ifToParse = new ArrayList (funcsToParse->Count);
		for (int i=0; i < funcsToParse->Count; i++)
		{
			InterceptedFunction *ifFunc = new InterceptedFunction();
			ifFunc->Name = dynamic_cast <String *> (funcsToParse->Item[i]);
			ifToParse->Add (ifFunc);
		}
		
		selFuncs = ifToParse;
		bFiltFunc = false;
	}

	//*************************************************************************
	// Method:		HFileParser
	// Description: Destructor for the HFileParser class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HFileParser::~HFileParser()
	{
	}

	//*************************************************************************
	// Method:		parseFile
	// Description: Parses the file, reads line by line
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing function objects for functions in .h file
	//*************************************************************************
	ArrayList *HFileParser::parseFile()
	{
		//check if file exists
		if(!File::Exists(szFile))
			return 0;

		//if selected functions file is specified, read the functionsList of functions to be parsed
		if (bFiltFunc)
			this->readSelectedFuncsList();

		hFileReader = File::OpenText(szFile);

		//main file reading loop
		Char braces[] = {'{', '}'};
		Char space[] = {' '};
		while((szFileLine =(hFileReader->ReadLine())) != 0)
		{
			szFileLine = szFileLine->Trim();

			if(szFileLine->IndexOf("//") != -1)
			{
				szFileLine = szFileLine->Substring(0, szFileLine->IndexOf("//"));
				szFileLine = szFileLine->Trim();
			}
			if(szFileLine->IndexOf("/*") != -1)
			{
				String *temp = szFileLine->Substring(0, szFileLine->IndexOf("/*"))-> Trim();
				readTillCommentEnd();
				szFileLine = temp;
			}
			if(szFileLine == "")
			{
				continue;
			}
			if(szFileLine->StartsWith("/*"))
			{
				readTillCommentEnd();
				continue;
			}
			else if(szFileLine->StartsWith("#") || szFileLine->StartsWith("using") || szFileLine->StartsWith("namespace") || szFileLine->StartsWith("//"))
			{
				while(szFileLine->EndsWith("\\"))
					szFileLine = hFileReader->ReadLine();
				continue;
			}
			else if(szFileLine->StartsWith("typedef") || szFileLine->StartsWith("struct") || szFileLine->StartsWith("DECLARE_") || szFileLine->StartsWith("DEFINE_"))
			{
				readTillCodeBlockEnd();
				continue;
			}
			else if(szFileLine->StartsWith("class "))
			{
				parseClass();
				continue;
			}
			//no punctuation, probably a InterceptedFunction Modifier or a part of some function definition
			else if(!(szFileLine->IndexOfAny(braces) != -1))
			{
				szFunction = String::Concat(szFunction, " ", szFileLine->Trim());
			}
			
			if(szFunction->Length > 0)
			{
				szFunction->Trim();
				splitString =(szFunction->Split(space));
				if((splitString->Count >= 3) &&(szFunction->IndexOf("=") == -1) &&(szFunction->IndexOf("::") == -1))
				{
					if((szFunction->IndexOf("(") != -1 && szFunction->IndexOf(")") != -1))
					{
						parseFunction();
					}
				}
				else if(szFunction->EndsWith(";"))
				{
					szFunction = "";
					continue;
				}
			}
		}
		return funcList;
	}

	//*************************************************************************
	// Method:		parseClass
	// Description: parses functions from inside a class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HFileParser::parseClass()
	{
		int BlockOpenCount = 0;

		//read till class's main { is found
		while(szFileLine->IndexOf("{") == -1)
			szFileLine = hFileReader->ReadLine();

		BlockOpenCount++;
		int bracePos = szFileLine->IndexOf("{");

		if(szFileLine->Length > bracePos+1)
			szFileLine = szFileLine->Substring(bracePos+1, szFileLine->Length - bracePos - 1);
		else 
			szFileLine = hFileReader->ReadLine();

		Char separators[] = {':'};
		Char space[] = {' '};
		while(BlockOpenCount != 0)
		{
			szFileLine = szFileLine->Trim();

			if(szFileLine->IndexOf("//") != -1)
			{
				szFileLine = szFileLine->Substring(0, szFileLine->IndexOf("//"));
				szFileLine = szFileLine->Trim();
			}
			if(szFileLine->IndexOf("/*") != -1)
			{
				String *temp;
				temp = szFileLine->Substring(0, szFileLine->IndexOf("/*"))->Trim();
				readTillCommentEnd();
				szFileLine = temp;
			}
			if(szFileLine->Equals(""))
			{
				BlockOpenCount = readNextLine(szFileLine, hFileReader, BlockOpenCount);
				szFileLine = hFileReader->ReadLine();
				continue;
			}
			if(szFileLine->StartsWith("/*"))
			{
				readTillCommentEnd();
				BlockOpenCount = readNextLine(szFileLine, hFileReader, BlockOpenCount);
				szFileLine = hFileReader->ReadLine();
				continue;
			}
			else if(szFileLine->StartsWith("#") || szFileLine->StartsWith("using") || szFileLine->StartsWith("namespace") || szFileLine->StartsWith("//"))
			{
				while(szFileLine->EndsWith("\\"))
				szFileLine = hFileReader->ReadLine();
				BlockOpenCount = readNextLine(szFileLine, hFileReader, BlockOpenCount);
				szFileLine = hFileReader->ReadLine();
				continue;
			}
			//no punctuation, probably a InterceptedFunction Modifier or a part of some function definition
			else if(!(szFileLine->IndexOfAny(separators) != -1) ||(szFileLine->IndexOf("(") != -1) &&(szFileLine->IndexOf(")") != -1)&&(szFileLine->IndexOf("::") == -1))
			{
				szFunction = String::Concat(szFunction, " ", szFileLine->Trim());
			}

			if(szFunction->Length > 0)
			{
				szFunction = szFunction->Trim();
				if((szFunction->IndexOf("(") != -1) &&(szFunction->IndexOf(")") != -1)&&(szFunction->IndexOf("::") == -1))
				{
					int parenthPos = szFunction->IndexOf("(");
					String *temp = szFunction->Substring(0, parenthPos);
					String *temp2[] = temp->Split(space);
					ArrayList *actualTokens = getValidTokens(temp2);

					if(actualTokens->Count > 1)
					{
						//checking for single line inline funcs
						if(szFunction->IndexOf("{") != -1 && szFunction->IndexOf("}") != -1)
							szFunction = String::Concat(szFunction->Substring(0, szFunction->IndexOf(")")+1), ";");							
						else
							szFunction = szFunction->Substring(0, szFunction->IndexOf(")")+1);
						parseFunction();
					}
					else
					szFunction = "";
				}
				else if(szFunction->IndexOf(";") != -1 || szFunction->IndexOf("}") != -1 && szFunction->IndexOf("(") == -1)
					szFunction = "";
			}

			BlockOpenCount = readNextLine(szFileLine, hFileReader, BlockOpenCount);
			szFileLine = hFileReader->ReadLine();
		}
		//now read till class ending ';'
		while(szFileLine && szFileLine->IndexOf(";") == -1)
		{
			szFileLine = hFileReader->ReadLine();
		}

		return;
	}

	//*************************************************************************
	// Method:		readNextLine
	// Description: keeps count of number of code blocks opened/closed in current line
	//				and reads in the next line.
	//
	// Parameters:
	//	szFileLine - current line read from file
	//	hFileReader - streamreader for h file
	//  BlockOpenCount - current number of open code blocks 
	//
	// Return Value: updated number of open code blocks
	//*************************************************************************
	int HFileParser::readNextLine(String *szFileLine, StreamReader *hFileReader, int BlockOpenCount)
	{
		int BlockBeginPos = -1;
		int BlockEndPos = -1;

		if(szFileLine->IndexOf("{") != -1)
		{
			while((BlockBeginPos < szFileLine->Length) &&(szFileLine->IndexOf("{", ++BlockBeginPos) != -1))
			{
				BlockOpenCount++;
				BlockBeginPos = szFileLine->IndexOf("{", BlockBeginPos);
			}
		}
		if(szFileLine->IndexOf("}") != -1)
		{
			while((BlockEndPos < szFileLine->Length) &&(szFileLine->IndexOf("}", ++BlockEndPos) != -1))
			{
				BlockOpenCount--;
				BlockEndPos = szFileLine->IndexOf("}", BlockEndPos);
			}
		}
		return BlockOpenCount;
	}

	//*************************************************************************
	// Method:		readTillCodeBlockEnd
	// Description: advances the file reader to the end of the current code block
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::readTillCodeBlockEnd()
	{
		bool isTypedef = false;
		String *szBlockBegin = "";
		String *szBlockEnd = "";
		int BlockOpenCount = 0;
		bool blockBeginFound = false;

		if(szFileLine->StartsWith("typedef") || szFileLine->StartsWith("struct") || szFileLine->StartsWith("DECLARE_") || szFileLine->StartsWith("DEFINE_"))
			isTypedef = true;

		while(!blockBeginFound)
		{
			if(szFileLine->IndexOf("{") != -1)
			{
				szBlockBegin = "{";
				szBlockEnd = "}";
				blockBeginFound = true;
				continue;
			}
			else if(szFileLine->IndexOf("(") != -1)
			{
				szBlockBegin = "(";
				szBlockEnd = ")";
				blockBeginFound = true;
				continue;
			}
			else if(szFileLine->IndexOf(";") != -1)
			{					
				return;
			}
			szFileLine = hFileReader->ReadLine();
		}

		BlockOpenCount++;
		int BlockBeginPos = szFileLine->IndexOf(szBlockBegin);
		int BlockEndPos = -1;
		while(BlockOpenCount > 0)
		{
			while((BlockBeginPos < szFileLine->Length) &&(szFileLine->IndexOf(szBlockBegin, ++BlockBeginPos) != -1))
			{
				BlockOpenCount++;
				BlockBeginPos = szFileLine->IndexOf(szBlockBegin, BlockBeginPos);
			}
			while((BlockEndPos < szFileLine->Length) &&(szFileLine->IndexOf(szBlockEnd, ++BlockEndPos) != -1))
			{
				BlockOpenCount--;
				BlockEndPos = szFileLine->IndexOf(szBlockEnd, BlockEndPos);
			}
			if(BlockOpenCount != 0)
			{
				szFileLine = hFileReader->ReadLine();
			}
			// skipping inline functions, they dont end with ';' 
			else if(isTypedef)
			{
				while(szFileLine->IndexOf(";") == -1)
				{
					szFileLine = hFileReader->ReadLine();
				}
				return;
			}			
					
			BlockBeginPos = -1;
			BlockEndPos = -1;
		}
	}

	//*************************************************************************
	// Method:		readTillCommentEnd
	// Description: advances the file reader to the end of a multiline comment
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::readTillCommentEnd()
	{
		while(szFileLine->IndexOf("*/") == -1)
		{
			szFileLine = hFileReader->ReadLine();
		}
	}

	//*************************************************************************
	// Method:		parseFunction
	// Description: puts the function signature in one string
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::parseFunction()
	{
		while(true)
		{
			if(szFileLine->EndsWith(";") || szFunction->EndsWith(";")) 
			{
				break;					
			}
			else if(szFileLine->IndexOf("{") != -1)
			{
				int iBraceIndex = szFileLine->IndexOf("{");
				szFunction = String::Concat(szFunction, " ", szFileLine->Substring(0, iBraceIndex));
				readTillCodeBlockEnd();
				parseFunctionLine();
				return;
			}
			szFileLine = hFileReader->ReadLine();
		}
		parseFunctionLine();
		return;
	}

	//*************************************************************************
	// Method:		parseFunctionLine
	// Description: parse the single function string into a function object
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::parseFunctionLine()
	{
		szFunction = szFunction->Trim();

		int parenthPos = szFunction->LastIndexOf("(");
		// Catch wrongly parsed funcs
		// make sure function line has "(", and make sure not more than one "(" is there
		//if(parenthPos == -1 ||(szFunction->IndexOf("(", parenthPos+1) != -1) ||(szFunction->IndexOf("__delegate") != -1) ||(szFunction->IndexOf("__property") != -1) ||(szFunction->IndexOf(":") != -1)||(szFunction->IndexOf(" struct ") != -1 && szFunction->IndexOf(" struct ") < szFunction->IndexOf("(")) )
		if(parenthPos == -1 ||(szFunction->IndexOf("__delegate") != -1) ||(szFunction->IndexOf("__property") != -1) ||(szFunction->IndexOf(":") != -1)||(szFunction->IndexOf(" struct ") != -1 && szFunction->IndexOf(" struct ") < szFunction->IndexOf("(")) )
		{
			szFunction = "";
			return;
		}
		String *szFunc = szFunction->Substring(0, parenthPos)->Trim();
		String *szFuncParams = szFunction->Substring(parenthPos + 1, szFunction->LastIndexOf(")") - parenthPos-1)->Trim();

		currentFunc = new InterceptedFunction();
		parseFuncModifiers(szFunc);
		parseFuncParams(szFuncParams);
		generateHelplink();
		NumFuncs++;

		if (bFiltFunc)
		{
			if (currentFunc->get_Name() != 0 && (String::Compare(currentFunc->get_Name(), "") != 0))
			{
				if(selFuncsListContains(currentFunc->get_Name()))
				{
					funcList->Add (currentFunc);
				}
			}
		}
		else
			funcList->Add(currentFunc);
		
		szFunction = "";
	}

	//*************************************************************************
	// Method:		selFuncsListContains
	// Description: checks if the selected functions functionsList contains a function
	//				adds info from input file to the function object
	//
	// Parameters:
	//	szFuncName - name of function to check for
	//
	// Return Value: True if the function is one of the selected functions, false otherwise
	//*************************************************************************	
	bool HFileParser::selFuncsListContains(String *szFuncName)
	{
		for (int i = 0; i < selFuncs->Count; i ++)
		{
			InterceptedFunction *func = dynamic_cast <InterceptedFunction *>(selFuncs->get_Item(i));
			if (szFuncName->Equals(func->get_Name()))
			{
				if(func->get_HelpLink() != 0 && !(func->get_HelpLink()->Equals("")))
                    currentFunc->set_HelpLink(func->get_HelpLink());
				if(func->get_OriginalDll() != 0 && !(func->get_OriginalDll()->Equals("")))
					currentFunc->set_OriginalDll(func->get_OriginalDll());
				if(func->get_InterceptedDll() != 0 && !(func->get_InterceptedDll()->Equals("")))
                    currentFunc->set_InterceptedDll(func->get_InterceptedDll());
				if(func->get_Category() != 0 && !(func->get_Category()->Equals("")))
					currentFunc->set_Category(func->get_Category());

				for (int j = 0; j < func->get_TypeModifiers()->GetLength(0); j++)
				{
					String *mod = dynamic_cast <String *> (func->get_Modifiers()->GetValue(j));
					currentFunc->AddTypeModifier(mod);
				}

				for (int j = 0; j < func->get_CallingConventionModifiers()->GetLength(0); j++)
				{
					String *mod = dynamic_cast <String *> (func->get_Modifiers()->GetValue(j));
					currentFunc->AddCallingConventionModifier(mod);
				}

				for (unsigned int j = 0; j < func->get_ReturnValueCount(); j++)
				{
					InterceptedFunctionReturnValue *retVal = dynamic_cast <InterceptedFunctionReturnValue *> (func->get_ReturnValues()->GetValue((int)j));
					currentFunc->AddReturnValue(retVal);
				}
				return true;
			}
		}
		return false;
	}

	//*************************************************************************
	// Method:		parseFuncModifiers
	// Description: parse function modifiers and name into a function object
	//
	// Parameters:
	//	szFuncMods - part of the function signature before (
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::parseFuncModifiers(String *szFuncMods)
	{
		Char space[] = {' '};
		bool foundRetVal = false;

		String *szFunc[] = szFuncMods->Split(space);
		ArrayList *actualTokens = getValidTokens(szFunc);
				
		//parse and add * to the right place
		Char asterisk[] = {'*'};
		String *szAsterisk = "*";
		for(int i = 0; i < actualTokens->Count; i++)
		{
			// if * is independent token, add to previous token, can only be the return value
			if(String::Compare(actualTokens->get_Item(i)->ToString(), szAsterisk) == 0)
			{
				actualTokens->set_Item(i-1, String::Concat(actualTokens->get_Item(i-1)->ToString(), "*"));
				currentFunc->set_ReturnType((actualTokens->get_Item(i-1)->ToString()));
				actualTokens->RemoveAt(i-1);
				actualTokens->Remove(szAsterisk);
				foundRetVal = true;
				break;
			}
			// if token starts with *, wrongly parsed, remove and add to pervious token
			else if(actualTokens->get_Item(i)->ToString()->StartsWith(szAsterisk))
			{
				actualTokens->set_Item(i, actualTokens->get_Item(i)->ToString()->TrimStart(asterisk));
				actualTokens->set_Item(i-1, String::Concat(actualTokens->get_Item(i-1)->ToString(), "*"));
				currentFunc->set_ReturnType((actualTokens->get_Item(i-1)->ToString()));
				actualTokens->RemoveAt(i-1);
				foundRetVal = true;
				break;
			}
			// if token end with *, this token is the return value
			else if(actualTokens->get_Item(i)->ToString()->EndsWith(szAsterisk))
			{
				currentFunc->set_ReturnType(actualTokens->get_Item(i)->ToString());
				actualTokens->RemoveAt(i);
				foundRetVal = true;
				break;
			}
		}

		//check for 'unsigned', can only be applicable to the return value
		String *szUnsigned = "unsigned";
		if(actualTokens->Contains(szUnsigned))
		{
			if(foundRetVal)
			{
				currentFunc->set_ReturnType(String::Concat("unsigned ", currentFunc->get_ReturnType()));
				actualTokens->Remove(szUnsigned);
			}
			else
			{
				int index = actualTokens->IndexOf(szUnsigned);
				currentFunc->set_ReturnType(String::Concat("unsigned ", actualTokens->get_Item(index+1)->ToString()));
				foundRetVal = true;
				actualTokens->RemoveAt(index);
				actualTokens->RemoveAt(index);
			}
		}

		//get function name, last token
		currentFunc->set_Name(actualTokens->get_Item((actualTokens->Count)-1)->ToString());
		actualTokens->RemoveAt(actualTokens->Count-1);

		parseFuncKeywords(actualTokens);
		
		switch(actualTokens->Count)
		{
			case 3:
				if(foundRetVal)
				{
					currentFunc->AddCallingConventionModifier(actualTokens->get_Item(2)->ToString());
					currentFunc->AddTypeModifier(actualTokens->get_Item(1)->ToString());
					currentFunc->AddTypeModifier(actualTokens->get_Item(0)->ToString());
				}
				else
				{
					currentFunc->AddCallingConventionModifier(actualTokens->get_Item(2)->ToString());
					currentFunc->AddTypeModifier(actualTokens->get_Item(0)->ToString());
					currentFunc->set_ReturnType((actualTokens->get_Item(1))->ToString());
				}
				break;

			case 2:
				if(foundRetVal)
				{
					currentFunc->AddCallingConventionModifier(actualTokens->get_Item(1)->ToString());
					currentFunc->AddTypeModifier(actualTokens->get_Item(0)->ToString());
				}
				else
				{
					currentFunc->AddCallingConventionModifier(actualTokens->get_Item(1)->ToString());
					currentFunc->set_ReturnType((actualTokens->get_Item(0))->ToString());
				}
				break;

			case 1:
				if(foundRetVal)
					currentFunc->AddCallingConventionModifier(actualTokens->get_Item(0)->ToString());
				else
					currentFunc->set_ReturnType((actualTokens->get_Item(0))->ToString());
				break;
		}
		return;
	}

	//*************************************************************************
	// Method:		parseFuncKeywords
	// Description: parse keywords and add to correct modifier list
	//
	// Parameters:
	//	funcModTokens - part of function line before first (
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::parseFuncKeywords(ArrayList *funcModTokens)
	{
		String *szPublic = "public";
		String *szPrivate = "private";
		String *szProtected = "protected";
		String *szStatic = "static";
		String *szVirtual = "virtual";
		String *szConst = "const";

		if(funcModTokens->Contains(szPublic))
		{
			currentFunc->AddTypeModifier(szPublic);
			funcModTokens->Remove(szPublic);
		}

		if(funcModTokens->Contains(szPrivate))
		{
			currentFunc->AddTypeModifier(szPrivate);
			funcModTokens->Remove(szPrivate);
		}

		if(funcModTokens->Contains(szProtected))
		{
			currentFunc->AddTypeModifier(szProtected);
			funcModTokens->Remove(szProtected);
		}

		if(funcModTokens->Contains(szStatic))
		{
			currentFunc->AddTypeModifier(szStatic);
			funcModTokens->Remove(szStatic);
		}

		if(funcModTokens->Contains(szVirtual))
		{
			currentFunc->AddTypeModifier(szVirtual);
			funcModTokens->Remove(szVirtual);
		}

		if(funcModTokens->Contains(szConst))
		{
			currentFunc->AddTypeModifier(szConst);
			funcModTokens->Remove(szConst);
		}
		return;
	}
	//*************************************************************************
	// Method:		parseFuncParams
	// Description: parse function parameters
	//
	// Parameters:
	//	szFuncParams - part of the function signature after (
	//
	// Return Value: None
	//*************************************************************************	
	void HFileParser::parseFuncParams(String *szFuncParams)
	{
		Char space[] = {' '};
		Char comma[] = {','};
		String *szParams[] = szFuncParams->Split(comma);

		//cant call getValidTokens, checking for void params also in this loop
		ArrayList *actualTokens = new ArrayList();
		int numTokens = 0;
		for(int i = 0; i < szParams->Length; i++)
		{
			if(String::Compare(szParams[i], "") != 0 && String::Compare(szParams[i]->Trim()->ToUpper(), "VOID") !=0)
			{
				actualTokens->Add(szParams->get_Item(i));
				numTokens++;
			}
		}
		actualTokens->TrimToSize();

		// loop to go thru all params
		for(int prmNum = 0; prmNum < actualTokens->Count; prmNum++)
		{
			InterceptedFunctionParameter *param = parseParamString(actualTokens->get_Item(prmNum)->ToString(), prmNum);
			currentFunc->AddParameter(param);	
		}
		return;
	}

	//*************************************************************************
	// Method:		generateHelplink
	// Description: generates helplink for windows functions based on category
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	void HFileParser::generateHelplink()
	{
		helplink = "";

		if(currentFunc->get_Name()->EndsWith("A"))
			functionName = currentFunc->get_Name()->Substring(0,currentFunc->get_Name()->LastIndexOf("A"));
		else if(currentFunc->get_Name()->EndsWith("W"))
			functionName = currentFunc->get_Name()->Substring(0,currentFunc->get_Name()->LastIndexOf("W"));
		else
			functionName = currentFunc->get_Name();
		
		if(currentFunc->get_Category() != 0 && (String::Compare(currentFunc->get_Category(), "") != 0))
		{
			helplink = "http://msdn.microsoft.com/library/default.asp?url=/library/en-us/";
			if(String::Compare(currentFunc->get_Category(),"FILE") == 0)
				helplink = String::Concat(helplink,"fileio/base/",functionName,".asp");
			else if(String::Compare(currentFunc->get_Category(),"MEMORY") == 0)
				helplink = String::Concat(helplink,"memory/base/",functionName,".asp");
			else if(String::Compare(currentFunc->get_Category(),"PROCESS") == 0 ||String::Compare(currentFunc->get_Category(),"LIBRARY") == 0 ||String::Compare(currentFunc->get_Category(),"MISCELLANEOUS") == 0 )
				helplink = String::Concat(helplink,"dllproc/base/",functionName,".asp");
			else if(String::Compare(currentFunc->get_Category(),"NETWORK") == 0 && String::Compare(currentFunc->get_OriginalDll(),"ws2_32.dll") != 0)
				helplink = String::Concat(helplink,"winsock/winsock/",functionName,".asp");
			else if(String::Compare(currentFunc->get_Category(),"NETWORK") == 0 && String::Compare(currentFunc->get_OriginalDll(),"ws2_32.dll") == 0)
				helplink = String::Concat(helplink,"winsock/winsock/",functionName,"_2.asp");
			else if(String::Compare(currentFunc->get_Category(),"REGISTRY") == 0)
				helplink = String::Concat(helplink,"sysinfo/base/",functionName,".asp");


			currentFunc->set_HelpLink(helplink);
		}
	}

	//*************************************************************************
	// Method:		parseParamString
	// Description: parse the paramstring into a InterceptedFunctionParameter object
	//
	// Parameters:
	//	szPrmString - one individual parameter in one string
	//
	// Return Value: InterceptedFunctionParameter object for the parameter 
	//*************************************************************************
	InterceptedFunctionParameter *HFileParser::parseParamString(String *szPrmString, int paramIndex)
	{
		currentParam = new InterceptedFunctionParameter();

		Char space[] = {' '};		
		String *szPrmProps[] = szPrmString->Split(space);
		ArrayList *actualTokens = getValidTokens(szPrmProps);

		setParamAccess(actualTokens);
		
		setParamPtrsAndRefs(actualTokens);		

		bool foundPrmType = setParamModifiers(actualTokens);

		String *prmType = "";

		switch (actualTokens->Count)
		{
			case 3:
				currentParam->set_Name (actualTokens->get_Item(2)->ToString());
				prmType = String::Concat(actualTokens->get_Item(0)->ToString(), " ", actualTokens->get_Item(1)->ToString());
				currentParam->set_Type (String::Concat (prmType, " ", currentParam->get_Type()));
				break;

			case 2:
				currentParam->set_Name (actualTokens->get_Item(1)->ToString());
				currentParam->set_Type (String::Concat (actualTokens->get_Item(0)->ToString(), " ", currentParam->get_Type()));
				break;

			case 1:
				if(foundPrmType)
					currentParam->set_Name (actualTokens->get_Item(0)->ToString());
				else
					currentParam->set_Type (String::Concat (actualTokens->get_Item(0)->ToString(), " ", currentParam->get_Type()));
				break;
		}

		if(currentParam->Type == 0)
			currentParam->Type = "";
		currentParam->set_Type(currentParam->get_Type()->Trim());

		if (currentParam->Name == 0 || String::Compare(currentParam->Name, "") == 0)
			currentParam->Name = String::Concat("param", __box(paramIndex)->ToString());

		setParamCompareAs();

		return currentParam;
	}

	//*************************************************************************
	// Method:		setParamAccess
	// Description: parses the param string line for access tokens and sets in currentParam
	//
	// Parameters:
	//	paramStringTokens - ArrayList containing string tokens
	//
	// Return Value: None 
	//*************************************************************************
	void HFileParser::setParamAccess(ArrayList *paramStringTokens)
	{
		String *szIn = "IN";
		String *szOut = "OUT";
		String *szOptional = "OPTIONAL";

		if(paramStringTokens->Contains(szIn))
		{
			currentParam->Access = "IN";
			paramStringTokens->Remove(szIn);
		}

		if(paramStringTokens->Contains(szOut))
		{
			if(currentParam->Access != 0 && (currentParam->Access->Length) > 0)
				currentParam->Access = String::Concat(currentParam->Access->ToString(), " OUT");
			else
				currentParam->Access = "OUT";
			paramStringTokens->Remove(szOut);
		}

		if(paramStringTokens->Contains(szOptional))
		{
			if(currentParam->Access != 0 && (currentParam->Access->Length) > 0)
				currentParam->Access = String::Concat(currentParam->Access->ToString(), " OPTIONAL");
			else
				currentParam->Access = "OPTIONAL";
			paramStringTokens->Remove(szOptional);
		}

		paramStringTokens->TrimToSize();

		if (currentParam->Access == 0)
			currentParam->Access = "NONE";
		return;
	}

	//*************************************************************************
	// Method:		setParamPtrsAndRefs
	// Description: parses the param string line for * and &, and adds to correct token
	//
	// Parameters:
	//	paramStringTokens - ArrayList containing string tokens
	//
	// Return Value: None 
	//*************************************************************************
	void HFileParser::setParamPtrsAndRefs(ArrayList *paramStringTokens)
	{
		String *twoAsterisks = "**";
		String *oneAsterisk = "*";
		String *andSign = "&";
		Char asterisk[] = {'*'};
		Char andSigns[] = {'&'};
		for(int i = 0; i < paramStringTokens->Count; i++)
		{
			if(String::Compare(paramStringTokens->get_Item(i)->ToString(), "**") == 0)
			{
				paramStringTokens->set_Item(i-1, String::Concat(paramStringTokens->get_Item(i-1)->ToString(), "**"));
				paramStringTokens->Remove(twoAsterisks);
				break;
			}
			else if(String::Compare(paramStringTokens->get_Item(i)->ToString(), "*") == 0)
			{
				paramStringTokens->set_Item(i-1, String::Concat(paramStringTokens->get_Item(i-1)->ToString(), "*"));
				paramStringTokens->Remove(oneAsterisk);
				break;
			}
			else if (paramStringTokens->get_Item(i)->ToString()->StartsWith("**"))
			{
				paramStringTokens->set_Item(i-1, String::Concat (paramStringTokens->get_Item(i-1)->ToString(), "**"));
				paramStringTokens->set_Item(i, paramStringTokens->get_Item(i)->ToString()->TrimStart(asterisk));
			}
			else if (paramStringTokens->get_Item(i)->ToString()->StartsWith("*"))
			{
				paramStringTokens->set_Item (i-1,String::Concat (paramStringTokens->get_Item(i-1)->ToString(), "*"));
				paramStringTokens->set_Item(i, paramStringTokens->get_Item(i)->ToString()->TrimStart(asterisk));
			}
					
			if (paramStringTokens->get_Item(i)->ToString()->Equals ("&"))
			{
				paramStringTokens->set_Item (i-1,String::Concat (paramStringTokens->get_Item(i-1)->ToString(), "&"));
				paramStringTokens->Remove (andSign);
				break;
			}
			else if (paramStringTokens->get_Item(i)->ToString()->StartsWith("&"))
			{
				paramStringTokens->set_Item (i-1,String::Concat (paramStringTokens->get_Item(i-1)->ToString(), "&"));
				paramStringTokens->set_Item(i, paramStringTokens->get_Item(i)->ToString()->TrimStart(andSigns));
			}
		}
		return;
	}

	//*************************************************************************
	// Method:		setParamModifiers
	// Description: parses the param string line for const, volatile etc.
	//
	// Parameters:
	//	paramStringTokens - ArrayList containing string tokens
	//
	// Return Value: None 
	//*************************************************************************
	bool HFileParser::setParamModifiers(ArrayList *paramStringTokens)
	{
		bool foundPrmType = false;
		String *szUnsigned = "unsigned";
		String *szVolatile = "volatile";
		String *szVolatileAsterisk = "volatile*";
		String *szConst = "const";
		String *szCapsConst = "CONST";

		if (paramStringTokens->IndexOf(szUnsigned) != -1)
		{
			currentParam->set_Type (String::Concat ("unsigned", paramStringTokens->get_Item(paramStringTokens->IndexOf(szUnsigned)+1)->ToString()));
			paramStringTokens->RemoveAt (paramStringTokens->IndexOf(szUnsigned)+1);
			paramStringTokens->Remove (szUnsigned);
			foundPrmType = true;
		}

		if (paramStringTokens->IndexOf(szVolatile) != -1)
		{
			currentParam->set_Type (String::Concat (currentParam->get_Type(), " volatile"));
			paramStringTokens->Remove (szVolatile);
		}
		else if(paramStringTokens->IndexOf(szVolatileAsterisk) != -1)
		{
			currentParam->set_Type (String::Concat (currentParam->get_Type(), " volatile*"));
			paramStringTokens->Remove (szVolatileAsterisk);
		}

		int constPos = -1;
		if ((constPos = paramStringTokens->IndexOf(szConst)) != -1)
		{
			if (constPos < paramStringTokens->Count - 1)
				paramStringTokens->set_Item ((constPos+1), String::Concat ("const ", paramStringTokens->get_Item(constPos+1)->ToString()));
			else
				paramStringTokens->set_Item ((constPos-1), String::Concat (paramStringTokens->get_Item(constPos-1)->ToString(), " const"));
			paramStringTokens->Remove (szConst);
		}

		constPos = -1;
		if ((constPos = paramStringTokens->IndexOf(szCapsConst)) != -1)
		{
			if (constPos < paramStringTokens->Count - 1)
				paramStringTokens->set_Item ((constPos+1), String::Concat ("CONST ", paramStringTokens->get_Item(constPos+1)->ToString()));
			else
				paramStringTokens->set_Item ((constPos-1), String::Concat (paramStringTokens->get_Item(constPos-1)->ToString(), " CONST"));
			paramStringTokens->Remove (szCapsConst);
		}
		return foundPrmType;
	}

	//*************************************************************************
	// Method:		setParamCompareAs
	// Description: set the param's CompareAs prop, default to PointerType
	//
	// Parameters:
	//	None
	//
	// Return Value: None 
	//*************************************************************************
	void HFileParser::setParamCompareAs()
	{
		currentParam->set_CompareAsType(PointerType);
				
		if (((currentParam->get_Type()->StartsWith("LP"))||(currentParam->get_Type()->StartsWith("P"))||(currentParam->get_Type()->EndsWith("_PTR"))||(currentParam->get_Type()->StartsWith("H"))||(currentParam->get_Type()->EndsWith("*"))||(currentParam->get_Type()->EndsWith("SOCKET"))))
		{
			currentParam->set_CompareAsType(PointerType);
		}
		if(currentParam->get_Type()->EndsWith("**"))
		{
			currentParam->set_CompareAsType(PointerPointerType);
		}
		if ((((currentParam->get_Type()->Equals("LPCSTR"))||(currentParam->get_Type()->Equals("LPSTR"))) && currentParam->get_Access()->Equals("IN")))
		{
			currentParam->set_CompareAsType(StringType);
		}
		if ((((currentParam->get_Type()->Equals("LPCWSTR"))||(currentParam->get_Type()->Equals("LPWSTR"))) && currentParam->get_Access()->Equals("IN")))
		{
			currentParam->set_CompareAsType(WideStringType);
		}
		if ((currentParam->get_Type()->Equals("DWORD"))||(currentParam->get_Type()->Equals("unsigned long"))||(currentParam->get_Type()->Equals("unsigned int"))||(currentParam->get_Type()->Equals("unsigned short"))||(currentParam->get_Type()->Equals("ULONG"))||(currentParam->get_Type()->Equals("SIZE_T"))||(currentParam->get_Type()->Equals("u_short")))
		{
			currentParam->set_CompareAsType(UnsignedLongType);
		}
		if ((currentParam->get_Type()->Equals("LONG"))||(currentParam->get_Type()->Equals("long"))||(currentParam->get_Type()->Equals("int"))||(currentParam->get_Type()->Equals("WORD"))||(currentParam->get_Type()->Equals("INT"))||(currentParam->get_Type()->Equals("long"))||(currentParam->get_Type()->Equals("UINT"))||(currentParam->get_Type()->Equals("ATOM")))
		{
			currentParam->set_CompareAsType(IntegerType);
		}
		if ((currentParam->get_Type()->Equals("LONGLONG"))||(currentParam->get_Type()->Equals("DWORDLONG")))
		{
			currentParam->set_CompareAsType(Integer64Type);
		}
		if ((currentParam->get_Type()->Equals("BOOL"))||(currentParam->get_Type()->Equals("BOOLEAN"))||(currentParam->get_Type()->Equals("bool"))||(currentParam->get_Type()->Equals("boolean")))
		{
			currentParam->set_CompareAsType(BooleanType);
		}
		if ((currentParam->get_Type()->Equals("CHAR"))||(currentParam->get_Type()->Equals("char")))
		{
			currentParam->set_CompareAsType(StringType);
		}
		if ((currentParam->get_Type()->Equals("WCHAR"))||(currentParam->get_Type()->Equals("wchar_t")))
		{
			currentParam->set_CompareAsType(WideStringType);
		}
		return;
	}

	//*************************************************************************
	// Method:		readSelectedFuncsList
	// Description: reads functionsList of functions to be parsed from input file
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing function objects in input file 
	//*************************************************************************
	ArrayList* HFileParser::readSelectedFuncsList()
	{
		int index = 0;
		InterceptedFunction* func = new InterceptedFunction();
		String *funcDetails = "";
		String *ValueOp = "";
		Char quote[] = {'\''} ;		
	
		if(!File::Exists(szFile) || !File::Exists(funcFilterFile))
			return 0;

		srSelFuncFile = File::OpenText(funcFilterFile);
		while((szInputFileLine = (srSelFuncFile->ReadLine())) != 0)
		{
			szInputFileLine = szInputFileLine->Trim();
			if(szInputFileLine->Equals(""))
				continue;
			if(szInputFileLine->IndexOf(S"</Function>") == -1) 
			{
				funcDetails = String::Concat(funcDetails,szInputFileLine);
				continue;
			}
			else
			{
				funcDetails = String::Concat(funcDetails,szInputFileLine);
				Char tags[] = {'<','>'};
				String *szInput[] = funcDetails->Split(tags);
				ArrayList *actualTokens = getValidTokens(szInput);

				for(int i = 0;i < actualTokens->Count; i++) 
				{
					String *thisToken = actualTokens->get_Item(i)->ToString();
					if(String::Compare(thisToken,"FunctionName") == 0)
						func->set_Name(actualTokens->get_Item(i+1)->ToString());

					else if(String::Compare(thisToken,"HelpLink") == 0) 
						func->set_HelpLink(actualTokens->get_Item(i+1)->ToString());

					else if(String::Compare(thisToken,"OriginalDll") == 0)
						func->set_OriginalDll(actualTokens->get_Item(i+1)->ToString());

					else if(String::Compare(thisToken,"InterceptedDll") == 0)
						func->set_InterceptedDll(actualTokens->get_Item(i+1)->ToString());

					else if(String::Compare(thisToken,"Category") == 0)
						func->set_Category(actualTokens->get_Item(i+1)->ToString());

					else if((thisToken->IndexOf("/ReturnValueSuccess") == -1 && thisToken->IndexOf("ReturnValueSuccess") != -1) || (thisToken->IndexOf("/ReturnValueError") == -1 && thisToken->IndexOf("ReturnValueError") != -1))
					{
						InterceptedFunctionReturnValue *retVal = new InterceptedFunctionReturnValue();

						if (thisToken->IndexOf("ReturnValueSuccess") != -1)
                                retVal->set_Type(ReturnValueType::Success);
						else if(thisToken->IndexOf("ReturnValueError") != -1)
							retVal->set_Type(ReturnValueType::Error);

						if(thisToken->IndexOf("ValueOperator") != -1)
						{
							index = thisToken->IndexOf("ValueOperator");
							ValueOp = thisToken->Substring(index,thisToken->Length - index);
							ValueOp = thisToken->Substring(thisToken->IndexOf("'"),thisToken->Length - thisToken->IndexOf("'"));
							ValueOp = ValueOp->Trim(quote);
							
							if (String::Compare(ValueOp, "EqualTo") == 0)
								retVal->set_OperatorType(ReturnValueOperatorType::EqualTo);
							else if (String::Compare(ValueOp, "GreaterThan") == 0)
								retVal->set_OperatorType(ReturnValueOperatorType::GreaterThan);
							else if (String::Compare(ValueOp, "GreaterThanOrEqualTo") == 0)
								retVal->set_OperatorType(ReturnValueOperatorType::GreaterThanOrEqualTo);
							else if (String::Compare(ValueOp, "LessThan") == 0)
								retVal->set_OperatorType(ReturnValueOperatorType::LessThan);
							else if (String::Compare(ValueOp, "LessThanOrEqualTo") == 0)
								retVal->set_OperatorType(ReturnValueOperatorType::LessThanOrEqualTo);
							else if (String::Compare(ValueOp, "NotEqualTo") == 0)
								retVal->set_OperatorType(ReturnValueOperatorType::NotEqualTo);

							retVal->set_ReturnValue(Convert::ToUInt32(actualTokens->get_Item(i+1)->ToString()));
						}

						func->AddReturnValue(retVal);
					}
					else if(String::Compare(thisToken,"ReturnType") == 0)
						func->set_ReturnType(actualTokens->get_Item(i+1)->ToString());
					else if(String::Compare(thisToken,"TypeModifier") == 0)
						func->AddTypeModifier(actualTokens->get_Item(i+1)->ToString());
					else if(String::Compare(thisToken,"CallingConventionModifier") == 0)
						func->AddCallingConventionModifier(actualTokens->get_Item(i+1)->ToString());
					else if(String::Compare(thisToken,"Category") == 0)
						func->set_Category(actualTokens->get_Item(i+1)->ToString());					

				}
				if(funcDetails->IndexOf("<HelpLink>") == -1)
				{
					if(func->get_Name()->EndsWith("A"))
						functionName = func->get_Name()->Substring(0,func->get_Name()->LastIndexOf("A"));
					else if(func->get_Name()->EndsWith("W"))
						functionName = func->get_Name()->Substring(0,func->get_Name()->LastIndexOf("W"));
					else
						functionName = func->get_Name();

					if(func->get_Category() != 0 && (String::Compare(func->get_Category(), "") != 0))
					{
						
						helplink = "http://msdn.microsoft.com/library/default.asp?url=/library/en-us/";
						if(String::Compare(func->get_Category(),"FILE") == 0)
							helplink = String::Concat(helplink,"fileio/base/",functionName,".asp");
						else if(String::Compare(func->get_Category(),"MEMORY") == 0)
							helplink = String::Concat(helplink,"memory/base/",functionName,".asp");
						else if(String::Compare(func->get_Category(),"PROCESS") == 0 || String::Compare(func->get_Category(),"LIBRARY") == 0 ||String::Compare(func->get_Category(),"MISCELLANEOUS") == 0 )
							helplink = String::Concat(helplink,"dllproc/base/",functionName,".asp");
						else if(String::Compare(func->get_Category(),"NETWORK") == 0 && String::Compare(func->get_OriginalDll(),"ws2_32.dll") != 0)
							helplink = String::Concat(helplink,"winsock/winsock/",functionName,".asp");
						else if(String::Compare(func->get_Category(),"NETWORK") == 0 && String::Compare(func->get_OriginalDll(),"ws2_32.dll") == 0)
							helplink = String::Concat(helplink,"winsock/winsock/",functionName,"_2.asp");
						else if(String::Compare(func->get_Category(),"REGISTRY") == 0)
							helplink = String::Concat(helplink, "sysinfo/base/", functionName, ".asp");

						func->set_HelpLink(helplink);
					}
				}
				selFuncs->Add(func);
				func = new InterceptedFunction();
				funcDetails = "";
				index = 0;
				ValueOp = "";
			}
		}
		return selFuncs;
	}

	//*************************************************************************
	// Method:		getValidTokens
	// Description: String.Split sometimes returns "" as tokens, removes those
	//
	// Parameters:
	//	tokens - return value from a String.Split() call
	//
	// Return Value: ArrayList containing valid tokens 
	//*************************************************************************
	ArrayList *HFileParser::getValidTokens(String *tokens[])
	{
		ArrayList *actualTokens = new ArrayList();
		int numTokens = 0;

		for(int i = 0; i < tokens->Length; i++)		
		{
			if(String::Compare(tokens[i], "") != 0)
			{
				actualTokens->Add(tokens[i]->Trim());
				numTokens++;
			}
		}
		actualTokens->TrimToSize();
		return actualTokens;
	}
}




