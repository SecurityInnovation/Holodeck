using System;
using System.Reflection;
using TestFramework;
using TestFramework.Exceptions;
using TestFramework.Run;
using TestFramework.Log;

// NOTE: Always make sure you use the right namespace.
namespace ResourcesPaneTest
{
	public class CannotGenerateTestApp : Exception {
		public CannotGenerateTestApp (string message) : base (message) {
		}
	}

    /// <summary>
    /// This is the actual code for the test which inherits from the test framework...
    /// </summary>
    public class ResourcesPaneTest : RunTest
    {
        // Define the prefix for the name of the class we will load.
        public static string classNamePrefix        = "ResourcesPaneTest.ResourcesPaneTest";

		protected string compareSwitch = string.Empty;

		public override void performPreRunTasks() {
			Holodeck.Base.LoadPaneSettings ("..\\..\\PaneSettingsKey.xml");

			Holodeck.HolodeckPane.Reset ();
			Holodeck.HolodeckPane.HidePane (Holodeck.HolodeckPane.RemovablePane.DynamicHelpPane);
			Holodeck.HolodeckPane.HidePane (Holodeck.HolodeckPane.RemovablePane.LimitsPane);
			// the 0th element in the pane ordering is always the MainPane
			Holodeck.HolodeckPane.ReorderPane (Holodeck.HolodeckPane.RemovablePane.ResourcesPane, 1);
			Holodeck.HolodeckPane.ReorderPane (Holodeck.HolodeckPane.RemovablePane.PropertiesPane, 2);
			Holodeck.HolodeckPane.ReorderPane (Holodeck.HolodeckPane.RemovablePane.ProjectPane, 3);
		}

		protected virtual void ProcessTestSpecificArguments () {
			foreach (string s in testSpecificParameters) {
				switch (s) {
					case "log": {
						compareSwitch = s;
						break;
					}
					case "known": {
						compareSwitch = s;
						break;
					}
					default: {
						throw new ArgumentException ("ResourcePaneTest error: custom option '" + s + "' is unknown.");
					}
				}
			}
		}

		protected virtual Holodeck.Resource[] ReadResources (System.IO.StreamReader file) {
			const string Name = "Name: ";
			const string Func = "Func: ";
			const string Proc = "Proc: ";
			const string Time = "Time: ";
			const string Retv = "Retv: ";
			const string Errc = "Errc: ";
			const string Exce = "Exce: ";

			Holodeck.Resource resource;
			System.Collections.Hashtable resources = new System.Collections.Hashtable (10);
			string stream = file.ReadToEnd ();
			try {
				int begIndex = 0;
				int endIndex = 0;
				while (true) {

					begIndex = stream.IndexOf (Name, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Name.Length;
					resource.Name = stream.Substring (begIndex, endIndex - begIndex - 1);

					begIndex = stream.IndexOf (Func, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Func.Length;
					resource.LastFunction = stream.Substring (begIndex, endIndex - begIndex - 1);

					begIndex = stream.IndexOf (Proc, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Proc.Length;
					resource.processID = Int64.Parse (stream.Substring (begIndex, endIndex - begIndex - 1));

					begIndex = stream.IndexOf (Time, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Time.Length;
					resource.TimeStamp = stream.Substring (begIndex, endIndex - begIndex - 1);

					begIndex = stream.IndexOf (Retv, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Retv.Length;
					resource.ReturnValue = stream.Substring (begIndex, endIndex - begIndex - 1);

					begIndex = stream.IndexOf (Errc, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Errc.Length;
					resource.ErrorCode = stream.Substring (begIndex, endIndex - begIndex - 1);

					begIndex = stream.IndexOf (Exce, endIndex);
					endIndex = stream.IndexOf ("\n", begIndex);
					begIndex += Exce.Length;
//					resource. = stream.Substring (begIndex, endIndex - begIndex - 1);


					resource.NumHits = 1;
					resource.threadID = 0;

					if (resources.Contains (resource.Name)) {
						Holodeck.Resource oldResource = (Holodeck.Resource) resources[resource.Name];
						resource.NumHits += oldResource.NumHits;
						resources.Remove (resource.Name);
					}
					resources.Add (resource.Name, resource);
				}
			} catch (Exception) {
			}

			Holodeck.Resource[] result = new Holodeck.Resource[resources.Count];
			System.Collections.IEnumerator iEnum = resources.Values.GetEnumerator ();
			int i = 0;
			while (iEnum.MoveNext ()) {
				result[i] = (Holodeck.Resource) iEnum.Current;
				i++;
			}

			return result;
		}
		
		protected virtual string GenerateTestApplication (string testID) {
			throw new CannotGenerateTestApp ("No default definition of GenerateTestapplication() exists.");
		}

		protected virtual string GetApplicationPath (string testID) {
			throw new CannotGenerateTestApp ("No default definition of GetApplicationPath() exists.");
		}
			
		public override object getTestClass( string testID )
        {
            try
            {
                Assembly assembly = Assembly.GetExecutingAssembly( );
            
                // get the full name of the type while ignoring case...
                string typeName = assembly.GetType( classNamePrefix + testID, true, true ).FullName;
                
                Log.printSuccess( "INSTANTIATING CLASS", typeName );

                // Get an instance of the class...
                return ( (RunTest)assembly.CreateInstance( typeName ) );
            }
            catch ( Exception )
            {
                // throw an exception stating that no classes were found to match the specified
                // test ID...
                throw new NoTestClassException( );
            }
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static int Main(string[] args)
        {
            try
            {
                ResourcesPaneTest resourcesPaneTest = new ResourcesPaneTest( );
            
                // parse any command line arguments...
                resourcesPaneTest.parseCommandLineArgs( args );

                if ( ! RunTest.printingUsageInfo )
                    Log.printTestStatus( "==> TEST PASSED! <==" );

                return 0;
            }
            catch ( TestExceptions )
            {
                // Exceptions that have occurred have been rethrown, but
                // they were already printed out, so we just catch it here.
                // if any exception occurs, it is thrown again...
                // this prevents us from causing an unhandled exception...
            }
            catch ( Exception e )
            {
                // Catch ANY other exception and print out an error.
                Log.printError( "\nMessage:\n\n" + e.Message + "\n\nStack Trace:\n\n" + e.StackTrace );
            }
            finally
            {
                // Close the logfile if it is open...
                if ( Log.logWriter != null )
                    Log.logWriter.Close( );
            }

            if ( ! RunTest.printingUsageInfo )
                Log.printTestStatus( "==> TEST FAILED! <==" );

            // some exception occurred...
            return 1;
        }
    }
}
