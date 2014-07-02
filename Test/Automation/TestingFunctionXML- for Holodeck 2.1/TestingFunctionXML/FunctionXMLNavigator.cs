using  System;
using  System.Xml;
using  System.Xml.XPath;
using  System.Xml.Schema;
using  System.Collections;


namespace TestingFunctionXML
{
	/// <summary>
	/// Summary description for FunctionXMLNavigator.
	/// *************************************************************
	/// Contains functionality to get data from "Function.Xml" File		
	/// *************************************************************
	/// </summary>
	public class FunctionXMLNavigator
	{
		Hashtable FunctionTableByName;
				
		public FunctionXMLNavigator()
		{
			this.FunctionTableByName = new Hashtable();
		}
	
		

		//*************************************************************************
		// Method:		parseXmlDocument
		// Description: parses the xml document and extracts a function from it
		//
		// Parameters:
		//	xmlFileName - the path of the function.xml file
		//
		// Return Value: true if successful, false otherwise
		//*************************************************************************
		public bool parseXmlDocument(string xmlFileName)
		{
			loadXmlDocument(xmlFileName);
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


		//*************************************************************************
		// Method:		loadXmlDocument
		// Description: loads the xml document and validates it using function.dtd
		//
		// Parameters:
		//	xmlFileName - the path of the function.xml file
		// Return Value : None 
		//*************************************************************************
		protected void loadXmlDocument(String xmlFileName)
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


		//*************************************************************************
		// Method:		ValidationHandler
		// Description: callback function that handles any xml document validation 
		//	errors
		//
		// Parameters:
		//	sender - the object that sent the validation error
		//	args - the arguments that indicate the validation error
		//
		// Return Value: None
		//*************************************************************************
		protected void ValidationHandler(Object sender, ValidationEventArgs args)
		{
			throw new System.Exception(args.Message);
		}


		//*************************************************************************
		// Method:		ParseFunction
		// Description: parses the xml document and extracts a function from it
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the function node
		//		to extract
		//
		// Return Value: the extracted function
		//*************************************************************************
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

					case "ReturnValueDescription":
					{
						function.ReturnValueDescription = functionNavigator.Value;
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


		//*************************************************************************
		// Method:		ParseErrorCode
		// Description: parses the xml document and extracts an error code value
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the error code node
		//		            to extract
		//	value - (out) - the value parsed from the xml document
		//
		// Return Value: true if successful, false otherwise
		//*************************************************************************
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

				hasMoreAttributes = ecNavigator.MoveToNextAttribute();
			}
			// get back to the error code tag
			ecNavigator.MoveToParent();

			return errorCode;

		}


		//*************************************************************************
		// Method:		ParseParameter
		// Description: parses the xml document and extracts a parameter from it
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the parameter node
		//		            to extract
		//
		//  Return Value: the extracted intercepted function parameter
		//*************************************************************************
		protected Parameter ParseParameter(XPathNavigator childNavigator)
		{
			XPathNavigator paramNavigator = childNavigator.Clone();
			Parameter parameter = new Parameter();

			parameter.Name = paramNavigator.Value;

			// get the attributes of the fault tag
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

			// get back to the fault tag
			paramNavigator.MoveToParent();

			return parameter;
		}


		//*************************************************************************
		// Method:		AddFunctionToTableByName
		// Description: Adding Functions to hashtable by functionName as Key
		//
		// Parameters:
		//	newFunctionToAdd - the object of type Function Class
		//
		//  Return Value: true if successful, false otherwise
		//*************************************************************************
		protected bool AddFunctionToTableByName(Function newFunctionToAdd)
		{
			FunctionTableByName.Add(newFunctionToAdd.FunctionName,newFunctionToAdd);
			return true;

		}


