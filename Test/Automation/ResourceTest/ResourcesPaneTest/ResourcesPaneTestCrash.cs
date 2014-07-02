using System;
using TestFramework.Log;

namespace ResourcesPaneTest
{
	/// <summary>
	/// This test tests whether blah happens to balh...    
	/// </summary>
	public class ResourcesPaneTestCrash : ResourcesPaneTest
	{
		public override void executeTest( )
		{
			Holodeck.HolodeckProcess.Start ();

			Holodeck.HolodeckPane.Reset ();
			Holodeck.HolodeckPane.File_NewProject ();
			Holodeck.NewProjectPane1.Reset ();
			Holodeck.NewProjectPane1.SetProjectLocation ("C:\\ResourcePaneTest.hdp");
			Holodeck.NewProjectPane1.Next ();
			Holodeck.NewProjectPane2.Reset ();
			Holodeck.NewProjectPane2.SetApplicationName ("C:\\WINDOWS\\notepad.exe");
			Holodeck.NewProjectPane2.Next ();
			Holodeck.NewProjectPane3.Reset ();
			Holodeck.NewProjectPane3.Back ();
			Holodeck.NewProjectPane2.Reset ();
			Holodeck.NewProjectPane2.AttachToApplication ();
			Holodeck.NewProjectPane2.Finish ();
			System.Threading.Thread.Sleep (5000);
		}
	}
}
