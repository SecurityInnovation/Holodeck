//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		FileCorrupt.cpp
//
// DESCRIPTION: Contains functions to corrupt files
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 22 Oct 2003		 R. Wagner	 File created.
//*************************************************************************

#include "Corrupt.h"
#include "ReplacementLibrary.h"
#include <shlwapi.h>

using namespace std;
using namespace SiUtils;


CRITICAL_SECTION fileCorruptCS;


class FileStream
{
	HANDLE hFile;
	__int64 size;
	BYTE buffer[4096];
	__int64 bufferBegin, bufferEnd;

	void FillBuffer(__int64 begin)
	{
		if (begin >= size)
		{
			// Past end of file
			bufferBegin = begin;
			bufferEnd = begin;
			return;
		}

		bufferBegin = begin;
		bufferEnd = begin + 4096;
		if (bufferEnd > size)
			bufferEnd = size;

		LONG posLow = (LONG)bufferBegin;
		LONG posHigh = (LONG)(bufferBegin >> 32);
		SetFilePointer(hFile, posLow, &posHigh, FILE_BEGIN);

		DWORD dwBytes;
		ReadFile(hFile, buffer, (DWORD)(bufferEnd - bufferBegin), &dwBytes, NULL);
	}

	char Read(__int64 pos)
	{
		if ((pos >= bufferBegin) && (pos < bufferEnd))
			return buffer[(int)(pos - bufferBegin)];

		// Fill buffer on a 4096 byte boundary
		FillBuffer(pos & (~4095L));
		return buffer[(int)(pos - bufferBegin)];
	}

public:
	class FileStreamIterator: public iterator<random_access_iterator_tag, char, __int64>
	{
		FileStream* stream;
		__int64 pos;

	public:
		FileStreamIterator(FileStream* s, __int64 p)
		{
			stream = s;
			pos = p;
		}

		FileStreamIterator()
		{
			stream = NULL;
			pos = 0;
		}

		char operator*() const
		{
			return stream->Read(pos);
		}

		FileStreamIterator& operator++()
		{
			pos++;
			return *this;
		}

		FileStreamIterator operator++(int)
		{
			FileStreamIterator cur = *this;
			pos++;
			return cur;
		}

		FileStreamIterator& operator+=(int n)
		{
			pos += n;
			return *this;
		}

		FileStreamIterator& operator--()
		{
			pos--;
			return *this;
		}

		FileStreamIterator operator--(int)
		{
			FileStreamIterator cur = *this;
			pos--;
			return cur;
		}

		FileStreamIterator& operator-=(int n)
		{
			pos -= n;
			return *this;
		}

		FileStreamIterator operator+(int n) const
		{
			return FileStreamIterator(stream, pos + n);
		}

		FileStreamIterator operator-(int n) const
		{
			return FileStreamIterator(stream, pos - n);
		}

		__int64 operator-(const FileStreamIterator& a) const
		{
			return pos - a.pos;
		}

		bool operator==(const FileStreamIterator& i) const
		{
			return pos == i.pos;
		}

		bool operator!=(const FileStreamIterator& i) const
		{
			return pos != i.pos;
		}

		bool operator<(const FileStreamIterator& i) const
		{
			return pos < i.pos;
		}

		bool operator>(const FileStreamIterator& i) const
		{
			return pos > i.pos;
		}
	};

	FileStream(const SiString& sFileName)
	{
		hFile = CreateFile((char*)sFileName, GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING,  0, NULL);
		DWORD sizeLow, sizeHigh;
		sizeLow = GetFileSize(hFile, &sizeHigh);
		size = ((__int64)sizeHigh << 32) | sizeLow;

		FillBuffer(0);
	}

