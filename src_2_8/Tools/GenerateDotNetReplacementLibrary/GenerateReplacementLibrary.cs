//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GenerateReplacementLibrary.cs
//
// DESCRIPTION: Automatic generation of .NET replacement libraries based
//				on the metadata in the target assembly.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 21 Apr 2003		 R. Wagner		File created.
//*************************************************************************

using System;
using System.Reflection;
using System.IO;
using System.Collections;

class GenerateReplacementLibrary
{
	private static ArrayList includes, excludes;

	private static StreamWriter xmlFile;
	private static StreamWriter exceptionFile;

	private enum IniSection
	{
		None,
		Include,
		Exclude
	}

	private struct IncludeDef
	{
		public string matchStr;
		public string category;
	}

	//*************************************************************************
	// Method:	 ParseIni
	// Description: Parses the include/exclude list .ini file
	// Parameters:
	//	iniFile - StreamReader object containing the open .ini file
	//
	// Return Value: None
	//*************************************************************************
	private static void ParseIni(StreamReader iniFile)
	{
		includes = new ArrayList();
		excludes = new ArrayList();

		IniSection section = IniSection.None;

		while (true)
		{
			string line = iniFile.ReadLine();
			if (line == null)
				break;

			line = line.TrimEnd(new char[]{'\n'});
			line = line.Trim();
			if (line == "")
				continue;

			if (line == "[Include]")
				section = IniSection.Include;
			else if (line == "[Exclude]")
				section = IniSection.Exclude;
			else
			{
				if (section == IniSection.None)
					throw new FormatException("Expected [Include] or [Exclude] section");
				else if (section == IniSection.Include)
				{
					int commaIndex = line.IndexOf(',');
					if (commaIndex == -1)
						throw new FormatException("Expected category name in Include section");
					IncludeDef id;
					id.category = line.Substring(0, commaIndex).Trim();
					id.matchStr = line.Substring(commaIndex + 1).Trim();
					includes.Add(id);
				}
				else
					excludes.Add(line);
			}
		}
	}

	//*************************************************************************
	// Method:	 MatchString
	// Description: Matches two strings with wildcards, used for the
	//				include/exclude lists
	// Parameters:
	//	name - String to match (no wildcards)
	//  test - Test string, possibly including wildcards at the beginning, end,
	//		or both ends of the string, that determines if the name matches
	//
	// Return Value: true if the strings match
	//*************************************************************************
	static bool MatchString(string name, string test)
	{
		if (test.StartsWith("*"))
		{
			if (test.EndsWith("*"))
				return (name.IndexOf(test.Substring(1, test.Length - 2)) != -1);
			return name.EndsWith(test.Substring(1));
		}
		else if (test.EndsWith("*"))
			return name.StartsWith(test.Substring(0, test.Length - 1));
		else
			return name == test;
	}

	//*************************************************************************
	// Method:	 ShouldInclude
	// Description: Determines if a function should be intercepted
	// Parameters:
	//	name - Full name, including namespace and type, of the method in
	//		question
	//
	// Return Value: Category name that the function resides in, or the
	//		empty string if it shouldn't be intercepted
	//*************************************************************************
	static string ShouldInclude(string name)
	{
		bool included = false;
		string category = "";

		foreach (IncludeDef id in includes)
		{
			if (MatchString(name, id.matchStr))
			{
				included = true;
				category = id.category;
			}
		}
		foreach (string s in excludes)
		{
			if (MatchString(name, s))
				included = false;
		}
		if (included)
			return category;
		return "";
	}

	//*************************************************************************
	// Method:	 GetTypeName
	// Description: Returns the full name of a type as it should be used in
	//				C# source.
	// Parameters:
	//	t - Type object representing the type
	//
	// Return Value: String containing the name of the type as it should be
	//		represented in C# source.
	//*************************************************************************
	static string GetTypeName(Type t)
	{
		if (t == typeof(void))
			return "void";
		if (t.IsByRef)
			return "ref " + t.FullName.Replace("+", ".").Replace("&", "");
		return t.FullName.Replace("+", ".");
	}

