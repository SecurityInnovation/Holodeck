//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		MemoryMappedFile.h
//
// DESCRIPTION: Contains definition for the memory mapped file class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "SiString.h"

namespace SiUtils
{
	//*************************************************************************
	// class to represent and encapsulate a memory mapped file
	//*************************************************************************
	class MemoryMappedFile
	{
	private:
		HANDLE fileHandle;
		void *mapView;
		SiString fileName;

	public:
		MemoryMappedFile();
		~MemoryMappedFile();

		bool Create(const SiString &name, unsigned int size);
		bool Open(const SiString &name);
		void Close();
		void Write(char *str, unsigned int numBytes);
		unsigned int GetNumberOfBytesInFile();
		void Read(char *str);
	};
}