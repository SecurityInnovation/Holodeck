using System;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Windows.Forms;
using Microsoft.Win32;

namespace Holodeck
{
	public struct WindowInfo
	{
		public string title;
		public IntPtr hWnd;
	}

	public struct WindowEnvironment
	{
		public bool					checkTitle;
		public string				title;
		public WindowEnvironment[]	childs;
		public bool					searchedHandle;
	}

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
		public static extern bool SetForegroundWindow (IntPtr windowHandle);

		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowA (string className, string windowName);
        
		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowExA (IntPtr parentHWnd, IntPtr fromChildHWnd, string className, string windowName);

		[DllImportAttribute("user32.dll")]
		protected static extern int GetWindowTextLengthA (IntPtr hWnd);

		[DllImportAttribute("user32.dll")]
		protected static extern int GetWindowTextA (IntPtr hWnd, StringBuilder windowText, int maxLength);

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
		/// Gets information on a window specified by its handle
		/// -- Might throw a NullReferenceException
		/// </summary>
		/// <param name="hWnd">the handle to the window to query</param>
		/// <returns>a WindowInfo containing the queried information about the specified window</returns>
		public static WindowInfo GetWindowInfo (IntPtr hWnd)
		{
			if (hWnd == IntPtr.Zero) 
			{
				throw new NullReferenceException ();
			}

			WindowInfo info;
			info.hWnd = IntPtr.Zero;
			info.title = "";

			int length = GetWindowTextLengthA (hWnd);
			StringBuilder sb = new StringBuilder (length + 1);
			GetWindowTextA (hWnd, sb, sb.Capacity);
			info.title = sb.ToString ();
			info.hWnd = hWnd;

			return info;
		}

		/// <summary>
		/// Enumerates all the child windows of a parent window
		/// </summary>
		/// <param name="parentHWnd">the handle to the parent window</param>
		/// <returns>list of WindowInfo structures containing the data of the child windows</returns>
		public static WindowInfo[] EnumChildWindows (IntPtr parentHWnd) 
		{
			System.Collections.ArrayList childWindowArray = new System.Collections.ArrayList (10);

			IntPtr lastChildHWnd = FindWindowExA (parentHWnd, IntPtr.Zero, null, null);
			while (lastChildHWnd != IntPtr.Zero) 
			{
				childWindowArray.Add (GetWindowInfo (lastChildHWnd));
				lastChildHWnd = FindWindowExA (parentHWnd, lastChildHWnd, null, null);
			}

			WindowInfo[] returnArray = new WindowInfo[childWindowArray.Count];
			for (int i = 0; i < childWindowArray.Count; i++) 
			{
				returnArray[i] = (WindowInfo) childWindowArray[i];
			}

			return returnArray;
		}

		/// <summary>
		/// Matches a specific window environment against a specified window and its childs
		/// -- Might throw a HolodeckExceptions.MismatchingWindowEnvironmentException
		/// </summary>
		/// <param name="info">the window info to match against the window environment</param>
		/// <param name="environment">the window environment to match against</param>
		/// <returns>the handler to the matched window (specified by the environment), or null if no such window was found</returns>
		protected static IntPtr MatchEnvironment (WindowInfo info, WindowEnvironment environment)
		{
			IntPtr searchedHandle = IntPtr.Zero;

			WindowInfo[] array = EnumChildWindows (info.hWnd);
			bool matchTitle = (!environment.checkTitle || 
							   (environment.checkTitle && (info.title == environment.title)));
			bool matchChildSize = (((environment.childs == null) && (array.Length == 0)) ||
								   (environment.childs.Length == array.Length));
			if (matchTitle &&
				matchChildSize) 
			{
				if (environment.searchedHandle) 
				{
					// We might have found our window, but we also need to check the child environments.
					// Store our handle in searchedHandle for now.
					searchedHandle = info.hWnd;
				}

				if (environment.childs != null) 
				{
					for (int i = 0; i < array.Length; i++) 
					{
						IntPtr retHandle = MatchEnvironment (array[i], environment.childs[i]);
						// MatchEnvironment might throw an exception, but we don't need to worry about it here
						if (retHandle != IntPtr.Zero) 
						{
							searchedHandle = retHandle;
						}
					}
				}

				// If we got here, that means that all the child environments matched as well.
				// searchHandle is not IntPtr.Zero if we've found our window.
				return searchedHandle;
			} 
			else 
			{
				// We need to throw this to differentiate from matching sub-environments that do not contain the searched window handle.
				throw new HolodeckExceptions.MismatchingWindowEnvironmentException ("The window doesn't match the specified environment");
			}
		}