	//*************************************************************************
	// Method:	 GetHelpLink
	// Description: Gets the correct help link for a constructor
	// Parameters:
	//	t - the type of the class
	//	c - the constructor info of the function to get the link for
	//
	// Return Value: the help link for the method
	//*************************************************************************
	static string GetHelpLink(System.Type t, System.Reflection.ConstructorInfo c)
	{
		string className;

		if (t == c.DeclaringType)
			className = t.FullName;
		else
			className = c.DeclaringType.FullName;

		string classWithoutDots = className.Replace(".", "");
		classWithoutDots = classWithoutDots.ToLower();
		string url = "http://msdn.microsoft.com/library/default.asp?url=/library/en-us/cpref/html/frlrf" + classWithoutDots + "class" + c.Name.ToLower().Replace(".", "") + "topic.asp";

		return url;
	}

	//*************************************************************************
	// Method:	 GetHelpLink
	// Description: Gets the correct help link for a function
	// Parameters:
	//	t - the type of the class
	//	m - the method info of the function to get the link for
	//
	// Return Value: the help link for the method
	//*************************************************************************
	static string GetHelpLink(System.Type t, System.Reflection.MethodInfo m)
	{
		string className;

		if (t == m.DeclaringType)
			className = t.FullName;
		else
			className = m.DeclaringType.FullName;

		string classWithoutDots = className.Replace(".", "");
		classWithoutDots = classWithoutDots.ToLower();
		string url = "http://msdn.microsoft.com/library/default.asp?url=/library/en-us/cpref/html/frlrf" + classWithoutDots + "class" + m.Name.ToLower() + "topic.asp";

		return url;
	}

	//*************************************************************************
	// Method:	 Help
	// Description: Displays usage information on the command line.
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	static void Help()
	{
		Console.WriteLine("GenerateReplacementLibrary funcxml exceptionxml dllname inifile outputpath");
		Console.WriteLine("                           [dllname inifile outputpath [...]]");
		Console.WriteLine();
		Console.WriteLine("Generates a Holodeck replacement library for the .NET assembly at path dllname,");
		Console.WriteLine("outputting a C# source file that contains the handler methods.  The .ini file");
		Console.WriteLine("given should contain the include/exclude list for which methods to intercept.");
		Console.WriteLine("There should be two sections named \"Include\" and \"Exclude\".");
		Console.WriteLine();
	}

