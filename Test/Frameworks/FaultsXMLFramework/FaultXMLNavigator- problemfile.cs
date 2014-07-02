using  System;
using  System.Xml;
using  System.Xml.XPath;
using  System.Xml.Schema;
using  System.Collections;

namespace TestingFaultsXML
{
	/// <summary>
	/// Summary description for FaultXMLNavigator.
	/// *************************************************************
	/// Contains functionality to get data from "Fault.Xml" File 
	/// *************************************************************
	/// /// </summary>
	public class FaultXMLNavigator
	{
		Hashtable FaultTableByName;
		
		public FaultXMLNavigator()
		{
			this.FaultTableByName = new Hashtable();	
		}

		//*************************************************************************
		// Method:		loadXmlDocument
		// Description: loads the xml document and validates it using faults.dtd
		//
		// Parameters:
		//	xmlFileName - the path of the faults.xml file
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
		// Method:		parseXmlDocument
		// Description: parses the xml document and extracts a faults from it
		//
		// Parameters:
		//	xmlFileName - the path of the faults.xml file
		//
		// Return Value: true if successful, false otherwise
		//*************************************************************************
		public bool parseXmlDocument(string xmlFileName)
		{
			loadXmlDocument(xmlFileName);
			this.FaultTableByName.Clear();

			XPathDocument doc = new XPathDocument(xmlFileName);
			XPathNavigator rootNavigator = doc.CreateNavigator();
			
			bool hasmoreFaults = rootNavigator.HasChildren; 
			if (hasmoreFaults)
			{
                
				// move till end of comment tag
				rootNavigator.MoveToFirstChild();
				while(rootNavigator.NodeType == XPathNodeType.Comment)
					rootNavigator.MoveToNext();
				
			}
			
			// move to fault tag
			rootNavigator.MoveToNext();
			
			rootNavigator.MoveToFirstChild();

			while(hasmoreFaults)
			{
				this.AddFaultToTableByName(this.ParseFault(rootNavigator));
				hasmoreFaults = rootNavigator.MoveToNext();
			}

			return true; 

		}
		
		
		//*************************************************************************
		// Method:		ParseFault
		// Description: parses the xml document and extracts a fault from it
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the fault node
		//		to extract
		//
		// Return Value: the extracted fault
		//*************************************************************************
		protected Fault ParseFault(XPathNavigator childNavigator)
		{
			XPathNavigator faultNavigator = childNavigator.Clone();
			
			Fault fault = new Fault();

			// parsing fault tag attributes
			bool hasMoreAttributes = faultNavigator.MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				switch(faultNavigator.Name)
				{
					case "Name":
					{
						fault.Name = faultNavigator.Value;
						break;
					}
					
					case "ReturnValue":
					{
						fault.ReturnValue = faultNavigator.Value;
						break;
					}
					
					case "ErrorCode":
					{
						fault.ErrorCode = faultNavigator.Value;
						break;
					}
				}

				hasMoreAttributes = faultNavigator.MoveToNextAttribute();
			}

			// get back to the fault tag
			faultNavigator.MoveToParent();
			
			bool hasMoreFaultElements = faultNavigator.MoveToFirstChild();
			while (hasMoreFaultElements)
			{
				if (faultNavigator.Name.CompareTo("Function") == 0)
				{
					FaultFunction function = ParseFaultFunction(faultNavigator);
					if (function != null)
						fault.Function.Add(function);
				}

				hasMoreFaultElements = faultNavigator.MoveToNext();
			}

			return fault;
		}
		
		
		//*************************************************************************
		// Method:		ParseFaultFunction
		// Description: parses the xml document and extracts a fault function from it
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the fault function node
		//		to extract
		//	returnValue - the default return value to set for the function if not 
		//		overridden in the xml
		//	errorCode - the default errorCode to set for the function if not overridden
		//		in the xml
		//
		// Return Value: the extracted fault function
		//*************************************************************************
		protected FaultFunction ParseFaultFunction(XPathNavigator childNavigator)
		{
			XPathNavigator functionNavigator = childNavigator.Clone();
			FaultFunction function = new FaultFunction();

			function.Name = functionNavigator.Value;

			// get the attributes of the function tag
			bool hasMoreAttributes = functionNavigator.MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				switch(functionNavigator.Name)
				{
					case "Name":
					{
						function.Name = functionNavigator.Value;
						break;
					}
					case "OverrideErrorCode":
					{
						function.OverrideErrorCode = functionNavigator.Value;
						break;
					}
					case "OverrideReturnValue":
					{
						function.OverrideReturnValue = functionNavigator.Value;
						break;
					}
					case "PassThrough":
					{
						function.PassThrough = functionNavigator.Value;
						break;
					}
					case "Exception":
					{
						function.Exception = functionNavigator.Value;
						break;
					}
					case "Allocation":
					{
						function.Allocation = functionNavigator.Value;
						break;
					}
				}

				hasMoreAttributes = functionNavigator.MoveToNextAttribute();
			}
			