		/// <summary>
		/// Searches for a specified window environment to get the desired pane.
		/// -- Might throw an HolodeckExceptions.MismatchingWindowEnvironmentException
		/// </summary>
		/// <param name="hWnd">the current window to match against the specified environment</param>
		/// <param name="environment">the WindowEnvironment to match against</param>
		/// <returns>the handler to the matched window (specified by the environment), or null if no such window was found</returns>
		protected static IntPtr FindHolodeckPane2 (IntPtr hWnd, WindowEnvironment environment)
		{
			try 
			{
				IntPtr window = MatchEnvironment (GetWindowInfo (hWnd), environment);
				if (window != IntPtr.Zero) 
				{
					// We've found the window we were looking for.
					return window;
				} 
				else 
				{
					// We shouldn't've gotten here... Being here means the environments matched, but there was no searchedHandle set to true in the environment variable.
					throw new HolodeckExceptions.MismatchingWindowEnvironmentException ("Bad WindowEnvironment setting. No searchedHandle was specified.");
				}
			} 
			catch (HolodeckExceptions.MismatchingWindowEnvironmentException) 
			{
				// The environment didn't match for this window. Carry on to check its child windows.
			}

			Holodeck.WindowInfo[] array = Holodeck.Base.EnumChildWindows (hWnd);
			foreach (Holodeck.WindowInfo tempInfo in array)
			{
				IntPtr window = FindHolodeckPane2 (tempInfo.hWnd, environment);
				if (window != IntPtr.Zero) 
				{
					return window;
				}
			}

			return IntPtr.Zero;
		}

		/// <summary>
		/// Searches for a specified window environment to get the desired pane.
		/// -- Might throw a HolodeckExceptions.MismatchingWindowEnvironmentException
		/// </summary>
		/// <param name="environment">the WindowEnvironment to match against</param>
		/// <returns>a handle to the found pane; or IntPtr, if there were no matching pane</returns>
		public static IntPtr FindHolodeckPane (WindowEnvironment environment)
		{
			return FindHolodeckPane2 (Holodeck.Base.GetMainWindowHandle (), environment);
		}

		/// <summary>
		/// Gets the entries of a ListView up to a specified number of columns
		/// -- Might throw a Helper_dll.Helper.HelperException
		/// </summary>
		/// <param name="hWnd">the handle to the ListView</param>
		/// <param name="columnNumber">the number of columns to query</param>
		/// <returns>the entries of the ListView; the first dimension is the row, the second is the column</returns>
		protected static string[][] GetListViewEntries (IntPtr hWnd, int columnNumber)
		{
			int itemNumber = Helper_dll.Helper.GetListViewItemNumber (hWnd);
			String[][] retArray = new String[itemNumber][];
			for (int i = 0; i < itemNumber; i++) 
			{
				retArray[i] = new String[columnNumber];
			}

			for (int i = 0; i < itemNumber; i++)
			{
				for (int j = 0; j < columnNumber; j++) 
				{
					string itemText = Helper_dll.Helper.GetListViewItemText (GetProcessID (), hWnd, i, j);
					retArray[i][j] = itemText;
				}
			}

			return retArray;
		}

