using System;

namespace Database
{
    /// <summary>
    /// Defines a set of permissions to be used with a user.
    /// </summary>
    public class Permissions
    {
        private bool CanCreateTests;
        private bool CanDeleteTests;
        private bool CanModifyTests;
        private bool CanCreateAreas;
        private bool CanDeleteAreas;
        private bool CanModifyAreas;

        public Permissions ( )
        {
            CanCreateTests = true;
            CanDeleteTests = true;
            CanModifyTests = true;
            CanCreateAreas = true;
            CanDeleteAreas = true;
            CanModifyAreas = true;
        }

        public bool CreateTests
        {
            get
            {
                return CanCreateTests;
            }
            set
            {
                CanCreateTests = value;
            }
        }

        public bool DeleteTests
        {
            get
            {
                return CanDeleteTests;
            }
            set
            {
                CanDeleteTests = value;
            }
        }

        public bool ModifyTests
        {
            get
            {
                return CanModifyTests;
            }
            set
            {
                CanModifyTests = value;
            }
        }

        public bool CreateAreas
        {
            get
            {
                return CanCreateAreas;
            }
            set
            {
                CanCreateAreas = value;
            }
        }

        public bool DeleteAreas
        {
            get
            {
                return CanDeleteAreas;
            }
            set
            {
                CanDeleteAreas = value;
            }
        }

        public bool ModifyAreas
        {
            get
            {
                return CanModifyAreas;
            }
            set
            {
                CanModifyAreas = value;
            }
        }
    }

	/// <summary>
	/// Summary description for Users.
	/// </summary>
	public class User
	{
        private string UserID;
        private string ExternalID;
        private string UserName;
        private Permissions UserPermissions;

        public string ID
        {
            get
            {
                return UserID;
            }
            set
            {
                UserID = value;
            }
        }

        public string ExternID
        {
            get
            {
                return ExternalID;
            }
            set
            {
                ExternalID = value;
            }
        }

        public string Name
        {
            get
            {
                return UserName;
            }
            set
            {
                UserName = value;
            }
        }

        public Permissions PermissionSettings
        {
            get
            {
                return UserPermissions;
            }
            set
            {
                UserPermissions = value;
            }
        }

		public User()
		{
            UserName = null;
            UserID = Guid.NewGuid( ).ToString( );
            ExternalID = null;

			UserPermissions = new Permissions( );
		}
	}
}
