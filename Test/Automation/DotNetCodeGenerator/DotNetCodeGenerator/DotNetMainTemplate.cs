using System;
using System.Reflection;
using System.IO;
using Microsoft.Win32;

namespace DotNetInterceptTester
{

	public class InterceptionMaintenance
	{

		/// <summary>
		/// Disables interception in the current .NET thread.
		/// </summary>
		public static void disableInterception( )
		{
			Assembly dotNetAssembly = Assembly.LoadWithPartialName( "HeatDotNet" );

			// Get the Interception type...
			System.Type InterceptionType = dotNetAssembly.GetType( "HeatDotNet.Interception" );    			
 
				InterceptionType.InvokeMember( "DisableInterceptionInCurrentThread", 
					System.Reflection.BindingFlags.InvokeMethod, 
					null, 
					new Object( ), 
					null );		
		}

		/// <summary>
		/// Enables interception in the current .NET thread.
		/// </summary>
		public static void enableInterception( )
		{
			Assembly dotNetAssembly = Assembly.LoadWithPartialName( "HeatDotNet" );

			// Get the Interception type...
			System.Type InterceptionType = dotNetAssembly.GetType( "HeatDotNet.Interception" );             		
 
			InterceptionType.InvokeMember( "EnableInterceptionInCurrentThread", 
				System.Reflection.BindingFlags.InvokeMethod, 
				null, 
				new Object( ), 
				null );	
		}
		bool printFunctionResult( bool bSuccess, string functionName )
		{
            // append results of each individual test to log file.

			string LogOutputPath;
			LogOutputPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
			LogOutputPath = string.Concat(LogOutputPath, "\\function_db\\TestResultsForDotNetInterceptor.txt");

			//StreamWriter sw = new StreamWriter(@"C:\cvsroot\CVSROOT\Holodeck\Test\Automation\DotNetCodeGenerator\TestResults.txt", true);
			StreamWriter sw = new StreamWriter(LogOutputPath, true);
			sw.Write(functionName);

			if (bSuccess)
				sw.WriteLine(" Passed");   
			else
			{
				sw.WriteLine(" Failed");
			}

			sw.WriteLine();
			sw.Close();

			return bSuccess;
		}

		public void InterceptMain( )
		{
			System.Console.WriteLine( "<== HOLODECK INTERCEPT TESTER 1.0 ==>\n\n" );

			disableInterception( );

			enableInterception( );
    
			bool bSuccess = true;
			[FUNCTIONGENERATION]

			// get output path
			string outputPath;
			outputPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
			outputPath = string.Concat(outputPath, "\\function_db\\dotNetFunctions.xml");

			//append results of entire test to log file
			StreamWriter sw = new StreamWriter(outputPath, true);

			if ( bSuccess )
				sw.WriteLine("Test Passed.");
			else
				sw.WriteLine("Test Failed.");
		}
	}
}