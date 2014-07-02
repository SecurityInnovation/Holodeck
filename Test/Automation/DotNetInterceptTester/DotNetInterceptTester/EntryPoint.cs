using System;

namespace DotNetInterceptTester
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			InterceptionMaintenance test = new InterceptionMaintenance();
			test.InterceptMain();
		}
	}
}
