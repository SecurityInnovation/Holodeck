#pragma once
#include <windows.h>

#using <mscorlib.dll>

using namespace System;
using namespace System::Reflection;
using namespace TestFramework;
using namespace TestFramework::Exceptions;
using namespace TestFramework::Run;
using namespace TestFramework::Utils;
using namespace TestFramework::Log;

namespace HEATTestMain
{
	/// <summary>
	/// Summary description for LimitTest.
	/// </summary>
	__gc class HEATTest : public RunTest
	{
		// Define the prefix for the name of the class we will load.
	public:
		static System::Collections::ArrayList * processList;
		static String * classNamePrefix = "HEATTests.HEATTest";
		static String * testApplication = "Notepad.exe";
		static String * testApplicationPath = "C:\\Windows";
		Object * getTestClass( String * testID );		
		void performPostRunTasks( );
		void addProcessToList( int processID );
		static char * convertToManaged( System::String __gc * stringInput );
		static bool checkHEATDLLInProcess( int processID );
		static char * getCurrentExecutionPath( );
		static char * getReplacementLibraryFullPath( );
		static bool isFunctionIntercepted( char * functionName, char * module, DWORD dwProcess );
		static bool isThreadInProcessSpace( int threadID, int processID );
		
		HEATTest( )
		{
			// Setup an arraylist to contain the list of all process that should be terminated
			// once test(s) are completed...
	
			processList = new System::Collections::ArrayList( );
		}
	};
}