using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Data.SqlClient;
using System.DirectoryServices;

namespace BugTracker
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainWindow : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuItem4;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.Panel DetailPanel;
		private System.Windows.Forms.Splitter splitter1;

		public string username = "";
		public string password = "";
		public string server = "";
		public string dbserver = "teejay2.se.fit.edu";
		public string database = "hdbugdb";
		public string dbusername = "hdbugdb";
		public string dbpassword = "#hdbugdb#";
		public ArrayList servers = new ArrayList( );


		private System.Data.SqlClient.SqlConnection dbConnection;
		private System.Windows.Forms.StatusBar statusBar1;
		private System.Windows.Forms.StatusBarPanel statusBarPanel1;
		private DataSet bugDataList;
		private System.Windows.Forms.ListView bugListView;
		private System.Windows.Forms.ColumnHeader submitter;
		private System.Windows.Forms.ColumnHeader submitDate;
		private System.Windows.Forms.ColumnHeader owner;
		private System.Windows.Forms.ColumnHeader priority;
		private System.Windows.Forms.ColumnHeader title;
		private System.Windows.Forms.ColumnHeader ID;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.ComboBox milestoneComboBox;
		private System.Windows.Forms.ComboBox priorityComboBox;
		private System.Windows.Forms.ComboBox productComboBox;
		private System.Windows.Forms.ComboBox typeComboBox;
		private System.Windows.Forms.TextBox descriptionTextBox;
		private System.Windows.Forms.ComboBox ownerComboBox;
		private System.Windows.Forms.Button addButton;
		private System.Windows.Forms.Button updateButton;
		private System.Windows.Forms.Button newBugButton;
		private System.Windows.Forms.DateTimePicker submitDatePicker;
		private System.Windows.Forms.ComboBox submitterComboBox;
		private System.Windows.Forms.DateTimePicker closeDatePicker;
		private System.Windows.Forms.Label IDLabel;
		private System.Windows.Forms.StatusBarPanel statusBarPanel2;
		private System.Windows.Forms.ComboBox openStateComboBox;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.TextBox titleTextBox;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Button refreshButton;
		private System.Windows.Forms.Button upButton;
		private System.Windows.Forms.Button downButton;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.ComboBox submitterFilterComboBox;
		private System.Windows.Forms.ComboBox statusFilterComboBox;
		private System.Windows.Forms.ComboBox ownerFilterComboBox;
		private System.Windows.Forms.MenuItem menuItem7;
		private System.Windows.Forms.MenuItem menuItem8;
		private System.Windows.Forms.MenuItem menuItem9;
		private System.Windows.Forms.MenuItem menuItem10;
		private Crownwood.Magic.Menus.MenuCommand menuCommand1;
		private Crownwood.Magic.Controls.InertButton inertButton1;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainWindow()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			servers.Add( "hq.se.fit.edu" );
			servers.Add( "teejay2.se.fit.edu" );

			Login loginDialog = new Login( this );
			loginDialog.StartPosition = FormStartPosition.CenterScreen;

			loginDialog.ShowDialog( );
			
			if ( dbConnection.State == ConnectionState.Open )
				menuItem5.Enabled = false;
			else
				menuItem5.Enabled = true;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainWindow));
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.menuItem6 = new System.Windows.Forms.MenuItem();
			this.menuItem4 = new System.Windows.Forms.MenuItem();
			this.menuItem7 = new System.Windows.Forms.MenuItem();
			this.menuItem8 = new System.Windows.Forms.MenuItem();
			this.menuItem9 = new System.Windows.Forms.MenuItem();
			this.menuItem10 = new System.Windows.Forms.MenuItem();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.DetailPanel = new System.Windows.Forms.Panel();
			this.label12 = new System.Windows.Forms.Label();
			this.titleTextBox = new System.Windows.Forms.TextBox();
			this.label11 = new System.Windows.Forms.Label();
			this.openStateComboBox = new System.Windows.Forms.ComboBox();
			this.closeDatePicker = new System.Windows.Forms.DateTimePicker();
			this.submitDatePicker = new System.Windows.Forms.DateTimePicker();
			this.ownerComboBox = new System.Windows.Forms.ComboBox();
			this.newBugButton = new System.Windows.Forms.Button();
			this.submitterComboBox = new System.Windows.Forms.ComboBox();
			this.IDLabel = new System.Windows.Forms.Label();
			this.label10 = new System.Windows.Forms.Label();
			this.updateButton = new System.Windows.Forms.Button();
			this.addButton = new System.Windows.Forms.Button();
			this.label9 = new System.Windows.Forms.Label();
			this.descriptionTextBox = new System.Windows.Forms.TextBox();
			this.typeComboBox = new System.Windows.Forms.ComboBox();
			this.label8 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.productComboBox = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.priorityComboBox = new System.Windows.Forms.ComboBox();
			this.milestoneComboBox = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.splitter1 = new System.Windows.Forms.Splitter();
			this.dbConnection = new System.Data.SqlClient.SqlConnection();
			this.statusBar1 = new System.Windows.Forms.StatusBar();
			this.statusBarPanel1 = new System.Windows.Forms.StatusBarPanel();
			this.statusBarPanel2 = new System.Windows.Forms.StatusBarPanel();
			this.bugListView = new System.Windows.Forms.ListView();
			this.ID = new System.Windows.Forms.ColumnHeader();
			this.submitter = new System.Windows.Forms.ColumnHeader();
			this.submitDate = new System.Windows.Forms.ColumnHeader();
			this.owner = new System.Windows.Forms.ColumnHeader();
			this.priority = new System.Windows.Forms.ColumnHeader();
			this.title = new System.Windows.Forms.ColumnHeader();
			this.panel1 = new System.Windows.Forms.Panel();
			this.panel2 = new System.Windows.Forms.Panel();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label15 = new System.Windows.Forms.Label();
			this.statusFilterComboBox = new System.Windows.Forms.ComboBox();
			this.label14 = new System.Windows.Forms.Label();
			this.ownerFilterComboBox = new System.Windows.Forms.ComboBox();
			this.label13 = new System.Windows.Forms.Label();
			this.submitterFilterComboBox = new System.Windows.Forms.ComboBox();
			this.downButton = new System.Windows.Forms.Button();
			this.upButton = new System.Windows.Forms.Button();
			this.refreshButton = new System.Windows.Forms.Button();
			this.menuCommand1 = new Crownwood.Magic.Menus.MenuCommand();
			this.inertButton1 = new Crownwood.Magic.Controls.InertButton();
			this.DetailPanel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanel1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanel2)).BeginInit();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem3,
																					  this.menuItem7,
																					  this.menuItem1});
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 0;
			this.menuItem3.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem5,
																					  this.menuItem6,
																					  this.menuItem4});
			this.menuItem3.Text = "File";
			this.menuItem3.Click += new System.EventHandler(this.menuItem3_Click);
			// 
			// menuItem5
			// 
			this.menuItem5.Index = 0;
			this.menuItem5.Text = "&Connect To DB...";
			this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
			// 
			// menuItem6
			// 
			this.menuItem6.Index = 1;
			this.menuItem6.Text = "-";
			// 
			// menuItem4
			// 
			this.menuItem4.Index = 2;
			this.menuItem4.Text = "E&xit";
			this.menuItem4.Click += new System.EventHandler(this.menuItem4_Click);
			// 
			// menuItem7
			// 
			this.menuItem7.Index = 1;
			this.menuItem7.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem8,
																					  this.menuItem9,
																					  this.menuItem10});
			this.menuItem7.Text = "View";
			// 
			// menuItem8
			// 
			this.menuItem8.Checked = true;
			this.menuItem8.Index = 0;
			this.menuItem8.Text = "Bug List Tools";
			this.menuItem8.Click += new System.EventHandler(this.menuItem8_Click);
			// 
			// menuItem9
			// 
			this.menuItem9.Checked = true;
			this.menuItem9.Index = 1;
			this.menuItem9.Text = "Bug List";
			this.menuItem9.Click += new System.EventHandler(this.menuItem9_Click);
			// 
			// menuItem10
			// 
			this.menuItem10.Checked = true;
			this.menuItem10.Index = 2;
			this.menuItem10.Text = "Bug Entry Fields";
			this.menuItem10.Click += new System.EventHandler(this.menuItem10_Click);
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 2;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem2});
			this.menuItem1.Text = "Help";
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 0;
			this.menuItem2.Text = "&About...";
			this.menuItem2.Click += new System.EventHandler(this.menuItem2_Click);
			// 
			// DetailPanel
			// 
			this.DetailPanel.AutoScroll = true;
			this.DetailPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.DetailPanel.Controls.Add(this.inertButton1);
			this.DetailPanel.Controls.Add(this.label12);
			this.DetailPanel.Controls.Add(this.titleTextBox);
			this.DetailPanel.Controls.Add(this.label11);
			this.DetailPanel.Controls.Add(this.openStateComboBox);
			this.DetailPanel.Controls.Add(this.closeDatePicker);
			this.DetailPanel.Controls.Add(this.submitDatePicker);
			this.DetailPanel.Controls.Add(this.ownerComboBox);
			this.DetailPanel.Controls.Add(this.newBugButton);
			this.DetailPanel.Controls.Add(this.submitterComboBox);
			this.DetailPanel.Controls.Add(this.IDLabel);
			this.DetailPanel.Controls.Add(this.label10);
			this.DetailPanel.Controls.Add(this.updateButton);
			this.DetailPanel.Controls.Add(this.addButton);
			this.DetailPanel.Controls.Add(this.label9);
			this.DetailPanel.Controls.Add(this.descriptionTextBox);
			this.DetailPanel.Controls.Add(this.typeComboBox);
			this.DetailPanel.Controls.Add(this.label8);
			this.DetailPanel.Controls.Add(this.label7);
			this.DetailPanel.Controls.Add(this.productComboBox);
			this.DetailPanel.Controls.Add(this.label6);
			this.DetailPanel.Controls.Add(this.priorityComboBox);
			this.DetailPanel.Controls.Add(this.milestoneComboBox);
			this.DetailPanel.Controls.Add(this.label5);
			this.DetailPanel.Controls.Add(this.label4);
			this.DetailPanel.Controls.Add(this.label3);
			this.DetailPanel.Controls.Add(this.label2);
			this.DetailPanel.Controls.Add(this.label1);
			this.DetailPanel.Dock = System.Windows.Forms.DockStyle.Top;
			this.DetailPanel.Location = new System.Drawing.Point(0, 0);
			this.DetailPanel.Name = "DetailPanel";
			this.DetailPanel.Size = new System.Drawing.Size(736, 296);
			this.DetailPanel.TabIndex = 0;
			this.DetailPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.DetailPanel_Paint);
			// 
			// label12
			// 
			this.label12.Location = new System.Drawing.Point(8, 8);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(80, 16);
			this.label12.TabIndex = 33;
			this.label12.Text = "Title:";
			// 
			// titleTextBox
			// 
			this.titleTextBox.Enabled = false;
			this.titleTextBox.Location = new System.Drawing.Point(104, 3);
			this.titleTextBox.Name = "titleTextBox";
			this.titleTextBox.Size = new System.Drawing.Size(520, 20);
			this.titleTextBox.TabIndex = 1;
			this.titleTextBox.Text = "";
			this.titleTextBox.TextChanged += new System.EventHandler(this.titleTextBox_TextChanged);
			// 
			// label11
			// 
			this.label11.Location = new System.Drawing.Point(101, 136);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(40, 16);
			this.label11.TabIndex = 31;
			this.label11.Text = "Status:";
			// 
			// openStateComboBox
			// 
			this.openStateComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.openStateComboBox.Enabled = false;
			this.openStateComboBox.Items.AddRange(new object[] {
																   "OPEN",
																   "CLOSED"});
			this.openStateComboBox.Location = new System.Drawing.Point(152, 136);
			this.openStateComboBox.Name = "openStateComboBox";
			this.openStateComboBox.Size = new System.Drawing.Size(152, 21);
			this.openStateComboBox.TabIndex = 10;
			this.openStateComboBox.SelectedIndexChanged += new System.EventHandler(this.openStateComboBox_SelectedIndexChanged);
			// 
			// closeDatePicker
			// 
			this.closeDatePicker.Enabled = false;
			this.closeDatePicker.Location = new System.Drawing.Point(424, 32);
			this.closeDatePicker.Name = "closeDatePicker";
			this.closeDatePicker.TabIndex = 3;
			// 
			// submitDatePicker
			// 
			this.submitDatePicker.Enabled = false;
			this.submitDatePicker.Location = new System.Drawing.Point(104, 56);
			this.submitDatePicker.Name = "submitDatePicker";
			this.submitDatePicker.TabIndex = 4;
			// 
			// ownerComboBox
			// 
			this.ownerComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ownerComboBox.Enabled = false;
			this.ownerComboBox.Location = new System.Drawing.Point(104, 80);
			this.ownerComboBox.Name = "ownerComboBox";
			this.ownerComboBox.Size = new System.Drawing.Size(200, 21);
			this.ownerComboBox.TabIndex = 6;
			// 
			// newBugButton
			// 
			this.newBugButton.Enabled = false;
			this.newBugButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.newBugButton.Location = new System.Drawing.Point(520, 184);
			this.newBugButton.Name = "newBugButton";
			this.newBugButton.Size = new System.Drawing.Size(80, 24);
			this.newBugButton.TabIndex = 13;
			this.newBugButton.Text = "New Bug";
			this.newBugButton.Click += new System.EventHandler(this.button4_Click);
			// 
			// submitterComboBox
			// 
			this.submitterComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.submitterComboBox.Enabled = false;
			this.submitterComboBox.ItemHeight = 13;
			this.submitterComboBox.Location = new System.Drawing.Point(104, 32);
			this.submitterComboBox.Name = "submitterComboBox";
			this.submitterComboBox.Size = new System.Drawing.Size(200, 21);
			this.submitterComboBox.TabIndex = 2;
			// 
			// IDLabel
			// 
			this.IDLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.IDLabel.Location = new System.Drawing.Point(392, 138);
			this.IDLabel.Name = "IDLabel";
			this.IDLabel.Size = new System.Drawing.Size(280, 16);
			this.IDLabel.TabIndex = 11;
			// 
			// label10
			// 
			this.label10.Location = new System.Drawing.Point(337, 138);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(24, 16);
			this.label10.TabIndex = 22;
			this.label10.Text = "ID#";
			// 
			// updateButton
			// 
			this.updateButton.Enabled = false;
			this.updateButton.Location = new System.Drawing.Point(520, 248);
			this.updateButton.Name = "updateButton";
			this.updateButton.Size = new System.Drawing.Size(80, 24);
			this.updateButton.TabIndex = 15;
			this.updateButton.Text = "Update";
			this.updateButton.Click += new System.EventHandler(this.updateButton_Click);
			// 
			// addButton
			// 
			this.addButton.Enabled = false;
			this.addButton.Location = new System.Drawing.Point(520, 216);
			this.addButton.Name = "addButton";
			this.addButton.Size = new System.Drawing.Size(80, 24);
			this.addButton.TabIndex = 14;
			this.addButton.Text = "Add";
			this.addButton.Click += new System.EventHandler(this.addButton_Click);
			// 
			// label9
			// 
			this.label9.Location = new System.Drawing.Point(8, 136);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(80, 16);
			this.label9.TabIndex = 19;
			this.label9.Text = "Description:";
			// 
			// descriptionTextBox
			// 
			this.descriptionTextBox.Enabled = false;
			this.descriptionTextBox.Location = new System.Drawing.Point(8, 168);
			this.descriptionTextBox.Multiline = true;
			this.descriptionTextBox.Name = "descriptionTextBox";
			this.descriptionTextBox.Size = new System.Drawing.Size(496, 120);
			this.descriptionTextBox.TabIndex = 12;
			this.descriptionTextBox.Text = "";
			// 
			// typeComboBox
			// 
			this.typeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.typeComboBox.Enabled = false;
			this.typeComboBox.Location = new System.Drawing.Point(424, 104);
			this.typeComboBox.Name = "typeComboBox";
			this.typeComboBox.Size = new System.Drawing.Size(200, 21);
			this.typeComboBox.TabIndex = 9;
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(336, 104);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(80, 16);
			this.label8.TabIndex = 16;
			this.label8.Text = "Type:";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(8, 104);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(80, 16);
			this.label7.TabIndex = 15;
			this.label7.Text = "Product:";
			// 
			// productComboBox
			// 
			this.productComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.productComboBox.Enabled = false;
			this.productComboBox.Location = new System.Drawing.Point(104, 104);
			this.productComboBox.Name = "productComboBox";
			this.productComboBox.Size = new System.Drawing.Size(200, 21);
			this.productComboBox.TabIndex = 8;
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(336, 80);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(80, 16);
			this.label6.TabIndex = 13;
			this.label6.Text = "Priority:";
			// 
			// priorityComboBox
			// 
			this.priorityComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.priorityComboBox.Enabled = false;
			this.priorityComboBox.Location = new System.Drawing.Point(424, 80);
			this.priorityComboBox.Name = "priorityComboBox";
			this.priorityComboBox.Size = new System.Drawing.Size(200, 21);
			this.priorityComboBox.TabIndex = 7;
			// 
			// milestoneComboBox
			// 
			this.milestoneComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.milestoneComboBox.Enabled = false;
			this.milestoneComboBox.Location = new System.Drawing.Point(424, 56);
			this.milestoneComboBox.Name = "milestoneComboBox";
			this.milestoneComboBox.Size = new System.Drawing.Size(200, 21);
			this.milestoneComboBox.TabIndex = 5;
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(336, 56);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(80, 16);
			this.label5.TabIndex = 8;
			this.label5.Text = "Milestone:";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(336, 32);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(80, 16);
			this.label4.TabIndex = 6;
			this.label4.Text = "Close Date:";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 80);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(80, 16);
			this.label3.TabIndex = 4;
			this.label3.Text = "Owner:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 56);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(80, 16);
			this.label2.TabIndex = 2;
			this.label2.Text = "Submit Date:";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 32);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(80, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Submitter:";
			// 
			// splitter1
			// 
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Top;
			this.splitter1.Location = new System.Drawing.Point(0, 296);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(736, 4);
			this.splitter1.TabIndex = 2;
			this.splitter1.TabStop = false;
			// 
			// dbConnection
			// 
			this.dbConnection.StateChange += new System.Data.StateChangeEventHandler(this.dbConnection_StateChange);
			// 
			// statusBar1
			// 
			this.statusBar1.Location = new System.Drawing.Point(0, 456);
			this.statusBar1.Name = "statusBar1";
			this.statusBar1.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
																						  this.statusBarPanel1,
																						  this.statusBarPanel2});
			this.statusBar1.ShowPanels = true;
			this.statusBar1.Size = new System.Drawing.Size(736, 24);
			this.statusBar1.TabIndex = 3;
			// 
			// statusBarPanel1
			// 
			this.statusBarPanel1.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring;
			this.statusBarPanel1.Text = "Not Connected";
			this.statusBarPanel1.Width = 710;
			// 
			// statusBarPanel2
			// 
			this.statusBarPanel2.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Contents;
			this.statusBarPanel2.Width = 10;
			// 
			// bugListView
			// 
			this.bugListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						  this.ID,
																						  this.submitter,
																						  this.submitDate,
																						  this.owner,
																						  this.priority,
																						  this.title});
			this.bugListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.bugListView.FullRowSelect = true;
			this.bugListView.HideSelection = false;
			this.bugListView.Location = new System.Drawing.Point(0, 0);
			this.bugListView.MultiSelect = false;
			this.bugListView.Name = "bugListView";
			this.bugListView.Size = new System.Drawing.Size(736, 156);
			this.bugListView.TabIndex = 16;
			this.bugListView.View = System.Windows.Forms.View.Details;
			this.bugListView.MouseDown += new System.Windows.Forms.MouseEventHandler(this.bugListView_MouseDown);
			this.bugListView.ItemActivate += new System.EventHandler(this.bugListView_SelectedIndexChanged);
			this.bugListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.bugListView_ColumnClick);
			this.bugListView.SelectedIndexChanged += new System.EventHandler(this.bugListView_SelectedIndexChanged);
			// 
			// ID
			// 
			this.ID.Text = "ID";
			this.ID.Width = 215;
			// 
			// submitter
			// 
			this.submitter.Text = "Submitter";
			this.submitter.Width = 88;
			// 
			// submitDate
			// 
			this.submitDate.Text = "Submit Date";
			this.submitDate.Width = 134;
			// 
			// owner
			// 
			this.owner.Text = "Owner";
			this.owner.Width = 80;
			// 
			// priority
			// 
			this.priority.Text = "Priority";
			this.priority.Width = 48;
			// 
			// title
			// 
			this.title.Text = "Title";
			this.title.Width = 417;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.panel2);
			this.panel1.Controls.Add(this.bugListView);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Location = new System.Drawing.Point(0, 300);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(736, 156);
			this.panel1.TabIndex = 4;
			// 
			// panel2
			// 
			this.panel2.AutoScroll = true;
			this.panel2.Controls.Add(this.groupBox1);
			this.panel2.Controls.Add(this.downButton);
			this.panel2.Controls.Add(this.upButton);
			this.panel2.Controls.Add(this.refreshButton);
			this.panel2.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel2.Location = new System.Drawing.Point(0, 114);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(736, 42);
			this.panel2.TabIndex = 0;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.label15);
			this.groupBox1.Controls.Add(this.statusFilterComboBox);
			this.groupBox1.Controls.Add(this.label14);
			this.groupBox1.Controls.Add(this.ownerFilterComboBox);
			this.groupBox1.Controls.Add(this.label13);
			this.groupBox1.Controls.Add(this.submitterFilterComboBox);
			this.groupBox1.Location = new System.Drawing.Point(200, 0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(488, 40);
			this.groupBox1.TabIndex = 10;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Filters";
			// 
			// label15
			// 
			this.label15.Location = new System.Drawing.Point(329, 18);
			this.label15.Name = "label15";
			this.label15.Size = new System.Drawing.Size(40, 16);
			this.label15.TabIndex = 15;
			this.label15.Text = "Status:";
			// 
			// statusFilterComboBox
			// 
			this.statusFilterComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.statusFilterComboBox.Location = new System.Drawing.Point(377, 14);
			this.statusFilterComboBox.Name = "statusFilterComboBox";
			this.statusFilterComboBox.Size = new System.Drawing.Size(96, 21);
			this.statusFilterComboBox.TabIndex = 14;
			this.statusFilterComboBox.SelectedIndexChanged += new System.EventHandler(this.statusFilterComboBox_SelectedIndexChanged);
			// 
			// label14
			// 
			this.label14.Location = new System.Drawing.Point(176, 17);
			this.label14.Name = "label14";
			this.label14.Size = new System.Drawing.Size(48, 16);
			this.label14.TabIndex = 13;
			this.label14.Text = "Owner:";
			// 
			// ownerFilterComboBox
			// 
			this.ownerFilterComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ownerFilterComboBox.Location = new System.Drawing.Point(226, 14);
			this.ownerFilterComboBox.Name = "ownerFilterComboBox";
			this.ownerFilterComboBox.Size = new System.Drawing.Size(96, 21);
			this.ownerFilterComboBox.TabIndex = 12;
			this.ownerFilterComboBox.SelectedIndexChanged += new System.EventHandler(this.ownerFilterComboBox_SelectedIndexChanged);
			// 
			// label13
			// 
			this.label13.Location = new System.Drawing.Point(16, 17);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(56, 16);
			this.label13.TabIndex = 11;
			this.label13.Text = "Submitter: ";
			// 
			// submitterFilterComboBox
			// 
			this.submitterFilterComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.submitterFilterComboBox.Location = new System.Drawing.Point(72, 14);
			this.submitterFilterComboBox.Name = "submitterFilterComboBox";
			this.submitterFilterComboBox.Size = new System.Drawing.Size(96, 21);
			this.submitterFilterComboBox.TabIndex = 10;
			this.submitterFilterComboBox.SelectedIndexChanged += new System.EventHandler(this.submitterFilterComboBox_SelectedIndexChanged);
			// 
			// downButton
			// 
			this.downButton.Image = ((System.Drawing.Image)(resources.GetObject("downButton.Image")));
			this.downButton.Location = new System.Drawing.Point(152, 10);
			this.downButton.Name = "downButton";
			this.downButton.Size = new System.Drawing.Size(32, 24);
			this.downButton.TabIndex = 2;
			this.downButton.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.downButton.Click += new System.EventHandler(this.downButton_Click);
			// 
			// upButton
			// 
			this.upButton.Image = ((System.Drawing.Image)(resources.GetObject("upButton.Image")));
			this.upButton.Location = new System.Drawing.Point(112, 10);
			this.upButton.Name = "upButton";
			this.upButton.Size = new System.Drawing.Size(32, 24);
			this.upButton.TabIndex = 1;
			this.upButton.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.upButton.Click += new System.EventHandler(this.upButton_Click);
			// 
			// refreshButton
			// 
			this.refreshButton.Image = ((System.Drawing.Image)(resources.GetObject("refreshButton.Image")));
			this.refreshButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.refreshButton.Location = new System.Drawing.Point(7, 10);
			this.refreshButton.Name = "refreshButton";
			this.refreshButton.Size = new System.Drawing.Size(89, 24);
			this.refreshButton.TabIndex = 0;
			this.refreshButton.Text = "Refresh List";
			this.refreshButton.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.refreshButton.Click += new System.EventHandler(this.refreshButton_Click);
			// 
			// menuCommand1
			// 
			this.menuCommand1.Description = "MenuItem";
			// 
			// inertButton1
			// 
			this.inertButton1.Location = new System.Drawing.Point(632, 200);
			this.inertButton1.Name = "inertButton1";
			this.inertButton1.Size = new System.Drawing.Size(56, 32);
			this.inertButton1.TabIndex = 34;
			this.inertButton1.Text = "inertButton1";
			// 
			// MainWindow
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.AutoScroll = true;
			this.ClientSize = new System.Drawing.Size(736, 480);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.statusBar1);
			this.Controls.Add(this.splitter1);
			this.Controls.Add(this.DetailPanel);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Menu = this.mainMenu1;
			this.Name = "MainWindow";
			this.Text = "BugTracker";
			this.Load += new System.EventHandler(this.MainWindow_Load);
			this.Activated += new System.EventHandler(this.MainWindow_Activated);
			this.DetailPanel.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanel1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanel2)).EndInit();
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new MainWindow());
			
		}

		private void menuItem3_Click(object sender, System.EventArgs e)
		{
		
		}

		private void menuItem5_Click(object sender, System.EventArgs e)
		{
			Login loginDialog = new Login( this );			

			loginDialog.Visible = false;			

			// Start the login window...
			loginDialog.ShowDialog( this );
		}

		private void menuItem4_Click(object sender, System.EventArgs e)
		{
			this.Close( );
		}

		private void DetailPanel_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
		
		}

		public void listBugs( )
		{
			if ( dbConnection.State != ConnectionState.Open )
				return;

			if ( submitterFilterComboBox.Items.Count == 0 )
			{
				clearFilterBoxes( );
				fillFilterBoxes( );
			}

			string selectQuery = "SELECT bugID, submitter, submitDate, owner, priority, title, isOpen FROM " + "BugDB";

			SqlDataReader sr = null;			

			// Clear out the current list...
			bugListView.Items.Clear( );

			try
			{
				SqlCommand queryCommand = new SqlCommand( selectQuery, dbConnection );
			
				sr = queryCommand.ExecuteReader( );				
				
				while ( sr.Read( ) )
				{										
					ListViewItem listItem = new ListViewItem( );
					listItem.Text = sr.GetGuid( 0 ).ToString( );					
					listItem.SubItems.Add( sr.GetString( 1 ) );
					listItem.SubItems.Add( sr.GetDateTime( 2 ).ToString( ) );
					listItem.SubItems.Add( sr.GetString( 3 ) );
					listItem.SubItems.Add( sr.GetInt32( 4 ).ToString( ) );
					listItem.SubItems.Add( sr.GetString( 5 ) );					

					if ( statusFilterComboBox.SelectedIndex == 0 )
						bugListView.Items.Add( listItem );
					else if ( ( statusFilterComboBox.SelectedIndex == 1 ) && sr.GetBoolean( 6 ) )
						bugListView.Items.Add( listItem );
					else if ( ( statusFilterComboBox.SelectedIndex == 2 ) && !sr.GetBoolean( 6 ) )
						bugListView.Items.Add( listItem );

				}
			}
			catch
			{
				
			}
			finally
			{
				// Close out the data reader...
				sr.Close( );
			}
			
			if ( bugListView.Items.Count != 0 )
				applyFilters( );
		}

		public int boolToBit( bool val )
		{
			if ( val )
				return 1;
			else
				return 0;
		}

		public bool addBugToDatabase( BugEntry entry )
		{				
			// Build up the insertion query string...
			string insertQuery = "INSERT " + "BugDB" + " (submitter, submitDate, owner, closeDate, mileStone, priority, product, type, description, history, bugID, title, isOpen ) VALUES ('" +
				 entry.submitter + "', '" + entry.submitDate.ToString( ) + "', '" + entry.owner + "', '" + entry.closeDate + "', '" + entry.mileStone + "', '" + entry.priority.ToString( ) + "', '" + entry.product + "', '" + 
				 entry.type + "', '" + entry.description + "', '" + entry.history + "', '" + entry.bugID.ToString( ) + "', '" + entry.title + "', '" + boolToBit( entry.isOpen ).ToString( ) + "');";

			try
			{
				// Build up the sql command...
				SqlCommand cmd = new SqlCommand( insertQuery, dbConnection );

				// Perform the query to add the information to the database...
				cmd.ExecuteNonQuery( );
			}
			catch  ( Exception e )
			{
				MessageBox.Show( "A problem occurred during the add operation: \n\n" + e.Message, "Error adding to DB" );
				return false;
			}
		
			return true;
		}

		public bool updateBugInDatabase( BugEntry entry )
		{
			// Build up the insertion query string...
			string updateQuery = "UPDATE " + "BugDB" + " SET submitter='" + entry.submitter + "', submitDate='" + entry.submitDate.ToString( ) + "', owner='" + entry.owner + "', closeDate='" + entry.closeDate.ToString( ) + "', mileStone='" + entry.mileStone + "', priority='" + entry.priority.ToString( ) +
								 "', product='" + entry.product + "', type='" + entry.type + "', description='" + entry.description + "', history='" + entry.history + "', title='" + entry.title + "', isOpen='" + boolToBit( entry.isOpen ).ToString( ) + "' WHERE bugID='" + entry.bugID.ToString( ) + "';";

			try
			{
				// Build up the sql command...
				SqlCommand cmd = new SqlCommand( updateQuery, dbConnection );

				// Perform the query to add the information to the database...
				cmd.ExecuteNonQuery( );
			}
			catch  ( Exception e )
			{
				MessageBox.Show( "A problem occurred during the add operation: \n\n" + e.Message, "Error adding to DB" );
				return false;
			}
		
			return true;
		}

		public BugEntry getBugFromDatabase( Guid guid )
		{
			string selectQuery = "SELECT submitter, submitDate, owner, closeDate, mileStone, priority, product, type, description, history, bugID, title, isOpen FROM " + "BugDB WHERE " +
								 "bugID='" + guid.ToString( ) + "';";

			BugEntry dbEntry = null;

			SqlDataReader sr = null;			

			try
			{
				SqlCommand queryCommand = new SqlCommand( selectQuery, dbConnection );
			
				sr = queryCommand.ExecuteReader( );				
				
				dbEntry = new BugEntry( );

				while ( sr.Read( ) )
				{
					dbEntry.submitter = sr.GetString( 0 );
					dbEntry.submitDate = sr.GetDateTime( 1 );
					dbEntry.owner = sr.GetString( 2 );
					dbEntry.closeDate = sr.GetDateTime( 3 );
					dbEntry.mileStone = sr.GetString( 4 );
					dbEntry.priority = sr.GetInt32( 5 );
					dbEntry.product = sr.GetString( 6 );
					dbEntry.type = sr.GetString( 7 );
					dbEntry.description = sr.GetString( 8 );
					dbEntry.history = sr.GetString( 9 );
					dbEntry.bugID = sr.GetGuid( 10 );
					dbEntry.title = sr.GetString( 11 );
					dbEntry.isOpen = sr.GetBoolean( 12 );
				}
			}
			catch
			{
				// Return null if failure...
				dbEntry = null;
			}
			finally
			{
				sr.Close( );
			}

			return dbEntry;
		}

		public UserDB getUserFromDatabase( string userLoginName )
		{
			string selectQuery = "SELECT userID, canAddBug, canDeleteBug, canModifyBug FROM " + "UserDB";

			UserDB dbEntry = null;

			SqlDataReader sr = null;			

			try
			{
				SqlCommand queryCommand = new SqlCommand( selectQuery, dbConnection );
			
				sr = queryCommand.ExecuteReader( );				
				
				if ( !sr.HasRows )
					return null;

				dbEntry = new UserDB( );

				while ( sr.Read( ) )
				{
					dbEntry.userID = sr.GetString( 0 );
					dbEntry.canAddBug = sr.GetBoolean( 1 );
					dbEntry.canDeleteBug = sr.GetBoolean( 2 );
					dbEntry.canModifyBug = sr.GetBoolean( 3 );
				}
			}
			catch
			{
				// Return null if failure...
				dbEntry = null;
			}
			finally
			{
				sr.Close( );
			}

			return dbEntry;
		}

		// Retrieves the list of all users on the server...
		public string [] retrieveUserList( )
		{
			string selectQuery = "SELECT userID FROM " + "UserDB";

			ArrayList userList = new ArrayList( );

			SqlDataReader sr = null;			

			try
			{
				SqlCommand queryCommand = new SqlCommand( selectQuery, dbConnection );
			
				sr = queryCommand.ExecuteReader( );				
				
				if ( !sr.HasRows )
					return null;				

				while ( sr.Read( ) )
				{
					userList.Add( sr.GetString( 0 ) );
				}
			}
			catch
			{
				// Return null if failure...
				userList = null;
			}
			finally
			{
				sr.Close( );
			}

			return (string [])userList.ToArray( typeof( string ) );
		}

		public string [] getPropertyList( string propertyTable )
		{
			string selectQuery = "SELECT * FROM " + propertyTable;

			ArrayList itemList = new ArrayList( );

			SqlDataReader sr = null;			

			try
			{
				SqlCommand queryCommand = new SqlCommand( selectQuery, dbConnection );
			
				sr = queryCommand.ExecuteReader( );				
				
				if ( !sr.HasRows )
					return null;				

				while ( sr.Read( ) )
				{
					itemList.Add( sr.GetString( 0 ) );
				}
			}
			catch
			{
				// Return null if failure...
				itemList = null;
			}
			finally
			{
				sr.Close( );
			}

			return (string [])itemList.ToArray( typeof( string ) );
		}

		public bool connectToServer( )
		{
			string connectionString = "database=" + database + ";server=" + dbserver + ";User ID=" + dbusername + ";password=" + dbpassword + ";Connect Timeout=30";

			dbConnection.ConnectionString = connectionString;						

			try
			{
				dbConnection.Open( );				

				// Setup a connection string for the directory...
				DirectoryEntry entry = new DirectoryEntry( "WinNT://" + server + ",computer", username, password );

				// Attempt to access directory forcing authentication to occur...
				Object native = entry.NativeObject;

				// We've made it this far... now time to check if the user is in the
				// database...
				UserDB userInfo = getUserFromDatabase( username );

				if ( userInfo == null )
					throw new Exception( "User does not have permission to access database." );

				statusBar1.Panels[1].Text = "USER: " + username;
				
				// Update panel...
				statusBar1.Panels[0].Text = "Connected to: " + server;

				newBugButton.Enabled = true;

				// List all bugs in the list...
				listBugs( );
			}

			catch ( Exception e )
			{
				// if this fails, show message...

				System.Windows.Forms.MessageBox.Show( "There was an error connecting to the database server. Check that the server is available.\n\n" + e.Message, "Connection Error!" );

				// Ensurre that panel contains text Not Connected...
				statusBar1.Panels[0].Text = "Not Connected";
				statusBar1.Panels[1].Text = "";

				newBugButton.Enabled = false;

				dbConnection.Close( );

				return false;
			}

			return true;
		}

		public void applyFilters( )
		{			
			foreach( ListViewItem item in bugListView.Items )
			{
				if ( submitterFilterComboBox.SelectedIndex != 0 )
				{
					if ( item.SubItems[1].Text != submitterFilterComboBox.SelectedItem.ToString( ) )
						item.Remove( );
				}

				if ( ownerFilterComboBox.SelectedIndex != 0 )
				{
					if ( item.SubItems[3].Text != ownerFilterComboBox.SelectedItem.ToString( ) )
						item.Remove( );				
				}
			}
		
		}

		public void fillFilterBoxes( )
		{
			submitterFilterComboBox.Items.Add( "<< ALL >>" );			
			submitterFilterComboBox.Items.AddRange( retrieveUserList( ) );
			submitterFilterComboBox.SelectedIndex = 0;
			
			ownerFilterComboBox.Items.Add( "<< ALL >>" );
			ownerFilterComboBox.Items.AddRange( retrieveUserList( ) );
			ownerFilterComboBox.SelectedIndex = 0;

			statusFilterComboBox.Items.Add( "<< ALL >>" );
			statusFilterComboBox.Items.Add( "OPEN" );
			statusFilterComboBox.Items.Add( "CLOSED" );
			statusFilterComboBox.SelectedIndex = 0;
		}

		public void clearFilterBoxes( )
		{
			submitterFilterComboBox.Items.Clear( );
			ownerFilterComboBox.Items.Clear( );
			statusFilterComboBox.Items.Clear( );
		}

		/// <summary>
		/// Fill combo boxes with common information from database...
		/// </summary>
		public void fillComboBoxes( )
		{
			// Get list of users to populate the combo boxes...
			string [] userList = retrieveUserList( );

			submitterComboBox.Items.AddRange( userList );
			ownerComboBox.Items.AddRange( userList );

			// Populate remaining combo boxes...
			productComboBox.Items.AddRange( getPropertyList( "ProductDB" ) );

			milestoneComboBox.Items.AddRange( getPropertyList( "MilestoneDB" ) );

			priorityComboBox.Items.AddRange( getPropertyList( "PriorityDB" ) );

			typeComboBox.Items.AddRange( getPropertyList( "TypeDB" ) );			
		}

		public void clearAllFields( )
		{
			// Clear out all combo boxes and fields...
			submitterComboBox.Items.Clear( );
			submitDatePicker.ResetText( );
			ownerComboBox.Items.Clear( );
			productComboBox.Items.Clear( );
			descriptionTextBox.Text = "";
			closeDatePicker.ResetText( );
			milestoneComboBox.Items.Clear( );
			priorityComboBox.Items.Clear( );
			typeComboBox.Items.Clear( );
			titleTextBox.Text = "";
		}

		private void button4_Click(object sender, System.EventArgs e)
		{
			submitterComboBox.Enabled = true;
			//submitDatePicker.Enabled = true;
			ownerComboBox.Enabled = true;
			productComboBox.Enabled = true;
			descriptionTextBox.Enabled = true;
			closeDatePicker.Enabled = true;
			milestoneComboBox.Enabled = true;
			priorityComboBox.Enabled = true;
			typeComboBox.Enabled = true;
			addButton.Enabled = true;
			openStateComboBox.Enabled = true;
			titleTextBox.Enabled = true;

			clearAllFields( );

			// Set the state to OPEN...
			openStateComboBox.Text = openStateComboBox.Items[0].ToString( );			

			IDLabel.Text = Guid.NewGuid( ).ToString( );
			
			fillComboBoxes( );

			updateButton.Enabled = false;

			// Set the submitter to be the current user by default...
			submitterComboBox.SelectedIndex = submitterComboBox.Items.IndexOf( username );			
		}

		private void menuItem2_Click(object sender, System.EventArgs e)
		{
			About aboutWindow = new About( );
			aboutWindow.Visible = false;

			aboutWindow.ShowDialog( this );
		}

		private void addButton_Click(object sender, System.EventArgs e)
		{
			if ( validateEntryInformation( ) )
			{
				BugEntry entry = new BugEntry( );

				entry.title = titleTextBox.Text;
				entry.submitter = submitterComboBox.Text;
				entry.submitDate = DateTime.Now;
				entry.owner = ownerComboBox.Text;
				entry.product = productComboBox.Text;
				entry.description = descriptionTextBox.Text;
				entry.closeDate = DateTime.Parse( closeDatePicker.Text );
				entry.mileStone = milestoneComboBox.Text;
				entry.priority = System.Convert.ToInt32( priorityComboBox.Text, 10 );
				entry.type = typeComboBox.Text;
				entry.isOpen = ( openStateComboBox.Text == "OPEN" );
				entry.bugID = new Guid( IDLabel.Text );
				entry.history = "USER " + username + " added bug " + entry.bugID + " on " + entry.submitDate.ToString( );

				if ( !addBugToDatabase( entry ) )
				{
					MessageBox.Show( "Could not add bug to database!", "Error adding to DB" );
				}
				else
				{
					addButton.Enabled = false;
					updateButton.Enabled = true;
				
					// Update the listing of bugs...
					listBugs( );
				}
			}
		}

		public void fillInBugInformation( BugEntry entry )
		{
			titleTextBox.Text = entry.title;
			submitterComboBox.SelectedIndex = submitterComboBox.Items.IndexOf( entry.submitter );
			submitDatePicker.Text = entry.submitDate.ToString( );
			ownerComboBox.SelectedIndex = ownerComboBox.Items.IndexOf( entry.owner );
			productComboBox.SelectedIndex = productComboBox.Items.IndexOf( entry.product );
			descriptionTextBox.Text = entry.description;
			closeDatePicker.Text = entry.closeDate.ToString( );
			milestoneComboBox.SelectedIndex = milestoneComboBox.Items.IndexOf( entry.mileStone );
			priorityComboBox.SelectedIndex = priorityComboBox.Items.IndexOf( entry.priority.ToString( ) );
			typeComboBox.SelectedIndex = typeComboBox.Items.IndexOf( entry.type );
			
			if ( entry.isOpen )
				openStateComboBox.SelectedIndex = openStateComboBox.Items.IndexOf( "OPEN" );
			else
				openStateComboBox.SelectedIndex = openStateComboBox.Items.IndexOf( "CLOSED" );

			IDLabel.Text = entry.bugID.ToString( );
		}

		public void enableBugEntry( )
		{
			titleTextBox.Enabled = true;
			submitterComboBox.Enabled = true;
			ownerComboBox.Enabled = true;
			productComboBox.Enabled = true;
			descriptionTextBox.Enabled = true;
			closeDatePicker.Enabled = true;
			milestoneComboBox.Enabled = true;
			priorityComboBox.Enabled = true;
			typeComboBox.Enabled = true;
			openStateComboBox.Enabled = true;
		}

		private void bugListView_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			if ( bugListView.SelectedItems.Count == 0 )
				return;

			// Get the bug from the database...
			BugEntry entry = getBugFromDatabase( new Guid( bugListView.SelectedItems[0].Text ) );
		
			enableBugEntry( );
			addButton.Enabled = false;
			updateButton.Enabled = true;			

			clearAllFields( );

			fillComboBoxes( );

			fillInBugInformation( entry );
		}

		public bool validateEntryInformation( )
		{
			if ( titleTextBox.Text == "" )
			{
				MessageBox.Show( "Please add data to the title of your entry.", "A title is required." );
				return false;
			}

			if ( submitterComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add the name of the submitter to your entry.", "A submitter name is required." );
				return false;
			}

			if ( ownerComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add the name of the owner to your entry.", "A owner name is required." );
				return false;
			}

			if ( productComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add the name of the product to your entry.", "A product name is required." );
				return false;
			}
			
			if ( descriptionTextBox.Text == "" )
			{
				MessageBox.Show( "Please add a description to your entry.", "A description is required." );
				return false;
			}

			if ( closeDatePicker.Text == "" )
			{
				MessageBox.Show( "A close date is required for your entry.", "A close date is required." );
				return false;
			}

			if ( milestoneComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add a milestone to your entry.", "A milestone is required." );
				return false;
			}

		
			if ( priorityComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add a priority to your entry.", "A priority is required." );
				return false;
			}

			if ( typeComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add a type to your entry.", "A type is required." );
				return false;
			}

			if ( openStateComboBox.SelectedItem == null )
			{
				MessageBox.Show( "Please add an open state to your entry.", "An open state is required." );
				return false;
			}

			return true;
		}

		private void bugListView_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if ( bugListView.SelectedItems.Count != 0 )
				bugListView_SelectedIndexChanged( null, null );
		}

		private void openStateComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		
		}

		private void titleTextBox_TextChanged(object sender, System.EventArgs e)
		{
		
		}

		private void updateButton_Click(object sender, System.EventArgs e)
		{
			BugEntry entry = new BugEntry( );
							 
			entry.title = titleTextBox.Text;
			entry.submitter = submitterComboBox.Text;
			entry.submitDate = DateTime.Now;
			entry.owner = ownerComboBox.Text;
			entry.product = productComboBox.Text;
			entry.description = descriptionTextBox.Text;
			entry.closeDate = DateTime.Parse( closeDatePicker.Text );
			entry.mileStone = milestoneComboBox.Text;
			entry.priority = System.Convert.ToInt32( priorityComboBox.Text, 10 );
			entry.type = typeComboBox.Text;
			entry.isOpen = ( openStateComboBox.Text == "OPEN" );
			entry.bugID = new Guid( IDLabel.Text );
			entry.history = "USER " + username + " updated bug " + entry.bugID + " on " + entry.submitDate.ToString( );

			// Update the entry in the database...
			updateBugInDatabase( entry );

			listBugs( );
		}
		
		private void bugListView_ColumnClick(object sender, System.Windows.Forms.ColumnClickEventArgs e)
		{
			// First, we need to pull out all items...
			ArrayList listItemArray = new ArrayList( );

			foreach ( ListViewItem item in bugListView.Items )
			{
				ShownItem listItem = new ShownItem( );

				listItem.ID = item.Text;
				listItem.submitter = item.SubItems[1].Text;
				listItem.submitDate= item.SubItems[2].Text;
				listItem.owner = item.SubItems[3].Text;
				listItem.priority = item.SubItems[4].Text;
				listItem.title = item.SubItems[5].Text;

				// Add the entry to the arraylist...
				listItemArray.Add( listItem );
			}
			
			ShownItemComparer.indexToCompare = e.Column;
			
			// Sort the array of items...
			listItemArray.Sort( 0, listItemArray.Count, new ShownItemComparer( ) );

			// Clear the list view...
			bugListView.Items.Clear( );

			// Build up the list view...
			foreach( ShownItem item in listItemArray )
			{
				ListViewItem lvItem = new ListViewItem( );

				lvItem.Text = item.ID;
				lvItem.SubItems.Add( item.submitter );
				lvItem.SubItems.Add( item.submitDate );
				lvItem.SubItems.Add( item.owner );
				lvItem.SubItems.Add( item.priority );
				lvItem.SubItems.Add( item.title );

				bugListView.Items.Add( lvItem );
			}			
		}

		private void MainWindow_Load(object sender, System.EventArgs e)
		{
			
		}

		private void MainWindow_Validated(object sender, System.EventArgs e)
		{
			
		}

		private void MainWindow_Activated(object sender, System.EventArgs e)
		{
			
		}

		private void dbConnection_StateChange(object sender, System.Data.StateChangeEventArgs e)
		{
			if ( dbConnection.State != ConnectionState.Open )
			{
				MessageBox.Show( "The connection to the server was lost! Please login again from the FILE menu.", "Connection Lost." );

				// Close the connection if it is still open in some other connection state...
				try
				{
					dbConnection.Close( );
				}
				catch
				{
				}

				menuItem5.Enabled = true;
			}
			else
			{
				menuItem5.Enabled = false;
			}
		}

		private void upButton_Click(object sender, System.EventArgs e)
		{
			if ( bugListView.SelectedIndices.Count != 0 )
			{
				if ( bugListView.SelectedIndices[0] != 0 )
					bugListView.Items[bugListView.SelectedIndices[0] - 1].Selected = true;
			}
		}

		private void downButton_Click(object sender, System.EventArgs e)
		{
			if ( bugListView.SelectedIndices.Count != 0 )
			{
				if ( bugListView.SelectedIndices[0] != bugListView.Items.Count - 1 )
				{
					bugListView.Items[bugListView.SelectedIndices[0] + 1].Selected = true;
				}
			}
		}

		private void refreshButton_Click(object sender, System.EventArgs e)
		{
			listBugs( );
		}

		private void submitterFilterComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			listBugs( );
		}

		private void ownerFilterComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			listBugs( );
		}

		private void statusFilterComboBox_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			listBugs( );
		}

		private void menuItem8_Click(object sender, System.EventArgs e)
		{		
			if ( menuItem8.Checked )
				menuItem8.Checked = false;
			else
				menuItem8.Checked = true;

			panel2.Visible = menuItem8.Checked;				
		}

		private void menuItem9_Click(object sender, System.EventArgs e)
		{
			if ( menuItem9.Checked )
				menuItem9.Checked = false;
			else
				menuItem9.Checked = true;

			bugListView.Visible = menuItem9.Checked;
		}

		private void menuItem10_Click(object sender, System.EventArgs e)
		{
			if ( menuItem10.Checked )
				menuItem10.Checked = false;
			else
				menuItem10.Checked = true;

			DetailPanel.Visible = menuItem10.Checked;
		}
	}
}
