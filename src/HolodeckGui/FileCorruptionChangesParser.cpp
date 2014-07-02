//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionChangesParser.cpp
//
// DESCRIPTION: Contains implementation of the singleton class 
//	FileCorruptionChangesParser.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include <stdio.h>
#include "SiString.h"
#include "FileCorruptionChangesParser.h"
#include "StringConverter.h"
#include "defines.h"
#include "UserNotification.h"

using namespace SiUtils;
using namespace System::Text;

namespace UIData
{
	//*************************************************************************
	// Method:		FileCorruptionChangesParser
	// Description: Constructor for the FileCorruptionChangesParser class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionChangesParser::FileCorruptionChangesParser() : isLoaded(false)
	{
		actions = new Queue();
	}

	//*************************************************************************
	// Method:		ValidationHandler
	// Description: callback function that handles any xml document validation 
	//	errors
	//
	// Parameters:
	//	sender - the object that sent the validation error
	//	args - the arguments that indicate the validation error
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionChangesParser::ValidationHandler(Object *sender, ValidationEventArgs *args)
	{
		throw new Exception(args->Message);
	}
	
	//*************************************************************************
	// Method:		ValidateDocument
	// Description: checks the xml document for syntax errors
	//
	// Parameters:
	//	xmlFileName - the filename of the document to validate
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionChangesParser::ValidateDocument(String *xmlFileName)
	{
		System::IO::StreamReader* in = new System::IO::StreamReader(xmlFileName);

		__try
		{
			XmlTextReader *reader = new XmlTextReader(in);
			XmlValidatingReader *validator = new XmlValidatingReader(reader);

			validator->ValidationType = ValidationType::DTD;
			validator->ValidationEventHandler += new ValidationEventHandler(this, ValidationHandler);

			// do nothing in this loop, all errors will be handled by ValidationHandler
			while (validator->Read())
			{
			}
		}
		__finally
		{
			in->Close();
		}
	}

	//*************************************************************************
	// Method:		GetActionTypeFromString
	// Description: converts a compare as type string to a corresponding
	//	CorruptActionType value
	//
	// Parameters:
	//	actionTypeString - the string representing the action type
	//
	// Return Value: the converted ParameterType value
	//*************************************************************************
	CorruptActionType FileCorruptionChangesParser::GetActionTypeFromString(String *actionTypeString)
	{
		if (actionTypeString->CompareTo("Replace") == 0)
			return ReplaceCorrupt;
		else if (actionTypeString->CompareTo("Insert") == 0)
			return InsertCorrupt;
		else if (actionTypeString->CompareTo("Delete") == 0)
			return DeleteCorrupt;
		return (CorruptActionType)0;
	}

	//*************************************************************************
	// Method:		GetByteArrayFromString
	// Description: converts a whitespace delimited hex string into a byte array
	//
	// Parameters:
	//	arrayString - the string representing the byte array
	//
	// Return Value: the converted byte array
	//*************************************************************************
	Byte FileCorruptionChangesParser::GetByteArrayFromString(String *arrayString, int size) __gc[]
	{
		Byte byteArray __gc[] = new Byte __gc[size];

		for (int i=0; i<size; i++)
		{
			int index = arrayString->IndexOf(' ');
			if (index == -1)
			{
				byteArray[i] = Byte::Parse(arrayString, System::Globalization::NumberStyles::AllowHexSpecifier);
				arrayString = "";
			}
			else
			{
				byteArray[i] = Byte::Parse(arrayString->Substring(0, index), System::Globalization::NumberStyles::AllowHexSpecifier);
				arrayString = arrayString->Substring(index + 1)->Trim();
			}
		}
		return byteArray;
	}

	//*************************************************************************
	// Method:		ParseAction
	// Description: parses the xml document and extracts an action from it
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the action node
	//		to extract
	//
	// Return Value: the extracted action
	//*************************************************************************
	CorruptAction *FileCorruptionChangesParser::ParseAction(XPathNavigator *childNavigator)
	{
		XPathNavigator *actionNavigator = childNavigator->Clone();
		CorruptAction *action = new CorruptAction();

		if (actionNavigator->NodeType == XPathNodeType::Comment)
			return NULL;

		bool hasMoreActionElements = actionNavigator->MoveToFirstAttribute();
		String* oldValue = NULL;
		String* newValue = NULL;
		while (hasMoreActionElements)
		{
			if (actionNavigator->Name->CompareTo("Type") == 0)
				action->ActionType = GetActionTypeFromString(actionNavigator->Value);
			else if (actionNavigator->Name->CompareTo("Offset") == 0)
				action->Offset = Int64::Parse(actionNavigator->Value);
			else if (actionNavigator->Name->CompareTo("ByteCount") == 0)
				action->ByteCount = Int32::Parse(actionNavigator->Value);
			else if (actionNavigator->Name->CompareTo("OldValue") == 0)
				oldValue = actionNavigator->Value;
			else if (actionNavigator->Name->CompareTo("NewValue") == 0)
				newValue = actionNavigator->Value;

			hasMoreActionElements = actionNavigator->MoveToNextAttribute();
		}

		if (oldValue != NULL)
			action->OldValue = GetByteArrayFromString(oldValue, action->ByteCount);
		if (newValue != NULL)
			action->NewValue = GetByteArrayFromString(newValue, action->ByteCount);

		return action;
	}

