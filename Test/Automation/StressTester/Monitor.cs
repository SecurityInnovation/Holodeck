using System;
using System.Threading;

namespace StressTester
{
	/// <summary>
	/// Provides monitoring using the logging class and information from the Application class...
	/// </summary>
	public class Monitor
	{
        private Application application = null;
        public int applicationNotRespondingFailureRate = 20;           // number of times in thread loop to go before terminating...
        public TimeSpan threadSleepTime = TimeSpan.FromSeconds( 5 );   // determines the sampling rate for the application...
        private Log log;
        public string logLocation;
        private bool STOP_THREAD = false;
        private bool THREAD_STOPPED = false;
        private Thread monitorThread = null;

		public Monitor( Application appClass )
		{
            // Get the application class...
			application = appClass;

            // Since paths are not always guaranteed, we use the process name...
            logLocation = application.ProcessClass.ProcessName + ".log";

            log = new Log( logLocation );
		}

        private void AddStatisticsEntryToLog( )
        {
            string LogEntry = null;

            application.ProcessClass.Refresh( );

            LogEntry = DateTime.Now.ToString( );
            LogEntry += " TOTAL PROC TIME:" + application.ProcessClass.TotalProcessorTime;            
            LogEntry += " PHYS MEM USAGE:" + application.ProcessClass.WorkingSet;            
            LogEntry += " VIRTUAL MEM USAGE:" + application.ProcessClass.PagedMemorySize;
            LogEntry += " THREAD COUNT:" + application.ProcessClass.Threads.Count;
            LogEntry += " HANDLE COUNT:" + application.ProcessClass.HandleCount;    
                       
            log.WriteLine( LogEntry );         
        }

        /// <summary>
        /// This thread continually monitors the application and logs information about the app.
        /// </summary>
        private void MonitorThread( )
        {
            int notRespondingCount = applicationNotRespondingFailureRate;

            // Run forever, as long as the application exists...
            while ( (!application.ProcessClass.HasExited) && (!STOP_THREAD) )
            {
                Thread.Sleep( threadSleepTime );

                try
                {
                    AddStatisticsEntryToLog( );
                }
                catch ( System.InvalidOperationException )
                {
                    break;
                }               

                // If the application is not responding, decrement the counter...
                if ( !application.ProcessClass.Responding )
                    notRespondingCount--;
                else
                    notRespondingCount = applicationNotRespondingFailureRate;

                // Throw an exception if the application has apparently stopped responding...
                if ( notRespondingCount == 0 )
                {
                    log.WriteLine( "\nAPP NOT RESPONDING - SHUTTING DOWN" );

                    throw new Exception( "ERROR: Application stopped responding!" );
                }
            }

            // This information may not be available since we may not have started the process...
            try
            {
                log.WriteLine( "\nThe application exited with exit code: " + application.ProcessClass.ExitCode );
            
                log.WriteLine( "\nErrors recorded during execution: \n\t" + application.ProcessClass.StandardError.ReadToEnd( ) );
            }
            catch ( Exception )
            {
                // The above may not work if the app was attached to... If so, standarderror was not redirected...
            }

            log.Close( );

            THREAD_STOPPED = true;
        }

        /// <summary>
        /// Start monitoring the application specified in the appClass... 
        /// </summary>       
        public void Start( )
        {
            monitorThread = new Thread( new ThreadStart( MonitorThread ) );
            monitorThread.Start( );
        }

        public void Stop( )
        {
            // Stop the logging thread...
            STOP_THREAD = true;

            // Wait for the thread to send the stopped signal (closes the file)...
            while ( !THREAD_STOPPED )
                Thread.Sleep( 100 );
        }
	}
}
