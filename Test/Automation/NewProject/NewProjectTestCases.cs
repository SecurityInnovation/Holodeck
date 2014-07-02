using System;
using System.Xml;
using System.IO;
using Holodeck;
using TestFramework.Run;
using TestFramework.Exceptions;
using TestFramework.Utils;
using TestFramework.DependencyVerifier;
using TestFramework.Log;
using Microsoft.Win32;

namespace NewProjectTest
{
	/// <summary>
	/// Inherits from the test control source...
	/// </summary>
	
	/// <summary>
	/// Test Case 1-
	/// Parameters passed for project path and application path are not sufficient.
	/// Holodeck is expected to give message that project path is not correct.
	/// </summary>

	public class NewProjectTest1 : NewProjectTest
	{
		public override void executeTest( )
		{			
            NewProjectTest.StartHolodeckSettingProjectLocation( "C:\\Holodeck Ver 2.5\\" );

            Holodeck.NewProjectWizard.ExitHolodeckOnErrorDialog( );                        

            WaitForHolodeckToExit( "Failure! Holodeck failed to exit as expected when path is not correct." );
		}
    }

	/// <summary>
	/// Test Case 2-
	/// Parameters passed for project path and application path are not sufficient.
	/// Holodeck is expected to give message that project path is not correct.
	/// </summary>
	public class NewProjectTest2 : NewProjectTest
	{
		public override void executeTest( )
		{			
			NewProjectTest.StartHolodeckSettingProjectLocation( "g:\\Test" );

			Holodeck.NewProjectWizard.ExitHolodeckOnErrorDialog( );                        

			WaitForHolodeckToExit( "Failure! Holodeck failed to exit as expected when path is not correct." );
		}
	}

	/// <summary>
	/// Test Case 3-
	/// Parameter passed for project path is not sufficient.
	/// Holodeck is expected to give message that project path is not correct.
	/// </summary>
	public class NewProjectTest3 : NewProjectTest
	{
		public override void executeTest( )
		{			
			NewProjectTest.StartHolodeckSettingProjectLocation( "Test" );

			Holodeck.NewProjectWizard.ExitHolodeckOnErrorDialog( );                        

			WaitForHolodeckToExit( "Failure! Holodeck failed to exit as expected when path is not correct." );
		}
	}

	/// <summary>
	/// Test Case 4-
	/// Parameters passed for project path and application path are not sufficient.
	/// Holodeck is expected to give message that application path is not correct.
	/// </summary>
	public class NewProjectTest4 : NewProjectTest
	{
		public override void executeTest( )
		{			
			NewProjectTest.StartHolodeckSettingProjectLocationAppLocation( "C:\\Test.hdp","g:\\notepad.exe" );

			Holodeck.NewProjectWizard.ExitHolodeckOnErrorDialog( );                        

			WaitForHolodeckToExit( "Failure! Holodeck failed to exit as expected when path is not correct." );
		}
	}

	/// <summary>
	/// Test Case 5-
	/// Parameters passed application is not sufficient.
	/// Holodeck is expected to give message that application path is not correct.
	/// </summary>
	public class NewProjectTest5 : NewProjectTest
	{
		public override void executeTest( )
		{			
			NewProjectTest.StartHolodeckSettingProjectLocationAppLocation( "C:\\Test.hdp","z:\\notepad.exe" );

			Holodeck.NewProjectWizard.ExitHolodeckOnErrorDialog( );                        

			WaitForHolodeckToExit( "Failure! Holodeck failed to exit as expected when path is not correct." );
		}
	}

	
	
	/// <summary>
	/// Test Case 6-
	/// Parameters passed for project path and application path are correct.
	/// Applicaton path is passed as "c:\\windows\\..\\windows\\..\\windows\\system32\\notepad.exe" 
	/// to check how Holodeck handles relative paths.
	/// </summary>
	public class NewProjectTest6 : NewProjectTest
    {
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("C:\\test","c:\\windows\\..\\windows\\..\\windows\\system32\\notepad.exe",false,true,"No","No");            

            // Make sure that the AUT is running...
            //WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

            // Now, we must exit Holodeck...
            Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

            // Wait for Holodeck to exit...
            WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}
	}

	/// <summary>
	/// Test Case 7-
	/// Parameters passed for project path and application path are correct.
	/// Project path is passed as "c:\\મ.hdp" 
	/// to check how Holodeck handles unicode chracters as parameters.
	/// </summary>
	public class NewProjectTest7 : NewProjectTest
    {
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\મ.hdp","c:\\windows\\system32\\notepad.exe",false,true,"All","No");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 8-
	/// Project path is passed as "z:\\test\\..\\test\\test.hdp" where Z drive is existing drive.
	/// Application Path is sent as blank.
	/// Holodeck is expected to attach currently running application.
	/// </summary>
	public class NewProjectTest8 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("z:\\test.hdp","",true,false,"No","All");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...
/*
			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		*/
		}    
	}

	/// <summary>
	/// Test Case 9-
	/// Project path is passed as "g:\\test\\..\\test1.hdp" where g drive is non-existing drive.
	/// </summary>
	public class NewProjectTest9 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("g:\\test\\..\\test1.hdp","c:\\windows\\system32\\notepad.exe",true,false,"All","All");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 10-
	/// Application Path is sent as blank.
	/// Holodeck is expected to attach currently running application.
	/// parameter nativeFunction is passed as "All" and netFunctions as "Some"
	/// </summary>
	public class NewProjectTest10 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\notepad.exe",true,false,"Some","No");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 11-
	/// parameter nativeFunction is passed as "All" and netFunctions as "No"
	/// </summary>
	public class NewProjectTest11 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\notepad.exe",true,false,"All","No");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 12-
	/// parameter nativeFunction is passed as "No" and netFunctions as "All"
	/// </summary>
	public class NewProjectTest12 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\notepad.exe",true,false,"No","All");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 13-
	/// parameter nativeFunction is passed as "No" and netFunctions as "Some"
	/// </summary>
	public class NewProjectTest13 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\notepad.exe",true,false,"No","Some");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}
	
	/// <summary>
	/// Test Case 14-
	/// parameter nativeFunction is passed as "All" and netFunctions as "All"
	/// </summary>
	public class NewProjectTest14 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\write.exe",true,false,"All","All");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 15-
	/// parameter nativeFunction is passed as "Some" and netFunctions as "Some"
	/// </summary>
	public class NewProjectTest15 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\write.exe",true,false,"Some","Some");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

	/// <summary>
	/// Test Case 16-
	/// parameter nativeFunction is passed as "No" and netFunctions as "No"
	/// </summary>
	public class NewProjectTest16 : NewProjectTest
	{
		public override void executeTest( )
		{
			NewProjectTest.RunTestCases("c:\\test1.hdp","c:\\windows\\system32\\write.exe",true,false,"No","No");

			// Make sure that the AUT is running...
			//WaitForAutToStart( ); // will throw an exception if it doesn't start after a specific period of time...

			// Now, we must exit Holodeck...
			Holodeck.NewProjectWizard.ExitHolodeckWhenRunningNormally( );

			// Wait for Holodeck to exit...
			WaitForHolodeckToExit( "Failure! Holodeck failed to exit while normally intercepting an AUT." );
		}    
	}

}
