//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		IconLibrary.cs
//
// DESCRIPTION: Contains class IconLibrary
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 2 Jul 2004		 B. Shirey	 File created.
//*************************************************************************
using System;
using System.Reflection;
using System.Collections;
using System.Drawing;
using System.IO;

namespace SiIconLib
{
	//*************************************************************************
	// valid icon types for SI projects
	//*************************************************************************
	public enum IconType
	{
		ActiveLogIconType,
		BugIconType,
		CompareLogsIconType,
		CreateReportIconType,
		CreateTestIconType,
		CustomIconType,
		DiskIconType,
		ExceptionsIconType,
		FaultIconType,
		FaultsIconType,
		FileCorruptionPaneIconType,
		FileCorruptionIconType,
		FilesIconType,
		HelpContentsPaneIconType,
		HelpPaneIconType,
		HolodeckLogoIconType,
		HolodeckMainIconType,
		I2IdeMainIconType,
		I2LogoIconType,
		LimitsIconType,
		LimitsPaneIconType,
		LogComparisonIconType,
		LogsIconType,
		MemoryIconType,
		NetworkIconType,
		NetworkLogGifIconType,
		NetworkLogIcoIconType,
		NetworkMessagePaneIconType,
		NetworkLogsIconType,
		NewFaultIconType,
		NewProjectIconType,
		NewTestApplicationIconType,
		OpenProjectIconType,
		PausedProcessIconType,
		ProcessFaultsIconType,
		ProcessIconType,
		ProjectPaneIconType,
		PropertiesPaneIconType,
		RecordedSessionIconType,
		RecordedSessionsIconType,
		RegistryIconType,
		RegistryEntryIconType,
		RegistryFaultsIconType,
		ReportIconType,
		ReportsIconType,
		ResourcePaneIconType,
		ResourceFaultsIconType,
		ResourcesIconType,
		SavedLogIconType,
		SaveProjectIconType,
		SortAscendingIconType,
		SortDescendingIconType,
		TerminatedProcessIconType,
		TestIconType,
		TestsIconType,
		ThreadIconType,
		TreeViewPlusIconType,
		TreeViewMinusIconType,
		DebugArrowIconType,
		DebugBreakpointIconType,
		DebugContinueIconType,
		DebugBreakIconType,
		DebugStopIconType,
		DebugRestartIconType,
		DebugStepIntoIconType,
		DebugStepOverIconType,
		DebugStepOutIconType,
		RegistersIconType,
		DisassemblerIconType,
		CallStackIconType,
		BacktraceIconType,
		CallTraceIconType,
		FunctionIconType,
		RemoveIconType,
		CutIconType,
		CopyIconType,
		PasteIconType,
		NewBreakpointType
	};

	//*************************************************************************
	// class to help get embedded images from the assembly
	//*************************************************************************
	public class IconLibrary
	{
		private Hashtable iconTypeToFileNameTable;
		private static IconLibrary instance = null;

		//*************************************************************************
		// property to get the single instance of this class
		//*************************************************************************
		public static IconLibrary Instance
		{
			get
			{
				if (instance == null)
					instance = new IconLibrary();
				return instance;
			}
		}

