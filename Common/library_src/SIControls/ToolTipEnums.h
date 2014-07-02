//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		ToolTipEnums.h
//
// DESCRIPTION: Contains definition of common enums used by tooltips
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 7 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#pragma once

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
	public __value enum TooltipIcon
	{
		None,
		Info,
		Warning,
		Error
	};

	public __value enum BalloonAlignment
	{
		TopLeft,
		TopMiddle,
		TopRight,
		LeftMiddle,
		RightMiddle,
		BottomLeft,
		BottomMiddle,
		BottomRight
	};

	public __value enum BalloonPosition
	{
		Absolute,
		Track
	};
}