using System;
using System.Diagnostics;

namespace StressTester
{
	/// <summary>
	/// The application class provides information for the AUT. This class also monitors AUT activity.
	/// This class provides a multithreaded log reporting mechanism to determine when AUT failures
	/// are occurring.
	/// </summary>
	public class Application
	{        
        public string Executable = null;
        public string ExecutableArgs = null;
        public string ExecutablePath = null;
        public Process ProcessClass = null;
        public Monitor MonitorClass = null;

        /// <summary>
        /// Constructor to create an empty Application class that can later be filled in.
        /// </summary>
		public Application()
		{
            ProcessClass = new Process( );
            
		}

        /// <summary>
        /// Construct an app class using an existing process class...
        /// </summary>
        /// <param name="processClass"></param>
        public Application( Process processClass )
        {
            ProcessClass = processClass;

            Executable = ProcessClass.StartInfo.FileName;
            ExecutableArgs = ProcessClass.StartInfo.Arguments;

            if ( ProcessClass.StartInfo.WorkingDirectory != "" )
                ExecutablePath = ProcessClass.StartInfo.WorkingDirectory;
            else
                ExecutablePath = System.IO.Path.GetDirectoryName( ProcessClass.StartInfo.FileName );
        }

        /// <summary>
        /// Constructor for the case where only the executable is necessary.
        /// </summary>
        /// <param name="Executable"></param>
        public Application( string executable )
        {
            ProcessClass = new Process( );

            Executable = executable;

            ProcessClass.StartInfo.FileName = Executable;
            ProcessClass.StartInfo.RedirectStandardError = true;
            ProcessClass.StartInfo.UseShellExecute = false;
        }

        /// <summary>
        /// Constructor for the case where executable should be used with command line args.
        /// </summary>
        /// <param name="Executable"></param>
        /// <param name="ExecutableArgs"></param>
        public Application( string executable, string executableArgs )
        {
            ProcessClass = new Process( );

            Executable = executable;
            ExecutableArgs = executableArgs;

            ProcessClass.StartInfo.FileName = Executable;
            ProcessClass.StartInfo.Arguments = ExecutableArgs;
            ProcessClass.StartInfo.RedirectStandardError = true;
        }

        /// <summary>
        /// Constructor for the case where a path and command line arguments are needed for the 
        /// executable.
        /// </summary>
        /// <param name="Executable"></param>
        /// <param name="ExecutableArgs"></param>
        /// <param name="ExecutablePath"></param>
        public Application( string executable, string executableArgs, string executablePath )
        {
            ProcessClass = new Process( );

            Executable = executable;
            ExecutableArgs = executableArgs;
            ExecutablePath = executablePath;

            ProcessClass.StartInfo.FileName = Executable;
            ProcessClass.StartInfo.Arguments = ExecutableArgs;
            ProcessClass.StartInfo.WorkingDirectory = ExecutablePath;
            ProcessClass.StartInfo.RedirectStandardError = true;
            
        }

        /// <summary>
        /// Starts the AUT.
        /// </summary>
        public void StartApplication( )
        {
            // Start the process specified with the constructor - if not an exception is thrown...
            ProcessClass.Start( );
        }

        /// <summary>
        /// Attaches to an already running AUT. If multiple AUTs by the same name are running, only
        /// the first application retrieved is used to perform the attachment. Use the overloaded
        /// process ID method to be more specific.
        /// </summary>
        public void AttachToApplication( string processName )
        {
            ProcessClass = Process.GetProcessesByName( processName )[0];

            Executable = ProcessClass.StartInfo.FileName;
            ExecutableArgs = ProcessClass.StartInfo.Arguments;            
            ExecutablePath = ProcessClass.StartInfo.WorkingDirectory;
            ProcessClass.StartInfo.RedirectStandardError = true;
        }

        /// <summary>
        /// Attaches to an already running AUT uniquely using the process ID of the AUT.
        /// </summary>
        /// <param name="processID"></param>
        public Application( int processID )
        {
            ProcessClass = Process.GetProcessById( processID );

            Executable = ProcessClass.StartInfo.FileName;
            ExecutableArgs = ProcessClass.StartInfo.Arguments;
            ExecutablePath = ProcessClass.StartInfo.WorkingDirectory;
        }

        /// <summary>
        /// Starts monitoring the application started or attached to...
        /// </summary>
        public void StartMonitoring( )
        {
            MonitorClass = new Monitor( this );

            MonitorClass.Start( );
        }
	}
}
