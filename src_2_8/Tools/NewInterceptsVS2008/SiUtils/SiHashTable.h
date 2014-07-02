//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SiHashTable.h
//
// DESCRIPTION: Contains definition for the utility class SiHashTable
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 May 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "SiString.h"
#include "SiArray.h"

namespace SiUtils
{
	//*************************************************************************
	// Templated hash table with an SiString key
	//*************************************************************************
	template <class T, int hashSize = 8017> // Use a prime number hash table size
	class SiHashTable
	{
	protected:
		struct HashTableEntry
		{
		public:
			SiString key;
			T value;
		};

		typedef SiArray<HashTableEntry> HashBucket;

		SiArray<HashBucket> contents;

	public:
		SiHashTable()
		{
			contents.EnsureSize(hashSize);
			HashBucket empty;
			for (int i=0; i<hashSize; i++)
				contents.Add(empty);
		}

		SiHashTable(const SiHashTable<T, hashSize>& ht) { contents = ht.contents; }

		T& operator[](const char* key)
		{
			unsigned int hashCode = SiString::GetHashCode(key) % hashSize;
			for (unsigned int i=0; i<contents[hashCode].GetLength(); i++)
			{
				if (contents[hashCode][i].key == (char*)key)
					return contents[hashCode][i].value;
			}

			HashTableEntry newEntry;
			newEntry.key = key;
			int index = contents[hashCode].GetLength();
			contents[hashCode].Add(newEntry);
			return contents[hashCode][index].value;
		}

		bool KeyExists(const char* key) const
		{
			unsigned int hashCode = SiString::GetHashCode(key) % hashSize;
			for (unsigned int i=0; i<contents[hashCode].GetLength(); i++)
			{
				if (contents[hashCode][i].key == (char*)key)
					return true;
			}
			return false;
		}

		void Add(const SiString& key, const T& value)
		{
			unsigned int hashCode = SiString::GetHashCode(key) % hashSize;
			for (unsigned int i=0; i<contents[hashCode].GetLength(); i++)
			{
				if (contents[hashCode][i].key == (char*)key)
				{
					contents[hashCode][i].value = value;
					return;
				}
			}

			HashTableEntry newEntry;
			newEntry.key = key;
			newEntry.value = value;
			contents[hashCode].Add(newEntry);
		}

		void Remove(const SiString& key)
		{
			for (unsigned int i=0; i<contents[key.GetHashCode() % hashSize].GetLength(); i++)
			{
				if (contents[key.GetHashCode() % hashSize][i].key == (char*)key)
				{
					contents[key.GetHashCode() % hashSize].Remove(i);
					return;
				}
			}
		}

		void Clear()
		{
			for (int i=0; i<hashSize; i++)
				contents[i].Clear();
		}

		SiArray<SiString> GetKeyList()
		{
			SiArray<SiString> list;
			for (int i=0; i<hashSize; i++)
			{
				for (unsigned int j=0; j<contents[i].GetLength(); j++)
					list.Add(contents[i][j].key);
			}
			return list;
		}
	};
}
