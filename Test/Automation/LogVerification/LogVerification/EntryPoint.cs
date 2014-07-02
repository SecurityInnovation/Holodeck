using System;
using System.Diagnostics;
using System.IO;
using System.Collections;
using System.Security.Permissions;
using Microsoft.Win32;


namespace LogVerification
{
	/// <summary>
	/// Summary description for EntryPoint.
	/// </summary>
	public class EntryPoint
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
            bool LogExportComplete = false;

			LogVerificationTest myLogVerificationTest = new LogVerificationTest();
			
			myLogVerificationTest.hdlPath = @"C:\Documents and Settings\vparikh\My Documents\HD Logs\testwin32_Logs\2504.hdl";
			myLogVerificationTest.ExportLogPath = @"C:\testExportHDLog.txt";
			
			LogExportComplete = myLogVerificationTest.LogExport(myLogVerificationTest.hdlPath,myLogVerificationTest.ExportLogPath);

			myLogVerificationTest.searchResult(LogExportComplete);
			myLogVerificationTest.printFailedFunction();

		}
	}
}
