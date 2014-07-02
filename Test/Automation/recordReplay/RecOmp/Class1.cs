using System;
using System.Collections;
using System.IO;
using System.Xml;

using CoreTester;

namespace RecOmp
{
	class XMLFileComp 
	{
		public bool hasFailed = false;
		private int enabled, testFailed, testNo, testPassed, type;
		private Queue genFileQ, genContentsQ, origFileQ, origContentsQ;
		private string genFileName, name, origFileName;
		private XmlTextReader xmlFileRdr;

		public XMLFileComp(string origXmlDir, string genXmlDir, bool isDir) 
		{
			if (isDir) 
			{
				//get the two sets of file from both directories
				genFileQ = getFileList(genXmlDir);
				origFileQ = getFileList(origXmlDir);
			
				//initialise the integers used
				initInt();			

				//show the file counts in either dir
				Console.Out.WriteLine("Generated Directory [" + genXmlDir + "] has in [" + genFileQ.Count + "] files.");
				Console.Out.WriteLine("Original Directory [" + origXmlDir + "] has in [" + origFileQ.Count + "] files.");

				if (genFileQ.Count != origFileQ.Count) 
				{
					//if the two queues are not the same length then quit
					Console.Out.WriteLine("File count is not the same!! Exiting.");
				}

				//begin the file comparision
				Console.Out.WriteLine("Beginning file comparision");
				Console.Out.WriteLine("--------------------------");
				beginFileCompare();
			}

			else 
			{
				//just two files to compare
				origFileName = origXmlDir;
				genFileName = genXmlDir;

				//compare the two files
				getContents(origFileName, genFileName);
			}
		}

		private void beginFileCompare() 
		{
			//starts to compare the files
			while (origFileQ.Count != 0) 
			{
				//get the names of the two file names
				origFileName = (string) origFileQ.Dequeue();
				genFileName = (string) genFileQ.Dequeue();
				
				//compare the two files
				getContents(origFileName, genFileName);

				//make sure count is accurate for the next iteration
				origFileQ.TrimToSize();
			}
		}

		private void compare() 
		{
			//shorten the file names
			String[] splitOrg = origFileName.Split('\\');
			String shortOrigName = splitOrg[(splitOrg.Length - 1)]; 
				
			String[] splitGen = genFileName.Split('\\');
			String shortGenName = splitGen[(splitGen.Length - 1)];

			Console.Out.WriteLine("Comparing Xml files Original File [" + shortOrigName  + "] and Generated File [" + shortGenName + "].");
			Console.Out.WriteLine("-----------------------------------------------------------------------------------------------------");

			//compare the contents of the two files
			Console.Out.WriteLine("Tests in Original File: " + origContentsQ.Count);
			Console.Out.WriteLine("Tests in Result File: " + genContentsQ.Count);

			if (origContentsQ.Count != genContentsQ.Count) 
			{
				//files don't contain the same amount of files
				Console.Out.WriteLine("Files do not contain the same amount of tests");
				Console.Out.WriteLine("---------------------------------------------");
			}

			Console.Out.WriteLine();

			//start to enumerator through the two queues
			IEnumerator origFileEnum = origContentsQ.GetEnumerator();
			IEnumerator genFileEnum = genContentsQ.GetEnumerator();

			while (origFileEnum.MoveNext()) 
			{
				//for each test
				//increase the number of tests
				TestCase origTest = (TestCase) origFileEnum.Current;
				testNo++;

				try 
				{
					//get the next test case from the generated file
					genFileEnum.MoveNext();
					TestCase genTest = (TestCase) genFileEnum.Current;

					Console.Out.Write("[" + testNo + "] test has ");

					if ((origTest.injection == genTest.injection) && (origTest.enabled == genTest.enabled)) 
					{
						//this test has passed since they have the same information 
						Console.Out.WriteLine("passed.  Injection [" + origTest.injection + "] Enabled [" + convertToBoolString(genTest.enabled) + "].");
						testPassed++;
					}

					else 
					{
						//something is wrong with this test and it has failed
						hasFailed = true;

						if (origTest.injection != genTest.injection) 
						{
							//its the injection that is the problem
							Console.Out.Write("  Orignal Injection [" + origTest.injection + "] does not match with Generated Injection [" + genTest.injection + "].");
						}

						if (origTest.enabled != genTest.enabled) 
						{
							//its the enabled thats the problem
							Console.Out.Write("  Original Enabled [" + convertToBoolString(origTest.enabled) + "] does not match with Generated Enabled [" + convertToBoolString(genTest.enabled) + "].");
						}

						Console.WriteLine();
						testFailed++;
					}
				}

				catch (Exception e) 
				{
				}
			}

			//print out information about this file
			int totalTests = testFailed + testPassed;
			Console.Out.WriteLine();
			Console.Out.WriteLine("----------------------------------------------------------------------------------------------------");
			Console.Out.WriteLine("Finished comparing [" + origFileName + "] and [" + genFileName + "]");
			Console.Out.WriteLine("Tests Failed [" + testFailed + "] Tests Passed [" + testPassed + "] Total Tests [" + totalTests + "]");
            Console.Out.WriteLine("----------------------------------------------------------------------------------------------------");
			Console.Out.WriteLine();

			//set the integer values back to to their original values
			initInt();
		}

