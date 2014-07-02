///******************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		EntryPoint.cs
///
/// DESCRIPTION: Creates template files for the functions in DotNetFunctions.XML
///
///==============================================================================
/// Modification History
///
/// Date				Name			         Purpose
/// -----------    -----------     ------------------------------------------
/// 29 Oct 2003	    Ankur Jauhari	            File created.
/// 31 Oct 2003		Vaibhav Parikh				File Modified
/// 24 Nov 2003     Jessica Winter				File modified
///******************************************************************************

using System;
using System.Diagnostics;
using System.IO;
using System.Collections;
using System.Security.Permissions;
using Microsoft.Win32;
using Holodeck;
//using DotNetInterceptTester;

namespace DotNetCodeGenerator
{
	/// <summary>
	/// Summary description for EntryPoint.
	/// </summary>
	class EntryPoint
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			Process currentProcess = new Process();
			currentProcess = Process.GetCurrentProcess();


			//getting File path from Holodeck Setup Folder
			string holodeckFunctionDBPath;
			holodeckFunctionDBPath = (string) Registry.LocalMachine.OpenSubKey("Software\\HolodeckEE", true).GetValue ("InstallPath");
			string DotNetFunctionsXMLFilePath = string.Concat(holodeckFunctionDBPath,"\\function_db\\DotNetFunctions.xml");
			string DotNetCodeGeneratorDumpDirPath = string.Concat(holodeckFunctionDBPath,"\\DotNetCodeGeneratorDump");

		
			if (!Directory.Exists(DotNetCodeGeneratorDumpDirPath)) 
			{
				DirectoryInfo di = Directory.CreateDirectory(DotNetCodeGeneratorDumpDirPath);
			}

			/// code to generate .cs files for each functions found in dontnetfunctions.xml
			CodeGenerator DotNetCodeGenerator = new CodeGenerator(DotNetFunctionsXMLFilePath);
			DotNetCodeGenerator.WriteFile();


			/// generate main.cs file
//			MainGenerator buildMain = new MainGenerator();
//			buildMain.generateMain();


			/// code to generate the main.cs file which will help in compiling the generated .cs files
			/// 
			//why common.h here??
			//string common_h_FilePath = string.Concat(holodeckFunctionDBPath,"\\common.h");
			//string buildDotNetInterceptFunction_cmd_FilePath = string.Concat(holodeckFunctionDBPath,"\\buildDotNetInterceptFunction.cmd");
			
			//FileInfo FileToCopy = new FileInfo(common_h_FilePath);
			//FileToCopy.CopyTo(string.Concat(DotNetCodeGeneratorDumpDirPath,"\\common.h"),true);
			
			//FileToCopy = new FileInfo(buildDotNetInterceptFunction_cmd_FilePath);
			//FileToCopy.CopyTo(string.Concat(DotNetCodeGeneratorDumpDirPath,"\\buildDotNetInterceptFunction.cmd"),true);
						
	    	
			/// building dotNetIntercept.exe using build script which uses all .cs files in DotNetInterceptDump folder
			 		
		    currentProcess.Close();
			currentProcess.Refresh();
			
			
//			Process compileProcess = new Process();
//			compileProcess.StartInfo.FileName = string.Concat(DotNetCodeGeneratorDumpDirPath,"\\buildDotNetInterceptFunction.cmd");
//			compileProcess.StartInfo.CreateNoWindow = true;
//		    compileProcess.Start();
//
//			
//         	//launching AUT
//			Holodeck.HolodeckProcess.Start();
//			Holodeck.HolodeckPane.Reset();
//			Holodeck.HolodeckPane.File_NewProject();
//			Holodeck.NewProjectPane1.Reset();
//			Holodeck.NewProjectPane1.SetProjectLocation(@"c:\DotNetTester");
//			Holodeck.NewProjectPane1.Next();
//			Holodeck.NewProjectPane2.Reset();
//			Holodeck.NewProjectPane2.SetApplicationName(string.Concat(DotNetCodeGeneratorDumpDirPath,"\\DotNetInterceptFunction.exe"));
//			Holodeck.NewProjectPane2.Next();
//			Holodeck.NewProjectPane3.Reset();
//			Holodeck.NewProjectWizard.NativeFunctions("All");
//			Holodeck.NewProjectWizard.NetFunctions("All");
//			Holodeck.NewProjectPane3.Finish();
		}
	}
}
