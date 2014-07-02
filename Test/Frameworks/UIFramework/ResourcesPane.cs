using System;

namespace Holodeck
{
	public struct Resource
	{
		public string	ErrorCode;
		public string	LastFunction;
		public string	Name;
		public long		NumHits;
		public long		processID;
		public string	ReturnValue;
		public long		threadID;
		public string	TimeStamp;
	}

	/// <summary>
	/// Summary description for ResourcesPane.
	/// </summary>
	public class ResourcesPane : Base
	{
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		public enum TabStop
		{
			HeadLine,
			RegistryPane,
			FilePane
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.HeadLine;

		/// <summary>
		/// Go to the next visible pane at the Holodeck main window
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		private static void GoToNext ()
		{
			switch (currPos)
			{
				case TabStop.HeadLine:
				{
					currPos = TabStop.RegistryPane;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.RegistryPane: 
				{
					currPos = TabStop.FilePane;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.FilePane: 
				{
					currPos = TabStop.HeadLine;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				default: 
				{
					// TODO: provide meaningful information in the exception
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}

		/// <summary>
		/// Change the focus to the specified control
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
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
					// TODO: provide meaningful information in the exception
					throw new HolodeckExceptions.UnaccessableControlException ();
				}
			}
		}

		/// <summary>
		/// Sets the "Resources" pane to its default state
		/// </summary>
		public static void Reset ()
		{
			ResetFileResourcesPane ();
			ResetRegistryResourcesPane ();
		}

		/// <summary>
		/// Sets the "File Resources" subpane to its default state
		/// </summary>
		protected static void ResetFileResourcesPane ()
		{
			SelectFileResourcesPane ();
			GoToFirstElement ();
		}

		/// <summary>
		/// Sets the "Registry Resources" subpane to its default state
		/// </summary>
		protected static void ResetRegistryResourcesPane ()
		{
			SelectFileResourcesPane ();
			GoToFirstElement ();
		}

		/// <summary>
		/// Selects the "File Resources" subpane
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		protected static void SelectFileResourcesPane ()
		{
			GoTo (TabStop.FilePane);
		}

		/// <summary>
		/// Selects the "Registry Resources" subpane
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		protected static void SelectRegistryResourcesPane ()
		{
			GoTo (TabStop.RegistryPane);
		}

		/// <summary>
		/// Gets the "File Resources" subpane's entries as Resources
		/// </summary>
		/// <returns>the resources associated with the "File Resources" subpane entries</returns>
		public static Resource[] GetFileResources ()
		{
			SelectFileResourcesPane ();
			return GetResourceEntries ();
		}

		/// <summary>
		/// Gets the "Registry Resources" subpane's entries as Resources
		/// </summary>
		/// <returns>the resources associated with the "Registry Resources" subpane entries</returns>
		public static Resource[] GetRegistryResources ()
		{
			SelectRegistryResourcesPane ();
			return GetResourceEntries ();
		}

		/// <summary>
		/// Gets the resources of the current subpane
		/// Note: Assumes that the current control is at one of the subpanes
		/// -- Might throw a HolodeckExceptions.WindowNotFoundException
		/// -- Might throw a HolodeckExceptions.IncorrectOperationException
		/// </summary>
		/// <returns>the resources associated with the current subpane entries</returns>
		protected static Resource[] GetResourceEntries ()
		{
			WindowEnvironment environment;
			IntPtr resourceWindowHandle = IntPtr.Zero;

			environment.checkTitle = true;
			environment.title = "";
			environment.searchedHandle = false;
			environment.childs = new WindowEnvironment[3];

			environment.childs[0].checkTitle = true;
			environment.childs[0].title = "";
			environment.childs[0].searchedHandle = false;
			environment.childs[0].childs = null;

			environment.childs[1].checkTitle = true;
			environment.childs[1].title = "";
			environment.childs[1].searchedHandle = false;
			environment.childs[1].childs = null;

			environment.childs[2].checkTitle = true;
			environment.childs[2].title = "";
			environment.childs[2].searchedHandle = false;
			environment.childs[2].childs = null;

			switch (currPos) 
			{
				case TabStop.FilePane: 
				{
					environment.childs[2].searchedHandle = true;

					try {
						resourceWindowHandle = Base.FindHolodeckPane (environment);
					} catch (HolodeckExceptions.MismatchingWindowEnvironmentException) {
						resourceWindowHandle = IntPtr.Zero;
					} finally {
						if (resourceWindowHandle == IntPtr.Zero) {
							throw new HolodeckExceptions.WindowNotFoundException ("hWnd to \"File Resource\" pane was not found!");
						}
					}

					break;
				}
				case TabStop.RegistryPane:
				{
					environment.childs[0].searchedHandle = true;

					try {
						resourceWindowHandle = Base.FindHolodeckPane (environment);
					} catch {
						resourceWindowHandle = IntPtr.Zero;
					} finally {
						if (resourceWindowHandle == IntPtr.Zero) {
							throw new HolodeckExceptions.WindowNotFoundException ("hWnd to \"Registry Resource\" pane was not found!");
						}
					}

					break;
				} 
				default:
				{
					throw new HolodeckExceptions.IncorrectOperationException ("Trying to access resources, while not at the File or Registry resources pane.");
				}
			} 

			return GetResourceEntries (resourceWindowHandle, currPos);
		}

		/// <summary>
		/// Gets the resources of the current subpane
		/// Note: Assumes that the current control is at one of the subpanes
		/// -- Might throw a HolodeckExceptions.WindowNotFoundException
		/// -- Might throw a HolodeckExceptions.IncorrectOperationException
		/// </summary>
		/// <param name="resourceWindowHandle">handle to the subpane to get the resources from</param>
		/// <param name="pane">the TabStop representing the current pane</param>
		/// <returns>the resources associated with the current subpane entries</returns>
		
		protected static Resource[] GetResourceEntries (IntPtr resourceWindowHandle, TabStop pane)
		{
			string[][] resourceEntries = GetListViewEntries (resourceWindowHandle, 1);
			Resource[] retArray = new Resource[resourceEntries.Length];
			Holodeck.ResourcesPane.GoToFirstElement (); // This automatically updates the Property Pane
			for (int i = 0; i < retArray.Length; i++) 
			{
				GoTo (TabStop.HeadLine);
				HolodeckPane.SelectPropertiesPane ();
				PropertiesPane.Reset ();
				retArray[i] = PropertiesPane.GetResourcePropertyGridViewElements ();
				HolodeckPane.SelectResourcesPane ();
				GoTo (pane);
				Holodeck.ResourcesPane.GoToNextElement (); // This automatically updates the Property Pane
			}

			return retArray;
		}

		/// <summary>
		/// Go to the first element of the current subpane
		/// </summary>
		public static void GoToFirstElement () 
		{
			SendKeyStrokes ("{HOME}"); // Home
			// The first should be enough, but sometimes it just refreshes/rearranges the contents of the pane...
			SendKeyStrokes ("{HOME}"); // Home
		}

		/// <summary>
		/// Go to the next element of the current subpane
		/// </summary>
		public static void GoToNextElement () 
		{
			SendKeyStrokes ("{DOWN}"); // Down arrow
		}
	}
}
