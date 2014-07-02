//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		XmlGenerator.cpp
//
// DESCRIPTION: Contains implementation for the class XmlGenerator
//
//=========================================================================
// Modification History
//
// Date         SCR  Name							Purpose
// -----------  ---  -----------					-----------------------
// 05 Oct 2003		 P.Singh, P.Patel, M.Pancholi	File created.
//*************************************************************************

#include "xmlgenerator.h"

namespace HeaderParser
{
	//*************************************************************************
	// Method:		XmlGenerator
	// Description: Constructor for the XmlGenerator class
	//
	// Parameters:
	//	funcList - ArrayList of function objects to be added to functions.xml
	//	newXmlFileName - filename/path for new xml file to be generated
	//
	// Return Value: None
	//*************************************************************************
	XmlGenerator::XmlGenerator(ArrayList *funcList, String *xmlFileName, bool appendFile)
	{
		functionsList = dynamic_cast <ArrayList *>(funcList->Clone());	
		xmlFilePath = xmlFileName;
		generateNewFile = !appendFile;
	}
	
	//*************************************************************************
	// Method:		XmlGenerator
	// Description: Constructor for the XmlGenerator class
	//
	// Parameters:
	//	subsetXMLFile - xml file containing functions to be added
	//	writeDotNetFuncsXml - true if adding dot net functions database file
	//
	// Return Value: None
	//*************************************************************************
	XmlGenerator::XmlGenerator(String *subsetXMLFile, String * destinationPath)
	{
		generateNewFile = false;
		xmlFilePath = destinationPath;	

		InterceptedFunctionDB *tempDB = InterceptedFunctionDB::GetInstance("");
		tempDB->LoadDatabase(0, subsetXMLFile);
		//add functions from database to functionsList
		functionsList = new ArrayList();
		for (int i = 0; i < tempDB->Functions->Count; i++)
			functionsList->Add(tempDB->Functions->GetValue(i));
	}

	//*************************************************************************
	// Method:		~XmlGenerator
	// Description: Destructor for the XmlGenerator class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	XmlGenerator::~XmlGenerator()
	{
	}
	
	//*************************************************************************
	// Method:		writeFunctionsXml
	// Description: adds/replaces functions' xml block in functions.xml
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::writeFunctionsXml()						 
	{
		// Load the Xml document into XML DOM
		loadXmlDocument();
		
		// This is the pointer to the end of XML file 
		XmlNode *lastNode = xmlFile->SelectSingleNode("/Functions/Function[last()]");	
		
		for(int i=0; i < functionsList->Count; i++)
		{
			InterceptedFunction *currFunc = dynamic_cast <InterceptedFunction *>(functionsList->get_Item(i));
			XmlNode *functionNode = createFunctionNode(currFunc);
			//check if function already exists in functions.xml, add/replace accordingly
			XmlNode *searchNode = xmlFile->SelectSingleNode(String::Concat("/Functions/Function[FunctionName=\"", currFunc->get_Name(), "\"]"));
			if(searchNode)
			{
				XmlNode *replaceThisNode = replaceFunction(searchNode, currFunc);
				searchNode->ParentNode->ReplaceChild(replaceThisNode, searchNode);
			}

			else if(lastNode)
			{	
				lastNode->ParentNode->AppendChild(functionNode);
			}

			else if (xmlFile->DocumentElement)
			{
				xmlFile->DocumentElement->AppendChild(functionNode);
				lastNode = xmlFile->SelectSingleNode("/Functions/Function[last()]");	
			}
		}
		xmlFile->Save(xmlFilePath);
	}

