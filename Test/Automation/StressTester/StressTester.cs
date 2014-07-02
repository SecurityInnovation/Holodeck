using System;
using System.Diagnostics;
using ExcelDocumentCreator;
using Holodeck;
using System.IO;
using Microsoft.Win32;

/// <summary>
/// Main stress tester class...
/// </summary>

namespace StressTester
{
    public class StressTester
    {        
        // Defines the file that stores the names of the monitor log files... this is used by the excel
        // assembly to build the excel spreadsheet and charts...
        // NOTE: If you change this string, you must also change it in the excel document creator code.
        public static string MonitorClassFile = "MONLIST.DAT";

        public static string ExcelDocument = @"..\..\ExcelDocumentCreator\TestResults.xls";

        /// <summary>
        /// Start Holodeck by passing in the executable it is supposed to run...
        /// </summary>
        /// <param name="Executable"></param>
        public static int startHolodeck( string Executable )
        {
            HolodeckProcess.Start(100);
             
            HolodeckPane.Reset ();
            HolodeckPane.File_NewProject ();
            NewProjectPane1.Reset ();
            NewProjectPane1.SetProjectLocation ("C:\\Test.hdp");
            NewProjectPane1.Next ();
            NewProjectPane2.Reset ();
            NewProjectPane2.SetApplicationName ( Executable );
            NewProjectPane2.Next ();
            NewProjectPane3.Reset ();
            NewProjectPane3.SetPauseApplication (true);             
            NewProjectPane3.Finish ();            

            return (int)Holodeck.HolodeckProcess.GetProcessID( );
        }

        public static void resumeAUT( )
        {
            Holodeck.HolodeckPane.Application_Resume( );
        }

        public static void buildMonitorFileList( string FileList, Application holodeckApp, Application autApp )
        {
            StreamWriter fileListWriter = new StreamWriter( FileList );

            fileListWriter.WriteLine( Directory.GetCurrentDirectory( ) + "\\" + holodeckApp.MonitorClass.logLocation );
            fileListWriter.WriteLine( Directory.GetCurrentDirectory( ) + "\\" + autApp.MonitorClass.logLocation );

            fileListWriter.Close( );
        }

        public static void moveMonitorFileListEntryUp( string FileList )
        {
            StreamReader fileListReader = new StreamReader( FileList );

            // Read the first line...
            fileListReader.ReadLine( );

            // Read the new first line...
            string newFirstLine = fileListReader.ReadLine( );

            fileListReader.Close( );

            // Write the new first line to the file...
            StreamWriter fileListWriter = new StreamWriter( FileList, false );

            fileListWriter.WriteLine( newFirstLine );

            fileListWriter.Close( );

        }

        public static void openExcelDocument( string Document, Application app )
        {
            Process process = new Process( );
            process.StartInfo.FileName = Document;            

            // Filename cannot have been saved yet, because Excel wasn't started yet...
            DateTime now = DateTime.Now;

            process.Start( );                        

            // Excel generated file...
            string excelFile = Directory.GetCurrentDirectory( ) + "\\" + app.MonitorClass.logLocation + ".xls";

            // If file exists, wait for it to be saved with a name newer than it is now...
            if ( File.Exists( excelFile ) )
            {

                while ( File.GetLastAccessTime( excelFile ) < now )
                {
                    // sleep while the file has not been modified...
                    System.Threading.Thread.Sleep( 1000 );
                }
            }
            else
            {
                // If the file doesn't exist, wait for it to exist...
                while ( !File.Exists( excelFile ) )
                    System.Threading.Thread.Sleep( 1000 );

            }

            // Once the file is modified... We need to shut down excel...
            System.Threading.Thread.Sleep( 2000 ); // wait 2 secs...

            // Kill the process...
            process.Kill( );
        }

