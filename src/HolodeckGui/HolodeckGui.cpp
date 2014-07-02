//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HolodeckGui.cpp
//
// DESCRIPTION: File containing the entry point function (WinMain) for the
//	Entire Holodeck application.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 Mar 2003		 B. Shirey	 File created.
//*************************************************************************

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

#include <windows.h>
#include <vcclr.h>
#include "MainFrame.h"
#include "SplashScreen.h"
#include "UserNotification.h"
#include "RegistrationMgr.h"
#include "defines.h"
#include "TimeTrialNoticeDialog.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Threading;
using namespace System::Windows::Forms;
using namespace HolodeckGui;
using namespace HolodeckRegistration;

#undef MessageBox

namespace HolodeckGui
{
	public __gc class Launcher
	{
	private:
		static HolodeckGui::SplashScreen *splashScreen;

		//*************************************************************************
		// Method:		IsAdmin
		// Description: Checks for administrative privileges
		//
		// Parameters:
		//	None
		//
		// Return Value: true if the user has administrative privileges, false
		//	otherwise
		//*************************************************************************
		bool Launcher::IsAdmin(void)
		{
			HKEY testKey;
			String *installFolderName;

			//check registry key access
			if (RegOpenKeyEx(INSTALLED_ROOT_KEY, ROOT_PATH, 0, KEY_ALL_ACCESS, &testKey) != ERROR_SUCCESS)
			{
				String *msg = "Holodeck couldn't get read/write access to: HKEY_LOCAL_MACHINE\\";

				msg = String::Concat(msg, ROOT_PATH);
				msg = String::Concat(msg, " reg key.");
				UserNotification::ErrorNotify(msg);
				return false;
			}

			RegCloseKey(testKey);

			//check directory access
			RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
			installFolderName = static_cast<String *>(key->GetValue(INSTALL_PATH_VALUE_NAME));
			key->Close();

			try
			{
				unsigned int i = 1;
				String *fileName = String::Format("{0}\\hdtest{1}.txt", installFolderName, i.ToString());
				// i will eventually rollover to 0 once we've used every other possibility, so stop then
				while (System::IO::File::Exists(fileName) && (i != 0))
				{
					i++;
					fileName = String::Format("{0}\\hdtest{1}.txt", installFolderName, i.ToString());
				}

				// couldn't find a file to try to write.  very unlikely
				if (System::IO::File::Exists(fileName))
					throw new Exception();
				
				//try writing a file...
				StreamWriter *testFile = System::IO::File::CreateText(fileName);
				testFile->Write("Test");
				testFile->Close();
				System::IO::File::Delete(fileName);
			}
			catch(...)
			{
				String *msg = "Holodeck couldn't get read/write access to the install folder: ";

				msg = String::Concat(msg, installFolderName);
				UserNotification::ErrorNotify(msg);
				return false;
			}

			return true;
		}

		//*************************************************************************
		// Method:		ShowSplashScreenFunc
		// Description: shows the splash screen
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		void Launcher::ShowSplashScreenFunc()
		{
			splashScreen = new HolodeckGui::SplashScreen();
			Application::Run(splashScreen);
		}

		//*************************************************************************
		// Method:		CloseSplashScreen
		// Description: closes and cleans up the splash screen
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		void Launcher::CloseSplashScreen()
		{
			if (splashScreen)
			{
				splashScreen->Close();
				splashScreen = NULL;
			}
		}

		//*************************************************************************
		// Method:		onStartupUserNotification
		// Description: called when mainframe needs to notify the user of something
		//	during its initialization process
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		void Launcher::onStartupUserNotification()
		{
			CloseSplashScreen();
		}

		//*************************************************************************
		// Method:		LaunchThreadFunc
		// Description: launches the app in a separate thread
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		void Launcher::LaunchThreadFunc()
		{
			ArrayList *commandLineArgs = new ArrayList();

			try
			{
				if (!IsAdmin())
				{
					UserNotification::ErrorNotify("You lack sufficient security priviliges to run Holodeck.  Holodeck will now exit.");
					return;  
				}
			}
			catch(...)
			{
				UserNotification::ErrorNotify("You lack sufficient security priviliges to run Holodeck.  Holodeck will now exit.");
				return;  
			}

			RegistrationMgr *mgr = RegistrationMgr::GetInstance();
			if (mgr->IsTimeTrial())
			{
				//if trial version that check that the time limit hasn't expired
				int daysLeft = RegistrationMgr::GetInstance()->GetNumberOfDaysRemaining();
				TimeTrialNoticeDialog *dialog = new TimeTrialNoticeDialog(daysLeft);
				DialogResult result = dialog->ShowDialog();

				// if they want to register
				if (result == DialogResult::OK)
				{
					RegistrationDialog *regDialog = new RegistrationDialog(true);
					regDialog->ShowDialog();
				}

				// recheck the days
				mgr->ReloadRegistrationData();
				if (mgr->IsTimeTrial() && (mgr->GetNumberOfDaysRemaining() <= 0))
					ExitProcess(0);
			}

			String *Args[] = Environment::GetCommandLineArgs();

			// check to see if we want to update the dot net cache
			bool noDotNetCacheGen = false;
			bool isLaunchingTool = false;
			if (Args->Length > 1)
			{
				for (int i=1; i<Args->Length; i++)
				{
					Args[i] = Args[i]->ToLower();
					if (Args[i]->CompareTo("/nodotnetcachegen") == 0)
						noDotNetCacheGen = true;
					else if ((Args[i]->CompareTo("/addnewintercepts") == 0) ||
							 (Args[i]->CompareTo("/customtestproject") == 0))
					{
						isLaunchingTool = true;
					}
				}
			}

			if (!noDotNetCacheGen)
			{
				// update the .NET instrumented binaries
				try
				{
					String *installFolder = GuiSupport::RegistryHelper::getInstance()->GetInstallationPath();
					System::Diagnostics::Process* dotNetUpdateProcess = new System::Diagnostics::Process();
					dotNetUpdateProcess->StartInfo->FileName = Path::Combine(installFolder, "DotNetCacheGen.exe");
					dotNetUpdateProcess->Start();
					dotNetUpdateProcess->WaitForExit();

					System::Diagnostics::Process* dotNetUpdateProcess35 = new System::Diagnostics::Process();
					dotNetUpdateProcess35->StartInfo->FileName = Path::Combine(installFolder, "DotNetCacheGen35.exe");
					dotNetUpdateProcess35->Start();
					dotNetUpdateProcess35->WaitForExit();

				}
				catch (...)
				{
					UserNotification::ErrorNotify("Could not start .NET module update process");
				}
			}

			Threading::Thread *splashThread = new Threading::Thread(new ThreadStart(this, ShowSplashScreenFunc));
			splashThread->IsBackground = true;
			splashThread->ApartmentState = ApartmentState::STA;

			if (!isLaunchingTool)
				splashThread->Start();

			StartupUserNotificationDelegate *startupNotificationDelegate = new StartupUserNotificationDelegate(this, onStartupUserNotification);

			MainFrame *mainFrame = new MainFrame(startupNotificationDelegate);

			if (Args->Length > 1)
			{
				for (int i=1; i<Args->Length; i++)
				{
					Args[i] = Args[i]->ToLower();
					commandLineArgs->Add(Args[i]);
				}
				
				mainFrame->CommandLineArgs = commandLineArgs;
			}

			CloseSplashScreen();
			mainFrame->ProcessCommandLine();
			
			if (!isLaunchingTool)
			{
				mainFrame->CheckForFaultyUserLibraries();
				Application::Run(mainFrame);
			}
			else
				ExitProcess(0);
		}

	public:
		//*************************************************************************
		// Method:		Launcher
		// Description: Constructor for the Launcher class
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		Launcher()
		{
			Threading::ThreadStart *launchThreadStart = new Threading::ThreadStart(this, LaunchThreadFunc);
			Threading::Thread *launchThread = new Threading::Thread(launchThreadStart);
			launchThread->ApartmentState = ApartmentState::STA;
			launchThread->Start();
			launchThread->Join();
		}
		
		//*************************************************************************
		// Method:		~Launcher
		// Description: Destructor for the Launcher class
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		Launcher::~Launcher()
		{
			// Get the log folder to delete the log files
			RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
			String *installFolderName = static_cast<String *>(key->GetValue(INSTALL_PATH_VALUE_NAME));
			String *logFolderName = static_cast<String *>(key->GetValue(LOG_PATH_VALUE_NAME));
			key->Close();

			if (!installFolderName || !logFolderName)
				return;

			String *fullPath = String::Concat(installFolderName, "\\", logFolderName);
			String *files[] = Directory::GetFiles(fullPath);
			for (int i = 0; i < files->Length; i++)
			{
				try
				{
					File::Delete(files[i]);
				}
				catch(...)
				{
				}
			}
		}
	};
}

