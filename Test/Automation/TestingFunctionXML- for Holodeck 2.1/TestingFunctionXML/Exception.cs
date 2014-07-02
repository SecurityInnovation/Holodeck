using System;

namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for Exception.
	/// </summary>
	public class Exception
	{
		protected string szName;
		public string Name
		{
			get
			{
				return this.szName;
			}
			set
			{
				this.szName = value;
			}
		}
		public Exception()
		{
			//
			// TODO: Add constructor logic here
			//
		}
	}
}
