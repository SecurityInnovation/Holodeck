using System;


namespace LogExportTest
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class LogExportTestClass
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			HoloScript.LogReaderV2_5 logReader = new HoloScript.LogReaderV2_5( );

			logReader.OpenLogFile( @"C:\Documents and Settings\vparikh\My Documents\HD Logs\testwin32_Logs\1016.hdl", "C:\\testExportHDLog.txt" );
		}
	}
}
