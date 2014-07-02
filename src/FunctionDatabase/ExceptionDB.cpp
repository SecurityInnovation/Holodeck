//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionDB.cpp
//
// DESCRIPTION: Contains implementation for the singleton class ExceptionDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 APR 2003		 R. Wagner	 File created.
//*************************************************************************
#include "exceptiondb.h"
#using <mscorlib.dll>

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		ExceptionDB
	// Description: Constructor for the ExceptionDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ExceptionDB::ExceptionDB()
	{
		this->exceptionList = new ArrayList();
	}

	//*************************************************************************
	// Method:		~ExceptionDB
	// Description: Destructor for the ExceptionDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ExceptionDB::~ExceptionDB()
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
	ExceptionDB *ExceptionDB::GetInstance()
	{
		if (instance == NULL)
			instance = new ExceptionDB();

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
	void ExceptionDB::ValidationHandler(Object *sender, ValidationEventArgs *args)
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
	void ExceptionDB::ValidateDocument(String *xmlFileName)
	{
		XmlTextReader *reader = new XmlTextReader(xmlFileName);
		XmlValidatingReader *validator = new XmlValidatingReader(reader);

		validator->ValidationType = ValidationType::DTD;
		validator->ValidationEventHandler += new ValidationEventHandler(this, ValidationHandler);

		// do nothing in this loop, all errors will be handled by ValidationHandler
		while (validator->Read())
		{
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
	bool ExceptionDB::LoadDatabase(String *databaseFolder)
	{
		String *xmlFileName = new String("");

		// get the full path to the database file
		if (databaseFolder != NULL)
		{
			if (databaseFolder->EndsWith("\\"))
				xmlFileName = String::Concat(databaseFolder, EXCEPTION_DB_FILE_NAME);
			else
				xmlFileName = String::Concat(databaseFolder, "\\", EXCEPTION_DB_FILE_NAME);
		}
		else
			xmlFileName = EXCEPTION_DB_FILE_NAME;

		ValidateDocument(xmlFileName);

		this->exceptionList->Clear();

		try
		{
			XPathDocument *doc = new XPathDocument(xmlFileName);
			XPathNavigator *navigator = doc->CreateNavigator();

			bool hasMoreExceptions = false;
			navigator->MoveToFirstChild();

			// check for comments
			while (navigator->NodeType == XPathNodeType::Comment)
				navigator->MoveToNext();

			if (navigator->HasChildren)
			{
				navigator->MoveToFirstChild();
				hasMoreExceptions = true;
			}

			while (hasMoreExceptions)
			{
				String *name = NULL;

				bool hasMoreAttributes = navigator->MoveToFirstAttribute();
				while (hasMoreAttributes)
				{
					if (navigator->Name->CompareTo("Name") == 0)
						name = navigator->Value;

					hasMoreAttributes = navigator->MoveToNextAttribute();
				}
				// get back to the parent tag
				navigator->MoveToParent();

				if (name != NULL)
					this->exceptionList->Add(name);

				hasMoreExceptions = navigator->MoveToNext();
			}
		}
		catch(System::IO::FileNotFoundException *e)
		{
			String *msg = new String("Could not find the exception database file: \"");
			msg->Concat(e->FileName);
			throw new Exception(msg);
		}

		isLoaded = true;
		return true;
	}
}
