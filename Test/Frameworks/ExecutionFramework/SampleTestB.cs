using System;
using TestFramework.Log;

namespace NewProject
{
	/// <summary>
	/// Summary description for SampleTestB.
	/// </summary>
	public class NewProjectMySpecialCase : NewProject
	{
        public override void executeTest( )
        {
            Log.printTestStatus( "Hello sample test B" );
        }
	}
}