	//*************************************************************************
	// Method:		LoadDatabase
	// Description: load the database into this object
	//
	// Parameters:
	//	databaseFolder - The folder that contains the database files
	//
	// Return Value: true if loaded successfully, false otherwise
	//*************************************************************************
	bool FileCorruptionChangesParser::Load(String *xmlFileName)
	{
		// make sure the document is syntactically correct
		try
		{
			ValidateDocument(xmlFileName);
		}
		catch (System::Exception*)
		{
			// Validation failed
			return false;
		}

		// clear all old data
		actions->Clear();

		// load in the data
		System::IO::StreamReader* in;
		try
		{
			in = new System::IO::StreamReader(xmlFileName);
		}
		catch(System::IO::FileNotFoundException*)
		{
			// File doesn't exist
			return false;
		}

		try
		{
			XPathDocument *doc = new XPathDocument(in);
			XPathNavigator *navigator = doc->CreateNavigator();

			bool hasMoreElements = false;
			navigator->MoveToFirstChild();

			if (navigator->HasChildren)
			{
				navigator->MoveToFirstChild();
				hasMoreElements = true;
			}

			while (hasMoreElements)
			{
				if (navigator->Name->Equals("OriginalFile"))
					origFile = navigator->Value;
				else if (navigator->Name->Equals("CorruptedFile"))
					corruptFile = navigator->Value;
				else if (navigator->Name->Equals("Actions"))
				{
					XPathNavigator *actionNavigator = navigator->Clone();

					bool hasMoreActions = false;
					if (actionNavigator->MoveToFirstChild())
						hasMoreActions = true;

					while (hasMoreActions)
					{
						CorruptAction *action = ParseAction(actionNavigator);

						if (action)
							actions->Enqueue(action);

						hasMoreActions = actionNavigator->MoveToNext();
					}
				}

				hasMoreElements = navigator->MoveToNext();
			}
		}
		catch (System::Exception*)
		{
			// XML parse failed
			in->Close();
			return false;
		}
		in->Close();

		isLoaded = true;
		return true;
	}

