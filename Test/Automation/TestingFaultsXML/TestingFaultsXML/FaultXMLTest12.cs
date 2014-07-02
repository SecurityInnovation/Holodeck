using System;
using System.IO;
using System.Xml;
using System.Xml.XPath;
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
	/// Summary description for FaultXMLTest12.
	/// Make Faults.xml with Faults Fault ReturnValue Value Edited to "originalValue + testValue"
	/// </summary>
	public class FaultXMLTest12:FaultXMLTest
	{
		string FaultsXMLFilePath;
		string FaultsXMLBackupFilePath;
		FileInfo modifyThisFile;

		public FaultXMLTest12()
		{
			
		}
		
		public override void executeTest( )
		{
			Console.WriteLine("Test Case :Make Faults.xml with Faults Fault ReturnValue Value Edited to \"originalValue + testValue\"");
			try
			{
				string holodeckPath;
				holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
				
				FaultsXMLFilePath = string.Concat(holodeckPath,"\\function_db\\faults.xml");
				
				FaultsXMLBackupFilePath = string.Concat(FaultsXMLFilePath,".bak");

				modifyThisFile = new FileInfo(FaultsXMLFilePath);
				modifyThisFile.Attributes = FileAttributes.Normal;
				modifyThisFile.CopyTo(FaultsXMLBackupFilePath,true);

				//modify xml here
				FaultsXMLFilePath = modifyThisFile.FullName;
				XmlDocument xmlDocument = new XmlDocument( );
				xmlDocument.Load(FaultsXMLFilePath);

				XmlNode nodeFault = xmlDocument.SelectSingleNode( "/Faults/Fault[@ReturnValue]" );
				
				EditValues editFalutReturnValue = new EditValues();
				editFalutReturnValue.EditFault(nodeFault,testValue,"ReturnValue");
		
				Console.WriteLine("Editing Fault ReturnValue Attribute of {0} fault to {1}",nodeFault.Attributes["Name"].Value.ToString(),nodeFault.Attributes["ReturnValue"].Value.ToString());
				
				xmlDocument.Save(FaultsXMLFilePath);
				
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
