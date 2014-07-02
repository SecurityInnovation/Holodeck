using System;
using System.Collections;
using TestFramework.Log;

namespace ResourcesPaneTest
{
	/// <summary>
	/// Summary description for RegistryHelper.
	/// </summary>
	public class RegistryHelper : Helper
	{
		protected Hashtable predefTable = new Hashtable (); // contains regHandle-resourceName pairs

		public RegistryHelper ()
		{
			Reset ();
		}

		public override void Reset ()
		{
			base.Reset ();
			predefTable.Clear ();
			SetPredefValues (predefTable);
			SetPredefValues (handleTable);
		}

		protected void SetPredefValues (Hashtable table)
		{
			table.Add (((Int64) 0x80000000).ToString (), "HKEY_CLASSES_ROOT");
			table.Add (((Int64) 0x80000001).ToString (), "HKEY_CURRENT_USER");
			table.Add (((Int64) 0x80000002).ToString (), "HKEY_LOCAL_MACHINE");
			table.Add (((Int64) 0x80000003).ToString (), "HKEY_USERS");
			table.Add (((Int64) 0x80000004).ToString (), "HKEY_PERFORMANCE_DATA");
			table.Add (((Int64) 0x80000005).ToString (), "HKEY_CURRENT_CONFIG");
			table.Add (((Int64) 0x80000006).ToString (), "HKEY_DYN_DATA");
			table.Add (((Int64) 0x80000050).ToString (), "HKEY_PERFORMANCE_TEXT");
			table.Add (((Int64) 0x80000060).ToString (), "HKEY_PERFORMANCE_NLSTEXT");
		}

		//------------Native Functions---------------//

		protected void Process_RegCloseKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			RemoveFromHandleTable (handleEntry, regEntry);
		}

