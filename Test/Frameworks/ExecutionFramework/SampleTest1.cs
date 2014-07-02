using System;
using System.IO;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;



namespace NewProject
{        
    /// <summary>
    /// Inherits from the test control source...
    /// </summary>
    public class NewProject1 : NewProject
    {        
        public override void executeTest( )
        {                              
            Log.printError( "This is an error" );
            Log.printSuccess( "NAME", "VALUE" );
            Log.printSuccess( "this was a action" );

            Log.printTestStatus( "Testing test class - test case" );
            // Let's make the test fail!

            try
            {
                throw new Exception( "This is an error!" );
            }
            catch ( RunTestExceptions e )
            {
                
            }

        }
    }
    
    /// <summary>
    /// This test tests whether blah happens to balh...    
    /// </summary>
    public class NewProjectNoTest : NewProject
    {        
        
    }

    public class NewProject2 : NewProject
    {
        public override void executeTest( )
        {
            Console.WriteLine( "hello world" );
        }
    }

    public class NewProjectA : NewProject
    {
        public static void runsomething( )
        {
            Console.WriteLine( "Run something" );
        }

        public override void executeTest( )
        {
            Console.WriteLine( "hello world A" );

            
            Holodeck.HolodeckProcess.Start ();
            

            Holodeck.HolodeckPane.Reset ();
            Holodeck.HolodeckPane.NewProject ();
            Holodeck.NewProjectPane1.Reset ();
            Holodeck.NewProjectPane1.SetProjectLocation ("C:\\project2.hdp");
            Holodeck.NewProjectPane1.Next ();
            Holodeck.NewProjectPane2.Reset ();
            Holodeck.NewProjectPane2.SetApplicationName ("C:\\WINDOWS\\notepad.exe");
            Holodeck.NewProjectPane2.Next ();
            Holodeck.NewProjectPane3.Reset ();
            Holodeck.NewProjectPane3.SetPauseApplication (true);
            try 
            {
                Holodeck.NewProjectPane3.Finish ();
            }
            catch (Holodeck.HolodeckExceptions.HolodeckCrashedException)
            {

                return;
            }
            catch (System.Exception)
            {

                return;
            }
        }

        
    }
}
