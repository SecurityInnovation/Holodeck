using System;
using System.IO;
using System.Collections;
using System.Xml;
using Microsoft.Win32;

namespace NETInterceptsFunctionGenerator
{
	/// <summary>
	/// Generates main function call code...
	/// </summary>
	class NETFunctionGenerator
	{
		// This defines the name and location of the functions.xml file...
		static String functionXMLFile = null;

		/// <summary>
		/// Generates the function list...
		/// </summary>
		/// <returns></returns>
		static String GenerateFunctionList( )
		{
			// EXAMPLE of what we want to generate...
			// bSuccess *= printFunctionResult( My_HeapValidate( ), "HeapValidate" );

			String functionListResults = null;

			XmlTextReader xr = new XmlTextReader( functionXMLFile );

			while ( xr.Read( ) )
			{
				// Find out what type of node we are looking at. The XR reader will read one
				// XML element/value at a time (NOT one line at time)...
				if ( xr.NodeType == XmlNodeType.Element )
				{
					if ( xr.Name == "FunctionName" )
					{
						if ( !xr.Read( ) )
							break;

						Console.WriteLine( "Writing Function: " + xr.Value );
						
						functionListResults += "    bSuccess *= printFunctionResult( My_" + xr.Value + "( ), \"" + xr.Value + "\" );\r\n";
					}                    
				}
			}

			Exception e;            
			xr.Close( );

			return functionListResults;
		}
		static void SetupHolodeckLocation( )
		{
			RegistryKey localMachineHive = Registry.LocalMachine;

			// Open the subkey...
			RegistryKey pathLocationKey = localMachineHive.OpenSubKey( "SOFTWARE\\HolodeckEE" );

			functionXMLFile = (string)pathLocationKey.GetValue( "InstallPath" ) + "\\function_db\\dotNetFunctions.xml";
		}
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			// Get the location of Holodeck...
			SetupHolodeckLocation( );
            // ** To do: change directory
			StreamReader sr = new StreamReader( @"C:\cvsroot\CVSROOT\Holodeck\Test\Automation\NETInterceptsFunctionGeneration\NETTemplate.cs" );            
			StreamWriter sw = new StreamWriter( @"C:\cvsroot\CVSROOT\Holodeck\Test\Automation\NETInterceptsFunctionGeneration\NETintercepts.cs", false );

			String fileLine = null;

			while ( ( fileLine = sr.ReadLine( ) ) != null )
			{
				if ( fileLine.IndexOf( "[FUNCTIONGENERATION]" ) != -1 )                
					sw.WriteLine( fileLine.Replace( "[FUNCTIONGENERATION]", GenerateFunctionList( ) ) );                                    
				else                
					sw.WriteLine( fileLine );                
			}                    
        
			sw.Close( );
			sr.Close( );            			
		}
	}
}
