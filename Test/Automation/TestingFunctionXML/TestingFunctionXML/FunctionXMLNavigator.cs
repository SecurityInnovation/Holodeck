///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		FunctionXMLNavigator.cs
///
/// DESCRIPTION: Contains properties and method implementation for the class FunctionXMLNavigator.
///
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 08 sept 2003	  V. Parikh		File created.
///*************************************************************************

using  System;
using  System.IO;
using  System.Xml;
using  System.Xml.XPath;
using  System.Xml.Schema;
using  System.Collections;


namespace TestingFunctionXML
{
	/// *************************************************************
	/// <summary>
	/// Summary description for FunctionXMLNavigator:
	///
	/// Contains functionality to get data from "Function.Xml" File 
	/// </summary>
	/// *************************************************************
	public class FunctionXMLNavigator
	{
		Hashtable FunctionTableByName;
				
		public FunctionXMLNavigator()
		{
			this.FunctionTableByName = new Hashtable();
		}
	
		

		///*************************************************************************
		/// Method:		parseXmlDocument
		/// Description: parses the xml document and extracts a function from it
		///
		/// Parameters:
		///	xmlFileName - the path of the function.xml file
		///
		/// Return Value: true if successful, false otherwise
		///*************************************************************************
		public bool parseXmlDocument(string xmlFileName)
		{
			//ValidateXmlDocument(xmlFileName);
			this.FunctionTableByName.Clear();

			
			XPathDocument doc = new XPathDocument(xmlFileName);
			XPathNavigator rootNavigator = doc.CreateNavigator();
			
			bool hasmoreFunctions = rootNavigator.HasChildren; 
			if (hasmoreFunctions)
			{
				// move to the function tag
				rootNavigator.MoveToFirstChild();
				rootNavigator.MoveToFirstChild();
			}
			while(hasmoreFunctions)
			{
				this.AddFunctionToTableByName(this.ParseFunction(rootNavigator));
				hasmoreFunctions = rootNavigator.MoveToNext();
			}
			
			return true;
		    

		}


		///*************************************************************************
		/// Method:		ValidateXmlDocument
		/// Description: loads the xml document and validates it using function.dtd
		///
		/// Parameters:
		///	xmlFileName - the path of the function.xml file
		/// Return Value : None 
		///*************************************************************************
		public void ValidateXmlDocument(String xmlFileName)
		{
			XmlTextReader reader = new XmlTextReader(xmlFileName);
            XmlValidatingReader validator = new XmlValidatingReader(reader);

			validator.ValidationType = ValidationType.DTD;
			validator.ValidationEventHandler += new ValidationEventHandler(this.ValidationHandler);

			// do nothing in this loop, all errors will be handled by ValidationHandler
			while (validator.Read())
			{
				
			}

		}


		///*************************************************************************
		/// Method:		ValidationHandler
		/// Description: callback function that handles any xml document validation 
		///	errors
		///
		/// Parameters:
		///	sender - the object that sent the validation error
		///	args - the arguments that indicate the validation error
		///
		/// Return Value: None
		///*************************************************************************
		protected void ValidationHandler(Object sender, ValidationEventArgs args)
		{
			throw new System.Exception(args.Message);
		}


