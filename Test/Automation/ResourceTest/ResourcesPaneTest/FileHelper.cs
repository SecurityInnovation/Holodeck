using System;
using System.Collections;
using TestFramework.Log;

namespace ResourcesPaneTest
{
	/// <summary>
	/// Summary description for FileHelper.
	/// </summary>
	public class FileHelper : Helper
	{
		public FileHelper ()
		{
			Reset ();
		}

		public override void Reset ()
		{
			base.Reset ();
		}

		protected static string GetCanonName (string pathName)
		{
			string[] pathElements = pathName.Split ('\\');
			System.Collections.ArrayList newPathElements = new System.Collections.ArrayList ();
			newPathElements.Add (pathElements[0]);
			for (int i = 1; i < pathElements.Length; i++) 
			{
				if ((pathElements[i] == "..") &&
					(((string) newPathElements[newPathElements.Count - 1]) != "..")) 
				{
					newPathElements.RemoveAt (newPathElements.Count - 1);
				} 
				else 
				{
					newPathElements.Add (pathElements[i]);
				}
			}

			string newPath = "";
			foreach (string pathElement in newPathElements)
			{
				newPath += pathElement + "\\";
			}

			newPath = newPath.TrimEnd ('\\');

			return newPath;
		}

		protected static string GetPathName (string rawPathName)
		{
			const string NoPathParsing = "\\\\?\\";
			const string UNCPath = "\\\\?\\UNC\\";
			string pathName = rawPathName;
			if (pathName.StartsWith (NoPathParsing))
			{
				pathName.Remove (0, NoPathParsing.Length);
			} 
			else if (pathName.StartsWith (UNCPath))
			{
				pathName.Remove (0, UNCPath.Length);
			}

			return GetCanonName (pathName);
		}

		//------------Native Functions---------------//

