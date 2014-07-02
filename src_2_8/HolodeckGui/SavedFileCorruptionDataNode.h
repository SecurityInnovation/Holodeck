//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SavedFileCorruptionDataNode.h
//
// DESCRIPTION: Contains definition for the class SavedFileCorruptionDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace UIData
{
	public __gc class SavedFileCorruptionDataNode : public DisplayableDataNode
	{
	protected:
		String *origFile;
		String *corruptedFile;
		String *xmlChangesFile;
		String *detailsFile;

	public:
		SavedFileCorruptionDataNode(String *Name, NodeType type, int pID);
		~SavedFileCorruptionDataNode();

		[ReadOnly(true), Description("The original file name"), CategoryAttribute("Fault Info")]
		__property String *get_OriginalFile() { return origFile; }
		__property void set_OriginalFile(String *value) { origFile = value; }
		
		[ReadOnly(true), Description("The fuzzed file name"), CategoryAttribute("Fault Info")]
		__property String *get_CorruptedFile() { return corruptedFile; }
		__property void set_CorruptedFile(String *value) { corruptedFile = value; }

		[Browsable(false)]
		__property String *get_ChangesFile() { return xmlChangesFile; }
		__property void set_ChangesFile(String *value) { xmlChangesFile = value; }

		[Browsable(false)]
		__property String *get_DetailsFile() { return detailsFile; }
		__property void set_DetailsFile(String *value) { detailsFile = value; }
	};
}