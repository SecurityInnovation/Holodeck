//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		FaultFunctionCheckResource.cs
//
// DESCRIPTION: Contains definitions for the class FaultFunctionCheckResource.
//
//=========================================================================
// Modification History
//
// Date				Name			Purpose
// -----------    ----------- ------------------------------------------
// 17 sept 2003	  V. Parikh		File created.
//*************************************************************************
using System;

namespace TestingFaultsXML
{
	/// <summary>
	/// Summary description for CheckResource.
	/// </summary>
	public class FaultFunctionCheckResource
	{
		protected string szParamIndex;
		protected string szExists;

		public string Exists
		{
			get
			{
				return this.szExists;
			}
			set
			{
				this.szExists = value;
			}

		}
		public string ParamIndex
		{
			get
			{
				return this.szParamIndex;
			}
			set
			{
				this.szParamIndex = value;
			}

		}
		public FaultFunctionCheckResource()
		{
			this.szExists = null;
			this.szParamIndex = null;
		}
	}
}
