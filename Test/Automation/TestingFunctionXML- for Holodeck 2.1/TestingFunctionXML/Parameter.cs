using System;

namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for Parameter.
	/// </summary>
	public class Parameter
	{
		protected int paraIndex;
		protected string szType;
		protected string szCompareAsType;
		protected string szAccess;
		protected string szName;
			
		public int Index
		{
			get
			{
				return this.paraIndex;
			}
			set
			{
				this.paraIndex = value;
			}
		}
		public string Type
		{
			get
			{
				return this.szType;
			}
			set
			{
				this.szType = value;
			}
		}
		public string CompareAs
		{
			get
			{
				return this.szCompareAsType;
			}
			set
			{
				this.szCompareAsType = value;
			}
		}
		public string Access
		{
			get
			{
				return this.szAccess;
			}
			set
			{
				this.szAccess = value;
			}
		}

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
		public Parameter()
		{
			//
			// TODO: Add constructor logic here
			//
		}

	}
}
