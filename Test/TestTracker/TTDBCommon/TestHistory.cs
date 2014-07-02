using System;

namespace Database
{
	/// <summary>
	/// Summary description for TestHistory.
	/// </summary>
    public class TestHistory
    {
        private string HistoryID;
        private DateTime HistoryRuntime;
        private string HistoryTestID;
        private string HistoryStatus;

        public TestHistory()
        {
            HistoryID = Guid.NewGuid( ).ToString( );
        }

        public string ID
        {
            get
            {
                return HistoryID;
            }
            set
            {
                HistoryID = value;
            }
        }

        public DateTime Runtime
        {
            get
            {
                return HistoryRuntime;
            }
            set
            {
                HistoryRuntime = value;
            }
        }

        public string TestID
        {
            get
            {
                return HistoryTestID;
            }
            set
            {
                HistoryTestID = value;
            }
        }

        public string Status
        {
            get
            {
                return HistoryStatus;
            }
            set
            {
                HistoryStatus = value;
            }
        }
	}
}
