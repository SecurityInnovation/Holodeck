using System;

namespace TestApplication5
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Test
	{
		protected const int	sleepTime = 50;

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			System.Threading.Thread.Sleep (sleepTime);
			Console.WriteLine ("Hello World");
			System.Threading.Thread.Sleep (sleepTime);
		}
	}
}
