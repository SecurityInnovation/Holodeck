//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SiArray.h
//
// DESCRIPTION: Contains definition for the utility class SiArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Apr 2003		 R. Wagner	 File created.
//*************************************************************************

#ifndef __SIARRAY_H__
#define __SIARRAY_H__

#include <windows.h>


namespace SiUtils
{
	//*************************************************************************
	// Generic growable array container.  Methods must be implemented in the
	// header as it is a template class.
	//*************************************************************************
	template <class T>
	class SiArray
	{
	private:
		T* buffer;
		unsigned int count;
		unsigned int allocSize;

	public:
		//*************************************************************************
		// Method:	 SiArray::SiArray
		// Description: Default constructor for the SiArray class
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		SiArray(): count(0), allocSize(0), buffer(NULL)
		{
		}


		//*************************************************************************
		// Method:	 SiArray::SiArray
		// Description: Copy constructor for the SiArray class
		// Parameters:
		//	array - Array to copy
		//
		// Return Value: None
		//*************************************************************************
		SiArray(const SiArray<T>& array): count(0), allocSize(0), buffer(NULL)
		{
			// Only allocate space if there were actually elements in the old array
			if ((array.buffer == NULL) || (array.count == 0))
				return;

			// Copy over the elements from the old array
			buffer = new T[array.count];
			allocSize = array.count;
			count = array.count;
			for (unsigned int i=0; i<count; i++)
				buffer[i] = array.buffer[i];
		}


		//*************************************************************************
		// Method:	 SiArray::~SiArray
		// Description: Destructor for the SiArray class
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		~SiArray()
		{
			Clear();
		}


		//*************************************************************************
		// Method:	 SiArray::Clear
		// Description: Removes all elements from the array
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		void Clear()
		{
			if (buffer)
			{
				delete[] buffer;
				buffer = NULL;
				allocSize = count = 0;
			}
		}


		//*************************************************************************
		// Method:	 SiArray::GetLength
		// Description: Returns the current number of elements in the array
		// Parameters:
		//	None
		//
		// Return Value: Number of elements in the array
		//*************************************************************************
		unsigned int GetLength() const
		{
			return count;
		}

		//*************************************************************************
		// Method:	 SiArray::GetSize
		// Description: Returns the allocated size of the array
		// Parameters:
		//	None
		//
		// Return Value: Number of elements in the array
		//*************************************************************************
		unsigned int GetSize() const
		{
			return allocSize;
		}


		//*************************************************************************
		// Method:	 SiArray::operator=
		// Description: Assignment operator for the SiArray class
		// Parameters:
		//	array - Array to copy into this instance
		//
		// Return Value: Reference to the assigned value
		//*************************************************************************
		const SiArray<T>& operator =(const SiArray<T>& array)
		{
			// First free any existing buffers
			if (buffer)
			{
				delete[] buffer;
				buffer = NULL;
				allocSize = count = 0;
			}

			// Allocate a new buffer and copy over all the elements
			if ((array.buffer != NULL) && (array.count > 0))
			{
				buffer = new T[array.count];
				allocSize = array.count;
				count = array.count;
				for (unsigned int i=0; i<count; i++)
					buffer[i] = array.buffer[i];
			}

			return *this;
		}


		//*************************************************************************
		// Method:	 SiArray::operator[]
		// Description: Returns a specific element from the array
		// Parameters:
		//	pos - Index of element
		//
		// Return Value: Reference to element at the requested index
		//*************************************************************************
		T& operator [](unsigned int pos)
		{
			return buffer[pos];
		}


		//*************************************************************************
		// Method:	 SiArray::operator[]
		// Description: Returns the const form of a specific element from the array.
		//				This method is required to retrieve elements from arrays
		//				marked with const.
		// Parameters:
		//	pos - Index of element
		//
		// Return Value: Reference to element at the requested index
		//*************************************************************************
		const T& operator [](unsigned int pos) const
		{
			return buffer[pos];
		}


		//*************************************************************************
		// Method:	 SiArray::Add
		// Description: Adds a new element to the end of the array
		// Parameters:
		//	value - Element to add
		//
		// Return Value: None
		//*************************************************************************
		void Add(const T& value)
		{
			Insert(count, value);
		}


