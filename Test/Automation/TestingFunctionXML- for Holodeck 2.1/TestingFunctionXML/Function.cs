using System;
using System.Collections;

namespace TestingFunctionXML
{
	
	public class Function
	{
		public Function()
		{
			modifiers = new ArrayList(); 
			errorCodes = new ArrayList();
			parameters = new ArrayList();
			exceptions = new ArrayList();
		}
		

		//variables
		protected string szFunctionName;
		protected string szOriginalDll;
		protected string szInterceptedDll;
		protected string szReplacementFunctionName;
		protected string szReturnType;
		protected ArrayList modifiers; 
		protected string szCategory;
		protected string szReturnValueDescription;
		protected ArrayList errorCodes;
		protected ArrayList parameters;
		protected ArrayList exceptions;
		
		//properties
		public string FunctionName
		{
			get
			{
				return this.szFunctionName;
			}
			set
			{
				this.szFunctionName = value;
			}
		}
		
		public string OriginalDll
		{
			get
			{
				return this.szOriginalDll;
			}
			set
			{
				this.szOriginalDll = value;
			}
		}

		public string InterceptedDll
		{
			get
			{
				return this.szInterceptedDll;
			}
			set
			{
				this.szInterceptedDll = value;
			}
		}
		public string ReplacementFunctionName
		{
			get
			{
				return this.szReplacementFunctionName;
			}
			set
			{
				this.szReplacementFunctionName = value;
			}
		}
		public string ReturnType
		{
			get
			{
				return this.szReturnType;
			}
			set
			{
				this.szReturnType = value;
			}
		}

		public ArrayList Modifiers
		{
			get
			{
				return this.modifiers;
			}
			set
			{
				this.modifiers.Add(value);
			}
		}
		public string Category
		{
			get
			{
				return this.szCategory;
			}
			set
			{
				this.szCategory = value;
			}
		}
		public string ReturnValueDescription
		{
			get
			{
				return this.szReturnValueDescription;
			}
			set
			{
				this.szReturnValueDescription = value;
			}
		}

		public ArrayList ErrorCode
		{
			get
			{
				return this.errorCodes;
			}
			set
			{
				this.errorCodes.Add(value);
			}
			
		}
		public ArrayList Parameter
		{
			get
			{
				return this.parameters;
			}
			set
			{
				this.parameters.Add(value);
			}
		}
		public ArrayList Exception
		{
			get
			{
				return this.exceptions;
			}
			set
			{
				this.exceptions.Add(value);
			}
		}
		
		
		
		

		


	}
}
