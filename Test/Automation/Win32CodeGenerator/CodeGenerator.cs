
using System;
using System.IO;
using System.Collections;
using TestingFunctionXML;
using System.Security.Permissions;
using Microsoft.Win32;


namespace Win32CodeGenerator
{
	/// <summary>
	/// Receives objects fuction object from fuctions.XML parser and generates a new C# file for each and every function
	/// </summary>
	public class CodeGenerator
	{
		public CodeGenerator()
		{
			
		}
		
		///********************************************************************************
		/// actual writing of c# code in the file
		/// 
		/// parameter:cfile
		/// <param name="cfile">the fuction name which would be the name of C# file</param>
		/// 
		/// parameter:ftemp
		/// <param name="ftemp">the fuction object</param>
		/// *******************************************************************************
		public void createCFile(string cfile,Function ftemp)
		{
			/*if the function returns a void, there has to be no variable to receive the return object.
			 * isVoid boolean checks for that*/
			bool isVoid = false;

			/*checks if the variable type is array*/
			bool isArray = false;

			/*there are some special types which give a problem being initialized to 0 or NULL, 
			 * hence these are carried on with no initialization COMPROMIsING the warnings
			 * MEMORY_RESOURCE_NOTIFICATION_TYPE
			 * FINDEX_INFO_LEVELS
			 * FINDEX_SEARCH_OPS
			 * FINDEX_INFO_LEVELS
			 * FINDEX_SEARCH_OPS
			 * GET_FILEEX_INFO_LEVELS",
			 * FILE_SEGMENT_ELEMENT[]
			 * 
			 * 
			 * isSpecial bool checks for that*/

			bool isSpecial = false;
			bool xSpecial = false;
			int xSpecialNumber=0;
			string arrType="",ptype="";
			string[] funcs = {"GlobalAlloc","HeapCreate","LocalAlloc","TlsAlloc","TlsFree","CreateFiber","CreateFiberEx","CreateToolHelp32Snapshot"};
			
			for(int i=0;i<8;i++)
			{
				if(ftemp.FunctionName.Equals(funcs[i]))
				{
					xSpecial = true;
					xSpecialNumber=i;
					break;
				}
			}
			
			StreamWriter sw = File.CreateText(cfile);
			
			if(ftemp.ReturnType.Equals("VOID"))
				isVoid=true;
			sw.WriteLine( "#include \"common.h\"\n\n" );
			sw.WriteLine("BOOL My_"+ ftemp.FunctionName+ "()");
			sw.WriteLine("{");
			sw.Write("\t");
			ArrayList al = ftemp.Parameter;
			string[] sp = {"MEMORY_RESOURCE_NOTIFICATION_TYPE","FINDEX_INFO_LEVELS","FINDEX_SEARCH_OPS","FINDEX_INFO_LEVELS","FINDEX_SEARCH_OPS","GET_FILEEX_INFO_LEVELS","FILE_SEGMENT_ELEMENT[]"};
			for(int i=0;i<al.Count;i++)
			{
				Parameter p = (Parameter)al[i];
				ptype = p.Type;
				for(int c=0;c<7;c++)
				{
					if(ptype.Equals(sp[c]))
					{
						isSpecial=true;
						break;
					}
					else
						isSpecial=false;
				}
				
				if(ptype.IndexOf("[]")==-1)
					isArray=false;
				else
					isArray=true;
				if(!isArray)
				{
					sw.Write(ptype+ " ");
					if(isSpecial)
						sw.WriteLine(p.Name+";");
					else
					{
						if(xSpecial==true && xSpecialNumber==6 && i<2)
							sw.WriteLine(p.Name+"=-1;");
						else if(xSpecial==true && xSpecialNumber==7 && i<2)
						{
							if(i==0)
								sw.WriteLine(p.Name+"=TH32CS_SNAPALL;");
							else
								sw.WriteLine(p.Name+"=-123;");
						}
						else if(xSpecial==true && xSpecialNumber==5 && i==0)
								sw.WriteLine(p.Name+"=-1;");
						else
							sw.WriteLine(p.Name+"=NULL;");
					}
				}
				else
				{
					
					arrType = ptype.Substring(0,ptype.Length-2);
					sw.Write(arrType+ " ");
					if(isSpecial)
						sw.WriteLine(p.Name+"[50];");
					else
						sw.WriteLine(p.Name+"[] =NULL;");
				}
				sw.Write("\t");
			}
			if(!isVoid)
			{
				sw.WriteLine(ftemp.ReturnType+ " returnVal_Real = NULL;");
				sw.Write("\t");
				sw.WriteLine(ftemp.ReturnType+ " returnVal_Intercepted = NULL;");
			}
			sw.WriteLine();
			sw.Write("\t");
			sw.WriteLine("DWORD error_Real = 0;");
			sw.Write("\t");
			sw.WriteLine("DWORD error_Intercepted = 0;");
			sw.Write("\t");
			sw.WriteLine("__try{");
			sw.Write("\t");
			sw.WriteLine("disableInterception();");

			sw.Write("\t");
			if(!isVoid)
			sw.Write("returnVal_Real = ");
			if(xSpecial==true&&xSpecialNumber==4)
				sw.Write("TlsFree (TlsAlloc( ));");
			else
			{
				sw.Write (ftemp.FunctionName +" (");
				for(int cnt=0;cnt<al.Count;cnt++)
				{
					Parameter p1 = (Parameter)al[cnt];
					sw.Write(p1.Name);
					if(cnt!=al.Count-1)
						sw.Write(",");
				}
				sw.WriteLine(");");
			}

			sw.Write("\t");
			sw.WriteLine("error_Real = GetLastError();");

			
			sw.Write("\t");
			sw.WriteLine("enableInterception();");

			sw.Write("\t");
			if(!isVoid)
				sw.Write("returnVal_Intercepted = ");
			if(xSpecial==true&&xSpecialNumber==4)
				sw.Write("TlsFree (TlsAlloc( ));");
			else
			{
				sw.Write (ftemp.FunctionName +" (");
				for(int cnt2=0;cnt2<al.Count;cnt2++)
				{
					Parameter p2 = (Parameter)al[cnt2];
					sw.Write(p2.Name);
					if(cnt2!=al.Count-1)
						sw.Write(",");
				}
				sw.WriteLine(");");
			}
			sw.Write("\t");
			sw.WriteLine("error_Intercepted = GetLastError();");
			sw.Write("\t");
			sw.WriteLine("}__except(puts(\"in filter\"), 1){puts(\"exception caught\");}");
			sw.Write("\t");
			if(!isVoid)
			{
				if(xSpecial==true && xSpecialNumber==0)
					sw.WriteLine("return ((GlobalSize(returnVal_Real) == GlobalSize(returnVal_Intercepted)) && (error_Real == error_Intercepted));");
				else if(xSpecial==true && xSpecialNumber==2)
					sw.WriteLine("return ((LocalSize(returnVal_Real) == LocalSize(returnVal_Intercepted)) && (error_Real == error_Intercepted));");
				else if(xSpecial==true && xSpecialNumber==1)
					sw.WriteLine("return (( HeapValidate( returnVal_Real, NULL, NULL ) == HeapValidate( returnVal_Intercepted, NULL, NULL )) && (error_Real == error_Intercepted));");
				else if(xSpecial==true && xSpecialNumber==3)
					sw.WriteLine("return ((( returnVal_Real != NULL ) == ( returnVal_Intercepted != NULL )) && (error_Real == error_Intercepted));");
				else
					sw.WriteLine("return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));");
			}
			else
				sw.WriteLine("return (error_Real == error_Intercepted);");
			sw.WriteLine("}");
			
			sw.Flush();
			sw.Close();
		}

/// *****************************************************************************************
/// method name: fileWriter()
/// 
/// Collects all the information from the FuctionXMLNavigator which is the XML parser
/// receives HashTable htemp which has the collection of all the Function Objects
///  
/// *****************************************************************************************
		public void fileWriter()
		{
			string holodeckPath;
			holodeckPath = (string) Registry.LocalMachine.OpenSubKey ("Software\\HolodeckEE", true).GetValue ("InstallPath");
		
			string holodeckFunctionDBPath = string.Concat(holodeckPath,"\\function_db");
			string FunctionsXMLFilePath = string.Concat(holodeckPath,"\\function_db\\functions.xml");
						
			FunctionXMLNavigator FuncXMLNav = new FunctionXMLNavigator();

			FuncXMLNav.ValidateXmlDocument(FunctionsXMLFilePath);
			FuncXMLNav.parseXmlDocument(FunctionsXMLFilePath);
			Hashtable htemp = (Hashtable)FuncXMLNav.FunctionTableByName.Clone();
			
			IDictionaryEnumerator HashFuctionNames = htemp.GetEnumerator();
			Function ftemp=null;
			while(HashFuctionNames.MoveNext())
			{
				ftemp = (Function)HashFuctionNames.Value;
			
				//create Win32CodeGeneratorDump Folder
				string Win32CodeGeneratorDumpDirPath = string.Concat(holodeckFunctionDBPath,"\\Win32CodeGeneratorDump");

				if (!Directory.Exists(Win32CodeGeneratorDumpDirPath)) 
				{
					DirectoryInfo di = Directory.CreateDirectory(Win32CodeGeneratorDumpDirPath);
				}
				
				string cfile = string.Concat(Win32CodeGeneratorDumpDirPath,"\\", ftemp.FunctionName);
				cfile = string.Concat(cfile,".cpp");
				createCFile(cfile,ftemp);


			}	
		
		}

	}
}
