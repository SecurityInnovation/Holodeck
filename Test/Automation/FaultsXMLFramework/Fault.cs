//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		Fault.cs
//
// DESCRIPTION: Contains definitions for the class Fault.
//
//=========================================================================
// Modification History
//
// Date				Name			Purpose
// -----------    ----------- ------------------------------------------
// 17 sept 2003	  V. Parikh		File created.
//*************************************************************************
using System;
using System.Collections;

namespace FaultsXMLFramework
{
	/// <summary>
	/// Summary description for Falut.
	/// </summary>
	public class Fault
	{
		protected string szName;
		protected string szReturnValue;
		protected string szErrorCode;
		protected ArrayList function;

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
		public string ReturnValue
		{
			get
			{
				return this.szReturnValue;
			}
			set
			{
				this.szReturnValue = value;
			}
		}
		public string ErrorCode
		{
			get
			{
				return this.szErrorCode;
			}
			set
			{
				this.szErrorCode = value;
			}

		}


		public ArrayList Function
		{
			get
			{
				
				return this.function;
			}
			set
			{
				this.function.Add(value);
			}
		}

		public Fault()
		{
			this.szErrorCode = null;
			this.szName = null;
			this.szReturnValue = null;
			function = new ArrayList();
			
		}
	}
}
