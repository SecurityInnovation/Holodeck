using System;
using System.IO;
using System.Threading;
using TestFramework.Utils;
using TestFramework.Run;

namespace TestFramework.Log
{
	/// <summary>
	/// Summary description for Log.
	/// </summary>
	public class Log
	{
        public static bool color = false;
        public static StreamWriter logWriter = null;
        public static bool logSilent = false;

        
        /// <summary>
        /// Sets up a new streamwriter for the calling thread when we are using multithreading...
        /// </summary>
        public static void setupNewStreamWriterForThread( )
        {
            // Wait for the mutex to become available before doing our logging...
            if ( RunTest.runningConcurrently )
                RunTest.g_mutex.WaitOne( );

            // If using multithreading, we open a new streamwriter which writes to separate logs depending
            // on the current thread calling us...
            if ( logWriter != null )
            {
                if ( RunTest.runningConcurrently )
                {
                    // We always append to the existing log file...
                    logWriter = new StreamWriter( 
                        RunTest.OutputPath + RunTest.logFile + "." + Thread.CurrentThread.Name,
                        true );
                }
            }
        }

        public static void closeCurrentStreamWriterForThread( )
        {
            if ( logWriter != null )
            {
                if ( RunTest.runningConcurrently )
                {
                    logWriter.Close( );
                }
            }

            // release the mutex upon closing...
            if ( RunTest.runningConcurrently )
                RunTest.g_mutex.ReleaseMutex( );
        }

        public static void printError( string error )
        {
            setupNewStreamWriterForThread( );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Cyan );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.Write( "==>" );
                }
            }
            else
                logWriter.Write( "==>" );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Red );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.WriteLine( "ERROR: " + error );
                }
            }
            else
                logWriter.WriteLine( "ERROR: " + error );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Grey, false );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.WriteLine( );
                }
            }
            else
                logWriter.WriteLine( );

            closeCurrentStreamWriterForThread( );

            if ( RunTest.runningConcurrently )
                RunTest.g_mutex.WaitOne( );
        }

        public static void printSuccess( string attribute, string message )
        {
            setupNewStreamWriterForThread( );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Cyan );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.Write( "==>" );
                }
            }
            else
                logWriter.Write( "==>" );

            if ( attribute != null )
            {
                if ( color )
                    ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Yellow, true );

                if ( logWriter == null )
                {
                    if ( ! logSilent )
                    {
                        Console.Write( attribute + ": " );
                    }
                }
                else
                    logWriter.Write( attribute + ": " );
            }

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Green );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.WriteLine( message );
                }
            }
            else
                logWriter.WriteLine( message );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Grey, false );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.WriteLine( );
                }
            }
            else
                logWriter.WriteLine( );

            closeCurrentStreamWriterForThread( );
        }

        public static void printSuccess( string message )
        {
            printSuccess( null, message );
        }

        public static void printTestStatus( string message )
        {
            setupNewStreamWriterForThread( );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Grey, true );

            if ( logWriter == null )
            {
                if ( ! logSilent )
                {
                    Console.WriteLine( message + "\n" );
                }
            }
            else
                logWriter.WriteLine( message + "\n" );

            if ( color )
                ConsoleColor.SetForeGroundColor( ConsoleColor.ForeGroundColor.Grey, false );
            
            closeCurrentStreamWriterForThread( );
        }
	}
}
