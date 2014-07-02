using System;

namespace Holodeck
{
	public struct LogEntry
	{
		public string TimeStamp;
		public long Thread;
		public string Category;
		public string Dll;
		public string Function;
		public string ReturnValue;
		public string ErrorCode;
		public string Exception;
		public string Parameter1;
		public string Parameter2;
		public string Parameter3;
		public string Parameter4;
		public string Parameter5;
		public string Parameter6;
		public string Parameter7;
		public string Parameter8;
		public string Parameter9;
		public string Parameter10;
		public string Parameter11;
		public string Parameter12;
	}

	/// <summary>
	/// Summary description for LogPane.
	/// </summary>
	public class LogPane : Base
	{
		/// <summary>
		/// Gets the entries of the specified log window
		/// Note: Assumes that the window is a ListView and it has at least 20 columns
		/// -- Might throw a Helper_dll.Helper.HelperException
		/// </summary>
		/// <param name="logWindowHandle">handle to a ListView</param>
		/// <returns>string array representing the entries in the ListView; first dimension is the row, the second is the column</returns>
		protected static string[][] GetLogEntries (IntPtr logWindowHandle)
		{
			// TODO: use tempfile if the log is too large
			return Base.GetListViewEntries (logWindowHandle, 20);
		}

		/// <summary>
		/// Gets the entries of the specified log window
		/// Note: Assumes that the window is a ListView and it has at least 20 columns
		/// -- Might throw a Helper_dll.Helper.HelperException
		/// -- Might throw a HolodeckExceptions.MismatchingWindowEnvironmentException
		/// -- Might throw a HolodeckExceptions.WindowNotFoundException
		/// </summary>
		/// <param name="logWindowHandle">handle to a ListView</param>
		/// <returns>string array representing the entries in the ListView; first dimension is the row, the second is the column</returns>
		public static string[][] GetLogEntries ()
		{
			// TODO: handle unordered log entries with same timestamp
			WindowEnvironment environment;

			environment.checkTitle = true;
			environment.title = "";
			environment.searchedHandle = false;
			environment.childs = new Holodeck.WindowEnvironment[4];

			environment.childs[0].checkTitle = true;
			environment.childs[0].title = "";
			environment.childs[0].searchedHandle = false;
			environment.childs[0].childs = null;

			environment.childs[1].checkTitle = true;
			environment.childs[1].title = "";
			environment.childs[1].searchedHandle = false;
			environment.childs[1].childs = null;

			environment.childs[2].checkTitle = true;
			environment.childs[2].title = "";
			environment.childs[2].searchedHandle = false;
			environment.childs[2].childs = null;

			environment.childs[3].checkTitle = true;
			environment.childs[3].title = "";
			environment.childs[3].searchedHandle = false;
			environment.childs[3].childs = new Holodeck.WindowEnvironment[2];

			environment.childs[3].childs[0].checkTitle = true;
			environment.childs[3].childs[0].title = "";
			environment.childs[3].childs[0].searchedHandle = true;
			environment.childs[3].childs[0].childs = new Holodeck.WindowEnvironment[1];

			environment.childs[3].childs[0].childs[0].checkTitle = false;
			environment.childs[3].childs[0].childs[0].title = null;
			environment.childs[3].childs[0].childs[0].searchedHandle = false;
			environment.childs[3].childs[0].childs[0].childs = null;

			environment.childs[3].childs[1].checkTitle = true;
			environment.childs[3].childs[1].title = "";
			environment.childs[3].childs[1].searchedHandle = false;
			environment.childs[3].childs[1].childs = null;

			IntPtr logWindow = Base.FindHolodeckPane (environment);
			if (logWindow == IntPtr.Zero) 
			{
				throw new HolodeckExceptions.WindowNotFoundException ("hWnd to \"Log\" pane was not found!");
			}

			return GetLogEntries (logWindow);
		}
	}
}
