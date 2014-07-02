//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionGenerator.cpp
//
// DESCRIPTION: Contains implementation for the file corruption classes
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 May 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FileCorruptionGenerator.h"

namespace FileCorruptionGenerator
{
	//*************************************************************************
	// Method:		RandomByteCorruptor
	// Description: Constructor for the RandomByteCorruptor class
	//
	// Parameters:
	//	corruptCount - the number of bytes to corrupt in the file
	//
	// Return Value: None
	//*************************************************************************
	RandomByteCorruptor::RandomByteCorruptor(unsigned int corruptCount)
	{
		this->corruptByteCount = corruptCount;
		rand = new Random();
	}

	//*************************************************************************
	// Method:		~RandomByteCorruptor
	// Description: Destructor for the RandomByteCorruptor class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	RandomByteCorruptor::~RandomByteCorruptor()
	{
	}

	//*************************************************************************
	// Method:		GenerateFile
	// Description: generates a random byte corrupted file
	//
	// Parameters:
	//	inFile - the file to use as the template for corruption
	//	outFile - the corrupted output file
	//
	// Return Value: None
	//*************************************************************************
	void RandomByteCorruptor::GenerateFile(String *inFile, String *outFile)
	{
		BinaryReader *br = new BinaryReader(new FileStream(inFile, FileMode::Open, FileAccess::ReadWrite, FileShare::ReadWrite));
		BinaryWriter *bw = new BinaryWriter(new FileStream(outFile, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::ReadWrite));

		for (int i = 0; i < br->BaseStream->Length; i++)
			bw->Write(br->ReadByte());

		for (unsigned int i = 0; i < this->corruptByteCount; i++)
		{
			bw->Seek(rand->Next((int)(br->BaseStream->Length)), SeekOrigin::Begin);
			bw->Write((unsigned char)(rand->Next(256)));
		}

		br->Close();
		bw->Close();
	}

	//*************************************************************************
	// Method:		LongStringCorruptor
	// Description: Constructor for the LongStringCorruptor class
	//
	// Parameters:
	//	corruptCount - the length of the string to corrupt
	//
	// Return Value: None
	//*************************************************************************
	LongStringCorruptor::LongStringCorruptor(unsigned int corruptCount)
	{
		this->corruptByteCount = corruptCount;
		rand = new Random();
	}

	//*************************************************************************
	// Method:		~LongStringCorruptor
	// Description: Destructor for the LongStringCorruptor class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	LongStringCorruptor::~LongStringCorruptor()
	{
	}

	//*************************************************************************
	// Method:		GenerateFile
	// Description: generates a long string corrupted file
	//
	// Parameters:
	//	inFile - the file to use as the template for corruption
	//	outFile - the corrupted output file
	//
	// Return Value: None
	//*************************************************************************
	void LongStringCorruptor::GenerateFile(String *inFile, String *outFile)
	{
		BinaryReader *br = new BinaryReader(new FileStream(inFile, FileMode::Open, FileAccess::ReadWrite, FileShare::ReadWrite));
		BinaryWriter *bw = new BinaryWriter(new FileStream(outFile, FileMode::OpenOrCreate, FileAccess::ReadWrite, FileShare::ReadWrite));

		for (int i = 0; i < br->BaseStream->Length; i++)
			bw->Write(br->ReadByte());

		bw->Seek(rand->Next((int)(br->BaseStream->Length)), SeekOrigin::Begin);
		int len = rand->Next(this->corruptByteCount);
		int character = rand->Next(256);
		for (int i = 0; i < len; i++)
			bw->Write((unsigned char)character);

		br->Close();
		bw->Close();
	}
}