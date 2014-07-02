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
		//Hashtable EditedFunctionTableByName;
		public FunctionXMLNavigator()
		{
			this.FunctionTableByName = new Hashtable();
		}
	
		

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

		public Function GetFunctionByName( string FunctionName )
		{
			return (Function)FunctionTableByName[FunctionName];
		}

		public void UpdateFunctionByName( string FunctionName, Function NewDefinition )
		{
			FunctionTableByName[FunctionName] = NewDefinition; 

			return;
		}
		
		public void printTable()
		{
			Console.WriteLine("{0}", FunctionTableByName.Count.ToString());
		}
		

		public void displayData()
		{
			Function f = new Function( );
		
			foreach( string key in FunctionTableByName.Keys)
			{
					Console.WriteLine("{0} \t{1}",((Function)FunctionTableByName[key]).FunctionName,((Function)FunctionTableByName[key]).OriginalDll);
			}
		}
	
		
		public void saveFunctionXmlDocument(FunctionXMLNavigator functionXMLNavigator)
		{
			XmlDocument xmlDoc = new XmlDocument();
			
			XmlDeclaration xmlDecl;
			xmlDecl = xmlDoc.CreateXmlDeclaration("1.0",null,null);
//			xmlDecl.Encoding="UTF-8";
//			xmlDecl.Standalone="yes";
			XmlNode docTypeNode = xmlDoc.CreateNode( XmlNodeType.DocumentType, "Functions","functions.dtd" );
			

			XmlElement rootElement = xmlDoc.CreateElement("Functions");

			xmlDoc.AppendChild(rootElement);
			xmlDoc.InsertBefore(xmlDecl,rootElement);
			xmlDoc.InsertBefore(docTypeNode,rootElement);

			foreach(string FunctionNameAsKey in FunctionTableByName.Keys)
			{
				Function FunctionToSave = functionXMLNavigator.GetFunctionByName(FunctionNameAsKey);
								
				///Element = Function
				XmlElement FuncElement = xmlDoc.CreateElement("Function");
				rootElement.AppendChild (FuncElement);
				
				///Element = FunctionName
				XmlElement elmFunctionName = xmlDoc.CreateElement("FunctionName");
				elmFunctionName.InnerText= FunctionToSave.FunctionName;
				FuncElement.AppendChild (elmFunctionName);

				///Element = OriginalDll
				XmlElement elmOriginalDll = xmlDoc.CreateElement("OriginalDll");
				elmOriginalDll.InnerText= FunctionToSave.OriginalDll;
				FuncElement.AppendChild (elmOriginalDll);

				
				///Element = InterceptedDll
				XmlElement elmInterceptedDll = xmlDoc.CreateElement("InterceptedDll");
				elmInterceptedDll.InnerText= FunctionToSave.InterceptedDll;
				FuncElement.AppendChild (elmInterceptedDll);
				
				///Element = ReplacementFunctionName
				XmlElement elmReplacementFunctionName = xmlDoc.CreateElement("ReplacementFunctionName");
				elmReplacementFunctionName.InnerText= FunctionToSave.ReplacementFunctionName;
				FuncElement.AppendChild (elmReplacementFunctionName);

				///Element = ReturnType
				XmlElement elmReturnType = xmlDoc.CreateElement("ReturnType");
				elmReturnType.InnerText= FunctionToSave.ReturnType;
				FuncElement.AppendChild (elmReturnType);


				///Element = Modifier
				for(int indexModifier =0; indexModifier < FunctionToSave.Modifiers.Count;indexModifier ++)
				{
					XmlElement elmModifier = xmlDoc.CreateElement("Modifier");
					elmModifier.InnerText = FunctionToSave.Modifiers[indexModifier].ToString();
					FuncElement.AppendChild (elmModifier);
				}
				
				///Element = Category
				XmlElement elmCategory = xmlDoc.CreateElement("Category");
				elmCategory.InnerText= FunctionToSave.Category;
				FuncElement.AppendChild (elmCategory);

				///Element = ReturnValueDescription
				XmlElement elmReturnValueDescription = xmlDoc.CreateElement("ReturnValueDescription");
				elmReturnValueDescription.InnerText= FunctionToSave.ReturnValueDescription;
				FuncElement.AppendChild (elmReturnValueDescription);
				
				// Element = Param
				foreach(Parameter param in FunctionToSave.Parameter)
				{
					///Element = Param 
					XmlElement elmParam  = xmlDoc.CreateElement("Param");
					elmParam.InnerText = param.Name;
					FuncElement.AppendChild (elmParam);

					//Attribute = Index
					XmlAttribute attrIndex = xmlDoc.CreateAttribute("Index");
					attrIndex.Value = param.Index.ToString();
					elmParam.Attributes.Append (attrIndex);

					//Attribute = Type
					XmlAttribute attrType = xmlDoc.CreateAttribute("Type");
					attrType.Value = param.Type;
					elmParam.Attributes.Append (attrType);

					//Attribute = CompareAs
					XmlAttribute attrCompareAs = xmlDoc.CreateAttribute("CompareAs");
					attrCompareAs.Value = param.CompareAs;
					elmParam.Attributes.Append (attrCompareAs);

					//Attribute = Access
					XmlAttribute attrAccess = xmlDoc.CreateAttribute("Access");
					attrAccess.Value = param.Access;
					elmParam.Attributes.Append (attrAccess);

				}			

				//Element = ErrorCode
				foreach(ErrorCode errorCode in FunctionToSave.ErrorCode)
				{
					///Element = ErrorCode 
					XmlElement elmErrorCode  = xmlDoc.CreateElement("ErrorCode");
					elmErrorCode.InnerText = errorCode.NodeValue;
					FuncElement.AppendChild (elmErrorCode);

					//Attribute = IdentifyBy
					XmlAttribute attrIdentifyBy = xmlDoc.CreateAttribute("IdentifyBy");
					attrIdentifyBy.Value = errorCode.IdentifiedBy;
					elmErrorCode.Attributes.Append (attrIdentifyBy);
				}



			}

			//Saving Xml doc
			xmlDoc.Save("functions.xml");



		}
		
		
	}
}
