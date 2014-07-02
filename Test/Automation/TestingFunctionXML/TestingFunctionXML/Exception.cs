///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		Exception.cs
///
/// DESCRIPTION: Contains definitions for the class Exception.
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
			this.szName = null;
		}
	}
}
