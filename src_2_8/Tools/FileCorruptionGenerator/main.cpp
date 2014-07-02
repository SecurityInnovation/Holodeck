//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionGenerator.h
//
// DESCRIPTION: Contains definition for the file corruption classes
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 May 2003		 B. Shirey	 File created.
//*************************************************************************
#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

#include <windows.h>
#include <vcclr.h>
#include "FileCorruptionGenerator.h"
#include "FileCorruptionDialog.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Threading;
using namespace System::Windows::Forms;
using namespace FileCorruptionGenerator;

__value enum FileCorruptionType
{
	SingleBytes = 0,
	LongStrings
};

void DoCorruption(String *fileToCorrupt, String *destinationFile, unsigned int amount, FileCorruptionType type)
{
	IFileCorruptor *corruptor;

	if (type == FileCorruptionType::SingleBytes)
		corruptor = new RandomByteCorruptor(amount);
	else if (type == FileCorruptionType::LongStrings)
		corruptor = new LongStringCorruptor(amount);

	corruptor->GenerateFile(fileToCorrupt, destinationFile);
}

// This is the entry point for this application
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Thread::CurrentThread->ApartmentState = ApartmentState::STA;

		String *Args[] = Environment::GetCommandLineArgs();
		if (Args->Length <= 1)
		{
			FileCorruptionDialog *dialog = new FileCorruptionDialog();
			dialog->ShowDialog();

			Cursor::Current = Cursors::WaitCursor;

			if (dialog->DialogResult == DialogResult::OK)
			{
				FileCorruptionType type;
				if (dialog->SingleByteCorruption)
					type = FileCorruptionType::SingleBytes;
				else if (dialog->LongStringCorruption)
					type = FileCorruptionType::LongStrings;

				DoCorruption(dialog->FileToCorrupt, dialog->DestinationFile, dialog->CorruptionAmount, type);

				Cursor::Current = Cursors::Default;
				String *msg = String::Concat("File Corruption Successful.  Corrupted File stored at ", dialog->DestinationFile);
				const __wchar_t __pin *msgString = PtrToStringChars(msg);
				MessageBoxW(NULL, msgString, L"File Corruption Generator", MB_ICONINFORMATION | MB_TOPMOST | MB_TASKMODAL);
			}
		}
		else if (Args->Length != 5)
		{
			Cursor::Current = Cursors::Default;
			String *msg;
			msg = "FileCorruptionGenerator <src_file> <dest_file> <random|longstring> <amount>\n";
			msg = String::Concat(msg, "  where random does random single byte corruption and amount indicates the\n");
			msg = String::Concat(msg, "  number of bytes to corrupt, and longstring does long string insertion and\n");
			msg = String::Concat(msg, "  amount indicates the length of the string to insert.");

			const __wchar_t __pin *msgString = PtrToStringChars(msg);
			MessageBoxW(NULL, msgString, L"File Corruption Generator", MB_ICONINFORMATION | MB_TOPMOST | MB_TASKMODAL);
		}
		else
		{
			try
			{
				FileCorruptionType type;
				if (Args[3]->ToUpper()->Equals("RANDOM"))
					type = FileCorruptionType::SingleBytes;
				else if (Args[3]->ToUpper()->Equals("LONGSTRING"))
					type = FileCorruptionType::LongStrings;
				else
				{
					MessageBoxW(NULL, L"Invalid type:  Use either random or longstring", L"File Corruption Generator", MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);
					return -1;
				}

				DoCorruption(Args[1], Args[2], UInt32::Parse(Args[4]), type);
			}
			catch(Exception *)
			{
				String *msg = "Error occurred.  File corruption did not complete successfully.\n\n";
				msg = String::Concat(msg, "Most likely reason is that one of the files does not exist or you do not have permissions to corrupt that file.");

				const __wchar_t __pin *msgString = PtrToStringChars(msg);
				MessageBoxW(NULL, msgString, L"File Corruption Generator", MB_ICONERROR | MB_TOPMOST | MB_TASKMODAL);
			}
		}

		return 0;
	}
	catch(Exception *)
	{
		const wchar_t __pin* wszMessage = L"Error occurred.  File corruption did not complete successfully.\n\nMost likely reason is that one of the files does not exist or you do not have permissions to corrupt that file.";
		MessageBoxW(NULL, wszMessage, L"Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
	}
	catch(...)
	{
		MessageBox(NULL, "Unknown exception has occurred.  Exiting File Corruption Generator...", "Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
	}
}