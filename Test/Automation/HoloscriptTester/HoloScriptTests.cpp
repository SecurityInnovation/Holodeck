#include "stdafx.h"
#include "HoloScriptTester.h"
#include "HoloScriptTests.h"

using namespace System::IO;

namespace HoloScriptTests
{
	// First simple test case just runs notepad without any special features.
	void HoloScriptTest1::executeTest( )
	{
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		TestFramework::Log::Log::printSuccess( "LOG OPENED", holoApp->GetLogFileName( ) );		

		System::Threading::Thread::Sleep( System::TimeSpan::FromSeconds( 10 ) );		

		holoApp->TerminateApplication( );		
	}

	// Use LaunchApplication(...) method call with no log filters specified (NULL).
	void HoloScriptTest2::executeTest( )
	{
		holoApp = new HoloScriptApplication( );

		holoApp->LaunchApplication( HoloScriptTestMain::HoloScriptTest::testApplication,
			"",
			false,
			NULL );

		TestFramework::Log::Log::printSuccess( "LOG OPENED", holoApp->GetLogFileName( ) );		

		System::Threading::Thread::Sleep( System::TimeSpan::FromSeconds( 10 ) );		

		holoApp->TerminateApplication( );			

	}

	// Use LaunchApplication(...) method call with one function filter specified...
	void HoloScriptTest3::executeTest( )
	{
		holoApp = new HoloScriptApplication( );

		ArrayList * functionFilterList = new ArrayList( );

		functionFilterList->Add( S"CreateFileA" );

		if ( !holoApp->LaunchApplication( HoloScriptTestMain::HoloScriptTest::testApplication,
			"",
			false,
			functionFilterList ) )
			throw new Exception( "Could not attach to application." );


		TestFramework::Log::Log::printSuccess( "LOG OPENED", holoApp->GetLogFileName( ) );		

		System::Threading::Thread::Sleep( System::TimeSpan::FromSeconds( 10 ) );		

		holoApp->TerminateApplication( );			
	}

	// Use LaunchApplication(...) method call with half the functions specified in the filter...
	void HoloScriptTest4::executeTest( )
	{
		holoApp = new HoloScriptApplication( );

		ArrayList * functionFilterList = new ArrayList( );

		// Build a list of half of the functions...
		for ( int count = 0; count < holoApp->GetFilteredFunctionList( )->Count / 2; count++ )
		{
			functionFilterList->Add( holoApp->GetFilteredFunctionList( )->get_Item( count ) );
		}
		
		if ( !holoApp->LaunchApplication( HoloScriptTestMain::HoloScriptTest::testApplication,
			"",
			false,
			functionFilterList ) )
			throw new Exception ( "Could not attach to application." );

		TestFramework::Log::Log::printSuccess( "LOG OPENED", holoApp->GetLogFileName( ) );		

		System::Threading::Thread::Sleep( System::TimeSpan::FromSeconds( 10 ) );		

		holoApp->TerminateApplication( );			
	}

	// Use LaunchApplication(...) method call with all the functions specified in the filter...
	void HoloScriptTest5::executeTest( )
	{
		holoApp = new HoloScriptApplication( );
		
		if ( !holoApp->LaunchApplication( HoloScriptTestMain::HoloScriptTest::testApplication,
			"",
			false,
			holoApp->GetFilteredFunctionList( ) ) )
			throw new Exception ( "Could not attach to application." );

		TestFramework::Log::Log::printSuccess( "LOG OPENED", holoApp->GetLogFileName( ) );		

		System::Threading::Thread::Sleep( System::TimeSpan::FromSeconds( 10 ) );		

		holoApp->TerminateApplication( );			
	}

	// Use GetFilteredFunctionList(...) method to get all functions before interception...
	void HoloScriptTest6::executeTest( )
	{
		holoApp = new HoloScriptApplication( );
		
		if ( holoApp->GetFilteredFunctionList( )->Count <= 0 )
			throw new Exception ( "Function filter list count is 0 or less!" );
	}

	// Use GetFilteredFunctionList(...) method to get currently filtered functions after interception
	// with no filters specified (NULL)...
	void HoloScriptTest7::executeTest( )
	{
		HoloScriptTest2 * test = new HoloScriptTest2( );

		test->executeTest( );
		
		if ( !test->holoApp->GetFilteredFunctionList( )->Count == 0 )
			throw new Exception ( "Function filter list count should be zero but is not" );
	}

