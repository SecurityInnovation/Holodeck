//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReplacementLibraryGenerator.cs
//
// DESCRIPTION: Generates xml and replacement libraries based on the access db
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Mar 2004		 B. Shirey	 File created.
//*************************************************************************
using System;
using System.IO;
using Microsoft.Win32;
using System.Windows.Forms;
using System.Data;
using System.Data.OleDb;
using System.Collections;

namespace GenerateNativeReplacementLibraries
{
	//*************************************************************************
	// class that generates the xml and replacement libraries
	//*************************************************************************
	public class ReplacementLibraryGenerator
	{
		#region Variables

		private readonly string xmlFileRelativePath = "..\\FunctionDatabase\\functions.xml";
		private readonly string functionsDbRelativePath = "..\\Tools\\GenerateNativeReplacementLibraries\\";

		private readonly string RegRoot;
		private readonly RegistryKey RegRootKey;

		private string xmlFilePath;
		private string connectionString;
		private System.Data.OleDb.OleDbConnection dbConnection;

		#endregion

		#region Properties

		public string XMLFileName
		{
			get
			{
				return xmlFilePath;
			}
		}

		#endregion

		#region Constructors and Destructors

		//*************************************************************************
		// Method:		ReplacementLibraryGenerator
		// Description: Constructor for the ReplacementLibraryGenerator class
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		public ReplacementLibraryGenerator()
		{
			RegRoot = "Software\\Security Innovation\\Holodeck Enterprise Edition";
			RegRootKey = Registry.LocalMachine.CreateSubKey(RegRoot);

			xmlFilePath = String.Concat(GetInstallPath(), "\\", xmlFileRelativePath);
			connectionString = String.Concat("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=", GetInstallPath(), "\\");
			connectionString = String.Concat(connectionString, functionsDbRelativePath, "functions.mdb");

			dbConnection = new OleDbConnection(connectionString);
			dbConnection.Open();
		}

		//*************************************************************************
		// Method:		~ReplacementLibraryGenerator
		// Description: Destructor for the ReplacementLibraryGenerator class
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		~ReplacementLibraryGenerator()
		{
		}


		#endregion

		#region Helper Functions

		//*************************************************************************
		// Method:		GetInstallPath
		// Description: Gets the install path out of the registry
		//
		// Parameters:
		//	None
		//
		// Return Value: the install path for Holodeck out of the registry
		//*************************************************************************
		protected String GetInstallPath()
		{
			String installPath = "";
			String defaultValue = "C:\\source_code\\commercial\\Holodeck\\src\\HolodeckGui";
			if (RegRootKey != null)
			{
				installPath = (String)RegRootKey.GetValue("InstallPath");
				if (installPath != "")
					return installPath;
				else if (System.IO.Directory.Exists(defaultValue))
					return defaultValue;
				else
				{
					System.Windows.Forms.MessageBox.Show("The Holodeck installation path could not be read from the registry, Holodeck will exit now", "Holodeck");
					throw new Exception("Installation Path Not Found");
				}

			}
			else
			{
				if (System.IO.Directory.Exists(defaultValue))
					return installPath;
				else
				{
					System.Windows.Forms.MessageBox.Show("The Holodeck installation path could not be read from the registry, Holodeck will exit now", "Holodeck");
					throw new Exception("Installation Path Not Found");
				}
			}
		}


		#endregion

		#region XML Generating Functions

		//*************************************************************************
		// Method:		GenerateXML
		// Description: Generates the xml file from the database
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		public void GenerateXML()
		{
			File.SetAttributes(xmlFilePath, FileAttributes.Normal);

			DataSet functionsTableDataSet = new DataSet();
			OleDbDataAdapter functionsTableDataAdapter = new OleDbDataAdapter();
			functionsTableDataAdapter.SelectCommand = new OleDbCommand("SELECT * FROM Functions WHERE IncludeInBuild=Yes ORDER BY ID", dbConnection);
			functionsTableDataAdapter.Fill(functionsTableDataSet);

			StreamWriter xmlWriter = new StreamWriter(xmlFilePath);
			xmlWriter.WriteLine("<?xml version=\"1.0\" ?><!DOCTYPE Functions SYSTEM \"functions.dtd\"[]>");
			xmlWriter.WriteLine("<Functions>");

			// used in the inner loops
			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();

			if (functionsTableDataSet.Tables.Count > 0)
			{
				DataTable functionsDataTable = functionsTableDataSet.Tables[0];
				IEnumerator enumerator = functionsDataTable.Rows.GetEnumerator();
				while (enumerator.MoveNext())
				{
					DataRow currentRow = (DataRow)enumerator.Current;
					if (currentRow == null)
						continue;

					System.Console.WriteLine(String.Format("Generating XML For Function {0}", currentRow["FunctionName"]));

					xmlWriter.WriteLine("\t<Function>");

					// write out the function name
					xmlWriter.Write("\t\t<FunctionName>");
					xmlWriter.Write(currentRow["FunctionName"]);
					xmlWriter.WriteLine("</FunctionName>");

					// write out the original dll
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Dll FROM Dlls WHERE ID={0}", currentRow["OriginalDll"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write("\t\t<OriginalDll>");
					xmlWriter.Write(dataSet.Tables[0].Rows[0][0]);
					xmlWriter.WriteLine("</OriginalDll>");

					// write out the intercepted dll
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Dll FROM Dlls WHERE ID={0}", currentRow["InterceptedDll"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write("\t\t<InterceptedDll>");
					xmlWriter.Write(dataSet.Tables[0].Rows[0][0]);
					xmlWriter.WriteLine("</InterceptedDll>");

					// write out the replacement function name
					xmlWriter.Write("\t\t<ReplacementFunctionName>");
					xmlWriter.Write(currentRow["ReplacementFunctionName"]);
					xmlWriter.WriteLine("</ReplacementFunctionName>");

					// write out the return type
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReturnType FROM ReturnTypes WHERE ID={0}", currentRow["ReturnType"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write("\t\t<ReturnType>");
					xmlWriter.Write(dataSet.Tables[0].Rows[0][1]);
					xmlWriter.WriteLine("</ReturnType>");

					// write out the type modifier if there is one
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT TypeModifier FROM TypeModifiers WHERE ID={0}", currentRow["TypeModifier"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write("\t\t<TypeModifier>");
					if (dataSet.Tables[0].Rows.Count > 0)
					{
						xmlWriter.Write(dataSet.Tables[0].Rows[0][2]);
					}
					xmlWriter.WriteLine("</TypeModifier>");

					// write out the calling convention modifier if there is one
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CallingConventionModifier FROM CallingConventionModifiers WHERE ID={0}", currentRow["CallingConventionModifier"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write("\t\t<CallingConventionModifier>");
					if (dataSet.Tables[0].Rows.Count > 0)
					{
						xmlWriter.Write(dataSet.Tables[0].Rows[0][3]);
					}
					xmlWriter.WriteLine("</CallingConventionModifier>");

					// write out the category
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Category FROM Categories WHERE ID={0}", currentRow["Category"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write("\t\t<Category>");
					xmlWriter.Write(dataSet.Tables[0].Rows[0][4]);
					xmlWriter.WriteLine("</Category>");

					uint functionID = Convert.ToUInt32(currentRow["ID"].ToString());
					GenerateXMLReturnValues(functionID, xmlWriter);
					GenerateXMLErrorCodes(functionID, xmlWriter);

					// write out the helplink if there is one
					if (!currentRow["HelpLink"].ToString().Equals(""))
					{
						xmlWriter.Write("\t\t<HelpLink>");
						xmlWriter.Write(currentRow["HelpLink"]);
						xmlWriter.WriteLine("</HelpLink>");
					}

					GenerateXMLParams(functionID, xmlWriter);

					xmlWriter.WriteLine("\t</Function>");
				}
			}

			xmlWriter.WriteLine("</Functions>");
			xmlWriter.Close();

			File.SetAttributes(xmlFilePath, FileAttributes.ReadOnly);
		}

