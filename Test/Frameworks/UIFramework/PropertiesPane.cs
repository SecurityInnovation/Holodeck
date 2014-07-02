using System;

namespace Holodeck
{
	/// <summary>
	/// Summary description for PropertiesPane.
	/// </summary>
	public class PropertiesPane : Base
	{
		/// <summary>
		/// The possible states for control focus accessible through TABs
		/// </summary>
		private enum TabStop 
		{
			HeadLine,
			ToolBar,
			PropertyName,
			PropertyValue
		}

		private enum ResourcePropertyIndex
		{
			ErrorCode = 0,
			HasResourceTest = 1,
			LastFunction = 2,
			Name = 3,
			NumHits = 4,
			processID = 5,
			ReturnValue = 6,
			threadID = 7,
			TimeStamp = 8
		}

		/// <summary>
		/// The control having the focus
		/// </summary>
		private static TabStop currPos = TabStop.HeadLine;
//		private static IntPtr gridViewHandle = IntPtr.Zero;
		private static int ResourcePropertyNumber = 9;

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
					currPos = TabStop.ToolBar;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.ToolBar: 
				{
					currPos = TabStop.PropertyName;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.PropertyName: 
				{
					currPos = TabStop.PropertyValue;
					SendKeyStrokes ("{TAB}"); // Tab
					break;
				}
				case TabStop.PropertyValue: 
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
		/// Sets the "Properties" pane to its default state
		/// </summary>
		public static void Reset ()
		{
			currPos = TabStop.HeadLine;
		}

		/// <summary>
		/// Gets the handle to the "PropertyGridView" GridView
		/// </summary>
		/// <returns>the handle to the GridView</returns>
		private static IntPtr FindPropertyGridViewHandle ()
		{
			WindowEnvironment environment;

			environment.checkTitle = true;
			environment.title = "PropertyGridView";
			environment.searchedHandle = true;
			environment.childs = new WindowEnvironment[1];

			environment.childs[0].checkTitle = false;
			environment.childs[0].title = "";
			environment.childs[0].searchedHandle = false;
			environment.childs[0].childs = null;

			IntPtr propertyGridView = Base.FindHolodeckPane (environment);
			if (propertyGridView == IntPtr.Zero) 
			{
				throw new HolodeckExceptions.WindowNotFoundException ("hWnd to \"Property GridView\" pane was not found!");
			}

			return propertyGridView;
		}

		/// <summary>
		/// Sets the display of the GridView to ascending name ordering
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		private static void SetNameOrdering ()
		{
			GoTo (TabStop.ToolBar);
			Base.SendKeyStrokes ("{RIGHT}");	// Right arrow
			Base.SendKeyStrokes (" ");			// space
		}

		/// <summary>
		/// Go to the first element of the GridView
		/// </summary>
		private static void GoToFirstProperty ()
		{
			Base.SendKeyStrokes ("{HOME}");
		}

		/// <summary>
		/// Go to the next element of the GridView
		/// </summary>
		private static void GoToNextProperty () 
		{
			Base.SendKeyStrokes ("{DOWN}");
		}

		/// <summary>
		/// Gets the elements of the GridView as Name - Value pairs
		/// Note: Assumes that the GridView is showing the properties of a resource
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		/// <returns>the string list containing the Name - Value pairs of the GridView entries</returns>
		protected static string[] GetResourcePropertyGridViewElements2 ()
		{
			SetNameOrdering ();
			// this sets us to the PropertyName field, so we need to update currPos
			currPos = TabStop.PropertyName;

			GoToFirstProperty ();

			string[] retArray = new string[ResourcePropertyNumber];
			for (int i = 0; i < retArray.Length; i++) 
			{
				GoTo (TabStop.PropertyValue);

				retArray[i] = Base.GetEditBoxText (false);

				GoTo (TabStop.PropertyName);
				GoToNextProperty ();
			}

			GoTo (TabStop.HeadLine);

			return retArray;
		}

		/// <summary>
		/// Gets the elements of the GridView as Name - Value pairs
		/// Note: Assumes that the GridView is showing the properties of a resource
		/// -- Might throw a HolodeckExceptions.UnaccessableControlException
		/// </summary>
		/// <returns>the Resource representing the of the GridView entries</returns>
		public static Resource GetResourcePropertyGridViewElements ()
		{
			string[] GridViewEntries = GetResourcePropertyGridViewElements2 ();
			Resource resource;
			resource.ErrorCode = GridViewEntries[(int) ResourcePropertyIndex.ErrorCode];
			resource.LastFunction = GridViewEntries[(int) ResourcePropertyIndex.LastFunction];
			resource.Name = GridViewEntries[(int) ResourcePropertyIndex.Name].TrimEnd ('\\');
			resource.NumHits = Int64.Parse (GridViewEntries[(int) ResourcePropertyIndex.NumHits]);
			resource.processID = Int64.Parse (GridViewEntries[(int) ResourcePropertyIndex.processID]);
			resource.ReturnValue = GridViewEntries[(int) ResourcePropertyIndex.ReturnValue];
			resource.threadID = Int64.Parse (GridViewEntries[(int) ResourcePropertyIndex.threadID]);
			resource.TimeStamp = GridViewEntries[(int) ResourcePropertyIndex.TimeStamp];

			return resource;
		}
	}
}
