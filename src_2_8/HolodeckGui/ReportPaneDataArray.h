//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReportPaneDataArray.h
//
// DESCRIPTION: Contains definition for the class ReportPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 8 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Xml.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "LogPaneDataArray.h"
#include "FaultPaneDataArray.h"
#include "ResourcePaneDataArray.h"
#include "ReportPaneDataNode.h"
#include "ScheduledTestDataArray.h"
#include "LimitPaneDataArray.h"
#include "defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Xml;
using namespace System::Text;
using namespace HoloScript;

namespace UIData
{
	public __gc class ReportPaneDataArray : public DisplayableDataArray
	{
	public:
		[Browsable(false)]
		__property String *get_ReportFolder() { return reportFolder; }
		[ReadOnly(true), Description("The name of this report"), Category("Owner Info")]
		__property String *get_ReportName() { return reportName->Substring(0, reportName->Length-4); }
		__property void set_ReportName(String *value) 
		{ 
			String *oldReportFolder = reportFolder;
			String *oldReportName = reportName;
			reportName = value; 
			if (!reportName->EndsWith(".htm"))
				reportName = String::Concat(reportName, ".htm");
			reportPath = String::Concat(reportFolder, "\\");
			reportPath = String::Concat(reportPath, reportName);
			SaveReportFiles(oldReportFolder, oldReportName, true);
		}
		[ReadOnly(true), Description("The full path to this report"), Category("Owner Info")]
		__property String *get_ReportPath() { return reportPath; }
		__property void set_ReportPath(String *value)
		{
			int index;
			String *oldReportFolder = reportFolder;
			String *oldReportName = reportName;
			reportPath = value;
			index = reportPath->LastIndexOf("\\");
			index++;
			reportName = reportPath->Substring(index, reportPath->Length-index);
			reportFolder = reportPath->Substring(0, index-1);
			SaveReportFiles(oldReportFolder, oldReportName, true);
		}

		ReportPaneDataArray(HoloScriptRegistry *HoloScriptRegistry);
		~ReportPaneDataArray();
		void CleanUp();

		bool CreateReport(String *ReportFolder, SortedList *DataArrayList);
		void DeleteReport();
		void SetReportFolder(String *newFolder, bool copy);

	private:
		static ReportPaneDataArray *instance = NULL;
		SortedList *dataArrayList;
		Hashtable *processTable;
		String *reportPath, *reportFolder, *reportName, *defaultReportPath;
		ModifyReportDelegate *ModifyReportHandler;
		HoloScriptRegistry *holoScriptRegistry;
		ErrorCodeDB *errorCodeDB;
		InterceptedFunctionDB *interceptedFunctionDB;
		InterceptedFunctionDB *dotNetInterceptedFunctionDB;

		bool CreateReportXML(ReportPaneDataNode *reportDataNode);
		void CreateLogReportData(ReportPaneDataNode *reportDataNode);
		void CreateFaultReportData(ReportPaneDataNode *reportDataNode);
		void CreateTestReportData(ReportPaneDataNode *reportDataNode);
		void CreateParamValueReportData(ReportPaneDataNode *reportDataNode);
		void CreateResourceDependencyReportData(ReportPaneDataNode *reportDataNode);
		void CreateUsageReportData(ReportPaneDataNode *usageDataNode);
		String *ConstructProcessName(int processID);
		void onModifyReport(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
		void SaveReportFiles(String *oldReportFolder, String *oldReportName, bool copyFiles);
		void ChangeInternalValues(String *reportPath, String *oldValue, String *newValue);
		String *XMLFilter(String *filterString);
		bool ReportExists(String *reportName);
		String *GetReturnValueType(String *functionName, String *returnValue);
		bool CompareValues(int origValue, int valueToCompare, ReturnValueOperatorType operatorType);
	};
}