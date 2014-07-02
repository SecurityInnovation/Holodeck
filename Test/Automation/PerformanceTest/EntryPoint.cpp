//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		EntryPoint.cpp
//
// DESCRIPTION: Creates a log entry n times where n times is passed a
//              command line argument.
//=========================================================================
// Modification History
//
// Date				Name			     Purpose
// -----------    -----------   ------------------------------------------
// 31 Oct 2003	  Ankur Jauhari	       File created.
//*************************************************************************

#include "LogSpeed.h"

#using <mscorlib.dll>
using namespace System;
using namespace System::IO;
using namespace PerformanceTest;


// Entry point for the programme.
int wmain(int argc,wchar_t* argv[])
{

	
	if (argc < 2)
	{
		Console::WriteLine(" ---- Error While Using Application. ----");
		Console::WriteLine("Usage: PerformanceTest.exe NumberOfCallsToBeMade");
		Console::WriteLine("Try with correct Arguments as Shown above. ");
		return -1;
	}

	

	if(!File::Exists("Log.txt"))
	{
		StreamWriter* sw = File::CreateText("Log.txt");
		LogSpeed* test = new LogSpeed(sw,Convert::ToInt32(argv[1]));
		test->ReadszFile();
	}
	else
	{
		StreamWriter* sw = File::AppendText("Log.txt");
		LogSpeed* test = new LogSpeed(sw,Convert::ToInt32(argv[1]));
		test->ReadszFile();
		Console::WriteLine("File already exists.");
	}
	
	// We're done doing stuff.
	

	

	//System::Diagnostics::Process * proc = new System::Diagnostics::Process( );

	//proc->StartInfo->FileName = "C:\\Documents and Settings\\ajauhari\\My Documents\\Visual Studio Projects\\ExcelProject2\\ExcelProject2.xls";

	//proc->Start( );

	return 0;
}




