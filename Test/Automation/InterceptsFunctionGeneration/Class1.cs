using System;
using System.IO;
using System.Collections;
using System.Xml;
using Microsoft.Win32;

namespace InterceptsFunctionGeneration
{
	/// <summary>
	/// Generates main function call code...
	/// </summary>
	class FunctionGenerator
	{
        // This defines the name and location of the functions.xml file...
        public static String functionXMLFile = null;

        /// <summary>
        /// Generates the function list...
        /// </summary>
        /// <returns></returns>
        public static String GenerateFunctionList( bool bIsHeader )
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
                        if ( !bIsHeader )
                            functionListResults += "    bSuccess *= printFunctionResult( My_" + xr.Value + "( ), \"" + xr.Value + "\" );\r\n";
                        else
                            functionListResults += "    BOOL My_" + xr.Value + "( )" + ";\r\n";

                    }                    
                }
            }

            Exception e;            
            xr.Close( );

            return functionListResults;
        }

        public static void SetupHolodeckLocation( )
        {
            RegistryKey localMachineHive = Registry.LocalMachine;

            // Open the subkey...
            RegistryKey pathLocationKey = localMachineHive.OpenSubKey( "SOFTWARE\\HolodeckEE" );

            functionXMLFile = (string)pathLocationKey.GetValue( "InstallPath" ) + "\\function_db\\functions.xml";
        }

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		public static void Main(string[] args)
		{
            // Get the location of Holodeck...
            SetupHolodeckLocation( );

            StreamReader sr = new StreamReader( @"F:\CVS Root\CVSROOT\Holodeck\Test\Automation\InterceptsFunctionGeneration\intercepts.cpp.template" );            
            StreamWriter sw = new StreamWriter( @"F:\CVS Root\CVSROOT\Holodeck\Test\Automation\InterceptsFunctionGeneration\intercepts.cpp", false );

            String fileLine = null;

            while ( ( fileLine = sr.ReadLine( ) ) != null )
            {
                if ( fileLine.IndexOf( "[FUNCTIONGENERATION]" ) != -1 )                
                    sw.WriteLine( fileLine.Replace( "[FUNCTIONGENERATION]", GenerateFunctionList( false ) ) );                                    
                else                
                    sw.WriteLine( fileLine );                
            }                    
        
            sw.Close( );
            sr.Close( );           
 			
            // Get the location of Holodeck...
            SetupHolodeckLocation( );

            sr = new StreamReader( @"F:\CVS Root\CVSROOT\Holodeck\Test\Automation\InterceptsFunctionGeneration\stdafx.h.template" );            
            sw = new StreamWriter( @"F:\CVS Root\CVSROOT\Holodeck\Test\Automation\InterceptsFunctionGeneration\stdafx.h", false );

            fileLine = null;

            while ( ( fileLine = sr.ReadLine( ) ) != null )
            {
                if ( fileLine.IndexOf( "[FUNCTIONGENERATION]" ) != -1 )                
                    sw.WriteLine( fileLine.Replace( "[FUNCTIONGENERATION]", GenerateFunctionList( true ) ) );                                    
                else                
                    sw.WriteLine( fileLine );                
            }                    
        
            sw.Close( );
            sr.Close( );            			
		}
	}
}
