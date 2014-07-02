///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		Parameter.cs
///
/// DESCRIPTION: Contains definitions for the class Parameter.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 08 sept 2003	  V. Parikh		File created.
///*************************************************************************

using System;

namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for Parameter.
	/// </summary>
	public class Parameter
	{
		//contructor
		public Parameter()
		{
			this.szAccess = null;
			this.szType = null;
			this.szName = null;
			this.szCompareAsType = null;
			this.paraIndex = -1;
		}

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
		

	}
}
