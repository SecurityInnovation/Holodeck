//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LimitPaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class LimitPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 5 Mar 2004		 J. Taylor	 File created.
//*************************************************************************
#include "LimitPaneDataArray.h"

namespace UIData
{

	//*************************************************************************
	// Method:		LimitPaneDataArray
	// Description: Constructor for the LimitPaneDataArray class
	//
	// Parameters:
	//	procID - The process id associated with these limits
	//
	// Return Value: None
	//*************************************************************************
	LimitPaneDataArray::LimitPaneDataArray(unsigned int procID)
	{
		processID = procID;
		processTerminated = false;

		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		CreateLimitHandler = new CreateLimitDelegate(this, onCreateLimit);
		dataEventRegistry->OnCreateLimit += CreateLimitHandler;
		ModifyLimitHandler = new ModifyLimitDelegate(this, onModifyLimit);
		dataEventRegistry->OnModifyLimit += ModifyLimitHandler;
		DeleteLimitHandler = new DeleteLimitDelegate(this, onDeleteLimit);
		dataEventRegistry->OnDeleteLimit += DeleteLimitHandler;

		ProcessTerminatedHandler = new ProcessTerminatedDelegate(this, onProcessTerminated);
		UIEventRegistry::getInstance()->OnProcessTerminated += ProcessTerminatedHandler;

		try
		{
			PerformanceCounterCategory *networkPerfCounterCategory = new PerformanceCounterCategory("Network Interface");
			String *instanceNames[] = networkPerfCounterCategory->GetInstanceNames();
			maxUsageCounter = new PerformanceCounter("Network Interface", "Current Bandwidth", instanceNames[0], true);
		}
		catch (...)
		{
			maxUsageCounter = NULL;
		}
	}

