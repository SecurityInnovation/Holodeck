using System;
using System.Xml;
using System.IO;
using System.Collections;
using TestFramework.Exceptions;
using TestFramework.Run;

namespace TestFramework
{
    namespace DependencyVerifier
    {
        /// <summary>
        /// This struct is used for storage of dependency data in an array... 
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class DependencyData
        {
            public string id;
            public string name;
            public string location;
            public string version;

            /// <summary>
            /// Represents the name and location of the full path to the dependency.
            /// </summary>
            /// <returns>A string representing the fully qualified path and filename of the dependency.</returns>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public string FullPath
            {
                get
                {
                    return ( location + "\\" + name );
                }
            }
        }

        /// <summary>
        /// This struct is used for storage of extension data in an array...
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class ExtensionData
        {
            public string id;
            public string name;
            public string location;
        }

        /// <summary>
        /// This class handles dependency verification. It ensures that files
        /// exist where they are claimed to exist. If verification fails, then
        /// the test cannot be executed. The class uses a forward read only
        /// XML text reader to parse the dependency documents.
        /// </summary>
        /// <remarks>
        /// Author(s):  teejay
        /// Revision:   1.0
        /// Modified:   6/04/2003
        /// </remarks>
        public class Dependency
        {

            public const string XML_MAIN_DOCUMENT_TAG =         "dependencies";
            public const string XML_DEPENDENCY_TAG =            "dependency";
            public const string XML_DEPENDENCY_ID_TAG =         "id";
            public const string XML_DEPENDENCY_NAME_TAG =       "name";
            public const string XML_DEPENDENCY_LOCATION_TAG =   "location";
            public const string XML_DEPENDENCY_VERSION_TAG =    "version";
            public const string XML_EXTENSION_TAG =             "extension";
            public const string XML_EXTENSION_ID_TAG =          "id";
            public const string XML_EXTENSION_LOCATION_TAG =    "location";
            public const string XML_EXTENSION_NAME_TAG =        "name";

            private string m_dependencyFile;
            private XmlTextReader xmlReader;
            private ArrayList m_dependencyArray = new ArrayList( );
            private ArrayList m_extensionArray = new ArrayList( );
            private string m_dependencyLocation;

            /// <summary>
            /// This constructor takes in the dependency file and automatically starts
            /// a verification process using it. Failure of the verification can be
            /// determined by checking for exceptions of the DependencyExceptions class.
            /// </summary>
            /// <param name="dependencyFile">The XML dependency file to check dependencies with.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public Dependency( string dependencyFile )
            {
                m_dependencyLocation = System.IO.Path.GetDirectoryName( dependencyFile );

                PerformVerification( dependencyFile );
            }

            /// <summary>
            /// Opens the dependency file with a text reader first ensuring that the file
            /// exists.
            /// </summary>
            /// <param name="dependencyFile">The XML dependency file to load.</param>
            /// <returns>An XMLTextReader instance starting at the beginning of the file.</returns>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            private XmlTextReader LoadDependencyFileLocally( string dependencyFile )
            {
                m_dependencyFile = dependencyFile;
                XmlTextReader m_xmlReader = null;
                
                if ( RunTest.verbose )
                    Log.Log.printSuccess( "Opening Dependency File", m_dependencyFile );

                if ( File.Exists( m_dependencyFile ) )
                {
                    // instantiate a new xml Reader based off of the input file...
                    m_xmlReader = new XmlTextReader( m_dependencyFile );
                }
                else
                {
                    // throw an exception with an extension to the error to be thrown...
                    throw new DependencyFileDoesNotExistException( "[" + m_dependencyFile + "] " );
                }

                return m_xmlReader;
            }

            /// <summary>
            /// Stores the XmlTextReader instance in a local class variable after obtaining it
            /// from the LoadDependencyFileLocally call.
            /// </summary>
            /// <param name="dependencyFile">The XML dependency file to load.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public void LoadDependencyFile( string dependencyFile )
            {
                xmlReader = LoadDependencyFileLocally( dependencyFile );
            }

