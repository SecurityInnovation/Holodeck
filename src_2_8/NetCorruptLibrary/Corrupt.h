//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		Corrupt.h
//
// DESCRIPTION: Contains functions to corrupt a stream of data
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 22 Oct 2003		 R. Wagner	 File created.
//*************************************************************************

#ifndef __CORRUPT_H__
#define __CORRUPT_H__

#include "NetCorruptLibrary.h"
#include <stdio.h>
#include <string>
#include <utility>
using namespace std;
using namespace Replacement;

//*************************************************************************
// Auto-seed multiply-with-carry random number generator.  Has a very long
// period of 2^125.
//
// Parameters:
//   None
//
// Return value:
//   Random 32-bit unsigned integer
//*************************************************************************
__forceinline unsigned long mulrng()
{
	static bool init = false;
	static unsigned long x = 123456789;
	static unsigned long y = 362436069;
	static unsigned long z = 77465321;
	static unsigned long c = 13579;

	if (!init)
	{
		// Pick some time-based values to seed the RNG
		x = time(NULL);
		LARGE_INTEGER cnt;
		QueryPerformanceCounter(&cnt);
		y = cnt.LowPart;
		z = cnt.HighPart;
		init = true;
	}

	unsigned __int64 t = UInt32x32To64(x, 916905990) + c;

	x = y;
	y = z;
	z = (unsigned long)(t & 0xFFFFFFFF);
	c = (unsigned long)(t >> 32);

	return z;
}

// GetReplacementString will return a string used to replace a regular
// expression match.
//
// Parameters:
//   match       - Regular expression match to be replaced
//   replacement - Replacement string given by the user
//
// Return value:
//   String that should be used for replacement
template <class matchtype>
string GetReplacementString(matchtype& match, string replacement)
{
	string finalReplace = "";
	// Parse our replacement string.  The regular expression library will take care of much
	// of the job (the $@ and ? operators, for example), but we need to take care of our
	// custom escape sequences, such as the $Rmin,max$ form for random strings.  The possible
	// escape sequences are documented in the Holodeck documentation
	for (DWORD i=0; i<replacement.length(); )
	{
		if (replacement[i] == '$')
		{
			// We found a $, see if we have one of our custom escape sequences.
			if ((i + 1) < replacement.length())
			{
				DWORD min, max, count;
				DWORD commaPos;
				if (replacement[i + 1] == 'R')
				{
					// Random string (form $Rmin,max$), get the minimum number of characters
					min = atoi(replacement.c_str() + i + 2);
					// Find the comma following the minimum value
					commaPos = (DWORD)replacement.find(',', i + 2);
					if ((commaPos != string::npos) && ((commaPos + 1) < replacement.length()))
					{
						// Get the maximum number of characters
						max = atoi(replacement.c_str() + commaPos + 1);
						// Find the final $
						i = (DWORD)replacement.find('$', commaPos);
						if (i == string::npos)
						{
							// Didn't find the $, just finish off the string
							i = (DWORD)replacement.length();
						}
						else
						{
							// Skip the final $
							i++;
						}

						// Pick a number of characters
						count = (mulrng() % (max - min + 1)) + min;
						// Generate a random string of count characters
						char *str = new char[count * 4 + 1];
						for (DWORD j=0; j<count; j++)
							sprintf(&str[j * 4], "\\x%2.2X", mulrng() & 255);
						// Use the new string instead of the encoded form in the final
						// replacement string
						finalReplace += str;
						delete[] str;
						continue;
					}
				}
				else if (replacement[i + 1] == 'S')
				{
					// Repetition of a character (form $Smin,max,char$), get the minimum number of characters
					min = atoi(replacement.c_str() + i + 2);
					// Find the comma following the minimum value
					commaPos = (DWORD)replacement.find(',', i + 2);
					if ((commaPos != string::npos) && ((commaPos + 1) < replacement.length()))
					{
						// Get the maximum number of characters
						max = atoi(replacement.c_str() + commaPos + 1);
						// Find the comma following the minimum value
						commaPos = (DWORD)replacement.find(',', commaPos + 1);
						if ((commaPos != string::npos) && ((commaPos + 1) < replacement.length()))
						{
							string repChar = "";
							for (i=commaPos+1; i<replacement.length(); i++)
							{
								if (replacement[i] == '\\')
								{
									// Escape sequence, don't try to interpret the following character
									repChar += string(1, replacement[i]);
									i++;
									if (i >= replacement.length())
										break;
								}
								else if (replacement[i] == '$')
								{
									if (replacement[i + 1] == '$')
									{
										// Double $ is expanded to a single $ to allow the ability to
										// duplicate the entire match or a submatch
										repChar += "$";
										i++;
										continue;
									}
									break;
								}
								repChar += string(1, replacement[i]);
							}

							// i is in the position of the final $
							if (i == string::npos)
							{
								// Didn't find the $, just finish off the string
								i = (DWORD)replacement.length();
							}
							else
							{
								// Skip the final $
								i++;
							}

							// Pick a number of characters
							count = (mulrng() % (max - min + 1)) + min;
							// Generate a string of count repetitions of repChar
							char *str = new char[count * repChar.length() + 1];
							for (DWORD j=0; j<count; j++)
								strcpy(&str[j * repChar.length()], repChar.c_str());
							// Use the new string instead of the encoded form in the final
							// replacement string
							finalReplace += str;
							delete[] str;
							continue;
						}
					}
				}
			}
		}
		finalReplace += replacement[i++];
	}
	// We've got our replacement string with our custom esacpe sequences parsed out and replaced
	// with a form manageable by the regular expression library.  Now just pass it on to the
	// library.
	return reg_format(match, finalReplace.c_str());
}


