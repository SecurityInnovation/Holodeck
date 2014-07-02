//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PortDB.cpp
//
// DESCRIPTION: Contains implementation for the singleton class PortDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 2 May 2003		 R. Wagner	 File created.
//*************************************************************************
#include "PortDB.h"
#using <mscorlib.dll>

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		PortDB
	// Description: Constructor for the PortDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	PortDB::PortDB()
	{
		this->byNumberKeyTable = new Hashtable();
	}

	//*************************************************************************
	// Method:		~PortDB
	// Description: Destructor for the PortDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	PortDB::~PortDB()
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
	PortDB *PortDB::GetInstance()
	{
		if (instance == NULL)
			instance = new PortDB();

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
	void PortDB::ValidationHandler(Object *sender, ValidationEventArgs *args)
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
	void PortDB::ValidateDocument(String *xmlFileName)
	{
		XmlTextReader *reader = new XmlTextReader(xmlFileName);
		XmlValidatingReader *validator = new XmlValidatingReader(reader);

		validator->ValidationType = ValidationType::DTD;
		validator->ValidationEventHandler += new ValidationEventHandler(this, &FunctionDatabase::PortDB::ValidationHandler);

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
	bool PortDB::LoadDatabase(String *databaseFolder)
	{
		String *xmlFileName = new String("");

		// get the full path to the database file
		if (databaseFolder != NULL)
		{
			if (databaseFolder->EndsWith("\\"))
				xmlFileName = String::Concat(databaseFolder, PORT_DB_FILE_NAME);
			else
				xmlFileName = String::Concat(databaseFolder, "\\", PORT_DB_FILE_NAME);
		}
		else
			xmlFileName = PORT_DB_FILE_NAME;

		ValidateDocument(xmlFileName);

		this->byNumberKeyTable->Clear();

		try
		{
			XPathDocument *doc = new XPathDocument(xmlFileName);
			XPathNavigator *navigator = doc->CreateNavigator();

			bool hasMorePorts = false;
			navigator->MoveToFirstChild();

			// check for comments
			while (navigator->NodeType == XPathNodeType::Comment)
				navigator->MoveToNext();

			if (navigator->HasChildren)
			{
				navigator->MoveToFirstChild();
				hasMorePorts = true;
			}

			while (hasMorePorts)
			{
				String *str = NULL;
				String *code = NULL;

				bool hasMoreAttributes = navigator->MoveToFirstAttribute();
				while (hasMoreAttributes)
				{
					if (navigator->Name->CompareTo("Name") == 0)
						str = navigator->Value;
					else if (navigator->Name->CompareTo("Number") == 0)
						code = navigator->Value;

					hasMoreAttributes = navigator->MoveToNextAttribute();
				}
				// get back to the parent tag
				navigator->MoveToParent();

				if ((str != NULL) && (code != NULL))
				{
					this->byNumberKeyTable->Add(code, str);
				}

				hasMorePorts = navigator->MoveToNext();
			}
		}
		catch(System::IO::FileNotFoundException *e)
		{
			String *msg = new String("Could not find the port database file: \"");
			msg->Concat(e->FileName);
			throw new Exception(msg);
		}

		isLoaded = true;
		return true;
	}
}
