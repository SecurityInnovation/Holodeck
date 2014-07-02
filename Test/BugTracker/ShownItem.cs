using System;

namespace BugTracker
{
	public class ShownItemComparer : System.Collections.IComparer
	{
		public static int indexToCompare = 0;

		#region IComparer Members

		public int Compare(object x, object y)
		{			
			ShownItem itemA = (ShownItem)x;
			ShownItem itemB = (ShownItem)y;

			string stringA = null;
			string stringB = null;

			switch ( indexToCompare )
			{
				case 0:
					stringA = itemA.ID;
					stringB = itemB.ID;
					break;

				case 1:
					stringA = itemA.submitter;
					stringB = itemB.submitter;
					break;

				case 2:
					stringA = itemA.submitDate;
					stringB = itemB.submitDate;
					break;

				case 3:
					stringA = itemA.owner;
					stringB = itemB.owner;
					break;

				case 4:
					stringA = itemA.priority;
					stringB = itemB.priority;
					break;

				case 5:
					stringA = itemA.title;
					stringB = itemB.title;
					break;
			}

			return stringA.CompareTo( stringB );			
		}

		#endregion

	}

	/// <summary>
	/// Summary description for ShownItem.
	/// </summary>
	public class ShownItem
	{
		public string ID;
		public string submitter;
		public string submitDate;
		public string owner;
		public string priority;
		public string title;

		public ShownItem()
		{
			//
			// TODO: Add constructor logic here
			//
		}
	}
}