            /// <summary>
            /// Adds extensions to the extension array list. This function handles the starting
            /// node in the XML document tree with XML_EXTENSION_TAG. Each extension is parsed
            /// for name, id, and location.
            /// </summary>
            /// <param name="m_xmlReader">An XmlTextReader instance starting at the XML_EXTENSION_TAG node of the XML document.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            private void AddExtension ( XmlTextReader m_xmlReader )
            {
                // The extension will refer to a new dependency file to load which
                // will be read from this new XmlTextReader...
                XmlTextReader m_newReader;
                string m_elementName;
                string m_elementValue;

                // instantaite a new struct to store the data...
                ExtensionData m_extensionData = new ExtensionData( );
                
                try
                {
                    while ( m_xmlReader.Read( ) )
                    {
                        switch ( m_xmlReader.NodeType )
                        {
                            case XmlNodeType.EndElement:

                                if ( m_xmlReader.Name == XML_EXTENSION_TAG )
                                    goto finished;

                                break;

                            case XmlNodeType.Element:

                                // get the name of the element...
                                m_elementName = m_xmlReader.Name;

                                // attempt to read the actual value of the element...
                                m_xmlReader.Read( );

                                if ( m_xmlReader.NodeType == XmlNodeType.EndElement )
                                {
                                    // no value exists for this element...
                                    m_elementValue = "";
                                }
                                else
                                    m_elementValue = m_xmlReader.Value;

                                if ( m_elementName == XML_EXTENSION_ID_TAG )
                                    m_extensionData.id = m_elementValue;

                                if ( m_elementName == XML_EXTENSION_NAME_TAG )
                                    m_extensionData.name = m_elementValue;

                                if ( m_elementName == XML_EXTENSION_LOCATION_TAG )
                                    if ( Directory.Exists( m_elementValue ) )
                                        m_extensionData.location = m_elementValue;
                                    else
                                        m_extensionData.location = 
                                            System.IO.Path.GetFullPath( System.IO.Path.GetFullPath( m_dependencyLocation ) + "\\" + m_elementValue );

                                break;
                        }
                    }

                finished:

                    // add the extension to the array...        
                    m_extensionArray.Add( m_extensionData );

                    // now attempt to load the extended dependency data as part of the current
                    // list of dependencies...
                    m_newReader = LoadDependencyFileLocally( m_extensionData.location + "\\" + m_extensionData.name );
                        

                    // parse dependencies on this extended dependency file...
                    ParseDependencies( m_newReader );
                }
                catch ( DependencyFileDoesNotExistException e )
                {
                    TestExceptions.printException( e );

                    throw e;
                }
            }

            /// <summary>
            /// Adds dependencies to the dependency array list. This function handles the starting
            /// node in the XML document tree with XML_DEPENDENCY_TAG. Each dependency is parsed
            /// for id, name, location, and version.
            /// </summary>
            /// <param name="m_xmlReader">An XmlTextReader instance starting at the XML_DEPENDENCY_TAG node of the XML document.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            private void AddDependency ( XmlTextReader m_xmlReader )
            {
                // instantaite a new struct to store the data...
                DependencyData m_dependencyData = new DependencyData( );
                string m_elementName;
                string m_elementValue;
                
                try
                {
                    while ( m_xmlReader.Read( ) )
                    {
                        switch ( m_xmlReader.NodeType )
                        {
                            case XmlNodeType.EndElement:

                                if ( m_xmlReader.Name == XML_DEPENDENCY_TAG )
                                    goto finished;

                                break;

                            case XmlNodeType.Element:

                                // get the name of the element...
                                m_elementName = m_xmlReader.Name;

                                // attempt to read the actual value of the element...
                                m_xmlReader.Read( );

                                if ( m_xmlReader.NodeType == XmlNodeType.EndElement )
                                {
                                    // no value exists for this element...
                                    m_elementValue = "";
                                }
                                else
                                    m_elementValue = m_xmlReader.Value;

                                if ( m_elementName == XML_DEPENDENCY_ID_TAG )
                                    m_dependencyData.id = m_elementValue;

                                if ( m_elementName == XML_DEPENDENCY_NAME_TAG )
                                    m_dependencyData.name = m_elementValue;

                                if ( m_elementName == XML_DEPENDENCY_VERSION_TAG )
                                    m_dependencyData.version = m_elementValue;

                                if ( m_elementName == XML_DEPENDENCY_LOCATION_TAG )
                                    if ( Directory.Exists( m_elementValue ) )
                                        m_dependencyData.location = m_elementValue;
                                    else
                                        m_dependencyData.location = 
                                	        System.IO.Path.GetFullPath( System.IO.Path.GetFullPath( m_dependencyLocation ) + "\\" + m_elementValue );

                                break;
                        }
                    }
                finished:

                    m_dependencyArray.Add( (object) m_dependencyData );
                }
                catch ( XmlException e )
                {
                    throw new DependencyErrorParsingXMLException(  "[LINE: " + e.LineNumber + " POS: " + e.LinePosition + " MESSAGE: " + e.Message + "] " );
                }
            }

