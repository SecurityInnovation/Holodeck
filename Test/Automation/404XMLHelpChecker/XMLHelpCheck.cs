using System;
using System.Xml;

namespace _404XMLHelpChecker
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class XMLHelpCheck
	{	
		// Defines the location of the help links in the XML files...
		// Change this if the specification for the XML file is modified.
		static string xPathHelpLinkLocation = "Functions/Function/HelpLink";

		/// <summary>
		/// Prints usage information to the console...
		/// </summary>
		static void printUsage( )
		{
			Console.WriteLine( "<< XML Help 404 ERROR Verifier >>\n" );
			Console.WriteLine( "\tXMLHelpCheck [XMLFILE]\n" );
			Console.WriteLine( "Where XMLFILE is the name of a XML function definition file.\n" );
		}

		/// <summary>
		/// Performs the check of the specified XML file for 404 errors in the help links...
		/// </summary>
		/// <param name="fileNameAndPath"></param>
		static void perform404Check( string fileNameAndPath )
		{	
			System.Net.WebClient webClient = new System.Net.WebClient( );	
			bool bSuccess = true;

			try
			{
				XmlDocument xmlDoc = new XmlDocument( );

				xmlDoc.Load( fileNameAndPath );

				XmlNodeList helpLinkNodes = xmlDoc.SelectNodes( xPathHelpLinkLocation );
				
				// Cycle through all the help link nodes...
				foreach( XmlNode node in helpLinkNodes )
				{
					try
					{
						Console.Write( "Checking Link: \"{0}\"", node.InnerText );
						webClient.DownloadData( node.InnerText );

						Console.WriteLine( " SUCCESS!" );
						bSuccess &= true;
					}
					catch ( Exception webError )
					{						
						Console.WriteLine( " FAILURE!" );
						Console.WriteLine( "\nWeb link: \"{0}\" FAILED!\n\t{1}\n", node.InnerText, webError.Message );

						bSuccess &= false;
					}
				}
			}
			catch ( Exception e )
			{
				Console.WriteLine( "An error occurred while parsing the XML file: \n\t{0}", e.Message );
			}

			if ( bSuccess )
				Console.WriteLine( "ALL Web Links Could be Navigated Successfully!" );
			else
				Console.WriteLine( "SOME Web Links Failed to Load!" );
		}

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static int Main(string[] args)
		{
			if ( args.Length != 1 )
				printUsage( );
			else
			{
				Console.WriteLine( "<< XML Help 404 ERROR Verifier >>\n" );
				Console.WriteLine( "Checking File: {0}\n", args[0] );

				// Verify that the file exists before continuing...
				if ( !System.IO.File.Exists( args[0] ) )
				{
					Console.WriteLine( "The specified file or path does not exist!" );

					// Return an error value...
					return 1;
				}				

				perform404Check( args[0] );
			}

			// Return ERROR LEVEL for success...
			return 0;
		}
	}
}