		//*************************************************************************
		// Method:		IconLibrary
		// Description: Constructor for the IconLibrary class
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		private IconLibrary()
		{
			iconTypeToFileNameTable = new Hashtable();

			iconTypeToFileNameTable.Add(IconType.ActiveLogIconType, "ActiveLog.ico");
			iconTypeToFileNameTable.Add(IconType.BugIconType, "Bug.ico");
			iconTypeToFileNameTable.Add(IconType.CompareLogsIconType, "compare_logs.gif");
			iconTypeToFileNameTable.Add(IconType.CreateReportIconType, "create_report.gif");
			iconTypeToFileNameTable.Add(IconType.CreateTestIconType, "create_test.gif");
			iconTypeToFileNameTable.Add(IconType.CustomIconType, "Custom.ico");
			iconTypeToFileNameTable.Add(IconType.DiskIconType, "Disk.ico");
			iconTypeToFileNameTable.Add(IconType.ExceptionsIconType, "Exceptions.ico");
			iconTypeToFileNameTable.Add(IconType.FaultIconType, "Fault.ico");
			iconTypeToFileNameTable.Add(IconType.FaultsIconType, "Faults.ico");
			iconTypeToFileNameTable.Add(IconType.FileCorruptionPaneIconType, "file_corrupt_pane.ico");
			iconTypeToFileNameTable.Add(IconType.FileCorruptionIconType, "FileCorruption.ico");
			iconTypeToFileNameTable.Add(IconType.FilesIconType, "Files.ico");
			iconTypeToFileNameTable.Add(IconType.HelpContentsPaneIconType, "help_contents_pane.gif");
			iconTypeToFileNameTable.Add(IconType.HelpPaneIconType, "help_pane.gif");
			iconTypeToFileNameTable.Add(IconType.HolodeckLogoIconType, "HolodeckEE.bmp");
			iconTypeToFileNameTable.Add(IconType.HolodeckMainIconType, "HDApp.ico");
			iconTypeToFileNameTable.Add(IconType.I2IdeMainIconType, "I2app.ico");
			iconTypeToFileNameTable.Add(IconType.I2LogoIconType, "i2_logo.bmp");
			iconTypeToFileNameTable.Add(IconType.LimitsIconType, "Limits.ico");
			iconTypeToFileNameTable.Add(IconType.LimitsPaneIconType, "limits_pane.gif");
			iconTypeToFileNameTable.Add(IconType.LogComparisonIconType, "LogComparison.ico");
			iconTypeToFileNameTable.Add(IconType.LogsIconType, "Logs.ico");
			iconTypeToFileNameTable.Add(IconType.MemoryIconType, "Memory.ico");
			iconTypeToFileNameTable.Add(IconType.NetworkIconType, "Network.ico");
			iconTypeToFileNameTable.Add(IconType.NetworkLogGifIconType, "network_log.gif");
			iconTypeToFileNameTable.Add(IconType.NetworkLogIcoIconType, "NetworkLog.ico");
			iconTypeToFileNameTable.Add(IconType.NetworkMessagePaneIconType, "network_message_pane.ico");
			iconTypeToFileNameTable.Add(IconType.NetworkLogsIconType, "NetworkLogs.ico");
			iconTypeToFileNameTable.Add(IconType.NewFaultIconType, "new_fault.gif");
			iconTypeToFileNameTable.Add(IconType.NewProjectIconType, "new_project.gif");
			iconTypeToFileNameTable.Add(IconType.NewTestApplicationIconType, "new_test_app.gif");
			iconTypeToFileNameTable.Add(IconType.OpenProjectIconType, "open_project.gif");
			iconTypeToFileNameTable.Add(IconType.PausedProcessIconType, "PausedProcess.ico");
			iconTypeToFileNameTable.Add(IconType.ProcessFaultsIconType, "ProcessFaults.ico");
			iconTypeToFileNameTable.Add(IconType.ProcessIconType, "Process.ico");
			iconTypeToFileNameTable.Add(IconType.ProjectPaneIconType, "project_pane.gif");
			iconTypeToFileNameTable.Add(IconType.PropertiesPaneIconType, "properties_pane.gif");
			iconTypeToFileNameTable.Add(IconType.RecordedSessionIconType, "RecordedSession.ico");
			iconTypeToFileNameTable.Add(IconType.RecordedSessionsIconType, "RecordedSessions.ico");
			iconTypeToFileNameTable.Add(IconType.RegistryIconType, "Registry.ico");
			iconTypeToFileNameTable.Add(IconType.RegistryEntryIconType, "RegistryEntry.ico");
			iconTypeToFileNameTable.Add(IconType.RegistryFaultsIconType, "RegistryFaults.ico");
			iconTypeToFileNameTable.Add(IconType.ReportIconType, "Report.ico");
			iconTypeToFileNameTable.Add(IconType.ReportsIconType, "Reports.ico");
			iconTypeToFileNameTable.Add(IconType.ResourcePaneIconType, "resource_pane.gif");
			iconTypeToFileNameTable.Add(IconType.ResourceFaultsIconType, "ResourceFaults.ico");
			iconTypeToFileNameTable.Add(IconType.ResourcesIconType, "Resources.ico");
			iconTypeToFileNameTable.Add(IconType.SavedLogIconType, "SavedLog.ico");
			iconTypeToFileNameTable.Add(IconType.SaveProjectIconType, "save_project.gif");
			iconTypeToFileNameTable.Add(IconType.SortAscendingIconType, "sort_ascending.ico");
			iconTypeToFileNameTable.Add(IconType.SortDescendingIconType, "sort_descending.ico");
			iconTypeToFileNameTable.Add(IconType.TerminatedProcessIconType, "TerminatedProcess.ico");
			iconTypeToFileNameTable.Add(IconType.TestIconType, "Test.ico");
			iconTypeToFileNameTable.Add(IconType.TestsIconType, "Tests.ico");
			iconTypeToFileNameTable.Add(IconType.ThreadIconType, "Thread.ico");
			iconTypeToFileNameTable.Add(IconType.TreeViewPlusIconType, "tv_plus.bmp");
			iconTypeToFileNameTable.Add(IconType.TreeViewMinusIconType, "tv_minus.bmp");
			iconTypeToFileNameTable.Add(IconType.DebugArrowIconType, "debug_arrow.bmp");
			iconTypeToFileNameTable.Add(IconType.DebugBreakpointIconType, "debug_bp.bmp");
			iconTypeToFileNameTable.Add(IconType.DebugContinueIconType, "debug_go.ico");
			iconTypeToFileNameTable.Add(IconType.DebugBreakIconType, "debug_break.ico");
			iconTypeToFileNameTable.Add(IconType.DebugStopIconType, "debug_stop.ico");
			iconTypeToFileNameTable.Add(IconType.DebugRestartIconType, "debug_restart.ico");
			iconTypeToFileNameTable.Add(IconType.DebugStepIntoIconType, "debug_stepinto.ico");
			iconTypeToFileNameTable.Add(IconType.DebugStepOverIconType, "debug_stepover.ico");
			iconTypeToFileNameTable.Add(IconType.DebugStepOutIconType, "debug_stepout.ico");
			iconTypeToFileNameTable.Add(IconType.RegistersIconType, "registers.ico");
			iconTypeToFileNameTable.Add(IconType.DisassemblerIconType, "disassembler.ico");
			iconTypeToFileNameTable.Add(IconType.CallStackIconType, "call_stack.ico");
			iconTypeToFileNameTable.Add(IconType.BacktraceIconType, "backtrace.ico");
			iconTypeToFileNameTable.Add(IconType.CallTraceIconType, "call_trace.ico");
			iconTypeToFileNameTable.Add(IconType.FunctionIconType, "function.ico");
			iconTypeToFileNameTable.Add(IconType.RemoveIconType, "remove.ico");
			iconTypeToFileNameTable.Add(IconType.CutIconType, "cut.ico");
			iconTypeToFileNameTable.Add(IconType.CopyIconType, "copy.ico");
			iconTypeToFileNameTable.Add(IconType.PasteIconType, "paste.ico");
			iconTypeToFileNameTable.Add(IconType.NewBreakpointType, "new_breakpoint.ico");
		}