            /// <summary>
            /// Fills up the corresponding DependencyData or ExtensionData array list depending on
            /// the node element name encountered.
            /// </summary>
            /// <param name="m_xmlReader">An XmlTextReader starting at the beginning of the dependency XML document.</param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            private void ParseDependencies( XmlTextReader m_xmlReader )
            {
                try
                {
                    // read all the nodes in the XML file...
                    while ( m_xmlReader.Read( ) )
                    {
                        switch ( m_xmlReader.NodeType )
                        {
                            case XmlNodeType.Element:

                                if ( m_xmlReader.Name == XML_DEPENDENCY_TAG )
                                    AddDependency( m_xmlReader );

                                if ( m_xmlReader.Name == XML_EXTENSION_TAG )
                                    AddExtension( m_xmlReader );
                                    
                                break;
                        }
                    }
                }
                catch ( XmlException e )
                {
                    throw new DependencyErrorParsingXMLException(  "[LINE: " + e.LineNumber + " POS: " + e.LinePosition + " MESSAGE: " + e.Message + "] " );
                }
            }

            /// <summary>
            /// Calls the real ParseDependencies( xmlReader ) function with the locally stored class
            /// variable.
            /// </summary>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public void ParseDependencies(  )
            {
                ParseDependencies( xmlReader );
            }

            /// <summary>
            /// Once dependencies have been loaded from the dependency xml file, the dependencies
            /// are verified one by one by checking if the file exists.
            /// </summary>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public void VerifyLoadedDependencies( )
            {
                bool bDependencyResult = true;
                bool bTemp = false;

                foreach( DependencyData depends in m_dependencyArray )
                {

                    // First, try the full path...
                    bTemp = File.Exists( depends.location + "\\" + depends.name );

                    // Then, try the full path based on the current dependency location (in case it is relative)...
                    bTemp |= File.Exists( m_dependencyLocation + "\\" + depends.location + "\\" + depends.name );

                    if ( !bTemp )
                        Log.Log.printError( "DEPENDENCY NOT FOUND: " + depends.location + "\\" + depends.name );

                    bDependencyResult &= bTemp;
                }

                if  ( !bDependencyResult )
                    throw new DependenciesNotVerifiedException( );
            }

            /// <summary>
            /// This function performs all steps necessary to verify dependencies. It
            /// 1) Loads the first dependency file, 2) parses the first and successive dependency
            /// files, and 3) verifies the loaded dependency information.
            /// </summary>
            /// <param name="dependencyFile"></param>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public void PerformVerification( string dependencyFile )
            {
                try
                {
                    LoadDependencyFile( dependencyFile );
                
                    ParseDependencies( );
                 
                    VerifyLoadedDependencies( );

                    if ( RunTest.verbose )                
                        Log.Log.printSuccess( "Dependencies Verified Successfully." );

                }
                catch ( DependencyFileDoesNotExistException e )
                {
                    TestExceptions.printException( e );

                    throw e;
                }
                catch ( DependencyErrorParsingXMLException e )
                {
                    TestExceptions.printException( e );

                    throw e;
                }
                catch ( DependenciesNotVerifiedException e )
                {
                    TestExceptions.printException( e );

                    throw e;
                }

            }

            /// <summary>
            /// Provides a method for the tester to obtain the full path and names of
            /// dependencies defined in a dependency or set of dependency files.
            /// </summary>
            /// <param name="dependencyId">The dependency ID as defined in the dependency XML file.</param>
            /// <returns>A DependencyData class containing id, name, location, and version information.</returns>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public DependencyData getDependencyById( string dependencyId )
            {
                foreach ( DependencyData dependency in m_dependencyArray )
                {
                    if ( dependency.id == dependencyId )
                        return dependency;
                }

                throw new DependencyIDDoesNotExistException( "[ID: " + dependencyId + "] " );
            }

            /// <summary>
            /// Pulls out the name and location information from a DependencyData class instance and
            /// concatenates the data together to form a fully qualified path for accessing the file
            /// with XML routines or any other file access routine the tester needs to use.
            /// </summary>
            /// <param name="m_dependency">The DependencyData class instance containing the information of interest.</param>
            /// <returns>A string representing the fully qualified path and filename of the dependency.</returns>
            /// <remarks>
            /// Author(s):  teejay
            /// Revision:   1.0
            /// Modified:   6/04/2003
            /// </remarks>
            public string getFullPathFromDependency( DependencyData m_dependency, bool relative )
            {
                if ( m_dependency == null )
                    throw new DependencyInvalidData( );

                if ( !relative )
                    return ( m_dependency.location + "\\" + m_dependency.name );
                else
                    return ( m_dependencyLocation + "\\" + m_dependency.location + "\\" + m_dependency.name );
            }

            /// <summary>
            /// By default we will use relative paths.
            /// </summary>
            /// <param name="m_dependency"></param>
            /// <param name="relative"></param>
            /// <returns></returns>
            public string getFullPathFromDependency( DependencyData m_dependency )
            {
                return getFullPathFromDependency( m_dependency, true );
            }
        }
    }
}
