using System;
using System.IO;
using System.Security.Permissions;
using Microsoft.Win32;
using System.Reflection;
using TestFramework;
using TestFramework.Exceptions;
using TestFramework.Run;
using TestFramework.Log;
using Holodeck;
using System.Threading;

namespace UIFaultIntercepterTest
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
		
	

	public class UIFaultIntercepterTest: RunTest
	{        
		// Define the prefix for the name of the class we will load.
		public static string classNamePrefix = "UIFaultIntercepterTest.UIFaultIntercepterTest";


		public void GetHolodeck(string faultname){
		
			//launch HD
			//t.LaunchFaultInterceptorAsAUT();
			string holodeckPath;
			holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue("InstallPath");
			string FaultsInterceptorPath = (System.Environment.CurrentDirectory); 
			int index = FaultsInterceptorPath.IndexOf("UIFaultIntercepterTest");
			if (index != -1)
			{
				FaultsInterceptorPath = FaultsInterceptorPath.Substring(0, index-1);
			}

			FaultsInterceptorPath=String.Concat(FaultsInterceptorPath,"\\FaultIntercepter\\Debug\\FaultIntercepter.exe");
			//Console.WriteLine(FaultsInterceptorPath);
						
			//launching AUT
			Holodeck.HolodeckProcess.Start();
			Holodeck.HolodeckPane.Reset();
			Holodeck.HolodeckPane.File_NewProject(); //  -- NOT WORKING SO USING MANUAL KEY STROKES
			Holodeck.NewProjectPane1.Reset();
			Holodeck.NewProjectPane1.SetProjectLocation(string.Concat(holodeckPath,"\\FaultIntercepter"));
			Holodeck.NewProjectPane1.Next();
			Holodeck.NewProjectPane2.Reset();
			Holodeck.NewProjectPane2.SetApplicationName(FaultsInterceptorPath);
			
			//NOT WORKING - DOING MANUAL
			//Holodeck.NewProjectPane2.SetApplicationParams(faultname); 
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}"); 
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane2.SetEditBoxText(faultname); 
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane2.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");

			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			//select native and .net functions and goto main holodeck window
			Holodeck.NewProjectPane3.SendKeyStrokes("{TAB}");
			Holodeck.NewProjectPane3.SendKeyStrokes(" ");
			Holodeck.NewProjectPane3.Finish();
//			//Get faults Pane
			Holodeck.HolodeckPane.SendKeyStrokes("%A");
			Holodeck.HolodeckPane.SendKeyStrokes("F");
		
		}


		public void CheckOutcome()
		{

			RegistryKey LMachine = RegistryKey.OpenRemoteBaseKey(RegistryHive.LocalMachine, System.Environment.MachineName);            
			RegistryKey SKey = LMachine.OpenSubKey( "SOFTWARE", true );
			RegistryKey HKey =  SKey.OpenSubKey( "HolodeckEE", true);
			RegistryKey OutcomeKey;
			do
			{
				Thread.Sleep(500);
				OutcomeKey = HKey.OpenSubKey("Outcome");
			}while(OutcomeKey == null);
					
			String OutcomeType = (String)(OutcomeKey.GetValue("Outcome"));
			Console.WriteLine("==> Test Outcome: {0}! <==",OutcomeType);
		
			Thread.Sleep(4000);
			Console.WriteLine("Now Closing Holodeck...");
			Thread.Sleep(3000);
			Holodeck.HolodeckPane.File_Exit(false);   
		}


	public override object getTestClass( string testID )
		{            
			try
			{
				Assembly assembly = Assembly.GetExecutingAssembly( );
            
				// get the full name of the type while ignoring case...
				string typeName = assembly.GetType( classNamePrefix + testID, true, true ).FullName;
                
				Log.printSuccess( "INSTANTIATING CLASS", typeName );

				// Get an instance of the class...
				return ( (RunTest)assembly.CreateInstance( typeName ) );
			}
			catch ( Exception )
			{
				// throw an exception stating that no classes were found to match the specified
				// test ID...
				throw new NoTestClassException( );
			}
		}
	

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static int Main(string[] args)
		{

	RegistryKey LMachine = RegistryKey.OpenRemoteBaseKey(RegistryHive.LocalMachine, System.Environment.MachineName);            
	RegistryKey SKey = LMachine.OpenSubKey( "SOFTWARE", true );
	RegistryKey HKey =  SKey.OpenSubKey( "HolodeckEE", true);
	try{
		HKey.DeleteSubKey("FaultIntercepterReady");
		HKey.DeleteSubKey("FaultReady");
		}catch(System.ArgumentException ArgumentError){}


			try
			{
				UIFaultIntercepterTest UIFaultIntercepterTest = new UIFaultIntercepterTest( );
            
				// parse any command line arguments...
				UIFaultIntercepterTest.parseCommandLineArgs( args );                

				if ( ! RunTest.printingUsageInfo )
				{

//					RegistryKey OutcomeKey;
//
//					do
//					{
//						Thread.Sleep(500);
//						OutcomeKey = HKey.OpenSubKey("Outcome");
//					}while(OutcomeKey == null);
//					
//					String OutcomeType = (String)(OutcomeKey.GetValue("Outcome"));
//					Console.WriteLine("==> Test Outcome: {0}! <==",OutcomeType);
//		
//					Thread.Sleep(4000);
//					Console.WriteLine("Now Closing Holodeck...");
//					Thread.Sleep(3000);
//					Holodeck.HolodeckPane.File_Exit(false);   

					return 0;
				}
			}
				
			catch ( TestExceptions )
			{
				// Exceptions that have occurred have been rethrown, but
				// they were already printed out, so we just catch it here.
				// if any exception occurs, it is thrown again...
				// this prevents us from causing an unhandled exception...
			}
			catch ( Exception e )
			{
				// Catch ANY other exception and print out an error.
				Log.printError( "\nMessage:\n\n" + e.Message + "\n\nStack Trace:\n\n" + e.StackTrace );
			}
			finally
			{
				// Close the logfile if it is open...
				if ( Log.logWriter != null )
					Log.logWriter.Close( );
			}

			if ( ! RunTest.printingUsageInfo )
				Log.printTestStatus( "==> TEST FAILED! <==" );

			// some exception occurred...
			return 1;
		}
	}
}
