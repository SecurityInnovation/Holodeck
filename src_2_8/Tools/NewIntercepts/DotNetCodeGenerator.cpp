//***********************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DotNetCodeGenerator.cpp
//
// DESCRIPTION: Contains implementation for the class DotNetCodeGenerator
//
//===================================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 Nov 2003		 Ady K		 Ported from Rusty's GenerateReplacmentLibrary.cs
//***********************************************************************************

#include "DotNetCodeGenerator.h"

namespace AddNewIntercepts
{
	DotNetCodeGenerator::DotNetCodeGenerator(bool isAddNewIntercepts)
	{
		this->isAddNewIntercepts = isAddNewIntercepts;

		//Get the template path from the registry
		Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		this->holodeckPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		this->gacUtilPath = dynamic_cast<String *> (key->GetValue(TOOLS_PATH_VALUE_NAME, new String(".")));
		this->gacUtilPath = String::Concat (holodeckPath, Path::DirectorySeparatorChar.ToString(), this->gacUtilPath, Path::DirectorySeparatorChar.ToString(), S"gacutil.exe");
		this->replacementFolderPath = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));
		this->replacementFolderPath = String::Concat (holodeckPath, Path::DirectorySeparatorChar.ToString(), this->replacementFolderPath);
		this->templatePath = dynamic_cast<String *> (key->GetValue(NEWINTERCEPTS_TPL_PATH_NAME, new String(".")));
		this->templatePath = String::Concat(holodeckPath, Path::DirectorySeparatorChar.ToString(), this->templatePath);
		this->holodeckDatabaseFolderPath = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		this->holodeckDatabaseFolderPath = String::Concat (holodeckPath, Path::DirectorySeparatorChar.ToString(), this->holodeckDatabaseFolderPath);
		
		if (! templatePath->EndsWith (Path::DirectorySeparatorChar.ToString()))
			templatePath = String::Concat (templatePath, Path::DirectorySeparatorChar.ToString());

		this->frameworkDirectory = NULL;
	}

	DotNetCodeGenerator::~DotNetCodeGenerator()
	{

	}

	//*************************************************************************
	// Method:	 CreateProject
	// Description: Creates a C# Class Library project with interception code
	//				for the requested functions.
	// Parameters:
	//	projectFileName - the path of the project file (Ex: c:\temp\replace_lib.csproj)
	//	libraryPathList - a list of .NET libraries, from which to generate interception routines
	//	interceptArray - an array of InterceptedFunction types
	//
	// Return Value: None
	//*************************************************************************
	void DotNetCodeGenerator::CreateProject (String * projectFileName, ArrayList * libraryPathList, ArrayList * interceptArray)
	{
		if (interceptArray->Count < 1)
			return;

		//Set internal variables
		
		//Create a hashtable of all the methods for faster lookup
		this->methodsTable = new Hashtable();
		ArrayList * methodList = NULL;
		for (int i=0; i < interceptArray->Count; i++)
		{
			InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (interceptArray->Item[i]);
			String * funcNameWOParams = ifunc->Name;
			if (funcNameWOParams->IndexOf ('(') > 0)
				funcNameWOParams = funcNameWOParams->Substring (0, funcNameWOParams->IndexOf ('('));

			if (this->methodsTable->ContainsKey (funcNameWOParams))
			{
				methodList = dynamic_cast <ArrayList*> (this->methodsTable->Item[funcNameWOParams]);
				methodList->Add (ifunc);
			}
			else
			{
				methodList = new ArrayList();
				methodList->Add (ifunc);
				this->methodsTable->Add (funcNameWOParams, methodList);
			}
		}

		//Define a ProjectInformation object
		String * projectPath = Path::GetDirectoryName (projectFileName);
		String * projectName = Path::GetFileNameWithoutExtension (projectFileName);
		String * csFile = String::Concat (projectPath, Path::DirectorySeparatorChar.ToString(), projectName, new String(".cs"));

		if ((! File::Exists (String::Concat (this->templatePath, "AssemblyInfo.cs.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "DotNetProject.csproj.dat"))))
		{
			MessageBox::Show ("Error: Template files required for code generation could not be located.\nPlease reinstall Holodeck", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		//Create the target directory
		try
		{
			Directory::CreateDirectory (projectPath);
		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("Error:\n", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}

		//Create the .cs file
		this->GenerateLibrary (libraryPathList, interceptArray, csFile);

		//Create the .csproj file
		this->GenerateProjectFile (projectFileName, libraryPathList);
	}

	//*************************************************************************
	// Method:	 GenerateProjectFile
	// Description: Generates the .csproj file for the replacement library
	//
	// Parameters:
	//	projFileName - Path and name of the project file to be created
	//	libraryPathList - a list of original library path and names to
	//					add to the the project references
	//
	// Return Value: None
	//*************************************************************************	
	void DotNetCodeGenerator::GenerateProjectFile (String * projFileName, ArrayList * libraryPathList)
	{
		//Get the target directory
		String * targetDirectory = Path::GetDirectoryName (projFileName);
		if (!targetDirectory->EndsWith (Path::DirectorySeparatorChar.ToString()))
			targetDirectory = String::Concat (targetDirectory, Path::DirectorySeparatorChar.ToString());

		//Copy the AssemblyInfo file from the template directory to the target directory		
		File::Copy (String::Concat (this->templatePath, "AssemblyInfo.cs.dat"),
					String::Concat (targetDirectory, "AssemblyInfo.cs"), true);

		//Make the assembly.cs file not read-only
		File::SetAttributes (String::Concat (targetDirectory, "AssemblyInfo.cs"), static_cast <FileAttributes>(File::GetAttributes(String::Concat (targetDirectory, "AssemblyInfo.cs")) & ~FileAttributes::ReadOnly));

		//Open the .csproj template file for reading
		StreamReader * projTemplate = File::OpenText (String::Concat (this->templatePath, "DotNetProject.csproj.dat"));
		//Open the .csproj output file for writing
		StreamWriter * projWriter = File::CreateText (projFileName);

		String * tplString = "";
		String * parsedString = "";
		while ((tplString = projTemplate->ReadLine()) != NULL) 
		{
			parsedString = tplString;
			if ((parsedString->Trim()->StartsWith ("#IF_NEW_INTERCEPTS#") && !this->isAddNewIntercepts))
				continue;
			else
				parsedString = parsedString->Replace ("#IF_NEW_INTERCEPTS#", "");
			
			if ((parsedString->Trim()->StartsWith ("#IF_NOT_NEW_INTERCEPTS#") && this->isAddNewIntercepts))
				continue;
			else
				parsedString = parsedString->Replace ("#IF_NOT_NEW_INTERCEPTS#", "");

			parsedString = parsedString->Replace ("$PROJECT_NAME$", Path::GetFileNameWithoutExtension (projFileName));
			parsedString = parsedString->Replace ("$REPLACEMENT_FOLDER_PATH$", this->replacementFolderPath);
			parsedString = parsedString->Replace ("$ORIGINAL_LIB_REFERENCES$", this->getLibReferencesText(libraryPathList));
			parsedString = parsedString->Replace ("$HDDB_FOLDER_PATH$", this->holodeckDatabaseFolderPath);
			projWriter->WriteLine (parsedString);
		}
		
		//Flush and close the output stream
		projWriter->Flush();
		projWriter->Close();
		//Close the input template stream
		projTemplate->Close();
	}

	//*************************************************************************
	// Method:	 GenerateLibrary
	// Description: Generates a replacement library for a given module.
	// Parameters:
	//	assemblyFileName - Path to the original module to generate the
	//		replacement library for
	//  interceptMethods - a list of methods for which interception code needs to be
	//		generated
	//  outFileName - Path to the .cs output file containing the replacement
	//		library code
	//
	// Return Value: None
	//*************************************************************************
	void DotNetCodeGenerator::GenerateLibrary(ArrayList * assemblyFileNameList, ArrayList * interceptMethods, String * outFileName)
	{
		this->GetFrameworkDir();

		//Get the full path to functions.dtd
		String *dtdPath;
		String *functionDBFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		if(functionDBFolder != NULL)
		{
			if (functionDBFolder->EndsWith(Path::DirectorySeparatorChar.ToString()))
				dtdPath = String::Concat(functionDBFolder, "functions.dtd");
			else
				dtdPath = String::Concat(functionDBFolder, Path::DirectorySeparatorChar.ToString(), "functions.dtd");
		}
		else
			dtdPath = "functions.dtd";


		//Get the XML file name from the .CS filename
		String * xmlFileName = String::Concat (Path::GetDirectoryName(outFileName), Path::DirectorySeparatorChar.ToString(), Path::GetFileNameWithoutExtension (outFileName), S".xml");
		xmlFile = new StreamWriter(xmlFileName);

		xmlFile->WriteLine("<?xml version=\"1.0\" ?>");
		xmlFile->WriteLine(String::Concat ("<!DOCTYPE Functions SYSTEM \"", dtdPath, "\"[]>"));
		xmlFile->WriteLine("<Functions>");

		//Open the output C# file for writing
		StreamWriter * outFile = new StreamWriter(outFileName);
		
		String * replacementLibraryName = "";
		//Begin writing the code file
		outFile->WriteLine("// This file was automatically generated by Holodeck DotNet Code Generator.");
		outFile->WriteLine();
		outFile->WriteLine("using Replacement;");
		outFile->WriteLine("using System;");
		outFile->WriteLine();
		outFile->WriteLine();
		outFile->WriteLine("namespace Replacement");
		outFile->WriteLine("{");
		for (int asmCount=0; asmCount < assemblyFileNameList->Count; asmCount++)
		{
			replacementLibraryName = Path::GetFileName (dynamic_cast <String *> (assemblyFileNameList->Item[asmCount]));
			outFile->WriteLine(String::Concat ("\tpublic class " , String::Concat (Path::GetFileNameWithoutExtension (replacementLibraryName)->Replace (".", "_"), "ReplacementClass")));
			outFile->WriteLine("\t{");
			outFile->WriteLine(String::Concat ("\t\tpublic static DotNetReplacementLibrary ReplacementLibrary = new DotNetReplacementLibrary(\"" , replacementLibraryName , "\");"));
			outFile->WriteLine("\t};");
		}
		outFile->WriteLine("}");
		outFile->WriteLine();
		outFile->WriteLine();
		outFile->WriteLine("namespace HeatInterceptHandlers");
		outFile->WriteLine("{");

		String * assemblyFileName = "";
		String * replacementLibraryClassName = "";
		Assembly * asmInfo;
		
		int handlerMethodIndex = 0;
		int handlerClassIndex = 0;

		Hashtable* alreadyIncludedList = new Hashtable();

		for (int asmListCounter=0; asmListCounter < assemblyFileNameList->Count; asmListCounter++)
		{
			assemblyFileName = dynamic_cast <String *> (assemblyFileNameList->Item[asmListCounter]);
			replacementLibraryClassName = String::Concat (Path::GetFileNameWithoutExtension(assemblyFileName)->Replace(".", "_"), "ReplacementClass");
			
			asmInfo = this->LoadAssembly (assemblyFileName, true);

			if (!asmInfo)
			{
				MessageBox::Show (String::Concat (S"Could not load the assembly:\r\n", assemblyFileName, S"\r\nInterception code for this library file will not be generated"), S"Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
				continue;;
			}

			//Parse the IL code to look for which exceptions each method throws
			Hashtable * exceptionTable = new Hashtable();
			
			Type * typeList[] = NULL;

			try
			{
				typeList = asmInfo->GetTypes();
			}
			catch (Exception *)
			{
				MessageBox::Show (String::Concat (S"Could not get type modules from the assembly ", Path::GetFileNameWithoutExtension(assemblyFileName), S"\r\n\r\nThis module will be skipped"), S"Holodeck");
			}
			
			if (!typeList)
				continue;

			for (int tCounter=0; tCounter < typeList->Count; tCounter++)
			{
				Type * t = dynamic_cast <Type *> (typeList->Item[tCounter]);
				try
				{
					// Don't intercept enumerations, non-top level classes,
					// or delegates
					if (t->IsEnum)
						continue;
					if (t->FullName->IndexOf("+") != -1)
						continue;
					if (t->BaseType == __typeof(MulticastDelegate))
						continue;
					if (t->BaseType == __typeof(Delegate))
						continue;

					// Create some containers for the caller and handler method code
					ArrayList * callerMethods = new ArrayList();
					ArrayList * handlerMethods = new ArrayList();

					// Iterate through all methods in the type and add the ones that should be included
					//Get the public methods
					MethodInfo * public_methodList[] = t->GetMethods();
					//Get the private methods
					MethodInfo * private_methodList[] = t->GetMethods(static_cast <BindingFlags> (BindingFlags::NonPublic|BindingFlags::Instance));
					
					//Combine the two method lists
					ArrayList * methodList = new ArrayList (public_methodList->Count + private_methodList->Count);
					
					for (int addPrivate=0; addPrivate < private_methodList->Count; addPrivate++)
						methodList->Add (private_methodList->Item[addPrivate]);
					
					for (int addPublic=0; addPublic < public_methodList->Count; addPublic++)
						methodList->Add (public_methodList->Item[addPublic]);
					
					for (int mCounter=0; mCounter < methodList->Count; mCounter++)
					{
						MethodInfo * m = dynamic_cast <MethodInfo *> (methodList->Item[mCounter]);
						
						if (m->IsAbstract)
							continue;
						
						ParameterInfo * paramList[] = m->GetParameters();

						String * methodName = String::Concat (t->FullName, ".", m->Name);
						String * methodNameWithParams = this->GetMethodNameWithParams (methodName, paramList);
						String * category = ShouldInclude(methodName, paramList);

						// Don't generate a method with the same signature twice
						if (alreadyIncludedList->ContainsKey(methodNameWithParams))
							continue;

						if (category != "")
						{
							// Method is included.  Generate a caller and handler method
							alreadyIncludedList->Add(methodNameWithParams, __box(true));

							// Method always has a MethodInterceptInfo parameter
							int specialParamCount = 1;

							// Generate function signature for caller method
							handlerMethodIndex++;
							
							callerMethods->Add(String::Concat ("\t\t// ", GetTypeName(t), ".", m->Name));
							callerMethods->Add(String::Concat ("\t\tprivate static bool CallerMethod", handlerMethodIndex.ToString(), "(Object[] paramObjs, ref Object retValue, ref Exception exception)"));
							callerMethods->Add(new String ("\t\t{"));

							// Check if parameter list is the correct size
							callerMethods->Add(String::Concat ("\t\t\tif (paramObjs.Length != ", (paramList->Length +
								specialParamCount).ToString(), ") return false;"));

							// Generate a parameter array that does not include the first MethodInterceptInfo parameter
							callerMethods->Add(new String ("\t\t\tObject[] methodParams = new Object[paramObjs.Length - 1];"));
							callerMethods->Add(new String ("\t\t\tArray.Copy(paramObjs, 1, methodParams, 0, paramObjs.Length - 1);"));

							// Enclose the call to the original method in a try block so that we can catch
							// and log any exceptions that it throws.
							callerMethods->Add(new String ("\t\t\ttry"));
							callerMethods->Add(new String ("\t\t\t{"));

							callerMethods->Add(new String ("\t\t\t\tDotNetReplacementLibrary.EnableNativeInterception();"));
							callerMethods->Add(new String ("\t\t\t\tHeatDotNet.Interception.EnableInterceptionInCurrentThread();"));
							callerMethods->Add(new String ("\t\t\t\tHeatDotNet.Interception.BeginForceOriginalCode();"));

							// Generate name of original function
							String * origCallStr = "((HeatDotNet.MethodInterceptInfo)paramObjs[0]).Invoke(methodParams)";

							// Store the return value if there is one
							if (m->ReturnType != __typeof(void))
								origCallStr = String::Concat ("retValue = ", origCallStr);
							callerMethods->Add(String::Concat ("\t\t\t\t", origCallStr, ";"));

							callerMethods->Add(new String ("\t\t\t\tHeatDotNet.Interception.DisableInterceptionInCurrentThread();"));
							callerMethods->Add(new String ("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();"));

							if (m->ReturnType == __typeof(void))
								callerMethods->Add(new String ("\t\t\t\tretValue = null;"));
							callerMethods->Add(new String ("\t\t\t\texception = null;"));

							// Catch any exceptions that the original method threw
							callerMethods->Add(new String ("\t\t\t}"));
							callerMethods->Add(new String ("\t\t\tcatch (Exception e)"));
							callerMethods->Add(new String ("\t\t\t{"));
							callerMethods->Add(new String ("\t\t\t\tretValue = null;"));
							callerMethods->Add(new String ("\t\t\t\texception = e;"));
							callerMethods->Add(new String ("\t\t\t\tHeatDotNet.Interception.DisableInterceptionInCurrentThread();"));
							callerMethods->Add(new String ("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();"));

							callerMethods->Add(new String ("\t\t\t}"));

							bool hasRefParams = false;
							for (int i=0; i<paramList->Length; i++)
							{
								if (paramList[i]->ParameterType->IsByRef)
									hasRefParams = true;
							}

							// Copy the updated paramater values if the function has reference parameters
							if (hasRefParams)
								callerMethods->Add(new String ("\t\t\tArray.Copy(methodParams, 0, paramObjs, 1, paramObjs.Length - 1);"));
							
							callerMethods->Add(new String ("\t\t\treturn true;"));

							callerMethods->Add(new String ("\t\t}"));
							callerMethods->Add(new String (""));

							// Generate the handler method
							String * handlerFuncProto = String::Concat (new String ("\t\tpublic static "), GetTypeName(m->ReturnType), new String (" "), m->Name, new String ("("));

							handlerFuncProto = String::Concat (handlerFuncProto, "HeatDotNet.MethodInterceptInfo methodIntercept");
							if (paramList->Length > 0)
								handlerFuncProto = String::Concat (handlerFuncProto, ", ");

							// Generate parameter list for function signature
							for (int i=0; i<paramList->Length; i++)
							{
								handlerFuncProto = String::Concat (handlerFuncProto, GetTypeName(paramList[i]->ParameterType));
								handlerFuncProto = String::Concat (handlerFuncProto, " ", paramList[i]->Name);

								if ((i + 1) < paramList->Length)
									handlerFuncProto = String::Concat (handlerFuncProto, ", ");
							}

							handlerFuncProto = String::Concat (handlerFuncProto, ")");

							handlerMethods->Add(handlerFuncProto);
							handlerMethods->Add(new String ("\t\t{"));

							handlerMethods->Add(new String ("\t\t\tDotNetReplacementLibrary.DisableNativeInterception();"));

							// Create an Object array to store the parameters
							handlerMethods->Add(String::Concat ("\t\t\tObject[] paramObjs = new Object[",
								(paramList->Length + specialParamCount).ToString(), "];"));
							handlerMethods->Add(new String ("\t\t\tObject retValue = null;"));
							handlerMethods->Add(new String ("\t\t\tException exception = null;"));
							for (int i=0; i<(paramList->Length + specialParamCount); i++)
							{
								if (i < specialParamCount)
									handlerMethods->Add(new String ("\t\t\tparamObjs[0] = methodIntercept;"));
								else
								{
									handlerMethods->Add(String::Concat (new String ("\t\t\tparamObjs["), i.ToString(), new String ("] = "),
										paramList[i - specialParamCount]->Name, new String (";")));
								}
							}

							// Call the RunStandardTestsAndGetResults for this method
							handlerMethods->Add(String::Concat (new String ("\t\t\t"), replacementLibraryClassName, new String (".ReplacementLibrary.RunStandardTestsAndGetResults("),
								new String ("new OriginalMethodCaller(CallerMethod"), handlerMethodIndex.ToString(), new String ("), "),
								new String ("\""), category, new String ("\", \""), methodNameWithParams, new String ("\", paramObjs, "),
								new String ("typeof("), GetTypeName(m->ReturnType), new String ("), ref retValue, ref exception);")));

							// Write out the new values of any and all reference parameters
							for (int i=0; i<paramList->Length; i++)
							{
								if (paramList[i]->ParameterType->IsByRef)
								{
									handlerMethods->Add(String::Concat (new String ("\t\t\t"), paramList[i]->Name,
										new String (" = ("), GetTypeName(paramList[i]->ParameterType)->Replace("ref ", ""),
										new String (")paramObjs["), (i + specialParamCount).ToString(), new String ("];")));
								}
							}

							handlerMethods->Add(new String ("\t\t\tDotNetReplacementLibrary.EnableNativeInterception();"));

							handlerMethods->Add(new String ("\t\t\tif (exception != null) HeatDotNet.Interception.ThrowException(exception);"));

							// Return the return value if there is one
							if (m->ReturnType != __typeof(void))
							{
								handlerMethods->Add(String::Concat ("\t\t\treturn (", GetTypeName(m->ReturnType), ")retValue;"));
							}

							handlerMethods->Add(new String ("\t\t}"));
							handlerMethods->Add(new String (""));

							xmlFile->WriteLine("\t<Function>");
							xmlFile->WriteLine(String::Concat ("\t\t<FunctionName>", methodNameWithParams->Replace("&", "&amp;"), "</FunctionName>"));
							xmlFile->WriteLine(String::Concat ("\t\t<OriginalDll>", Path::GetFileName(assemblyFileName), "</OriginalDll>"));
							xmlFile->WriteLine(String::Concat ("\t\t<InterceptedDll>", Path::GetFileNameWithoutExtension(outFileName), ".dll</InterceptedDll>"));
							xmlFile->WriteLine(String::Concat (new String ("\t\t<ReplacementFunctionName>HeatInterceptHandlers."),
								replacementLibraryName->Replace(".", "_"), new String("HandlerClass"),
								(handlerClassIndex + 1).ToString(), new String ("."), m->Name, new String ("</ReplacementFunctionName>")));

							if (m->ReturnType == __typeof(void))
								xmlFile->WriteLine("\t\t<ReturnType>void</ReturnType>");
							else
								xmlFile->WriteLine(String::Concat ("\t\t<ReturnType>", m->ReturnType->FullName, "</ReturnType>"));

							xmlFile->WriteLine(String::Concat ("\t\t<Category>", category, "</Category>"));

							// don't write return values for .net functions

							if (exceptionTable->ContainsKey(methodName))
							{
								ArrayList * exceptionList = dynamic_cast <ArrayList *>(exceptionTable->Item[methodName]);
								for (int eCounter=0; eCounter < exceptionList->Count; eCounter++)
								{
									String * exception = dynamic_cast <String *> (exceptionList->Item[eCounter]);
									xmlFile->WriteLine(String::Concat ("\t\t<Exception>", exception, "</Exception>"));
								}
							}

							xmlFile->WriteLine(String::Concat ("\t\t<HelpLink>", GetHelpLink(t, m), "</HelpLink>"));

							if (!m->IsStatic)
								xmlFile->WriteLine(String::Concat ("\t\t<Param Index=\"0\" Type=\"", t->FullName, "\" CompareAs=\"WideStringType\" Access=\"IN\">thisObj</Param>"));
							else
								specialParamCount = 0; // Don't include MethodInterceptInfo parameter for static methods in the XML
							for (int i=0; i<paramList->Length; i++)
							{
								xmlFile->WriteLine(String::Concat (new String ("\t\t<Param Index=\""), (i + specialParamCount).ToString(), new String ("\" Type=\""),
									paramList[i]->ParameterType->FullName->Replace("&", "&amp;"), new String ("\" CompareAs=\"WideStringType\" Access=\""),
									(paramList[i]->ParameterType->IsByRef ? "IN OUT" : "IN"), new String ("\">"), paramList[i]->Name,
									new String ("</Param>")));
							}

							xmlFile->WriteLine("\t</Function>");
						}
					}
					
					// Iterate through all constructors in the type and add the ones that should be included
					ConstructorInfo * constructorList[] = t->GetConstructors();
					for (int ciCounter=0; ciCounter < constructorList->Count; ciCounter++)
					{
						ConstructorInfo * m = dynamic_cast <ConstructorInfo *> (constructorList->Item[ciCounter]);
						
						/*if (!m->IsPublic)
							continue;*/

						ParameterInfo * paramList[] = m->GetParameters();

						String * methodName = String::Concat (GetTypeName(t), new String(".ctor"));
						String * methodNameWithParams = this->GetMethodNameWithParams (methodName, paramList);
						String * category = ShouldInclude(methodName, paramList);

						if (category != "")
						{
							// Method is included.  Generate a caller and handler method

							// Constructor handlers have an extra ConstructorInterceptInfo parameter
							int specialParamCount = 1;

							// Generate function signature for caller method
							handlerMethodIndex++;
							callerMethods->Add(String::Concat (new String("\t\t// "), GetTypeName(t), new String("."), m->Name));
							callerMethods->Add(String::Concat (new String ("\t\tprivate static bool CallerMethod"), handlerMethodIndex.ToString(),
								new String ("(Object[] paramObjs, ref Object retValue, ref Exception exception)")));
							callerMethods->Add(new String ("\t\t{"));

							// Check if parameter list is the correct size
							callerMethods->Add(String::Concat (new String("\t\t\tif (paramObjs.Length != "), (paramList->Length +
								specialParamCount).ToString(), new String (") return false;")));

							// Generate a parameter array that does not include the first ConstructorInterceptInfo parameter
							callerMethods->Add(new String("\t\t\tObject[] constructorParams = new Object[paramObjs.Length - 1];"));
							callerMethods->Add(new String ("\t\t\tArray.Copy(paramObjs, 1, constructorParams, 0, paramObjs.Length - 1);"));

							// Enclose the call to the original method in a try block so that we can catch
							// and log any exceptions that it throws.
							callerMethods->Add(new String("\t\t\ttry"));
							callerMethods->Add(new String ("\t\t\t{"));

							callerMethods->Add(new String ("\t\t\t\tDotNetReplacementLibrary.EnableNativeInterception();"));

							// Call the original function
							callerMethods->Add(new String ("\t\t\t\t((HeatDotNet.MethodInterceptInfo)paramObjs[0]).Invoke(constructorParams);"));

							callerMethods->Add(new String ("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();"));

							callerMethods->Add(new String ("\t\t\t\tretValue = null;"));
							callerMethods->Add(new String ("\t\t\t\texception = null;"));

							// Catch any exceptions that the original method threw
							callerMethods->Add(new String ("\t\t\t}"));
							callerMethods->Add(new String ("\t\t\tcatch (Exception e)"));
							callerMethods->Add(new String ("\t\t\t{"));
							callerMethods->Add(new String ("\t\t\t\tretValue = null;"));
							callerMethods->Add(new String ("\t\t\t\texception = e;"));
							callerMethods->Add(new String ("\t\t\t\tDotNetReplacementLibrary.DisableNativeInterception();"));

							callerMethods->Add(new String ("\t\t\t}"));

							bool hasRefParams = false;
							for (int i=0; i<paramList->Length; i++)
							{
								if (paramList[i]->ParameterType->IsByRef)
									hasRefParams = true;
							}

							// Copy the updated paramater values if the function has reference parameters
							if (hasRefParams)
								callerMethods->Add(new String ("\t\t\tArray.Copy(constructorParams, 0, paramObjs, 1, paramObjs.Length - 1);"));
							
							callerMethods->Add(new String ("\t\t\treturn true;"));

							callerMethods->Add(new String ("\t\t}"));
							callerMethods->Add(new String (""));

							// Generate the handler method
							String * handlerFuncProto = "\t\tpublic static void ctor(";

							// First parameter is a MethodInterceptInfo object
							handlerFuncProto = String::Concat (handlerFuncProto, new String ("HeatDotNet.MethodInterceptInfo constructorIntercept"));
							if (paramList->Length > 0)
								handlerFuncProto = String::Concat (handlerFuncProto, new String (", "));

							// Generate parameter list for function signature
							for (int i=0; i<paramList->Length; i++)
							{
								handlerFuncProto = String::Concat (handlerFuncProto, GetTypeName(paramList[i]->ParameterType));
								handlerFuncProto = String::Concat (handlerFuncProto,  new String(" "),  paramList[i]->Name);

								if ((i + 1) < paramList->Length)
									handlerFuncProto = String::Concat (handlerFuncProto, ", ");
							}

							handlerFuncProto = String::Concat (handlerFuncProto, new String(")"));
							handlerMethods->Add(handlerFuncProto);
							handlerMethods->Add(new String ("\t\t{"));

							handlerMethods->Add(new String ("\t\t\tDotNetReplacementLibrary.DisableNativeInterception();"));

							// Create an Object array to store the parameters
							handlerMethods->Add(String::Concat (new String ("\t\t\tObject[] paramObjs = new Object["),
								(paramList->Length + specialParamCount).ToString(), new String ("];")));
							handlerMethods->Add(new String ("\t\t\tObject retValue = null;"));
							handlerMethods->Add(new String ("\t\t\tException exception = null;"));
							
							for (int i=0; i<(paramList->Length + specialParamCount); i++)
							{
								if (i < specialParamCount)
									handlerMethods->Add(new String ("\t\t\tparamObjs[0] = constructorIntercept;"));
								else
								{
									handlerMethods->Add(String::Concat (new String ("\t\t\tparamObjs["), i.ToString(),new String ("] = "),
										paramList[i - specialParamCount]->Name, new String (";")));
								}
							}

							// Call the RunStandardTestsAndGetResults for this method
							handlerMethods->Add(String::Concat (new String ("\t\t\t"), replacementLibraryClassName, new String (".ReplacementLibrary.RunStandardTestsAndGetResults("),
								new String ("new OriginalMethodCaller(CallerMethod"), handlerMethodIndex.ToString(), new String ("), "),
								new String ("\""), category, new String ("\", \""), methodNameWithParams, new String ("\", paramObjs, "),
								new String ("typeof(void), ref retValue, ref exception);")));

							// Write out the new values of any and all reference parameters
							for (int i=0; i<paramList->Length; i++)
							{
								if (paramList[i]->ParameterType->IsByRef)
								{
									handlerMethods->Add(String::Concat (new String("\t\t\t"), paramList[i]->Name,
										new String(" = ("), GetTypeName(paramList[i]->ParameterType)->Replace("ref ", ""),
										new String(")paramObjs["), (i + specialParamCount).ToString(), new String( "];")));
								}
							}

							handlerMethods->Add(new String ("\t\t\tDotNetReplacementLibrary.EnableNativeInterception();"));

							handlerMethods->Add(new String ("\t\t\tif (exception != null) HeatDotNet.Interception.ThrowException(exception);"));

							handlerMethods->Add(new String("\t\t}"));
							handlerMethods->Add(new String (""));

							xmlFile->WriteLine("\t<Function>");
							xmlFile->WriteLine(String::Concat (new String("\t\t<FunctionName>"), methodNameWithParams->Replace("&", "&amp;"), new String("</FunctionName>")));
							xmlFile->WriteLine(String::Concat (new String("\t\t<OriginalDll>"), Path::GetFileName(assemblyFileName), new String("</OriginalDll>")));
							xmlFile->WriteLine(String::Concat (new String("\t\t<InterceptedDll>"), Path::GetFileNameWithoutExtension(outFileName), new String(".dll</InterceptedDll>")));
							xmlFile->WriteLine(String::Concat (new String("\t\t<ReplacementFunctionName>HeatInterceptHandlers."),
								replacementLibraryName->Replace(".", "_"), new String("HandlerClass"),
								(handlerClassIndex + 1).ToString(), new String(".ctor"), new String("</ReplacementFunctionName>")));
							xmlFile->WriteLine("\t\t<ReturnType>void</ReturnType>");

							// don't write return value tags for .net

							xmlFile->WriteLine(String::Concat (new String("\t\t<Category>"), category, new String("</Category>")));

							if (exceptionTable->ContainsKey(methodName))
							{
								ArrayList * exceptionList = dynamic_cast <ArrayList *> (exceptionTable->Item[methodName]);
								for (int eCounter=0; eCounter<exceptionList->Count; eCounter++)
								{
									String * exception = dynamic_cast <String *> (exceptionList->Item[eCounter]);
									xmlFile->WriteLine(String::Concat (new String("\t\t<Exception>"), exception, new String ("</Exception>")));
								}
							}

							xmlFile->WriteLine(String::Concat (new String("\t\t<HelpLink>"), GetHelpLink(t, m), new String ("</HelpLink>")));

							xmlFile->WriteLine(String::Concat (new String("\t\t<Param Index=\"0\" Type=\""), t->FullName, new String( "\" CompareAs=\"WideStringType\" Access=\"IN\">thisObj</Param>")));
							for (int i=0; i<paramList->Length; i++)
							{
								xmlFile->WriteLine(String::Concat (new String("\t\t<Param Index=\""), (i + specialParamCount).ToString(), new String("\" Type=\""),
									paramList[i]->ParameterType->FullName, new String("\" CompareAs=\"WideStringType\" Access=\""),
									(paramList[i]->ParameterType->IsByRef ? "IN OUT" : "IN"), new String("\">"), paramList[i]->Name,
									new String("</Param>")));
							}

							xmlFile->WriteLine("\t</Function>");
						}
					}

					if ((callerMethods->Count != 0) || (handlerMethods->Count != 0))
					{
						handlerClassIndex++;
						outFile->WriteLine(String::Concat (new String("\t[HeatDotNet.InterceptTypeName(\""), GetTypeName(t), new String("\")]")));
						outFile->WriteLine(String::Concat (new String ("\tpublic class "),
							replacementLibraryName->Replace(".", "_"), new String("HandlerClass"),
							handlerClassIndex.ToString()));
						outFile->WriteLine("\t{");
						for (int sCounter=0; sCounter < callerMethods->Count; sCounter++)
						{
							String * s = dynamic_cast <String *> (callerMethods->Item[sCounter]);
							outFile->WriteLine(s);
						}
							
						for (int sCounter=0; sCounter < handlerMethods->Count; sCounter++)
						{
							String * s = dynamic_cast <String *> (handlerMethods->Item[sCounter]);
							outFile->WriteLine(s);
						}

						outFile->WriteLine("\t};");
						outFile->WriteLine();
					}
				}
				catch (Exception *)
				{
				}
			}
		}

		outFile->WriteLine("}");
		outFile->Close();

		xmlFile->WriteLine("</Functions>");
		xmlFile->Close();
	}

	//*************************************************************************
	// Method:	 ShouldInclude
	// Description: Determines if a function should be intercepted
	// Parameters:
	//	methodName - name of method
	//	paramList - params of method [array of type ParameterInfo]
	//
	// Return Value: Category name that the function resides in, or the
	//		empty string if it shouldn't be intercepted
	//*************************************************************************
	String * DotNetCodeGenerator::ShouldInclude(String * methodName, ParameterInfo * paramList[])
	{
		String * category = "";
		String * methodNameWithoutParams = "";

		if (!this->methodsTable->ContainsKey (methodName))
			return S"";

		ArrayList * methodsArray = dynamic_cast <ArrayList*> (this->methodsTable->Item[methodName]);
		for (int f=0; f < methodsArray->Count; f++)
		{
			InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (methodsArray->Item[f]);
			
			if (ifunc->Name->IndexOf ('(') == -1)
				methodNameWithoutParams = ifunc->Name;
			else
				methodNameWithoutParams = ifunc->Name->Substring (0, ifunc->Name->IndexOf ('('));

			//If method names and paramCount's match, then compare param types
			if ((String::Compare (methodNameWithoutParams, methodName) == 0) && (ifunc->ParameterCount == paramList->Count))
			{
				bool paramsMatch = true;	
				for (unsigned int p=0; p < ifunc->ParameterCount; p++)
				{
					InterceptedFunctionParameter * ifuncparam = dynamic_cast <InterceptedFunctionParameter *> (ifunc->Parameters->Item[p]);
					ParameterInfo * paramInfo = dynamic_cast <ParameterInfo *> (paramList->Item[p]);

					//If any one param type does not match, then fail match
					if (String::Compare (ifuncparam->Type, paramInfo->ParameterType->ToString()->Trim(), true) != 0)
						paramsMatch = false;
				}

				//If the params also matched, then return the category
				if (paramsMatch)
					return ifunc->Category;
			}
			
		}

		//If no match found, then return empty string
		return "";
	}

	//*************************************************************************
	// Method:	 GetMethodNameWithParams
	// Description: Returns a method name along with param info in ()
	//				Ex. FunctionName (ParamType1, ParamType2 ...)
	//
	// Parameters:
	//	methodName - name of the method
	//	paramList - an array of type ParameterInfo
	//
	// Return Value: Function definition based on the information provided
	//*************************************************************************
	String * DotNetCodeGenerator::GetMethodNameWithParams (String * methodName, ParameterInfo * paramList[])
	{
		String * methodNameWithParams = String::Concat (methodName, "(");
		
		for (int i=0; i<paramList->Length; i++)
		{
			String * typeName;
			if (paramList[i]->ParameterType->IsPublic)
				typeName = paramList[i]->ParameterType->FullName;
			else
				typeName = "System.Object";

			// Remove namespace information from the parameter type
			int dotIndex = typeName->LastIndexOf('.');
			if (dotIndex != -1)
				typeName = typeName->Substring(dotIndex + 1);

			methodNameWithParams = String::Concat (methodNameWithParams, typeName);
			if ((i + 1) < paramList->Length)
				methodNameWithParams = String::Concat (methodNameWithParams, ", ");
		}
		
		methodNameWithParams = String::Concat (methodNameWithParams, ")");

		return methodNameWithParams;
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
	String * DotNetCodeGenerator::GetTypeName(Type * t)
	{
		if (t == __typeof(void))
			return "void";
		if (t->IsByRef)
		{
			if (t->IsPublic)
				return String::Concat ("ref " , t->FullName->Replace("+", ".")->Replace("&", ""));
			return "ref System.Object";
		}
		if (t->IsPublic)
			return t->FullName->Replace("+", ".");
		return "System.Object";
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
	String * DotNetCodeGenerator::GetHelpLink(System::Type * t, System::Reflection::ConstructorInfo *c)
	{
		String * className;

		if (t == c->DeclaringType)
			className = t->FullName;
		else
			className = c->DeclaringType->FullName;

		String * classWithoutDots = className->Replace(".", "");
		classWithoutDots = classWithoutDots->ToLower();
		String * url = String::Concat (new String("http://msdn.microsoft.com/library/default.asp?url=/library/en-us/cpref/html/frlrf") , classWithoutDots , new String("class") , c->Name->ToLower()->Replace(".", "") , new String("topic.asp"));

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
	String * DotNetCodeGenerator::GetHelpLink(System::Type * t, System::Reflection::MethodInfo *m)
	{
		String * className;

		if (t == m->DeclaringType)
			className = t->FullName;
		else
			className = m->DeclaringType->FullName;

		String * classWithoutDots = className->Replace(".", "");
		classWithoutDots = classWithoutDots->ToLower();
		String * url = String::Concat (new String("http://msdn.microsoft.com/library/default.asp?url=/library/en-us/cpref/html/frlrf"), classWithoutDots, new String("class"), m->Name->ToLower(), new String("topic.asp"));

		return url;
	}

	//*************************************************************************
	// Method:	 GetFrameworkDir
	// Description: Locates the .NET Framework directory and stores the value
	//				in an internal variable
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void DotNetCodeGenerator::GetFrameworkDir()
	{
		this->frameworkDirectory = String::Concat (System::Web::HttpRuntime::ClrInstallDirectory, Path::DirectorySeparatorChar.ToString());
	}

	//*************************************************************************
	// Method:	 getLibReferencesText
	// Description: Forms Reference Tags for use in csproj files based on the
	//				list of libraries provided
	// Parameters:
	//	listOfLibraries - list of libraries to generate Reference tags for
	//
	// Return Value: Formatted string of reference tags
	//*************************************************************************
	String* DotNetCodeGenerator::getLibReferencesText(ArrayList * listOfLibraries)
	{
		if (!listOfLibraries || listOfLibraries->Count == 0)
			return "";

		String * refTag = S"<Reference Name = \"$LIBRARY_NAME$\" AssemblyName = \"$ASSEMBLY_NAME$\" HintPath = \"$LIBRARY_PATH$\"/>";
		String * referenceTags = S"";
		String * workTag = S"";
		for (int i=0; i <listOfLibraries->Count; i++)
		{
			String * lib = dynamic_cast <String *> (listOfLibraries->Item[i]);
			System::Reflection::Assembly * asmb = NULL;
			asmb = this->LoadAssembly (lib, false);
			if (!asmb)
				continue;

			workTag = refTag;
			workTag = workTag->Replace ("$LIBRARY_NAME$", Path::GetFileNameWithoutExtension (lib));
			workTag = workTag->Replace ("$ASSEMBLY_NAME$", asmb->FullName->Substring (0, asmb->FullName->IndexOf (',')));
			workTag = workTag->Replace ("$LIBRARY_PATH$", asmb->Location);
			referenceTags = String::Concat (referenceTags, workTag, "\r\n\t\t");
		}

		return referenceTags;
	}

	//*************************************************************************
	// Method:	 LoadAssembly
	// Description: Loads an assembly using a variety of methods
	//
	// Parameters:
	//	assemblyString - name of assembly file to load, partial name or assembly name
	//	askUserIfNotFound - it true, user is prompted to locate the assembly file
	//
	// Return Value: Assembly object if loaded, NULL otherwise
	//*************************************************************************
	System::Reflection::Assembly* DotNetCodeGenerator::LoadAssembly (String * assemblyString, bool askUserIfNotFound)
	{
		//Get framework directory
		if (this->frameworkDirectory == NULL)
			this->GetFrameworkDir();
		
		System::Reflection::Assembly* asmInfo = NULL;
		
		try
		{
			if (File::Exists (assemblyString))
				asmInfo = Assembly::LoadFrom(assemblyString);
			else
				asmInfo = Assembly::LoadWithPartialName (assemblyString);

			//if could not load even with partial name then try looking for in framework directory
			//if still not found, then ask user what to do
			if (!asmInfo)
			{
				String * tempPath = String::Concat (this->frameworkDirectory, assemblyString);
				if (File::Exists (tempPath))
					asmInfo = Assembly::LoadFile(tempPath);
				else
				{
					if (askUserIfNotFound)
					{
						if (MessageBox::Show (String::Concat (S"Holodeck could not locate the assembly:\r\n", assemblyString, S"\r\nWould you like to locate it yourself? Selecting 'No' will skip interception methods for this assembly"), S"Holodeck", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == DialogResult::Yes)
						{
							if (!fileDialog)
								fileDialog = new OpenFileDialog();

							fileDialog->Filter = "Assembly files (*.dll)|*.dll|All files (*.*)|*.*" ;
							fileDialog->CheckFileExists = true;
							fileDialog->CheckPathExists = true;
							if (fileDialog->ShowDialog() == DialogResult::OK)
								asmInfo = Assembly::LoadFile(fileDialog->FileName);
						}
					}
				}
			}
		}
		catch (...)
		{
			return NULL;
		}

		return asmInfo;
	}

}