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
	/// Summary description for Class1.
	/// </summary>
	class LogReaderUITest
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		
		static void GetHolodeck()
		{
		
			//launch HD
			//t.LaunchFaultInterceptorAsAUT();
			string holodeckPath;
			holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue("InstallPath");
			string LogReaderPath = (System.Environment.CurrentDirectory); 
			int index = LogReaderPath.IndexOf("UILogExportTest");
			if (index != -1)
			{
				LogReaderPath = LogReaderPath.Substring(0, index-1);
			}
			
			LogReaderPath=String.Concat(LogReaderPath,"\\LogExportTest\\bin\\Debug\\LogExportTest.exe");
			//LogReaderPath = "F:\\CVS Root\\CVSROOT\\Holodeck\\Test\\Automation\\LogExportTest\\bin\\Debug\\LogExportTest.exe";								
			Console.WriteLine(LogReaderPath);
			//launching AUT
			Holodeck.HolodeckProcess.Start();
			Holodeck.HolodeckPane.Reset();
			Holodeck.HolodeckPane.File_NewProject(); 
			Holodeck.NewProjectPane1.Reset();
			Holodeck.NewProjectPane1.SetProjectLocation("C:\\LogExportTest");
			Holodeck.NewProjectPane1.Next();
			Holodeck.NewProjectPane2.Reset();
			Holodeck.NewProjectPane2.SetApplicationName(LogReaderPath);
			
			//NOT WORKING ON BUILDS >58(DOING MANUAL - USING SENDKEYS)
			//Holodeck.NewProjectPane2.SetApplicationParams(faultname); 
			
			//-->USE FOR HOLODECK BUILD(S) 77
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}"); 
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}");
			//Holodeck.NewProjectPane2.SetEditBoxText(faultname); 
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			//Select native and .net functions and goto main holodeck window
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			Holodeck.NewProjectPane3.Finish();
			//Wait for 10 seconds while holodeck does its stuff
			Thread.Sleep(10000);
		}

		[STAThread]
		static void Main(string[] args)
		{

			TestCases myTestCase = new TestCases();

			GetHolodeck();
			

			//geth the id of the process in holodeck
			Process[] myProcesses;
			myProcesses = Process.GetProcesses();
			String processid="";
			foreach (Process myProcess in myProcesses)
			{
				processid = Convert.ToString(myProcess.Id);
				if(String.Compare(myProcess.ProcessName,"LogExportTest") == 0) break;
			}

			//encoporate filters here
			for (int i=0;i < args.Length;i++)
			{
				if (args.GetValue(i).ToString() == "Memory")
				{
				Console.WriteLine("Now performing Memory filter operations...");
				myTestCase.MemoryFilter(); 
				}
				
				if (args.GetValue(i).ToString() == "File")
				{
					Console.WriteLine("Now performing File filter operations...");
					myTestCase.FileFilter(); 
				}

				if (args.GetValue(i).ToString() == "Net")
				{
					//myTestCase.MemoryFilter(); 
				}

				if (args.GetValue(i).ToString() == "Native")
				{
					//myTestCase.MemoryFilter(); 
				}

				if (args.GetValue(i).ToString() == "NetNative")
				{
					//myTestCase.MemoryFilter(); 
				}
			
			}
			
			Thread.Sleep(50);
			Holodeck.HolodeckPane.SendKeyStrokes("%F");
			Holodeck.HolodeckPane.SendKeyStrokes("E");
			Holodeck.Base.SetEditBoxText("C:\\UILogTest");  
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			Holodeck.HolodeckPane.SendKeyStrokes("%F");
			Holodeck.HolodeckPane.SendKeyStrokes("S");
            
			//need to enumerate processes to get the processid of aut log
							
			HoloScript.LogReaderV2_5 logReader = new HoloScript.LogReaderV2_5( );
			String LogExportPath = "C:\\LogExportTest_Logs\\";
			String LogName = String.Concat(processid,".hdl");
            String FullLogName = String.Concat(LogExportPath,LogName);
			Console.WriteLine(FullLogName);
            logReader.OpenLogFile( FullLogName, "C:\\TestExportHolodeckLog.txt" );	
			
			Thread.Sleep(20);
			System.Diagnostics.Process proc = new System.Diagnostics.Process(); 
			proc.EnableRaisingEvents=false; 
			proc.StartInfo.FileName="fc /L c:\\UILogTest.txt c:\\TestExportHolodeckLog.txt >c:\\TestOutput.txt"; 
			proc.Start(); 

			Console.ReadLine();
		
		}			
	}
}
