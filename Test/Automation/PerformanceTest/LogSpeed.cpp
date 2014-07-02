//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		LogSpeed.cpp
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

#include "logspeed.h"
#using <mscorlib.dll>


///*************************************************************************
/// Method:		LogSpeed
/// Description: Constructor
///
/// Parameters:
///	File Handle for log.txt, argument for number of iterations
/// Return Value : None 
///*************************************************************************
namespace PerformanceTest
{
	LogSpeed::LogSpeed()
	{
	}
	LogSpeed::LogSpeed(StreamWriter* FtHandle, int arg)
	{
		fileHandle = FtHandle;
		count = arg;
		line = "";
		//Data = new ArrayList();
	}


	///*************************************************************************
	/// Method:		LogSpeed
	/// Description: Destructor
	///
	/// Parameters:
	///	None
	/// Return Value : None 
	///*************************************************************************
	LogSpeed::~LogSpeed(void)
	{

	}

	///*************************************************************************
	/// Method:		ReadszFile
	/// Description: Calls Heap Alloc and Heap Free n times.
	///
	/// Parameters:
	///	None
	/// Return Value : None 
	///*************************************************************************
	void LogSpeed::ReadszFile()
	{
		int nTimes = 1000;
		int* iAlloc = NULL;
		double startTime = 0;
		double endTime = 0;
		//double TimeTaken = 0;
		
		String *pszCount = NULL;
		String *information = NULL;
		String *sTime = NULL;
		String *eTime = NULL;


		HANDLE hHeap = GetProcessHeap( );

		for(int argCount = 0; argCount < count; argCount++)
		{
			startTime = GetTimeinSeconds();
			sTime = String::Concat("#",Convert::ToString(startTime),"#");

			for(int iCount = 0; iCount < 10; iCount++)
			{
				iAlloc = (int*)(HeapAlloc(hHeap, NULL, sizeof(int)));
				logFunction("HeapAlloc(hHeap, NULL, sizeof(int))");	
				HeapFree(hHeap,NULL,iAlloc);
				logFunction("HeapFree(hHeap, NULL, sizeof(int))");
			}
			endTime = GetTimeinSeconds();
			eTime = String::Concat(Convert::ToString(endTime),"#");

			fileHandle->WriteLine();
			fileHandle->WriteLine(String::Concat(Convert::ToString(argCount+1),sTime,eTime,Convert::ToString(ProcessTime(startTime,endTime))));
			//fileHandle->Write(String::Concat("Time Taken for ",Convert::ToString(count)));
			//fileHandle->WriteLine(String::Concat("--->",Convert::ToString(ProcessTime(startTime,endTime))));
			fileHandle->WriteLine();

			
			//Data->Add(information);
		}
		//return Data;
		fileHandle->Close();
	}

	///*************************************************************************
	/// Method:		logFunction
	/// Description: Creates the data to be put in file.
	///
	/// Parameters:
	///	heap string
	/// Return Value : None 
	///*************************************************************************
	void LogSpeed::logFunction(String* pszName)
	{
		LPSYSTEMTIME pSystemTime = NULL;

		String* pszDate;
		String* pszTime;
		String* pszTemp;

		// format function call timer and log...
		pSystemTime = new SYSTEMTIME;
		GetLocalTime(pSystemTime);
		
		pszDate = Convert::ToString(pSystemTime->wMonth);
		
		pszTemp = Convert::ToString(pSystemTime->wDay);
		pszDate = String::Concat(pszDate, "/");
		pszDate = String::Concat(pszDate, pszTemp);

		pszTemp = Convert::ToString(pSystemTime->wYear);
		pszDate = String::Concat(pszDate, "/");
		pszDate = String::Concat(pszDate, pszTemp," ");

		pszTime = Convert::ToString(pSystemTime->wHour);
		
		pszTemp = Convert::ToString(pSystemTime->wMinute);
		pszTime = String::Concat(pszTime, ":");
		pszTime = String::Concat(pszTime, pszTemp);

		pszTemp = Convert::ToString(pSystemTime->wSecond);
		pszTime = String::Concat(pszTime, ":");
		pszTime = String::Concat(pszTime, pszTemp);

		pszTemp = Convert::ToString(pSystemTime->wMilliseconds);
		pszTime = String::Concat(pszTime, ":");
		pszTime = String::Concat(pszTime, pszTemp," ");

		line = String::Concat(pszDate, pszTime, pszName);
		fileHandle->WriteLine(line);
	}

	double LogSpeed::GetTimeinSeconds()
	{
		LPSYSTEMTIME pSystemTime = new SYSTEMTIME;
		GetLocalTime(pSystemTime);
		double pszTime = 0;
		double pszTemp = 0;
		

		pszTemp = Convert::ToDouble(pSystemTime->wHour);
		pszTemp *= 3600;
		pszTime += pszTemp;

		pszTemp = Convert::ToDouble(pSystemTime->wMinute);
		pszTemp *= 60;
		pszTime += pszTemp;

		pszTemp = Convert::ToDouble(pSystemTime->wSecond);
		pszTime += pszTemp;

		pszTemp = Convert::ToDouble(pSystemTime->wMilliseconds);
		pszTemp = pszTemp/60;
		pszTime += pszTemp;

		return pszTime;
	}

	double LogSpeed::ProcessTime(double sTime, double eTime)
	{
		return eTime - sTime;
	}
}





