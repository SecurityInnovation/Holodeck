//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StringTools.h
//
// DESCRIPTION: Contains definition for the class StringTools
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>

using namespace System;
using namespace System::Drawing;

namespace SIControls
{
	//*********************************************************************
	// Provides some common string operations used by the SIControls project
	//*********************************************************************
	public __gc class StringTools
	{
	private:
		StringTools();
	public:
		static SizeF MeasureDisplayString(Graphics *graphics, String *text, Font *font);
		static int MeasureDisplayStringWidth(Graphics *graphics, String *text, Font *font);
	};
}