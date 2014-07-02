using System;
using System.Reflection;
//using TestFramework;
using System.Threading;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32;
using System.IO;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;


// NOTE: Always make sure you use the right namespace.
namespace NewProjectTest
{
	/// <summary>
    /// This is the actual code for the test which inherits from the test framework...
    /// </summary>
    public class NewProjectTest : RunTest
    {
        // Define the prefix for the name of the class we will load.
        public static string classNamePrefix        = "NewProjectTest.NewProjectTest";
	
        /// <summary>
        /// This method is for test cases where a failure is expected to occur after the project
        /// location is set...
        /// </summary>
        /// <param name="projectPath"></param>
        public static void StartHolodeckSettingProjectLocation( string projectPath)
        {
            Holodeck.HolodeckProcess.Start();
            Holodeck.HolodeckPane.Reset ();
            Holodeck.HolodeckPane.File_NewProject ();
            Holodeck.NewProjectPane1.Reset ();
            Holodeck.NewProjectPane1.SetProjectLocation (projectPath);
            Holodeck.NewProjectPane1.Next ();

            // Holodeck will now either continue in the wizard, or stop with an error...
            
        }

		/// <summary>
		/// This method is for test cases where a failure is expected to occur after the project
		/// location and application path is set...
		/// </summary>
		/// <param name="projectPath"></param>
		/// <param name="appPath"></param>
		public static void StartHolodeckSettingProjectLocationAppLocation( string projectPath, string appPath)
		{
			Holodeck.HolodeckProcess.Start();
			Holodeck.HolodeckPane.Reset();
			Holodeck.HolodeckPane.File_NewProject();
			Holodeck.NewProjectPane1.Reset();
			Holodeck.NewProjectPane1.SetProjectLocation (projectPath);
			Holodeck.NewProjectPane1.Next();
			Holodeck.NewProjectPane2.SetApplicationName (appPath);

			// Holodeck will now either continue in the wizard, or stop with an error...
            
		}
		
		
        /// <summary>
        /// This function waits for Holodeck to exit. If Holodeck has not exited after a specific
        /// period of time, an exception will be thrown.
        /// </summary>
        /// <param name="ErrorMessageToUse"></param>
        public static void WaitForHolodeckToExit( string ErrorMessageToUse )
        {
            int waitCounter = 0;

            while ( Holodeck.HolodeckProcess.IsRunning( ) )
            {
                System.Threading.Thread.Sleep( 500 );

                waitCounter++;

                if ( waitCounter == 20 )
                    throw new Exception ( ErrorMessageToUse );
            }
        }

		/*
        public static void WaitForAutToStart( )
        {
            ProcessTools.ProcessTools processTools = new ProcessTools.ProcessTools( );

            for (;;)
            {
                int timerCount = 0;

                try
                {
                    System.Threading.Thread.Sleep( 200 );

                    ProcessTools.ProcessTools processTool = new ProcessTools.ProcessTools( );

                    // This will throw an exception if the AUT is NOT running...
                    processTool.GetChildProcess( Holodeck.HolodeckProcess.GetProcessID( ) );

                    // Get out of the loop...
                    break;
                }
                catch ( Exception )
                {
                    // we don't do anything with the exception...
                    timerCount++;

                    if ( timerCount == 20 )
                        throw new Exception( "Holodeck never started the AUT." );
                }
            }
        }
*/
        /// <summary>
        /// This method is used for cases where the wizard can fully be cycled without any error occurring.
        /// </summary>
        /// <param name="projectPath"></param>
        /// <param name="appPath"></param>
        /// <param name="pauseApp"></param>
        /// <param name="procChaining"></param>
        /// <param name="nativeFunctions"></param>
        /// <param name="netFunctions"></param>
        public static void RunTestCases(string projectPath,string appPath,bool pauseApp,bool procChaining,string nativeFunctions,string netFunctions)
        {
            try
            {
                Holodeck.HolodeckProcess.Start();
                Holodeck.HolodeckPane.Reset ();
                Holodeck.HolodeckPane.File_NewProject ();
                Holodeck.NewProjectPane1.Reset ();
                Holodeck.NewProjectPane1.SetProjectLocation (projectPath);
                Holodeck.NewProjectPane1.Next ();
                Holodeck.NewProjectPane2.Reset ();
                Holodeck.NewProjectPane2.SetApplicationName (appPath);
    			
                //If application path is not given it will attach running application
                if (appPath == "")
                {
                    Holodeck.NewProjectWizard.AttachRunningApplication ();
                }
                else
                {
                    Holodeck.NewProjectPane2.SetApplicationName (appPath);
                }
    			
                Holodeck.NewProjectPane2.Next ();
                Holodeck.NewProjectPane3.Reset ();	
                Holodeck.NewProjectPane3.SetPauseApplication (pauseApp);
                Holodeck.NewProjectPane3.SetProcessChaining (procChaining);
    			
                //To select Native Functions or .NET Functions.
                //New class NewProject Wizard is created in UIFramework for the same.
                Holodeck.NewProjectWizard.NativeFunctions(nativeFunctions);
                Holodeck.NewProjectWizard.NetFunctions(nativeFunctions,netFunctions);
            }			
			catch (Holodeck.HolodeckExceptions.HolodeckCrashedException)
			{
				Log.printTestStatus("Holodeck crashed!");
				return;
			}
			catch (Holodeck.HolodeckExceptions.HolodeckAlreadyRunningException)
			{
				Log.printTestStatus("Holodeck Already Running!!Please Close it");
				return;
			}
			catch (System.Exception)
			{
				Log.printTestStatus("Something bad happened!");
				return;
			}
		}

        public override object getTestClass( string testID )
        {            
            try
            {
                Assembly assembly = Assembly.GetExecutingAssembly( );
            
                // Get the full name of the type while ignoring case...
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
            try
            {
                NewProjectTest NewProjectTest = new NewProjectTest( );
            
                // parse any command line arguments...
                NewProjectTest.parseCommandLineArgs( args );

                if ( ! RunTest.printingUsageInfo )
                    Log.printTestStatus( "==> TEST PASSED! <==" );

                return 0;
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
