//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		ServiceHelper.cpp
//
// DESCRIPTION: Contains implementation for the class ServiceHelper
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#include "ServiceHelper.h"
#include "StringConverter.h"

#include <string>

using namespace std;
using namespace Holodeck;

namespace HoloScript
{
	//*************************************************************************
	// Method:		ServiceHelper
	// Description: Constructor for the ServiceHelper class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ServiceHelper::ServiceHelper()
	{
	}

	//*************************************************************************
	// Method:		~ServiceHelper
	// Description: Destructor for the ServiceHelper class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ServiceHelper::~ServiceHelper()
	{
	}

	//*************************************************************************
	// Method:		GetCurrentSharedServiceGroup
	// Description: Gets the name of the service group this service is in
	//
	// Parameters:
	//	serviceName - the name of the service to get the group of
	//
	// Return Value: the name of the service group if successful, NULL otherwise
	//*************************************************************************
	String *ServiceHelper::GetCurrentSharedServiceGroup(String *serviceName)
	{
		RegistryKey *groupKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICE_GROUP_KEY_NAME);
		if (!groupKey)
			return NULL;

		String *groupValues[] = groupKey->GetValueNames();
		for (int i = 0; i < groupValues->Length; i++)
		{
			// read the value to see if our service name is in it
			Object *value = groupKey->GetValue(groupValues[i]);
			System::Array *strings = dynamic_cast<System::Array *>(value);
			if (!strings)
				continue;

			// see if the service is in this set of strings
			IEnumerator *enumerator = strings->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *service = dynamic_cast<String *>(enumerator->Current);
				if (!service)
					continue;

				if (service->ToLower()->CompareTo(serviceName->ToLower()) == 0)
				{
					groupKey->Close();
					return groupValues[i];
				}
			}
		}

		groupKey->Close();
		return NULL;
	}

	//*************************************************************************
	// Method:		CreateNewServiceGroup
	// Description: Creates a new service group with all the settings of the
	//	original group
	//
	// Parameters:
	//	basedOnGroupName - the name of the service group to copy
	//	serviceName - the service to put in the new group
	//
	// Return Value: the name of the new service group if successful, NULL otherwise
	//*************************************************************************
	String *ServiceHelper::CreateNewServiceGroup(String *basedOnGroupName, String *serviceName)
	{
		RegistryKey *groupKey;
		RegistryKey *originalServiceKey;
		RegistryKey *newServiceKey;

		try
		{
			groupKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICE_GROUP_KEY_NAME, true);

			// find a name that is not taken for the copied group
			String *newGroupName;
			String *groups[] = groupKey->GetValueNames();
			int suffixIndex = 0;
			do
			{
				suffixIndex++;
				newGroupName = String::Concat(basedOnGroupName, __box(suffixIndex)->ToString());
			} while (groups->IndexOf(groups, newGroupName) != -1);

			RegistryKey *originalServiceKey = groupKey->OpenSubKey(basedOnGroupName);
			// the group might not have a subkey
			if (originalServiceKey)
			{
				groupKey->CreateSubKey(newGroupName);
				newServiceKey = groupKey->OpenSubKey(newGroupName, true);

				// copy the subkey's values
				String *originalGroupValues[] = originalServiceKey->GetValueNames();
				for (int i = 0; i < originalGroupValues->Length; i++)
				{
					newServiceKey->SetValue(originalGroupValues[i], originalServiceKey->GetValue(originalGroupValues[i]));
				}

				newServiceKey->Close();
				originalServiceKey->Close();
			}

			// create a new multi string value for the group's services
			String *valuesInNewGroup[] = new String *[1];
			valuesInNewGroup[0] = serviceName;
			groupKey->SetValue(newGroupName, valuesInNewGroup);
			groupKey->Close();

			return newGroupName;
		}
		catch(...)
		{
			if (groupKey)
				groupKey->Close();

			if (originalServiceKey)
				originalServiceKey->Close();

			if (newServiceKey)
				newServiceKey->Close();
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		ChangeServiceImagePathToUseGroup
	// Description: Checks a service's image path to see if it uses the old group
	//	name, and if so replaces it with the new group name
	//
	// Parameters:
	//	groupName - the name of the service group to remove the service from
	//	serviceName - the service to remove from group
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ServiceHelper::ChangeServiceImagePathToUseGroup(String *originalGroupName, String *groupName, String *serviceName)
	{
		RegistryKey *serviceBaseKey;
		RegistryKey *serviceKey;

		try
		{
			serviceBaseKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICES_BASE_KEY_NAME, true);
			serviceKey = serviceBaseKey->OpenSubKey(serviceName, true);

			String *oldImagePath = dynamic_cast<String *>(serviceKey->GetValue(IMAGE_PATH_VALUE_NAME));
			String *toLookFor = String::Concat("-k ", originalGroupName);
			String *toReplaceWith = String::Concat("-k ", groupName);

			int pos = oldImagePath->LastIndexOf(toLookFor);
			if (pos != -1)
			{
				String *newImagePath = oldImagePath->Replace(toLookFor, toReplaceWith);
				serviceKey->SetValue(IMAGE_PATH_VALUE_NAME, newImagePath);
			}

			serviceKey->Close();
			serviceBaseKey->Close();
			return true;
		}
		catch(...)
		{
		}

		if (serviceKey)
			serviceKey->Close();

		if (serviceBaseKey)
			serviceBaseKey->Close();

		return false;
	}

	//*************************************************************************
	// Method:		DeleteServiceFromGroup
	// Description: Deletes a service from a service group
	//
	// Parameters:
	//	groupName - the name of the service group to remove the service from
	//	serviceName - the service to remove from group
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ServiceHelper::DeleteServiceFromGroup(String *groupName, String *serviceName)
	{
		RegistryKey *groupKey;
		try
		{
			groupKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICE_GROUP_KEY_NAME, true);
			if (!groupKey)
				return false;

			// read the value to see if our service name is in it
			Object *value = groupKey->GetValue(groupName);
			System::Array *strings = dynamic_cast<System::Array *>(value);
			if (!strings)
				return false;

			ArrayList *newValue = new ArrayList();
			IEnumerator *enumerator = strings->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *thisString = dynamic_cast<String *>(enumerator->Current);
				if (!thisString)
					continue;

				if (thisString->ToLower()->CompareTo(serviceName->ToLower()) != 0)
					newValue->Add(thisString);
			}

			String *newStrings[] = new String *[newValue->Count];
			for (int i = 0; i < newValue->Count; i++)
			{
				newStrings[i] = dynamic_cast<String *>(newValue->get_Item(i));
			}
			groupKey->SetValue(groupName, newStrings);

			groupKey->Close();
			return true;
		}
		catch(...)
		{
		}

		if (groupKey)
			groupKey->Close();
		return false;
	}

	//*************************************************************************
	// Method:		AddServiceToGroup
	// Description: Adds a service to a service group
	//
	// Parameters:
	//	groupName - the name of the service group to add the service to
	//	serviceName - the service to add to the group
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ServiceHelper::AddServiceToGroup(String *groupName, String *serviceName)
	{
		RegistryKey *groupKey;
		try
		{
			groupKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICE_GROUP_KEY_NAME, true);
			if (!groupKey)
				return false;

			// read the value to see if our service name is in it
			Object *value = groupKey->GetValue(groupName);
			System::Array *strings = dynamic_cast<System::Array *>(value);
			if (!strings)
				return false;

			String *newStrings[] = new String *[strings->Length + 1];
			for (int i = 0; i < strings->Length; i++)
			{
				newStrings[i] = dynamic_cast<String *>(strings->get_Item(i));
			}
			newStrings[i] = serviceName;

			groupKey->SetValue(groupName, newStrings);

			groupKey->Close();
			return true;
		}
		catch(...)
		{
		}

		if (groupKey)
			groupKey->Close();
		return false;
	}

	//*************************************************************************
	// Method:		DeleteServiceGroup
	// Description: Deletes a service group
	//
	// Parameters:
	//	groupName - the name of the service group to remove
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ServiceHelper::DeleteServiceGroup(String *groupName)
	{
		RegistryKey *groupKey;
		try
		{
			groupKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICE_GROUP_KEY_NAME, true);
			if (!groupKey)
				return false;

			groupKey->DeleteValue(groupName);
			groupKey->DeleteSubKeyTree(groupName);

			groupKey->Close();
			return true;
		}
		catch(...)
		{
		}

		if (groupKey)
			groupKey->Close();
		return false;
	}

	//*************************************************************************
	// Method:		StartServiceToLaunch
	// Description: Starts the service to launch
	//
	// Parameters:
	//	serviceName - the name of the service to start
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ServiceHelper::StartServiceToLaunch(String *serviceName)
	{
		SiString serviceNameStr;
		Holodeck::StringConverter::GetInstance()->ConvertToSiString(serviceName, &serviceNameStr);

		SC_HANDLE scManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE service = OpenService(scManager, serviceNameStr, SERVICE_ALL_ACCESS);

		bool retVal = (StartService(service, 0, NULL) == TRUE);

		CloseServiceHandle(service);
		CloseServiceHandle(scManager);

		return retVal;
	}

	//**************************************************************************
	// Method:		GetServiceExecutablePath
	// Description: gets the .exe path of the specified service
	//
	// Parameters:
	//  serviceName - the name of the service to get the path for
	//
	// Return Value: the .exe path of the specified service
	//*************************************************************************
	String *ServiceHelper::GetServiceExecutablePath(String *serviceName)
	{
		RegistryKey *serviceBaseKey;
		RegistryKey *serviceKey;

		try
		{
			ServiceController *service = new ServiceController(serviceName);
			if (!service)
				return NULL;

			serviceBaseKey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(SERVICES_BASE_KEY_NAME, true);
			serviceKey = serviceBaseKey->OpenSubKey(service->ServiceName, true);

			String *imagePath = dynamic_cast<String *>(serviceKey->GetValue(IMAGE_PATH_VALUE_NAME));
			String *toLookFor = ".exe";

			int pos = imagePath->ToLower()->LastIndexOf(toLookFor);
			String *retVal = imagePath;
			if (pos != -1)
			{
				retVal = imagePath->Substring(0, pos + toLookFor->Length);
			}

			serviceKey->Close();
			serviceBaseKey->Close();
			return retVal;
		}
		catch(...)
		{
		}

		if (serviceKey)
			serviceKey->Close();

		if (serviceBaseKey)
			serviceBaseKey->Close();

		return NULL;
	}

	//**************************************************************************
	// Method:		GetProcessIDOfRunningService
	// Description: gets the process id of a running service
	//
	// Parameters:
	//  serviceName - the name of the service to get the process id of
	//
	// Return Value: process id of the running service, or 0 if not running
	//*************************************************************************
	unsigned long ServiceHelper::GetProcessIDOfRunningService(String *serviceName)
	{
		// Get the pid of the service
		SiString serviceNameStr;
		Holodeck::StringConverter::GetInstance()->ConvertToSiString(serviceName, &serviceNameStr);

		SC_HANDLE scHandle = OpenSCManager(NULL, NULL, GENERIC_READ);
		SC_HANDLE serviceHandle = OpenService(scHandle, serviceNameStr, GENERIC_READ);
		if (!serviceHandle)
		{
			CloseServiceHandle(serviceHandle);
			CloseServiceHandle(scHandle);
			return 0;
		}

		SERVICE_STATUS controlStatus;
		if (!ControlService(serviceHandle, SERVICE_CONTROL_INTERROGATE, &controlStatus))
		{
			CloseServiceHandle(serviceHandle);
			CloseServiceHandle(scHandle);
			return 0;
		}

		SERVICE_STATUS_PROCESS status;
		DWORD size;
		if (!QueryServiceStatusEx(serviceHandle, SC_STATUS_PROCESS_INFO, (LPBYTE)&status, sizeof(SERVICE_STATUS_PROCESS), &size))
		{
			CloseServiceHandle(serviceHandle);
			CloseServiceHandle(scHandle);
			return 0;
		}

		if (status.dwCurrentState != SERVICE_RUNNING)
		{
			CloseServiceHandle(serviceHandle);
			CloseServiceHandle(scHandle);
			return 0;
		}

		unsigned long pid = status.dwProcessId;
		CloseServiceHandle(serviceHandle);
		CloseServiceHandle(scHandle);

		return pid;
	}
}