	// Use GetFilteredFunctionList(...) method to get currently filtered functions after interception
	// with one filter specified...
	void HoloScriptTest8::executeTest( )
	{
		HoloScriptTest3 * test = new HoloScriptTest3( );

		test->executeTest( );
		
		if ( test->holoApp->GetFilteredFunctionList( )->Count == 1 )
			throw new Exception ( "Function filter list should be 1 but is not!" );
	}

	// Use GetFilteredFunctionList(...) method to get currently filtered functions after interception
	// with all filters specified...
	void HoloScriptTest9::executeTest( )
	{
		HoloScriptTest3 * test = new HoloScriptTest3( );

		test->executeTest( );
		
		if ( test->holoApp->GetFilteredFunctionList( )->Count <= 0 )
			throw new Exception ( "Function filter count should not be <= 0!" );
	}

	// Use OpenLogFile(...) on an existing log file to attempt to switch over...
	void HoloScriptTest10::executeTest( )
	{
		// Use the general test case to start up holoApp...
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		// Get the log used in previous test...
		String * fileName = test->holoApp->GetLogFileName( );

		test = new HoloScriptTest1( );
		
		holoApp = new HoloScriptApplication( );		

		holoApp = startAppRunning( );

		holoApp->OpenLogFile( fileName );

		TestFramework::Log::Log::printSuccess( "LOG OPENED", holoApp->GetLogFileName( ) );		

		System::Threading::Thread::Sleep( System::TimeSpan::FromSeconds( 10 ) );		

		holoApp->TerminateApplication( );	
	}

	// Use OpenLogFile(...) on a non-existent log file.
	void HoloScriptTest11::executeTest( )
	{		
		holoApp = new HoloScriptApplication( );			
		
		holoApp->OpenLogFile( "xjsiah" );

		String * logFile = holoApp->GetLogFileName( );

		TestFramework::Log::Log::printSuccess( "LOG RECORDED BY HOLOAPP", logFile );

		if ( logFile != NULL )
		{
			if ( logFile->Length > 0 )
				throw new Exception ( "Opening of non-existent log file apparently successful!"  );
		}
	}

	// Use OpenLogFile(...) on an empty log file...
	void HoloScriptTest12::executeTest( )
	{		
		holoApp = new HoloScriptApplication( );			
		
		StreamWriter * sw = new StreamWriter( "test.hdl" );

		sw->Close( );
		
		holoApp->OpenLogFile( "test.hdl" );

		String * logFile = holoApp->GetLogFileName( );

		TestFramework::Log::Log::printSuccess( "LOG RECORDED BY HOLOAPP", logFile );

		if ( logFile == NULL )
		{
			throw new Exception ( "Holodeck did not open the log file or retain information about it!"  );
		}
		else
		{
			if ( logFile->Length == 0 )
				throw new Exception ( "Holodeck did not open the log file or retain information about it!"  );
		}
	}

	//////////////////////////////////////////////////////////////
	////// TESTS 13-14 already covered in above test cases ///////
	//////////////////////////////////////////////////////////////

	// Use SearchInLogFile(...) searching for a known string in the log starting at record 0.
	void HoloScriptTest15::executeTest( )
	{		
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		int locationInLog = test->holoApp->SearchInLogFile( S"LoadLibraryExW", 0 );

		unsigned long returned = 0;

		HoloScript::LogItem * item = 
			test->holoApp->GetLogEntryAt( locationInLog, returned );

		if ( item == NULL )
			throw new Exception ( "Attempt to search for item in log retrieved no entries!" );

		TestFramework::Log::Log::printSuccess( "FUNCTION", item->Function );
		TestFramework::Log::Log::printSuccess( "CATEGORY", item->Category );
		TestFramework::Log::Log::printSuccess( "CALLDEPTH", item->CallDepth );
	}

	// Use SearchInLogFile(...) searching for an empty string in the log starting at record 0.
	void HoloScriptTest16::executeTest( )
	{		
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		int locationInLog = test->holoApp->SearchInLogFile( S"", 0 );

		// If there are no exceptions, this test passes since we don't know whether an empty
		// string could be in the log or not. The main purpose here is to see how the method
		// handles this call.
	}

