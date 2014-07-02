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

#include "FaultyUserLibrariesDialog.h"

using namespace SiUtils;
using namespace System;
using namespace System::ComponentModel;

namespace DotNetCacheGen
{
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
		timer->add_Tick(new EventHandler(this, UpdateProgressBar));
		timer->Interval = 1000;
		timer->Enabled = true;

		this->Controls->Add(descLabel);
		this->Controls->Add(progressBar);

		this->Text = ".NET Update";
		this->FormBorderStyle = FormBorderStyle::FixedDialog;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ClientSize = System::Drawing::Size(300, 64);
		this->StartPosition = FormStartPosition::CenterScreen;

		genThread = new System::Threading::Thread(new System::Threading::ThreadStart(this, GenerateInterceptModules));
		genThread->Start();
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
				CLRAddInterceptHandlerModule("MSCorLibReplacementLibrary.dll");
				CLRAddInterceptHandlerModule("SystemReplacementLibrary.dll");
				CLRAddInterceptHandlerModule("SystemServiceProcessReplacementLibrary.dll");
				CLRAddInterceptHandlerModule("SystemXMLReplacementLibrary.dll");
			
			
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

			progressBar->Maximum = generateList->Count * CLRGetInterceptCount();

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
							MessageBox(NULL, message, "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
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
					// something went wrong, tell the user and ask them if they want to remove it (scr 2068)
					String *message = String::Concat("There was a problem updating Holodeck's .NET interception modules.\nThe library that caused the problem was \"", moduleName, "\"");
					message = String::Concat(message, "\nThe reason for the problem was: ", e->Message, "\n\n");
					message = String::Concat(message, "Would you like to remove this library from the list of modules Holodeck will intercept?  Choosing yes will prevent this message from being displayed for this library in the future.");

					const __wchar_t __pin *msgString = PtrToStringChars(message);
					int answer = MessageBoxW(NULL, msgString, L"Holodeck", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST | MB_TASKMODAL);
					if (answer == IDYES)
					{
						try
						{
							// they want to remove the library
							HolodeckGui::FaultyUserLibrariesDialog *dialog = new HolodeckGui::FaultyUserLibrariesDialog();
							dialog->PerformCheckAndPopulateList();

							// figure out the replacementlibrary name for the module throwing the exception
							Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
							if (key)
							{
								Microsoft::Win32::RegistryKey *dotNetKey = key->OpenSubKey(DOTNET_REPLACEMENT_DLLS_KEY);
								String *replacementDllName = NULL;
								if (dotNetKey)
								{
									String *values[] = dotNetKey->GetValueNames();
									for (int i = 0; i < values->Length; i++)
									{
										Object *value = dotNetKey->GetValue(values[i]);
										if (value)
										{
											String *testString = value->ToString();
											if (testString->Equals(moduleName))
											{
												replacementDllName = values[i];
												break;
											}
										}
									}
								}

								if (!replacementDllName)
									throw new Exception();

								// get the dll name without the path
								String *fileNameBase = Path::GetFileNameWithoutExtension(replacementDllName);
							
								String *HolodeckInstallPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
								String *replacementDir = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));
								replacementDir = String::Concat(HolodeckInstallPath, "\\", replacementDir);
								String *functionDBFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
								functionDBFolder = String::Concat(HolodeckInstallPath, "\\", functionDBFolder);

								String *dllNameToRemove = String::Concat(replacementDir, "\\", fileNameBase, ".dll");
								String *xmlNameToRemove = String::Concat(functionDBFolder, "\\dotnet_", fileNameBase, ".dll.xml");
								dialog->RemoveLibrary(dllNameToRemove, xmlNameToRemove);
							}
						}
						catch(...)
						{
							MessageBoxW(NULL, L"Could not remove the library.", L"Holodeck", MB_OK | MB_TOPMOST | MB_TASKMODAL);
						}
					}
				}
			}
		}
		catch(Exception *)
		{
		}
		Close();
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
		progressBar->Value = CLRGetInterceptModuleStatus();
	}
}
