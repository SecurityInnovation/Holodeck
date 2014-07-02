using System;

namespace BugTracker
{
	/// <summary>
	/// Summary description for UserDB.
	/// </summary>
	public class UserDB
	{
		public string userID = null;
		public bool canAddBug = false;
		public bool canDeleteBug = false;
		public bool canModifyBug = false;

		public UserDB()
		{
			
		}
	}
}
