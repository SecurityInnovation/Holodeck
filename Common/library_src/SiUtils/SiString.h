//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SiString.h
//
// DESCRIPTION: Contains definition for the utility class SiString
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>

namespace SiUtils
{
	//*************************************************************************
	// Combines common functionality of string operations into a class
	//*************************************************************************
	class SiString
	{
	private:
		wchar_t *value;
		unsigned int size;							// the number of characters (not bytes)

		char *singleBuffer;							// buffer for conversion to char *

		static HANDLE heap;
	public:
		SiString();
		SiString(const SiString &representedValue);		// copy constructor
		SiString(const char *representedValue);
		SiString(const wchar_t *representedValue);
		~SiString();

		unsigned int GetLength() const { return size; };

		operator char *() const;
		operator wchar_t *() const;
		const SiString& operator =(char *right);
		const SiString& operator =(wchar_t *right);
		const SiString& operator =(const SiString &right);
		const SiString& operator +=(char *right);
		const SiString& operator +=(wchar_t *right);
		const SiString& operator +=(const SiString &right);
		const SiString& operator +=(char right);
		const SiString& operator +=(wchar_t right);

		bool operator ==(char *right) const;
		bool operator ==(wchar_t *right) const;
		bool operator !=(char *right) const;
		bool operator !=(wchar_t *right) const;

		int CompareTo(char *right) const;
		int CompareTo(wchar_t *right) const;
		int CompareTo(const SiString &right) const;
		int CompareToIgnoreCase(char *right) const;
		int CompareToIgnoreCase(wchar_t *right) const;
		int CompareToIgnoreCase(const SiString &right) const;

		bool SubString(SiString &subString, int startingPosition, int endingPosition = -1);
		int FindFirst(const SiString &stringToFind);
		int FindFirst(char charToFind);
		int FindFirst(wchar_t charToFind);
		int FindFirstIgnoreCase(const SiString &stringToFind);
		int FindFirstIgnoreCase(char charToFind);
		int FindFirstIgnoreCase(wchar_t charToFind);
		int FindLast(const SiString &stringToFind);
		int FindLast(char charToFind);
		int FindLast(wchar_t charToFind);
		int FindLastIgnoreCase(const SiString &stringToFind);
		int FindLastIgnoreCase(char charToFind);
		int FindLastIgnoreCase(wchar_t charToFind);

		bool RemoveCharacterAt(unsigned int position);
		bool Remove(char value);
		bool Remove(wchar_t value);

		void MakeUpper();
		void MakeLower();

		unsigned int GetHashCode() const;
		static unsigned int GetHashCode(const char* str);
	};
}; // namespace
