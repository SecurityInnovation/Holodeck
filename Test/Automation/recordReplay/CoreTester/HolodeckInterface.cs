using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Microsoft.Win32;

namespace CoreTester
{
	/*Attila Ondi's UI framework...stripped down to bare functionality necessary for this program
	 *with some custom additions
	 */
	public class HolodeckInterface
	{
		public bool diskFault, crashed, faultWindowOpen, hasTabbed, memFault, netFault, startedRecording, startupOk;
        private int diskPos, memPos, netPos, tabPos;

		private IntPtr holodeckMainWindowHandle = IntPtr.Zero; //Handle to Holodeck's main window
		public Process holodeckProcess; //Holodeck's main process
		private String holodeckPath;

		//used windows built in methods to set the foreground window
		[DllImportAttribute("user32.dll")]
		protected static extern bool SetForegroundWindow(IntPtr windowHandle);

		//uses windows built in methods to find a window
		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowA(string className, string windowName);

		public HolodeckInterface()
		{
			//initialise the bools that we are going to be using
			initBool();
			initInt();
		}

		public bool closeHolodeck() 
		{
			//open an existing project
			WaitForHolodeck();

			sendKeyStrokes("%f"); //ALT-f (File menu)
			sendKeyStrokes("x"); //o (exit)
			sendKeyStrokes("n"); //no on the "do you want to save" dialog

			//wait for holodeck to exit
			holodeckProcess.WaitForExit();
			return true;
		}

		private int curMove(int whereToMove, int curPos) 
		{
			//how many times we have to press up to get to this position
            return (whereToMove - curPos);
		}

		private bool getDiskFault(string injection, int enabled) 
		{
			//provides the key strokes to get a disk fault
			switch (injection) 
			{
				case "Corrupt structure": 
				{
					moveDisk(enabled, 1);
					break;
				}
				case "Invalid path": 
				{
					moveDisk(enabled, 2);
					break;
				}
				case "Directory not empty": 
				{
					moveDisk(enabled, 3);
					break;
				}
				case "Can't remove directory": 
				{
					moveDisk(enabled, 4);
					break;
				}
				case "Can't find path":
				{
					moveDisk(enabled, 5);
					break;
				}
				case "Directory is read only": 
				{
					moveDisk(enabled, 6);
					break;
				}
				case "File name too long": 
				{
					moveDisk(enabled, 7);
					break;
				}
				case "File can't be created":
				{
					moveDisk(enabled, 8);
					break;
				}
				case "File already exists": 
				{
					moveDisk(enabled, 9);
					break;
				}
				case "Disk full": 
				{
					moveDisk(enabled, 10);
					break;
				}
				case "File locked": 
				{
					moveDisk(enabled, 11);
					break;
				}
				case "File in use": 
				{
					moveDisk(enabled, 12);
					break;
				}
				case "Can't seek disk": 
				{
					moveDisk(enabled, 13);
					break;
				}
				case "Data error": 
				{
					moveDisk(enabled, 14);
					break;
				}
				case "File is write protected": 
				{
					moveDisk(enabled, 15);
					break;
				}
				case "Access denied to file": 
				{
					moveDisk(enabled, 16);
					break;
				}
				case "Can't access file": 
				{
					moveDisk(enabled, 17);
					break;
				}
				case "File not found":
				{
					moveDisk(enabled, 18);
					break;
				}
			}
			return true;
		}

		private bool getMemoryFault(string injection, int enabled) 
		{
			//provides the key strokes to get a memory fault
			switch (injection) 
			{
				case "Page file too small": 
				{
					moveMemory(enabled, 1);
					break;
				}
				case "Invalid address": 
				{
					moveMemory(enabled, 2);
					break;
				}
				case "Segment locked": 
				{
					moveMemory(enabled, 3);
					break;
				}
				case "Invalid access": 
				{
					moveMemory(enabled, 4);
					break;
				}
				case "Insufficient memory": 
				{
					moveMemory(enabled, 5);
					break;
				}
			}

			return true;
		}

