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

namespace UIFaultIntercepterTest
{
	/// <summary>
	/// Summary description for UIFaultIntercepterTest1.
	/// </summary>
	public class UIFaultIntercepterTest1:UIFaultIntercepterTest
	{
		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowA (string className, string windowName);

		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr FindWindowExA (IntPtr parentHWnd, IntPtr fromChildHWnd, string className, string windowName);

		[DllImportAttribute("user32.dll")]
		protected static extern int GetWindowTextLengthA (IntPtr hWnd);

		[DllImportAttribute("user32.dll")]
		protected static extern int GetWindowTextA (IntPtr hWnd, StringBuilder windowText, int maxLength);

		[DllImportAttribute("user32.dll")]
		protected static extern IntPtr GetNextWindow (IntPtr hWnd, int wCmd);

		public UIFaultIntercepterTest1()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public override void executeTest( )
		{
			Console.WriteLine("Test Case 1 :File Not Found Fault activated");
			UIFaultIntercepterTest1 t = new UIFaultIntercepterTest1();
			//launch HD
			t.GetHolodeck("File_Not_Found");

						Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
						Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
						Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
						Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");

			RegistryKey LocalMachine = RegistryKey.OpenRemoteBaseKey(RegistryHive.LocalMachine, System.Environment.MachineName);            
			RegistryKey SoftwareKey = LocalMachine.OpenSubKey("SOFTWARE", true );
			RegistryKey HolodeckKey =  SoftwareKey.OpenSubKey("HolodeckEE", true);
			RegistryKey InterceptKey; 

		//////////////////////////	
//			do
//			{
//				Thread.Sleep(500);
//				InterceptKey = HolodeckKey.OpenSubKey("FaultIntercepterReady");
//			}while(InterceptKey == null);

						//Thread.Sleep(30000);
//						Console.WriteLine("Now Setting Fault...");
//
//						Holodeck.NewProjectPane3.SendKeyStrokes("{DOWN}");
//						Console.WriteLine("Fault has been set.");
//						
//						RegistryKey FaultIntercepter = HolodeckKey.CreateSubKey("FaultReady");
//						Thread.Sleep(6000);
//////////////////////////////////////////
						
//						IntPtr xx = UIFaultIntercepterTest1.FindWindowExA(IntPtr.Zero,IntPtr.Zero,"WindowsForms10.Window.8.app5","Holodeck Enterprise Edition");
//						IntPtr xx2 = UIFaultIntercepterTest1.FindWindowExA(xx,IntPtr.Zero,"WindowsForms10.Window.8.app5","");
//						IntPtr xx3 = UIFaultIntercepterTest1.FindWindowExA(xx2,IntPtr.Zero,"WindowsForms10.Window.8.app5","");
//						IntPtr xx4 = UIFaultIntercepterTest1.GetNextWindow(xx3,0);
//						if (xx == IntPtr.Zero){Console.WriteLine("FAILED1");}
//						
//						if (xx2 == IntPtr.Zero){Console.WriteLine("FAILED2");}
//						
//						if (xx3 == IntPtr.Zero){Console.WriteLine("FAILED3");}
//						
//						IntPtr ff = UIFaultIntercepterTest1.FindWindowExA(xx,IntPtr.Zero,"WindowsForms10.BUTTON.app5","File Not Found");
//			
//						Console.WriteLine(xx2);
//						Console.WriteLine(xx3);
//						Console.WriteLine(xx4);
						HolodeckKey.DeleteSubKey("FaultReady");
	
		
		}
		}
}
