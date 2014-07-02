//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DisplayableDataNode.h
//
// DESCRIPTION: Contains definition for the class DisplayableDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>

using namespace System;
using namespace System::ComponentModel;

#include "DataEventRegistry.h"

namespace UIData
{
	public __value enum NodeType			//enum containing all the node types
	{
		Process,
		Thread,
		RecordedSessions,
		RecordedSession,
		Reports,
		Report,
		Logs,
		SavedLogs,
		ActiveLogPane,
		SavedLogPane,
		Resources,
		MemoryResource,
		DiskResource,
		NetworkResource,
		Files,
		File,
		Registry,
		RegistryEntry,
		Faults,
		MemoryFaultType,
		MemoryFault,
		DiskFaultType,
		DiskFault,
		NetworkFaultType,
		NetworkFault,
		RegistryFaultType,
		RegistryFault,
		ProcessFaultType,
		ProcessFault,
		CustomFaultType,
		CustomFault,
		NetworkCorruptionFaultType,
		NetworkCorruptionFault,
		FileCorruptionFaultType,
		FileCorruptionFault,
		CorruptedFiles,
		CorruptedFile,
		SavedCorruptedFiles,
		SavedCorruptedFile,
		Limits,
		DiskLimit,
		MemoryLimit,
		NetworkUpLimit,
		NetworkDownLimit,
		Tests,
		Test,
		ResourceFaults,
		ResourceFault,
		DynamicHelp,
		HelpDetails,
		NetworkLogs,
		SavedNetworkLogs,
		ActiveNetworkLogPane,
		SavedNetworkLogPane,
		ExceptionInfo,
		ExceptionPane,
		ComObject
	};

	public __gc class DisplayableDataNode;
	public __delegate void NodeModifiedDelegate(DisplayableDataNode *Node, String *propertyName);

	public __gc class DisplayableDataNode : public Object
	{
	public:
		NodeModifiedDelegate *onNodeModified;

		DisplayableDataNode();
		DisplayableDataNode(String *Name, NodeType Type, int pID, int tID);
		DisplayableDataNode(DisplayableDataNode *dataNode);
		~DisplayableDataNode();
		void InternalConstructor(String *Name, NodeType Type, int pID, int tID);
		void InternalConstructor(DisplayableDataNode *dataNode);
		void UpdateDelegates();
	
		[Browsable(false)]
		__property String* get_ID() {return nodeID;}
		__property void set_ID(String* value) 
		{
			nodeID = value; 
			if (onNodeModified)
				onNodeModified->Invoke(this, "ID");
		}

		[Browsable(false)]
		__property bool get_PerThreadMode() {return this->perThereadMode;}
		__property void set_PerThreadMode(bool value) 
		{
			perThereadMode = value; 
		}

		[Browsable(false)]
		__property Object* get_Element() {return uiElement;}
		__property void set_Element(Object *value) 
		{
			uiElement = value; 
		}

		[Browsable(false)]
		__property Object* get_DataArray() {return dataArray;}
		__property void set_DataArray(Object *value) { dataArray = value; }

		[Browsable(false)]
		__property NodeType get_Type() {return nodeType;}
		__property void set_Type(NodeType value) 
		{
			nodeType = value; 
			if (onNodeModified)
				onNodeModified->Invoke(this, "Type");
		}

		[ReadOnly(true), Description("The name associated with the selection"), Category("Owner Info")]
		__property String* get_Name() {return nodeName;}
		__property void set_Name(String *value) 
		{
			nodeName = value; 
			if (onNodeModified)
				onNodeModified->Invoke(this, "Name");
		}

		[ReadOnly(true), Description("The processID the node is associated with"), Category("Owner Info")]
		__property unsigned int get_processID() {return nodeProcessID;}
		__property void set_processID(unsigned int pID) 
		{
			nodeProcessID = pID; 
			if (onNodeModified)
				onNodeModified->Invoke(this, "processID");
		}

		[ReadOnly(true), Description("The threadID the node is associated with"), Category("Owner Info")]
		__property unsigned int get_threadID() {return nodeThreadID;}
		__property void set_threadID(unsigned int tID) 
		{
			nodeThreadID = tID; 
			if (onNodeModified)
				onNodeModified->Invoke(this, "threadID");
		}

		[Browsable(false)]
		__property Delegate* get_ToggleDelegate() {return toggleDelegate;}
		[Browsable(false)]
		__property Delegate* get_CreateDelegate() {return createDelegate;}
		[Browsable(false)]
		__property Delegate* get_DeleteDelegate() {return deleteDelegate;}
		[Browsable(false)]
		__property Delegate* get_ModifyDelegate() {return modifyDelegate;}
		[Browsable(false)]
		__property Delegate* get_SaveDelegate() {return saveDelegate;}
		[Browsable(false)]
		__property Delegate* get_CreatedDelegate() {return createdDelegate;}
		[Browsable(false)]
		__property Delegate* get_DeletedDelegate() {return deletedDelegate;}
		[Browsable(false)]
		__property Delegate* get_ModifiedDelegate() {return modifiedDelegate;}


	protected:
		DataEventRegistry *dataEventRegistry;
		String*	nodeID;						//used for various reasons (eg. test id)
		bool perThereadMode;			//flag for knowing the application mode
		Object* uiElement;				//optional pointer to the uielement that this node is bound to
		Object* dataArray;				//optional pointer to the dataarray this node is a child of
		NodeType nodeType;				//type of node
		String *nodeName;				//text that will be displayed in the ui pane
		unsigned int nodeProcessID;				//process this node is associated with
		unsigned int nodeThreadID;				//thread this node is associated with
		Delegate *toggleDelegate;		//will point to the appropriate delegate for the node type
		Delegate *createDelegate;		//will point to the appropriate delegate for the node type
		Delegate *deleteDelegate;		//will point to the appropriate delegate for the node type
		Delegate *modifyDelegate;		//will point to the appropriate delegate for the node type
		Delegate *saveDelegate;			//will point to the appropriate delegate for the node type
		Delegate *createdDelegate;		//will point to the appropriate delegate for the node type
		Delegate *deletedDelegate;		//will point to the appropriate delegate for the node type
		Delegate *modifiedDelegate;		//will point to the appropriate delegate for the node type
	};

}
