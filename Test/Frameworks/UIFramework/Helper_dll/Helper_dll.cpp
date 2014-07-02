// This is the main DLL file.

#include "Helper_dll.h"
#include <commctrl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "_vcclrit.h"
#using <mscorlib.dll>

namespace Helper_dll
{
	HelperException::HelperException (String* message) : System::Exception (message)
	{
	}

	int	Helper::GetListViewItemNumber	(IntPtr	listViewHWnd)
	{
		return ListView_GetItemCount ((HWND) listViewHWnd.ToPointer ());
	} // GetListViewItemNumber

	String*	Helper::GetListViewItemText	(DWORD	processID,
										 IntPtr	listViewHWnd,
										 int	rowNum,
										 int	colNum)
	{
		const	int	textSize = 1024; // TODO: figure out the correct size
		HANDLE hProcess = OpenProcess (STANDARD_RIGHTS_REQUIRED | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processID);
		char *pTextProcess = (char *) VirtualAllocEx (hProcess, NULL, textSize, MEM_COMMIT, PAGE_READWRITE);
		LV_ITEM *pListViewItemProcess = (LV_ITEM *) VirtualAllocEx (hProcess, NULL, sizeof (LV_ITEM), MEM_COMMIT, PAGE_READWRITE);

		BOOL	result = FALSE;
		char	pszText[textSize];
		ZeroMemory (pszText, textSize * sizeof (char));
		unsigned long	bytesRead = 0;
		unsigned long	bytesWritten = 0;

		LV_ITEM listViewItem;
		ZeroMemory (&listViewItem, sizeof (listViewItem));
		listViewItem.mask = LVIF_TEXT;
		listViewItem.iItem = rowNum;
		listViewItem.iSubItem = colNum;
		listViewItem.cchTextMax = textSize;
		listViewItem.pszText = pTextProcess;

		result = WriteProcessMemory (hProcess, pListViewItemProcess, &listViewItem, sizeof (listViewItem), &bytesWritten);
		if (!result) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;
			throw new HelperException ((new String ("WriteProcessMemory Error - "))->Concat (message));
		}

		result = ListView_GetItem ((HWND) listViewHWnd.ToPointer (), pListViewItemProcess);
		if (!result) {
			throw new HelperException ("ListView_GetItem Error - ListView_GetItem failed");
		}

		result = ReadProcessMemory (hProcess, pListViewItemProcess, &listViewItem, sizeof (listViewItem), &bytesRead);
		if (!result) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("ReadProcessMemory Error - "))->Concat (message));
		}

		result = ReadProcessMemory (hProcess, listViewItem.pszText, pszText, textSize, &bytesRead);
		if (!result) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("ReadProcessMemory Error - "))->Concat (message));
		}

		String *itemText = new String (pszText);

		return itemText;
	} // GetListViewItemText

	String*	Helper::GetTextFromClipboard	(void)
	{
		if (!OpenClipboard (NULL)) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("OpenClipboard Error - "))->Concat (message));
		}

		HANDLE hGlb = GetClipboardData (CF_UNICODETEXT);
		if (hGlb == NULL) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("GetClipboardData Error - "))->Concat (message));
		}

		wchar_t *text = (wchar_t *) GlobalLock (hGlb);
		if (text == NULL) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);

			throw new HelperException ((new String ("GetClipboardData Error - "))->Concat (message));
		}

		String *clipboardText = new String (text);

		GlobalUnlock (hGlb);

		if (!CloseClipboard ()) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("CloseClipboard Error - "))->Concat (message));
		}

		return clipboardText;
	}

	void	Helper::SetTextToClipboard	(String *text)
	{
		long textLength = text->Length;

		if (!OpenClipboard (NULL)) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("OpenClipboard Error - "))->Concat (message));
		}

		HANDLE hGlb = GlobalAlloc (GHND, (text->Length + 1) * sizeof (wchar_t));
		if (hGlb == NULL) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("GlobalAlloc Error - "))->Concat (message));
		}

		wchar_t *textToCopy = (wchar_t *) GlobalLock (hGlb);
		if (hGlb == NULL) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("GlobalLock Error - "))->Concat (message));
		}

		System::CharEnumerator *textEnumerator = text->GetEnumerator ();
		textEnumerator->Reset ();
		for (int i = 0; i < text->Length; i++) {
			textEnumerator->MoveNext ();
			textToCopy[i] = textEnumerator->Current;
		}

		GlobalUnlock (hGlb);
		textToCopy = NULL;

		if (SetClipboardData (CF_UNICODETEXT, hGlb) == NULL) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("SetClipboardData Error - "))->Concat (message));
		}

		if (!CloseClipboard ()) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			String *message = new String (msg);
			LocalFree (msg);
			msg = NULL;

			throw new HelperException ((new String ("CloseClipboard Error - "))->Concat (message));
		}
	}

} // namespace

//--- The following code is based on http://support.microsoft.com/?id=814472 ---

// This code verifies that DllMain is not automatically called 
// by the Loader when linked with /noentry. It also checks some
// functions that the CRT initializes.

BOOL ManagedWrapper::minitialize (void)
{
	BOOL retval = TRUE;
	try {
		retval =  __crt_dll_initialize();
	} catch(System::Exception* e) {
		Console::WriteLine(e->Message);
		retval = FALSE;
	}
	return retval;
}

BOOL ManagedWrapper::mterminate (void)
{
	BOOL retval = TRUE;
	try {
		retval = __crt_dll_terminate();
	} catch(System::Exception* e) {
		Console::WriteLine(e->Message);
		retval = FALSE;
	}
	return retval;
}

BOOL WINAPI DllMain (HINSTANCE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	Console::WriteLine(S"DllMain is called...");

	return TRUE;
} /* DllMain */
