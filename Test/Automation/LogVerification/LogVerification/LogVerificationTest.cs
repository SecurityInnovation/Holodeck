using System;
using System.Diagnostics;
using System.IO;
using System.Collections;
using System.Security.Permissions;
using Microsoft.Win32;
using TestingFunctionXML;

namespace LogVerification
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class LogVerificationTest
	{
		public string ExportLogPath;
		public string hdlPath;
		public ArrayList failedFunctionList = new ArrayList();


		/// <summary>
		/// 
		/// </summary>
		public LogVerificationTest()
		{
			this.ExportLogPath = "";
			this.hdlPath = "";
			this.failedFunctionList.Clear();
		}
	
		/// <summary>
		/// 
		/// </summary>
		public void searchResult(bool logExportComplete)
		{
			string holodeckPath;
			holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
		
			string holodeckFunctionDBPath = string.Concat(holodeckPath,"\\function_db");
			string FunctionsXMLPath = string.Concat(holodeckPath,"\\function_db\\functions.xml");
			
			//storing functions.xml in datastructure
			FunctionXMLNavigator myFunctionXMLNavigator = new FunctionXMLNavigator();
			//myFunctionXMLNavigator.ValidateXmlDocument(FunctionsXMLPath);
			myFunctionXMLNavigator.parseXmlDocument(FunctionsXMLPath);
			
			//string myHDLPath = hdlPath;
			string myExportLogPath = ExportLogPath;


			foreach(string functionName in myFunctionXMLNavigator.FunctionList)
			{
			
				if(logExportComplete)
				{
					System.Diagnostics.Process myProc = new System.Diagnostics.Process( );
					string systemDir = System.Environment.SystemDirectory;
					
					myProc.StartInfo.FileName = systemDir + "\\findstr.exe";
					myProc.StartInfo.Arguments =  functionName  + " " +myExportLogPath;                
					
					myProc.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
					myProc.Start( );

					// Wait for execution to finish...
					myProc.WaitForExit( );

					if(myProc.ExitCode == 1)
					{
						failedFunctionList.Add(functionName);
					}
					else
					{
						Console.WriteLine(functionName);
					}
					
				}
			}

			

		}
		
		public void printFailedFunction()
		{
			string holodeckPath;
			holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");

			//FileStream failedfunctions = File.AppendText(holodeckPath + "\\FailedFunctions.txt");
			StreamWriter failFunction = new StreamWriter(holodeckPath + "\\FailedFunctions.txt",false);
			failFunction.WriteLine("----------------------------------------------------------------------------------------------");
			failFunction.WriteLine("Log Verification Failed for following functions");
			failFunction.WriteLine("----------------------------------------------------------------------------------------------");
			failFunction.WriteLine();
			foreach(string functionName in failedFunctionList)
			{
				failFunction.WriteLine(functionName);
			
			}

			failFunction.Flush();

			Console.WriteLine("Result is available at {0}" ,holodeckPath + "\\FailedFunctions.txt");


		}


		/// <summary>
		/// 
		/// </summary>
		/// <param name="hdlPath"></param>
		/// <param name="fileExportPath"></param>
		/// <returns></returns>
		public bool LogExport(string hdlPath , string ExportLogPath)
		{
           			
			HoloScript.LogReaderV2_5 logReader = new HoloScript.LogReaderV2_5( );

			logReader.OpenLogFile(hdlPath , ExportLogPath);
			
			return (File.Exists(ExportLogPath));
		}



	}

	
}
