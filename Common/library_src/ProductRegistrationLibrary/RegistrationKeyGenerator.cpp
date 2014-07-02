//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		RegistrationKeyGenerator.cpp
//
// DESCRIPTION: Contains implementation for the class that encodes and decodes
//				product registration keys
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Sep 2003		 B Shirey	 File created.
//*************************************************************************
#include "RegistrationKeyGenerator.h"

#include <time.h>

namespace RegistrationLibrary
{
	RegistrationKeyGenerator *RegistrationKeyGenerator::instance = NULL;

	//*************************************************************************
	// Method:		RegistrationKeyGenerator
	// Description: Constructor for the RegistrationKeyGenerator class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationKeyGenerator::RegistrationKeyGenerator()
	{
	}

	//*************************************************************************
	// Method:		RegistrationKeyGenerator
	// Description: Destructor for the RegistrationKeyGenerator class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationKeyGenerator::~RegistrationKeyGenerator()
	{
	}

	//*************************************************************************
	// Method:		GetInstance
	// Description: returns the single instance of the class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the class
	//*************************************************************************
	RegistrationKeyGenerator *RegistrationKeyGenerator::GetInstance()
	{
		if (instance == NULL)
			instance = new RegistrationKeyGenerator();

		return instance;
	}

	//*************************************************************************
	// Method:		EncodeKey
	// Description: encodes a key based on the product version
	//
	// Parameters:
	//	data - the registration data to encode
	//	resultKey - the string containing the result
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::EncodeKey(RegistrationData *data, SiString &resultKey)
	{
		bool result = true;

		switch(data->GetProductType())
		{
		case Holodeck:
			if (!EncodeHolodeckKey(data, resultKey))
				result = false;
			break;
		default:
			result = false;
			break;
		}

		return result;
	}

	//*************************************************************************
	// Method:		EncodeHolodeckKey
	// Description: encodes a holodeck key based on the key version
	//
	// Parameters:
	//	data - the registration data to encode
	//	resultKey - the string containing the result
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::EncodeHolodeckKey(RegistrationData *data, SiString &resultKey)
	{
		bool result = true;

		switch (data->GetKeyVersion())
		{
		case HolodeckBasic:
			if (!EncodeHolodeckBasicKey(data, resultKey))
				result = false;
			break;
		default:
			result = false;
			break;
		}

		return result;
	}

	//*************************************************************************
	// Method:		EncodeHolodeckBasicKey
	// Description: encodes a holodeck basic key
	//
	// Parameters:
	//	data - the registration data to encode
	//	resultKey - the string containing the result
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::EncodeHolodeckBasicKey(RegistrationData *data, SiString &resultKey)
	{
		int index;
		SiString byteArray;

		// Seed the random-number generator with current time so that
		// the numbers will be different every time we run.
		srand((unsigned)time(NULL));

		// Create a byte array by using numbered pairs from the serial number.
		GenerateSerialNumberByteArray(data->GetSerialNumber(), byteArray);

		// insert the number of days til expiration
		index = rand() % byteArray.GetLength();
		InsertIntoByteArray(byteArray, index, data->GetNumberOfDaysValid(), true);

		// insert the number of licenses purchased
		index = rand() % byteArray.GetLength();
		InsertIntoByteArray(byteArray, index, data->GetNumberOfLicensesPurchased(), true);

		// insert the functionality level
		index = rand() % byteArray.GetLength();
		InsertIntoByteArray(byteArray, index, data->GetFunctionalityType(), false);

		// insert the customer type
		index = rand() % byteArray.GetLength();
		InsertIntoByteArray(byteArray, index, data->GetCustomerType(), false);

		resultKey = byteArray;

		return true;
	}

	//*************************************************************************
	// Method:		DecodeKey
	// Description: decodes a key based on the product version
	//
	// Parameters:
	//	resultKey - the string containing the key to decode
	//	data - the registration data to populate
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::DecodeKey(SiString &key, RegistrationData *data)
	{
		bool result = true;

		switch(data->GetProductType())
		{
		case Holodeck:
			if (!DecodeHolodeckKey(key, data))
				result = false;
			break;
		default:
			result = false;
			break;
		}

		return result;
	}
	//*************************************************************************
	// Method:		DecodeHolodeckKey
	// Description: decodes a key based on the holodeck key version
	//
	// Parameters:
	//	resultKey - the string containing the key to decode
	//	data - the registration data to populate
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::DecodeHolodeckKey(SiString &key, RegistrationData *data)
	{
		bool result = true;

		switch (data->GetKeyVersion())
		{
		case HolodeckBasic:
			if (!DecodeHolodeckBasicKey(key, data))
				result = false;
			break;
		default:
			result = false;
			break;
		}

		return result;
	}
	//*************************************************************************
	// Method:		DecodeHolodeckBasicKey
	// Description: decodes a holodeck basic key
	//
	// Parameters:
	//	resultKey - the string containing the key to decode
	//	data - the registration data to populate
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::DecodeHolodeckBasicKey(SiString &key, RegistrationData *data)
	{
		int value = 0;

		// get the customer type
		value = ExtractFromByteArray(key, false);
		if (value == -1)
			return false;
		data->SetCustomerType((CustomerType)value);

		// get the functionality level
		value = ExtractFromByteArray(key, false);
		if (value == -1)
			return false;
		data->SetFunctionalityType((FunctionalityType)value);

		// get the number of licenses purchased
		value = ExtractFromByteArray(key, true);
		if (value == -1)
			return false;
		data->SetNumberOfLicensesPurchased(value);

		// get the days til expiration
		value = ExtractFromByteArray(key, true);
		if (value == -1)
			return false;
		data->SetNumberOfDaysValid(value);

		return true;
	}