	// Use SearchInLogFile(...) searching for an empty string in the log starting at record -1.
	void HoloScriptTest17::executeTest( )
	{		
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		int locationInLog = test->holoApp->SearchInLogFile( S"", -1 );

		// If there are no exceptions, this test passes since we don't know whether an empty
		// string could be in the log or not. The main purpose here is to see how the method
		// handles this call.
	}

	// Use SearchInLogFile(...) searching for a known string in the log starting at record -1.
	void HoloScriptTest18::executeTest( )
	{		
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		int locationInLog = test->holoApp->SearchInLogFile( S"LoadLibraryExW", -1 );

		// If there are no exceptions, this test passes since we don't know whether an empty
		// string could be in the log or not. The main purpose here is to see how the method
		// handles this call.
	}

	// Use SearchInLogFile(...) searching for an empty string in the log starting at record MAXINT.
	void HoloScriptTest19::executeTest( )
	{		
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		int locationInLog = test->holoApp->SearchInLogFile( S"", 4294967295 );

		// If there are no exceptions, this test passes since we don't know whether an empty
		// string could be in the log or not. The main purpose here is to see how the method
		// handles this call.
	}

	// Use SearchInLogFile(...) searching for a known string in the log starting at record MAXINT.
	void HoloScriptTest20::executeTest( )
	{		
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );

		int locationInLog = test->holoApp->SearchInLogFile( S"LoadLibraryExW", 4294967295 );

