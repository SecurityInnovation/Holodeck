using System;
using Holodeck;

namespace Holodeck
{
	/// <summary>
	/// The first page of the "Create A New Project" wizard
	/// </summary>
	public class NewProjectPane1: Base
	{

		private static bool newVersion = true;//is this the new version of Holodeck or not (2.5.77+)

		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			ProjectLocation,
			Browse,
			Cancel,
			Next,
			Finish				// enabled if the 3rd page had been visited
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.ProjectLocation;

		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () 
		{
			switch (currPos) 
			{
				case TabStop.ProjectLocation: 
				{
					currPos = TabStop.Browse;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Browse: 
				{
					if (!NewProjectPane3.IsVisited ()) 
					{
						currPos = TabStop.Cancel;
					} 
					else 
					{
						currPos = TabStop.Finish;
					}
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Cancel: 
				{
					currPos = TabStop.Next;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Next: 
				{
					currPos = TabStop.ProjectLocation;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Finish: 
				{
					currPos = TabStop.Cancel;
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
		private static void GoTo (TabStop toPos) 
		{
			TabStop origPos = currPos;
			while (currPos != toPos) 
			{
				GoToNext ();
				if (currPos == origPos) 
				{
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}

		/// <summary>
		/// Bring this pane to its default state
		/// </summary>
		public static void Reset () 
		{
			// TODO: reset the pane

			if (NewProjectPane3.IsVisited ()) 
			{
				SendKeyStrokes ("{TAB}"); // Tab
			}

			currPos = TabStop.ProjectLocation;
		}

		/// <summary>
		/// Set the location of the new project
		/// </summary>
		/// <param name="location">the project's location</param>
		public static void SetProjectLocation (string location) 
		{
			GoTo (TabStop.ProjectLocation);

			SetEditBoxText (location);
		}
			
		/// <summary>
		/// Select and activate the "Next" button
		/// </summary>
		public static void Next () 
		{
			GoTo (TabStop.Next);

			SendKeyStrokes (" "); // Space
		}
	} // class NewProjectPane1


	/// <summary>
	/// The second page of the "Create A New Project" wizard
	/// </summary>
	public class NewProjectPane2: Base
	{
		private static bool newVersion = true;//is this the new version of Holodeck or not (2.5.77+)

		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			LaunchApplication,
			AttachToApplication,
			ApplicationName,
			Browse,
			Parameters,
			Refresh,
			PIDs,
			Cancel,
			Back,
			Next,
			Finish					// enabled if the 3rd page had been visited
		}

		/// <summary>
		/// Teh possible states of the pane
		/// </summary>
		private enum Selection
		{
			Launch,	// Launch Application
			Attach	// Attach to Application
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.LaunchApplication;
		/// <summary>
		/// The actual state of the pane
		/// </summary>
		private static Selection selectionStatus = Selection.Launch;
			
		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () 
		{
			// TODO: figure out a better way if possible

			switch (selectionStatus) 
			{
				case Selection.Launch: 
				{
					switch (currPos) 
					{
						case TabStop.LaunchApplication: 
						{
							if (newVersion) 
							{
								//new version
								currPos = TabStop.ApplicationName;
							}

							else 
							{
								//old version
								currPos = TabStop.PIDs;
							}

							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.AttachToApplication: // unreachable by TABs
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
						case TabStop.ApplicationName: 
						{
							if (newVersion) 
							{
								//new version
								currPos = TabStop.Parameters;
							}

							else 
							{
								//old version
								currPos = TabStop.Browse;
							}

							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Browse: 
						{
							currPos = TabStop.LaunchApplication;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Parameters: 
						{
							if (!NewProjectPane3.IsVisited ()) 
							{
								currPos = TabStop.Cancel;
							} 
							else 
							{
								currPos = TabStop.Finish;
							}
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Refresh: // disabled
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
						case TabStop.PIDs: 
						{
							currPos = TabStop.Parameters;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Cancel: 
						{
							if (newVersion) 
							{
								currPos = TabStop.Back;
							}

							else 
							{
								//old version
								currPos = TabStop.Next;
							}

							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Back: 
						{
							if (newVersion) 
							{
								//new version
								currPos = TabStop.Next;
							}
							else 
							{
								//old version
								currPos = TabStop.ApplicationName;
							}

							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Next: 
						{
							if (newVersion) 
							{
								//new version
								currPos = TabStop.ApplicationName;
							}

							else 
							{
								//old version
								currPos = TabStop.Back;
							}

							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Finish: 
						{
							if (newVersion) 
							{
								//new version
								currPos = TabStop.ApplicationName;
							}
							
							else 
							{
								//old version
								currPos = TabStop.Cancel;
							}

							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						default : 
						{
							break;
						}
					}
					break;
				} // case Selection.Launch
				case Selection.Attach: 
				{
					switch (currPos) 
					{
						case TabStop.LaunchApplication: // unreachable by TABs
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
						case TabStop.AttachToApplication: 
						{
							currPos = TabStop.PIDs;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.ApplicationName: // disabled
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
						case TabStop.Browse: // disabled
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
						case TabStop.Parameters: //disabled
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
						case TabStop.Refresh: 
						{
							if (!NewProjectPane3.IsVisited ()) 
							{
								currPos = TabStop.Cancel;
							} 
							else 
							{
								currPos = TabStop.Finish;
							}
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.PIDs: 
						{
							currPos = TabStop.Refresh;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Cancel: 
						{
							currPos = TabStop.Next;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Back: 
						{
							currPos = TabStop.LaunchApplication;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Next: 
						{
							currPos = TabStop.Back;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						case TabStop.Finish: 
						{
							currPos = TabStop.Cancel;
							SendKeyStrokes ("{TAB}"); // Tab
							break;
						}
						default : 
						{
							throw new HolodeckExceptions.UnaccessableControlException ();
						}
					}
					break;
				} // case Selection.Attach
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
		private static void GoTo (TabStop toPos) 
		{
			TabStop origPos = currPos;
			while (currPos != toPos) 
			{
				GoToNext ();
				if (currPos == origPos) 
				{
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}
			
		/// <summary>
		/// Bring this pane to its default state
		/// </summary>
		public static void Reset () 
		{
			// TODO: figure out a better way to handle the first time selection (which is between Back and ApplicationName)

			SendKeyStrokes ("{TAB}"); // Tab
			currPos = TabStop.ApplicationName;
			selectionStatus = Selection.Launch;
		}

		/// <summary>
		/// Set the name of the application
		/// </summary>
		/// <param name="name">the application's name</param>
		public static void SetApplicationName (string name) 
		{
			GoTo (TabStop.ApplicationName);

			SetEditBoxText (name);
		}

		/// <summary>
		/// Set the parameters of the application
		/// </summary>
		/// <param name="parameters">the application's parameters in one string</param>
		public static void SetApplicationParams (string parameters) {
			GoTo (TabStop.Parameters);

			SetEditBoxText (parameters);
		}

		/// <summary>
		/// Set the parameters of the application
		/// </summary>
		/// <param name="parameters">the application's parameters in a string array</param>
		public static void SetApplicationParams (string[] parameters) {
			GoTo (TabStop.Parameters);

			string p = "";
			foreach (string s in parameters) {
				p += s;
			}

			SetEditBoxText (p);
		}

		/// <summary>
		/// Select and activate the "Next" button
		/// </summary>
		public static void Next () 
		{
			GoTo (TabStop.Next);

			SendKeyStrokes (" "); // Space
		}

		/// <summary>
		/// Select and activate the "Back" button
		/// </summary>
		public static void Back () 
		{
			GoTo (TabStop.Back);

			SendKeyStrokes (" "); // Space
		}

		/// <summary>
		/// Select and activate the "Finish" button
		/// </summary>
		public static void Finish () 
		{
			if (!NewProjectPane3.IsVisited ()) 
			{
				throw new HolodeckExceptions.UnaccessableControlException ();
			}

			GoTo (TabStop.Finish);

			SendKeyStrokes (" "); // Space

			// Clean up after clicking "Finish"
			NewProjectPane3.CleanUp ();
		}

		/// <summary>
		/// Select the "Attach to Aplication" checkbox
		/// </summary>
		public static void AttachToApplication () 
		{
			if (selectionStatus != Selection.Attach) 
			{
				GoTo (TabStop.LaunchApplication);

				SendKeyStrokes ("{LEFT}"); // Left arrow
			}
		}
	} // class NewProjectPane2


	/// <summary>
	/// The third page of the "Create A New Project" wizard
	/// </summary>
	public class NewProjectPane3: Base
	{
		private static bool newVersion = true;//is this the new version of Holodeck or not (2.5.77+)

		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop // TODO: create a class from this with methods: Next, Current. The logic should be loaded from a config file.
		{
			PauseApplication,
			ProcessChaining,
			Functions,
			Cancel,
			Back,
			Finish
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.PauseApplication;
		/// <summary>
		/// The current status of the "Pause Application" checkbox
		/// true - checked; false - unchecked
		/// </summary>
		private static bool pauseApplicationState = false; // TODO: enclose it to an object
		/// <summary>
		/// The current status of the "Process Chaining" checkbox
		/// true - checked; false - unchecked
		/// </summary>
		private static bool processChainingState = true; // TODO: enclose it to an object
		/// <summary>
		/// Has the pane been visited yet
		/// </summary>
		private static bool isVisited = false;

		/// <summary>
		/// Return if the pane has been visited
		/// </summary>
		/// <returns>has the pane been visited</returns>
		public static bool IsVisited () 
		{
			return isVisited;
		}

		/// <summary>
		/// Change the focus to the next control
		/// </summary>
		private static void GoToNext () 
		{
			switch (currPos) 
			{
				case TabStop.PauseApplication: 
				{
					if (newVersion) 
					{
						//new version
						SendKeyStrokes("{TAB}");
					}

					currPos = TabStop.ProcessChaining;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.ProcessChaining:
				{
					currPos = TabStop.Functions;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Functions: 
				{
					currPos = TabStop.Finish;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Cancel: 
				{
					currPos = TabStop.Back;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Back: 
				{
					currPos = TabStop.PauseApplication;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.Finish: 
				{
					currPos = TabStop.Cancel;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				default : 
				{
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}
			
		/// <summary>
		/// Change the focus to the specified control
		/// </summary>
		/// <param name="toPos">the desired control</param>
		private static void GoTo (TabStop toPos) 
		{
			TabStop origPos = currPos;
			while (currPos != toPos) 
			{
				GoToNext ();
				if (currPos == origPos) 
				{
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}
			
		/// <summary>
		/// Bring this pane to its default state
		/// </summary>
		public static void Reset () 
		{
			// TODO: figure out a better way to handle the first time selection (which is between Back and PauseApplication)

			SendKeyStrokes ("{TAB}"); // Tab
			currPos = TabStop.PauseApplication;
			isVisited = true;
		}

		/// <summary>
		/// Set the state of the "Pause Application" checkbox
		/// </summary>
		/// <param name="newState">the new state</param>
		public static void SetPauseApplication (bool newState) 
		{
			if (newState != pauseApplicationState) 
			{
				GoTo (TabStop.PauseApplication);

				SendKeyStrokes (" "); // Space
			}
		}
			
		/// <summary>
		/// Set the state of the "Process Chaining" checkbox
		/// </summary>
		/// <param name="newState">the new state</param>
		public static void SetProcessChaining (bool newState) 
		{
			if (newState != processChainingState) 
			{
				GoTo (TabStop.ProcessChaining);

				SendKeyStrokes (" "); // Space
			}
		}
			
		/// <summary>
		/// Select and activate the "Back" button
		/// </summary>
		public static void Back () 
		{
			GoTo (TabStop.Back);

			SendKeyStrokes (" "); // Space
		}

		/// <summary>
		/// Select and activate the "Finish" button
		/// </summary>
		public static void Finish () 
		{

			GoTo (TabStop.Finish);

			SendKeyStrokes (" "); // Space

/*
			SendKeyStrokes ("&f"); // ALT-f
*/
			// Clean up after clicking "Finish"
			CleanUp ();
		}

		/// <summary>
		/// Clean up work after clicking "Finish" on any of the panes
		/// </summary>
		public static void CleanUp () 
		{
			// The next time the wizard is invoked, this pane will not have been visied
			isVisited = false;

			// Bring Holodeck's main window back to focus
			// TODO: figure out a better way to do this (we should not "hack" like this here...)
			HolodeckProcess.WaitForHolodeck ();
			SetForegroundWindow (holodeckMainWindowHandle);
		}
	} // class NewProjectPane3
} // namespace Holodeck