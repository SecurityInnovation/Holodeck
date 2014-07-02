using System;

namespace Holodeck
{
	/// <summary>
	/// Summary description for NewProjectWizard.
	/// </summary>
	public class NewProjectWizard:Base
	{
		private static bool newVersion = true;//is this the new version of Holodeck or not (2.5.77+)
		/// <summary>
		/// NativeFunctions is used to select or deselect the the native functions
		/// Depending on the passed parameters this function will select All,No or some Native Functions
		/// </summary>
		public static void NativeFunctions(string nativeFunctions)
		{
			if (nativeFunctions == "No")
			{
				if (newVersion) 
				{
					//new version of holodeck
					SendKeyStrokes ("{TAB}"); 
					SendKeyStrokes ("{TAB}"); 
				}

				SendKeyStrokes ("{TAB}"); // Tab
				SendKeyStrokes (" "); // Space
			}
			else if (nativeFunctions == "All")
			{
				if (newVersion) 
				{
					//new version of holodeck
					SendKeyStrokes ("{TAB}");
					SendKeyStrokes ("{TAB}");
				}

				SendKeyStrokes ("{TAB}"); // Tab
				SendKeyStrokes (" "); // Space
				SendKeyStrokes (" "); // Space		
			}
			else
			{
				if (newVersion) 
				{
					//new version of holodeck
					SendKeyStrokes ("{TAB}");
					SendKeyStrokes ("{TAB}");
				}

				SendKeyStrokes ("{TAB}"); // Tab
				SendKeyStrokes ("(DOWN)"); // Down
				SendKeyStrokes ("(DOWN)"); // Down
				SendKeyStrokes (" "); // Space
				SendKeyStrokes ("(DOWN)"); // Down
				SendKeyStrokes ("(DOWN)"); // Down
				SendKeyStrokes (" "); // Space		
			}	
		}

		/// <summary>
		/// NetFunctions is used to select or deselect the the .NET functions
		/// Depending on the passed parameters this function will select All,No or some .NET Functions
		/// </summary>
		public static void NetFunctions(string netFunctions)
		{
			if (netFunctions == "No")
			{
				//Do nothing.By default its not checked.
			}
			else if (netFunctions == "All")
			{
				if (newVersion) 
				{
					//new version of holdoeck
					for (int tLoop = 0; tLoop < 3; tLoop++) 
					{
						//3 tabs
						SendKeyStrokes("{TAB}");
					}

					for (int dLoop = 0; dLoop < 9; dLoop++) 
					{
						//9 downs
						SendKeyStrokes("{DOWN}");
					}

					SendKeyStrokes(" "); //space
				}

				else 
				{
					//older version of holodeck
					SendKeyStrokes ("{TAB}"); // Tab
					SendKeyStrokes ("{LEFT}"); // Right Arrow
					SendKeyStrokes ("{DOWN}"); // Down Arrow
					SendKeyStrokes (" "); // Space
				}
			}
			else
			{
				if (newVersion) 
				{
					//new version of holodeck
					SendKeyStrokes ("{TAB}");
					SendKeyStrokes ("{TAB}");
				}

				SendKeyStrokes ("{TAB}"); // Tab
				SendKeyStrokes ("{RIGHT}"); // Right arrow
				SendKeyStrokes ("{DOWN}"); // Down Arrow
				SendKeyStrokes ("{LEFT}"); // Left arrow
				SendKeyStrokes ("{DOWN}"); // Down Arrow
				SendKeyStrokes (" "); // Space
				SendKeyStrokes ("{DOWN}"); // Down Arrow
				SendKeyStrokes (" "); // Space
				SendKeyStrokes ("{DOWN}"); // Down Arrow
				SendKeyStrokes ("{DOWN}"); // Down Arrow
                SendKeyStrokes (" ");
			}
            //SendKeyStrokes ("{ENTER}"); // Finish Button         
		}

        public static void ExitHolodeckOnErrorDialog( )
        {
            SendKeyStrokes ( "{ENTER}" );   // OK
            SendKeyStrokes ( "%C" );        // CANCEL
            SendKeyStrokes ( "%F" );        // FILE->
            SendKeyStrokes ( "X" );         // E(x)it
        }

        public static void ExitHolodeckWhenRunningNormally( )
        {
            // Make the Holodeck window the foreground window (since we may have switched to the AUT)...
            Holodeck.Base.SetForegroundWindow( Holodeck.Base.GetMainWindowHandle( ) );

            // Wait 1 second for Holodeck to come to the front...
            System.Threading.Thread.Sleep( 1000 );

            SendKeyStrokes ( "%{TAB}" );
            /*
            SendKeyStrokes ( "%F" );        // FILE->
            SendKeyStrokes ( "X" );         // E(x)it
            */
        }
	}
}
