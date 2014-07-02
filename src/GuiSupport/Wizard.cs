using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for Wizard.
	/// </summary>
	public class Wizard : System.Windows.Forms.Form, AbstractLaunchWizard
	{
		private System.Windows.Forms.Button btn_OKButton;
		private System.Windows.Forms.Button btn_CancelButton;
		private System.Windows.Forms.Button btn_FileBrowseButton;
		private System.Windows.Forms.PictureBox pctbx_Logo;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ListView lv_ProcessListBox;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.RadioButton rdb_FromProcessRadio;
		private System.Windows.Forms.RadioButton rdb_FromFileRadio;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.LinkLabel refreshLink;
		private System.Windows.Forms.ComboBox cmbx_FileInput;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private bool switchModes;
		private ProcessListItemSorter pidSorter;
		private ProcessListItemSorter processSorter;
		private System.Windows.Forms.Label label1;
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
			get { return this.rdb_FromFileRadio.Checked; }
			set { this.rdb_FromFileRadio.Checked = value; }
		}

		public bool UseProcessID
		{
			get { return this.rdb_FromProcessRadio.Checked; }
			set { this.rdb_FromProcessRadio.Checked = value; }
		}

		public string ProcessPath
		{
			get { return this.cmbx_FileInput.Text; }
			set { this.cmbx_FileInput.Text = value; }
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

		public Wizard()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.SuspendLayout();
			this.logFilterTreeView = new LogFilterTreeView();
			this.logFilterTreeView.Location = new System.Drawing.Point(16, 370);
			this.logFilterTreeView.Size = new System.Drawing.Size(456, 263);
			this.Controls.AddRange(new System.Windows.Forms.Control[] { this.logFilterTreeView });
			this.ResumeLayout(false);

			switchModes = false;

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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Wizard));
			this.btn_OKButton = new System.Windows.Forms.Button();
			this.btn_CancelButton = new System.Windows.Forms.Button();
			this.btn_FileBrowseButton = new System.Windows.Forms.Button();
			this.pctbx_Logo = new System.Windows.Forms.PictureBox();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.lv_ProcessListBox = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.rdb_FromProcessRadio = new System.Windows.Forms.RadioButton();
			this.rdb_FromFileRadio = new System.Windows.Forms.RadioButton();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.refreshLink = new System.Windows.Forms.LinkLabel();
			this.cmbx_FileInput = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.chk_AppPaused = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// btn_OKButton
			// 
			this.btn_OKButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btn_OKButton.Location = new System.Drawing.Point(168, 640);
			this.btn_OKButton.Name = "btn_OKButton";
			this.btn_OKButton.Size = new System.Drawing.Size(144, 24);
			this.btn_OKButton.TabIndex = 19;
			this.btn_OKButton.Text = "OK";
			this.btn_OKButton.Click += new System.EventHandler(this.btn_OKButton_Click);
			// 
			// btn_CancelButton
			// 
			this.btn_CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btn_CancelButton.Location = new System.Drawing.Point(328, 640);
			this.btn_CancelButton.Name = "btn_CancelButton";
			this.btn_CancelButton.Size = new System.Drawing.Size(144, 24);
			this.btn_CancelButton.TabIndex = 17;
			this.btn_CancelButton.Text = "Cancel";
			// 
			// btn_FileBrowseButton
			// 
			this.btn_FileBrowseButton.Location = new System.Drawing.Point(384, 136);
			this.btn_FileBrowseButton.Name = "btn_FileBrowseButton";
			this.btn_FileBrowseButton.Size = new System.Drawing.Size(88, 24);
			this.btn_FileBrowseButton.TabIndex = 14;
			this.btn_FileBrowseButton.Text = "Browse...";
			this.btn_FileBrowseButton.Click += new System.EventHandler(this.btn_FileBrowseButton_Click);
			// 
			// pctbx_Logo
			// 
			this.pctbx_Logo.Name = "pctbx_Logo";
			this.pctbx_Logo.Size = new System.Drawing.Size(504, 104);
			this.pctbx_Logo.TabIndex = 12;
			this.pctbx_Logo.TabStop = false;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Process name";
			this.columnHeader2.Width = 360;
			// 
			// lv_ProcessListBox
			// 
			this.lv_ProcessListBox.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																								this.columnHeader1,
																								this.columnHeader2});
			this.lv_ProcessListBox.FullRowSelect = true;
			this.lv_ProcessListBox.HideSelection = false;
			this.lv_ProcessListBox.Location = new System.Drawing.Point(16, 192);
			this.lv_ProcessListBox.MultiSelect = false;
			this.lv_ProcessListBox.Name = "lv_ProcessListBox";
			this.lv_ProcessListBox.Size = new System.Drawing.Size(456, 144);
			this.lv_ProcessListBox.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.lv_ProcessListBox.TabIndex = 20;
			this.lv_ProcessListBox.View = System.Windows.Forms.View.Details;
			this.lv_ProcessListBox.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.lv_ProcessListBox_ColumnClick);
			this.lv_ProcessListBox.SelectedIndexChanged += new System.EventHandler(this.lv_ProcessListBox_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "PID";
			// 
			// openFileDialog1
			// 
			this.openFileDialog1.Filter = "Executable Files|*.exe|Holodeck Projects|*.hpl";
			// 
			// rdb_FromProcessRadio
			// 
			this.rdb_FromProcessRadio.Location = new System.Drawing.Point(16, 168);
			this.rdb_FromProcessRadio.Name = "rdb_FromProcessRadio";
			this.rdb_FromProcessRadio.Size = new System.Drawing.Size(200, 16);
			this.rdb_FromProcessRadio.TabIndex = 16;
			this.rdb_FromProcessRadio.Text = "Load a Currently Running Process:";
			this.rdb_FromProcessRadio.CheckedChanged += new System.EventHandler(this.rdb_FromProcessRadio_CheckedChanged);
			// 
			// rdb_FromFileRadio
			// 
			this.rdb_FromFileRadio.Checked = true;
			this.rdb_FromFileRadio.Location = new System.Drawing.Point(16, 112);
			this.rdb_FromFileRadio.Name = "rdb_FromFileRadio";
			this.rdb_FromFileRadio.Size = new System.Drawing.Size(224, 16);
			this.rdb_FromFileRadio.TabIndex = 15;
			this.rdb_FromFileRadio.TabStop = true;
			this.rdb_FromFileRadio.Text = "Start Application from Executable File:";
			this.rdb_FromFileRadio.CheckedChanged += new System.EventHandler(this.rdb_FromFileRadio_CheckedChanged);
			// 
			// pictureBox1
			// 
			this.pictureBox1.Image = ((System.Drawing.Bitmap)(resources.GetObject("pictureBox1.Image")));
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(490, 93);
			this.pictureBox1.TabIndex = 25;
			this.pictureBox1.TabStop = false;
			// 
			// refreshLink
			// 
			this.refreshLink.Location = new System.Drawing.Point(432, 176);
			this.refreshLink.Name = "refreshLink";
			this.refreshLink.Size = new System.Drawing.Size(48, 16);
			this.refreshLink.TabIndex = 26;
			this.refreshLink.TabStop = true;
			this.refreshLink.Text = "Refresh";
			this.refreshLink.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.refreshLink_LinkClicked);
			// 
			// cmbx_FileInput
			// 
			this.cmbx_FileInput.Location = new System.Drawing.Point(16, 136);
			this.cmbx_FileInput.Name = "cmbx_FileInput";
			this.cmbx_FileInput.Size = new System.Drawing.Size(360, 21);
			this.cmbx_FileInput.TabIndex = 27;
			this.cmbx_FileInput.SelectedIndexChanged += new System.EventHandler(this.cmbx_FileInput_SelectedIndexChanged);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(16, 352);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(256, 16);
			this.label1.TabIndex = 29;
			this.label1.Text = "Allow Logging of the Following Functions:";
			// 
			// chk_AppPaused
			// 
			this.chk_AppPaused.Location = new System.Drawing.Point(256, 108);
			this.chk_AppPaused.Name = "chk_AppPaused";
			this.chk_AppPaused.Size = new System.Drawing.Size(160, 24);
			this.chk_AppPaused.TabIndex = 30;
			this.chk_AppPaused.Text = "Pause Application?";
			// 
			// Wizard
			// 
			this.AcceptButton = this.btn_OKButton;
			this.AutoScale = false;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.btn_CancelButton;
			this.ClientSize = new System.Drawing.Size(484, 668);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.chk_AppPaused,
																		  this.label1,
																		  this.cmbx_FileInput,
																		  this.refreshLink,
																		  this.pictureBox1,
																		  this.btn_CancelButton,
																		  this.btn_FileBrowseButton,
																		  this.pctbx_Logo,
																		  this.lv_ProcessListBox,
																		  this.rdb_FromProcessRadio,
																		  this.rdb_FromFileRadio,
																		  this.btn_OKButton});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Wizard";
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Holodeck Enterprise Edition : Launchpad";
			this.Load += new System.EventHandler(this.Wizard_Load);
			this.ResumeLayout(false);

		}
		#endregion

		private void Wizard_Load(object sender, System.EventArgs e)
		{
			refreshLink_LinkClicked(sender,null);
			Array recentItems = RegistryHelper.getInstance().GetRecentlyLoadedItems();
			for (int i = 0; i < recentItems.Length; i++)
			{
				string val = (string)recentItems.GetValue(0, i);
				if (val != null)
				{
					cmbx_FileInput.Items.Add(recentItems.GetValue(0, i));
				}
			}
		}

		private void btn_FileBrowseButton_Click(object sender, System.EventArgs e)
		{
			openFileDialog1.FileName=cmbx_FileInput.Text;
			openFileDialog1.ShowDialog(this);
			cmbx_FileInput.Text=openFileDialog1.FileName;
		}

		private void rdb_FromFileRadio_CheckedChanged(object sender, System.EventArgs e)
		{
			if (rdb_FromFileRadio.Checked)
				rdb_FromProcessRadio.Checked=false;
		}

		private void rdb_FromProcessRadio_CheckedChanged(object sender, System.EventArgs e)
		{
			if (rdb_FromProcessRadio.Checked)
				rdb_FromFileRadio.Checked=false;
		}

		private void lv_ProcessListBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			rdb_FromProcessRadio.Checked=true;
		}

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

		private void cmbx_FileInput_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		
		}

		private void btn_OKButton_Click(object sender, System.EventArgs e)
		{
			this.DialogResult = DialogResult.OK;

			if ((rdb_FromProcessRadio.Checked == false) && ((this.cmbx_FileInput.Text == "") || (!System.IO.File.Exists(this.cmbx_FileInput.Text))))
			{
				MessageBox.Show(this, "You have not entered a valid application path");
				this.DialogResult = DialogResult.None;
				return;
			}

			Close();
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
