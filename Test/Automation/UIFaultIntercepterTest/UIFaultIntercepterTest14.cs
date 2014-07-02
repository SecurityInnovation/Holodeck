using System;
using System.Text;
using System.IO;
using System.Security.Permissions;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32;

//14-18 memory faults
namespace UIFaultIntercepterTest
{
	/// <summary>
	/// Summary description for UIFaultIntercepterTest1.
	/// </summary>
	public class UIFaultIntercepterTest14:UIFaultIntercepterTest
	{
		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowA (string className, string windowName);

		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowExA (IntPtr parentHWnd, IntPtr fromChildHWnd, string className, string windowName);

		[DllImportAttribute("user32.dll")]
		protected static extern int GetWindowTextLengthA (IntPtr hWnd);

		[DllImportAttribute("user32.dll")]
		protected static extern int GetWindowTextA (IntPtr hWnd, StringBuilder windowText, int maxLength);

		public UIFaultIntercepterTest14()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public override void executeTest( )
		{
			Console.WriteLine("Test Case 14 :INSUFFICIENT MEMORY Fault activated");
			UIFaultIntercepterTest1 t = new UIFaultIntercepterTest1();
			//launch HD
			t.GetHolodeck("Insufficient_Memory");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			//Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			
			RegistryKey LocalMachine = RegistryKey.OpenRemoteBaseKey(RegistryHive.LocalMachine, System.Environment.MachineName);            
			RegistryKey SoftwareKey = LocalMachine.OpenSubKey( "SOFTWARE", true );
			RegistryKey HolodeckKey = SoftwareKey.OpenSubKey("HolodeckEE", true);
			RegistryKey InterceptKey; 
			
			do
			{
				Thread.Sleep(500);
				InterceptKey = HolodeckKey.OpenSubKey("FaultIntercepterReady");
			}while(InterceptKey == null);
			Console.WriteLine("Now Setting Fault");
			FileStream ReadyFile = new FileStream("C:\\READY.TXT",FileMode.Create  , FileAccess.Write);
			StreamWriter readyWriter = new StreamWriter(ReadyFile);
			readyWriter.WriteLine("READY");
			readyWriter.Close();
			//set fault
			Holodeck.NewProjectPane3.SendKeyStrokes("{DOWN}");
			
			RegistryKey FaultIntercepter = HolodeckKey.CreateSubKey("FaultReady");
			Thread.Sleep(6000);
CheckOutcome();
			HolodeckKey.DeleteSubKey("FaultReady");
		
		}
	}
}
