///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		EditValues.cs
///
/// DESCRIPTION: Contains definitions for the class EditValues.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 10 Oct 2003	  V. Parikh		File created.
///*************************************************************************

using System;
using System.Xml;
using System.Xml.XPath;
using System.Text;

namespace TestingFaultsXML
{
	/// <summary>
	/// Summary description for EditValues.
	/// </summary>
	public class EditValues
	{
		public EditValues()
		{
		}

		public string [] SpecialCharacters = new string[]{@"/" , "//" , "_" , "__" , "*" , "@" , "@*" , ":" , "()" , "{}" , "[]" , "+" , "-" , "Div" , "mod" , "/ //" , "\\" , "\\+" , "\\&" , "\\*" , "\\[" , "\\]" , "\\." , "\\\\" , "|","<%%>" }; 
		public string [] EscapedCharacters = new string[]{"\\a" , "\\b" , "\\t" , "\\t" , "\\r" , "\\v" , "\\f" , "\\n" , "\\e" , "\\040" , "\\x20" , "\\cC" , "\\u0020" , "\\"};
		public string [] XmlCoreEntityChars = new string[]{"lt" , "gt" , "amp" , "apos" , "quot" , "&# value" , "&# x value" , "$#"};
		
		public static Encoding UTF7 = Encoding.UTF7;
		public static Encoding UTF8 = Encoding.UTF8;
		public static Encoding ASCII = Encoding.ASCII;
		public static Encoding Unicode = Encoding.Unicode;
		
		//		public string setString( )
		//		{			
		//			szEncodingString = ASCII.GetString( UTF8.GetBytes( "abc?" ) );
		//		}

		public string changeEncodingToUTF7(string szEncodingString )
		{			
			szEncodingString = UTF7.GetString( ASCII.GetBytes( szEncodingString ) );
			return szEncodingString;
		}

		public string changeEncodingToUTF8(string szEncodingString )
		{			
			szEncodingString = UTF8.GetString( ASCII.GetBytes( szEncodingString ) );
			return szEncodingString;
		}

		public string changeEncodingToASCII(string szEncodingString )
		{			
			szEncodingString = ASCII.GetString( UTF8.GetBytes( szEncodingString ) );
			return szEncodingString;
		}

		public string changeEncodingToUnicode(string szEncodingString )
		{			
			szEncodingString = Unicode.GetString( ASCII.GetBytes( szEncodingString ) );
			return szEncodingString;
		}
		
		public string ToUpper(string convertThisString)
		{
			return convertThisString.ToUpper();
			
		}
		public string ToLower(string convertThisString)
		{
			return convertThisString.ToLower();
			
		}
		public string SubstituteToNull(string convertThisString)
		{
			convertThisString = String.Empty;

			return convertThisString;
		}
		public string InsetAtEnd(string convertThisString, string stringToAppend)
		{
			
			return String.Concat(convertThisString,stringToAppend);
		}
		public string InsertAtStart(string convertThisString, string stringToAppend)
		{
			
			return String.Concat(stringToAppend,convertThisString);
		}

		public string SplitString(string convertThisString)
		{
			if(convertThisString != null)
				convertThisString = String.Copy(convertThisString.Insert((convertThisString.Length)/4," "));
			return convertThisString;
		}


