using System;
using System.IO;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;



namespace ResourcesPaneTest {        
	/// <summary>
	/// Basic managed file testing
	/// </summary>
	public class ResourcesPaneTest6 : ResourcesPaneTest {
		protected override string GenerateTestApplication (string testID) {
			string appPath = "";
			TestGenerator testGenerator = new TestGenerator ("ResourcePaneTestApplication" + testID);
			testGenerator.AddMainCode ("Console.WriteLine (\"Hello World\");\n");
			int compRetVal = testGenerator.GenerateTest ();
			Log.printTestStatus ("compiler returned: " + compRetVal);
			Log.printTestStatus ("compiler output:\n" + testGenerator.GetOutPutString ());
			if (compRetVal == 0) {
				appPath = testGenerator.GetAppPathString ();
			} else {
				string errors = "Couldn't generate test application...\n\n";
				errors += testGenerator.FormatErrors (testGenerator.GetErrors ());

				throw new CannotGenerateTestApp (errors);
			}

			return appPath;
		}

		public override void executeTest( ) {                              
			string applicationPath = GenerateTestApplication (TestCaseID);

			Holodeck.HolodeckProcess.Start ();

			Holodeck.HolodeckPane.Reset ();
			Holodeck.HolodeckPane.File_NewProject ();
			Holodeck.NewProjectPane1.Reset ();
			Holodeck.NewProjectPane1.SetProjectLocation ("C:\\ResourcePaneTest.hdp");
			Holodeck.NewProjectPane1.Next ();
			Holodeck.NewProjectPane2.Reset ();

			Holodeck.NewProjectPane2.SetApplicationName (applicationPath);
			Holodeck.NewProjectPane2.Next ();
			Holodeck.NewProjectPane3.Reset ();
			Holodeck.NewProjectPane3.SetPauseApplication (true);
			Holodeck.NewProjectPane3.Finish ();
			// TODO: make sure that "Project" -> "Active Log" is selected (it seems unnecessary...)
			Holodeck.HolodeckPane.Application_Resume ();
			// TODO: change this to non-architecture dependant waiting
			System.Threading.Thread.Sleep (10000);
			Holodeck.HolodeckPane.Application_Pause ();

			Holodeck.HolodeckPane.SelectResourcesPane ();
			Holodeck.ResourcesPane.Reset ();
			Holodeck.Resource[][] resources = new Holodeck.Resource[2][];
			resources[0] = Holodeck.ResourcesPane.GetFileResources ();
			resources[1] = Holodeck.ResourcesPane.GetRegistryResources ();
			Holodeck.HolodeckPane.SelectLogPane ();
			string[][] logs = Holodeck.LogPane.GetLogEntries ();
			string[] result = Helper.CompareResourcesWithLogs (resources, logs);

			if (result.Length != 0) {
				string errorMessage = "";
				foreach (string s in result) {
					errorMessage += s + "\n";
				}

				throw new Exception (errorMessage);
			}
		}
	}
}
