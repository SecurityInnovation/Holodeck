///******************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		NetCodeGenerator.cs
///
/// DESCRIPTION: Creates template files for the functions in DotNetFunctions.XML
///
///==============================================================================
/// Modification History
///
/// Date				Name			         Purpose
/// -----------    -----------     ------------------------------------------
/// 29 Oct 2003	    Ankur Jauhari	            File created.
/// 31 Oct 2003		Vaibhav Parikh				File Modified
/// 19 Dec 2003		Jessica Winter				File Modified
///******************************************************************************

using System;
using System.IO;
using TestingFunctionXML;
using System.Collections;
using System.Security.Permissions;
using Microsoft.Win32;
using System.Reflection;

namespace DotNetCodeGenerator
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class CodeGenerator
	{
		string filePath = "";		// path to FunctionDotNet.XML

		///*************************************************************************
		/// Method:		NetCodeGenerator
		/// Description: constructor
		///
		/// Parameters:
		///	xml file path
		///
		///  Return Value:  None
		///*************************************************************************
		public CodeGenerator(string path)
		{
			filePath = path;
		}
		
		///*************************************************************************
		/// Method:		WriteFile
		/// Description: Reads from the xml and gets the hashTable
		///
		///	Parameters: None
		///
		///  Return Value:  None
		///*************************************************************************
		public void WriteFile()
		{
			FunctionXMLNavigator XMLreader = new FunctionXMLNavigator();
			XMLreader.ValidateXmlDocument(filePath);
			XMLreader.parseXmlDocument(filePath);
			Hashtable funcInfo = (Hashtable)XMLreader.FunctionTableByName.Clone();
			IDictionaryEnumerator funcDetails = funcInfo.GetEnumerator();
			Function func = null;
			
			while(funcDetails.MoveNext())
			{
				func = (Function)funcDetails.Value;
				string fileName = string.Concat(func.FunctionName,".cs");
				generateFile(fileName, func);
			}
			
		}
	
		///*************************************************************************
		/// Method:	 generateFile
		/// Description: writing template files for all the funtiondotnet.xml 
		///
		/// Parameters:
		///	functionName, functionsObject
		///
		/// Return Value:  None
		///*************************************************************************
		public void generateFile(string fName , Function funcName)
		{
			
			string[] funcprop = funcName.FunctionName.Split('.');
			
			string nspace = "";
			string n_space = "";
			string className = "";
			string functionName = "";
			int counter = 0;
			bool isStatic = false;			

			if ( funcName.FunctionName.IndexOf( "System.ComponentModel.ArrayConverter" ) > 0 )
				System.Diagnostics.Debugger.Break( );

			if ( funcName.FunctionName.IndexOf( "ctor" ) > 0 )
			{
				return;
			}

			for(int i = funcprop.Length; i >= 0; i--)
			{
				if(i == funcprop.Length)
				{
					className = funcprop[funcprop.Length-1];
					functionName = funcprop[funcprop.Length-1];
				}
				else
				{
					if(i-1 >= 0)
					{
						if(i != 1)
							nspace += funcprop[counter]+".";
						else
							nspace += funcprop[counter];
						counter++;
					}
					
				}
			}
			
			string holodeckFunctionDBPath;
			holodeckFunctionDBPath = (string) Registry.LocalMachine.OpenSubKey("Software\\HolodeckEE", true).GetValue ("InstallPath");

			string DotNetCodeGeneratorDumpDirPath = string.Concat(holodeckFunctionDBPath,"\\function_db\\DotNetCodeGeneratorDump");

		
			if (!Directory.Exists(DotNetCodeGeneratorDumpDirPath)) 
			{
				DirectoryInfo di = Directory.CreateDirectory(DotNetCodeGeneratorDumpDirPath);
			}
			//fName = string.Concat(DotNetCodeGeneratorDumpDirPath,"\\", fName);
			//fName = string.Concat(@"C:\DELETEME","\\",fName); //jess
			fName = string.Concat(@"E:\CVS Root\CVSROOT\Holodeck\Test\Automation\DotNetInterceptTester\DotNetInterceptTester","\\",fName);

			ArrayList parList = funcName.Parameter;
			StreamWriter writeFile = File.CreateText(fName);
			
			//file writing start here
			
			writeFile.WriteLine("using System;");
			writeFile.WriteLine("using System.Security.Permissions;");

			writeFile.WriteLine();
			writeFile.WriteLine("namespace DotNetInterceptTester.My_"+nspace);
			writeFile.WriteLine("{");

			string[] classprop = functionName.Split('(');
			classprop[0] = AddParameters(classprop[0], parList);
			writeFile.WriteLine("public class "+ classprop[0]);
			writeFile.WriteLine("{");
			
			//DotNetCodeGenerator.cs
			string[] funcType = functionName.Split('(');
			funcType[0] = AddParameters(funcType[0],parList);
			writeFile.WriteLine("public static bool _" + funcType[0]+"( )");
			writeFile.WriteLine("{");
			
			n_space = nspace.Replace( ".", "_" );
			
			writeFile.WriteLine();
			
			string GACReference = funcName.OriginalDll.Remove( funcName.OriginalDll.IndexOf( ".dll" ), 4 );

			//check for static method
			Assembly checkAssembly = System.Reflection.Assembly.LoadWithPartialName( GACReference );

			if ( checkAssembly == null )
				checkAssembly = System.Reflection.Assembly.LoadWithPartialName( "mscorlib" );
			
			string[] methodToCheck = className.Split('(');

			// Get the type of the class...
			Type classType = checkAssembly.GetType( nspace );

			// If the class is abstract, we don't test it ;-)
			if ( classType.IsAbstract )
			{			
				// Close the file stream writer...
				writeFile.Close( );

				// Delete the file...
				System.IO.File.Delete( fName );

				// Stop and get out of this method...
				return;
			}

			MethodInfo [] methodInfoItems = classType.GetMethods( );

			MethodInfo methodInfoItemToCheck = null;

			bool isMethodMatched = false;

			int iParameterOffset = 0;

			foreach ( MethodInfo methodInfoItem in methodInfoItems )
			{
				// Get the parameters for the current methodinfo item we are looking at...
				ParameterInfo [] parameterInfoItems = methodInfoItem.GetParameters( );
				
				int iParameterCount = 0;
				

				if ( methodInfoItem.Name == methodToCheck[0] )
				{
					// If the count for the parameter list is 0, we don't need to cycle through
					// all the parameters...
					if ( ( (ArrayList)funcName.Parameter ).Count != 0 )
					{
						// If we are looking at thisObj, then we know that we should NOT be static and that
						// the count we are looking at will have to be subtracted...
						if ( ( (Parameter)( ( (ArrayList)funcName.Parameter )[iParameterCount] ) ).Name == "thisObj" )
							iParameterOffset = 1;

						// Cycle through all the parameters to find out if we have a match...
						for ( iParameterCount = 0; iParameterCount < parameterInfoItems.Length; iParameterCount++ )
						{						
							// If this function doesn't have the right number of parameters, it's outta here...
							if ( ( (ArrayList)funcName.Parameter ).Count - iParameterOffset != parameterInfoItems.Length )
							{
								isMethodMatched = false;

								break;
							}

							// Otherwise, we continue and check each parameter one by one...
							if ( parameterInfoItems[iParameterCount].ParameterType.ToString( ) !=
								( (Parameter)( (ArrayList)funcName.Parameter )[iParameterCount + iParameterOffset] ).Type )
							{
								isMethodMatched = false;

								break;
							}					

							isMethodMatched = true;
						
						}
					}

					// Check to make sure (if we have no parameters) that this is correct.
					if ( ( parameterInfoItems.Length == 0 )&& 
						( ( ( (ArrayList)funcName.Parameter ).Count - iParameterOffset ) == 0 ) )
					{
						isMethodMatched = true;
					}
  
					// Setup the method item that we know is a match :-)....
					if ( isMethodMatched )
					{
						methodInfoItemToCheck = methodInfoItem;

						// Eureka! We found it!
						break;
					}
				}
			}

			if ( methodToCheck[0] != "ctor" )
			{
				if ( methodInfoItemToCheck == null )
					throw new System.Exception( "ERROR: The method " + methodToCheck[0] + " could not be found in the GAC/mscorlib" );
							
				isStatic = methodInfoItemToCheck.IsStatic;
						
				if ( isStatic && ( iParameterOffset == 1 ) )
					throw new System.Exception( "ERROR: The method was supposed to be static because there was no thisObj, but it wasn't" );
			}
			else
				isStatic = false;
			
			
			
			if(isStatic)
			{
				writeFile.WriteLine("   //class object not created since static method");
			}

			else
			{
				if(nspace == "Microsoft.Win32.RegistryKey")
				{
					writeFile.WriteLine("   //class object");
					writeFile.WriteLine("    " + nspace + " _" + n_space + " = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(\"TestHD\");");

				}
				else
				{
					writeFile.WriteLine("   //class object");
					writeFile.WriteLine("    " + nspace + " _" + n_space + " = new " + nspace +"();");
				}
			}


			writeFile.WriteLine();
			writeFile.WriteLine("   "+"//Parameters");				

			
			
			for(int i = 0; i < parList.Count; i++)
			{
				
				Parameter param = (Parameter)parList[i];
				if(param.Name == "thisObj")
					continue;
				else
				{
					if ( param.Name != "value" )
					{
						if ( param.Type.Equals("System.Boolean"))
							writeFile.WriteLine("   " + param.Type + " " + param.Name + " = false;");
						else
							switch(param.Type)
							{
								case "System.Boolean" :
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = false;");
									break;
								}
								case "System.Int64" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Int32" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Int16" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.UInt16" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.UInt32" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.UInt64" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Single" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Double" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Decimal" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Char" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = '0';");
									break;
								}
								case "System.TimeSpan" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Byte" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.SByte" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.DateTime" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Guid" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = 0;");
									break;
								}
								case "System.Object" : 
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = null;");
									break;
								}
								default :
								{
									writeFile.WriteLine("   " + param.Type + " " + param.Name + " = null;");
									break;
								}
						
							}
					}
					else
					{
						switch(param.Type)
						{
							case "System.Boolean" :
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = false;");
								break;
							}
							case "System.Int64" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Int32" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Int16" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.UInt64" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.UInt16" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.UInt32" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Single" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Double" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Decimal" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Char" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = '0';");
								break;
							}
							case "System.SByte" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Byte" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.TimeSpan" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.DateTime" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Guid" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = 0;");
								break;
							}
							case "System.Object" : 
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = null;");
								break;
							}
							default :
							{
								writeFile.WriteLine("   " + param.Type + " _" + param.Name + " = null;");
								break;
							}
						
						}

					}		
				}
				
			}

			writeFile.WriteLine();

			if(funcName.ReturnType != "void")
			{
				writeFile.WriteLine("   "+"//ReturnType/Value");
				
				string editedReturnType = funcName.ReturnType.Replace("+",".");

				switch(funcName.ReturnType)
				{
					case "System.Boolean" :
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = false;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = false;");
						break;
					}
					case "System.Int64" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Int32" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Int16" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.UInt16" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.UInt32" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.UInt64" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Single" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Double" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Decimal" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Char" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = '0';");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = '0';");
						break;
					}
					case "System.SByte" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Byte" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.TimeSpan" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.DateTime" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Guid" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = 0;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = 0;");
						break;
					}
					case "System.Object" : 
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = null;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = null;");
						break;
					}
					default :
					{
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = null;");
						writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = null;");
						break;
					}
						
				}
				
