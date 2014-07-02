using System;
using System.Collections;
using Database;         // use the database namespace core DB components
using System.Data.SqlClient;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Web.Mail;

namespace TTScheduler
{    
    /// <summary>
    /// The QueueEntry is the full representation of information needed to run the test. This is
    /// basically just an encapsulation of the Test class.
    /// </summary>
    public class QueueEntry
    {
        public Test TestInformation;

        public QueueEntry( )
        {
            TestInformation = new Test( );
        }

        public QueueEntry( Test testItem )
        {
            TestInformation = testItem;
        }
    }

    /// <summary>
    /// The RunItem class represents ONLY the information required to identify the test and
    /// the time that it should be added to the queue. This information is then used to
    /// query the SQL database for the full test information and put that into the queue
    /// for execution. The reason for this is so that we do not need to store the full 
    /// test structure in memory the whole time.
    /// </summary>
    public class RunItem
    {
        public string TestID;
        public string Frequency;
        private bool isRunning;

        public RunItem( )
        {
            TestID = null;
            Frequency = null;
            isRunning = false;
        }

        public RunItem( string testID, string frequency )
        {
            TestID = testID;
            Frequency = frequency;
            isRunning = false;
        }

        public bool IsRunning
        {
            get
            {
                return isRunning;
            }
            set
            {
                isRunning = value;
            }
        }
    }

	/// <summary>
	/// Summary description for RunQueue.
	/// </summary>
	public class RunQueue
	{
        public static String EmailSuffix = "se.fit.edu";
        public static String EmailSMTP = "hq.se.fit.edu";


        [DllImportAttribute( "user32.dll" )]
        public static extern IntPtr OpenInputDesktop( Int32 dwFlags, Int32 fInherit, Int32 dwDesiredAccess );

        [DllImportAttribute( "user32.dll" )]
        public static extern Int32 SetThreadDesktop( IntPtr hDesktop );

        private EventLog eventLog = new EventLog( "Application", System.Environment.MachineName, "TTScheduler" );
        private ArrayList RunItems = new ArrayList( );
        private Queue ExecutionQueue = new Queue( );
        public SQLDatabase dbConnection;
        private Mutex updateMutex = new Mutex( );
        private Mutex executionMutex = new Mutex( );

        // Represents the delay between additions to the queue - adjust as necessary.
        public TimeSpan QueueDelay = new TimeSpan( 0, 0, 10 );      // Queue (default: 30 secs)
        public TimeSpan RunItemDelay = new TimeSpan( 0, 0, 10 );     // RunTime SQL Queries (default: 2 mins)
        public TimeSpan ExecutionDelay = new TimeSpan( 0, 0, 10 );   // Delay between checking queue and attempting to run items

        /// <summary>
        /// Custom SQL Query Retrieves Scheduling Information for the scheduler...
        /// </summary>
        /// <returns></returns>
        public ArrayList RetrieveTestScheduleInformation( )
        {
            ArrayList runItems = new ArrayList( );

            string testCommand = "SELECT TestID,RunFrequency FROM " + dbConnection.TestTable;

            SqlCommand cmd = new SqlCommand( testCommand, dbConnection.SqlDataConnection );

            SqlDataReader dataReader = cmd.ExecuteReader( );

            // Read all items matching the criteria...
            while ( dataReader.Read( ) )
            {
                RunItem runItem = new RunItem( );

                runItem.TestID = dataReader["TestID"].ToString( );
                runItem.Frequency = dataReader["RunFrequency"].ToString( );
            
                runItems.Add( runItem );
            }

            dataReader.Close( );

            return runItems;
        }

        public void UpdateRunItemFrequency( string ID, string runFrequency )
        {
            string testCommand = "UPDATE " + dbConnection.TestTable + " SET RunFrequency='" + runFrequency + "' WHERE TestID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testCommand, dbConnection.SqlDataConnection );

            cmd.ExecuteNonQuery( );
        }

