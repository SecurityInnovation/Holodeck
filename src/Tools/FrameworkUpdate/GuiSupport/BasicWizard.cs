using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for BasicWizard.
	/// </summary>
	public class BasicWizard : System.Windows.Forms.Form, AbstractLaunchWizard
	{
		private const int StepPanelLocationX = 134;
		private const int StepPanelLocationY = 94;
		private const int StepPanelWidth = 370;
		private const int StepPanelHeight = 252;
		private const int NumberOfSteps = 3;

		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.Panel buttonPanel;
		private System.Windows.Forms.Button prevButton;
		private System.Windows.Forms.Button nextButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Panel stepOnePanel;
		private System.Windows.Forms.Panel stepsPanel;
		private System.Windows.Forms.Label stepOneTextLabel;
		private System.Windows.Forms.Label stepOneLabel;
		private System.Windows.Forms.Label stepTwoLabel;
		private System.Windows.Forms.Label stepTwoTextLabel;
		private System.Windows.Forms.Label stepThreeLabel;
		private System.Windows.Forms.GroupBox stepOneGroupBox;
		private System.Windows.Forms.RadioButton loadFromFileRadioButton;
		private System.Windows.Forms.RadioButton runningProcessRadioButton;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Panel stepTwoFromFilePanel;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button browseButton;
		private System.Windows.Forms.LinkLabel refreshLink;
		private System.Windows.Forms.ListView lv_ProcessListBox;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Panel stepTwoRunningProcessPanel;
		private System.Windows.Forms.Panel stepThreePanel;
		private System.Windows.Forms.Label stepThreeTextLabel;
		private System.Windows.Forms.OpenFileDialog appFileBrowseDialog;
		private System.Windows.Forms.LinkLabel switchToAdvancedModeLinkLabel;
		private int currentStepNumber;
		private System.Windows.Forms.ComboBox applicationPath;
		private bool switchModes;
		private ProcessListItemSorter pidSorter;
		private System.Windows.Forms.Label label1;
		private ProcessListItemSorter processSorter;
		private System.Windows.Forms.CheckBox chk_AppPaused;
		private LogFilterTreeView logFilterTreeView;

		#region properties code

		public bool SwitchModes
		{
			get { return this.switchModes; }
		}

		public bool PredefineTests
		{
			get { return false; }
		}

		public bool ProcessChain
		{
			get { return false; }
		}

		public bool AppPaused
		{
			get { return this.chk_AppPaused.Checked; }
			set { this.chk_AppPaused.Checked = value; }
		}

		public bool UseProcessFile
		{
			get { return this.loadFromFileRadioButton.Checked; }
			set { this.loadFromFileRadioButton.Checked = value; }
		}

		public bool UseProcessID
		{
			get { return this.runningProcessRadioButton.Checked; }
			set { this.runningProcessRadioButton.Checked = value; }
		}

		public string ProcessPath
		{
			get { return this.applicationPath.Text; }
			set { this.applicationPath.Text = value; }
		}

		public ArrayList FilteredOutFunctions
		{
			get { return this.logFilterTreeView.FilteredOutFunctionNames; }
		}

		public ListView ProcessList
		{
			get { return this.lv_ProcessListBox; }
		}

		#endregion

		public BasicWizard()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.SuspendLayout();
			this.logFilterTreeView = new LogFilterTreeView();
			this.logFilterTreeView.Location = new System.Drawing.Point(8, 26);
			this.logFilterTreeView.Size = new System.Drawing.Size(332, 218);
			this.stepThreePanel.Controls.AddRange(new System.Windows.Forms.Control[] { this.logFilterTreeView });
			this.ResumeLayout(false);

			currentStepNumber = 1;
			stepOnePanel.Left = StepPanelLocationX;
			stepOnePanel.Top = StepPanelLocationY;
			stepOnePanel.Width = StepPanelWidth;
			stepOnePanel.Height = StepPanelHeight;
			stepOnePanel.Visible = true;

			pidSorter = new ProcessListItemSorter(0);
			processSorter = new ProcessListItemSorter(1);

			this.lv_ProcessListBox.ListViewItemSorter = pidSorter;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(BasicWizard));
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.buttonPanel = new System.Windows.Forms.Panel();
			this.switchToAdvancedModeLinkLabel = new System.Windows.Forms.LinkLabel();
			this.cancelButton = new System.Windows.Forms.Button();
			this.nextButton = new System.Windows.Forms.Button();
			this.prevButton = new System.Windows.Forms.Button();
			this.stepOnePanel = new System.Windows.Forms.Panel();
			this.stepOneGroupBox = new System.Windows.Forms.GroupBox();
			this.runningProcessRadioButton = new System.Windows.Forms.RadioButton();
			this.loadFromFileRadioButton = new System.Windows.Forms.RadioButton();
			this.stepsPanel = new System.Windows.Forms.Panel();
			this.stepThreeTextLabel = new System.Windows.Forms.Label();
			this.stepThreeLabel = new System.Windows.Forms.Label();
			this.stepTwoTextLabel = new System.Windows.Forms.Label();
			this.stepTwoLabel = new System.Windows.Forms.Label();
			this.stepOneTextLabel = new System.Windows.Forms.Label();
			this.stepOneLabel = new System.Windows.Forms.Label();
			this.stepTwoFromFilePanel = new System.Windows.Forms.Panel();
			this.applicationPath = new System.Windows.Forms.ComboBox();
			this.browseButton = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.stepTwoRunningProcessPanel = new System.Windows.Forms.Panel();
			this.label3 = new System.Windows.Forms.Label();
			this.refreshLink = new System.Windows.Forms.LinkLabel();
			this.lv_ProcessListBox = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.stepThreePanel = new System.Windows.Forms.Panel();
			this.label1 = new System.Windows.Forms.Label();
			this.appFileBrowseDialog = new System.Windows.Forms.OpenFileDialog();
			this.chk_AppPaused = new System.Windows.Forms.CheckBox();
			this.buttonPanel.SuspendLayout();
			this.stepOnePanel.SuspendLayout();
			this.stepOneGroupBox.SuspendLayout();
			this.stepsPanel.SuspendLayout();
			this.stepTwoFromFilePanel.SuspendLayout();
			this.stepTwoRunningProcessPanel.SuspendLayout();
			this.stepThreePanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// pictureBox1
			// 
			this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.pictureBox1.Image = ((System.Drawing.Bitmap)(resources.GetObject("pictureBox1.Image")));
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(490, 93);
			this.pictureBox1.TabIndex = 0;
			this.pictureBox1.TabStop = false;
			// 
			// buttonPanel
			// 
			this.buttonPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.buttonPanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																					  this.switchToAdvancedModeLinkLabel,
																					  this.cancelButton,
																					  this.nextButton,
																					  this.prevButton});
			this.buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.buttonPanel.Location = new System.Drawing.Point(0, 346);
			this.buttonPanel.Name = "buttonPanel";
			this.buttonPanel.Size = new System.Drawing.Size(482, 60);
			this.buttonPanel.TabIndex = 1;
			// 
			// switchToAdvancedModeLinkLabel
			// 
			this.switchToAdvancedModeLinkLabel.Location = new System.Drawing.Point(8, 24);
			this.switchToAdvancedModeLinkLabel.Name = "switchToAdvancedModeLinkLabel";
			this.switchToAdvancedModeLinkLabel.Size = new System.Drawing.Size(144, 23);
			this.switchToAdvancedModeLinkLabel.TabIndex = 3;
			this.switchToAdvancedModeLinkLabel.TabStop = true;
			this.switchToAdvancedModeLinkLabel.Text = "Switch to LaunchPad Mode";
			this.switchToAdvancedModeLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.switchToAdvancedModeLinkLabel_LinkClicked);
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(392, 16);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Cancel";
			this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
			// 
			// nextButton
			// 
			this.nextButton.Location = new System.Drawing.Point(280, 16);
			this.nextButton.Name = "nextButton";
			this.nextButton.TabIndex = 1;
			this.nextButton.Text = "Next >>";
			this.nextButton.Click += new System.EventHandler(this.nextButton_Click);
			// 
			// prevButton
			// 
			this.prevButton.Enabled = false;
			this.prevButton.Location = new System.Drawing.Point(192, 16);
			this.prevButton.Name = "prevButton";
			this.prevButton.TabIndex = 0;
			this.prevButton.Text = "<< Previous";
			this.prevButton.Click += new System.EventHandler(this.prevButton_Click);
			// 
			// stepOnePanel
			// 
			this.stepOnePanel.Anchor = ((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.stepOnePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.stepOnePanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																					   this.stepOneGroupBox});
			this.stepOnePanel.Location = new System.Drawing.Point(296, 104);
			this.stepOnePanel.Name = "stepOnePanel";
			this.stepOnePanel.Size = new System.Drawing.Size(10, 10);
			this.stepOnePanel.TabIndex = 2;
			// 
			// stepOneGroupBox
			// 
			this.stepOneGroupBox.Controls.AddRange(new System.Windows.Forms.Control[] {
																						  this.chk_AppPaused,
																						  this.runningProcessRadioButton,
																						  this.loadFromFileRadioButton});
			this.stepOneGroupBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.stepOneGroupBox.Location = new System.Drawing.Point(16, 16);
			this.stepOneGroupBox.Name = "stepOneGroupBox";
			this.stepOneGroupBox.Size = new System.Drawing.Size(320, 216);
			this.stepOneGroupBox.TabIndex = 0;
			this.stepOneGroupBox.TabStop = false;
			this.stepOneGroupBox.Text = "Select An Application Type To Test:";
			// 
			// runningProcessRadioButton
			// 
			this.runningProcessRadioButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.runningProcessRadioButton.Location = new System.Drawing.Point(24, 72);
			this.runningProcessRadioButton.Name = "runningProcessRadioButton";
			this.runningProcessRadioButton.Size = new System.Drawing.Size(232, 24);
			this.runningProcessRadioButton.TabIndex = 1;
			this.runningProcessRadioButton.Text = "Select A Currently Running Process";
			// 
			// loadFromFileRadioButton
			// 
			this.loadFromFileRadioButton.Checked = true;
			this.loadFromFileRadioButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.loadFromFileRadioButton.Location = new System.Drawing.Point(24, 32);
			this.loadFromFileRadioButton.Name = "loadFromFileRadioButton";
			this.loadFromFileRadioButton.Size = new System.Drawing.Size(264, 24);
			this.loadFromFileRadioButton.TabIndex = 0;
			this.loadFromFileRadioButton.TabStop = true;
			this.loadFromFileRadioButton.Text = "Load An Application From An Executable File";
			// 
			// stepsPanel
			// 
			this.stepsPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.stepsPanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																					 this.stepThreeTextLabel,
																					 this.stepThreeLabel,
																					 this.stepTwoTextLabel,
																					 this.stepTwoLabel,
																					 this.stepOneTextLabel,
																					 this.stepOneLabel});
			this.stepsPanel.Location = new System.Drawing.Point(0, 94);
			this.stepsPanel.Name = "stepsPanel";
			this.stepsPanel.Size = new System.Drawing.Size(137, 252);
			this.stepsPanel.TabIndex = 3;
			// 
			// stepThreeTextLabel
			// 
			this.stepThreeTextLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
			this.stepThreeTextLabel.Location = new System.Drawing.Point(16, 184);
			this.stepThreeTextLabel.Name = "stepThreeTextLabel";
			this.stepThreeTextLabel.Size = new System.Drawing.Size(112, 32);
			this.stepThreeTextLabel.TabIndex = 10;
			this.stepThreeTextLabel.Text = "Set Various Testing Options";
			// 
			// stepThreeLabel
			// 
			this.stepThreeLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
			this.stepThreeLabel.Location = new System.Drawing.Point(8, 168);
			this.stepThreeLabel.Name = "stepThreeLabel";
			this.stepThreeLabel.Size = new System.Drawing.Size(100, 16);
			this.stepThreeLabel.TabIndex = 9;
			this.stepThreeLabel.Text = "Step 3:";
			// 
			// stepTwoTextLabel
			// 
			this.stepTwoTextLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
			this.stepTwoTextLabel.Location = new System.Drawing.Point(16, 112);
			this.stepTwoTextLabel.Name = "stepTwoTextLabel";
			this.stepTwoTextLabel.Size = new System.Drawing.Size(112, 32);
			this.stepTwoTextLabel.TabIndex = 8;
			this.stepTwoTextLabel.Text = "Select the Application To Test";
			// 
			// stepTwoLabel
			// 
			this.stepTwoLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
			this.stepTwoLabel.Location = new System.Drawing.Point(8, 96);
			this.stepTwoLabel.Name = "stepTwoLabel";
			this.stepTwoLabel.Size = new System.Drawing.Size(100, 16);
			this.stepTwoLabel.TabIndex = 7;
			this.stepTwoLabel.Text = "Step 2:";
			// 
			// stepOneTextLabel
			// 
			this.stepOneTextLabel.Location = new System.Drawing.Point(16, 40);
			this.stepOneTextLabel.Name = "stepOneTextLabel";
			this.stepOneTextLabel.Size = new System.Drawing.Size(112, 32);
			this.stepOneTextLabel.TabIndex = 6;
			this.stepOneTextLabel.Text = "Select an Application Type to Test";
			// 
			// stepOneLabel
			// 
			this.stepOneLabel.Location = new System.Drawing.Point(8, 24);
			this.stepOneLabel.Name = "stepOneLabel";
			this.stepOneLabel.Size = new System.Drawing.Size(120, 16);
			this.stepOneLabel.TabIndex = 5;
			this.stepOneLabel.Text = "Step 1:";
			// 
			// stepTwoFromFilePanel
			// 
			this.stepTwoFromFilePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.stepTwoFromFilePanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																							   this.applicationPath,
																							   this.browseButton,
																							   this.label2});
			this.stepTwoFromFilePanel.Location = new System.Drawing.Point(320, 104);
			this.stepTwoFromFilePanel.Name = "stepTwoFromFilePanel";
			this.stepTwoFromFilePanel.Size = new System.Drawing.Size(10, 10);
			this.stepTwoFromFilePanel.TabIndex = 4;
			this.stepTwoFromFilePanel.Visible = false;
			// 
			// applicationPath
			// 
			this.applicationPath.Location = new System.Drawing.Point(16, 88);
			this.applicationPath.Name = "applicationPath";
			this.applicationPath.Size = new System.Drawing.Size(232, 21);
			this.applicationPath.TabIndex = 3;
			// 
			// browseButton
			// 
			this.browseButton.Location = new System.Drawing.Point(256, 88);
			this.browseButton.Name = "browseButton";
			this.browseButton.TabIndex = 2;
			this.browseButton.Text = "Browse...";
			this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
			// 
			// label2
			// 
			this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label2.Location = new System.Drawing.Point(16, 56);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(312, 16);
			this.label2.TabIndex = 0;
			this.label2.Text = "Enter the Application\'s Path Below or Click Browse";
			// 
			// stepTwoRunningProcessPanel
			// 
			this.stepTwoRunningProcessPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.stepTwoRunningProcessPanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																									 this.label3,
																									 this.refreshLink,
																									 this.lv_ProcessListBox});
			this.stepTwoRunningProcessPanel.Location = new System.Drawing.Point(336, 104);
			this.stepTwoRunningProcessPanel.Name = "stepTwoRunningProcessPanel";
			this.stepTwoRunningProcessPanel.Size = new System.Drawing.Size(10, 10);
			this.stepTwoRunningProcessPanel.TabIndex = 5;
			this.stepTwoRunningProcessPanel.Visible = false;
			// 
			// label3
			// 
			this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label3.Location = new System.Drawing.Point(8, 16);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(176, 16);
			this.label3.TabIndex = 29;
			this.label3.Text = "Select A Running Process:";
			// 
			// refreshLink
			// 
			this.refreshLink.Location = new System.Drawing.Point(288, 16);
			this.refreshLink.Name = "refreshLink";
			this.refreshLink.Size = new System.Drawing.Size(48, 16);
			this.refreshLink.TabIndex = 28;
			this.refreshLink.TabStop = true;
			this.refreshLink.Text = "Refresh";
			this.refreshLink.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.refreshLink_LinkClicked);
			// 
			// lv_ProcessListBox
			// 
			this.lv_ProcessListBox.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																								this.columnHeader1,
																								this.columnHeader2});
			this.lv_ProcessListBox.FullRowSelect = true;
			this.lv_ProcessListBox.HideSelection = false;
			this.lv_ProcessListBox.Location = new System.Drawing.Point(8, 40);
			this.lv_ProcessListBox.MultiSelect = false;
			this.lv_ProcessListBox.Name = "lv_ProcessListBox";
			this.lv_ProcessListBox.Size = new System.Drawing.Size(328, 200);
			this.lv_ProcessListBox.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lv_ProcessListBox.TabIndex = 27;
			this.lv_ProcessListBox.View = System.Windows.Forms.View.Details;
			this.lv_ProcessListBox.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.lv_ProcessListBox_ColumnClick);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "PID";
			this.columnHeader1.Width = 45;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Process name";
			this.columnHeader2.Width = 279;
			// 
			// stepThreePanel
			// 
			this.stepThreePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.stepThreePanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																						 this.label1});
			this.stepThreePanel.Location = new System.Drawing.Point(136, 96);
			this.stepThreePanel.Name = "stepThreePanel";
			this.stepThreePanel.Size = new System.Drawing.Size(368, 248);
			this.stepThreePanel.TabIndex = 6;
			this.stepThreePanel.Visible = false;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(224, 16);
			this.label1.TabIndex = 24;
			this.label1.Text = "Allow Logging of the Following Functions:";
			// 
			// appFileBrowseDialog
			// 
			this.appFileBrowseDialog.Filter = "Executable Files|*.exe";
			this.appFileBrowseDialog.Title = "Select The Application Executable File to Load";
			// 
			// chk_AppPaused
			// 
			this.chk_AppPaused.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.chk_AppPaused.Location = new System.Drawing.Point(24, 112);
			this.chk_AppPaused.Name = "chk_AppPaused";
			this.chk_AppPaused.Size = new System.Drawing.Size(184, 24);
			this.chk_AppPaused.TabIndex = 2;
			this.chk_AppPaused.Text = "Pause Application?";
			// 
			// BasicWizard
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(482, 406);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.stepThreePanel,
																		  this.stepTwoRunningProcessPanel,
																		  this.stepTwoFromFilePanel,
																		  this.stepsPanel,
																		  this.stepOnePanel,
																		  this.buttonPanel,
																		  this.pictureBox1});
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "BasicWizard";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "BasicWizard";
			this.Load += new System.EventHandler(this.BasicWizard_Load);
			this.buttonPanel.ResumeLayout(false);
			this.stepOnePanel.ResumeLayout(false);
			this.stepOneGroupBox.ResumeLayout(false);
			this.stepsPanel.ResumeLayout(false);
			this.stepTwoFromFilePanel.ResumeLayout(false);
			this.stepTwoRunningProcessPanel.ResumeLayout(false);
			this.stepThreePanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void refreshLink_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process[] ProcessList;
			ProcessList = System.Diagnostics.Process.GetProcesses();
			System.Windows.Forms.ListViewItem lvi = new System.Windows.Forms.ListViewItem();
			lv_ProcessListBox.Items.Clear();
			foreach(System.Diagnostics.Process myProcess in ProcessList)
			{
				lvi = new System.Windows.Forms.ListViewItem();
				lvi.Text = myProcess.Id.ToString();
				try
				{
					String procName = myProcess.MainModule.FileName;
					int slashPosition = procName.LastIndexOf('\\');
					if (slashPosition != -1)
						procName = procName.Substring(slashPosition + 1);

					lvi.SubItems.Add(procName);
				}
				catch
				{
					lvi.SubItems.Add(myProcess.ProcessName);
				}
				lv_ProcessListBox.Items.Add(lvi);
			}
			this.lv_ProcessListBox.ListViewItemSorter = processSorter;
			lv_ProcessListBox.Sort();
		}

		private void BasicWizard_Load(object sender, System.EventArgs e)
		{
			this.refreshLink_LinkClicked(sender, null);
			Array recentItems = RegistryHelper.getInstance().GetRecentlyLoadedItems();
			for (int i = 0; i < recentItems.Length; i++)
			{
				string val = (string)recentItems.GetValue(0, i);
				if (val != null)
				{
					applicationPath.Items.Add(recentItems.GetValue(0, i));
				}
			}

		}

		private void cancelButton_Click(object sender, System.EventArgs e)
		{
			Close();
		}

		private void prevButton_Click(object sender, System.EventArgs e)
		{
			switch(currentStepNumber)
			{
				case 1:
				{
					// impossible
					break;
				}
				case 2:
				{
					prevButton.Enabled = false;

					stepTwoLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
					stepTwoTextLabel.ForeColor = System.Drawing.SystemColors.ControlDark;

					stepOneLabel.ForeColor = System.Drawing.SystemColors.ControlText;
					stepOneTextLabel.ForeColor = System.Drawing.SystemColors.ControlText;

					stepTwoFromFilePanel.Visible = false;
					stepTwoRunningProcessPanel.Visible = false;

					stepOnePanel.Left = StepPanelLocationX;
					stepOnePanel.Top = StepPanelLocationY;
					stepOnePanel.Width = StepPanelWidth;
					stepOnePanel.Height = StepPanelHeight;
					stepOnePanel.Visible = true;

					currentStepNumber--;
					break;
				}
				case 3:
				{
					stepThreeLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
					stepThreeTextLabel.ForeColor = System.Drawing.SystemColors.ControlDark;

					stepTwoLabel.ForeColor = System.Drawing.SystemColors.ControlText;
					stepTwoTextLabel.ForeColor = System.Drawing.SystemColors.ControlText;

					stepThreePanel.Visible = false;

					Panel prevPanel;
					if (loadFromFileRadioButton.Checked)
					{
						prevPanel = stepTwoFromFilePanel;
					}
					else
					{
						prevPanel = stepTwoRunningProcessPanel;
					}

					prevPanel.Left = StepPanelLocationX;
					prevPanel.Top = StepPanelLocationY;
					prevPanel.Width = StepPanelWidth;
					prevPanel.Height = StepPanelHeight;
					prevPanel.Visible = true;

					currentStepNumber--;

					nextButton.Text = "Next >>";
					nextButton.DialogResult = DialogResult.None;

					break;
				}
			}
		}

		private void nextButton_Click(object sender, System.EventArgs e)
		{
			switch(currentStepNumber)
			{
				case 1:
				{
					prevButton.Enabled = true;

					stepOneLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
					stepOneTextLabel.ForeColor = System.Drawing.SystemColors.ControlDark;

					stepTwoLabel.ForeColor = System.Drawing.SystemColors.ControlText;
					stepTwoTextLabel.ForeColor = System.Drawing.SystemColors.ControlText;

					stepOnePanel.Visible = false;
					Panel nextPanel;
					if (loadFromFileRadioButton.Checked)
					{
						nextPanel = stepTwoFromFilePanel;
					}
					else
					{
						nextPanel = stepTwoRunningProcessPanel;
					}

					nextPanel.Left = StepPanelLocationX;
					nextPanel.Top = StepPanelLocationY;
					nextPanel.Width = StepPanelWidth;
					nextPanel.Height = StepPanelHeight;
					nextPanel.Visible = true;

					currentStepNumber++;

					break;
				}
				case 2:
				{
					if (loadFromFileRadioButton.Checked)
					{
						if ((applicationPath.Text == "") || (!System.IO.File.Exists(applicationPath.Text)))
						{
							MessageBox.Show(this, "You have not entered a valid application path");
							return;
						}
					}
					else
					{
						if (this.lv_ProcessListBox.SelectedItems.Count <= 0)
						{
							MessageBox.Show(this, "You have not selected a valid application to test");
							return;
						}
					}

					stepTwoLabel.ForeColor = System.Drawing.SystemColors.ControlDark;
					stepTwoTextLabel.ForeColor = System.Drawing.SystemColors.ControlDark;

					stepThreeLabel.ForeColor = System.Drawing.SystemColors.ControlText;
					stepThreeTextLabel.ForeColor = System.Drawing.SystemColors.ControlText;

					stepTwoFromFilePanel.Visible = false;
					stepTwoRunningProcessPanel.Visible = false;

					stepThreePanel.Left = StepPanelLocationX;
					stepThreePanel.Top = StepPanelLocationY;
					stepThreePanel.Width = StepPanelWidth;
					stepThreePanel.Height = StepPanelHeight;
					stepThreePanel.Visible = true;

					nextButton.Text = "Finish";
					nextButton.DialogResult = DialogResult.OK;

					currentStepNumber++;
					break;
				}
				case 3:
				{

					Close();
					break;
				}
			}
		}

		private void browseButton_Click(object sender, System.EventArgs e)
		{
			appFileBrowseDialog.ShowDialog(this);
			if (appFileBrowseDialog.FileName != "")
			{
				applicationPath.Text = appFileBrowseDialog.FileName;
			}
		}

		private void switchToAdvancedModeLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			Close();
			switchModes = true;
		}

		private void lv_ProcessListBox_ColumnClick(object sender, System.Windows.Forms.ColumnClickEventArgs e)
		{
			ProcessListItemSorter sorter = null;

			switch(e.Column)
			{
				case 0:
					this.lv_ProcessListBox.ListViewItemSorter = pidSorter;
					break;
				case 1:
					this.lv_ProcessListBox.ListViewItemSorter = processSorter;
					break;
				default:
					break;
			}

			switch(this.lv_ProcessListBox.Sorting)
			{
				case SortOrder.Ascending:
					this.lv_ProcessListBox.Sorting = SortOrder.Descending;
					sorter = (ProcessListItemSorter)this.lv_ProcessListBox.ListViewItemSorter;
					sorter.Ascending = false;
					break;
				case SortOrder.Descending:
					this.lv_ProcessListBox.Sorting = SortOrder.Ascending;
					sorter = (ProcessListItemSorter)this.lv_ProcessListBox.ListViewItemSorter;
					sorter.Ascending = true;
					break;
				default:
					this.lv_ProcessListBox.Sorting = SortOrder.Ascending;
					break;
			}
			this.lv_ProcessListBox.Sort();
		}
	}
}
