using System;
using System.Collections;
using System.Text;
using TestFramework.Log;


namespace ResourcesPaneTest
{
	/// <summary>
	/// Summary description for Helper.
	/// </summary>
	public class Helper
	{
		protected Hashtable thisTable = new Hashtable (); // contains thisPointer-resourceName pairs
		protected Hashtable handleTable = new Hashtable (); // contains handle-resourceName pairs
		protected Hashtable entryTable = new Hashtable (); // contains resourceName-resource pairs

		protected Hashtable heuristicsBeforeOpened = new Hashtable (); // contains magicHandle-resource pairs
		protected Hashtable heuristicsAfterClosed = new Hashtable (); // contains handle-resourceName pairs

		protected string lastTimeStamp = "NoTime";

		const string magickEntry1 = "ResourcePaneTest[";
		const string magickEntry2 = "]";

		protected enum LogColumns {
			TimeStamp = 0,
			Thread = 1,
			Category = 2,
			Dll = 3,
			Function = 4,
			ReturnValue = 5,
			ErrorCode = 6,
			Exception = 7,
			Parameter1 = 8,
			Parameter2 = 9,
			Parameter3 = 10,
			Parameter4 = 11,
			Parameter5 = 12,
			Parameter6 = 13,
			Parameter7 = 14,
			Parameter8 = 15,
			Parameter9 = 16,
			Parameter10 = 17,
			Parameter11 = 18,
			Parameter12 = 19,
		}

		public Helper ()
		{
			Reset ();
		}

		public virtual void Reset ()
		{
			thisTable.Clear ();
			handleTable.Clear ();
			entryTable.Clear ();

			heuristicsBeforeOpened.Clear ();
			heuristicsAfterClosed.Clear ();
		}

		protected Hashtable GetEntryTable ()
		{
			return entryTable;
		}

		//---------Heuristics begin----------//
		protected string ReportErrors ()
		{
			string errorMessages = "";
			IEnumerator enumerator = heuristicsBeforeOpened.Keys.GetEnumerator ();
			enumerator.Reset ();
			while (enumerator.MoveNext ()) {
				string resourceName =(string) enumerator.Current;
				errorMessages += "Log mentiones a resource with unknown handle [" + resourceName + "]\n";
			}

			Log.printError (errorMessages);

			return errorMessages;
		}
		//---------Heuristics end----------//

		//---------Heuristics begin----------//
		protected void HandleAccessedBeforeOpen (string key, string resourceName)
		{
			IEnumerator enumerator = heuristicsBeforeOpened.Keys.GetEnumerator ();
			enumerator.Reset ();
			while (enumerator.MoveNext ()) {
				string enumKey = (string) enumerator.Current;
				int enumKeyIndex = enumKey.IndexOf (magickEntry2);
				if (enumKeyIndex != -1) {
					string enumUnknownKey = enumKey.Substring (0, enumKeyIndex);
					if (key == enumUnknownKey) {
						string newResourceName = key + enumKey.Substring (enumKeyIndex + 1);
						Holodeck.Resource resource = (Holodeck.Resource) heuristicsBeforeOpened[enumKey];

						string[] logEntry = new string[20];
						for (int i = 0; i < 20; i++) {
							logEntry[i] = "";
						}
						logEntry[(int) LogColumns.ErrorCode] = resource.ErrorCode;
						logEntry[(int) LogColumns.Function] = resource.LastFunction;
						// resource.processID; // TODO: figure out the process ID
						logEntry[(int) LogColumns.ReturnValue] = resource.ReturnValue;
						logEntry[(int) LogColumns.Thread] = resource.threadID.ToString ();
						logEntry[(int) LogColumns.TimeStamp] = resource.TimeStamp;

						IncreaseEntryTableCount (resourceName, logEntry, resource.NumHits);
					}
				} else {
					// we shouldn't be here
				}
			}
		}
		//---------Heuristics end----------//

		//---------Heuristics begin----------//
		protected string HandleAccessedAfterClosed (string key)
		{
			string resourceName = (string) heuristicsAfterClosed[key];

			return resourceName;
		}
		//---------Heuristics end----------//

		protected void ChangeHashtable (Hashtable table, string resourceName, string[] logEntry, long numHits)
		{
			Holodeck.Resource resource;

			resource.ErrorCode = logEntry[(int) LogColumns.ErrorCode];
			resource.LastFunction = logEntry[(int) LogColumns.Function];
			resource.Name = resourceName;
			resource.NumHits = numHits;
			resource.processID = 0; // TODO: figure out the process ID
			resource.ReturnValue = logEntry[(int) LogColumns.ReturnValue];
			resource.threadID = Int64.Parse (logEntry[(int) LogColumns.Thread]);
			resource.TimeStamp = logEntry[(int) LogColumns.TimeStamp];

			if (table.Contains (resourceName)) {
				table.Remove (resourceName);
			}

			if (resourceName != magickEntry1 + "0" + magickEntry2) {
				table.Add (resourceName, resource);
			}
		}

