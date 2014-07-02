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
	/// This test tests whether Disk Limit Accepts Negative Number    
	/// </summary>
	public class Limittest2 : LimitTest
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
			Holodeck.LimitsPane.SetDiskUnits (Holodeck.LimitsPane.Unit.Byte);
			Holodeck.LimitsPane.SetDiskLimit(-5); // Set Disk Limit to -ve value
			System.Threading.Thread.Sleep (1000);


			/*The below code can be safely deleted
			 * 
			 * SendKeys.SendWait("{TAB}"); // Tab disk slide
			System.Threading.Thread.Sleep (1000);
			SendKeys.SendWait("{TAB}"); // Tab disk used
			System.Threading.Thread.Sleep (1000);
			SendKeys.SendWait("{TAB}"); // Tab disk limit	
			System.Threading.Thread.Sleep (1000);

	
			SendKeys.SendWait("{TAB}"); // Tab disk units
			System.Threading.Thread.Sleep (1000);
			SendKeys.SendWait("{UP}"); // Set to Bytes
			SendKeys.SendWait("{ENTER}");
			System.Threading.Thread.Sleep (1000);
			SendKeys.SendWait("+{TAB}");
			Holodeck.LimitsPane.SetDiskLimit(-10); // Set Disk Limit
			System.Threading.Thread.Sleep (1000);
			SendKeys.SendWait("{TAB}");
			System.Threading.Thread.Sleep (1000); */

			
    		//exiting running Holodeck by killing its process
			/*if(Holodeck.HolodeckProcess.IsRunning())
			{
				Holodeck.HolodeckProcess.Stop();

			}*/
		}
	}
}