		//*************************************************************************
		// Method:		GetStreamForType
		// Description: Gets the stream for the appropriate icon type
		//
		// Parameters:
		//	type - the icon type to get the stream for
		//
		// Return Value: the stream for the specified icon type
		//*************************************************************************
		public Stream GetStreamForType(IconType type)
		{
			try
			{
				System.Reflection.Assembly assembly = System.Reflection.Assembly.Load("SiIconLib");
				if (assembly == null)
					return null;

				String fileNameForType = (String)iconTypeToFileNameTable[type];
				if (fileNameForType == null)
					return null;

				return assembly.GetManifestResourceStream(String.Concat("SiIconLib.", fileNameForType));
			}
			catch(Exception)
			{
			}

			return null;
		}

		//*************************************************************************
		// Method:		GetIconForType
		// Description: Gets the icon for the appropriate icon type
		//
		// Parameters:
		//	type - the icon type to get the icon for
		//
		// Return Value: the icon for the specified icon type
		//*************************************************************************
		public Icon GetIconForType(IconType type)
		{
			try
			{
				return new Icon(GetStreamForType(type));
			}
			catch(Exception)
			{
			}

			return null;
		}

		//*************************************************************************
		// Method:		GetBitmapForType
		// Description: Gets the bitmap for the appropriate icon type
		//
		// Parameters:
		//	type - the icon type to get the bitmap for
		//
		// Return Value: the bitmap for the specified icon type
		//*************************************************************************
		public Bitmap GetBitmapForType(IconType type)
		{
			try
			{
				return new Bitmap(GetStreamForType(type));
			}
			catch(Exception)
			{
			}

			return null;
		}
	}
}
