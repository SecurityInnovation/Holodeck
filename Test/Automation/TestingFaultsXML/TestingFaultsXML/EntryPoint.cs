//*************************************************************************
// Center for Information Assurance - Florida Institute Of Technology
//
// FILE:		EntryPoint.cs
//
// DESCRIPTION: this is the entry point for the testing faults.xml file
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
	
	class EntryPoint
	{
		
		static void Main(string[] args)
		{
			Console.WriteLine("This is Test Appliacation to test faults.xml");	
			try
			{
				FaultXMLNavigator FaultXMLNav = new FaultXMLNavigator();
				//string FaultXMLFilePath = @"E:\Fall2003\Holodeck xml files\faults.xml";
				string FaultXMLFilePath = @"E:\Fall2003\Holodeck xml files\holo 2.5\faultsnew.xml";
				FaultXMLNav.ValidateXmlDocument(FaultXMLFilePath);
				FaultXMLNav.parseXmlDocument(FaultXMLFilePath);
			
				//modify fault
				Fault modifyThisFault = FaultXMLNav.GetFaultByName("PagingFileTooSmall");

				if(modifyThisFault != null)
				{
					modifyThisFault.ReturnValue = "test_0";
					modifyThisFault.ErrorCode = "Test_1454";
					
					FaultFunction function = new FaultFunction();
					function.Name = "GlobalReAlloc";
					function.Allocation = "test_GT";
					function.PassThrough = "TEST_not_there";
				
					FaultFunctionMatchParams matchparams = new FaultFunctionMatchParams();
					
					FaultFunctionMatchParams matchparam = new FaultFunctionMatchParams();
					matchparam.Name= "Flags";
					matchparam.TestOperator = "test_not contains";
					matchparam.TestValue = "Test_128";
					matchparam.CompareAsType = "test_4";
					matchparam.ID = "test_2";
					
					matchparams.MatchParam.Insert(0,matchparam);

					function.MatchParams.Insert(0,matchparams);

					modifyThisFault.Function[0] = function;
					
					FaultXMLNav.UpdateFault("PagingFileTooSmall",modifyThisFault);

				}

				FaultXMLNav.saveFaultXmlDocument(FaultXMLNav,"fault-save.xml","",true);
			}

			catch(FileNotFoundException f)
			{
				Console.WriteLine(" File Not Found Exception caught.... : " + f.Message);
				Console.WriteLine("Details: " + f.StackTrace);
			}
			catch(SystemException e)
			{
				Console.WriteLine("Caught  System Exception .... : " + e.Message);
				Console.WriteLine("Details: " + e.StackTrace);
			}

		}
	}
}
