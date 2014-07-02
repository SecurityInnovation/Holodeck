using System;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;
using System.Diagnostics;
using System.Collections;

/// <summary>
/// Defines the test cases for the stress tester...
/// </summary>
namespace StressTester
{
    /// <summary>
    /// BVT: Test using NOTEPAD with no automation, simply a timer.
    /// </summary>
    public class StressTesterExecBVT : StressTesterExec
    {   
        /// <summary>
        /// This class contains the details of what to do when automating the application.
        /// The "processClass" is provided so that you can get the mainwindow handle and perform
        /// UI manipulation.
        /// </summary>
        public class NotepadAutomation : AppAutomationBase
        {
            public override void performAUTAutomation(System.Diagnostics.Process processClass)
            {
                // Sleep five seconds (demo)...
                System.Threading.Thread.Sleep( 20000 );
            }
        }

        public override void executeTest( )
        {
            // Start the application with the overloaded inherited version of the main base automation
            // class...
            StressTester.startApplicationTest( System.Environment.SystemDirectory + "\\Notepad.exe", new NotepadAutomation( ) );
        }
    }

    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    /// <summary>
    /// Test case 1: Automate Microsoft Word operations for a period of time.
    /// </summary>
    public class StressTesterExec1 : StressTesterExec
    {
        public static string OfficeLocation = @"C:\Program Files\Microsoft Office\OFFICE11";

        public class WordAutomation : AppAutomationBase
        {            
            public override void performAUTAutomation(System.Diagnostics.Process processClass)
            {                              
                TestFramework.Log.Log.printSuccess( "ATTACHING TO PROCESS:", processClass.Id.ToString( ) );

                System.Threading.Thread.Sleep( 500 );

                // Set Word to be the foreground window...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle );

                SendKeyStrokes( "The quick brown fox jumped over the lazy dog.", processClass );
                                
                SendKeyStrokes( "{HOME}", processClass );     // move cursor to start of line.
                SendKeyStrokes( "+{END}", processClass );     // select all the text.
                SendKeyStrokes( "^C", processClass );         // copy the text.

                // Paste the text 200 times.
                for ( int count = 0; count < 200; count++ )
                {
                    SendKeyStrokes( "^V", processClass );
                }                

                // We must wait for input idle, otherwise word will close instantly...
                processClass.WaitForInputIdle( );
                                
                // Sleep a few seconds just to make sure things have some buffer...
                System.Threading.Thread.Sleep( 3000 );
            }
        }

        public override void executeTest()
        {
            // Word is tricky... If an existing word is running, the new process gets transferred
            // into that process (so only one word is running). So, we must close any existing Word
            // applications running on the machine...
            TestFramework.Log.Log.printSuccess( "KILLING Existing MS Word Processes." );
            KillAll( "WINWORD" );

            StressTester.startApplicationTest( OfficeLocation + "\\WINWORD.EXE", new WordAutomation( ) );
        }
    }

    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    /// <summary>
    /// Test case 2: Automate Microsoft IE operations for a period of time.
    /// </summary>
    public class StressTesterExec2 : StressTesterExec
    {
        public static string IELocation = @"C:\Program Files\Internet Explorer";
        public static TimeSpan RunTime = TimeSpan.FromHours( 3 );
        public static ArrayList WebSiteArray = new ArrayList( );

        public class IEAutomation : AppAutomationBase
        {      
            public override void performAUTAutomation(System.Diagnostics.Process processClass)
            {
                // Wait 10 seconds for IE to load and be ready...
                System.Threading.Thread.Sleep( 10000 );

                // Set Word to be the foreground window...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle );                

                // Get the current date and time...
                DateTime now = DateTime.Now;

                for (;;)
                {
                    // If we have run as long as we need to, then quit the automation...
                    if ( ( now + RunTime ) < DateTime.Now )
                        break;

                    // Go through all the web sites in the web site array...
                    foreach ( string webSite in WebSiteArray )
                    {
                        SendKeyStrokes( "%D", processClass );     // Gets to the address bar...
                        SendKeyStrokes( webSite, processClass );
                        SendKeyStrokes( "{ENTER}", processClass );

                        // Give it 5 seconds to go to that website...
                        System.Threading.Thread.Sleep( TimeSpan.FromSeconds( 5 ) );
                    }
                }            
            }
        }

