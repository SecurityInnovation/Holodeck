using System;
using System.IO;
using System.Web;
using System.Text;
using System.Web.Mail;
using System.Threading;
using System.Collections;
using System.Diagnostics;
using System.Windows.Forms;

namespace CSIBuilder
{
	public delegate void LogDataWrittenDelegate (String text);

	public class FindReplaceInfo
	{
		public string findString;
		public string replaceString;

		public FindReplaceInfo()
		{
			findString = "";
			replaceString = "";
		}

		public FindReplaceInfo (string fs, string rs)
		{
			findString = fs;
			replaceString = rs;
		}
	};

	public class SIBuilderInfo
	{
		public ListView.ListViewItemCollection Projects;
		public string ProjectName;
		public string OutputDir;
		public string LogDir;
		public string PackageScript;
		public string PackageOutputFile;
		public string DevenvLocation;

		public int majorVersion;
		public int minorVersion;
		public int buildNumber;

		public Stream LogOutputStream;

		public SIBuilderInfo()
		{
			this.ProjectName = "";
			this.Projects = null;
			this.OutputDir = "";
			this.LogDir = "";
			this.PackageScript = "";
			this.PackageOutputFile = "";
			this.DevenvLocation = "";
			this.majorVersion = 0;
			this.minorVersion = 0;
			this.buildNumber = 0;

			this.LogOutputStream = null;
		}
	}

