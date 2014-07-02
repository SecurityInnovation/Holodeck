using System;
using System.IO;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;


namespace ScheduleTest
{        
	public class ScheduleTestStart : ScheduleTest 
	{
		public override void executeTest( )
		{            
			HolodeckProcess.Start(100);
           	HolodeckPane.Reset ();
			HolodeckPane.File_NewProject ();
			NewProjectPane1.Reset ();
			NewProjectPane1.SetProjectLocation ("C:\\Test.hdp");
			NewProjectPane1.Next ();
			NewProjectPane2.Reset ();
			NewProjectPane2.SetApplicationName ("C:\\WINDOWS\\Notepad.exe");
			NewProjectPane2.Next ();
			NewProjectPane3.Reset ();
			NewProjectPane3.SetPauseApplication (true);             
            NewProjectPane3.Finish ();
            HolodeckPane.Application_ScheduledTest( );
            try 
			{				
				Holodeck.ScheduleTestWizard1.StartScheduleTest (functionToBeTested);
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