        public override void executeTest()
        {            
            // Build a web site list...
            WebSiteArray.Add( "http://www.yahoo.com" );
            WebSiteArray.Add( "http://www.microsoft.com" );
            WebSiteArray.Add( "http://www.fit.edu" );
            WebSiteArray.Add( "http://badwebsite" );
            WebSiteArray.Add( "http://www.sisecure.com" );

            StressTester.startApplicationTest( IELocation + "\\IEXPLORE.EXE", new IEAutomation( ) );
        }
    }

    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    /// <summary>
    /// Test case 3: Automate Microsoft Windows Media Player.
    /// NOTE: Before this test can be run, Windows Media Player must first be run standalone to make sure
    /// the initial wizard settings are set (otherwise the initial wizard will appear for the first time
    /// preventing this test from running properly).
    /// </summary>
    public class StressTesterExec3 : StressTesterExec
    {
        public static string MediaPlayerLocation = @"C:\Program Files\Windows Media Player";
        public static string MediaFile = System.IO.Directory.GetCurrentDirectory( ) + "\\..\\..\\clip128web.wmv";
        public static TimeSpan RunTime = TimeSpan.FromHours( 3 );     

        public class MediaPlayerAutomation : AppAutomationBase
        {      
            public override void performAUTAutomation(System.Diagnostics.Process processClass)
            {
                // Wait for Windows Media Player to load...
                System.Threading.Thread.Sleep( 20000 );

                // Set WMP to be the foreground window...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle );                

                // Wait 10 seconds for Media Player to refresh and be ready...
                System.Threading.Thread.Sleep( 10000 );
                
                // Get the current date and time...
                DateTime now = DateTime.Now;

                // First thing we do is open a media file...
                SendKeyStrokes( "^M", processClass );       // Display Menu Bar (if not already shown)...
                System.Threading.Thread.Sleep( 200 );
                // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                SendKeyStrokes( "%F", processClass );       // Open File Menu...
                // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                SendKeyStrokes( "O", processClass );        // Get Open File Dialog...
                ////// UNSAFE //// Holodeck COULD grab here.
                SendKeyStrokes( MediaFile, processClass );  // Type the file location...
                SendKeyStrokes( "{ENTER}", processClass );  // Press Enter...
                /////////////////
                System.Threading.Thread.Sleep( 200 );
                // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                SendKeyStrokes( "^P", processClass );       // Start Playing the file...                    
                System.Threading.Thread.Sleep( 200 );
                // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                SendKeyStrokes( "^T", processClass );       // Set to repeat...                    

                for (;;)
                {
                    // If we have run as long as we need to, then quit the automation...
                    if ( ( now + RunTime ) < DateTime.Now )
                        break;

                    // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                    Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                                                           
                    System.Threading.Thread.Sleep( 200 );
                    // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                    Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                    SendKeyStrokes( "%1", processClass );       // Zoom at 50%...
                    System.Threading.Thread.Sleep( 200 );
                    // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                    Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                    SendKeyStrokes( "%2", processClass );       // Zoom at 100%...
                    System.Threading.Thread.Sleep( 200 );
                    // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                    Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                    SendKeyStrokes( "%3", processClass );       // Zoom at 200%...            
                    System.Threading.Thread.Sleep( 200 );
                    // Set WMP to be the foreground window (because Holodeck grabs input for some reason)...
                    Holodeck.Base.SetForegroundWindow( processClass.MainWindowHandle ); 
                    SendKeyStrokes( "^1", processClass );       // Switch to full mode...
                    System.Threading.Thread.Sleep( 200 );
                    
                    // Now we start over again, reopening the file, etc...
                }            

                TestFramework.Log.Log.printSuccess( "FINISHED AUT Automation" );
            }
        }

        public override void executeTest()
        {                       
            TestFramework.Log.Log.printSuccess( @"NOTE: Before this test can be run, Windows Media Player must first be run standalone to make sure the initial wizard settings are set (otherwise the initial wizard will appear for the first time preventing this test from running properly)." );

            StressTester.startApplicationTest( MediaPlayerLocation + "\\WMPLAYER.EXE", new MediaPlayerAutomation( ) );
        }
    }

    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    /// <summary>
    /// Test case 4: Automate Quake III .NET Edition for a period of time.
    /// </summary>
    public class StressTesterExec4 : StressTesterExec
    {
        public static string QuakeLocation = System.IO.Directory.GetCurrentDirectory( ) + "\\..\\..\\QuakeII.NET\\managed";
        public static TimeSpan RunTime = TimeSpan.FromHours( 3 );

        public class QuakeAutomation : AppAutomationBase
        {      
            public override void performAUTAutomation(System.Diagnostics.Process processClass)
            {
                // All we have to do is sleep and let the app run since it will show in demo mode...
                System.Threading.Thread.Sleep( RunTime );
            }
        }

        public override void executeTest()
        {            
            StressTester.startApplicationTest( QuakeLocation + "\\QUAKE2.EXE", new QuakeAutomation( ) );
        }
    }
}