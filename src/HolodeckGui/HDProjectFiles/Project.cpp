//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Project.cpp
//
// DESCRIPTION: Contains implementation for the class Project
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Jan 07 2004		 B. Shirey	 File created.
//*************************************************************************
#include "Project.h"

namespace UIData
{
	//*************************************************************************
	// Method:		Project
	// Description: Constructor for the Project class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Project::Project()
	{
		version = HolodeckVersion::V2_5;
		holoReg = HoloScriptRegistry::getInstance();
		dataArrayList = new SortedList();
		active = false;
		perThreadMode = false;
	}

	//*************************************************************************
	// Method:		~Project
	// Description: Destructor for the Project class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Project::~Project()
	{
	}

	//*************************************************************************
	// Method:		get_VersionString
	// Description: gets the version string for the project
	//
	// Parameters:
	//	None
	//
	// Return Value: the version string for the project
	//*************************************************************************
	String *Project::get_VersionString()
	{
		switch(version)
		{
		case HolodeckVersion::V2_0:
			return "V2_0";
		case HolodeckVersion::V2_1:
			return "V2_1";
		case HolodeckVersion::V2_5:
			return "V2_5";
		}

		return "Unknown";
	}

	//*************************************************************************
	// Method:		set_VersionString
	// Description: sets the version string for the project
	//
	// Parameters:
	//	value - the version string for the project
	//
	// Return Value: None
	//*************************************************************************
	void Project::set_VersionString(String *value)
	{
		if (value->CompareTo("V2_5") == 0)
			version = HolodeckVersion::V2_5;
		else if (value->CompareTo("V2_1") == 0)
			version = HolodeckVersion::V2_1;
		else if (value->CompareTo("V2_0") == 0)
			version = HolodeckVersion::V2_0;
		else
			version = HolodeckVersion::V2_0;
	}

	//*************************************************************************
	// Method:		GetDisplayableDataArray
	// Description: Gets a specific data array from the project
	//
	// Parameters:
	//	index - the index of the data array to get
	//
	// Return Value: the displayable data array object, null if not successful
	//*************************************************************************
	DisplayableDataArray *Project::GetDisplayableDataArray(int index)
	{
		if ((index >= dataArrayList->Count) || (index < 0))
			return NULL;

		return dynamic_cast<DisplayableDataArray *>(dataArrayList->GetByIndex(index));
	}

