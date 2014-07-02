using System;

namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for ErrorCode.
	/// </summary>
	public class ErrorCode
	{
		protected string szIdentifiedBy;
		protected string szNodeValue;

		//will be used for Errorcode.xml
		protected string szCode;
		protected string szString;

		public string IdentifiedBy
		{
			get
			{
				return this.szIdentifiedBy;
			}
			set
			{
				this.szIdentifiedBy = value;
			}
		}
		public string NodeValue
		{
			get
			{
				return this.szNodeValue;
			}
			set
			{
				this.szNodeValue = value;
			}
		}
		
		
		public string Code
		{
			get
			{
				return this.szCode;
			}
			set
			{
				this.szCode = value;
			}
		}
		public string String
		{
			get
			{
				return this.szString;
			}
			set
			{
				this.szString = value;
			}
		}

		public ErrorCode()
		{
			//
			// TODO: Add constructor logic here
			//
		}

	}
}
