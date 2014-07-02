using System;
using System.Data;
using System.Data.SqlClient;
using System.Collections;

namespace Database
{
    /// <summary>
    /// Represents a connection to the SQL database storing 
    /// </summary>
    public class SQLDatabase
    {
        // The strings below represent the name of the server, the security
        // type and the name of the database to open.
        public string DataSource = "localhost";
        public string IntegratedSecurity = "SSPI";
        public string InitialCatalog = "testtracker";
        public string AreaTable = "Areas";
        public string UserTable = "Users";
        public string TestTable = "Tests";
        public string TestHistoryTable = "TestHistory";
        public string BugTable = "Bugs";

        public SqlConnection SqlDataConnection;

        public void Connect( )
        {
            string connectionString = "Data Source=" + DataSource + ";Integrated Security="
                + IntegratedSecurity + ";Initial Catalog="
                + InitialCatalog;
            
            SqlDataConnection = new SqlConnection( connectionString );

            SqlDataConnection.Open( );

            return;
        }

        public void Connect( string connectionString )
        {
            SqlDataConnection = new SqlConnection( connectionString );

            SqlDataConnection.Open( );

            return;
        }

        public void Disconnect( )
        {
            SqlDataConnection.Close( );
        }

        public static string BoolToBitString( bool booleanExpr )
        {
            if ( booleanExpr )
                return "1";
            else
                return "0";
        }

        /// <summary>
        /// Adds an area by taking in an instance of an Area object.
        /// </summary>
        /// <param name="area"></param>
        public void AddArea( Area area )
        {
            string areaCommand = "INSERT " + AreaTable + " (AreaID,AreaName,ParentAreaID) VALUES('" +
                area.ID + "','" + area.Name + "','" + area.Parent + "')";
            
            SqlCommand cmd = new SqlCommand( areaCommand, SqlDataConnection );

            // Execute the query to add to the table the new area.
            cmd.ExecuteNonQuery( );

            return;
        }

