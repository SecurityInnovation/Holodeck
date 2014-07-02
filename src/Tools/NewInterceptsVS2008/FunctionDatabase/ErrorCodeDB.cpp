//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ErrorCodeDB.cpp
//
// DESCRIPTION: Contains implementation for the singleton class ErrorCodeDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 FEB 2003		 B. Shirey	 File created.
//*************************************************************************
#include "errorcodedb.h"
#using <mscorlib.dll>

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		ErrorCodeDB
	// Description: Constructor for the ErrorCodeDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ErrorCodeDB::ErrorCodeDB()
	{
		this->byCodeKeyTable = new Hashtable();
		this->byStringKeyTable = new Hashtable();
	}

	//*************************************************************************
	// Method:		~ErrorCodeDB
	// Description: Destructor for the ErrorCodeDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ErrorCodeDB::~ErrorCodeDB()
	{
	}

	//*************************************************************************
	// Method:		GetInstance
	// Description: return the single instance of the class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the class
	//*************************************************************************
	ErrorCodeDB *ErrorCodeDB::GetInstance()
	{
		if (instance == NULL)
			instance = new ErrorCodeDB();

		return instance;
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
	void ErrorCodeDB::ValidationHandler(Object *sender, ValidationEventArgs *args)
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
	void ErrorCodeDB::ValidateDocument(String *xmlFileName)
	{
		XmlTextReader *reader = new XmlTextReader(xmlFileName);
		XmlValidatingReader *validator = new XmlValidatingReader(reader);

		validator->ValidationType = ValidationType::DTD;
		validator->ValidationEventHandler += new ValidationEventHandler(this, &FunctionDatabase::ErrorCodeDB::ValidationHandler);

		try
		{
			// do nothing in this loop, all errors will be handled by ValidationHandler
			while (validator->Read())
			{
			}
		}
		catch(...)
		{
			String *msg = "The file Errorcodes.dtd could not be found, Holodeck cannot operate without this file, and will exit now";
			throw new Exception(msg);
		}
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
	bool ErrorCodeDB::LoadDatabase(String *databaseFolder)
	{
		String *xmlFileName = new String("");

		// get the full path to the database file
		if (databaseFolder != NULL)
		{
			if (databaseFolder->EndsWith("\\"))
				xmlFileName = String::Concat(databaseFolder, ERROR_CODE_DB_FILE_NAME);
			else
				xmlFileName = String::Concat(databaseFolder, "\\", ERROR_CODE_DB_FILE_NAME);
		}
		else
			xmlFileName = ERROR_CODE_DB_FILE_NAME;

		ValidateDocument(xmlFileName);

		this->byCodeKeyTable->Clear();
		this->byStringKeyTable->Clear();

		try
		{
			XPathDocument *doc = new XPathDocument(xmlFileName);
			XPathNavigator *navigator = doc->CreateNavigator();

			bool hasMoreErrorCodes = false;
			navigator->MoveToFirstChild();

			// check for comments
			while (navigator->NodeType == XPathNodeType::Comment)
				navigator->MoveToNext();

			if (navigator->HasChildren)
			{
				navigator->MoveToFirstChild();
				hasMoreErrorCodes = true;
			}

			while (hasMoreErrorCodes)
			{
				String *str = NULL;
				String *code = NULL;

				bool hasMoreAttributes = navigator->MoveToFirstAttribute();
				while (hasMoreAttributes)
				{
					if (navigator->Name->CompareTo("String") == 0)
						str = navigator->Value;
					else if (navigator->Name->CompareTo("Code") == 0)
						code = navigator->Value;

					hasMoreAttributes = navigator->MoveToNextAttribute();
				}
				// get back to the parent tag
				navigator->MoveToParent();

				if ((str != NULL) && (code != NULL))
				{
					this->byCodeKeyTable->Add(code, str);
					this->byStringKeyTable->Add(str, code);
				}

				hasMoreErrorCodes = navigator->MoveToNext();
			}
		}
		catch(System::IO::FileNotFoundException *e)
		{
			String *msg = new String("Could not find the error code database file: \"");
			msg->Concat(e->FileName);
			throw new Exception(msg);
		}

		isLoaded = true;
		return true;
	}
}
