//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		UserNotification.h
//
// DESCRIPTION: Contains definition for the class UserNotification
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Jun 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "defines.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;

namespace HolodeckGui
{

	//*********************************************************************
	// used to notify the user of an error or other important message
	//*********************************************************************
	public __gc class UserNotification
	{
	public:
		__property static void set_SilentMode(bool value) 
		{ 
			silentMode = value; 

			if (silentMode)
			{
				fileWriter = File::CreateText(ERROR_LOG_FILE);
				fileWriter->AutoFlush = true;
			}
			else if (fileWriter)
			{
				fileWriter->Close();
				fileWriter = 0;
			}
		}

		UserNotification();
		~UserNotification();

		static void ErrorNotify(String *message);
		static void InfoNotify(String *message);
		static int AskNotify(String *message);

	private:
		static bool silentMode;
		static StreamWriter *fileWriter;
	};
}