		//*************************************************************************
		// Method:		GenerateXMLReturnValues
		// Description: Writes out the return values section of each function in the
		//	xml file
		//
		// Parameters:
		//	functionID - the function id of the function to write the return values for
		//	xmlWriter - the object used to write out the xml to a file
		//
		// Return Value: None
		//*************************************************************************
		protected void GenerateXMLReturnValues(uint functionID, StreamWriter xmlWriter)
		{
			DataSet mainDataSet = new DataSet();
			OleDbDataAdapter mainDataAdapter = new OleDbDataAdapter();
			mainDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM ReturnValues WHERE FunctionID={0}", functionID.ToString()), dbConnection);
			mainDataAdapter.Fill(mainDataSet);

			// used in the inner loops
			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();

			IEnumerator enumerator = mainDataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				xmlWriter.Write("\t\t<ReturnValue Type=\"");

				// write out the type
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReturnValueType FROM ReturnValueTypes WHERE ID={0}", currentRow["Type"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				xmlWriter.Write(dataSet.Tables[0].Rows[0][0]);

				if (Convert.ToUInt32(currentRow["ValueOperator"].ToString()) != 6)
				{
					xmlWriter.Write("\" ValueOperator=\"");

					// write out the value operator type
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReturnValueOperator FROM ReturnValueOperators WHERE ID={0}", currentRow["ValueOperator"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write(dataSet.Tables[0].Rows[0][1]);
				}

				xmlWriter.Write("\">");
				xmlWriter.Write(currentRow["ReturnValue"]);
				xmlWriter.WriteLine("</ReturnValue>");
			}
		}

