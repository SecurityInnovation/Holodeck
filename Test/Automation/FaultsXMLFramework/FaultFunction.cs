//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		FaultFunction.cs
//
// DESCRIPTION: Contains definitions for the class FaultFunction.
//
//=========================================================================
// Modification History
//
// Date				Name			Purpose
// -----------    ----------- ------------------------------------------
// 17 sept 2003	  V. Parikh		File created.
//*************************************************************************
using System;
using System.IO;
using System.Collections;

namespace FaultsXMLFramework
{
	/// <summary>
	/// Summary description for Function.
	/// </summary>
	public class FaultFunction
	{
		protected string szName;
		protected string szOverrideErrorCode;
		protected string szOverrideReturnValue;
		protected string szPassThrough;
		protected string szAllocation;
		protected string szException;
		protected ArrayList matchParams;
		protected ArrayList checkResource;

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
		public string OverrideErrorCode
		{
			get
			{
				return this.szOverrideErrorCode;
			}
			set
			{
				this.szOverrideErrorCode = value;
			}
		}
		public string OverrideReturnValue
		{
			get
			{
				return this.szOverrideReturnValue;
			}
			set
			{
				this.szOverrideReturnValue = value;
			}
		}
		
		public string PassThrough
		{
			get
			{
				return this.szPassThrough;
			}
			set
			{
				this.szPassThrough = value;
			}
		}
		public string Allocation
		{
			get
			{
				return this.szAllocation;
			}
			set
			{
				this.szAllocation = value;
			}
		}
		public string Exception
		{
			get
			{
				return this.szException;
			}
			set
			{
				this.szException = value;
			}
		}
		public ArrayList MatchParams
		{
			get
			{
				return this.matchParams;
			}
			set
			{
				this.matchParams.Add(value);
			}
		}
		
		public ArrayList CheckResource
		{
			get
			{
				return this.checkResource;
			}

			set
			{
				this.checkResource.Add(value);
			}

		}
		public FaultFunction()
		{
			this.szAllocation = null;
			this.szException = null;
			this.szName = null;
			this.szPassThrough = null;
			this.szOverrideErrorCode = null;
			this.szOverrideReturnValue = null;

			this.matchParams = new ArrayList();
			this.checkResource = new ArrayList();
			
			
		}
	}
}
