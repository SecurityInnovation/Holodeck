//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		XmlGenerator.h
//
// DESCRIPTION: Contains definition for the class XmlGenerator
//
//=========================================================================
// Modification History
//
// Date         SCR  Name							Purpose
// -----------  ---  -----------					-----------------------
// 05 Oct 2003		 P.Singh, P.Patel, M.Pancholi	File created.
//*************************************************************************

#pragma once

#include "HFileParser.h"
#include "defines.h"
#include "ParameterType.h"

#using <functiondatabase.dll>
#using <mscorlib.dll>
#using <System.dll>
#using <System.Xml.dll>
#using <GuiSupport.dll>

using namespace FunctionDatabase;
using namespace System;
using namespace System::IO;
using namespace System::Collections;
using namespace System::Xml;
using namespace System::Xml::XPath;

namespace HeaderParser
{
	//*********************************************************************
	// Adds functions to functions.xml 
	//*********************************************************************
	public __gc class XmlGenerator
	{
		public:
			XmlGenerator(ArrayList *funcList, String *xmlFileName, bool appendFile);
			XmlGenerator(String *subsetXMLFile, String * destinationPath);
			~XmlGenerator();
			void writeFunctionsXml();
			
		private:
			bool generateNewFile;	//true if generating new xml file instead of adding to Holodeck Database
			XmlDocument *xmlFile;	//xml document loaded from functions.xml
			String *xmlFilePath;	//path to functions.xml
			ArrayList *functionsList;		//functionsList of function objects to be added
			void loadXmlDocument();
			XmlNode *createFunctionNode(InterceptedFunction *function);
			void setElementValue (XmlNode *parentNode, XmlElement *element, String *value);
			void setAttributeValue (XmlElement *parentElement, String *attrName, String *value);
			XmlElement *createRetValNode(InterceptedFunctionReturnValue *retVal);
			XmlElement *createParamNode(InterceptedFunctionParameter *paramVal, int indexNum);
			XmlNode *replaceFunction (XmlNode *bookmarkNode, InterceptedFunction *function);
			void replaceNode (XmlNode *newNode, XmlNode *oldNode, String *elementName, String *value);
			void replaceList (XmlNodeList *list, XmlNode *rNode, XmlNode *cNode, int newListCount, XmlNode *oldItem, String *newValue, String *elementName);
			void replaceRetValList (XmlNodeList *list, XmlNode *rNode, int newListCount, XmlNode *oldItem, InterceptedFunctionReturnValue *newItem);
			void replaceRetValAttr (XmlAttributeCollection *attrList, InterceptedFunctionReturnValue *newItem, int Index);
			void replaceParamValList (XmlNodeList *list, XmlNode *rNode, int newListCount, XmlNode *oldItem, InterceptedFunctionParameter *newItem, int Index);
			void replaceParamValAttr (XmlAttributeCollection *attrList, InterceptedFunctionParameter *newItem, int Index, int paramNumber);
	};
}
