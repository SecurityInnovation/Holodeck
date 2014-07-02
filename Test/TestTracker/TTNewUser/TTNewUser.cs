using System;
using Database;

namespace TTNewUser
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
    class TTNewUser
    {
        private static string UserName = null;

        private static bool ConvertResponseToBool( string response )
        {
            if ( response.ToUpper( ) == "Y" )
                return true;
            else
                return false;
        }

        private static void PrintUserInformation( User user )
        {
            Console.WriteLine( "User Information:" );
            Console.WriteLine( "\tUser Name: \t\t" + user.Name );
            Console.WriteLine( "\tExternal Account: \t" + user.ExternID );
            Console.WriteLine( "\tInternal GUID: \t\t" + user.ID );
            Console.WriteLine( "\tCanCreateTests: \t" + user.PermissionSettings.CreateTests );
            Console.WriteLine( "\tCanDeleteTests: \t" + user.PermissionSettings.DeleteTests );
            Console.WriteLine( "\tCanModifyTests: \t" + user.PermissionSettings.ModifyTests );
            Console.WriteLine( "\tCanCreateAreas: \t" + user.PermissionSettings.CreateAreas );
            Console.WriteLine( "\tCanDeleteAreas: \t" + user.PermissionSettings.DeleteAreas );
            Console.WriteLine( "\tCanModifyAreas: \t" + user.PermissionSettings.ModifyAreas );
        }

        private static void AddUser( )
        {
            string externalAccount = null;
            bool userAlreadyExists = false;

            Console.WriteLine( "\nADD USER: " + UserName + "\n" );
            Console.WriteLine( "To add a user, you must associate the username with an external" );
            Console.WriteLine( "account on the local machine or on a domain server. To do this," );
            Console.WriteLine( "enter the username as \"DOMAIN\\USERNAME\" or as \"MACHINE\\USERNAME\"\n");
            Console.Write( "External Account: " );

            // Read in the external user account to use...
            externalAccount = Console.ReadLine( );

            Permissions permissions = new Permissions( );

            Console.WriteLine( "\nNow, permissions for this user must be set.\n" );
            Console.Write( "Allow to Create Tests [y/n]? " );
            permissions.CreateTests = ConvertResponseToBool( Console.ReadLine( ) );

            Console.Write( "Allow to Delete Tests [y/n]? " );
            permissions.DeleteTests = ConvertResponseToBool( Console.ReadLine( ) );

            Console.Write( "Allow to Modify Tests [y/n]? " );
            permissions.ModifyTests = ConvertResponseToBool( Console.ReadLine( ) );

            Console.Write( "Allow to Create Areas [y/n]? " );
            permissions.CreateAreas = ConvertResponseToBool( Console.ReadLine( ) );

            Console.Write( "Allow to Delete Areas [y/n]? " );
            permissions.DeleteAreas = ConvertResponseToBool( Console.ReadLine( ) );

            Console.Write( "Allow to Modify Areas [y/n]? " );
            permissions.ModifyAreas = ConvertResponseToBool( Console.ReadLine( ) );

            User user = new User( );
            user.PermissionSettings = permissions;
            user.ExternID = externalAccount.ToUpper( );
            user.Name = UserName.ToUpper( );

            Console.WriteLine( "\n**YOU ENTERED**" );
            PrintUserInformation( user );
            Console.Write( "\nDoes the above information look correct [y/n]?" );
            
            try
            {
                if ( ConvertResponseToBool( Console.ReadLine( ) ) )
                {
                    SQLDatabase dbConnection = new SQLDatabase( );
                    dbConnection.Connect( );

                    try
                    {
                        dbConnection.GetUserIDFromName( user.Name );

                        userAlreadyExists = true;
                    }
                    catch ( Exception )
                    {
                        // this is the SUCCESS case...
                    }

                    if ( userAlreadyExists )
                        throw new Exception( "User Already Exists!" );

                    dbConnection.AddUser( user );
                    dbConnection.Disconnect( );

                    Console.WriteLine( "\nTHE USER WAS ADDED SUCCESSFULLY" );
                }
                else
                {
                    Console.WriteLine( "\nPLEASE RUN THE TOOL AGAIN WITH THE PROPER INFORMATION" );
                }
            }
            catch ( Exception )
            {
                Console.WriteLine( "\nERROR: The user could not be added because the user already exists, or" );
                Console.WriteLine( "you do not have access to the sql database." );
            }
        }

        private static void ShowUsageInformation ( )
        {
            Console.WriteLine( "== Test Tracker User Add/Remove Tool ==" );
            Console.WriteLine( "-a [USERNAME]       - Adds the specified user." );   
            Console.WriteLine( "-d [USERNAME]       - Deletes the specified user." );
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            char [] argumentArray;

            if ( args.Length == 0 )
            {
                Console.WriteLine( "ERROR: No arguments provided." );
                ShowUsageInformation( );
            }
            else
            {
                // go through all the arguments passed on the commandline...
                for ( int argCount = 0; argCount < args.Length; argCount ++ )
                {
                    argumentArray = args[argCount].ToCharArray( );
                            
                    if ( args.Length > 1 )
                    {
                        if ( ( argumentArray[0] == '-' ) || ( argumentArray[0] == '/' ) && ( argCount != 0 ) )
                        {
                            // switch based on 
                            switch( argumentArray[1] )
                            {
                                case 'a':

                                    if ( ( argCount + 1 ) >= args.Length )
                                    {
                                        Console.WriteLine( "No argument provided!" );
                                    }
                                    else
                                    {
                                        UserName = args[argCount + 1];

                                        AddUser( );
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}