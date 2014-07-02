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
	//*********************************************************************
	// internal class to handle the message balloon's tooltip
	//*********************************************************************
	__gc class BalloonTooltip : public NativeWindow
	{
	public:
		BalloonTooltip();
		~BalloonTooltip();
	};

	//*********************************************************************
	// Represents a hover balloon object
	//*********************************************************************
	public __gc class HoverBalloon : public IDisposable
	{
	public:
		HoverBalloon();
		~HoverBalloon();

		void Dispose();
		void Hide();
		void Show();

		__property void set_Title(String *value);
		__property String *get_Title() { return title; }

		__property void set_Text(String *value);
		__property String *get_Text() { return text; }

		__property void set_TitleIcon(TooltipIcon value);
		__property TooltipIcon get_TitleIcon() { return tooltipIcon; }

		__property void set_Location(Point value);
		__property Point get_Location() { return location; }

		__property void set_Parent(Control *value) { parent = value; }
		__property Control *get_Parent() { return parent; }

		__property bool get_IsVisible() { return visible; }

	private:
		void Dispose(bool disposing);
		void Display(bool show);
		void CreateTooltip();

		BalloonTooltip *tooltip;
		int maxWidth;
		String *text;
		String *title;
		TooltipIcon tooltipIcon;
		TOOLINFO *toolInfo;
		Control *parent;
		bool disposed;
		bool visible;
		Point location;
	};
}