		protected void Process_RegConnectRegistry (string[] logEntry)
		{
			string regEntry = "";
			if (logEntry[(int) LogColumns.Parameter1] != "0")
			{
				regEntry = logEntry[(int) LogColumns.Parameter1] + "\\";
			} 
			regEntry += ((string) handleTable[logEntry[(int) LogColumns.Parameter2]]);

			IncreaseEntryTableCount (regEntry, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter3] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.Parameter3], regEntry);
			}
		}

		protected void Process_RegCreateKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter3] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.Parameter3], regEntry);
			}
		}

		protected void Process_RegCreateKeyEx (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter8] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.Parameter8], regEntry);
			}
		}

		protected void Process_RegDeleteKey (string[] logEntry)
		{
			Process_RegCloseKey (logEntry);
		}

		protected void Process_RegDeleteValue (string[] logEntry)
		{
			// TODO: look up MSDN on lpValueName (Parameter2) if it's NULL, or points to an empty string
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegEnumKey (string[] logEntry)
		{
			// TODO: have to look into this further
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter3];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegEnumKeyEx (string[] logEntry)
		{
			// TODO: have to look into this further
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter3];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegEnumValue (string[] logEntry)
		{
			// TODO: have to look into this further
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter3];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegFlushKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegKeySecurity (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegLoadKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);

			// TODO: deal with Parameter3
		}

		protected void Process_RegNotifyChangeKeyValue (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegOpenCurrentUser (string[] logEntry)
		{
			string regEntry = "HKEY_CURRENT_USER";
			IncreaseEntryTableCount (regEntry, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter2] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.Parameter2], regEntry);
			}
		}

		protected void Process_RegOpenKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter3] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.Parameter3], regEntry);
			}
		}

		protected void Process_RegOpenKeyEx (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);

			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter5] != "0"))
			{
				AddToHandleTable (logEntry[(int) LogColumns.Parameter5], regEntry);
			}
		}

		protected void Process_RegOverridePredefKey (string[] logEntry)
		{
			if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
				(logEntry[(int) LogColumns.Parameter2] != "0"))
			{
				string handleEntry = logEntry[(int) LogColumns.Parameter2];
				string regEntryNew = GetHandleTableEntry (handleEntry);
				RemoveFromHandleTable (logEntry[(int) LogColumns.Parameter1], null);
				handleTable.Add (logEntry[(int) LogColumns.Parameter1], regEntryNew);
			} 
			else if ((logEntry[(int) LogColumns.ErrorCode] == "ERROR_SUCCESS") &&
					 (logEntry[(int) LogColumns.Parameter2] == "0")) 
			{
				string handleEntry = logEntry[(int) LogColumns.Parameter2];
				string defaultRegEntry = (string) predefTable[handleEntry];
				RemoveFromHandleTable (logEntry[(int) LogColumns.Parameter1], null);
				handleTable.Add (logEntry[(int) LogColumns.Parameter1], defaultRegEntry);
			}
		}

		protected void Process_RegQueryInfoKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegQueryMultipleValues (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			// TODO: figure it out (see MSDN)
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegQueryValue (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegQueryValueEx (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegReplaceKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);

			// TODO: deal with parameter3
			// TODO: deal with parameter4
		}

		protected void Process_RegRestoreKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);

			// TODO: deal with parameter2
		}

		protected void Process_RegSaveKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);

			// TODO: deal with parameter2
		}

		protected void Process_RegSaveKeyEx (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);

			// TODO: deal with parameter2
		}

		protected void Process_RegSetKeySecurity (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry);
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegSetValue (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegSetValueEx (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		protected void Process_RegUnloadKey (string[] logEntry)
		{
			string handleEntry = logEntry[(int) LogColumns.Parameter1];
			string regEntry = GetHandleTableEntry (handleEntry) + "\\" + logEntry[(int) LogColumns.Parameter2];
			IncreaseEntryTableCount (regEntry, logEntry);
		}

		//------------.NET Functions---------------//

		protected void Process_Microsoft_Win32_RegistryKey_Close (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_CreateSubKey (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_DeleteSubKey (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_DeleteSubKeyTree (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_DeleteValue (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_Flush (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_GetSubKeyNames (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				IncreaseEntryTableCount (regEntry, logEntry);
				// TODO: check if ve can use the return value
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_GetValue (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_GetValueNames (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				IncreaseEntryTableCount (regEntry, logEntry);
				// TODO: check if ve can use the return value
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_OpenRemoteBaseKey (string[] logEntry)
		{
			string regEntry = "";
			if (logEntry[(int) LogColumns.Parameter2] != "") 
			{
				regEntry = "\\\\" + logEntry[(int) LogColumns.Parameter2] + "\\";
			}

			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				regEntry += logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_OpenSubKey (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
		}

		protected void Process_Microsoft_Win32_RegistryKey_SetValue (string[] logEntry)
		{
			int regKeyNameEnd = logEntry[(int) LogColumns.Parameter1].IndexOf (" [");
			if (regKeyNameEnd != -1) 
			{
				string regEntry = logEntry[(int) LogColumns.Parameter1].Substring (0, regKeyNameEnd);
				regEntry += "\\" + logEntry[(int) LogColumns.Parameter2];
				IncreaseEntryTableCount (regEntry, logEntry);
			}
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
			if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
				(logEntry[(int) LogColumns.Function] == "RegCloseKey")) 
			{
				Process_RegCloseKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegConnectRegistryA") &&
					  (logEntry[(int) LogColumns.Function] == "RegConnectRegistryW"))) 
			{
				Process_RegConnectRegistry (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegCreateKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegCreateKeyW"))) 
			{
				Process_RegCreateKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegCreateKeyExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegCreateKeyExW"))) 
			{
				Process_RegCreateKeyEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegDeleteKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegdeleteKeyW"))) 
			{
				Process_RegDeleteKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegDeleteValueA") ||
					  (logEntry[(int) LogColumns.Function] == "RegDeleteValueW"))) 
			{
				Process_RegDeleteValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegEnumKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegEnumKeyW"))) 
			{
				Process_RegEnumKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegEnumKeyExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegEnumKeyExW"))) 
			{
				Process_RegEnumKeyEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegEnumValueA") ||
					  (logEntry[(int) LogColumns.Function] == "RegEnumValueW"))) 
			{
				Process_RegEnumValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RegFlushKey")) 
			{
				Process_RegFlushKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RegKeySecurity")) 
			{
				Process_RegKeySecurity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegLoadKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegLoadKeyW"))) 
			{
				Process_RegLoadKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RegNotifyChangeKeyValue")) 
			{
				Process_RegNotifyChangeKeyValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RegOpenCurrentUser")) 
			{
				Process_RegOpenCurrentUser (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegOpenKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegOpenKeyW"))) 
			{
				Process_RegOpenKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegOpenKeyExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegOpenKeyExW"))) 
			{
				Process_RegOpenKeyEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RegOverridePredefKey")) 
			{
				Process_RegOverridePredefKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegQueryInfoKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegQueryInfoKeyW"))) 
			{
				Process_RegQueryInfoKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegQueryMultipleValuesA") ||
					  (logEntry[(int) LogColumns.Function] == "RegQueryMultipleValuesW"))) 
			{
				Process_RegQueryMultipleValues (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegQueryValueA") ||
					  (logEntry[(int) LogColumns.Function] == "RegQueryValueW"))) 
			{
				Process_RegQueryValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegQueryValueExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegQueryValueExW"))) 
			{
				Process_RegQueryValueEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegReplaceKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegReplaceKeyW"))) 
			{
				Process_RegReplaceKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegRestoreKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegRestoreKeyW"))) 
			{
				Process_RegRestoreKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegSaveKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegSaveKeyW"))) 
			{
				Process_RegSaveKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegSaveKeyExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegSaveKeyExW"))) 
			{
				Process_RegSaveKeyEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 (logEntry[(int) LogColumns.Function] == "RegSetKeySecurity"))
			{
				Process_RegSetKeySecurity (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegSetValueA") ||
					  (logEntry[(int) LogColumns.Function] == "RegSetValueW"))) 
			{
				Process_RegSetValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegSetValueExA") ||
					  (logEntry[(int) LogColumns.Function] == "RegSetValueExW"))) 
			{
				Process_RegSetValueEx (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "advapi32.dll") &&
					 ((logEntry[(int) LogColumns.Function] == "RegUnloadKeyA") ||
					  (logEntry[(int) LogColumns.Function] == "RegUnloadKeyW"))) 
			{
				Process_RegUnloadKey (logEntry);
			}
			//---------.NET Functions----------//
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.Close(")))
			{
				Process_Microsoft_Win32_RegistryKey_Close (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.CreateSubKey(")))
			{
				Process_Microsoft_Win32_RegistryKey_CreateSubKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.DeleteSubKey(")))
			{
				Process_Microsoft_Win32_RegistryKey_DeleteSubKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.DeleteSubKeyTree(")))
			{
				Process_Microsoft_Win32_RegistryKey_DeleteSubKeyTree (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.DeleteValue(")))
			{
				Process_Microsoft_Win32_RegistryKey_DeleteValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.Flush(")))
			{
				Process_Microsoft_Win32_RegistryKey_Flush (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.GetSubKeyNames(")))
			{
				Process_Microsoft_Win32_RegistryKey_GetSubKeyNames (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.GetValue(")))
			{
				Process_Microsoft_Win32_RegistryKey_GetValue (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.GetValueNames(")))
			{
				Process_Microsoft_Win32_RegistryKey_GetValueNames (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(")))
			{
				Process_Microsoft_Win32_RegistryKey_OpenRemoteBaseKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.OpenSubKey(")))
			{
				Process_Microsoft_Win32_RegistryKey_OpenSubKey (logEntry);
			}
			else if ((logEntry[(int) LogColumns.Dll] == "mscorlib.dll") &&
					 (logEntry[(int) LogColumns.Function].StartsWith ("Microsoft.Win32.RegistryKey.SetValue(")))
			{
				Process_Microsoft_Win32_RegistryKey_SetValue (logEntry);
			}
		}
	}
}

/*
Notes: 
HKEY_CLASSES_ROOT and HKEY_CURRENT_USER handle values represent subsets of the HKEY_LOCAL_MACHINE and HKEY_USERS handle values, respectively. 

*/