	public struct MailInfo
	{
		public string MailRecipients;
		public string MailSender;
		public string SmtpServer;
	}

	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class Builder
	{
		private SIBuilderInfo sibInfo;
		private MailInfo miInfo;
		private ListView.ListViewItemCollection SourceControlDetails;
		private StringBuilder LogData;
		private StreamWriter logStreamWriter;
		private Thread WorkerThread;
		
		public LogDataWrittenDelegate OnLogDataWritten;

		public bool IsBuildAborted;
		public bool IsBuilding;
		public bool IsPaused;
		
		public Builder(SIBuilderInfo sib, MailInfo mi, ListView.ListViewItemCollection scDetails)
		{
			sibInfo = sib;
			miInfo = mi;
			IsBuilding = false;
			IsPaused = false;
			IsBuildAborted = false;
			SourceControlDetails = scDetails;
		}

		
		public void Start()
		{
			ThreadStart threadStart = new ThreadStart (WorkerThreadFunction);
			WorkerThread = new Thread(threadStart);

			WorkerThread.Start();
			IsBuilding = true;
		}

		public void StartInSameThread()
		{
			IsBuilding = true;
			WorkerThreadFunction();
		}

		public void Stop()
		{
			if (WorkerThread != null && WorkerThread.IsAlive)
				WorkerThread.Abort();

			IsBuildAborted = true;
			IsBuilding = false;
		}

		public void Pause()
		{
			WorkerThread.Suspend();
			IsPaused = true;
		}

		public void Resume()
		{
			WorkerThread.Resume();
			IsPaused = false;
		}


		private void WorkerThreadFunction()
		{
			Log ("STARTED:" + DateTime.Now.ToShortDateString() + "  " + DateTime.Now.ToShortTimeString());

			Log ("GETTING LATEST FILES");
			GetLatestFiles();

			// make the full version build
			if (!CleanProjects())
				LogAndExit("Error while Cleaning Projects, please review logs.");

			if (!BuildProjects())
				LogAndExit("Error while Building Projects, please review logs.");

			if (!ModifyInstallationVersionInfo(sibInfo))
				LogAndExit("Error while Modifying Version Info, please review logs.");

			if (!BuildInstallation(sibInfo.PackageScript))
				LogAndExit("Error while Building Installation, please review logs.");

			if (!PlaceInstallation(sibInfo.PackageOutputFile, sibInfo.OutputDir))
				Log("THE BUILD COULD NOT BE COPIED TO ALL REQUEST LOCATIONS!");

			Log ("ENDED:" + DateTime.Now.ToShortDateString() + "  " + DateTime.Now.ToShortTimeString());

			SaveLogFile(sibInfo.LogDir, true);

			if (!MailResults(miInfo.MailRecipients, miInfo.MailSender, miInfo.SmtpServer, true))
				Log ("MAIL SEND FAILED: Build report was not mailed!");
		}


		private void LogAndExit(String Message)
		{
			SaveLogFile (sibInfo.LogDir, false);
			MailResults (miInfo.MailRecipients, miInfo.MailSender, miInfo.SmtpServer, false);
			this.Stop();
		}

		
		private String[] ReadBuildFile(string buildFileName)
		{
			if (!File.Exists (buildFileName))
				return null;

			ArrayList tempArray = new ArrayList();
			StreamReader reader = new StreamReader (buildFileName);
			while (reader.Peek() > -1)
				tempArray.Add (reader.ReadLine ());
			reader.Close ();
			
			String [] projectsArray = new String[tempArray.Count];
			tempArray.CopyTo (projectsArray);
			
			return projectsArray;
		}

		private void GetLatestFiles()
		{
			for (int getCounter=0; getCounter < this.SourceControlDetails.Count; getCounter++)
			{
				ListViewItem lviSCItem = SourceControlDetails[getCounter];
				if (File.Exists (lviSCItem.SubItems[0].Text))
				{
					ProcessStartInfo psi = new ProcessStartInfo();
					psi.FileName = lviSCItem.SubItems[0].Text;
					psi.Arguments = lviSCItem.SubItems[1].Text;
					Process process = Process.Start(psi);
					process.WaitForExit();
					process.Close();
				}
				else
					Log ("Pre-build event task" + getCounter.ToString() + " file not found.\r\n" + lviSCItem.SubItems[0].Text + "\r\nSkipping!");
			}
		}

		private bool CleanProjects()
		{
			Log ("CLEANING PROJECTS");
			ListViewItem lviProject;
			for (int i=0; i < sibInfo.Projects.Count; i++)
			{
				lviProject = sibInfo.Projects[i];
				Log ("Cleaning " + Path.GetFileNameWithoutExtension (lviProject.SubItems[0].Text.Trim()) + "||" + lviProject.SubItems[1].Text + ", for " + lviProject.SubItems[2].Text.ToUpper() + " configuration...");
				Log (CallDevenv(sibInfo.DevenvLocation, lviProject.SubItems[0].Text, lviProject.SubItems[1].Text, lviProject.SubItems[2].Text, "clean"));
			}
			return true;
		}
		

		private bool BuildProjects()
		{
			try
			{
				Log ("BUILDING PROJECTS");
				String logFeedback = "";

				int failed_index = 0;
				int succeeded_index = 0;
				int fail_count = 0;

				bool BuildFailed  = false;
			
				//Build the projects
				for (int i=0; i < sibInfo.Projects.Count; i++)
				{
					ListViewItem lviProject = sibInfo.Projects[i];

					//BUILD the solution\project and obtain the build log
					Log ("Building " + Path.GetFileNameWithoutExtension (lviProject.SubItems[0].Text.Trim()) + "||" + lviProject.SubItems[1].Text + ", for " + lviProject.SubItems[2].Text.ToUpper() + " configuration...");
					logFeedback = CallDevenv(sibInfo.DevenvLocation, lviProject.SubItems[0].Text, lviProject.SubItems[1].Text, lviProject.SubItems[2].Text, "build");

					if (logFeedback != null)
					{
						Log (logFeedback);

						//Check to see if there were any failures
						failed_index = logFeedback.IndexOf("failed", 0, logFeedback.Length);
						succeeded_index = logFeedback.IndexOf("succeeded,", 0, logFeedback.Length) + "succeeded,".Length;
				
						if ((failed_index != -1) && (succeeded_index != -1))
						{
							fail_count = Int32.Parse (logFeedback.Substring (succeeded_index, failed_index - succeeded_index));
							if (fail_count > 0)
							{
								BuildFailed = true;
								Log ("*** FAILED ***");
							}
						}
					}
				}

				if (BuildFailed)
				{
					Log ("There were failure(s) during the build process. Installation will not be built. Please review the build log.");
					Log ("Build Unsuccessful!");
					return false;
				}
			
				Log ("Build Successful.");
			}
			catch (Exception)
			{
			}
			
			return true;
		}


		private bool ModifyVersionNumber(SIBuilderInfo info)
		{
			string tempFileName = Path.GetFileNameWithoutExtension (this.sibInfo.PackageScript);
			string iprFile = String.Concat (Path.GetDirectoryName (this.sibInfo.PackageScript), Path.DirectorySeparatorChar.ToString(), tempFileName);
			string scriptFileName = Path.GetFileName (iprFile);

			string newVersionString = String.Concat (info.majorVersion.ToString(), ".", info.minorVersion.ToString(), ".", info.buildNumber.ToString());

			ArrayList FindReplaceList = new ArrayList();
			FindReplaceList.Add (new FindReplaceInfo ("<row><td>ProductVersion</td><td>0.0.0</td><td/></row>" , String.Concat ("<row><td>ProductVersion</td><td>", newVersionString, "</td><td/></row>")));
			FindReplaceList.Add (new FindReplaceInfo ("MajorVersion</td><td>#0", String.Concat ("MajorVersion</td><td>#", info.majorVersion.ToString().Trim())));
			FindReplaceList.Add (new FindReplaceInfo ("MinorVersion</td><td>#0", String.Concat ("MinorVersion</td><td>#", info.minorVersion.ToString().Trim())));
			FindReplaceList.Add (new FindReplaceInfo ("BuildNumber</td><td>#0", String.Concat ("BuildNumber</td><td>#", info.buildNumber.ToString().Trim())));
			FindAndReplaceInFile (iprFile, FindReplaceList);

			return true;
		}

		private bool MakeIprFileWritable(bool writable)
		{
			string tempFileName = Path.GetFileNameWithoutExtension (this.sibInfo.PackageScript);
			string iprFile = String.Concat (Path.GetDirectoryName (this.sibInfo.PackageScript), Path.DirectorySeparatorChar.ToString(), tempFileName);
			//Get file attributes
			FileAttributes fAttribs = System.IO.File.GetAttributes (iprFile);
			if (writable)
			{
				if (FileAttributes.ReadOnly.CompareTo (fAttribs & FileAttributes.ReadOnly) == 0)
				{
					//If file is read-only, remove read-only attribute
					System.IO.File.SetAttributes (iprFile, fAttribs & (~FileAttributes.ReadOnly));
				}
			}
			else
			{
				//Make the file read-only
				System.IO.File.SetAttributes (iprFile, FileAttributes.ReadOnly);
			}
			return true;
		}

		private bool ModifyInstallationVersionInfo(SIBuilderInfo info)
		{
			Log("MODIFYING VERSION INFO");

			string version = String.Concat(info.majorVersion.ToString(), ".", info.minorVersion.ToString());
			version = String.Concat(version, ".", info.buildNumber.ToString());

			Log("Making ipr file writable");
			if (!MakeIprFileWritable(true))
				return false;

			Log("Modifying Version number");
			if (!this.ModifyVersionNumber(info))
				return false;

//			Log("Copying ipr file");
//			if (!this.CopyModifiedIprFile())
//				return false;

			Log("Making ipr file read only");
			if (!MakeIprFileWritable(false))
				return false;

			return true;
		}

		private bool BuildInstallation(string BuildScriptFile)
		{
			Log ("BUILDING INSTALLATION");

			String tempfile = System.IO.Path.GetTempFileName();
			
			ProcessStartInfo psi = new ProcessStartInfo();
			psi.CreateNoWindow = false;
			psi.UseShellExecute = false;
			psi.RedirectStandardOutput = false;
			
			psi.FileName = Environment.SystemDirectory + Path.DirectorySeparatorChar + "cmd.exe";
			psi.Arguments = "/C \"" + BuildScriptFile + "\" >" + tempfile;
			psi.WorkingDirectory = Path.GetDirectoryName (BuildScriptFile);

			Process process = Process.Start (psi);
			process.WaitForExit();
			
			Log (ReadBuildLog(tempfile));

			if (process.ExitCode > 0)
			{
				process.Close();
				Log ("Packaging Failed!");
				Log ("There was a compiler or builder error in building the installation file through InstallShield, please review log.");
				return false;
			}

			process.Close();
			Log ("Packaging Successful.");
			return true;
		}
		

		private bool PlaceInstallation(string InstallationFile, string OutputDir)
		{
			ArrayList outputDirs = new ArrayList();
			int delimiterCount = 0;
			int tempCounter = 0;
			for (int searchDelimiter=0; searchDelimiter < OutputDir.Length; searchDelimiter++)
				if (OutputDir[searchDelimiter].ToString() == ";")
					delimiterCount++;
					
			if (delimiterCount == 0)
				outputDirs.Add (OutputDir.Trim());
			else
			{
				//Add an extra ";" at the end of the string
				OutputDir = String.Concat (OutputDir, ";");
				for (int x=0; x <= delimiterCount; x++)
				{
					string path = OutputDir.Substring  (tempCounter, OutputDir.IndexOf (";", tempCounter) - tempCounter);
					tempCounter += path.Length + 1; // +1 for ';' character
					if (path != "")
						outputDirs.Add (path);
				}
			}

			for (int copyCounter = 0; copyCounter < outputDirs.Count; copyCounter++)
			{
				string outputDirectory = outputDirs[copyCounter].ToString();
				Log ("COPYING INSTALLATION TO REQUESTED DIRECTORY (" + outputDirectory + ")");
				String SaveFilename = "";

				if (outputDirectory.EndsWith (Path.DirectorySeparatorChar.ToString().Trim()))
					SaveFilename = outputDirectory + GetFinalName();
				else
					SaveFilename = outputDirectory + Path.DirectorySeparatorChar + GetFinalName();

				if (!Directory.Exists (outputDirectory))
				{
					try
					{
						Log ("Output directory does not exist, creating .. ");
						Directory.CreateDirectory (outputDirectory);
					}
					catch (Exception e)
					{
						Log ("ERROR: Could not create the build output directory, Installation file will not be copied there.\nError: " + e.Message);
						return false;
					}
				}
			
	
				try
				{
					Log ("Copy installation file from " + InstallationFile + " to " + outputDirectory);
					File.Copy (InstallationFile, SaveFilename, true);
				}
				catch (Exception e)
				{
					Log ("Could not copy the installation file to " + outputDirectory + "\n\n Error: " + e.Message);
					return false;
				}
			}

			return true;
		}

		
		private void SaveLogFile(String logDir, bool IsSuccess)
		{
			if (!logDir.EndsWith (Path.DirectorySeparatorChar.ToString().Trim()))
				logDir += Path.DirectorySeparatorChar;

			String logFile = logDir + Path.GetFileNameWithoutExtension (GetFinalName());
			
			if (IsSuccess)
				logFile += ".log";
			else
				logFile += "_FAILED.log";
				
			StreamWriter strmLog = new StreamWriter (logFile);
			strmLog.Write (GetLogText());
			strmLog.Close();
		}


		private bool MailResults(String MailRecipients, String mailsender, String SmtpServer, bool IsSuccess)
		{
			MailMessage mailObj = new MailMessage(); 
			
			mailObj.From = mailsender; 
			mailObj.To = MailRecipients; 
			mailObj.Body = GetLogText(); 
			mailObj.BodyFormat = MailFormat.Text; 
			SmtpMail.SmtpServer = SmtpServer; 
			mailObj.Subject = Path.GetFileNameWithoutExtension (GetFinalName());
			
			if (IsSuccess)
				mailObj.Subject += ": BUILD SUCCESS";
			else
				mailObj.Subject += ": BUILD FAILED";
			
			try
			{
				SmtpMail.Send(mailObj); 
			}
			catch
			{
				return false;
			}

			return true;
		}


		private string CallDevenv(string devenv, string sln, string project, string config, string action)
		{
			String tempfile = System.IO.Path.GetTempFileName();
			ProcessStartInfo psi = new ProcessStartInfo();
			psi.CreateNoWindow = true;
			psi.UseShellExecute = false;
			psi.RedirectStandardOutput = true;
			
			if (project != "")
				psi.Arguments += "/out " + tempfile + " /" + action + " " + config + " \"" + sln + "\" " + "/project " + project;
			else
				psi.Arguments += "/out " + tempfile + " /" + action + " " + config + " \"" + sln + "\"";

			psi.FileName = devenv;
			psi.WorkingDirectory = Path.GetDirectoryName (devenv);

			Process process = Process.Start (psi);
			process.WaitForExit();
			process.Close();
			return ReadBuildLog(tempfile);
		}
		

		private String ReadBuildLog(string tempfile)
		{
			char [] buffer;
			String returnValue = "";
			StreamReader reader = new StreamReader (tempfile);
			buffer = new Char [reader.BaseStream.Length];
			reader.Read (buffer,0, (int) reader.BaseStream.Length);
			for (int i=0; i<buffer.Length; i++)
				if (buffer[i] != 0)
					returnValue += buffer[i].ToString();
			reader.Close ();
			
			return returnValue;
		}
		

		private String GetFinalName()
		{
			String finalName = sibInfo.ProjectName + "(";
			finalName += sibInfo.majorVersion.ToString().Trim() + "_";
			finalName += sibInfo.minorVersion.ToString().Trim() + "_";
			finalName += sibInfo.buildNumber.ToString().Trim() + ").";
			finalName += "exe";

			return finalName;
		}


		private void Log (String logText)
		{
			if (logText == null)
				return;

			if (LogData == null)
				LogData = new StringBuilder ();
				
			LogData.Append (logText + "\r\n");

			if (this.OnLogDataWritten != null)
				OnLogDataWritten(logText + "\r\n");

			if (sibInfo.LogOutputStream != null)
			{
				if (logStreamWriter == null)
					logStreamWriter = new StreamWriter (sibInfo.LogOutputStream);

				logStreamWriter.Write (logText + "\r\n");
			}
				
		}
		

		private String GetLogText()
		{
			return LogData.ToString(0, LogData.Length);
		}

		
		private void FindAndReplaceInFile(string fileName, ArrayList FindReplaceStrings)
		{
			string tempFileName = Path.GetTempFileName();
			StreamReader inFile = File.OpenText (fileName);
			StreamWriter outFile = File.CreateText (tempFileName);
			while (inFile.Peek() > -1)
			{
				string readString = inFile.ReadLine();
				for (int i=0; i < FindReplaceStrings.Count; i++)
				{
					FindReplaceInfo frInfo = FindReplaceStrings[i] as FindReplaceInfo;
					if (readString.IndexOf (frInfo.findString, 0) > 0)
						readString = readString.Replace (frInfo.findString, frInfo.replaceString);
				}
				outFile.WriteLine (readString);
			}
			inFile.Close();
			outFile.Flush();
			outFile.Close();
			
			File.Copy (tempFileName, fileName, true);
		}

		public string OutputFilePath
		{
			get
			{
				return sibInfo.PackageOutputFile;
			}
		}

		public string FinalName
		{
			get
			{
				return this.GetFinalName();
			}
		}

		public bool wasBuildAborted
		{
			get
			{
				return this.IsBuildAborted;
			}
		}
	}
}