	//*************************************************************************
	// Method:		GenerateChecksum
	// Description: generate a checksum by xor-ing each character in the string
	//	this reduces the likelihood that a key will be hacked
	//
	// Parameters:
	//	byteArray - The string to generate the checksum for
	//
	// Return Value: the checksum value
	//*************************************************************************
	int RegistrationKeyGenerator::GenerateChecksum(const SiString &byteArray)
	{
		int byte = 0;
		char *array = byteArray;

		for (unsigned int i = 0; i < byteArray.GetLength(); i++)
		{
			byte ^= array[i];
		}

		return byte % NUM_LETTERS;
	}

	//*************************************************************************
	// Method:		EncodeNumber
	// Description: encodes a number value and returns the encoded string
	//
	// Parameters:
	//	value - the value to encode
	//	result - on exit, the resulting encoded string
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationKeyGenerator::EncodeNumber(int value, SiString &result)
	{
		SiString encodedValue;

		bool done = false;
		int modulus = value % NUM_LETTERS;
		int multiplier = value / NUM_LETTERS;
		int multiplierDigits = 0;
		int digit;

		while (!done)
		{
			digit = multiplier % 10;
			SiString temp;

			temp += (char)(digit + ASCII_BIG_A);
			temp += encodedValue;
			encodedValue = temp;

			multiplierDigits++;
			if (multiplier < 10)
				done = true;

			multiplier = multiplier / 10;
		}

		SiString temp2;
		temp2 += (char)(multiplierDigits + ASCII_BIG_A);
		temp2 += encodedValue;
		encodedValue = temp2;
		encodedValue += (char)(modulus + ASCII_BIG_A);

		result = encodedValue;
	}

	//*************************************************************************
	// Method:		DecodeNumber
	// Description: decodes a number value from a byte array and returns the
	//	value
	//
	// Parameters:
	//	byteArray - the string with the number to decode
	//	index - the position in the byte array to start decoding
	//
	// Return Value: the value extracted from the string, or -1 on error
	//*************************************************************************
	int RegistrationKeyGenerator::DecodeNumber(SiString &byteArray, int &index)
	{
		int modulus;
		int multiplier = 0;
		int multiplierDigits;

		if (index > (int)byteArray.GetLength())
			return -1;

		// extract the size of the multiplier
		multiplierDigits = (int)(((char *)byteArray)[index++]) - ASCII_BIG_A;

		// extract the multiplier
		while (multiplierDigits > 0)
		{
			if (index > (int)(byteArray.GetLength() - 1))
				return -1;

			multiplier = (multiplier * 10) + (int)(((char *)byteArray)[index++]) - ASCII_BIG_A;
			multiplierDigits--;
		}

		if (index > (int)(byteArray.GetLength() - 1))
			return -1;

		// extract the modulus
		modulus = (int)(((char *)byteArray)[index]) - ASCII_BIG_A;

		// compute the return value
		return ((multiplier * NUM_LETTERS) + modulus);
	}

	//*************************************************************************
	// Method:		GenerateSerialNumberByteArray
	// Description: generates a byte array based on the encoding of the serial
	//	number
	//
	// Parameters:
	//	serialNum - the serial number to generate the byte array for
	//	byteArray - the string representing the encoded serial number on exit
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationKeyGenerator::GenerateSerialNumberByteArray(const SiString &serialNum, SiString &byteArray)
	{
		SiString result;
		long byte;

		byteArray = "";

		// get rid of spaces and dashes
		result = serialNum;
		result.Remove(' ');
		result.Remove('-');

		char *buffer = result;
		for (unsigned int i = 0; i < result.GetLength() - 1; i += 2)
		{
			byte = ((buffer[i] - ASCII_ZERO) * 10) + (buffer[i+1] - ASCII_ZERO);
			byteArray += (char)((byte % NUM_LETTERS) + ASCII_BIG_A);
		}
	}

	//*************************************************************************
	// Method:		InsertIntoByteArray
	// Description: inserts an encoded string into the byte array
	//	number
	//
	// Parameters:
	//	byteArray - the string to insert into
	//	pos - the position to insert the value
	//	value - the value to insert
	//	encodeLength - true if the length should be encoded as well
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationKeyGenerator::InsertIntoByteArray(SiString &byteArray, int pos, int value, bool encodeLength)
	{
		SiString encodedValue = "";

		if (encodeLength)
			EncodeNumber(value, encodedValue);
		else
			encodedValue += (char)(value + ASCII_BIG_A);

		SiString leftPart;
		SiString rightPart;

		if (pos > 0)
			byteArray.SubString(leftPart, 0, pos - 1);
		else
			leftPart = "";

		byteArray.SubString(rightPart, pos);

		byteArray = leftPart;
		byteArray += encodedValue;
		byteArray += rightPart;
		byteArray += (char)(pos + INDEX_OFFSET);
	}