        public void UpdateTestState( string ID, string state )
        {
            string testCommand = "UPDATE " + dbConnection.TestTable + " SET State='" + state + "' WHERE TestID='" + ID + "'";

            SqlCommand cmd = new SqlCommand( testCommand, dbConnection.SqlDataConnection );

            cmd.ExecuteNonQuery( );
        }

        /// <summary>
        /// Constructor: Sets up a database connection and loads all the Test run information into
        /// memory for fast and easy access.
        /// </summary>
		public RunQueue( )
		{
            // First, setup a connection to the database...
            dbConnection = new SQLDatabase( );

            dbConnection.Connect( );

		    // First thing we need to do is load all the RunItems...
            RunItems = RetrieveTestScheduleInformation( );
		}

        /// <summary>
        /// If items are added to the SQL Database or Modified, we need to refresh the RunItems structure.
        /// </summary>
        private void RunItemRefresher( )
        {
            for (;;)
            {
                Thread.Sleep( RunItemDelay );

                updateMutex.WaitOne( );         // Start waiting for the mutex...

                // Clear out the current run items...
                RunItems.Clear( );

                // Retrieve the test schedule information again and reload into memory...
                RunItems = RetrieveTestScheduleInformation( );

                updateMutex.ReleaseMutex( );    // Release the mutex when done...
            }
        }

        /// <summary>
        /// Calculates the date and time that the test should run based on the specified start time and
        /// time interval.
        /// </summary>
        /// <param name="startTime"></param>
        /// <param name="interval"></param>
        /// <returns></returns>
        private DateTime CalculateRunTime( DateTime startTime, TimeSpan interval )
        {
            DateTime runTime = startTime;

            while ( runTime < DateTime.Now )
            {
                // Keep incrementing the runtime until it is greater than the current time...
                runTime = runTime + interval;
            }

            return runTime;
        }

        private void WriteExceptionToLog( Exception e )
        {
            eventLog.WriteEntry( "An Exception Occurred Internally: " + e.Message + "\n" + "Stack Trace: " + e.StackTrace,
                                 System.Diagnostics.EventLogEntryType.Error );
        }

        /// <summary>
        /// Calling this function uses the current date and time to update the queue with run Items...
        /// </summary>
        private void UpdateQueueWithRunItems( )
        {
            foreach ( RunItem item in RunItems )
            {
                try
                {
                    // Pull the frequency string into the RunFrequency class to split into its components...
                    RunFrequency runFrequency = new RunFrequency( item.Frequency );

                    DateTime runTime = new DateTime( 0 );

                    // Is it time for the application to run?
                    if ( runFrequency.StartDate + runFrequency.Interval <= DateTime.Now )
                    {
                        // Has the app already run the number of times it is allowed to?
                        if ( ( runFrequency.RunInfinite ) || ( runFrequency.RunNumberOfTimes != 0 ) )
                        {
                            // Is the application already in the queue, or is it already running?
                            if ( ( !IsRunItemInQueue( item ) ) && ( !( item.IsRunning ) ) )
                            {
                                // If everything checked out ok, we add the application to the queue.

                                // Perform a query on the Test ID to get the Test object back...
                                Test testRun = dbConnection.RetrieveTest( item.TestID );

                                // Create a QueueEntry item...
                                QueueEntry queueEntry = new QueueEntry( testRun );

                                // Add the item to the queue...
                                ExecutionQueue.Enqueue( queueEntry );
                            }
                        }
                    }

                }
                catch ( Exception e )
                {
                    // We catch all exceptions and log them to the event log...
                    WriteExceptionToLog( e );
                }                                
            }
        }

        /// <summary>
        /// A loop that constantly checks the current time to determine when items should be exceuted.
        /// </summary>
        private void Scheduler( )
        {
            for (;;)
            {
                // Sleep until the queue delay time has elapsed...
                Thread.Sleep( QueueDelay );

                updateMutex.WaitOne( );         // Start waiting for the mutex...

                // Check to see if any of the RunItems need to be added to the queue...
                UpdateQueueWithRunItems( );

                updateMutex.ReleaseMutex( );    // Release the mutex when done...
            }   
        }

