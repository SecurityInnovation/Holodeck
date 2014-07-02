using System;
using System.IO;
using System.Security.Permissions;
using Microsoft.Win32;
using System.Reflection;
using TestFramework;
using TestFramework.Exceptions;
using TestFramework.Run;
using TestFramework.Log;
using Holodeck;
using System.Threading;
using System.Diagnostics;

namespace LogReader___Automation
{
	/// <summary>
	/// Summary description for TestCases.
	/// </summary>
	public class TestCases
	{
		public void MemoryFilter()
		{
			Holodeck.HolodeckPane.SendKeyStrokes("%L");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{ENTER}");
			Holodeck.HolodeckPane.SendKeyStrokes("{TAB}");
			Holodeck.HolodeckPane.SendKeyStrokes("{TAB}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("{RIGHT}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");	
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");	
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");	
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("{TAB}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			
		}
		
		public void FileFilter()
		{
			Holodeck.HolodeckPane.SendKeyStrokes("%L");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{ENTER}");
			Holodeck.HolodeckPane.SendKeyStrokes("{TAB}");
			Holodeck.HolodeckPane.SendKeyStrokes("{TAB}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("{RIGHT}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");	
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");	
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");	
			Holodeck.HolodeckPane.SendKeyStrokes("{DOWN}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("{TAB}");
			Holodeck.HolodeckPane.SendKeyStrokes(" ");
			
		}
	}
	
}