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
			this.szValue = null;
			this.szMapTo = null;
			this.szPointsToResource = null;
			this.szConcatWith = null;
		}

		protected int paraIndex;
		protected string szType;
		protected string szCompareAsType;
		protected string szAccess;
		protected string szPointsToResource;
		protected string szConcatWith;
		protected string szMapTo;
		protected string szName;
		protected string szValue;
			
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
		public string PointsToResource
		{
			get
			{
				return this.szPointsToResource;
			}
			set
			{
				this.szPointsToResource = value;
			}
		}
		public string ConcatWith
		{
			get
			{
				return this.szConcatWith;
			}
			set
			{
				this.szConcatWith = value;
			}
		}
		public string MapTo
		{
			get
			{
				return this.szMapTo;
			}
			set
			{
				this.szMapTo = value;
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
		
		public string AssignedValue
		{
			get
			{
				return this.szValue;
			}
			set
			{
				this.szValue = value;
			}
		}

	}
}
