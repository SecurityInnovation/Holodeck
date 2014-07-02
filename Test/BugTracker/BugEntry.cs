using System;

namespace BugTracker
{
	/// <summary>
	/// Summary description for BugEntry.
	/// </summary>
	public class BugEntry
	{
		public string submitter = null;
		public DateTime submitDate;
		public string owner = null;
		public DateTime closeDate;
		public string mileStone = null;
		public int priority;
		public string product = null;
		public string type = null;
		public string description = null;
		public string history = null;
		public Guid bugID;
		public string title = null;
		public bool isOpen = false;

		public BugEntry()
		{
			

			//
			// TODO: Add constructor logic here
			//
		}
	}
}
