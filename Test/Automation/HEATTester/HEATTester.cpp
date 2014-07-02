// This is the main project file for VC++ application project 
// generated using an Application Wizard.

#include "stdafx.h"
#include "HEATTester.h"

namespace HEATTestMain
{		
	// Handles tasks that need to be performed after tasks have finished. This, for us,
	// just involves terminating any AUTs that were previously opened...
	void HEATTest::performPostRunTasks( )
	{
		System::Diagnostics::Process * procToKill = NULL;

		for ( int procCount = 0; procCount < processList->Count; procCount++ )
		{
			// Get an instance of the process object...
			procToKill = System::Diagnostics::Process::GetProcessById( *( dynamic_cast<System::Int32 *>( processList->get_Item( procCount ) ) ) );

			// Kill the process...
			procToKill->Kill( );
		}
	}

	void HEATTest::addProcessToList( int processID )
	{
		System::Object * gcProcID = __box( processID );	

		this->processList->Add( gcProcID );
	}

	// Determines if the specified thread ID exists in the process space...
	bool HEATTest::isThreadInProcessSpace( int threadID, int processID )
	{
		System::Diagnostics::Process * mainProc = 
			System::Diagnostics::Process::GetProcessById( processID );

		for ( int threadCount = 0; threadCount < mainProc->Threads->Count; threadCount++ )
		{
			if ( mainProc->Threads->get_Item( threadCount )->Id == threadID )
				return true;
		}

		return false;
	}

	Object * HEATTest::getTestClass( String * testID )
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

	// Converts a managed gc string to an unmanaged nogc character array...
	char * HEATTest::convertToManaged( System::String __gc * stringInput )
	{       
		return (char*)( System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi( stringInput->ToString( ) ) ).ToPointer( );
	}

	// Checks to ensure that HEAT4DLL.dll is part of the process space...
	bool HEATTest::checkHEATDLLInProcess( int processID )
	{
		// Get the process class instance...
		System::Diagnostics::Process * process = 
			System::Diagnostics::Process::GetProcessById( processID );

		// Get all the modules currently part of the process space...
		System::Diagnostics::ProcessModuleCollection * modules = 
			process->get_Modules( );

		// Cycle through all the modules...
		for ( int modCount = 0; modCount < modules->Count; modCount++ )
		{
			// If the module name is HEAT4DLL.DLL, return true, else keep going...
			if ( modules->get_Item( modCount )->ModuleName->ToUpper( )->CompareTo( "HEAT4DLL.DLL" ) == 0 )
			{
				return true;
			}
		}

		// If we get here, the HEAT4DLL.DLL module was never found...
		return false;
	}

	// Returns the path to the location where the HEAT Tester is running...
	char * HEATTest::getCurrentExecutionPath( )
	{
		System::Reflection::Assembly * assembly = System::Reflection::Assembly::GetExecutingAssembly( );

		System::IO::FileInfo * fileInfo = new System::IO::FileInfo( assembly->get_Location( ) );

		// Return the location of currently executing assembly...
		return convertToManaged( fileInfo->DirectoryName );
	}

	char * HEATTest::getReplacementLibraryFullPath( )
	{
		char * interceptDLLPath = new char[MAX_PATH];
		char * currentExecutionPath = getCurrentExecutionPath( );
		strcpy( interceptDLLPath, currentExecutionPath );
		strcat( interceptDLLPath, "\\HeatTesterIntercepts.DLL" );

		return interceptDLLPath;
	}

	// Determines if a function has been intercepted by disassembling it at the application end...
	bool HEATTest::isFunctionIntercepted( char * functionName, char * module, DWORD dwProcess )
	{
		// Sleep for a small time just to give Notepad a chance to start up...
		System::Threading::Thread::Sleep( 1000 );

		// We must first build up the pipe name...
		char * pipeName = new char[256];
		char * eventName = new char[256];
		char * eventName2 = new char[256];
		char * processID = new char[10];

		// Convert the process ID to a string...
		ltoa( dwProcess, processID, 10 );

		// Setup the eventname...
		strcpy( eventName, processID );
		strcat( eventName, "RETRIEVE" );

		// Build up the name further...
		strcpy( pipeName, "\\\\.\\pipe\\" );
		strcat( pipeName, processID );
		strcat( pipeName, "RETRIEVEPIPE" );

		strcpy( eventName2, eventName );
		strcat( eventName2, "2" );

		// Open the event...
		HANDLE hEvent = OpenEvent( EVENT_ALL_ACCESS,
			NULL,
			eventName );

		if ( hEvent == INVALID_HANDLE_VALUE )
			throw new Exception( "Could not open event from intercepted app!" );

		// Open the event...
		HANDLE hEvent2 = OpenEvent( EVENT_ALL_ACCESS,
			NULL,
			eventName2 );

		if ( hEvent == INVALID_HANDLE_VALUE )
			throw new Exception( "Could not open event from intercepted app!" );

		// Reset the event (ensure it is ready)...
		ResetEvent( hEvent );

		// We communicate with a pipe server which handles this request for us at the
		// client end. We can't directly open the process here with READ access...
		HANDLE hFile = CreateFile(
			pipeName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL );

		if ( hFile == INVALID_HANDLE_VALUE )
			throw new Exception( "Could not open a named pipe connection!" );

		DWORD dwState = PIPE_READMODE_MESSAGE;

		// Set the state of the pipe to message mode...
		if ( !SetNamedPipeHandleState( hFile,
									&dwState,
									NULL,
									NULL ) )
			throw new Exception( "Could not set named pipe state!" );

		// Send a message to the pipe asking whether the function is currently intercepted
		// or not...

		// First, build the message...
		char * sendMessage = new char[256];
		strcpy( sendMessage, functionName );
		strcat( sendMessage, ";" );
		strcat( sendMessage, module );

		DWORD dwBytes = 0;

		DWORD dwError = 0;

		if ( !WriteFile( hFile,
						sendMessage,
						255,
						&dwBytes,
						NULL ) )
		{
			dwError = GetLastError( );

			throw new Exception( "Pipe was opened, but the message could not be sent." );
		}		

		// Now, read the response after waiting for the server to send it...
		WaitForSingleObject( hEvent, INFINITE );

		ResetEvent( hEvent );

		char * receivedMessage = new char[256];

		if ( !ReadFile( hFile,
						receivedMessage,
						255,
						&dwBytes,
						NULL ) )
		{
			dwError = GetLastError( );

			throw new Exception( "Pipe was opened, message was sent, but there was no response." );
		}
		
		// When done, tell the server to close the pipe and wait for another request...
		SetEvent( hEvent2 );

		// Check the message to see if it contains a 1 or 0 ( 1= TRUE, 0 = FALSE)
		if ( atol( receivedMessage ) == 1 )
			return true;
		else
			return false;		
		
	}	
}

/// <summary>
/// The main entry point for the application.
/// </summary>
int main( int argCount, char * args[] )
{
	try
	{
		HEATTestMain::HEATTest * HEATTest = new HEATTestMain::HEATTest( );
    
		// Need to build a managed string array...
		String * stringArray[] = new String * [argCount -1];

		stringArray->Clear( stringArray, 0, stringArray->Count );
		
		for ( int argCounter = 1; argCounter < argCount; argCounter++ )
		{
			stringArray[argCounter - 1] =  new String( args[argCounter] );
		}

		// parse any command line arguments...
		HEATTest->parseCommandLineArgs( stringArray );

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

