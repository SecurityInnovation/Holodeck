using System;
using System.IO;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;



namespace LimitTest
{        
	/// <summary>
	/// This test tests whether blah happens to balh...    
	/// </summary>
	public class LimitTest1 : LimitTest
	{
		public override void executeTest( )
		{
			Holodeck.HolodeckProcess.Start ();

			Holodeck.HolodeckPane.File_NewProject ();
			Holodeck.NewProjectPane1.Reset ();
			Holodeck.NewProjectPane1.SetProjectLocation ("C:\\LimitTest.hdp");
			Holodeck.NewProjectPane1.Next ();
			Holodeck.NewProjectPane2.Reset ();

			string applicationToLaunchPath = System.Environment.SystemDirectory + "\\..\\notepad.exe";

			Holodeck.NewProjectPane2.SetApplicationName (applicationToLaunchPath);
			Holodeck.NewProjectPane2.Next ();
			Holodeck.NewProjectPane3.Reset ();
			Holodeck.NewProjectPane3.SetPauseApplication (true);
			Holodeck.NewProjectPane3.Finish ();

			Holodeck.HolodeckPane.SelectLimitsPane ();

			//SendKeyStrokes ("{TAB}");

			Holodeck.HolodeckPane.Application_Resume ();

			if(Holodeck.HolodeckProcess.IsRunning())
			{
				Holodeck.HolodeckProcess.Stop();

			}
		}
	}
}