		private bool getNetworkFault(string injection, int enabled) 
		{
			//provides the key strokes to get a network fault
			switch (injection) 
			{
				case "Network down": 
				{
					moveNetwork(enabled, 1);
					break;
				}
				case "No ports available":
				{
					moveNetwork(enabled, 2);
					break;
				}
				case "Task limit reached": 
				{
					moveNetwork(enabled, 3);
					break;
				}
				case "Wrong version of Winsock": 
				{
					moveNetwork(enabled, 4);
					break;
				}
				case "Network not installed": 
				{
					moveNetwork(enabled, 5);
					break;
				}
				case "Network disconnected": 
				{
					moveNetwork(enabled, 6);
					break;
				}
			}
			return true;
		}

		public bool getFault(int type, string injection, int enabled) 
		{
			WaitForHolodeck();
			if (faultWindowOpen) 
			{
				//if the fault window is open
				switch (type) 
				{
					case 19: 
					{
						//memory fault
						getMemoryFault(injection, enabled);
						break;
					}

					case 21: 
					{
						//disk fault
						getDiskFault(injection, enabled);
						break;
					}

					case 23: 
					{
						//network fault so one tab will go here
						getNetworkFault(injection, enabled);
						break;
					}
				}
			}

			return true; 
		}

		public bool getFaultType(int faultType)
		{
			//gets to the correct fault pane based on the fault
			while (!holodeckProcess.Responding) 
			{
			}

			WaitForHolodeck();

			if (faultWindowOpen) 
			{
				//if the fault window is open 
				switch (faultType) 
				{
					case 19: 
					{
						//memory fault
						moveTab(3);
						break;
					}
					case 21: 
					{
						//disk fault
						moveTab(4);
						break;
					}
					case 23: 
					{
						//network fault so one tab will go here	
						moveTab(2);
						break;
					}
				}
			}
			return true;
		}

		public bool hasCrashed() 
		{
			//has the program crashed
			return crashed;
		}

		private bool initBool() 
		{
			//setup the bool values
			crashed = false;
			diskFault = false;
			faultWindowOpen = false;
			hasTabbed = false;
			memFault = false;
			netFault = false;
			startupOk = true;
			startedRecording = false;

			return true;
		}

		private bool initInt() 
		{
			diskPos = 0;
			memPos = 0;
			netPos = 0;
			tabPos = 0;
			return true;
		}

		public bool isFaultWindowOpen() 
		{
			//is the fault window open
			return faultWindowOpen;
		}

		public void LoadPaneSettings (string inputFilePath)
		{
			System.IO.StreamReader inputFile = new System.IO.StreamReader (inputFilePath);
			string fileContent = inputFile.ReadToEnd ();
			char[] newRegPaneSettings = fileContent.ToCharArray ();
			System.Text.Encoder encoder = System.Text.Encoding.Unicode.GetEncoder ();
			byte[] encodedPaneSettings = new byte[newRegPaneSettings.Length * 2];
			encoder.GetBytes (newRegPaneSettings, 0, newRegPaneSettings.Length, encodedPaneSettings, 0, true);
			Microsoft.Win32.Registry.LocalMachine.OpenSubKey ("SOFTWARE\\HolodeckEE", true).SetValue ("PaneSettings", encodedPaneSettings);
		}

		private bool moveDisk(int enabled, int pressUp) 
		{
			int pressUpI = 0;

			if (!diskFault) 
			{
				//first time we are moving in the disk pane
				if (enabled == 2) 
				{
					//enable it
					pressUpI = pressUp;
				}

				else 
				{
					//disable it
				}
				diskFault = true;
			}

			else 
			{
				//we have moved in the disk pane before
				if (enabled == 2) 
				{
					pressUpI = curMove(pressUp, diskPos);
				}

				else 
				{
					//disable it
				}
			}

			//show where the cursor is now in the disk pane
			setDiskPos(pressUp);

			if (pressUpI > 0) 
			{
				//we need to go down
				for (int pLoop = 0; pLoop < pressUpI; pLoop++) 
				{
					//press up this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}

					sendKeyStrokes("{UP}");
				}
			}

			else {
				//we need to go up
				pressUpI = pressUpI * -1;
				for (int pLoop = 0; pLoop < pressUpI; pLoop++) 
				{
					//press down this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}

					sendKeyStrokes("{DOWN}");
				}
			}

			WaitForHolodeck();
			return true;
		}

