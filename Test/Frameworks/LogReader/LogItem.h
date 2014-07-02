//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogItem.h
//
// DESCRIPTION: Contains definition for the log item class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <memory.h>
#include <stdio.h>
#include "ManagedEnums.h"

#using <mscorlib.dll>

using namespace Holodeck;
using namespace System;
using namespace System::Collections;

namespace HoloScript
{
	public __value enum LogEventType
	{
		FileCorruptionEvent = 1
	};

	public __gc class LogEvent
	{
	private:
		LogEventType type;
		unsigned char value __gc[];
		String* stringValue;

	public:
		LogEvent() {}
		~LogEvent() {}

		__property LogEventType get_Type() { return type; }
		__property void set_Type(LogEventType t) { type = t; }

		__property unsigned char get_Bytes() __gc[] { return value; }
		__property void set_Bytes(unsigned char v __gc[]) { value = v; }

		__property String* get_StringValue() { return stringValue; }
		__property void set_StringValue(String* v) { stringValue = v; }
	};

	//*************************************************************************
	// managed class to hold a log item
	//*************************************************************************
	public __gc class LogItem
	{
	private:
		UInt64 uint64TimeStamp;
		String *timeStamp;
		String *threadID;
		String *category;
		String *dllName;
		String *function;
		String *errorCode;
		String *exception;
		String *returnValue;
		ArrayList *parameters;
		ArrayList *events;			// v2_5 and later
		unsigned int callDepth;			// v2_5 and later
		HolodeckVersion version;
		unsigned long filePos;

		String *ConvertTimeToString(SYSTEMTIME systemTime);
		void Initialize(HolodeckVersion v);

	public:
		LogItem();
		LogItem(HolodeckVersion v);
		~LogItem();

		__property UInt64 get_UInt64TimeStamp() { return uint64TimeStamp; }

		__property String *get_TimeStamp() { return timeStamp; }
		__property void set_TimeStamp(String * value) { timeStamp = value; }

		__property String *get_CallDepth() { return __box(callDepth)->ToString(); }
		__property void set_CallDepth(String *value) { callDepth = UInt32::Parse(value); }

		__property unsigned int get_UInt32CallDepth() { return callDepth; }
		__property void set_UInt32CallDepth(unsigned int value) { callDepth = value; }

		__property String *get_ThreadID() { return threadID; }
		__property void set_ThreadID(String * value) { threadID = value; }

		__property String *get_Category() { return category; }
		__property void set_Category(String * value) { category = value; }

		__property String *get_DllName() { return dllName; }
		__property void set_DllName(String * value) { dllName = value; }
		
		__property String *get_Function() { return function; }
		__property void set_Function(String * value) { function = value; }

		__property String *get_ErrorCode() { return errorCode; }
		__property void set_ErrorCode(String * value) { errorCode = value; }

		__property String *get_Exception() { return exception; }
		__property void set_Exception(String * value) { exception = value; }

		__property String *get_ReturnValue() { return returnValue; }
		__property void set_ReturnValue(String * value) { returnValue = value; }

		__property ArrayList *get_Parameters() { return parameters; }
		__property void set_Parameters(ArrayList * value) { parameters = value; }

		__property ArrayList *get_Events() { return events; }
		__property void set_Events(ArrayList * value) { events = value; }

		__property unsigned long get_FilePosition() { return filePos; }
		__property void set_FilePosition(unsigned long value) { filePos = value; }

		void ConvertFromByteSequence(unsigned char *buffer);
	};
}