	//*************************************************************************
	// Method:		loadXmlDocument
	// Description: loads functions.xml into DOM
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::loadXmlDocument()
	{
		// A new file has to be created before it can be loaded into DOM
		// Creating a dummy file with just "<Functions />" in it
		if (generateNewFile)
		{
			//Delete file if it already exists
			if (File::Exists(xmlFilePath))
				File::Delete(xmlFilePath);

			// get the full path to functions.dtd
			String *dtdPath;
			String *functionDBFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();	
			if(functionDBFolder != 0)
			{
				if (functionDBFolder->EndsWith("\\"))
					dtdPath = String::Concat(functionDBFolder, "functions.dtd");
				else
					dtdPath = String::Concat(functionDBFolder, "\\", "functions.dtd");
			}
			else
				dtdPath = "functions.dtd";

			XmlTextWriter * xmlWriter = new XmlTextWriter (xmlFilePath, System::Text::Encoding::UTF8);
			
			xmlWriter->Formatting = Formatting::Indented;
			xmlWriter->Indentation = 4;
			xmlWriter->WriteStartDocument(false);
			xmlWriter->WriteDocType("Functions", 0, dtdPath, 0);
			xmlWriter->WriteStartElement("Functions");
			xmlWriter->WriteFullEndElement();
			xmlWriter->Close();
		}

		if((File::GetAttributes(xmlFilePath) & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
			File::SetAttributes(xmlFilePath, static_cast <FileAttributes>(File::GetAttributes(xmlFilePath) & ~FileAttributes::ReadOnly));

		xmlFile = new XmlDocument();
		xmlFile->Load(xmlFilePath);
	}
	
	//*************************************************************************
	// Method:		createFunctionNode
	// Description: creates function node for one function
	//
	// Parameters:
	//	function - function object containing info for a function
	//
	// Return Value: XmlNode for the function
	//*************************************************************************
	XmlNode *XmlGenerator::createFunctionNode(InterceptedFunction *function)
	{
		XmlNode *functionNode = xmlFile->CreateNode(XmlNodeType::Element, "Function", 0);
		
		XmlElement *FunctionNameElem = xmlFile->CreateElement("FunctionName");
		setElementValue(functionNode, FunctionNameElem, function->get_Name());

		XmlElement *FunctionOriginalDll = xmlFile->CreateElement("OriginalDll");
		setElementValue(functionNode, FunctionOriginalDll, function->get_OriginalDll());

		XmlElement *FunctionIntDll = xmlFile->CreateElement("InterceptedDll");
		setElementValue(functionNode, FunctionIntDll, function->get_InterceptedDll());

		XmlElement *FunctionReplFunc = xmlFile->CreateElement("ReplacementFunctionName");
		if (function->get_ReplacementFunction() == 0 || function->get_ReplacementFunction()->Equals(""))
            setElementValue(functionNode, FunctionReplFunc, String::Concat(function->get_Name(), "Replacement"));
		else
			setElementValue(functionNode, FunctionReplFunc, function->get_ReplacementFunction());

		XmlElement *FunctionRetType = xmlFile->CreateElement("ReturnType");
		setElementValue(functionNode, FunctionRetType, function->get_ReturnType());

		// create the set of modifiers and their values for the concerned function
		for(int i=0; i<function->get_TypeModifiers()->GetLength(0); i++)
		{
			XmlElement *FunctionModElem = xmlFile->CreateElement("TypeModifier");
			String *mod = dynamic_cast <String *>(function->get_TypeModifiers()->GetValue(i));
			setElementValue(functionNode, FunctionModElem, mod);
		}

		// create the set of "CallingConventionModifier" and their values for the concerned function
		for(int i=0; i < function->get_CallingConventionModifiers()->GetLength(0); i++)
		{
			XmlElement *FunctionCallingConventionMod = xmlFile->CreateElement("CallingConventionModifier");
			String *CallingConventionMod = dynamic_cast <String *>(function->get_CallingConventionModifiers()->GetValue(i));
			setElementValue(functionNode, FunctionCallingConventionMod, CallingConventionMod);
		}

		XmlElement *FunctionCategory = xmlFile->CreateElement("Category");
		setElementValue(functionNode, FunctionCategory, function->get_Category());

		for(int i = 0; i < function->get_ReturnValues()->GetLength(0); i++)
		{
			InterceptedFunctionReturnValue *retVal = dynamic_cast<InterceptedFunctionReturnValue *>(function->get_ReturnValues()->GetValue(i));
			if (retVal != 0)
			{
				XmlElement *FunctionRetVal = createRetValNode(retVal);
				functionNode->AppendChild(FunctionRetVal);
			}
		}

		XmlElement *FunctionHelpLink = xmlFile->CreateElement("HelpLink");
		setElementValue(functionNode, FunctionHelpLink, function->get_HelpLink());

		// create the set of parameters and their values(attributes) for this concerned function
		for(int i = 0; i < function->get_Parameters()->GetLength(0); i++)
		{
			InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *>(function->get_Parameters()->GetValue(i));
			if (prm != 0)
			{
				XmlElement *FunctionParamElem = createParamNode(prm, i);
				functionNode->AppendChild(FunctionParamElem);
			}
		}
		return functionNode;
	}

	//*************************************************************************
	// Method:		setElementValue
	// Description: sets the value for the XML Elements
	//
	// Parameters:
	//	parentNode - the parent node of the element to be added
	//  element - the element to be added
	//  value - the value of the element
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::setElementValue(XmlNode *parentNode, XmlElement *element, String *value)
	{
		if(value == 0 || (String::Compare(value, "") == 0))
			element->set_InnerXml("");
		else
		{
			value = value->Replace(S"&amp;", S"&");
			value = value->Replace(S"&", S"&amp;");
			element->set_InnerXml(value);
		}
		parentNode->AppendChild(element);
	}

	//*************************************************************************
	// Method:		setAttributeValue
	// Description: sets the values for the XML Attributes (for XML elements
	//              ReturnValue and Param)
	//
	// Parameters:
	//  parentElement - parent element of the attribute to be added
	//  attrName - name of the element (e.g. "ValueOperator" for XML element
	//             "ReturnValue")
	//  value - value of the attribute to be added
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::setAttributeValue(XmlElement *parentElement, String *attrName, String *value)
	{
		XmlAttribute *attribute = xmlFile->CreateAttribute(attrName);
		attribute->set_Value(value);
		parentElement->Attributes->Append(attribute);
	}

	//*************************************************************************
	// Method:		createRetValNode
	// Description: creates return value node for a function
	//
	// Parameters:
	//	retVal - return value object
	//
	// Return Value: XmlElement for the return value
	//*************************************************************************
	XmlElement *XmlGenerator::createRetValNode(InterceptedFunctionReturnValue *retVal)
	{
		String *value = "";
		XmlElement *FunctionRetVal = xmlFile->CreateElement("ReturnValue");
		FunctionRetVal->set_InnerText(__box(retVal->get_ReturnValue())->ToString());

		switch(retVal->get_Type())
		{
			case ReturnValueType::Success:
				value = "Success";
				break;
			case ReturnValueType::Error:
				value = "Error";
				break;			
			case ReturnValueType::Informational:
				value = "Informational";
				break;
			case ReturnValueType::Warning:
				value = "Warning";
				break;
		}
		setAttributeValue(FunctionRetVal, "Type", value);

		switch(retVal->get_OperatorType())
		{
			case ReturnValueOperatorType::EqualTo:
				value = "EqualTo";
				break;
			case ReturnValueOperatorType::GreaterThan:
				value = "GreaterThan";
				break;
			case ReturnValueOperatorType::GreaterThanOrEqualTo:
				value = "GreaterThanOrEqualTo";
				break;
			case ReturnValueOperatorType::LessThan:
				value = "LessThan";
				break;
			case ReturnValueOperatorType::LessThanOrEqualTo:
				value = "LessThanOrEqualTo";
				break;
			case ReturnValueOperatorType::NotEqualTo:
				value = "NotEqualTo";
				break;
		}
		setAttributeValue(FunctionRetVal, "ValueOperator", value);

		return FunctionRetVal;
	}

	//*************************************************************************
	// Method:		createParamNode
	// Description: creates the XML Element "Param" as it has special attributes
	//
	// Parameters:
	//	paramVal - the Object parameter to be added
	//  indexNum - the index number of the "Param" element to be added
	//
	// Return Value: the XML Element "Param" with its values and attributes
	//*************************************************************************
	XmlElement *XmlGenerator::createParamNode(InterceptedFunctionParameter *paramVal, int indexNum)
	{
		String *value = "";
		XmlElement *FunctionParamElem = xmlFile->CreateElement("Param");
		FunctionParamElem->set_InnerText(paramVal->get_Name());

		setAttributeValue(FunctionParamElem, "Index", Convert::ToString(indexNum));

		setAttributeValue(FunctionParamElem, "Type", paramVal->get_Type());

		switch (paramVal->get_CompareAsType())
		{
			case NullType:
				value = "NullType";
				break;
			case StringType:
				value = "StringType";
				break;
			case WideStringType:
				value = "WideStringType";
				break;
			case BooleanType:
				value = "BooleanType";
				break;
			case IntegerType:
				value = "IntegerType";
				break;
			case UnsignedLongType:
				value = "UnsignedLongType";
				break;
			case RealType:
				value = "RealType";
				break;
			case PointerType:
				value = "PointerType";
				break;
			case PointerPointerType:
				value = "PointerPointerType";
				break;
			case Integer64Type:
				value = "Integer64Type";
				break;
			case IgnoreType:
				value = "IgnoreType";
				break;
		}
		setAttributeValue(FunctionParamElem, "CompareAs", value);

		setAttributeValue(FunctionParamElem, "Access", paramVal->get_Access());

		return FunctionParamElem;
	}

	//*************************************************************************
	// Method:		replaceFunction
	// Description: creates the replacement function (once a function by the
	//              same name is found)
	//
	// Parameters:
	//	bookmarkNode - the original node (which is to be replaced)
	//  function - the Object function to be replaced with
	//
	// Return Value: the Node with the new (replaced) function
	//*************************************************************************
	XmlNode *XmlGenerator::replaceFunction(XmlNode *bookmarkNode, InterceptedFunction *function)
	{
		XmlNode *replaceThisNode = bookmarkNode->Clone();
		XmlNode *changeToThisNode;
		int newListCount = 0;
		XmlNode *oldItem;
		String *newValue;

		if(function->get_Name() != 0)
		{
			replaceNode(changeToThisNode, replaceThisNode, "FunctionName", function->get_Name());
			replaceNode(changeToThisNode, replaceThisNode, "ReplacementFunctionName", String::Concat(function->get_Name(), "Replacement"));
		}

		if(function->get_OriginalDll() != 0)
			replaceNode(changeToThisNode, replaceThisNode, "OriginalDll", function->get_OriginalDll());

		if(function->get_InterceptedDll() != 0)
			replaceNode(changeToThisNode, replaceThisNode, "InterceptedDll", function->get_InterceptedDll());

		if(function->get_ReturnType() != 0)
			replaceNode(changeToThisNode, replaceThisNode, "ReturnType", function->get_ReturnType());

		XmlNodeList *modList = replaceThisNode->SelectNodes("TypeModifier");
		newListCount = function->get_TypeModifiers()->GetLength(0);
		for (int i=0; i < newListCount; i++)
		{
			oldItem = modList->get_ItemOf(i);
			newValue = function->get_TypeModifiers()->GetValue(i)->ToString();
			replaceList(modList, replaceThisNode, changeToThisNode, newListCount, oldItem, newValue, "TypeModifier");
		}

		XmlNodeList *CallingConModList = replaceThisNode->SelectNodes("CallingConventionModifier");
		newListCount = function->get_CallingConventionModifiers()->GetLength(0);
		for (int i=0; i < newListCount; i++)
		{
			oldItem = CallingConModList->get_ItemOf(i);
			newValue = function->get_CallingConventionModifiers()->GetValue(i)->ToString();
			replaceList(CallingConModList, replaceThisNode, changeToThisNode, newListCount, oldItem, newValue, "CallingConventionModifier");
		}

		XmlNodeList *retValList = replaceThisNode->SelectNodes("ReturnValue");
		newListCount = function->get_ReturnValues()->GetLength(0);
		for (int i=0; i < newListCount; i++)
		{
			oldItem = retValList->get_ItemOf(i);
			InterceptedFunctionReturnValue *newRetValNode = dynamic_cast <InterceptedFunctionReturnValue *> (function->get_ReturnValues()->GetValue(i));
			replaceRetValList (retValList, replaceThisNode, newListCount, oldItem, newRetValNode);
		}

		if(function->get_HelpLink() != 0)
			replaceNode(changeToThisNode, replaceThisNode, "HelpLink", function->get_HelpLink());

		XmlNodeList *paramValList = replaceThisNode->SelectNodes("Param");
		newListCount = function->get_Parameters()->GetLength(0);
		for (int i=0; i < newListCount; i++)
		{
			oldItem = paramValList->get_ItemOf(i);
			InterceptedFunctionParameter *newParamValNode = dynamic_cast <InterceptedFunctionParameter *> (function->get_Parameters()->GetValue(i));
			replaceParamValList (paramValList, replaceThisNode, newListCount, oldItem, newParamValNode, i);
		}

		return replaceThisNode;
	}

	//*************************************************************************
	// Method:		replaceNode
	// Description: sets the new value of the node to be replaced
	//
	// Parameters:
	//  newNode - the new node
	//  oldNode - the old node
	//  elementName - the name of the element (e.g. "FunctionName")
	//	value - the value of the new node
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::replaceNode(XmlNode *newNode, XmlNode *oldNode, String *elementName, String *value)
	{
		newNode = oldNode->SelectSingleNode(elementName);
		newNode->set_InnerXml(value);
	}

	//*************************************************************************
	// Method:		replaceList
	// Description: replaces a list of function elements ("TypeModifier" and
	//              "CallingConventionModifier")
	//
	// Parameters:
	//  list - list of "TypeModifier" or "CallingConventionModifier" values
	//  rNode - "old" (the node to be replaced) node
	//  cNode - "new" node
	//  newListCount - the number of new nodes to be added
	//  oldItem - the location of the old node
	//	newValue - the new value
	//  elementName - the element name (e.g. "TypeModifier")
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::replaceList(XmlNodeList *list, XmlNode *rNode, XmlNode *cNode, int newListCount, XmlNode *oldItem, String *newValue, String *elementName)
	{
		if (list->get_Count() == newListCount)
		{
			for (int i=0; i < newListCount; i++)
				oldItem->set_InnerXml(newValue);
		}
		else if (list->get_Count() < newListCount)
		{
			for (int i=0; i < list->get_Count(); i++)
				oldItem->set_InnerXml(newValue);
			for (int i=list->get_Count(); i < newListCount; i++)
			{
				XmlElement *newModElem = xmlFile->CreateElement(elementName);
				newModElem->set_InnerXml(newValue);

				XmlNode *lastMod = rNode->SelectSingleNode(String::Concat("/", elementName, "[last()]"));
				rNode->InsertAfter(newModElem, lastMod);
			}
		}
		else if (list->get_Count() > newListCount)
		{
			for (int i=0; i < newListCount; i++)
				oldItem->set_InnerXml(newValue);
			for (int i=newListCount; i < list->get_Count(); i++)
			{
				cNode = oldItem;
				rNode->RemoveChild(cNode);
			}
		}
	}

	//*************************************************************************
	// Method:		replaceRetValList
	// Description: replaces the list of new "ReturnValue" elements
	//
	// Parameters:
	//  list - list of the "ReturnValue" values
	//  rNode - "old" (the node to be replaced) node
	//  newListCount - the number of new nodes to be added
	//  oldItem - the location of the old node
	//  newItem - the (new) Object "ReturnValue" with the new characteristics
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::replaceRetValList(XmlNodeList *list, XmlNode *rNode, int newListCount, XmlNode *oldItem, InterceptedFunctionReturnValue *newItem)
	{
		if (list->get_Count() == newListCount)
		{
			for (int i=0; i<newListCount; i++)
			{
				XmlNode *retReplNode = oldItem;
				XmlAttributeCollection *retReplAttr = retReplNode->get_Attributes();
				retReplNode->set_InnerXml (Convert::ToString(newItem->get_ReturnValue()));
				replaceRetValAttr (retReplAttr, newItem, i);
			}
		}
		else if (list->get_Count() < newListCount)
		{
			for (int i=0; i<list->get_Count(); i++)
			{
				XmlNode *retReplNode = oldItem;
				XmlAttributeCollection *retReplAttr = retReplNode->get_Attributes();
				retReplNode->set_InnerXml (Convert::ToString(newItem->get_ReturnValue()));
				replaceRetValAttr (retReplAttr, newItem, i);
			}
			for (int i=list->get_Count(); i < newListCount; i++)
			{
				XmlElement *newRetElem = createRetValNode (newItem);
				XmlNode* lastRet = rNode->SelectSingleNode("/ReturnValue[last()]");
				rNode->InsertAfter(newRetElem, lastRet);
			}
		}
		else if (list->get_Count() > newListCount)
		{
			for (int i=0; i<newListCount; i++)
			{
				XmlNode *retReplNode = oldItem;
				XmlAttributeCollection *retReplAttr = retReplNode->get_Attributes();
				retReplNode->set_InnerXml (Convert::ToString(newItem->get_ReturnValue()));
				replaceRetValAttr (retReplAttr, newItem, i);
			}
			for (int i=newListCount; i < list->get_Count(); i++)
			{
				XmlNode *retExtraNode = oldItem;
				rNode->RemoveChild(retExtraNode);
			}
		}
	}

	//*************************************************************************
	// Method:		replaceRetValAttr
	// Description: replaces the list of the "ReturnValue" attributes 
	//              ("Type" and "ValueOperator")
	//
	// Parameters:
	//  attrList - list of the new attribute values
	//  newItem - the (new) Object "ReturnValue" with the new attribute values
	//  Index - the index value of the "ReturnValue" element
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::replaceRetValAttr(XmlAttributeCollection *attrList, InterceptedFunctionReturnValue *newItem, int Index)
	{
		XmlAttribute *retAttr = attrList->get_ItemOf(Index);
		if (retAttr->get_Name()->Equals("Type"))
		{
			String *strType = "";
			switch (newItem->get_Type())
			{    
				case Success :
					strType = "Success";
					break;
				case Informational :
					strType = "Informational";
					break;
				case Warning :
					strType = "Warning";
					break;
				case Error :
					strType = "Error";
					break;
			}
			if(strType == 0 || (String::Compare(strType, "") == 0))
				retAttr->set_Value("");
			else
				retAttr->set_Value(strType);
		}

		if (retAttr->get_Name()->Equals("ValueOperator"))
		{
			String *strOp = "";
			switch (newItem->get_OperatorType())
			{
				case EqualTo :
					strOp = "EqualTo";
					break;
				case GreaterThan :
					strOp = "GreaterThan";
					break;
				case LessThan :
					strOp = "LessThan";
					break;
				case GreaterThanOrEqualTo :
					strOp = "GreaterThanOrEqualTo";
					break;
				case LessThanOrEqualTo :
					strOp = "LessThanOrEqualTo";
					break;
				case NotEqualTo :
					strOp = "NotEqualTo";
					break;
				default :
					strOp = "EqualTo";
					break;
			}
			if(retAttr->get_Value() == 0 || (String::Compare(retAttr->get_Value(), "") == 0))
				retAttr->set_Value("");
			else
				retAttr->set_Value(strOp);
		}
	}

	//*************************************************************************
	// Method:		replaceParamValList
	// Description: replaces the list of new "Param" elements
	//
	// Parameters:
	//  list - list of the "Param" values
	//  rNode - "old" (the node to be replaced) node
	//  newListCount - the number of new nodes to be added
	//  oldItem - the location of the old node
	//  newItem - the (new) Object "Param" with the new characteristics
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::replaceParamValList (XmlNodeList *list, XmlNode *rNode, int newListCount, XmlNode *oldItem, InterceptedFunctionParameter *newItem, int Index)
	{
		if (list->get_Count() == newListCount)
		{
			for (int i=0; i<newListCount; i++)
			{
				XmlNode *paramReplNode = oldItem;
				XmlAttributeCollection *paramReplAttr = paramReplNode->get_Attributes();
				paramReplNode->set_InnerXml (Convert::ToString(newItem->get_Name()));
				for (int paramAttrIndex=0; paramAttrIndex<4; paramAttrIndex++)
					replaceParamValAttr (paramReplAttr, newItem, paramAttrIndex, Index);
			}
		}
		else if (list->get_Count() < newListCount)
		{
			for (int i=0; i<list->get_Count(); i++)
			{
				XmlNode *paramReplNode = oldItem;
				XmlAttributeCollection *paramReplAttr = paramReplNode->get_Attributes();
				paramReplNode->set_InnerXml (Convert::ToString(newItem->get_Name()));
				for (int paramAttrIndex=0; paramAttrIndex<4; paramAttrIndex++)
					replaceParamValAttr (paramReplAttr, newItem, paramAttrIndex, Index);
			}
			for (int i=list->get_Count(); i < newListCount; i++)
			{
				XmlElement *newParamElem = createParamNode (newItem, i);
				XmlNode* lastParam = rNode->SelectSingleNode("/Param[last()]");
				rNode->InsertAfter(newParamElem, lastParam);
			}
		}
		else if (list->get_Count() > newListCount)
		{
			for (int i=0; i<newListCount; i++)
			{
				XmlNode *paramReplNode = oldItem;
				XmlAttributeCollection *paramReplAttr = paramReplNode->get_Attributes();
				paramReplNode->set_InnerXml (Convert::ToString(newItem->get_Name()));
				for (int paramAttrIndex=0; paramAttrIndex<4; paramAttrIndex++)
					replaceParamValAttr (paramReplAttr, newItem, paramAttrIndex, Index);
			}
			for (int i=newListCount; i < list->get_Count(); i++)
			{
				XmlNode *retExtraNode = oldItem;
				rNode->RemoveChild(retExtraNode);
			}
		}
	}

	//*************************************************************************
	// Method:		replaceParamValAttr
	// Description: replaces the list of the "Param" attributes ("Index",
	//              "Type", "CompareAs" and "Access")
	//
	// Parameters:
	//  attrList - list of the new attribute values
	//  newItem - the (new) Object "Param" with the new attribute values
	//  Index - the index value of the "Param" element
	//
	// Return Value: None
	//*************************************************************************
	void XmlGenerator::replaceParamValAttr (XmlAttributeCollection *attrList, InterceptedFunctionParameter *newItem, int Index, int paramNumber)
	{
		XmlAttribute *paramAttr = attrList->get_ItemOf(Index);
		if (paramAttr->get_Name()->Equals("Index"))
			paramAttr->set_Value(Convert::ToString(paramNumber));

		if (paramAttr->get_Name()->Equals("Type"))
			paramAttr->set_Value(newItem->get_Type()->ToString());

		if (paramAttr->get_Name()->Equals("CompareAs"))
		{
			String *strType = "";
			switch (newItem->get_CompareAsType())
			{
				case NullType:
					strType = "NullType";
					break;
				case StringType:
					strType = "StringType";
					break;
				case WideStringType:
					strType = "WideStringType";
					break;
				case BooleanType:
					strType = "BooleanType";
					break;
				case IntegerType:
					strType = "IntegerType";
					break;
				case UnsignedLongType:
					strType = "UnsignedLongType";
					break;
				case RealType:
					strType = "RealType";
					break;
				case PointerType:
					strType = "PointerType";
					break;
				case PointerPointerType:
					strType = "PointerPointerType";
					break;
				case Integer64Type:
					strType = "Integer64Type";
					break;
				case IgnoreType:
					strType = "IgnoreType";
					break;
			}
			if(strType == 0 || (String::Compare(strType, "") == 0))
				paramAttr->set_Value("");
			else
				paramAttr->set_Value(strType);
		}

		if (paramAttr->get_Name()->Equals("Access"))
			paramAttr->set_Value(newItem->get_Access()->ToString());
	}
}