//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectPaneDataArray.h
//
// DESCRIPTION: Contains definition for the class ProjectPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>

#include "DisplayableDataArray.h"
#include "DisplayableDataNode.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;

namespace UIData
{
	public __gc class ProjectPaneDataArray : public DisplayableDataArray
	{
	public:
		ProjectPaneDataArray();
		~ProjectPaneDataArray();
		
		void CleanUp();
		void RemoveProcess(unsigned int processID);

	private:
		TestDeletedDelegate *TestDeletedHandler;		
		ReportDeletedDelegate *ReportDeletedHandler;		
		RecordedSessionDeletedDelegate *RecordedSessionDeletedHandler;		
		LogPaneDeletedDelegate *LogPaneDeletedHandler;			
		NetworkLogPaneDeletedDelegate *NetworkLogPaneDeletedHandler;
		LimitDeletedDelegate *LimitDeletedHandler;				
		FaultDeletedDelegate *FaultDeletedHandler;
		ResourceTestDeletedDelegate *ResourceTestDeletedHandler;
		NetworkCorruptionDeletedDelegate *NetworkCorruptionDeletedHandler;
		FileCorruptionDeletedDelegate *FileCorruptionDeletedHandler;
		SavedFileCorruptionDeletedDelegate *SavedFileCorruptionDeletedHandler;
		ExceptionDeletedDelegate *ExceptionDeletedHandler;

		TestCreatedDelegate *TestCreatedHandler;		
		ReportCreatedDelegate *ReportCreatedHandler;		
		RecordedSessionCreatedDelegate *RecordedSessionCreatedHandler;		
		LogPaneCreatedDelegate *LogPaneCreatedHandler;
		NetworkLogPaneCreatedDelegate *NetworkLogPaneCreatedHandler;
		LimitCreatedDelegate *LimitCreatedHandler;	
		FaultCreatedDelegate *FaultCreatedHandler;		
		ResourceTestCreatedDelegate *ResourceTestCreatedHandler;		
		NetworkCorruptionCreatedDelegate *NetworkCorruptionCreatedHandler;		
		FileCorruptionCreatedDelegate *FileCorruptionCreatedHandler;		
		SavedFileCorruptionCreatedDelegate *SavedFileCorruptionCreatedHandler;		
		ExceptionCreatedDelegate *ExceptionCreatedHandler;		

		TestModifiedDelegate *TestModifiedHandler;		
		ReportModifiedDelegate *ReportModifiedHandler;		
		RecordedSessionModifiedDelegate *RecordedSessionModifiedHandler;		
		LogPaneModifiedDelegate *LogPaneModifiedHandler;
		NetworkLogPaneModifiedDelegate *NetworkLogPaneModifiedHandler;
		FaultModifiedDelegate *FaultModifiedHandler;		
		ResourceTestModifiedDelegate *ResourceTestModifiedHandler;		
		NetworkCorruptionModifiedDelegate *NetworkCorruptionModifiedHandler;		
		FileCorruptionModifiedDelegate *FileCorruptionModifiedHandler;		

		void CreateDataNode(DisplayableDataNode *dataNode);
	};
}