		private string convertToBoolString(int boolInt) 
		{
			//change the ints used in the files to string representation of their boolean values
			if (boolInt == 0) 
			{
				return "False";
			}
			else 
			{
				return "True";
			}
		}

		private void getContents(string origFileName, string genFileName) 
		{
			//compare the two files
			xmlFileRdr = new XmlTextReader(origFileName);
			origContentsQ = new Queue();

			int testNo = 0;

			//get the contents of the orignal file
			while (xmlFileRdr.Read())
			{
				switch (xmlFileRdr.NodeType)
				{
					case XmlNodeType.Element: 
					{
						switch (xmlFileRdr.Name) 
						{
							case "Inject": 
							{
								testNo++;
								//information about fault, limit, or test
								type = Convert.ToInt16(xmlFileRdr.GetAttribute("Type").ToString());
								name = xmlFileRdr.GetAttribute("Name").ToString();
								enabled = Convert.ToInt16(xmlFileRdr.GetAttribute("Enabled").ToString());

								//create a new test case
								TestCase newCase = new TestCase("", enabled, name, type, testNo);
								origContentsQ.Enqueue(newCase);
								break;
							}

							case "TestFile": 
							{
								//start of the test file
								Console.Out.WriteLine();
								Console.Out.WriteLine("Purpose of this test is to [" + xmlFileRdr.GetAttribute("Purpose") + "]");
								Console.Out.WriteLine("-----------------------------------------------------------------------");
								Console.Out.WriteLine();
								break;
							}

						}
						break;
					}
				}
			}

			xmlFileRdr.Close();
			origContentsQ.TrimToSize();

			//get the contents of the generated file
			xmlFileRdr = new XmlTextReader(genFileName);
			genContentsQ = new Queue();

			while (xmlFileRdr.Read())
			{
				switch (xmlFileRdr.NodeType)
				{
					case XmlNodeType.Element: 
					{
						switch (xmlFileRdr.Name) 
						{
							case "Inject": 
							{
								testNo++;
								//information about fault, limit, or test
								type = Convert.ToInt16(xmlFileRdr.GetAttribute("Type").ToString());
								name = xmlFileRdr.GetAttribute("Name").ToString();
								enabled = Convert.ToInt16(xmlFileRdr.GetAttribute("Enabled").ToString());

								//create a new test case
								TestCase newCase = new TestCase("", enabled, name, type, testNo);
								genContentsQ.Enqueue(newCase);
								break;
							}
						}
						break;
					}
				}
			}

			//close the file and make sure the queue is as small as it can be
			xmlFileRdr.Close();
			genContentsQ.TrimToSize();

			//compare the two contents of the file
			compare();
		}

		private Queue getFileList(string dirIn) 
		{
			//return the list of files in this folder
			Queue fileQ = new Queue();

			try 
			{
				string[] files = Directory.GetFiles(Directory.GetCurrentDirectory() + "\\" + dirIn);
                
				foreach (string aFile in files) 
				{
					//add the file name to this queue
					fileQ.Enqueue(aFile);
				}
			}

			catch(DirectoryNotFoundException DNFE) 
			{
				//we can't find this particular directory
				Console.Out.WriteLine("Directory [" + dirIn + "] not found.");
			}

			return fileQ;
		}
	
		private void initInt() 
		{
			//initialise the integers
			testFailed = 0;
			testNo = 0;
			testPassed = 0;
		}
	}

	class Class1
	{
		static int showUsage() 
		{
			//shows the correct way to use the program
			Console.Out.WriteLine("Usage RecOmp -f <original xml file> <generated file>");
			Console.Out.WriteLine("Usage RecOmp -d <directory of original xml files> <directory of generated xml files>");
			return 1;
		}

		[STAThread]
		static int Main(string[] cmdArgs)
		{
			if (cmdArgs.Length != 3) 
			{
				//show correct usage
				return showUsage();
			}
			
			else
			{
				//we have at least two command line arguments so continue
				if ((cmdArgs[0] == "-f") || (cmdArgs[0] == "-d")) 
				{
					//one of the recognized parameters

					XMLFileComp fileComp;

					if (cmdArgs[0] == "-d") 
					{
						//directory comparision
						fileComp = new XMLFileComp(cmdArgs[1], cmdArgs[2], true);
					}

					else 
					{
						//file comparision
						fileComp = new XMLFileComp(cmdArgs[1], cmdArgs[2], false);
					}

					if (fileComp.hasFailed) 
					{
						//test failed
						return 1;
					}
					else 
					{
						//all tests passed
						return 0;
					}
				}

				else 
				{
					//show correct usage
					return showUsage();
				}
			}
		}
	}
}
