//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		RegistrationKeyGenerator.h
//
// DESCRIPTION: Contains definition for the class that encodes and decodes
//				product registration keys
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Sep 2003		 B Shirey	 File created.
//*************************************************************************
#pragma once

#include "RegistrationData.h"

namespace RegistrationLibrary
{
	//*************************************************************************
	// encodes/decodes registration keys
	//*************************************************************************
	class RegistrationKeyGenerator
	{
	private:
		static RegistrationKeyGenerator *instance;

		RegistrationKeyGenerator();
		~RegistrationKeyGenerator();

		bool EncodeKey(RegistrationData *data, SiString &resultKey);
		bool EncodeHolodeckKey(RegistrationData *data, SiString &resultKey);
		bool EncodeHolodeckBasicKey(RegistrationData *data, SiString &resultKey);

		bool DecodeKey(SiString &key, RegistrationData *data);
		bool DecodeHolodeckKey(SiString &key, RegistrationData *data);
		bool DecodeHolodeckBasicKey(SiString &key, RegistrationData *data);

		int GenerateChecksum(const SiString &byteArray);
		void GenerateSerialNumberByteArray(const SiString &serialNum, SiString &byteArray);

		void EncodeNumber(int value, SiString &result);
		int DecodeNumber(SiString &byteArray, int &index);

		void InsertIntoByteArray(SiString &byteArray, int pos, int value, bool encodeLength);
		int ExtractFromByteArray(SiString &byteArray, bool lengthEncoded);

	public:
		static RegistrationKeyGenerator *GetInstance();

		bool GenerateRegistrationKey(RegistrationData *data);
		bool DecodeRegistrationKey(RegistrationData *data);
	};
}