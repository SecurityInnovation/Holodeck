using System;
using System.IO;
using Microsoft.Win32;
using TestFramework.Run;
using TestFramework.Log;
using TestFramework.Exceptions;
using Holodeck;
using TestingFunctionXML;

namespace FunctionXMLTest
{     
    /// <summary>
    /// 
    /// Inherits from the test control source...
    /// </summary>
	public class FunctionXMLTestn : FunctionXMLTest
	{

		public override void executeTest( )
		{   
			Console.WriteLine("test2");

//			try
//			{
///*
//				String holodeckPath;
//				holodeckPath = (String) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
//				string FunctionXMLFilePath = string.Concat(holodeckPath,"\\functions.xml");
//*/
//				FunctionXMLNavigator FuncXMLNav = new FunctionXMLNavigator();
//			
//				//enter the "Function.xml" path
//				string FunctionXMLFilePath = @"E:\Fall2003\Holodeck xml files\holo 2.5\functions.xml";
//			
//				FuncXMLNav.ValidateXmlDocument(FunctionXMLFilePath);
//				FuncXMLNav.parseXmlDocument(FunctionXMLFilePath);
//
//				//modifying function here
//				Function modifyThis = FuncXMLNav.GetFunctionByName( "bind" );
//
//				//if function not found in original finctions.xml file then it we cannot modify it 
//				// nor we can add new function to the file
//				if(modifyThis != null)
//				{
//					modifyThis.OriginalDll = "TESTws2_32.dll";
//					modifyThis.InterceptedDll = "TESTws2_32replacementlibrary.dll";
//					modifyThis.ReplacementFunctionName = "TESTbindReplacement";
//					modifyThis.ReturnType = "TESTint";
//					modifyThis.Modifiers[0] = "TESTWINSOCK_API_LINKAGE";
//					modifyThis.Modifiers[1] = "TESTWSAAPI";
//					modifyThis.Category = "TESTNETWORK";
//				
//					ReturnValue modifiedReturnValue_0 = new ReturnValue();
//					modifiedReturnValue_0.NodeValue = "TEST_0";
//					modifiedReturnValue_0.Type = "TEST_Success";
//					modifyThis.ReturnValue[0] = modifiedReturnValue_0;
//
//					ReturnValue modifiedReturnValue_1 = new ReturnValue();
//					modifiedReturnValue_1.NodeValue = "TEST_-1(SOCKET_ERROR)";
//					modifiedReturnValue_0.Type= "TEST_Error";
//					modifyThis.ReturnValue[1] = modifiedReturnValue_1;
//
//					modifyThis.HelpLink = @"TEST_http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/winsock/bind_2.asp";
//			
//					Parameter modifiedParam = new Parameter();
//					modifiedParam.Name ="TESTs" ;
//					modifiedParam.Index = 01;
//					modifiedParam.Type ="TESTSOCKET" ;
//					modifiedParam.CompareAs ="TESTPointerType" ;
//					modifiedParam.Access ="TESTIN" ;
//					modifyThis.Parameter[0] = modifiedParam;
//
//					Parameter modifiedParam1 = new Parameter();
//					modifiedParam1.Name ="TESTname" ;
//					modifiedParam1.Index = 11;
//					modifiedParam1.Type ="TESTconst struct sockaddr FAR *" ;
//					modifiedParam1.CompareAs ="TESTPointerType" ;
//					modifiedParam1.Access ="TESTIN" ;
//					modifyThis.Parameter[1] = modifiedParam1;
//			
//					Parameter modifiedParam2 = new Parameter();
//					modifiedParam2.Name = "TESTnamelen" ;
//					modifiedParam2.Index = 22;
//					modifiedParam2.Type ="TESTint" ;
//					modifiedParam2.CompareAs ="TESTIntegerType" ;
//					modifiedParam2.Access ="TESTIN" ;
//					modifyThis.Parameter[2] = modifiedParam2;
//			
//					FuncXMLNav.UpdateFunctionByName("bind",modifyThis);
//				}
//			 
//				//saving the functions.xml
//				FuncXMLNav.saveFunctionXmlDocument(FuncXMLNav,"functions.xml");
//
//				///cannot use DotNetFunctions.xml as new version 2.5 file is not available.
//				//			// DotNetFunctions.xml
//				//			Console.WriteLine("This is Test Appliacation to test DotNetFunctions.xml");	
//				//			FunctionXMLNavigator DotNetFuncXMLNav = new FunctionXMLNavigator();
//				//			string DotNetFunctionXMLFilePath = @"E:\Fall2003\Holodeck xml files\DotNetFunctions.xml";
//				//			DotNetFuncXMLNav.parseXmlDocument(DotNetFunctionXMLFilePath);
//				//			DotNetFuncXMLNav.saveFunctionXmlDocument(DotNetFuncXMLNav,"DotNetFunctions.xml");
//			
//			
//				EditValues test = new EditValues();
//				//string vp = "abcಎejj1@3%";
//				string vp = "abc__ejj1@3%";
//				Console.WriteLine(vp);
//				Console.WriteLine(test.ToLower(vp));
//				Console.WriteLine(test.ToUpper(vp));
//				Console.WriteLine(test.InsertAtStart(vp,"pppp"));
//				Console.WriteLine(test.InsetAtEnd(vp,"PPPP"));
//				Console.WriteLine("SplitString:" +test.SplitString(vp));
//				Console.WriteLine(test.SubstituteToNull(vp)); 
//				foreach(string sp in test.SpecialCharacters)
//				{
//					Console.WriteLine(sp);
//				
//				}
//			
//				Console.WriteLine(test.changeEncodingToASCII(vp));
//				Console.WriteLine(test.changeEncodingToUnicode(vp));
//				Console.WriteLine(test.changeEncodingToUTF7(vp));
//				Console.WriteLine(test.changeEncodingToUTF8(vp));
//
//			}
//			catch(FileNotFoundException f)
//			{
//				Console.WriteLine(" File Not Found Exception caught.... : " + f.Message);
//				Console.WriteLine("Details: " + f.StackTrace);
//			}
//			catch(SystemException e)
//			{
//				Console.WriteLine("Caught  System Exception .... : " + e.Message);
//				Console.WriteLine("Details: " + e.StackTrace);
//			}

		}	
    }
               
}