		//*************************************************************************
		// Method:		GetFunctionByName
		// Description: Getting Functions from hashtable as function type object
		//
		// Parameters:
		//	FunctionName - FunctionName - Key for HashTable
		//
		//  Return Value:  object of Function Class
		//*************************************************************************
		public Function GetFunctionByName( string FunctionName )
		{
			return (Function)FunctionTableByName[FunctionName];
		}

		
		//*************************************************************************
		// Method:		UpdateFunctionByName
		// Description: Updating entry of the edited function in FunctionTableByName 
		//
		// Parameters:
		//	FunctionName = name of the edited function here acting as Key in hashtable
		//  EditedFunction = Object of edited function 
		//
		//  Return Value:  None
		//*************************************************************************
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
	
		
		//*************************************************************************
		// Method:		saveFunctionXmlDocument
		// Description: recreating functions.xml document 
		//
		// Parameters:
		//	functionXMLNavigator 
		//
		//  Return Value:  None
		//*************************************************************************
		public void saveFunctionXmlDocument(FunctionXMLNavigator functionXMLNavigator, string fileNameToSaveAs)
		{
						
			XmlTextWriter saveFunctionXml = new XmlTextWriter(fileNameToSaveAs,null);
			saveFunctionXml.Formatting = Formatting.Indented;

			saveFunctionXml.WriteRaw( "<?xml version= \"1.0\"?>" );
			
			saveFunctionXml.WriteDocType("Functions",null,"functions.dtd","");
			
			saveFunctionXml.WriteStartElement("Functions");


			foreach(string FunctionNameAsKey in FunctionTableByName.Keys)
			{
				Function FunctionToSave = functionXMLNavigator.GetFunctionByName(FunctionNameAsKey);
								
				///Element = Function
				saveFunctionXml.WriteStartElement("Function");
				
								
				//Element = FunctionName
				saveFunctionXml.WriteStartElement("FunctionName");
				saveFunctionXml.WriteString(FunctionToSave.FunctionName);
				saveFunctionXml.WriteFullEndElement();

				//Element = OriginalDll
				saveFunctionXml.WriteStartElement("OriginalDll");
				saveFunctionXml.WriteString(FunctionToSave.OriginalDll);
				saveFunctionXml.WriteFullEndElement();
				
				///Element = InterceptedDll
				saveFunctionXml.WriteStartElement("InterceptedDll");
				saveFunctionXml.WriteString(FunctionToSave.InterceptedDll);
				saveFunctionXml.WriteFullEndElement();
				
				///Element = ReplacementFunctionName
				saveFunctionXml.WriteStartElement("ReplacementFunctionName");
				saveFunctionXml.WriteString(FunctionToSave.ReplacementFunctionName);
				saveFunctionXml.WriteFullEndElement();

				///Element = ReturnType
				saveFunctionXml.WriteStartElement("ReturnType");
				saveFunctionXml.WriteString(FunctionToSave.ReturnType);
				saveFunctionXml.WriteFullEndElement();
				
				///Element = Modifier
				for(int indexModifier =0; indexModifier < FunctionToSave.Modifiers.Count;indexModifier ++)
				{
					saveFunctionXml.WriteStartElement("Modifier");
					saveFunctionXml.WriteString(FunctionToSave.Modifiers[indexModifier].ToString());
					saveFunctionXml.WriteFullEndElement();
				}
				
				///Element = Category
				saveFunctionXml.WriteStartElement("Category");
				saveFunctionXml.WriteString(FunctionToSave.Category);
				saveFunctionXml.WriteFullEndElement();

				///Element = ReturnValueDescription
				saveFunctionXml.WriteStartElement("ReturnValueDescription");
				saveFunctionXml.WriteString(FunctionToSave.ReturnValueDescription);
				saveFunctionXml.WriteFullEndElement();

				///Element = Exception
				for(int indexException =0; indexException < FunctionToSave.Exception.Count;indexException ++)
				{
					saveFunctionXml.WriteStartElement("Exception");
					saveFunctionXml.WriteString(FunctionToSave.Exception[indexException].ToString());
					saveFunctionXml.WriteFullEndElement();
				}

				/// Element = Param
				foreach(Parameter param in FunctionToSave.Parameter)
				{
					///Element = Param 
					saveFunctionXml.WriteStartElement("Param");
					
					///Attribute = Index
					saveFunctionXml.WriteAttributeString("Index",param.Index.ToString());
					
					///Attribute = Type
					saveFunctionXml.WriteAttributeString("Type",param.Type);
									

					///Attribute = CompareAs
					saveFunctionXml.WriteAttributeString("CompareAs",param.CompareAs);
					

					///Attribute = Access
					saveFunctionXml.WriteAttributeString("Access",param.Access);
					
					saveFunctionXml.WriteString(param.Name);
					saveFunctionXml.WriteFullEndElement();

				}			

				///Element = ErrorCode
				foreach(ErrorCode errorCode in FunctionToSave.ErrorCode)
				{
					///Element = ErrorCode 
					saveFunctionXml.WriteStartElement("ErrorCode");
					
					///Attribute = IdentifyBy
					saveFunctionXml.WriteAttributeString("IdentifyBy",errorCode.IdentifiedBy);
	
					saveFunctionXml.WriteString(errorCode.NodeValue);
					saveFunctionXml.WriteFullEndElement();
				}
			
				saveFunctionXml.WriteFullEndElement();		

			}
			saveFunctionXml.WriteFullEndElement();			
		}
		
	}
}