			// get back to the function tag
			functionNavigator.MoveToParent();
			
			bool hasMoreFunctionElements = functionNavigator.MoveToFirstChild();
			while (hasMoreFunctionElements)
			{
				switch (functionNavigator.Name)
				{
					case "MatchParams":
					{
						FaultFunctionMatchParams FunctionMatchParams = ParseFaultFunctionMatchParams(functionNavigator);
						if (FunctionMatchParams != null)
							function.MatchParams.Add(FunctionMatchParams);
						break;

					}
					case "CheckResource":
					{
						FaultFunctionCheckResource FunctionCheckResource = ParseFaultFunctionCheckResource(functionNavigator);
						if (FunctionCheckResource != null)
							function.CheckResource.Add(FunctionCheckResource);
						break;
					}
				}

				hasMoreFunctionElements = functionNavigator.MoveToNext();
			}

			functionNavigator.MoveToParent();

			return function;
		}
		
		
		//*************************************************************************
		// Method:		FaultFunctionCheckResource
		// Description: Extracts a fault function  CheckResource from fault xml
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the fault function  CheckResource node
		//		to extract
		//
		// Return Value: the extracted FaultFunctionCheckResource function
		//*************************************************************************
		protected FaultFunctionCheckResource ParseFaultFunctionCheckResource(XPathNavigator childNavigator)
		{
			XPathNavigator CheckResourceNavigator = childNavigator.Clone();
			FaultFunctionCheckResource CheckResource = new FaultFunctionCheckResource();
			
			// get the attributes of the function tag
			bool hasMoreCheckResourceAttributes = CheckResourceNavigator.MoveToFirstAttribute();
			while(hasMoreCheckResourceAttributes)
			{
				switch(CheckResourceNavigator.Name)
				{
					case "ParamIndex":
					{
						CheckResource.ParamIndex = CheckResourceNavigator.Value;
						break;
					}
					case "Exists":
					{
						CheckResource.Exists = CheckResourceNavigator.Value;
						break;
					}
				}

				hasMoreCheckResourceAttributes = CheckResourceNavigator.MoveToNextAttribute();
			}
			
			return CheckResource;

		}
		
		
		//*************************************************************************
		// Method:		ParseFaultFunctionMatchParams
		// Description: Extracts a fault function  Match Params from fault xml
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the fault function  match param node
		//		to extract
		//
		// Return Value: the extracted FaultFunctionMatchParams function
		//*************************************************************************
		protected FaultFunctionMatchParams ParseFaultFunctionMatchParams(XPathNavigator childNavigator)
		{
			XPathNavigator MatchParamsNavigator = childNavigator.Clone();
			FaultFunctionMatchParams MatchParams = new FaultFunctionMatchParams();
			
			// Element "MatchPara"
			bool hasMoreMatchParamsElements = MatchParamsNavigator.MoveToFirstChild();
			while (hasMoreMatchParamsElements)
			{
				switch (MatchParamsNavigator.Name)
				{
					case "MatchParam":
					{
						// recursivly calling "ParseFaultFunctionMatchParams" method
						FaultFunctionMatchParams attMatchParam = ParseMatchParam(MatchParamsNavigator);
						if (attMatchParam != null)
							MatchParams.MatchParam.Add(attMatchParam);
						break;

					}
				}
				
				hasMoreMatchParamsElements = MatchParamsNavigator.MoveToNext();
			}
			
			return MatchParams;
		}

		
		//*************************************************************************
		// Method:		ParseMatchParam
		// Description: Extracts a Match Param from Fault function Match Params
		//
		// Parameters:
		//	childNaviator - the path navigator that represents the fault function match params node
		//
		// Return Value: the extracted FaultFunctionMatchParams  matchParam 
		//*************************************************************************
		protected FaultFunctionMatchParams ParseMatchParam (XPathNavigator childNavigator)
		{
			XPathNavigator MatchParamNavigator = childNavigator.Clone();
			FaultFunctionMatchParams matchParam = new FaultFunctionMatchParams();

			// get the attributes of the function tag
			bool hasMoreMatchParamAttributes = MatchParamNavigator.MoveToFirstAttribute();
			while(hasMoreMatchParamAttributes)
			{
				switch(MatchParamNavigator.Name)
				{
					case "Name":
					{
						matchParam.Name = MatchParamNavigator.Value;
						break;
					}
					case "TestOperator":
					{
						matchParam.TestOperator = MatchParamNavigator.Value;
						break;
					}
					case "TestValue":
					{
						matchParam.TestValue = MatchParamNavigator.Value;
						break;
					}
					case "CompareAsType":
					{
						matchParam.CompareAsType = MatchParamNavigator.Value;
						break;
					}
					case "ID":
					{
						matchParam.ID = MatchParamNavigator.Value;
						break;
					}

				}

				hasMoreMatchParamAttributes = MatchParamNavigator.MoveToNextAttribute();
			}
			
			return matchParam;
		}
		
		
		//*************************************************************************
		// Method:		AddFaultToTableByName
		// Description: Adding Fault to hashtable by FaultName as Key
		//
		// Parameters:
		//	newFaultToAdd - the object of type Fault Class
		//
		//  Return Value: true if successful, false otherwise
		//*************************************************************************
		protected bool AddFaultToTableByName(Fault newFaultToAdd)
		{
			
			FaultTableByName.Add(newFaultToAdd.Name,newFaultToAdd);
			return true;
			
		}

		
		//*************************************************************************
		// Method:		GetFaultByName
		// Description: Getting Fault from hashtable as Fault type object
		//
		// Parameters:
		//	FunctionName - FaultName - Key for HashTable
		//
		//  Return Value:  object of Function Class
		//*************************************************************************
		public Fault GetFaultByName( string FaultName )
		{
			return (Fault)FaultTableByName[FaultName];
		}

		//*************************************************************************
		// Method:		UpdateFault
		// Description: Updating entry of the edited Fault in FaultTableByName 
		//
		// Parameters:
		//	FaultName = name of the edited Fault here acting as Key in hashtable
		//  EditedFault = Object of edited Fault 
		//
		//  Return Value:  None
		//*************************************************************************
		public void UpdateFunctionByName(string FaultName, Fault EditedFault)
		{
			if(FaultTableByName.Contains(FaultName))
			{
				FaultTableByName.Remove(FaultName);
				
				FaultTableByName.Add(EditedFault.Name,EditedFault);
			}
			else
			{
				FaultTableByName.Add(EditedFault.Name,EditedFault);
			}
		}
		
		//*************************************************************************
		// Method:		saveFaultXmlDocument
		// Description: recreating fault.xml document 
		//
		// Parameters:
		//	faultXMLNavigator 
		//
		//  Return Value:  None
		//*************************************************************************
		public void saveFaultXmlDocument(FaultXMLNavigator faultXMLNavigator, string fileNameToSaveAs)
		{
						
			XmlTextWriter saveFaultXml = new XmlTextWriter(fileNameToSaveAs,null);
			saveFaultXml.Formatting = Formatting.Indented;

			saveFaultXml.WriteRaw( "<?xml version= \"1.0\"?>" );
			
			saveFaultXml.WriteDocType("Faults",null,"faultsNew.dtd","");
			
			saveFaultXml.WriteStartElement("Faults");

			foreach(string FaultNameAsKey in FaultTableByName.Keys)
			{
				Fault FaultToSave = faultXMLNavigator.GetFaultByName(FaultNameAsKey);
								
				///Element = Fault
				saveFaultXml.WriteStartElement("Fault");
				
				///Attribute = Name
				if(FaultToSave.Name != null)
					saveFaultXml.WriteAttributeString("Name",FaultToSave.Name);
				Console.WriteLine("{0}",FaultToSave.Name.ToString());
			
				///Attribute = ReturnValue
				if(FaultToSave.ReturnValue != null)
					saveFaultXml.WriteAttributeString("ReturnValue",FaultToSave.ReturnValue);
				Console.WriteLine("{0}",FaultToSave.ReturnValue.ToString());
			
				///Attribute = ErrorCode
				if(FaultToSave.ErrorCode != null)
					saveFaultXml.WriteAttributeString("ErrorCode",FaultToSave.ErrorCode);
				Console.WriteLine("{0}",FaultToSave.ErrorCode.ToString());
			
				saveFaultXml.WriteEndElement();	
				Console.WriteLine("{0}",FaultToSave.Name.ToString());

				Console.WriteLine("============");
			
			}
			//closeing Faluts Tag
			saveFaultXml.WriteFullEndElement();

			Console.WriteLine("Save Complete");
			//saveFaultXml.WriteEndDocument();
	
			

		}
	}
}
