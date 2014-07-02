using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Threading;
using System.Windows.Forms;
using System.Data;

namespace CoreTester
{
	
	public class MainForm : System.Windows.Forms.Form
	{
		public System.Windows.Forms.ListView testListView;
		private System.Windows.Forms.Button stopButton;
		private System.ComponentModel.Container components = null;
        		
		private System.Windows.Forms.Button runButton;
		public System.Windows.Forms.RichTextBox infoTextBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;

		public bool hasArguments;
		public int fileNum;
		public LTime thisTime = new LTime();
        public String[] files;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		
		private TestCaseFile thisTestFile = null;
		private System.Windows.Forms.Button createButton;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		public String path;
		public Thread testThread = null;

		public MainForm(String pathIn, bool hasArgumentsIn, String[] filesIn)
		{
		    path = pathIn;
			hasArguments = hasArgumentsIn;

			if (hasArguments) 
			{
				//have tests coming in
				files = filesIn;
			}

			InitializeComponent();
            InitButtons();
			setupInt();
			showStartupInfo();

			if (hasArguments) 
			{
			   //we have files as arguments so lets run multiple tests
               runMultipleTests();
			}
		}

		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.testListView = new System.Windows.Forms.ListView();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.stopButton = new System.Windows.Forms.Button();
			this.runButton = new System.Windows.Forms.Button();
			this.infoTextBox = new System.Windows.Forms.RichTextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.createButton = new System.Windows.Forms.Button();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// testListView
			// 
			this.testListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						   this.columnHeader3,
																						   this.columnHeader1,
																						   this.columnHeader4,
																						   this.columnHeader5,
																						   this.columnHeader2});
			this.testListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.testListView.FullRowSelect = true;
			this.testListView.Location = new System.Drawing.Point(16, 160);
			this.testListView.MultiSelect = false;
			this.testListView.Name = "testListView";
			this.testListView.Size = new System.Drawing.Size(488, 160);
			this.testListView.TabIndex = 0;
			this.testListView.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "File";
			this.columnHeader3.Width = 100;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Test #";
			this.columnHeader1.Width = 50;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Function";
			this.columnHeader4.Width = 180;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Outcome";
			this.columnHeader2.Width = 70;
			// 
			// stopButton
			// 
			this.stopButton.BackColor = System.Drawing.Color.Red;
			this.stopButton.Enabled = false;
			this.stopButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.stopButton.Location = new System.Drawing.Point(520, 64);
			this.stopButton.Name = "stopButton";
			this.stopButton.Size = new System.Drawing.Size(104, 23);
			this.stopButton.TabIndex = 1;
			this.stopButton.Text = "&Stop";
			// 
			// runButton
			// 
			this.runButton.BackColor = System.Drawing.Color.Lime;
			this.runButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.runButton.Location = new System.Drawing.Point(520, 40);
			this.runButton.Name = "runButton";
			this.runButton.Size = new System.Drawing.Size(104, 24);
			this.runButton.TabIndex = 2;
			this.runButton.Text = "&Run";
			// 
			// infoTextBox
			// 
			this.infoTextBox.Cursor = System.Windows.Forms.Cursors.Default;
			this.infoTextBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.infoTextBox.Location = new System.Drawing.Point(16, 16);
			this.infoTextBox.Name = "infoTextBox";
			this.infoTextBox.ReadOnly = true;
			this.infoTextBox.Size = new System.Drawing.Size(488, 120);
			this.infoTextBox.TabIndex = 3;
			this.infoTextBox.Text = "";
			this.infoTextBox.WordWrap = false;
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label1.Location = new System.Drawing.Point(16, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(136, 16);
			this.label1.TabIndex = 4;
			this.label1.Text = "CoreTester Information:";
			// 
			// label2
			// 
			this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label2.Location = new System.Drawing.Point(16, 144);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(100, 16);
			this.label2.TabIndex = 5;
			this.label2.Text = "Test Information:";
			// 
			// createButton
			// 
			this.createButton.BackColor = System.Drawing.Color.Yellow;
			this.createButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.createButton.Location = new System.Drawing.Point(520, 16);
			this.createButton.Name = "createButton";
			this.createButton.Size = new System.Drawing.Size(104, 23);
			this.createButton.TabIndex = 6;
			this.createButton.Text = "&Create Test File";
			this.createButton.Click += new System.EventHandler(this.createButton_Click);
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Enabled";
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(634, 328);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.createButton,
																		  this.label2,
																		  this.label1,
																		  this.infoTextBox,
																		  this.runButton,
																		  this.stopButton,
																		  this.testListView});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.Name = "MainForm";
			this.Text = "CoreTester Holodeck EE";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.MainForm_Closing);
			this.ResumeLayout(false);

		}
		#endregion

		private void InitButtons() 
		{
			//initialise the state of the buttons on start up
			setupButtons(!hasArguments);
			runButtons(hasArguments);
		}

		private void runButtons(bool toggle) 
		{
			//the state of buttons to do with running a test
			stopButton.Enabled = toggle;
		}

		public void runMultipleTests() 
		{
			if (fileNum <= files.Length) 
			{
				thisTime.updateTime();
				infoTextBox.AppendText(thisTime.getTime() + "Parsing Test Cases from file [" + files[fileNum] + "]\n");
				
				if (thisTestFile != null) 
				{
					//remove the last test case file from memory
					thisTestFile = null;
					GC.Collect();
				}

				//create a new test case file
				thisTestFile = new TestCaseFile(this, files[fileNum]);

				//create and start this new test
				testThread = new Thread(new ThreadStart(thisTestFile.InstanceMethod));
				testThread.Start();
			  
				//move onto the next test case file if we have one
				fileNum++;
			}
		}

		private void setupButtons(bool toggle) 
		{
			//the state of buttons to do with setting up a test
			runButton.Enabled = toggle;
		}

		private void setupInt() 
		{
			//setup the integer values for this program
			fileNum = 0;
		}

		private void showStartupInfo() 
		{
			//show the startup information for this program
			thisTime.updateTime();
			infoTextBox.AppendText(thisTime.getDate() + "\n");
		}

		[STAThread]
		static void Main(string[] cmdArgs) 
		{
			if (cmdArgs.Length == 0) 
			{
				//if there are no arguments then we don't have any files to test straight away
				Application.Run(new MainForm(Application.StartupPath, false, null));
			}

			else 
			{
				//there are arguments so we have files to test straight away
				Application.Run(new MainForm(Application.StartupPath, true, cmdArgs));
			}
		}

		private void MainForm_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			if (testThread != null)
			{
				if (testThread.IsAlive)
				{
					//if the thread exists and is running then turn it off when we are closing
					testThread.Abort();
				}
			}
		}

		private void createButton_Click(object sender, System.EventArgs e)
		{
			//show the create test form
			TestForm thisTestForm = new TestForm(this);
			thisTestForm.ShowDialog(this);
		}
	}
}
