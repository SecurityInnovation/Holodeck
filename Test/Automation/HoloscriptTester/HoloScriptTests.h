#include "stdafx.h"
#include "HoloScriptTester.h"

using namespace HoloScript;
using namespace System::Collections;

namespace HoloScriptTests
{	
	void waitForAppToExit( HoloScriptApplication * holoApp )
	{
		UInt32 __gc * appProcessID = new UInt32;
		
		holoApp->GetApplicationProcessID( *appProcessID );

		TestFramework::Log::Log::printSuccess( "Waiting for Application to Exit to Stop Interception." );
		
		// Wait for notepad to exit before continuing...		
		System::Diagnostics::Process * process = System::Diagnostics::Process::GetProcessById( (int)*appProcessID );

		process->WaitForExit( );
	}

	// Starts the test application in a paused state...
	HoloScriptApplication * startAppPaused( )
	{
		HoloScriptApplication * holoApp =
			new HoloScriptApplication( );
		
		holoApp->LaunchApplication( HoloScriptTestMain::HoloScriptTest::testApplication,
			"",
			true,
			holoApp->GetFilteredFunctionList( ) );		

		return holoApp;
	}

	// Starts the test application in a running state...
	HoloScriptApplication * startAppRunning( )
	{
		HoloScriptApplication * holoApp =
			new HoloScriptApplication( );
		
		holoApp->LaunchApplication( HoloScriptTestMain::HoloScriptTest::testApplication,
			"",
			false,
			new ArrayList( ) );

		return holoApp;
	}

	// Starts the app separately from HD, then uses HoloScript to attach...
	HoloScriptApplication * startAppAttachment( )
	{
		System::Diagnostics::Process * process = new System::Diagnostics::Process( );
		process->StartInfo->FileName = HoloScriptTestMain::HoloScriptTest::testApplication;
		process->Start( );

		// Wait for application to fully start...
		process->WaitForInputIdle( );

		HoloScriptApplication * holoApp =
			new HoloScriptApplication( );
		
		holoApp->AttachApplication( (unsigned long)process->Id, 
			false, 
			holoApp->GetFilteredFunctionList( ),
			false );

		return holoApp;
	}

	__gc class HoloScriptTest1 : public HoloScriptTestMain::HoloScriptTest
	{		
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest2 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest3 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest4 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest5 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest6 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest7 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest8 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest9 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest10 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest11 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest12 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest15 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest16 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest17 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest18 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest19 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest20 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest21 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			unsigned long * position;
			LogItem * logItem;
			void executeTest( );				
	};

	__gc class HoloScriptTest22 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			unsigned long * position;
			LogItem * logItem;
			void executeTest( );				
	};

	__gc class HoloScriptTest23 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			unsigned long * position;
			LogItem * logItem;
			void executeTest( );				
	};

	__gc class HoloScriptTest24 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			unsigned long * position;
			LogItem * logItem;
			void executeTest( );				
	};

	__gc class HoloScriptTest26A : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest26B : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest26C : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	
	};

	__gc class HoloScriptTest26D : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest27 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest28 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest29 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest44 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest45 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};
	
	__gc class HoloScriptTest47 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest48 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest49 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest50 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest52 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest55 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest58 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest59 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest66 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest68 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};

	__gc class HoloScriptTest70 : public HoloScriptTestMain::HoloScriptTest
	{
		public:
			HoloScriptApplication * holoApp;
			void executeTest( );				
	};
}