//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReplacementLibraryVerify.cs
//
// DESCRIPTION: Verifies a Holodeck replacement library
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 May 2003       R. Wagner   File created
//*************************************************************************
using System;
using System.IO;
using System.Collections;

namespace ReplacementLibraryVerify
{
	class ReplacementLibraryVerify
	{
		[STAThread]
		static void Main(string[] args)
		{
			if (args.Length != 1)
			{
				Console.WriteLine("ReplacementLibraryVerify cppfile");
				Console.WriteLine();
				Console.WriteLine("Verifies a Holodeck replacement library for correctness.");
				Console.WriteLine();
			}

			Hashtable funcParams = new Hashtable();
			StreamReader r = new StreamReader(args[0]);

			while (true)
			{
				string line = r.ReadLine();
				if (line == null)
					break;

				if (line.IndexOf("Caller(") != -1)
				{
					// Caller function

					string funcName = line.Substring(0, line.IndexOf("Caller("));
					funcName = funcName.Substring(funcName.LastIndexOf(' ') + 1);
					string realFuncName = "real" + funcName;
					string netCorruptFuncName = "NetCorrupt_" + funcName;
					bool realCalled = false;
					bool numParamsFound = false;

					while (true)
					{
						line = r.ReadLine();
						if (line == null)
							break;

						line.Trim();
						if (line == "}")
							break; // end of function

						if (line.IndexOf("numParams") != -1)
						{
							// Parameter count check
							string countStr = line.Substring(line.IndexOf("numParams") + "numParams".Length);
							countStr = countStr.Substring(countStr.IndexOf("!=") + "!=".Length);
							countStr.Trim();
							countStr = countStr.Substring(0, countStr.IndexOf(')'));
							countStr.Trim();
							int count = Int32.Parse(countStr);
							// Make sure parameter count matches existing value if there is one, or store
							// parameter count if not
							if (funcParams.ContainsKey(funcName))
							{
								if ((int)funcParams[funcName] != count)
									Console.WriteLine(funcName + "Caller has incorrect numParams");
							}
							else
								funcParams.Add(funcName, count);
							numParamsFound = true;
						}

						if ((line.IndexOf(realFuncName) != -1) || (line.IndexOf(netCorruptFuncName) != -1))
						{
							// Call to real function
							realCalled = true;
							if (line.IndexOf(realFuncName) != -1)
								line = line.Substring(line.IndexOf(realFuncName) + realFuncName.Length);
							else
								line = line.Substring(line.IndexOf(netCorruptFuncName) + netCorruptFuncName.Length);
							int correctIndex = 0;
							// Verify parameters to real function
							while (true)
							{
								if (line.IndexOf("replacementBinary") != -1)
								{
									// "replacementBinary" is used for .NET interception and replaces one of the
									// original parameters.  Skip this one.
									line = line.Substring(line.IndexOf("replacementBinary") + "replacementBinary".Length);
									correctIndex++;
								}
								if (line.IndexOf('[') != -1)
								{
									// Make sure the index is in the correct sequence
									line = line.Substring(line.IndexOf('[') + 1);
									line.Trim();
									string indexStr = line.Substring(0, line.IndexOf(']'));
									indexStr.Trim();
									int index = Int32.Parse(indexStr);
									if (index != correctIndex)
									{
										Console.WriteLine(funcName + "Caller passes incorrect paramaters to real function");
										break;
									}
									correctIndex++;
								}
								else
								{
									// Read the next line if the statement hasn't ended
									if (line.IndexOf(';') != -1)
										break;
									line = r.ReadLine();
									if (line == null)
										break;
								}
							}

							// Check the parameter count
							if (correctIndex != (int)funcParams[funcName])
								Console.WriteLine(funcName + "Caller passes incorrect paramaters to real function");
						}
					}

					if (!realCalled)
						Console.WriteLine(funcName + "Caller does not call " + realFuncName + " function");
					if (!numParamsFound)
						Console.WriteLine(funcName + "Caller does not check numParams");
				}

				if (line.IndexOf("Replacement(") != -1)
				{
					// Replacement function

					string funcName = line.Substring(0, line.IndexOf("Replacement("));
					funcName = funcName.Substring(funcName.LastIndexOf(' ') + 1);
					string realFuncName = "real" + funcName;
					bool realCalled = false;
					bool numParamsFound = false;
					bool paramsArrayCheck = false;
					string nonArrayStrName = "";
					ArrayList strArrayNames = new ArrayList();

					ArrayList paramNames = new ArrayList();

					// Fill in parameter names from the prototype
					while (true)
					{
						if (line.IndexOf(',') != -1)
						{
							string paramName = line.Substring(0, line.IndexOf(','));
							line = line.Substring(line.IndexOf(',') + 1).Trim();
							paramName = paramName.Trim();
							if (paramName.LastIndexOf(' ') != -1)
								paramName = paramName.Substring(paramName.LastIndexOf(' '));
							paramName = paramName.Trim();
							while ((paramName.StartsWith("*")) || (paramName.StartsWith("&")))
								paramName = paramName.Substring(1);
							if (paramName.EndsWith("[]"))
								paramName = paramName.Substring(0, paramName.Length - 2);
							paramNames.Add(paramName);
						}
						else if (line.IndexOf(')') != -1)
						{
							string paramName = line.Substring(0, line.IndexOf(')'));
							paramName = paramName.Trim();
							if (paramName.LastIndexOf(' ') != -1)
								paramName = paramName.Substring(paramName.LastIndexOf(' '));
							paramName = paramName.Trim();
							while ((paramName.StartsWith("*")) || (paramName.StartsWith("&")))
								paramName = paramName.Substring(1);
							if (paramName.EndsWith("[]"))
								paramName = paramName.Substring(0, paramName.Length - 2);
							paramNames.Add(paramName);
							break;
						}
						else
						{
							line = r.ReadLine().Trim();
							if (line == "{")
								break;
						}
					}

					// Read each line of the function
					while (true)
					{
						line = r.ReadLine();
						if (line == null)
							break;

						line.Trim();
						if (line == "}")
							break;

						if ((line.IndexOf("SiString str[]") != -1) || (line.IndexOf("SiString strings[]") != -1))
						{
							// Array of SiStrings to hold string arguments.  Keep the real parameter names around
							// for verification.
							line = line.Substring(line.IndexOf('{'));
							line = line.Trim();
							line = line.Substring(0, line.IndexOf('}'));
							line = line.Trim();
							while (line != "")
							{
								string paramName;
								if (line.IndexOf(',') != -1)
								{
									paramName = line.Substring(0, line.IndexOf(','));
									line = line.Substring(line.IndexOf(',') + 1).Trim();
								}
								else
								{
									paramName = line;
									line = "";
								}
								paramName = paramName.Trim();
								if (paramName.LastIndexOf(' ') != -1)
									paramName = paramName.Substring(paramName.LastIndexOf(' ') + 1);
								if (paramName.LastIndexOf(')') != -1)
									paramName = paramName.Substring(paramName.LastIndexOf(')') + 1);
								paramName = paramName.Trim();
								while ((paramName.StartsWith("*")) || (paramName.StartsWith("&")))
									paramName = paramName.Substring(1);
								strArrayNames.Add(paramName);
							}
						}
						else if (line.IndexOf("SiString str") != -1)
						{
							// Single SiString holding a string argument.  Keep the original parameter name
							// around for verification.
							nonArrayStrName = line.Substring(0, line.IndexOf(';')).Trim();
							if (nonArrayStrName.LastIndexOf('=') != -1)
								nonArrayStrName = nonArrayStrName.Substring(nonArrayStrName.LastIndexOf('=') + 1);
							if (nonArrayStrName.LastIndexOf(' ') != -1)
								nonArrayStrName = nonArrayStrName.Substring(nonArrayStrName.LastIndexOf(' ') + 1);
							if (nonArrayStrName.LastIndexOf(')') != -1)
								nonArrayStrName = nonArrayStrName.Substring(nonArrayStrName.LastIndexOf(')') + 1);
							nonArrayStrName = nonArrayStrName.Trim();
							while ((nonArrayStrName.StartsWith("*")) || (nonArrayStrName.StartsWith("&")))
								nonArrayStrName = nonArrayStrName.Substring(1);
						}

						if ((line.IndexOf("numParams") != -1) && (line.IndexOf("const int") != -1))
						{
							// Parameter count
							string countStr = line.Substring(line.IndexOf("numParams") + "numParams".Length);
							countStr = countStr.Substring(countStr.IndexOf("=") + "=".Length);
							countStr = countStr.Trim();
							countStr = countStr.Substring(0, countStr.IndexOf(';'));
							countStr = countStr.Trim();
							int count = Int32.Parse(countStr);
							// Make sure parameter count matches existing value if there is one, or store
							// parameter count if not
							if (funcParams.ContainsKey(funcName))
							{
								if ((int)funcParams[funcName] != count)
									Console.WriteLine(funcName + "Replacement has incorrect numParams");
							}
							else
								funcParams.Add(funcName, count);
							numParamsFound = true;
						}

						if (line.IndexOf("params[numParams]") != -1)
						{
							// Parameter array.  Read the entire array into a buffer.
							line = line.Substring(line.IndexOf('{'));
							line = line.Trim();
							while (line.IndexOf('}') == -1)
								line += r.ReadLine();
							line = line.Substring(0, line.IndexOf('}'));
							line = line.Trim();
							int paramIndex = 0;
							// Verify the parameters are put into the array in the correct order
							while (line != "")
							{
								string paramName;
								if (line.IndexOf(',') != -1)
								{
									paramName = line.Substring(0, line.IndexOf(','));
									line = line.Substring(line.IndexOf(',') + 1).Trim();
								}
								else
								{
									paramName = line;
									line = "";
								}
								paramName = paramName.Trim();
								if (paramName.LastIndexOf(' ') != -1)
									paramName = paramName.Substring(paramName.LastIndexOf(' '));
								paramName = paramName.Trim();
								while ((paramName.StartsWith("*")) || (paramName.StartsWith("&")))
									paramName = paramName.Substring(1);
								if (paramName == "str") // "str" is an SiString placeholder
									paramName = nonArrayStrName;
								if ((paramName.StartsWith("str") || paramName.StartsWith("strings")) &&
									(paramName.IndexOf('[') != -1))
								{
									// SiString array reference.  Look up the real parameter name for verification.
									string strIndexStr = paramName.Substring(paramName.IndexOf('[') + 1);
									strIndexStr = strIndexStr.Substring(0, strIndexStr.IndexOf(']'));
									strIndexStr = strIndexStr.Trim();
									int strIndex = Int32.Parse(strIndexStr);
									if (strIndex >= strArrayNames.Count)
										Console.WriteLine(funcName + "Replacement accesses outside bounds of str array");
									else
										paramName = (string)strArrayNames[strIndex];
								}
								// Verify the parameter names
								if (paramName != (string)paramNames[paramIndex])
									Console.WriteLine(funcName + "Replacement has incorrect params array");
								paramIndex++;
							}

							if (paramIndex != (int)funcParams[funcName])
								Console.WriteLine(funcName + "Replacement has incorrect params array");

							paramsArrayCheck = true;
						}

						if ((line.IndexOf("functionName") != -1) && (line.IndexOf("char") != -1))
						{
							// Setting function name, verify that the name string is correct
							string nameStr = line.Substring(line.IndexOf("functionName") + "functionName".Length);
							nameStr = nameStr.Substring(nameStr.IndexOf("=") + "=".Length);
							nameStr = nameStr.Trim();
							nameStr = nameStr.Substring(0, nameStr.IndexOf(';'));
							nameStr = nameStr.Trim();
							nameStr = nameStr.Substring(1, nameStr.Length - 2);

							if (nameStr != funcName)
							{
								Console.WriteLine(funcName + "Replacement sets functionName to wrong value");
							}
						}

						if (line.IndexOf(realFuncName) != -1)
							realCalled = true;
						else if (line.IndexOf("real") != -1)
							Console.WriteLine(funcName + "Replacement sets wrong realxxx function");
					}

					if (!realCalled)
						Console.WriteLine(funcName + "Replacement does not set " + realFuncName);
					if (!numParamsFound)
						Console.WriteLine(funcName + "Replacement does not set numParams");
					if ((!paramsArrayCheck) && ((int)funcParams[funcName] > 0))
						Console.WriteLine(funcName + "Replacement does not set params array");
				}
			}
		}
	}
}
