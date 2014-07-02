///************************************************************************
///	Center for Information Assurance - Florida Institute Of	Technology
///
///	FILE:		EntryPoint.cpp
///
///	
///
///========================================================================
///	Modification History
///
///	Date				Name			Purpose
///	-----------	   ----------- ------------------------------------------
///	03/10/2003	   Gavin Menezes	File Created.
///*************************************************************************

#include "stdafx.h"
#include "TestFaults.h"
#using <mscorlib.dll>
using namespace	System;
using namespace	System::IO;
using namespace	Microsoft::Win32;

int	main(int argc, char* argv[])
	{
	//open registry	and	delete all values set by FaultIntercepter (Initilize)
	RegistryKey	*LMachine =	RegistryKey::OpenRemoteBaseKey(RegistryHive::LocalMachine, System::Environment::MachineName);			 
	RegistryKey	*SKey =	LMachine->OpenSubKey("SOFTWARE", true);
	RegistryKey	*HKey =	 SKey->OpenSubKey("HolodeckEE",	true);
	try
		{
		HKey->DeleteSubKey("Outcome");
		HKey->DeleteSubKey("FaultIntercepterReady");
		HKey->DeleteSubKey("FaultReady");
		}catch(System::ArgumentException *ArgumentError){}

	String *path = System::Environment::CurrentDirectory;
	int	index =	path->IndexOf("Debug");

	if (index != -1)
		{
		path = path->Substring(0, index-1);
		}

	path = String::Concat(path,"\\Logs\\");
	Console::WriteLine("Logs in	{0}",path);

	//Begin	a streamwriter to output whether a fault has passed	or failed. 
	FileStream *DefaultFileStreamer;
	String *FaultFileLogName = String::Concat(path,	"DefaultLog.Log");
	DefaultFileStreamer	= new FileStream(FaultFileLogName, FileMode::OpenOrCreate, FileAccess::Write  );
	StreamWriter * DefaultWriter = new StreamWriter(DefaultFileStreamer);

	TestFaults *FaultTester	= new TestFaults();

	//load the functions and faults	xml	files to memory
	FaultTester->LoadFaultsXmlFile();
	FaultTester->LoadFunctionsXmlFile();

	//if no	arguments were entered then	notify user	and	exit
	if (argc ==	1)
		{
		Console::WriteLine("Please enter fault names as arguments!");
		Sleep(8000);
		return 0;
		}

	//initilize	detailed log files to record the return	values and errorcodes for each function
	//in a fault.
	String *DetailLogFile =	String::Concat(path,"LastRun-DetailLog.Log");
	FileStream *DetailLogStreamer;
	DetailLogStreamer =	new	FileStream(DetailLogFile, FileMode::OpenOrCreate , FileAccess::Write  );
	StreamWriter * DetailLogWriter = new StreamWriter(DetailLogStreamer);

	bool FaultOutcome =	false;

	for(int	arg=1;arg<argc;arg++)
		{
		//if just one fault	iss	being run then open	a log with the fault name
		if (argc == 2)
			{
			String *FaultFileLogName = String::Concat(path,argv[arg],".Log");
			FileStream *FileStreamer;
			FileStreamer = new FileStream(FaultFileLogName,	FileMode::OpenOrCreate,	FileAccess::Write );
			StreamWriter * Writer =	new	StreamWriter(FileStreamer);
			Console::WriteLine("Currently running {0} fault...", Convert::ToString(argv[arg]));
			FaultOutcome = FaultTester->TestFault(argv[arg],Writer,DetailLogWriter);
			Writer->Close();
			}

		//if more than one argument	then load the default log to store all the faults that are to be run
		if(argc > 2)
			{
			FaultOutcome = FaultTester->TestFault(argv[arg],DefaultWriter,DetailLogWriter);
			}

		RegistryKey	*PassFail =	HKey->CreateSubKey("Outcome");

		if(FaultOutcome	== true)
			{
			PassFail->SetValue("Outcome",static_cast<String	*>("PASS"));
			}
		else 
			{
			PassFail->SetValue("Outcome",static_cast<String	*>("FAIL"));
			}
		FaultOutcome = false;
		}
	//close	all	open writers
	DefaultWriter->Close();
	DetailLogWriter->Close();
	Console::ReadLine();
	return 0;
	}