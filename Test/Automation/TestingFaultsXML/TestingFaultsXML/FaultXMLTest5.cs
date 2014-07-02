using System;
using System.IO;
using System.Security.Permissions;
using Microsoft.Win32;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;
using FaultsXMLFramework;

namespace FaultXMLTest
{
	/// <summary>
	/// Summary description for FaultXMLTest5.
	/// Remove Faults.DTD line from xml file so no validation
	/// </summary>
	public class FaultXMLTest5:FaultXMLTest
	{
		string FaultsXMLFilePath;
		string FaultsXMLBackupFilePath;
		FileInfo modifyThisFile;

		public FaultXMLTest5()
		{
			
		}
		
		public override void executeTest( )
		{
			Console.WriteLine("Test Case :  Remove Faults.DTD line from xml file so no validation ##### NotWorking need some thought");
			try
			{
				string holodeckPath;
				holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
				
				FaultsXMLFilePath = string.Concat(holodeckPath,"\\function_db\\faults.xml");
				
				FaultsXMLBackupFilePath = string.Concat(FaultsXMLFilePath,".bak");

				modifyThisFile = new FileInfo(FaultsXMLFilePath);
				modifyThisFile.Attributes = FileAttributes.Normal;
				modifyThisFile.CopyTo(FaultsXMLBackupFilePath,true);
							

				//string FaultsXMLFilePathNEW = @"C:\Program Files\Security Innovation\Holodeck Enterprise Edition\function_db\faultstesting.xml";

				//modify xml here
				FaultXMLNavigator FaultXMLNav = new FaultXMLNavigator();
				FaultsXMLFilePath = modifyThisFile.FullName;
				FaultXMLNav.ValidateXmlDocument(FaultsXMLFilePath);
				FaultXMLNav.parseXmlDocument(FaultsXMLFilePath);
				

				//saving the functions.xml
				//FaultXMLNav.saveFaultXmlDocument(FaultXMLNav,@"C:\Program Files\Security Innovation\Holodeck Enterprise Edition\function_db\faultsnew.xml","UTF-8",false);
				FaultXMLNav.saveFaultXmlDocument(FaultXMLNav,FaultsXMLFilePath,"UTF-8",false);
//				
//				
//				modifyThisFile.Delete();
//
//				FileInfo test = new FileInfo(@"C:\Program Files\Security Innovation\Holodeck Enterprise Edition\function_db\faultsnew.xml");
//				test.MoveTo(string.Concat(holodeckPath,"\\function_db\\faults.xml"));

				//add code here which will launch Holodeck
				Holodeck.HolodeckProcess.Start();				

				
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

				FileInfo regainOriginal = new FileInfo(FaultsXMLBackupFilePath);
				regainOriginal.MoveTo(FaultsXMLFilePath);

			}

		
		}
	}
}