template <class iterator>
int __cdecl MatchCompareFunc(const pair<unsigned, reg_match<iterator, regex::alloc_type> >* a,
							 const pair<unsigned, reg_match<iterator, regex::alloc_type> >* b)
{
	// Sort first by position, start to end
	if (a->second[0].first < b->second[0].first)
		return -1;
	if (a->second[0].first > b->second[0].first)
		return 1;
	// Sort next by match length, high to low
	if (a->second[0].second > b->second[0].second)
		return -1;
	if (a->second[0].second < b->second[0].second)
		return 1;
	return 0;
}


//*************************************************************************
// Method:		CorruptData
// Description: Corrupts a stream of data
//
// Parameters:
//	begin - Start of data buffer
//  end - End of data buffer
//  modList - List of Modification structures describing the corruption
//		to perform
//  writeFunc - Function to copy data to the output data stream
//  writeCharsFunc - Function to write bytes to the output data stream
//  writeUserData - User-defined data to pass to writeFunc and writeCharsFunc
//  changesXmlFile - File for logging changes (optional)
//
// Return Value: None
//*************************************************************************
template <class iterator>
__int64 CorruptData(iterator begin, iterator end, const ModificationList& modList,
					void (*writeFunc)(iterator begin, iterator end, void* userData),
					void (*writeCharsFunc)(const char* begin, const char* end, void* userData),
					void* userData, FILE* changesXmlFile = NULL)
{
	vector<pair<unsigned, reg_match<iterator, regex::alloc_type> > > matchList;

	// Find out which data modification filter(s) should be used on this port
	const Modification* mod;
	for (unsigned i=0; i<modList.GetLength(); i++)
	{
		mod = &modList[i];
		iterator start = begin;

		// Check which type of modification we are doing, and perform it
		if (mod->GetType() == MOD_SEARCH_REPLACE)
		{
			// Search and replace modification (replace each occurence of a regular
			// expression with a replacement string)
			regex e((char*)mod->GetRegExpr()); // Create the regular expression
			reg_match<iterator, regex::alloc_type> m;
			// Loop while there are matches to the regular expression
			while (reg_search(start, end, m, e))
			{
				matchList.push_back(pair<unsigned, reg_match<iterator, regex::alloc_type> >(i, m));
				// Update the start pointer so that it is past the matched string
				start = m[0].second;
			}
		}
		else if (mod->GetType() == MOD_RAND_BYTES)
		{
			// Random bytes modification (replace mod->dwProbability out of 10,000
			// bytes with a replacement string)
			reg_match<iterator, regex::alloc_type> m;
			int j = 0;
			// Loop through each byte
			while ((start + j)< end)
			{
				// Pick a random number and check if this byte should be one of
				// the mod->dwProbability out of 10,000 to be replaced.
				if ((DWORD)(mulrng() % 10000) < mod->GetProbability())
				{
					// Add the match to the match list
					m.set_size(1);
					m.set_first(start + j, 0);
					m.set_second(start + j, 0);
					matchList.push_back(pair<unsigned, reg_match<iterator, regex::alloc_type> >(i, m));

					// Restart at the byte following the one replaced
					start = start + j + 1;
					j = 0;
					continue;
				}
				j++;
			}
		}
	}

	qsort(&matchList[0], matchList.size(), sizeof(pair<unsigned, reg_match<iterator, regex::alloc_type> >),
		(int (__cdecl*)(const void*, const void*))((int (__cdecl*)(const pair<unsigned, reg_match<iterator,
		regex::alloc_type> >*, const pair<unsigned, reg_match<iterator, regex::alloc_type> >*))MatchCompareFunc));

	__int64 len = 0;
	iterator start = begin;
	vector<pair<unsigned, reg_match<iterator, regex::alloc_type> > >::iterator best = matchList.begin();
	while (best != matchList.end())
	{
		// Perform the modification at the first match just found
		mod = &modList[best->first];
		iterator matchStart = best->second[0].first;
		iterator matchEnd = best->second[0].second;
		// Add the data that precedes the match
		if (matchStart != start)
		{
			writeFunc(start, matchStart, userData);
			len += matchStart - start;
		}
		// Add the replacement string for this match
		string sReplace = GetReplacementString(best->second, (char*)mod->GetReplacementStr());

		if (mod->GetOverwrite())
		{
			if ((__int64)(end - best->second[0].first) < (__int64)sReplace.length())
				sReplace = sReplace.substr(0, (size_t)(end - best->second[0].first));
		}

		writeCharsFunc(sReplace.c_str(), sReplace.c_str() + sReplace.length(), userData);

		if (mod->GetOverwrite())
		{
			if (changesXmlFile)
			{
				// Log the change to the XML file as a replace
				fprintf(changesXmlFile, "\t\t<Action Type=\"Replace\" Offset=\"%I64d\" ByteCount=\"%d\" OldValue=\"",
					len,  (int)sReplace.length());
				for (unsigned j=0; j<sReplace.length(); j++)
				{
					fprintf(changesXmlFile, "%2.2x", (int)(unsigned char)*(best->second[0].first + j));
					if ((j + 1) < sReplace.length())
						fprintf(changesXmlFile, " ");
				}
				fprintf(changesXmlFile, "\" NewValue=\"");
				for (unsigned j=0; j<sReplace.length(); j++)
				{
					fprintf(changesXmlFile, "%2.2x", (int)(unsigned char)sReplace[j]);
					if ((j + 1) < sReplace.length())
						fprintf(changesXmlFile, " ");
				}
				fprintf(changesXmlFile, "\" />\n");
			}

			// Use replacement string length for overwrite
			start = best->second[0].first;
			start += (int)sReplace.length();
		}
		else
		{
			if (changesXmlFile)
			{
				// Log the change to the XML file as a delete and insert
				int oldSize = (int)(best->second[0].second - best->second[0].first);

				if (oldSize > 0)
				{
					fprintf(changesXmlFile, "\t\t<Action Type=\"Delete\" Offset=\"%I64d\" ByteCount=\"%d\" OldValue=\"",
						len,  oldSize);
					for (int j=0; j<oldSize; j++)
					{
						fprintf(changesXmlFile, "%2.2x", (int)(unsigned char)*(best->second[0].first + j));
						if ((j + 1) < oldSize)
							fprintf(changesXmlFile, " ");
					}
					fprintf(changesXmlFile, "\" />\n");
				}
				fprintf(changesXmlFile, "\t\t<Action Type=\"Insert\" Offset=\"%I64d\" ByteCount=\"%d\" NewValue=\"",
					len, (int)sReplace.length());
				for (unsigned j=0; j<sReplace.length(); j++)
				{
					fprintf(changesXmlFile, "%2.2x", (int)(unsigned char)sReplace[j]);
					if ((j + 1) < sReplace.length())
						fprintf(changesXmlFile, " ");
				}
				fprintf(changesXmlFile, "\" />\n");
			}

			// Update the start pointer so that it is past the matched string
			start = best->second[0].second;
		}

		len += sReplace.length();

		// Remove the modification and any other modifications that take place before the new start
		best++;
		while (best != matchList.end())
		{
			if (!(best->second[0].first < start))
				break;
			best++;
		}
	}

	if (start < end)
	{
		// Data still remains, add it to the queue
		writeFunc(start, end, userData);
		len += end - start;
	}
	return len;
}

#endif
