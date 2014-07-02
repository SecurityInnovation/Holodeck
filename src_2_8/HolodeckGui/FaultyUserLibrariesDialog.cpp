#include "FaultyUserLibrariesDialog.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		RemoveLibrary
	// Description: Removes a library and xml file from holodeck's intercept list
	//
	// Parameters:
	//	dllFileName - the name of the dll to remove
	//	xmlFileName - the name of the xml file to remove
	//
	// Return Value: None
	//*************************************************************************
	void FaultyUserLibrariesDialog::RemoveLibrary(String *dllFileName, String *xmlFileName)
	{
		try
		{
			if (File::Exists (dllFileName))
			{
				if((File::GetAttributes(dllFileName) & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
					File::SetAttributes(dllFileName, static_cast <FileAttributes>(File::GetAttributes(dllFileName) & ~FileAttributes::ReadOnly));
				File::Delete (dllFileName);
			}

			if (File::Exists (xmlFileName))
			{
				if((File::GetAttributes(xmlFileName) & FileAttributes::ReadOnly) == FileAttributes::ReadOnly)
					File::SetAttributes(xmlFileName, static_cast <FileAttributes>(File::GetAttributes(xmlFileName) & ~FileAttributes::ReadOnly));
				File::Delete (xmlFileName);
			}

			try
			{
				String * path = String::Concat (ROOT_PATH, "\\", DOTNET_REPLACEMENT_DLLS_KEY);
				Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(path, true);
				if (key)
					key->DeleteValue (Path::GetFileName(xmlFileName), false);
			}
			catch(...)
			{
			}
		}
		catch (...)
		{
			MessageBox (NULL, "Could not delete some of the files. Please delete them manually", "Holodeck", 0);
		}
	}
}