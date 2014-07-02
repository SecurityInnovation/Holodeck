#include "memorymappedfile.h"

namespace SiUtils
{
	//*************************************************************************
	// Method:		MemoryMappedFile
	// Description: Constructor for the MemoryMappedFile class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MemoryMappedFile::MemoryMappedFile()
	{
	}

	//*************************************************************************
	// Method:		MemoryMappedFile
	// Description: Destructor for the MemoryMappedFile class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	MemoryMappedFile::~MemoryMappedFile()
	{
	}

	//*************************************************************************
	// Method:		Create
	// Description: Creates a new memory mapped file
	//
	// Parameters:
	//	name - The name to identify this mapped file
	//	size - The number of bytes that this file will be able to contain
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool MemoryMappedFile::Create(const SiString &name, unsigned int size)
	{
		fileName = name;

		PSECURITY_DESCRIPTOR pSD;
		SECURITY_ATTRIBUTES sa;

		pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
		// Initialize a security attributes structure.
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = FALSE;

		// add to the size from what was passed in so we have room for the
		// size of the contents
		fileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, size + sizeof(unsigned int), name);
		if (fileHandle == INVALID_HANDLE_VALUE)
			return false;

		mapView = MapViewOfFile(fileHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (mapView == NULL)
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		Open
	// Description: Opens an existing memory mapped file
	//
	// Parameters:
	//	name - The name to identify this mapped file
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool MemoryMappedFile::Open(const SiString &name)
	{
		fileName = name;
		fileHandle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, false, name);
		if (fileHandle == INVALID_HANDLE_VALUE)
			return false;

		mapView = MapViewOfFile(fileHandle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (mapView == NULL)
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		Close
	// Description: Closes an existing memory mapped file
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void MemoryMappedFile::Close()
	{
		if (mapView != NULL)
		{
			UnmapViewOfFile(mapView);
			mapView = NULL;
		}

		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(fileHandle);
			fileHandle = INVALID_HANDLE_VALUE;
		}
	}

	//*************************************************************************
	// Method:		Write
	// Description: Writes a string to the memory mapped file
	//
	// Parameters:
	//	str - The data to write to the file
	//	numBytes - The number of bytes in str
	//
	// Return Value: None
	//*************************************************************************
	void MemoryMappedFile::Write(char *str, unsigned int numBytes)
	{
		if (mapView != NULL)
		{
			CopyMemory(mapView, &numBytes, sizeof(unsigned int));
			CopyMemory((char *)mapView + sizeof(unsigned int), str, numBytes);
		}
	}

	//*************************************************************************
	// Method:		GetNumberOfBytesInFile
	// Description: Gets the number of bytes of data in the file
	//
	// Parameters:
	//	None
	//
	// Return Value: The number of bytes of data in the file
	//*************************************************************************
	unsigned int MemoryMappedFile::GetNumberOfBytesInFile()
	{
		if (mapView == NULL)
			return 0;

		unsigned int retVal;
		CopyMemory(&retVal, mapView, sizeof(unsigned int));

		return retVal;
	}

	//*************************************************************************
	// Method:		Read
	// Description: Reads a string from the memory mapped file
	//
	// Parameters:
	//	str - The data to read from the file
	//
	// Return Value: None
	//*************************************************************************
	void MemoryMappedFile::Read(char *str)
	{
		if ((mapView == NULL) || (str == NULL))
			return;

		unsigned int numBytes = GetNumberOfBytesInFile();
		CopyMemory(str, (char *)mapView + sizeof(unsigned int), numBytes);
	}
}