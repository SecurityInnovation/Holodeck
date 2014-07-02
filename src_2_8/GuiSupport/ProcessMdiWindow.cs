//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002 – All Rights Reserved.
//
// FILE:	 <ProcessMdiWindow.cs>
//
// DESCRIPTION: Provides all the functionality for the MDI window in the Holodeck winforms UI
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// ??			xxx  B. Shirey   File created.
// 04 FEB 2003  xxx  J. Taylor   Added a listview for displaying scheduled tests.  Includes add and edit functionality but not delete.
// 04 FEB 2003  xxx	 J. Taylor   Fixed a bug in which CreateScheduledTest was not returning a new testID
// 04 FEB 2003  xxx  J. Taylor   Added tooltips to log and scheduled test listview to display parameters since these are truncated
// 04 FEB 2003  xxx  J. Taylor   Fixed a bug in which create tests dlg cancel button was being ignored
// 04 FEB 2003  xxx  J. Taylor   Fixed a minor fit and finish bug by switching return value and error code cols in both listviews
// 05 FEB 2003  xxx  J. Taylor   Fixed a bug in which the modify test dlg was faulting on an ignored parameter
// 06 FEB 2003  xxx  J. Taylor   Added ability to delete scheduled tests from listview
// 11 FEB 2003  xxx  A. Kakrania Added function GetUsageInfo to retrieve disk and memory usage information
// 11 FEB 2003  xxx  A. Kakrania Slight change in constructor to set KB as default unit for DM limits and 100% as default network throttle
// 11 FEB 2003  xxx  A. Kakrania Fixed a small switch-statement bug in LimitDiskUsage and LimitMemUsage
// 15 FEB 2003  xxx  A. Kakrania Globally renamed some disk fault properties to make them "reflection compatible" for File>>Save code.
// 15 FEB 2003  xxx  A. Kakrania Added functionality to save Project settings to an XML file
//*************************************************************************

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using FunctionDatabase;
using HoloScript;
using System.Xml;
using System.Reflection;
using System.IO;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for ProcessMdiWindow.
	/// </summary>
	public class ProcessMdiWindow : System.Windows.Forms.Form
	{
		// the max number of log entries in memory before and after the current entry
		private const int LogEntriesBufferSize = 100;

		#region Member declarations

		private String SavedLogFileName;
		private bool bPredefine_Tasks;
		private bool bProcessChain;
		private bool bApplicationPaused;
		private bool bUseFile;
		private bool bUseProcess;
		private string pszFile;
		private string searchInfo;
		private int iProcessListSelection;
		private HoloScriptApplication holoScriptApp;
		//private HoloScript.LogsGeneratedDelegate logHandler;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage monitorTab;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.GroupBox gb_MemoryMonitor;
		private System.Windows.Forms.ProgressBar pb_Memory;
		private System.Windows.Forms.Label lbl_MemoryFreeAmount;
		private System.Windows.Forms.Label lbl_MemoryFree;
		private System.Windows.Forms.Label lbl_MemoryUsedAmount;
		private System.Windows.Forms.Label lbl_MemoryUsed;
		private System.Windows.Forms.Label lbl_MemoryLimitAmount;
		private System.Windows.Forms.Label lb_MemoryLimit;
		private System.Windows.Forms.ProgressBar pb_Disk;
		private System.Windows.Forms.Label lbl_DiskFreeAmount;
		private System.Windows.Forms.Label lbl_DiskFree;
		private System.Windows.Forms.Label lbl_DiskUsedAmount;
		private System.Windows.Forms.Label lbl_DiskUsed;
		private System.Windows.Forms.Label lbl_DiskLimitAmount;
		private System.Windows.Forms.Label lbl_DiskLimit;
		private System.Windows.Forms.GroupBox groupBox1;
//		private int numVisibleLogEntries;
//		private int numLogEntries;
		private int currentLogEntry;
//		private int lastLogEntryInListView;
//		private bool loadingPreviouslySavedLogFile;

		private ScrollDelegate scrollUpHandler;
		private ScrollDelegate scrollDownHandler;
		private ScrollDelegate scrollLeftHandler;
		private ScrollDelegate scrollRightHandler;
		
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Drawing.Point lastRightClickPoint;
		private System.Collections.ArrayList filteredOutFunctionList;
		private System.Collections.Queue filteredOutLogQueue;
		private LogListView lv_LogInformation;

		private bool injectFileNotFound;
		private bool injectFileCannotAccess;
		private bool injectFileAccessDenied;
		private bool injectFileWriteProtected;
		private bool injectFileCrcDataError;
		private bool injectFileCannotSeekDisk;
		private bool injectFileInUse;
		private bool injectFileLocked;
		private bool injectFileDiskFull;
		private bool injectFileAlreadyExists;
		private bool injectFileCannotBeCreated;
		private bool injectFileNameTooLong;
		private bool injectFileReadOnlyDirectory;
		private bool injectFilePathNotFound;
		private bool injectFileCannotBeRemoved;
		private bool injectFileDirectoryNotEmpty;
		private bool injectFilePathInvalid;
		private bool injectFileCorruptStructure;
		private bool injectMemoryInsufficientMemory;
		private bool injectMemoryInvalidAccess;
		private bool injectMemorySegmentIsLocked;
		private bool injectMemoryInvalidAddress;
		private bool injectMemoryPagingFileTooSmall;
		private bool injectNetworkDisconnect;
		private bool injectNetworkNotInstalled;
		private bool injectNetworkWrongWinsock;
		private bool injectNetworkTaskLimitReached;
		private bool injectNetworkNoPorts;
		private bool injectNetworkNetworkDown;

		private bool throttleNetworkSpeed;
		private bool setMemoryLimit;
		private bool setDiskLimit;

		private uint networkSpeed;
		private uint memByteLimit;
		private uint diskByteLimit;
		private string diskLimitUnit;
		private string memLimitUnit;
		private System.Windows.Forms.TabPage scheduledTasksTab;
		private System.Windows.Forms.ListView lv_ScheduledTests;
		private System.Windows.Forms.ColumnHeader TestHeader_Parameters;
		private System.Windows.Forms.ColumnHeader TestHeader_ErrorCode;
		private System.Windows.Forms.ColumnHeader TestHeader_ReturnValue;
		private System.Windows.Forms.ColumnHeader TestHeader_Function;
		private System.Windows.Forms.ColumnHeader TestHeader_TestID;
		private System.Windows.Forms.ContextMenu modifyTestPopupMenu;
		private System.Windows.Forms.MenuItem modifyTestMenuItem;
		private System.Windows.Forms.MenuItem deleteTestMenuItem;
		private ToolTip lv_ToolTip;
		private SearchLogDialog searchLogDialog;

		#endregion

		#region Normal Properties

		public int AppProcessID
		{
			get
			{
				if (this.holoScriptApp != null)
				{
					uint pid = 0;
					this.holoScriptApp.GetApplicationProcessID(ref pid);

					return (int)pid;
				}

				return (int)-1;
			}
		}

		public bool UseProcessFile
		{
			get { return this.bUseFile; }
			set { this.bUseFile = value; }
		}

		public bool UseProcessID
		{
			get { return this.bUseProcess; }
			set { this.bUseProcess = value; }
		}

		public string ProcessFile
		{
			get { return this.pszFile; }
			set { this.pszFile = value; }
		}

		public int ProcessID
		{
			get { return this.iProcessListSelection; }
			set { this.iProcessListSelection = value; }
		}

		public bool AppPaused
		{
			get { return this.bApplicationPaused; }
			set { this.bApplicationPaused = value; }
		}

		public bool HasSavedLogFile
		{
			get
			{
				if (SavedLogFileName != "")
					return true;
				else
					return false;
			}
				
		}

		public bool ProcessChain
		{
			get { return this.bProcessChain; }
			set { this.bProcessChain = value; }
		}

		public bool PredefineTests
		{
			get { return this.bPredefine_Tasks; }
			set { this.bPredefine_Tasks = value; }
		}

		public ArrayList FilteredOutFunctions
		{
			get { return filteredOutFunctionList; }
			set 
			{ 
				filteredOutFunctionList = (ArrayList)value.Clone(); 
				this.FilterLogs();
			}
		}

		public string SearchInfo
		{
			get { return this.searchInfo; }
			set { this.searchInfo = value; }
		}

		#endregion

		#region Fault Injection Properties

		public bool InjectDiskFileNotFound
		{
			get { return injectFileNotFound; }
			set 
			{ 
				injectFileNotFound = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileNotFound)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileNotFound);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileNotFound);
					}
				}
			}
		}
    
		public bool InjectDiskCannotAccess
		{
			get { return injectFileCannotAccess; }
			set 
			{ 
				injectFileCannotAccess = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileCannotAccess)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileCannotBeAccessed);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileCannotBeAccessed);
					}
				}
			}
		}
    
		public bool InjectDiskAccessDenied
		{
			get { return injectFileAccessDenied; }
			set 
			{ 
				injectFileAccessDenied = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileAccessDenied)
					{
						//this.holoScriptApp.InjectFault(FaultType.AccessDenied);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.AccessDenied);
					}
				}
			}
		}
    
		public bool InjectDiskWriteProtected
		{
			get { return injectFileWriteProtected; }
			set 
			{ 
				injectFileWriteProtected = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileWriteProtected)
					{
						//this.holoScriptApp.InjectFault(FaultType.WriteProtected);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.WriteProtected);
					}
				}
			}
		}
    
		public bool InjectDiskCrcDataError
		{
			get { return injectFileCrcDataError; }
			set 
			{ 
				injectFileCrcDataError = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileCrcDataError)
					{
						//this.holoScriptApp.InjectFault(FaultType.CrcDataError);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.CrcDataError);
					}
				}
			}
		}
    
		public bool InjectDiskDriveCannotSeekDisk
		{
			get { return injectFileCannotSeekDisk; }
			set 
			{ 
				injectFileCannotSeekDisk = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileCannotSeekDisk)
					{
						//this.holoScriptApp.InjectFault(FaultType.DriveCannotSeekDisk);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.DriveCannotSeekDisk);
					}
				}
			}
		}
    
		public bool InjectDiskFileInUse
		{
			get { return injectFileInUse; }
			set 
			{ 
				injectFileInUse = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileInUse)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileInUse);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileInUse);
					}
				}
			}
		}
    
		public bool InjectDiskLocked
		{
			get { return injectFileLocked; }
			set 
			{ 
				injectFileLocked = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileLocked)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileLocked);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileLocked);
					}
				}
			}
		}
    
		public bool InjectDiskDiskFull
		{
			get { return injectFileDiskFull; }
			set 
			{ 
				injectFileDiskFull = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileDiskFull)
					{
						//this.holoScriptApp.InjectFault(FaultType.DiskFull);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.DiskFull);
					}
				}
			}
		}
    
		public bool InjectDiskAlreadyExists
		{
			get { return injectFileAlreadyExists; }
			set 
			{ 
				injectFileAlreadyExists = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileAlreadyExists)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileAlreadyExists);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileAlreadyExists);
					}
				}
			}
		}
    
		public bool InjectDiskCannotBeCreated
		{
			get { return injectFileCannotBeCreated; }
			set 
			{ 
				injectFileCannotBeCreated = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileCannotBeCreated)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileCannotBeCreated);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileCannotBeCreated);
					}
				}
			}
		}
    
		public bool InjectDiskNameTooLong
		{
			get { return injectFileNameTooLong; }
			set 
			{ 
				injectFileNameTooLong = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileNameTooLong)
					{
						//this.holoScriptApp.InjectFault(FaultType.FileNameTooLong);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.FileNameTooLong);
					}
				}
			}
		}
    
		public bool InjectDiskReadOnlyDirectory
		{
			get { return injectFileReadOnlyDirectory; }
			set 
			{ 
				injectFileReadOnlyDirectory = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileReadOnlyDirectory)
					{
						//this.holoScriptApp.InjectFault(FaultType.DirectoryReadOnly);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.DirectoryReadOnly);
					}
				}
			}
		}
    
		public bool InjectDiskPathNotFound
		{
			get { return injectFilePathNotFound; }
			set 
			{ 
				injectFilePathNotFound = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFilePathNotFound)
					{
						//this.holoScriptApp.InjectFault(FaultType.PathNotFound);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.PathNotFound);
					}
				}
			}
		}
    
		public bool InjectDiskCannotBeRemoved
		{
			get { return injectFileCannotBeRemoved; }
			set 
			{ 
				injectFileCannotBeRemoved = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileCannotBeRemoved)
					{
						//this.holoScriptApp.InjectFault(FaultType.DirectoryCannotBeRemoved);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.DirectoryCannotBeRemoved);
					}
				}
			}
		}
    
		public bool InjectDiskDirectoryNotEmpty
		{
			get { return injectFileDirectoryNotEmpty; }
			set 
			{ 
				injectFileDirectoryNotEmpty = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileDirectoryNotEmpty)
					{
						//this.holoScriptApp.InjectFault(FaultType.DirectoryNotEmpty);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.DirectoryNotEmpty);
					}
				}
			}
		}
    
		public bool InjectDiskPathInvalid
		{
			get { return injectFilePathInvalid; }
			set 
			{ 
				injectFilePathInvalid = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFilePathInvalid)
					{
						//this.holoScriptApp.InjectFault(FaultType.PathInvalid);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.PathInvalid);
					}
				}
			}
		}
    
		public bool InjectDiskCorruptStructure
		{
			get { return injectFileCorruptStructure; }
			set 
			{ 
				injectFileCorruptStructure = value; 
				if (this.holoScriptApp != null)
				{
					if (injectFileCorruptStructure)
					{
						//this.holoScriptApp.InjectFault(FaultType.CorruptStructure);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.CorruptStructure);
					}
				}
			}
		}

		public bool InjectMemoryInsufficientMemory
		{
			get { return injectMemoryInsufficientMemory; }
			set 
			{ 
				injectMemoryInsufficientMemory = value; 
				if (this.holoScriptApp != null)
				{
					if (injectMemoryInsufficientMemory)
					{
						//this.holoScriptApp.InjectFault(FaultType.InsufficientMemory);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.InsufficientMemory);
					}
				}
			}
		}

		public bool InjectMemoryInvalidAccess
		{
			get { return injectMemoryInvalidAccess; }
			set 
			{ 
				injectMemoryInvalidAccess = value; 
				if (this.holoScriptApp != null)
				{
					if (injectMemoryInvalidAccess)
					{
						//this.holoScriptApp.InjectFault(FaultType.InvalidAccessToLocation);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.InvalidAccessToLocation);
					}
				}
			}
		}

		public bool InjectMemorySegmentIsLocked
		{
			get { return injectMemorySegmentIsLocked; }
			set 
			{ 
				injectMemorySegmentIsLocked = value; 
				if (this.holoScriptApp != null)
				{
					if (injectMemorySegmentIsLocked)
					{
						//this.holoScriptApp.InjectFault(FaultType.SegmentLocked);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.SegmentLocked);
					}
				}
			}
		}

		public bool InjectMemoryInvalidAddress
		{
			get { return injectMemoryInvalidAddress; }
			set 
			{ 
				injectMemoryInvalidAddress = value; 
				if (this.holoScriptApp != null)
				{
					if (injectMemoryInvalidAddress)
					{
						//this.holoScriptApp.InjectFault(FaultType.InvalidAddress);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.InvalidAddress);
					}
				}
			}
		}

		public bool InjectMemoryPagingFileTooSmall
		{
			get { return injectMemoryPagingFileTooSmall; }
			set 
			{ 
				injectMemoryPagingFileTooSmall = value; 
				if (this.holoScriptApp != null)
				{
					if (injectMemoryPagingFileTooSmall)
					{
						//this.holoScriptApp.InjectFault(FaultType.PagingFileTooSmall);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.PagingFileTooSmall);
					}
				}
			}
		}

		public bool InjectNetworkDisconnect
		{
			get { return injectNetworkDisconnect; }
			set 
			{ 
				injectNetworkDisconnect = value; 
				if (this.holoScriptApp != null)
				{
					if (injectNetworkDisconnect)
					{
						//this.holoScriptApp.InjectFault(FaultType.Disconnected);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.Disconnected);
					}
				}
			}
		}

		public bool InjectNetworkNotInstalled
		{
			get { return injectNetworkNotInstalled; }
			set 
			{ 
				injectNetworkNotInstalled = value; 
				if (this.holoScriptApp != null)
				{
					if (injectNetworkNotInstalled)
					{
						//this.holoScriptApp.InjectFault(FaultType.NetworkNotInstalled);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.NetworkNotInstalled);
					}
				}
			}
		}

		public bool InjectNetworkWrongWinsock
		{
			get { return injectNetworkWrongWinsock; }
			set 
			{ 
				injectNetworkWrongWinsock = value; 
				if (this.holoScriptApp != null)
				{
					if (injectNetworkWrongWinsock)
					{
						//this.holoScriptApp.InjectFault(FaultType.WrongWinsockVersion);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.WrongWinsockVersion);
					}
				}
			}
		}

		public bool InjectNetworkTaskLimitReached
		{
			get { return injectNetworkTaskLimitReached; }
			set 
			{ 
				injectNetworkTaskLimitReached = value; 
				if (this.holoScriptApp != null)
				{
					if (injectNetworkTaskLimitReached)
					{
						//this.holoScriptApp.InjectFault(FaultType.WinsockTaskLimitReached);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.WinsockTaskLimitReached);
					}
				}
			}
		}

		public bool InjectNetworkNoPorts
		{
			get { return injectNetworkNoPorts; }
			set 
			{ 
				injectNetworkNoPorts = value; 
				if (this.holoScriptApp != null)
				{
					if (injectNetworkNoPorts)
					{
						//this.holoScriptApp.InjectFault(FaultType.AllPortsUnavailable);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.AllPortsUnavailable);
					}
				}
			}
		}

		public bool InjectNetworkNetworkDown
		{
			get { return injectNetworkNetworkDown; }
			set 
			{ 
				injectNetworkNetworkDown = value; 
				if (this.holoScriptApp != null)
				{
					if (injectNetworkNetworkDown)
					{
						//this.holoScriptApp.InjectFault(FaultType.NetworkIsDown);
					}
					else
					{
						//this.holoScriptApp.RemoveFault(FaultType.NetworkIsDown);
					}
				}
			}
		}


		public bool LimitDiskUsage
		{
			get { return setDiskLimit; }
			set 
			{ 
				setDiskLimit = value;
				SetAppDiskLimit(this.diskByteLimit);
				UpdateMonitors();
			}
		}

		public bool LimitMemoryUsage
		{
			get { return setMemoryLimit; }
			set 
			{ 
				setMemoryLimit = value;
				SetAppMemoryLimit(this.memByteLimit);
				UpdateMonitors();
			}
		}

		public bool LimitNetworkSpeed
		{
			get { return throttleNetworkSpeed; }
			set 
			{ 
				throttleNetworkSpeed = value;
				SetAppNetworkSpeed(this.networkSpeed);
			}
		}

		public uint MaxDiskUsage
		{
			get { return diskByteLimit; }
			set 
			{ 
				diskByteLimit = value;
				SetAppDiskLimit(this.diskByteLimit);
			}
		}

		public uint MaxMemoryUsage
		{
			get { return memByteLimit; }
			set 
			{ 
				memByteLimit = value;
				SetAppMemoryLimit(this.memByteLimit);
			}
		}

		public uint MaxNetworkSpeed
		{
			get { return networkSpeed; }
			set 
			{ 
				networkSpeed = value;
				SetAppNetworkSpeed(this.networkSpeed);
			}
		}

		public string MaxDiskUsageUnit
		{
			get { return this.diskLimitUnit; }
			set 
			{ 
				this.diskLimitUnit = value; 
				this.SetAppDiskLimit(this.diskByteLimit);
			}
		}

		public string MaxMemoryUsageUnit
		{
			get { return this.memLimitUnit; }
			set 
			{ 
				this.memLimitUnit = value; 
				this.SetAppMemoryLimit(this.memByteLimit);
			}
		}


		#endregion

		#region Non Event Driven Methods

		public ProcessMdiWindow()
		{
			Initialize();
		}

		public ProcessMdiWindow(String logFileName)
		{
			Initialize();
			this.OpenLog(logFileName);
		}

		private void Initialize()
		{
			this.holoScriptApp = new HoloScriptApplication();
			//this.logHandler = new HoloScript.LogsGeneratedDelegate(OnLogGenerated);
			//this.holoScriptApp.OnLogGenerated += logHandler;

			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			filteredOutFunctionList = new ArrayList();
			filteredOutLogQueue = new System.Collections.Queue();

			// build the error code hash tables
			LoadErrorCodeTables();

			this.scrollUpHandler = new ScrollDelegate(this.LogListViewScrollUp);
			this.scrollDownHandler = new ScrollDelegate(this.LogListViewScrollDown);
			this.scrollLeftHandler = new ScrollDelegate(this.LogListViewScrollLeft);
			this.scrollRightHandler = new ScrollDelegate(this.LogListViewScrollRight);

			this.lv_LogInformation.OnScrollUp += scrollUpHandler;
			this.lv_LogInformation.OnScrollDown += scrollDownHandler;
			this.lv_LogInformation.OnScrollLeft += scrollLeftHandler;
			this.lv_LogInformation.OnScrollRight += scrollRightHandler;

//			this.numLogEntries = 0;
//			this.numVisibleLogEntries = 0;
			this.currentLogEntry = 0;
//			this.lastLogEntryInListView = -1;

			lv_ToolTip = new ToolTip();
			lv_ToolTip.AutoPopDelay = 4000;
			lv_ToolTip.InitialDelay = 500;
			lv_ToolTip.ReshowDelay = 500;
			lv_ToolTip.ShowAlways = true;

			this.diskLimitUnit = "KB";
			this.memLimitUnit = "KB";
			this.networkSpeed = 100;
			this.SavedLogFileName = "";
			this.searchInfo = "";
			
//			this.loadingPreviouslySavedLogFile = false;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
				//if (holoScriptApp != null)
				//	this.holoScriptApp.OnLogGenerated -= logHandler;

				this.lv_LogInformation.OnScrollUp -= scrollUpHandler;
				this.lv_LogInformation.OnScrollDown -= scrollDownHandler;
				this.lv_LogInformation.OnScrollLeft -= scrollLeftHandler;
				this.lv_LogInformation.OnScrollRight -= scrollRightHandler;
			}
			base.Dispose( disposing );
		}

		// copies the properties of the window passed in
		public void SetFaultsLimitsAndFilters(ProcessMdiWindow window)
		{
			if (window != null)
			{
				this.InjectDiskAccessDenied = window.InjectDiskAccessDenied;
				this.InjectDiskAlreadyExists = window.InjectDiskAlreadyExists;
				this.InjectDiskCannotAccess = window.InjectDiskCannotAccess;
				this.InjectDiskCannotBeCreated = window.InjectDiskCannotBeCreated;
				this.InjectDiskCannotBeRemoved = window.InjectDiskCannotBeRemoved;
				this.InjectDiskCorruptStructure = window.InjectDiskCorruptStructure;
				this.InjectDiskCrcDataError = window.InjectDiskCrcDataError;
				this.InjectDiskDirectoryNotEmpty = window.InjectDiskDirectoryNotEmpty;
				this.InjectDiskDiskFull = window.InjectDiskDiskFull;
				this.InjectDiskDriveCannotSeekDisk = window.InjectDiskDriveCannotSeekDisk;
				this.InjectDiskFileInUse = window.InjectDiskFileInUse;
				this.InjectDiskFileNotFound = window.InjectDiskFileNotFound;
				this.InjectDiskLocked = window.InjectDiskLocked;
				this.InjectDiskNameTooLong = window.InjectDiskNameTooLong;
				this.InjectDiskPathInvalid = window.InjectDiskPathInvalid;
				this.InjectDiskPathNotFound = window.InjectDiskPathNotFound;
				this.InjectDiskReadOnlyDirectory = window.InjectDiskReadOnlyDirectory;
				this.InjectDiskWriteProtected = window.InjectDiskWriteProtected;
				this.InjectMemoryInsufficientMemory = window.InjectMemoryInsufficientMemory;
				this.InjectMemoryInvalidAccess = window.InjectMemoryInvalidAccess;
				this.InjectMemoryInvalidAddress = window.InjectMemoryInvalidAddress;
				this.InjectMemoryPagingFileTooSmall = window.InjectMemoryPagingFileTooSmall;
				this.InjectMemorySegmentIsLocked = window.InjectMemorySegmentIsLocked;
				this.InjectNetworkDisconnect = window.InjectNetworkDisconnect;
				this.InjectNetworkNetworkDown = window.InjectNetworkNetworkDown;
				this.InjectNetworkNoPorts = window.InjectNetworkNoPorts;
				this.InjectNetworkNotInstalled = window.InjectNetworkNotInstalled;
				this.InjectNetworkWrongWinsock = window.InjectNetworkWrongWinsock;

				this.LimitDiskUsage = window.LimitDiskUsage;
				this.LimitMemoryUsage = window.LimitMemoryUsage;
				this.LimitNetworkSpeed = window.LimitNetworkSpeed;
				this.MaxDiskUsage = window.MaxDiskUsage;
				this.MaxMemoryUsage = window.MaxMemoryUsage;
				this.MaxNetworkSpeed = window.MaxNetworkSpeed;
				this.MaxDiskUsageUnit = window.MaxDiskUsageUnit;
				this.MaxMemoryUsageUnit = window.MaxMemoryUsageUnit;

				this.FilteredOutFunctions = window.FilteredOutFunctions;
			}
		}

		public void ShowSearchDialog()
		{
			if (this.searchLogDialog == null)
				searchLogDialog = new SearchLogDialog(this);
    			
			searchLogDialog.ShowInTaskbar = false;
			searchLogDialog.Show();
			searchLogDialog.SetTextFocus();
		}
    
		public bool GetSelectedItemsText(ref String selectString)
		{
			if (lv_LogInformation.SelectedItems.Count > 0)
			{
				selectString = lv_LogInformation.SelectedItems[0].Text;
				return true;
			}
			
			return false;
		}
    
		public void SearchLogFile ()
		{
			if (SearchInfo == "")
			{
				this.ShowSearchDialog();
			}
			else
			{
				int startRecord = 0;
					
				//Figure out the record number of the currently selected item
				//in reference to the logfile
				if (this.lv_LogInformation.SelectedItems.Count == 0)
					startRecord = currentLogEntry;
				else
					startRecord = currentLogEntry + this.lv_LogInformation.SelectedItems[0].Index;

				//start search from the enter after the selected entry.
				startRecord++;
	    
				//perform search
				int entryNumber = this.holoScriptApp.SearchInLogFile (SearchInfo, startRecord);
	    
				//if search was not successful, then start search again from the top of the file
				if (entryNumber < 0)
					entryNumber = this.holoScriptApp.SearchInLogFile (SearchInfo, 0);

				if (entryNumber >= 0)
				{
					searchLogDialog.Hide();
					currentLogEntry = entryNumber;
					FillLogListFromScratch();
					lv_LogInformation.Items[0].Selected = true;
				}
				else
					MessageBox.Show ("Search string not found", "Holodeck");
			}
		}

		public void LaunchApp()
		{
			if (this.holoScriptApp != null)
			{
				//this.holoScriptApp.LaunchApplication (this.ProcessFile, this.AppPaused, this.filteredOutFunctionList);
			}
		}

		public void AttachApp(ArrayList filteredFunctionList)
		{
			if (this.holoScriptApp != null)
			{
				//this.holoScriptApp.AttachApplication((uint)this.ProcessID, this.AppPaused, filteredFunctionList);
			}
		}

		public void TerminateApp()
		{
			if (this.holoScriptApp != null)
			{
				this.holoScriptApp.TerminateApplication();
			}
		}

		public void PauseApp()
		{
			if (this.holoScriptApp != null)
			{
				this.holoScriptApp.PauseApplication();
				this.AppPaused = true;
			}
		}

		public void ResumeApp()
		{
			if (this.holoScriptApp != null)
			{
				this.holoScriptApp.ResumeApplication();
				this.AppPaused = false;
			}
		}

		private void FilterLogs()
		{
			Cursor.Current = Cursors.WaitCursor;
			this.holoScriptApp.PauseApplication();

			InterceptedFunctionDB db = InterceptedFunctionDB.GetInstance("functions.xml");
			IEnumerator enumerator = db.Functions.GetEnumerator();
			
			// enable everything
			while (enumerator.MoveNext())
			{
				InterceptedFunction function = (InterceptedFunction)enumerator.Current;
				this.holoScriptApp.EnableFunctionLogging(function.Category, function.Name);
			}

			// disable the ones we want to filter out
			foreach (string funcName in filteredOutFunctionList)
			{
				InterceptedFunction function = db.GetFunctionByName(funcName);
				if (function != null)
					this.holoScriptApp.DisableFunctionLogging(function.Category, function.Name);
			}

			this.holoScriptApp.ResumeApplication();
			Cursor.Current = Cursors.Default;
		}

		public void GetUsageInfo(ref ulong currentMemUsage, ref ulong currentDiskUsage)
		{
			if (holoScriptApp != null)
			{
				UInt64 currMemoryUsage = 0, currDiskUsage = 0, maxMemoryUsage = 0, maxDiskUsage = 0;
				this.holoScriptApp.GetMonitorInformation (ref maxDiskUsage, ref currDiskUsage, ref maxMemoryUsage, ref currMemoryUsage); 
    				
				currentMemUsage = currMemoryUsage;
				currentDiskUsage = currDiskUsage;
			}
		}

		private void SetAppMemoryLimit(uint numBytes)
		{
			if (holoScriptApp != null)
			{
				uint multiplier = 1;
				switch(this.memLimitUnit)
				{
					case "B":
						multiplier = 1;
						break;
					case "KB":
						multiplier = 1024;
						break;
					case "MB":
						multiplier = 1024 * 1024;
						break;
					case "GB":
						multiplier = 1024 * 1024 * 1024;
						break;
					default:
						multiplier = 1;
						break;
				}
				this.holoScriptApp.SetMaximumMemoryUsage(numBytes * multiplier, this.LimitMemoryUsage);
			}
		}

		private void SetAppDiskLimit(uint numBytes)
		{
			if (holoScriptApp != null)
			{
				uint multiplier = 1;
				switch(this.diskLimitUnit)
				{
					case "B":
						multiplier = 1;
						break;
					case "KB":
						multiplier = 1024;
						break;
					case "MB":
						multiplier = 1024 * 1024;
						break;
					case "GB":
						multiplier = 1024 * 1024 * 1024;
						break;
					default:
						multiplier = 1;
						break;
				}
				this.holoScriptApp.SetMaximumDiskUsage(numBytes * multiplier, this.LimitDiskUsage);
			}
		}

		private void SetAppNetworkSpeed(uint netSpeed)
		{
			if (holoScriptApp != null)
			{
				//this.holoScriptApp.SetNetworkThrottleLevel(netSpeed, this.throttleNetworkSpeed);
			}
		}

		//*************************************************************************
		// Method:	 CreateSavedScheduledTest
		//
		// Description: Creates a scheduled test that was previously saved in a 
		//	project file and adds a scheduled test to the listview.
		//
		// Parameters:
		//	functionName - string that contains the function name
		//  category - string that contains the function's category
		//	errorCode - string that contains the error code
		//	returnValue - string that contains the return value
		//	parameters - string that contains a comma delimeted list of parameter values to the function
		//  testID - string that contains the unique testID for this test
		//
		// Return Value: None
		//*************************************************************************
		public void CreateSavedScheduledTest(string functionName, string category, string errorCode, string returnValue, string parameters, string testID)
		{
			if (this.holoScriptApp != null)
			{
				string action = returnValue + " " + errorCode;

				// split the parameter string into arrays
				//int numParams = 0;
				//parameters = parameters.Trim();
				////this.holoScriptApp.CreateScheduledTest(category, functionName, action, numParams, AppParamValues, AppTypes, AppPos, ref testID);

				AddScheduledTestItem(functionName, errorCode, returnValue, parameters, testID);
			}
		}

		//*************************************************************************
		// Method:	 AddScheduledTestItem
		//
		// Description: Adds a scheduled test to the listview.  We will modify the existing entryif the testID 
		// already exists, otherwise we'll add a new item
		//
		// Parameters:
		//	functionName - string that contains the function name
		//	errorCode - string that contains the error code
		//	returnValue - string that contains the return value
		//	parameters - string that contains a comma delimeted list of parameter values to the function
		//  testID - string that contains the unique testID for this test
		//
		// Return Value: always returns true
		//*************************************************************************
		private bool AddScheduledTestItem(string functionName, string errorCode, string returnValue, string parameters, string testID)
		{
			//check if we are modifying an existing test
			if ( (this.lv_ScheduledTests.SelectedItems.Count > 0) && (this.lv_ScheduledTests.SelectedItems[0].SubItems[0].Text == testID) )
			{
				ListViewItem listItem = this.lv_ScheduledTests.SelectedItems[0];
				listItem.SubItems[1].Text = functionName;
				listItem.SubItems[2].Text = returnValue;
				listItem.SubItems[3].Text = errorCode;
				listItem.SubItems[4].Text = parameters;
			}
			else
			{
				ListViewItem listItem = new ListViewItem();
				listItem.Text = testID;
				listItem.SubItems.Add(functionName);
				listItem.SubItems.Add(returnValue);
				listItem.SubItems.Add(errorCode);
				listItem.SubItems.Add(parameters);
				this.lv_ScheduledTests.Items.Add(listItem);
			}

			return true;
		}

		public ListView.ListViewItemCollection GetScheduledTests()
		{
			return lv_ScheduledTests.Items;
		}

		public void ShowCreateTestWindow()
		{
			if (this.holoScriptApp == null)
				return;

			this.holoScriptApp.PauseApplication();
			int index = 0;
			
			ListViewItem selectedItem;
			if (this.lv_LogInformation.SelectedItems.Count > 0)
				selectedItem = this.lv_LogInformation.SelectedItems[0];
			else
				selectedItem = null;

			CreateTestWindow window;

			// get the function from the database
			if (selectedItem != null)
			{
				String functionStr = selectedItem.SubItems[3].Text;
				InterceptedFunction function = InterceptedFunctionDB.GetInstance("functions.xml").GetFunctionByName(functionStr);
				if (function == null)
				{
					MessageBox.Show(this, "Function " + functionStr + " not defined in the function database.");
					this.holoScriptApp.ResumeApplication();
					return;
				}

				// get the parameters string from the list view
				String paramStr = selectedItem.SubItems[6].Text;
				String[] splitParamStrings = paramStr.Split(',');

				// remove any leading spaces
				index = 0;
				foreach (String p in splitParamStrings)
				{
					splitParamStrings[index] = p.Trim();
					index++;
				}

				// set the default values of each test parameter
				if (function != null)
				{
					int paramIndex = 0;
					foreach (InterceptedFunctionParameter fp in function.Parameters)
					{
						if (splitParamStrings != null)
							fp.TestValue = splitParamStrings[paramIndex];
						else
							fp.TestValue = null;

						paramIndex++;
					}
				}

				window = new CreateTestWindow(function);
			}
			else
				window = new CreateTestWindow(null);

			if (selectedItem != null)
			{
				String rv = selectedItem.SubItems[4].Text;
				window.ReturnValue = (rv == null) ? "0" : rv;

				String ec = selectedItem.SubItems[5].Text;
				window.ErrorCode = (ec == null) ? "0" : ec;
			}
			else
			{
				window.ErrorCode = "0";
				window.ReturnValue = "0";
			}
			
			window.ShowDialog();
			while ( (window.FunctionName == null) || (window.DialogResult != DialogResult.OK) )
			{
				if (window.DialogResult != DialogResult.OK)
				{
					this.holoScriptApp.ResumeApplication();
					return;
				}
				else if (window.FunctionName == null) 
				{
					MessageBox.Show("Please choose a function to test", "Create a Test");
					window.ShowDialog();
				}
			}


			string testID = null;
			string section = window.Section;
			string testFunction = window.FunctionName;
			string action = window.Action;
			int numParams = window.NumApplicableParams;
			Array AppParamValues = window.ApplicableParamValues;
			Array AppParams = window.ApplicableParams;
			Array AppTypes = window.ApplicableParamTypes;
			Array AppPos = window.ApplicableParamPos;

			//make sure we capture changes made in the dlg
			string[] actionArray = action.Split(' ');
			string newReturnValue = actionArray[0];
			string newErrorCode = actionArray[1];
			string newParamStr = "";

			// Convert the error string into an error code
			string strErrorCode = null;
			if (actionArray.Length > 0)
				strErrorCode = ErrorCodeDB.GetInstance().GetCodeFromString(actionArray[1]);
			if (strErrorCode != null)
			{
				action = actionArray[0] + ' ';
				action += strErrorCode;
			}
			
			index = 1;
			foreach (string param in AppParamValues)
			{
				if (index != AppParamValues.Length) 
					newParamStr += param + ", ";
				else
					newParamStr += param;
				index++;
			}

			if (testFunction.Trim().CompareTo("") != 0)
				//this.holoScriptApp.CreateScheduledTest(section, testFunction, action, numParams, AppParamValues, AppTypes, AppPos, ref testID);

			//if testid is new then a new entry will be created, otherwise the existing one will be altered
			AddScheduledTestItem(testFunction, newErrorCode, newReturnValue, newParamStr, testID);

			this.holoScriptApp.ResumeApplication();
		}

		//*************************************************************************
		// Method:	 ModifyTest
		//
		// Description: Worker function for modifying an existing test.  Called by 
		//	right clicking or double clicking a scheduled test in the list view
		//
		// Parameters:
		//	functionName - string that contains the function name
		//	paramStr - string that contains a comma delimeted list of parameter values to the function
		//	returnValue - string that contains the return value
		//	errorCode - string that contains the error code
		//  testID - string that contains the unique testID for this test
		//
		// Return Value: none
		//*************************************************************************
		private void ModifyTest(string functionName, string paramStr, string returnValue, string errorCode, string testID)
		{
			this.holoScriptApp.PauseApplication();

			InterceptedFunction function = InterceptedFunctionDB.GetInstance("functions.xml").GetFunctionByName(functionName);
			if (function == null)
			{
				MessageBox.Show(this, "Function " + functionName + " not defined in the function database.");
				this.holoScriptApp.ResumeApplication();
				return;
			}
			
			String[] functionParameters = paramStr.Split(',');

			// remove any leading spaces
			int index = 0;
			foreach (String p in functionParameters)
			{
				functionParameters[index] = p.Trim();
				index++;
			}

			// set the default values of each test parameter
			index = 0;
			foreach (InterceptedFunctionParameter fp in function.Parameters)
			{
				fp.TestValue = functionParameters[index];
				index++;
			}

			CreateTestWindow window = new CreateTestWindow(function);
			window.ReturnValue = returnValue;
			window.ErrorCode = errorCode;
			if (window.ShowDialog() != DialogResult.OK)
			{
				this.holoScriptApp.ResumeApplication();
				return;
			}
		
			string section = window.Section;
			string testFunction = window.FunctionName;
			string action = window.Action;
			int numParams = window.NumApplicableParams;
			Array AppParamValues = window.ApplicableParamValues;
			Array AppParams = window.ApplicableParams;
			Array AppTypes = window.ApplicableParamTypes;
			Array AppPos = window.ApplicableParamPos;
			
			//make sure we capture changes made in the dlg
			string[] actionArray = action.Split(' ');
			string newReturnValue = actionArray[0];
			string newErrorCode = actionArray[1];
			string newParamStr = "";

			// Convert the error string into an error code
			string strErrorCode = null;
			if (actionArray.Length > 0)
				strErrorCode = ErrorCodeDB.GetInstance().GetCodeFromString(actionArray[1]);
			if (strErrorCode != null)
			{
				action = actionArray[0] + ' ';
				action += strErrorCode;
			}
			
			index = 1;
			foreach (string param in AppParamValues)
			{
				if (index != AppParamValues.Length) 
					newParamStr += param + ", ";
				else
					newParamStr += param;
				index++;
			}

			//if testid is null a new test will be created, otherwise the existing one will be altered
			//this.holoScriptApp.CreateScheduledTest(section, testFunction, action, numParams, AppParamValues, AppTypes, AppPos, ref testID);

			//if testid is new then a new entry will be created, otherwise the existing one will be altered
			AddScheduledTestItem(testFunction, newErrorCode, newReturnValue, newParamStr, testID);

			this.holoScriptApp.ResumeApplication();

		}

		//called to update the resource monitors in the UI.
		private void UpdateMonitors()
		{
			// by default display as kb
			UInt64 unitsKB = 1024;
			string strUnitsKB = " kb";
			string strUnitsB = " b";
			UInt64 maximumDiskUsage = 0, currentDiskUsage = 0, maximumMemoryUsage = 0, currentMemoryUsage = 0;

			if (this.holoScriptApp.GetMonitorInformation(ref maximumDiskUsage, ref currentDiskUsage, ref maximumMemoryUsage, ref currentMemoryUsage))
			{
				string strMaxDiskUsage = null, strCurrentDiskUsage = null;
				string strMaxMemoryUsage = null, strCurrentMemoryUsage = null;
				string strFree = null;

				//set the disk label, convert to kb if necessary.
				if (maximumDiskUsage < unitsKB)
				{
					strMaxDiskUsage = Convert.ToString(maximumDiskUsage);
					this.lbl_DiskLimitAmount.Text = strMaxDiskUsage + strUnitsB;
				}
				else
				{
					strMaxDiskUsage = Convert.ToString(maximumDiskUsage/unitsKB);
					this.lbl_DiskLimitAmount.Text = strMaxDiskUsage + strUnitsKB;
				}

				if (currentDiskUsage < unitsKB)
				{
					strCurrentDiskUsage = Convert.ToString(currentDiskUsage);
					this.lbl_DiskUsedAmount.Text = strCurrentDiskUsage + strUnitsB;
				}
				else
				{
					strCurrentDiskUsage = Convert.ToString(currentDiskUsage/unitsKB);
					this.lbl_DiskUsedAmount.Text = strCurrentDiskUsage + strUnitsKB;
				}
	
				if ( (maximumDiskUsage - currentDiskUsage) < unitsKB)
				{
					strFree = Convert.ToString(maximumDiskUsage - currentDiskUsage);
					this.lbl_DiskFreeAmount.Text = strFree + strUnitsB;
				}
				else
				{
					strFree = Convert.ToString((maximumDiskUsage - currentDiskUsage)/unitsKB);
					this.lbl_DiskFreeAmount.Text = strFree + strUnitsKB;
				}
			

				//set the disk progress bar, make sure we don't div by 0 or set over 100%
				if ( (maximumDiskUsage == 0) || (maximumDiskUsage < currentDiskUsage) )
					this.pb_Disk.Value = 0;
				else
				{
					Decimal Numerator=currentDiskUsage, Denominator=maximumDiskUsage, Result=0;
					Result = Decimal.Divide(Numerator, Denominator);
					Result = Result * 100;
					this.pb_Disk.Value = Decimal.ToInt16(Result);
				}

				//set the memory label convert to kb is necessary.
				if (maximumMemoryUsage < unitsKB)
				{
					strMaxMemoryUsage = Convert.ToString(maximumMemoryUsage);
					this.lbl_MemoryLimitAmount.Text = strMaxMemoryUsage + strUnitsB;
				}
				else
				{
					strMaxMemoryUsage = Convert.ToString(maximumMemoryUsage/unitsKB);
					this.lbl_MemoryLimitAmount.Text = strMaxMemoryUsage + strUnitsKB;
				}

				if (currentMemoryUsage < unitsKB)
				{
					strCurrentMemoryUsage = Convert.ToString(currentMemoryUsage);
					this.lbl_MemoryUsedAmount.Text = strCurrentMemoryUsage + strUnitsB;
				}
				else
				{
					strCurrentMemoryUsage = Convert.ToString(currentMemoryUsage/unitsKB);
					this.lbl_MemoryUsedAmount.Text = strCurrentMemoryUsage + strUnitsKB;
				}

				if ( (maximumMemoryUsage - currentMemoryUsage) < unitsKB)
				{
					strFree = Convert.ToString(maximumMemoryUsage - currentMemoryUsage);	
					this.lbl_MemoryFreeAmount.Text = strFree + strUnitsB;
				}
				else
				{
					strFree = Convert.ToString((maximumMemoryUsage - currentMemoryUsage)/unitsKB);	
					this.lbl_MemoryFreeAmount.Text = strFree + strUnitsKB;
				}

				//set the memory progress bar.  Make sure we don't div by zero or set over 100%
				if ( (maximumMemoryUsage == 0) || (maximumMemoryUsage < currentMemoryUsage) )
					this.pb_Memory.Value = 0;
				else
				{
					Decimal Numerator=currentMemoryUsage, Denominator=maximumMemoryUsage, Result=0;
					Result = Decimal.Divide(Numerator, Denominator);
					Result = Result * 100;
					this.pb_Memory.Value = Decimal.ToInt16(Result);
				}
			}
		}

		private void LoadErrorCodeTables()
		{
			ErrorCodeDB db = ErrorCodeDB.GetInstance();
			RegistryHelper helper = RegistryHelper.getInstance();
			String databaseFolder = helper.GetFunctionDatabaseFolderPath();

			if (!db.IsLoaded)
				db.LoadDatabase(databaseFolder);
		}

		public ListViewItem GetLogItemAt(int position)
		{
			// commented out becuase it caused a compiler error in the new UI
			// we can uncomment it if we figure out how to pass the filePos variable correctly
			// Should not be a problem since we don't use this file anymore
			//LogItem item = this.holoScriptApp.GetLogEntryAt(position, ref filePos);
			LogItem item = null;

			if (item == null)
				return null;

			ListViewItem listItem = new ListViewItem();
			listItem.Text = item.TimeStamp;
			listItem.SubItems.Add(item.Category);
			listItem.SubItems.Add(item.DllName);
			listItem.SubItems.Add(item.Function);
			listItem.SubItems.Add(item.ReturnValue);

			// convert the error code into an error string for display in the listview
			ErrorCodeDB db = ErrorCodeDB.GetInstance();
			string errorCode = db.GetStringFromCode(item.ErrorCode);
			if (errorCode == null)
				listItem.SubItems.Add(item.ErrorCode);
			else
				listItem.SubItems.Add(errorCode);
				
			String paramStr = "";
			int j = 0;
			IEnumerator paramEnum = item.Parameters.GetEnumerator();
			while (paramEnum.MoveNext())
			{
				String param = (String)paramEnum.Current;
				if (j > 0)
					paramStr = String.Concat(paramStr, ", ", param);
				else
					paramStr = param;
				j++; 
			}
			listItem.SubItems.Add(paramStr);

			return listItem;
		}

		public void FillLogListFromScratch()
		{
			ListView.ListViewItemCollection items = this.lv_LogInformation.Items;
			items.Clear();

			// fill the logs buffers
			for (int i = 0; i < ProcessMdiWindow.LogEntriesBufferSize; i++)
			{
				// fill the list view
				ListViewItem item = GetLogItemAt(currentLogEntry + i);
				if (item != null)
					items.Add(item);
			}
		}

		private void OpenLog(String fileName)
		{
			if (fileName == null)
				return;
			
//			this.loadingPreviouslySavedLogFile = true;
			this.holoScriptApp.OpenLogFile(fileName);
//			this.loadingPreviouslySavedLogFile = false;
		}

		//*************************************************************************
		// Method:	 SaveLog
		//
		// Description: Saves the log to a persistent file.
		//
		// Parameters:
		//	fileName - file to save project to.
		//	useExistingFilename - if true, fileName param. is ignored and the existing
		//  filename is used
		//
		// Return Value: none
		//*************************************************************************
		public void SaveLog(String fileName, bool useExistingFilename)
		{
			if (this.holoScriptApp == null)
				return;

			if (!useExistingFilename)
				SavedLogFileName = fileName;

			String originalFilename = this.holoScriptApp.GetLogFileName();
			System.IO.File.Copy(originalFilename, fileName, true);
		}

		#endregion

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(ProcessMdiWindow));
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.monitorTab = new System.Windows.Forms.TabPage();
			this.panel1 = new System.Windows.Forms.Panel();
			this.panel2 = new System.Windows.Forms.Panel();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.lbl_DiskFree = new System.Windows.Forms.Label();
			this.pb_Disk = new System.Windows.Forms.ProgressBar();
			this.lbl_DiskFreeAmount = new System.Windows.Forms.Label();
			this.lbl_DiskUsed = new System.Windows.Forms.Label();
			this.lbl_DiskUsedAmount = new System.Windows.Forms.Label();
			this.lbl_DiskLimitAmount = new System.Windows.Forms.Label();
			this.lbl_DiskLimit = new System.Windows.Forms.Label();
			this.gb_MemoryMonitor = new System.Windows.Forms.GroupBox();
			this.pb_Memory = new System.Windows.Forms.ProgressBar();
			this.lbl_MemoryFreeAmount = new System.Windows.Forms.Label();
			this.lbl_MemoryFree = new System.Windows.Forms.Label();
			this.lbl_MemoryUsedAmount = new System.Windows.Forms.Label();
			this.lbl_MemoryUsed = new System.Windows.Forms.Label();
			this.lb_MemoryLimit = new System.Windows.Forms.Label();
			this.lbl_MemoryLimitAmount = new System.Windows.Forms.Label();
			this.scheduledTasksTab = new System.Windows.Forms.TabPage();
			this.lv_ScheduledTests = new System.Windows.Forms.ListView();
			this.TestHeader_TestID = new System.Windows.Forms.ColumnHeader();
			this.TestHeader_Function = new System.Windows.Forms.ColumnHeader();
			this.TestHeader_ReturnValue = new System.Windows.Forms.ColumnHeader();
			this.TestHeader_ErrorCode = new System.Windows.Forms.ColumnHeader();
			this.TestHeader_Parameters = new System.Windows.Forms.ColumnHeader();
			this.modifyTestPopupMenu = new System.Windows.Forms.ContextMenu();
			this.modifyTestMenuItem = new System.Windows.Forms.MenuItem();
			this.deleteTestMenuItem = new System.Windows.Forms.MenuItem();
			this.tabControl1.SuspendLayout();
			this.monitorTab.SuspendLayout();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.gb_MemoryMonitor.SuspendLayout();
			this.lv_LogInformation = new LogListView(this);
			this.scheduledTasksTab.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					  this.monitorTab,
																					  this.scheduledTasksTab});
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(792, 438);
			this.tabControl1.TabIndex = 0;
			// 
			// monitorTab
			// 
			this.monitorTab.AutoScroll = true;
			this.monitorTab.Controls.AddRange(new System.Windows.Forms.Control[] {
																					 this.panel1});
			this.monitorTab.Location = new System.Drawing.Point(4, 22);
			this.monitorTab.Name = "monitorTab";
			this.monitorTab.Size = new System.Drawing.Size(784, 412);
			this.monitorTab.TabIndex = 0;
			this.monitorTab.Text = "Monitor";
			
			this.lv_LogInformation.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
			this.lv_LogInformation.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.lv_LogInformation.TabIndex = 10;
			// 
			// panel1
			// 
			this.panel1.Controls.AddRange(new System.Windows.Forms.Control[] {this.panel2, this.lv_LogInformation});
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(784, 412);
			this.panel1.TabIndex = 0;
			// 
			// panel2
			// 
			this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel2.Controls.AddRange(new System.Windows.Forms.Control[] {
																				 this.groupBox1,
																				 this.gb_MemoryMonitor});
			this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel2.Location = new System.Drawing.Point(0, 340);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(784, 72);
			this.panel2.TabIndex = 11;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.lbl_DiskFree,
																					this.pb_Disk,
																					this.lbl_DiskFreeAmount,
																					this.lbl_DiskUsed,
																					this.lbl_DiskUsedAmount,
																					this.lbl_DiskLimitAmount,
																					this.lbl_DiskLimit});
			this.groupBox1.Location = new System.Drawing.Point(384, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(392, 56);
			this.groupBox1.TabIndex = 36;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Disk";
			// 
			// lbl_DiskFree
			// 
			this.lbl_DiskFree.Location = new System.Drawing.Point(256, 16);
			this.lbl_DiskFree.Name = "lbl_DiskFree";
			this.lbl_DiskFree.Size = new System.Drawing.Size(40, 16);
			this.lbl_DiskFree.TabIndex = 4;
			this.lbl_DiskFree.Text = "Free:";
			// 
			// pb_Disk
			// 
			this.pb_Disk.Location = new System.Drawing.Point(16, 32);
			this.pb_Disk.Name = "pb_Disk";
			this.pb_Disk.Size = new System.Drawing.Size(360, 16);
			this.pb_Disk.TabIndex = 6;
			// 
			// lbl_DiskFreeAmount
			// 
			this.lbl_DiskFreeAmount.Location = new System.Drawing.Point(296, 16);
			this.lbl_DiskFreeAmount.Name = "lbl_DiskFreeAmount";
			this.lbl_DiskFreeAmount.Size = new System.Drawing.Size(80, 16);
			this.lbl_DiskFreeAmount.TabIndex = 5;
			this.lbl_DiskFreeAmount.Text = "X MB";
			// 
			// lbl_DiskUsed
			// 
			this.lbl_DiskUsed.Location = new System.Drawing.Point(136, 16);
			this.lbl_DiskUsed.Name = "lbl_DiskUsed";
			this.lbl_DiskUsed.Size = new System.Drawing.Size(40, 16);
			this.lbl_DiskUsed.TabIndex = 2;
			this.lbl_DiskUsed.Text = "Used:";
			// 
			// lbl_DiskUsedAmount
			// 
			this.lbl_DiskUsedAmount.Location = new System.Drawing.Point(176, 16);
			this.lbl_DiskUsedAmount.Name = "lbl_DiskUsedAmount";
			this.lbl_DiskUsedAmount.Size = new System.Drawing.Size(80, 16);
			this.lbl_DiskUsedAmount.TabIndex = 3;
			this.lbl_DiskUsedAmount.Text = "X MB";
			// 
			// lbl_DiskLimitAmount
			// 
			this.lbl_DiskLimitAmount.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.lbl_DiskLimitAmount.Location = new System.Drawing.Point(56, 16);
			this.lbl_DiskLimitAmount.Name = "lbl_DiskLimitAmount";
			this.lbl_DiskLimitAmount.Size = new System.Drawing.Size(80, 16);
			this.lbl_DiskLimitAmount.TabIndex = 1;
			this.lbl_DiskLimitAmount.Text = "X MB";
			// 
			// lbl_DiskLimit
			// 
			this.lbl_DiskLimit.Location = new System.Drawing.Point(16, 16);
			this.lbl_DiskLimit.Name = "lbl_DiskLimit";
			this.lbl_DiskLimit.Size = new System.Drawing.Size(40, 16);
			this.lbl_DiskLimit.TabIndex = 0;
			this.lbl_DiskLimit.Text = "Limit:";
			// 
			// gb_MemoryMonitor
			// 
			this.gb_MemoryMonitor.Controls.AddRange(new System.Windows.Forms.Control[] {
																						   this.pb_Memory,
																						   this.lbl_MemoryFreeAmount,
																						   this.lbl_MemoryFree,
																						   this.lbl_MemoryUsedAmount,
																						   this.lbl_MemoryUsed,
																						   this.lb_MemoryLimit,
																						   this.lbl_MemoryLimitAmount});
			this.gb_MemoryMonitor.Location = new System.Drawing.Point(8, 8);
			this.gb_MemoryMonitor.Name = "gb_MemoryMonitor";
			this.gb_MemoryMonitor.Size = new System.Drawing.Size(360, 56);
			this.gb_MemoryMonitor.TabIndex = 35;
			this.gb_MemoryMonitor.TabStop = false;
			this.gb_MemoryMonitor.Text = "Memory";
			// 
			// pb_Memory
			// 
			this.pb_Memory.Location = new System.Drawing.Point(16, 32);
			this.pb_Memory.Name = "pb_Memory";
			this.pb_Memory.Size = new System.Drawing.Size(328, 16);
			this.pb_Memory.TabIndex = 6;
			// 
			// lbl_MemoryFreeAmount
			// 
			this.lbl_MemoryFreeAmount.Location = new System.Drawing.Point(280, 16);
			this.lbl_MemoryFreeAmount.Name = "lbl_MemoryFreeAmount";
			this.lbl_MemoryFreeAmount.Size = new System.Drawing.Size(72, 16);
			this.lbl_MemoryFreeAmount.TabIndex = 5;
			this.lbl_MemoryFreeAmount.Text = "X MB";
			// 
			// lbl_MemoryFree
			// 
			this.lbl_MemoryFree.Location = new System.Drawing.Point(240, 16);
			this.lbl_MemoryFree.Name = "lbl_MemoryFree";
			this.lbl_MemoryFree.Size = new System.Drawing.Size(40, 16);
			this.lbl_MemoryFree.TabIndex = 4;
			this.lbl_MemoryFree.Text = "Free:";
			// 
			// lbl_MemoryUsedAmount
			// 
			this.lbl_MemoryUsedAmount.Location = new System.Drawing.Point(168, 16);
			this.lbl_MemoryUsedAmount.Name = "lbl_MemoryUsedAmount";
			this.lbl_MemoryUsedAmount.Size = new System.Drawing.Size(72, 16);
			this.lbl_MemoryUsedAmount.TabIndex = 3;
			this.lbl_MemoryUsedAmount.Text = "X MB";
			// 
			// lbl_MemoryUsed
			// 
			this.lbl_MemoryUsed.Location = new System.Drawing.Point(128, 16);
			this.lbl_MemoryUsed.Name = "lbl_MemoryUsed";
			this.lbl_MemoryUsed.Size = new System.Drawing.Size(40, 16);
			this.lbl_MemoryUsed.TabIndex = 2;
			this.lbl_MemoryUsed.Text = "Used:";
			// 
			// lb_MemoryLimit
			// 
			this.lb_MemoryLimit.Location = new System.Drawing.Point(16, 16);
			this.lb_MemoryLimit.Name = "lb_MemoryLimit";
			this.lb_MemoryLimit.Size = new System.Drawing.Size(40, 16);
			this.lb_MemoryLimit.TabIndex = 0;
			this.lb_MemoryLimit.Text = "Limit:";
			// 
			// lbl_MemoryLimitAmount
			// 
			this.lbl_MemoryLimitAmount.ImageAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.lbl_MemoryLimitAmount.Location = new System.Drawing.Point(56, 16);
			this.lbl_MemoryLimitAmount.Name = "lbl_MemoryLimitAmount";
			this.lbl_MemoryLimitAmount.Size = new System.Drawing.Size(72, 16);
			this.lbl_MemoryLimitAmount.TabIndex = 1;
			this.lbl_MemoryLimitAmount.Text = "X MB";
			// 
			// scheduledTasksTab
			// 
			this.scheduledTasksTab.Controls.AddRange(new System.Windows.Forms.Control[] {
																							this.lv_ScheduledTests});
			this.scheduledTasksTab.Location = new System.Drawing.Point(4, 22);
			this.scheduledTasksTab.Name = "scheduledTasksTab";
			this.scheduledTasksTab.Size = new System.Drawing.Size(784, 412);
			this.scheduledTasksTab.TabIndex = 1;
			this.scheduledTasksTab.Text = "Scheduled Tests";
			// 
			// lv_ScheduledTests
			// 
			this.lv_ScheduledTests.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.lv_ScheduledTests.AutoArrange = false;
			this.lv_ScheduledTests.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																								this.TestHeader_TestID,
																								this.TestHeader_Function,
																								this.TestHeader_ReturnValue,
																								this.TestHeader_ErrorCode,
																								this.TestHeader_Parameters});
			this.lv_ScheduledTests.FullRowSelect = true;
			this.lv_ScheduledTests.HideSelection = false;
			this.lv_ScheduledTests.ImeMode = System.Windows.Forms.ImeMode.Off;
			this.lv_ScheduledTests.LabelWrap = false;
			this.lv_ScheduledTests.MultiSelect = false;
			this.lv_ScheduledTests.Name = "lv_ScheduledTests";
			this.lv_ScheduledTests.Size = new System.Drawing.Size(784, 416);
			this.lv_ScheduledTests.TabIndex = 11;
			this.lv_ScheduledTests.View = System.Windows.Forms.View.Details;
			this.lv_ScheduledTests.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.lv_ScheduledTests_KeyPress);
			this.lv_ScheduledTests.DoubleClick += new System.EventHandler(this.lv_ScheduledTests_DoubleClick);
			this.lv_ScheduledTests.MouseUp += new System.Windows.Forms.MouseEventHandler(this.lv_ScheduledTests_MouseUp);
			this.lv_ScheduledTests.MouseHover += new System.EventHandler(this.lv_ScheduledTests_MouseHover);
			// 
			// TestHeader_TestID
			// 
			this.TestHeader_TestID.Text = "TestID";
			this.TestHeader_TestID.Width = 0;
			// 
			// TestHeader_Function
			// 
			this.TestHeader_Function.Text = "Function";
			this.TestHeader_Function.Width = 80;
			// 
			// TestHeader_ReturnValue
			// 
			this.TestHeader_ReturnValue.Text = "ReturnValue";
			this.TestHeader_ReturnValue.Width = 80;
			// 
			// TestHeader_ErrorCode
			// 
			this.TestHeader_ErrorCode.Text = "Error Code";
			this.TestHeader_ErrorCode.Width = 80;
			// 
			// TestHeader_Parameters
			// 
			this.TestHeader_Parameters.Text = "Parameters";
			this.TestHeader_Parameters.Width = 350;
			// 
			// modifyTestPopupMenu
			// 
			this.modifyTestPopupMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																								this.modifyTestMenuItem,
																								this.deleteTestMenuItem});
			// 
			// modifyTestMenuItem
			// 
			this.modifyTestMenuItem.Index = 0;
			this.modifyTestMenuItem.Text = "Modify Test";
			this.modifyTestMenuItem.Click += new System.EventHandler(this.modifyTestMenuItem_Click);
			// 
			// deleteTestMenuItem
			// 
			this.deleteTestMenuItem.Index = 1;
			this.deleteTestMenuItem.Text = "Delete Test";
			this.deleteTestMenuItem.Click += new System.EventHandler(this.deleteTestMenuItem_Click);
			// 
			// ProcessMdiWindow
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(792, 438);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.tabControl1});
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "ProcessMdiWindow";
			this.Text = "ProcessMdiWindow";
			this.Closed += new System.EventHandler(this.ProcessMdiWindow_Closed);
			this.tabControl1.ResumeLayout(false);
			this.monitorTab.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.gb_MemoryMonitor.ResumeLayout(false);
			this.scheduledTasksTab.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		#region Event Driven Methods

		private void ProcessMdiWindow_Closed(object sender, System.EventArgs e)
		{
			if (this.holoScriptApp != null)
			{
				this.TerminateApp();
				this.holoScriptApp.__dtor();
			}
		}

		private void lv_ScheduledTests_DoubleClick(object sender, System.EventArgs e)
		{
			if (this.lv_ScheduledTests.SelectedItems.Count <= 0)
			{
				MessageBox.Show("Please select a test to modify", "Modify Test");
				return;
			}

			ListViewItem selectedItem = this.lv_ScheduledTests.SelectedItems[0];
	
			string testID = selectedItem.SubItems[0].Text;

			// get the function name			
			String functionName = selectedItem.SubItems[1].Text;
		
			// get the return value and error code
			String returnValue = selectedItem.SubItems[2].Text;
			String errorCode = selectedItem.SubItems[3].Text;
					
			// get the parameters string from the list view
			String paramStr = selectedItem.SubItems[4].Text;
	
			ModifyTest(functionName, paramStr, returnValue, errorCode, testID);

		}

		private void lv_ScheduledTests_MouseHover(object sender, System.EventArgs e)
		{
			Point currentPosition = MousePosition;
			String	toolTipString;

			currentPosition = this.lv_ScheduledTests.PointToClient(currentPosition);
			ListViewItem lvi_CurrentItem = this.lv_ScheduledTests.GetItemAt(currentPosition.X, currentPosition.Y);
			if (lvi_CurrentItem != null)
				toolTipString = lvi_CurrentItem.SubItems[4].Text;
			else
				toolTipString = null;
			
			lv_ToolTip.SetToolTip(this.lv_ScheduledTests, toolTipString);

		}

		private void lv_ScheduledTests_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == System.Windows.Forms.MouseButtons.Right)
			{
				lastRightClickPoint = new System.Drawing.Point(e.X, e.Y);
				modifyTestPopupMenu.Show(this, lastRightClickPoint);
			}

		}

		private void modifyTestMenuItem_Click(object sender, System.EventArgs e)
		{
			this.lv_ScheduledTests_DoubleClick(sender, e);
		}

		private void lv_ScheduledTests_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			// if backspace was pressed
			if (e.KeyChar == (char)8)
			{
				this.deleteTestMenuItem_Click(sender, e);
				e.Handled = true;
			}

		}

		private void deleteTestMenuItem_Click(object sender, System.EventArgs e)
		{
			if (this.lv_ScheduledTests.SelectedItems.Count <= 0)
			{
				MessageBox.Show("Please select a test to delete", "Delete Test");
				return;
			}

			ListViewItem selectedItem = this.lv_ScheduledTests.SelectedItems[0];
			string testID = selectedItem.SubItems[0].Text;     
			this.lv_ScheduledTests.Items.Remove(selectedItem);

			this.holoScriptApp.PauseApplication();
			//this.holoScriptApp.DeleteScheduledTest(testID);
			this.holoScriptApp.ResumeApplication();

		}

		#endregion

		#region Callbacks

