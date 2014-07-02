using System;

namespace TestFramework
{
    namespace Exceptions
    {
        /// <summary>
        /// TestExceptions is the base exception class for all the Test Framework exceptions.
        /// This class inherits from the ApplicationException base class.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class TestExceptions : ApplicationException
        {
            public static string UNEXPECTED_EXCEPTION           = "An unexpected error occurred in the Test Framework";

            public static void printException( Exception e )
            {
                Log.Log.printError( e.Message );
            }

            public TestExceptions() 
                : base( )
            {
            }

            public TestExceptions( string Message ) 
                : base( Message )
            {
            }

            public TestExceptions( string Message, Exception InnerException ) 
                : base( Message, InnerException )
            {
            }
        }

        /// <summary>
        /// UnexpectedException is a generic exception that is thrown when no other exception matches the failure.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class UnexpectedException : TestExceptions
        {
            public UnexpectedException( ) 
                : base( UNEXPECTED_EXCEPTION )
            {
            }

            public UnexpectedException( string Message ) 
                : base( Message + UNEXPECTED_EXCEPTION )
            {
            }

            public UnexpectedException( string Message, Exception InnerException ) 
                : base( Message + UNEXPECTED_EXCEPTION, InnerException )
            {   
            }
        }
    }
}
