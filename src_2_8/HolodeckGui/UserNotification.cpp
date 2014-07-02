//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		UserNotification.cpp
//
// DESCRIPTION: Contains implementation for the class UserNotification
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Jun 2003		 J.Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include <vcclr.h>
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		UserNotification
	// Description: Constructor for the UserNotification class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	UserNotification::UserNotification()
	{
	}

	//*************************************************************************
	// Method:		~UserNotification
	// Description: Destructor for the UserNotification class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	UserNotification::~UserNotification()
	{
	}

	//*************************************************************************
	// Method:		ErrorNotify
	// Description: Notifies the user of an error
	//
	// Parameters:
	//	message - string to present to the user
	//
	// Return Value: None
	//*************************************************************************
	void UserNotification::ErrorNotify(String *message)
	{
		if (silentMode)
			fileWriter->WriteLine(String::Concat("Error: ", message));
		else
		{
			const __wchar_t __pin *msgString = PtrToStringChars(message);
			MessageBoxW(NULL, msgString, L"Holodeck", MB_OK | MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);
		}
	}

	//*************************************************************************
	// Method:		StatusNotify
	// Description:  Gives the user a status notification
	//
	// Parameters:
	//	message - string to present to the user
	//
	// Return Value: None
	//*************************************************************************
	void UserNotification::InfoNotify(String *message)
	{
		if (silentMode)
			fileWriter->WriteLine(String::Concat("Info: ", message));
		else
		{
			const __wchar_t __pin *msgString = PtrToStringChars(message);
			MessageBoxW(NULL, msgString, L"Holodeck", MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_TASKMODAL);
		}
	}


	//*************************************************************************
	// Method:		AskNotify
	// Description: Asks a question and get a Yes or No result
	//				If in silent mode, does not ask the question and
	//				assumes return value as IDNO
	//
	// Parameters:
	//	message - string to present to the user
	//
	// Return Value: IDNO or IDYES depending on the user selection
	//*************************************************************************
	int UserNotification::AskNotify(String *message)
	{
		if (silentMode)
		{
			fileWriter->WriteLine(String::Concat("Question: ", message, "\tAssumed answer NO"));
			return IDNO;
		}
		else
		{
			const __wchar_t __pin *msgString = PtrToStringChars(message);
			return MessageBoxW(NULL, msgString, L"Holodeck", MB_YESNO | MB_ICONQUESTION | MB_TOPMOST | MB_TASKMODAL);
		}
	}
}