        /// <summary>
        /// Performs a blocking execution based on the Test information in the queue entry that is
        /// passed. The method will NOT exit until the test has completed running.
        /// </summary>
        /// <param name="entry"></param>
        private int ExecuteQueueEntry( QueueEntry entry )
        {
            // We create a run string using the execution path... Note, that if the path is also included in
            // the filename, we truncate and remove just the file first.
            string RunString = entry.TestInformation.ExecutionPath + "\\" + System.IO.Path.GetFileName( entry.TestInformation.ExecutionBinary );

            Process appProc = new Process( );

            // Set information for the process...
            appProc.StartInfo.WorkingDirectory = entry.TestInformation.ExecutionPath;
            appProc.StartInfo.FileName = RunString;
            appProc.StartInfo.UseShellExecute = false;

            // Set the arguments to be passed to the application if they are provided...
            if ( entry.TestInformation.ExecutionArgs.Length != 0 )
                appProc.StartInfo.Arguments = entry.TestInformation.ExecutionArgs;

            try
            {
                // Start the process...
                appProc.Start( );
               
                // Wait for the process to exit before allowing any other processes to run...
                appProc.WaitForExit( );

                //appProc.ExitCode this represents success or failure...
                return appProc.ExitCode;
            }
            catch ( Exception e )
            {
                WriteExceptionToLog( e );
            }

            // If we get here, we failed...
            return 1;
        }

        /// <summary>
        /// Determines if a RunItem is in the execution queue...
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        private bool IsRunItemInQueue( RunItem item )
        {
            foreach( QueueEntry entry in ExecutionQueue )
            {
                if ( entry.TestInformation.ID == item.TestID )
                    return true;
            }

            return false;
        }
        
        public static void SendMailMessage( 
            String toEmail,
            String fromEmail,
            String subject,
            String message,
            String SMTPServer )
        {
            MailMessage messageInstance = new MailMessage( );
            messageInstance.To = toEmail;
            messageInstance.From = fromEmail;
            messageInstance.Subject = subject;
            messageInstance.Body = message;

            try
            {
                SmtpMail.SmtpServer = SMTPServer;
                SmtpMail.Send( messageInstance );
            }
            catch
            {

            }
        }

        private void SendEmail( RunItem item, bool bSuccess )
        {
            // Perform a query on the Test ID to get the Test object back...
            Test testInfo = dbConnection.RetrieveTest( item.TestID );

            String successString;

            User testOwner = dbConnection.RetrieveUser( testInfo.OwnerID );

            if ( bSuccess )
                successString = "SUCCESS";
            else
                successString = "FAILURE";

            String emailSubject = "TEST CASE \"" + testInfo.Name + "\" " + successString;

            String emailMessage = emailSubject + "\n\n";

            emailMessage += "DETAILS:\n";
            emailMessage += "RUNTIME: " + DateTime.Now.ToString( ) + "\n";
            emailMessage += "ID: " + testInfo.ID + "\n";
            emailMessage += "NAME: " + testInfo.Name + "\n";
            emailMessage += "OWNER: " + testOwner.Name + "\n";

            emailMessage += "\n\nPlease check any failing tests at the following server: \n";
            emailMessage += "\t" + Environment.MachineName;

            SendMailMessage( testOwner.Name + "@" + EmailSuffix,
                "testtracker@" + Environment.MachineName,
                emailSubject,
                emailMessage,
                EmailSMTP );
        }