		/// <summary>
		/// Returns Holodecks main process ID
		/// </summary>
		public static uint GetProcessID () 
		{
			return (uint) holodeckProcess.Id;
		}

		/// <summary>
		/// Returns the handle to Holodeck's main window
		/// </summary>
		public static IntPtr GetMainWindowHandle () 
		{
			return holodeckMainWindowHandle;
		}

		/// <summary>
		/// Send keystrokes to the active window (should be Holodeck)
		/// </summary>
		/// <param name="keys">the actual keystrokes (see "SendWait method" in MSDN for exact syntax)</param>
		public static void SendKeyStrokes (string keys) 
		{
			SendKeyStrokes (keys, 100);
		}
		
		/// <summary>
		/// Send keystrokes to the active window (should be Holodeck)
		/// </summary>
		/// <param name="keys">the actual keystrokes (see "SendWait method" in MSDN for exact syntax)</param>
		/// <param name="waitMilliseconds">the number of seconds to wait before sending the keystroke</param>
		protected static void SendKeyStrokes (string keys, int waitMilliseconds) {
			holodeckProcess.WaitForInputIdle ();
			System.Threading.Thread.Sleep (waitMilliseconds);
			System.Windows.Forms.SendKeys.SendWait (keys);
		}
		
		/// <summary>
		/// Sets the text of the currently active editbox
		/// Note: assumes that the current focus is at an EditBox
		/// </summary>
		/// <param name="text">the new text</param>
		protected static void SetEditBoxText (string text) 
		{
			// Select the text in the EditBox
			SendKeyStrokes ("{HOME}"); // Home
			SendKeyStrokes ("+{END}"); // Shift-End

			// Replace it with the specified string
			SendKeyStrokes (text);
		}

		/// <summary>
		/// Gets the text of the currently active EditBox
		/// Note: assumes that the current focus is at an EditBox
		/// </summary>
		/// <returns>the text of the EditBox</returns>
		protected static string GetEditBoxText ()
		{
			return GetEditBoxText (true);
		}

		/// <summary>
		/// Gets the text of the currently active EditBox
		/// Note: assumes that the current focus is at an EditBox
		/// </summary>
		/// <param name="select">specifies if the text in the editbox needs to be selected</param>
		/// <returns>the text of the EditBox</returns>
		protected static string GetEditBoxText (bool select) 
		{
			if (select) {
				// Select the text in the editbox
				SendKeyStrokes ("{HOME}");	// Home
				SendKeyStrokes ("+{END}");	// Shift-End
			}

			// This is to be able to check if we have copied anything from the clipboard
			string inValidText = "UIFramework - This is not a valid clipboard text";
			Helper_dll.Helper.SetTextToClipboard (inValidText);

			SendKeyStrokes ("^c");		// Ctrl-c

			string clipboardText = Helper_dll.Helper.GetTextFromClipboard ();
			if (clipboardText == inValidText) {
				// The editbox was empty
				clipboardText = string.Empty;
			}

//			IDataObject ido = Clipboard.GetDataObject ();
//			string clipboardText = ido.GetData (DataFormats.Text);

//System.Console.WriteLine ("clipboard: " + clipboardText);
			return clipboardText;
		}

		/// <summary>
		/// Saves the [HKEY_LOCAL_MACHINE\SOFTWARE\HolodeckEE\PaneSettings] registry value to the specified file
		/// </summary>
		/// <param name="outputFilePath">the path of the file to save to</param>
		public static void SavePaneSettings (string outputFilePath)
		{
			byte[] currentRegPaneSettings = (byte[]) Microsoft.Win32.Registry.LocalMachine.OpenSubKey ("SOFTWARE\\HolodeckEE").GetValue ("PaneSettings");
			System.IO.StreamWriter outputFile = new System.IO.StreamWriter (outputFilePath);
			System.Text.Decoder decoder = System.Text.Encoding.Unicode.GetDecoder ();
			char[] decodedPaneSettings = new char[currentRegPaneSettings.Length];
			decoder.GetChars (currentRegPaneSettings, 0, currentRegPaneSettings.Length, decodedPaneSettings, 0);
			outputFile.Write (decodedPaneSettings);
			outputFile.Close ();
		}

