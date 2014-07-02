using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32;


namespace Holodeck
{
	/// <summary>
	/// Everything that doesn't really belong to any specific pane
	/// Specific pane classes should be inherited from this class
	/// </summary>
	public class Base
	{
		/// <summary>
		/// Imported Win32 functions
		/// </summary>

		[DllImportAttribute ("user32.dll")]
		protected static extern bool SetForegroundWindow (IntPtr windowHandle);

		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowA (string className, string windowName);


		/// <summary>
		/// The description of the Holodeck main process
		/// </summary>
		protected static Process holodeckProcess;
		/// <summary>
		/// Handle to Holodeck's main window
		/// Should be holodeckProcess.MainWindowHandle, but that's null for some reason...
		/// </summary>
		protected static IntPtr holodeckMainWindowHandle = IntPtr.Zero;


		/// <summary>
		/// Send keystrokes to the active window (should be Holodeck)
		/// </summary>
		/// <param name="keys">the actual keystrokes (see "SendWait method" in MSDN for exact syntax)</param>
		protected static void SendKeyStrokes (string keys) 
		{
			holodeckProcess.WaitForInputIdle ();
			System.Windows.Forms.SendKeys.SendWait (keys);
		}
		
		/// <summary>
		/// Set the text of the currently active editbox
		/// Note: assumes that the current focus is at an editbox
		/// </summary>
		/// <param name="text">the new text</param>
		protected static void SetEditBoxText (string text) 
		{
			// Select the text in the editbox
			SendKeyStrokes ("{HOME}"); // Home
			SendKeyStrokes ("+{END}"); // Shift-End

			// Replace it with the specified string
			SendKeyStrokes (text);
		}
	}

	/// <summary>
	/// Possible exceptions thrown from these class
	/// </summary>
	public class HolodeckExceptions: Base
	{
		public class CannotStartHolodeckException : System.Exception
		{
			// Empty
		}

		public class HolodeckExitedException : System.Exception
		{
			// Empty
		}

		public class IncorrectRegistryException : System.Exception
		{
			// Empty
		}

		public class CannotFindHolodeckMainWindowException : System.Exception
		{
			// Empty
		}

		public class HolodeckCrashedException : System.Exception
		{
			// Empty
		}

		public class UnaccessableControlException : System.Exception
		{
			// Empty
		}

	}

	/// <summary>
	/// Holodeck's process related stuff
	/// </summary>
	public class HolodeckProcess: Base
	{
		/// <summary>
		/// Start Holodeck
		/// </summary>
		public static void Start () 
		{
			// TODO: move the mouse pointer to one of the corners of the desktop to circumvent any interaction with Holodeck

			String holodeckPath;
			
			/* get Holodeck path from the registry */
			try 
			{
				holodeckPath = (String) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
			} 
			catch
			{
				throw new HolodeckExceptions.IncorrectRegistryException ();
			}

			/* start Holodeck based on the above path */;
			holodeckProcess = Process.Start (holodeckPath + "\\HolodeckGUI.exe");
			if (holodeckProcess.Handle == IntPtr.Zero) 
			{
				throw new HolodeckExceptions.CannotStartHolodeckException ();
			}

			holodeckProcess.WaitForInputIdle ();

			// Would be better to use holodeckProcess.MainWindowHandle, but it's null for some case...
			holodeckMainWindowHandle = IntPtr.Zero;
			while (holodeckMainWindowHandle == IntPtr.Zero) 
			{
				holodeckMainWindowHandle = FindWindowA ("WindowsForms10.Window.8.app1", "Holodeck Enterprise Edition");
			}

			SetForegroundWindow (holodeckMainWindowHandle);
		}

		/// <summary>
		/// Stop Holodeck (doesn't quite work yet...)
		/// </summary>
		/// <returns>exit status</returns>
		public static int Stop () 
		{
			WaitForHolodeck ();
			
			/* exit Holodeck */
			holodeckProcess.Close ();
			holodeckProcess.WaitForExit ();

			return holodeckProcess.ExitCode;
		}