		protected void IncreaseEntryTableCount (string resourceName, string[] logEntry)
		{
			IncreaseEntryTableCount (resourceName, logEntry, 1);
		}

		protected void IncreaseEntryTableCount (string resourceName, string[] logEntry, long incValue)
		{
			if (resourceName != null) {
				resourceName = resourceName.TrimEnd ('\\');

				if (!resourceName.StartsWith (magickEntry1)) { // if this is a known entry
					if (entryTable.Contains (resourceName)) {
						Holodeck.Resource resource = (Holodeck.Resource) entryTable[resourceName];
						entryTable.Remove (resourceName);
						ChangeHashtable (entryTable, resourceName, logEntry, resource.NumHits + incValue);
					} else {
						ChangeHashtable (entryTable, resourceName, logEntry, incValue);
					}
				} else { // this entry is unknown - probably an access before open
					//---------Heuristics begin----------//
					string tempResourceName = resourceName.Substring (magickEntry1.Length);
					if (heuristicsBeforeOpened.Contains (tempResourceName)) {
						Holodeck.Resource resource = (Holodeck.Resource) heuristicsBeforeOpened[tempResourceName];
						heuristicsBeforeOpened.Remove (resourceName);
						ChangeHashtable (heuristicsBeforeOpened, resourceName, logEntry, resource.NumHits + incValue);
					} else {
						ChangeHashtable (heuristicsBeforeOpened, resourceName, logEntry, incValue);
					}
					//---------Heuristics end----------//
				}
			} else {
				Log.printError ("ResourcesPaneTest.FileHelper.IncreaseEntryTableCount Error - function \"" + logEntry[(int) LogColumns.Function] + "\" is accessing unknown file entry at [" + logEntry[(int) LogColumns.TimeStamp] + "]");
			}
		}

		protected string GetHandleTableEntry (string key)
		{
			string resourceName = (string) handleTable[key];

			//---------Heuristics begin----------//
			if (resourceName == null) {
				resourceName = HandleAccessedAfterClosed (key);
			}

			if (resourceName == null) {
				resourceName = magickEntry1 + key + magickEntry2; // for handling as resource access before open
			}
			//---------Heuristics end----------//

			return resourceName;
		}

		protected void AddToHandleTable (string key, string resourceName)
		{
			if (key == null) {
				Log.printError ("ResourcesPaneTest.RegistryHelper.AddToHandleTable Error - trying to add null reference as key for registry entry [" + resourceName + "]");
				return;
			} else if (key == "0") {
				// the open function returned a null-pointer -> open failed
				return;
			}

			//---------Heuristics begin----------//
			HandleAccessedBeforeOpen (key, resourceName);
			//---------Heuristics end----------//

			try {
				resourceName = resourceName.TrimEnd ('\\');

				if (handleTable.Contains (key)) {
					// this shouldn't have happened
					Log.printError ("We got a handle to a file that was already open...");
					RemoveFromHandleTable (key, null);
				} 

				handleTable.Add (key, resourceName);
			} catch (ArgumentNullException) {
				Log.printError ("ResourcesPaneTest.FileHelper.AddToHandleTable Error - trying to add null reference as key for the file [" + resourceName + "]");
			}
		}

		protected void RemoveFromHandleTable (string key, string resourceName)
		{
			//---------Heuristics begin----------//
			try {
				if (heuristicsAfterClosed.Contains (key)) {
					HandleAccessedAfterClosed (key);
					heuristicsAfterClosed.Remove (key);
				}
				if (resourceName != null) {
					heuristicsAfterClosed.Add (key, resourceName);
				}
			} catch (Exception) {
			}
			//---------Heuristics end----------//

			handleTable.Remove (key);
		}

		/// <summary>
		/// Creates a hash table of resources based on their names
		/// </summary>
		/// <param name="resources">arrray of resources</param>
		/// <returns>the created hash table</returns>
		protected static Hashtable CreateHashtable (Holodeck.Resource[] resources)
		{
			Hashtable retTable = new Hashtable ();
			for (int i = 0; i < resources.Length; i++) 
			{
				try {
					retTable.Add (resources[i].Name, resources[i]);
				} catch (ArgumentNullException e) {
					Log.printError ("ResourcesPaneTest.Helper.CreateHashtable Error while processing resource No." + i + " - " + e.Message);
				} catch (ArgumentException e) {
					Log.printError ("ResourcesPaneTest.Helper.CreateHashtable Error while processing resource No." + i + " - " + e.Message);
					retTable.Remove (resources[i].Name);
					retTable.Add (resources[i].Name, resources[i]);
				}
			}

			return retTable;
		}