		//*************************************************************************
		// Method:		GenerateXMLErrorCodes
		// Description: Writes out the error codes section of each function in the
		//	xml file
		//
		// Parameters:
		//	functionID - the function id of the function to write the error codes for
		//	xmlWriter - the object used to write out the xml to a file
		//
		// Return Value: None
		//*************************************************************************
		protected void GenerateXMLErrorCodes(uint functionID, StreamWriter xmlWriter)
		{
			DataSet mainDataSet = new DataSet();
			OleDbDataAdapter mainDataAdapter = new OleDbDataAdapter();
			mainDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM ErrorCodes WHERE FunctionID={0}", functionID.ToString()), dbConnection);
			mainDataAdapter.Fill(mainDataSet);

			// used in the inner loops
			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();

			IEnumerator enumerator = mainDataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				xmlWriter.Write("\t\t<ErrorCode IdentifyBy=\"");

				// write out the identify by type
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT IdentifyBy FROM ErrorCodeIdentifyByValues WHERE ID={0}", currentRow["IdentifyBy"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				xmlWriter.Write(dataSet.Tables[0].Rows[0][0]);

				xmlWriter.Write("\" Type=\"");

				// write out the type
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Type FROM ErrorCodeTypes WHERE ID={0}", currentRow["Type"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				xmlWriter.Write(dataSet.Tables[0].Rows[0][1]);

				xmlWriter.Write("\">");

				// write out the value
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Name FROM KnownErrorCodes WHERE ID={0}", currentRow["ErrorCode"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				xmlWriter.Write(dataSet.Tables[0].Rows[0][2]);
				xmlWriter.WriteLine("</ErrorCode>");
			}
		}

		//*************************************************************************
		// Method:		GenerateXMLParams
		// Description: Writes out the parameters section of each function in the
		//	xml file
		//
		// Parameters:
		//	functionID - the function id of the function to write the params for
		//	xmlWriter - the object used to write out the xml to a file
		//
		// Return Value: None
		//*************************************************************************
		protected void GenerateXMLParams(uint functionID, StreamWriter xmlWriter)
		{
			DataSet mainDataSet = new DataSet();
			OleDbDataAdapter mainDataAdapter = new OleDbDataAdapter();
			mainDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM ParameterValues WHERE FunctionID={0}", functionID.ToString()), dbConnection);
			mainDataAdapter.Fill(mainDataSet);

			// used in the inner loops
			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();

			IEnumerator enumerator = mainDataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				xmlWriter.Write("\t\t<Param Index=\"");

				// write out the index
				xmlWriter.Write(currentRow["Index"]);

				xmlWriter.Write("\" Type=\"");

				// write out the type
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ParameterType FROM ParameterTypes WHERE ID={0}", currentRow["Type"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				xmlWriter.Write(dataSet.Tables[0].Rows[0][0]);

				xmlWriter.Write("\" CompareAs=\"");

				// write out the compare as value
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CompareAsType FROM CompareAsTypes WHERE ID={0}", currentRow["CompareAs"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				xmlWriter.Write(dataSet.Tables[0].Rows[0][1]);
				xmlWriter.Write("\"");

				int index = 2;

				// write out the access type if there is one
				if (!currentRow["Access"].Equals(DBNull.Value) && Convert.ToUInt32(currentRow["Access"].ToString()) > 0)
				{
					xmlWriter.Write(" Access=\"");
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Access FROM AccessTypes WHERE ID={0}", currentRow["Access"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write(dataSet.Tables[0].Rows[0][index]);
					xmlWriter.Write("\"");
					index++;
				}

				// write out the points to resource value if there is one
				if (!currentRow["PointsToResource"].Equals(DBNull.Value) && Convert.ToInt32(currentRow["PointsToResource"].ToString()) > 0)
				{
					xmlWriter.Write(" PointsToResource=\"");
					dataSet.Clear();
					dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT PointsToResourceValue FROM PointsToResourceValues WHERE ID={0}", currentRow["PointsToResource"].ToString()), dbConnection);
					dataAdapter.Fill(dataSet);
					xmlWriter.Write(dataSet.Tables[0].Rows[0][index]);
					xmlWriter.Write("\"");
					index++;
				}

				// write out the concat with value if there is one
				if (!currentRow["ConcatWith"].Equals(DBNull.Value) && !currentRow["ConcatWith"].ToString().Equals(""))
				{
					xmlWriter.Write(" ConcatWith=\"");
					xmlWriter.Write(currentRow["ConcatWith"]);
					xmlWriter.Write("\"");
				}

				// write out the map to value if there is one
				if (!currentRow["MapTo"].Equals(DBNull.Value) && !currentRow["MapTo"].ToString().Equals(""))
				{
					xmlWriter.Write(" MapTo=\"");
					xmlWriter.Write(currentRow["MapTo"]);
					xmlWriter.Write("\"");
				}

				xmlWriter.Write(">");
				xmlWriter.Write(currentRow["Name"]);
				xmlWriter.WriteLine("</Param>");
			}
		}


		#endregion

		#region Replacement Library Generating Functions

		//*************************************************************************
		// Method:		GenerateReplacementLibraries
		// Description: generates the replacement libraries based on the database
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		public void GenerateReplacementLibraries()
		{
			DataSet replacementLibraryTableDataSet = new DataSet();
			OleDbDataAdapter replacementLibraryTableDataAdapter = new OleDbDataAdapter();
			replacementLibraryTableDataAdapter.SelectCommand = new OleDbCommand("SELECT * FROM ReplacementLibraryGeneration ORDER BY ID", dbConnection);
			replacementLibraryTableDataAdapter.Fill(replacementLibraryTableDataSet);

			IEnumerator enumerator = replacementLibraryTableDataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				uint replacementDllInt = Convert.ToUInt32(currentRow["ReplacementDll"].ToString());
				uint originalDllInt = Convert.ToUInt32(currentRow["OriginalDll"].ToString());

				DataSet dataSet = new DataSet();
				OleDbDataAdapter dataAdapter = new OleDbDataAdapter();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Dll FROM Dlls WHERE ID={0}", replacementDllInt.ToString()), dbConnection);
				dataAdapter.Fill(dataSet);

				string replacementLibrary = dataSet.Tables[0].Rows[0][0].ToString();
				string defTemplate = String.Concat(GetInstallPath(), "\\", functionsDbRelativePath, currentRow["TemplateFilePathForDefGeneration"]);
				string hTemplate = String.Concat(GetInstallPath(), "\\", functionsDbRelativePath, currentRow["TemplateFilePathForHGeneration"]);
				string cppTemplate = String.Concat(GetInstallPath(), "\\", functionsDbRelativePath, currentRow["TemplateFilePathForCppGeneration"]);
				string defOutputFile = String.Concat(GetInstallPath(), "\\", currentRow["OutputDefFilePath"].ToString());
				string hOutputFile = String.Concat(GetInstallPath(), "\\", currentRow["OutputHFilePath"].ToString());
				string cppOutputFile = String.Concat(GetInstallPath(), "\\", currentRow["OutputCppFilePath"].ToString());

				// check for existence of the defDatFile for the row
				if (!File.Exists(defTemplate))
				{
					System.Console.WriteLine(String.Format("DEF Template File {0} does not exist.  Skipping generation of {1}", defTemplate, replacementLibrary));
					continue;
				}

				// check for existence of the hDatFile for the row
				if (!File.Exists(hTemplate))
				{
					System.Console.WriteLine(String.Format("H Template File {0} does not exist.  Skipping generation of {1}", hTemplate, replacementLibrary));
					continue;
				}

				// check for existence of the hDatFile for the row
				if (!File.Exists(cppTemplate))
				{
					System.Console.WriteLine(String.Format("CPP Template File {0} does not exist.  Skipping generation of {1}", cppTemplate, replacementLibrary));
					continue;
				}

				System.Console.WriteLine(String.Format("Starting generation of replacement library {0}", replacementLibrary));
				GenerateReplacementLibraryDefFile(originalDllInt, replacementDllInt, defOutputFile, defTemplate);
				GenerateReplacementLibraryHFile(originalDllInt, replacementDllInt, hOutputFile, hTemplate);
				GenerateReplacementLibraryCppFile(originalDllInt, replacementDllInt, cppOutputFile, cppTemplate);
				System.Console.WriteLine(String.Format("Generation of replacement library {0} complete", replacementLibrary));
			}
		}

		//*************************************************************************
		// Method:		GenerateReplacementLibraryDefFile
		// Description: generates the def file for the replacement library specfied
		//
		// Parameters:
		//	originalDllInt - the id of the original dll in the database
		//	replacementDllInt - the id of the replacement dll in the database
		//	outputFileName - the output file to write to
		//	templateFileName - the template to use as the basis for the output file
		//
		// Return Value: None
		//*************************************************************************
		protected void GenerateReplacementLibraryDefFile(uint originalDllInt, uint replacementDllInt, string outputFileName, string templateFileName)
		{
			System.Console.WriteLine(String.Format("Generating DEF file {0}", outputFileName));
			StreamReader reader = new StreamReader(templateFileName);

			File.SetAttributes(outputFileName, FileAttributes.Normal);
			StreamWriter writer = new StreamWriter(outputFileName);

			string line = reader.ReadLine();
			while (line != null)
			{
				if (line.IndexOf("[REPLACEMENTFUNCTIONS]") == -1)
				{
					writer.WriteLine(line);
				}
				else
				{
					ReplaceFunctionsInDefFile(replacementDllInt, writer);
				}

				line = reader.ReadLine();
			}

			writer.Close();
			reader.Close();

			File.SetAttributes(outputFileName, FileAttributes.ReadOnly);
			System.Console.WriteLine(String.Format("Generation of DEF file {0} complete", outputFileName));
		}

		//*************************************************************************
		// Method:		ReplaceFunctionsInDefFile
		// Description: replaces the [REPLACEMENTFUNCTIONS] tag in the template file
		//	with the exported functions
		//
		// Parameters:
		//	replacementDllInt - the id of the replacement dll in the database
		//	writer - the object used to write the functions
		//
		// Return Value: None
		//*************************************************************************
		protected void ReplaceFunctionsInDefFile(uint replacementDllInt, StreamWriter writer)
		{
			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();
			dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReplacementFunctionName FROM Functions WHERE InterceptedDll={0} ORDER BY ID", replacementDllInt.ToString()), dbConnection);
			dataAdapter.Fill(dataSet);

			IEnumerator enumerator = dataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				string functionName = currentRow["ReplacementFunctionName"].ToString();

				System.Console.WriteLine(String.Format("Adding function {0} to the DEF file", functionName));
				writer.WriteLine(functionName);
			}
		}

		//*************************************************************************
		// Method:		ReplaceModificationTimeStamp
		// Description: replaces the [MODIFICATIONTIMESTAMP] tag in a template file
		//
		// Parameters:
		//	writer - the object used to write the timestamp
		//
		// Return Value: None
		//*************************************************************************
		protected void ReplaceModificationTimeStamp(StreamWriter writer)
		{
			writer.WriteLine(String.Concat("// Generated ", DateTime.Now.ToString("MM/dd/yyyy hh:mm:ss tt")));
		}

		//*************************************************************************
		// Method:		GenerateReplacementLibraryHFile
		// Description: generates the h file for the replacement library specfied
		//
		// Parameters:
		//	originalDllInt - the id of the original dll in the database
		//	replacementDllInt - the id of the replacement dll in the database
		//	outputFileName - the output file to write to
		//	templateFileName - the template to use as the basis for the output file
		//
		// Return Value: None
		//*************************************************************************
		protected void GenerateReplacementLibraryHFile(uint originalDllInt, uint replacementDllInt, string outputFileName, string templateFileName)
		{
			System.Console.WriteLine(String.Format("Generating H file {0}", outputFileName));
			StreamReader reader = new StreamReader(templateFileName);

			File.SetAttributes(outputFileName, FileAttributes.Normal);
			StreamWriter writer = new StreamWriter(outputFileName);

			string line = reader.ReadLine();
			while (line != null)
			{
				if (line.IndexOf("[MODIFICATIONTIMESTAMP]") != -1)
				{
					ReplaceModificationTimeStamp(writer);
				}
				else if (line.IndexOf("[REPLACEMENTFUNCTIONPOINTERS]") != -1)
				{
					ReplaceFunctionPointersInHFile(replacementDllInt, writer);
				}
				else if (line.IndexOf("[REPLACEMENTFUNCTIONPOINTERTYPEDEFS]") != -1)
				{
					ReplaceFunctionPointerTypedefsInHFile(replacementDllInt, writer);
				}
				else
				{
					writer.WriteLine(line);
				}

				line = reader.ReadLine();
			}

			writer.Close();
			reader.Close();

			File.SetAttributes(outputFileName, FileAttributes.ReadOnly);
			System.Console.WriteLine(String.Format("Generation of H file {0} complete", outputFileName));
		}

		//*************************************************************************
		// Method:		ReplaceFunctionPointersInHFile
		// Description: replaces the [REPLACEMENTFUNCTIONPOINTERS] tag in the template file
		//
		// Parameters:
		//	replacementDllInt - the id of the replacement dll in the database
		//	writer - the object used to write the functions
		//
		// Return Value: None
		//*************************************************************************
		protected void ReplaceFunctionPointersInHFile(uint replacementDllInt, StreamWriter writer)
		{
			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();
			dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT FunctionName FROM Functions WHERE InterceptedDll={0} ORDER BY ID", replacementDllInt.ToString()), dbConnection);
			dataAdapter.Fill(dataSet);

			IEnumerator enumerator = dataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				string functionName = currentRow["FunctionName"].ToString();
				string lineToAdd = String.Concat(functionName, "Function real", functionName, " = NULL;");

				System.Console.WriteLine(String.Format("Adding function pointer real{0} to the H file", functionName));
				writer.WriteLine(lineToAdd);
			}
		}

		//*************************************************************************
		// Method:		ReplaceFunctionPointersTypedefsInHFile
		// Description: replaces the [REPLACEMENTFUNCTIONPOINTERTYPEDEFS] tag in the template file
		//
		// Parameters:
		//	replacementDllInt - the id of the replacement dll in the database
		//	writer - the object used to write the functions
		//
		// Return Value: None
		//*************************************************************************
		protected void ReplaceFunctionPointerTypedefsInHFile(uint replacementDllInt, StreamWriter writer)
		{
			DataSet functionDataSet = new DataSet();
			OleDbDataAdapter functionDataAdapter = new OleDbDataAdapter();
			functionDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM Functions WHERE InterceptedDll={0} ORDER BY ID", replacementDllInt.ToString()), dbConnection);
			functionDataAdapter.Fill(functionDataSet);

			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();

			IEnumerator enumerator = functionDataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				string functionName = currentRow["FunctionName"].ToString();
				string lineToAdd = "typedef ";
				
				// get the type modifier if there is one
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT TypeModifier FROM TypeModifiers WHERE ID={0}", currentRow["TypeModifier"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				if (dataSet.Tables[0].Rows.Count > 0)
				{
					lineToAdd = String.Concat(lineToAdd, dataSet.Tables[0].Rows[0]["TypeModifier"], " ");
				}

				// get the return type
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReturnType FROM ReturnTypes WHERE ID={0}", currentRow["ReturnType"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				lineToAdd = String.Concat(lineToAdd, dataSet.Tables[0].Rows[0]["ReturnType"], " (");

				// get the type modifier if there is one
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CallingConventionModifier FROM CallingConventionModifiers WHERE ID={0}", currentRow["CallingConventionModifier"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				if (dataSet.Tables[0].Rows.Count > 0)
				{
					lineToAdd = String.Concat(lineToAdd, dataSet.Tables[0].Rows[0]["CallingConventionModifier"], " ");
				}

				// add the function name
				lineToAdd = String.Concat(lineToAdd, "*", functionName, "Function)(");

				// add the parameter types
				dataSet.Clear();
				dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM ParameterValues WHERE FunctionID={0} ORDER BY Index", currentRow["ID"].ToString()), dbConnection);
				dataAdapter.Fill(dataSet);
				IEnumerator paramEnumerator = dataSet.Tables[0].Rows.GetEnumerator();
				int i = 0;
				while (paramEnumerator.MoveNext())
				{
					DataRow currentParamRow = (DataRow)paramEnumerator.Current;
					if (currentParamRow == null)
						continue;

					DataSet paramDataSet = new DataSet();
					OleDbDataAdapter paramDataAdapter = new OleDbDataAdapter();
					paramDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ParameterType FROM ParameterTypes WHERE ID={0}", currentParamRow["Type"]), dbConnection);
					paramDataAdapter.Fill(paramDataSet);

					if (i > 0)
						lineToAdd = String.Concat(lineToAdd, ", ");

					lineToAdd = String.Concat(lineToAdd, paramDataSet.Tables[0].Rows[0]["ParameterType"]);

					i++;
				}

				lineToAdd = String.Concat(lineToAdd, ");");

				System.Console.WriteLine(String.Format("Adding function pointer typedef {0}Function to the H file", functionName));
				writer.WriteLine(lineToAdd);
			}
		}

		//*************************************************************************
		// Method:		GenerateReplacementLibraryCppFile
		// Description: generates the cpp file for the replacement library specfied
		//
		// Parameters:
		//	originalDllInt - the id of the original dll in the database
		//	replacementDllInt - the id of the replacement dll in the database
		//	outputFileName - the output file to write to
		//	templateFileName - the template to use as the basis for the output file
		//
		// Return Value: None
		//*************************************************************************
		protected void GenerateReplacementLibraryCppFile(uint originalDllInt, uint replacementDllInt, string outputFileName, string templateFileName)
		{
			System.Console.WriteLine(String.Format("Generating CPP file {0}", outputFileName));
			StreamReader reader = new StreamReader(templateFileName);

			File.SetAttributes(outputFileName, FileAttributes.Normal);
			StreamWriter writer = new StreamWriter(outputFileName);

			string line = reader.ReadLine();
			while (line != null)
			{
				if (line.IndexOf("[MODIFICATIONTIMESTAMP]") != -1)
				{
					ReplaceModificationTimeStamp(writer);
				}
				else if (line.IndexOf("[STARTREPLACEMENTCALLERFUNCTIONS]") != -1)
				{
					ReplaceCallerFunctionsInCppFile(replacementDllInt, reader, writer);
				}
				else if (line.IndexOf("[STARTREPLACEMENTFUNCTIONS]") != -1)
				{
					ReplaceReplacementFunctionsInCppFile(replacementDllInt, reader, writer);
				}
				else
				{
					writer.WriteLine(line);
				}

				line = reader.ReadLine();
			}

			writer.Close();
			reader.Close();

			File.SetAttributes(outputFileName, FileAttributes.ReadOnly);
			System.Console.WriteLine(String.Format("Generation of CPP file {0} complete", outputFileName));
		}

		//*************************************************************************
		// Method:		ReplaceCallerFunctionsInCppFile
		// Description: replaces caller functions in the template with the real code
		//
		// Parameters:
		//	replacementDllInt - the id of the replacement dll in the database
		//	reader - the template reader
		//	writer - the cpp output writer
		//
		// Return Value: None
		//*************************************************************************
		protected void ReplaceCallerFunctionsInCppFile(uint replacementDllInt, StreamReader reader, StreamWriter writer)
		{
			string templatedFunction = "";
			string line = reader.ReadLine();
			while ((line != null) && (line.IndexOf("[ENDREPLACEMENTCALLERFUNCTIONS]") == -1))
			{
				templatedFunction = String.Concat(templatedFunction, line, "\r\n");
				line = reader.ReadLine();
			}

			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();
			dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM Functions WHERE InterceptedDll={0} ORDER BY ID", replacementDllInt.ToString()), dbConnection);
			dataAdapter.Fill(dataSet);

			// for all the functions in this dll
			IEnumerator enumerator = dataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				// first check for a custom caller function.  if we find one, output it not the templated one
				DataSet customDataSet = new DataSet();
				OleDbDataAdapter customDataAdapter = new OleDbDataAdapter();
				customDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM CustomCallerFunctions WHERE FunctionID={0}", currentRow["ID"].ToString()), dbConnection);
				customDataAdapter.Fill(customDataSet);

				if (customDataSet.Tables[0].Rows.Count > 0)
				{
					writer.WriteLine(customDataSet.Tables[0].Rows[0]["CallerFunction"].ToString());
					continue;
				}

				// get all the params
				DataSet paramDataSet = new DataSet();
				OleDbDataAdapter paramDataAdapter = new OleDbDataAdapter();
				paramDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM ParameterValues WHERE FunctionID={0} ORDER BY Index", currentRow["ID"].ToString()), dbConnection);
				paramDataAdapter.Fill(paramDataSet);
				int paramIndex = 0;
				string paramsString = "";
				IEnumerator paramEnumerator = paramDataSet.Tables[0].Rows.GetEnumerator();
				while (paramEnumerator.MoveNext())
				{
					DataRow currentParamRow = (DataRow)paramEnumerator.Current;
					if (currentParamRow == null)
						continue;

					if (paramIndex > 0)
						paramsString = String.Concat(paramsString, ", ");

					// get the parameter type
					DataSet paramTypeDataSet = new DataSet();
					OleDbDataAdapter paramTypeDataAdapter = new OleDbDataAdapter();
					paramTypeDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ParameterType FROM ParameterTypes WHERE ID={0}", currentParamRow["Type"].ToString()), dbConnection);
					paramTypeDataAdapter.Fill(paramTypeDataSet);

					string paramType = paramTypeDataSet.Tables[0].Rows[0]["ParameterType"].ToString();
					
					paramTypeDataSet.Clear();
					paramTypeDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CompareAsType FROM CompareAsTypes WHERE ID={0}", currentParamRow["CompareAs"].ToString()), dbConnection);
					paramTypeDataAdapter.Fill(paramTypeDataSet);

					string compareAsType = paramTypeDataSet.Tables[0].Rows[0]["CompareAsType"].ToString();

					string typeToAdd = "";
					if (compareAsType.Equals("StringType") || compareAsType.Equals("WideStringType"))
						typeToAdd = "SiString";
					else
						typeToAdd = paramType;

					paramsString = String.Concat(paramsString, String.Format("*(({0} *)params[{1}])", typeToAdd, paramIndex.ToString()));

					paramIndex++;
				}

				string numParameters = paramDataSet.Tables[0].Rows.Count.ToString();
				string functionName = currentRow["FunctionName"].ToString();

				// check for a custom caller function pointer
				DataSet tempDataSet = new DataSet();
				OleDbDataAdapter tempDataAdapter = new OleDbDataAdapter();
				tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CustomCaller FROM CustomCallerFunctionPointers WHERE FunctionID={0}", currentRow["ID"].ToString()), dbConnection);
				tempDataAdapter.Fill(tempDataSet);

				string callerName = "";
				if (tempDataSet.Tables[0].Rows.Count > 0)
				{
					callerName = tempDataSet.Tables[0].Rows[0]["CustomCaller"].ToString();
				}
				else
				{
					callerName = String.Concat("real", functionName);
				}

				// get the return type
				tempDataSet.Clear();
				tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReturnType FROM ReturnTypes WHERE ID={0}", currentRow["ReturnType"].ToString()), dbConnection);
				tempDataAdapter.Fill(tempDataSet);
				string returnType = tempDataSet.Tables[0].Rows[0]["ReturnType"].ToString();

				// check the return value for void
				bool isVoid = returnType.ToLower().Equals("void");

				// replace the template caller function with real data
				string lineToAdd = templatedFunction.Replace("[FUNCTIONNAME]", functionName);
				lineToAdd = lineToAdd.Replace("[NUMPARAMS]", numParameters);
				lineToAdd = lineToAdd.Replace("[PARAMS]", paramsString);
				lineToAdd = lineToAdd.Replace("[REALFUNCTIONNAME]", callerName);
				lineToAdd = lineToAdd.Replace("[RETURNTYPE]", returnType);

				// remove either the void block or the non-void block depending on return type
				if (isVoid)
				{
					int startOfNonVoidBlock = lineToAdd.IndexOf("[STARTNONVOIDBLOCK]");
					int endOfNonVoidBlock = lineToAdd.IndexOf("[ENDNONVOIDBLOCK]") + "[ENDNONVOIDBLOCK]".Length;
					lineToAdd = String.Concat(lineToAdd.Substring(0, startOfNonVoidBlock), lineToAdd.Substring(endOfNonVoidBlock));
				}
				else
				{
					int startOfVoidBlock = lineToAdd.IndexOf("[STARTVOIDBLOCK]");
					int endOfVoidBlock = lineToAdd.IndexOf("[ENDVOIDBLOCK]") + "[ENDVOIDBLOCK]".Length;
					lineToAdd = String.Concat(lineToAdd.Substring(0, startOfVoidBlock), lineToAdd.Substring(endOfVoidBlock));
				}

				// remove the void block tags
				lineToAdd = lineToAdd.Replace("[STARTVOIDBLOCK]", "");
				lineToAdd = lineToAdd.Replace("[ENDVOIDBLOCK]", "");
				lineToAdd = lineToAdd.Replace("[STARTNONVOIDBLOCK]", "");
				lineToAdd = lineToAdd.Replace("[ENDNONVOIDBLOCK]", "");

				System.Console.WriteLine(String.Format("Adding caller function {0}Caller to the CPP file", functionName));
				writer.Write(lineToAdd);
			}
		}

		//*************************************************************************
		// Method:		ReplaceReplacementFunctionsInCppFile
		// Description: replaces replacement functions in the template with the real code
		//
		// Parameters:
		//	replacementDllInt - the id of the replacement dll in the database
		//	reader - the template reader
		//	writer - the cpp output writer
		//
		// Return Value: None
		//*************************************************************************
		protected void ReplaceReplacementFunctionsInCppFile(uint replacementDllInt, StreamReader reader, StreamWriter writer)
		{
			string templatedFunction = "";
			string line = reader.ReadLine();
			while ((line != null) && (line.IndexOf("[ENDREPLACEMENTFUNCTIONS]") == -1))
			{
				templatedFunction = String.Concat(templatedFunction, line, "\r\n");
				line = reader.ReadLine();
			}

			DataSet dataSet = new DataSet();
			OleDbDataAdapter dataAdapter = new OleDbDataAdapter();
			dataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM Functions WHERE InterceptedDll={0} ORDER BY ID", replacementDllInt.ToString()), dbConnection);
			dataAdapter.Fill(dataSet);

			// for all the functions in this dll
			IEnumerator enumerator = dataSet.Tables[0].Rows.GetEnumerator();
			while (enumerator.MoveNext())
			{
				DataRow currentRow = (DataRow)enumerator.Current;
				if (currentRow == null)
					continue;

				// first check for a custom replacement function.  if we find one, output it not the templated one
				DataSet customDataSet = new DataSet();
				OleDbDataAdapter customDataAdapter = new OleDbDataAdapter();
				customDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM CustomReplacementFunctions WHERE FunctionID={0}", currentRow["ID"].ToString()), dbConnection);
				customDataAdapter.Fill(customDataSet);

				if (customDataSet.Tables[0].Rows.Count > 0)
				{
					writer.WriteLine(customDataSet.Tables[0].Rows[0]["ReplacementFunction"].ToString());
					continue;
				}

				string functionName = currentRow["FunctionName"].ToString();
				string replacementFunctionName = currentRow["ReplacementFunctionName"].ToString();

				DataSet tempDataSet = new DataSet();
				OleDbDataAdapter tempDataAdapter = new OleDbDataAdapter();

				// get the return type
				tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ReturnType FROM ReturnTypes WHERE ID={0}", currentRow["ReturnType"].ToString()), dbConnection);
				tempDataAdapter.Fill(tempDataSet);
				string returnType = tempDataSet.Tables[0].Rows[0]["ReturnType"].ToString();

				// get the default return value
				string defaultReturnStatement = "";
				bool isVoid = true;
				if (!returnType.ToLower().Equals("void"))
				{
					defaultReturnStatement = "return 0;";
					isVoid = false;
				}

				// get the category
				tempDataSet.Clear();
				tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT Category FROM Categories WHERE ID={0}", currentRow["Category"].ToString()), dbConnection);
				tempDataAdapter.Fill(tempDataSet);
				string category = tempDataSet.Tables[0].Rows[0]["Category"].ToString();

				// get the calling convention modifier
				tempDataSet.Clear();
				tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CallingConventionModifier FROM CallingConventionModifiers WHERE ID={0}", currentRow["CallingConventionModifier"].ToString()), dbConnection);
				tempDataAdapter.Fill(tempDataSet);
				string callingConvention = tempDataSet.Tables[0].Rows[0]["CallingConventionModifier"].ToString();

				// get the parameters
				DataSet paramTableDataSet = new DataSet();
				OleDbDataAdapter paramTableDataAdapter = new OleDbDataAdapter();
				paramTableDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM ParameterValues WHERE FunctionID={0} ORDER BY Index", currentRow["ID"].ToString()), dbConnection);
				paramTableDataAdapter.Fill(paramTableDataSet);
				string numParams = paramTableDataSet.Tables[0].Rows.Count.ToString();

				string stringArray = "";
				string paramDefs = "";
				string paramRefs = "void *params[numParams] = { ";
				string paramCompareAsTypes = "ParameterType paramTypes[numParams] = { ";
				IEnumerator paramEnumerator = paramTableDataSet.Tables[0].Rows.GetEnumerator();
				int paramIndex = 0;
				int stringArrayIndex = 0;
				while (paramEnumerator.MoveNext())
				{
					DataRow currentParamRow = (DataRow)paramEnumerator.Current;
					if (currentParamRow == null)
						continue;

					if (paramIndex > 0)
					{
						paramDefs = String.Concat(paramDefs,  ", ");
						paramRefs = String.Concat(paramRefs,  ", ");
						paramCompareAsTypes = String.Concat(paramCompareAsTypes,  ", ");
					}

					// get the param type
					tempDataSet.Clear();
					tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT ParameterType FROM ParameterTypes WHERE ID={0}", currentParamRow["Type"].ToString()), dbConnection);
					tempDataAdapter.Fill(tempDataSet);

					string paramName = currentParamRow["Name"].ToString();
					string paramType = tempDataSet.Tables[0].Rows[0]["ParameterType"].ToString();

					// get the param compare as type
					tempDataSet.Clear();
					tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT CompareAsType FROM CompareAsTypes WHERE ID={0}", currentParamRow["CompareAs"].ToString()), dbConnection);
					tempDataAdapter.Fill(tempDataSet);

					string paramCompareAsType = tempDataSet.Tables[0].Rows[0]["CompareAsType"].ToString();

					if (paramType.Equals("PHKEY"))
						paramCompareAsTypes = String.Concat(paramCompareAsTypes, "PointerPointerType");
					else
						paramCompareAsTypes = String.Concat(paramCompareAsTypes, paramCompareAsType);

					paramDefs = String.Concat(paramDefs, paramType, " ", paramName);

					// add to the string array if necessary
					if (paramCompareAsType.Equals("StringType") || paramCompareAsType.Equals("WideStringType"))
					{
						if (stringArrayIndex == 0)
						{
							stringArray = "SiString str[] = { ";
						}
						else if (stringArrayIndex > 0)
						{
							stringArray = String.Concat(stringArray, ", ");
						}

						if (paramCompareAsType.Equals("StringType"))
							stringArray = String.Concat(stringArray, "(char *)");
						else
							stringArray = String.Concat(stringArray, "(wchar_t *)");

						stringArray = String.Concat(stringArray, paramName);

						paramRefs = String.Concat(paramRefs, "&str[", stringArrayIndex.ToString(), "]");

						stringArrayIndex++;
					}
					else
						paramRefs = String.Concat(paramRefs, "&", paramName);

					paramIndex++;
				}

				// if there were no params, change the line
				if (paramIndex == 0)
				{
					paramRefs = "void **params = NULL;";
					paramCompareAsTypes = "ParameterType *paramTypes = NULL;";
				}
				else
				{
					paramRefs = String.Concat(paramRefs, " };");
					paramCompareAsTypes = String.Concat(paramCompareAsTypes, " };");
				}

				// if there were any string params, end the array
				if (stringArrayIndex > 0)
					stringArray = String.Concat(stringArray, " };");

				// get custom code blocks if there are any
				tempDataSet.Clear();
				tempDataAdapter.SelectCommand = new OleDbCommand(String.Format("SELECT * FROM CustomReplacementFunctionCodeBlocks WHERE FunctionID={0}", currentRow["ID"].ToString()), dbConnection);
				tempDataAdapter.Fill(tempDataSet);

				string allocInfoCode = "";
				string successfulRunStandardTestsCode = "";
				string failedRunStandardTestsCode = "";
				string postRunStandardTestsCode = "";
				string allocInfo = "0";
				string positiveAllocInfo = "true";
				if (tempDataSet.Tables[0].Rows.Count > 0)
				{
					allocInfoCode = tempDataSet.Tables[0].Rows[0]["CustomAllocationInfoCode"].ToString();
					successfulRunStandardTestsCode = tempDataSet.Tables[0].Rows[0]["SuccessfulRunStandardTestsCode"].ToString();
					failedRunStandardTestsCode = tempDataSet.Tables[0].Rows[0]["FailedRunStandardTestsCode"].ToString();
					postRunStandardTestsCode = tempDataSet.Tables[0].Rows[0]["PostRunStandardTestsCode"].ToString();
					allocInfo = tempDataSet.Tables[0].Rows[0]["AllocInfoVarName"].ToString();
					positiveAllocInfo = tempDataSet.Tables[0].Rows[0]["PositiveAllocInfoVarName"].ToString();
				}

				// replace the template caller function with real data
				string lineToAdd = templatedFunction.Replace("[FUNCTIONNAME]", functionName);
				lineToAdd = lineToAdd.Replace("[REPLACEMENTFUNCTIONNAME]", replacementFunctionName);
				lineToAdd = lineToAdd.Replace("[RETURNTYPE]", returnType);
				lineToAdd = lineToAdd.Replace("[DEFAULTRETURN]", defaultReturnStatement);
				lineToAdd = lineToAdd.Replace("[CATEGORYNAME]", category);
				lineToAdd = lineToAdd.Replace("[CALLINGCONVENTIONMODIFIER]", callingConvention);
				lineToAdd = lineToAdd.Replace("[NUMPARAMS]", numParams);
				lineToAdd = lineToAdd.Replace("[PARAMDEFS]", paramDefs);
				lineToAdd = lineToAdd.Replace("[STRINGARRAY]", stringArray);
				lineToAdd = lineToAdd.Replace("[PARAMREFERENCES]", paramRefs);
				lineToAdd = lineToAdd.Replace("[PARAMCOMPAREASTYPES]", paramCompareAsTypes);
				lineToAdd = lineToAdd.Replace("[CUSTOMALLOCATIONINFOCODE]", allocInfoCode);
				lineToAdd = lineToAdd.Replace("[ALLOCINFO]", allocInfo);
				lineToAdd = lineToAdd.Replace("[POSITIVEALLOCATIONBOOL]", positiveAllocInfo);
				lineToAdd = lineToAdd.Replace("[SUCCESSFULRUNSTANDARDTESTSCALLCUSTOMCODE]", successfulRunStandardTestsCode);
				lineToAdd = lineToAdd.Replace("[FAILEDRUNSTANDARDTESTSCALLCUSTOMCODE]", failedRunStandardTestsCode);
				lineToAdd = lineToAdd.Replace("[POSTRUNSTANDARDTESTSCUSTOMCODE]", postRunStandardTestsCode);

				// remove either the void block or the non-void block depending on return type
				if (isVoid)
				{
					int startOfNonVoidBlock1 = lineToAdd.IndexOf("[STARTNONVOIDBLOCK1]");
					int endOfNonVoidBlock1 = lineToAdd.IndexOf("[ENDNONVOIDBLOCK1]") + "[ENDNONVOIDBLOCK1]".Length;
					lineToAdd = String.Concat(lineToAdd.Substring(0, startOfNonVoidBlock1), lineToAdd.Substring(endOfNonVoidBlock1));

					int startOfNonVoidBlock2 = lineToAdd.IndexOf("[STARTNONVOIDBLOCK2]");
					int endOfNonVoidBlock2 = lineToAdd.IndexOf("[ENDNONVOIDBLOCK2]") + "[ENDNONVOIDBLOCK2]".Length;
					lineToAdd = String.Concat(lineToAdd.Substring(0, startOfNonVoidBlock2), lineToAdd.Substring(endOfNonVoidBlock2));

					lineToAdd = lineToAdd.Replace("[RETURNVALVARREF]", "&returnValue");
				}
				else
				{
					int startOfVoidBlock1 = lineToAdd.IndexOf("[STARTVOIDBLOCK1]");
					int endOfVoidBlock1 = lineToAdd.IndexOf("[ENDVOIDBLOCK1]") + "[ENDVOIDBLOCK1]".Length;
					lineToAdd = String.Concat(lineToAdd.Substring(0, startOfVoidBlock1), lineToAdd.Substring(endOfVoidBlock1));

					int startOfVoidBlock2 = lineToAdd.IndexOf("[STARTVOIDBLOCK2]");
					int endOfVoidBlock2 = lineToAdd.IndexOf("[ENDVOIDBLOCK2]") + "[ENDVOIDBLOCK2]".Length;
					lineToAdd = String.Concat(lineToAdd.Substring(0, startOfVoidBlock2), lineToAdd.Substring(endOfVoidBlock2));

					lineToAdd = lineToAdd.Replace("[RETURNVALVARREF]", "&tempReturnValue");
				}

				// remove the void block tags
				lineToAdd = lineToAdd.Replace("[STARTVOIDBLOCK1]", "");
				lineToAdd = lineToAdd.Replace("[ENDVOIDBLOCK1]", "");
				lineToAdd = lineToAdd.Replace("[STARTNONVOIDBLOCK1]", "");
				lineToAdd = lineToAdd.Replace("[ENDNONVOIDBLOCK1]", "");
				lineToAdd = lineToAdd.Replace("[STARTVOIDBLOCK2]", "");
				lineToAdd = lineToAdd.Replace("[ENDVOIDBLOCK2]", "");
				lineToAdd = lineToAdd.Replace("[STARTNONVOIDBLOCK2]", "");
				lineToAdd = lineToAdd.Replace("[ENDNONVOIDBLOCK2]", "");

				System.Console.WriteLine(String.Format("Adding replacement function {0} to the CPP file", replacementFunctionName));
				writer.Write(lineToAdd);
			}
		}

		#endregion

		#region Main

		//*************************************************************************
		// Method:		Main
		// Description: runs the generator
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		[STAThread]
		static void Main()
		{
			ReplacementLibraryGenerator generator = new ReplacementLibraryGenerator();

			System.Console.WriteLine(String.Concat("Starting generation of XML file: ", generator.XMLFileName));
			generator.GenerateXML();
			System.Console.WriteLine("Generation of XML complete");

			System.Console.WriteLine("Starting generation of Replacement Libraries");
			generator.GenerateReplacementLibraries();
			System.Console.WriteLine("Generation of Replacement Libraries complete");
		}

		#endregion
	}
}