		/// <summary>
		/// EditFault
		/// </summary>
		/// <param name="FaultNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditFault(XmlNode FaultNode,string testValue, string flags)
		{
			bool bName = false;
			bool bReturnValue = false;
			bool bErrorCode = false;
			bool bFunction = false;
			
			String[] szTokens = flags.Split();
						
			foreach (String flag in szTokens)
			{
				switch(flag)
				{
					case "Name":
					{
						bName = true;
						break;
					}
					
					case "ReturnValue":
					{
						bReturnValue = true;
						break;
					}
					
					case "ErrorCode":
					{
						bErrorCode = true;
						break;
					}

					case "Function":
					{
						bFunction = true;
						break;
					}
					
				}
			}

			foreach(XmlAttribute faultAttr in FaultNode.Attributes)
			{
				switch(faultAttr.Name)
				{
					case "Name":
					{
						if(bName)
							faultAttr.InnerText = string.Concat(faultAttr.InnerText,testValue);
						break;
					}
					case "ReturnValue":
					{
						if(bReturnValue)
							faultAttr.InnerText = string.Concat(faultAttr.InnerText,testValue);
						break;
					}
					
					case "ErrorCode":
					{
						if(bErrorCode)
							faultAttr.InnerText = string.Concat(faultAttr.InnerText,testValue);
						break;
					}
				}
			}

			foreach(XmlNode node in FaultNode.ChildNodes)
			{
				
				switch(node.Name)
				{
					case "Function":
					{
						if(bFunction)
						{
							EditFunction(node,testValue,"Name OverrideErrorCode OverrideReturnValue PassThrough Allocation Exception MatchParams CheckResource");
						}
						break;
					}
					
				}
			}

		}

		
		/// <summary>
		/// EditFunction
		/// </summary>
		/// <param name="FunctionNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditFunction(XmlNode FunctionNode,string testValue, string flags)
		{
			bool bName = false;
			bool bOverrideErrorCode = false;
			bool bOverrideReturnValue = false;
			bool bPassThrough = false;
			bool bAllocation = false;
			bool bException = false;
			bool bMatchParams = false;
			bool bCheckResource = false;

			String[] szTokens = flags.Split();
						
			foreach (String flag in szTokens)
			{
				switch(flag)
				{
					case "Name":
					{
						bName = true;
						break;
					}
					
					case "OverrideErrorCode":
					{
						bOverrideErrorCode = true;
						break;
					}
					
					case "OverrideReturnValue":
					{
						bOverrideReturnValue = true;
						break;
					}

					case "PassThrough":
					{
						bPassThrough = true;
						break;
					}

					case "Allocation":
					{
						bAllocation = true;
						break;
					}
					
					case "Exception":
					{
						bException = true;
						break;
					}
					
					case "MatchParams":
					{
						bMatchParams = true;
						break;
					}

					case "CheckResource":
					{
						bCheckResource = true;
						break;
					}					
				}
			}

			foreach(XmlAttribute functionAttr in FunctionNode.Attributes)
			{
				switch(functionAttr.Name)
				{
					case "Name":
					{
						if(bName)
							functionAttr.InnerText = string.Concat(functionAttr.InnerText,testValue);
						break;
					}
					case "OverrideErrorCode":
					{
						if(bOverrideErrorCode)
							functionAttr.InnerText = string.Concat(functionAttr.InnerText,testValue);
						break;
					}
					
					case "OverrideReturnValue":
					{
						if(bOverrideReturnValue)
							functionAttr.InnerText = string.Concat(functionAttr.InnerText,testValue);
						break;
					}

					case "PassThrough":
					{
						if(bPassThrough)
							functionAttr.InnerText = string.Concat(functionAttr.InnerText,testValue);
						break;
					}

					case "Allocation":
					{
						if(bAllocation)
							functionAttr.InnerText = string.Concat(functionAttr.InnerText,testValue);
						break;
					}
					
					case "Exception":
					{
						if(bException)
							functionAttr.InnerText = string.Concat(functionAttr.InnerText,testValue);
						break;
					}
				}
			}
			foreach(XmlNode node in FunctionNode.ChildNodes)
			{
				
				switch(node.Name)
				{
					case "MatchParams":
					{
						if(bMatchParams)
						{
							EditMatchParam(node,testValue,"CompareAsType ID Name TestOperator TestValue");
						}
						break;
					}
					case "CheckResource":
					{
						if(bCheckResource)
						{
							EditCheckResource(node,testValue,"ParamIndex Exists");
						}
						break;
					}
					
				}
			}

		}

	
		/// <summary>
		/// EditCheckResource
		/// </summary>
		/// <param name="CheckResourceNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditCheckResource(XmlNode CheckResourceNode,string testValue, string flags)
		{
			bool bParamIndex = false;
			bool bExists = false;

			String[] szTokens = flags.Split();
						
			foreach (String flag in szTokens)
			{
				switch(flag)
				{
					case "ParamIndex":
					{
						bParamIndex = true;
						break;
					}
					
					case "Exists":
					{
						bExists = true;
						break;
					}
				}
			}

			foreach(XmlAttribute checkResourceAttr in CheckResourceNode.Attributes)
			{
				switch(checkResourceAttr.Name)
				{
					case "ParamIndex":
					{
						if(bParamIndex)
							checkResourceAttr.InnerText = string.Concat(checkResourceAttr.InnerText,testValue);
						break;
					}
					case "Exists":
					{
						if(bExists)
							checkResourceAttr.InnerText = string.Concat(checkResourceAttr.InnerText,testValue);
						break;
					}
				}
			}


		}


		/// <summary>
		/// EditMatchParam
		/// </summary>
		/// <param name="MatchParamNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditMatchParam(XmlNode MatchParamNode,string testValue, string flags)
		{
			bool bCompareAsType = false;
			bool bID = false;
			bool bName = false;
			bool bTestOperator = false;
			bool bTestValue = false;
			
			String[] szTokens = flags.Split();
						
			foreach (String flag in szTokens)
			{
				switch(flag)
				{
					case "CompareAsType":
					{
						bCompareAsType = true;
						break;
					}
					
					case "ID":
					{
						bID = true;
						break;
					}
					
					case "Name":
					{
						bName = true;
						break;
					}

					case "TestOperator":
					{
						bTestOperator = true;
						break;
					}

					case "TestValue":
					{
						bTestValue = true;
						break;
					}
				}
			}

			foreach(XmlAttribute matchParamAttr in MatchParamNode.Attributes)
			{
				switch(matchParamAttr.Name)
				{
					case "CompareAsType":
					{
						if(bCompareAsType)
							matchParamAttr.InnerText = string.Concat(matchParamAttr.InnerText,testValue);
						break;
					}
					case "ID":
					{
						if(bID)
							matchParamAttr.InnerText = string.Concat(matchParamAttr.InnerText,testValue);
						break;
					}
					case "Name":
					{
						if(bName)
							matchParamAttr.InnerText = string.Concat(matchParamAttr.InnerText,testValue);
						break;
					}
					case "TestOperator":
					{
						if(bTestOperator)
							matchParamAttr.InnerText = string.Concat(matchParamAttr.InnerText,testValue);
						break;
					}
					case "TestValue":
					{
						if(bTestValue)
							matchParamAttr.InnerText = string.Concat(matchParamAttr.InnerText,testValue);
						break;
					}
					
				}
			}
		}
	}
}