/*		public void OnLogGenerated(int numNewLogs)
		{
			// don't do the work if there are no logs
			if (numNewLogs > 0)
			{
				int numToAdd = this.currentLogEntry + ProcessMdiWindow.LogEntriesBufferSize - this.lv_LogInformation.MaximumVerticalRange;
				if (numToAdd > numNewLogs)
					numToAdd = numNewLogs;

				ListViewItem[] newItems = null;

				if (numToAdd > 0)
					newItems = new ListViewItem[numToAdd];

				Rectangle viewport = this.lv_LogInformation.Viewport;
				for (int i = 0; i < numToAdd; i++)
				{
					this.lastLogEntryInListView++;
					ListViewItem item = GetLogItemAt(this.lastLogEntryInListView);
					if ((item != null) && (newItems != null))
						newItems[i] = item;
				}

				if ((numToAdd > 0) && (newItems != null))
				{
					this.lv_LogInformation.Items.AddRange(newItems);

					Rectangle lastItemLocation = this.lv_LogInformation.GetItemRectangle(this.lv_LogInformation.Items.Count - 1);
					this.numVisibleLogEntries = (viewport.Height / lastItemLocation.Height) - 1;

					if ((lastItemLocation.Y + lastItemLocation.Height) > viewport.Height)
						this.lv_LogInformation.VerticalScrollingEnabled = true;
				}

				this.numLogEntries += numNewLogs;
				this.lv_LogInformation.MaximumVerticalRange = this.numLogEntries;
			}

			// always update the monitors
			if (!loadingPreviouslySavedLogFile)
				UpdateMonitors();
		}
*/
		public void LogListViewScrollUp(int amount)
		{
			ListView.ListViewItemCollection items = this.lv_LogInformation.Items;

			// make sure there are items to view
			if (items.Count == 0)
				return;

			// do a clean get from file if we moved a long way
			if (amount > (ProcessMdiWindow.LogEntriesBufferSize / 2))
			{
				currentLogEntry -= amount;
				FillLogListFromScratch();
				return;
			}

			for (int i = 0; i < amount; i++)
			{
				currentLogEntry--;

				ListViewItem item = GetLogItemAt(currentLogEntry - 1);
				if (item != null)
					items.Insert(0, item);
			}
		}

		public void LogListViewScrollDown(int amount)
		{
			ListView.ListViewItemCollection items = this.lv_LogInformation.Items;

			// make sure there are items to view
			if (items.Count == 0)
				return;

			// do a clean get from file if we moved a long way
			if (amount > (ProcessMdiWindow.LogEntriesBufferSize / 2))
			{
				currentLogEntry += amount;
				FillLogListFromScratch();
				return;
			}

			for (int i = 0; i < amount; i++)
			{
				currentLogEntry++;

				// remove the first item, effectively moving everything up visually
				items.RemoveAt(0);

				ListViewItem item = GetLogItemAt(currentLogEntry + ProcessMdiWindow.LogEntriesBufferSize);
				if (item != null)
					items.Add(item);
			}
		}

		public void LogListViewScrollLeft(int amount)
		{
		}

		public void LogListViewScrollRight(int amount)
		{
		}

		#endregion
	}
}