		/// <summary>
		/// Comapres two hash table containing resources
		/// </summary>
		/// <param name="logTable">a hash table; usually the one created from the logs</param>
		/// <param name="resourceTable">another hash table; usually the one gathered from one of the subpanes of the "Resources" pane</param>
		/// <returns>an ArrayList containing strings of error messages produced during the comparison</returns>
		protected static System.Collections.ArrayList CompareResourceTables (Hashtable logTable, Hashtable resourceTable, bool allInclusive)
		{
			System.Collections.ArrayList reportArray = new System.Collections.ArrayList (10);
			IEnumerator logTableEnumerator = logTable.Keys.GetEnumerator ();
			logTableEnumerator.Reset ();
			while (logTableEnumerator.MoveNext ())
			{
				string resourceName = (string) logTableEnumerator.Current;
				Holodeck.Resource logResource = (Holodeck.Resource) logTable[resourceName];
				// TODO: make the comparison case-insensitive
				if (resourceTable.Contains (resourceName))
				{
					Holodeck.Resource resource = (Holodeck.Resource) resourceTable[resourceName];
					if ((logResource.NumHits != 0) &&
						(logResource.NumHits != resource.NumHits)) 
					{
						if (allInclusive ||
							(!allInclusive && logResource.NumHits > resource.NumHits)) {
							reportArray.Add ("Mismatching hit numbers for [" + resourceName + "] - reported: " + resource.NumHits + ", calculated: " + logResource.NumHits);
						}
					}

					if (allInclusive &&
						(logResource.ErrorCode != resource.ErrorCode))
					{
						reportArray.Add ("Mismatching error codes for [" + resourceName + "] - reported: " + resource.ErrorCode + ", calculated: " + logResource.ErrorCode);
					}

					if (logResource.LastFunction != resource.LastFunction) {
						reportArray.Add ("Mismatching last functions for [" + resourceName + "] - reported: " + resource.LastFunction + ", calculated: " + logResource.LastFunction);
					}

					// TODO: compare process ID

					if (allInclusive &&
						(logResource.ReturnValue != resource.ReturnValue)) {
						reportArray.Add ("Mismatching return values for [" + resourceName + "] - reported: " + resource.ReturnValue + ", calculated: " + logResource.ReturnValue);
					}

					if (allInclusive &&
						(logResource.threadID != resource.threadID)) {
						reportArray.Add ("Mismatching thread IDs for [" + resourceName + "] - reported: " + resource.threadID + ", calculated: " + logResource.threadID);
					}

					if (allInclusive &&
						(logResource.TimeStamp != resource.TimeStamp)) {
						reportArray.Add ("Mismatching time stamps for [" + resourceName + "] - reported: " + resource.TimeStamp + ", calculated: " + logResource.TimeStamp);
					}

					resourceTable.Remove (logTableEnumerator.Current);
				} 
				else if (logResource.NumHits != 0) 
				{
					reportArray.Add ("Missing resource in \"Resources\" pane [" + resourceName + "], calculated hit number: " + logResource.NumHits);
				}
			}

			if (resourceTable.Count != 0) 
			{
				IEnumerator resourceTableEnumerator = resourceTable.Keys.GetEnumerator ();
				resourceTableEnumerator.Reset ();
				while (resourceTableEnumerator.MoveNext ())
				{
					string resourceName = (string) resourceTableEnumerator.Current;
					Holodeck.Resource resource = (Holodeck.Resource) resourceTable[resourceName];
					if (allInclusive) {
						reportArray.Add ("Unlogged resource appears in \"Resources\" pane [" + resourceName + "], reported hit number:" + resource.NumHits);
					} else if (resourceName.StartsWith ("Name: ") ||
							   resourceName.StartsWith ("Func: ") ||
							   resourceName.StartsWith ("Proc: ") ||
							   resourceName.StartsWith ("Time: ") ||
							   resourceName.StartsWith ("Retv: ") ||
							   resourceName.StartsWith ("Errc: ") ||
							   resourceName.StartsWith ("Exce: ")) {
						reportArray.Add ("Unknown resource appears in \"Resources\" pane [" + resourceName + "], reported hit number:" + resource.NumHits);
					}
				}
			}

			return reportArray;
		}