//				if(funcName.ReturnType == "System.Boolean")
//				{
//					writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = false;");
//					writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = false;");
//				}
//				else
//				{
//					writeFile.WriteLine("   " + editedReturnType + " returnVal_Real = null;");
//					writeFile.WriteLine("   " + editedReturnType + " returnVal_Intercepted = null;");
//				}
			}

			writeFile.WriteLine();
			writeFile.WriteLine("   "+"//Exception");
			writeFile.WriteLine("   string exception_Real = null;");
			writeFile.WriteLine("   string exception_Intercepted = null;");
			writeFile.WriteLine();

			writeFile.WriteLine("   InterceptionMaintenance.disableInterception( );");
			writeFile.WriteLine();
			
			if(isStatic)
			{
				generateTryCatch("_Real = ", writeFile, funcName, parList, nspace,isStatic);
			}
			else 
			{
				generateTryCatch("_Real = ", writeFile, funcName, parList, n_space,isStatic);
			}
			writeFile.WriteLine();
			
			writeFile.WriteLine("   InterceptionMaintenance.enableInterception( );");
			writeFile.WriteLine();
			
			if(isStatic)
			{
				generateTryCatch("_Intercepted = ", writeFile, funcName, parList, nspace,isStatic);
			}
			else
			{
				generateTryCatch("_Intercepted = ", writeFile, funcName, parList, n_space,isStatic);
			}

			writeFile.WriteLine();

			
			if(funcName.ReturnType != "void" )
				writeFile.WriteLine("   return( ( exception_Real == exception_Intercepted ) && ( returnVal_Real == returnVal_Intercepted ) );");
            else
				writeFile.WriteLine("   return( ( exception_Real == exception_Intercepted ));");
			
			writeFile.WriteLine("}");
			writeFile.WriteLine("}");
			writeFile.WriteLine("}");

			writeFile.Flush();
		}

		public string AddParameters(string funcName, ArrayList parList)
		{
			
			for(int i = 0; i <parList.Count;i++)
			{
				Parameter param = (Parameter)parList[i];
				funcName += "_"+param.Type.Replace( ".", "_" ).Replace( "[", "_" ).Replace( "]", "_" ).Replace( "&", "_" );
			}
			return funcName;
		}


		///*************************************************************************
		/// Method:		generateTryCatch
		/// Description: generates the try catch block of the code generator
		///
		/// Parameters:
		///	string (interceped or real), filewriter, function object, arraylist
		///	
		/// Return Value:  None
		///*************************************************************************

		public void generateTryCatch(string type, StreamWriter fWriter, Function szfunc , ArrayList paramList , string classObject, bool isStatic)
		{

			if(isStatic)
			{
				classObject = classObject.Replace("_", ".");
			}
			else
			{
				 classObject = string.Concat("_",classObject);
			}
			string retType = type;
			string parameters = "";
			fWriter.WriteLine("   try");
			fWriter.WriteLine("   {");
			string[] spFunc = szfunc.FunctionName.Split('(');
			string[] functionCall = szfunc.FunctionName.Split('.');
			
			//modify method name
			string methodName = functionCall[functionCall.Length -1].ToString();
			string [] modifiedMethodName = methodName.Split('(');

			methodName = modifiedMethodName[0]+"(";

			parameters = spFunc[0]+"(";
			
			
			if ( (paramList.Count == 1) && ( ((Parameter)paramList[0]).Name == "thisObj" ) )
			{
				if (String.Compare(szfunc.ReturnType,  "void") == 0)
				{
					
					//fWriter.WriteLine("      " + parameters + ");");
					//Console.WriteLine("      " + parameters + ");");
					fWriter.WriteLine("      " + classObject + "." + methodName +");");
					//Console.WriteLine("      " + classObject + "." + methodName +");");
				}
				else
				{
					//fWriter.WriteLine("      returnVal" + retType  + parameters + ");"); //???
					//Console.WriteLine("      returnVal" + retType  + parameters + ");"); //???
					fWriter.WriteLine("      returnVal" + retType  + classObject + "." + methodName + ");"); //???
				}
			}
			else
			{
				parameters = string.Concat(classObject, "." ,methodName);//??

				for(int i = 0; i < paramList.Count; i++)
				{
					Parameter param = (Parameter)paramList[i];
					if(param.Name == "thisObj")
						continue;
						
					else if(i != paramList.Count -1)
					{
						if ( param.Name != "value" )
							parameters += param.Name + ",";
						else
							parameters += "_" + param.Name + ",";
					}
					else
					{
						if ( param.Name != "value" )
							parameters += param.Name;
							//parameters += param.Name + ")"; //original
						else
							parameters += "_" + param.Name;
							//parameters += "_" + param.Name + ")"; //original
					}
					
				}
				
				if (String.Compare(szfunc.ReturnType,  "void") == 0)
				{
					fWriter.WriteLine("      " + parameters + ");");
					//Console.WriteLine(parameters + ";");
					//fWriter.WriteLine(classObject + "." + methodName  + ";");
					//Console.WriteLine(classObject + "." + methodName  + ";");
				}
				else
				{

					fWriter.WriteLine("      returnVal" + retType  + parameters + ");"); //???
					//Console.WriteLine("      returnVal" + retType  + parameters + ";"); //???
					//fWriter.WriteLine("      returnVal" + retType  + classObject + "." + methodName + ");"); //???
				}
			}

			fWriter.WriteLine("   }");
			fWriter.WriteLine();
			fWriter.WriteLine("   catch( System.Exception e )");
			fWriter.WriteLine("   {");
			fWriter.WriteLine("      exception" + retType + "e.Message;");
			fWriter.WriteLine("   }");
			fWriter.WriteLine();
		}
	}
}