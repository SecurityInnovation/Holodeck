//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		LogSpeed.h
//
// DESCRIPTION: Defination of LogSpeed.cpp
//
//=========================================================================
// Modification History
//
// Date				Name			     Purpose
// -----------    -----------   ------------------------------------------
// 31 Oct 2003	  Ankur Jauhari	       File created.
//*************************************************************************
#pragma once
#using <mscorlib.dll>
#include <stdio.h>
#include <Windows.h>
#using <System.dll>
using namespace System;
using namespace System::IO;
using namespace Microsoft::Win32;
using namespace System::Collections;

namespace PerformanceTest
{
	public __gc class LogSpeed
	{
		public:
			LogSpeed();
			LogSpeed(StreamWriter *FtHandle , int args);
			~LogSpeed(void);
			void ReadszFile();
			void logFunction(String* psz);
			double GetTimeinSeconds();
			double ProcessTime(double , double);

			StreamWriter* fileHandle;    // Writer to log.txt
			String* line;                // Contains all information on one line
			int count;                   // Count is passed as argument
			ArrayList* Data;
	};
}