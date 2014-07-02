using System;
using System.Collections;

namespace TestingFunctionXML
{
	
	class EntryPoint
	{
		
		static void Main(string[] args)
		{
			Console.WriteLine("This is Test Appliacation to test Function.xml");	
			FunctionXMLNavigator FuncXMLNav = new FunctionXMLNavigator();
			string FunctionXMLFilePath = @"E:\Fall2003\Holodeck xml files\functions.xml";
			FuncXMLNav.parseXmlDocument(FunctionXMLFilePath);

			//modifying function here
			Function modifyThis = FuncXMLNav.GetFunctionByName( "bind" );
			modifyThis.OriginalDll = "TESTws2_32.dll";
			modifyThis.InterceptedDll = "TESTws2_32replacementlibrary.dll";
			modifyThis.ReplacementFunctionName = "TESTbindReplacement";
			modifyThis.ReturnType = "TESTint";
			modifyThis.Modifiers[0] = "TESTWINSOCK_API_LINKAGE";
			modifyThis.Modifiers[1] = "TESTWSAAPI";
			modifyThis.Category = "TESTNETWORK";
			modifyThis.ReturnValueDescription = "TESTOn success returns zero. On failure returns SOCKET_ERROR (-1).";
			
			Parameter modifiedParam = new Parameter();
			modifiedParam.Name ="TESTs" ;
			modifiedParam.Index = 01;
			modifiedParam.Type ="TESTSOCKET" ;
			modifiedParam.CompareAs ="TESTPointerType" ;
			modifiedParam.Access ="TESTIN" ;
			modifyThis.Parameter[0] = modifiedParam;

			Parameter modifiedParam1 = new Parameter();
			modifiedParam1.Name ="TESTname" ;
			modifiedParam1.Index = 11;
			modifiedParam1.Type ="TESTconst struct sockaddr FAR *" ;
			modifiedParam1.CompareAs ="TESTPointerType" ;
			modifiedParam1.Access ="TESTIN" ;
			modifyThis.Parameter[1] = modifiedParam1;
			
			Parameter modifiedParam2 = new Parameter();
			modifiedParam2.Name = "TESTnamelen" ;
			modifiedParam2.Index = 22;
			modifiedParam2.Type ="TESTint" ;
			modifiedParam2.CompareAs ="TESTIntegerType" ;
			modifiedParam2.Access ="TESTIN" ;
			modifyThis.Parameter[2] = modifiedParam2;
			
			FuncXMLNav.UpdateFunctionByName("bind",modifyThis);
			 
			//saving the function.xml
			FuncXMLNav.saveFunctionXmlDocument(FuncXMLNav,"functions.xml");


			// DotNetFunctions.xml
			Console.WriteLine("This is Test Appliacation to test DotNetFunctions.xml");	
			FunctionXMLNavigator DotNetFuncXMLNav = new FunctionXMLNavigator();
			string DotNetFunctionXMLFilePath = @"E:\Fall2003\Holodeck xml files\DotNetFunctions.xml";
			DotNetFuncXMLNav.parseXmlDocument(DotNetFunctionXMLFilePath);
			DotNetFuncXMLNav.saveFunctionXmlDocument(DotNetFuncXMLNav,"DotNetFunctions.xml");
			

		}
	}
}
