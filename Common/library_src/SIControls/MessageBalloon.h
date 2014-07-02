//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		MessageBalloon.h
//
// DESCRIPTION: Contains definition for the class MessageBalloon
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 7 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <commctrl.h>
#include <vcclr.h>
#include "ToolTipEnums.h"

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Text;
using namespace System::Windows::Forms;

namespace SIControls
{
	public __delegate void DeActivateWindowDelegate();

	//*********************************************************************
	// internal class to handle the message balloon's tooltip
	//*********************************************************************
	__gc class MessageBalloonTooltip : public NativeWindow
	{
	public:
		MessageBalloonTooltip();
		~MessageBalloonTooltip();

		DeActivateWindowDelegate *OnDeActivateWindow;

	protected:
		void WndProc(Message *m);
	};

	//*********************************************************************
	// Represents a message balloon object
	//*********************************************************************
	public __gc class MessageBalloon : public IDisposable
	{
	public:
		MessageBalloon();
		~MessageBalloon();

		void Dispose();
		void Hide();
		void Show();

		__property void set_Title(String *value);
		__property String *get_Title() { return title; }

		__property void set_Text(String *value);
		__property String *get_Text() { return text; }

		__property void set_TitleIcon(TooltipIcon value);
		__property TooltipIcon get_TitleIcon() { return tooltipIcon; }

		__property void set_Parent(Control *value) { parent = value; }
		__property Control *get_Parent() { return parent; }

		__property void set_Alignment(BalloonAlignment value) { alignment = value; }
		__property BalloonAlignment get_Alignment() { return alignment; }

		__property void set_UseAbsolutePositioning(bool value) { absolute = value; }
		__property bool get_UseAbsolutePositioning() { return absolute; }

		__property void set_CenterStem(bool value) { centerStem = value; }
		__property bool get_CenterStem() { return centerStem; }

		__property void set_HideOnMouseOver(bool value) { hideOnMouseOver = value; }
		__property bool get_HideOnMouseOver() { return hideOnMouseOver; }

		__property bool get_IsVisible() { return visible; }

	private:
		void Dispose(bool disposing);
		void CreateTooltip();
		void SetBalloonPosition(RECT rect);
		void Display(bool show);
		void OnTooltipMouseOver();

		MessageBalloonTooltip *tooltip;
		Control *parent;
		TOOLINFO *toolInfo;
		int maxWidth;
		String *text;
		String *title;
		TooltipIcon tooltipIcon;
		BalloonAlignment alignment;
		bool absolute;
		bool centerStem;
		bool disposed;
		bool hideOnMouseOver;
		bool visible;
	};
}