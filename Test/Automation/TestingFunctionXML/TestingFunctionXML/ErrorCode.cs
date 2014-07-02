///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		ErrorCode.cs
///
/// DESCRIPTION: Contains definitions for the class ErrorCode.
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
	/// Summary description for ErrorCode.
	/// </summary>
	public class ErrorCode
	{
		protected string szIdentifiedBy;
		protected string szNodeValue;
		protected string szType;


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
			this.szCode = null;
			this.szIdentifiedBy = null;
			this.szNodeValue = null;
			this.szString = null;
		}

	}
}
