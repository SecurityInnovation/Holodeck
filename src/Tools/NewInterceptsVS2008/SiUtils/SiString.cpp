//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SiString.cpp
//
// DESCRIPTION: Contains implementation of the class SiString.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include "SiString.h"

namespace SiUtils
{
	HANDLE SiString::heap = NULL;

	//*************************************************************************
	// Method:		SiString
	// Description: Constructor for the SiString class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SiString::SiString() : value(NULL), size(0), singleBuffer(NULL)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		size = 0;
		value = (wchar_t *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (size + 1));
		singleBuffer = (char *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (size + 1));
	}

	//*************************************************************************
	// Method:		SiString
	// Description: Constructor for the SiString class
	//
	// Parameters:
	//	representedValue - The value to represent in the string
	//
	// Return Value: None
	//*************************************************************************
	SiString::SiString(const SiString &representedValue) : value(NULL), size(0), singleBuffer(NULL)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		if (representedValue.value == NULL)
			return;

		size = ((SiString &)representedValue).GetLength();
		value = (wchar_t *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (size + 1));
		singleBuffer = (char *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (size + 1));

		if ((value == NULL) || (singleBuffer == NULL))
			return;

		lstrcpyW(value, representedValue);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';
	}

	//*************************************************************************
	// Method:		SiString
	// Description: Constructor for the SiString class
	//
	// Parameters:
	//	representedValue - The value to represent in the string
	//
	// Return Value: None
	//*************************************************************************
	SiString::SiString(const char *representedValue) : value(NULL), size(0), singleBuffer(NULL)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		if (representedValue == NULL)
			return;

		size = (unsigned int)lstrlenA(representedValue);
		
		value = (wchar_t *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (size + 1));
		singleBuffer = (char *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (size + 1));

		if ((value == NULL) || (singleBuffer == NULL))
			return;

		wchar_t *temp = (wchar_t *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (size + 1));
		MultiByteToWideChar(CP_ACP, 0, representedValue, -1, temp, size + 1);

		lstrcpyW(value, temp);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';

		HeapFree(heap, HEAP_ZERO_MEMORY, temp);
	}

	//*************************************************************************
	// Method:		SiString
	// Description: Constructor for the SiString class
	//
	// Parameters:
	//	representedValue - The value to represent in the string
	//
	// Return Value: None
	//*************************************************************************
	SiString::SiString(const wchar_t *representedValue) : value(NULL), size(0), singleBuffer(NULL)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		if (representedValue == NULL)
			return;

		size = (unsigned int)lstrlenW(representedValue);

		value = (wchar_t *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(wchar_t) * (size + 1));
		singleBuffer = (char *) HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (size + 1));

		if ((value == NULL) || (singleBuffer == NULL))
			return;

		lstrcpyW(value, representedValue);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';
	}

	//*************************************************************************
	// Method:		~SiString
	// Description: Destructor for the SiString class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SiString::~SiString()
	{
		if (value != NULL)
		{
			HeapFree(heap, 0, value);
			value = NULL;
		}

		if (singleBuffer != NULL)
		{
			HeapFree(heap, 0, singleBuffer);
			singleBuffer = NULL;
		}

		size = -1;
	}

	//*************************************************************************
	// Method:		operator char *
	// Description: Converts the string's representation to a single byte 
	//		string
	//
	// Parameters:
	//	None
	//
	// Return Value: The single byte string representing this value
	//*************************************************************************
	SiString::operator char *() const
	{
		return singleBuffer;
	}

	//*************************************************************************
	// Method:		operator wchar_t *
	// Description: Converts the string's representation to a double byte 
	//		string
	//
	// Parameters:
	//	None
	//
	// Return Value: The double byte string representing this value
	//*************************************************************************
	SiString::operator wchar_t *() const
	{
		return value;
	}

	//*************************************************************************
	// Method:		operator =
	// Description: Assigns the value of this string
	//
	// Parameters:
	//	right - The value to copy into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator =(char *right)
	{
		// make sure they aren't passing a null pointer
		if (right == NULL)
		{
			if (value != NULL)
			{
				HeapFree(heap, HEAP_ZERO_MEMORY, value);
				value = NULL;
			}

			if (singleBuffer != NULL)
			{
				HeapFree(heap, HEAP_ZERO_MEMORY, singleBuffer);
				singleBuffer = 0;
			}

			size = 0;
			return *this;
		}

		size = (unsigned int)lstrlenA(right);
		
		if (value == NULL)
		{
			value = (wchar_t *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(char));
		}
		else
		{
			value = (wchar_t *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, value, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, singleBuffer, (size + 1) * sizeof(char));
		}

		if ((value == NULL) || (singleBuffer == NULL))
			return *this;

		SiString temp = right;

		lstrcpyW(value, temp);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';

		return *this;
	}

	//*************************************************************************
	// Method:		operator =
	// Description: Assigns the value of this string
	//
	// Parameters:
	//	right - The value to copy into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator =(wchar_t *right)
	{
		// make sure they aren't passing a null pointer
		if (right == NULL)
		{
			if (value != NULL)
			{
				HeapFree(heap, HEAP_ZERO_MEMORY, value);
				value = NULL;
			}

			if (singleBuffer != NULL)
			{
				HeapFree(heap, HEAP_ZERO_MEMORY, singleBuffer);
				singleBuffer = 0;
			}

			size = 0;
			return *this;
		}

		size = (unsigned int)lstrlenW(right);

		if (value == NULL)
		{
			value = (wchar_t *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(char));
		}
		else
		{
			value = (wchar_t *) HeapReAlloc(heap, HEAP_ZERO_MEMORY, value, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, singleBuffer, (size + 1) * sizeof(char));
		}

		if ((value == NULL) || (singleBuffer == NULL))
			return *this;

		lstrcpyW(value, right);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';

		return *this;
	}

	//*************************************************************************
	// Method:		operator =
	// Description: Assigns the value of this string
	//
	// Parameters:
	//	right - The value to copy into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator =(const SiString &right)
	{
		return this->operator =((wchar_t *)right);
	}

	//*************************************************************************
	// Method:		operator +=
	// Description: Concatenates a string to the end of this string
	//
	// Parameters:
	//	right - The value to append into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator +=(char *right)
	{
		// make sure they aren't passing a null pointer
		if (right == NULL)
			return *this;

		SiString rightString(right);
		
		size = size + rightString.GetLength();
		if (value == NULL)
		{
			value = (wchar_t *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(char));
		}
		else
		{
			value = (wchar_t *) HeapReAlloc (heap, HEAP_ZERO_MEMORY, value, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, singleBuffer, (size + 1) * sizeof(char));
		}

		if ((value == NULL) || (singleBuffer == NULL))
			return *this;

		lstrcatW(value, rightString);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';

		return *this;
	}

	//*************************************************************************
	// Method:		operator +=
	// Description: Concatenates a string to the end of this string
	//
	// Parameters:
	//	right - The value to append into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator +=(wchar_t *right)
	{
		// make sure they aren't passing a null pointer
		if (right == NULL)
			return *this;

		SiString rightString(right);
		
		size = size + rightString.GetLength();
		if (value == NULL)
		{
			value = (wchar_t *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(char));
		}
		else
		{
			value = (wchar_t *) HeapReAlloc (heap, HEAP_ZERO_MEMORY, value, (size + 1) * sizeof(wchar_t));
			singleBuffer = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, singleBuffer, (size + 1) * sizeof(char));
		}

		if ((value == NULL) || (singleBuffer == NULL))
			return *this;

		lstrcatW(value, rightString);
		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';
		return *this;
	}

	//*************************************************************************
	// Method:		operator +=
	// Description: Concatenates a string to the end of this string
	//
	// Parameters:
	//	right - The value to append into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator +=(const SiString &right)
	{
		return this->operator +=((wchar_t *)right);
	}

	//*************************************************************************
	// Method:		operator +=
	// Description: Appends a character to the end of this string
	//
	// Parameters:
	//	right - The value to append into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator +=(char right)
	{
		char newStr[2];
		newStr[0] = right;
		newStr[1] = '\0';

		return this->operator +=(newStr);
	}

	//*************************************************************************
	// Method:		operator +=
	// Description: Appends a character to the end of this string
	//
	// Parameters:
	//	right - The value to append into this string
	//
	// Return Value: this
	//*************************************************************************
	const SiString& SiString::operator +=(wchar_t right)
	{
		wchar_t newStr[2];
		newStr[0] = right;
		newStr[1] = L'\0';

		return this->operator +=(newStr);
	}

	//*************************************************************************
	// Method:		operator ==
	// Description: Compares two strings for equality
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: true if the strings are the same length and value, false
	//	otherwise
	//*************************************************************************
	bool SiString::operator ==(char *right) const
	{
		SiString rightString = right;

		if (size != rightString.GetLength())
			return false;
		
		if (lstrcmpW(value, rightString) != 0)
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		operator ==
	// Description: Compares two strings for equality
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: true if the strings are the same length and value, false
	//	otherwise
	//*************************************************************************
	bool SiString::operator ==(wchar_t *right) const
	{
		SiString rightString = right;

		if (size != rightString.GetLength())
			return false;
		
		if (lstrcmpW(value, rightString) != 0)
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		operator !=
	// Description: Compares two strings for inequality
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: false if the strings are the same length and value, true
	//	otherwise
	//*************************************************************************
	bool SiString::operator !=(char *right) const
	{
		SiString rightString = right;

		if ((size == rightString.GetLength()) && (lstrcmpW(value, rightString) == 0))
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		operator !=
	// Description: Compares two strings for inequality
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: false if the strings are the same length and value, true
	//	otherwise
	//*************************************************************************
	bool SiString::operator !=(wchar_t *right) const
	{
		SiString rightString = right;

		if ((size == rightString.GetLength()) && (lstrcmpW(value, rightString) == 0))
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		CompareTo
	// Description: Compares two strings
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: < 0 if this is less than right, 0 if equal, > 0 if right
	//	is greater than this
	//*************************************************************************
	int SiString::CompareTo(char *right) const
	{
		return this->CompareTo((SiString)right);
	}
	
	//*************************************************************************
	// Method:		CompareTo
	// Description: Compares two strings
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: < 0 if this is less than right, 0 if equal, > 0 if right
	//	is greater than this
	//*************************************************************************
	int SiString::CompareTo(wchar_t *right) const
	{
		return this->CompareTo((SiString)right);
	}

	//*************************************************************************
	// Method:		CompareTo
	// Description: Compares two strings
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: < 0 if this is less than right, 0 if equal, > 0 if right
	//	is greater than this
	//*************************************************************************
	int SiString::CompareTo(const SiString &right) const
	{
		return lstrcmpW(value, right);
	}

	//*************************************************************************
	// Method:		CompareToIgnoreCase
	// Description: Compares two strings case insensitive
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: < 0 if this is less than right, 0 if equal, > 0 if right
	//	is greater than this
	//*************************************************************************
	int SiString::CompareToIgnoreCase(char *right) const
	{
		return this->CompareToIgnoreCase((SiString)right);
	}
	
	//*************************************************************************
	// Method:		CompareToIgnoreCase
	// Description: Compares two strings case insensitive
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: < 0 if this is less than right, 0 if equal, > 0 if right
	//	is greater than this
	//*************************************************************************
	int SiString::CompareToIgnoreCase(wchar_t *right) const
	{
		return this->CompareToIgnoreCase((SiString)right);
	}

	//*************************************************************************
	// Method:		CompareToIgnoreCase
	// Description: Compares two strings case insensitive
	//
	// Parameters:
	//	right - The value to compare to this string
	//
	// Return Value: < 0 if this is less than right, 0 if equal, > 0 if right
	//	is greater than this
	//*************************************************************************
	int SiString::CompareToIgnoreCase(const SiString &right) const
	{
		return lstrcmpiW(value, right);
	}

	//*************************************************************************
	// Method:		SubString
	// Description: Returns a substring of this string
	//
	// Parameters:
	//	subString - The resulting substring
	//	startingPosition - The index of the first character in the substring to extract
	//	endingPosition - The index of the last character of the substring to extract.
	//		Note: -1 means from the startingPosition until the end of stringToSplit
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SiString::SubString(SiString &subString, int startingPosition, int endingPosition)
	{
		if ((startingPosition < 0) || ((unsigned int)startingPosition >= size))
			return false;

		if (endingPosition != -1)
			if ((endingPosition < startingPosition) || (endingPosition >= (int)size))
				return false;

		// make a copy of the original string
		wchar_t *start = NULL;
		wchar_t *end = NULL;
		wchar_t *copy = (wchar_t *)HeapAlloc(heap, HEAP_ZERO_MEMORY, (size + 1) * sizeof(wchar_t));
		if (copy == NULL)
			return false;
		lstrcpyW(copy, value);

		// get the starting and ending locations for the substring
		start = copy + startingPosition;
		if (endingPosition == -1)
			end = copy + size;
		else
			end = copy + endingPosition + 1;

		// modify the ending location to an end of string char
		*end = L'\0';

		// copy the string to the returned substring
		subString = start;

		// free the memory for the copy string
		HeapFree(heap, HEAP_ZERO_MEMORY, copy);
		return true;
	}

	//*************************************************************************
	// Method:		FindFirst
	// Description: Returns the index of the first occurrence of the string to
	//	find in this string
	//
	// Parameters:
	//	stringToFind - The string to find in this string
	//
	// Return Value: the index of the starting character of the string to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindFirst(const SiString &stringToFind)
	{
		// need a win32 equivalent of wcsstr.  MS KB article Q99456 says there
		// isn't one so i guess we do it ourselves.
		int findLength = stringToFind.GetLength();
		int numCompares = size - findLength;
		wchar_t *findStr = stringToFind;
		
		for (int i = 0; i <= numCompares; i++)
		{
			bool match = true;
			int j = 0;
			while ((match) && (j < findLength))
			{
				if (value[i + j] != findStr[j])
					match = false;
				j++;
			}

			if (match)
				return i; 
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindFirst
	// Description: Returns the index of the first occurrence of the char to
	//	find in this string
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindFirst(char charToFind)
	{
		wchar_t wideChar;
		mbtowc(&wideChar, &charToFind, 1);

		// need a win32 equivalent of wcschr.  MS KB article Q99456 says there
		// isn't one so i guess we do it ourselves.
		for (unsigned int i = 0; i < size; i++)
		{
			if (value[i] == wideChar)
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindFirst
	// Description: Returns the index of the first occurrence of the char to
	//	find in this string
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindFirst(wchar_t charToFind)
	{
		// need a win32 equivalent of wcschr.  MS KB article Q99456 says there
		// isn't one so i guess we do it ourselves.
		for (unsigned int i = 0; i < size; i++)
		{
			if (value[i] == charToFind)
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindFirstIgnoreCase
	// Description: Returns the index of the first occurrence of the string to
	//	find in this string in a case insensitive fashion
	//
	// Parameters:
	//	stringToFind - The string to find in this string
	//
	// Return Value: the index of the starting character of the string to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindFirstIgnoreCase(const SiString &stringToFind)
	{
		// need a win32 equivalent of wcsstr.  MS KB article Q99456 says there
		// isn't one so i guess we do it ourselves.
		int findLength = stringToFind.GetLength();
		int numCompares = size - findLength;
		wchar_t *findStr = stringToFind;
		
		for (int i = 0; i <= numCompares; i++)
		{
			bool match = true;
			int j = 0;
			while ((match) && (j < findLength))
			{
				if (toupper(value[i + j]) != (toupper(findStr[j])))
					match = false;
				j++;
			}

			if (match)
				return i; 
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindFirstIgnoreCase
	// Description: Returns the index of the first occurrence of the char to
	//	find in this string in a case insensitive fashion
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindFirstIgnoreCase(char charToFind)
	{
		wchar_t wideChar;
		mbtowc(&wideChar, &charToFind, 1);

		// need a win32 equivalent of wcschr.  MS KB article Q99456 says there
		// isn't one so i guess we do it ourselves.
		for (unsigned int i = 0; i < size; i++)
		{
			if (toupper(value[i]) == toupper(wideChar))
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindFirstIgnoreCase
	// Description: Returns the index of the first occurrence of the char to
	//	find in this string in a case insensitive fashion
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindFirstIgnoreCase(wchar_t charToFind)
	{
		// need a win32 equivalent of wcschr.  MS KB article Q99456 says there
		// isn't one so i guess we do it ourselves.
		for (unsigned int i = 0; i < size; i++)
		{
			if (toupper(value[i]) == toupper(charToFind))
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindLast
	// Description: Returns the index of the last occurrence of the string to
	//	find in this string
	//
	// Parameters:
	//	stringToFind - The string to find in this string
	//
	// Return Value: the index of the starting character of the string to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindLast(const SiString &stringToFind)
	{
		int findLength = stringToFind.GetLength();
		int numCompares = size - findLength;
		wchar_t *findStr = stringToFind;
		
		for (int i = numCompares; i >= 0; i--)
		{
			bool match = true;
			int j = 0;
			while ((match) && (j < findLength))
			{
				if (value[i + j] != findStr[j])
					match = false;
				j++;
			}

			if (match)
				return i; 
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindLast
	// Description: Returns the index of the last occurrence of the char to
	//	find in this string
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindLast(char charToFind)
	{
		wchar_t wideChar;
		mbtowc(&wideChar, &charToFind, 1);

		for (int i = size - 1; i >= 0; i--)
		{
			if (value[i] == wideChar)
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindLast
	// Description: Returns the index of the first occurrence of the char to
	//	find in this string
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindLast(wchar_t charToFind)
	{
		for (int i = size - 1; i >= 0; i--)
		{
			if (value[i] == charToFind)
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindLastIgnoreCase
	// Description: Returns the index of the last occurrence of the string to
	//	find in this string in a case insensitive fashion
	//
	// Parameters:
	//	stringToFind - The string to find in this string
	//
	// Return Value: the index of the starting character of the string to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindLastIgnoreCase(const SiString &stringToFind)
	{
		int findLength = stringToFind.GetLength();
		int numCompares = size - findLength;
		wchar_t *findStr = stringToFind;
		
		for (int i = numCompares; i >= 0; i--)
		{
			bool match = true;
			int j = 0;
			while ((match) && (j < findLength))
			{
				if (toupper(value[i + j]) != (toupper(findStr[j])))
					match = false;
				j++;
			}

			if (match)
				return i; 
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindLastIgnoreCase
	// Description: Returns the index of the last occurrence of the char to
	//	find in this string in a case insensitive fashion
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindLastIgnoreCase(char charToFind)
	{
		wchar_t wideChar;
		mbtowc(&wideChar, &charToFind, 1);

		for (int i = size - 1; i >= 0; i--)
		{
			if (toupper(value[i]) == toupper(wideChar))
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		FindLastIgnoreCase
	// Description: Returns the index of the last occurrence of the char to
	//	find in this string in a case insensitive fashion
	//
	// Parameters:
	//	charToFind - The char to find in this string
	//
	// Return Value: the index of the character to find
	//	if successful, -1 otherwise
	//*************************************************************************
	int SiString::FindLastIgnoreCase(wchar_t charToFind)
	{
		for (int i = size; i >= 0; i--)
		{
			if (toupper(value[i]) == toupper(charToFind))
				return i;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		RemoveCharacterAt
	// Description: Deletes the character at the specified position from the
	//	string
	//
	// Parameters:
	//	position - the index in the string of the character to remove
	//
	// Return Value: true if character removed, false otherwise
	//*************************************************************************
	bool SiString::RemoveCharacterAt(unsigned int position)
	{
		if ((position >= size) || (value == NULL) || (singleBuffer == NULL))
			return false;

		unsigned int i = 0;
		for (i = position; i < size - 1; i++)
		{
			value[i] = value[i+1];
		}
		value[i] = L'\0';

		size--;
		value = (wchar_t *) HeapReAlloc (heap, HEAP_ZERO_MEMORY, value, (size + 1) * sizeof(wchar_t));
		singleBuffer = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, singleBuffer, (size + 1) * sizeof(char));

		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';

		return true;
	}

	//*************************************************************************
	// Method:		Remove
	// Description: Deletes all instances of a character from the string
	//
	// Parameters:
	//	value - the character to remove from the string
	//
	// Return Value: true if character removed, false otherwise
	//*************************************************************************
	bool SiString::Remove(char value)
	{
		bool removed = false;

		int posToRemove = FindFirst(value);
		while (posToRemove != -1)
		{
			RemoveCharacterAt(posToRemove);
			posToRemove = FindFirst(value);
		}

		return removed;
	}

	//*************************************************************************
	// Method:		Remove
	// Description: Deletes all instances of a character from the string
	//
	// Parameters:
	//	value - the character to remove from the string
	//
	// Return Value: true if character removed, false otherwise
	//*************************************************************************
	bool SiString::Remove(wchar_t value)
	{
		bool removed = false;

		int posToRemove = FindFirst(value);
		while (posToRemove != -1)
		{
			RemoveCharacterAt(posToRemove);
			posToRemove = FindFirst(value);
		}

		return removed;
	}

	//*************************************************************************
	// Method:		MakeUpper
	// Description: Makes all characters in the string upper case
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SiString::MakeUpper()
	{
		for (unsigned int i = 0; i < size; i++)
		{
			value[i] = toupper(value[i]);
		}

		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';
	}

	//*************************************************************************
	// Method:		MakeLower
	// Description: Makes all characters in the string lower case
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SiString::MakeLower()
	{
		for (unsigned int i = 0; i < size; i++)
		{
			value[i] = tolower(value[i]);
		}

		WideCharToMultiByte(CP_ACP, 0, value, -1, singleBuffer, size + 1, NULL, NULL);
		singleBuffer[size] = '\0';
	}

	//*************************************************************************
	// Method:		GetHashCode
	// Description: Returns a hash code for this string
	//
	// Parameters:
	//	None
	//
	// Return Value: Integer representing a hash code based on the string
	//*************************************************************************
	unsigned int SiString::GetHashCode() const
	{
		unsigned int hash = 0;
		for (unsigned int i=0; i<size; i++)
		{
			// Rotate left by one and add the next character
			hash = (hash << 1) | (hash >> 31);
			hash += (int)singleBuffer[i];
		}
		return hash;
	}

	//*************************************************************************
	// Method:		GetHashCode
	// Description: Returns a hash code for a string
	//
	// Parameters:
	//	str - String to get hash code for
	//
	// Return Value: Integer representing a hash code based on the string
	//*************************************************************************
	unsigned int SiString::GetHashCode(const char* str)
	{
		unsigned int hash = 0;
		for (unsigned int i=0; str[i]; i++)
		{
			// Rotate left by one and add the next character
			hash = (hash << 1) | (hash >> 31);
			hash += (int)str[i];
		}
		return hash;
	}

}; // namespace