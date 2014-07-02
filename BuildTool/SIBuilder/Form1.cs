using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.Win32;
using System.Data;
using System.Diagnostics;
using System.Threading;
using System.IO;
using System.Web;
using System.Web.Mail;
using System.Xml;

namespace SiBuilder
{
	/// <summary>
	/// Summary description for mainForm.
	/// </summary>
	public class mainForm : System.Windows.Forms.Form
	{
		private const string errorDialogCaption = "SIBuilder";
		private static string [] cmdline;
		private bool HasProductDataChanged = false;
		private bool ExitOnFinish;
		
		private string dtdName;
		private string productName = "";
		private string configFileName= "";

		private string optionProductText = "/PRODUCT:";
		private string optionConfigFileText = "/CONFIG:";
		private string optionAutoText = "/AUTO";
		
		private ArrayList cfProducts;
		private int iLogBox;
		
		private CSIBuilder.Builder builder;
		private CSIBuilder.LogDataWrittenDelegate logDataWrittenHandler;

		#region UI Member declarations and constructor
		private System.Windows.Forms.Button btnBuild;
		private System.Windows.Forms.StatusBar sbStatus;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.TextBox txtPackageScript;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox txtLogOut;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox txtBuildOut;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtBuild;
		private System.Windows.Forms.TextBox txtMinor;
		private System.Windows.Forms.TextBox txtMajor;
		private System.Windows.Forms.TextBox txtInstallShieldOutputFile;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.TextBox txtDevEnvFile;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.TextBox txtMailFrom;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.GroupBox grpNotificationOptions;
		private System.Windows.Forms.GroupBox grpBuildOptions;
		private System.Windows.Forms.GroupBox grpMainGroup;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.TextBox txtProductName;
		private System.Windows.Forms.TextBox txtMailRecipients;
		private System.Windows.Forms.TextBox txtMailSMTPServer;
		private System.Windows.Forms.GroupBox grpProjects;
		private System.Windows.Forms.ListView lvProjects;
		private System.Windows.Forms.ColumnHeader chSolution;
		private System.Windows.Forms.ColumnHeader chProject;
		private System.Windows.Forms.ColumnHeader chConfig;
		private System.Windows.Forms.ComboBox lvCmbBox;
		private System.Windows.Forms.TextBox lvTxtBox;
		private Point g_firstMouseClickPoint;
		private ListViewItem lvItem;
		private int g_subItemSelected = 0;
		private string subItemText;
		private System.Windows.Forms.OpenFileDialog dialogOpenFile;
		private System.Windows.Forms.Button btnRemoveProject;
		private System.Windows.Forms.Button btnAddProject;
		private System.Windows.Forms.GroupBox grpBuildProgress;
		private System.Windows.Forms.GroupBox grpProductIdentifier;
		private System.Windows.Forms.GroupBox grpSourceControlOptions;
		private System.Windows.Forms.Button btnAddSCItem;
		private System.Windows.Forms.Button btnDelSCItem;
		private System.Windows.Forms.ColumnHeader chSCProgram;
		private System.Windows.Forms.ColumnHeader chArguments;
		private System.Windows.Forms.ListView lvSCOptions;
		private System.Windows.Forms.ToolTip toolTip;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.LinkLabel lnkNewConfigFile;
		private System.Windows.Forms.LinkLabel lnkLoadConfigFile;
		private System.Windows.Forms.LinkLabel lnkSaveSelected;
		private System.Windows.Forms.LinkLabel lnkDeleteSelected;
		private System.Windows.Forms.LinkLabel lnkCreateNewProductEntry;
		private System.Windows.Forms.ComboBox cbProducts;
		private System.Windows.Forms.Label lblSelectProduct;
		private System.Windows.Forms.Button btn_AbortBuild;
		private System.Windows.Forms.RichTextBox txtBuilderProgress;
		private System.ComponentModel.IContainer components;

		public mainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Other UI components
			this.lvCmbBox = new System.Windows.Forms.ComboBox();
			this.lvCmbBox.Size = new System.Drawing.Size(0, 21);
			this.lvCmbBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.CmbKeyPress);
			this.lvCmbBox.SelectedIndexChanged += new System.EventHandler(this.CmbSelected);
			this.lvCmbBox.LostFocus += new System.EventHandler(this.CmbFocusOver);
			this.lvCmbBox.TabStop = false;

