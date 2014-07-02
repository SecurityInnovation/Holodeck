using System;
using System.Diagnostics;
using System.IO;
using System.Collections;
using System.Security.Permissions;
using Microsoft.Win32;
using Holodeck;

namespace Win32CodeGenerator
{

	class EntryPoint
	{
		
		static void Main(string[] args)
		{
			Process currentProcess = new Process();
			currentProcess = Process.GetCurrentProcess();

			CodeGenerator codeGen = new CodeGenerator();
			codeGen.fileWriter();
			
			
			// generating intercept.cpp and stdafx.h
			InterceptCodeGenerator interceptCodeGen = new InterceptCodeGenerator();
			interceptCodeGen.GenerateInterceptsFile();

			/// include code here to build project i.e compiling *.cpp file created
			RegistryKey localMachineHive = Registry.LocalMachine;
			// Open the subkey...
			RegistryKey pathLocationKey = localMachineHive.OpenSubKey( "SOFTWARE\\HolodeckEE" );

			string functionXMLFile = (string)pathLocationKey.GetValue( "InstallPath" ) + "\\function_db\\functions.xml";
			string holodeckFunctionDBPath = (string)pathLocationKey.GetValue( "InstallPath" ) + "\\function_db";
			
			string Win32CodeGeneratorDumpDirPath = string.Concat(holodeckFunctionDBPath,"\\Win32CodeGeneratorDump");



			if (!Directory.Exists(Win32CodeGeneratorDumpDirPath)) 
			{
				DirectoryInfo di = Directory.CreateDirectory(Win32CodeGeneratorDumpDirPath);
			}
			
			string common_h_FilePath = string.Concat(holodeckFunctionDBPath,"\\common.h");
			string buildWin32InterceptFunction_cmd_FilePath = string.Concat(holodeckFunctionDBPath,"\\buildWin32InterceptFunction.cmd");

			FileInfo FileToCopy = new FileInfo(common_h_FilePath);
			FileToCopy.CopyTo(string.Concat(Win32CodeGeneratorDumpDirPath,"\\common.h"),true);

			FileToCopy = new FileInfo(buildWin32InterceptFunction_cmd_FilePath);
			FileToCopy.CopyTo(string.Concat(Win32CodeGeneratorDumpDirPath,"\\buildWin32InterceptFunction.cmd"),true);
			
			
			currentProcess.Close();
			currentProcess.Refresh();
			
			Process compileProcess = new Process();
			compileProcess.StartInfo.FileName = string.Concat(Win32CodeGeneratorDumpDirPath,"\\buildWin32InterceptFunction.cmd");
			compileProcess.StartInfo.CreateNoWindow = true;
			compileProcess.Start();



			//launching AUT
			Holodeck.HolodeckProcess.Start();
			Holodeck.HolodeckPane.Reset();
			Holodeck.HolodeckPane.File_NewProject();
			Holodeck.NewProjectPane1.Reset();
			Holodeck.NewProjectPane1.SetProjectLocation(@"c:\Win32InterceptGen");
			Holodeck.NewProjectPane1.Next();
			Holodeck.NewProjectPane2.Reset();
			Holodeck.NewProjectPane2.SetApplicationName(string.Concat(Win32CodeGeneratorDumpDirPath,"\\FunctionIntercepts.exe"));
			Holodeck.NewProjectPane2.Next();
			Holodeck.NewProjectPane3.Reset();
			Holodeck.NewProjectWizard.NativeFunctions("All");
			Holodeck.NewProjectWizard.NetFunctions("All");
			Holodeck.NewProjectPane3.Finish();
			

		}
	}
}
