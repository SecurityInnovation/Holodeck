//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		HoverBalloon.cpp
//
// DESCRIPTION: Contains implementation for the class HoverBalloon
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 8 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#include "HoverBalloon.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		BalloonTooltip
	// Description: Constructor for the BalloonTooltip class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	BalloonTooltip::BalloonTooltip()
	{
	}

	//*************************************************************************
	// Method:		~BalloonTooltip
	// Description: Destructor for the BalloonTooltip class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	BalloonTooltip::~BalloonTooltip()
	{
	}

	//*************************************************************************
	// Method:		HoverBalloon
	// Description: Constructor for the HoverBalloon class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HoverBalloon::HoverBalloon()
	{
		tooltip = new BalloonTooltip();
		maxWidth = 250;
		text = "";
		title = "";
		tooltipIcon = TooltipIcon::None;
		disposed = false;
		location = Point(0, 0);
		parent = NULL;
		visible = false;
	}

	//*************************************************************************
	// Method:		~HoverBalloon
	// Description: Destructor for the HoverBalloon class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HoverBalloon::~HoverBalloon()
	{
		Dispose(false);
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Cleans up the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::Dispose()
	{
		Dispose(true);
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Cleans up the class
	//
	// Parameters:
	//	disposing - true if disposing, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::Dispose(bool disposing)
	{
		if (!disposed)
		{
			Hide();
		}
		disposed = true;
	}

	//*************************************************************************
	// Method:		CreateTooltip
	// Description: creates the tooltip internally
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::CreateTooltip()
	{
		if (!parent)
			return;

		CreateParams *params = new CreateParams();
		params->ClassName = "tooltips_class32";
		params->Style = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
		params->Parent = parent->Handle;
		tooltip->CreateHandle(params);

		toolInfo = new TOOLINFO;
		toolInfo->cbSize = sizeof(TOOLINFO);
		toolInfo->uFlags = TTF_TRANSPARENT | TTF_SUBCLASS | TTF_TRACK;
		toolInfo->hwnd = (HWND)parent->Handle.ToPointer();

		GetClientRect((HWND)parent->Handle.ToPointer(), &toolInfo->rect);

		char *asciiTextStr = NULL;
		if (text)
		{
			const __wchar_t __pin *textStr = PtrToStringChars(text);
			asciiTextStr = new char[text->Length + 1];
			WideCharToMultiByte(CP_ACP, 0, textStr, -1, asciiTextStr, text->Length + 1, NULL, NULL);
			asciiTextStr[text->Length] = '\0';
			toolInfo->lpszText = asciiTextStr;
		}

		toolInfo->hwnd = (HWND)parent->Handle.ToPointer();

		int width = toolInfo->rect.right - toolInfo->rect.left;
		int height = toolInfo->rect.bottom - toolInfo->rect.top;

		toolInfo->rect.left = location.X;
		toolInfo->rect.top = location.Y;
		toolInfo->rect.right = location.X + width;
		toolInfo->rect.bottom = location.Y + height;

		HWND tooltipHWND = (HWND)tooltip->Handle.ToPointer();

		// make it the top level window
		SetWindowPos(tooltipHWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

		SendMessage(tooltipHWND, TTM_ADDTOOL, 0, (LPARAM)toolInfo);
		SendMessage(tooltipHWND, TTM_SETMAXTIPWIDTH, 0, maxWidth);

		if (title)
		{
			const __wchar_t __pin *titleStr = PtrToStringChars(title);
			char *str = new char[title->Length + 1];
			WideCharToMultiByte(CP_ACP, 0, titleStr, -1, str, title->Length + 1, NULL, NULL);
			str[title->Length] = '\0';
			SendMessage(tooltipHWND, TTM_SETTITLE, (int)tooltipIcon, (LPARAM)str);
			delete[] str;
		}

		SendMessage((HWND)tooltip->Handle.ToPointer(), TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(location.X, location.Y));

		if (asciiTextStr)
			delete[] asciiTextStr;
	}

	//*************************************************************************
	// Method:		Display
	// Description: displays the balloon
	//
	// Parameters:
	//	show - true to show, false to hide
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::Display(bool show)
	{
		SendMessage((HWND)tooltip->Handle.ToPointer(), TTM_TRACKACTIVATE, show ? -1 : 0, (LPARAM)toolInfo);
		visible = show;
	}

	//*************************************************************************
	// Method:		Hide
	// Description: hides the balloon
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::Hide()
	{
		Display(false);
		tooltip->DestroyHandle();
	}

	//*************************************************************************
	// Method:		Show
	// Description: shows the balloon
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::Show()
	{
		Hide();
		CreateTooltip();
		Display(true);
	}

	//*************************************************************************
	// Method:		set_Text
	// Description: sets the tooltip's text
	//
	// Parameters:
	//	value - the text to set
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::set_Text(String *value)
	{
		text = value;
		if (visible && tooltip && toolInfo && value)
		{
			char *asciiTextStr = NULL;
			const __wchar_t __pin *textStr = PtrToStringChars(text);
			asciiTextStr = new char[text->Length + 1];
			WideCharToMultiByte(CP_ACP, 0, textStr, -1, asciiTextStr, text->Length + 1, NULL, NULL);
			asciiTextStr[text->Length] = '\0';
			toolInfo->lpszText = asciiTextStr;
			SendMessage((HWND)tooltip->Handle.ToPointer(), TTM_UPDATETIPTEXT, 0, (LPARAM)toolInfo);
			delete[] asciiTextStr;
		}
	}

	//*************************************************************************
	// Method:		set_Title
	// Description: sets the tooltip's title
	//
	// Parameters:
	//	value - the title to set
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::set_Title(String *value)
	{
		title = value;
		if (visible && tooltip && toolInfo && value)
		{
			char *asciiTitleStr = NULL;
			const __wchar_t __pin *titleStr = PtrToStringChars(title);
			asciiTitleStr = new char[title->Length + 1];
			WideCharToMultiByte(CP_ACP, 0, titleStr, -1, asciiTitleStr, title->Length + 1, NULL, NULL);
			asciiTitleStr[title->Length] = '\0';
			SendMessage((HWND)tooltip->Handle.ToPointer(), TTM_SETTITLE, (int)tooltipIcon, (LPARAM)asciiTitleStr);
			delete[] asciiTitleStr;
		}
	}

	//*************************************************************************
	// Method:		set_TitleIcon
	// Description: sets the tooltip's title icon
	//
	// Parameters:
	//	value - the title icon to set
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::set_TitleIcon(TooltipIcon value)
	{
		tooltipIcon = value;
		if (visible && tooltip && toolInfo)
		{
			set_Title(title);
		}
	}

	//*************************************************************************
	// Method:		set_Location
	// Description: sets the tooltip's location
	//
	// Parameters:
	//	value - the location to set
	//
	// Return Value: None
	//*************************************************************************
	void HoverBalloon::set_Location(Point value)
	{
		location = value;
		if (visible && tooltip && toolInfo)
		{
			SendMessage((HWND)tooltip->Handle.ToPointer(), TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(location.X, location.Y));
		}
	}
}