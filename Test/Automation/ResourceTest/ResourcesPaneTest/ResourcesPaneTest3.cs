using System;
using System.IO;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;



namespace ResourcesPaneTest {        
	/// <summary>
	/// Basic native file testing
	/// </summary>
	public class ResourcesPaneTest3 : ResourcesPaneTest {
		protected override string GetApplicationPath (string testID) {
			int i = Int32.Parse (testID);
			string appPath = "..\\..\\..\\TestApplication" + i + "\\";

			appPath += "Debug";
			appPath += "\\TestApplication" + i + ".exe";

			return (new System.IO.DirectoryInfo (appPath)).FullName;
		}

		public override void executeTest( ) {                              
			string applicationPath = GetApplicationPath (TestCaseID);
			string[] result = new string[0];

			ProcessTestSpecificArguments ();


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


			switch (compareSwitch) {
				case "log": {
					Holodeck.HolodeckPane.SelectResourcesPane ();
					Holodeck.ResourcesPane.Reset ();
					Holodeck.Resource[][] resources = new Holodeck.Resource[2][];
					resources[0] = Holodeck.ResourcesPane.GetFileResources ();
					resources[1] = Holodeck.ResourcesPane.GetRegistryResources ();
					Holodeck.HolodeckPane.SelectLogPane ();
					string[][] logs = Holodeck.LogPane.GetLogEntries ();

					result = Helper.CompareResourcesWithLogs (resources, logs);
					break;
				}
				case "known": {
					Holodeck.HolodeckPane.SelectResourcesPane ();
					Holodeck.ResourcesPane.Reset ();
					string tempPath = System.Environment.GetEnvironmentVariable ("TEMP");
					if (tempPath == string.Empty) {
						tempPath = System.Environment.GetEnvironmentVariable ("TMP");
					}
					string fileResourceFileName = "resources" + TestCaseID + "file.txt";
					Holodeck.Resource[][] resources2 = new Holodeck.Resource[2][];
					System.IO.StreamReader inFile = System.IO.File.OpenText (tempPath + "\\" + fileResourceFileName);
					resources2[0] = ReadResources (inFile);
					resources2[1] = new Holodeck.Resource[0];
					inFile.Close ();
					Holodeck.Resource[][] resources = new Holodeck.Resource[2][];
					resources[0] = Holodeck.ResourcesPane.GetFileResources ();
					resources[1] = Holodeck.ResourcesPane.GetRegistryResources ();
					result = Helper.CompareResourcesWithResources (resources, resources2);
					break;
				}
				default: {
					throw new ArgumentException ("ResourcePaneTest" + TestCaseID + " error: custom option '" + compareSwitch + "' is unknown.");
				}
			}

			Holodeck.HolodeckPane.File_Exit (false);

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
