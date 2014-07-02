//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DotNetCacheStatusForm.cpp
//
// DESCRIPTION: Contains definition for the .NET cache generator classes
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 May 2003		 R. Wagner	 File created.
//*************************************************************************
#include "SiString.h"
#include "CLRIntercept.h"
#include <vcclr.h>
#include "DotNetCacheStatusForm.h"
#include "StringConverter.h"

#using <System.dll>

//#include "FaultyUserLibrariesDialog.h"

using namespace SiUtils;
using namespace System;
using namespace System::ComponentModel;

namespace DotNetCacheGen
{

	__delegate void dlgSetMaximum(System::Windows::Forms::ProgressBar *pb, System::UInt32 max);
	void DotNetCacheStatusForm::setMaximum(System::Windows::Forms::ProgressBar *pb, System::UInt32 max)
	{
		pb->Maximum = max;
	}


	__delegate void dlgSetValue(System::Windows::Forms::ProgressBar *pb, System::UInt32 value);
	void DotNetCacheStatusForm::setValue(System::Windows::Forms::ProgressBar *pb, System::UInt32 value)
	{
		pb->Value = value;
	}

	__delegate void dlgClose();

	//*************************************************************************
	// Method:		DotNetCacheStatusForm
	// Description: Constructor for the DotNetCacheStatusForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DotNetCacheStatusForm::DotNetCacheStatusForm(ArrayList* genList, bool IsCommandLine, String * cmdLineLibrary)
	{
		this->generateList = genList;
		this->commandLineLibrary = cmdLineLibrary;
		this->isCommandLine = IsCommandLine;

		Label *descLabel = new Label();
		descLabel->Text = "Updating Holodeck's .NET interception modules...";
		descLabel->Location = System::Drawing::Point(8, 8);
		descLabel->Size = System::Drawing::Size(284, 16);

		progressBar = new ProgressBar();
		progressBar->Location = System::Drawing::Point(8, 32);
		progressBar->Size = System::Drawing::Size(284, 24);
		progressBar->Minimum = 0;
		progressBar->Value = 0;

		timer = new Timer();
		timer->add_Tick(new EventHandler(this, &DotNetCacheStatusForm::UpdateProgressBar));
		timer->Interval = 1000;
		timer->Enabled = true;

		this->Controls->Add(descLabel);
		this->Controls->Add(progressBar);

		this->Text = ".NET Update (2.0 and above libraries)";
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ClientSize = System::Drawing::Size(300, 64);
		this->StartPosition = FormStartPosition::CenterScreen;

		genThread = new System::Threading::Thread(new System::Threading::ThreadStart(this, &DotNetCacheStatusForm::GenerateInterceptModules));
		genThread->Start();
//		DotNetCacheStatusForm::GenerateInterceptModules();
	}

	//*************************************************************************
	// Method:		~DotNetCacheStatusForm
	// Description: Destructor for the DotNetCacheStatusForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DotNetCacheStatusForm::~DotNetCacheStatusForm()
	{
	}

	//*************************************************************************
	// Method:		GenerateInterceptModules
	// Description: Generates cached instrumented .NET modules
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void DotNetCacheStatusForm::GenerateInterceptModules()
	{
		try
		{
			CLRInitializeIntercepts();

			//Cache user Holodeck dotnet replacement libraries
			if (! this->isCommandLine)
			{
				//CLRAddInterceptHandlerModule("MSCorLibReplacementLibrary.dll");
				//CLRAddInterceptHandlerModule("SystemReplacementLibrary.dll");
				//CLRAddInterceptHandlerModule("SystemServiceProcessReplacementLibrary.dll");
				//CLRAddInterceptHandlerModule("SystemXMLReplacementLibrary.dll");
			
				CLRAddInterceptHandlerModule("MSCorLibReplacementLibrary35.dll");
				CLRAddInterceptHandlerModule("SystemReplacementLibrary35.dll");
				CLRAddInterceptHandlerModule("SystemServiceProcessReplacementLibrary35.dll");
				CLRAddInterceptHandlerModule("SystemXMLReplacementLibrary35.dll");
			
				//Cache user generated dotnet replacement libraries
				SiUtils::Registry registry;
				SiString sInstalledPath;
				SiString sDotNetReplacementKey;

				SiArray<SiString> interceptedDLLs;
				if (registry.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH))
				{
					interceptedDLLs = registry.GetValueNames (registry.GetCurrentKey(), DOTNET_REPLACEMENT_DLLS_KEY);

					for (unsigned int j=0; j<interceptedDLLs.GetLength(); j++)
					{
						SiString iDLL = interceptedDLLs[j];
						CLRAddInterceptHandlerModule((char *) iDLL);
					}
				}
				registry.Close();
			}
			else
			{
				//Call CLRAddInterceptHandlerModule on the lib specified thru the command line
				Holodeck::StringConverter *converter = Holodeck::StringConverter::GetInstance();
				SiString replName;
				converter->ConvertToSiString(this->commandLineLibrary, &replName);
				CLRAddInterceptHandlerModule((char *) replName);
			}

			progressBar->Invoke(new dlgSetMaximum(this, &DotNetCacheStatusForm::setMaximum),	\
								__gc new System::Object*[2] { progressBar, __box(generateList->Count * CLRGetInterceptCount()) });
			

			for (int i=0; i<generateList->Count; i++)
			{
				String* moduleName = dynamic_cast<String*>(generateList->Item[i]);
				try
				{
					SiString wszModuleName;
					Holodeck::StringConverter::GetInstance()->ConvertToSiString(moduleName, &wszModuleName);
					
					SiString interceptedModule = CLRCheckInterceptedCacheForAllModules(wszModuleName);
					if (interceptedModule == "")
					{
						// Not already intercepted.  Call CLRInterceptModule to generate the instrumented binary.
						if (CLRInterceptModule(wszModuleName) == "")
						{
							char message[MAX_PATH * 2];
							sprintf(message, "Failed to generate interception module for %S", wszModuleName);
							::MessageBox(NULL, message, "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
						}
					}
					else
					{
						// Already intercepted, skip the module
						CLRSkipModuleAndUpdateStatus();
					}
				}
				catch(Exception *e)
				{
					// An error occured while intercepting this module, skip it
					CLRSkipModuleAndUpdateStatus();
				}
			}
		}
		catch(Exception *)
		{
		}
		this->Invoke(new dlgClose(this, &DotNetCacheStatusForm::Close));
	}


	//*************************************************************************
	// Method:		UpdateProgressBar
	// Description: Updates the progress bar with the current status
	//
	// Parameters:
	//	obj - Object that triggered the event
	//  args - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void DotNetCacheStatusForm::UpdateProgressBar(Object* obj, EventArgs* args)
	{
		progressBar->Invoke(new dlgSetValue(this, &DotNetCacheStatusForm::setValue),	\
					   __gc new System::Object*[2] { progressBar, __box(CLRGetInterceptModuleStatus()) });
			
	}
}
