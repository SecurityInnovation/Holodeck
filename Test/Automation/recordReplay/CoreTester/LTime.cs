using System;

namespace CoreTester
{
	
	public class LTime
	{
		private String thisDate, thisTime, thisTimeStamp;

		public LTime()
		{
			//main constructor for the class
			updateTime();
		}

		public String getDate() 
		{
			//return the date in the format we want
			return thisDate;
		}

		public String getTime() 
		{
			//return the time in the format we want
			return thisTime;
		}

		public String getTimeStamp() 
		{
			//return the time in the format we want
			return thisTimeStamp;
		}
		
		public void updateTime()
		{
			//get todays date
			DateTime todayDate = DateTime.Now;

			//now format the date and time how we want them
			thisDate = "[" + todayDate.ToString("F") + "] ";
			thisTime = "[" + todayDate.ToString("T") + "] ";

	

			thisTimeStamp = todayDate.ToString("M/d/yyyy H:mm:ss.fff"); 
		}
	}
}