//*************************************************************************
// Method:		WinMain
// Description: Entry point for the application
//
// Parameters:
//	See WinMain in MSDN
//
// Return Value: See WinMain in MSDN
//*************************************************************************
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Thread::CurrentThread->ApartmentState = ApartmentState::STA;

	Launcher *launcher = NULL;
	String *Args[] = Environment::GetCommandLineArgs();
	try
	{
		//check for /?
		for (int i=0; i < Args->Length; i++)
		{
			if (Args[i]->ToLower()->IndexOf ("/?") >= 0)
			{
				String * help = "Holodeck Enterprise Edition, Command-line usage:\r\n\r\n";
				help = String::Concat (help, "HolodeckGui [startwizard_option] [project_option] <options>", "\r\n\r\n");
				help = String::Concat (help, "\t[startwizard_option] choices", "\r\n");
				help = String::Concat (help, "\t/addnewintercepts\t\t- starts the 'Add Holodeck Intercepts' wizard on startup", "\r\n");
				help = String::Concat (help, "\t/customtestproject\t\t- starts the 'Create a custom test project' wizard on startup", "\r\n\r\n");
				help = String::Concat (help, "\t[project_option] choices", "\r\n");
				help = String::Concat (help, "\tprojectfile.hdp\t\t- loads the specified project file on startup", "\r\n");
				help = String::Concat (help, "\t/launch:exefilename\t- launches the specified exe with default options*", "\r\n");
				help = String::Concat (help, "\t/attach:<process_id>\t- attaches to the specified processID with default options*", "\r\n");
				help = String::Concat (help, "\t/launchservice:servicename\t- launches the specified service with default options*", "\r\n");
				help = String::Concat (help, "\t/attachservice:servicename\t- attachs to the specified running service with default options*", "\r\n\r\n");
				help = String::Concat (help, "\t<options> choices", "\r\n");
				help = String::Concat (help, "\t/randomstress:[low,medium,high]\t\t- starts the random test generator with the specified intensity", "\r\n");
				help = String::Concat (help, "\t/intelligentstress:[low,medium,high]\t\t- starts the intelligent test generator with the specified intensity", "\r\n");
				help = String::Concat (help, "\t/codecoverage:[low,medium,high][,timeout]\t- starts the code coverage generator with the specified intensity and optional timeout in seconds", "\r\n");
				help = String::Concat (help, "\t/record\t\t\t\t\t- starts recording the session on startup", "\r\n");
				help = String::Concat (help, "\t/silent\t\t\t\t\t- starts Holodeck in slient mode, messages are logged to ", ERROR_LOG_FILE, "\r\n\r\n");
				help = String::Concat (help, "\t*default options are:", "\r\n");
				help = String::Concat (help, "\tPause on start - false", "\r\n");
				help = String::Concat (help, "\tProcess chaining - true", "\r\n");
				help = String::Concat (help, "\tInherit settings - true", "\r\n");
				help = String::Concat (help, "\tAttach debugger - true", "\r\n");
				help = String::Concat (help, "\tLogging options - same defaults as in New Project Wizard", "\r\n\r\n");
				help = String::Concat (help, "Examples:", "\r\n");
				help = String::Concat (help, "\tHolodeckGui myproject.hdp", "\r\n");
				help = String::Concat (help, "\tHolodeckGui myproject.hdp /codecoverage:low", "\r\n");
				help = String::Concat (help, "\tHolodeckGui myproject.hdp /codecoverage:high,15", "\r\n");
				help = String::Concat (help, "\tHolodeckGui myproject.hdp /record", "\r\n");
				help = String::Concat (help, "\tHolodeckGui /launch:c:\\windows\\notepad.exe", "\r\n");
				help = String::Concat (help, "\tHolodeckGui /attach:2146 /randomstress:high", "\r\n");
				help = String::Concat (help, "\tHolodeckGui /launchservice:\"Indexing Service\"", "\r\n");
				help = String::Concat (help, "\tHolodeckGui /attachservice:\"ClipBook\" /record /slient", "\r\n");

				MessageBox::Show (help, "Holodeck Command-line usage");
				return 0;
			}
		}

		launcher = new Launcher();
	}
	catch(Exception *e)
	{
		for (int i = 0; i < Args->Length; i++)
		{
			if (Args[i]->ToLower()->StartsWith("/debug"))
			{
				String* Message = System::String::Concat(
					"Unhandled exception in Holodeck:\n", e->ToString());
				UserNotification::ErrorNotify(Message);
				break;
			}
		}
	}
	catch(...)
	{
		UserNotification::ErrorNotify("Unknown exception has occurred.  Exiting Holodeck...");
	}

	if (launcher)
	{
		delete launcher;
		launcher = NULL;
	}
}