		///*************************************************************************
		/// Method:		ParseFunction
		/// Description: parses the xml document and extracts a function from it
		///
		/// Parameters:
		///	childNaviator - the path navigator that represents the function node
		///		to extract
		///
		/// Return Value: the extracted function
		///*************************************************************************
		protected Function ParseFunction(XPathNavigator childNavigator)
		{
			XPathNavigator functionNavigator = childNavigator.Clone();
			
			Function function = new Function();
			
			if (functionNavigator.NodeType == XPathNodeType.Comment)
				return null;

			bool hasMoreFunctionElements = functionNavigator.MoveToFirstChild();
			
			while (hasMoreFunctionElements)
			{
				
				switch (functionNavigator.Name)
				{
					case "FunctionName":
					{
						function.FunctionName = functionNavigator.Value;
						break;
					}
					
					case "OriginalDll":
					{
						function.OriginalDll = functionNavigator.Value;
						break;
					}
					
					case "InterceptedDll":
					{
						function.InterceptedDll = functionNavigator.Value;
						break;
					}

					case "ReplacementFunctionName":
					{
						function.ReplacementFunctionName = functionNavigator.Value;
						break;
					}
					case "ReturnType":
					{
						function.ReturnType = functionNavigator.Value;
						break;
					}
					case "Modifier": 
					{
						function.Modifiers.Add(functionNavigator.Value);
						break;
					}
					case "Category":
					{
						function.Category = functionNavigator.Value;
						break;
					}

					case "ReturnValue":
					{
						ReturnValue returnValue = ParseReturnValue(functionNavigator);
						if(returnValue != null)
							function.ReturnValue.Add(returnValue);
						break;
					}
					
					case "ErrorCode": 
					{	
						ErrorCode errorCode = ParseErrorCode(functionNavigator);
						if (errorCode != null)
							function.ErrorCode.Add(errorCode);

						break;
					}

					case "Exception":
					{
						function.Exception.Add(functionNavigator.Value);
						break;
					}
					case "HelpLink":
					{
						function.HelpLink = functionNavigator.Value;
						break;
					}
					case "Param":
					{
						Parameter param = ParseParameter(functionNavigator);
						if (param != null)
							function.Parameter.Add(param);
						break;
					}
						
				}		
				
				hasMoreFunctionElements = functionNavigator.MoveToNext();
			}

			return function;

		}

		
		///*************************************************************************
		/// Method:		ParseReturnValue
		/// Description: parses the xml document and extracts an ReturnValue
		///
		/// Parameters:
		///	childNaviator - the path navigator that represents the ReturnValue node
		///		            to extract
		///	
		/// Return Value: the extracted ReturnValue
		///*************************************************************************
		protected ReturnValue ParseReturnValue(XPathNavigator childNavigator)
		{
			XPathNavigator ecNavigator = childNavigator.Clone();
			
			ReturnValue returnValue = new ReturnValue();
			returnValue.NodeValue = ecNavigator.Value;
			
			returnValue.Type = null;

			// get the attributes of the ReturnValue tag
			bool hasMoreAttributes = ecNavigator.MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (ecNavigator.Name.CompareTo("Type") == 0)
				{
					returnValue.Type = ecNavigator.Value;
					
				}

				hasMoreAttributes = ecNavigator.MoveToNextAttribute();
			}

			// get back to the ReturnValue tag
			ecNavigator.MoveToParent();

			return returnValue;

		}



		///*************************************************************************
		/// Method:		ParseErrorCode
		/// Description: parses the xml document and extracts an error code value
		///
		/// Parameters:
		///	childNaviator - the path navigator that represents the error code node
		///		            to extract
		///
		/// Return Value: the extracted ErrorCode
		///*************************************************************************
		protected ErrorCode ParseErrorCode(XPathNavigator childNavigator)
		{
			XPathNavigator ecNavigator = childNavigator.Clone();
			
			ErrorCode errorCode = new ErrorCode();
			errorCode.NodeValue = ecNavigator.Value;
			
			errorCode.IdentifiedBy = null;

			// get the attributes of the error code tag
			bool hasMoreAttributes = ecNavigator.MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (ecNavigator.Name.CompareTo("IdentifyBy") == 0)
				{
					errorCode.IdentifiedBy = ecNavigator.Value;
					
				}
				if (ecNavigator.Name.CompareTo("Type") == 0)
				{
					errorCode.Type = ecNavigator.Value;
					
				}

				hasMoreAttributes = ecNavigator.MoveToNextAttribute();
			}
			// get back to the error code tag
			ecNavigator.MoveToParent();