		/// <summary>
		/// Process  the specified log entries and create a hash table of the resources touched
		/// </summary>
		/// <param name="log">a log entry</param>
		/// <returns>a two element array of hashtables; [0] contains the file resources, [1] contains the registry resources</returns>
		protected static Hashtable[] ProcessLog (String[][] log)
		{
			Hashtable[] retTable = new Hashtable[2];
			RegistryHelper registryHelper = new RegistryHelper ();
			FileHelper fileHelper = new FileHelper ();

			for (int i = 0; i < log.Length; i++) 
			{
				// TODO: handle exceptions
				registryHelper.AddEntry (log[i]);
				fileHelper.AddEntry (log[i]);
			}

			retTable[0] = fileHelper.GetEntryTable ();
			retTable[1] = registryHelper.GetEntryTable ();
			return retTable;
		}

		/// <summary>
		/// Compares the given resources with the given log entries
		/// -- Might throw a System.ArgumentNullException
		/// -- Might throw a System.ArgumentException
		/// </summary>
		/// <param name="resources">array of resources; [0] contains the file resources, [1] contains the registry resources</param>
		/// <param name="log">a log entry; the first dimension is the row, the second is the column</param>
		/// <returns>a string[] containing strings of error messages produced during the comparison</returns>
		public static string[] CompareResourcesWithLogs (Holodeck.Resource[][] resources, String[][] log)
		{
			Hashtable fileResources = CreateHashtable (resources[0]);
			Hashtable registryResources = CreateHashtable (resources[1]);
			Hashtable[] logResources = ProcessLog (log);

			System.Collections.ArrayList[] diff = new System.Collections.ArrayList[2];
			diff[0] = CompareResourceTables (logResources[0], fileResources, true);
			diff[1] = CompareResourceTables (logResources[1], registryResources, true);

			string[] differences = new string[diff[0].Count + diff[1].Count];
			int i = 0;
			for (int round = 0; round < 2; round++)
			{
				for (int j = 0; j < diff[round].Count; j++, i++) 
				{
					differences[i] = (string) diff[round][j];
				}
			}

			return differences;
		}

		/// <summary>
		/// Compares the given two sets of resources
		/// -- Might throw a System.ArgumentNullException
		/// -- Might throw a System.ArgumentException
		/// </summary>
		/// <param name="resources">array of resources; [0] contains the file resources, [1] contains the registry resources</param>
		/// <param name="resources2">array of resources; [0] contains the file resources, [1] contains the registry resources</param>
		/// <returns>a string[] containing strings of error messages produced during the comparison</returns>
		public static string[] CompareResourcesWithResources (Holodeck.Resource[][] resources, Holodeck.Resource[][] resources2) {
			Hashtable fileResources = CreateHashtable (resources[0]);
			Hashtable registryResources = CreateHashtable (resources[1]);
			Hashtable knownFileResources = CreateHashtable (resources2[0]);
			Hashtable knownRegistryResources = CreateHashtable (resources2[1]);

			System.Collections.ArrayList[] diff = new System.Collections.ArrayList[2];
			diff[0] = CompareResourceTables (knownFileResources, fileResources, false);
			diff[1] = CompareResourceTables (knownRegistryResources, registryResources, false);

			string[] differences = new string[diff[0].Count + diff[1].Count];
			int i = 0;
			for (int round = 0; round < 2; round++) {
				for (int j = 0; j < diff[round].Count; j++, i++) {
					differences[i] = (string) diff[round][j];
				}
			}

			return differences;
		}
			
		/// <summary>
		/// Compares the given resources with the given log entries
		/// -- Might throw a System.ArgumentNullException
		/// -- Might throw a System.ArgumentException
		/// </summary>
		/// <param name="resources">array of resources; [0] contains the file resources, [1] contains the registry resources</param>
		/// <param name="log">a log entry; the first dimension is the row, the second is the column</param>
		/// <returns>an ArrayList containing strings of error messages produced during the comparison</returns>
		public static string[] CompareIncludedResources (Holodeck.Resource[][] resources, Hashtable[] knownResources) {
			Hashtable fileResources = CreateHashtable (resources[0]);
			Hashtable registryResouces = CreateHashtable (resources[1]);

			System.Collections.ArrayList[] diff = new System.Collections.ArrayList[2];
			diff[0] = CompareResourceTables (knownResources[0], fileResources, false);
			diff[1] = CompareResourceTables (knownResources[1], registryResouces, false);

			string[] differences = new string[diff[0].Count + diff[1].Count];
			int i = 0;
			for (int round = 0; round < 2; round++) {
				for (int j = 0; j < diff[round].Count; j++, i++) {
					differences[i] = (string) diff[round][j];
				}
			}

			return differences;
		}
	} // class Helper
} // namespace TestResourcePane