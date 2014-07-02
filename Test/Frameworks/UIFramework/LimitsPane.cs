using System;

namespace Holodeck {
	/// <summary>
	/// Summary description for LimitsPane.
	/// </summary>
	public class LimitsPane : Base {
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop { // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
			Header,
			
			DiskSlide,
			DiskUsed,
			DiskLimit,
			DiskUnits,
			
			MemorySlide,
			MemoryUsed,
			MemoryLimit,
			MemoryUnits,
			
			NUBSlide,
			NUBUsed,
			NUBLimit,
			NUBUnits,
			
			NDBSlide,
			NDBUsed,
			NDBLimit,
			NDBUnits, 
			
		}

		public enum Unit 
		{
			Byte,
			KB,
			MB,
			GB
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.Header;

		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () {
			switch (currPos) {
				case TabStop.Header: {
					currPos = TabStop.DiskSlide;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.DiskSlide: {
					currPos = TabStop.DiskUsed;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.DiskUsed: {
					currPos = TabStop.DiskLimit;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.DiskLimit: {
					currPos = TabStop.DiskUnits;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.DiskUnits: {
					currPos = TabStop.MemorySlide;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.MemorySlide: 
				{
					currPos = TabStop.MemoryUsed;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.MemoryUsed: 
				{
					currPos = TabStop.MemoryLimit;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.MemoryLimit: 
				{
					currPos = TabStop.MemoryUnits;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.MemoryUnits: 
				{
					currPos = TabStop.NUBSlide;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NUBSlide: 
				{
					currPos = TabStop.NUBUsed;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NUBUsed: 
				{
					currPos = TabStop.NUBLimit;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NUBLimit: 
				{
					currPos = TabStop.NUBUnits;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NUBUnits: 
				{
					currPos = TabStop.NDBSlide;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}


				case TabStop.NDBSlide: 
				{
					currPos = TabStop.NDBUsed;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NDBUsed: 
				{
					currPos = TabStop.NDBLimit;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NDBLimit: 
				{
					currPos = TabStop.NDBUnits;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}

				case TabStop.NDBUnits: 
				{
					currPos = TabStop.Header;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				default: 
				{
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}

		
		/// <summary>
		/// Change the focus to the specified control
		/// </summary>
		/// <param name="toPos">the desired control</param>
		private static void GoTo (TabStop toPos) {
			TabStop origPos = currPos;
			while (currPos != toPos) {
				GoToNext ();
				if (currPos == origPos) {
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}

		/// <summary>
		/// Set the location of the new project
		/// </summary>
		/// <param name="location">the project's location</param>
		public static void SetDiskLimit (long limit) {
			GoTo (TabStop.DiskLimit);
			SetEditBoxText (limit.ToString ());
		}

		/// <summary>
		/// Set the location of the new project
		/// </summary>
		/// <param name="location">the project's location</param>
		public static void SetDiskUnits (Unit unit) 
		{
			GoTo (TabStop.DiskUnits);
			SendKeyStrokes ("{HOME}"); // Home (selects Bytes)
			switch (unit) 
			{
				case Unit.Byte: 
				{
					// We don't have to do anything here
					break;
				}
				case Unit.KB: 
				{
					SendKeyStrokes ("{DOWN}"); // down arrow
					break;
				}
				case Unit.MB: 
				{
					SendKeyStrokes ("{DOWN}"); // down arrow
					SendKeyStrokes ("{DOWN}"); // down arrow
					break;
				}
				case Unit.GB: 
				{
					SendKeyStrokes ("{DOWN}"); // down arrow
					SendKeyStrokes ("{DOWN}"); // down arrow
					SendKeyStrokes ("{DOWN}"); // down arrow
					break;
				}
			}
		}

		
			
		// TODO: provide functions for accessing the tabstops. Make them public. They will call GoTo()
		// much like SetDiskLimit()
	}
}

// TODO: 