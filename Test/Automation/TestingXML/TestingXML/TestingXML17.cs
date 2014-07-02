using System;
using System.IO;
using System.Security.Permissions;
using Microsoft.Win32;
using System.Xml;
using System.Xml.XPath;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;

namespace TestingXML
{
	/// <summary>
	/// Summary description for TestingXML17.
	/// Extra Attribute added to the Element having testValue as node value
	/// </summary>
	public class TestingXML17:TestingXML
	{
		public TestingXML17()
		{
			
		}
		
		
		string XMLFilePath;
		string XMLBackupFilePath;

		FileInfo modifyThisFile;

		public override void executeTest( )
		{
			Console.WriteLine("Test Case 17 : Extra Attribute added to the Element having testValue as node value ");
			try
			{
				string holodeckPath;
				holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
				
				XMLFilePath = string.Concat(holodeckPath,"\\function_db\\",XmlFileName);
				
				XMLBackupFilePath = string.Concat(XMLFilePath,".bak");

				modifyThisFile = new FileInfo(XMLFilePath);
				modifyThisFile.Attributes = FileAttributes.Normal;
				modifyThisFile.CopyTo(XMLBackupFilePath,true);
				
				//modify xml file here
				XMLFilePath = modifyThisFile.FullName;
				
				XmlDocument xmlDocument = new XmlDocument( );
				xmlDocument.Load(XMLFilePath);
				
				
				XmlNode node = xmlDocument.SelectSingleNode( "child::node()" ); // Xpath query Here */*
				
				XmlNode nodeTest = node.FirstChild;

				bool hasChild = true;
			
				while(hasChild)
				{
					
					if((nodeTest.Attributes != null) && (nodeTest.Attributes.Count > 0 ))
					{
						XmlAttribute newAttribute = xmlDocument.CreateAttribute(testValue);
						newAttribute.Value = testValue;

						nodeTest.Attributes.Append(newAttribute);
						
						hasChild = false;

					}
					else
					{
						if(nodeTest.HasChildNodes)
						{
							nodeTest = nodeTest.FirstChild;
						}
						else
						{
							nodeTest = nodeTest.ParentNode.NextSibling;
						}
					}
				}
				
	
				xmlDocument.Save(XMLFilePath);

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
				
				modifyThisFile.Attributes = FileAttributes.Normal;
				//reverting back to original
				modifyThisFile.Delete();

				FileInfo regainOriginal = new FileInfo(XMLBackupFilePath);
				regainOriginal.MoveTo(XMLFilePath);

			}
			
		}

	}
}
