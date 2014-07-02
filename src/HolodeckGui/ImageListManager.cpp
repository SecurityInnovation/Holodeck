//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ImageListManager.cpp
//
// DESCRIPTION: Contains implementation for the singleton class 
//	ImageListManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "ImageListManager.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ImageListManager
	// Description: Constructor for the ImageListManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ImageListManager::ImageListManager()
	{
		try
		{
			installFolder = GuiSupport::RegistryHelper::getInstance()->GetInstallationPath();

			CreateNodeTypeImageList();
			CreateButtonBarImageList();
			CreateLogTreeViewImageList();

			appIcon = new System::Drawing::Icon(String::Concat(installFolder, "\\", ICON_FILE_NAME));
			logoBitmap = new System::Drawing::Bitmap(String::Concat(installFolder, "\\", HOLODECK_LOGO_FILE));
			sortAscendingIcon = new System::Drawing::Bitmap(String::Concat(installFolder, "\\", SORT_ASCENDING_ICON_FILE));
			sortDescendingIcon = new System::Drawing::Bitmap(String::Concat(installFolder, "\\", SORT_DESCENDING_ICON_FILE));
		}
		catch(System::IO::FileNotFoundException *e)
		{
			String *message = String::Concat("Could not find file: ", e->FileName);
			UserNotification::ErrorNotify(message);
		}
		catch(System::IO::DirectoryNotFoundException *e)
		{
			UserNotification::ErrorNotify(e->Message);
		}
		catch (System::ArgumentException *)
		{
			UserNotification::ErrorNotify("Could not find image files.  Please Re-Install Holodeck");
		}
	}

	//*************************************************************************
	// Method:		~ImageListManager
	// Description: Destructor for the ImageListManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ImageListManager::~ImageListManager()
	{
	}

	//*************************************************************************
	// Method:		CreateNodeTypeImageList
	// Description: Creates the node type image list
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ImageListManager::CreateNodeTypeImageList()
	{
		nodeTypeImageList = new ImageList();

		// process node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", PROCESS_ICON_FILE)));

		// thread node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", THREAD_ICON_FILE)));

		// recorded sessions node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", RECORDEDSESSIONS_ICON_FILE)));
		
		// recorded session node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", RECORDEDSESSION_ICON_FILE)));

		// reports node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", REPORTS_ICON_FILE)));

		// report node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", REPORT_ICON_FILE)));

		// logs node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", LOGS_ICON_FILE)));
	
		// saved logs node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", LOGS_ICON_FILE)));

		// active log view node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", ACTIVELOG_ICON_FILE)));

		// saved log view node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", SAVEDLOG_ICON_FILE)));

		// resources node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", RESOURCES_ICON_FILE)));

		// memory type nodes
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", MEMORY_ICON_FILE)));

		// disk type nodes
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", DISK_ICON_FILE)));

		// network type nodes
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_ICON_FILE)));

		// files node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILES_ICON_FILE)));

		// file node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILE_ICON_FILE)));

		// registry node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", REGISTRY_ICON_FILE)));

		// registry entry node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", REGISTRYENTRY_ICON_FILE)));

		// faults node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULTS_ICON_FILE)));

		// memory fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", MEMORY_ICON_FILE)));

		// memory fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// disk fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", DISK_ICON_FILE)));

		// disk fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// network fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_ICON_FILE)));

		// network fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// registry fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", REGISTRY_FAULTS_ICON_FILE)));

		// registry fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// process fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", PROCESS_FAULTS_ICON_FILE)));

		// process fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// custom fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", CUSTOM_ICON_FILE)));

		// custom fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// network corruption fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_ICON_FILE)));

		// network corruption fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// file corruption fault type node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILE_CORRUPTION_ICON_FILE)));

		// file corruption fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// corrupted files
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILES_ICON_FILE)));

		// corrupted file
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILE_ICON_FILE)));
		
		// saved corrupted files
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILES_ICON_FILE)));

		// saved corrupted file
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILE_ICON_FILE)));

		// limits node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", LIMITS_ICON_FILE)));

		// disk limit node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", DISK_ICON_FILE)));

		// memory limit node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", MEMORY_ICON_FILE)));

		// network upload limit node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_ICON_FILE)));

		// network download limit node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_ICON_FILE)));

		// tests node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", TESTS_ICON_FILE)));

		// test node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", TEST_ICON_FILE)));

		// resource faults node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", RESOURCE_FAULTS_ICON_FILE)));

		// resource fault node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FAULT_ICON_FILE)));

		// dynamic help
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", DYNAMIC_HELP_ICON_FILE)));

		// help contents
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", HELP_CONTENTS_ICON_FILE)));

		// network logs node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_LOGS_ICON_FILE)));
		
		// saved network logs node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_LOGS_ICON_FILE)));

		// active network log view node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORKLOG_ICON_FILE)));

		// saved network log view node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", SAVEDLOG_ICON_FILE)));

		// exception node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", EXCEPTION_ICON_FILE)));

		// exception log view node
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", EXCEPTIONS_ICON_FILE)));
		
		//end of defined datanodes

		// paused process
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", PAUSED_PROCESS_ICON_FILE)));

		// terminated process
		nodeTypeImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", TERM_PROCESS_ICON_FILE)));
	}

	//*************************************************************************
	// Method:		CreateButtonBarImageList
	// Description: Creates the button bar image list
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ImageListManager::CreateButtonBarImageList()
	{
		buttonBarImageList = new ImageList();

		//0 - new project
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NEW_PROJECT_ICON_FILE)));

		//1 - open project
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", OPEN_PROJECT_ICON_FILE)));

		//2 - save project
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", SAVE_PROJECT_ICON_FILE)));

		//3 - new test app
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NEW_TEST_APP_ICON_FILE)));

		//4 - create test
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", CREATE_TEST_ICON_FILE)));

		//4 - create resource fault
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", RESOURCE_FAULTS_ICON_FILE)));

		//5 - new fault
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NEW_FAULT_ICON_FILE)));

		//6 - create report
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", CREATE_REPORT_ICON_FILE)));

		//7 - show project pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", PROJECT_PANE_ICON_FILE)));

		//8 - show resource pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", RESOURCE_PANE_ICON_FILE)));

		//9 - show limits pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", LIMITS_PANE_ICON_FILE)));

		//10 - show file corruption details pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILE_CORRUPT_PANE_ICON_FILE)));

		//11 - show network corruption details pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_MSG_PANE_ICON_FILE)));

		//12 - show properties pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", PROPERTIES_PANE_ICON_FILE)));

		//13 - show help pane
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", DYNAMIC_HELP_ICON_FILE)));

		//14 - file corruption fault
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", FILE_CORRUPTION_ICON_FILE)));

		//15 - network corruption fault
		buttonBarImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORK_ICON_FILE)));
	}

	//*************************************************************************
	// Method:		CreateLogTreeViewImageList
	// Description: Creates the button bar image list
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ImageListManager::CreateLogTreeViewImageList()
	{
		logTreeViewImageList = new ImageList();

		// 0 - nothing
		logTreeViewImageList->Images->Add(new System::Drawing::Bitmap(1, 1));

		// 1 - network log icon
		logTreeViewImageList->Images->Add(new Bitmap(String::Concat(installFolder, "\\", NETWORKLOG_ICON_FILE)));
	}

	//*************************************************************************
	// Method:		getInstance
	// Description: Return the single instance of the ImageListManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the ImageListManager class
	//*************************************************************************
	ImageListManager *ImageListManager::getInstance()
	{
		if (instance == NULL)
			instance = new ImageListManager();

		return instance;
	}
}