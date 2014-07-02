using System;

namespace HelperCodeGenerator
{
	/// <summary>
	/// Summary description for HelperCodeGenerator.
	/// </summary>
	class HelperCodeGenerator
	{
		static void Usage ()
		{
			System.Console.WriteLine ();
			System.Console.WriteLine ("Usage:");
			System.Console.WriteLine ();
			System.Console.WriteLine (" HelperCodeGenerator");
			System.Console.WriteLine ("   Takes module name (optional) with function names separated (by spaces) in console one per line.");
			System.Console.WriteLine ("   If module name is omitted the previous module name is used (the default is \"mscorlib.dll\").");
			System.Console.WriteLine ("   Reads until it encounters an empty line.");
			System.Console.WriteLine ("   Generates code as output for ResourcePaneTest Helper classes to the standard output.");
			System.Console.WriteLine ();
			System.Console.WriteLine (" HelperCodeGenerator /h");
			System.Console.WriteLine ("   Prints this help message.");
			System.Console.WriteLine ();
		}

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main (string[] args)
		{
			System.Collections.ArrayList functionNames = new System.Collections.ArrayList (10);

			if (args.Length >= 1) 
			{
				if ((args[0] == "/h") ||
					(args[0] == "-h") ||
					(args[0] == "--h")) 
				{
					Usage ();
					return;
				}
			}

			string moduleName = "mscorlib.dll";
			string functionName = null;
			string inLine = System.Console.ReadLine ();
			while ((inLine != null) &&
				   (inLine != "\n") &&
				   (inLine != ""))
			{
				inLine.Trim ();

				int index = inLine.IndexOf ("\n");
				if (index != -1) 
				{
					inLine = inLine.Substring (0, index);
				}

				string[] names = inLine.Split (' ');
				if (names.Length == 1) 
				{
					functionName = names[0];
				} 
				else if (names.Length == 2) 
				{
					moduleName = names[0];
					functionName = names[1];
				} 
				else 
				{
					System.Console.WriteLine ("/*");
					System.Console.WriteLine ("unrecognized input format: ");
					System.Console.WriteLine (inLine);
					System.Console.WriteLine ("*/");
					break;
				}

				functionNames.Add (functionName);

				System.Console.WriteLine ("else if ((logEntry[(int) LogColumns.Dll] == \"" + moduleName + "\") &&");
				System.Console.WriteLine ("\t\t (logEntry[(int) LogColumns.Function].StartsWith (\""+ functionName + "(\")))");
				System.Console.WriteLine ("{");
				System.Console.Write ("\tProcess");

				string[] functionComponents = functionName.Split ('.');
				for (int i = 0; i < functionComponents.Length; i++) 
				{
					System.Console.Write ("_" + functionComponents[i]);
				}
				System.Console.WriteLine (" (logEntry);");
				System.Console.WriteLine ("}");

				inLine = System.Console.ReadLine ();
			} // while ()

			System.Console.WriteLine ();

			foreach (string functionName2 in functionNames) 
			{
				System.Console.Write ("protected static void Process");
				string[] functionComponents = functionName2.Split ('.');
				for (int i = 0; i < functionComponents.Length; i++) 
				{
					System.Console.Write ("_" + functionComponents[i]);
				}
				System.Console.WriteLine (" (String[] logEntry)");
				System.Console.WriteLine ("{");
				System.Console.WriteLine ("}");
				System.Console.WriteLine ();
			}
		} // Main
	} // class HelperCodeGenerator
} // namespace HelperCodeGenerator