		private bool moveMemory(int enabled, int pressUp) 
		{
			int pressUpI = 0;

			if (!memFault) 
			{
				//first time we are moving in the memory pane
				if (enabled == 2) 
				{
					//enable it
					pressUpI = pressUp;
				}

				else 
				{
					//disable it
				}
				memFault = true;
			}

			else 
			{
				//we have moved in the memory pane before
				if (enabled == 2) 
				{
					pressUpI = curMove(pressUp, memPos);
				}

				else 
				{
					//disable it
				}
			}

			//show where the cursor is now in the memory pane
			setMemoryPos(pressUp);

			if (pressUpI > 0) 
			{
				//we need to go down
				for (int pLoop = 0; pLoop < pressUpI; pLoop++) 
				{
					//press up this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}

					sendKeyStrokes("{UP}");
				}
			}

			else 
			{
				//we need to go up
				pressUpI = pressUpI * -1;
				for (int pLoop = 0; pLoop < pressUpI; pLoop++) 
				{
					//press down this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}

					sendKeyStrokes("{DOWN}");
				}
			}

			WaitForHolodeck();
			return true;
		}

		private bool moveNetwork(int enabled, int pressUp) 
		{
			int pressUpI = 0;

			if (!netFault) 
			{
				//first time we are moving in the network pane
				if (enabled == 2) 
				{
					//enable it
					pressUpI = pressUp;
				}

				else 
				{
					//disable it
				}
				netFault = true;
			}

			else 
			{
				//we have moved in the network pane before
				if (enabled == 2) 
				{
					pressUpI = curMove(pressUp, netPos);
				}

				else 
				{
					//disable it
				}
			}

			//show where the cursor is now in the disk pane
			setNetworkPos(pressUp);

			if (pressUpI > 0) 
			{
				//we need to go down
				for (int pLoop = 0; pLoop < pressUpI; pLoop++) 
				{
					//press up this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}

					sendKeyStrokes("{UP}");
				}
			}

			else 
			{
				//we need to go up
				pressUpI = pressUpI * -1;
				for (int pLoop = 0; pLoop < pressUpI; pLoop++) 
				{
					//press down this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}

					sendKeyStrokes("{DOWN}");
				}
			}

			WaitForHolodeck();
			return true;
		}

		private bool moveTab(int pressTab) 
		{
			int pressTabI = 0;

			if (pressTab != tabPos) 
			{
				//only move the tab if it needs to be moved
				if (!hasTabbed) 
				{
					//first time we are tabbing
					pressTabI = pressTab;
					hasTabbed = true;
				}

				else 
				{
					//we have moved in the disk pane before
					pressTabI = tabMov(pressTab);
				}

				//show where the cursor is now in the disk pane
				setTabPos(pressTab);

				for (int pLoop = 0; pLoop < pressTabI; pLoop++) 
				{
					//press tab this many times
					while (!holodeckProcess.Responding) 
					{
						//wait before moving on
					}
				
					sendKeyStrokes("{TAB}");
				}
			}

			WaitForHolodeck();
			return true;
		}

		public bool openFaultWindow() 
		{
			//get the fault window up and running
			WaitForHolodeck();
			SetForegroundWindow(holodeckMainWindowHandle);

			while (!holodeckProcess.Responding) 
			{
				//wait for the project to be loaded before continuing
			}
			
			sendKeyStrokes("%a"); //ALT-a (Application menu)
			sendKeyStrokes("f"); //f (create a new fault)

			faultWindowOpen = true;

			WaitForHolodeck();
			return true;
		}

		public bool openProject(string pathIn) 
		{
			//open an existing project
			WaitForHolodeck();

			sendKeyStrokes("%f"); //ALT-f (File menu)
			sendKeyStrokes("o"); //o (open project)
			sendKeyStrokes(pathIn + "\\test1.hdp");//name of the project to open
			sendKeyStrokes("{ENTER}"); //open this project

			WaitForHolodeck();
			return true;
		}