	//*************************************************************************
	// Method:		SaveRTFDetails
	// Description: saves the details of the corruption to an html file
	//
	// Parameters:
	//	outFile - The file to write the html to
	//
	// Return Value: true if loaded written, false otherwise
	//*************************************************************************
	bool FileCorruptionChangesParser::SaveRTFDetails(String* outFile)
	{
		try
		{
			SiString corruptFileName, outFileName;
			Holodeck::StringConverter::GetInstance()->ConvertToSiString(CorruptedFile, &corruptFileName);
			Holodeck::StringConverter::GetInstance()->ConvertToSiString(outFile, &outFileName);

			PSECURITY_DESCRIPTOR pSD;
			SECURITY_ATTRIBUTES sa;

			pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
			InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
			// Initialize a security attributes structure.
			sa.nLength = sizeof (SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = pSD;
			sa.bInheritHandle = FALSE;
			HANDLE corruptFile = CreateFile(corruptFileName, GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (corruptFile == INVALID_HANDLE_VALUE)
				return false;

			HANDLE outputFile = CreateFile(outFileName, GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (outputFile == INVALID_HANDLE_VALUE)
			{
				CloseHandle(corruptFile);
				return false;
			}

			unsigned long totalOriginalBytes = GetFileSize(corruptFile, NULL);
			unsigned char *originalData = new unsigned char[totalOriginalBytes];
			if (!originalData)
				return false;

			// get the original data into a buffer
			unsigned long totalNumRead = 0;
			unsigned long numRead = 0;
			unsigned long totalRemaining = totalOriginalBytes;
			while (totalNumRead < totalOriginalBytes)
			{
				ReadFile(corruptFile, originalData + totalNumRead, totalRemaining * sizeof(unsigned char), &numRead, NULL);
				totalNumRead += numRead;
				totalRemaining -= numRead;
			}
			CloseHandle(corruptFile);

			unsigned long numWritten;
			SiString intro = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033{\\fonttbl{\\f0\\fnil Courier New;}"
			"{\\f1\\fnil\\fcharset0 Courier New;}}{\\colortbl ;\\red0\\green0\\blue0;\\red0\\green0\\blue255;"
			"\\red255\\green0\\blue0;}\\viewkind4\\uc1\\pard\\b\\f0\\fs16\\par\n\\cf1\\b0 ";
			WriteFile(outputFile, (char *)intro, intro.GetLength(), &numWritten, NULL);

			// let's put the corruptionaction objects in a hashtable so we can access them fast
			// the hashtable is key'd on offset, and value'd on an arraylist of actions for
			// that offset
			Hashtable *actionsTable = new Hashtable();
			IEnumerator* enumerator = actions->GetEnumerator();
			while (enumerator->MoveNext())
			{
				CorruptAction* action = dynamic_cast<CorruptAction*>(enumerator->Current);
				if (!action)
					continue;

				if (action->ActionType == CorruptActionType::DeleteCorrupt)
				{
					ArrayList *listAtOffset = dynamic_cast<ArrayList *>(actionsTable->get_Item(__box(action->Offset)));
					if (!listAtOffset)
					{
						listAtOffset = new ArrayList();
						listAtOffset->Add(action);
						actionsTable->Add(__box(action->Offset), listAtOffset);
					}
					else
						listAtOffset->Add(action);
				}
				else
				{
					for (unsigned long i = (unsigned long)action->Offset; i < (action->Offset + action->ByteCount); i++)
					{
						Int64 offset = i;
						ArrayList *listAtOffset = dynamic_cast<ArrayList *>(actionsTable->get_Item(__box(offset)));
						if (!listAtOffset)
						{
							listAtOffset = new ArrayList();
							listAtOffset->Add(action);
							actionsTable->Add(__box(offset), listAtOffset);
						}
						else
							listAtOffset->Add(action);
					}
				}
			}

			// guess that the total size will be at least three times as big as 
			// the original data to speed it up
			StringBuilder *builder = new StringBuilder(3 * totalOriginalBytes);

			String *ascii;
			for (unsigned int i = 0; i < totalOriginalBytes; i++)
			{
				if ((i % 16) == 0)
				{
					if (i > 0)
					{
						builder->Append("  ");
						builder->Append(ascii->ToCharArray());
						builder->Append("\\par\n");
					}

					ascii = "";
					builder->Append(__box(i)->ToString("X8"));
					builder->Append(" ");
				}

				bool needCloseFontTag = false;
				bool needSpace = true;

				// get the actions at this offset, if any
				Int64 offset = i;
				ArrayList *actionList = dynamic_cast<ArrayList *>(actionsTable->get_Item(__box(offset)));
				if (actionList)
				{
					IEnumerator* enumerator = actionList->GetEnumerator();
					while (enumerator->MoveNext())
					{
						CorruptAction* action = dynamic_cast<CorruptAction*>(enumerator->Current);
						if (action->ActionType == CorruptActionType::DeleteCorrupt)
						{
							builder->Append("\\cf3 x");
							needSpace = false;
						}
						if (action->ActionType == CorruptActionType::ReplaceCorrupt)
						{
							ascii = String::Concat(ascii, "\\cf3 ");
							builder->Append("\\cf3 ");
							needCloseFontTag = true;
						}
						else
						{
							ascii = String::Concat(ascii, "\\cf2 ");
							builder->Append("\\cf2 ");
							needCloseFontTag = true;
						}
					}
				}

				if (needSpace)
					builder->Append(" ");

				Byte value = originalData[i];
				if ((value > 32) && (value < 127))
				{
					if (value == '\\')
						ascii = String::Concat(ascii, "\\\\");
					else if (value == '{')
						ascii = String::Concat(ascii, "\\{");
					else if (value == '}')
						ascii = String::Concat(ascii, "\\}");
					else
						ascii = String::Concat(ascii, new String((wchar_t)value, 1));
				}
				else
					ascii = String::Concat(ascii, S".");

				builder->Append(__box(value)->ToString("X2"));
				if (needCloseFontTag)
				{
					builder->Append("\\cf1 ");
					ascii = String::Concat(ascii, "\\cf1 ");
				}
			}

			// Align to right column (ASCII column) and output the remaining ASCII characters
			for (; (i % 16) != 0; i++)
				builder->Append("   ");
			builder->Append("  ");
			builder->Append(ascii);
			builder->Append("\\par\n");

			String *text = builder->ToString();
			SiString textStr;
			Holodeck::StringConverter::GetInstance()->ConvertToSiString(text, &textStr);

			unsigned long totalToWrite = textStr.GetLength();
			unsigned long totalWritten = 0;
			unsigned long totalRemainingToWrite = textStr.GetLength();
			numWritten = 0;
			while (totalWritten < totalToWrite)
			{
				WriteFile(outputFile, ((char *)textStr + totalWritten), totalRemainingToWrite * sizeof(unsigned char), &numWritten, NULL);
				totalWritten += numWritten;
				totalRemainingToWrite -= numWritten;
			}

			SiString conclusion = "\\cf0\\par\n}\n";
			WriteFile(outputFile, (char *)conclusion, conclusion.GetLength(), &numWritten, NULL);
			
			CloseHandle(outputFile);
			delete[] originalData;
		}
		catch (System::Exception *e)
		{
			HolodeckGui::UserNotification::ErrorNotify(String::Concat("Could not save the RTF details file.  Reason: ", e->Message));
			return false;
		}
		return true;
	}
} // namespace

