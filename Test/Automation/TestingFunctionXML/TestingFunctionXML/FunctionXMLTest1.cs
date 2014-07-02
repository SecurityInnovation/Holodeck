using System;
using System.IO;
using System.Security.Permissions;
using Microsoft.Win32;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;
using TestingFunctionXML;

namespace FunctionXMLTest
{
	/// <summary>
	/// Summary description for FunctionXml1.
	/// File Not Found as we are renaming the file to testingFunctions.xml
	/// </summary>
	public class FunctionXMLTest1:FunctionXMLTest
	{
		string FunctionsXMLFilePath;
		string FunctionsXMLBackupFilePath;
		FileInfo modifyThisFile;

		public FunctionXMLTest1()
		{
		}

		public override void executeTest( )
		{
			Console.WriteLine("Test Case : Functions.xml is renamed to testingFunctions.xml");
			try
			{
				string holodeckPath;
				holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
				
				FunctionsXMLFilePath = string.Concat(holodeckPath,"\\function_db\\functions.xml");
				
				FunctionsXMLBackupFilePath = string.Concat(FunctionsXMLFilePath,".bak");

				modifyThisFile = new FileInfo(FunctionsXMLFilePath);
				modifyThisFile.Attributes = FileAttributes.Normal;
				
				modifyThisFile.CopyTo(FunctionsXMLBackupFilePath,true);
				modifyThisFile.MoveTo(string.Concat(holodeckPath,"\\function_db\\testingfunctions.xml"));

				try	
				{	//add code here which will launch Holodeck
					Holodeck.HolodeckProcess.Start();				
				}
				catch(Holodeck.HolodeckExceptions.CannotStartHolodeckException ex)
				{
					Console.WriteLine("Cannot Start Holodeck Exception thrown ");
					Console.WriteLine(ex.Message);
				}
					
			}
			catch(HolodeckExceptions.IncorrectRegistryException e)
			{
				Console.WriteLine(" Incorrect Registry Exception caught.... : " + e.Message);
				Console.WriteLine("Details: " + e.StackTrace);
			}
			catch(FileNotFoundException f)
			{
				Console.WriteLine(" File Not Found Exception caught.... : " + f.Message);
				Console.WriteLine("Details: " + f.StackTrace);
			}
			finally
			{
				if(Holodeck.HolodeckProcess.IsRunning ())
				{
					Holodeck.HolodeckProcess.Stop();
				}
				
				//reverting back to original
				modifyThisFile.Delete();

				FileInfo regainOriginal = new FileInfo(FunctionsXMLBackupFilePath);
				regainOriginal.MoveTo(FunctionsXMLFilePath);

			}
			
		}
	}
}
