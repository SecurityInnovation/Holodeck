// This is the main project file for VC++ application project 
// generated using an Application Wizard.

#include "stdafx.h"
#include "HoloScriptTester.h"

namespace HoloScriptTestMain
{	
	Object * HoloScriptTest::getTestClass( String * testID )
	{            
		try
		{
			Assembly * assembly = Assembly::GetExecutingAssembly( );
        			
			classNamePrefix = classNamePrefix->Concat( classNamePrefix, testID );

			// get the full name of the type while ignoring case...
			String * typeName = assembly->GetType( classNamePrefix, true, true )->FullName;
            
			TestFramework::Log::Log::printSuccess( "INSTANTIATING CLASS", typeName );
			
			// Get an instance of the class...
			return ( dynamic_cast<RunTest *>( assembly->CreateInstance( typeName ) ) );
		}
		catch ( Exception * )
		{
			// throw an exception stating that no classes were found to match the specified
			// test ID...
			throw new NoTestClassException( );
		}
	}	
}

/// <summary>
/// The main entry point for the application.
/// </summary>
int main( int argCount, char * args[] )
{
	try
	{
		HoloScriptTestMain::HoloScriptTest * holoscriptTest = new HoloScriptTestMain::HoloScriptTest( );
    
		// Need to build a managed string array...
		String * stringArray[] = new String * [argCount -1];

		stringArray->Clear( stringArray, 0, stringArray->Count );
		
		for ( int argCounter = 1; argCounter < argCount; argCounter++ )
		{
			stringArray[argCounter - 1] =  new String( args[argCounter] );
		}

		// parse any command line arguments...
		holoscriptTest->parseCommandLineArgs( stringArray );

		if ( ! RunTest::printingUsageInfo )
			TestFramework::Log::Log::printTestStatus( "==> TEST PASSED! <==" );

		return 0;
	}
	catch ( TestExceptions * )
	{
		// Exceptions that have occurred have been rethrown, but
		// they were already printed out, so we just catch it here.
		// if any exception occurs, it is thrown again...
		// this prevents us from causing an unhandled exception...
	}
	catch ( Exception * e )
	{
		String * errorString = "";
		errorString = errorString->Concat( "\nMessage:\n\n", 
			e->Message, 
			"\n\nStack Trace:\n\n", 
			e->StackTrace );

		// Catch ANY other exception and print out an error.
		TestFramework::Log::Log::printError( errorString );
	}
	__finally
	{
		// Close the logfile if it is open...
		if ( TestFramework::Log::Log::logWriter != NULL )
			TestFramework::Log::Log::logWriter->Close( );
	}

	if ( ! RunTest::printingUsageInfo )
		TestFramework::Log::Log::printTestStatus( "==> TEST FAILED! <==" );

	// some exception occurred...
	return 1;
}

