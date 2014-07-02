// Helper_dll.h

#pragma once

#include <windows.h>
#include <vector>

using namespace System;

namespace Helper_dll
{
	// Standard exception for the Helper_dll namespace
	public __gc class HelperException : public System::Exception
	{
		public:
			HelperException (String* message);
	};

	public __gc class Helper
	{
		public:
			// Gets the number of items in a ListView
			//
			// listViewHWnd - handle to the ListView
			static int	GetListViewItemNumber	(IntPtr	listViewHWnd);

			// Gets the text of a column of a row of a ListView in a process
			//
			// processID - the id of the process the ListView is in
			// listViewHWnd - handle to the ListView
			// rowNum - the row number in the ListView
			// colNum - the column number in the ListView
			//
			// might throw a HelperException
			static String*	GetListViewItemText	(DWORD	processID,
												 IntPtr	listViewHWnd,
												 int	rowNum,
												 int	colNum);

			// Gets the data from the clipboard converted to text format
			//
			// might throw a HelperException
			static String*	GetTextFromClipboard	(void);

			static void	SetTextToClipboard	(String *text);
	};
}

//--- The following code is based on http://support.microsoft.com/?id=814472 ---

public __gc class ManagedWrapper {
	public:
		static BOOL minitialize (void);
		static BOOL mterminate (void);
};
