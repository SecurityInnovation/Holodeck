using System;
using System.IO;
using System.Security.Permissions;
using Microsoft.Win32;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;

namespace TestingXML
{
	/// <summary>
	/// Summary description for TestingXML7.
	/// Change xml file attribute as System file
	/// </summary>
	public class TestingXML7:TestingXML
	{
		public TestingXML7()
		{
			
		}
		
		
		string XMLFilePath;
		string XMLBackupFilePath;

		FileInfo modifyThisFile;

		public override void executeTest( )
		{
			Console.WriteLine("Test Case 7 :Change xml file attribute as System file");
			try
			{
				string holodeckPath;
				holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
				
				XMLFilePath = string.Concat(holodeckPath,"\\function_db\\",XmlFileName);
				
				XMLBackupFilePath = string.Concat(XMLFilePath,".bak");

				modifyThisFile = new FileInfo(XMLFilePath);
				modifyThisFile.Attributes = FileAttributes.Normal;
				
				modifyThisFile.CopyTo(XMLBackupFilePath,true);
				modifyThisFile.MoveTo(string.Concat(holodeckPath,"\\function_db\\",XmlFileName));
				modifyThisFile.Attributes = FileAttributes.System;

				
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
				
				modifyThisFile.Attributes = FileAttributes.Normal;
				//reverting back to original
				modifyThisFile.Delete();

				FileInfo regainOriginal = new FileInfo(XMLBackupFilePath);
				regainOriginal.MoveTo(XMLFilePath);

			}
			
		}

	}
}
