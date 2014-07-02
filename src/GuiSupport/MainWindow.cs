using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Threading;
using System.Xml;
using System.Xml.Schema;
using System.Xml.XPath;
using System.Reflection;
using Microsoft.Win32;
using FunctionDatabase;
using HoloScript;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainWindow : System.Windows.Forms.Form
	{
		#region Member Variable Declarations

		private string SavedProjectFileName;

		// contains path to a project file taken as a cmd line arg
		private string cmdLineFile = null;

		private System.Windows.Forms.Panel leftPanel;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.MenuItem fileNewProjectMenuItem0;
		private System.Windows.Forms.MenuItem fileAddTestApplicationMenuItem;
		private System.Windows.Forms.MenuItem fileNewProjectMenuItem3;
		private System.Windows.Forms.MenuItem menuItem39;
		private System.Windows.Forms.MenuItem menuItem41;
		private System.Windows.Forms.MenuItem logFilterMenuItem;
		private System.Windows.Forms.MenuItem applicationMenu;
		private System.Windows.Forms.MenuItem helpMenu;
		private System.Windows.Forms.MenuItem applicationPauseMenuItem;
		private System.Windows.Forms.MenuItem applicationStopMenuItem;
		private System.Windows.Forms.MenuItem helpTopicsMenuItem;
		private System.Windows.Forms.MenuItem helpIndexMenuItem;
		private System.Windows.Forms.MenuItem helpDistributionSiteMenuItem;
		private System.Windows.Forms.MenuItem helpReportABugMenuItem;
		private System.Windows.Forms.MenuItem helpAboutMenuItem;
		private System.Windows.Forms.MenuItem logMenu;
		private System.Windows.Forms.MenuItem editMenu;
		private System.Windows.Forms.MenuItem editCopyMenuItem;
		private System.Windows.Forms.MenuItem viewMenu;
		private System.Windows.Forms.MenuItem viewTinyModeMenuItem;
		private System.Windows.Forms.MenuItem viewFullModeMenuItem;
		private System.Windows.Forms.MenuItem windowMenu;
		private System.Windows.Forms.MenuItem fileMenu;
		private System.Windows.Forms.MenuItem fileOpenProjectMenuItem;
		private System.Windows.Forms.MenuItem fileOpenLogMenuItem;
		private System.Windows.Forms.MenuItem fileSaveLogMenuItem;
		private System.Windows.Forms.MenuItem filePrintMenuItem;
		private System.Windows.Forms.MenuItem fileExitMenuItem;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage diskFaultsTabPage;
		private System.Windows.Forms.TabPage memoryFaultsTabPage;
		private System.Windows.Forms.TabPage networkFaultsTabPage;
		private System.Windows.Forms.CheckBox chkbx_DiskDirectoryNotEmpty;
		private System.Windows.Forms.CheckBox chkbx_DiskCannotBeRemoved;
		private System.Windows.Forms.CheckBox chkbx_DiskCorruptStructure;
		private System.Windows.Forms.CheckBox chkbx_DiskPathInvalid;
		private System.Windows.Forms.CheckBox chkbx_DiskLocked;
		private System.Windows.Forms.CheckBox chkbx_DiskFileInUse;
		private System.Windows.Forms.CheckBox chkbx_DiskDriveCannotSeekDisk;
		private System.Windows.Forms.CheckBox chkbx_DiskPathNotFound;
		private System.Windows.Forms.CheckBox chkbx_DiskReadOnlyDirectory;
		private System.Windows.Forms.CheckBox chkbx_DiskNameTooLong;
		private System.Windows.Forms.CheckBox chkbx_DiskCannotBeCreated;
		private System.Windows.Forms.CheckBox chkbx_DiskAlreadyExists;
		private System.Windows.Forms.CheckBox chkbx_DiskDiskFull;
		private System.Windows.Forms.CheckBox chkbx_DiskCrcDataError;
		private System.Windows.Forms.CheckBox chkbx_DiskWriteProtected;
		private System.Windows.Forms.CheckBox chkbx_DiskAccessDenied;
		private System.Windows.Forms.CheckBox chkbx_DiskCannotAccess;
		private System.Windows.Forms.CheckBox chkbx_DiskFileNotFound;
		private System.Windows.Forms.NumericUpDown diskLimitNumber;
		private System.Windows.Forms.ComboBox diskLimitScale;
		private System.Windows.Forms.CheckBox chkbx_MemoryInvalidAccess;
		private System.Windows.Forms.CheckBox chkbx_MemoryInsufficientMemory;
		private System.Windows.Forms.CheckBox chkbx_MemoryPagingFileTooSmall;
		private System.Windows.Forms.CheckBox chkbx_MemoryInvalidAddress;
		private System.Windows.Forms.CheckBox chkbx_MemorySegmentIsLocked;
		private System.Windows.Forms.ComboBox memoryLimitScale;
		private System.Windows.Forms.NumericUpDown memoryLimitNumber;
		private System.Windows.Forms.CheckBox chkbx_NetworkWrongWinsock;
		private System.Windows.Forms.CheckBox chkbx_NetworkNoPorts;
		private System.Windows.Forms.CheckBox chkbx_NetworkNetworkDown;
		private System.Windows.Forms.CheckBox chkbx_NetworkDisconnect;
		private System.Windows.Forms.CheckBox chkbx_NetworkNotInstalled;
		private System.Windows.Forms.CheckBox chkbx_NetworkTaskLimitReached;
		private System.Windows.Forms.TrackBar trkb_Network;
		private System.Windows.Forms.MainMenu mainMenu;
		private System.Windows.Forms.Splitter mainSplitter;
		private SplashScreen splashScreen;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem viewAlwaysOnTopMenuItem;
		private System.Windows.Forms.ComboBox applicationsComboBox;
		private int fullModeWidth;
		private System.Windows.Forms.Timer splashScreenTimer;
		private ArrayList mdiWindowList;
		private System.Windows.Forms.HelpProvider mainHelpProvider;
		private System.Windows.Forms.CheckBox chkbx_DiskSpaceLimit;
		private System.Windows.Forms.CheckBox chkbx_MemoryLimit;
		private System.Windows.Forms.CheckBox chkbx_ThrottleNetwork;
		private System.Windows.Forms.MenuItem fileNewProjectMenuItem;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuItem4;
		private System.Windows.Forms.MenuItem viewPreferencesMenuItem;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem applicationCreateATestMenuItem;
		private System.Windows.Forms.Label label_throttleLevel;
		private System.Windows.Forms.MenuItem fileSaveProjectMenuItem;
		private System.Windows.Forms.MenuItem fileSaveProjectAsMenuItem;
		private System.Windows.Forms.MenuItem fileSaveLogAsMenuItem;
		private System.Windows.Forms.MenuItem fileCloseProjectMenuItem;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem logFindMenuItem;
		private System.Windows.Forms.MenuItem logFindNextMenuItem;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.Label label_NetworkPercent;

		#endregion

		#region properties 

		public String CommandLineFile
		{
			get { return this.cmdLineFile; }
			set { this.cmdLineFile = value; }
		}

		public bool HasSavedProjectFile
		{
			get
			{
				if (SavedProjectFileName != "")
					return true;
				else
					return false;
			}
				
		}

		#endregion

		#region Non-Event Driven Methods

		public MainWindow()
		{
			splashScreen = new SplashScreen();
			splashScreen.Show();

			mdiWindowList = new ArrayList();

			try
			{
				RegistryHelper helper = RegistryHelper.getInstance();
				String databaseFolder = helper.GetFunctionDatabaseFolderPath();
				InterceptedFunctionDB.GetInstance("functions.xml").LoadDatabase(databaseFolder, "functions.xml");
				FaultFunctionDB.GetInstance().LoadDatabase(databaseFolder);
			}
			catch(Exception e)
			{
				String message = "There was an error in the function database:\n" + e.Message;
				message += "\n\nThe application will now exit.";

				MessageBox.Show(this, message, "Holodeck Error");
				System.Environment.Exit(-1);
			}

			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			splashScreenTimer.Start();
			viewFullModeMenuItem.Checked = true;
			this.SavedProjectFileName = "";
			mainHelpProvider.HelpNamespace = RegistryHelper.getInstance().GetInstallationPath() + "\\HolodeckEE.chm";
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		private ProcessMdiWindow GetActiveChildWindow()
		{
			return (ProcessMdiWindow)this.ActiveMdiChild;
		}

		private void UpdateFaultsForActiveWindow()
		{
			ProcessMdiWindow window = this.GetActiveChildWindow();

			if (window != null)
			{
				chkbx_DiskFileNotFound.Checked = window.InjectDiskFileNotFound;
				chkbx_DiskCannotAccess.Checked = window.InjectDiskCannotAccess;
				chkbx_DiskAccessDenied.Checked = window.InjectDiskAccessDenied;
				chkbx_DiskWriteProtected.Checked = window.InjectDiskWriteProtected;
				chkbx_DiskCrcDataError.Checked = window.InjectDiskCrcDataError;
				chkbx_DiskDriveCannotSeekDisk.Checked = window.InjectDiskDriveCannotSeekDisk;
				chkbx_DiskFileInUse.Checked = window.InjectDiskFileInUse;
				chkbx_DiskLocked.Checked = window.InjectDiskLocked;
				chkbx_DiskDiskFull.Checked = window.InjectDiskDiskFull;
				chkbx_DiskAlreadyExists.Checked = window.InjectDiskAlreadyExists;
				chkbx_DiskCannotBeCreated.Checked = window.InjectDiskCannotBeCreated;
				chkbx_DiskNameTooLong.Checked = window.InjectDiskNameTooLong;
				chkbx_DiskReadOnlyDirectory.Checked = window.InjectDiskReadOnlyDirectory;
				chkbx_DiskPathNotFound.Checked = window.InjectDiskPathNotFound;
				chkbx_DiskCannotBeRemoved.Checked = window.InjectDiskCannotBeRemoved;
				chkbx_DiskDirectoryNotEmpty.Checked = window.InjectDiskDirectoryNotEmpty;
				chkbx_DiskPathInvalid.Checked = window.InjectDiskPathInvalid;
				chkbx_DiskCorruptStructure.Checked = window.InjectDiskCorruptStructure;
				chkbx_MemoryInsufficientMemory.Checked = window.InjectMemoryInsufficientMemory;
				chkbx_MemoryInvalidAccess.Checked = window.InjectMemoryInvalidAccess;
				chkbx_MemorySegmentIsLocked.Checked = window.InjectMemorySegmentIsLocked;
				chkbx_MemoryInvalidAddress.Checked = window.InjectMemoryInvalidAddress;
				chkbx_MemoryPagingFileTooSmall.Checked = window.InjectMemoryPagingFileTooSmall;
				chkbx_NetworkDisconnect.Checked = window.InjectNetworkDisconnect;
				chkbx_NetworkNotInstalled.Checked = window.InjectNetworkNotInstalled;
				chkbx_NetworkWrongWinsock.Checked = window.InjectNetworkWrongWinsock;
				chkbx_NetworkTaskLimitReached.Checked = window.InjectNetworkTaskLimitReached;
				chkbx_NetworkNoPorts.Checked = window.InjectNetworkNoPorts;
				chkbx_NetworkNetworkDown.Checked = window.InjectNetworkNetworkDown;

				chkbx_DiskSpaceLimit.Checked = window.LimitDiskUsage;
				chkbx_MemoryLimit.Checked = window.LimitMemoryUsage;
				chkbx_ThrottleNetwork.Checked = window.LimitNetworkSpeed;

				this.diskLimitScale.SelectedItem = window.MaxDiskUsageUnit;
				this.memoryLimitScale.SelectedItem = window.MaxMemoryUsageUnit;

				this.diskLimitNumber.Value = window.MaxDiskUsage;
				this.memoryLimitNumber.Value = window.MaxMemoryUsage;
				this.trkb_Network.Value = (int)window.MaxNetworkSpeed;
			}
		}

		/// <summary>
		/// Clears the project settings
		///		- unchecks all fault checkboxes
		///		- sets all limits to zero
		///		- clears the saved project filename
		/// </summary>
		private void ClearProjectSettings()
		{
			chkbx_DiskDirectoryNotEmpty.Checked = false;
			chkbx_DiskCannotBeRemoved.Checked = false;
			chkbx_DiskCorruptStructure.Checked = false;
			chkbx_DiskPathInvalid.Checked = false;
			chkbx_DiskLocked.Checked = false;
			chkbx_DiskFileInUse.Checked = false;
			chkbx_DiskDriveCannotSeekDisk.Checked = false;
			chkbx_DiskPathNotFound.Checked = false;
			chkbx_DiskReadOnlyDirectory.Checked = false;
			chkbx_DiskNameTooLong.Checked = false;
			chkbx_DiskCannotBeCreated.Checked = false;
			chkbx_DiskAlreadyExists.Checked = false;
			chkbx_DiskDiskFull.Checked = false;
			chkbx_DiskCrcDataError.Checked = false;
			chkbx_DiskWriteProtected.Checked = false;
			chkbx_DiskAccessDenied.Checked = false;
			chkbx_DiskCannotAccess.Checked = false;
			chkbx_DiskFileNotFound.Checked = false;
			chkbx_MemoryInvalidAccess.Checked = false;
			chkbx_MemoryInsufficientMemory.Checked = false;
			chkbx_MemoryPagingFileTooSmall.Checked = false;
			chkbx_MemoryInvalidAddress.Checked = false;
			chkbx_MemorySegmentIsLocked.Checked = false;
			chkbx_NetworkWrongWinsock.Checked = false;
			chkbx_NetworkNoPorts.Checked = false;
			chkbx_NetworkNetworkDown.Checked = false;
			chkbx_NetworkDisconnect.Checked = false;
			chkbx_NetworkNotInstalled.Checked = false;
			chkbx_NetworkTaskLimitReached.Checked = false;
			chkbx_DiskSpaceLimit.Checked = false;
			chkbx_MemoryLimit.Checked = false;
			chkbx_ThrottleNetwork.Checked = false;
		
			this.memoryLimitScale.Text = "KB";
			this.diskLimitScale.Text = "KB";
			this.diskLimitNumber.Value = 0;
			this.memoryLimitNumber.Value = 0;
			this.trkb_Network.Value = 100;
			this.label_throttleLevel.Text = "100 %";
		
			this.SavedProjectFileName = "";
		}

		/// <summary>
		/// Sets the appropriate checkbox in the ui
		/// </summary>
		private void SetCheckBox(string Name)
		{

			//bugbug - would rather use reflection but can't get it to work.
			//FieldInfo windowField = this.GetType().GetField(attributeValue, BindingFlags.NonPublic|BindingFlags.Instance);
			//test1
			//this.GetType().InvokeMember(windowField.Name, BindingFlags.SetField, null, this, new Object[] {true});
			//test2
			//PropertyInfo chkbxProperty = windowField.GetType().GetProperty("Checked", BindingFlags.Public|BindingFlags.Instance);
			//windowField.GetType().InvokeMember(chkbxProperty.Name, BindingFlags.SetProperty, null, this, new object[] {true});

			switch (Name)
			{
				case "chkbx_DiskDirectoryNotEmpty":
					chkbx_DiskDirectoryNotEmpty.Checked = true;
					break;
				case "chkbx_DiskCannotBeRemoved":
					chkbx_DiskCannotBeRemoved.Checked = true;
					break;
				case "chkbx_DiskCorruptStructure":
					chkbx_DiskCorruptStructure.Checked = true;
					break;
				case "chkbx_DiskPathInvalid":
					chkbx_DiskPathInvalid.Checked = true;
					break;
				case "chkbx_DiskLocked":
					chkbx_DiskLocked.Checked = true;
					break;
				case "chkbx_DiskFileInUse":
					chkbx_DiskFileInUse.Checked = true;
					break;
				case "chkbx_DiskDriveCannotSeekDisk":
					chkbx_DiskDriveCannotSeekDisk.Checked = true;
					break;
				case "chkbx_DiskPathNotFound":
					chkbx_DiskPathNotFound.Checked = true;
					break;
				case "chkbx_DiskReadOnlyDirectory":
					chkbx_DiskReadOnlyDirectory.Checked = true;
					break;
				case "chkbx_DiskNameTooLong":
					chkbx_DiskNameTooLong.Checked = true;
					break;
				case "chkbx_DiskCannotBeCreated":
					chkbx_DiskCannotBeCreated.Checked = true;
					break;
				case "chkbx_DiskAlreadyExists":
					chkbx_DiskAlreadyExists.Checked = true;
					break;
				case "chkbx_DiskDiskFull":
					chkbx_DiskDiskFull.Checked = true;
					break;
				case "chkbx_DiskCrcDataError":
					chkbx_DiskCrcDataError.Checked = true;
					break;
				case "chkbx_DiskWriteProtected":
					chkbx_DiskWriteProtected.Checked = true;
					break;
				case "chkbx_DiskAccessDenied":
					chkbx_DiskAccessDenied.Checked = true;
					break;
				case "chkbx_DiskCannotAccess":
					chkbx_DiskCannotAccess.Checked = true;
					break;
				case "chkbx_DiskFileNotFound":
					chkbx_DiskFileNotFound.Checked = true;
					break;
				case "chkbx_MemoryInvalidAccess":
					chkbx_MemoryInvalidAccess.Checked = true;
					break;
				case "chkbx_MemoryInsufficientMemory":
					chkbx_MemoryInsufficientMemory.Checked = true;
					break;
				case "chkbx_MemoryPagingFileTooSmall":
					chkbx_MemoryPagingFileTooSmall.Checked = true;
					break;
				case "chkbx_MemoryInvalidAddress":
					chkbx_MemoryInvalidAddress.Checked = true;
					break;
				case "chkbx_MemorySegmentIsLocked":
					chkbx_MemorySegmentIsLocked.Checked = true;
					break;
				case "chkbx_NetworkWrongWinsock":
					chkbx_NetworkWrongWinsock.Checked = true;
					break;
				case "chkbx_NetworkNoPorts":
					chkbx_NetworkNoPorts.Checked = true;
					break;
				case "chkbx_NetworkNetworkDown":
					chkbx_NetworkNetworkDown.Checked = true;
					break;
				case "chkbx_NetworkDisconnect":
					chkbx_NetworkDisconnect.Checked = true;
					break;
				case "chkbx_NetworkNotInstalled":
					chkbx_NetworkNotInstalled.Checked = true;
					break;
				case "chkbx_NetworkTaskLimitReached":
					chkbx_NetworkTaskLimitReached.Checked = true;
					break;
				case "chkbx_DiskSpaceLimit":
					chkbx_DiskSpaceLimit.Checked = true;
					break;
				case "chkbx_MemoryLimit":
					chkbx_MemoryLimit.Checked = true;
					break;
				case "chkbx_ThrottleNetwork":
					chkbx_ThrottleNetwork.Checked = true;
					break;
			}
		}


		private void OpenLogFile(String logFileName)
		{
			if (logFileName == null)
				return;

			ProcessMdiWindow window = new ProcessMdiWindow(logFileName);
			window.MdiParent = this;
			window.Show();
			mdiWindowList.Add(window);
		}


		private bool VerifyDiskLimitBounds(bool displayMessage)
		{
			foreach (ProcessMdiWindow window in mdiWindowList)
			{
				ulong diskUsed = 0, memUsed = 0;
				int ulMultiplier = (int) System.Math.Pow (1024, diskLimitScale.SelectedIndex);
				window.GetUsageInfo (ref memUsed, ref diskUsed);

				//Check to see if this limit is greater than the currently used amount
				if (diskLimitNumber.Value *  ulMultiplier < diskUsed)
				{
					if (displayMessage)
					{
						string diskUsedInfo = "";
						if (diskUsed < 1024)
							diskUsedInfo = diskUsed.ToString() + " bytes";
						else
						{
							double _diskUsed = Convert.ToDouble (diskUsed) / 1024;
							Math.Round (_diskUsed, 2);
							diskUsedInfo = Convert.ToString (_diskUsed.ToString() + " KB");
						}
						MessageBox.Show ("The application under test (" + window.Text + ") has already used atleast " + diskUsedInfo + " of disk space, please restrict to a size greater than that.", "Disk Limit");
					}
					return false;
				}
				//Check to see if this limit is less than 4GB
				if (diskLimitNumber.Value *  ulMultiplier > 4 * (long)System.Math.Pow (1024, 3))
				{
					if (displayMessage)
						MessageBox.Show ("The maximum limit supported is 4GB, please choose a lower value", "Disk Limit");	
					return false;
				}
			}

			return true;
		}


		private bool VerifyMemoryLimitBounds(bool displayMessage)
		{
			foreach (ProcessMdiWindow window in mdiWindowList)			
			{
				ulong diskUsed = 0, memUsed = 0;
				int ulMultiplier = (int) System.Math.Pow (1024, memoryLimitScale.SelectedIndex);
				window.GetUsageInfo (ref memUsed, ref diskUsed);

				//Check to see if this limit is greater than the currently used amount
				if (memoryLimitNumber.Value *  ulMultiplier < memUsed)
				{
					if (displayMessage)
					{
						string memUsedInfo = "";
						if (memUsed < 1024)
							memUsedInfo = memUsed.ToString() + " bytes";
						else
						{
							double _memUsed = Convert.ToDouble (memUsed) / 1024;
							Math.Round (_memUsed, 2);
							memUsedInfo = Convert.ToString (_memUsed.ToString() + " KB");
						}
						MessageBox.Show ("The application under test (" + window.Text + ") has already used atleast " + memUsedInfo + " of memory, please restrict to a size greater than that.", "Memory Limit");
					}
					return false;
				}
				//Check to see if this limit is less than 4GB
				if (memoryLimitNumber.Value *  ulMultiplier >  4 * (long)System.Math.Pow (1024, 3))
				{
					if (displayMessage)
						MessageBox.Show ("The maximum limit supported is 4GB, please choose a lower value", "Memory Limit");	
					return false;
				}
			}

			return true;
		}

		//*************************************************************************
		// Method:	 SaveProject
		//
		// Description: Saves project settings to an XML file.
		//
		// Parameters:
		//	fileName - file to save project to.
		//	useExistingFilename - if true, fileName param. is ignored and the existing
		//  filename is used
		//
		// Return Value: none
		//*************************************************************************
		public void SaveProject(String fileName, bool useExistingFilename)
		{
			if (!useExistingFilename)
				SavedProjectFileName = fileName;

			InterceptedFunctionDB db = InterceptedFunctionDB.GetInstance("functions.xml");

			RegistryHelper helper = RegistryHelper.getInstance();
			string dtdName = helper.GetDTDFolder();
			if (dtdName == null)
				dtdName = "projectfile.dtd";
			else if (dtdName.EndsWith("\\"))
				dtdName = string.Concat(dtdName, "projectfile.dtd");
			else
				dtdName = string.Concat(dtdName, "\\projectfile.dtd");
		
			XmlTextWriter xwriter = new XmlTextWriter(SavedProjectFileName,System.Text.Encoding.UTF8);
			xwriter.Formatting = Formatting.Indented;
			xwriter.Indentation = 4;
			xwriter.WriteStartDocument(false);
			xwriter.WriteDocType ("Project", null, dtdName, null);
			xwriter.WriteStartElement ("Project");
			
			foreach(ProcessMdiWindow window in mdiWindowList)
			{
				xwriter.WriteStartElement ("ApplicationUnderTest");

				xwriter.WriteStartElement ("AppInfo");
				xwriter.WriteAttributeString ("ApplicationPathAndName", window.ProcessFile);
				if (window.UseProcessFile)
					xwriter.WriteAttributeString ("InjectMode", "LAUNCH");
				else
					xwriter.WriteAttributeString ("InjectMode", "ATTACH");
				xwriter.WriteAttributeString ("StartPaused", window.AppPaused.ToString());
				xwriter.WriteFullEndElement (); //End for AppInfo

				xwriter.WriteStartElement ("FilteredFunctions");
				foreach (string fName in window.FilteredOutFunctions)
				{
					InterceptedFunction function = db.GetFunctionByName(fName);
					if (function != null)
					{
						xwriter.WriteStartElement ("FilteredFunction");
						xwriter.WriteAttributeString ("FunctionName", function.Name);
						xwriter.WriteFullEndElement();
					}
				}
				xwriter.WriteFullEndElement(); //end for FilteredFunctions

				xwriter.WriteStartElement ("Faults");
				xwriter.WriteStartElement ("DiskFaults");
				EnumerateCheckAndSaveFaults ("Disk", ref xwriter, window);
				xwriter.WriteFullEndElement();

				xwriter.WriteStartElement ("MemoryFaults");
				EnumerateCheckAndSaveFaults ("Memory", ref xwriter, window);
				xwriter.WriteFullEndElement();

				xwriter.WriteStartElement ("NetworkFaults");
				EnumerateCheckAndSaveFaults ("Network", ref xwriter, window);
				xwriter.WriteFullEndElement();
			
				xwriter.WriteFullEndElement(); //End for "Faults"

				xwriter.WriteStartElement ("Limits");

				xwriter.WriteStartElement ("DiskSpaceLimit");
				xwriter.WriteAttributeString ("LimitActivated", window.LimitDiskUsage.ToString());
				xwriter.WriteAttributeString ("Limit", window.MaxDiskUsage.ToString());
				xwriter.WriteAttributeString ("LimitUnit", window.MaxDiskUsageUnit);
				xwriter.WriteFullEndElement();

				xwriter.WriteStartElement ("MemorySpaceLimit");
				xwriter.WriteAttributeString ("LimitActivated", window.LimitMemoryUsage.ToString());
				xwriter.WriteAttributeString ("Limit", window.MaxMemoryUsage.ToString());
				xwriter.WriteAttributeString ("LimitUnit", window.MaxMemoryUsageUnit);
				xwriter.WriteFullEndElement();

				xwriter.WriteStartElement ("NetworkThrottle");
				xwriter.WriteAttributeString ("ThrottleActivated", window.LimitNetworkSpeed.ToString());
				xwriter.WriteAttributeString ("ThrottlePercent", window.MaxNetworkSpeed.ToString());
				xwriter.WriteFullEndElement();
	
				xwriter.WriteFullEndElement();		//End for "Limits"

				xwriter.WriteStartElement ("ScheduledTests");
				foreach (ListViewItem item in window.GetScheduledTests())
				{
					InterceptedFunction f = InterceptedFunctionDB.GetInstance("functions.xml").GetFunctionByName(item.SubItems[1].Text);
					if (f != null)
					{
						xwriter.WriteStartElement ("ScheduledTest");
						xwriter.WriteAttributeString("TestID",item.SubItems[0].Text);
						xwriter.WriteAttributeString("FunctionName", f.Name);
						xwriter.WriteAttributeString("Category", f.Category);
						xwriter.WriteAttributeString("ErrorCode", item.SubItems[2].Text);
						xwriter.WriteAttributeString("ReturnValue", item.SubItems[3].Text);
						xwriter.WriteAttributeString("Parameters", item.SubItems[4].Text);
						xwriter.WriteFullEndElement();
					}
				}
				xwriter.WriteFullEndElement();		//End for "ScheduledTests"

				xwriter.WriteFullEndElement();	//end for ApplicationUnderTest
			}

			xwriter.WriteFullEndElement();		//End for "Project"

			xwriter.WriteEndDocument ();
			xwriter.Flush();
			xwriter.Close();
		}

		//*************************************************************************
		// Method:	 EnumerateCheckAndSaveFaults
		//
		// Description: Uses Reflection to enumerate all Faults in each specified category
		//				and checks to see if they are enabled, if so, they are saved.
		// Parameters:
		//	categoryName - the category name for which to enumerate and check faults
		//	xwriter - reference to an existing XmlTextWriter object which is used to write
		//
		// Return Value: none
		//*************************************************************************
		private void EnumerateCheckAndSaveFaults (String categoryName, ref XmlTextWriter xwriter, ProcessMdiWindow window)
		{
			foreach (PropertyInfo pi in window.GetType().GetProperties())
			{
				if (pi.Name.StartsWith ("Inject" + categoryName))
				{
					bool IsFaultInjected = (bool) window.GetType().InvokeMember (pi.Name, BindingFlags.GetProperty, null, window, null);
					if (IsFaultInjected)
					{
						xwriter.WriteStartElement (categoryName + "Fault");
						xwriter.WriteAttributeString ("Name", pi.Name.Replace("Inject" + categoryName, ""));
						xwriter.WriteFullEndElement ();
					}
				}
			}

		}

		#endregion

		#region Open Project code

		private void ParseAppInfo(XPathNavigator navigator, AbstractLaunchWizard wizard)
		{
			XPathNavigator nav = navigator.Clone();

			bool hasMoreAttributes = nav.MoveToFirstAttribute();
			bool atLeastOne = hasMoreAttributes;
			string attributeValue;

			while (hasMoreAttributes)
			{
				switch (nav.Name)
				{
					case "ApplicationPathAndName":
						attributeValue = nav.Value;
						if (attributeValue != null)
							wizard.ProcessPath = attributeValue;
						break;
					case "InjectMode":
						attributeValue = nav.Value;
						if ((attributeValue != null) && (attributeValue.CompareTo("LAUNCH") == 0))
							wizard.UseProcessFile = true;
						else
							wizard.UseProcessID = true;
						break;
					case "StartPaused":
						attributeValue = nav.Value;
						if (attributeValue != null)
							wizard.AppPaused = (attributeValue.CompareTo("True") == 0);
						break;
				}

				hasMoreAttributes = nav.MoveToNextAttribute();
			}

			if (atLeastOne)
				nav.MoveToParent();
		}

		private void ParseFilteredFunctions(XPathNavigator navigator, AbstractLaunchWizard wizard)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreFunctions = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreFunctions;
			string attributeValue;
			bool hasMoreAttributes;

			while (hasMoreFunctions)
			{
				switch (nav.Name)
				{
					case "FilteredFunction":
						hasMoreAttributes = nav.MoveToFirstAttribute();

						while (hasMoreAttributes)
						{
							switch (nav.Name)
							{
								case "FunctionName":
									attributeValue = nav.Value;
									if (attributeValue != null)
										wizard.FilteredOutFunctions.Add(attributeValue);
									break;
							}
						
							hasMoreAttributes = nav.MoveToNextAttribute();
						}
						nav.MoveToParent();
						break;
				}

				hasMoreFunctions = nav.MoveToNext();
			}

			if (atLeastOne)
				nav.MoveToParent();
		}

		private void ParseDiskFaults(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreFunctions = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreFunctions;
			bool hasMoreAttributes = false;
			string attributeValue;

			while (hasMoreFunctions)
			{
				switch (nav.Name)
				{
					case "DiskFault":
						hasMoreAttributes = nav.MoveToFirstAttribute();

						while (hasMoreAttributes)
						{
							switch (nav.Name)
							{
								case "Name":
									if (nav.Value != null)
									{
										attributeValue = "chkbx_Disk";
										attributeValue += nav.Value;
										SetCheckBox(attributeValue);
									}
									break;
							}
						
							hasMoreAttributes = nav.MoveToNextAttribute();
						}
						nav.MoveToParent();
						break;
				}

				hasMoreFunctions = nav.MoveToNext();
			}

			if (atLeastOne)
				nav.MoveToParent();
		}

		private void ParseMemoryFaults(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreFunctions = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreFunctions;
			bool hasMoreAttributes = false;
			string attributeValue;

			while (hasMoreFunctions)
			{
				switch (nav.Name)
				{
					case "MemoryFault":
						hasMoreAttributes = nav.MoveToFirstAttribute();

						while (hasMoreAttributes)
						{
							switch (nav.Name)
							{
								case "Name":
									if (nav.Value != null)
									{
										attributeValue = "chkbx_Memory";
										attributeValue += nav.Value;
										SetCheckBox(attributeValue);
									}
									break;
							}
						
							hasMoreAttributes = nav.MoveToNextAttribute();
						}
						nav.MoveToParent();
						break;
				}

				hasMoreFunctions = nav.MoveToNext();
			}

			if (atLeastOne)
				nav.MoveToParent();
		}

		private void ParseNetworkFaults(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreFunctions = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreFunctions;
			bool hasMoreAttributes = false;
			string attributeValue;

			while (hasMoreFunctions)
			{
				switch (nav.Name)
				{
					case "NetworkFault":
						hasMoreAttributes = nav.MoveToFirstAttribute();

						while (hasMoreAttributes)
						{
							switch (nav.Name)
							{
								case "Name":
									if (nav.Value != null)
									{
										attributeValue = "chkbx_Network";
										attributeValue += nav.Value;
										SetCheckBox(attributeValue);
									}
									break;
							}
						
							hasMoreAttributes = nav.MoveToNextAttribute();
						}
						nav.MoveToParent();
						break;
				}

				hasMoreFunctions = nav.MoveToNext();
			}

			if (atLeastOne)
				nav.MoveToParent();
		}

		private void ParseFaults(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreFaults = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreFaults;

			while (hasMoreFaults)
			{
				switch (nav.Name)
				{
					case "DiskFaults":
						ParseDiskFaults(nav);
						break;

					case "MemoryFaults":
						ParseMemoryFaults(nav);
						break;

					case "NetworkFaults":
						ParseNetworkFaults(nav);
						break;
				}

				hasMoreFaults = nav.MoveToNext();
			}
			if (atLeastOne)
				nav.MoveToParent();
		}
		
		private void ParseDiskSpaceLimit(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreAttributes = nav.MoveToFirstAttribute();
			bool atLeastOne = hasMoreAttributes;

			string activated = null, limit = null, unit = null;

			while (hasMoreAttributes)
			{
				switch (nav.Name)
				{
					case "LimitActivated":
						activated = nav.Value;
						break;
					case "Limit":
						limit = nav.Value;
						break;
					case "LimitUnit":
						unit = nav.Value;
						break;
				}
						
				hasMoreAttributes = nav.MoveToNextAttribute();
			}
			
			if (atLeastOne)
				nav.MoveToParent();

			if ((activated != null) && (limit != null) && (unit != null) && (activated.CompareTo("True") == 0))
			{
				this.diskLimitNumber.Value = Convert.ToDecimal(limit);
				int index = this.diskLimitScale.Items.IndexOf(unit);
				this.diskLimitScale.SelectedIndex = index;
				this.chkbx_DiskSpaceLimit.Checked = true;
			}
		}

		private void ParseMemorySpaceLimit(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreAttributes = nav.MoveToFirstAttribute();
			bool atLeastOne = hasMoreAttributes;

			string activated = null, limit = null, unit = null;

			while (hasMoreAttributes)
			{
				switch (nav.Name)
				{
					case "LimitActivated":
						activated = nav.Value;
						break;
					case "Limit":
						limit = nav.Value;
						break;
					case "LimitUnit":
						unit = nav.Value;
						break;
				}
						
				hasMoreAttributes = nav.MoveToNextAttribute();
			}

			if (hasMoreAttributes)
				nav.MoveToParent();	

			if ((activated != null) && (limit != null) && (unit != null) && (activated.CompareTo("True") == 0))
			{
				this.memoryLimitNumber.Value = Convert.ToDecimal(limit);
				int index = this.memoryLimitScale.Items.IndexOf(unit);
				this.memoryLimitScale.SelectedIndex = index;
				this.chkbx_MemoryLimit.Checked = true;
			}
		}

		private void ParseNetworkLimit(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreAttributes = nav.MoveToFirstAttribute();
			bool atLeastOne = hasMoreAttributes;

			string activated = null, percent = null;

			while (hasMoreAttributes)
			{
				switch (nav.Name)
				{
					case "ThrottleActivated":
						activated = nav.Value;
						break;
					case "ThrottlePercent":
						percent = nav.Value;
						break;
				}
						
				hasMoreAttributes = nav.MoveToNextAttribute();
			}

			if (atLeastOne)
				nav.MoveToParent();

			if ((activated != null) && (percent != null) && (activated.CompareTo("True") == 0))
			{
				this.trkb_Network.Value = Convert.ToInt16(percent, 10);
				this.label_throttleLevel.Text = percent + " %";
				this.chkbx_ThrottleNetwork.Checked = true;
			}
		}

		private void ParseLimits(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreLimits = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreLimits;

			while (hasMoreLimits)
			{
				switch (nav.Name)
				{
					case "DiskSpaceLimit":
					{
						ParseDiskSpaceLimit(nav);
						break;
					}
					case "MemorySpaceLimit":
					{
						ParseMemorySpaceLimit(nav);
						break;
					}
					case "NetworkThrottle":
					{
						ParseNetworkLimit(nav);
						break;
					}
				}

				hasMoreLimits = nav.MoveToNext();
			}
			if (atLeastOne)
				nav.MoveToParent();
		}

		private Queue ParseScheduledTests(XPathNavigator navigator)
		{
			XPathNavigator nav = navigator.Clone();
			bool hasMoreTests = nav.MoveToFirstChild();
			bool atLeastOne = hasMoreTests;
			bool hasMoreAttributes;
			Queue returnValue = new Queue();
			if (returnValue == null)
				return null;

			while (hasMoreTests)
			{
				switch (nav.Name)
				{
					case "ScheduledTest":
						hasMoreAttributes = nav.MoveToFirstAttribute();

						while (hasMoreAttributes)
						{
							switch (nav.Name)
							{
								case "TestID":
									break;
								case "FunctionName":
									break;
								case "Category":
									break;
								case "ErrorCode":
									break;
								case "ReturnValue":
									break;
								case "Parameters":
									break;
							}
						
							hasMoreAttributes = nav.MoveToNextAttribute();
						}
						nav.MoveToParent();

						break;
				}

				hasMoreTests = nav.MoveToNext();
			}

			if (atLeastOne)
				nav.MoveToParent();

			return returnValue;
		}

		private void ParseAUTPre(XPathNavigator navigator, AbstractLaunchWizard wizard)
		{
			XPathNavigator nav = navigator.Clone();

			bool hasMoreElements = false;
			
			if (nav.HasChildren)
			{
				hasMoreElements = true;
				nav.MoveToFirstChild();
			}

			while (hasMoreElements)
			{
				switch(nav.Name)
				{
					case "AppInfo":
						ParseAppInfo(nav, wizard);
						break;
					case "FilteredFunctions":
						ParseFilteredFunctions(nav, wizard);
						break;
				}

				hasMoreElements = nav.MoveToNext();
			}
		}

		private void ParseAUTPost(XPathNavigator navigator, ProcessMdiWindow window)
		{
			XPathNavigator nav = navigator.Clone();

			bool hasMoreElements = false;
			
			if (nav.HasChildren)
			{
				hasMoreElements = true;
				nav.MoveToFirstChild();
			}

			while (hasMoreElements)
			{
				switch(nav.Name)
				{
					case "Faults":
						ParseFaults(nav);
						break;
					case "Limits":
						ParseLimits(nav);
						break;
					case "ScheduledTests":
					{
						Queue tests = ParseScheduledTests(nav);
						if ((window != null) && (tests != null))
						{
							
						}
						break;
					}
				}

				hasMoreElements = nav.MoveToNext();
			}
		}


		// Loads the project file, launches the aut, and then injects faults and sets limits.  
		// Start app paused so that faults and limits are on as early as possible.
		// projectFileName - path to the project file
		private void OpenProject(String projectFileName)
		{
			XmlTextReader xReader = null;
			XmlValidatingReader validator = null;

			try
			{
				xReader = new XmlTextReader(projectFileName);
				validator = new XmlValidatingReader(xReader);
				validator.ValidationType = ValidationType.DTD;
				validator.ValidationEventHandler += new ValidationEventHandler(ValidationHandler);

				// any errors will cause an XmlException to be thrown
				while (validator.Read())
				{
				}

				XPathDocument doc = new XPathDocument(projectFileName);
				XPathNavigator nav = doc.CreateNavigator();

				// move to the project tag
				nav.MoveToFirstChild();
				bool hasMoreAUTs = false;

				if (nav.HasChildren)
				{
					// move to the first aut tag
					nav.MoveToFirstChild();
					hasMoreAUTs = true;
				}

				while (hasMoreAUTs)
				{
					Wizard wizard = new Wizard();

					//Parse the wizard related project info
					ParseAUTPre(nav, wizard);

					bool appPaused = wizard.AppPaused;
					wizard.AppPaused = true;
					ProcessMdiWindow window = LaunchApplicationForTest(wizard, false);
	
					// Parse the fault, limit, and test related project info
					ParseAUTPost(nav, window);

					if (window != null)
					{
						// add scheduled tests here

						if (!appPaused)
							this.applicationPauseMenuItem_Click(null, null);
					}


					// move to the next aut tag
					hasMoreAUTs = nav.MoveToNext();
				}

				this.SavedProjectFileName = projectFileName;
			}
			finally
			{
				if (xReader != null)
					xReader.Close();
				if (validator != null)
					validator.Close();
			}
		}

		// callback that handles xml document validation errors
		// Sender - the object that caused the error
		// args - error information
		private void ValidationHandler(Object sender, ValidationEventArgs args)
		{
			throw new Exception(args.Message);
		}

		#endregion

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainWindow));
			this.leftPanel = new System.Windows.Forms.Panel();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.diskFaultsTabPage = new System.Windows.Forms.TabPage();
			this.chkbx_DiskSpaceLimit = new System.Windows.Forms.CheckBox();
			this.diskLimitScale = new System.Windows.Forms.ComboBox();
			this.diskLimitNumber = new System.Windows.Forms.NumericUpDown();
			this.chkbx_DiskDirectoryNotEmpty = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskCannotBeRemoved = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskCorruptStructure = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskPathInvalid = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskLocked = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskFileInUse = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskDriveCannotSeekDisk = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskPathNotFound = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskReadOnlyDirectory = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskNameTooLong = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskCannotBeCreated = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskAlreadyExists = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskDiskFull = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskCrcDataError = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskWriteProtected = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskAccessDenied = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskCannotAccess = new System.Windows.Forms.CheckBox();
			this.chkbx_DiskFileNotFound = new System.Windows.Forms.CheckBox();
			this.memoryFaultsTabPage = new System.Windows.Forms.TabPage();
			this.chkbx_MemoryLimit = new System.Windows.Forms.CheckBox();
			this.memoryLimitScale = new System.Windows.Forms.ComboBox();
			this.memoryLimitNumber = new System.Windows.Forms.NumericUpDown();
			this.chkbx_MemoryInvalidAccess = new System.Windows.Forms.CheckBox();
			this.chkbx_MemoryInsufficientMemory = new System.Windows.Forms.CheckBox();
			this.chkbx_MemoryPagingFileTooSmall = new System.Windows.Forms.CheckBox();
			this.chkbx_MemoryInvalidAddress = new System.Windows.Forms.CheckBox();
			this.chkbx_MemorySegmentIsLocked = new System.Windows.Forms.CheckBox();
			this.networkFaultsTabPage = new System.Windows.Forms.TabPage();
			this.label_throttleLevel = new System.Windows.Forms.Label();
			this.label_NetworkPercent = new System.Windows.Forms.Label();
			this.chkbx_ThrottleNetwork = new System.Windows.Forms.CheckBox();
			this.chkbx_NetworkWrongWinsock = new System.Windows.Forms.CheckBox();
			this.chkbx_NetworkNoPorts = new System.Windows.Forms.CheckBox();
			this.chkbx_NetworkNetworkDown = new System.Windows.Forms.CheckBox();
			this.chkbx_NetworkDisconnect = new System.Windows.Forms.CheckBox();
			this.chkbx_NetworkNotInstalled = new System.Windows.Forms.CheckBox();
			this.chkbx_NetworkTaskLimitReached = new System.Windows.Forms.CheckBox();
			this.trkb_Network = new System.Windows.Forms.TrackBar();
			this.applicationsComboBox = new System.Windows.Forms.ComboBox();
			this.mainSplitter = new System.Windows.Forms.Splitter();
			this.mainMenu = new System.Windows.Forms.MainMenu();
			this.fileMenu = new System.Windows.Forms.MenuItem();
			this.fileNewProjectMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem6 = new System.Windows.Forms.MenuItem();
			this.fileOpenProjectMenuItem = new System.Windows.Forms.MenuItem();
			this.fileSaveProjectMenuItem = new System.Windows.Forms.MenuItem();
			this.fileSaveProjectAsMenuItem = new System.Windows.Forms.MenuItem();
			this.fileCloseProjectMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.fileOpenLogMenuItem = new System.Windows.Forms.MenuItem();
			this.fileSaveLogMenuItem = new System.Windows.Forms.MenuItem();
			this.fileSaveLogAsMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem39 = new System.Windows.Forms.MenuItem();
			this.filePrintMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem41 = new System.Windows.Forms.MenuItem();
			this.fileExitMenuItem = new System.Windows.Forms.MenuItem();
			this.editMenu = new System.Windows.Forms.MenuItem();
			this.editCopyMenuItem = new System.Windows.Forms.MenuItem();
			this.applicationMenu = new System.Windows.Forms.MenuItem();
			this.applicationPauseMenuItem = new System.Windows.Forms.MenuItem();
			this.applicationStopMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.applicationCreateATestMenuItem = new System.Windows.Forms.MenuItem();
			this.logMenu = new System.Windows.Forms.MenuItem();
			this.logFindMenuItem = new System.Windows.Forms.MenuItem();
			this.logFindNextMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.logFilterMenuItem = new System.Windows.Forms.MenuItem();
			this.viewMenu = new System.Windows.Forms.MenuItem();
			this.viewTinyModeMenuItem = new System.Windows.Forms.MenuItem();
			this.viewFullModeMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.viewAlwaysOnTopMenuItem = new System.Windows.Forms.MenuItem();
			this.menuItem4 = new System.Windows.Forms.MenuItem();
			this.viewPreferencesMenuItem = new System.Windows.Forms.MenuItem();
			this.windowMenu = new System.Windows.Forms.MenuItem();
			this.helpMenu = new System.Windows.Forms.MenuItem();
			this.helpTopicsMenuItem = new System.Windows.Forms.MenuItem();
			this.helpIndexMenuItem = new System.Windows.Forms.MenuItem();
			this.fileNewProjectMenuItem0 = new System.Windows.Forms.MenuItem();
			this.fileAddTestApplicationMenuItem = new System.Windows.Forms.MenuItem();
			this.helpDistributionSiteMenuItem = new System.Windows.Forms.MenuItem();
			this.helpReportABugMenuItem = new System.Windows.Forms.MenuItem();
			this.fileNewProjectMenuItem3 = new System.Windows.Forms.MenuItem();
			this.helpAboutMenuItem = new System.Windows.Forms.MenuItem();
			this.splashScreenTimer = new System.Windows.Forms.Timer(this.components);
			this.mainHelpProvider = new System.Windows.Forms.HelpProvider();
			this.leftPanel.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.diskFaultsTabPage.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.diskLimitNumber)).BeginInit();
			this.memoryFaultsTabPage.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.memoryLimitNumber)).BeginInit();
			this.networkFaultsTabPage.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trkb_Network)).BeginInit();
			this.SuspendLayout();
			// 
			// leftPanel
			// 
			this.leftPanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.tabControl1,
																					this.applicationsComboBox});
			this.leftPanel.Dock = System.Windows.Forms.DockStyle.Left;
			this.leftPanel.Name = "leftPanel";
			this.leftPanel.Size = new System.Drawing.Size(175, 585);
			this.leftPanel.TabIndex = 1;
			this.leftPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
			// 
			// tabControl1
			// 
			this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Left;
			this.tabControl1.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.tabControl1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					  this.diskFaultsTabPage,
																					  this.memoryFaultsTabPage,
																					  this.networkFaultsTabPage});
			this.tabControl1.Multiline = true;
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(175, 584);
			this.tabControl1.TabIndex = 0;
			// 
			// diskFaultsTabPage
			// 
			this.diskFaultsTabPage.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.diskFaultsTabPage.Controls.AddRange(new System.Windows.Forms.Control[] {
																							this.chkbx_DiskSpaceLimit,
																							this.diskLimitScale,
																							this.diskLimitNumber,
																							this.chkbx_DiskDirectoryNotEmpty,
																							this.chkbx_DiskCannotBeRemoved,
																							this.chkbx_DiskCorruptStructure,
																							this.chkbx_DiskPathInvalid,
																							this.chkbx_DiskLocked,
																							this.chkbx_DiskFileInUse,
																							this.chkbx_DiskDriveCannotSeekDisk,
																							this.chkbx_DiskPathNotFound,
																							this.chkbx_DiskReadOnlyDirectory,
																							this.chkbx_DiskNameTooLong,
																							this.chkbx_DiskCannotBeCreated,
																							this.chkbx_DiskAlreadyExists,
																							this.chkbx_DiskDiskFull,
																							this.chkbx_DiskCrcDataError,
																							this.chkbx_DiskWriteProtected,
																							this.chkbx_DiskAccessDenied,
																							this.chkbx_DiskCannotAccess,
																							this.chkbx_DiskFileNotFound});
			this.diskFaultsTabPage.Location = new System.Drawing.Point(23, 4);
			this.diskFaultsTabPage.Name = "diskFaultsTabPage";
			this.diskFaultsTabPage.Size = new System.Drawing.Size(148, 576);
			this.diskFaultsTabPage.TabIndex = 0;
			this.diskFaultsTabPage.Text = "Disk Faults";
			// 
			// chkbx_DiskSpaceLimit
			// 
			this.chkbx_DiskSpaceLimit.Location = new System.Drawing.Point(8, 8);
			this.chkbx_DiskSpaceLimit.Name = "chkbx_DiskSpaceLimit";
			this.chkbx_DiskSpaceLimit.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskSpaceLimit.TabIndex = 40;
			this.chkbx_DiskSpaceLimit.Text = "Limit Disk Space to:";
			this.chkbx_DiskSpaceLimit.CheckedChanged += new System.EventHandler(this.chkbx_DiskSpaceLimit_CheckedChanged);
			// 
			// diskLimitScale
			// 
			this.diskLimitScale.Items.AddRange(new object[] {
																"B",
																"KB",
																"MB",
																"GB"});
			this.diskLimitScale.Location = new System.Drawing.Point(84, 32);
			this.diskLimitScale.Name = "diskLimitScale";
			this.diskLimitScale.Size = new System.Drawing.Size(48, 21);
			this.diskLimitScale.TabIndex = 39;
			this.diskLimitScale.Text = "KB";
			this.diskLimitScale.SelectedValueChanged += new System.EventHandler(this.diskLimitScale_SelectedValueChanged);
			this.diskLimitScale.SelectedIndexChanged += new System.EventHandler(this.diskLimitScale_SelectedIndexChanged);
			// 
			// diskLimitNumber
			// 
			this.diskLimitNumber.Location = new System.Drawing.Point(12, 32);
			this.diskLimitNumber.Maximum = new System.Decimal(new int[] {
																			1048576,
																			0,
																			0,
																			0});
			this.diskLimitNumber.Name = "diskLimitNumber";
			this.diskLimitNumber.Size = new System.Drawing.Size(64, 20);
			this.diskLimitNumber.TabIndex = 38;
			this.diskLimitNumber.TextChanged += new System.EventHandler(this.diskLimitNumber_ValueChanged);
			this.diskLimitNumber.ValueChanged += new System.EventHandler(this.diskLimitNumber_ValueChanged);
			// 
			// chkbx_DiskDirectoryNotEmpty
			// 
			this.chkbx_DiskDirectoryNotEmpty.Location = new System.Drawing.Point(10, 312);
			this.chkbx_DiskDirectoryNotEmpty.Name = "chkbx_DiskDirectoryNotEmpty";
			this.chkbx_DiskDirectoryNotEmpty.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskDirectoryNotEmpty.TabIndex = 34;
			this.chkbx_DiskDirectoryNotEmpty.Text = "Directory Not Empty";
			this.chkbx_DiskDirectoryNotEmpty.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskCannotBeRemoved
			// 
			this.chkbx_DiskCannotBeRemoved.Location = new System.Drawing.Point(10, 296);
			this.chkbx_DiskCannotBeRemoved.Name = "chkbx_DiskCannotBeRemoved";
			this.chkbx_DiskCannotBeRemoved.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskCannotBeRemoved.TabIndex = 33;
			this.chkbx_DiskCannotBeRemoved.Text = "Cannot Be Removed";
			this.chkbx_DiskCannotBeRemoved.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskCorruptStructure
			// 
			this.chkbx_DiskCorruptStructure.Location = new System.Drawing.Point(10, 344);
			this.chkbx_DiskCorruptStructure.Name = "chkbx_DiskCorruptStructure";
			this.chkbx_DiskCorruptStructure.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskCorruptStructure.TabIndex = 36;
			this.chkbx_DiskCorruptStructure.Text = "Corrupt Structure";
			this.chkbx_DiskCorruptStructure.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskPathInvalid
			// 
			this.chkbx_DiskPathInvalid.Location = new System.Drawing.Point(10, 328);
			this.chkbx_DiskPathInvalid.Name = "chkbx_DiskPathInvalid";
			this.chkbx_DiskPathInvalid.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskPathInvalid.TabIndex = 35;
			this.chkbx_DiskPathInvalid.Text = "Path Invalid";
			this.chkbx_DiskPathInvalid.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskLocked
			// 
			this.chkbx_DiskLocked.Location = new System.Drawing.Point(10, 184);
			this.chkbx_DiskLocked.Name = "chkbx_DiskLocked";
			this.chkbx_DiskLocked.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskLocked.TabIndex = 26;
			this.chkbx_DiskLocked.Text = "Locked";
			this.chkbx_DiskLocked.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskFileInUse
			// 
			this.chkbx_DiskFileInUse.Location = new System.Drawing.Point(10, 168);
			this.chkbx_DiskFileInUse.Name = "chkbx_DiskFileInUse";
			this.chkbx_DiskFileInUse.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskFileInUse.TabIndex = 25;
			this.chkbx_DiskFileInUse.Text = "File in Use";
			this.chkbx_DiskFileInUse.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskDriveCannotSeekDisk
			// 
			this.chkbx_DiskDriveCannotSeekDisk.Location = new System.Drawing.Point(10, 152);
			this.chkbx_DiskDriveCannotSeekDisk.Name = "chkbx_DiskDriveCannotSeekDisk";
			this.chkbx_DiskDriveCannotSeekDisk.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskDriveCannotSeekDisk.TabIndex = 24;
			this.chkbx_DiskDriveCannotSeekDisk.Text = "Cannot Seek Disk";
			this.chkbx_DiskDriveCannotSeekDisk.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskPathNotFound
			// 
			this.chkbx_DiskPathNotFound.Location = new System.Drawing.Point(10, 280);
			this.chkbx_DiskPathNotFound.Name = "chkbx_DiskPathNotFound";
			this.chkbx_DiskPathNotFound.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskPathNotFound.TabIndex = 32;
			this.chkbx_DiskPathNotFound.Text = "Path Not Found";
			this.chkbx_DiskPathNotFound.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskReadOnlyDirectory
			// 
			this.chkbx_DiskReadOnlyDirectory.Location = new System.Drawing.Point(10, 264);
			this.chkbx_DiskReadOnlyDirectory.Name = "chkbx_DiskReadOnlyDirectory";
			this.chkbx_DiskReadOnlyDirectory.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskReadOnlyDirectory.TabIndex = 31;
			this.chkbx_DiskReadOnlyDirectory.Text = "Read Only Directory";
			this.chkbx_DiskReadOnlyDirectory.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskNameTooLong
			// 
			this.chkbx_DiskNameTooLong.Location = new System.Drawing.Point(10, 248);
			this.chkbx_DiskNameTooLong.Name = "chkbx_DiskNameTooLong";
			this.chkbx_DiskNameTooLong.TabIndex = 41;
			this.chkbx_DiskNameTooLong.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskNameTooLong.Text = "Name Too Long";
			this.chkbx_DiskNameTooLong.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskCannotBeCreated
			// 
			this.chkbx_DiskCannotBeCreated.Location = new System.Drawing.Point(10, 232);
			this.chkbx_DiskCannotBeCreated.Name = "chkbx_DiskCannotBeCreated";
			this.chkbx_DiskCannotBeCreated.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskCannotBeCreated.TabIndex = 29;
			this.chkbx_DiskCannotBeCreated.Text = "Cannot Be Created";
			this.chkbx_DiskCannotBeCreated.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskAlreadyExists
			// 
			this.chkbx_DiskAlreadyExists.Location = new System.Drawing.Point(10, 216);
			this.chkbx_DiskAlreadyExists.Name = "chkbx_DiskAlreadyExists";
			this.chkbx_DiskAlreadyExists.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskAlreadyExists.TabIndex = 28;
			this.chkbx_DiskAlreadyExists.Text = "Already Exists";
			this.chkbx_DiskAlreadyExists.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskDiskFull
			// 
			this.chkbx_DiskDiskFull.Location = new System.Drawing.Point(10, 200);
			this.chkbx_DiskDiskFull.Name = "chkbx_DiskDiskFull";
			this.chkbx_DiskDiskFull.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskDiskFull.TabIndex = 27;
			this.chkbx_DiskDiskFull.Text = "Disk Full";
			this.chkbx_DiskDiskFull.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskCrcDataError
			// 
			this.chkbx_DiskCrcDataError.Location = new System.Drawing.Point(10, 136);
			this.chkbx_DiskCrcDataError.Name = "chkbx_DiskCrcDataError";
			this.chkbx_DiskCrcDataError.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskCrcDataError.TabIndex = 23;
			this.chkbx_DiskCrcDataError.Text = "CRC Data Error";
			this.chkbx_DiskCrcDataError.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskWriteProtected
			// 
			this.chkbx_DiskWriteProtected.Location = new System.Drawing.Point(10, 120);
			this.chkbx_DiskWriteProtected.Name = "chkbx_DiskWriteProtected";
			this.chkbx_DiskWriteProtected.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskWriteProtected.TabIndex = 22;
			this.chkbx_DiskWriteProtected.Text = "Write Protected";
			this.chkbx_DiskWriteProtected.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskAccessDenied
			// 
			this.chkbx_DiskAccessDenied.Location = new System.Drawing.Point(10, 104);
			this.chkbx_DiskAccessDenied.Name = "chkbx_DiskAccessDenied";
			this.chkbx_DiskAccessDenied.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskAccessDenied.TabIndex = 21;
			this.chkbx_DiskAccessDenied.Text = "Access Denied";
			this.chkbx_DiskAccessDenied.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskCannotAccess
			// 
			this.chkbx_DiskCannotAccess.Location = new System.Drawing.Point(10, 88);
			this.chkbx_DiskCannotAccess.Name = "chkbx_DiskCannotAccess";
			this.chkbx_DiskCannotAccess.TabIndex = 42;
			this.chkbx_DiskCannotAccess.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskCannotAccess.Text = "Cannot Access";
			this.chkbx_DiskCannotAccess.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_DiskFileNotFound
			// 
			this.chkbx_DiskFileNotFound.Location = new System.Drawing.Point(10, 72);
			this.chkbx_DiskFileNotFound.Name = "chkbx_DiskFileNotFound";
			this.chkbx_DiskFileNotFound.Size = new System.Drawing.Size(128, 16);
			this.chkbx_DiskFileNotFound.TabIndex = 19;
			this.chkbx_DiskFileNotFound.Text = "File not Found";
			this.chkbx_DiskFileNotFound.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// memoryFaultsTabPage
			// 
			this.memoryFaultsTabPage.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.memoryFaultsTabPage.Controls.AddRange(new System.Windows.Forms.Control[] {
																							  this.chkbx_MemoryLimit,
																							  this.memoryLimitScale,
																							  this.memoryLimitNumber,
																							  this.chkbx_MemoryInvalidAccess,
																							  this.chkbx_MemoryInsufficientMemory,
																							  this.chkbx_MemoryPagingFileTooSmall,
																							  this.chkbx_MemoryInvalidAddress,
																							  this.chkbx_MemorySegmentIsLocked});
			this.memoryFaultsTabPage.Location = new System.Drawing.Point(23, 4);
			this.memoryFaultsTabPage.Name = "memoryFaultsTabPage";
			this.memoryFaultsTabPage.Size = new System.Drawing.Size(148, 576);
			this.memoryFaultsTabPage.TabIndex = 1;
			this.memoryFaultsTabPage.Text = "Memory Faults";
			// 
			// chkbx_MemoryLimit
			// 
			this.chkbx_MemoryLimit.Location = new System.Drawing.Point(8, 8);
			this.chkbx_MemoryLimit.Name = "chkbx_MemoryLimit";
			this.chkbx_MemoryLimit.Size = new System.Drawing.Size(128, 16);
			this.chkbx_MemoryLimit.TabIndex = 42;
			this.chkbx_MemoryLimit.Text = "Limit Memory to:";
			this.chkbx_MemoryLimit.CheckedChanged += new System.EventHandler(this.chkbx_MemoryLimit_CheckedChanged);
			// 
			// memoryLimitScale
			// 
			this.memoryLimitScale.Items.AddRange(new object[] {
																  "B",
																  "KB",
																  "MB",
																  "GB"});
			this.memoryLimitScale.Location = new System.Drawing.Point(84, 32);
			this.memoryLimitScale.Name = "memoryLimitScale";
			this.memoryLimitScale.Size = new System.Drawing.Size(48, 21);
			this.memoryLimitScale.TabIndex = 41;
			this.memoryLimitScale.Text = "MB";
			this.memoryLimitScale.SelectedValueChanged += new System.EventHandler(this.memoryLimitScale_SelectedValueChanged);
			this.memoryLimitScale.SelectedIndexChanged += new System.EventHandler(this.memoryLimitScale_SelectedIndexChanged);
			// 
			// memoryLimitNumber
			// 
			this.memoryLimitNumber.Location = new System.Drawing.Point(12, 32);
			this.memoryLimitNumber.Maximum = new System.Decimal(new int[] {
																			  1048576,
																			  0,
																			  0,
																			  0});
			this.memoryLimitNumber.Name = "memoryLimitNumber";
			this.memoryLimitNumber.Size = new System.Drawing.Size(64, 20);
			this.memoryLimitNumber.TabIndex = 40;
			this.memoryLimitNumber.TextChanged += new System.EventHandler(this.memoryLimitNumber_ValueChanged);
			this.memoryLimitNumber.ValueChanged += new System.EventHandler(this.memoryLimitNumber_ValueChanged);
			// 
			// chkbx_MemoryInvalidAccess
			// 
			this.chkbx_MemoryInvalidAccess.Location = new System.Drawing.Point(10, 88);
			this.chkbx_MemoryInvalidAccess.Name = "chkbx_MemoryInvalidAccess";
			this.chkbx_MemoryInvalidAccess.Size = new System.Drawing.Size(128, 16);
			this.chkbx_MemoryInvalidAccess.TabIndex = 7;
			this.chkbx_MemoryInvalidAccess.Text = "Invalid Access";
			this.chkbx_MemoryInvalidAccess.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_MemoryInsufficientMemory
			// 
			this.chkbx_MemoryInsufficientMemory.Location = new System.Drawing.Point(10, 72);
			this.chkbx_MemoryInsufficientMemory.Name = "chkbx_MemoryInsufficientMemory";
			this.chkbx_MemoryInsufficientMemory.Size = new System.Drawing.Size(128, 16);
			this.chkbx_MemoryInsufficientMemory.TabIndex = 6;
			this.chkbx_MemoryInsufficientMemory.Text = "Insufficient Memory";
			this.chkbx_MemoryInsufficientMemory.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_MemoryPagingFileTooSmall
			// 
			this.chkbx_MemoryPagingFileTooSmall.Location = new System.Drawing.Point(10, 136);
			this.chkbx_MemoryPagingFileTooSmall.Name = "chkbx_MemoryPagingFileTooSmall";
			this.chkbx_MemoryPagingFileTooSmall.Size = new System.Drawing.Size(128, 16);
			this.chkbx_MemoryPagingFileTooSmall.TabIndex = 10;
			this.chkbx_MemoryPagingFileTooSmall.Text = "Paging file too Small";
			this.chkbx_MemoryPagingFileTooSmall.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_MemoryInvalidAddress
			// 
			this.chkbx_MemoryInvalidAddress.Location = new System.Drawing.Point(10, 120);
			this.chkbx_MemoryInvalidAddress.Name = "chkbx_MemoryInvalidAddress";
			this.chkbx_MemoryInvalidAddress.Size = new System.Drawing.Size(128, 16);
			this.chkbx_MemoryInvalidAddress.TabIndex = 9;
			this.chkbx_MemoryInvalidAddress.Text = "Invalid Address";
			this.chkbx_MemoryInvalidAddress.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_MemorySegmentIsLocked
			// 
			this.chkbx_MemorySegmentIsLocked.Location = new System.Drawing.Point(10, 104);
			this.chkbx_MemorySegmentIsLocked.Name = "chkbx_MemorySegmentIsLocked";
			this.chkbx_MemorySegmentIsLocked.Size = new System.Drawing.Size(128, 16);
			this.chkbx_MemorySegmentIsLocked.TabIndex = 8;
			this.chkbx_MemorySegmentIsLocked.Text = "Segment is Locked";
			this.chkbx_MemorySegmentIsLocked.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// networkFaultsTabPage
			// 
			this.networkFaultsTabPage.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.networkFaultsTabPage.Controls.AddRange(new System.Windows.Forms.Control[] {
																							   this.label_throttleLevel,
																							   this.label_NetworkPercent,
																							   this.chkbx_ThrottleNetwork,
																							   this.chkbx_NetworkWrongWinsock,
																							   this.chkbx_NetworkNoPorts,
																							   this.chkbx_NetworkNetworkDown,
																							   this.chkbx_NetworkDisconnect,
																							   this.chkbx_NetworkNotInstalled,
																							   this.chkbx_NetworkTaskLimitReached,
																							   this.trkb_Network});
			this.networkFaultsTabPage.Location = new System.Drawing.Point(23, 4);
			this.networkFaultsTabPage.Name = "networkFaultsTabPage";
			this.networkFaultsTabPage.Size = new System.Drawing.Size(148, 576);
			this.networkFaultsTabPage.TabIndex = 2;
			this.networkFaultsTabPage.Text = "Network Faults";
			// 
			// label_throttleLevel
			// 
			this.label_throttleLevel.Location = new System.Drawing.Point(72, 24);
			this.label_throttleLevel.Name = "label_throttleLevel";
			this.label_throttleLevel.Size = new System.Drawing.Size(40, 16);
			this.label_throttleLevel.TabIndex = 16;
			this.label_throttleLevel.Text = "100 %";
			// 
			// label_NetworkPercent
			// 
			this.label_NetworkPercent.Location = new System.Drawing.Point(8, 72);
			this.label_NetworkPercent.Name = "label_NetworkPercent";
			this.label_NetworkPercent.Size = new System.Drawing.Size(128, 16);
			this.label_NetworkPercent.TabIndex = 15;
			this.label_NetworkPercent.Text = "  0                             100";
			// 
			// chkbx_ThrottleNetwork
			// 
			this.chkbx_ThrottleNetwork.Location = new System.Drawing.Point(8, 8);
			this.chkbx_ThrottleNetwork.Name = "chkbx_ThrottleNetwork";
			this.chkbx_ThrottleNetwork.Size = new System.Drawing.Size(128, 32);
			this.chkbx_ThrottleNetwork.TabIndex = 14;
			this.chkbx_ThrottleNetwork.Text = "Throttle network speed to:";
			this.chkbx_ThrottleNetwork.CheckedChanged += new System.EventHandler(this.chkbx_ThrottleNetwork_CheckedChanged);
			// 
			// chkbx_NetworkWrongWinsock
			// 
			this.chkbx_NetworkWrongWinsock.Location = new System.Drawing.Point(8, 128);
			this.chkbx_NetworkWrongWinsock.Name = "chkbx_NetworkWrongWinsock";
			this.chkbx_NetworkWrongWinsock.Size = new System.Drawing.Size(128, 16);
			this.chkbx_NetworkWrongWinsock.TabIndex = 10;
			this.chkbx_NetworkWrongWinsock.Text = "Wrong Winsock";
			this.chkbx_NetworkWrongWinsock.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_NetworkNoPorts
			// 
			this.chkbx_NetworkNoPorts.Location = new System.Drawing.Point(8, 160);
			this.chkbx_NetworkNoPorts.Name = "chkbx_NetworkNoPorts";
			this.chkbx_NetworkNoPorts.Size = new System.Drawing.Size(128, 16);
			this.chkbx_NetworkNoPorts.TabIndex = 12;
			this.chkbx_NetworkNoPorts.Text = "No Ports";
			this.chkbx_NetworkNoPorts.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_NetworkNetworkDown
			// 
			this.chkbx_NetworkNetworkDown.Location = new System.Drawing.Point(8, 176);
			this.chkbx_NetworkNetworkDown.Name = "chkbx_NetworkNetworkDown";
			this.chkbx_NetworkNetworkDown.Size = new System.Drawing.Size(128, 16);
			this.chkbx_NetworkNetworkDown.TabIndex = 13;
			this.chkbx_NetworkNetworkDown.Text = "Network Down";
			this.chkbx_NetworkNetworkDown.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_NetworkDisconnect
			// 
			this.chkbx_NetworkDisconnect.Location = new System.Drawing.Point(8, 96);
			this.chkbx_NetworkDisconnect.Name = "chkbx_NetworkDisconnect";
			this.chkbx_NetworkDisconnect.Size = new System.Drawing.Size(128, 16);
			this.chkbx_NetworkDisconnect.TabIndex = 7;
			this.chkbx_NetworkDisconnect.Text = "Disconnect";
			this.chkbx_NetworkDisconnect.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_NetworkNotInstalled
			// 
			this.chkbx_NetworkNotInstalled.Location = new System.Drawing.Point(8, 112);
			this.chkbx_NetworkNotInstalled.Name = "chkbx_NetworkNotInstalled";
			this.chkbx_NetworkNotInstalled.Size = new System.Drawing.Size(128, 16);
			this.chkbx_NetworkNotInstalled.TabIndex = 9;
			this.chkbx_NetworkNotInstalled.Text = "Not Installed";
			this.chkbx_NetworkNotInstalled.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// chkbx_NetworkTasklimitReached
			// 
			this.chkbx_NetworkTaskLimitReached.Location = new System.Drawing.Point(8, 144);
			this.chkbx_NetworkTaskLimitReached.Name = "chkbx_NetworkTaskLimitReached";
			this.chkbx_NetworkTaskLimitReached.Size = new System.Drawing.Size(128, 16);
			this.chkbx_NetworkTaskLimitReached.TabIndex = 11;
			this.chkbx_NetworkTaskLimitReached.Text = "Tasklimit Reached";
			this.chkbx_NetworkTaskLimitReached.CheckedChanged += new System.EventHandler(this.Fault_CheckedChanged_Handler);
			// 
			// trkb_Network
			// 
			this.trkb_Network.Location = new System.Drawing.Point(8, 40);
			this.trkb_Network.Maximum = 100;
			this.trkb_Network.Name = "trkb_Network";
			this.trkb_Network.Size = new System.Drawing.Size(128, 45);
			this.trkb_Network.TabIndex = 8;
			this.trkb_Network.TickFrequency = 10;
			this.trkb_Network.Value = 100;
			this.trkb_Network.ValueChanged += new System.EventHandler(this.trkb_Network_ValueChanged);
			this.trkb_Network.Scroll += new System.EventHandler(this.trkb_Network_Scroll);
			// 
			// applicationsComboBox
			// 
			this.applicationsComboBox.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.applicationsComboBox.Name = "applicationsComboBox";
			this.applicationsComboBox.Size = new System.Drawing.Size(176, 21);
			this.applicationsComboBox.TabIndex = 4;
			this.applicationsComboBox.Visible = false;
			// 
			// mainSplitter
			// 
			this.mainSplitter.Location = new System.Drawing.Point(175, 0);
			this.mainSplitter.Name = "mainSplitter";
			this.mainSplitter.Size = new System.Drawing.Size(8, 585);
			this.mainSplitter.TabIndex = 2;
			this.mainSplitter.TabStop = false;
			// 
			// mainMenu
			// 
			this.mainMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.fileMenu,
																					 this.editMenu,
																					 this.applicationMenu,
																					 this.logMenu,
																					 this.viewMenu,
																					 this.windowMenu,
																					 this.helpMenu});
			// 
			// fileMenu
			// 
			this.fileMenu.Index = 0;
			this.fileMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.fileNewProjectMenuItem,
																					 this.fileAddTestApplicationMenuItem,
																					 this.menuItem6,
																					 this.fileOpenProjectMenuItem,
																					 this.fileSaveProjectMenuItem,
																					 this.fileSaveProjectAsMenuItem,
																					 this.fileCloseProjectMenuItem,
																					 this.menuItem3,
																					 this.fileOpenLogMenuItem,
																					 this.fileSaveLogMenuItem,
																					 this.fileSaveLogAsMenuItem,
																					 this.menuItem39,
																					 this.filePrintMenuItem,
																					 this.menuItem41,
																					 this.fileExitMenuItem});
			this.fileMenu.Text = "&File";
			// 
			// fileNewProjectMenuItem
			// 
			this.fileNewProjectMenuItem.Index = 0;
			this.fileNewProjectMenuItem.Text = "&New Project...";
			this.fileNewProjectMenuItem.Click += new System.EventHandler(this.fileNewProjectMenuItem_Click);
			// 
			// fileAddTestApplicationMenuItem
			// 
			this.fileAddTestApplicationMenuItem.Index = 1;
			this.fileAddTestApplicationMenuItem.Text = "New Test Application...";
			this.fileAddTestApplicationMenuItem.Click += new System.EventHandler(this.fileAddTestApplicationMenuItem_Click);

			// 
			// menuItem6
			// 
			this.menuItem6.Index = 2;
			this.menuItem6.Text = "-";
			// 
			// fileOpenProjectMenuItem
			// 
			this.fileOpenProjectMenuItem.Index = 3;
			this.fileOpenProjectMenuItem.Text = "&Open Project...";
			this.fileOpenProjectMenuItem.Click += new System.EventHandler(this.fileOpenProjectMenuItem_Click);
			// 
			// fileSaveProjectMenuItem
			// 
			this.fileSaveProjectMenuItem.Index = 4;
			this.fileSaveProjectMenuItem.Text = "&Save Project...";
			this.fileSaveProjectMenuItem.Click += new System.EventHandler(this.fileSaveProjectMenuItem_Click);
			// 
			// fileSaveProjectAsMenuItem
			// 
			this.fileSaveProjectAsMenuItem.Index = 5;
			this.fileSaveProjectAsMenuItem.Text = "Save Project &As...";
			this.fileSaveProjectAsMenuItem.Click += new System.EventHandler(this.fileSaveProjectMenuItem_Click);
			// 
			// fileCloseProjectMenuItem
			// 
			this.fileCloseProjectMenuItem.Index = 6;
			this.fileCloseProjectMenuItem.Text = "&Close";
			this.fileCloseProjectMenuItem.Click += new System.EventHandler(this.fileNewProjectMenuItem_Click);
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 7;
			this.menuItem3.Text = "-";
			// 
			// fileOpenLogMenuItem
			// 
			this.fileOpenLogMenuItem.Index = 8;
			this.fileOpenLogMenuItem.Text = "Open &Log...";
			this.fileOpenLogMenuItem.Click += new System.EventHandler(this.fileOpenLogMenuItem_Click);
			// 
			// fileSaveLogMenuItem
			// 
			this.fileSaveLogMenuItem.Index = 9;
			this.fileSaveLogMenuItem.Text = "Save L&og...";
			this.fileSaveLogMenuItem.Click += new System.EventHandler(this.fileSaveLogMenuItem_Click);
			// 
			// fileSaveLogAsMenuItem
			// 
			this.fileSaveLogAsMenuItem.Index = 10;
			this.fileSaveLogAsMenuItem.Text = "Sa&ve Log As...";
			this.fileSaveLogAsMenuItem.Click += new System.EventHandler(this.fileSaveLogMenuItem_Click);
			// 
			// menuItem39
			// 
			this.menuItem39.Index = 11;
			this.menuItem39.Text = "-";
			// 
			// filePrintMenuItem
			// 
			this.filePrintMenuItem.Index = 12;
			this.filePrintMenuItem.Text = "&Print Log...";
			// 
			// menuItem41
			// 
			this.menuItem41.Index = 13;
			this.menuItem41.Text = "-";
			// 
			// fileExitMenuItem
			// 
			this.fileExitMenuItem.Index = 14;
			this.fileExitMenuItem.Text = "&Exit";
			this.fileExitMenuItem.Click += new System.EventHandler(this.fileExitMenuItem_Click);
			// 
			// editMenu
			// 
			this.editMenu.Index = 1;
			this.editMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.editCopyMenuItem});
			this.editMenu.Text = "&Edit";
			// 
			// editCopyMenuItem
			// 
			this.editCopyMenuItem.Index = 0;
			this.editCopyMenuItem.Text = "C&opy";
			this.editCopyMenuItem.Click += new System.EventHandler(this.editCopyMenuItem_Click);
			// 
			// applicationMenu
			// 
			this.applicationMenu.Index = 2;
			this.applicationMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																							this.applicationPauseMenuItem,
																							this.applicationStopMenuItem,
																							this.menuItem5,
																							this.applicationCreateATestMenuItem});
			this.applicationMenu.Text = "&Application";
			// 
			// applicationPauseMenuItem
			// 
			this.applicationPauseMenuItem.Index = 0;
			this.applicationPauseMenuItem.Text = "&Pause";
			this.applicationPauseMenuItem.Click += new System.EventHandler(this.applicationPauseMenuItem_Click);
			// 
			// applicationStopMenuItem
			// 
			this.applicationStopMenuItem.Index = 1;
			this.applicationStopMenuItem.Text = "S&top";
			this.applicationStopMenuItem.Click += new System.EventHandler(this.applicationStopMenuItem_Click);
			// 
			// menuItem5
			// 
			this.menuItem5.Index = 2;
			this.menuItem5.Text = "-";
			// 
			// applicationCreateATestMenuItem
			// 
			this.applicationCreateATestMenuItem.Index = 3;
			this.applicationCreateATestMenuItem.Text = "&Create a Test...";
			this.applicationCreateATestMenuItem.Click += new System.EventHandler(this.applicationCreateATestMenuItem_Click);
			// 
			// logMenu
			// 
			this.logMenu.Index = 3;
			this.logMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.logFindMenuItem,
																					this.logFindNextMenuItem,
																					this.menuItem1,
																					this.logFilterMenuItem});
			this.logMenu.Text = "&Log";
			// 
			// logFindMenuItem
			// 
			this.logFindMenuItem.Index = 0;
			this.logFindMenuItem.Shortcut = System.Windows.Forms.Shortcut.CtrlF;
			this.logFindMenuItem.Text = "&Find...";
			this.logFindMenuItem.Click += new System.EventHandler(this.logFindMenuItem_Click);
			// 
			// logFindNextMenuItem
			// 
			this.logFindNextMenuItem.Index = 1;
			this.logFindNextMenuItem.Shortcut = System.Windows.Forms.Shortcut.F3;
			this.logFindNextMenuItem.Text = "Find &Next";
			this.logFindNextMenuItem.Click += new System.EventHandler(this.logFindNextMenuItem_Click);
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 2;
			this.menuItem1.Text = "-";
			// 
			// logFilterMenuItem
			// 
			this.logFilterMenuItem.Index = 3;
			this.logFilterMenuItem.Text = "&Filter...";
			this.logFilterMenuItem.Click += new System.EventHandler(this.logFilterMenuItem_Click);
			// 
			// viewMenu
			// 
			this.viewMenu.Index = 4;
			this.viewMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.viewTinyModeMenuItem,
																					 this.viewFullModeMenuItem,
																					 this.menuItem2,
																					 this.viewAlwaysOnTopMenuItem,
																					 this.menuItem4,
																					 this.viewPreferencesMenuItem});
			this.viewMenu.Text = "&View";
			// 
			// viewTinyModeMenuItem
			// 
			this.viewTinyModeMenuItem.Index = 0;
			this.viewTinyModeMenuItem.Text = "&Compact Mode";
			this.viewTinyModeMenuItem.Click += new System.EventHandler(this.viewTinyModeMenuItem_Click);
			// 
			// viewFullModeMenuItem
			// 
			this.viewFullModeMenuItem.Index = 1;
			this.viewFullModeMenuItem.Text = "&Full Mode";
			this.viewFullModeMenuItem.Click += new System.EventHandler(this.viewFullModeMenuItem_Click);
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 2;
			this.menuItem2.Text = "-";
			// 
			// viewAlwaysOnTopMenuItem
			// 
			this.viewAlwaysOnTopMenuItem.Index = 3;
			this.viewAlwaysOnTopMenuItem.Text = "&Always On Top";
			this.viewAlwaysOnTopMenuItem.Click += new System.EventHandler(this.viewAlwaysOnTopMenuItem_Click);
			// 
			// menuItem4
			// 
			this.menuItem4.Index = 4;
			this.menuItem4.Text = "-";
			// 
			// viewPreferencesMenuItem
			// 
			this.viewPreferencesMenuItem.Index = 5;
			this.viewPreferencesMenuItem.Text = "Preferences...";
			this.viewPreferencesMenuItem.Click += new System.EventHandler(this.viewPreferencesMenuItem_Click);
			// 
			// windowMenu
			// 
			this.windowMenu.Index = 5;
			this.windowMenu.MdiList = true;
			this.windowMenu.Text = "&Window";
			// 
			// helpMenu
			// 
			this.helpMenu.Index = 6;
			this.helpMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					 this.helpTopicsMenuItem,
																					 this.helpIndexMenuItem,
																					 this.fileNewProjectMenuItem0,
																					 this.helpDistributionSiteMenuItem,
																					 this.helpReportABugMenuItem,
																					 this.fileNewProjectMenuItem3,
																					 this.helpAboutMenuItem});
			this.helpMenu.Text = "&Help";
			// 
			// helpTopicsMenuItem
			// 
			this.helpTopicsMenuItem.Index = 0;
			this.helpTopicsMenuItem.Text = "&Help Topics...";
			this.helpTopicsMenuItem.Click += new System.EventHandler(this.helpTopicsMenuItem_Click);
			// 
			// helpIndexMenuItem
			// 
			this.helpIndexMenuItem.Index = 1;
			this.helpIndexMenuItem.Text = "Help &Index...";
			this.helpIndexMenuItem.Click += new System.EventHandler(this.helpIndexMenuItem_Click);
			// 
			// fileNewProjectMenuItem0
			// 
			this.fileNewProjectMenuItem0.Index = 2;
			this.fileNewProjectMenuItem0.Text = "-";
			// 
			// helpDistributionSiteMenuItem
			// 
			this.helpDistributionSiteMenuItem.Index = 3;
			this.helpDistributionSiteMenuItem.Text = "Holodeck &Distribution Site";
			this.helpDistributionSiteMenuItem.Click += new System.EventHandler(this.helpDistributionSiteMenuItem_Click);
			// 
			// helpReportABugMenuItem
			// 
			this.helpReportABugMenuItem.Index = 4;
			this.helpReportABugMenuItem.Text = "&Report A Bug";
			this.helpReportABugMenuItem.Click += new System.EventHandler(this.helpReportABugMenuItem_Click);
			// 
			// fileNewProjectMenuItem3
			// 
			this.fileNewProjectMenuItem3.Index = 5;
			this.fileNewProjectMenuItem3.Text = "-";
			// 
			// helpAboutMenuItem
			// 
			this.helpAboutMenuItem.Index = 6;
			this.helpAboutMenuItem.Text = "&About Holodeck...";
			this.helpAboutMenuItem.Click += new System.EventHandler(this.helpAboutMenuItem_Click);
			// 
			// splashScreenTimer
			// 
			this.splashScreenTimer.Enabled = true;
			this.splashScreenTimer.Interval = 100;
			this.splashScreenTimer.Tick += new System.EventHandler(this.splashScreenTimer_Tick);
			// 
			// mainHelpProvider
			// 
			this.mainHelpProvider.HelpNamespace = "C:\\Holodeck 2\\HolodeckEEGui\\HolodeckEE.chm";
			// 
			// MainWindow
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(992, 585);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.mainSplitter,
																		  this.leftPanel});
			this.HelpButton = true;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.IsMdiContainer = true;
			this.Menu = this.mainMenu;
			this.Name = "MainWindow";
			this.Text = "Holodeck Enterprise Edition";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.MainWindow_Closing);
			this.MdiChildActivate += new System.EventHandler(this.MainWindow_MdiChildActivate);
			this.Load += new System.EventHandler(this.MainWindow_Load);
			this.leftPanel.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.diskFaultsTabPage.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.diskLimitNumber)).EndInit();
			this.memoryFaultsTabPage.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.memoryLimitNumber)).EndInit();
			this.networkFaultsTabPage.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.trkb_Network)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		#region Wizard running code

		protected void RunWizardIfNecessary()
		{
			// don't launch a wizard if we were started from the cmd line.
			if (cmdLineFile == null)
			{
				if (RegistryHelper.getInstance().GetNeedToRunWizard())
				{
					RunWizard();
				}
				else if (RegistryHelper.getInstance().GetNeedToRunLaunchPad())
				{
					RunLaunchPad();
				}
			}
			else
			{
				if (cmdLineFile.EndsWith(".hdp"))
					OpenProject(cmdLineFile);
				else if (cmdLineFile.EndsWith(".hdl"))
					OpenLogFile(cmdLineFile);
				cmdLineFile = null;
			}
		}

		protected ProcessMdiWindow LaunchApplicationForTest(AbstractLaunchWizard wizard, bool copyPreviousWindow)
		{
			Cursor.Current = Cursors.WaitCursor;
			ProcessMdiWindow newWindow = new ProcessMdiWindow();

			newWindow.MdiParent = this;
				
			newWindow.PredefineTests = wizard.PredefineTests;
			newWindow.ProcessChain = wizard.ProcessChain;
			newWindow.UseProcessFile = false;
			newWindow.UseProcessID = false;
			newWindow.AppPaused = wizard.AppPaused;

			newWindow.FilteredOutFunctions = wizard.FilteredOutFunctions;

			if ((this.GetActiveChildWindow() != null) && (copyPreviousWindow))
				newWindow.SetFaultsLimitsAndFilters(this.GetActiveChildWindow());

			if (wizard.UseProcessFile)
			{
				newWindow.UseProcessFile = true;
				newWindow.ProcessFile = wizard.ProcessPath;
				newWindow.LaunchApp();
					
				int PID = newWindow.AppProcessID;
				newWindow.Text = PID.ToString() + " : " + newWindow.ProcessFile;
				RegistryHelper.getInstance().AddRecentItem(newWindow.ProcessFile);
			}
			else
			{
				newWindow.UseProcessID = true;
				newWindow.ProcessID = int.Parse(wizard.ProcessList.SelectedItems[0].Text);
				newWindow.AttachApp(null);

				string processPath = wizard.ProcessList.SelectedItems[0].SubItems[1].Text;
				newWindow.Text = newWindow.ProcessID + " : " + processPath;
			}

			if (wizard.AppPaused)
			{
				this.applicationPauseMenuItem.Text = "&Resume";
				newWindow.Text += " - Paused!";
			}

			newWindow.Show();
			mdiWindowList.Add(newWindow);
			Cursor.Current = Cursors.Default;

			return newWindow;
		}

		protected void RunWizard()
		{
			BasicWizard wizard = new BasicWizard();
			wizard.ShowDialog();
			if (wizard.SwitchModes)
			{
				RegistryHelper.getInstance().SetNeedToRunWizardAtStartup(true);
				RegistryHelper.getInstance().SetNeedToRunLaunchPad(true);
				
				RunLaunchPad();
			}
			else if (wizard.DialogResult == DialogResult.OK)
			{
				LaunchApplicationForTest(wizard, true);
			}
		}

		protected void RunLaunchPad()
		{
			Wizard wizard = new Wizard();
			wizard.ShowDialog();
			if (wizard.SwitchModes)
			{
				RegistryHelper.getInstance().SetNeedToRunWizardAtStartup(true);
				RegistryHelper.getInstance().SetNeedToRunWizard(true);

				RunWizard();
			}
			else if (wizard.DialogResult == DialogResult.OK)
			{
				LaunchApplicationForTest(wizard, true);
			}
		}

		#endregion

		#region Check Box changed handlers

		/// <summary>
		/// Common handler for all Fault checkboxes
		/// Uses reflection to set the appropriate fault property in all MDIs to true/false
		/// </summary>
		/// <param name="sender">The checkbox which trigerred this event</param>
		/// <param name="e">Event Arguments</param>
		private void Fault_CheckedChanged_Handler(object sender, System.EventArgs e)
		{
			foreach (ProcessMdiWindow pmw in mdiWindowList)
			{
				if (pmw != null)
				{
					String propertyName = (sender as CheckBox).Name;
					propertyName = "Inject" + propertyName.Replace ("chkbx_", "");
					pmw.GetType().InvokeMember (propertyName, BindingFlags.SetProperty, null, pmw, new object[] {(sender as CheckBox).Checked});
				}
			}
		}

		#endregion

		#region Menu Item Click Handlers

		private void applicationPauseMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow ActiveChild = GetActiveChildWindow();
			if (ActiveChild != null)
			{
				if (applicationPauseMenuItem.Text == "&Pause")
				{
					ActiveChild.PauseApp();
					applicationPauseMenuItem.Text = "&Resume";
					ActiveChild.Text += " - Paused!";
				}
				else
				{
					ActiveChild.ResumeApp();
					applicationPauseMenuItem.Text = "&Pause";
					ActiveChild.Text = ActiveChild.Text.Replace(" - Paused!", "");
				}
															 
			}
			else
				MessageBox.Show ("You must select an application to be paused.", "Holodeck");
		}

		private void applicationStopMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow ActiveChild = GetActiveChildWindow();
			if (ActiveChild != null)
			{
				ActiveChild.TerminateApp();
				ActiveChild.Close();
				mdiWindowList.Remove(ActiveChild);
			}
		}

		private void viewTinyModeMenuItem_Click(object sender, System.EventArgs e)
		{
			if (!viewTinyModeMenuItem.Checked)
			{
				viewTinyModeMenuItem.Checked = true;
				viewFullModeMenuItem.Checked = false;

				editMenu.Visible = false;
				logMenu.Visible = false;
				windowMenu.Visible = false;
				mainSplitter.Visible = false;

				applicationsComboBox.Visible = true;

				tabControl1.Top = applicationsComboBox.Bottom;
				tabControl1.Height -= applicationsComboBox.Bottom;

				fullModeWidth = this.Width;
				this.Width = tabControl1.Width + 8;
				this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
				this.MaximizeBox = false;
			}
		}

		private void viewFullModeMenuItem_Click(object sender, System.EventArgs e)
		{
			if (!viewFullModeMenuItem.Checked)
			{
				viewFullModeMenuItem.Checked = true;
				viewTinyModeMenuItem.Checked = false;

				editMenu.Visible = true;
				logMenu.Visible = true;
				windowMenu.Visible = true;
				mainSplitter.Visible = true;

				applicationsComboBox.Visible = false;

				tabControl1.Top = 0;
				tabControl1.Height += applicationsComboBox.Bottom;

				this.Width = fullModeWidth;
				this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;
				this.MaximizeBox = true;
			}
		}

		private void viewAlwaysOnTopMenuItem_Click(object sender, System.EventArgs e)
		{
			viewAlwaysOnTopMenuItem.Checked = !viewAlwaysOnTopMenuItem.Checked;
			this.TopMost = viewAlwaysOnTopMenuItem.Checked;
		}

		private void logFilterMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeWindow = this.GetActiveChildWindow();
			bool appPaused = false;

			LogFilterDialog dialog = new LogFilterDialog();

			if (activeWindow != null)
			{
				dialog.FilteredOutFunctionNames = activeWindow.FilteredOutFunctions;
				appPaused = activeWindow.AppPaused;
				if (!appPaused)
					activeWindow.PauseApp();
			}

			dialog.ShowDialog();
			
			if ((activeWindow != null) && (dialog.DialogResult == DialogResult.OK))
			{
				activeWindow.FilteredOutFunctions = dialog.FilteredOutFunctionNames;
				if (!appPaused)
					activeWindow.ResumeApp();
			}
		}
		private void helpAboutMenuItem_Click(object sender, System.EventArgs e)
		{
			AboutBox box = new AboutBox();
			box.ShowDialog();
		}

		private void helpTopicsMenuItem_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.Help.ShowHelp(this, mainHelpProvider.HelpNamespace, HelpNavigator.TableOfContents, "");
		}

		private void helpIndexMenuItem_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.Help.ShowHelpIndex(this, mainHelpProvider.HelpNamespace);
		}

		private void helpDistributionSiteMenuItem_Click(object sender, System.EventArgs e)
		{
			System.Diagnostics.Process.Start("http://www.sisecure.com");
		}

		private void helpReportABugMenuItem_Click(object sender, System.EventArgs e)
		{
			System.Diagnostics.Process.Start("http://se.fit.edu/projects/holodeck/bugs.html");
		}
		
		private void fileNewApplicationMenuItem_Click(object sender, System.EventArgs e)
		{
			int wizard = RegistryHelper.getInstance().GetWizardToRun();
			switch(wizard)
			{
				case 0:
					RunLaunchPad();
					break;
				case 1:
					RunWizard();
					break;
				default:
					RunWizard();
					break;
			}
		}

		private void viewPreferencesMenuItem_Click(object sender, System.EventArgs e)
		{
			PreferencesDialog dialog = new PreferencesDialog();
			dialog.ShowDialog();
		}

		private void applicationCreateATestMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow window = GetActiveChildWindow();
			if (window != null)
				window.ShowCreateTestWindow();
		}

		private void fileOpenProjectMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeChild = this.GetActiveChildWindow();
			RegistryHelper regHelper = RegistryHelper.getInstance();
			System.Windows.Forms.OpenFileDialog openDialog = new System.Windows.Forms.OpenFileDialog();

			// setup open file dialog
			openDialog.AddExtension = true;
			openDialog.Filter = "Holodeck projects(*.hdp)|*.hdp";
			openDialog.DefaultExt = "*.hdp";
			openDialog.Title = (sender as MenuItem).Text.Replace("&", "");
			openDialog.CheckPathExists = true;
			openDialog.CheckFileExists = true;
			openDialog.InitialDirectory = regHelper.GetLastProjectSavePath();

			if (activeChild != null)
			{
				DialogResult dResult = MessageBox.Show ("Would you like to save your project?", "Holodeck", MessageBoxButtons.YesNoCancel);
				if (dResult == DialogResult.Yes)
				{
					if (this.HasSavedProjectFile)
						this.SaveProject ("", true);
					else
						fileSaveProjectMenuItem_Click (fileSaveProjectMenuItem, EventArgs.Empty);
				}
				else if (dResult == DialogResult.Cancel)
					return;

				if ((dResult == DialogResult.No)||(dResult == DialogResult.Yes))
				{
					foreach(ProcessMdiWindow window in mdiWindowList)
					{
						if (window != null)
						{
							window.TerminateApp();
							window.Close();
						}
					}	
				}
			}

			if (openDialog.ShowDialog (this) == DialogResult.OK)
			{
				regHelper.SetLastProjectSavePath (System.IO.Path.GetDirectoryName (openDialog.FileName));
				OpenProject(openDialog.FileName);
			}
		}

		private void fileSaveProjectMenuItem_Click(object sender, System.EventArgs e)
		{
			//If this is a File>>Save_Project event and the project has been saved previously
			//then use the existing filename to save.
			if ((Object.Equals (this.fileSaveProjectMenuItem, (sender as MenuItem))) && (this.HasSavedProjectFile))
			{
				this.SaveProject("", true);
			}
			//Otherwise prompt the user for a file name
			else
			{
				System.Windows.Forms.SaveFileDialog saveDialog = new System.Windows.Forms.SaveFileDialog();
				RegistryHelper regHelper = RegistryHelper.getInstance();
			
				saveDialog.AddExtension = true;
				saveDialog.Filter = "Holodeck projects(*.hdp)|*.hdp";
				saveDialog.DefaultExt = "*.hdp";
				saveDialog.Title = (sender as MenuItem).Text.Replace("&", "");
				saveDialog.CheckPathExists = true;
				if (regHelper.GetLastProjectSavePath() != null) saveDialog.InitialDirectory = regHelper.GetLastProjectSavePath();

				if (saveDialog.ShowDialog (this) == DialogResult.OK)
				{
					this.SaveProject(saveDialog.FileName, false);
					regHelper.SetLastProjectSavePath (System.IO.Path.GetDirectoryName (saveDialog.FileName));
				}
			}
		}

		private void fileNewProjectMenuItem_Click(object sender, System.EventArgs e)
		{
			DialogResult result = DialogResult.No;

			// if there is an mdi window open then prompt for saving this project
			if (mdiWindowList.Count > 0)
				result = MessageBox.Show(this, "Would you like to save your project?", "Holodeck", MessageBoxButtons.YesNoCancel);

			//If want to save, then save the project
			if (result == DialogResult.Yes)
			{
				if (HasSavedProjectFile)
					SaveProject ("", true);
				else
					fileSaveProjectMenuItem_Click (fileSaveProjectMenuItem, EventArgs.Empty);
			}

			//Now close the previous project and if required, start a new one
			if ((result == DialogResult.No) || (result == DialogResult.Yes))
			{
				foreach(ProcessMdiWindow window in mdiWindowList)
				{
					if (window != null)
					{
						window.TerminateApp();
						window.Close();
					}
				}
				mdiWindowList.Clear();

				ClearProjectSettings();

				//Finally, if the caller was "File>>New Project" run the wizard to start a new project
				if ((sender as MenuItem) == this.fileNewProjectMenuItem)
				{
					if (RegistryHelper.getInstance().GetNeedToRunWizard())
						RunWizard();
					else
						RunLaunchPad();
				}
					
			}
		}

		private void fileAddTestApplicationMenuItem_Click(object sender, System.EventArgs e)
		{
			if (RegistryHelper.getInstance().GetNeedToRunWizard())
				RunWizard();
			else
				RunLaunchPad();
		}

		private void fileOpenLogMenuItem_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.OpenFileDialog openDialog = new System.Windows.Forms.OpenFileDialog();
			RegistryHelper regHelper = RegistryHelper.getInstance();

			openDialog.AddExtension = true;
			openDialog.Filter = "Holodeck Log Files(*.hdl)|*.hdl";
			openDialog.DefaultExt = "*.hdl";
			openDialog.Title = (sender as MenuItem).Text.Replace("&", "");
			openDialog.CheckPathExists = true;
			openDialog.CheckFileExists = true;
			openDialog.InitialDirectory = regHelper.GetLastLogSavePath();

			if (openDialog.ShowDialog (this) == DialogResult.OK)
			{
				regHelper.SetLastLogSavePath (System.IO.Path.GetDirectoryName (openDialog.FileName));
				OpenLogFile(openDialog.FileName);
			}
		}

		private void fileSaveLogMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeWindow = this.GetActiveChildWindow();

			if (activeWindow != null)
			{
				//If this is a File>>Save_Log event and the Log has been saved previously
				//then use the existing filename to save.
				if ((Object.Equals (this.fileSaveLogMenuItem, (sender as MenuItem))) && (activeWindow.HasSavedLogFile))
				{
					activeWindow.SaveLog("", true);
				}
					//Otherwise prompt the user for a file name
				else
				{
					System.Windows.Forms.SaveFileDialog saveDialog = new System.Windows.Forms.SaveFileDialog();
					RegistryHelper regHelper = RegistryHelper.getInstance();
				
					saveDialog.AddExtension = true;
					saveDialog.Filter = "Holodeck Log Files(*.hdl)|*.hdl";
					saveDialog.DefaultExt = "*.hdl";
					saveDialog.Title = (sender as MenuItem).Text.Replace("&", "");
					saveDialog.CheckPathExists = true;
					if (regHelper.GetLastLogSavePath() != "") saveDialog.InitialDirectory = regHelper.GetLastLogSavePath();

					if (saveDialog.ShowDialog (this) == DialogResult.OK)
					{
						activeWindow.SaveLog(saveDialog.FileName, false);
						regHelper.SetLastLogSavePath (System.IO.Path.GetDirectoryName (saveDialog.FileName));
					}
				}
				
			}
			else
				MessageBox.Show ("There are no logs to save", "Holodeck");
		}

		private void fileProjectCloseMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeWindow = this.GetActiveChildWindow();
			if (activeWindow != null)
			{
				activeWindow.Close();
				mdiWindowList.Remove (activeWindow);
			}
		}

		private void fileExitMenuItem_Click(object sender, System.EventArgs e)
		{
			DialogResult result = DialogResult.No;

			// if there is an mdi window open then prompt for saving this project
			if (mdiWindowList.Count > 0)
				result = MessageBox.Show(this, "Would you like to save your project?", "Holodeck", MessageBoxButtons.YesNoCancel);

			//If want to save, then save the project
			if (result == DialogResult.Yes)
			{
				if (HasSavedProjectFile)
					SaveProject ("", true);
				else
					fileSaveProjectMenuItem_Click (fileSaveProjectMenuItem, EventArgs.Empty);
			}

			if (result != DialogResult.Cancel)
			{
				//First close the project
				this.fileProjectCloseMenuItem_Click (sender, e);
				//Exit the application
				Application.Exit ();
			}
		}

		#endregion

		#region Main Window Miscellaneous Event Handlers

		private void MainWindow_MdiChildActivate(object sender, System.EventArgs e)
		{
			UpdateFaultsForActiveWindow();
		}

		private void splashScreenTimer_Tick(object sender, System.EventArgs e)
		{
			splashScreenTimer.Stop();
			splashScreen.Close();
			splashScreen = null;
			
			this.RunWizardIfNecessary();
		}

		private void MainWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			foreach(ProcessMdiWindow window in mdiWindowList)
			{
				if (window != null)
				{
					window.TerminateApp();
					window.Close();
				}
			}
		}

		private void panel1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
		
		}

		private void MainWindow_Load(object sender, System.EventArgs e)
		{
		}

		private void diskLimitScale_SelectedValueChanged(object sender, System.EventArgs e)
		{
			//Verify that the text entered is valid; and then update limit
			if ((diskLimitScale.Text.ToUpper() == "B") ||
				(diskLimitScale.Text.ToUpper() == "KB") ||
				(diskLimitScale.Text.ToUpper() == "MB") ||
				(diskLimitScale.Text.ToUpper() == "GB"))
			{
				diskLimitScale_SelectedIndexChanged(sender, e);
			}
		}

		private void diskLimitScale_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if (!VerifyDiskLimitBounds(false))
			{
				if (this.chkbx_DiskSpaceLimit.Checked)
				{
					//Call again to display the warning message
					VerifyDiskLimitBounds(true);
					this.chkbx_DiskSpaceLimit.Checked = false;
				}
			}
			else
			{
				foreach (ProcessMdiWindow window in mdiWindowList)
				{
					window.MaxDiskUsageUnit = diskLimitScale.Text;
				}
			}
		}


		private void memoryLimitScale_SelectedValueChanged(object sender, System.EventArgs e)
		{
			//Verify that the text entered is valid; and then update limit
			if ((memoryLimitScale.Text.ToUpper() == "B") ||
				(memoryLimitScale.Text.ToUpper() == "KB") ||
				(memoryLimitScale.Text.ToUpper() == "MB") ||
				(memoryLimitScale.Text.ToUpper() == "GB"))
			{
				memoryLimitScale_SelectedIndexChanged(sender, e);
			}
		}

		private void memoryLimitNumber_ValueChanged(object sender, System.EventArgs e)
		{
			if (!VerifyMemoryLimitBounds (false))
			{
				if (this.chkbx_MemoryLimit.Checked)
				{
					//Call again to display the warning message
					VerifyMemoryLimitBounds (true);
					this.chkbx_MemoryLimit.Checked = false;
				}
			}
			else
			{
				foreach (ProcessMdiWindow window in mdiWindowList)
				{
					window.MaxMemoryUsage = uint.Parse(this.memoryLimitNumber.Value.ToString());
				}
			}
		}

		private void diskLimitNumber_ValueChanged(object sender, System.EventArgs e)
		{
			if (!VerifyDiskLimitBounds(false))
			{
				if (this.chkbx_DiskSpaceLimit.Checked)
				{
					//Call again to display the warning message
					VerifyDiskLimitBounds(true);
					this.chkbx_DiskSpaceLimit.Checked = false;
				}
			}
			else
			{
				foreach (ProcessMdiWindow window in mdiWindowList)
				{
					window.MaxDiskUsage = uint.Parse(this.diskLimitNumber.Value.ToString());
				}
			}
		}

		private void trkb_Network_ValueChanged(object sender, System.EventArgs e)
		{
			foreach (ProcessMdiWindow window in mdiWindowList)
			{
				if (window != null)
				{
					window.MaxNetworkSpeed = (uint)this.trkb_Network.Value;
				}
			}
		}

		private void chkbx_DiskSpaceLimit_CheckedChanged(object sender, System.EventArgs e)
		{
			foreach (ProcessMdiWindow window in mdiWindowList)
			{
				if (this.chkbx_DiskSpaceLimit.Checked)
				{
					if (VerifyDiskLimitBounds(true))
						window.LimitDiskUsage = true;
					else
					{
						window.LimitDiskUsage = false;
						chkbx_DiskSpaceLimit.Checked = false;
					}
				}
				else
					window.LimitDiskUsage = false;
			}
		}

		private void chkbx_MemoryLimit_CheckedChanged(object sender, System.EventArgs e)
		{
			foreach (ProcessMdiWindow window in mdiWindowList)
			{
				if (this.chkbx_MemoryLimit.Checked)
				{
					if (VerifyMemoryLimitBounds(true))
						window.LimitMemoryUsage = true;
					else
					{
						window.LimitMemoryUsage = false;
						chkbx_MemoryLimit.Checked = false;
					}
				}
				else
					window.LimitMemoryUsage = false;
			}
		}

		private void chkbx_ThrottleNetwork_CheckedChanged(object sender, System.EventArgs e)
		{
			foreach (ProcessMdiWindow window in mdiWindowList)
			{
				if (chkbx_ThrottleNetwork.Checked)
					window.LimitNetworkSpeed = true;
				else
					window.LimitNetworkSpeed = false;
			}
		}

		private void trkb_Network_Scroll(object sender, System.EventArgs e)
		{
			label_throttleLevel.Text = trkb_Network.Value.ToString() + " %";
		}
		
		private void memoryLimitScale_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if (!VerifyMemoryLimitBounds(false))
			{
				if (this.chkbx_MemoryLimit.Checked)
				{
					//Call again to display the warning message
					VerifyMemoryLimitBounds (true);
					this.chkbx_MemoryLimit.Checked = false;
				}
			}
			else
			{
				foreach (ProcessMdiWindow window in mdiWindowList)
				{
					window.MaxMemoryUsageUnit = this.memoryLimitScale.Text;
				}
			}
		
		}

		private void logFindMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeWindow = this.GetActiveChildWindow();

			if (activeWindow != null)
				activeWindow.ShowSearchDialog();
		}

		private void editCopyMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeWindow = this.GetActiveChildWindow();
			if (activeWindow != null)
			{
				String copyString = "";
				if (activeWindow.GetSelectedItemsText(ref copyString))
					System.Windows.Forms.Clipboard.SetDataObject (copyString, true);
			}
		}

		private void logFindNextMenuItem_Click(object sender, System.EventArgs e)
		{
			ProcessMdiWindow activeWindow = this.GetActiveChildWindow();

			if (activeWindow != null)
				activeWindow.SearchLogFile ();
		}

		#endregion
	}
}