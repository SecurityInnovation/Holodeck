//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		MessageBalloon.cpp
//
// DESCRIPTION: Contains implementation for the class MessageBalloon
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 7 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#include "MessageBalloon.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		MessageBalloonTooltip
	// Description: Constructor for the MessageBalloonTooltip class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MessageBalloonTooltip::MessageBalloonTooltip()
	{
	}

	//*************************************************************************
	// Method:		~MessageBalloonTooltip
	// Description: Destructor for the MessageBalloonTooltip class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MessageBalloonTooltip::~MessageBalloonTooltip()
	{
	}

	//*************************************************************************
	// Method:		WndProc
	// Description: main window message handling procedure for the MessageBalloon
	//	tooltip class
	//
	// Parameters:
	//	m - the message to process
	//
	// Return Value: None
	//*************************************************************************
	void MessageBalloonTooltip::WndProc(Message *m)
	{
		if (m->Msg == WM_NCHITTEST)
		{
			if (OnDeActivateWindow)
				OnDeActivateWindow->Invoke();
		}

		__super::WndProc(m);
	}

	//*************************************************************************
	// Method:		MessageBalloon
	// Description: Constructor for the MessageBalloon class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MessageBalloon::MessageBalloon()
	{
		tooltip = new MessageBalloonTooltip();
		tooltip->OnDeActivateWindow += new DeActivateWindowDelegate(this, OnTooltipMouseOver);
		maxWidth = 250;
		text = "Control Display Message";
		title = "Tooltip Message";
		tooltipIcon = TooltipIcon::None;
		alignment = BalloonAlignment::TopRight;
		absolute = false;
		centerStem = false;
		disposed = false;
		hideOnMouseOver = false;
		visible = false;
	}

	//*************************************************************************
	// Method:		~MessageBalloon
	// Description: Destructor for the MessageBalloon class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MessageBalloon::~MessageBalloon()
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
	void MessageBalloon::Dispose()
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
	void MessageBalloon::Dispose(bool disposing)
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
	void MessageBalloon::CreateTooltip()
	{
		if (!parent)
			return;

		CreateParams *params = new CreateParams();
		params->ClassName = "tooltips_class32";
		params->Style = WS_POPUP | TTS_BALLOON | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_CLOSE;
		tooltip->CreateHandle(params);

		toolInfo = new TOOLINFO;
		toolInfo->cbSize = sizeof(TOOLINFO);
		toolInfo->uFlags = TTF_TRACK | TTF_IDISHWND | TTF_TRANSPARENT | TTF_SUBCLASS | TTF_PARSELINKS;

		if (absolute)
			toolInfo->uFlags |= TTF_ABSOLUTE;
		if (centerStem)
			toolInfo->uFlags |= TTF_CENTERTIP;

		toolInfo->uId = tooltip->Handle.ToInt32();

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

		GetClientRect(toolInfo->hwnd, &toolInfo->rect);
		POINT point;
		point.x = toolInfo->rect.left;
		point.y = toolInfo->rect.top;
		int xDiff = toolInfo->rect.right - toolInfo->rect.left;
		int yDiff = toolInfo->rect.bottom - toolInfo->rect.top;
		ClientToScreen(toolInfo->hwnd, &point);

		toolInfo->rect.left = point.x;
		toolInfo->rect.right = toolInfo->rect.left + xDiff;
		toolInfo->rect.top = point.y;
		toolInfo->rect.bottom = toolInfo->rect.top + yDiff;

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

		if (asciiTextStr)
			delete[] asciiTextStr;

		SetBalloonPosition(toolInfo->rect);
	}

	//*************************************************************************
	// Method:		SetBalloonPosition
	// Description: sets the location of the balloon
	//
	// Parameters:
	//	rect - the rectangle to set the balloon at
	//
	// Return Value: None
	//*************************************************************************
	void MessageBalloon::SetBalloonPosition(RECT rect)
	{
		int x = 0;
		int y = 0;

		switch(alignment)
		{
		case BalloonAlignment::TopLeft:
			x = rect.left;
			y = rect.top;
			break;
		case BalloonAlignment::TopMiddle:
			x = rect.left + (rect.right / 2);
			y = rect.top;
			break;
		case BalloonAlignment::TopRight:
			x = rect.left + rect.right;
			y = rect.top;
			break;
		case BalloonAlignment::LeftMiddle:
			x = rect.left;
			y = rect.top + (rect.bottom / 2);
			break;
		case BalloonAlignment::RightMiddle:
			x = rect.left + rect.right;
			y = rect.top + (rect.bottom / 2);
			break;
		case BalloonAlignment::BottomLeft:
			x = rect.left;
			y = rect.top + rect.bottom;
			break;
		case BalloonAlignment::BottomMiddle:
			x = rect.left + (rect.right / 2);
			y = rect.top + rect.bottom;
			break;
		case BalloonAlignment::BottomRight:
			x = rect.left + rect.right;
			y = rect.top + rect.bottom;
			break;
		}

		SendMessage((HWND)tooltip->Handle.ToPointer(), TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(x, y));
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
	void MessageBalloon::Display(bool show)
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
	void MessageBalloon::Hide()
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
	void MessageBalloon::Show()
	{
		Hide();
		CreateTooltip();
		Display(true);
	}

	//*************************************************************************
	// Method:		OnTooltipMouseOver
	// Description: called when the user moves the mouse onto the message balloon tooltip
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MessageBalloon::OnTooltipMouseOver()
	{
		if (hideOnMouseOver)
			Hide();
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
	void MessageBalloon::set_Text(String *value)
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
	void MessageBalloon::set_Title(String *value)
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
	void MessageBalloon::set_TitleIcon(TooltipIcon value)
	{
		tooltipIcon = value;
		if (visible && tooltip && toolInfo)
		{
			set_Title(title);
		}
	}
}