	//*************************************************************************
	// Method:		LimitPaneDataArray
	// Description: Destructor for the LimitPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LimitPaneDataArray::~LimitPaneDataArray()
	{
		CleanUp();
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Cleans up all dataarray delegates
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void LimitPaneDataArray::CleanUp()
	{
		dataEventRegistry->OnCreateLimit -= CreateLimitHandler;
		dataEventRegistry->OnModifyLimit -= ModifyLimitHandler;
		dataEventRegistry->OnDeleteLimit -= DeleteLimitHandler;
		UIEventRegistry::getInstance()->OnProcessTerminated -= ProcessTerminatedHandler;

		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateLimit
	// Description: Called when a ui pane creates a Limit
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void LimitPaneDataArray::onCreateLimit(DisplayableDataNode *dataNode)
	{
		LimitPaneDataNode *limitNode = dynamic_cast <LimitPaneDataNode*> (dataNode);
		if (!limitNode)
			return;

		if (!limitNode->CreatedDelegate)
			return;
		else if (limitNode->processID == processID)
		{
			switch (limitNode->Type)
			{
			case UIData::DiskLimit:
				holoScriptApp->SetMaximumDiskUsage(limitNode->LimitValue, true);
				break;
			case UIData::MemoryLimit:
				holoScriptApp->SetMaximumMemoryUsage(limitNode->LimitValue, true);
				break;
			case UIData::NetworkUpLimit:
				holoScriptApp->SetNetworkUploadBandwidth(limitNode->LimitValue, true);
				break;
			case UIData::NetworkDownLimit:
				holoScriptApp->SetNetworkDownloadBandwidth(limitNode->LimitValue, true);
				break;
			}

			limitNode->ID = String::Concat(processID.ToString(), threadID.ToString(), Convert::ToString(limitNode->Type));
			CreateDataNode(limitNode);
			if (limitNode->CreatedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = limitNode;
				limitNode->CreatedDelegate->DynamicInvoke(args);
			}
			else
				return;
		}
	}

	//*************************************************************************
	// Method:		onModifyLimit
	// Description: Called when a ui pane modifies a Limit
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void LimitPaneDataArray::onModifyLimit(DisplayableDataNode *originalNode, DisplayableDataNode *modifiedNode, String *propertyName)
	{
		UInt64 realMaxDisk, realMaxMemory, realMaxNetworkUp, realMaxNetworkDown; 
		LimitPaneDataNode *oldLimitNode = dynamic_cast <LimitPaneDataNode*> (this->GetDataNodeByID(String::Concat(processID.ToString(), threadID.ToString(), Convert::ToString(originalNode->Type))));
		LimitPaneDataNode *limitNode = dynamic_cast <LimitPaneDataNode*> (modifiedNode);
		bool limitDeleted = false;
		
		if (!holoScriptApp || !limitNode)
			return;

		if (!oldLimitNode)
		{
			onCreateLimit(modifiedNode);
			return;
		}

		if (limitNode->processID == processID)
		{
			//get limit information
			holoScriptApp->GetRealSpaceInformation(realMaxDisk, realMaxMemory);
			GetRealMaxNetwork(realMaxNetworkUp, realMaxNetworkDown);

			switch (limitNode->Type)
			{
			case UIData::DiskLimit:
				if (limitNode->LimitValue >= realMaxDisk)
				{
					holoScriptApp->SetMaximumDiskUsage(0, false);
					limitDeleted = true;
				}
				else
					holoScriptApp->SetMaximumDiskUsage(limitNode->LimitValue, true);
				break;
			case UIData::MemoryLimit:
				if (limitNode->LimitValue >= realMaxMemory)
				{
					holoScriptApp->SetMaximumMemoryUsage(0, false);
					limitDeleted = true;
				}
				else
					holoScriptApp->SetMaximumMemoryUsage(limitNode->LimitValue, true);
				break;
			case UIData::NetworkUpLimit:
				if (limitNode->LimitValue >= realMaxNetworkUp)
				{
					holoScriptApp->SetNetworkUploadBandwidth(0, false);
					limitDeleted = true;
				}
				else
					holoScriptApp->SetNetworkUploadBandwidth(limitNode->LimitValue, true);
				break;
			case UIData::NetworkDownLimit:
				if (limitNode->LimitValue >= realMaxNetworkDown)
				{
					holoScriptApp->SetNetworkDownloadBandwidth(0, false);
					limitDeleted = true;
				}
				else
					holoScriptApp->SetNetworkDownloadBandwidth(limitNode->LimitValue, true);
				break;
			}	

			if (limitDeleted)
			{
				DeleteDataNode(limitNode);
				if (limitNode->DeletedDelegate)
				{
					Object *args[] = __gc new Object*[1];
					args[0] = limitNode;
					limitNode->DeletedDelegate->DynamicInvoke(args);
				}
			}
			else
			{
				oldLimitNode->LimitValue = limitNode->LimitValue;
				if (limitNode->ModifiedDelegate)
				{
					Object *args[] = __gc new Object*[3];
					args[0] = oldLimitNode;
					args[1] = limitNode;
					args[2] = propertyName;
					limitNode->ModifiedDelegate->DynamicInvoke(args);
				}
			}
		}
	}
	
	//*************************************************************************
	// Method:		onDeleteLimit
	// Description: Called when a ui pane deletes a limit
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void LimitPaneDataArray::onDeleteLimit(DisplayableDataNode *dataNode)
	{
		if (dataNode->processID == processID)
		{
			switch (dataNode->Type)
			{
			case UIData::DiskLimit:
				holoScriptApp->SetMaximumDiskUsage(0, false);
				break;
			case UIData::MemoryLimit:
				holoScriptApp->SetMaximumMemoryUsage(0, false);
				break;
			case UIData::NetworkUpLimit:
				holoScriptApp->SetNetworkUploadBandwidth(0, false);
				break;
			case UIData::NetworkDownLimit:
				holoScriptApp->SetNetworkDownloadBandwidth(0, false);
				break;
			}

			DeleteDataNode(dataNode);
			if (dataNode->DeletedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeletedDelegate->DynamicInvoke(args);
			}
		}
	}

	//*************************************************************************
	// Method:		GetRealMaxNetwork
	// Description: Retrieves current network bandwidth in bytes/second
	//
	// Parameters:
	//	realMaxNetworkUp - out-param that contains the max possible net bandwidth in bytes/sec
	//	realMaxNetworkDown - out-param that contains the max possible net bandwidth in bytes/sec
	//
	// Return Value: none
	//*************************************************************************
	bool LimitPaneDataArray::GetRealMaxNetwork(UInt64 &realMaxNetworkUp, UInt64 &realMaxNetworkDown)
	{
		realMaxNetworkUp = maxUsageCounter->RawValue;
		realMaxNetworkDown = realMaxNetworkUp;

		return true;
	}

	//*************************************************************************
	// Method:		GetMonitorInformation
	// Description: Gets information about the disk and memory usage of the
	//	application being monitored.
	//
	// Parameters:
	//	maximumDiskUsage - stores the maximum limit to the disk space allocated to
	//		the application being monitored.
	//	currentDiskUsage - stores the current amount of disk space being used by the
	//		application being monitored
	//	maximumMemoryUsage - stores the maximum limit to the memory allocated to the
	//		application being monitored
	//	currentMemoryUsage - stores the current amount of memory being used by
	//		the application being monitored.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LimitPaneDataArray::GetMonitorInformation(UInt64 &maximumDiskUsage, UInt64 &currentDiskUsage, UInt64 &maximumMemoryUsage, UInt64 &currentMemoryUsage)
	{
		static sampleCount = 0;
		bool retVal = holoScriptApp->GetMonitorInformation(maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage);

		if (retVal && !processTerminated)
		{
			sampleCount++;
			if (maxRecordedDiskUsage < currentDiskUsage)
				maxRecordedDiskUsage = currentDiskUsage;

			averageRecordedDiskUsage = averageRecordedDiskUsage * ((sampleCount-1)/sampleCount);
			averageRecordedDiskUsage += currentDiskUsage/sampleCount;

			if (maxRecordedMemoryUsage < currentMemoryUsage)
				maxRecordedMemoryUsage = currentMemoryUsage;

			averageRecordedMemoryUsage = averageRecordedMemoryUsage * ((sampleCount-1)/sampleCount);
			averageRecordedMemoryUsage += currentMemoryUsage/sampleCount;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		GetRealSpaceInformation
	// Description: returns max disk and memory space regardless of limits set
	//
	// Parameters:
	//	maximumDiskUsage - stores the maximum limit to the disk space allocated to
	//		the application being monitored.
	//	maximumMemoryUsage - stores the maximum limit to the memory allocated to the
	//		application being monitored
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LimitPaneDataArray::GetRealSpaceInformation(UInt64 &maximumDiskUsage, UInt64 &maximumMemoryUsage)
	{
		return holoScriptApp->GetRealSpaceInformation(maximumDiskUsage, maximumMemoryUsage);
	}

	//*************************************************************************
	// Method:		GetNetworkUploadBandwidth
	// Description: Gets the maximum upload bandwidth allowed
	//
	// Parameters:
	//  uploadBandwidth - the allowed upload bw in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LimitPaneDataArray::GetNetworkUploadBandwidth(UInt64 &uploadBandwidth)
	{
		return holoScriptApp->GetNetworkUploadBandwidth(uploadBandwidth);
	}

	//*************************************************************************
	// Method:		GetNetworkDownloadBandwidth
	// Description: Gets the maximum download bandwidth allowed
	//
	// Parameters:
	//  uploadBandwidth - the allowed download bw in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LimitPaneDataArray::GetNetworkDownloadBandwidth(UInt64 &downloadBandwidth)
	{
		return holoScriptApp->GetNetworkDownloadBandwidth(downloadBandwidth);
	}

	//*************************************************************************
	// Method:		GetNetworkMonitorInformation
	// Description: Gets information about the network usage
	//
	// Parameters:
	//	uploadUsage - receives the upload usage information in bytes/second
	//	downloadUsage - receives the download usage information in bytes/second
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LimitPaneDataArray::GetNetworkMonitorInformation (UInt64 &UploadUsage, UInt64 &DownloadUsage)
	{
		static sampleCount = 0;

		bool retVal = holoScriptApp->GetNetworkMonitorInformation(UploadUsage, DownloadUsage);
		if (retVal && !processTerminated)
		{
			sampleCount++;
			if (maxRecordedNetUpUsage < UploadUsage)
				maxRecordedNetUpUsage = UploadUsage;

			averageRecordedNetUpUsage = averageRecordedNetUpUsage * ((sampleCount-1)/sampleCount);
			averageRecordedNetUpUsage += UploadUsage/sampleCount;

			if (maxRecordedNetDownUsage < DownloadUsage)
				maxRecordedNetDownUsage = DownloadUsage;

			averageRecordedNetDownUsage = averageRecordedNetDownUsage * ((sampleCount-1)/sampleCount);
			averageRecordedNetDownUsage += DownloadUsage/sampleCount;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		onProcessTerminated
	// Description: called when a process is terminated
	//
	// Parameters:
	//	processID - process that was terminated
	//
	// Return Value: None
	//*************************************************************************
	void LimitPaneDataArray::onProcessTerminated(unsigned int pID)
	{
		if (pID == this->ProcessID)
			processTerminated = true;
	}
}