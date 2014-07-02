//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		DockingManagerStateSaver.cpp
//
// DESCRIPTION: Contains implementation for the class DockingManagerStateSaver
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 1 Jul 2004		 B. Shirey	 File created.
//*************************************************************************
#include "DockingManagerStateSaver.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		DockingManagerStateSaver
	// Description: Constructor for the DockingManagerStateSaver class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DockingManagerStateSaver::DockingManagerStateSaver()
	{
		manager = NULL;
	}

	//*************************************************************************
	// Method:		~DockingManagerStateSaver
	// Description: Destructor for the DockingManagerStateSaver class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DockingManagerStateSaver::~DockingManagerStateSaver()
	{
	}

	//*************************************************************************
	// Method:		IsSomeStateSaved
	// Description: Determines whether there is state info already saved
	//
	// Parameters:
	//	None
	//
	// Return Value: true if there is state info already saved, false otherwise
	//*************************************************************************
	bool DockingManagerStateSaver::IsSomeStateSaved()
	{
		bool retVal = true;

		try
		{
			if (!regPath || !regValue)
			{
				retVal = false;
			}
			else
			{
				RegistryKey *key = Registry::LocalMachine->OpenSubKey(regPath);
				retVal = key->GetValueNames()->Contains(regValue);
				key->Close();
			}
		}
		catch(...)
		{
			retVal = false;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		SaveDefaultValuesFromFile
	// Description: Reads default settings from a file and stores them as the
	//	current state
	//
	// Parameters:
	//	fileName - the full path to the file to get the default values from
	//
	// Return Value: None
	//*************************************************************************
	void DockingManagerStateSaver::SaveDefaultValuesFromFile(String *fileName)
	{
		if (!fileName)
			return;

		if (File::Exists(fileName))
		{
			StreamReader *reader = new StreamReader(fileName);
			String *dataString = "";
			unsigned int maxBufferSize = (unsigned int)reader->BaseStream->Length;
			Byte buffer[] = new Byte[maxBufferSize];
			
			unsigned int counter = 0;
			while (reader->Peek() > -1)
			{
				dataString = reader->ReadLine();
				for (int i=0; i < dataString->Length; i+= 2)
				{
					buffer[counter] = Byte::Parse (dataString->Substring(i,2), System::Globalization::NumberStyles::HexNumber);
					counter++;

					//if the panesettings file was corrupted or has too much data
					//then return doing nothing
					if (counter >= maxBufferSize)
						return;
				}
			}
			
			Byte sizedBuffer[] = new Byte[counter + 1];
			for (unsigned int i=0; i < counter; i++)
				sizedBuffer[i] = buffer[i];

			reader->Close();

			this->SaveData(sizedBuffer);
		}
	}
    
	//*************************************************************************
	// Method:		LoadData
	// Description: Loads the data into the docking manager
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void DockingManagerStateSaver::LoadData()
	{
		if (!manager || !regPath || !regValue)
			return;

		try
		{
			Byte buffer[];

			RegistryKey *key = Registry::LocalMachine->CreateSubKey(regPath);
			buffer = dynamic_cast<Byte []>(key->GetValue(regValue));
			manager->LoadConfigFromArray(buffer);
			key->Close();
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		SaveData
	// Description: Saves the data from the docking manager
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void DockingManagerStateSaver::SaveData()
	{
		SaveData(NULL);
	}

	//*************************************************************************
	// Method:		SaveData
	// Description: Saves the data from the docking manager
	//
	// Parameters:
	//	data - the data to save
	//
	// Return Value: None
	//*************************************************************************
	void DockingManagerStateSaver::SaveData(Byte data[])
	{
		if (!manager || !regPath || !regValue)
			return;

		try
		{
			Byte buffer[];

			if (!data)
				buffer = manager->SaveConfigToArray();
			else
				buffer = data;

			RegistryKey *key = Registry::LocalMachine->CreateSubKey(regPath);
			key->SetValue(regValue, buffer);
			key->Close();
		}
		catch(...)
		{
		}
	}
}