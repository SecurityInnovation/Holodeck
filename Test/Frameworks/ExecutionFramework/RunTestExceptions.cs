using System;

namespace TestFramework
{
    namespace Exceptions
    {
        /// <summary>
        /// Defines exceptions that can occur directly relating to test runs. The
        /// exceptions derined are derived from the class TestExceptions.
        /// </summary>
        public class RunTestExceptions : TestExceptions
        {
            // Define messages to be used for the RunTestExceptions...
            public static string NO_EXECUTE_TEST_METHOD
                = "No executeTest(...) method Associated with Specified Test ID.";

            public static string NO_GET_TEST_METHOD
                = "No getTestMethod(...) method Associated with Specified Test ID.";

            public static string NO_TEST_CLASS
                = "No Test Classes are associated with the Specified Test ID.";

            public static string NO_DEPENDENCY_FILE_SPECIFIED
                = "Alternate dependency option specified without providing a file.";

            public static string NO_TEST_SPECIFIED
                = "No test ID specified.";


            public RunTestExceptions( ) 
                : base( )
            {			
            }

            public RunTestExceptions( string Message ) 
                : base( Message )
            {
            }

            public RunTestExceptions( string Message, Exception InnerException ) 
                : base ( Message, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception is thrown when no "executeTest(...)" method exists in the
        /// test case class defined by the tester.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class NoExecuteTestMethodException : RunTestExceptions
        {
            public NoExecuteTestMethodException ( ) 
                : base ( NO_EXECUTE_TEST_METHOD )
            {
            }

            public NoExecuteTestMethodException ( string Message ) 
                : base ( Message + NO_EXECUTE_TEST_METHOD )
            {
            }

            public NoExecuteTestMethodException ( string Message, Exception InnerException ) 
                : base ( Message + NO_EXECUTE_TEST_METHOD, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception is thrown when no "getTestClass(...)" method exists in the
        /// test control class defined by the tester.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class NoGetTestMethodException : RunTestExceptions
        {
            public NoGetTestMethodException ( ) 
                : base ( NO_GET_TEST_METHOD )
            {
            }

            public NoGetTestMethodException ( string Message ) 
                : base ( Message + NO_GET_TEST_METHOD )
            {
            }

            public NoGetTestMethodException ( string Message, Exception InnerException ) 
                : base ( Message + NO_GET_TEST_METHOD, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception is thrown when a test class was not obtained or does not exist
        /// corresponding with the specified Test ID.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class NoTestClassException : RunTestExceptions
        {
            public NoTestClassException ( ) 
                : base ( NO_TEST_CLASS )
            {
            }

            public NoTestClassException ( string Message ) 
                : base ( Message + NO_TEST_CLASS )
            {
            }

            public NoTestClassException ( string Message, Exception InnerException ) 
                : base ( Message + NO_TEST_CLASS, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception is thrown when the custom dependency file defined by the user on the
        /// command line does not exist.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class NoDependencyFileException : RunTestExceptions
        {
            public NoDependencyFileException ( ) 
                : base ( NO_DEPENDENCY_FILE_SPECIFIED )
            {
            }

            public NoDependencyFileException ( string Message ) 
                : base ( Message + NO_DEPENDENCY_FILE_SPECIFIED )
            {
            }

            public NoDependencyFileException ( string Message, Exception InnerException ) 
                : base ( Message + NO_DEPENDENCY_FILE_SPECIFIED, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception is thrown when no test ID is specified on the command line
        /// by the user.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class NoTestException : RunTestExceptions
        {
            public NoTestException ( ) 
                : base ( NO_TEST_SPECIFIED )
            {
            }

            public NoTestException ( string Message ) 
                : base ( Message + NO_TEST_SPECIFIED )
            {
            }

            public NoTestException ( string Message, Exception InnerException ) 
                : base ( Message + NO_TEST_SPECIFIED, InnerException )
            {
            }
        }
    }
}
