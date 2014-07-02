using System;
using System.IO;

namespace StressTester
{
	/// <summary>
	/// This class provides logging methods. These methods must handle multiple threads and
	/// prevent data from getting corrupted.
	/// </summary>
	public class Log
	{
        private StreamWriter logWriter = null;

		public Log( string fileName )
		{
            // Create a thread safe stream writer which appends to any existing file...
            logWriter = new StreamWriter( fileName, false );
            logWriter.AutoFlush = true;

            // Write the current date and time in the file as a separator...
            logWriter.WriteLine( "\n\n=== APPLICATION LOGGER STARTING: " + System.DateTime.Now + " ===\n" );
		}

        /// <summary>
        /// Writes raw text to the log...
        /// </summary>
        /// <param name="text"></param>
        public void Write( string text )
        {
            logWriter.Write( text );
        }

        /// <summary>
        /// Writes raw text to the log with a newline on the end...
        /// </summary>
        /// <param name="text"></param>
        public void WriteLine( string text )
        {
            logWriter.WriteLine( text );
        }

        public void Close( )
        {
            logWriter.Close( );
        }
	}
}
