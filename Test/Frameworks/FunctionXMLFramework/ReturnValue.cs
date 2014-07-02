///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		ReturnValue.cs
///
/// DESCRIPTION: Contains definitions for the class ReturnValue.
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
	/// Summary description for ReturnValue.
	/// </summary>
	public class ReturnValue
	{	
		protected string szNodeValue;
		protected string szType;
		protected string szValueOperator;

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
		public string ValueOperator
		{
			get
			{
				return this.szValueOperator;
			}
			set
			{
				this.szValueOperator = value;
			}

		}
		 
		public ReturnValue()
		{
			this.szNodeValue = null;
			this.szType = null;
			this.szValueOperator = null;
		}
	}
}
