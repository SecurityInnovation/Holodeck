using System;
using System.Collections;
using System.Data;

namespace CoreTester
{
	public class TestCase
	{
		public int enabled, testNo, type;
		public string file, outcome, injection;

		public TestCase(String fileIn, int enabledIn, String injectionIn, int typeIn, int testNoIn)
		{
			file = fileIn;
			enabled = enabledIn;
			injection = injectionIn;
			type = typeIn;
			testNo = testNoIn;
		}
	}
}
