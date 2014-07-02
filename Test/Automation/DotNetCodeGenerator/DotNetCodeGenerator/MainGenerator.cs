using System;
using System.IO;
using System.Collections;
using System.Xml;
using Microsoft.Win32;
using System.Reflection;


namespace DotNetCodeGenerator
{
	/// <summary>
	/// Generates main function call code...
	/// </summary>
	public class MainGenerator
	{
		// This defines the name and location of the functions.xml file...
		static String functionXMLFile = null;

		/// <summary>
		/// Finds out if a specific Function is abstract...
		/// </summary>
		/// <param name="FunctionName"></param>
		/// <param name="Assembly"></param>
		/// <returns></returns>
		public static bool IsFunctionFromAbstractClass( string FunctionName, string Assembly )
		{
			//check for static method
			Assembly checkAssembly = System.Reflection.Assembly.LoadWithPartialName( Assembly );

			if ( checkAssembly == null )
				checkAssembly = System.Reflection.Assembly.LoadWithPartialName( "mscorlib" );
			
			string methodToCheck = FunctionName.Split('(')[0];
			string classToCheck = methodToCheck.Remove( 
				methodToCheck.LastIndexOf( "." ), 
				methodToCheck.Length - methodToCheck.LastIndexOf( "." ) );

			// Get the type of the class...
			Type classType = checkAssembly.GetType( classToCheck );

			//if ( classType.IsAbstract )
			//	System.Diagnostics.Debugger.Break( );

			return classType.IsAbstract;
		}

		/// <summary>
		/// Generates the function list...
		/// </summary>
		/// <returns></returns>
		public static String GenerateFunctionList( )
	    {

			String functionListResults = null;

			XmlTextReader xr = new XmlTextReader( functionXMLFile );

			bool foundParam = false;

			string functionParameters = null;
			string functionNameEnd = null;
			string typeName = null;
			string originalDll = null;

			while ( xr.Read( ) )
			{
				foundParam = false;
				functionParameters = null;
				functionNameEnd = null;
				typeName = null;				

				// Find out what type of node we are looking at. The XR reader will read one
				// XML element/value at a time (NOT one line at time)...
				if ( xr.NodeType == XmlNodeType.Element )
				{
					// If we've reached the end of the function definition, we don't want to look
					// at it anymore.
					if ( xr.Name == "Function" && xr.NodeType == XmlNodeType.EndElement )
						break;

					if ( xr.Name == "FunctionName" )
					{
						if ( !xr.Read( ) )
							break;

						//if ( xr.Value.IndexOf( "EnterDebugMode" ) > 0 )
							//System.Diagnostics.Debugger.Break( );
						
						Console.WriteLine( "Writing Function: " + xr.Value );
						String[] stringArray = xr.Value.Split('(');
						String functionCall = stringArray[0];
						
						while ( xr.Read( ) )
						{
							// Find the originalDLL...
							if ( xr.Name == "OriginalDll" && xr.NodeType == XmlNodeType.Element )
							{
								// Read to hit the value of the OriginalDLL node...
								xr.Read( );
		
								originalDll = xr.Value.Replace( ".dll", "" );

								// Get out of the loop...
								break;
							}
						}
						
						if ( ( functionCall.IndexOf( "ctor" ) > 0 ) ||
							IsFunctionFromAbstractClass( functionCall, originalDll ) )
						{
							continue;
						}

						// write to file here: write the results of each individual function. 
						functionListResults += "    bSuccess = bSuccess && printFunctionResult( My_" + functionCall;

						while ( xr.Read( ) )
						{		
							if ( xr.Name == "Function" && xr.NodeType == XmlNodeType.EndElement )
								break;
			
							if ( xr.Name == "Param" )
							{
								xr.MoveToContent( );
								typeName = xr.GetAttribute( "Type" );

								foundParam = true;

								if ( !xr.Read( ) )
									break;

								if ( ( xr.Value != "" ) && ( xr.Value.IndexOf( "\r\n\t", 0, xr.Value.Length ) < 0 ) )
								{
									

									if ( typeName != null )
									{
										functionParameters += "_" + typeName.Replace( ".", "_" ).Replace( "[", "_" ).Replace( "]", "_" ).Replace( "&", "_" );
										functionListResults += "_" + typeName.Replace( ".", "_" ).Replace( "[", "_" ).Replace( "]", "_" ).Replace( "&", "_" );
									}									
								}
							}
							else
							{
								if ( foundParam )
									break;
							}
						}

						

						functionNameEnd = functionCall.Split( '.' )[functionCall.Split('.').Length - 1];

						functionListResults += "._" + functionNameEnd + functionParameters + "(), \"" + functionCall + "\" );\r\n";
					}                  
  
					
				}
			}

			Exception e;            
			xr.Close( );

			return functionListResults;
		}
		public void SetupHolodeckLocation( )
		{
			RegistryKey localMachineHive = Registry.LocalMachine;

			// Open the subkey...
			RegistryKey pathLocationKey = localMachineHive.OpenSubKey( "SOFTWARE\\HolodeckEE" );

			functionXMLFile = (string)pathLocationKey.GetValue( "InstallPath" );
		    functionXMLFile = string.Concat(functionXMLFile, "\\function_db\\dotNetFunctions.xml");
	
		}
		
		public void generateMain()
		{
			// Get the location of Holodeck...
			SetupHolodeckLocation( );

			// Get output path
			
			string functionDBPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
            string outputPath = string.Concat(functionDBPath, "\\function_db\\DotNetCodeGeneratorDump\\main.cs");
            string inputPath = string.Concat(functionDBPath, "\\function_db\\DotNetMainTemplate.cs");
			string DotNetCodeGeneratorDumpDirPath = string.Concat(functionDBPath,"\\function_db\\DotNetCodeGeneratorDump");

			if (!Directory.Exists(DotNetCodeGeneratorDumpDirPath)) 
			{
				DirectoryInfo di = Directory.CreateDirectory(DotNetCodeGeneratorDumpDirPath);
			}

			StreamReader sr = new StreamReader(inputPath);            
			StreamWriter sw = new StreamWriter(outputPath, false);

			String fileLine = null;
			String newLine = null;
			String generatedFunctionList = null;

			while ( ( fileLine = sr.ReadLine( ) ) != null )
			{
				if ( fileLine.IndexOf( "[FUNCTIONGENERATION]" ) != -1 )                
				{
					generatedFunctionList = GenerateFunctionList( );

					newLine = fileLine.Replace( "[FUNCTIONGENERATION]", generatedFunctionList );					

					sw.WriteLine( newLine );                                
				}
				else                
					sw.WriteLine( fileLine );                
			}                    
        
			sw.Close( );
			sr.Close( );            			
		}
	}
}
