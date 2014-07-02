using System;
using System.IO;
using System.Windows.Forms;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;



namespace LimitTest
{        
	/// <summary>
	/// This test tests when the size exceeds the set size.  
	/// </summary>
	public class Limittest3 : LimitTest
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

			Holodeck.HolodeckPane.Application_Resume ();
			Holodeck.LimitsPane.SetDiskUnits (Holodeck.LimitsPane.Unit.Byte);
			Holodeck.LimitsPane.SetDiskLimit(5); // Set Disk Limits to 5 bytes

			System.Collections.Specialized.StringCollection keyList = new System.Collections.Specialized.StringCollection ();
			keyList.Add ("1234567");
			keyList.Add ("%f");
			keyList.Add ("s");
			keyList.Add ("c:\\1.txt");
			keyList.Add ("{ENTER}");
			System.Threading.Thread.Sleep (1000);
			
			Holodeck.HolodeckProcess.SendKeysToApp ("Untitled - Notepad", keyList);
			/*
			try 
			{
				keyList.Clear ();
				keyList.Add ("{ENTER}");
				Holodeck.HolodeckProcess.SendKeysToApp ("Notepad", keyList);
				
			}
			catch (Holodeck.HolodeckExceptions.CannotFindApplication e) // We couldn't find error dialog -> Holodeck didn't set the limits properly
			{
				// TODO: quit the application properly
				// TODO: exit Holodeck
				throw e;
			}*/
		}
	}	
}
