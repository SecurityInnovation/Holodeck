using System;
using System.Collections;
using System.Data;
using System.IO;
using System.Windows.Forms;
using System.Xml;

namespace CoreTester
{
	public class TestCaseFile
	{
		private bool holoTrouble;
		private FileStream myFileStream;
		private HolodeckInterface holodeckEE;
		private int enabled, featureNo, testNo, type;
		private MainForm mainForm;
		private Queue holoQ, testCaseQ;
		private StreamWriter myStreamWriter;
		private String fileName, name, outcome, testPurpose;
		private XmlTextReader holoReader, testCaseReader;
	
		public TestCaseFile(MainForm mainFormIn, String fileNameIn)
		{
			mainForm = mainFormIn;
			fileName = fileNameIn;

			//initiliase the program's variables
			initBool();
			initInt();
			initString();
		}

		private bool beginTestCaseFile()
		{	
			try 
			{
				testCaseReader = new XmlTextReader(fileName);
				
				while (testCaseReader.Read())
				{
					switch (testCaseReader.NodeType)
					{
						case XmlNodeType.Element: 
						{
							switch (testCaseReader.Name) 
							{
								case "TestFile": 
								{
									mainForm.thisTime.updateTime();

									//start of the test file
									testPurpose = testCaseReader.GetAttribute("Purpose");
									mainForm.infoTextBox.AppendText(mainForm.thisTime.getTime() + "Purpose of this test file is to test [" + testPurpose + "]\n");
									break;
								}

								case "Recorded_Session": 
								{
									//start of a test case
									testCaseQ = new Queue();
									testNo++;
									break;
								}

								case "Record":
								{
									//start of some recorded feature
									featureNo++;
									break;
								}
								
								case "Log": 
								{
									//information about the feature
									break;
								}

								case "Inject": 
								{
									//information about fault, limit, or test
									type = Convert.ToInt16(testCaseReader.GetAttribute("Type").ToString());
									name = testCaseReader.GetAttribute("Name").ToString();
									enabled = Convert.ToInt16(testCaseReader.GetAttribute("Enabled").ToString());

									//create a new test case
									TestCase newCase = new TestCase(fileName, enabled, name, type, testNo);
									testCaseQ.Enqueue(newCase);
									break;
								}
							}

							break;
						}

						case XmlNodeType.EndElement: 
						{
							switch (testCaseReader.Name) 
							{
								case "TestFile": 
								{
									//we have reached the end of this test case file
									mainForm.thisTime.updateTime();
								
									//now show the results for this test file
									mainForm.infoTextBox.AppendText(mainForm.thisTime.getTime() + "Finished!  Test Cases [" + testNo + "] Features Tested [" + featureNo + "]\n");
									
									//close holodeck
									//holodeckEE.closeHolodeck();

									endTestCaseFile();
									break;
								}

								case "Recorded_Session": 
								{
									//we have a complete recorded session now so now run the test cases from this session
									startTest();
									break;
								}

								case "Record":
								{
									//end the test of a feature
									break;
								}
							}
							break;
						}
					}
				}

				//close the test reader our work here is done
				testCaseReader.Close();
			}

			catch (FileNotFoundException) 
			{
				//could not find the file
				mainForm.thisTime.updateTime();
				mainForm.infoTextBox.AppendText("\n" + mainForm.thisTime.getTime() + "File [" + fileName + "] not Found!! Skipping..\n");
				endTestCaseFile();
			}

			return true;
		}

		private bool endTestCaseFile() 
		{
			//when the test case is over end this test
			if (mainForm.hasArguments) 
			{
				//run some other tests if we have any to run
				if (mainForm.fileNum <= mainForm.files.Length) 
				{
					//if we haven't already run all the tests then run the next test		
					mainForm.runMultipleTests();
				}
			}
			return true;
		}

		private void initBool() 
		{
			//initialise the bool
			holoTrouble = false;
		}

		private void initInt() 
		{
			//initialise the values of the integers
			enabled = 0;
			featureNo = 0;
			testNo= 0;
			type = 0;
		}

		private void initString() 
		{
			//initialise the values of the strings
			name = null;
			outcome = null;
			testPurpose = null;
		}

		private bool runTest() 
		{
			//run the test 
			IEnumerator testCaseQE = testCaseQ.GetEnumerator();
			
			//start recording
			holodeckEE.startRecord();

			while (testCaseQE.MoveNext())
			{
				//get the next testcase
				TestCase thisCase = (TestCase) testCaseQE.Current;	
		
				//open the set fault window
				if (!holodeckEE.isFaultWindowOpen()) 
				{
					holodeckEE.openFaultWindow();
				}

				//move to the correct pane for this specific test case 
				holodeckEE.getFaultType(thisCase.type);

				//now turn the fault on/off as required 
				holodeckEE.getFault(thisCase.type, thisCase.injection, thisCase.enabled);

				String enabledStr = "False";

				if (thisCase.enabled == 2) 
				{
					//this function is enabled 
					enabledStr = "True";
				}

				//show the information about this test case
				string[] testData = { thisCase.file, thisCase.testNo.ToString(), thisCase.injection, enabledStr, "" };
				showResults(testData);
			}

			return true;
		}

		private void showResults(string[] testDataIn) 
		{
			//show the results of each test
			ListViewItem testListing = new ListViewItem(testDataIn);
			mainForm.testListView.Items.Add(testListing);
			mainForm.testListView.Refresh();
		}

		private bool startTest() 
		{
			//starts to run the test

			//start holodeck
			mainForm.infoTextBox.AppendText(mainForm.thisTime.getTime() + "Running Test Case [" + testNo + "] from file [" + fileName + "]\n");									holodeckEE = new HolodeckInterface();
			
			//load the pane settings
			holodeckEE.LoadPaneSettings("paneSettings.txt");			
			
			//start holodeck
			holodeckEE.startHolodeck();
									
			//open the project we have created
			holodeckEE.openProject(mainForm.path);

			//run the test
			runTest();

			//stop the test
			stopTest(); 
			return true;
		}

		private bool stopTest() 
		{
			//stop recording
			holodeckEE.stopRecord();
			holodeckEE.closeHolodeck();
			return true;
		}

		public void InstanceMethod()
		{
			//this method is called when the thread is started
			beginTestCaseFile();
		}
	}
}