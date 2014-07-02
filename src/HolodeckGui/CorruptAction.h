//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CorruptAction.h
//
// DESCRIPTION: Represents a single action in a corruption changes file
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#include "defines.h"

using namespace System;

namespace UIData
{
	public __value enum CorruptActionType
	{
		ReplaceCorrupt,
		InsertCorrupt,
		DeleteCorrupt
	};

	public __gc class CorruptAction
	{
	private:
		CorruptActionType type;
		Int64 offset;
		int count;
		Byte oldValue __gc[];
		Byte newValue __gc[];

	public:
		__property CorruptActionType get_ActionType() { return type; }
		__property void set_ActionType(CorruptActionType value) { type = value; }

		__property Int64 get_Offset() { return offset; }
		__property void set_Offset(Int64 value) { offset = value; }

		__property int get_ByteCount() { return count; }
		__property void set_ByteCount(int value) { count = value; }

		__property Byte get_OldValue() __gc[] { return oldValue; }
		__property void set_OldValue(Byte value __gc[]) { oldValue = value; }

		__property Byte get_NewValue() __gc[] { return newValue; }
		__property void set_NewValue(Byte value __gc[]) { newValue = value; }
	};
};