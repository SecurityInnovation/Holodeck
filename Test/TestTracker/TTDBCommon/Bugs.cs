using System;

namespace Database
{
	/// <summary>
	/// Summary description for Bugs.
	/// </summary>
	public class Bug
	{
        private string BugID;
        private string BugTestID;
        private string BugTitle;
        private string BugDescription;
        private string BugOwner;

		public Bug()
		{
		    BugID = Guid.NewGuid( ).ToString( );
		}

        public string ID
        {
            get
            {
                return BugID;
            }
            set
            {
                BugID = value;
            }
        }

        public string TestID
        {
            get
            {
                return BugTestID;
            }
            set
            {
                BugTestID = value;
            }
        }

        public string Title
        {
            get
            {
                return BugTitle;
            }
            set
            {
                BugTitle = value;
            }
        }

        public string Description
        {
            get
            {
                return BugDescription;
            }
            set
            {
                BugDescription = value;
            }
        }

        public string Owner
        {
            get
            {
                return BugOwner;
            }
            set
            {
                BugOwner = value;
            }
        }
	}
}