        public static void setApplicationLocationForExcel( )
        {
            // Open the localmachine hive...
            RegistryKey localMachine = RegistryKey.OpenRemoteBaseKey( RegistryHive.LocalMachine, System.Environment.MachineName );            

            RegistryKey softwareKey = localMachine.OpenSubKey( "SOFTWARE", true );

            RegistryKey HDStressKey = softwareKey.CreateSubKey( "HDStress" );

            HDStressKey.SetValue( "FileLocation", System.IO.Directory.GetCurrentDirectory( ) );
        }

        public static void startApplicationTest( string ApplicationExecutable, AppAutomationBase automationClass )
        {
            setApplicationLocationForExcel( );

            Application holodeckApplication;
            Application AUTApplication;

            TestFramework.Log.Log.printSuccess( "STARTING HOLODECK with AUT." );

            // Start Notepad in Holodeck monitoring it...
            holodeckApplication = new Application( startHolodeck ( ApplicationExecutable ) );

            TestFramework.Log.Log.printSuccess( "STARTING Monitor Thread for Holodeck." );

            // Start Monitoring the Holodeck Process...
            holodeckApplication.StartMonitoring( );
                        
            TestFramework.Log.Log.printSuccess( "FINDING the AUT Process", ApplicationExecutable );

            // Get information about the AUT that Holodeck started... 
            ProcessTools.ProcessTools tools = new ProcessTools.ProcessTools( );			
            
            // Instantiate the application class based off of the child process ID created by Holodeck...
            AUTApplication = new Application( tools.GetChildProcess( holodeckApplication.ProcessClass.Id ) );
            
            TestFramework.Log.Log.printSuccess( "STARTING Monitor Thread for AUT." );

            // Start monitoring the AUT...
            AUTApplication.StartMonitoring( );                      

            TestFramework.Log.Log.printSuccess( "RESUMING the AUT Process." );

            // Resume the AUT...
            resumeAUT( );
            
            TestFramework.Log.Log.printSuccess( "PERFORMING AUT Automation." );

            automationClass.performAUTAutomation( AUTApplication.ProcessClass );

            TestFramework.Log.Log.printSuccess( "SHUTTING DOWN Holodeck and AUT." );

            // Shutdown Holodeck and the AUT...
            holodeckApplication.ProcessClass.Kill( );
            AUTApplication.ProcessClass.Kill( );

            TestFramework.Log.Log.printSuccess( "BUILDING Monitor File List", System.IO.Directory.GetCurrentDirectory( ) + "\\" + MonitorClassFile );

            // We now need to build a file containing a list of the locations where the monitored log files are located...
            buildMonitorFileList( MonitorClassFile, holodeckApplication, AUTApplication );
            
            // We must wait for the threads writing to the files to stop before continuing...
            holodeckApplication.MonitorClass.Stop( );

            TestFramework.Log.Log.printSuccess( "CREATING Excel SpreadSheet for Holodeck", System.IO.Directory.GetCurrentDirectory( ) + "\\" + holodeckApplication.MonitorClass.logLocation + ".xls" );

            // Open the excel document using the monitor file list...
            openExcelDocument( ExcelDocument, holodeckApplication );

            // Remove the topmost file from the file list...
            moveMonitorFileListEntryUp( MonitorClassFile );

            // Again we must wait for the threads to stop writing to files...
            AUTApplication.MonitorClass.Stop( );

            TestFramework.Log.Log.printSuccess( "CREATING Excel SpreadSheet for AUT", System.IO.Directory.GetCurrentDirectory( ) + "\\" + AUTApplication.MonitorClass.logLocation + ".xls" );

            // Open the excel document again using the monitor file list...
            openExcelDocument( ExcelDocument, AUTApplication );

            // A document should have been generated in the local build folder... This document is an excel spreadsheet,
            // which when opened will trigger code to execute.        
        }

        /*
        public static int Main ( string [] args )
        {            
            startApplicationTest( "C:\\Windows\\Notepad.exe" );


            return 0;
        }        */    }}