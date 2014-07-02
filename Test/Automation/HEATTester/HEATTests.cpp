#include "stdafx.h"
#include "CHeatApp.h"
#include "HEATTester.h"
#include "HEATTests.h"

using namespace System::IO;
using namespace System::Collections;

// So we can use .NET version...
#undef GetCurrentDirectory

/// REFER TO HEAT TEST PLAN FOR DETAILS ON TEST CASE INFORMATION

namespace HEATTests
{			
	// Test case 1...
	void HEATTest1::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );

		DWORD dwProcID = testApp->getProcessId( );

		addProcessToList( (int)dwProcID );

		DWORD result = testApp->forceInject(  );

		if ( result != HEAT_SUCCESS )
			throw new Exception( "Error attaching to application." );
	
		if ( !checkHEATDLLInProcess( dwProcID ) != HEAT_SUCCESS )
			throw new Exception( "The HEAT4DLL.DLL module was not found as part of the process space." );			
	}

	// Test case 2...
	void HEATTest2::executeTest( )
	{		
		// Execute test case 1...
		HEATTest1 * test = new HEATTest1( );
		test->executeTest( );
		
		DWORD dwResult = test->testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
			throw new Exception( "Failed to intercept a function!" );

		addProcessToList( (int)test->testApp->getProcessId( ) );

		// Run the app...
		test->testApp->runApp( );

		if ( isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was not intercepted!" );

		TestFramework::Log::Log::printSuccess( "DEBUG PROCESS",  test->testApp->getProcessId( ).ToString( ) );				

		// Now perform additional
	}

	// Test case 2...
	void HEATTest3::executeTest( )
	{		
		// Execute test case 1...
		HEATTest1 * test = new HEATTest1( );
		test->executeTest( );
		
		// Reassign pointers for reuse...
		testApp = test->testApp;

		DWORD dwResult = test->testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );			

		dwResult += test->testApp->interceptFunc( "WriteFile", 
			"KERNEL32.DLL", 
			"My_WriteFile",
			getReplacementLibraryFullPath( ) );			
	    

		if ( dwResult != HEAT_SUCCESS )
			throw new Exception( "Failed to intercept a function!" );

		addProcessToList( (int)test->testApp->getProcessId( ) );

		// Run the app...
		test->testApp->runApp( );

		// Ensure that functions are intercepted by disassembling them on the AUT end and checking
		// for the JMP automagically.
		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was not intercepted!" );

		if ( !isFunctionIntercepted( "WriteFile", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "WriteFile was not intercepted!" );	

		TestFramework::Log::Log::printSuccess( "DEBUG PROCESS",  test->testApp->getProcessId( ).ToString( ) );

		// Now perform additional
	}

	// Test case 1...
	void HEATTest5::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );

		DWORD dwProcID = testApp->getProcessId( );

		addProcessToList( (int)dwProcID );

		DWORD result = testApp->forceInject(  );

		// Get the address pointer to the function...
		void * fPtr = GetProcAddress( GetModuleHandle( "KERNEL32.DLL" ), "CreateFileA" );

		// Now perform interception on that pointer...
		if ( testApp->interceptFunc( fPtr, 
			"My_WriteFile",
			getReplacementLibraryFullPath( ) ) != HEAT_SUCCESS )
			throw new Exception( "Failed to intercept CreateFileA using function pointer on interceptFunc." );

		testApp->runApp( );

		TestFramework::Log::Log::printSuccess( "DEBUG PROCESS",  testApp->getProcessId( ).ToString( ) );		

		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was not intercepted!" );

		if ( result != HEAT_SUCCESS )
			throw new Exception( "Error attaching to application." );
	
		if ( !checkHEATDLLInProcess( dwProcID ) != HEAT_SUCCESS )
			throw new Exception( "The HEAT4DLL.DLL module was not found as part of the process space." );			
	}

	// Test case 9...
	void HEATTest9::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
		
		if ( test->testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" ) != HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc failed!" );

		if ( isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was unintercepted, but is still showing up as being intercepted." );		
		
	}

	void HEATTest10::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
		
		if ( test->testApp->unInterceptFunc( "CreateFileA", NULL ) == HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc should have failed, but didn't" );

		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA should still be intercepted since uninterceptFunc(...) failed." );		
		
	}
	
	void HEATTest12::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
		
		if ( test->testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" ) != HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc failed!" );

		if ( test->testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" ) == HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc should have feailed since it is already disabled!" );

		if ( isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was unintercepted, but is still showing up as being intercepted." );		
		
	}

	void HEATTest13::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
		
		if ( test->testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" ) != HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc failed!" );

		if ( isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was unintercepted, but is still showing up as being intercepted." );		

		DWORD dwResult = test->testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to re-intercept CreateFileA failed" );
		}

		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was re-intercepted, but does not appear to be intercepted." );						
	}

	void HEATTest14::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
		
		if ( test->testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" ) != HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc failed!" );

		if ( !test->testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" ) != HEAT_SUCCESS )
			throw new Exception( "Call to uninterceptFunc succeeded even though we called it previously!" );

		if ( isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was unintercepted, but is still showing up as being intercepted." );		

		DWORD dwResult = test->testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to re-intercept CreateFileA failed" );
		}

		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was re-intercepted, but does not appear to be intercepted." );						
	}

	void HEATTest15::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
			
		DWORD dwResult = test->testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult == HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to re-intercept CreateFileA should have failed" );
		}

		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was re-intercepted, but does not appear to be intercepted." );						
	}

	void HEATTest16::executeTest( )
	{		
		HEATTest3 * test = new HEATTest3( );
		test->executeTest( );
			
		DWORD dwResult = test->testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );	

		dwResult += test->testApp->interceptFunc( "CreateFileA", 
			"HEATInterceptable.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );	

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept a second createfile failed!" );
		}

		if ( !isFunctionIntercepted( "CreateFileA", "KERNEL32.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was re-intercepted, but does not appear to be intercepted." );						

		if ( !isFunctionIntercepted( "CreateFileA", "HEATInterceptable.DLL", test->testApp->getProcessId( ) ) )
			throw new Exception( "CreateFileA was re-intercepted, but does not appear to be intercepted." );						
	}

	void HEATTest18::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		DWORD dwResult = testApp->interceptFunc( "CreateFileA", 
			"KERNEL32.DLL", 
			"My_CreateFileA",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult == HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept CreateFileA should have failed" );
		}		
	}

	void HEATTest19::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		// Of course, this function does not exist (not intercepted)...
		DWORD dwResult = testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" );

		if ( dwResult == HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept CreateFileA should have failed" );
		}		
	}

	void HEATTest33::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->isReady( ) )
			throw new Exception( "The application should not be ready as it has not been initialized!" );
	}

	void HEATTest34::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( !testApp->isReady( ) )
			throw new Exception( "The application should be ready, but is not." );
		
	}

	void HEATTest36::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->deinitializeApp( ) != HEAT_SUCCESS )
			throw new Exception( "Error de-initializing the application!" );

		if ( testApp->isReady( ) )
			throw new Exception( "The application should NOT be ready, but is." );
		
	}

	void HEATTest37::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->detachFromApp( ) != HEAT_SUCCESS )
			throw new Exception( "Error detaching from the application!" );

		if ( testApp->isReady( ) )
			throw new Exception( "The application should NOT be ready, but is." );
		
	}

	void HEATTest38::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		PROCESS_INFORMATION pi;

		// Build up invalid PI info.
		pi.dwProcessId = 0xFFFFFFFF;
		pi.dwThreadId = 0xFFFFFFFF;
		pi.hProcess = NULL;
		pi.hThread = NULL;

		if ( testApp->attachToApp( pi ) == HEAT_SUCCESS )
			throw new Exception( "Attachment should have failed but didn't." );		
		
	}

	void HEATTest39::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->attachToApp( 0xFFFFFFFF ) == HEAT_SUCCESS )
			throw new Exception( "Attachment should have failed but didn't." );		
		
	}

	void HEATTest40::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		PROCESS_INFORMATION pi;

		// Build up invalid PI info.
		pi.dwProcessId = 0xFFFFFFFF;
		pi.dwThreadId = 0xFFFFFFFF;
		pi.hProcess = NULL;
		pi.hThread = NULL;

		if ( testApp->attachToRunningApp( pi ) == HEAT_SUCCESS )
			throw new Exception( "Attachment should have failed but didn't." );		
		
	}

	void HEATTest41::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->attachToRunningApp( 0xFFFFFFFF ) == HEAT_SUCCESS )
			throw new Exception( "Attachment should have failed but didn't." );		
		
	}

	void HEATTest42::executeTest( )
	{	
		System::Diagnostics::Process * mainProc =
			new System::Diagnostics::Process( );

		System::String * fullAppPath = testApplicationPath;

		fullAppPath = fullAppPath->Concat( S"\\" );
		fullAppPath = fullAppPath->Concat( testApplication );

		// Setup the process and start it...
		mainProc->StartInfo->FileName = fullAppPath;
		mainProc->Start( );

		addProcessToList( mainProc->Id );

		testApp = new CHeatApp( );
		
		if ( testApp->attachToRunningApp( mainProc->Id ) != HEAT_SUCCESS )
			throw new Exception( "Attachment should have succeeded but didn't." );		

		if ( testApp->attachToRunningApp( mainProc->Id ) == HEAT_SUCCESS )
			throw new Exception( "Trying to attach a second time succeeded, but shouldn't have." );		
	}

	void HEATTest44::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->forceInject( ) == HEAT_SUCCESS )
			throw new Exception( "Injection should have failed but didn't." );		
		
	}

	void HEATTest45::executeTest( )
	{	
		System::Diagnostics::Process * mainProc =
			new System::Diagnostics::Process( );

		System::String * fullAppPath = testApplicationPath;

		fullAppPath = fullAppPath->Concat( S"\\" );
		fullAppPath = fullAppPath->Concat( testApplication );

		// Setup the process and start it...
		mainProc->StartInfo->FileName = fullAppPath;
		mainProc->Start( );

		addProcessToList( mainProc->Id );

		testApp = new CHeatApp( );
		
		if ( testApp->attachToRunningApp( mainProc->Id ) != HEAT_SUCCESS )
			throw new Exception( "Attachment should have succeeded but didn't." );		

		if ( testApp->forceInject( ) != HEAT_SUCCESS )
			throw new Exception( "Trying to inject after attaching should have succeeded but didn't." );		
	}

	void HEATTest46::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->delayedInject( ) == HEAT_SUCCESS )
			throw new Exception( "Injection should have failed but didn't." );		
		
	}

	void HEATTest47::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->delayedInject( ) != HEAT_SUCCESS )
			throw new Exception( "Delayed Injection of application failed." );
	}	

	void HEATTest48::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->delayedInject( ) != HEAT_SUCCESS )
			throw new Exception( "Delayed Injection of application failed." );

		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) == HEAT_SUCCESS )		
			throw new Exception( "Initiailization should have failed." );						
	}	

	void HEATTest49::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->delayedInject( ) != HEAT_SUCCESS )
			throw new Exception( "Delayed Injection of application failed." );

		if ( testApp->attachToApp( testApp->getProcessId( ) ) == HEAT_SUCCESS )
			throw new Exception( "Attachment should have failed." );						
	}

	void HEATTest50::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->runApp( ) == HEAT_SUCCESS )
			throw new Exception( "RunApp should have failed because app is already running!" );
	}

	void HEATTest51::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );
	}

	void HEATTest52::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );

		if ( testApp->pauseApp( ) == HEAT_SUCCESS )
			throw new Exception( "PauseApp should have failed!" );

		if ( testApp->resumeApp( ) != HEAT_SUCCESS )
			throw new Exception( "ResumeApp failed!" );
	}

	void HEATTest53::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->resumeApp( ) == HEAT_SUCCESS )
			throw new Exception( "ResumeApp should have failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );
	}

	void HEATTest54::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );

		if ( testApp->resumeApp( ) != HEAT_SUCCESS )
			throw new Exception( "ResumeApp failed!" );

		if ( testApp->resumeApp( ) == HEAT_SUCCESS )
			throw new Exception( "ResumeApp should have failed!" );	
	}

	void HEATTest55::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->getProcessId( ) == HEAT_SUCCESS )
			throw new Exception( "Process ID should not be obtainable since process is not attached or running!" );
	}

	void HEATTest56::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->getThreadId( ) == HEAT_SUCCESS )
			throw new Exception( "Thread ID should not be obtainable since process is not attached or running!" );
	}

	void HEATTest57::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		DWORD * threadList = new DWORD[50];

		if ( testApp->getThreadsInCurrentProcess( threadList ) == HEAT_SUCCESS )
			throw new Exception( "Thread ID should not be obtainable since process is not attached or running!" );
	}

	void HEATTest58::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );

		DWORD dwThreadId = testApp->getThreadId( );

		// Verify the thread is valid...
		if ( !isThreadInProcessSpace( (int)dwThreadId, testApp->getProcessId( ) ) )
			throw new Exception( "The thread ID retrieved was invalid." );
	}

	void HEATTest59::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );

		DWORD * dwThreads = new DWORD[50];

		DWORD dwThreadId = testApp->getThreadId( );

		// Get the threads and the thread count...
		DWORD dwThreadCount = testApp->getThreadsInCurrentProcess( dwThreads );

		// Cycle through all the threads and verify they are valid...
		for ( DWORD dwCounter = 0; dwCounter < dwThreadCount; dwCounter++ )
		{
			// Verify the thread is valid...
			if ( !isThreadInProcessSpace( (int)dwThreads[dwCounter], testApp->getProcessId( ) ) )
				throw new Exception( "The thread ID retrieved was invalid." );
		}
	}

	void HEATTest61::executeTest( )
	{	
		testApp = new CHeatApp( );

		if ( testApp->isRunning( ) == true )
			throw new Exception( "IsRunning should have failed." );
	}

	void HEATTest62::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->pauseApp( ) != HEAT_SUCCESS )
			throw new Exception( "PauseApp failed!" );

		if ( testApp->isRunning( ) != false )
			throw new Exception( "App showed up as running when paused." );
		
	}

	void HEATTest63::executeTest( )
	{	
		testApp = new CHeatApp( );

		if ( testApp->terminateApp( ) == HEAT_SUCCESS )
			throw new Exception( "terminateApp should have failed." );
	}

	void HEATTest64::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->terminateApp( ) != HEAT_SUCCESS )
			throw new Exception( "terminateApp Failed!" );

		if ( testApp->isRunning( ) != false )
			throw new Exception( "App showed up as running when terminated." );
		
	}

	void HEATTest65::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->terminateApp( ) != HEAT_SUCCESS )
			throw new Exception( "terminateApp Failed!" );

		if ( testApp->runApp( ) == HEAT_SUCCESS )
			throw new Exception( "App should not allow running after termination." );
		
	}

	void HEATTest66::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->runApp( ) != HEAT_SUCCESS )
			throw new Exception( "RunApp failed!" );

		if ( testApp->runApp( ) == HEAT_SUCCESS )
			throw new Exception( "App should not allow running after a previous runApp call." );
		
	}

	void HEATTest67::executeTest( )
	{	
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );				
		
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		if ( testApp->terminateApp( ) != HEAT_SUCCESS )
			throw new Exception( "TerminateApp failed!" );

		if ( testApp->terminateApp( ) == HEAT_SUCCESS )
			throw new Exception( "TerminateApp should not succeed after previous terminateApp call" );
		
	}

	void HEATTest68::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		DWORD dwResult = testApp->interceptFunc( "TlsGetValue", 
			"KERNEL32.DLL", 
			"My_TlsGetValue",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept TlsGetValue failed!" );
		}	
		
		// Run the app... see if it crashes...
		testApp->runApp( );

		if ( !isFunctionIntercepted( "TlsGetValue", "KERNEL32.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "TlsGetValue was not intercepted!" );
	}

	void HEATTest69::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		DWORD dwResult = testApp->interceptFunc( "TlsSetValue", 
			"KERNEL32.DLL", 
			"My_TlsSetValue",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept TlsSetValue failed!" );
		}	
		
		// Run the app... see if it crashes...
		testApp->runApp( );

		if ( !isFunctionIntercepted( "TlsSetValue", "KERNEL32.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "TlsSetValue was not intercepted!" );
	}

	void HEATTest70::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		DWORD dwResult = testApp->interceptFunc( "GetLastError", 
			"KERNEL32.DLL", 
			"My_GetLastError",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept GetLastError failed!" );
		}	
		
		// Run the app... see if it crashes...
		testApp->runApp( );

		if ( !isFunctionIntercepted( "GetLastError", "KERNEL32.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "GetLastError was not intercepted!" );
	}

	void HEATTest71::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		DWORD dwResult = testApp->interceptFunc( "SetLastError", 
			"KERNEL32.DLL", 
			"My_SetLastError",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept SetLastError failed!" );
		}	
		
		// Run the app... see if it crashes...
		testApp->runApp( );

		if ( !isFunctionIntercepted( "SetLastError", "KERNEL32.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "SetLastError was not intercepted!" );
	}

	void HEATTest72::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		DWORD dwResult = testApp->interceptFunc( "NtOpenFile", 
			"NTDLL.DLL", 
			"My_NtOpenFile",
			getReplacementLibraryFullPath( ) );		

		dwResult += testApp->interceptFunc( "ZwOpenFile", 
			"NTDLL.DLL", 
			"My_ZwOpenFile",
			getReplacementLibraryFullPath( ) );			

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "Attempt to intercept SetLastError failed!" );
		}	
		
		// Run the app... see if it crashes...
		testApp->runApp( );

		if ( !isFunctionIntercepted( "NtOpenFile", "NTDLL.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "NtOpenFile was not intercepted!" );

		if ( !isFunctionIntercepted( "ZwOpenFile", "NTDLL.DLL", testApp->getProcessId( ) ) )
			throw new Exception( "ZwOpenFile was not intercepted!" );
	}

	void HEATTest73::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		if ( testApp->forceInject( ) == HEAT_SUCCESS )
			throw new Exception( "Force inject should have failed." );
	}

	void HEATTest74::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->delayedInject( );

		if ( testApp->delayedInject( ) == HEAT_SUCCESS )
			throw new Exception( "Delayed inject should have failed." );
	}

	void HEATTest75::executeTest( )
	{		
		testApp = new CHeatApp( );
		
		if ( testApp->initializeApp( convertToManaged( HEATTest::testApplicationPath ), convertToManaged( HEATTest::testApplication ) ) != HEAT_SUCCESS )		
			throw new Exception( "Error initializing application for interception." );		
			
		TestFramework::Log::Log::printSuccess( "Process", testApp->getProcessId( ).ToString( ) );

		testApp->forceInject( );

		DWORD dwResult = HEAT_SUCCESS;

		// Get process classes representing this and the intercepted proc...
		System::Diagnostics::Process * thisProcess =
			System::Diagnostics::Process::GetCurrentProcess( );

		System::Diagnostics::Process * interceptedProcess =
			System::Diagnostics::Process::GetProcessById( testApp->getProcessId( ) );

		// Check the memory usage initially...

		TestFramework::Log::Log::printTestStatus( "<<< MEMORY BEFORE INTERCEPT/UNINTERCEPT CALLS >>" );
		TestFramework::Log::Log::printSuccess( "API PHYS MEM", thisProcess->WorkingSet.ToString( ) );
		TestFramework::Log::Log::printSuccess( "API VIRT MEM", thisProcess->VirtualMemorySize.ToString( ) );

		TestFramework::Log::Log::printSuccess( "AUT PHYS MEM", interceptedProcess->WorkingSet.ToString( ) );
		TestFramework::Log::Log::printSuccess( "AUT VIRT MEM", interceptedProcess->VirtualMemorySize.ToString( ) );

		TestFramework::Log::Log::printTestStatus( "PERFORMING INTERCEPTION AND DISINTERCEPTION... PLEASE WAIT..." );
		// Perform interception and uninterception multiple times to check
		// for memory leaks...
		for ( int numTimes = 0; numTimes < 100000; numTimes++ )
		{
			dwResult = testApp->interceptFunc( "CreateFileA", 
				"KERNEL32.DLL", 
				"My_CreateFileA",
				getReplacementLibraryFullPath( ) );			

			dwResult += testApp->unInterceptFunc( "CreateFileA", "KERNEL32.DLL" );
		}

		TestFramework::Log::Log::printTestStatus( "<<< MEMORY AFTER INTERCEPT/UNINTERCEPT CALLS >>" );
		TestFramework::Log::Log::printSuccess( "API PHYS MEM", thisProcess->WorkingSet.ToString( ) );
		TestFramework::Log::Log::printSuccess( "API VIRT MEM", thisProcess->VirtualMemorySize.ToString( ) );

		TestFramework::Log::Log::printSuccess( "AUT PHYS MEM", interceptedProcess->WorkingSet.ToString( ) );
		TestFramework::Log::Log::printSuccess( "AUT VIRT MEM", interceptedProcess->VirtualMemorySize.ToString( ) );

		if ( dwResult != HEAT_SUCCESS )
		{
			throw new Exception( "CreateFileA interception failed at some point..." );
		}		
	}
}
