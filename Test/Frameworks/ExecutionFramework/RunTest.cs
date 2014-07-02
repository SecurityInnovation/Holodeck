using System;
using System.IO;
using System.Collections;
using System.Threading;
using TestFramework;
using TestFramework.Exceptions;
using TestFramework.DependencyVerifier;
using TestFramework.Log;

// FEATURE: aliases could be used to customize dependencies so that the files referenced would automatically be searched
//          for on different machines.

namespace TestFramework
{
    namespace Run
    {
        public class DisabledTest
        {
            public string testID;
            public string reason;
        }

        /// <summary>
        /// The RunTest class provides test run management. The tester should create a separate
        /// control class which inherits from this class. This class should implement the
        /// "getTestClass(...)" method. If the method is not defined, an exception will be thrown.
        /// The "getTestClass(...)" method must give back a class instance representing the 
        /// specific class containing test code the tester wished to implement using the framework.
        /// The test class must inherit from the test control class, which in turn inherits from
        /// the RunTest class. The class hierarchy is something like:
        /// 
        ///     RunTest
        ///         TesterControlClass - implements getTestClass(...) giving back TestCase1Class...
        ///             TestCase1Class - implements executeTest(...)
        ///             TestCase2Class  ...
        ///             ...
        ///             
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class RunTest
        {
            /// Global public static string provides access to the test ID for file writing, etc.
            public string TestCaseID                        = "Undefined";      // allows access to test case ID
            public static string OutputPath                 = null;             // defines the output path for output created by the test.
            public static ArrayList DisableTests            = new ArrayList( ); // array of testIDs that should be disabled...
            public static string disableTestsLocation       = "Undefined";

            private const string DEFAULT_DEPENDENCY_FILE    = "dependency.xml";
            public static Dependency m_dependencyVerifier;
            public static bool printingUsageInfo = false;
            public static bool verbose = false;
            public static bool runningConcurrently = false;
            public static string logFile = "log.txt";
            public static ArrayList testSpecificParameters = new ArrayList( );

            // setup a global mutex for managing log writing - we are not initially the owner (we don't need it)...
            public static Mutex g_mutex                 = new Mutex( false );

            public ArrayList getDisabledTests( string location )
            {
                ArrayList testList = new ArrayList( );

                StreamReader sr = new StreamReader( location );

                string[] currentLine;
                DisabledTest disabledTest;
                
                // add all the test IDs to the list...
                while ( sr.Peek( ) >= 0 )
                {
                    currentLine = sr.ReadLine( ).Split( ( new char[] { ' ', '\t' } ), 2  );

                    disabledTest = new DisabledTest( );
                    
                    if ( currentLine.Length > 0 )
                        disabledTest.testID = currentLine[0];

                    if ( currentLine.Length > 1 )
                        disabledTest.reason = currentLine[1];
                    
                    testList.Add( disabledTest );
                }

                // close the file...
                sr.Close( );

                return testList;
            }

            /// <summary>
            /// Instantiates the Dependency class to verify dependencies in the dependency file.
            /// </summary>
            /// <param name="dependencyFile">A URL or full path to the location of the dependency XML file.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            private static void checkDependencies( string dependencyFile )
            {
                // the class, when instantiated with a dependency File, automatically performs
                // verification...
                m_dependencyVerifier = new Dependency( dependencyFile );        
            }

            /// <summary>
            /// Calls the overloaded "executeTest" method created by the user. If none exists,
            /// an exception is thrown.
            /// </summary>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public virtual void executeTest( )
            {
                throw new NoExecuteTestMethodException( );
            }

            /// <summary>
            /// Calls the overloaded "getTestClass" method created by the user. If none exists,
            /// an exception is thrown. The "getTestClass" provides a reference to the class
            /// representing the test. This class must overload the "executeTest" method defined
            /// in the RunTest class by inheriting from it.
            /// </summary>
            /// <param name="testID">The ID representing the test class.</param>
            /// <returns>An instance of the test class representing the test ID.</returns>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public virtual object getTestClass( string testID )
            {
                throw new NoGetTestMethodException( );
            }

            /// <summary>
            /// Allows you to perform operations right before the specified test is executed.
            /// </summary>
            public virtual void performPreRunTasks( )
            {

            }

			/// <summary>
			/// Allows you to perform operations after the test is executed.
			/// </summary>
			public virtual void performPostRunTasks( )
			{

			}

            /// <summary>
            /// Prints command line help if no parameters are passed on the command line of the
            /// test executable.
            /// </summary>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            private static void printCommandLineHelp( )
            {
                printingUsageInfo = true;

                Console.WriteLine( "<< Test Framework V. 1.0 Command Line Help >>\n" );
                Console.WriteLine( "\tUsage:\t[test executable] [test number] [options] -z [Test Case Options]\n" );
                Console.WriteLine( "\tOptions:\n\n\t-d [dependency file]\tUse alternate dependency file" );
                Console.WriteLine( "\t-a Enable breakpoint for debugging." );
                Console.WriteLine( "\t-c Use Color in output.");
                Console.WriteLine( "\t-l [log file]\tPrint output to a log file" );
                Console.WriteLine( "\t-s Run test silently (ignored with -l)" );
                Console.WriteLine( "\t-o [output path]\tPath to place output files." );
                Console.WriteLine( "\t-v Enable verbose logging" );
                Console.WriteLine( "\t-x [file] Disable test IDs specified line by line in the file.");                
                Console.WriteLine( "\t-z *Test Case Specific parameters FOLLOW this flag. No other flag can follow." );
                Console.WriteLine( );
                Console.WriteLine( "* = It is up to the tester to implement these features by checking the Runtest class" );
                Console.WriteLine( "    flags queryOnly and runAutomatically. Some tests may not provide these features." );
                Console.WriteLine( );
            }

            /// <summary>
            /// First verifies the dependencies defined in the depency XML file. After dependencies
            /// have been verified, we then call the getTestClass(...) method overloaded by the
            /// tester in their test control class to get an instance of one of the test case
            /// classes. We then execute the code associated with that test case (and class).
            /// </summary>
            /// <param name="testCaseID">The ID representing the test class.</param>
            /// <param name="dependencyFile">The dependency XML file to use to check dependencies.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public void performRun( string testCaseID, string dependencyFile )
            {
                // share the information about the test ID to all...
                TestCaseID = testCaseID;

                RunTest testClass = null;

                try
                {
                    // DEPENDENCY CHECKING IS DISABLED IN THIS VERSION! IF WE DETERMINE THAT WE
                    // NEED IT WE CAN ADD IT BACK IN.
                    // check the file dependencies...
                    // checkDependencies( dependencyFile );
                        
                    // get the instance of the test class created by the tester...
                    testClass = (RunTest)getTestClass( testCaseID );
                        
                    Log.Log.printSuccess( "EXECUTING Test Case", testCaseID );

                    // set the test ID...
                    testClass.TestCaseID = TestCaseID;

                    // run the executeTest(...) method defined by the test class...
                    testClass.executeTest( );
                }
                catch ( RunTestExceptions e )
                {
                    TestExceptions.printException( e );

                    // rethrow the exception...
                    throw e;
                }
            }

            /// <summary>
            /// Parses command line arguments passed to the test executable.
            /// </summary>
            /// <param name="args">The command line arguments string array passed from the entrypoint procedure.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public virtual void parseCommandLineArgs( string[] args )
            {
                char[] argumentArray;
                string testCaseID = "Unprovided";
                string dependencyFile = DEFAULT_DEPENDENCY_FILE;

                try 
                {
                    if ( args.Length == 0 )
                        throw new NoTestException( );
                    else
                    {
                        // go through all the arguments passed on the commandline...
                        for ( int argCount = 0; argCount < args.Length; argCount ++ )
                        {
                            argumentArray = args[argCount].ToCharArray( );
                            
                            if ( args.Length > 1 )
                            {
                                if ( ( argumentArray[0] == '-' ) || ( argumentArray[0] == '/' ) && ( argCount != 0 ) )
                                {
                                    // switch based on 
                                    switch( argumentArray[1] )
                                    {
                                        case 'd':
                                
                                            // check to ensure that a dependency exists...
                                            if ( ( argCount + 1 ) >= args.Length )
                                            {
                                                throw new NoDependencyFileException( );
                                            }
                                            else
                                            {
                                                // set to use the alternate dependency file specified...
                                                dependencyFile = args[argCount + 1];
                                            }

                                            break;

                                        case 'a':
                                            System.Diagnostics.Debugger.Break( );
                                            break;

                                        case 'c':
                                            Log.Log.color = true;
                                            break;

                                        case 'l':
                                            if ( ( argCount + 1 ) >= args.Length )
                                            {
                                                throw new Exception( "No log file name specified with 'l' option." );
                                            }
                                            else
                                            {
                                                logFile = args[argCount + 1];

                                                // set to use the log file specified...
                                                Log.Log.logWriter = new StreamWriter( logFile ); 
                                            }
                                            break;

                                        case 'o':
                                            if ( ( argCount + 1 ) >= args.Length )
                                            {
                                                throw new Exception( "No output path specified with 'o' option." );
                                            }
                                            else
                                            {
                                                // set to use the log file specified...
                                                OutputPath = args[argCount + 1]; 
                                            }
                                            break;

                                        case 's':
                                            Log.Log.logSilent = true;
                                            break;

                                        case 'v':
                                            verbose = true;
                                            break;

                                        case 'x':
                                            if ( ( argCount + 1 ) >= args.Length )
                                            {
                                                throw new Exception( "No file specified with 'l' option." );
                                            }
                                            else
                                            {
                                                // set to use the log file specified...
                                                disableTestsLocation = args[argCount + 1];

                                                DisableTests = getDisabledTests( disableTestsLocation );
                                            }
                                            break;

                                        case 'z':
                                            // create an arraylist containing all the test specific parameters...
                                            for ( int count = argCount + 1; count < args.Length; count ++ )
                                            {
                                                testSpecificParameters.Add( args[count] );
                                            }         
                                            break;
                                    }
                                }
                            }
                        }
                    }

                    if ( verbose )
                        Log.Log.printTestStatus( "<< Test Framework V. 1.0 Running >>\n" );

                    if ( verbose )
                    {
                        if ( dependencyFile != DEFAULT_DEPENDENCY_FILE )
                            Log.Log.printSuccess( "Using Alternate Dependency", dependencyFile );
                    }

                    // set the test case ID...
                    testCaseID = args[0];

                    performPreRunTasks( );

                    DisabledTest currentDisabledTest = null;

                    foreach ( DisabledTest disabledTest in DisableTests )
                    {
                        if ( disabledTest.testID == testCaseID )
                            currentDisabledTest = disabledTest;
                    }

                    if ( currentDisabledTest == null )
                    {
                        // perform the test run...
                        performRun( testCaseID, dependencyFile );

						// perform post run tasks...
						performPostRunTasks( );
                    }
                    else
                    {
                        Log.Log.printSuccess( "TEST DISABLED", "The test [" + testCaseID + "] has been disabled using a disable file.\n" + "REASON: [" + currentDisabledTest.reason + "]" );
                    }
                }
                catch ( NoTestException e )
                {
                    TestExceptions.printException( e );

                    printCommandLineHelp( );

                    // rethrow the exception to be caught again...
                    throw e;
                }
                catch ( TestExceptions e )
                {
                    // rethrow the exception to be caught again...
                    throw e;
                }            
            }
        }
    }
}