        private void Executer( )
        {
            int exitCode = 0;
            TestHistory testHistory = null;

            for (;;)
            {
                Thread.Sleep( ExecutionDelay );

                // If there are items waiting on the queue, we will remove the first item and begin
                // execution...
                if ( ExecutionQueue.Count > 0 )
                {
                    updateMutex.WaitOne( );         // Start waiting for the mutex...
                    
                    // Pull out the queue entry...
                    QueueEntry entry = (QueueEntry)ExecutionQueue.Dequeue( );

                    // Set a flag on the RunItem to show that it is out of the queue and currently
                    // running.
                    foreach ( RunItem item in RunItems )
                    {
                        if ( item.TestID == entry.TestInformation.ID )
                            item.IsRunning = true;
                    }

                    updateMutex.ReleaseMutex( );    // Release the mutex when done...

                    // Add a new history entry to record that the test is now starting...
                    testHistory = new TestHistory( );
                    testHistory.TestID = entry.TestInformation.ID;
                    testHistory.Runtime = DateTime.Now;
                    testHistory.Status = "STARTED";
                    dbConnection.AddTestHistory( testHistory );

                    // Update the CURRENT state of the test...
                    UpdateTestState( entry.TestInformation.ID, "RUNNING" );

                    // Now, outside the main mutex, we perform our concurrent action for running the 
                    // executable, but this time using a different mutex. This makes sure that
                    // each application never runs more than one at a time.
                    executionMutex.WaitOne( );

                    exitCode = ExecuteQueueEntry( entry );

                    executionMutex.ReleaseMutex( );

                    // We must grab the mutex again to mess with the RunItem structure...
                    updateMutex.WaitOne( );

                    // Since the position or location of the RunItem in memory cannot be guaranteed because we
                    // left the mutex for the run of the program, we must go back and find it again...
                    foreach ( RunItem item in RunItems )
                    {   
                        if ( item.TestID == entry.TestInformation.ID )
                        {
                            // The application is no longer running, so update that state in the current 
                            // memory structure...
                            item.IsRunning = false;

                            // Modify the existing runFrequency trackingItem definition to contain the current
                            // time as the start time...
                            RunFrequency runFrequency = new RunFrequency( item.Frequency );
                            runFrequency.StartDate = DateTime.Now;
                    
                            // If this test is supposed to run a static number of times, we decrement the counter...
                            if ( !runFrequency.RunInfinite )
                                runFrequency.RunNumberOfTimes--;

                            // Update the item already in memory with the new changes...
                            item.Frequency = runFrequency.GetFrequencyString( );

                            // Now, update the SQL database with this information...
                            UpdateRunItemFrequency( item.TestID, runFrequency.GetFrequencyString( ) );

                            // Add a new history entry to record the results of the test...
                            testHistory = new TestHistory( );
                            testHistory.TestID = item.TestID;
                            testHistory.Runtime = runFrequency.StartDate;
                            
                            if ( exitCode == 0 )
                            {
                                testHistory.Status = "SUCCESS";
                                SendEmail( item, true );
                            }
                            else
                            {
                                testHistory.Status = "FAILURE (" + exitCode.ToString( ) + ")";
                                SendEmail( item, false );
                            }

                            // Update the CURRENT state of the test... - SUCCESS or FAILURE (either determines that
                            // the test is no longer running - STOPPED).
                            UpdateTestState( item.TestID, testHistory.Status );

                            dbConnection.AddTestHistory( testHistory );
                        }
                        else
                        {
                            // We should NEVER get here...
                            eventLog.WriteEntry( "An item existed in the execution queue which didn't exist in the database!",
                                                 System.Diagnostics.EventLogEntryType.Error );
                        }
                    }                    

                    // Release the mutex so that further threads may modify the RunItem structures...
                    updateMutex.ReleaseMutex( );
                }                
            }
        }

        /// <summary>
        /// The main entry point for execution which starts threads for handling the scheduling of
        /// tests.
        /// </summary>
        public void StartScheduler( )
        {
            // Create and start the RunItem Refresher thread (queries database to add entries into memory)...
            Thread runItemRefresherThread = new Thread( new ThreadStart( this.RunItemRefresher ) );
            runItemRefresherThread.Start( );

            // Create and start the main scheduler (adds items to run queue as they are needed)...
            Thread schedulerThread = new Thread( new ThreadStart( this.Scheduler ) );
            schedulerThread.Start( );

            // Create and start the execution thread (executes items one-by-one from the run queue)...
            Thread executerThread = new Thread( new ThreadStart( this.Executer ) );
            executerThread.Start( );
        }
	}
}