        /// <summary>
        /// Adds a user by taking in an instance of a user object.
        /// </summary>
        /// <param name="UserName"></param>
        /// <param name="ExternalID"></param>
        /// <param name="PermissionSettings"></param>
        public void AddUser( User user )
        {
            string userCommand = "INSERT " + UserTable + " (UserID,ExternalID,UserName,CanCreateTests,CanDeleteTests,CanModifyTests,CanCreateAreas,CanDeleteAreas,CanModifyAreas) " +
                "VALUES ('" + user.ID + "','" + user.ExternID + "','" + user.Name + "'," + BoolToBitString( user.PermissionSettings.CreateTests ) +
                "," + BoolToBitString( user.PermissionSettings.DeleteTests ) + "," + BoolToBitString( user.PermissionSettings.ModifyTests ) + "," +
                BoolToBitString( user.PermissionSettings.CreateAreas ) + "," + BoolToBitString( user.PermissionSettings.DeleteAreas ) + "," +
                BoolToBitString( user.PermissionSettings.ModifyAreas ) + ")";
           
            SqlCommand cmd = new SqlCommand( userCommand, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void AddTest( Test test )
        {
            string testCommand = "INSERT " + TestTable + " (TestID,TestName,TestDescription,AreaID,TestPriority,TestOwnerID,ExecutionPath,ExecutionBinary,OperatingSystem,OSVersion,RunFrequency,ExecutionArgs,State) " +
                                 "VALUES ('" + test.ID + "','" + test.Name + "','" + test.Description + "','" + test.AreaID + "','" + test.Priority + "','" + test.OwnerID + "','" + test.ExecutionPath +
                                 "','" + test.ExecutionBinary + "','" + test.OperatingSystem + "','" + test.OSVersion + "','" + test.RunFrequency + "','" + test.ExecutionArgs + "','" + test.State + "')";
        
            SqlCommand cmd = new SqlCommand( testCommand, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void UpdateTest( string TestID, Test test )
        {
            string testCommand = "UPDATE " + TestTable + " SET " +
                "TestID='" + test.ID + "', TestName='" + test.Name + "', TestDescription='" + test.Description + "', AreaID='" + test.AreaID + "', TestPriority='" + test.Priority + "', TestOwnerID='" + test.OwnerID + "', ExecutionPath='" + test.ExecutionPath +
                "', ExecutionBinary='" + test.ExecutionBinary + "', OperatingSystem='" + test.OperatingSystem + "', OSVersion='" + test.OSVersion + "', RunFrequency='" + test.RunFrequency + "', ExecutionArgs='" + test.ExecutionArgs + "', State='" + test.State + "' WHERE TestID='" + TestID + "'";
        
            SqlCommand cmd = new SqlCommand( testCommand, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void AddTestHistory( TestHistory testHistory )
        {
            string testHistoryCommand = "INSERT " + TestHistoryTable + " (HistoryID,RunTime,TestID,Status) " + "VALUES ('" +
                                        testHistory.ID + "','" + testHistory.Runtime.ToString( ) + "','" + testHistory.TestID + "','" + testHistory.Status + "')";

            SqlCommand cmd = new SqlCommand( testHistoryCommand, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void AddBug( Bug bug )
        {
            string bugCommand = "INSERT " + BugTable + " (BugID,TestID,BugTitle,BugDescription,BugOwner) " + "VALUES ('" + bug.ID + "','" + bug.TestID + "','" + bug.Title +
                                "','" + bug.Description + "','" + bug.Owner + "')";
            
            SqlCommand cmd = new SqlCommand( bugCommand, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public string RetreiveRootAreaID ( )
        {
            string areaCommand = "SELECT * FROM " + AreaTable + " WHERE AreaID=ParentAreaID";

            SqlCommand cmd = new SqlCommand( areaCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            dataReader.Read( );

            string ID = dataReader["AreaID"].ToString( );

            dataReader.Close( );

            return ID;
        }

        public ArrayList RetrieveChildAreaIDs ( string parentID )
        {
            ArrayList childAreaIDs = new ArrayList( );

            string areaCommand = "SELECT * FROM " + AreaTable + " WHERE ParentAreaID='" + parentID + "'";

            SqlCommand cmd = new SqlCommand( areaCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            while ( dataReader.Read( ) )
            {
                childAreaIDs.Add( dataReader["AreaID"].ToString( ) );
            }

            dataReader.Close( );

            return childAreaIDs;

        }

        /// <summary>
        /// Retrive an Area by its ID...
        /// </summary>
        /// <param name="ID"></param>
        public Area RetrieveArea( string ID )
        {
            string areaCommand = "SELECT * FROM " + AreaTable + " WHERE AreaID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( areaCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            // we only expect one row in the result set, so read that row...
            dataReader.Read( );

            Area retrievedArea = new Area( );

            retrievedArea.ID = dataReader["AreaID"].ToString( );
            retrievedArea.Name = dataReader["AreaName"].ToString( );
            retrievedArea.Parent = dataReader["ParentAreaID"].ToString( );

            dataReader.Close( );

            return retrievedArea;
        }

        /// <summary>
        /// Retrieve a user by its ID...
        /// </summary>
        /// <param name="ID"></param>
        /// <returns></returns>
        public User RetrieveUser ( string ID )
        {
            string userCommand = "SELECT * FROM " + UserTable + " WHERE UserID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( userCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            User retrievedUser = new User( );
            retrievedUser.ID = "";

            if ( dataReader.HasRows )
            {
                dataReader.Read( );            

                retrievedUser.ID = dataReader["UserID"].ToString( );
                retrievedUser.ExternID = dataReader["ExternalID"].ToString( );
                retrievedUser.Name = dataReader["UserName"].ToString( );
                
                retrievedUser.PermissionSettings = new Permissions( );
                retrievedUser.PermissionSettings.CreateTests = (dataReader["CanCreateTests"].ToString( ) == "True");
                retrievedUser.PermissionSettings.ModifyTests = (dataReader["CanModifyTests"].ToString( ) == "True");
                retrievedUser.PermissionSettings.DeleteTests = (dataReader["CanDeleteTests"].ToString( ) == "True");
                retrievedUser.PermissionSettings.CreateAreas = (dataReader["CanCreateAreas"].ToString( ) == "True");
                retrievedUser.PermissionSettings.ModifyAreas = (dataReader["CanModifyAreas"].ToString( ) == "True");
                retrievedUser.PermissionSettings.DeleteAreas = (dataReader["CanDeleteAreas"].ToString( ) == "True");
                
            }
                            
            dataReader.Close( );

            return retrievedUser;
        }

        public ArrayList RetrieveAllUsers ( )
        {
            ArrayList users = new ArrayList( );

            string userCommand = "SELECT * FROM " + UserTable;

            SqlCommand cmd = new SqlCommand( userCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            User retrievedUser = null;            

            if ( dataReader.HasRows )
            {
                while( dataReader.Read( ) )
                {
                    retrievedUser = new User( );

                    retrievedUser.ID = dataReader["UserID"].ToString( );
                    retrievedUser.ExternID = dataReader["ExternalID"].ToString( );
                    retrievedUser.Name = dataReader["UserName"].ToString( );
                
                    retrievedUser.PermissionSettings = new Permissions( );
                    retrievedUser.PermissionSettings.CreateTests = (dataReader["CanCreateTests"].ToString( ) == "True");
                    retrievedUser.PermissionSettings.ModifyTests = (dataReader["CanModifyTests"].ToString( ) == "True");
                    retrievedUser.PermissionSettings.DeleteTests = (dataReader["CanDeleteTests"].ToString( ) == "True");
                    retrievedUser.PermissionSettings.CreateAreas = (dataReader["CanCreateAreas"].ToString( ) == "True");
                    retrievedUser.PermissionSettings.ModifyAreas = (dataReader["CanModifyAreas"].ToString( ) == "True");
                    retrievedUser.PermissionSettings.DeleteAreas = (dataReader["CanDeleteAreas"].ToString( ) == "True");

                    users.Add( retrievedUser );
                }

                dataReader.Close( );
            }
                            

            return users;
        }

        public string GetUserIDFromName ( string userName )
        {
            string user = null;

            string userCommand = "SELECT * FROM " + UserTable + " WHERE UserName='" + userName + "'";

            SqlCommand cmd = new SqlCommand( userCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            // If we can't find the user, an exception is thrown.
            if ( dataReader.HasRows )
            {
                dataReader.Read( );

                user = dataReader["UserID"].ToString( );
            }

            dataReader.Close( );            

            return user;
        }

        public string GetUserIDFromExternalID ( string externalID )
        {
            string user = null;

            string userCommand = "SELECT * FROM " + UserTable + " WHERE ExternalID='" + externalID + "'";

            SqlCommand cmd = new SqlCommand( userCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            // If we can't find the user, an exception is thrown.
            if ( dataReader.HasRows )
            {
                dataReader.Read( );

                user = dataReader["UserID"].ToString( );
            }

            dataReader.Close( );            

            return user;
        }

        /// <summary>
        /// Retrieves an ID list of all tests contained within the test area.
        /// </summary>
        /// <param name="areaID"></param>
        /// <returns></returns>
        public ArrayList RetrieveTestIDsInArea( string areaID )
        {
            ArrayList testIDs = new ArrayList( );

            string testCommand = "SELECT * FROM " + TestTable + " WHERE AreaID='" + areaID + "'";

            SqlCommand cmd = new SqlCommand( testCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            while ( dataReader.Read( ) )
            {
                testIDs.Add( dataReader["TestID"].ToString( ) );
            }

            dataReader.Close( );

            return testIDs;
        }

        public Test RetrieveTest ( string ID )
        {
            string testCommand = "SELECT * FROM " + TestTable + " WHERE TestID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            Test retrievedTest = new Test( );

            if ( dataReader.HasRows )
            {
                dataReader.Read( );                

                retrievedTest.ID = dataReader["TestID"].ToString( );
                retrievedTest.Name = dataReader["TestName"].ToString( );
                retrievedTest.Description = dataReader["TestDescription"].ToString( );
                retrievedTest.AreaID = dataReader["AreaID"].ToString( );
                retrievedTest.Priority = dataReader["TestPriority"].ToString( );
                retrievedTest.OwnerID = dataReader["TestOwnerID"].ToString( );
                retrievedTest.ExecutionPath = dataReader["ExecutionPath"].ToString( );
                retrievedTest.ExecutionBinary = dataReader["ExecutionBinary"].ToString( );
                retrievedTest.OperatingSystem = dataReader["OperatingSystem"].ToString( );
                retrievedTest.OSVersion = dataReader["OSVersion"].ToString( );
                retrievedTest.RunFrequency = dataReader["RunFrequency"].ToString( );
                retrievedTest.ExecutionArgs = dataReader["ExecutionArgs"].ToString( );
                retrievedTest.State = dataReader["State"].ToString( );

                dataReader.Close( );
            }
            else
            {
                dataReader.Close( );

                return null;
            }            

            return retrievedTest;
        }

        public TestHistory RetrieveTestHistory ( string ID )
        {
            string testHistoryCommand = "SELECT * FROM " + TestHistoryTable + " WHERE HistoryID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testHistoryCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            dataReader.Read( );

            TestHistory retrievedTestHistory = new TestHistory( );

            retrievedTestHistory.ID = dataReader["HistoryID"].ToString( );
            retrievedTestHistory.Runtime = DateTime.Parse( dataReader["RunTime"].ToString( ) );
            retrievedTestHistory.TestID = dataReader["TestID"].ToString( );
            retrievedTestHistory.Status = dataReader["Status"].ToString( );

            dataReader.Close( );

            return retrievedTestHistory;
        }

        public ArrayList RetrieveTestHistories( string testID )
        {
            ArrayList testHistories = new ArrayList( );

            string testHistoryCommand = "SELECT * FROM " + TestHistoryTable + " WHERE TestID='" + testID + "'";

            SqlCommand cmd = new SqlCommand( testHistoryCommand, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            TestHistory retrievedTestHistory = null;

            while ( dataReader.Read( ) )
            {                
                retrievedTestHistory = new TestHistory( );

                retrievedTestHistory.ID = dataReader["HistoryID"].ToString( );
                retrievedTestHistory.Runtime = DateTime.Parse( dataReader["RunTime"].ToString( ) );
                retrievedTestHistory.TestID = dataReader["TestID"].ToString( );
                retrievedTestHistory.Status = dataReader["Status"].ToString( );

                testHistories.Add( retrievedTestHistory );
            }

            dataReader.Close( );

            return testHistories;        
        }

        public Bug RetrieveBug( string ID )
        {
            string testBug = "SELECT * FROM " + BugTable + " WHERE BugID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testBug, SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            dataReader.Read( );

            Bug retrievedBug = new Bug( );

            retrievedBug.ID = dataReader["BugID"].ToString( );
            retrievedBug.TestID = dataReader["TestID"].ToString( );
            retrievedBug.Title = dataReader["BugTitle"].ToString( );
            retrievedBug.Description = dataReader["BugDescription"].ToString( );
            retrievedBug.Owner = dataReader["BugOwner"].ToString( );

            dataReader.Close( );

            return retrievedBug;
        }

        public void DeleteArea( string ID )
        {
            string testArea = "DELETE FROM " + AreaTable + " WHERE AreaID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testArea, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void DeleteUser( string ID )
        {
            string testUser = "DELETE FROM " + UserTable + " WHERE UserID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testUser, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void DeleteTest( string ID )
        {
            string testTest = "DELETE FROM " + TestTable + " WHERE TestID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testTest, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void DeleteTestHistory( string ID )
        {
            string testTestHistory = "DELETE FROM " + TestHistoryTable + " WHERE HistoryID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testTestHistory, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }

        public void DeleteBug( string ID )
        {
            string testBug = "DELETE FROM " + BugTable + " WHERE BugID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testBug, SqlDataConnection );

            cmd.ExecuteNonQuery( );

            return;
        }
    }
}