		//*************************************************************************
		// Method:	 SiArray::Insert
		// Description: Inserts a new element into the array
		// Parameters:
		//	pos - Index to insert at.  The element is inserted just before the
		//		element at this index.
		//  value - Element to insert
		//
		// Return Value: None
		//*************************************************************************
		void Insert(unsigned int pos, const T& value)
		{
			if (count >= allocSize)
			{
				// Already at max capacity, so we need to enlarge the buffer
				EnsureSize(((allocSize + 1) * 3) / 2); // Make the buffer 150% of the required capacity
			}

			// Make room for the new element
			for (unsigned int i = count; i > pos; i--)
				buffer[i] = buffer[i - 1];

			// Add the new element
			buffer[pos] = value;
			count++;
		}


		//*************************************************************************
		// Method:	 SiArray::InsertRange
		// Description: Inserts new elements into the array
		// Parameters:
		//	pos - Index to insert at.  The elements are inserted just before the
		//		element at this index.
		//  value - Elements to insert
		//
		// Return Value: None
		//*************************************************************************
		void InsertRange(unsigned int pos, const SiArray<T>& value)
		{
			if ((count + value.GetLength()) > allocSize)
			{
				// Already at max capacity, so we need to enlarge the buffer
				EnsureSize(((allocSize + value.GetLength()) * 3) / 2); // Make the buffer 150% of the required capacity
			}

			// Make room for the new element
			for (unsigned int i = count + value.GetLength() - 1; i >= (pos + value.GetLength()); i--)
				buffer[i] = buffer[i - value.GetLength()];

			// Add the new elements
			for (unsigned int i = 0; i < value.GetLength(); i++)
				buffer[pos + i] = value[i];
			count += value.GetLength();
		}


		//*************************************************************************
		// Method:	 SiArray::Remove
		// Description: Removes the element at a specific index
		// Parameters:
		//	pos - Index of element to remove
		//
		// Return Value: None
		//*************************************************************************
		void Remove(unsigned int pos)
		{
			// Shift all elements after this one back
			for (unsigned int i = pos; (i + 1) < count; i++)
				buffer[i] = buffer[i + 1];

			count--;
		}


		//*************************************************************************
		// Method:	 SiArray::RemoveRange
		// Description: Removes elements at a specific index
		// Parameters:
		//	pos - Index of first element to remove
		//  num - Number of elements to remove
		//
		// Return Value: None
		//*************************************************************************
		void RemoveRange(unsigned int pos, unsigned int num)
		{
			// Shift all elements after the range to be deleted back
			for (unsigned int i = pos; (i + num) < count; i++)
				buffer[i] = buffer[i + num];

			count -= num;
		}


		//*************************************************************************
		// Method:	 SiArray::EnsureSize
		// Description: Preallocates space for a given number of elements
		// Parameters:
		//	sizeRequest - Requested number of elements to preallocate
		//
		// Return Value: None
		//*************************************************************************
		void EnsureSize(unsigned int sizeRequest)
		{
			if (allocSize >= sizeRequest)
			{
				// Already have the requested size
				return;
			}

			// Enlarge the buffer
			allocSize = sizeRequest;
			T* newBuffer = new T[allocSize];
			for (unsigned int i=0; i<count; i++)
				newBuffer[i] = buffer[i];
			
			if (buffer)
				delete[] buffer;
			buffer = newBuffer;
		}


		//*************************************************************************
		// Method:	 SiArray::SetAtIndex
		// Description: Sets an element, making sure the array is large enough
		//				to hold pos+1 elements
		// Parameters:
		//	pos - Position
		//  value - Value to store
		//
		// Return Value: None
		//*************************************************************************
		void SetAtIndex(unsigned int pos, const T& value)
		{
			while (count <= pos)
			{
				// Not enough elements
				Add(T());
			}

			// Set the element at the requested position
			buffer[pos] = value;
		}

		//*************************************************************************
		// Method:	 SiArray::IndexOf
		// Description: Finds an element and returns the index
		// Parameters:
		//  value - Value to search for
		//
		// Return Value: Index of the found element, or -1 if not found
		//*************************************************************************
		int IndexOf(const T& value)
		{
			for (unsigned int i=0; i<count; i++)
			{
				if (buffer[i] == value)
					return i;
			}
			return -1;
		}
	};
}


#endif
