using System;
using System.Threading;
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
		/// Returns Holodecks main process ID
		/// </summary>
		public static int GetProcessID () 
		{
			return holodeckProcess.Id;
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
		public class HolodeckAlreadyRunningException : System.Exception
		{
			// Empty
		}

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
			Start (10);
		}

		/// <summary>
		/// Start Holodeck
		/// </summary>
		public static void Start (int sleepSeconds) 
		{
			// TODO: move the mouse pointer to one of the corners of the desktop to circumvent any interaction with Holodeck

			IntPtr oldHolodeckHandle = FindWindowA (null, "Holodeck Enterprise Edition");
			if (oldHolodeckHandle != IntPtr.Zero) 
			{
				throw new HolodeckExceptions.HolodeckAlreadyRunningException ();
			}

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

			WaitForHolodeck();

			// Would be better to use holodeckProcess.MainWindowHandle, but it's null for some case...
			holodeckMainWindowHandle = IntPtr.Zero;
			int tryNum = 0;
			while ((holodeckMainWindowHandle == IntPtr.Zero) && (tryNum < sleepSeconds)) 
			{
				System.Threading.Thread.Sleep (1000);	// sleep for one second
				holodeckMainWindowHandle = FindWindowA (null, "Holodeck Enterprise Edition");
				tryNum++;
			}

			if (holodeckMainWindowHandle == IntPtr.Zero)
			{
				throw new HolodeckExceptions.CannotStartHolodeckException ();
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
			holodeckProcess.Kill ();
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
				(FindWindowA (null, "HolodeckGUI.exe") != IntPtr.Zero)||(FindWindowA (null, "Holodeck") != IntPtr.Zero)) 
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
		public static void Reset ()
		{
			HolodeckProcess.WaitForHolodeck ();

			/* TODO: return to the default state */
		}

		/// <summary>
		/// Obsolete. Kept for compatibility reasons.
		/// Calls File_NewProject (...)
		/// </summary>
		public static void NewProject ()
		{
			File_NewProject ();
		}

		/// <summary>
		/// Bring up "Create A New Project" wizard
		/// </summary>
		// changed from "NewProject" to reflect its position in the menu hierarchy
		// "NewProject (...)" is now obsolete
		public static void File_NewProject ()
		{
			SendKeyStrokes ("%f"); //ALT-f (File menu)
			SendKeyStrokes ("n"); //n (New project)
		}

		public static void Application_Pause ()
		{
			SendKeyStrokes ("%a"); //ALT-a (Application menu)
			SendKeyStrokes ("p"); //p (Pause)
		}

		public static void Application_Resume ()
		{
			SendKeyStrokes ("%a"); //ALT-a (Application menu)
			SendKeyStrokes ("r"); //r (Resume)

			SetForegroundWindow (holodeckMainWindowHandle);
		}
	} // class HolodeckPane
} // namespace Holodeck