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
	/// Summary description for FaultXMLTest34.
	/// Make Faults.xml with 'Faults Fault Function CheckResource Exists' Value Edited to "originalValue + testValue"
	/// </summary>
	public class FaultXMLTest34:FaultXMLTest
	{
		string FaultsXMLFilePath;
		string FaultsXMLBackupFilePath;
		FileInfo modifyThisFile;

		public FaultXMLTest34()
		{
			
		}
		
		public override void executeTest( )
		{
			Console.WriteLine("Test Case :Make Faults.xml with 'Faults Fault Function CheckResource Exists' Value Edited to \"originalValue + testValue\"");
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

				XmlNode nodeCheckResource = xmlDocument.SelectSingleNode( "/Faults/Fault/Function/CheckResource[@Exists]" );
				
				
				EditValues editExists = new EditValues();
				editExists.EditCheckResource(nodeCheckResource,testValue,"Exists");
							
				
				xmlDocument.Save(FaultsXMLFilePath);
				
				try	
				{	//add code here which will launch Holodeck
					Holodeck.HolodeckProcess.Start();				
				}
				catch(Holodeck.HolodeckExceptions.CannotStartHolodeckException ex)
				{
					Console.WriteLine("Cannot Start Holodeck Exception thrown ");
					Console.WriteLine("Test Value not Found in enumeration List");
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

				FileInfo regainOriginal = new FileInfo(FaultsXMLBackupFilePath);
				regainOriginal.MoveTo(FaultsXMLFilePath);

			}

		
		}
	}
}