	~FileStream()
	{
		if (hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
	}

	bool IsValid()
	{
		return (hFile != INVALID_HANDLE_VALUE);
	}

	FileStreamIterator begin()
	{
		return FileStreamIterator(this, 0);
	}

	FileStreamIterator end()
	{
		return FileStreamIterator(this, size);
	}
};


void WriteToFile(FileStream::FileStreamIterator begin, FileStream::FileStreamIterator end, void* file)
{
	BYTE buffer[4096];
	// Write to the file in 4k chunks
	for (; begin != end; )
	{
		int size = 4096;
		if ((end - begin) < size)
			size = (int)(end - begin);

		for (int i=0; i<size; i++, begin++)
			buffer[i] = *begin;

		DWORD dwBytes;
		WriteFile((HANDLE)file, buffer, size, &dwBytes, NULL);
	}
}


void WriteCharsToFile(const char* begin, const char* end, void* file)
{
	DWORD dwBytes;
	WriteFile((HANDLE)file, begin, (DWORD)(end - begin), &dwBytes, NULL);
}


bool CorruptFile(const SiString& sOrigFileName, SiString& sNewFileName, SiString& sChangesXmlFileName,
				 const SiString& sNewFileFolder)
{
	ModificationList modList;
	bool alwaysRegen;

	if (!settingsManager->GetFileCorruptionFaults(sOrigFileName, modList, alwaysRegen))
		return false;

	if (!alwaysRegen)
	{
		// When not regenerating every time, we need to ensure only one thread tries to corrupt
		EnterCriticalSection(&fileCorruptCS);

		SiString corruptedFile, changesXmlFileName;
		if (settingsManager->GetLastFileCorruption(sOrigFileName, corruptedFile, changesXmlFileName))
		{
			if (PathFileExistsW((wchar_t*)changesXmlFileName) &&
				PathFileExistsW((wchar_t*)corruptedFile))
			{
				sChangesXmlFileName = changesXmlFileName;
				sNewFileName = corruptedFile;

				LeaveCriticalSection(&fileCorruptCS);
				return true;
			}
		}
	}

	FileStream fs(sOrigFileName);
	if (!fs.IsValid())
	{
		if (!alwaysRegen)
			LeaveCriticalSection(&fileCorruptCS);
		return false;
	}

	if (!PathFileExistsW((wchar_t*)sNewFileFolder))
		CreateDirectoryW((wchar_t*)sNewFileFolder, NULL);

	HANDLE hOutFile = CreateFile((char*)sNewFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
	{
		if (!alwaysRegen)
			LeaveCriticalSection(&fileCorruptCS);
		return false;
	}

	FILE* changesXmlFile = fopen((char*)sChangesXmlFileName, "w");
	if (!changesXmlFile)
	{
		CloseHandle(hOutFile);
		DeleteFile((char*)sNewFileName);
		if (!alwaysRegen)
			LeaveCriticalSection(&fileCorruptCS);
		return false;
	}

	fprintf(changesXmlFile, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n");
	fprintf(changesXmlFile, "<!DOCTYPE CorruptionLog SYSTEM \"");

	SiString dtdFolder;
	if (settingsManager->GetDTDFolder(dtdFolder))
	{
		if (((char*)dtdFolder)[dtdFolder.GetLength() - 1] == '\\')
			fprintf(changesXmlFile, "%s", (char*)dtdFolder); // already has trailing backslash
		else
			fprintf(changesXmlFile, "%s\\", (char*)dtdFolder); // doesn't already have trailing backslash
	}

	fprintf(changesXmlFile, "FileCorruptChanges.dtd\"[]>\n");
	fprintf(changesXmlFile, "<CorruptionLog>\n");
	fprintf(changesXmlFile, "\t<OriginalFile>%s</OriginalFile>\n", (char*)sOrigFileName);
	fprintf(changesXmlFile, "\t<CorruptedFile>%s</CorruptedFile>\n", (char*)sNewFileName);
	fprintf(changesXmlFile, "\t<Actions>\n");

	CorruptData(fs.begin(), fs.end(), modList, WriteToFile, WriteCharsToFile, hOutFile, changesXmlFile);
	CloseHandle(hOutFile);

	fprintf(changesXmlFile, "\t</Actions>\n");
	fprintf(changesXmlFile, "</CorruptionLog>\n");
	fclose(changesXmlFile);

	// Compress the XML file to save space (no effect on non-NTFS volumes)
	HANDLE hChangesFile = CreateFile(sChangesXmlFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hChangesFile != INVALID_HANDLE_VALUE)
	{
		USHORT compressionFormat = COMPRESSION_FORMAT_DEFAULT;
		DWORD dwBytes;
		DeviceIoControl(hChangesFile, FSCTL_SET_COMPRESSION, &compressionFormat, sizeof(USHORT), NULL, 0, &dwBytes, NULL);
		CloseHandle(hChangesFile);
	}

	settingsManager->SetLastFileCorruption(sOrigFileName, sNewFileName, sChangesXmlFileName);

	if (!alwaysRegen)
		LeaveCriticalSection(&fileCorruptCS);
	return true;
}


SiString FindNextCorruptedFileName(SiString basePath, SiString ext)
{
	for (int i=1; ; i++)
	{
		char indexStr[32];
		_ltoa(i, indexStr, 10);

		SiString path = basePath;
		path += "_";
		path += indexStr;

		SiString corruptFilePath = path;
		corruptFilePath += ext;

		SiString changesPath = path;
		changesPath += "_Changes.xml";

		if ((!PathFileExistsW((wchar_t*)corruptFilePath)) &&
			(!PathFileExistsW((wchar_t*)changesPath)))
			return path;
	}
}


HANDLE WINAPI FileCorrupt_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
									  LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
									  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!realCreateFileA)
		realCreateFileA = (CreateFileAFunction)GetOriginalFunctionCaller("CreateFileA", "kernel32.dll");

	if ((lpFileName == NULL) || (!strncmp(lpFileName, "\\\\", 2)))
	{
		return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	char fullPath[MAX_PATH];
	if (!GetFullPathName(lpFileName, MAX_PATH, fullPath, NULL))
	{
		return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	if (!GetLongPathName(fullPath, fullPath, MAX_PATH))
	{
		return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	char fileName[_MAX_FNAME];
	char fileExt[_MAX_EXT];
	char pidStr[32];

	_ltoa(GetCurrentProcessId(), pidStr, 10);

	_splitpath(fullPath, NULL, NULL, fileName, fileExt);
	SiString savePath;
	settingsManager->GetProjectFolder(savePath);
	SiString title;
	settingsManager->GetProjectTitle(title);
	savePath += title;
	savePath += "_CorruptedFiles";

	SiString saveFolder = savePath;

	savePath += "\\";
	savePath += fileName;
	savePath += "_";
	savePath += pidStr;

	savePath = FindNextCorruptedFileName(savePath, fileExt);

	SiString changesXmlFileName = savePath;
	savePath += fileExt;
	changesXmlFileName += "_Changes.xml";

	strlwr(fullPath);

	if (CorruptFile(fullPath, savePath, changesXmlFileName, saveFolder))
	{
		Log::LogEvent event;
		event.SetType(Log::FileCorruptionEvent);
		event.SetData((char*)changesXmlFileName, changesXmlFileName.GetLength() + 1);
		ReplacementLibrary::AddEventToCurrentEntry(event);
		return CreateFileA((char*)savePath, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	return CreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


HANDLE WINAPI FileCorrupt_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
									  LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
									  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (!realCreateFileW)
		realCreateFileW = (CreateFileWFunction)GetOriginalFunctionCaller("CreateFileW", "kernel32.dll");

	if ((lpFileName == NULL) || (!wcsncmp(lpFileName, L"\\\\", 2)))
	{
		return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	wchar_t fullPath[MAX_PATH];
	if (!GetFullPathNameW(lpFileName, MAX_PATH, fullPath, NULL))
	{
		return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	if (!GetLongPathNameW(fullPath, fullPath, MAX_PATH))
	{
		return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	wchar_t fileName[_MAX_FNAME];
	wchar_t fileExt[_MAX_EXT];
	wchar_t pidStr[32];

	_ltow(GetCurrentProcessId(), pidStr, 10);

	_wsplitpath(fullPath, NULL, NULL, fileName, fileExt);
	SiString savePath;
	settingsManager->GetProjectFolder(savePath);
	SiString title;
	settingsManager->GetProjectTitle(title);
	savePath += title;
	savePath += "_CorruptedFiles";

	SiString saveFolder = savePath;

	savePath += "\\";
	savePath += fileName;
	savePath += "_";
	savePath += pidStr;

	savePath = FindNextCorruptedFileName(savePath, fileExt);

	SiString changesXmlFileName = savePath;
	savePath += fileExt;
	changesXmlFileName += "_Changes.xml";

	wcslwr(fullPath);

	if (CorruptFile(fullPath, savePath, changesXmlFileName, saveFolder))
	{
		Log::LogEvent event;
		event.SetType(Log::FileCorruptionEvent);
		event.SetData((char*)changesXmlFileName, changesXmlFileName.GetLength() + 1);
		ReplacementLibrary::AddEventToCurrentEntry(event);
		return CreateFileW((wchar_t*)savePath, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


void InitFileCorruption()
{
	InitializeCriticalSection(&fileCorruptCS);
}


void CloseFileCorruption()
{
	DeleteCriticalSection(&fileCorruptCS);
}