	//*************************************************************************
	// Method:	 GenerateLibrary
	// Description: Generates a replacement library for a given module.
	// Parameters:
	//	assemblyFileName - Path to the original module to generate the
	//		replacement library for
	//  iniFileName - Path to the .ini file containing the include/exclude
	//		list for this module
	//  outFileName - Path to the .cs output file containing the replacement
	//		library code
	//
	// Return Value: None
	//*************************************************************************
	static void GenerateLibrary(string assemblyFileName, string iniFileName, string outFileName)
	{
		Console.WriteLine(assemblyFileName);

		Assembly a = Assembly.LoadFrom(assemblyFileName);
		StreamReader iniFile = new StreamReader(iniFileName);
		StreamWriter outFile = new StreamWriter(outFileName);

		string replacementLibraryName = Path.GetFileNameWithoutExtension(outFileName);

		outFile.WriteLine("// This file was automatically generated by GenerateReplacementLibrary.  Do not modify.");
		outFile.WriteLine();
		outFile.WriteLine("using Replacement;");
		outFile.WriteLine("using System;");
		outFile.WriteLine();
		outFile.WriteLine();
		outFile.WriteLine("namespace Replacement");
		outFile.WriteLine("{");
		outFile.WriteLine("\tpublic class " + replacementLibraryName);
		outFile.WriteLine("\t{");
		outFile.WriteLine("\t\tpublic static DotNetReplacementLibrary ReplacementLibrary = new DotNetReplacementLibrary(\"" +
			Path.GetFileName(assemblyFileName) + "\");");
		outFile.WriteLine("\t};");
		outFile.WriteLine("}");
		outFile.WriteLine();
		outFile.WriteLine();
		outFile.WriteLine("namespace HeatInterceptHandlers");
		outFile.WriteLine("{");

		ParseIni(iniFile);
		iniFile.Close();

		// Parse the IL code to look for which exceptions each method throws
		Hashtable exceptionTable = new Hashtable();

		System.Diagnostics.Process.Start("ildasm.exe", assemblyFileName + " /OUT=temp.dat").WaitForExit();
		StreamReader ilFile = new StreamReader("temp.dat");
		string curNamespace = "";
		string curClass = "";
		string curMethod = "";
		string curException = "";
		while (true)
		{
			string line = ilFile.ReadLine();
			if (line == null)
				break;

			line = line.Trim();

			if (line.StartsWith(".namespace"))
				curNamespace = line.Substring(line.IndexOf(' ') + 1).Trim();
			else if (line.StartsWith(".class"))
				curClass = line.Substring(line.LastIndexOf(' ') + 1).Trim();
			else if (line.StartsWith(".method"))
			{
				while (true)
				{
					if (line.IndexOf('(') != -1)
						break;
					line = ilFile.ReadLine();
				}

				curMethod = line.Substring(0, line.IndexOf('('));
				curMethod = curMethod.Substring(curMethod.LastIndexOf(' ') + 1).Trim();
				if (curMethod == ".ctor")
					curMethod = "ctor";
			}
			else if (line.IndexOf(':') != -1)
			{
				line = line.Substring(line.IndexOf(':') + 1).Trim();
				if (line.StartsWith("newobj"))
				{
					curException = line.Substring(0, line.IndexOf("::"));
					curException = curException.Substring(curException.LastIndexOf(' ') + 1).Trim();
				}
				else if (line.StartsWith("throw"))
				{
					if (a.GetType(curException) == null)
						continue;
					if (!a.GetType(curException).IsSubclassOf(typeof(Exception)))
						continue;
					if (a.GetType(curException).FullName.IndexOf("+") != -1)
						continue;
					if (a.GetType(curException).IsNotPublic)
						continue;

					string fullMethodName = curNamespace + "." + curClass + "." + curMethod;
					ArrayList exceptionList;

					if (exceptionTable.ContainsKey(fullMethodName))
						exceptionList = (ArrayList)exceptionTable[fullMethodName];
					else
						exceptionList = new ArrayList();

					if (!exceptionList.Contains(curException))
						exceptionList.Add(curException);

					if (exceptionTable.ContainsKey(fullMethodName))
						exceptionTable[fullMethodName] = exceptionList;
					else
						exceptionTable.Add(fullMethodName, exceptionList);
				}
			}
		}
		ilFile.Close();

		Type[] typeList = a.GetTypes();

		int handlerMethodIndex = 0;
		int handlerClassIndex = 0;

		foreach (Type t in typeList)
		{
			try
			{
				// Don't intercept enumerations, non-public classes, non-top level classes,
				// or delegates
				if (t.IsEnum)
					continue;
				if (t.IsNotPublic)
					continue;
				if (t.FullName.IndexOf("+") != -1)
					continue;
				if (t.BaseType == typeof(MulticastDelegate))
					continue;
				if (t.BaseType == typeof(Delegate))
					continue;

				if (t.IsSubclassOf(typeof(Exception)))
				{
					exceptionFile.WriteLine("\t<Exception Name=\"" + t.FullName + "\" />");
					continue;
				}

				// Create some containers for the caller and handler method code
				ArrayList callerMethods = new ArrayList();
				ArrayList handlerMethods = new ArrayList();

				// Iterate through all methods in the type and add the ones that should be included
				MethodInfo[] methodListInstance = t.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly);
				MethodInfo[] methodListStatic = t.GetMethods(BindingFlags.Public | BindingFlags.Static | BindingFlags.DeclaredOnly);

				MethodInfo[] methodList = new MethodInfo[methodListInstance.Length + methodListStatic.Length];
				Array.Copy(methodListInstance, 0, methodList, 0, methodListInstance.Length);
				Array.Copy(methodListStatic, 0, methodList, methodListInstance.Length, methodListStatic.Length);

				foreach (MethodInfo m in methodList)
				{
					if (m.IsAbstract)
						continue;
					if (!m.IsPublic)
						continue;

					// Cannot intercept instance methods in a value type
					if ((!m.IsStatic) && (t.IsValueType))
						continue;

					ParameterInfo[] paramList = m.GetParameters();

					bool hasVoidPtrOrTypedRef = false;
					for (int i=0; i<paramList.Length; i++)
					{
						if (paramList[i].ParameterType.FullName == "System.TypedReference")
							hasVoidPtrOrTypedRef = true;
						else if (paramList[i].ParameterType.FullName == "System.Void*")
							hasVoidPtrOrTypedRef = true;
					}
					if (hasVoidPtrOrTypedRef)
						continue;
					if (m.ReturnType.FullName == "System.TypedReference")
						continue;
					if (m.ReturnType.FullName == "System.Void*")
						continue;

					string methodName = t.FullName + "." + m.Name;
					string methodNameWithParams = methodName + "(";
					for (int i=0; i<paramList.Length; i++)
					{
						string typeName = paramList[i].ParameterType.FullName;

						// Remove namespace information from the parameter type
						int dotIndex = typeName.LastIndexOf('.');
						if (dotIndex != -1)
							typeName = typeName.Substring(dotIndex + 1);

						methodNameWithParams += typeName;
						if ((i + 1) < paramList.Length)
							methodNameWithParams += ", ";
					}
					methodNameWithParams += ")";

					string category = ShouldInclude(methodNameWithParams);

					if (category != "")
					{
						// Method is included.  Generate a caller and handler method

						// Method always has a MethodInterceptInfo parameter
						int specialParamCount = 1;

						// Generate function signature for caller method
						handlerMethodIndex++;
						callerMethods.Add("\t\t// " + GetTypeName(t) + "." + m.Name);
						callerMethods.Add("\t\tprivate static bool CallerMethod" + handlerMethodIndex.ToString() +
							"(Object[] paramObjs, ref Object retValue, ref Exception exception)");
						callerMethods.Add("\t\t{");

						// Check if parameter list is the correct size
						callerMethods.Add("\t\t\tif (paramObjs.Length != " + (paramList.Length +
							specialParamCount).ToString() + ") return false;");

						// Generate a parameter array that does not include the first MethodInterceptInfo parameter
						callerMethods.Add("\t\t\tObject[] methodParams = new Object[paramObjs.Length - 1];");
						callerMethods.Add("\t\t\tArray.Copy(paramObjs, 1, methodParams, 0, paramObjs.Length - 1);");

						// Enclose the call to the original method in a try block so that we can catch
						// and log any exceptions that it throws.
						callerMethods.Add("\t\t\ttry");
						callerMethods.Add("\t\t\t{");

						callerMethods.Add("\t\t\t\tDotNetReplacementLibrary.EnableNativeInterception();");
						callerMethods.Add("\t\t\t\tHeatDotNet.Interception.EnableInterceptionInCurrentThread();");
						callerMethods.Add("\t\t\t\tHeatDotNet.Interception.BeginForceOriginalCode();");

						// Generate name of original function
						string origCallStr = "((HeatDotNet.MethodInterceptInfo)paramObjs[0]).Invoke(methodParams)";

						// Store the return value if there is one
						if (m.ReturnType != typeof(void))
							origCallStr = "retValue = " + origCallStr;
						callerMethods.Add("\t\t\t\t" + origCallStr + ";");

						callerMethods.Add("\t\t\t\tHeatDotNet.Interception.DisableInterceptionInCurrentThread();");
						callerMethods.Add("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();");

						if (m.ReturnType == typeof(void))
							callerMethods.Add("\t\t\t\tretValue = null;");
						callerMethods.Add("\t\t\t\texception = null;");

						// Catch any exceptions that the original method threw
						callerMethods.Add("\t\t\t}");
						callerMethods.Add("\t\t\tcatch (Exception e)");
						callerMethods.Add("\t\t\t{");
						callerMethods.Add("\t\t\t\tretValue = null;");
						callerMethods.Add("\t\t\t\texception = e;");
						callerMethods.Add("\t\t\t\tHeatDotNet.Interception.DisableInterceptionInCurrentThread();");
						callerMethods.Add("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();");

						callerMethods.Add("\t\t\t}");

						bool hasRefParams = false;
						for (int i=0; i<paramList.Length; i++)
						{
							if (paramList[i].ParameterType.IsByRef)
								hasRefParams = true;
						}

						// Copy the updated paramater values if the function has reference parameters
						if (hasRefParams)
							callerMethods.Add("\t\t\tArray.Copy(methodParams, 0, paramObjs, 1, paramObjs.Length - 1);");
						
						callerMethods.Add("\t\t\treturn true;");

						callerMethods.Add("\t\t}");
						callerMethods.Add("");

						// Generate the handler method
						string handlerFuncProto = "\t\tpublic static " + GetTypeName(m.ReturnType) + " " + m.Name + "(";

						handlerFuncProto += "HeatDotNet.MethodInterceptInfo methodIntercept";
						if (paramList.Length > 0)
							handlerFuncProto += ", ";

						// Generate parameter list for function signature
						for (int i=0; i<paramList.Length; i++)
						{
							handlerFuncProto += GetTypeName(paramList[i].ParameterType);
							handlerFuncProto += " " + paramList[i].Name;

							if ((i + 1) < paramList.Length)
								handlerFuncProto += ", ";
						}

						handlerFuncProto += ")";

						handlerMethods.Add(handlerFuncProto);
						handlerMethods.Add("\t\t{");

						handlerMethods.Add("\t\t\tDotNetReplacementLibrary.DisableNativeInterception();");

						// Create an Object array to store the parameters
						handlerMethods.Add("\t\t\tObject[] paramObjs = new Object[" +
							(paramList.Length + specialParamCount).ToString() + "];");
						handlerMethods.Add("\t\t\tObject retValue = null;");
						handlerMethods.Add("\t\t\tException exception = null;");
						for (int i=0; i<(paramList.Length + specialParamCount); i++)
						{
							if (i < specialParamCount)
								handlerMethods.Add("\t\t\tparamObjs[0] = methodIntercept;");
							else
							{
								handlerMethods.Add("\t\t\tparamObjs[" + i.ToString() + "] = " +
									paramList[i - specialParamCount].Name + ";");
							}
						}

						// Call the RunStandardTestsAndGetResults for this method
						handlerMethods.Add("\t\t\t" + replacementLibraryName + ".ReplacementLibrary.RunStandardTestsAndGetResults(" +
							"new OriginalMethodCaller(CallerMethod" + handlerMethodIndex.ToString() + "), " +
							"\"" + category + "\", \"" + methodNameWithParams + "\", paramObjs, " +
							"typeof(" + GetTypeName(m.ReturnType) + "), ref retValue, ref exception);");

						// Write out the new values of any and all reference parameters
						for (int i=0; i<paramList.Length; i++)
						{
							if (paramList[i].ParameterType.IsByRef)
							{
								handlerMethods.Add("\t\t\t" + paramList[i].Name +
									" = (" + GetTypeName(paramList[i].ParameterType).Replace("ref ", "") +
									")paramObjs[" + (i + specialParamCount).ToString() + "];");
							}
						}

						handlerMethods.Add("\t\t\tDotNetReplacementLibrary.EnableNativeInterception();");

						handlerMethods.Add("\t\t\tif (exception != null) HeatDotNet.Interception.ThrowException(exception);");

						// Return the return value if there is one
						if (m.ReturnType != typeof(void))
						{
							handlerMethods.Add("\t\t\treturn (" + GetTypeName(m.ReturnType) +
								")retValue;");
						}

						handlerMethods.Add("\t\t}");
						handlerMethods.Add("");

						xmlFile.WriteLine("\t<Function>");
						xmlFile.WriteLine("\t\t<FunctionName>" + methodNameWithParams.Replace("&", "&amp;") + "</FunctionName>");
						xmlFile.WriteLine("\t\t<OriginalDll>" + Path.GetFileName(assemblyFileName) + "</OriginalDll>");
						xmlFile.WriteLine("\t\t<InterceptedDll>" + Path.GetFileNameWithoutExtension(outFileName) + ".dll</InterceptedDll>");
						xmlFile.WriteLine("\t\t<ReplacementFunctionName>HeatInterceptHandlers.HandlerClass" +
							(handlerClassIndex + 1).ToString() + "." + m.Name + "</ReplacementFunctionName>");

						if (m.ReturnType == typeof(void))
							xmlFile.WriteLine("\t\t<ReturnType>void</ReturnType>");
						else
							xmlFile.WriteLine("\t\t<ReturnType>" + m.ReturnType.FullName + "</ReturnType>");

						xmlFile.WriteLine("\t\t<Category>" + category + "</Category>");

						// don't write return values for .net functions

						if (exceptionTable.ContainsKey(methodName))
						{
							ArrayList exceptionList = (ArrayList)exceptionTable[methodName];
							foreach (string exception in exceptionList)
								xmlFile.WriteLine("\t\t<Exception>" + exception + "</Exception>");
						}

						xmlFile.WriteLine("\t\t<HelpLink>" + GetHelpLink(t, m) + "</HelpLink>");

						if (!m.IsStatic)
							xmlFile.WriteLine("\t\t<Param Index=\"0\" Type=\"" + t.FullName + "\" CompareAs=\"WideStringType\" Access=\"IN\">thisObj</Param>");
						else
							specialParamCount = 0; // Don't include MethodInterceptInfo parameter for static methods in the XML
						for (int i=0; i<paramList.Length; i++)
						{
							xmlFile.WriteLine("\t\t<Param Index=\"" + (i + specialParamCount).ToString() + "\" Type=\"" +
								paramList[i].ParameterType.FullName.Replace("&", "&amp;") + "\" CompareAs=\"WideStringType\" Access=\"" +
								(paramList[i].ParameterType.IsByRef ? "IN OUT" : "IN") + "\">" + paramList[i].Name +
								"</Param>");
						}

						xmlFile.WriteLine("\t</Function>");
					}
				}

				// Iterate through all constructors in the type and add the ones that should be included
				ConstructorInfo[] constructorList = t.GetConstructors();
				foreach (ConstructorInfo m in constructorList)
				{
					if (!m.IsPublic)
						continue;

					// Cannot intercept instance methods in a value type
					if (t.IsValueType)
						continue;

					ParameterInfo[] paramList = m.GetParameters();

					bool hasVoidPtrOrTypedRef = false;
					for (int i=0; i<paramList.Length; i++)
					{
						if (paramList[i].ParameterType.FullName == "System.TypedReference")
							hasVoidPtrOrTypedRef = true;
						else if (paramList[i].ParameterType.FullName == "System.Void*")
							hasVoidPtrOrTypedRef = true;
					}
					if (hasVoidPtrOrTypedRef)
						continue;

					string methodName = GetTypeName(t) + ".ctor";
					string methodNameWithParams = methodName + "(";
					for (int i=0; i<paramList.Length; i++)
					{
						string typeName = paramList[i].ParameterType.FullName;

						// Remove namespace information from the parameter type
						int dotIndex = typeName.LastIndexOf('.');
						if (dotIndex != -1)
							typeName = typeName.Substring(dotIndex + 1);

						methodNameWithParams += typeName;
						if ((i + 1) < paramList.Length)
							methodNameWithParams += ", ";
					}
					methodNameWithParams += ")";

					string category = ShouldInclude(methodNameWithParams);
					if (category != "")
					{
						// Method is included.  Generate a caller and handler method

						// Constructor handlers have an extra ConstructorInterceptInfo parameter
						int specialParamCount = 1;

						// Generate function signature for caller method
						handlerMethodIndex++;
						callerMethods.Add("\t\t// " + GetTypeName(t) + "." + m.Name);
						callerMethods.Add("\t\tprivate static bool CallerMethod" + handlerMethodIndex.ToString() +
							"(Object[] paramObjs, ref Object retValue, ref Exception exception)");
						callerMethods.Add("\t\t{");

						// Check if parameter list is the correct size
						callerMethods.Add("\t\t\tif (paramObjs.Length != " + (paramList.Length +
							specialParamCount).ToString() + ") return false;");

						// Generate a parameter array that does not include the first ConstructorInterceptInfo parameter
						callerMethods.Add("\t\t\tObject[] constructorParams = new Object[paramObjs.Length - 1];");
						callerMethods.Add("\t\t\tArray.Copy(paramObjs, 1, constructorParams, 0, paramObjs.Length - 1);");

						// Enclose the call to the original method in a try block so that we can catch
						// and log any exceptions that it throws.
						callerMethods.Add("\t\t\ttry");
						callerMethods.Add("\t\t\t{");

						callerMethods.Add("\t\t\t\tDotNetReplacementLibrary.EnableNativeInterception();");

						// Call the original function
						callerMethods.Add("\t\t\t\t((HeatDotNet.MethodInterceptInfo)paramObjs[0]).Invoke(constructorParams);");

						callerMethods.Add("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();");

						callerMethods.Add("\t\t\t\tretValue = null;");
						callerMethods.Add("\t\t\t\texception = null;");

						// Catch any exceptions that the original method threw
						callerMethods.Add("\t\t\t}");
						callerMethods.Add("\t\t\tcatch (Exception e)");
						callerMethods.Add("\t\t\t{");
						callerMethods.Add("\t\t\t\tretValue = null;");
						callerMethods.Add("\t\t\t\texception = e;");
						callerMethods.Add("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();");

						callerMethods.Add("\t\t\t}");

						bool hasRefParams = false;
						for (int i=0; i<paramList.Length; i++)
						{
							if (paramList[i].ParameterType.IsByRef)
								hasRefParams = true;
						}

						// Copy the updated paramater values if the function has reference parameters
						if (hasRefParams)
							callerMethods.Add("\t\t\tArray.Copy(constructorParams, 0, paramObjs, 1, paramObjs.Length - 1);");
						
						callerMethods.Add("\t\t\treturn true;");

						callerMethods.Add("\t\t}");
						callerMethods.Add("");

						// Generate the handler method
						string handlerFuncProto = "\t\tpublic static void ctor(";

						// First parameter is a MethodInterceptInfo object
						handlerFuncProto += "HeatDotNet.MethodInterceptInfo constructorIntercept";
						if (paramList.Length > 0)
							handlerFuncProto += ", ";

						// Generate parameter list for function signature
						for (int i=0; i<paramList.Length; i++)
						{
							handlerFuncProto += GetTypeName(paramList[i].ParameterType);
							handlerFuncProto += " " + paramList[i].Name;

							if ((i + 1) < paramList.Length)
								handlerFuncProto += ", ";
						}

						handlerFuncProto += ")";
						handlerMethods.Add(handlerFuncProto);
						handlerMethods.Add("\t\t{");

						handlerMethods.Add("\t\t\tDotNetReplacementLibrary.DisableNativeInterception();");

						// Create an Object array to store the parameters
						handlerMethods.Add("\t\t\tObject[] paramObjs = new Object[" +
							(paramList.Length + specialParamCount).ToString() + "];");
						handlerMethods.Add("\t\t\tObject retValue = null;");
						handlerMethods.Add("\t\t\tException exception = null;");
						for (int i=0; i<(paramList.Length + specialParamCount); i++)
						{
							if (i < specialParamCount)
								handlerMethods.Add("\t\t\tparamObjs[0] = constructorIntercept;");
							else
							{
								handlerMethods.Add("\t\t\tparamObjs[" + i.ToString() + "] = " +
									paramList[i - specialParamCount].Name + ";");
							}
						}

						// Call the RunStandardTestsAndGetResults for this method
						handlerMethods.Add("\t\t\t" + replacementLibraryName + ".ReplacementLibrary.RunStandardTestsAndGetResults(" +
							"new OriginalMethodCaller(CallerMethod" + handlerMethodIndex.ToString() + "), " +
							"\"" + category + "\", \"" + methodNameWithParams + "\", paramObjs, " +
							"typeof(void), ref retValue, ref exception);");

						// Write out the new values of any and all reference parameters
						for (int i=0; i<paramList.Length; i++)
						{
							if (paramList[i].ParameterType.IsByRef)
							{
								handlerMethods.Add("\t\t\t" + paramList[i].Name +
									" = (" + GetTypeName(paramList[i].ParameterType).Replace("ref ", "") +
									")paramObjs[" + (i + specialParamCount).ToString() + "];");
							}
						}

						handlerMethods.Add("\t\t\tDotNetReplacementLibrary.EnableNativeInterception();");

						handlerMethods.Add("\t\t\tif (exception != null) HeatDotNet.Interception.ThrowException(exception);");

						handlerMethods.Add("\t\t}");
						handlerMethods.Add("");

						xmlFile.WriteLine("\t<Function>");
						xmlFile.WriteLine("\t\t<FunctionName>" + methodNameWithParams.Replace("&", "&amp;") + "</FunctionName>");
						xmlFile.WriteLine("\t\t<OriginalDll>" + Path.GetFileName(assemblyFileName) + "</OriginalDll>");
						xmlFile.WriteLine("\t\t<InterceptedDll>" + Path.GetFileNameWithoutExtension(outFileName) + ".dll</InterceptedDll>");
						xmlFile.WriteLine("\t\t<ReplacementFunctionName>HeatInterceptHandlers.HandlerClass" +
							(handlerClassIndex + 1).ToString() + ".ctor" + "</ReplacementFunctionName>");
						xmlFile.WriteLine("\t\t<ReturnType>void</ReturnType>");

						// don't write return value tags for .net

						xmlFile.WriteLine("\t\t<Category>" + category + "</Category>");

						if (exceptionTable.ContainsKey(methodName))
						{
							ArrayList exceptionList = (ArrayList)exceptionTable[methodName];
							foreach (string exception in exceptionList)
								xmlFile.WriteLine("\t\t<Exception>" + exception + "</Exception>");
						}

						xmlFile.WriteLine("\t\t<HelpLink>" + GetHelpLink(t, m) + "</HelpLink>");

						xmlFile.WriteLine("\t\t<Param Index=\"0\" Type=\"" + t.FullName + "\" CompareAs=\"WideStringType\" Access=\"IN\">thisObj</Param>");
						for (int i=0; i<paramList.Length; i++)
						{
							xmlFile.WriteLine("\t\t<Param Index=\"" + (i + specialParamCount).ToString() + "\" Type=\"" +
								paramList[i].ParameterType.FullName + "\" CompareAs=\"WideStringType\" Access=\"" +
								(paramList[i].ParameterType.IsByRef ? "IN OUT" : "IN") + "\">" + paramList[i].Name +
								"</Param>");
						}

						xmlFile.WriteLine("\t</Function>");
					}
				}

				if ((callerMethods.Count != 0) || (handlerMethods.Count != 0))
				{
					handlerClassIndex++;
					outFile.WriteLine("\t[HeatDotNet.InterceptTypeName(\"" + GetTypeName(t) + "\")]");
					outFile.WriteLine("\tpublic class HandlerClass" + handlerClassIndex.ToString());
					outFile.WriteLine("\t{");
					foreach (string s in callerMethods)
						outFile.WriteLine(s);
					foreach (string s in handlerMethods)
						outFile.WriteLine(s);
					outFile.WriteLine("\t};");
					outFile.WriteLine();
				}
			}
			catch (Exception)
			{
			}
		}

