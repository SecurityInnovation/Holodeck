#pragma once

#using <mscorlib.dll>

using namespace System;
using namespace System::Reflection;
using namespace TestFramework;
using namespace TestFramework::Exceptions;
using namespace TestFramework::Run;
using namespace TestFramework::Utils;
using namespace TestFramework::Log;

namespace HoloScriptTestMain
{
	/// <summary>
	/// Summary description for LimitTest.
	/// </summary>
	__gc class HoloScriptTest : public RunTest
	{
		// Define the prefix for the name of the class we will load.
	public:
		static String * classNamePrefix = "HoloScriptTests.HoloScriptTest";
		static String * testApplication = "C:\\Windows\\Notepad.exe";
		Object * getTestClass( String * testID );		
	};


}