		/// <summary>
		/// Wait 'till Holodeck can process new user inputs
		/// </summary>
		public static void WaitForHolodeck () 
		{
			// check if Holodeck is still running
			if (holodeckProcess.HasExited)
			{
				throw new HolodeckExceptions.HolodeckExitedException ();
			}

			// wait till Holodeck can respond to user input
			holodeckProcess.WaitForInputIdle ();

			// check if Holodeck has crashed
			if ((FindWindowA ("WindowsForms10.Window.8.app1", "Microsoft .NET Framework") != IntPtr.Zero) ||
				(FindWindowA (null, "HolodeckGui.exe") != IntPtr.Zero)) 
			{
				throw new HolodeckExceptions.HolodeckCrashedException ();
			}
		}
	} // class HolodeckProcess


	/// <summary>
	/// Holodeck's main window
	/// </summary>
	public class HolodeckPane: Base
	{
		/// <summary>
		/// Set Holodeck's main window to its default state
		/// </summary>
		public static void Reset () {
			HolodeckProcess.WaitForHolodeck ();

			/* TODO: return to the default state */
		}

		/// <summary>
		/// Bring up "Create A New Project" wizard
		/// </summary>
		public static void NewProject () {
			SendKeyStrokes ("%f"); //ALT-f (File menu)
			SendKeyStrokes ("n"); //n (New project)
		}


	} // class HolodeckPane


