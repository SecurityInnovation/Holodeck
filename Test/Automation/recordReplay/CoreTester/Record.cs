using System;

namespace CoreTester
{
	public class Record
	{
		public int index, enabled, timeDelta, type, session;
		public String appName, function, name, timeStamp;
		
		public Record(int enabledIn, String functionIn, int typeIn, String nameIn, String timeStampIn)
		{
			enabled = enabledIn;
			function = functionIn;
			name = nameIn;
			type = typeIn;
			timeStamp = timeStampIn;
		}
	}
}
