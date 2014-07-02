using System;

namespace Database
{
	/// <summary>
	/// Provides a class based representation of the encoded RunFrequency string...
	/// </summary>
	public class RunFrequency
	{
        public DateTime StartDate;
        public bool RunInfinite;           // The task should run over and over forever...
        public int RunNumberOfTimes;       // The task should run only this number of times...
        public TimeSpan Interval;          // The interval between runs for the task...
        private string RunFrequencyString; // Internal string representation of the RunFrequency...
        private char Separator = ';';

        private void RebuildFrequencyString(  )
        {
            RunFrequencyString = StartDate.ToString( ) + ";" + Interval.ToString( ) + ";" + RunNumberOfTimes.ToString( ) + ";" +
                RunInfinite.ToString( ).ToUpper( );
        }

		public RunFrequency( DateTime startDate, TimeSpan interval, int runNumberOfTimes, bool runInfinite )
		{	
            StartDate = startDate;
            Interval = interval;
            RunNumberOfTimes = runNumberOfTimes;
            RunInfinite = runInfinite;

            RunFrequencyString = StartDate.ToString( ) + ";" + Interval.ToString( ) + ";" + RunNumberOfTimes.ToString( ) + ";" +
                                 RunInfinite.ToString( ).ToUpper( );
		}

        public RunFrequency( string runFrequencyString )
        {
            // Save the string that is being passed in...
            RunFrequencyString = runFrequencyString;

            // Split up the string into its separate components...
            string [] FrequencyComponents = runFrequencyString.Split( Separator );

            // Pull out the components and store into the proper data types...
            StartDate = DateTime.Parse( FrequencyComponents[0] );
            Interval = TimeSpan.Parse( FrequencyComponents[1] );            
            RunNumberOfTimes = Convert.ToInt32( FrequencyComponents[2].ToUpper( ) );
            RunInfinite = ( FrequencyComponents[3].ToUpper( ) == "TRUE" );

        }

        public string GetFrequencyString( )
        {
            // Make sure the frequency string is up to date...
            RebuildFrequencyString( );

            return RunFrequencyString;
        }
	}
}