	/// <summary>
	/// The first page of the "Create A New Project" wizard
	/// </summary>
	public class NewProjectPane1: Base
	{
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			ProjectLocation,
			Browse,
			Cancel,
			Next,
			Finish				// enabled if the 3rd page had been visited
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.ProjectLocation;
		
		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () 
		{
			switch (currPos) 
			{
				case TabStop.ProjectLocation: 
				{
					currPos = TabStop.Browse;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Browse: 
				{
					if (!NewProjectPane3.IsVisited ()) 
					{
						currPos = TabStop.Cancel;
					} 
					else 
					{
						currPos = TabStop.Finish;
					}
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Cancel: 
				{
					currPos = TabStop.Next;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Next: 
				{
					currPos = TabStop.ProjectLocation;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Finish: 
				{
					currPos = TabStop.Cancel;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				default: 
				{
					break;
				}
			}
		}

		/// <summary>
		/// Change the focus to the specified control
		/// </summary>
		/// <param name="toPos">the desired control</param>
		private static void GoTo (TabStop toPos) 
		{
			// TODO: Throw an exception if the specified control is unreachable in the current state

			while (currPos != toPos) 
			{
				GoToNext ();
			}
		}
		
		/// <summary>
		/// Bring this pane to its default state
		/// </summary>
		public static void Reset () 
		{
			// TODO: reset the pane

			if (NewProjectPane3.IsVisited ()) 
			{
				SendKeyStrokes ("{TAB}"); // Tab
			}

			currPos = TabStop.ProjectLocation;
		}

		/// <summary>
		/// Set the location of the new project
		/// </summary>
		/// <param name="location">the project's location</param>
		public static void SetProjectLocation (string location) 
		{
			GoTo (TabStop.ProjectLocation);

			SetEditBoxText (location);
		}
		
		/// <summary>
		/// Select and activate the "Next" button
		/// </summary>
		public static void Next () 
		{
			GoTo (TabStop.Next);

			SendKeyStrokes (" "); // Space
		}
	} // class NewProjectPane1


	/// <summary>
	/// The second page of the "Create A New Project" wizard
	/// </summary>
	public class NewProjectPane2: Base
	{
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			LaunchApplication,
			AttachToApplication,
			ApplicationName,
			Browse,
			Parameters,
			Refresh,
			PIDs,
			Cancel,
			Back,
			Next,
			Finish					// enabled if the 3rd page had been visited
		}

		/// <summary>
		/// Teh possible states of the pane
		/// </summary>
		private enum Selection
		{
			Launch,	// Launch Application
			Attach	// Attach to Application
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.LaunchApplication;
		/// <summary>
		/// The actual state of the pane
		/// </summary>
		private static Selection selectionStatus = Selection.Launch;
		
		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () 
		{
			// TODO: figure out a better way if possible

			switch (selectionStatus) 
			{
				case Selection.Launch: 
				{
					switch (currPos) 
					{
						case TabStop.LaunchApplication: 
						{
							currPos = TabStop.PIDs;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}

						case TabStop.AttachToApplication: // unreachable by TABs
						{
							currPos = TabStop.LaunchApplication;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}

						case TabStop.ApplicationName: 
						{
							currPos = TabStop.Browse;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Browse: 
						{
							currPos = TabStop.LaunchApplication;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Parameters: 
						{
							if (!NewProjectPane3.IsVisited ()) 
							{
								currPos = TabStop.Cancel;
							} 
							else 
							{
								currPos = TabStop.Finish;
							}
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
/*
						case TabStop.Refresh: // disabled
						{
							break;
						}
*/
						case TabStop.PIDs: 
						{
							currPos = TabStop.Parameters;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Cancel: 
						{
							currPos = TabStop.Next;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Back: 
						{
							currPos = TabStop.ApplicationName;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Next: 
						{
							currPos = TabStop.Back;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Finish: 
						{
							currPos = TabStop.Cancel;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						default : 
						{
							break;
						}
					}
					break;
				} // case Selection.Launch
				case Selection.Attach: 
				{
					switch (currPos) 
					{
/*
						case TabStop.LaunchApplication: // unreachable by TABs
						{
							break;
						}
*/
						case TabStop.AttachToApplication: 
						{
							currPos = TabStop.PIDs;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
/*
						case TabStop.ApplicationName: // disabled
						{
							break;
						}
						case TabStop.Browse: // disabled
						{
							break;
						}
						case TabStop.Parameters: //disabled
						{
							break;
						}
*/
						case TabStop.Refresh: 
						{
							if (!NewProjectPane3.IsVisited ()) 
							{
								currPos = TabStop.Cancel;
							} 
							else 
							{
								currPos = TabStop.Finish;
							}
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.PIDs: 
						{
							currPos = TabStop.Refresh;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Cancel: 
						{
							currPos = TabStop.Next;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Back: 
						{
							currPos = TabStop.LaunchApplication;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Next: 
						{
							currPos = TabStop.Back;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Finish: 
						{
							currPos = TabStop.Cancel;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						default : 
						{
							break;
						}
					}
					break;
				} // case Selection.Attach
				default: 
				{
					break;
				}
			}
		}
		
		/// <summary>
		/// Change the focus to the specified control
		/// </summary>
		/// <param name="toPos">the desired control</param>
		private static void GoTo (TabStop toPos) 
		{
			// TODO: Throw an exception if the specified control is unreachable in the current state

			while (currPos != toPos) 
			{
				GoToNext ();
			}
		}
		
		/// <summary>
		/// Bring this pane to its default state
		/// </summary>
		public static void Reset () 
		{
			// TODO: figure out a better way to handle the first time selection (which is between Back and ApplicationName)

			SendKeyStrokes ("{TAB}"); // Tab
			currPos = TabStop.ApplicationName;
			selectionStatus = Selection.Launch;
		}

		/// <summary>
		/// Set the name of the application
		/// </summary>
		/// <param name="name">the application's name</param>
		public static void SetApplicationName (string name) 
		{
			GoTo (TabStop.ApplicationName);

			SetEditBoxText (name);
		}
		
		/// <summary>
		/// Select and activate the "Next" button
		/// </summary>
		public static void Next () 
		{
			GoTo (TabStop.Next);

			SendKeyStrokes (" "); // Space
		}

		/// <summary>
		/// Select and activate the "Back" button
		/// </summary>
		public static void Back () 
		{
			GoTo (TabStop.Back);

			SendKeyStrokes (" "); // Space
		}

		/// <summary>
		/// Select and activate the "Finish" button
		/// </summary>
		public static void Finish () 
		{
			if (!NewProjectPane3.IsVisited ()) 
			{
				throw new HolodeckExceptions.UnaccessableControlException ();
			}

			GoTo (TabStop.Finish);

			SendKeyStrokes (" "); // Space

			// Clean up after clicking "Finish"
			NewProjectPane3.CleanUp ();
		}

		/// <summary>
		/// Select the "Attach to Aplication" checkbox
		/// </summary>
		public static void AttachToApplication () 
		{
			if (selectionStatus != Selection.Attach) 
			{
				GoTo (TabStop.LaunchApplication);

				SendKeyStrokes ("{LEFT}"); // Left arrow
			}
		}
	} // class NewProjectPane2


	/// <summary>
	/// The third page of the "Create A New Project" wizard
	/// </summary>
	public class NewProjectPane3: Base
	{
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			PauseApplication,
			ProcessChaining,
	//		NativeFunctions,
			Functions,
			Cancel,
			Back,
			Finish
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.PauseApplication;
		/// <summary>
		/// The current status of the "Pause Application" checkbox
		/// true - checked; false - unchecked
		/// </summary>
		private static bool pauseApplicationState = false; // TODO: enclose it to an object
		/// <summary>
		/// The current status of the "Process Chaining" checkbox
		/// true - checked; false - unchecked
		/// </summary>
		private static bool processChainingState = true; // TODO: enclose it to an object
		/// <summary>
		/// The current status of the "Native Functions" checkbox
		/// true - checked; false - unchecked
		/// </summary>
		private static bool nativeFunctionsState = true; // TODO: enclose it to an object
		/// <summary>
		/// Has the pane been visited yet
		/// </summary>
		private static bool isVisited = false;

		/// <summary>
		/// Return if the pane has been visited
		/// </summary>
		/// <returns>has the pane been visited</returns>
		public static bool IsVisited () 
		{
			return isVisited;
		}

		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () 
		{
			switch (currPos) 
			{
				case TabStop.PauseApplication: 
				{
					currPos = TabStop.ProcessChaining;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.ProcessChaining:
				{
				//	currPos = TabStop.NativeFunctions;
					currPos = TabStop.Functions;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
		/*      case TabStop.NativeFunctions:
				{
					currPos = TabStop.Finish;
					SendKeyStrokes("(TAB)");  // Tab
					break;
				}*/
				case TabStop.Functions: 
				{
					currPos = TabStop.Finish;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Cancel: 
				{
					currPos = TabStop.Back;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Back: 
				{
					currPos = TabStop.PauseApplication;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Finish: 
				{
					currPos = TabStop.Cancel;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				default : 
				{
					break;
				}
			}
		}
		
		/// <summary>
		/// Change the focus to the specified control
		/// </summary>
		/// <param name="toPos">the desired control</param>
		private static void GoTo (TabStop toPos) 
		{
			while (currPos != toPos) 
			{
				GoToNext ();
			}
		}
		
		/// <summary>
		/// Bring this pane to its default state
		/// </summary>
		public static void Reset () 
		{
			// TODO: figure out a better way to handle the first time selection (which is between Back and PauseApplication)

			SendKeyStrokes ("{TAB}"); // Tab
			currPos = TabStop.PauseApplication;
			isVisited = true;
		}

		/// <summary>
		/// Set the state of the "Pause Application" checkbox
		/// </summary>
		/// <param name="newState">the new state</param>
		public static void SetPauseApplication (bool newState) 
		{
			if (newState != pauseApplicationState) 
			{
				GoTo (TabStop.PauseApplication);

				SendKeyStrokes (" "); // Space
			}
		}
		
		/// <summary>
		/// Set the state of the "Process Chaining" checkbox
		/// </summary>
		/// <param name="newState">the new state</param>
		public static void SetProcessChaining (bool newState) 
		{
			if (newState != processChainingState) 
			{
				GoTo (TabStop.ProcessChaining);

				SendKeyStrokes (" "); // Space
			}
		}
		
		/// <summary>
		/// Set the state of the "Native Functions" checkbox
		/// </summary>
		/// <param name="newState">the new state</param>
		public static void SetNativeFunctions (bool newState) 
		{
			if (newState != nativeFunctionsState) 
			{
			//	GoTo (TabStop.NativeFunctions);
				
				SendKeyStrokes (" "); // Space
			}
		}
		

		/// <summary>
		/// Select and activate the "Back" button
		/// </summary>
		public static void Back () 
		{
			GoTo (TabStop.Back);

			SendKeyStrokes (" "); // Space
		}

		/// <summary>
		/// Select and activate the "Finish" button
		/// </summary>
		public static void Finish () 
		{
			GoTo (TabStop.Finish);

			SendKeyStrokes (" "); // Space

			// Clean up after clicking "Finish"
			CleanUp ();
		}

		/// <summary>
		/// Clean up work after clicking "Finish" on any of the panes
		/// </summary>
		public static void CleanUp () 
		{
			// The next time the wizard is invoked, this pane will not have been visied
			isVisited = false;

			// Bring Holodeck's main window back to focus
			// TODO: figure out a better way to do this (we should not "hack" like this here...)
			HolodeckProcess.WaitForHolodeck ();
			SetForegroundWindow (holodeckMainWindowHandle);
		}
	} // class NewProjectPane3
} // namespace Holodeck