	//**************************************************************************
	// Method:		GetDataArray
	// Description: Looks in the data array list for an array that matches the type
	//	and procid passed in.  There should be only one data array of each type for
	//	each process.  If it finds a match it returns the array.
	//
	// Parameters:
	//	type - type of dataarray to look for
	//
	// Return Value: a displayabledataarray from the data array list if one is found
	//	otherwise retun null
	//*************************************************************************
	DisplayableDataArray* Project::GetDataArray(DisplayableDataNode *dataNode)
	{
		int procID = dataNode->processID;
		int threadID = dataNode->threadID;
		UIData::NodeType type = dataNode->Type;
		String *name = dataNode->Name;

		for (int i=0; i<dataArrayList->Count; i++)
		{
			switch (type)
			{
			case ActiveLogPane:
			case SavedLogPane:
				{
					LogPaneDataArray *logDataArray = dynamic_cast <LogPaneDataArray*> (dataArrayList->GetByIndex(i));
					//log dataarrays are per-thread
					if ((logDataArray) && (logDataArray->ProcessID == procID) && (logDataArray->ThreadID == threadID) )
						return logDataArray;
					break;
				}
			case ActiveNetworkLogPane:
			case SavedNetworkLogPane:
				{
					NetworkLogPaneDataArray *logDataArray = dynamic_cast <NetworkLogPaneDataArray*> (dataArrayList->GetByIndex(i));
					//network log dataarrays are per-thread
					if ((logDataArray) && (logDataArray->ProcessID == procID) && (logDataArray->ThreadID == threadID) )
						return logDataArray;
					break;
				}
			case Faults:
			case MemoryFaultType:
			case DiskFaultType:
			case NetworkFaultType:
			case RegistryFaultType:
			case ProcessFaultType:
			case CustomFaultType:
			case MemoryFault:
			case DiskFault:
			case NetworkFault:
			case RegistryFault:
			case ProcessFault:
			case CustomFault:
				{
					FaultPaneDataArray *faultDataArray = dynamic_cast <FaultPaneDataArray*> (dataArrayList->GetByIndex(i));
					if ((faultDataArray) && (faultDataArray->ProcessID == procID))
						return faultDataArray;;
					break;
				}
			case Limits:
				{
					LimitPaneDataArray *limitDataArray = dynamic_cast <LimitPaneDataArray*> (dataArrayList->GetByIndex(i));
					if ((limitDataArray) && (limitDataArray->ProcessID == procID))
						return limitDataArray;;
					break;
				}
			case NetworkCorruptionFault:
			case NetworkCorruptionFaultType:
				{
					NetworkCorruptionDataArray *ncDataArray = dynamic_cast <NetworkCorruptionDataArray*> (dataArrayList->GetByIndex(i));
					if ((ncDataArray) && (ncDataArray->ProcessID == procID))
						return ncDataArray;
					break;
				}
			case FileCorruptionFault:
			case FileCorruptionFaultType:
				{
					FileCorruptionDataArray *ncDataArray = dynamic_cast <FileCorruptionDataArray*> (dataArrayList->GetByIndex(i));
					if ((ncDataArray) && (ncDataArray->ProcessID == procID))
						return ncDataArray;
					break;
				}
			case CorruptedFile:
			case CorruptedFiles:
			case SavedCorruptedFile:
			case SavedCorruptedFiles:
				{
					SavedFileCorruptionDataArray *ncDataArray = dynamic_cast <SavedFileCorruptionDataArray*> (dataArrayList->GetByIndex(i));
					if ((ncDataArray) && (ncDataArray->ProcessID == procID))
						return ncDataArray;
					break;
				}
			case Tests:
			case Test:
				{
					ScheduledTestDataArray *testDataArray = dynamic_cast <ScheduledTestDataArray*> (dataArrayList->GetByIndex(i));
					if ((testDataArray) && (testDataArray->ProcessID == procID))
						return testDataArray;
					break;
				}
			case ResourceFaults:
			case ResourceFault:
				{
					ResourceTestDataArray *resourceTestDataArray = dynamic_cast <ResourceTestDataArray*> (dataArrayList->GetByIndex(i));
					if ((resourceTestDataArray) && (resourceTestDataArray->ProcessID == procID))
						return resourceTestDataArray;
					break;
				}
			case Report:
				{
					//reports are identified by name
					ReportPaneDataArray *reportDataArray = dynamic_cast <ReportPaneDataArray*> (dataArrayList->GetByIndex(i));
					if ( (reportDataArray) && (dataNode->Name->CompareTo(reportDataArray->ReportName) == 0) )
						return reportDataArray;
					break;
				}
			case UIData::Resources:
				{
					ResourcePaneDataArray *resourceDataArray = dynamic_cast <ResourcePaneDataArray*> (dataArrayList->GetByIndex(i));
					if (resourceDataArray)
						return resourceDataArray;
					break;
				}
			case UIData::RecordedSession:
				{
					RecordedSessionDataArray *recordedDataArray = dynamic_cast <RecordedSessionDataArray*> (dataArrayList->GetByIndex(i));
					if ( (recordedDataArray) && (dataNode->Name->CompareTo(recordedDataArray->Name) == 0) )
						return recordedDataArray;
					break;
				}
			case UIData::ExceptionPane:
			case UIData::ExceptionInfo:
				{
					ExceptionDataArray *exceptionArray = dynamic_cast <ExceptionDataArray*> (dataArrayList->GetByIndex(i));
					if ((exceptionArray) && (exceptionArray->ProcessID == procID))
						return exceptionArray;
					break;
				}
			}
		}
		return NULL;
	}

	//**************************************************************************
	// Method:		ContainsDataArrayKey
	// Description: returns whether or not the set of data arrays contains a given key
	//
	// Parameters:
	//	key - the key to look for
	//
	// Return Value: true if the key is in the set, false otherwise
	//*************************************************************************
	bool Project::ContainsDataArrayKey(String *key)
	{
		return dataArrayList->Contains(key);
	}

	//**************************************************************************
	// Method:		GetDataArrayByKey
	// Description: returns the dataarray that matches the given key
	//
	// Parameters:
	//	key - the key to look for
	//
	// Return Value: the dataarray found or null
	//*************************************************************************
	DisplayableDataArray *Project::GetDataArrayByKey(String *key)
	{
		return dynamic_cast <DisplayableDataArray*> (dataArrayList->Item[key]);
	}

	//**************************************************************************
	// Method:		AddDataArray
	// Description: adds a key and data array to the set of data arrays for the project
	//
	// Parameters:
	//	key - the key to add
	//	dataArray - the array to add
	//
	// Return Value: None
	//*************************************************************************
	void Project::AddDataArray(String *key, DisplayableDataArray *dataArray)
	{
		if (!dataArray || !key)
			return;

		dataArrayList->Add(key, dataArray);
	}

	//**************************************************************************
	// Method:		RemoveDataArray
	// Description: removes the specified data array
	//
	// Parameters:
	//	dataArray - the array to remove
	//
	// Return Value: None
	//*************************************************************************
	void Project::RemoveDataArray(DisplayableDataArray *dataArray)
	{
		int index = dataArrayList->IndexOfValue(dataArray);
		if (index != -1)
			RemoveDataArrayAt(index);
	}

	//**************************************************************************
	// Method:		RemoveDataArrayAt
	// Description: removes the data array at the specified position
	//
	// Parameters:
	//	index - the position of the array to remove
	//
	// Return Value: None
	//*************************************************************************
	void Project::RemoveDataArrayAt(int index)
	{
		dataArrayList->RemoveAt(index);
	}
}