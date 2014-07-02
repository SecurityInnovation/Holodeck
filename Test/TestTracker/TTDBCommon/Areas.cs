using System;

namespace Database
{
    /// <summary>
    /// Defines the areas represented in the database system.
    /// </summary>
    public class Area
    {
        private string AreaName;
        private string AreaID;
        private string ParentID;

        public Area ( )
        {
            AreaName = null;
            AreaID = Guid.NewGuid( ).ToString( ); // assigns a GUID to the area...
            ParentID = AreaID;                    // initially assigns the Area as parent of itself...
        }

        public string Name
        {
            get
            {
                return AreaName;
            }
            set
            {
                AreaName = value;
            }
        }

        public string ID
        {
            get
            {
                return AreaID;
            }
            set
            {
                AreaID = value;
            }
        }

        public string Parent
        {
            get
            {
                return ParentID;
            }
            set
            {
                ParentID = value;
            }
        }
    }
}