		/// <summary>
		/// Loads the [HKEY_LOCAL_MACHINE\SOFTWARE\HolodeckEE\PaneSettings] registry value from the specified file
		/// </summary>
		/// <param name="inputFilePath">the path of the file to read from</param>
		public static void LoadPaneSettings (string inputFilePath)
		{
			System.IO.StreamReader inputFile = new System.IO.StreamReader (inputFilePath);
			string fileContent = inputFile.ReadToEnd ();
			char[] newRegPaneSettings = fileContent.ToCharArray ();
			System.Text.Encoder encoder = System.Text.Encoding.Unicode.GetEncoder ();
			byte[] encodedPaneSettings = new byte[newRegPaneSettings.Length * 2];
			encoder.GetBytes (newRegPaneSettings, 0, newRegPaneSettings.Length, encodedPaneSettings, 0, true);
			Microsoft.Win32.Registry.LocalMachine.OpenSubKey ("SOFTWARE\\HolodeckEE", true).SetValue ("PaneSettings", encodedPaneSettings);
		}
	}

	/// <summary>
	/// Possible exceptions thrown from these classes
	/// </summary>
	public class HolodeckExceptions: Base
	{
		public class HolodeckAlreadyRunningException : System.Exception
		{
			// Empty
		}

		public class CannotStartHolodeckException : System.Exception
		{
			public CannotStartHolodeckException (string message) : base (message)
			{
			}
		}

		public class HolodeckExitedException : System.Exception
		{
			// Empty
		}

		public class IncorrectRegistryException : System.Exception
		{
			public IncorrectRegistryException (string message) : base (message)
			{
			}
		}

		public class CannotFindHolodeckMainWindowException : System.Exception
		{
			// Empty
		}

		public class HolodeckCrashedException : System.Exception
		{
			public HolodeckCrashedException (string message) : base (message)
			{
			}
		}

		public class UnaccessableControlException : System.Exception
		{
			// Empty
		}

		public class MismatchingWindowEnvironmentException : System.Exception
		{
			public MismatchingWindowEnvironmentException (string message) : base (message)
			{
			}
		}

		public class IncorrectOperationException : System.Exception 
		{
			public IncorrectOperationException (string message) : base (message)
			{
			}
		}

		public class WindowNotFoundException : System.Exception 
		{
			public WindowNotFoundException (string message) : base (message)
			{
			}
		}

		public class CannotFindApplication : System.Exception
		{
			public CannotFindApplication (string message) : base (message)
			{
			}
		}

	}

	/// <summary>
	/// Holodeck's process related stuff
	/// </summary>
	public class HolodeckProcess: Base
	{
		/// <summary>
		/// Common crash window titles
		/// </summary>
		protected static string UnhandledExceptionWindowTitle = "Microsoft .NET Framework";
		protected static string NativeProblemWindowTitle = "HolodeckGUI.exe";
		/// <summary>
		/// An ArrayList containing the specified crash windows to look for
		/// </summary>
		protected static System.Collections.ArrayList crashWindows = new System.Collections.ArrayList (10);

		protected struct CrashWindow
		{
			public string title;
			public string labelContains;
		}

		/// <summary>
		/// Checks if Holodeck is still running.
		/// </summary>
		/// <returns>true if the Holodeck process is still running, false otherwise</returns>
		public static bool IsRunning ()
		{
			return !holodeckProcess.HasExited;
		}

		/// <summary>
		/// Start Holodeck
		/// -- Might throw a HolodeckExceptions.HolodeckAlreadyRunningException
		/// -- Might throw a HolodeckExceptions.IncorrectRegistryException
		/// -- Might throw a HolodeckExceptions.CannotStartHolodeckException
		/// </summary>
		public static void Start () 
		{
			Start (50);
		}

		/// <summary>
		/// Start Holodeck
		/// -- Might throw a HolodeckExceptions.HolodeckAlreadyRunningException
		/// -- Might throw a HolodeckExceptions.IncorrectRegistryException
		/// -- Might throw a HolodeckExceptions.CannotStartHolodeckException
		/// </summary>
		/// <param name="sleepSeconds">max period of time (in number of seconds) to find Holodeck's main window</param>
		public static void Start (int sleepSeconds) 
		{
			// TODO: move the mouse pointer to one of the corners of the desktop to circumvent any interaction with Holodeck

			IntPtr oldHolodeckHandle = FindWindowA (null, "Holodeck Enterprise Edition");
			if (oldHolodeckHandle != IntPtr.Zero) 
			{
				throw new HolodeckExceptions.HolodeckAlreadyRunningException ();
			}

			CrashWindow crashWindow;
			crashWindow.title = UnhandledExceptionWindowTitle;
			crashWindow.labelContains = "An unhandled exception has occurred in your application";
			crashWindows.Add (crashWindow);

			crashWindow.title = NativeProblemWindowTitle;
			crashWindow.labelContains = "has encountered a problem";
			crashWindows.Add (crashWindow); // TODO: find out the label text.

			String holodeckPath;
			
			/* get Holodeck path from the registry */
			string regKeyName = "Software\\HolodeckEE";
			string regValueName = "InstallPath";
			try 
			{
				holodeckPath = (String) Registry.LocalMachine.OpenSubKey (regKeyName, true).GetValue (regValueName);
			} 
			catch
			{
				throw new HolodeckExceptions.IncorrectRegistryException ("Couldn't find registry key \"" + regKeyName + "\" or the value: \"" + regKeyName + "\" in it");
			}

			/* start Holodeck based on the above path */;
			holodeckProcess = Process.Start (holodeckPath + "\\HolodeckGUI.exe");

			if (holodeckProcess.Handle == IntPtr.Zero) 
			{
				throw new HolodeckExceptions.CannotStartHolodeckException ("Couldn't start the process");
			}

			WaitForHolodeck();

			// Would be better to use holodeckProcess.MainWindowHandle, but it's null for some case...
			holodeckMainWindowHandle = IntPtr.Zero;
			int tryNum = 0;
			while ((holodeckMainWindowHandle == IntPtr.Zero) && (tryNum < sleepSeconds)) 
			{
				System.Threading.Thread.Sleep (System.TimeSpan.FromSeconds (1));	// sleep for one second
				holodeckMainWindowHandle = FindWindowA (null, "Holodeck Enterprise Edition");
				tryNum++;
			}

			if (holodeckMainWindowHandle == IntPtr.Zero)
			{
				throw new HolodeckExceptions.CannotStartHolodeckException ("Couldn't find the main window");
			}

			SetForegroundWindow (holodeckMainWindowHandle);
/*
			SendKeyStrokes ("% ");
			SendKeyStrokes ("{ESC}");
*/
		}

		/// <summary>
		/// Stop Holodeck (doesn't quite work yet...)
		/// </summary>
		/// <returns>exit status</returns>
		public static int Stop () 
		{
			WaitForHolodeck ();
			
			/* exit Holodeck */
			// This commented code is for if Kill () does not kill all the processes started by Holodeck...
			//			foreach(object o in holodeckProcess.Threads)
			//			{
			//				((Thread) o).Abort();
			//			}
			holodeckProcess.Kill ();
			holodeckProcess.WaitForExit ();

			return holodeckProcess.ExitCode;
		}

		/// <summary>
		/// Adds a new window to look for when deciding if Holodeck has crashed
		/// </summary>
		/// <param name="title">the title of the window to look for</param>
		/// <param name="labelContains">a substring of a label on the window to look for</param>
		public static void AddHolodeckCrashWindow (string title, string labelContains)
		{
			CrashWindow crashWindow;
			crashWindow.title = title;
			crashWindow.labelContains = labelContains;
			crashWindows.Add (crashWindow);
		}

		/// <summary>
		/// Searches for a label containing the specified string in a given window and in all its child windows
		/// </summary>
		/// <param name="info">the WindowInfo containing the handle to a window</param>
		/// <param name="labelSubString">the string to look for in the labels</param>
		/// <returns>the full text of the label if it was found, or string.Empty otherwise</returns>
		protected static string SearchForLabel (WindowInfo info, string labelSubString)
		{
			if (info.title.IndexOf (labelSubString) != -1) 
			{
				// We've found the specified label
				return info.title;
			}

			WindowInfo[] array = EnumChildWindows (info.hWnd);

			foreach (WindowInfo tempInfo in array)
			{
				string title = SearchForLabel (tempInfo, labelSubString);

				if (title != string.Empty) 
				{
					// One of our child window was the specified label
					return title;
				}
			}

			// None of our child window had the specified label
			return string.Empty;
		}

		/// <summary>
		/// Searches for a given crash window.
		/// Throws a HolodeckCrashedException if the crash window was found.
		/// -- Might throw a HolodeckExceptions.HolodeckCrashedException
		/// </summary>
		/// <param name="crashWindow">the crash window info to look for</param>
		protected static void SearchForCrashWindow (CrashWindow crashWindow)
		{
			IntPtr window = FindWindowA (null, crashWindow.title);
			if (window != IntPtr.Zero)
			{
				WindowInfo windowInfo = GetWindowInfo (window);
				string label = SearchForLabel (windowInfo, crashWindow.labelContains);
				if (label != string.Empty) 
				{
					// we've found the specified label -> Holodeck is crashed

					if (crashWindow.title == UnhandledExceptionWindowTitle)
					{
						label = SearchForLabel (windowInfo, "Exception Text");
						if (label != string.Empty) 
						{
							throw new HolodeckExceptions.HolodeckCrashedException (label);
						}
						// it seems that even if the "Details" button is unpressed, we still have the editbox containing "Exception Text", therefore we don't need the else branch...
					}

					// TODO: provide better exception message
					throw new HolodeckExceptions.HolodeckCrashedException (label);
				}
			}
		}

		/// <summary>
		/// Brings an application main window to the top
		/// Throws a CannotFindApplication if the window was found.
		/// -- Might throw a HolodeckExceptions.CannotFindApplication
		/// </summary>
		/// <param name="title">the title of the main window of the application</param>
		public static void BringAppToFront (string title)
		{
			IntPtr appHandle = FindWindowA (null, title);
			if (appHandle != IntPtr.Zero) {
				SetForegroundWindow (appHandle);
			} else {
				throw new HolodeckExceptions.CannotFindApplication ("Couldn't find application with title: " + title);
			}
		}

		/// <summary>
		/// Brings Holodeck main window to the top
		/// Note: Use this method only if there's no Holodeck dialog windows open.
		/// </summary>
		public static void BringHolodeckToFront ()
		{
			SetForegroundWindow (holodeckMainWindowHandle);
		}

		public static void SendKeysToApp (String title, System.Collections.Specialized.StringCollection keyList) 
		{
			BringAppToFront (title);
			foreach (string keys in keyList) 
			{
				SendKeyStrokes (keys);
			}
			BringHolodeckToFront ();
		}

		/// <summary>
		/// Wait 'till Holodeck can process new user inputs
		/// -- Might throw a HolodeckExceptions.HolodeckCrashedException
		/// </summary>
		public static void WaitForHolodeck () 		
        {                
//            System.Threading.Thread.Sleep( System.TimeSpan.FromSeconds( 2 ) );
                  
			// check if Holodeck is still running
			if (holodeckProcess.HasExited)
			{
				throw new HolodeckExceptions.HolodeckExitedException ();
			}

			// wait till Holodeck can respond to user input
			holodeckProcess.WaitForInputIdle ();

			// check if Holodeck has crashed
			foreach (CrashWindow crashWindow in crashWindows) 
			{
				SearchForCrashWindow (crashWindow);
			}
		}
	} // class HolodeckProcess


	/// <summary>
	/// Holodeck's main window
	/// </summary>
	public class HolodeckPane: Base
	{
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			MainPane = 0,
			ResourcesPane = 1,
			PropertiesPane = 2,
			ProjectPane = 3,
			LimitsPane = 4,
			DynamicHelpPane = 5,
		}

		private static int paneNumber = 6;

		public enum RemovablePane 
		{
			ResourcesPane = 1,
			PropertiesPane = 2,
			ProjectPane = 3,
			LimitsPane = 4,
			DynamicHelpPane = 5,
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.MainPane;
		private static System.Collections.ArrayList paneOrdering = new System.Collections.ArrayList (paneNumber);

		/// <summary>
		/// Returns back the index of the next visible pane after the specified one in paneOrdering
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		/// <param name="pane">the specified pane</param>
		/// <returns>the index of a paneOrdering element</returns>
		private static int GetNextPaneIndex (TabStop pane)
		{
			int position = 0;
			while ((position < paneOrdering.Count) &&
				(((TabStop) paneOrdering[position]) != pane)) 
			{
				position++;
			}
			if (position == paneOrdering.Count) 
			{
				// TODO: provide meaningful info in the exception
				throw new HolodeckExceptions.UnaccessableControlException ();
			} 
					
			position++;
			if (position == paneOrdering.Count) 
			{
				position = 0;
			} 
			return position;
		}

		/// <summary>
		/// Go to the next visible pane at the Holodeck main window
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		private static void GoToNext () 
		{
			switch (currPos) 
			{
				case TabStop.MainPane:
				{
					currPos = (TabStop) paneOrdering[GetNextPaneIndex (currPos)];
					SendKeyStrokes ("+{TAB}"); // Shift-Tab
					break;
				}
				default:
				{
					currPos = (TabStop) paneOrdering[GetNextPaneIndex (currPos)];
					SendKeyStrokes ("+{TAB}"); // Shift-Tab
					SendKeyStrokes ("+{TAB}"); // Shift-Tab
					break;
				}
			}
		}

		/// <summary>
		/// Change the focus to the specified control
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		/// <param name="toPos">the desired control</param>
		private static void GoTo (TabStop toPos) 
		{
			TabStop origPos = currPos;
			while (currPos != toPos) 
			{
				GoToNext ();
				if (currPos == origPos) 
				{
					// TODO: provide meaningful information in the exception
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}

		/// <summary>
		/// Sets Holodeck's main window to its default state
		/// </summary>
		public static void Reset ()
		{
			// Do not add WaitForHolodeck, since this method might be called before the Holodeck process started

			for (int i = 0; i < paneNumber; i++) 
			{
				paneOrdering.Add ((TabStop) i); // Default setting. Should be changed by HidePane() and ReorderPane().
			}

			// TODO: reset to the default settings
		}

		/// <summary>
		/// Remove a pane from the visible pane ordering
		/// </summary>
		/// <param name="pane">a TabStop representing the pane to be removed</param>
		public static void RemovePaneFromOrdering (RemovablePane pane) 
		{
			// Do not add WaitForHolodeck, since this method might be called before the Holodeck process started

			int position = 0;
			for (position = 0; position < paneOrdering.Count; position++) 
			{
				if ((RemovablePane) paneOrdering[position] == pane) 
				{
					break;
				}
			}
			if (position < paneOrdering.Count) 
			{
				paneOrdering.RemoveAt (position);
			}
		}

		/// <summary>
		/// Put a pane into the visible pane ordering at the specified index
		/// -- Might throw a System.ArgumentOutOfRangeException
		/// </summary>
		/// <param name="pane">a TabStop representing the specified pane</param>
		/// <param name="index">the index to insert the pane at</param>
		public static void ReorderPane (RemovablePane pane, int index) 
		{
			// Do not add WaitForHolodeck, since this method might be called before the Holodeck process started

			RemovePaneFromOrdering (pane);
			paneOrdering.Insert (index, pane);
		}

		/// <summary>
		/// Remove a pane from the visible pane ordering
		/// </summary>
		/// <param name="pane">a TabStop representing the pane to be hidden</param>
		public static void HidePane (RemovablePane pane) 
		{
			// Do not add WaitForHolodeck, since this method might be called before the Holodeck process started

			RemovePaneFromOrdering (pane);
		}

		/// <summary>
		/// Obsolete. Kept for compatibility reasons.
		/// Calls File_NewProject (...)
		/// </summary>
		[Obsolete ("This method is now obsolete. Use File_NewProject() instead.")]
		public static void NewProject ()
		{
			File_NewProject ();
		}

		/// <summary>
		/// Brings up "Create A New Project" wizard
		/// </summary>
		// changed from "NewProject" to reflect its position in the menu hierarchy
		// "NewProject (...)" is now obsolete
		public static void File_NewProject ()
		{
			SendKeyStrokes ("%f"); //ALT-f (File menu)
			SendKeyStrokes ("n"); //n (New project)
		}

		/// <summary>
		/// Exits Holodeck via File menu
		/// </summary>
		public static void File_Exit () {
			SendKeyStrokes ("%f"); //ALT-f (File menu)
			SendKeyStrokes ("x"); //x (Exit)
		}

		/// <summary>
		/// Exits Holodeck via File menu and closes the "Save project" dialog
		/// </summary>
		/// <param name="closeDialogAnswer">'true' if Holodeck has to save the project, 'false' otherwise</param>
		public static void File_Exit (bool closeDialogAnswer) {
			File_Exit ();
			if (closeDialogAnswer) {
				SendKeyStrokes (" "); //Space (Yes)
			} else {
				SendKeyStrokes ("{RIGHT}");	//Right arrow
				SendKeyStrokes (" ");		//Space (No)
			}
		}

		/// <summary>
		/// Pauses the currently running AUT
		/// </summary>
		public static void Application_Pause ()
		{
			SendKeyStrokes ("%a"); //ALT-a (Application menu)
			SendKeyStrokes ("p"); //p (Pause)
		}

		/// <summary>
		/// Resumes the currently paused AUT
		/// </summary>
		public static void Application_Resume ()
		{
			SendKeyStrokes ("%a"); //ALT-a (Application menu)
			SendKeyStrokes ("r"); //r (Resume)

			SetForegroundWindow (holodeckMainWindowHandle);
		}

		/// <summary>
		/// Brings up the "Scheduled Test" wizard
		/// Note: Assumes that there is a project opened
		/// </summary>
        public static void Application_ScheduledTest ()
        {
            SendKeyStrokes ("%a"); //ALT-a (Application menu)
            SendKeyStrokes ("s"); //s (ScheduledTest)            
        }

		/// <summary>
		/// Brings the control to the "Resource" pane
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		public static void SelectResourcesPane () 
		{
			GoTo (TabStop.ResourcesPane);
		}

		/// <summary>
		/// Brings the control to the "Properties" pane
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		public static void SelectPropertiesPane () 
		{
			GoTo (TabStop.PropertiesPane);
		}

		/// <summary>
		/// Brings the control to the "Log" pane
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		public static void SelectLogPane () 
		{
			// TODO: make sure that in the main page, the current pane really _is_ a log pane
			GoTo (TabStop.MainPane);
		}

		public static void SelectLimitsPane () {
			// TODO: make sure that in the main page, the current pane really _is_ a log pane
			GoTo (TabStop.LimitsPane);
		}
	} // class HolodeckPane
} // namespace Holodeck