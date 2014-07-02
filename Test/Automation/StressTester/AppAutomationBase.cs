using System;
using System.Diagnostics;

namespace StressTester
{
	/// <summary>
	/// This class defines, basically a template with ONE function that is called by the stress
	/// test execution. This function then determines exactly what will happen to the application
	/// under test...
	/// </summary>
	public class AppAutomationBase
	{		
        public virtual void performAUTAutomation( Process processClass )
        {
            throw new Exception( "The performAUTAutomation class was not overriden as required." );
        }
	}
}
