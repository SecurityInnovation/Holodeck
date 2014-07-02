using System;

namespace TestFramework
{
    namespace Exceptions
    {  
        /// <summary>
        /// Provides a derived exception class for Dependency specific exception handling.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependencyExceptions : TestExceptions
        {
            public static string FILE_DOES_NOT_EXIST            = "File does not exist.";
            public static string ERROR_PARSING_XML              = "Error parsing the XML File.";
            public static string DEPENDENCIES_NOT_VERIFIED      = "Could not verify dependencies.";
            public static string DEPENDENCY_ID_DOES_NOT_EXIST   = "Dependency ID does not exist.";
            public static string INVALID_DEPENDENCY_DATA        = "The DependencyData object is invalid.";

            public DependencyExceptions( )
                : base( )
            {
            }

            public DependencyExceptions( string Message )
                : base( Message )
            {
            }

            public DependencyExceptions( string Message, Exception InnerException )
                : base( Message, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception occurs when the dependency file specified does not exist.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependencyFileDoesNotExistException : DependencyExceptions
        {
            public DependencyFileDoesNotExistException( )
                : base( )
            {
            }

            public DependencyFileDoesNotExistException( string Message )
                : base( Message + FILE_DOES_NOT_EXIST )
            {
            }

            public DependencyFileDoesNotExistException( string Message, Exception InnerException )
                : base( Message + FILE_DOES_NOT_EXIST, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception occurs when an error occurs during the XML parsing of the dependency
        /// file.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependencyErrorParsingXMLException : DependencyExceptions
        {
            public DependencyErrorParsingXMLException( )
                : base( )
            {
            }

            public DependencyErrorParsingXMLException( string Message )
                : base( Message + ERROR_PARSING_XML )
            {
            }

            public DependencyErrorParsingXMLException( string Message, Exception InnerException )
                : base( Message + ERROR_PARSING_XML, InnerException )
            {
            }
        }

        /// <summary>
        /// This error occurs when the dependencies specified in the dependency XML file
        /// could not be verified.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependenciesNotVerifiedException : DependencyExceptions
        {
            public DependenciesNotVerifiedException( )
                : base( )
            {
            }

            public DependenciesNotVerifiedException( string Message )
                : base( Message + DEPENDENCIES_NOT_VERIFIED )
            {
            }

            public DependenciesNotVerifiedException( string Message, Exception InnerException )
                : base( Message + DEPENDENCIES_NOT_VERIFIED, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception occurs when the tester attempts to access a loaded DependencyData
        /// class according to a specific ID, but the ID does not exist in the array of
        /// dependencies.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependencyIDDoesNotExistException : DependencyExceptions
        {
            public DependencyIDDoesNotExistException( )
                : base( )
            {
            }

            public DependencyIDDoesNotExistException( string Message )
                : base( Message + DEPENDENCY_ID_DOES_NOT_EXIST )
            {
            }

            public DependencyIDDoesNotExistException( string Message, Exception InnerException )
                : base( Message + DEPENDENCY_ID_DOES_NOT_EXIST, InnerException )
            {
            }
        }

        /// <summary>
        /// This exception occurs when an invalid DependencyData class instance is used in one
        /// of the dependency functions that depends on it.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependencyInvalidData : DependencyExceptions
        {
            public DependencyInvalidData( )
                : base( )
            {
            }

            public DependencyInvalidData( string Message )
                : base( Message + INVALID_DEPENDENCY_DATA )
            {
            }

            public DependencyInvalidData( string Message, Exception InnerException )
                : base( Message + INVALID_DEPENDENCY_DATA, InnerException )
            {
            }
        }
    }
}