	//*************************************************************************
	// Method:		ExtractFromByteArray
	// Description: extracts an encoded string from the byte array as a number.
	//	This method is the reverse of InsertIntoByteArray
	//
	// Parameters:
	//	byteArray - the string to extract from
	//	lengthEncoded - true if the length was be encoded
	//
	// Return Value: the extracted value, or -1 on error
	//*************************************************************************
	int RegistrationKeyGenerator::ExtractFromByteArray(SiString &byteArray, bool lengthEncoded)
	{
		// get the index.  if it is invalid return an error
		int index = ((char *)byteArray)[byteArray.GetLength() - 1] - INDEX_OFFSET;
		if ((index > (int)(byteArray.GetLength() - 1)) || (index < 0))
			return -1;

		// store the index
		int leftIndex = index;

		int value = 0;
		if (lengthEncoded)
			value = DecodeNumber(byteArray, index);
		else
			value = (int)(((char *)byteArray)[index]) - ASCII_BIG_A;

		SiString left, right;
		if (leftIndex == 0)
			left = "";
		else
			byteArray.SubString(left, 0, leftIndex - 1);

		byteArray.SubString(right, index + 1, byteArray.GetLength() - 2);

		byteArray = left;
		byteArray += right;

		return value;
	}

	//*************************************************************************
	// Method:		GenerateRegistrationKey
	// Description: generates a registration key.  You must fill in all fields
	//	to the data parameter except for the registration key for this to work
	//
	// Parameters:
	//	data - The registration data to generate the key for.  Fill in every
	//		field except for registration key before calling this function.  On
	//		exit of the function, this parameter will have its registration key
	//		field filled in with the generated registration key string.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::GenerateRegistrationKey(RegistrationData *data)
	{
		if (data->GetSerialNumber().GetLength() < MIN_SERIAL_NUMBER_LENGTH)
			return false;

		// clear out the strings
		data->SetRegistrationKey("");

		SiString byteArray;
		EncodeKey(data, byteArray);

		// insert the product type
		int index = rand() % byteArray.GetLength();
		InsertIntoByteArray(byteArray, index, data->GetProductType(), false);

		// insert the key version
		index = rand() % byteArray.GetLength();
		InsertIntoByteArray(byteArray, index, data->GetKeyVersion(), false);

		// generate the checksum
		int checksum = GenerateChecksum(byteArray);
		byteArray += (char)(checksum + ASCII_BIG_A);

		// insert dashes every 5 chars for readability
		for (unsigned int i = 5; i < byteArray.GetLength(); i += 6)
		{
			SiString left, right;
			byteArray.SubString(left, 0, i - 1);
			byteArray.SubString(right, i);

			byteArray = left;
			byteArray += "-";
			byteArray += right;
		}

		data->SetRegistrationKey(byteArray);

		return true;
	}

	//*************************************************************************
	// Method:		DecodeRegistrationKey
	// Description: decodes a serial number/registration key combination.  you
	//	must fill in the serial number and registration key fields of the data
	//	parameter for this to work
	//
	// Parameters:
	//	data - The registration data to decode from.  Fill in the serial number
	//		and registration key fields before calling this function.  On exit
	//		of the function, this parameter will have all its fields filled in.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool RegistrationKeyGenerator::DecodeRegistrationKey(RegistrationData *data)
	{
		if ((data == NULL) || (data->GetSerialNumber().GetLength() == 0) ||
			(data->GetRegistrationKey().GetLength() == 0))
			return false;

		SiString key = data->GetRegistrationKey();
		key.Remove(' ');
		key.Remove('-');
		key.MakeUpper();

		// get the checksum from the key.  It will be the last char of the key
		int checksumFromKey = ((char *)key)[key.GetLength() - 1] - ASCII_BIG_A;
		key.RemoveCharacterAt(key.GetLength() - 1);

		// store off this key so we can compare checksum later
		SiString keyToCompareChecksumTo = key;

		int version = ExtractFromByteArray(key, false);
		int product = ExtractFromByteArray(key, false);
		if ((version == -1) || (product == -1))
			return false;

		data->SetProductType((ProductType)product);
		data->SetKeyVersion((RegKeyVersion)version);

		// decode the rest of the key
		if (!DecodeKey(key, data))
			return false;

		// make sure that what is left of the key matches the serial number encoding
		SiString computedSerialNumber;
		GenerateSerialNumberByteArray(data->GetSerialNumber(), computedSerialNumber);

		if (computedSerialNumber.CompareTo(key) != 0)
			return false;

		// check the original checksum with the computed checksum to make sure it is valid
		int computedChecksum = GenerateChecksum(keyToCompareChecksumTo);
		if (computedChecksum != checksumFromKey)
			return false;

		return true;
	}
}