		outFile.WriteLine("}");
		outFile.Close();
	}

	//*************************************************************************
	// Method:	 Main
	// Description: Main entry point of the application
	// Parameters:
	//	args - Command line arguments
	//
	// Return Value: None
	//*************************************************************************
	[STAThread]
	static void Main(string[] args)
	{
		if (args.Length < 5)
		{
			Help();
			return;
		}

		try
		{
			xmlFile = new StreamWriter(args[0]);
			exceptionFile = new StreamWriter(args[1]);

			xmlFile.WriteLine("<?xml version=\"1.0\" ?>");
			xmlFile.WriteLine("<!DOCTYPE Functions SYSTEM \"Functions.dtd\"[]>");
			xmlFile.WriteLine("<Functions>");

			exceptionFile.WriteLine("<?xml version=\"1.0\" ?>");
			exceptionFile.WriteLine("<!DOCTYPE Exceptions SYSTEM \"Exceptions.dtd\"[]>");
			exceptionFile.WriteLine("<Exceptions>");

			// Loop through each set of input arguments.  Each set has three arguments, the
			// first is the original .DLL file name, the second is the .INI file name, and
			// the third is the output .CS file name.
			for (int i = 2; i<args.Length; i += 3)
				GenerateLibrary(args[i], args[i + 1], args[i + 2]);

			xmlFile.WriteLine("</Functions>");
			xmlFile.Close();

			exceptionFile.WriteLine("</Exceptions>");
			exceptionFile.Close();
		}
		catch (Exception e)
		{
			Console.WriteLine("Error: " + e.Message);
		}
	}
}