		protected void Process__hread (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process__hwrite (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process__lclose (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
			RemoveFromHandleTable (handleEntry, fileName);
		}

		protected void Process__lcreat (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0") &&
				(logEntry[(int) LogColumns.ReturnValue] != "-1"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process__llseek (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process__lopen (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0") &&
				(logEntry[(int) LogColumns.ReturnValue] != "-1"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process__lread (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process__lwrite (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_BackupEventLog (String[] logEntry)
		{
		}

		protected void Process_BackupRead (String[] logEntry)
		{
		}

		protected void Process_BackupSeek (String[] logEntry)
		{
		}

		protected void Process_BackupWrite (String[] logEntry)
		{
		}

		protected void Process_ClearEventLog (String[] logEntry)
		{
		}

		protected void Process_CloseHandle (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			if (handleTable.Contains (handleEntry)) 
			{
				string fileName = GetHandleTableEntry (handleEntry);
				IncreaseEntryTableCount (fileName, logEntry);
				RemoveFromHandleTable (handleEntry, fileName);
			}
		}

		protected void Process_CopyFile (String[] logEntry)
		{
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_CopyFileEx (String[] logEntry)
		{
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_CopyLZFile (String[] logEntry)
		{
			// obsolete
		}

		protected void Process_CreateDirectory (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_CreateDirectoryEx (String[] logEntry)
		{
			string templateName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (templateName, logEntry);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_CreateFile (String[] logEntry)
		{
			if (logEntry[(int) LogColumns.Parameter2] == "0") 
			{
				// there was no device access, only information query
				return;
			} 

			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			if (fileName.StartsWith ("\\\\.\\") ||
				(fileName == "CON"))
			{
				// named pipe, mailslot, physical drive, logical partition or console access
				return;
			}
			IncreaseEntryTableCount (fileName, logEntry);

			if (logEntry[(int) LogColumns.ErrorCode] != "ERROR_ALREADY_EXISTS") 
			{
				string handleEntry = logEntry[(int) LogColumns.Parameter7];
				if (handleEntry != "0") 
				{
					string templateName = GetHandleTableEntry (handleEntry);
					IncreaseEntryTableCount (templateName, logEntry);
				}
			}

			if ((logEntry[(int) LogColumns.ReturnValue] != "0") &&
				(logEntry[(int) LogColumns.ReturnValue] != "-1"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_CreateFileMapping (String[] logEntry)
		{
/*
			// TODO: see MSDN for File Mapping Functions
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName);

			if ((logEntry[(int) LogColumns.ReturnValue] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
*/
		}

		protected void Process_CreateFileMoniker (String[] logEntry)
		{
		}

		protected void Process_CreateFilePKCS10 (String[] logEntry)
		{
		}

		protected void Process_CreateHardLink (String[] logEntry)
		{
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_DecryptFile (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_DeleteFile (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_EncryptFile (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_FileEncryptionStatus (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_FindFirstFile (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_FindFirstFileEx (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_FindNextFile (String[] logEntry)
		{
			// TODO: figure out how to handle this (access to a WIN32_FIND_DATA structure is needed)
		}

		protected void Process_FlushFileBuffers (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetBinaryType (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetCompressedFileSize (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileAttributes (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileAttributesEx (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileInformationByHandle (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileSecurity (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileSize (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileSizeEx (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileTime (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFileType (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_GetFullPathName (String[] logEntry) {
			// TODO: this filename might not be a full path
			string fileName = logEntry[(int) LogColumns.Parameter1];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_LoadLibrary (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			if (fileName.LastIndexOf (".") == -1) 
			{
				fileName += ".dll";
			}
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_LoadLibraryEx (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			if (fileName.LastIndexOf (".") == -1) 
			{
				fileName += ".dll";
			}
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_LockFile (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_LockFileEx (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_LZClose (String[] logEntry)
		{
		}

		protected void Process_LZCopy (String[] logEntry)
		{
		}

		protected void Process_LZDone (String[] logEntry)
		{
		}

		protected void Process_LZOpenFile (String[] logEntry)
		{
		}

		protected void Process_LZSeek (String[] logEntry)
		{
		}

		protected void Process_MapViewOfFile (String[] logEntry)
		{
		}

		protected void Process_MapViewOfFileEx (String[] logEntry)
		{
		}

		protected void Process_MoveFile (String[] logEntry)
		{
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_MoveFileEx (String[] logEntry)
		{
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_MoveFileWithProgress (String[] logEntry)
		{
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_OpenBackupEventLog (String[] logEntry)
		{
		}

		protected void Process_OpenEventLog (String[] logEntry)
		{
		}

		protected void Process_OpenFile (String[] logEntry)
		{
			// TODO: look it up in MSDN, pay attention to deatails
		}

		protected void Process_QueryUsersOnEncryptedFile (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_ReadFile (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_ReadFileEx (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_ReadFileScatter (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_RegLoadKey (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter3]);
			// TODO: figure out how to get remote file path
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_RegReplaceKey (String[] logEntry)
		{
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter3]);
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter4]);
			// TODO: figure out how to get remote file path
			IncreaseEntryTableCount (newFileName, logEntry);
			IncreaseEntryTableCount (oldFileName, logEntry);
		}

		protected void Process_RegRestoreKey (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			// TODO: figure out how to get remote file path
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_RegSaveKey (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			// TODO: figure out how to get remote file path
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_RegSaveKeyEx (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			// TODO: figure out how to get remote file path
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_RemoveDirectory (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_RemoveUsersFromEncryptedFile (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_SaferRecordEventLogEntry (String[] logEntry)
		{
		}

		protected void Process_SearchPath (String[] logEntry)
		{
		}

		protected void Process_SetEndOfFile (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_SetFileAttributes (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_SetFilePointer (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_SetFileSecurity (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_SetFileShortName (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			string shortName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (fileName, logEntry);
			IncreaseEntryTableCount (shortName, logEntry);
		}

		protected void Process_SetFileTime (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_SetFileValidData (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_UnlockFile (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_UnlockFileEx (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_WriteFile (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_WriteFileEx (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_WriteFileGather (String[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (fileName, logEntry);
		}


		//------------.NET Functions---------------//

		protected void Process_System_Console_SetIn (String[] logEntry)
		{
		}

		protected void Process_System_Console_SetOut (String[] logEntry)
		{
		}

		protected void Process_System_Console_SetError (String[] logEntry)
		{
		}

		protected void Process_System_Console_ReadLine (String[] logEntry)
		{
		}

		protected void Process_System_Console_WriteLine (String[] logEntry)
		{
		}

		protected void Process_System_Console_Write (String[] logEntry)
		{
		}

		protected void Process_System_Security_Cryptography_X509Certificates_X509Certificate_CreateFromCertFile (String[] logEntry)
		{
		}

		protected void Process_System_Security_Cryptography_X509Certificates_X509Certificate_CreateFromSignedFile (String[] logEntry)
		{
		}

		protected void Process_System_IO_BinaryWriter_Write (String[] logEntry)
		{
		}

		protected void Process_System_IO_BinaryWriter_Seek (String[] logEntry)
		{
		}

		protected void Process_System_IO_BinaryWriter_Flush (String[] logEntry)
		{
		}

		protected void Process_System_IO_BinaryWriter_Close (String[] logEntry)
		{
		}

		protected void Process_System_IO_BinaryWriter_ctor (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_WriteByte (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_Write (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_ReadByte (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_Read (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_SetLength (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_Seek (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_EndWrite (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_BeginWrite (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_EndRead (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_BeginRead (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_Flush (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_Close (String[] logEntry)
		{
		}

		protected void Process_System_IO_BufferedStream_ctor (String[] logEntry)
		{
		}

		protected void Process_System_IO_Directory_GetParent (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_CreateDirectory (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_Exists (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_SetCreationTime (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_GetCreationTime (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_SetLastWriteTime (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_GetLastWriteTime (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_SetLastAccessTime (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_GetLastAccessTime (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_GetFiles (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_GetDirectories (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_GetFileSystemEntries (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_SetCurrentDirectory (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_Directory_Move (String[] logEntry)
		{
			string oldDirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newDirName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldDirName, logEntry);
			IncreaseEntryTableCount (newDirName, logEntry);
		}

		protected void Process_System_IO_Directory_Delete (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_CreateSubdirectory (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			// TODO: use heuristics for the 'this' handle...
			// repeat for the others too
			string dirName = (string) thisTable[thisEntry];
			string subdirName = GetPathName (dirName + "\\" + logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (dirName, logEntry);
			IncreaseEntryTableCount (subdirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_Create (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string dirName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_GetFiles (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string dirName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_GetDirectories (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string dirName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_GetFileSystemInfos (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string dirName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_MoveTo (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string oldDirName = (string) thisTable[thisEntry];
			string newDirName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldDirName, logEntry);
			IncreaseEntryTableCount (newDirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_Delete (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string dirName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (dirName, logEntry);
		}

		protected void Process_System_IO_DirectoryInfo_ctor (String[] logEntry)
		{
			string dirName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], dirName);
			}
		}

		protected void Process_System_IO_File_OpenText (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_File_CreateText (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_File_AppendText (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_File_Create (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_File_Delete (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_Exists (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_Open (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_SetCreationTime (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_GetCreationTime (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_SetLastAccessTime (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_GetLastAccessTime (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_SetLastWriteTime (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_GetLastWriteTime (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_File_OpenRead (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_File_OpenWrite (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_File_Move (String[] logEntry)
		{
			string oldFileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_System_IO_FileInfo_Delete (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileInfo_OpenText (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileInfo_CreateText (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileInfo_AppendText (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileInfo_Create (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileInfo_Open (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileInfo_OpenWrite (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if ((logEntry[(int) LogColumns.Exception] == "") &&
				(logEntry[(int) LogColumns.ReturnValue] != "0")) 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileInfo_MoveTo (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string oldFileName = (string) thisTable[thisEntry];
			string newFileName = GetPathName (logEntry[(int) LogColumns.Parameter2]);
			IncreaseEntryTableCount (oldFileName, logEntry);
			IncreaseEntryTableCount (newFileName, logEntry);
		}

		protected void Process_System_IO_FileInfo_ctor (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_FileStream_Unlock (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_Lock (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_WriteByte (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_Write (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_ReadByte (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_Read (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_SetLength (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_Seek (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_EndWrite (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_BeginWrite (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_EndRead (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_BeginRead (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_Flush (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_FileStream_Close (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				thisTable.Remove (thisEntry);
			}
		}

		protected void Process_System_IO_FileStream_ctor (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_TextReader_ReadLine (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextReader_ReadBlock (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextReader_ReadToEnd (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextReader_Read (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextReader_Close (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				thisTable.Remove (thisEntry);
			}
		}

		protected void Process_System_IO_TextReader_ctor (String[] logEntry)
		{
			string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
			}
		}

		protected void Process_System_IO_StreamReader_ReadLine (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_StreamReader_ReadToEnd (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_StreamReader_Read (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_StreamReader_Close (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				thisTable.Remove (thisEntry);
			}
		}

		protected void Process_System_IO_StreamReader_ctor (String[] logEntry)
		{
			if (logEntry[(int) LogColumns.Function].StartsWith ("Process.System.IO.StreamReader.ctor(string")) 
			{
				string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
				IncreaseEntryTableCount (fileName, logEntry);
				if (logEntry[(int) LogColumns.Exception] == "") 
				{
					AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
				}
			} 
			else if (logEntry[(int) LogColumns.Function].StartsWith ("Process.System.IO.StreamReader.ctor(Stream")) 
			{
				string streamEntry = logEntry[(int) LogColumns.Parameter1];
				if ((logEntry[(int) LogColumns.Exception] == "") &&
					(thisTable.Contains (streamEntry))) 
				{
					string thisEntry = logEntry[(int) LogColumns.ReturnValue];
					string fileName = (string) handleTable[streamEntry];
					IncreaseEntryTableCount (fileName, logEntry);
					AddToHandleTable (thisEntry, fileName);
				}
			} 
			else 
			{
				return;
			}
		}

		protected void Process_System_IO_TextWriter_WriteLine (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextWriter_Write (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextWriter_Flush (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_TextWriter_Close (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				thisTable.Remove (thisEntry);
			}
		}

		protected void Process_System_IO_TextWriter_ctor (String[] logEntry)
		{
			// we don't need this
		}

		protected void Process_System_IO_StreamWriter_Write (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_StreamWriter_Flush (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
		}

		protected void Process_System_IO_StreamWriter_Close (String[] logEntry)
		{
			string thisEntry = logEntry[(int) LogColumns.Parameter1];
			string fileName = (string) thisTable[thisEntry];
			IncreaseEntryTableCount (fileName, logEntry);
			if (logEntry[(int) LogColumns.Exception] == "") 
			{
				thisTable.Remove (thisEntry);
			}
		}

		protected void Process_System_IO_StreamWriter_ctor (String[] logEntry)
		{
			if (logEntry[(int) LogColumns.Function].StartsWith ("Process.System.IO.StreamWriter.ctor(string")) 
			{
				string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
				IncreaseEntryTableCount (fileName, logEntry);
				if (logEntry[(int) LogColumns.Exception] == "") 
				{
					AddToHandleTable (logEntry[(int) LogColumns.ReturnValue], fileName);
				}
			} 
			else if (logEntry[(int) LogColumns.Function].StartsWith ("Process.System.IO.StreamWriter.ctor(Stream")) 
			{
				string streamEntry = logEntry[(int) LogColumns.Parameter1];
				if ((logEntry[(int) LogColumns.Exception] == "") &&
					(thisTable.Contains (streamEntry))) 
				{
					string thisEntry = logEntry[(int) LogColumns.ReturnValue];
					string fileName = (string) handleTable[streamEntry];
					IncreaseEntryTableCount (fileName, logEntry);
					AddToHandleTable (thisEntry, fileName);
				}
			} 
			else 
			{
				return;
			}
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_Dispose (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_Remove (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetUserStoreForDomain (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetUserStoreForAssembly (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetStore (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_DeleteFile (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_CreateDirectory (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_DeleteDirectory (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetFileNames (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetDirectoryNames (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFile_Close (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Unlock (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Lock (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_WriteByte (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Write (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_ReadByte (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Read (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_SetLength (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Seek (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_EndWrite (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_BeginWrite (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_EndRead (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_BeginRead (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Flush (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Close (String[] logEntry)
		{
		}

		protected void Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Net_WebClient_DownloadFile (String[] logEntry)
		{
		}

		protected void Process_System_Net_WebClient_UploadFile (String[] logEntry)
		{
		}

		protected void Process_System_IO_FileSystemWatcher_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Diagnostics_Process_Start (String[] logEntry)
		{
			if (logEntry[(int) LogColumns.Function].StartsWith ("System.Diagnostics.Process.Start(string"))
			{
				string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
				IncreaseEntryTableCount (fileName, logEntry);
			} 
			else if (logEntry[(int) LogColumns.Function].StartsWith ("System.Diagnostics.Process.Start(ProcessStartInfo"))
			{
				string thisEntry = logEntry[(int) LogColumns.Parameter1];
				string fileName = (string) thisTable[thisEntry];
				IncreaseEntryTableCount (fileName, logEntry);
			} 
		}

		protected void Process_System_Diagnostics_ProcessStartInfo_ctor (String[] logEntry)
		{
			if (logEntry[(int) LogColumns.Function].StartsWith ("System.Diagnostics.ProcessStartInfo(string")) 
			{
				string fileName = GetPathName (logEntry[(int) LogColumns.Parameter1]);
				if (logEntry[(int) LogColumns.Exception] == "") 
				{
					string thisEntry = logEntry[(int) LogColumns.ReturnValue];
					AddToHandleTable (thisEntry, fileName);
				}
			}
		}

		protected void Process_System_Configuration_AppSettingsReader_GetValue (String[] logEntry)
		{
		}

		protected void Process_System_Configuration_AppSettingsReader_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocument_Save (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocument_LoadXml (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocument_WriteContentTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocument_WriteTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocument_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocumentFragment_WriteContentTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocumentFragment_WriteTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocumentType_WriteContentTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlDocumentType_WriteTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlWriter_WriteNode (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlWriter_WriteAttributes (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlWriter_WriteStartElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlWriter_WriteAttributeString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlWriter_WriteStartAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlWriter_WriteElementString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteNode (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteAttributes (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteQualifiedName (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteName (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteNmToken (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_Flush (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_Close (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteBinHex (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteBase64 (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteRaw (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteChars (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteSurrogateCharEntity (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteWhitespace (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteCharEntity (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteEntityRef (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteProcessingInstruction (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteComment (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteCData (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteEndAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteStartAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteFullEndElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteEndElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteStartElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteDocType (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteEndDocument (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteStartDocument (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteAttributeString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_WriteElementString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextWriter_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_IsStartElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_ReadEndElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_ReadElementString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_ReadStartElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_MoveToContent (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_Skip (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_IsName (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlReader_IsNameToken (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadAttributeValue (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ResolveEntity (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_LookupNamespace (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadOuterXml (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadInnerXml (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_IsStartElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadEndElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadElementString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadStartElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_MoveToContent (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadString (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_Skip (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_Close (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_Read (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_MoveToElement (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_MoveToNextAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_MoveToFirstAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_MoveToAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_GetAttribute (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_GetRemainder (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadChars (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadBase64 (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ReadBinHex (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlTextReader_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlEntityReference_WriteContentTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlEntityReference_WriteTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlImplementation_CreateDocument (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlImplementation_ctor (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlText_WriteContentTo (String[] logEntry)
		{
		}

		protected void Process_System_Xml_XmlText_WriteTo (String[] logEntry)
		{
		}

		public void AddEntry (String[] logEntry)
		{
			//---------Heuristics begin----------//
			if (lastTimeStamp != logEntry[(int) LogColumns.TimeStamp]) {
				// TODO: use the ReportErrors return value
				ReportErrors ();
				heuristicsBeforeOpened.Clear ();
				heuristicsAfterClosed.Clear ();
				lastTimeStamp = logEntry[(int) LogColumns.TimeStamp];
			}
			//---------Heuristics end----------//

			//---------Native Functions----------//
			if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
				((logEntry[(int) LogColumns.Function] == "CopyFileA") ||
				 (logEntry[(int) LogColumns.Function] == "CopyFileW"))) 
			{
				Process_CopyFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "CopyFileExA") ||
					  (logEntry[(int) LogColumns.Function] == "CopyFileExW")))
			{
				Process_CopyFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "CreateDirectoryA") ||
					  (logEntry[(int) LogColumns.Function] == "CreateDirectoryW"))) 
			{
				Process_CreateDirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "CreateDirectoryExA") ||
					  (logEntry[(int) LogColumns.Function] == "CreateDirectoryExW"))) 
			{
				Process_CreateDirectoryEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "CreateFileA") ||
					  (logEntry[(int) LogColumns.Function] == "CreateFileW"))) 
			{
				Process_CreateFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "DeleteFileA") ||
					  (logEntry[(int) LogColumns.Function] == "DeleteFileW"))) 
			{
				Process_DeleteFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "FlushFileBuffers")) 
			{
				Process_FlushFileBuffers (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "GetBinaryTypeA") ||
					  (logEntry[(int) LogColumns.Function] == "GetBinaryTypeW"))) 
			{
				Process_GetBinaryType (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "GetFileAttributesA") ||
					  (logEntry[(int) LogColumns.Function] == "GetFileAttributesW"))) 
			{
				Process_GetFileAttributes (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "GetFileAttributesExA") ||
					  (logEntry[(int) LogColumns.Function] == "GetFileAttributesExW"))) 
			{
				Process_GetFileAttributesEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "GetFileInformationByHandle"))
			{
				Process_GetFileInformationByHandle (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "GetFileSize"))
			{
				Process_GetFileSize (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "GetFileSizeEx")) 
			{
				Process_GetFileSizeEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "GetFileType")) 
			{
				Process_GetFileType (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "GetFullPathNameA") ||
					  (logEntry[(int) LogColumns.Function] == "GetFullPathNameW")))
			{
				Process_GetFullPathName (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "LoadLibraryA") ||
					  (logEntry[(int) LogColumns.Function] == "LoadLibraryW"))) 
			{
				Process_LoadLibrary (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "LoadLibraryExA") ||
					  (logEntry[(int) LogColumns.Function] == "LoadLibraryExW"))) 
			{
				Process_LoadLibraryEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "MapViewOfFile")) 
			{
				Process_MapViewOfFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "MapViewOfFileEx")) 
			{
				Process_MapViewOfFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "MoveFileA") ||
					  (logEntry[(int) LogColumns.Function] == "MoveFileW"))) 
			{
				Process_MoveFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "MoveFileExA") ||
					  (logEntry[(int) LogColumns.Function] == "MoveFileExW"))) 
			{
				Process_MoveFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "MoveFileWithProgressA") ||
					  (logEntry[(int) LogColumns.Function] == "MoveFileWithProgressW"))) 
			{
				Process_MoveFileWithProgress (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "ReadFile")) 
			{
				Process_ReadFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "ReadFileEx")) 
			{
				Process_ReadFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "ReadFileScatter")) 
			{
				Process_ReadFileScatter (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegLoadKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegLoadKeyW"))) 
			{
				Process_RegLoadKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegReplaceKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegReplaceKeyW"))) 
			{
				Process_RegReplaceKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegRestoreKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegRestoreKeyW"))) 
			{
				Process_RegRestoreKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegSaveKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegSaveKeyW"))) 
			{
				Process_RegSaveKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegSaveKeyExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegSaveKeyExW"))) 
			{
				Process_RegSaveKeyEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RemoveDirectoryA") ||
					  (logEntry[(int) LogColumns.Function] == "RemoveDirectoryW"))) 
			{
				Process_RemoveDirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "SearchPathA") ||
					  (logEntry[(int) LogColumns.Function] == "SearchPathW"))) 
			{
				Process_SearchPath (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "SetFileAttributesA") ||
					  (logEntry[(int) LogColumns.Function] == "SetFileAttributesW"))) 
			{
				Process_SetFileAttributes (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "WriteFile")) 
			{
				Process_WriteFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "WriteFileEx"))
			{
				Process_WriteFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "WriteFileGather")) 
			{
				Process_WriteFileGather (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "BackupRead"))
			{
				Process_BackupRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "BackupSeek")) 
			{
				Process_BackupSeek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "BackupWrite")) 
			{
				Process_BackupWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "CloseHandle")) 
			{
				Process_CloseHandle (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "CopyLZFile")) 
			{
				// obsolete
				Process_CopyLZFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "LZCopy")) 
			{
				Process_LZCopy (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "CreateFileMappingA") ||
					  (logEntry[(int) LogColumns.Function] == "CreateFileMappingW")))
			{
				Process_CreateFileMapping (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "CreateFileMoniker")) 
			{
				Process_CreateFileMoniker (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "CreateFilePKCS10")) 
			{
				Process_CreateFilePKCS10 (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "CreateHardLinkA") || 
					  (logEntry[(int) LogColumns.Function] == "CreateHardLinkW"))) 
			{
				Process_CreateHardLink (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "FindFirstFileA") || 
					  (logEntry[(int) LogColumns.Function] == "FindFirstFileW"))) 
			{
				Process_FindFirstFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "FindFirstFileExA") || 
					  (logEntry[(int) LogColumns.Function] == "FindFirstFileExW"))) 
			{
				Process_FindFirstFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "FindNextFileA") || 
					  (logEntry[(int) LogColumns.Function] == "FindNextFileW"))) 
			{
				Process_FindNextFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "GetCompressedFileSizeA") || 
					  (logEntry[(int) LogColumns.Function] == "GetCompressedFileSizeW"))) 
			{
				Process_GetCompressedFileSize (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "LockFile")) 
			{
				Process_LockFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "LockFileEx")) 
			{
				Process_LockFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "LZClose")) 
			{
				Process_LZClose (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "LZDone")) 
			{
				Process_LZDone (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "LZOpenFileA") || 
					  (logEntry[(int) LogColumns.Function] == "LZOpenFileW"))) 
			{
				Process_LZOpenFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "LZSeek")) 
			{
				Process_LZSeek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "OpenFile")) 
			{
				Process_OpenFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "SetFilePointer")) 
			{
				Process_SetFilePointer (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "SetFileShortNameA") ||
					  (logEntry[(int) LogColumns.Function] == "SetFileShortNameW"))) 
			{
				Process_SetFileShortName (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "SetFileValidData")) 
			{
				Process_SetFileValidData (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "SetEndOfFile")) 
			{
				Process_SetEndOfFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "UnlockFile")) 
			{
				Process_UnlockFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "UnlockFileEx")) 
			{
				Process_UnlockFileEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_hread")) 
			{
				Process__hread (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_hwrite")) 
			{
				Process__hwrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_lclose")) 
			{
				Process__lclose (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_lcreat")) 
			{
				Process__lcreat (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_lread")) 
			{
				Process__lread (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_lwrite")) 
			{
				Process__lwrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_llseek")) 
			{
				Process__llseek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "_lopen")) 
			{
				Process__lopen (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "BackupEventLogA") || 
					  (logEntry[(int) LogColumns.Function] == "BackupEventLogW"))) 
			{
				Process_BackupEventLog (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "ClearEventLogA") || 
					  (logEntry[(int) LogColumns.Function] == "ClearEventLogW"))) 
			{
				Process_ClearEventLog (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "DecryptFileA") || 
					  (logEntry[(int) LogColumns.Function] == "DecryptFileW")))
			{
				Process_DecryptFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "EncryptFileA") || 
					  (logEntry[(int) LogColumns.Function] == "EncryptFileW"))) 
			{
				Process_EncryptFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "FileEncryptionStatusA") || 
					  (logEntry[(int) LogColumns.Function] == "FileEncryptionStatusW"))) 
			{
				Process_FileEncryptionStatus (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "GetFileSecurityA") || 
					  (logEntry[(int) LogColumns.Function] == "GetFileSecurityW"))) 
			{
				Process_GetFileSecurity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "OpenBackupEventLogA") || 
					  (logEntry[(int) LogColumns.Function] == "OpenBackupEventLogW"))) 
			{
				Process_OpenBackupEventLog (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "OpenEventLogA") || 
					  (logEntry[(int) LogColumns.Function] == "OpenEventLogW"))) 
			{
				Process_OpenEventLog (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "QueryUsersOnEncryptedFile")) 
			{
				Process_QueryUsersOnEncryptedFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RemoveUsersFromEncryptedFile")) 
			{
				Process_RemoveUsersFromEncryptedFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "SaferRecordEventLogEntry")) 
			{
				Process_SaferRecordEventLogEntry (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "SetFileSecurityA") || 
					  (logEntry[(int) LogColumns.Function] == "SetFileSecurityW"))) 
			{
				Process_SetFileSecurity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "GetFileTime")) 
			{
				Process_GetFileTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "kernel32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "SetFileTime")) 
			{
				Process_SetFileTime (logEntry);
			}

			//---------.NET Functions----------//
			if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
				(logEntry[(int) LogColumns.Function].StartsWith ("System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromCertFile(")))
			{
				Process_System_Security_Cryptography_X509Certificates_X509Certificate_CreateFromCertFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromSignedFile(")))
			{
				Process_System_Security_Cryptography_X509Certificates_X509Certificate_CreateFromSignedFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BinaryWriter.Write(")))
			{
				Process_System_IO_BinaryWriter_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BinaryWriter.Seek(")))
			{
				Process_System_IO_BinaryWriter_Seek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BinaryWriter.Flush(")))
			{
				Process_System_IO_BinaryWriter_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BinaryWriter.Close(")))
			{
				Process_System_IO_BinaryWriter_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BinaryWriter.ctor(")))
			{
				Process_System_IO_BinaryWriter_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.WriteByte(")))
			{
				Process_System_IO_BufferedStream_WriteByte (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.Write(")))
			{
				Process_System_IO_BufferedStream_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.ReadByte(")))
			{
				Process_System_IO_BufferedStream_ReadByte (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.Read(")))
			{
				Process_System_IO_BufferedStream_Read (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.SetLength(")))
			{
				Process_System_IO_BufferedStream_SetLength (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.Seek(")))
			{
				Process_System_IO_BufferedStream_Seek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.EndWrite(")))
			{
				Process_System_IO_BufferedStream_EndWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.BeginWrite(")))
			{
				Process_System_IO_BufferedStream_BeginWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.EndRead(")))
			{
				Process_System_IO_BufferedStream_EndRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.BeginRead(")))
			{
				Process_System_IO_BufferedStream_BeginRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.Flush(")))
			{
				Process_System_IO_BufferedStream_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.Close(")))
			{
				Process_System_IO_BufferedStream_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.BufferedStream.ctor(")))
			{
				Process_System_IO_BufferedStream_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Console.SetIn("))) {
				Process_System_Console_SetIn (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Console.SetOut("))) {
				Process_System_Console_SetOut (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Console.SetError("))) {
				Process_System_Console_SetError (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Console.ReadLine("))) {
				Process_System_Console_ReadLine (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Console.WriteLine("))) {
				Process_System_Console_WriteLine (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Console.Write("))) {
				Process_System_Console_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetParent(")))
			{
				Process_System_IO_Directory_GetParent (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.CreateDirectory(")))
			{
				Process_System_IO_Directory_CreateDirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.Exists(")))
			{
				Process_System_IO_Directory_Exists (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.SetCreationTime(")))
			{
				Process_System_IO_Directory_SetCreationTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetCreationTime(")))
			{
				Process_System_IO_Directory_GetCreationTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.SetLastWriteTime(")))
			{
				Process_System_IO_Directory_SetLastWriteTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetLastWriteTime(")))
			{
				Process_System_IO_Directory_GetLastWriteTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.SetLastAccessTime(")))
			{
				Process_System_IO_Directory_SetLastAccessTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetLastAccessTime(")))
			{
				Process_System_IO_Directory_GetLastAccessTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetFiles(")))
			{
				Process_System_IO_Directory_GetFiles (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetDirectories(")))
			{
				Process_System_IO_Directory_GetDirectories (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.GetFileSystemEntries(")))
			{
				Process_System_IO_Directory_GetFileSystemEntries (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.SetCurrentDirectory(")))
			{
				Process_System_IO_Directory_SetCurrentDirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.Move(")))
			{
				Process_System_IO_Directory_Move (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.Directory.Delete(")))
			{
				Process_System_IO_Directory_Delete (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.CreateSubdirectory(")))
			{
				Process_System_IO_DirectoryInfo_CreateSubdirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.Create(")))
			{
				Process_System_IO_DirectoryInfo_Create (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.GetFiles(")))
			{
				Process_System_IO_DirectoryInfo_GetFiles (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.GetDirectories(")))
			{
				Process_System_IO_DirectoryInfo_GetDirectories (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.GetFileSystemInfos(")))
			{
				Process_System_IO_DirectoryInfo_GetFileSystemInfos (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.MoveTo(")))
			{
				Process_System_IO_DirectoryInfo_MoveTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.DirectoryInfo.Delete(")))
			{
				Process_System_IO_DirectoryInfo_Delete (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.OpenText(")))
			{
				Process_System_IO_File_OpenText (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.CreateText(")))
			{
				Process_System_IO_File_CreateText (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.AppendText(")))
			{
				Process_System_IO_File_AppendText (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.Create(")))
			{
				Process_System_IO_File_Create (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.Delete(")))
			{
				Process_System_IO_File_Delete (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.Exists(")))
			{
				Process_System_IO_File_Exists (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.Open(")))
			{
				Process_System_IO_File_Open (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.SetCreationTime(")))
			{
				Process_System_IO_File_SetCreationTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.GetCreationTime(")))
			{
				Process_System_IO_File_GetCreationTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.SetLastAccessTime(")))
			{
				Process_System_IO_File_SetLastAccessTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.GetLastAccessTime(")))
			{
				Process_System_IO_File_GetLastAccessTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.SetLastWriteTime(")))
			{
				Process_System_IO_File_SetLastWriteTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.GetLastWriteTime(")))
			{
				Process_System_IO_File_GetLastWriteTime (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.OpenRead(")))
			{
				Process_System_IO_File_OpenRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.OpenWrite(")))
			{
				Process_System_IO_File_OpenWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.File.Move(")))
			{
				Process_System_IO_File_Move (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.Delete(")))
			{
				Process_System_IO_FileInfo_Delete (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.OpenText(")))
			{
				Process_System_IO_FileInfo_OpenText (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.CreateText(")))
			{
				Process_System_IO_FileInfo_CreateText (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.AppendText(")))
			{
				Process_System_IO_FileInfo_AppendText (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.Create(")))
			{
				Process_System_IO_FileInfo_Create (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.Open(")))
			{
				Process_System_IO_FileInfo_Open (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.OpenWrite(")))
			{
				Process_System_IO_FileInfo_OpenWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.MoveTo(")))
			{
				Process_System_IO_FileInfo_MoveTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileInfo.ctor(")))
			{
				Process_System_IO_FileInfo_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Unlock(")))
			{
				Process_System_IO_FileStream_Unlock (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Lock(")))
			{
				Process_System_IO_FileStream_Lock (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.WriteByte(")))
			{
				Process_System_IO_FileStream_WriteByte (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Write(")))
			{
				Process_System_IO_FileStream_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.ReadByte(")))
			{
				Process_System_IO_FileStream_ReadByte (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Read(")))
			{
				Process_System_IO_FileStream_Read (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.SetLength(")))
			{
				Process_System_IO_FileStream_SetLength (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Seek(")))
			{
				Process_System_IO_FileStream_Seek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.EndWrite(")))
			{
				Process_System_IO_FileStream_EndWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.BeginWrite(")))
			{
				Process_System_IO_FileStream_BeginWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.EndRead(")))
			{
				Process_System_IO_FileStream_EndRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.BeginRead(")))
			{
				Process_System_IO_FileStream_BeginRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Flush(")))
			{
				Process_System_IO_FileStream_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.Close(")))
			{
				Process_System_IO_FileStream_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileStream.ctor(")))
			{
				Process_System_IO_FileStream_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextReader.ReadLine(")))
			{
				Process_System_IO_TextReader_ReadLine (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextReader.ReadBlock(")))
			{
				Process_System_IO_TextReader_ReadBlock (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextReader.ReadToEnd(")))
			{
				Process_System_IO_TextReader_ReadToEnd (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextReader.Read(")))
			{
				Process_System_IO_TextReader_Read (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextReader.Close(")))
			{
				Process_System_IO_TextReader_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamReader.ReadLine(")))
			{
				Process_System_IO_StreamReader_ReadLine (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamReader.ReadToEnd(")))
			{
				Process_System_IO_StreamReader_ReadToEnd (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamReader.Read(")))
			{
				Process_System_IO_StreamReader_Read (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamReader.Close(")))
			{
				Process_System_IO_StreamReader_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamReader.ctor(")))
			{
				Process_System_IO_StreamReader_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextWriter.WriteLine(")))
			{
				Process_System_IO_TextWriter_WriteLine (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextWriter.Write(")))
			{
				Process_System_IO_TextWriter_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextWriter.Flush(")))
			{
				Process_System_IO_TextWriter_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.TextWriter.Close(")))
			{
				Process_System_IO_TextWriter_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamWriter.Write(")))
			{
				Process_System_IO_StreamWriter_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamWriter.Flush(")))
			{
				Process_System_IO_StreamWriter_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamWriter.Close(")))
			{
				Process_System_IO_StreamWriter_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.StreamWriter.ctor(")))
			{
				Process_System_IO_StreamWriter_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.Dispose(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_Dispose (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.Remove(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_Remove (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForDomain(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetUserStoreForDomain (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForAssembly(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetUserStoreForAssembly (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.GetStore(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetStore (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.DeleteFile(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_DeleteFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.CreateDirectory(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_CreateDirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.DeleteDirectory(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_DeleteDirectory (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.GetFileNames(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetFileNames (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.GetDirectoryNames(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_GetDirectoryNames (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFile.Close(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFile_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Unlock(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Unlock (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Lock(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Lock (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.WriteByte(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_WriteByte (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Write(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Write (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.ReadByte(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_ReadByte (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Read(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Read (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.SetLength(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_SetLength (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Seek(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Seek (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.EndWrite(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_EndWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.BeginWrite(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_BeginWrite (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.EndRead(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_EndRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.BeginRead(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_BeginRead (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Flush(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.Close(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.IsolatedStorage.IsolatedStorageFileStream.ctor(")))
			{
				Process_System_IO_IsolatedStorage_IsolatedStorageFileStream_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Net.WebClient.DownloadFile(")))
			{
				Process_System_Net_WebClient_DownloadFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Net.WebClient.UploadFile(")))
			{
				Process_System_Net_WebClient_UploadFile (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.IO.FileSystemWatcher.ctor(")))
			{
				Process_System_IO_FileSystemWatcher_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Diagnostics.Process.Start(")))
			{
				Process_System_Diagnostics_Process_Start (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Diagnostics.ProcessStartInfo.ctor(")))
			{
				Process_System_Diagnostics_ProcessStartInfo_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Configuration.AppSettingsReader.GetValue(")))
			{
				Process_System_Configuration_AppSettingsReader_GetValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Configuration.AppSettingsReader.ctor(")))
			{
				Process_System_Configuration_AppSettingsReader_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocument.Save(")))
			{
				Process_System_Xml_XmlDocument_Save (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocument.LoadXml(")))
			{
				Process_System_Xml_XmlDocument_LoadXml (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocument.WriteContentTo(")))
			{
				Process_System_Xml_XmlDocument_WriteContentTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocument.WriteTo(")))
			{
				Process_System_Xml_XmlDocument_WriteTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocument.ctor(")))
			{
				Process_System_Xml_XmlDocument_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocumentFragment.WriteContentTo(")))
			{
				Process_System_Xml_XmlDocumentFragment_WriteContentTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocumentFragment.WriteTo(")))
			{
				Process_System_Xml_XmlDocumentFragment_WriteTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocumentType.WriteContentTo(")))
			{
				Process_System_Xml_XmlDocumentType_WriteContentTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlDocumentType.WriteTo(")))
			{
				Process_System_Xml_XmlDocumentType_WriteTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlWriter.WriteNode(")))
			{
				Process_System_Xml_XmlWriter_WriteNode (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlWriter.WriteAttributes(")))
			{
				Process_System_Xml_XmlWriter_WriteAttributes (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlWriter.WriteStartElement(")))
			{
				Process_System_Xml_XmlWriter_WriteStartElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlWriter.WriteAttributeString(")))
			{
				Process_System_Xml_XmlWriter_WriteAttributeString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlWriter.WriteStartAttribute(")))
			{
				Process_System_Xml_XmlWriter_WriteStartAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlWriter.WriteElementString(")))
			{
				Process_System_Xml_XmlWriter_WriteElementString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteNode(")))
			{
				Process_System_Xml_XmlTextWriter_WriteNode (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteAttributes(")))
			{
				Process_System_Xml_XmlTextWriter_WriteAttributes (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteQualifiedName(")))
			{
				Process_System_Xml_XmlTextWriter_WriteQualifiedName (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteName(")))
			{
				Process_System_Xml_XmlTextWriter_WriteName (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteNmToken(")))
			{
				Process_System_Xml_XmlTextWriter_WriteNmToken (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.Flush(")))
			{
				Process_System_Xml_XmlTextWriter_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.Close(")))
			{
				Process_System_Xml_XmlTextWriter_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteBinHex(")))
			{
				Process_System_Xml_XmlTextWriter_WriteBinHex (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteBase64(")))
			{
				Process_System_Xml_XmlTextWriter_WriteBase64 (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteRaw(")))
			{
				Process_System_Xml_XmlTextWriter_WriteRaw (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteChars(")))
			{
				Process_System_Xml_XmlTextWriter_WriteChars (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteSurrogateCharEntity(")))
			{
				Process_System_Xml_XmlTextWriter_WriteSurrogateCharEntity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteString(")))
			{
				Process_System_Xml_XmlTextWriter_WriteString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteWhitespace(")))
			{
				Process_System_Xml_XmlTextWriter_WriteWhitespace (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteCharEntity(")))
			{
				Process_System_Xml_XmlTextWriter_WriteCharEntity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteEntityRef(")))
			{
				Process_System_Xml_XmlTextWriter_WriteEntityRef (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteProcessingInstruction(")))
			{
				Process_System_Xml_XmlTextWriter_WriteProcessingInstruction (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteComment(")))
			{
				Process_System_Xml_XmlTextWriter_WriteComment (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteCData(")))
			{
				Process_System_Xml_XmlTextWriter_WriteCData (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteEndAttribute(")))
			{
				Process_System_Xml_XmlTextWriter_WriteEndAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteStartAttribute(")))
			{
				Process_System_Xml_XmlTextWriter_WriteStartAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteFullEndElement(")))
			{
				Process_System_Xml_XmlTextWriter_WriteFullEndElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteEndElement(")))
			{
				Process_System_Xml_XmlTextWriter_WriteEndElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteStartElement(")))
			{
				Process_System_Xml_XmlTextWriter_WriteStartElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteDocType(")))
			{
				Process_System_Xml_XmlTextWriter_WriteDocType (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteEndDocument(")))
			{
				Process_System_Xml_XmlTextWriter_WriteEndDocument (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteStartDocument(")))
			{
				Process_System_Xml_XmlTextWriter_WriteStartDocument (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteAttributeString(")))
			{
				Process_System_Xml_XmlTextWriter_WriteAttributeString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.WriteElementString(")))
			{
				Process_System_Xml_XmlTextWriter_WriteElementString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextWriter.ctor(")))
			{
				Process_System_Xml_XmlTextWriter_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.IsStartElement(")))
			{
				Process_System_Xml_XmlReader_IsStartElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.ReadEndElement(")))
			{
				Process_System_Xml_XmlReader_ReadEndElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.ReadElementString(")))
			{
				Process_System_Xml_XmlReader_ReadElementString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.ReadStartElement(")))
			{
				Process_System_Xml_XmlReader_ReadStartElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.MoveToContent(")))
			{
				Process_System_Xml_XmlReader_MoveToContent (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.Skip(")))
			{
				Process_System_Xml_XmlReader_Skip (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.IsName(")))
			{
				Process_System_Xml_XmlReader_IsName (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlReader.IsNameToken(")))
			{
				Process_System_Xml_XmlReader_IsNameToken (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadAttributeValue(")))
			{
				Process_System_Xml_XmlTextReader_ReadAttributeValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ResolveEntity(")))
			{
				Process_System_Xml_XmlTextReader_ResolveEntity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.LookupNamespace(")))
			{
				Process_System_Xml_XmlTextReader_LookupNamespace (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadOuterXml(")))
			{
				Process_System_Xml_XmlTextReader_ReadOuterXml (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadInnerXml(")))
			{
				Process_System_Xml_XmlTextReader_ReadInnerXml (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.IsStartElement(")))
			{
				Process_System_Xml_XmlTextReader_IsStartElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadEndElement(")))
			{
				Process_System_Xml_XmlTextReader_ReadEndElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadElementString(")))
			{
				Process_System_Xml_XmlTextReader_ReadElementString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadStartElement(")))
			{
				Process_System_Xml_XmlTextReader_ReadStartElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.MoveToContent(")))
			{
				Process_System_Xml_XmlTextReader_MoveToContent (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadString(")))
			{
				Process_System_Xml_XmlTextReader_ReadString (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.Skip(")))
			{
				Process_System_Xml_XmlTextReader_Skip (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.Close(")))
			{
				Process_System_Xml_XmlTextReader_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.Read(")))
			{
				Process_System_Xml_XmlTextReader_Read (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.MoveToElement(")))
			{
				Process_System_Xml_XmlTextReader_MoveToElement (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.MoveToNextAttribute(")))
			{
				Process_System_Xml_XmlTextReader_MoveToNextAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.MoveToFirstAttribute(")))
			{
				Process_System_Xml_XmlTextReader_MoveToFirstAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.MoveToAttribute(")))
			{
				Process_System_Xml_XmlTextReader_MoveToAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.GetAttribute(")))
			{
				Process_System_Xml_XmlTextReader_GetAttribute (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.GetRemainder(")))
			{
				Process_System_Xml_XmlTextReader_GetRemainder (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadChars(")))
			{
				Process_System_Xml_XmlTextReader_ReadChars (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadBase64(")))
			{
				Process_System_Xml_XmlTextReader_ReadBase64 (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ReadBinHex(")))
			{
				Process_System_Xml_XmlTextReader_ReadBinHex (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlTextReader.ctor(")))
			{
				Process_System_Xml_XmlTextReader_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlEntityReference.WriteContentTo(")))
			{
				Process_System_Xml_XmlEntityReference_WriteContentTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlEntityReference.WriteTo(")))
			{
				Process_System_Xml_XmlEntityReference_WriteTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlImplementation.CreateDocument(")))
			{
				Process_System_Xml_XmlImplementation_CreateDocument (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlImplementation.ctor(")))
			{
				Process_System_Xml_XmlImplementation_ctor (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlText.WriteContentTo(")))
			{
				Process_System_Xml_XmlText_WriteContentTo (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("System.Xml.XmlText.WriteTo(")))
			{
				Process_System_Xml_XmlText_WriteTo (logEntry);
			}
		}
	}
}

/*
Possible bugs:
LoadLibrary
RegSaveKey
RegSaveKeyEx
RegLoadKey
RegRestoreKey
RegReplaceKey
SearchPath
CopyLZFile
LZCopy
CreateFileMoniker
CreateFilePKCS10
GetFullPathName 
SaferiIsExecutableFileType
*/