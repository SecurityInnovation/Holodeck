//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionGenerator.h
//
// DESCRIPTION: Contains definition for the file corruption classes
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 May 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>

using namespace System;
using namespace System::IO;

namespace FileCorruptionGenerator
{
	public __gc __interface IFileCorruptor
	{
	public:
		virtual void GenerateFile(String *inFile, String *outFile);
	};

	public __gc class RandomByteCorruptor : public IFileCorruptor
	{
	protected:
		unsigned int corruptByteCount;
		Random *rand;

	public:
		RandomByteCorruptor(unsigned int corruptCount);
		~RandomByteCorruptor();

		void GenerateFile(String *inFile, String *outFile);
	};

	public __gc class LongStringCorruptor : public IFileCorruptor
	{
	protected:
		unsigned int corruptByteCount;
		Random *rand;

	public:
		LongStringCorruptor(unsigned int corruptCount);
		~LongStringCorruptor();

		void GenerateFile(String *inFile, String *outFile);
	};
}