		// If there are no exceptions, this test passes since we don't know whether an empty
		// string could be in the log or not. The main purpose here is to see how the method
		// handles this call.
	}

	// Call GetLogEntryAt(...) before interception has started...
	void HoloScriptTest21::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		position = new unsigned long;
		*position = 0;

		logItem = holoApp->GetLogEntryAt( 0, *position );

		// Since no logs are open, we expect NULL...

		if ( logItem != NULL )
			throw new Exception( "Attempt to get log entry without a log open gave back an actual log entry!" );		
	}

	// Call GetLogEntryAt(...) with an index of 0 (after interception )....
	void HoloScriptTest22::executeTest( )
	{	
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );		

		position = new unsigned long;
		*position = 0;

		// We should have at least ONE entry in the log!
		logItem = test->holoApp->GetLogEntryAt( 0, *position );
		
		if ( logItem == NULL )
			throw new Exception( "No entries could be retrieved from log even though entries are known to be there!" );		
	}

	// Call GetLogEntryAt(...) with an index of 1 (after interception )....
	void HoloScriptTest23::executeTest( )
	{	
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );		

		position = new unsigned long;
		*position = 0;

		// We should have at least ONE entry in the log!
		logItem = test->holoApp->GetLogEntryAt( 1, *position );
		
		if ( logItem == NULL )
			throw new Exception( "No entries could be retrieved from log even though entries are known to be there!" );		
	}

	// Call GetLogEntryAt(...) with an index of max int (after interception )....
	void HoloScriptTest24::executeTest( )
	{	
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );		

		position = new unsigned long;
		*position = 0;

		// We should have at least ONE entry in the log!
		logItem = test->holoApp->GetLogEntryAt( 4294967295, *position );
		
		// If no exceptions are thrown, all is well.
	}

	/////////////////////////////////////////////////////////////////////////////
	///// PASS BY REFERENCE ON FILE POSITION MAKES TEST CASE #25 IMPOSSIBLE /////
	/////////////////////////////////////////////////////////////////////////////

	// Call GetLogEntryAtFilePosition(...) after retrieving file position using
	// GetLogEntryAt(...)... in test case 21...
	void HoloScriptTest26A::executeTest( )
	{	
		HoloScriptTest21 * test = new HoloScriptTest21( );

		test->executeTest( );				

		LogItem * logItem2 = test->holoApp->GetLogEntryAtFilePosition( *test->position );

		if ( !( ( logItem2->CallDepth->CompareTo( test->logItem->CallDepth ) == 0 )&&
			( logItem2->Category->CompareTo( test->logItem->Category ) == 0 ) &&
			( logItem2->DllName->CompareTo( test->logItem->DllName ) == 0 ) &&
			( logItem2->ErrorCode->CompareTo( test->logItem->ErrorCode ) == 0 ) &&
			( logItem2->Exception->CompareTo( test->logItem->Exception ) == 0 ) &&
			( logItem2->FilePosition == test->logItem->FilePosition ) &&
			( logItem2->Function->CompareTo( test->logItem->Function ) == 0 ) &&
			( logItem2->ThreadID->CompareTo( test->logItem->ThreadID ) == 0 ) &&
			( logItem2->TimeStamp->CompareTo( test->logItem->TimeStamp ) == 0 ) ) )
		{
			throw new Exception( "The two log items obtained are not a match to each other." );
		}		
		
		// If no exceptions are thrown, all is well.
	}

	// Call GetLogEntryAtFilePosition(...) after retrieving file position using
	// GetLogEntryAt(...)... in test case 21...
	void HoloScriptTest26B::executeTest( )
	{	
		HoloScriptTest22 * test = new HoloScriptTest22( );

		test->executeTest( );				

		LogItem * logItem2 = test->holoApp->GetLogEntryAtFilePosition( *test->position );

		if ( !( ( logItem2->CallDepth->CompareTo( test->logItem->CallDepth ) == 0 )&&
			( logItem2->Category->CompareTo( test->logItem->Category ) == 0 ) &&
			( logItem2->DllName->CompareTo( test->logItem->DllName ) == 0 ) &&
			( logItem2->ErrorCode->CompareTo( test->logItem->ErrorCode ) == 0 ) &&
			( logItem2->Exception->CompareTo( test->logItem->Exception ) == 0 ) &&
			( logItem2->FilePosition == test->logItem->FilePosition ) &&
			( logItem2->Function->CompareTo( test->logItem->Function ) == 0 ) &&
			( logItem2->ThreadID->CompareTo( test->logItem->ThreadID ) == 0 ) &&
			( logItem2->TimeStamp->CompareTo( test->logItem->TimeStamp ) == 0 ) ) )
		{
			throw new Exception( "The two log items obtained are not a match to each other." );
		}		
		
		// If no exceptions are thrown, all is well.
	}

	// Call GetLogEntryAtFilePosition(...) after retrieving file position using
	// GetLogEntryAt(...)... in test case 21...
	void HoloScriptTest26C::executeTest( )
	{	
		HoloScriptTest23 * test = new HoloScriptTest23( );

		test->executeTest( );				

		LogItem * logItem2 = test->holoApp->GetLogEntryAtFilePosition( *test->position );

		if ( !( ( logItem2->CallDepth->CompareTo( test->logItem->CallDepth ) == 0 )&&
			( logItem2->Category->CompareTo( test->logItem->Category ) == 0 ) &&
			( logItem2->DllName->CompareTo( test->logItem->DllName ) == 0 ) &&
			( logItem2->ErrorCode->CompareTo( test->logItem->ErrorCode ) == 0 ) &&
			( logItem2->Exception->CompareTo( test->logItem->Exception ) == 0 ) &&
			( logItem2->FilePosition == test->logItem->FilePosition ) &&
			( logItem2->Function->CompareTo( test->logItem->Function ) == 0 ) &&
			( logItem2->ThreadID->CompareTo( test->logItem->ThreadID ) == 0 ) &&
			( logItem2->TimeStamp->CompareTo( test->logItem->TimeStamp ) == 0 ) ) )
		{
			throw new Exception( "The two log items obtained are not a match to each other." );
		}		
		
		// If no exceptions are thrown, all is well.
	}

		// Call GetLogEntryAtFilePosition(...) after retrieving file position using
	// GetLogEntryAt(...)... in test case 21...
	void HoloScriptTest26D::executeTest( )
	{	
		HoloScriptTest24 * test = new HoloScriptTest24( );

		test->executeTest( );				

		LogItem * logItem2 = test->holoApp->GetLogEntryAtFilePosition( *test->position );

		if ( !( ( logItem2->CallDepth->CompareTo( test->logItem->CallDepth ) == 0 )&&
			( logItem2->Category->CompareTo( test->logItem->Category ) == 0 ) &&
			( logItem2->DllName->CompareTo( test->logItem->DllName ) == 0 ) &&
			( logItem2->ErrorCode->CompareTo( test->logItem->ErrorCode ) == 0 ) &&
			( logItem2->Exception->CompareTo( test->logItem->Exception ) == 0 ) &&
			( logItem2->FilePosition == test->logItem->FilePosition ) &&
			( logItem2->Function->CompareTo( test->logItem->Function ) == 0 ) &&
			( logItem2->ThreadID->CompareTo( test->logItem->ThreadID ) == 0 ) &&
			( logItem2->TimeStamp->CompareTo( test->logItem->TimeStamp ) == 0 ) ) )
		{
			throw new Exception( "The two log items obtained are not a match to each other." );
		}		
		
		// If no exceptions are thrown, all is well.
	}

	void HoloScriptTest27::executeTest( )
	{	
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );				

		LogItem * logItem2 = test->holoApp->GetLogEntryAtFilePosition( 0 );
		
		
		// If no exceptions are thrown, all is well.
	}

	void HoloScriptTest28::executeTest( )
	{	
		HoloScriptTest1 * test = new HoloScriptTest1( );

		test->executeTest( );				

		LogItem * logItem2 = test->holoApp->GetLogEntryAtFilePosition( 1 );
		
		
		// If no exceptions are thrown, all is well.
	}

	void HoloScriptTest29::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );	

		LogItem * logItem2 = holoApp->GetLogEntryAtFilePosition( 0xFFFFFFFF );
		
		
		// If no exceptions are thrown, all is well.
	}

	void HoloScriptTest44::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );
		
		// Pause
		if ( !holoApp->PauseApplication( ) )
			throw new Exception( "Error pausing the application!" );

		// Resume
		if ( !holoApp->ResumeApplication( ) )
			throw new Exception( "Error resuming the application!" );		
				
		// App should still be running without any suspended threads...
	}

	void HoloScriptTest45::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		// Pause
		if ( !holoApp->PauseApplication( ) )
			throw new Exception( "Error pausing the application!" );

		if ( holoApp->PauseApplication( ) )
			throw new Exception( "Error! Should not have been able to pause app twice with success!" );

		// Resume
		if ( !holoApp->ResumeApplication( ) )
			throw new Exception( "Error resuming the application!" );
				
		// App should still be running without any suspended threads...
	}

	void HoloScriptTest47::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		if ( !holoApp->TerminateApplication( ) )
			throw new Exception( "Could not terminate application!" );

		// App should still be running without any suspended threads...
	}

	void HoloScriptTest48::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		// Pause the app...
		holoApp->PauseApplication( );

		if ( !holoApp->TerminateApplication( ) )
			throw new Exception( "Could not terminate application!" );

		// App should still be running without any suspended threads...
	}

	void HoloScriptTest49::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp->get_ApplicationName( );
		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest50::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		TestFramework::Log::Log::printSuccess( "Application Name", holoApp->get_ApplicationName( ) );

		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest52::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		TestFramework::Log::Log::printSuccess( "Application Name", holoApp->get_ApplicationPath( ) );

		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest55::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		holoApp->set_ApplicationName( NULL );

		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest58::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		if ( holoApp->get_IsPaused( ) )
			throw new Exception( "App should not come back paused if not intercepted" );
		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest59::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		holoApp->PauseApplication( );

		if ( !holoApp->get_IsPaused( ) )
			throw new Exception( "App was paused, but status does not say this." );
		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest66::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		unsigned int procID = 0;
		if ( !holoApp->GetApplicationProcessID( procID ) )
			throw new Exception( "Error retrieving application proc id!" );

		TestFramework::Log::Log::printSuccess( "Process ID", procID.ToString( ) );		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest68::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		unsigned int tID = 0;
		if ( !holoApp->GetApplicationThreadID( tID ) )
			throw new Exception( "Error retrieving application thread id!" );

		TestFramework::Log::Log::printSuccess( "Thread ID", tID.ToString( ) );		

		// If we get here without exceptions, all is well...
	}

	void HoloScriptTest70::executeTest( )
	{	
		holoApp = new HoloScriptApplication( );

		holoApp = startAppRunning( );

		unsigned __int64 maxDiskUsage = 0;
		unsigned __int64 currentDiskUsage = 0;
		unsigned __int64 maximumMemoryUsage = 0;
		unsigned __int64 currentMemoryUsage = 0;

		if ( !holoApp->GetMonitorInformation( maxDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage ) )
			throw new Exception( "Error getting monitor information!" );

		TestFramework::Log::Log::printSuccess( "Max Disk", maxDiskUsage.ToString( ) );
		TestFramework::Log::Log::printSuccess( "Current Disk", currentDiskUsage.ToString( ) );
		TestFramework::Log::Log::printSuccess( "Max Memory", maximumMemoryUsage.ToString( ) );
		TestFramework::Log::Log::printSuccess( "Current Memory", currentMemoryUsage.ToString( ) );
		// If we get here without exceptions, all is well...
	}
}
