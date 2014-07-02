//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HFileParser.h
//
// DESCRIPTION: Contains definition for the class HFileParser
//
//=========================================================================
// Modification History
//
// Date         SCR  Name							Purpose
// -----------  ---  -----------					-----------------------
// 05 Oct 2003		 P.Singh, P.Patel, A.Jauhari	File created.
//*************************************************************************

#pragma once

#include "ParameterType.h"

#using <mscorlib.dll>
#using <System.dll>
#using <functiondatabase.dll>

using namespace FunctionDatabase;
using namespace System;
using namespace System::IO;
using namespace System::Collections;

namespace HeaderParser
{
    //*********************************************************************
	// Parses .h files for functions to be intercepted
	//*********************************************************************
	public __gc class HFileParser
	{
		public:
			HFileParser(String *szFilePath);
			HFileParser(String *szFilePath, String *funcFilterFilePath);
			HFileParser(String *szFilePath, ArrayList *funcsToParse);
			~HFileParser();
			ArrayList *parseFile();

		private:
			void parseClass();
			int readNextLine(String *szFileLine, StreamReader *hFileReader, int BlockOpenCount);
			void readTillCodeBlockEnd();
			void readTillCommentEnd();
			void parseFunction();
			void parseFunctionLine();
			bool selFuncsListContains(String *szFuncName);
			void parseFuncModifiers(String *szFuncMods);
			void parseFuncKeywords(ArrayList *funcModTokens);
			void parseFuncParams(String *szFuncParams);
			void generateHelplink();
			InterceptedFunctionParameter *parseParamString(String *szPrmString, int paramIndex);
			void setParamAccess(ArrayList *paramStringTokens);
			void setParamPtrsAndRefs(ArrayList *paramStringTokens);
			bool setParamModifiers(ArrayList *paramStringTokens);
			void setParamCompareAs();
			ArrayList *readSelectedFuncsList();			
			ArrayList *getValidTokens(String *tokens[]);

			String *szFile;					// path to the .h file
			StreamReader *hFileReader;				// streamreader for reading .h file
			InterceptedFunction *currentFunc;			// temp storage for the function being parsed
			InterceptedFunctionParameter *currentParam;  // temp storage for the param being parsed
			String *szFileLine;				// line read from file
			String *szFunction;				// all the function info needed is concatenated to make this string
			String *splitString[];			// contains tokenized value of various strings 
			int NumFuncs;					// number of functions parsed
			ArrayList *funcList;			// List of InterceptedFunction objects returned by the parser
			String *funcFilterFile;			// path to the file containing functions to be parsed
			ArrayList *selFuncs;			// List of functions to be parsed
			StreamReader *srSelFuncFile;	// stream reader for reading file with selected functions
			String *szInputFileLine;		// temp storage for storing lines read from funcFilterFile			
			bool bFiltFunc;					// true if there is functionsList of functions to be parsed
			String *helplink;				// helplink
		    String *functionName;			// function name
	};

}
