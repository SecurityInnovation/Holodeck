using System;
using System.Reflection;
using TestFramework;
using TestFramework.Exceptions;
using TestFramework.Run;
using TestFramework.Log;
using System.Diagnostics;

// NOTE: Always make sure you use the right namespace.
namespace StressTester
{
    /// <summary>
    /// This is the actual code for the test which inherits from the test framework...
    /// </summary>
    public class StressTesterExec : RunTest
    {        
        // Define the prefix for the name of the class we will load.
        public static string classNamePrefix = "StressTester.StressTesterExec";

        public static string testValue = "TestString";

        /// <summary>
        /// Send keystrokes to the active window (should be Holodeck)
        /// </summary>
        /// <param name="keys">the actual keystrokes (see "SendWait method" in MSDN for exact syntax)</param>
        public static void SendKeyStrokes (string keys, Process processClass) 
        {                         
            processClass.WaitForInputIdle ();
            System.Threading.Thread.Sleep( 100 );
            System.Windows.Forms.SendKeys.SendWait (keys);
        }

        /// <summary>
        /// Kills all process with the specified name.
        /// </summary>
        /// <param name="AppName"></param>
        public static void KillAll( string AppName )
        {
            foreach( Process process in Process.GetProcessesByName( AppName ) )
            {
                process.Kill( );
            }
        }

        public override object getTestClass( string testID )
        {            
            try
            {
                Assembly assembly = Assembly.GetExecutingAssembly( );
            
                // get the full name of the type while ignoring case...
                string typeName = assembly.GetType( classNamePrefix + testID, true, true ).FullName;
                
                TestFramework.Log.Log.printSuccess( "INSTANTIATING CLASS", typeName );

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
                StressTesterExec stressTesterExec = new StressTesterExec( );
            
                // parse any command line arguments...
                stressTesterExec.parseCommandLineArgs( args );

                if ( ! RunTest.printingUsageInfo )
                    TestFramework.Log.Log.printTestStatus( "==> TEST PASSED! <==" );

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
                TestFramework.Log.Log.printError( "\nMessage:\n\n" + e.Message + "\n\nStack Trace:\n\n" + e.StackTrace );
            }
            finally
            {
                // Close the logfile if it is open...
                if ( TestFramework.Log.Log.logWriter != null )
                    TestFramework.Log.Log.logWriter.Close( );

                // Kill any existing Holodecks running on the machine...
                KillAll( "HolodeckGUI" );
            }

            if ( ! RunTest.printingUsageInfo )
                TestFramework.Log.Log.printTestStatus( "==> TEST FAILED! <==" );

            // some exception occurred...
            return 1;
        }
    }
}
