// Helper_dll.cpp : Defines the entry point for the DLL application.
//

#include "Helper_dll.h"
#include <commctrl.h>
#include <string>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

HELPER_DLL_API int	GetListViewItemNumber	(HWND	listViewHWnd)
{
	return ListView_GetItemCount (listViewHWnd);
} // GetListViewItemNumber

HELPER_DLL_API char**	EnumListViewItemTexts	(DWORD	processID,
												 HWND	listViewHWnd)
{
	const	int	textSize = 100;

	int	i = 0;
	int	count = ListView_GetItemCount (listViewHWnd);

	HANDLE hProcess = OpenProcess (STANDARD_RIGHTS_REQUIRED | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processID);
	char *pTextProcess = (char *) VirtualAllocEx (hProcess, NULL, textSize, MEM_COMMIT, PAGE_READWRITE);
	LV_ITEM *pListViewItemProcess = (LV_ITEM *) VirtualAllocEx (hProcess, NULL, sizeof (LV_ITEM), MEM_COMMIT, PAGE_READWRITE);

	char** itemTexts = new char*[count];
	for (i=0; i<count; i++) {
		BOOL	result = FALSE;
		char	pszText[textSize];
//		std::string	message;
		ZeroMemory (pszText, textSize * sizeof (char));
		unsigned long	bytesRead = 0;
		unsigned long	bytesWritten = 0;

		LV_ITEM listViewItem;
		ZeroMemory (&listViewItem, sizeof (listViewItem));
		listViewItem.mask = LVIF_TEXT;
		listViewItem.iItem = i;
		listViewItem.iSubItem = 0;
		listViewItem.cchTextMax = textSize;
		listViewItem.pszText = pTextProcess;

		result = WriteProcessMemory (hProcess, pListViewItemProcess, &listViewItem, sizeof (listViewItem), &bytesWritten);
		if (!result) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			MessageBox (NULL, msg, "WriteProcessMemory Error", MB_OK);
			LocalFree (msg);
			msg = NULL;

			break;
		}

		result = ListView_GetItem (listViewHWnd, pListViewItemProcess);
		if (!result) {
			MessageBox (NULL, "ListView_GetItem failed", "ListView_GetItem Error", MB_OK);

			break;
		}

		result = ReadProcessMemory (hProcess, pListViewItemProcess, &listViewItem, sizeof (listViewItem), &bytesRead);
		if (!result) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			MessageBox (NULL, msg, "ReadProcessMemory Error", MB_OK);
			LocalFree (msg);
			msg = NULL;

			break;
		}

		result = ReadProcessMemory (hProcess, listViewItem.pszText, pszText, textSize, &bytesRead);
		if (!result) {
			char *msg = NULL;
			FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
			MessageBox (NULL, msg, "ReadProcessMemory Error", MB_OK);
			LocalFree (msg);
			msg = NULL;

			break;
		}

/*
		message = i;
		message += "/";
		message += count;
		message += ": ";
		message += pszText;
		MessageBox (NULL, message, "itemtext", MB_OK);
*/
//		MessageBox (NULL, pszText, "itemtext", MB_OK);
		itemTexts[i] = new char[bytesRead+1];
		strncpy (itemTexts[i], pszText, bytesRead);
		itemTexts[i][bytesRead] = '\0';
	} // for (0 <= i < count)

	return itemTexts;
} // EnumListViewItemTexts

HELPER_DLL_API char*	GetListViewItemText	(DWORD	processID,
											 HWND	listViewHWnd,
											 int	itemNum)
{
	const	int	textSize = 100;

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
	listViewItem.iItem = itemNum;
	listViewItem.iSubItem = 0;
	listViewItem.cchTextMax = textSize;
	listViewItem.pszText = pTextProcess;

	result = WriteProcessMemory (hProcess, pListViewItemProcess, &listViewItem, sizeof (listViewItem), &bytesWritten);
	if (!result) {
		char *msg = NULL;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
		MessageBox (NULL, msg, "WriteProcessMemory Error", MB_OK);
		LocalFree (msg);
		msg = NULL;

		return NULL;
	}

	result = ListView_GetItem (listViewHWnd, pListViewItemProcess);
	if (!result) {
		MessageBox (NULL, "ListView_GetItem failed", "ListView_GetItem Error", MB_OK);

		return NULL;
	}

	result = ReadProcessMemory (hProcess, pListViewItemProcess, &listViewItem, sizeof (listViewItem), &bytesRead);
	if (!result) {
		char *msg = NULL;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
		MessageBox (NULL, msg, "ReadProcessMemory Error", MB_OK);
		LocalFree (msg);
		msg = NULL;

		return NULL;
	}

	result = ReadProcessMemory (hProcess, listViewItem.pszText, pszText, textSize, &bytesRead);
	if (!result) {
		char *msg = NULL;
		FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &msg, 0, NULL);
		MessageBox (NULL, msg, "ReadProcessMemory Error", MB_OK);
		LocalFree (msg);
		msg = NULL;

		return NULL;
	}

	char *itemText = new char[bytesRead+1];
	ZeroMemory (itemText, (bytesRead + 1) * sizeof (char));
	strncpy (itemText, pszText, bytesRead);

	return itemText;
} // GetListViewItemText
