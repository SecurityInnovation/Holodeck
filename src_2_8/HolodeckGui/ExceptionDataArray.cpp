//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ExceptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 Feb 2004		 R. Wagner	 File created.
//*************************************************************************
#include <windows.h>
#include "ExceptionDataArray.h"
#include "UserNotification.h"

using namespace System::IO;
using namespace HolodeckGui;

namespace UIData
{
	//*************************************************************************
	// Method:		ExceptionDataArray
	// Description: Constructor for the ExceptionDataArray class
	//
	// Parameters:
	//	procID - the process id for the array
	//
	// Return Value: None
	//*************************************************************************
	ExceptionDataArray::ExceptionDataArray(unsigned int procID)
	{
		processID = procID;
		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		DeleteExceptionHandler = new DeleteExceptionDelegate(this, onDeleteException);
		dataEventRegistry->OnDeleteException += DeleteExceptionHandler;
		CreateExceptionHandler = new CreateExceptionDelegate(this, onCreateException);
		dataEventRegistry->OnCreateException += CreateExceptionHandler;

		if (holoScriptApp)
		{
			exceptionHandler = new HoloScript::ExceptionThrownDelegate(this, onExceptionThrown);
			holoScriptApp->OnExceptionThrown += exceptionHandler;
		}
	}

	//*************************************************************************
	// Method:		~ExceptionDataArray
	// Description: Destructor for the FileCorruptionDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ExceptionDataArray::~ExceptionDataArray()
	{
		CleanUp();
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Cleans up all dataarray delegates
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ExceptionDataArray::CleanUp()
	{
		if (holoScriptApp)
			holoScriptApp->OnExceptionThrown -= exceptionHandler;

		dataEventRegistry->OnDeleteException -= DeleteExceptionHandler;
		dataEventRegistry->OnCreateException -= CreateExceptionHandler;
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateException
	// Description: Called when a ui pane creates an exception
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void ExceptionDataArray::onCreateException(DisplayableDataNode *dataNode)
	{
		Object *args[] = __gc new Object*[1];
		ExceptionDataNode *eDataNode;

		if (!dataNode->CreatedDelegate)
			return;
		else if (dataNode->processID == processID)
		{
			eDataNode = dynamic_cast<ExceptionDataNode *>(dataNode);
			if (!eDataNode)
				return;
			
			eDataNode->Name = eDataNode->Exception;
			eDataNode->ID = eDataNode->MiniDumpFile;

			CreateDataNode(eDataNode);
			args[0] = eDataNode;
			eDataNode->CreatedDelegate->DynamicInvoke(args);
		}
	}

	//*************************************************************************
	// Method:		onDeleteException
	// Description: Called when a ui pane deletes an exception
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void ExceptionDataArray::onDeleteException(DisplayableDataNode *dataNode)
	{
		if (!dataNode->DeletedDelegate)
			return;
		else
		{
			// Datanode ID holds path to exception minidump, delete this file
			if (File::Exists(dataNode->ID))
			{
				try
				{
					File::Delete(dataNode->ID);
				}
				catch (Exception*)
				{
					UserNotification::ErrorNotify(String::Concat("Unable to delete mini dump file at ", dataNode->ID));
				}
			}

			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);
			DeleteDataNode(dataNode);
		}
	}

	//*************************************************************************
	// Method:		onExceptionThrown
	// Description: Called when an exception is generated for the process
	//
	// Parameters:
	//	pid - process id of the exception
	//  info - exception information
	//
	// Return Value: None
	//*************************************************************************
	void ExceptionDataArray::onExceptionThrown(unsigned int pid, HoloScript::ExceptionInfo info)
	{
		if (processID != pid)
			return; // not our process

		ExceptionDataNode* node = new ExceptionDataNode("Exception", pid, info.threadId);
		node->ExeName = holoScriptApp->ApplicationName;
		node->TimeStamp = info.timeStamp;
		node->MiniDumpFile = info.miniDumpFile;

		String* chance;
		if (info.firstChance)
			chance = S"First chance ";
		else
			chance = S"Second chance ";

		switch (info.exceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			node->Exception = String::Concat(chance, S"access violation while ", info.writeAccess ? S"writing to 0x" : S"reading from 0x",
				__box(info.memoryAddress)->ToString("X8"), S" at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			node->Exception = String::Concat(chance, S"array bounds exceeded at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_BREAKPOINT:
			node->Exception = String::Concat(chance, S"user breakpoint at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			node->Exception = String::Concat(chance, S"data misalignment at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			node->Exception = String::Concat(chance, S"floating point denormal operand at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			node->Exception = String::Concat(chance, S"floating point divide by zero at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			node->Exception = String::Concat(chance, S"inexact floating point result at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			node->Exception = String::Concat(chance, S"invalid floating point operation at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_OVERFLOW:
			node->Exception = String::Concat(chance, S"floating point overflow at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			node->Exception = String::Concat(chance, S"floating point stack exception at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			node->Exception = String::Concat(chance, S"floating point underflow at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			node->Exception = String::Concat(chance, S"illegal instruction at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			node->Exception = String::Concat(chance, S"in-page error at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			node->Exception = String::Concat(chance, S"integer divide by zero at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_INT_OVERFLOW:
			node->Exception = String::Concat(chance, S"integer overflow at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			node->Exception = String::Concat(chance, S"invalid disposition at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			node->Exception = String::Concat(chance, S"noncontinuable exception at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			node->Exception = String::Concat(chance, S"privileged instruction at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_SINGLE_STEP:
			node->Exception = String::Concat(chance, S"single-step exception at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		case EXCEPTION_STACK_OVERFLOW:
			node->Exception = String::Concat(chance, S"stack overflow at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		default:
			node->Exception = String::Concat(chance, S"exception code 0x", __box(info.exceptionCode)->ToString("X8"),
				S" at 0x", __box(info.exceptionAddress)->ToString("X8"));
			break;
		}

		if (node->CreateDelegate)
		{
			Object* args[] = __gc new Object*[1];
			args[0] = node;
			node->CreateDelegate->DynamicInvoke(args);
		}
	}
}
