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
/// 04 Oct 2003	  V. Parikh		File created.
///*************************************************************************

using System;
using System.Xml;
using System.Xml.XPath;
using System.Text;


namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for EditValues.
	/// </summary>
	public class EditValues
	{
		public string [] SpecialCharacters = new string[]{@"/" , "//" , "_" , "__" , "*" , "@" , "@*" , ":" , "()" , "{}" , "[]" , "+" , "-" , "Div" , "mod" , "/ //" , "\\" , "\\+" , "\\&" , "\\*" , "\\[" , "\\]" , "\\." , "\\\\" , "|","<%%>" }; 
		public string [] EscapedCharacters = new string[]{"\\a" , "\\b" , "\\t" , "\\t" , "\\r" , "\\v" , "\\f" , "\\n" , "\\e" , "\\040" , "\\x20" , "\\cC" , "\\u0020" , "\\"};
		public string [] XmlCoreEntityChars = new string[]{"lt" , "gt" , "amp" , "apos" , "quot" , "&# value" , "&# x value" , "$#"};
		
		public static Encoding UTF7 = Encoding.UTF7;
		public static Encoding UTF8 = Encoding.UTF8;
		public static Encoding ASCII = Encoding.ASCII;
		public static Encoding Unicode = Encoding.Unicode;
		
//		public string setString( )
//		{			
//			szEncodingString = ASCII.GetString( UTF8.GetBytes( "abcಎ" ) );
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
		
		public EditValues()
		{
			
		}

		/// <summary>
		/// EditFunction
		/// </summary>
		/// <param name="FunctionNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditFunction(XmlNode FunctionNode,string testValue, string flags)
		{
			bool bFunctionName = false;
			bool bOriginalDll = false;
			bool bInterceptedDll = false;
			bool bReplacementFunctionName= false;
			bool bReturnType = false;
			bool bModifier= false;
			bool bCategory= false;
			bool bReturnValue= false;
			bool bErrorCode= false;
			bool bException= false;
			bool bParam= false;

			String[] szTokens = flags.Split();
						
			foreach (String flag in szTokens)
			{
				switch(flag)
				{
					case "FunctionName":
					{
						bFunctionName = true;
						break;
					}
					
					case "OriginalDll":
					{
						bOriginalDll = true;
						break;
					}
					
					case "InterceptedDll":
					{
						bInterceptedDll = true;
						break;
					}

					case "ReplacementFunctionName":
					{
						bReplacementFunctionName = true;
						break;
					}
					case "ReturnType":
					{
						bReturnType = true;
						break;
					}
					case "Modifier": 
					{
						bModifier = true;
						break;
					}
					case "Category":
					{
						bCategory = true;
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

					case "Exception":
					{
						bException = true;
						break;
					}
					
					case "Param":
					{
						bParam = true;
						break;
					}

				}
			}


			foreach(XmlNode node in FunctionNode.ChildNodes)
			{
				
				switch(node.Name)
				{
					case "FunctionName":
					{
						if(bFunctionName)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}
					
					case "OriginalDll":
					{
						if(bOriginalDll)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}
					
					case "InterceptedDll":
					{
						if(bInterceptedDll)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}

					case "ReplacementFunctionName":
					{
						if(bReplacementFunctionName)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}
					case "ReturnType":
					{
						if(bReturnType)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}
					case "Modifier": 
					{
						if(bModifier)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}
					case "Category":
					{
						if(bCategory)
							node.InnerText = string.Concat(node.InnerText,testValue);
						break;
					}

					case "ReturnValue":
					{
						if(bReturnValue)
						{
							EditReturnValue(node,testValue,"NodeValue Type");
						}
						break;
					}
					
					case "ErrorCode": 
					{	
						if(bErrorCode)
						{
							EditErrorCode(node,testValue,"IdentifyBy Type NodeValue");
						}
						break;
					}

					case "Exception":
					{
						if(bException)
						{
							EditException(node,testValue,"Name");
						}
						break;
					}
					
					case "Param":
					{
						if(bParam)
						{
							EditParam(node,testValue,"Index Type CompareAs Access Name");
						}
						break;
					}

				}
			}
		}
	
		/// <summary>
		/// EditReturnValue
		/// </summary>
		/// <param name="ReturnValueNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditReturnValue(XmlNode ReturnValueNode,string testValue,string flags)
		{
			bool bNodeValue = false;
			bool bType = false;
			
									
			String[] szTokens = flags.Split();
			foreach(String flag in szTokens)
			{
				switch(flag)
				{
					case "Type":
					{
						bType = true;
						break;
					}
					case "NodeValue":
					{
						bNodeValue = true;
						break;
					}
				}
			}
						
			
			foreach(XmlAttribute returnValueAttr in ReturnValueNode.Attributes)
			{
				switch(returnValueAttr.Name)
				{
					case "Type":
					{
						if(bType)
							returnValueAttr.InnerText = string.Concat(returnValueAttr.InnerText,testValue);
						break;
					}
				}
			}
			
			if(bNodeValue)
				ReturnValueNode.InnerText = string.Concat(ReturnValueNode.InnerText,testValue);
								
		}

		/// <summary>
		/// EditErrorCode
		/// </summary>
		/// <param name="ErrorCodeNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditErrorCode(XmlNode ErrorCodeNode,string testValue,string flags)
		{
			bool bIdentifyBy = false;
			bool bType = false;
			bool bNodeValue = false;
						
			String[] szTokens = flags.Split();
			foreach(String flag in szTokens)
			{
				switch(flag)
				{
					case "IdentifyBy":
					{
						bIdentifyBy = true;
						break;
					}
					case "Type":
					{
						bType = true;
						break;
					}
					case "NodeValue":
					{
						bNodeValue = true;
						break;
					}
				}
			}
						
			
			foreach(XmlAttribute errorCodeAttr in ErrorCodeNode.Attributes)
			{
				switch(errorCodeAttr.Name)
                {
					case "IdentifyBy":
					{
						if(bIdentifyBy)
							errorCodeAttr.InnerText = string.Concat(errorCodeAttr.InnerText,testValue);
						break;
					}
					case "Type":
					{
						if(bType)
							errorCodeAttr.InnerText = string.Concat(errorCodeAttr.InnerText,testValue);
						break;
					}
				}
			}
			
			if(bNodeValue)
				ErrorCodeNode.InnerText = string.Concat(ErrorCodeNode.InnerText,testValue);
								
		}

		/// <summary>
		/// EditException
		/// </summary>
		/// <param name="ExceptionNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditException(XmlNode ExceptionNode,string testValue,string flags)
		{
			bool bName = false;
			
									
			String[] szTokens = flags.Split();
			foreach(String flag in szTokens)
			{
				switch(flag)
				{
					case "Name":
					{
						bName = true;
						break;
					}
				}
			}
				
			if(bName)
				ExceptionNode.InnerText = string.Concat(ExceptionNode.InnerText,testValue);
								
		}

		/// <summary>
		/// EditParam
		/// </summary>
		/// <param name="ParamNode"></param>
		/// <param name="testValue"></param>
		/// <param name="flags"></param>
		public void EditParam(XmlNode ParamNode,string testValue,string flags)
		{
			bool bIndex = false;
			bool bType = false;
			bool bCompareAs = false;
			bool bAccess = false;
			bool bName = false;
			
									
			String[] szTokens = flags.Split();
			foreach(String flag in szTokens)
			{
				switch(flag)
				{
					case "Index":
					{
						bIndex = true;
						break;
					}
					case "Type":
					{
						bType = true;
						break;
					}
					case "CompareAs":
					{
						bCompareAs = true;
						break;
					}
					case "Access":
					{
						bAccess = true;
						break;
					}
					case "Name":
					{
						bName = true;
						break;
					}
				}
			}
			
			foreach(XmlAttribute paramAttr in ParamNode.Attributes)
			{
				switch(paramAttr.Name)
				{
					case "Index":
					{
						if(bIndex)
							paramAttr.InnerText = string.Concat(paramAttr.InnerText,testValue);
						break;
					}
					case "Type":
					{
						if(bType)
							paramAttr.InnerText = string.Concat(paramAttr.InnerText,testValue);
						break;
					}
					case "CompareAs":
					{
						if(bCompareAs)
							paramAttr.InnerText = string.Concat(paramAttr.InnerText,testValue);
						break;
					}
					case "Access":
					{
						if(bAccess)
							paramAttr.InnerText = string.Concat(paramAttr.InnerText,testValue);
						break;
					}
				}
			}

			if(bName)
				ParamNode.InnerText = string.Concat(ParamNode.InnerText,testValue);
								
		}
		
	}
}
