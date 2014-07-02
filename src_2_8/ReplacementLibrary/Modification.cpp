//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Modification.cpp
//
// DESCRIPTION: Contains implementation for the class Modification.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Oct 2003		 R. Wagner	 File created.
//*************************************************************************

#include "Modification.h"

using namespace SiUtils;
using namespace Replacement;


//*************************************************************************
// Modification::Parse will take the modification string in sExpression
// and parse it down into the containing parts.
//
// Parameters:
//   None
//
// Return value:
//   true on success, false on failure.
//*************************************************************************
bool Modification::Parse()
{
	DWORD i;

	if (sExpression.GetLength() < 1)
	{
		// Empty modification string
		return false;
	}

	// Check for overwrite flag
	if (((char*)sExpression)[0] == 'o')
	{
		overwrite = true;
		SiString temp;
		sExpression.SubString(temp, 1);
		sExpression = temp;
	}
	else
		overwrite = false;

	switch (((char*)sExpression)[0])
	{
	case 's':
		// 's' type of modification string is a search and replace operation.
		// Each occurence of the given regular expression will be replaced with
		// a given replacement string.
		type = MOD_SEARCH_REPLACE;

		// 's' should be followed by a slash
		if (sExpression.GetLength() < 2)
			return false;
		if (((char*)sExpression)[1] != '/')
			return false;

		// Get the full regular expression
		sRegExpr = "";
		for (i=2; i<sExpression.GetLength(); i++)
		{
			if (((char*)sExpression)[i] == '/')
				break;
			if (((char*)sExpression)[i] == '\\')
			{
				// Escape sequence
				i++;
				if (i >= sExpression.GetLength())
				{
					// Invalid escape sequence
					return false;
				}

				if (((char*)sExpression)[i] == '/')
				{
					// A \/ escape sequence represents a normal / character
					sRegExpr += "/";
					continue;
				}
				else if (((char*)sExpression)[i] == ' ')
				{
					// Backslash followed by space.  Just insert a space into
					// the final string.
					sRegExpr += " ";
					continue;
				}
				else
				{
					// Other escape sequence, just leave it as is and let the
					// regular expression library handle it.
					sRegExpr += "\\";
				}
			}
			// Non-escape character, just add it to the final string.
			sRegExpr += ((char*)sExpression)[i];
		}

		if (i >= sExpression.GetLength())
		{
			// Expects / after regular expression
			return false;
		}

		i++;	// Go past / character

		// Get the replacement string
		sReplacement = "";
		for (; i<sExpression.GetLength(); i++)
		{
			if (((char*)sExpression)[i] == '/')
				break;
			if (((char*)sExpression)[i] == '\\')
			{
				// Escape sequence
				i++;
				if (i >= sExpression.GetLength())
				{
					// Invalid escape sequence
					return false;
				}

				if (((char*)sExpression)[i] == '/')
				{
					// A \/ escape sequence represents a normal / character
					sReplacement += "/";
					continue;
				}
				else if (((char*)sExpression)[i] == ' ')
				{
					// Backslash followed by space.  Just insert a space into
					// the final string.
					sReplacement += " ";
					continue;
				}
				else
				{
					// Other escape sequence, just leave it as is and let the
					// regular expression library handle it.
					sReplacement += "\\";
				}
			}
			// Non-escape character, just add it to the final string.
			sReplacement += ((char*)sExpression)[i];
		}
		// Done parsing
		break;

	case 'r':
		// 'r' type of modification string is a modification done on random bytes.
		// Every N out of 10,000 bytes will be replaced with a given replacement string.
		type = MOD_RAND_BYTES;

		// 'r' should be followed by a slash
		if (sExpression.GetLength() < 2)
			return false;
		if (((char*)sExpression)[1] != '/')
			return false;

		// Get the probability of replacing a byte
		dwProbability = atoi((char*)sExpression + 2);
		// Look for / after the numerical value
		for (i=2; i<sExpression.GetLength(); i++)
		{
			if (((char*)sExpression)[i] == '/')
				break;
		}

		if (i >= sExpression.GetLength())
		{
			// Expects / after probability
			return false;
		}

		i++;	// Go past / character

		// Get the replacement string
		sReplacement = "";
		for (; i<sExpression.GetLength(); i++)
		{
			if (((char*)sExpression)[i] == '/')
				break;
			if (((char*)sExpression)[i] == '\\')
			{
				// Escape sequence
				i++;
				if (i >= sExpression.GetLength())
				{
					// Invalid escape sequence
					return false;
				}

				if (((char*)sExpression)[i] == '/')
				{
					// A \/ escape sequence represents a normal / character
					sReplacement += "/";
					continue;
				}
				else if (((char*)sExpression)[i] == ' ')
				{
					// Backslash followed by space.  Just insert a space into
					// the final string.
					sReplacement += " ";
					continue;
				}
				else
				{
					// Other escape sequence, just leave it as is and let the
					// regular expression library handle it.
					sReplacement += "\\";
				}
			}
			// Non-escape character, just add it to the final string.
			sReplacement += ((char*)sExpression)[i];
		}
		// Done parsing
		break;

	default:
		// Unknown modification type
		return false;
	}
	return true;
}


Modification::Modification()
{
	valid = false;
}


Modification::Modification(const SiString& expr)
{
	SetExpression(expr);
}


void Modification::SetExpression(const SiString& expr)
{
	sExpression = expr;
	valid = Parse();
}


bool Modification::IsValid() const
{
	return valid;
}


ModificationType Modification::GetType() const
{
	return type;
}


SiString Modification::GetRegExpr() const
{
	return sRegExpr;
}


SiString Modification::GetReplacementStr() const
{
	return sReplacement;
}


DWORD Modification::GetProbability() const
{
	return dwProbability;
}

bool Modification::GetOverwrite() const
{
	return overwrite;
}
