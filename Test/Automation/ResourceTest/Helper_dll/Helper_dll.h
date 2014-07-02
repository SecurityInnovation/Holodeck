// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HELPER_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HELPER_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HELPER_DLL_EXPORTS
#define HELPER_DLL_API __declspec(dllexport)
#else
#define HELPER_DLL_API __declspec(dllimport)
#endif

#include <windows.h>
#include <string>

HELPER_DLL_API int	GetListViewItemNumber	(HWND	listViewHWnd);
HELPER_DLL_API char**	EnumListViewItemTexts	(DWORD	processID,
												 HWND	listViewHWnd);
HELPER_DLL_API char*	GetListViewItemText	(DWORD	processID,
											 HWND	listViewHWnd,
											 int	itemNum);
