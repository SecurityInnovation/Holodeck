using System;

namespace Database
{
	/// <summary>
	/// Summary description for Test.
	/// </summary>
	public class Test
	{
        private string TestID;
        private string TestName;
        private string TestDescription;
        private string TestAreaID;
        private string TestPriority;
        private string TestOwnerID;
        private string TestExecutionPath;
        private string TestExecutionBinary;
        private string TestOperatingSystem;
        private string TestOSVersion;
        private string TestRunFrequency;
        private string TestExecutionArgs;
        private string TestState;

        public Test ( )
        {
            TestID = Guid.NewGuid( ).ToString( );
            TestName = null;
            TestDescription = null;
            TestAreaID = null;
            TestPriority = "0";
            TestOwnerID = null;
            TestExecutionPath = null;
            TestExecutionBinary = null;
            TestOperatingSystem = null;
            TestOSVersion = null;
            TestRunFrequency = null;
        }

        public string ID
        {
            get
            {
                return TestID;
            }
            set
            {
                TestID = value;
            }
        }

        public string Name
        {
            get
            {
                return TestName;
            }
            set
            {
                TestName = value;
            }
        }

        public string Description
        {
            get
            {
                return TestDescription;
            }
            set
            {
                TestDescription = value;
            }
        }

        public string AreaID
        {
            get
            {
                return TestAreaID;
            }
            set
            {
                TestAreaID = value;
            }
        }

        public string Priority
        {
            get
            {
                return TestPriority;
            }
            set
            {
                TestPriority = value;
            }
        }

        public string OwnerID
        {
            get
            {
                return TestOwnerID;
            }
            set
            {
                TestOwnerID = value;
            }
        }

        public string ExecutionPath
        {
            get
            {
                return TestExecutionPath;
            }
            set
            {
                TestExecutionPath = value;
            }
        }

        public string ExecutionBinary
        {
            get
            {
                return TestExecutionBinary;
            }
            set
            {
                TestExecutionBinary = value;
            }
        }

        public string OperatingSystem
        {
            get
            {
                return TestOperatingSystem;
            }
            set
            {
                TestOperatingSystem = value;
            }
        }

        public string OSVersion
        {
            get
            {
                return TestOSVersion;
            }
            set
            {
                TestOSVersion = value;
            }
        }

        public string RunFrequency
        {
            get
            {
                return TestRunFrequency;
            }
            set
            {
                TestRunFrequency = value;
            }
        }

        public string ExecutionArgs
        {
            get
            {
                return TestExecutionArgs;
            }
            set
            {
                TestExecutionArgs = value;
            }
        }

        public string State
        {
            get
            {
                return TestState;
            }
            set
            {
                TestState = value;
            }
        }
		
	}
}