		public void SavePaneSettings (string outputFilePath)
		{
			byte[] currentRegPaneSettings = (byte[]) Microsoft.Win32.Registry.LocalMachine.OpenSubKey ("SOFTWARE\\HolodeckEE").GetValue ("PaneSettings");
			System.IO.StreamWriter outputFile = new System.IO.StreamWriter (outputFilePath);
			System.Text.Decoder decoder = System.Text.Encoding.Unicode.GetDecoder ();
			char[] decodedPaneSettings = new char[currentRegPaneSettings.Length];
			decoder.GetChars (currentRegPaneSettings, 0, currentRegPaneSettings.Length, decodedPaneSettings, 0);
			outputFile.Write (decodedPaneSettings);
			outputFile.Close ();
		}

		private void setDiskPos(int posIn) 
		{
			//sets where the disk postion is
			diskPos = posIn;
		}

		private void setMemoryPos(int posIn) 
		{
			//sets where the memory postion is
			memPos = posIn;
		}

		private void setNetworkPos(int posIn) 
		{
			//sets where the network position is
			netPos = posIn;
		}

		private void setTabPos(int posIn) 
		{
			//sets where the tab is
			tabPos = posIn;
		}

		private void sendKeyStrokes(string keys) 
		{
			//send these keystrokes to the window
			holodeckProcess.WaitForInputIdle();
			System.Threading.Thread.Sleep(100);
			SendKeys.SendWait(keys);
		}

		public bool startHolodeck() 
		{	
			//start the holodeck program
			try 
			{
				//try to get Holodeck's path from the registry
				holodeckPath = (String) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
			}
			catch
			{
				//can't find where holodeck is installed on this machine
				MessageBox.Show("Unable to find holodeck's path from the registry", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				startupOk = false;
			}

			//start Holodeck based on this path
			holodeckProcess = Process.Start(holodeckPath + "\\HolodeckGui.exe");
			
			if (holodeckProcess.Handle == IntPtr.Zero) 
			{
				//unable to start holodeck
				MessageBox.Show("Unable to start holodeck", "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				startupOk = false;
			}

			WaitForHolodeck();
			holodeckMainWindowHandle = IntPtr.Zero;

			while (holodeckMainWindowHandle == IntPtr.Zero) 
			{
				//get the main handle for holodeck's window
				System.Threading.Thread.Sleep(System.TimeSpan.FromSeconds(1));
				holodeckMainWindowHandle = FindWindowA (null, "Holodeck Enterprise Edition");
			}

			//set holodeck as the foreground window
			SetForegroundWindow(holodeckMainWindowHandle);
			return true;
		}

		public bool startRecord() 
		{
			//start recording a session
			if (!startedRecording) 
			{
				//if we have not started recording
				while (!holodeckProcess.Responding) 
				{
				}

				sendKeyStrokes("%s"); //ALT-s (Session menu)
				sendKeyStrokes("r"); //record session

				//we are recording
				startedRecording = true;
			}

			WaitForHolodeck();
			return true;
		}

		public bool stopRecord() 
		{
			//stop recording a session
			if (startedRecording) 
			{
				//if we have started recording
				WaitForHolodeck();

				while (!holodeckProcess.Responding) 
				{
				}

				sendKeyStrokes("%s"); //ALT-s (Session menu)
			
				//hit down down arrow twice then enter to get to "stop record"
				sendKeyStrokes("{DOWN}");
				sendKeyStrokes("{DOWN}");
				sendKeyStrokes("{ENTER}");

				//no longer recording a session
				startedRecording = false;
			}

			WaitForHolodeck();
			return true;
		}

		private int tabMov(int whereToTab) 
		{
			int newTabPos = whereToTab - tabPos;

			if (newTabPos < 0)
			{
				if ((tabPos == 3) && (whereToTab == 1)) 
				{
					newTabPos = whereToTab;
				}

				else 
				{
					newTabPos = tabPos;
				}
			}

			return newTabPos;
		}

		public void WaitForHolodeck () 
		{
			// check if Holodeck is still running
			if (holodeckProcess.HasExited)
			{
			}

			// wait till Holodeck can respond to user input
			holodeckProcess.WaitForInputIdle();

			// check if Holodeck has crashed
			if ((FindWindowA ("WindowsForms10.Window.8.app1", "Microsoft .NET Framework") != IntPtr.Zero) || (FindWindowA (null, "HolodeckGUI.exe") != IntPtr.Zero)) 
			{
				//holodeck crashed we have a bug!
				crashed = true;
			}
		}
	}
}