			this.lvTxtBox = new System.Windows.Forms.TextBox();
			this.lvTxtBox.Size = new System.Drawing.Size(0, 21);
			this.lvTxtBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TxtKeyPress);
			this.lvTxtBox.LostFocus += new System.EventHandler(this.TxtFocusOver);
			this.lvTxtBox.TabStop = false;
			this.lvTxtBox.BorderStyle = BorderStyle.FixedSingle;

			this.lvProjects.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lvProjects_MouseDown);
			this.lvProjects.Click += new System.EventHandler(this.lvProjects_Click);

			g_firstMouseClickPoint = new Point();

			//Add event handlers to UI elements which can be modified by the user
			//so that we know when the data has changed
			EventHandler productDataChangedHandler = new EventHandler (this.productDataChangedHandlerFunction);
			this.txtProductName.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtBuildOut.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtLogOut.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtPackageScript.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtBuildOut.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtInstallShieldOutputFile.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtDevEnvFile.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtMajor.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtMinor.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtBuild.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtMailFrom.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtMailSMTPServer.TextChanged += new EventHandler (productDataChangedHandler);
			this.txtMailRecipients.TextChanged += new EventHandler (productDataChangedHandler);
			this.lvProjects.TextChanged += new EventHandler (productDataChangedHandler);
			this.lvSCOptions.TextChanged += new EventHandler (productDataChangedHandler);

			dtdName = String.Concat (System.Environment.CurrentDirectory, "\\..\\..\\", "buildfile.dtd");

			//Initialize file open dialog box
			this.dialogOpenFile.Filter = "XML config files|*.xml";
			this.dialogOpenFile.CheckFileExists = false;
			this.dialogOpenFile.Multiselect = false;
			this.dialogOpenFile.Title = "Enter config filename";

			//Define Tool tip texts
			toolTip.SetToolTip(this.txtBuildOut, "The path(s) were the final build executable should be copied.\nEnter multiple paths using ';' as delimiter");

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
		#endregion

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.btnBuild = new System.Windows.Forms.Button();
			this.sbStatus = new System.Windows.Forms.StatusBar();
			this.grpMainGroup = new System.Windows.Forms.GroupBox();
			this.grpBuildOptions = new System.Windows.Forms.GroupBox();
			this.label8 = new System.Windows.Forms.Label();
			this.txtDevEnvFile = new System.Windows.Forms.TextBox();
			this.txtPackageScript = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.txtLogOut = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.txtBuildOut = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.txtInstallShieldOutputFile = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.txtBuild = new System.Windows.Forms.TextBox();
			this.txtMinor = new System.Windows.Forms.TextBox();
			this.txtMajor = new System.Windows.Forms.TextBox();
			this.label7 = new System.Windows.Forms.Label();
			this.grpSourceControlOptions = new System.Windows.Forms.GroupBox();
			this.btnDelSCItem = new System.Windows.Forms.Button();
			this.btnAddSCItem = new System.Windows.Forms.Button();
			this.lvSCOptions = new System.Windows.Forms.ListView();
			this.chSCProgram = new System.Windows.Forms.ColumnHeader();
			this.chArguments = new System.Windows.Forms.ColumnHeader();
			this.grpBuildProgress = new System.Windows.Forms.GroupBox();
			this.txtBuilderProgress = new System.Windows.Forms.RichTextBox();
			this.grpProjects = new System.Windows.Forms.GroupBox();
			this.btnRemoveProject = new System.Windows.Forms.Button();
			this.btnAddProject = new System.Windows.Forms.Button();
			this.lvProjects = new System.Windows.Forms.ListView();
			this.chSolution = new System.Windows.Forms.ColumnHeader();
			this.chProject = new System.Windows.Forms.ColumnHeader();
			this.chConfig = new System.Windows.Forms.ColumnHeader();
			this.grpProductIdentifier = new System.Windows.Forms.GroupBox();
			this.label12 = new System.Windows.Forms.Label();
			this.txtProductName = new System.Windows.Forms.TextBox();
			this.grpNotificationOptions = new System.Windows.Forms.GroupBox();
			this.label11 = new System.Windows.Forms.Label();
			this.txtMailRecipients = new System.Windows.Forms.TextBox();
			this.label10 = new System.Windows.Forms.Label();
			this.txtMailSMTPServer = new System.Windows.Forms.TextBox();
			this.label9 = new System.Windows.Forms.Label();
			this.txtMailFrom = new System.Windows.Forms.TextBox();
			this.dialogOpenFile = new System.Windows.Forms.OpenFileDialog();
			this.toolTip = new System.Windows.Forms.ToolTip(this.components);
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.lnkNewConfigFile = new System.Windows.Forms.LinkLabel();
			this.lnkDeleteSelected = new System.Windows.Forms.LinkLabel();
			this.lnkLoadConfigFile = new System.Windows.Forms.LinkLabel();
			this.lnkSaveSelected = new System.Windows.Forms.LinkLabel();
			this.lnkCreateNewProductEntry = new System.Windows.Forms.LinkLabel();
			this.cbProducts = new System.Windows.Forms.ComboBox();
			this.lblSelectProduct = new System.Windows.Forms.Label();
			this.btn_AbortBuild = new System.Windows.Forms.Button();
			this.grpMainGroup.SuspendLayout();
			this.grpBuildOptions.SuspendLayout();
			this.grpSourceControlOptions.SuspendLayout();
			this.grpBuildProgress.SuspendLayout();
			this.grpProjects.SuspendLayout();
			this.grpProductIdentifier.SuspendLayout();
			this.grpNotificationOptions.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btnBuild
			// 
			this.btnBuild.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.btnBuild.Location = new System.Drawing.Point(768, 608);
			this.btnBuild.Name = "btnBuild";
			this.btnBuild.Size = new System.Drawing.Size(96, 24);
			this.btnBuild.TabIndex = 6;
			this.btnBuild.Text = "BUILD";
			this.btnBuild.Click += new System.EventHandler(this.btnBuild_Click);
			// 
			// sbStatus
			// 
			this.sbStatus.Location = new System.Drawing.Point(0, 633);
			this.sbStatus.Name = "sbStatus";
			this.sbStatus.Size = new System.Drawing.Size(874, 22);
			this.sbStatus.TabIndex = 22;
			this.sbStatus.Text = "SIBuilder 2.5";
			// 
			// grpMainGroup
			// 
			this.grpMainGroup.Controls.Add(this.grpBuildOptions);
			this.grpMainGroup.Controls.Add(this.grpSourceControlOptions);
			this.grpMainGroup.Controls.Add(this.grpBuildProgress);
			this.grpMainGroup.Controls.Add(this.grpProjects);
			this.grpMainGroup.Controls.Add(this.grpProductIdentifier);
			this.grpMainGroup.Controls.Add(this.grpNotificationOptions);
			this.grpMainGroup.Location = new System.Drawing.Point(8, 56);
			this.grpMainGroup.Name = "grpMainGroup";
			this.grpMainGroup.Size = new System.Drawing.Size(864, 544);
			this.grpMainGroup.TabIndex = 27;
			this.grpMainGroup.TabStop = false;
			this.grpMainGroup.Text = "Product Details";
			// 
			// grpBuildOptions
			// 
			this.grpBuildOptions.Controls.Add(this.label8);
			this.grpBuildOptions.Controls.Add(this.txtDevEnvFile);
			this.grpBuildOptions.Controls.Add(this.txtPackageScript);
			this.grpBuildOptions.Controls.Add(this.label6);
			this.grpBuildOptions.Controls.Add(this.txtLogOut);
			this.grpBuildOptions.Controls.Add(this.label5);
			this.grpBuildOptions.Controls.Add(this.txtBuildOut);
			this.grpBuildOptions.Controls.Add(this.label4);
			this.grpBuildOptions.Controls.Add(this.txtInstallShieldOutputFile);
			this.grpBuildOptions.Controls.Add(this.label3);
			this.grpBuildOptions.Controls.Add(this.label2);
			this.grpBuildOptions.Controls.Add(this.label1);
			this.grpBuildOptions.Controls.Add(this.txtBuild);
			this.grpBuildOptions.Controls.Add(this.txtMinor);
			this.grpBuildOptions.Controls.Add(this.txtMajor);
			this.grpBuildOptions.Controls.Add(this.label7);
			this.grpBuildOptions.Location = new System.Drawing.Point(480, 16);
			this.grpBuildOptions.Name = "grpBuildOptions";
			this.grpBuildOptions.Size = new System.Drawing.Size(376, 184);
			this.grpBuildOptions.TabIndex = 29;
			this.grpBuildOptions.TabStop = false;
			this.grpBuildOptions.Text = "Build Options";
			// 
			// label8
			// 
			this.label8.Location = new System.Drawing.Point(40, 120);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(72, 16);
			this.label8.TabIndex = 28;
			this.label8.Text = "Devenv file";
			this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtDevEnvFile
			// 
			this.txtDevEnvFile.Location = new System.Drawing.Point(112, 120);
			this.txtDevEnvFile.Name = "txtDevEnvFile";
			this.txtDevEnvFile.Size = new System.Drawing.Size(256, 20);
			this.txtDevEnvFile.TabIndex = 9;
			this.txtDevEnvFile.Text = "";
			// 
			// txtPackageScript
			// 
			this.txtPackageScript.Location = new System.Drawing.Point(112, 72);
			this.txtPackageScript.Name = "txtPackageScript";
			this.txtPackageScript.Size = new System.Drawing.Size(256, 20);
			this.txtPackageScript.TabIndex = 7;
			this.txtPackageScript.Text = "";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(24, 48);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(88, 16);
			this.label6.TabIndex = 24;
			this.label6.Text = "Log Output Dir:";
			this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtLogOut
			// 
			this.txtLogOut.Location = new System.Drawing.Point(112, 48);
			this.txtLogOut.Name = "txtLogOut";
			this.txtLogOut.Size = new System.Drawing.Size(256, 20);
			this.txtLogOut.TabIndex = 6;
			this.txtLogOut.Text = "";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(24, 24);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(88, 16);
			this.label5.TabIndex = 22;
			this.label5.Text = "Build Output Dir:";
			this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtBuildOut
			// 
			this.txtBuildOut.Location = new System.Drawing.Point(112, 24);
			this.txtBuildOut.Name = "txtBuildOut";
			this.txtBuildOut.Size = new System.Drawing.Size(256, 20);
			this.txtBuildOut.TabIndex = 5;
			this.txtBuildOut.Text = "";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 96);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(104, 16);
			this.label4.TabIndex = 19;
			this.label4.Text = "InstallShield Output";
			this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtInstallShieldOutputFile
			// 
			this.txtInstallShieldOutputFile.Location = new System.Drawing.Point(112, 96);
			this.txtInstallShieldOutputFile.Name = "txtInstallShieldOutputFile";
			this.txtInstallShieldOutputFile.Size = new System.Drawing.Size(256, 20);
			this.txtInstallShieldOutputFile.TabIndex = 8;
			this.txtInstallShieldOutputFile.Text = "";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(280, 144);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(40, 16);
			this.label3.TabIndex = 17;
			this.label3.Text = "Build#:";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(176, 144);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(40, 16);
			this.label2.TabIndex = 16;
			this.label2.Text = "Minor:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(64, 144);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(48, 16);
			this.label1.TabIndex = 15;
			this.label1.Text = "Major:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtBuild
			// 
			this.txtBuild.Location = new System.Drawing.Point(320, 144);
			this.txtBuild.Name = "txtBuild";
			this.txtBuild.Size = new System.Drawing.Size(48, 20);
			this.txtBuild.TabIndex = 12;
			this.txtBuild.Text = "0";
			// 
			// txtMinor
			// 
			this.txtMinor.Location = new System.Drawing.Point(216, 144);
			this.txtMinor.Name = "txtMinor";
			this.txtMinor.Size = new System.Drawing.Size(48, 20);
			this.txtMinor.TabIndex = 11;
			this.txtMinor.Text = "0";
			// 
			// txtMajor
			// 
			this.txtMajor.Location = new System.Drawing.Point(112, 144);
			this.txtMajor.Name = "txtMajor";
			this.txtMajor.Size = new System.Drawing.Size(48, 20);
			this.txtMajor.TabIndex = 10;
			this.txtMajor.Text = "0";
			// 
			// label7
			// 
			this.label7.Location = new System.Drawing.Point(8, 72);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(104, 16);
			this.label7.TabIndex = 26;
			this.label7.Text = "InstallShield Script:";
			this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// grpSourceControlOptions
			// 
			this.grpSourceControlOptions.Controls.Add(this.btnDelSCItem);
			this.grpSourceControlOptions.Controls.Add(this.btnAddSCItem);
			this.grpSourceControlOptions.Controls.Add(this.lvSCOptions);
			this.grpSourceControlOptions.Location = new System.Drawing.Point(8, 312);
			this.grpSourceControlOptions.Name = "grpSourceControlOptions";
			this.grpSourceControlOptions.Size = new System.Drawing.Size(848, 224);
			this.grpSourceControlOptions.TabIndex = 101;
			this.grpSourceControlOptions.TabStop = false;
			this.grpSourceControlOptions.Text = "Pre-Build Events";
			// 
			// btnDelSCItem
			// 
			this.btnDelSCItem.Location = new System.Drawing.Point(816, 120);
			this.btnDelSCItem.Name = "btnDelSCItem";
			this.btnDelSCItem.Size = new System.Drawing.Size(24, 24);
			this.btnDelSCItem.TabIndex = 2;
			this.btnDelSCItem.Text = "-";
			this.btnDelSCItem.Click += new System.EventHandler(this.btnDelSCItem_Click);
			// 
			// btnAddSCItem
			// 
			this.btnAddSCItem.Location = new System.Drawing.Point(816, 88);
			this.btnAddSCItem.Name = "btnAddSCItem";
			this.btnAddSCItem.Size = new System.Drawing.Size(24, 24);
			this.btnAddSCItem.TabIndex = 1;
			this.btnAddSCItem.Text = "+";
			this.btnAddSCItem.Click += new System.EventHandler(this.btnAddSCItem_Click);
			// 
			// lvSCOptions
			// 
			this.lvSCOptions.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						  this.chSCProgram,
																						  this.chArguments});
			this.lvSCOptions.FullRowSelect = true;
			this.lvSCOptions.Location = new System.Drawing.Point(8, 16);
			this.lvSCOptions.MultiSelect = false;
			this.lvSCOptions.Name = "lvSCOptions";
			this.lvSCOptions.Size = new System.Drawing.Size(800, 200);
			this.lvSCOptions.TabIndex = 0;
			this.lvSCOptions.View = System.Windows.Forms.View.Details;
			this.lvSCOptions.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lvSCOptions_MouseDown);
			this.lvSCOptions.Click += new System.EventHandler(this.lvSCOptions_Click);
			// 
			// chSCProgram
			// 
			this.chSCProgram.Text = "Executable";
			this.chSCProgram.Width = 256;
			// 
			// chArguments
			// 
			this.chArguments.Text = "Arguments";
			this.chArguments.Width = 156;
			// 
			// grpBuildProgress
			// 
			this.grpBuildProgress.BackColor = System.Drawing.SystemColors.Control;
			this.grpBuildProgress.Controls.Add(this.txtBuilderProgress);
			this.grpBuildProgress.Location = new System.Drawing.Point(480, 200);
			this.grpBuildProgress.Name = "grpBuildProgress";
			this.grpBuildProgress.Size = new System.Drawing.Size(376, 104);
			this.grpBuildProgress.TabIndex = 100;
			this.grpBuildProgress.TabStop = false;
			this.grpBuildProgress.Text = "Build Log";
			// 
			// txtBuilderProgress
			// 
			this.txtBuilderProgress.Location = new System.Drawing.Point(3, 16);
			this.txtBuilderProgress.Name = "txtBuilderProgress";
			this.txtBuilderProgress.Size = new System.Drawing.Size(368, 80);
			this.txtBuilderProgress.TabIndex = 1;
			this.txtBuilderProgress.Text = "";
			// 
			// grpProjects
			// 
			this.grpProjects.Controls.Add(this.btnRemoveProject);
			this.grpProjects.Controls.Add(this.btnAddProject);
			this.grpProjects.Controls.Add(this.lvProjects);
			this.grpProjects.Location = new System.Drawing.Point(8, 176);
			this.grpProjects.Name = "grpProjects";
			this.grpProjects.Size = new System.Drawing.Size(464, 128);
			this.grpProjects.TabIndex = 32;
			this.grpProjects.TabStop = false;
			this.grpProjects.Text = "Projects to Build";
			// 
			// btnRemoveProject
			// 
			this.btnRemoveProject.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.btnRemoveProject.Location = new System.Drawing.Point(432, 80);
			this.btnRemoveProject.Name = "btnRemoveProject";
			this.btnRemoveProject.Size = new System.Drawing.Size(24, 24);
			this.btnRemoveProject.TabIndex = 17;
			this.btnRemoveProject.Text = "-";
			this.btnRemoveProject.Click += new System.EventHandler(this.btnRemoveProject_Click);
			// 
			// btnAddProject
			// 
			this.btnAddProject.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.btnAddProject.Location = new System.Drawing.Point(432, 48);
			this.btnAddProject.Name = "btnAddProject";
			this.btnAddProject.Size = new System.Drawing.Size(24, 24);
			this.btnAddProject.TabIndex = 16;
			this.btnAddProject.Text = "+";
			this.btnAddProject.Click += new System.EventHandler(this.btnAddProject_Click);
			// 
			// lvProjects
			// 
			this.lvProjects.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																						 this.chSolution,
																						 this.chProject,
																						 this.chConfig});
			this.lvProjects.FullRowSelect = true;
			this.lvProjects.LabelEdit = true;
			this.lvProjects.Location = new System.Drawing.Point(8, 16);
			this.lvProjects.Name = "lvProjects";
			this.lvProjects.Size = new System.Drawing.Size(416, 104);
			this.lvProjects.TabIndex = 0;
			this.lvProjects.View = System.Windows.Forms.View.Details;
			// 
			// chSolution
			// 
			this.chSolution.Text = "Solution";
			this.chSolution.Width = 232;
			// 
			// chProject
			// 
			this.chProject.Text = "Project";
			this.chProject.Width = 120;
			// 
			// chConfig
			// 
			this.chConfig.Text = "Config";
			// 
			// grpProductIdentifier
			// 
			this.grpProductIdentifier.Controls.Add(this.label12);
			this.grpProductIdentifier.Controls.Add(this.txtProductName);
			this.grpProductIdentifier.Location = new System.Drawing.Point(8, 16);
			this.grpProductIdentifier.Name = "grpProductIdentifier";
			this.grpProductIdentifier.Size = new System.Drawing.Size(464, 48);
			this.grpProductIdentifier.TabIndex = 31;
			this.grpProductIdentifier.TabStop = false;
			this.grpProductIdentifier.Text = "Product Identifier";
			// 
			// label12
			// 
			this.label12.Location = new System.Drawing.Point(8, 18);
			this.label12.Name = "label12";
			this.label12.Size = new System.Drawing.Size(104, 16);
			this.label12.TabIndex = 32;
			this.label12.Text = "Name";
			this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtProductName
			// 
			this.txtProductName.Location = new System.Drawing.Point(112, 18);
			this.txtProductName.Name = "txtProductName";
			this.txtProductName.Size = new System.Drawing.Size(312, 20);
			this.txtProductName.TabIndex = 4;
			this.txtProductName.Text = "";
			// 
			// grpNotificationOptions
			// 
			this.grpNotificationOptions.Controls.Add(this.label11);
			this.grpNotificationOptions.Controls.Add(this.txtMailRecipients);
			this.grpNotificationOptions.Controls.Add(this.label10);
			this.grpNotificationOptions.Controls.Add(this.txtMailSMTPServer);
			this.grpNotificationOptions.Controls.Add(this.label9);
			this.grpNotificationOptions.Controls.Add(this.txtMailFrom);
			this.grpNotificationOptions.Location = new System.Drawing.Point(8, 72);
			this.grpNotificationOptions.Name = "grpNotificationOptions";
			this.grpNotificationOptions.Size = new System.Drawing.Size(464, 104);
			this.grpNotificationOptions.TabIndex = 30;
			this.grpNotificationOptions.TabStop = false;
			this.grpNotificationOptions.Text = "Build Notification Options";
			// 
			// label11
			// 
			this.label11.Location = new System.Drawing.Point(8, 72);
			this.label11.Name = "label11";
			this.label11.Size = new System.Drawing.Size(72, 16);
			this.label11.TabIndex = 34;
			this.label11.Text = "Recipients";
			this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtMailRecipients
			// 
			this.txtMailRecipients.Location = new System.Drawing.Point(88, 72);
			this.txtMailRecipients.Name = "txtMailRecipients";
			this.txtMailRecipients.Size = new System.Drawing.Size(336, 20);
			this.txtMailRecipients.TabIndex = 15;
			this.txtMailRecipients.Text = "";
			// 
			// label10
			// 
			this.label10.Location = new System.Drawing.Point(8, 48);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(72, 16);
			this.label10.TabIndex = 32;
			this.label10.Text = "SMTP Server";
			this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtMailSMTPServer
			// 
			this.txtMailSMTPServer.Location = new System.Drawing.Point(88, 48);
			this.txtMailSMTPServer.Name = "txtMailSMTPServer";
			this.txtMailSMTPServer.Size = new System.Drawing.Size(336, 20);
			this.txtMailSMTPServer.TabIndex = 14;
			this.txtMailSMTPServer.Text = "";
			// 
			// label9
			// 
			this.label9.Location = new System.Drawing.Point(8, 24);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(72, 16);
			this.label9.TabIndex = 30;
			this.label9.Text = "Mail From";
			this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// txtMailFrom
			// 
			this.txtMailFrom.Location = new System.Drawing.Point(88, 24);
			this.txtMailFrom.Name = "txtMailFrom";
			this.txtMailFrom.Size = new System.Drawing.Size(336, 20);
			this.txtMailFrom.TabIndex = 13;
			this.txtMailFrom.Text = "";
			// 
			// groupBox1
			// 
			this.groupBox1.BackColor = System.Drawing.SystemColors.Info;
			this.groupBox1.Controls.Add(this.lnkNewConfigFile);
			this.groupBox1.Controls.Add(this.lnkDeleteSelected);
			this.groupBox1.Controls.Add(this.lnkLoadConfigFile);
			this.groupBox1.Controls.Add(this.lnkSaveSelected);
			this.groupBox1.Controls.Add(this.lnkCreateNewProductEntry);
			this.groupBox1.Controls.Add(this.cbProducts);
			this.groupBox1.Controls.Add(this.lblSelectProduct);
			this.groupBox1.Location = new System.Drawing.Point(0, 0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(872, 56);
			this.groupBox1.TabIndex = 34;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Control Panel";
			// 
			// lnkNewConfigFile
			// 
			this.lnkNewConfigFile.Location = new System.Drawing.Point(320, 24);
			this.lnkNewConfigFile.Name = "lnkNewConfigFile";
			this.lnkNewConfigFile.Size = new System.Drawing.Size(88, 16);
			this.lnkNewConfigFile.TabIndex = 40;
			this.lnkNewConfigFile.TabStop = true;
			this.lnkNewConfigFile.Text = "New Config File";
			this.lnkNewConfigFile.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkNewConfigFile_LinkClicked);
			// 
			// lnkDeleteSelected
			// 
			this.lnkDeleteSelected.Location = new System.Drawing.Point(512, 24);
			this.lnkDeleteSelected.Name = "lnkDeleteSelected";
			this.lnkDeleteSelected.Size = new System.Drawing.Size(104, 16);
			this.lnkDeleteSelected.TabIndex = 35;
			this.lnkDeleteSelected.TabStop = true;
			this.lnkDeleteSelected.Text = "Delete Product Info";
			this.lnkDeleteSelected.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.lnkDeleteSelected.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkDeleteSelected_LinkClicked);
			// 
			// lnkLoadConfigFile
			// 
			this.lnkLoadConfigFile.Location = new System.Drawing.Point(416, 16);
			this.lnkLoadConfigFile.Name = "lnkLoadConfigFile";
			this.lnkLoadConfigFile.Size = new System.Drawing.Size(88, 16);
			this.lnkLoadConfigFile.TabIndex = 39;
			this.lnkLoadConfigFile.TabStop = true;
			this.lnkLoadConfigFile.Text = "Load Config File";
			this.lnkLoadConfigFile.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkLoadConfigFile_LinkClicked);
			// 
			// lnkSaveSelected
			// 
			this.lnkSaveSelected.Location = new System.Drawing.Point(616, 16);
			this.lnkSaveSelected.Name = "lnkSaveSelected";
			this.lnkSaveSelected.Size = new System.Drawing.Size(104, 16);
			this.lnkSaveSelected.TabIndex = 36;
			this.lnkSaveSelected.TabStop = true;
			this.lnkSaveSelected.Text = "Save Product Info";
			this.lnkSaveSelected.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.lnkSaveSelected.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkSaveSelected_LinkClicked);
			// 
			// lnkCreateNewProductEntry
			// 
			this.lnkCreateNewProductEntry.Location = new System.Drawing.Point(728, 24);
			this.lnkCreateNewProductEntry.Name = "lnkCreateNewProductEntry";
			this.lnkCreateNewProductEntry.Size = new System.Drawing.Size(136, 16);
			this.lnkCreateNewProductEntry.TabIndex = 37;
			this.lnkCreateNewProductEntry.TabStop = true;
			this.lnkCreateNewProductEntry.Text = "Create new product entry";
			this.lnkCreateNewProductEntry.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.lnkCreateNewProductEntry.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.lnkCreateNewProductEntry_LinkClicked);
			// 
			// cbProducts
			// 
			this.cbProducts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbProducts.Location = new System.Drawing.Point(120, 24);
			this.cbProducts.Name = "cbProducts";
			this.cbProducts.Size = new System.Drawing.Size(184, 21);
			this.cbProducts.TabIndex = 34;
			this.cbProducts.SelectedIndexChanged += new System.EventHandler(this.cbProducts_SelectedIndexChanged);
			// 
			// lblSelectProduct
			// 
			this.lblSelectProduct.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lblSelectProduct.Location = new System.Drawing.Point(16, 26);
			this.lblSelectProduct.Name = "lblSelectProduct";
			this.lblSelectProduct.Size = new System.Drawing.Size(96, 16);
			this.lblSelectProduct.TabIndex = 38;
			this.lblSelectProduct.Text = "Select a product :";
			// 
			// btn_AbortBuild
			// 
			this.btn_AbortBuild.Enabled = false;
			this.btn_AbortBuild.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.btn_AbortBuild.Location = new System.Drawing.Point(648, 608);
			this.btn_AbortBuild.Name = "btn_AbortBuild";
			this.btn_AbortBuild.Size = new System.Drawing.Size(96, 24);
			this.btn_AbortBuild.TabIndex = 35;
			this.btn_AbortBuild.TabStop = false;
			this.btn_AbortBuild.Text = "ABORT";
			this.btn_AbortBuild.Click += new System.EventHandler(this.btn_AbortBuild_Click);
			// 
			// mainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(874, 655);
			this.Controls.Add(this.btn_AbortBuild);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.grpMainGroup);
			this.Controls.Add(this.sbStatus);
			this.Controls.Add(this.btnBuild);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.Name = "mainForm";
			this.Text = "SIBuilder 2.0";
			this.Load += new System.EventHandler(this.mainForm_Load);
			this.grpMainGroup.ResumeLayout(false);
			this.grpBuildOptions.ResumeLayout(false);
			this.grpSourceControlOptions.ResumeLayout(false);
			this.grpBuildProgress.ResumeLayout(false);
			this.grpProjects.ResumeLayout(false);
			this.grpProductIdentifier.ResumeLayout(false);
			this.grpNotificationOptions.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		#region UI Event handlers
		/// <summary>
		/// Event handler for Build event
		/// Creates a build thread and starts it
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnBuild_Click(object sender, System.EventArgs e)
		{
			this.grpMainGroup.Enabled = false;
			this.groupBox1.Enabled = false;
			Build();
			this.grpMainGroup.Enabled = true;
			this.groupBox1.Enabled = true;
		}

		/// <summary>
		/// Event handler to make the log listbox scroll to the bottom everytime it is changed
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>

		#endregion

		private class SolutionInfo
		{
			public string SolutionPath;
			public string ProjectName;
			public string BuildConfig;

			public SolutionInfo()
			{
				this.SolutionPath = "";
				this.ProjectName = "";
				this.BuildConfig = "";
			}

			public SolutionInfo (string sPath, string pName, string bConfig)
			{
				this.SolutionPath = sPath;
				this.ProjectName = pName;
				this.BuildConfig = bConfig;
			}
		}

		private class SourceControlInfo
		{
			public string Executable;
			public string Arguments;

			public SourceControlInfo()
			{
				this.Executable = "";
				this.Arguments = "";
			}

			public SourceControlInfo (string _exe, string _args)
			{
				this.Executable = _exe;
				this.Arguments = _args;
			}
		}

		private class ProductDetails
		{
			public string Name;
			public string MajorVersion;
			public string MinorVersion;
			public string BuildNumber;
			public string BuildsLocation;
			public string LogsLocation;
			public string DevenvFile;
			public string InstallShieldScript;
			public string InstallShieldOutput;
			public string MailRecipients;
			public string MailSenderAddress;
			public string MailServerAddress;
			public ArrayList SolutionInfo;
			public ArrayList SourceControlInfo;

			public ProductDetails()
			{
				Name = "";
				MajorVersion = "";
				MinorVersion = "";
				BuildNumber = "";
				BuildsLocation = "";
				LogsLocation = "";
				DevenvFile = "";
				InstallShieldScript = "";
				InstallShieldOutput = "";
				MailRecipients = "";
				MailSenderAddress = "";
				MailServerAddress = "";
				SolutionInfo = null;
				SourceControlInfo = null;
			}
		}

		private void Build()
		{
			sbStatus.Text = "Building ..";
			btnBuild.Enabled = false;
			this.btn_AbortBuild.Enabled = true;
			ThreadStart threadStart = new ThreadStart (WThreadFunction);
			Thread WorkerThread = new Thread(threadStart);
			WorkerThread.Start();
		}

		private void WThreadFunction()
		{
			//Clear last build information from the registry
			//This is for 3rd party apps
			Microsoft.Win32.RegistryKey regKey = Microsoft.Win32.Registry.LocalMachine.CreateSubKey ("Software\\SiBuilder");
			regKey.SetValue ("OUTPUT_FILE", "");
			regKey.SetValue ("FINAL_NAME", "");

			CSIBuilder.MailInfo mi;
			mi.MailRecipients = this.txtMailRecipients.Text;
			mi.MailSender = this.txtMailFrom.Text;
			mi.SmtpServer = this.txtMailSMTPServer.Text;

			CSIBuilder.SIBuilderInfo sib = new CSIBuilder.SIBuilderInfo();
			sib.ProjectName = this.txtProductName.Text;
			sib.Projects = this.lvProjects.Items;
			sib.buildNumber = Int32.Parse (this.txtBuild.Text);
			sib.DevenvLocation = this.txtDevEnvFile.Text;
			sib.LogDir = this.txtLogOut.Text;

			sib.LogOutputStream = null;

			sib.majorVersion = Int32.Parse(this.txtMajor.Text);
			sib.minorVersion = Int32.Parse(this.txtMinor.Text);
			sib.OutputDir = this.txtBuildOut.Text;
			sib.PackageOutputFile = this.txtInstallShieldOutputFile.Text;
			sib.PackageScript = this.txtPackageScript.Text;

			builder = new CSIBuilder.Builder (sib, mi, this.lvSCOptions.Items);
			logDataWrittenHandler = new CSIBuilder.LogDataWrittenDelegate(onLogDataWritten);
			builder.OnLogDataWritten += logDataWrittenHandler;
			builder.StartInSameThread();

			btnBuild.Enabled = true;
			this.btn_AbortBuild.Enabled = false;
			if (builder.wasBuildAborted)
				sbStatus.Text = "Process Aborted!";
			else
			{
				sbStatus.Text = "Process Complete!";

				//Update the buildnumber in the config file
				this.txtBuild.Text = Convert.ToString (Int32.Parse (this.txtBuild.Text) + 1);
				this.SaveCurrentProductDataToConfigFile (null);

				//Add a string to the registry under HKLM\Software\SIBuilder:OUTPUT_FILE
				//3rd party apps can pick this up to do whatever they want
				//Microsoft.Win32.RegistryKey regKey = Microsoft.Win32.Registry.LocalMachine.CreateSubKey ("Software\\SiBuilder");
				regKey.SetValue ("OUTPUT_FILE", builder.OutputFilePath);
				regKey.SetValue ("FINAL_NAME", builder.FinalName);
			}

			if (this.ExitOnFinish)
				Application.Exit();
		}

		private void onLogDataWritten(String text)
		{
			this.txtBuilderProgress.AppendText (text + "\r\n");
		}

		[STAThread]
		static void Main(string[] args) 
		{
			if (args.Length > 0)
			{
				cmdline = new String [args.Length];
				args.CopyTo (cmdline, 0);
			}
			else
				cmdline = new String[1] {""};

			Application.Run(new mainForm());
		}

		private void mainForm_Load(object sender, System.EventArgs e)
		{
			ExitOnFinish = false;
			ProductDetails productDetails = null;
			
			#region Parse command line options
			if (cmdline.Length >= 1)
			{
				for (int i=0; i< cmdline.Length; i++)
				{
					if (cmdline[i].ToUpper().StartsWith (optionConfigFileText))
					{
						configFileName = cmdline[i].Substring (this.optionConfigFileText.Length);
					}

					if (cmdline[i].ToUpper().StartsWith (optionProductText))
					{
						productName = cmdline[i].Substring (optionProductText.Length);
					}

					if (cmdline[i].ToUpper().StartsWith (optionAutoText))
					{
						ExitOnFinish = true;
					}
				}
			}
			#endregion

			if (configFileName != "")
			{
				if (!LoadProductNames())
				{
					return;
				}

				PopulateProductsList();
			}

			#region Load product information if one was specified on the command line
			if (productName != "")
			{
				if (this.cfProducts.Contains (productName))
				{
					LoadDetailsFromConfigFile (configFileName, productName, out productDetails);
					if (productDetails != null)
					{
						DisplayProductInformation (productDetails);
					}
				}
				else
				{
					MessageBox.Show (String.Concat ("The specified product name \"", productName, "\" could not be found in the config file!"), errorDialogCaption);
					this.productName = "";
				}
			}

			#endregion

			//Check if we want to start (and end) automatically
			if ((productName !="") && (ExitOnFinish))
				Build();
		}

		private bool LoadProductNames()
		{
			ProductDetails productDetails = null;

			if (!LoadDetailsFromConfigFile (configFileName, "", out productDetails))
			{
				MessageBox.Show (String.Concat ("There was a problem loading the configuration file:\n", this.configFileName), errorDialogCaption);
				return false;
			}
			return true;
		}

		private void PopulateProductsList()
		{
			cbProducts.Items.Clear();

			//Populate the products combo-box with product names
			for (int i=0; i< this.cfProducts.Count; i++)
			{
				cbProducts.Items.Add (cfProducts[i].ToString());
			}
		}


		#region Methods for handling editing in the projects listview box

		public void lvSCOptions_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			lvItem = lvSCOptions.GetItemAt(e.X , e.Y);
			g_firstMouseClickPoint.X = e.X ;
			g_firstMouseClickPoint.Y = e.Y ;
		}

		public void lvProjects_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			lvItem = lvProjects.GetItemAt(e.X , e.Y);
			g_firstMouseClickPoint.X = e.X ;
			g_firstMouseClickPoint.Y = e.Y ;
		}

		private void CmbKeyPress(object sender , System.Windows.Forms.KeyPressEventArgs e)
		{
			if ((e.KeyChar == 13) || (e.KeyChar == 27))
			{
				this.lvCmbBox.Hide();
			}
		}

		private void CmbSelected(object sender , System.EventArgs e)
		{
			int sel = this.lvCmbBox.SelectedIndex;
			if (sel >= 0)
			{
				string itemSel = lvCmbBox.Items[sel].ToString();
				this.lvItem.SubItems[g_subItemSelected].Text = itemSel;
			}
		}

		private void CmbFocusOver(object sender , System.EventArgs e)
		{
			if (!lvCmbBox.Focused)
			{
				// set the text for the value field
				lvItem.SubItems[g_subItemSelected].Text = lvCmbBox.Text;

				lvCmbBox.Hide();
				lvCmbBox.Items.Clear();
			}
		}

		private void TxtKeyPress(object sender , System.Windows.Forms.KeyPressEventArgs e)
		{
			if ((e.KeyChar == 13) || (e.KeyChar == 27))
			{
				this.lvTxtBox.Hide();
			}
		}

		private void TxtFocusOver(object sender , System.EventArgs e)
		{
			if (!lvTxtBox.Focused)
			{
				// set the text for the value field
				lvItem.SubItems[g_subItemSelected].Text = lvTxtBox.Text;

				lvTxtBox.Hide();
				lvTxtBox.Clear();
			}
		}

		public  void lvProjects_Click(object sender, System.EventArgs e)
		{
			if (lvTxtBox.Parent != null)
				lvTxtBox.Parent.Controls.Remove (lvTxtBox);
			this.lvProjects.Controls.Add (lvTxtBox);
			
			// Check the subitem clicked
			int colStartX = 0; 
			int colEndX = lvProjects.Columns[0].Width;

			for (int i = 0; i < lvProjects.Columns.Count; i++)
			{
				if ((g_firstMouseClickPoint.X > colStartX) && (g_firstMouseClickPoint.X < colEndX))
				{
					g_subItemSelected = i;
					break; 
				}
				
				colStartX = colEndX;

				if (i < (lvProjects.Columns.Count - 1))
				{
					colEndX += lvProjects.Columns[i + 1].Width;
				}
			}

			subItemText = lvItem.SubItems[g_subItemSelected].Text;

			string colName = lvProjects.Columns[g_subItemSelected].Text;

			//if (g_subItemSelected == 0)  //Solution File
		{
			lvTxtBox.Clear();
			lvTxtBox.Width = colEndX - colStartX;
			lvTxtBox.Height = lvItem.Bounds.Bottom - lvItem.Bounds.Top;
			lvTxtBox.Location = new System.Drawing.Point(colStartX , lvItem.Bounds.Y);
				
			lvTxtBox.Show();

			lvTxtBox.Text = subItemText;
			lvTxtBox.SelectAll();
			lvTxtBox.Focus();
		}
		}


		public void lvSCOptions_Click(object sender, System.EventArgs e)
		{
			if (lvTxtBox.Parent != null)
				lvTxtBox.Parent.Controls.Remove (lvTxtBox);
			this.lvSCOptions.Controls.Add (lvTxtBox);
			// Check the subitem clicked
			int colStartX = 0; 
			int colEndX = lvSCOptions.Columns[0].Width;

			for (int i = 0; i < lvSCOptions.Columns.Count; i++)
			{
				if ((g_firstMouseClickPoint.X > colStartX) && (g_firstMouseClickPoint.X < colEndX))
				{
					g_subItemSelected = i;
					break; 
				}
				
				colStartX = colEndX;

				if (i < (lvSCOptions.Columns.Count - 1))
				{
					colEndX += lvSCOptions.Columns[i + 1].Width;
				}
			}

			subItemText = lvItem.SubItems[g_subItemSelected].Text;

			string colName = lvSCOptions.Columns[g_subItemSelected].Text;

			//if (g_subItemSelected == 0)  //Solution File
		{
			lvTxtBox.Clear();
			lvTxtBox.Width = colEndX - colStartX;
			lvTxtBox.Height = lvItem.Bounds.Bottom - lvItem.Bounds.Top;
			lvTxtBox.Location = new System.Drawing.Point(colStartX , lvItem.Bounds.Y);
				
			lvTxtBox.Show();

			lvTxtBox.Text = subItemText;
			lvTxtBox.SelectAll();
			lvTxtBox.Focus();
		}
		}

		#endregion


		private void DisplayProductInformation(ProductDetails productDetails)
		{
			this.grpMainGroup.Enabled = true;
			this.cbProducts.Text = productDetails.Name;
			this.txtProductName.Text = productDetails.Name;
			this.txtBuildOut.Text = productDetails.BuildsLocation;
			this.txtInstallShieldOutputFile.Text = productDetails.InstallShieldOutput;
			this.txtLogOut.Text = productDetails.LogsLocation;
			this.txtPackageScript.Text = productDetails.InstallShieldScript;
			this.txtDevEnvFile.Text = productDetails.DevenvFile;

			this.txtMailFrom.Text = productDetails.MailSenderAddress;
			this.txtMailRecipients.Text = productDetails.MailRecipients;
			this.txtMailSMTPServer.Text = productDetails.MailServerAddress;

			this.txtMajor.Text = productDetails.MajorVersion;
			this.txtMinor.Text = productDetails.MinorVersion;
			this.txtBuild.Text = productDetails.BuildNumber;
			
			this.lvProjects.Items.Clear();
			if (productDetails.SolutionInfo != null)
			{
				for (int i=0; i< productDetails.SolutionInfo.Count; i++)
				{
					SolutionInfo solutionInfo = productDetails.SolutionInfo[i] as SolutionInfo;
					ListViewItem lviProjectItem = new ListViewItem (solutionInfo.SolutionPath);
					lviProjectItem.SubItems.Add (solutionInfo.ProjectName);
					lviProjectItem.SubItems.Add (solutionInfo.BuildConfig);
					this.lvProjects.Items.Add (lviProjectItem);
				}
			}

			this.lvSCOptions.Items.Clear();
			if (productDetails.SourceControlInfo != null)
			{
				for (int i=0; i< productDetails.SourceControlInfo.Count; i++)
				{
					SourceControlInfo scInfo = productDetails.SourceControlInfo[i] as SourceControlInfo;
					ListViewItem lviSCItem = new ListViewItem (scInfo.Executable);
					lviSCItem.SubItems.Add (scInfo.Arguments);
					this.lvSCOptions.Items.Add (lviSCItem);
				}
			}

			HasProductDataChanged= false;
		}
	
		/// <summary>
		/// LoadDetailsFromConfigFile:
		/// Loads details from the config file. It populates the global variable cfProducts with
		/// product names. If the productName parameter is not empty and contains a valid product
		/// name, then the productDetails parameter is populated with the build information for
		/// the specified product.
		/// </summary>
		/// <param name="configFileName">[required, in] The path and filename of the config file</param>
		/// <param name="productName">[optional, in] The name of the product to be loaded</param>
		/// <param name="productDetails">[optional, out] Required only if productName is specified</param>
		/// <returns>A boolean value indicating if product names were successfully added to the
		/// cfProducts variable</returns>
		private bool LoadDetailsFromConfigFile (string configFileName, string productName, out ProductDetails productDetails)
		{
			productDetails = null;
			
			//Local variables
			bool hasMoreProducts = true;

			//Erase any previously loaded information
			cfProducts = null;
			
			XmlTextReader xmlReader = null;
			XmlValidatingReader validator = null;
			System.Xml.XPath.XPathDocument doc;
			System.Xml.XPath.XPathNavigator nav;

			try
			{
				xmlReader = new XmlTextReader(configFileName);
				validator = new XmlValidatingReader(xmlReader);
				validator.ValidationType = ValidationType.DTD;
				//Any errors will cause an XmlException to be thrown
				while (validator.Read())
				{
				}
				
				doc = new System.Xml.XPath.XPathDocument(configFileName);
				nav = doc.CreateNavigator();
			}

			catch (Exception)
			{
				if (xmlReader != null)
					xmlReader.Close();
				if (validator != null)
					validator.Close();
				return false;
			}

			#region Load PRODUCT names (and product details if specified) from the XML configuration file
			//Move to the SIBuilder Tag
			nav.MoveToFirstChild();
			if (!nav.HasChildren)
			{
				//MessageBox.Show ("The configuration file does not contain any products that can be built", errorDialogCaption);
				xmlReader.Close();
				validator.Close();
				return false;				
			}
			
			this.cfProducts = new ArrayList();
			//Move to the first PRODUCT in the configuration file
			nav.MoveToFirstChild ();
			hasMoreProducts = true;

			while (hasMoreProducts)
			{
				this.cfProducts.Add (nav.GetAttribute ("Name", ""));
				
				//Check if the Name attribute was returned; if not declare config file corrupt
				if (this.cfProducts[this.cfProducts.Count - 1].ToString() == String.Empty)
				{
					this.cfProducts = null;
					xmlReader.Close();
					validator.Close();
					return false;
				}

				#region Load details of this product if requested by the user
				//Check if the user wants details about the current PRODUCT
				if ((productName != "") && (cfProducts[cfProducts.Count -1].ToString() == productName))
				{
					productDetails = new ProductDetails();
					//Add the product name
					productDetails.Name = productName;
					//Get the Versioning information
					productDetails.MajorVersion = nav.GetAttribute ("MajorVersion", "");
					productDetails.MinorVersion = nav.GetAttribute ("MinorVersion", "");
					productDetails.BuildNumber = nav.GetAttribute ("BuildNumber", "");
					//Get the other product details
					if (nav.HasChildren)
					{
						nav.MoveToFirstChild();
						bool moreProductDetails = true;
						bool moreChildren = true;

						while (moreProductDetails)
						{
							switch (nav.Name.ToUpper())
							{
								case "SOURCECONTROL":
									//Move into the children of the SOURCECONTROL node
									moreChildren= nav.MoveToFirstChild();
									if (!moreChildren)
										break;
									while (moreChildren)
									{
										if (productDetails.SourceControlInfo== null)
											productDetails.SourceControlInfo = new ArrayList();
 
										SourceControlInfo scInfo = new SourceControlInfo();
										scInfo.Executable = nav.Value;
										scInfo.Arguments = nav.GetAttribute ("Args", "");
										productDetails.SourceControlInfo.Add (scInfo);
										moreChildren = nav.MoveToNext();
									}
									//Move back to the SOURCECONTROL node
									nav.MoveToParent();
									break;
								case "PATHS":
									//Move into the children of the PATHS node
									moreChildren = nav.MoveToFirstChild();
									if (!moreChildren)
										break;
									while (moreChildren)
									{
										switch (nav.Name.ToUpper())
										{
											case "BUILDS_LOCATION":
												productDetails.BuildsLocation = nav.Value;
												break;
											case "LOGS_LOCATION":
												productDetails.LogsLocation = nav.Value;
												break;
											case "DEVENV_FILE":
												productDetails.DevenvFile = nav.Value;
												break;
											case "INSTALLSHIELD_SCRIPT":
												productDetails.InstallShieldScript = nav.Value;
												break;
											case "INSTALLSHIELD_OUTPUT":
												productDetails.InstallShieldOutput = nav.Value;
												break;
											default:
												break;
										}
										moreChildren = nav.MoveToNext();
									}
									//Move back to the PATHS node
									nav.MoveToParent();
									break;

								case "BUILDMAIL":
									//Move to the children of the BUILDMAIL node
									moreChildren = nav.MoveToFirstChild();
									if (!moreChildren)
										break;
									while (moreChildren)
									{
										switch (nav.Name.ToUpper())
										{
											case "RECIPIENTS":
												productDetails.MailRecipients = nav.Value;
												break;
											case "MAILSENDER":
												productDetails.MailSenderAddress = nav.GetAttribute ("Address", "");
												productDetails.MailServerAddress = nav.GetAttribute ("SmtpServer", "");
												break;
											default:
												break;
										}
										moreChildren = nav.MoveToNext();
									}
									//Move back to the BUILDMAIL node
									nav.MoveToParent();
									break;

								case "BUILD_SOLUTIONS":
									//Move to the children of the BUILD_SOLUTIONS node
									moreChildren = nav.MoveToFirstChild();
									if (!moreChildren)
										break;
									while (moreChildren)
									{
										switch (nav.Name.ToUpper())
										{
											case "SOLUTION":
												if (productDetails.SolutionInfo == null)
													productDetails.SolutionInfo = new ArrayList();

												SolutionInfo solInfo = new SolutionInfo();
												solInfo.SolutionPath = nav.Value;
												solInfo.ProjectName = nav.GetAttribute ("Project", "");
												solInfo.BuildConfig = nav.GetAttribute ("BuildConfig", "");
												productDetails.SolutionInfo.Add (solInfo);

												break;
											default:
												break;
										}
										moreChildren = nav.MoveToNext();
									}
									//Move back to the BUILD_SOLUTIONS node
									nav.MoveToParent();
									break;
								default:
									break;
							}
							moreProductDetails = nav.MoveToNext();
						}

					}
				}
				#endregion
				
				hasMoreProducts = nav.MoveToNext();
			}
			#endregion
			
			
			xmlReader.Close();
			validator.Close();
			return true;
		}

		private void btn_AbortBuild_Click(object sender, System.EventArgs e)
		{
			if (builder != null)
				builder.Stop();
			
			this.btn_AbortBuild.Enabled = false;
			this.btnBuild.Enabled = true;
			this.grpMainGroup.Enabled = true;
			this.groupBox1.Enabled = true;
			
			sbStatus.Text = "Aborted.";
		}

		private void cbProducts_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			DialogResult dResult;
			//Check if the previous product data was modified. If it was
			//then prompt the user to save the changes
			if (this.HasProductDataChanged)
			{
				dResult = MessageBox.Show (String.Concat ("The data for product ", this.txtProductName.Text, " was modified.\n Would you like to save the changes?"), "SIBuilder", MessageBoxButtons.YesNoCancel);
				switch (dResult)
				{
					case DialogResult.Cancel:
						return;
					case DialogResult.No:
						break;
					case DialogResult.Yes:
						SaveCurrentProductDataToConfigFile(null);
						break;
				}
			}
			
			HasProductDataChanged= false;

			ProductDetails pDetails = null;
			this.LoadDetailsFromConfigFile (this.configFileName, cbProducts.SelectedItem.ToString(), out pDetails);
			if (pDetails != null)
			{
				this.DisplayProductInformation (pDetails);
			}
			else
			{
				MessageBox.Show (String.Concat ("Error loading information for project:\n", this.cbProducts.SelectedItem.ToString()), "SIBuilder");
			}
			
		}

		public void productDataChangedHandlerFunction(Object sender, EventArgs e)
		{
			this.HasProductDataChanged = true;
		}

		private bool DeleteProductDataFromConfigFile(string configFile, string productName)
		{
			bool foundNodeToDelete = false;
			XmlTextReader reader = new XmlTextReader (configFile);
			reader.Read();
			XmlDocument xmlDocument = new XmlDocument();
			xmlDocument.Load (reader);
			reader.Close();

			if (xmlDocument.HasChildNodes)
			{
				System.Xml.XmlNodeList nodeList = xmlDocument.GetElementsByTagName ("PRODUCT");
				for (int i=0; i< nodeList.Count; i++)
				{
					XmlNode xmlNode = nodeList.Item(i);
					for (int z=0; z< xmlNode.Attributes.Count; z++)
					{
						if (xmlNode.Attributes.Item(z).Name == "Name")
						{
							if (xmlNode.Attributes.Item(z).Value == this.txtProductName.Text.Trim())
							{
								xmlDocument.DocumentElement.RemoveChild (xmlNode);
								foundNodeToDelete = true;
							}
						}
					}
				}
			}

			if (foundNodeToDelete)
			{
				XmlTextWriter writer = new XmlTextWriter (configFile, null);
				writer.Formatting = Formatting.Indented;
				xmlDocument.WriteTo (writer);
				writer.Flush();
				writer.Close();
			}

			return foundNodeToDelete;
		}

		private void CreateEmptyConfigFile (string newFileName)
		{
			XmlTextWriter writer = new XmlTextWriter (newFileName, System.Text.Encoding.UTF8);
			writer.Formatting = Formatting.Indented;

			writer.WriteStartDocument (false);
			writer.WriteDocType ("SIBUILDER", null, this.dtdName, null);
			writer.WriteStartElement ("SIBUILDER");
			writer.WriteEndElement ();
			writer.WriteEndDocument ();
			writer.Flush();
			writer.Close();
		}

		private bool SaveCurrentProductDataToConfigFile(string alternateConfigFile)
		{
			string _configFile = ((alternateConfigFile != null)&&(alternateConfigFile != "")) ? alternateConfigFile : this.configFileName;
			
			//check if config file is read-only
			FileAttributes fAttribs = System.IO.File.GetAttributes (_configFile);
			if (FileAttributes.ReadOnly.CompareTo (fAttribs & FileAttributes.ReadOnly) == 0)
			{
					System.IO.File.SetAttributes (_configFile, fAttribs & (~FileAttributes.ReadOnly));
			}

			//If current product already exists in the config file, then remove the old data
			DeleteProductDataFromConfigFile (_configFile, this.txtProductName.Text);

			//Now save the current product information
			XmlTextReader reader = new XmlTextReader (_configFile);
			reader.Read();
			XmlDocument xmlDocument = new XmlDocument();
			xmlDocument.Load (reader);
			reader.Close();

			//Create the product Node and add its attributes
			XmlNode newProductNode = xmlDocument.CreateNode (XmlNodeType.Element, "PRODUCT", "");
			XmlAttribute productNameAttrib = newProductNode.Attributes.Append (xmlDocument.CreateAttribute ("Name"));
			productNameAttrib.Value = this.txtProductName.Text;
			XmlAttribute majorVersionAttrib = newProductNode.Attributes.Append ((xmlDocument.CreateAttribute ("MajorVersion")));
			majorVersionAttrib.Value = this.txtMajor.Text;
			XmlAttribute minorVersionAttrib = newProductNode.Attributes.Append ((xmlDocument.CreateAttribute ("MinorVersion")));
			minorVersionAttrib.Value = this.txtMinor.Text;
			XmlAttribute buildNumberAttrib = newProductNode.Attributes.Append ((xmlDocument.CreateAttribute ("BuildNumber")));
			buildNumberAttrib.Value = this.txtBuild.Text;

			//Create the SOURCECONTROL node and its children and add it to PRODUCT node
			XmlNode scNode = newProductNode.AppendChild (xmlDocument.CreateElement ("SOURCECONTROL"));
			for (int scs=0; scs < this.lvSCOptions.Items.Count; scs++)
			{
				XmlNode executeNode = scNode.AppendChild (xmlDocument.CreateElement ("EXECUTE"));
				executeNode.InnerText = this.lvSCOptions.Items[scs].SubItems[0].Text;
				XmlAttribute argsAttrib = executeNode.Attributes.Append (xmlDocument.CreateAttribute ("Args"));
				argsAttrib.Value = this.lvSCOptions.Items[scs].SubItems[1].Text;
			}

			//Create the PATHS node and its children and add it to the PRODUCT node
			XmlNode pathsNode = newProductNode.AppendChild (xmlDocument.CreateElement ("PATHS"));
			XmlNode buildsLocationNode = pathsNode.AppendChild (xmlDocument.CreateElement ("BUILDS_LOCATION"));
			buildsLocationNode.InnerText = this.txtBuildOut.Text;
			XmlNode logsLocationNode = pathsNode.AppendChild (xmlDocument.CreateElement ("LOGS_LOCATION"));
			logsLocationNode.InnerText = this.txtLogOut.Text;
			XmlNode devenvFileNode = pathsNode.AppendChild (xmlDocument.CreateElement ("DEVENV_FILE"));
			devenvFileNode.InnerText = this.txtDevEnvFile.Text;
			XmlNode installShieldScriptNode = pathsNode.AppendChild (xmlDocument.CreateElement ("INSTALLSHIELD_SCRIPT"));
			installShieldScriptNode.InnerText = this.txtPackageScript.Text;
			XmlNode installShieldOutputNode = pathsNode.AppendChild (xmlDocument.CreateElement ("INSTALLSHIELD_OUTPUT"));
			installShieldOutputNode.InnerText = this.txtInstallShieldOutputFile.Text;

			//Create the BUILDMAIL node and its children and add it to the PRODUCT node
			XmlNode buildMailNode = newProductNode.AppendChild (xmlDocument.CreateElement("BUILDMAIL"));
			XmlNode recipientsNode = buildMailNode.AppendChild (xmlDocument.CreateElement ("RECIPIENTS"));
			recipientsNode.InnerText = this.txtMailRecipients.Text;
			XmlNode mailSenderNode = buildMailNode.AppendChild (xmlDocument.CreateElement ("MAILSENDER"));
			XmlAttribute senderAddressAttrib = mailSenderNode.Attributes.Append (xmlDocument.CreateAttribute ("Address"));
			senderAddressAttrib.Value = this.txtMailFrom.Text;
			XmlAttribute senderServerAttrib = mailSenderNode.Attributes.Append (xmlDocument.CreateAttribute ("SmtpServer"));
			senderServerAttrib.Value = this.txtMailSMTPServer.Text;

			//Create the BUILD_SOLUTIONS node and its children and add it to the PRODUCT node
			XmlNode buildSolutionsNode = newProductNode.AppendChild (xmlDocument.CreateElement ("BUILD_SOLUTIONS"));
			for (int sols = 0; sols < this.lvProjects.Items.Count; sols++)
			{
				XmlNode solutionNode = buildSolutionsNode.AppendChild (xmlDocument.CreateElement ("SOLUTION"));
				solutionNode.InnerText = this.lvProjects.Items[sols].SubItems[0].Text;
				XmlAttribute projectAttrib = solutionNode.Attributes.Append (xmlDocument.CreateAttribute ("Project"));
				projectAttrib.Value = this.lvProjects.Items[sols].SubItems[1].Text;
				XmlAttribute buildConfigAttrib = solutionNode.Attributes.Append (xmlDocument.CreateAttribute ("BuildConfig"));
				buildConfigAttrib.Value = this.lvProjects.Items[sols].SubItems[2].Text;
			}

			xmlDocument.DocumentElement.AppendChild (newProductNode);
			
			XmlTextWriter writer = new XmlTextWriter (_configFile, null);
			writer.Formatting = Formatting.Indented;
			xmlDocument.WriteTo (writer);
			writer.Flush();
			writer.Close();

			//Refresh
			this.HasProductDataChanged = false;
			this.LoadProductNames();
			this.PopulateProductsList();
			this.cbProducts.SelectedIndex = cbProducts.Items.Count - 1;

			return true;
		}

		private void lnkDeleteSelected_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			DialogResult dResult = MessageBox.Show (String.Concat ("Are you sure you want to delete the data for ", this.cbProducts.SelectedItem.ToString(), " ?"), "SIBuilder", MessageBoxButtons.YesNo);
			
			if (dResult == DialogResult.No)
				return;

			this.DeleteProductDataFromConfigFile (this.configFileName, this.cbProducts.SelectedItem.ToString());
			//Refresh
			this.HasProductDataChanged = false;
			if (this.LoadProductNames())
			{
				this.PopulateProductsList();
				this.cbProducts.SelectedIndex = 0;
			}
			else
			{
				this.cbProducts.Items.Clear();
				this.DisplayProductInformation (new ProductDetails());
			}
		}

		private void lnkSaveSelected_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			DialogResult dResult;
			if (this.configFileName == "")
			{
				dResult = this.dialogOpenFile.ShowDialog(this);

				if (dResult == DialogResult.OK)
				{
					//if config file does not exist, create a new one
					if (!System.IO.File.Exists (this.dialogOpenFile.FileName))
					{
						CreateEmptyConfigFile (this.dialogOpenFile.FileName);
					}
					else
					{
						System.IO.FileAttributes fAttribs = System.IO.File.GetAttributes (this.dialogOpenFile.FileName);
						if (FileAttributes.ReadOnly.CompareTo (fAttribs & FileAttributes.ReadOnly) == 0)
						{
							System.IO.File.SetAttributes (this.dialogOpenFile.FileName, fAttribs & (~FileAttributes.ReadOnly));
						}
					}

					this.configFileName = this.dialogOpenFile.FileName;
				}
			}

			if (this.configFileName != "")
				SaveCurrentProductDataToConfigFile(null);
		}

		private void lnkCreateNewProductEntry_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			this.DisplayProductInformation (new ProductDetails());
		}

		private void btnAddProject_Click(object sender, System.EventArgs e)
		{
			ListViewItem newProjectItem = new ListViewItem ("<enter solution path>");
			newProjectItem.SubItems.Add ("{optional}");
			newProjectItem.SubItems.Add ("Release");
			this.lvProjects.Items.Add (newProjectItem);
		}

		private void btnRemoveProject_Click(object sender, System.EventArgs e)
		{
			if (lvProjects.SelectedIndices.Count >0)
				this.lvProjects.Items.Remove (lvProjects.Items[this.lvProjects.SelectedIndices[0]]);
		}

		private void btnAddSCItem_Click(object sender, System.EventArgs e)
		{
			ListViewItem newSCItem = new ListViewItem ("<enter path to SC executable>");
			newSCItem.SubItems.Add ("<enter arguments>");
			this.lvSCOptions.Items.Add (newSCItem);
		}

		private void btnDelSCItem_Click(object sender, System.EventArgs e)
		{
			if (this.lvSCOptions.SelectedItems.Count > 0)
			{
				this.lvSCOptions.Items.RemoveAt (this.lvSCOptions.SelectedIndices[0]);
			}
		}

		private void lnkLoadConfigFile_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			this.DisplayProductInformation (new ProductDetails());
			if (this.dialogOpenFile.ShowDialog() == DialogResult.OK)
			{
				if (System.IO.File.Exists (this.dialogOpenFile.FileName))
				{
					this.configFileName = this.dialogOpenFile.FileName;
					this.LoadProductNames ();
					this.PopulateProductsList();
				}
				else
				{
					MessageBox.Show ("File does not exist, select another file");
					lnkLoadConfigFile_LinkClicked (sender, e);
				}
			}
		}

		private void lnkNewConfigFile_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			this.cbProducts.Items.Clear();
			this.DisplayProductInformation (new ProductDetails());

			if (this.dialogOpenFile.ShowDialog() == DialogResult.OK)
			{
				if (File.Exists (this.dialogOpenFile.FileName))
				{
					MessageBox.Show ("The filename entered already exists. Please select another file");
					lnkNewConfigFile_LinkClicked (sender, e);
				}

				this.configFileName = this.dialogOpenFile.FileName;
				this.CreateEmptyConfigFile (this.configFileName);
			}
		}
	}

}

