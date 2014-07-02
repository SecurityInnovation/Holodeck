using System;

namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for Port.
	/// </summary>
	public class Port
	{
		protected string szName;
		protected int iNumber;
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
		public int Number
		{
			get
			{
				return this.iNumber;
			}
			set
			{
				this.iNumber = value;
			}
		}
		
		public Port()
		{
			//
			// TODO: Add constructor logic here
			//
		}
	}
}
