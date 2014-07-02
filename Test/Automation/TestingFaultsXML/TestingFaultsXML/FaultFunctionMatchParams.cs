//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		FaultFunctionMatchParams.cs
//
// DESCRIPTION: Contains definitions for the class FaultFunctionMatchParams.
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

namespace TestingFaultsXML
{
	/// <summary>
	/// Summary description for FaultFunctionMatchParams.
	/// </summary>
	public class FaultFunctionMatchParams
	{
		public FaultFunctionMatchParams()
		{
			this.szCompareAsType = null;
			this.szID = null;
			this.szName = null;
			this.szTestOperator = null;
			this.szTestValue = null;
			this.matchParam = new ArrayList();
		}
	
		protected string szName;
		protected string szTestOperator;
		protected string szTestValue;
		protected string szCompareAsType;
		protected string szID;
		protected ArrayList matchParam;

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

		public string TestOperator
		{
			get
			{
				return this.szTestOperator;
			}
			set
			{
				this.szTestOperator = value;
			}
		}
		public string TestValue	
		{
			get
			{
				return this.szTestValue;
			}
			set
			{
				this.szTestValue = value;
			}
		}
		public string CompareAsType	
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
		public string ID	
		{
			get
			{
				return this.szID;
			}
			set
			{
				this.szID = value;
			}
		}
		public ArrayList MatchParam
		{
			get 
			{
              return this.matchParam;   
			}

			set
			{
				this.matchParam.Add(value);
			}
		}
	}
}
