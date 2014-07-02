using System;
using System.Reflection;
using TestFramework;
using TestFramework.Exceptions;
using TestFramework.Run;
using TestFramework.Log;

// NOTE: Always make sure you use the right namespace.
namespace TestingXML
{
	/// <summary>
	/// This is the actual code for the test which inherits from the test framework...
	/// </summary>
	public class TestingXML : RunTest
	{        
		// Define the prefix for the name of the class we will load.
		public static string classNamePrefix = "TestingXML.TestingXML";
		
		public static string XmlFileName = "ErrorCodes.xml";	//WARNING :: Do not include complete path
		//public static string XmlFileName = "Exceptions.xml";
		//public static string XmlFileName = "Ports.xml";
		//public static string XmlFileName = "Functions.xml";
		

		public static string testValue = "TestString";

     /*   public override void performPreRunTasks( )
        {
            XmlFileName = (string)testSpecificParameters[0];
			Console.WriteLine(XmlFileName);
        }
*/
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
			try
			{
				TestingXML TestingXML = new TestingXML( );
            
				// parse any command line arguments...
				TestingXML.parseCommandLineArgs( args );                

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