			return errorCode;

		}


		///*************************************************************************
		/// Method:		ParseParameter
		/// Description: parses the xml document and extracts a parameter from it
		///
		/// Parameters:
		///	childNaviator - the path navigator that represents the parameter node
		///		            to extract
		///
		///  Return Value: the extracted function parameter
		///*************************************************************************
		protected Parameter ParseParameter(XPathNavigator childNavigator)
		{
			XPathNavigator paramNavigator = childNavigator.Clone();
			Parameter parameter = new Parameter();

			parameter.Name = paramNavigator.Value;

			// get the attributes of the Param tag
			bool hasMoreAttributes = paramNavigator.MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				switch(paramNavigator.Name)
				{
					case "Index":
					{
						parameter.Index = Int32.Parse(paramNavigator.Value);
						break;

					}
					case "Type":
					{
						parameter.Type = paramNavigator.Value;
						break;
					}
					case "CompareAs":
					{
						parameter.CompareAs = paramNavigator.Value;
						break;
					}
					case "Access":
					{
						parameter.Access = paramNavigator.Value;
						break;
					}

				}

				hasMoreAttributes = paramNavigator.MoveToNextAttribute();
			}

			// get back to the Parameter tag
			paramNavigator.MoveToParent();

			return parameter;
		}


		///*************************************************************************
		/// Method:		AddFunctionToTableByName
		/// Description: Adding Functions to hashtable by functionName as Key
		///
		/// Parameters:
		///	newFunctionToAdd - the object of type Function Class
		///
		///  Return Value: true if successful, false otherwise
		///*************************************************************************
		protected bool AddFunctionToTableByName(Function newFunctionToAdd)
		{
			FunctionTableByName.Add(newFunctionToAdd.FunctionName,newFunctionToAdd);
			return true;

		}


		///*************************************************************************
		/// Method:		GetFunctionByName
		/// Description: Getting Functions from hashtable as function type object
		///
		/// Parameters:
		///	FunctionName - FunctionName - Key for HashTable
		///
		///  Return Value:  object of Function Class
		///*************************************************************************
		public Function GetFunctionByName( string FunctionName )
		{
			return (Function)FunctionTableByName[FunctionName];
		}

		
		///*************************************************************************
		/// Method:		UpdateFunctionByName
		/// Description: Updating entry of the edited function in FunctionTableByName 
		///
		/// Parameters:
		///	FunctionName = name of the edited function here acting as Key in hashtable
		///  EditedFunction = Object of edited function 
		///
		///  Return Value:  None
		///*************************************************************************
		public void UpdateFunctionByName(string FunctionName, Function EditedFunction)
		{
			if(FunctionTableByName.Contains(FunctionName))
			{
				FunctionTableByName.Remove(FunctionName);
				
				FunctionTableByName.Add(EditedFunction.FunctionName,EditedFunction);
			}
			else
			{
				FunctionTableByName.Add(EditedFunction.FunctionName,EditedFunction);
			}
		}
	
		
		///*************************************************************************
		/// Method:		saveFunctionXmlDocument
		/// Description: recreating functions.xml document 
		///
		/// Parameters:
		///	functionXMLNavigator :
		///
		///	fileNameToSaveAs : filename
		///
		///  Return Value:  None
		///*************************************************************************
		public void saveFunctionXmlDocument(FunctionXMLNavigator functionXMLNavigator, string fileNameToSaveAs,string fileEncoding,bool isValidationRequired)
		{
			XmlTextWriter saveFunctionXml;
			
			switch(fileEncoding.ToUpper())
			{
				case "UTF-8":
				case "":
				{
					saveFunctionXml = new XmlTextWriter(fileNameToSaveAs,System.Text.UTF8Encoding.UTF8);
					saveFunctionXml.Formatting = Formatting.Indented;
					saveFunctionXml.WriteRaw( "<?xml version= \"1.0\"?>" );
					break;
				}
				case "UTF-7":
				{
					saveFunctionXml = new XmlTextWriter(fileNameToSaveAs,System.Text.UTF7Encoding.UTF7);
					saveFunctionXml.Formatting = Formatting.Indented;
					saveFunctionXml.WriteRaw( "<?xml version= \"1.0\" encoding=\"UTF-7\"?>" );
					break;
				}
				case "ASCII":
				{
					saveFunctionXml = new XmlTextWriter(fileNameToSaveAs,System.Text.ASCIIEncoding.ASCII);
					saveFunctionXml.Formatting = Formatting.Indented;
					saveFunctionXml.WriteRaw( "<?xml version= \"1.0\" encoding=\"ASCII\"?>" );
					break;
				}
				case "Unicode":
				{
					saveFunctionXml = new XmlTextWriter(fileNameToSaveAs,System.Text.UnicodeEncoding.Unicode);
					saveFunctionXml.Formatting = Formatting.Indented;
					saveFunctionXml.WriteRaw( "<?xml version= \"1.0\" encoding=\"Unicode\"?>" );
					break;
				}
				default:
				{
					saveFunctionXml = new XmlTextWriter(fileNameToSaveAs,null);
					saveFunctionXml.Formatting = Formatting.Indented;
					saveFunctionXml.WriteRaw( "<?xml version= \"1.0\"?>" );
					break;
				}

			}

			if(isValidationRequired)
			{
				saveFunctionXml.WriteDocType("Functions",null,"functions.dtd","");
			}

			saveFunctionXml.WriteStartElement("Functions");

			
			foreach(string FunctionNameAsKey in FunctionTableByName.Keys)
			{
				Function FunctionToSave = functionXMLNavigator.GetFunctionByName(FunctionNameAsKey);
								
				///Element = Function
				saveFunctionXml.WriteStartElement("Function");
				
								
				//Element = FunctionName
				saveFunctionXml.WriteStartElement("FunctionName");
				saveFunctionXml.WriteString(FunctionToSave.FunctionName.ToString());
				saveFunctionXml.WriteFullEndElement();

				//Element = OriginalDll
				saveFunctionXml.WriteStartElement("OriginalDll");
				saveFunctionXml.WriteString(FunctionToSave.OriginalDll.ToString());
				saveFunctionXml.WriteFullEndElement();
				
				//Element = InterceptedDll
				saveFunctionXml.WriteStartElement("InterceptedDll");
				saveFunctionXml.WriteString(FunctionToSave.InterceptedDll.ToString());
				saveFunctionXml.WriteFullEndElement();
				
				//Element = ReplacementFunctionName
				saveFunctionXml.WriteStartElement("ReplacementFunctionName");
				saveFunctionXml.WriteString(FunctionToSave.ReplacementFunctionName.ToString());
				saveFunctionXml.WriteFullEndElement();

				//Element = ReturnType
				saveFunctionXml.WriteStartElement("ReturnType");
				saveFunctionXml.WriteString(FunctionToSave.ReturnType.ToString());
				saveFunctionXml.WriteFullEndElement();
				
				//Element = Modifier
				for(int indexModifier =0; indexModifier < FunctionToSave.Modifiers.Count;indexModifier ++)
				{
					saveFunctionXml.WriteStartElement("Modifier");
					saveFunctionXml.WriteString(FunctionToSave.Modifiers[indexModifier].ToString());
					saveFunctionXml.WriteFullEndElement();
				}
				
				//Element = Category
				saveFunctionXml.WriteStartElement("Category");
				saveFunctionXml.WriteString(FunctionToSave.Category.ToString());
				saveFunctionXml.WriteFullEndElement();
		
				///Element = ReturnValue
				foreach(ReturnValue returnValue in FunctionToSave.ReturnValue)
				{
					///Element = ReturnValue 
					saveFunctionXml.WriteStartElement("ReturnValue");
					
					///Attribute = IdentifyBy
					if(returnValue.Type != null)
					{
						saveFunctionXml.WriteAttributeString("Type",returnValue.Type.ToString());
					}

					saveFunctionXml.WriteString(returnValue.NodeValue.ToString());
					saveFunctionXml.WriteFullEndElement();
				}

				///Element = Exception
				for(int indexException =0; indexException < FunctionToSave.Exception.Count;indexException ++)
				{
					saveFunctionXml.WriteStartElement("Exception");
					saveFunctionXml.WriteString(FunctionToSave.Exception[indexException].ToString());
					saveFunctionXml.WriteFullEndElement();
				}

				/// Element = HelpLink
				saveFunctionXml.WriteStartElement("HelpLink");
				saveFunctionXml.WriteString(FunctionToSave.HelpLink.ToString());
				saveFunctionXml.WriteFullEndElement();

				/// Element = Param
				foreach(Parameter param in FunctionToSave.Parameter)
				{
					///Element = Param 
					saveFunctionXml.WriteStartElement("Param");
					
					///Attribute = Index
					if(param.Index != -1)
					{
						saveFunctionXml.WriteAttributeString("Index",param.Index.ToString());
					}

					///Attribute = Type
					if(param.Type != null)
					{
						saveFunctionXml.WriteAttributeString("Type",param.Type.ToString());
					}				

					///Attribute = CompareAs
					if(param.CompareAs != null)
					{
						saveFunctionXml.WriteAttributeString("CompareAs",param.CompareAs.ToString());
					}

					///Attribute = Access
					if(param.Access != null)
					{
						saveFunctionXml.WriteAttributeString("Access",param.Access.ToString());
					}

					saveFunctionXml.WriteString(param.Name.ToString());
					saveFunctionXml.WriteFullEndElement();

				}			

				///Element = ErrorCode
				foreach(ErrorCode errorCode in FunctionToSave.ErrorCode)
				{
					///Element = ErrorCode 
					saveFunctionXml.WriteStartElement("ErrorCode");
					
					///Attribute = IdentifyBy
					if(errorCode.IdentifiedBy != null)
					{
						saveFunctionXml.WriteAttributeString("IdentifyBy",errorCode.IdentifiedBy.ToString());
					}

					///Attribute = Type
					if(errorCode.Type != null)
					{
						saveFunctionXml.WriteAttributeString("Type",errorCode.Type.ToString());
					}
	
					saveFunctionXml.WriteString(errorCode.NodeValue.ToString());
					saveFunctionXml.WriteFullEndElement();
				}
			
				saveFunctionXml.WriteFullEndElement();		

			}
			saveFunctionXml.WriteFullEndElement();	
			
			//closing xmlwriter.
			saveFunctionXml.Close();
		}
		
	}
}
