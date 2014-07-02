using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace CoreTester
{
	
	public class TestForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button saveButton;
		private System.Windows.Forms.Button recordButton;
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.RadioButton radioButton2;
		private System.Windows.Forms.RadioButton radioButton3;
		private System.Windows.Forms.RadioButton radioButton4;
		private System.Windows.Forms.RadioButton radioButton6;
		private System.Windows.Forms.RadioButton radioButton7;
		private System.Windows.Forms.RadioButton radioButton8;
		private System.Windows.Forms.RadioButton radioButton9;
		private System.Windows.Forms.RadioButton radioButton10;
		private System.Windows.Forms.RadioButton radioButton11;
		private System.Windows.Forms.RadioButton radioButton12;
		private System.Windows.Forms.RadioButton radioButton13;
		private System.Windows.Forms.RadioButton radioButton14;
		private System.Windows.Forms.RadioButton radioButton15;
		private System.Windows.Forms.RadioButton radioButton16;
		private System.Windows.Forms.RadioButton radioButton17;
		private System.Windows.Forms.RadioButton radioButton18;
		private System.Windows.Forms.RadioButton radioButton19;
		private System.Windows.Forms.RadioButton radioButton20;
		private System.Windows.Forms.RadioButton radioButton22;
		private System.Windows.Forms.RadioButton radioButton23;
		private System.Windows.Forms.RadioButton radioButton24;
		private System.Windows.Forms.RadioButton radioButton25;
		private System.Windows.Forms.RadioButton radioButton26;
		private System.Windows.Forms.RadioButton radioButton27;
		private System.Windows.Forms.RadioButton noneDiskRadio;
		private System.Windows.Forms.RadioButton noneMemoryRadio;
		private System.Windows.Forms.RadioButton noneNetworkRadio;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TrackBar diskTrackBar;
		private System.Windows.Forms.Label diskLabel;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TrackBar memoryTrackBar;
		private System.Windows.Forms.Label memoryLabel;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TrackBar netUpTrackBar;
		private System.Windows.Forms.Label netUpLabel;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.TrackBar netDownTrackBar;
		private System.Windows.Forms.Label netDownLabel;
		private System.Windows.Forms.GroupBox diskGroupBox;
		private System.Windows.Forms.GroupBox memoryGroupBox;
		private System.Windows.Forms.GroupBox networkGroupBox;
		private System.Windows.Forms.GroupBox limitsGroupBox;
		
		private bool isRecording;
		private Record lastDisk, lastMemory, lastNetwork;
		private Queue recordQ = null;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.RadioButton radioButton5;
		private System.Windows.Forms.RadioButton radioButton21;
		private System.Windows.Forms.RadioButton radioButton28;
		private System.Windows.Forms.RadioButton radioButton29;
		private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private XmlTextWriter xmlWriter;

		private int secsElapsed, milliSecsElapsed;
		private LTime thisTime = new LTime();
		private System.Timers.Timer recordTimer;
		private System.Windows.Forms.Label timerLabel;
		private System.Windows.Forms.TextBox purposeBox;
		private MainForm mainDialog;

		public TestForm(MainForm mainDialogIn)
		{
			mainDialog = mainDialogIn;
			InitializeComponent();
			initRecords();
			initBool();
			initInt();

			//disable the radio buttons
			enableRadioButtons(false);
		}

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

		private void InitializeComponent()
		{
			this.saveButton = new System.Windows.Forms.Button();
			this.diskGroupBox = new System.Windows.Forms.GroupBox();
			this.radioButton29 = new System.Windows.Forms.RadioButton();
			this.radioButton28 = new System.Windows.Forms.RadioButton();
			this.radioButton21 = new System.Windows.Forms.RadioButton();
			this.radioButton5 = new System.Windows.Forms.RadioButton();
			this.radioButton1 = new System.Windows.Forms.RadioButton();
			this.radioButton15 = new System.Windows.Forms.RadioButton();
			this.radioButton14 = new System.Windows.Forms.RadioButton();
			this.radioButton13 = new System.Windows.Forms.RadioButton();
			this.radioButton12 = new System.Windows.Forms.RadioButton();
			this.radioButton11 = new System.Windows.Forms.RadioButton();
			this.radioButton10 = new System.Windows.Forms.RadioButton();
			this.radioButton9 = new System.Windows.Forms.RadioButton();
			this.radioButton8 = new System.Windows.Forms.RadioButton();
			this.radioButton7 = new System.Windows.Forms.RadioButton();
			this.radioButton6 = new System.Windows.Forms.RadioButton();
			this.radioButton4 = new System.Windows.Forms.RadioButton();
			this.radioButton3 = new System.Windows.Forms.RadioButton();
			this.radioButton2 = new System.Windows.Forms.RadioButton();
			this.noneDiskRadio = new System.Windows.Forms.RadioButton();
			this.recordButton = new System.Windows.Forms.Button();
			this.memoryGroupBox = new System.Windows.Forms.GroupBox();
			this.radioButton20 = new System.Windows.Forms.RadioButton();
			this.radioButton19 = new System.Windows.Forms.RadioButton();
			this.radioButton18 = new System.Windows.Forms.RadioButton();
			this.radioButton17 = new System.Windows.Forms.RadioButton();
			this.radioButton16 = new System.Windows.Forms.RadioButton();
			this.noneMemoryRadio = new System.Windows.Forms.RadioButton();
			this.networkGroupBox = new System.Windows.Forms.GroupBox();
			this.radioButton27 = new System.Windows.Forms.RadioButton();
			this.radioButton26 = new System.Windows.Forms.RadioButton();
			this.radioButton25 = new System.Windows.Forms.RadioButton();
			this.radioButton24 = new System.Windows.Forms.RadioButton();
			this.radioButton23 = new System.Windows.Forms.RadioButton();
			this.radioButton22 = new System.Windows.Forms.RadioButton();
			this.noneNetworkRadio = new System.Windows.Forms.RadioButton();
			this.limitsGroupBox = new System.Windows.Forms.GroupBox();
			this.netDownLabel = new System.Windows.Forms.Label();
			this.netDownTrackBar = new System.Windows.Forms.TrackBar();
			this.label4 = new System.Windows.Forms.Label();
			this.netUpLabel = new System.Windows.Forms.Label();
			this.netUpTrackBar = new System.Windows.Forms.TrackBar();
			this.label3 = new System.Windows.Forms.Label();
			this.memoryLabel = new System.Windows.Forms.Label();
			this.memoryTrackBar = new System.Windows.Forms.TrackBar();
			this.label2 = new System.Windows.Forms.Label();
			this.diskLabel = new System.Windows.Forms.Label();
			this.diskTrackBar = new System.Windows.Forms.TrackBar();
			this.label1 = new System.Windows.Forms.Label();
			this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
			this.recordTimer = new System.Timers.Timer();
			this.timerLabel = new System.Windows.Forms.Label();
			this.purposeBox = new System.Windows.Forms.TextBox();
			this.diskGroupBox.SuspendLayout();
			this.memoryGroupBox.SuspendLayout();
			this.networkGroupBox.SuspendLayout();
			this.limitsGroupBox.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.netDownTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.netUpTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.memoryTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.diskTrackBar)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.recordTimer)).BeginInit();
			this.SuspendLayout();
			// 
			// saveButton
			// 
			this.saveButton.BackColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(192)), ((System.Byte)(0)));
			this.saveButton.Enabled = false;
			this.saveButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.saveButton.Location = new System.Drawing.Point(752, 360);
			this.saveButton.Name = "saveButton";
			this.saveButton.Size = new System.Drawing.Size(112, 23);
			this.saveButton.TabIndex = 0;
			this.saveButton.Text = "&Save";
			this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
			// 
			// diskGroupBox
			// 
			this.diskGroupBox.Controls.AddRange(new System.Windows.Forms.Control[] {
																					   this.radioButton29,
																					   this.radioButton28,
																					   this.radioButton21,
																					   this.radioButton5,
																					   this.radioButton1,
																					   this.radioButton15,
																					   this.radioButton14,
																					   this.radioButton13,
																					   this.radioButton12,
																					   this.radioButton11,
																					   this.radioButton10,
																					   this.radioButton9,
																					   this.radioButton8,
																					   this.radioButton7,
																					   this.radioButton6,
																					   this.radioButton4,
																					   this.radioButton3,
																					   this.radioButton2,
																					   this.noneDiskRadio});
			this.diskGroupBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.diskGroupBox.Location = new System.Drawing.Point(8, 8);
			this.diskGroupBox.Name = "diskGroupBox";
			this.diskGroupBox.Size = new System.Drawing.Size(152, 408);
			this.diskGroupBox.TabIndex = 1;
			this.diskGroupBox.TabStop = false;
			this.diskGroupBox.Text = "Disk";
			// 
			// radioButton29
			// 
			this.radioButton29.Location = new System.Drawing.Point(8, 380);
			this.radioButton29.Name = "radioButton29";
			this.radioButton29.TabIndex = 18;
			this.radioButton29.Text = "File not found";
			this.radioButton29.Click += new System.EventHandler(this.radioButton29_Click);
			// 
			// radioButton28
			// 
			this.radioButton28.Location = new System.Drawing.Point(8, 360);
			this.radioButton28.Name = "radioButton28";
			this.radioButton28.Size = new System.Drawing.Size(112, 24);
			this.radioButton28.TabIndex = 17;
			this.radioButton28.Text = "Can\'t access file";
			this.radioButton28.Click += new System.EventHandler(this.radioButton28_Click);
			// 
			// radioButton21
			// 
			this.radioButton21.Location = new System.Drawing.Point(8, 340);
			this.radioButton21.Name = "radioButton21";
			this.radioButton21.Size = new System.Drawing.Size(136, 24);
			this.radioButton21.TabIndex = 16;
			this.radioButton21.Text = "Access denied to file";
			this.radioButton21.Click += new System.EventHandler(this.radioButton21_Click);
			// 
			// radioButton5
			// 
			this.radioButton5.Location = new System.Drawing.Point(8, 320);
			this.radioButton5.Name = "radioButton5";
			this.radioButton5.Size = new System.Drawing.Size(128, 24);
			this.radioButton5.TabIndex = 15;
			this.radioButton5.Text = "File write protected";
			this.radioButton5.Click += new System.EventHandler(this.radioButton5_Click);
			// 
			// radioButton1
			// 
			this.radioButton1.Location = new System.Drawing.Point(8, 300);
			this.radioButton1.Name = "radioButton1";
			this.radioButton1.Size = new System.Drawing.Size(80, 24);
			this.radioButton1.TabIndex = 14;
			this.radioButton1.Text = "Data Error";
			this.radioButton1.Click += new System.EventHandler(this.radioButton1_Click);
			// 
			// radioButton15
			// 
			this.radioButton15.Location = new System.Drawing.Point(8, 280);
			this.radioButton15.Name = "radioButton15";
			this.radioButton15.TabIndex = 13;
			this.radioButton15.Text = "Can\'t seek disk";
			this.radioButton15.Click += new System.EventHandler(this.radioButton15_Click);
			// 
			// radioButton14
			// 
			this.radioButton14.Location = new System.Drawing.Point(8, 256);
			this.radioButton14.Name = "radioButton14";
			this.radioButton14.Size = new System.Drawing.Size(80, 24);
			this.radioButton14.TabIndex = 12;
			this.radioButton14.Text = "File in use";
			this.radioButton14.Click += new System.EventHandler(this.radioButton14_Click);
			// 
			// radioButton13
			// 
			this.radioButton13.Location = new System.Drawing.Point(8, 238);
			this.radioButton13.Name = "radioButton13";
			this.radioButton13.Size = new System.Drawing.Size(80, 24);
			this.radioButton13.TabIndex = 11;
			this.radioButton13.Text = "File locked";
			this.radioButton13.Click += new System.EventHandler(this.radioButton13_Click);
			// 
			// radioButton12
			// 
			this.radioButton12.Location = new System.Drawing.Point(8, 218);
			this.radioButton12.Name = "radioButton12";
			this.radioButton12.Size = new System.Drawing.Size(72, 24);
			this.radioButton12.TabIndex = 10;
			this.radioButton12.Text = "Disk full";
			this.radioButton12.Click += new System.EventHandler(this.radioButton12_Click);
			// 
			// radioButton11
			// 
			this.radioButton11.Location = new System.Drawing.Point(8, 201);
			this.radioButton11.Name = "radioButton11";
			this.radioButton11.Size = new System.Drawing.Size(120, 24);
			this.radioButton11.TabIndex = 9;
			this.radioButton11.Text = "File already exists";
			this.radioButton11.Click += new System.EventHandler(this.radioButton11_Click);
			// 
			// radioButton10
			// 
			this.radioButton10.Location = new System.Drawing.Point(8, 184);
			this.radioButton10.Name = "radioButton10";
			this.radioButton10.Size = new System.Drawing.Size(128, 24);
			this.radioButton10.TabIndex = 8;
			this.radioButton10.Text = "File can\'t be created";
			this.radioButton10.Click += new System.EventHandler(this.radioButton10_Click);
			// 
			// radioButton9
			// 
			this.radioButton9.Location = new System.Drawing.Point(8, 160);
			this.radioButton9.Name = "radioButton9";
			this.radioButton9.Size = new System.Drawing.Size(120, 24);
			this.radioButton9.TabIndex = 7;
			this.radioButton9.Text = "File name too long";
			this.radioButton9.Click += new System.EventHandler(this.radioButton9_Click);
			// 
			// radioButton8
			// 
			this.radioButton8.Location = new System.Drawing.Point(8, 137);
			this.radioButton8.Name = "radioButton8";
			this.radioButton8.TabIndex = 6;
			this.radioButton8.Text = "Dir is read only";
			this.radioButton8.Click += new System.EventHandler(this.radioButton8_Click);
			// 
			// radioButton7
			// 
			this.radioButton7.Location = new System.Drawing.Point(8, 120);
			this.radioButton7.Name = "radioButton7";
			this.radioButton7.TabIndex = 5;
			this.radioButton7.Text = "Can\'t find path ";
			this.radioButton7.Click += new System.EventHandler(this.radioButton7_Click);
			// 
			// radioButton6
			// 
			this.radioButton6.Location = new System.Drawing.Point(8, 96);
			this.radioButton6.Name = "radioButton6";
			this.radioButton6.Size = new System.Drawing.Size(120, 24);
			this.radioButton6.TabIndex = 4;
			this.radioButton6.Text = "Can\'t Remove Dir";
			this.radioButton6.Click += new System.EventHandler(this.radioButton6_Click);
			// 
			// radioButton4
			// 
			this.radioButton4.Location = new System.Drawing.Point(8, 74);
			this.radioButton4.Name = "radioButton4";
			this.radioButton4.Size = new System.Drawing.Size(136, 24);
			this.radioButton4.TabIndex = 3;
			this.radioButton4.Text = "Dir Not Empty";
			this.radioButton4.Click += new System.EventHandler(this.radioButton4_Click);
			// 
			// radioButton3
			// 
			this.radioButton3.Location = new System.Drawing.Point(8, 56);
			this.radioButton3.Name = "radioButton3";
			this.radioButton3.Size = new System.Drawing.Size(88, 24);
			this.radioButton3.TabIndex = 2;
			this.radioButton3.Text = "Invalid Path";
			this.radioButton3.Click += new System.EventHandler(this.radioButton3_Click);
			// 
			// radioButton2
			// 
			this.radioButton2.Location = new System.Drawing.Point(8, 32);
			this.radioButton2.Name = "radioButton2";
			this.radioButton2.Size = new System.Drawing.Size(120, 32);
			this.radioButton2.TabIndex = 1;
			this.radioButton2.Text = "Corrupt Structure";
			this.radioButton2.Click += new System.EventHandler(this.radioButton2_Click);
			// 
			// noneDiskRadio
			// 
			this.noneDiskRadio.Checked = true;
			this.noneDiskRadio.Location = new System.Drawing.Point(8, 16);
			this.noneDiskRadio.Name = "noneDiskRadio";
			this.noneDiskRadio.Size = new System.Drawing.Size(56, 24);
			this.noneDiskRadio.TabIndex = 0;
			this.noneDiskRadio.TabStop = true;
			this.noneDiskRadio.Text = "None";
			this.noneDiskRadio.Click += new System.EventHandler(this.noneDiskRadio_Click);
			// 
			// recordButton
			// 
			this.recordButton.BackColor = System.Drawing.Color.Yellow;
			this.recordButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.recordButton.Location = new System.Drawing.Point(752, 336);
			this.recordButton.Name = "recordButton";
			this.recordButton.Size = new System.Drawing.Size(112, 23);
			this.recordButton.TabIndex = 2;
			this.recordButton.Text = "&Record";
			this.recordButton.Click += new System.EventHandler(this.recordButton_Click);
			// 
			// memoryGroupBox
			// 
			this.memoryGroupBox.Controls.AddRange(new System.Windows.Forms.Control[] {
																						 this.radioButton20,
																						 this.radioButton19,
																						 this.radioButton18,
																						 this.radioButton17,
																						 this.radioButton16,
																						 this.noneMemoryRadio});
			this.memoryGroupBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.memoryGroupBox.Location = new System.Drawing.Point(168, 8);
			this.memoryGroupBox.Name = "memoryGroupBox";
			this.memoryGroupBox.Size = new System.Drawing.Size(152, 408);
			this.memoryGroupBox.TabIndex = 3;
			this.memoryGroupBox.TabStop = false;
			this.memoryGroupBox.Text = "Memory";
			// 
			// radioButton20
			// 
			this.radioButton20.Location = new System.Drawing.Point(8, 120);
			this.radioButton20.Name = "radioButton20";
			this.radioButton20.Size = new System.Drawing.Size(128, 24);
			this.radioButton20.TabIndex = 5;
			this.radioButton20.Text = "Insufficient memory";
			this.radioButton20.Click += new System.EventHandler(this.radioButton20_Click);
			// 
			// radioButton19
			// 
			this.radioButton19.Location = new System.Drawing.Point(8, 96);
			this.radioButton19.Name = "radioButton19";
			this.radioButton19.TabIndex = 4;
			this.radioButton19.Text = "Invalid access";
			this.radioButton19.Click += new System.EventHandler(this.radioButton19_Click);
			// 
			// radioButton18
			// 
			this.radioButton18.Location = new System.Drawing.Point(8, 74);
			this.radioButton18.Name = "radioButton18";
			this.radioButton18.Size = new System.Drawing.Size(112, 24);
			this.radioButton18.TabIndex = 3;
			this.radioButton18.Text = "Segment locked";
			this.radioButton18.Click += new System.EventHandler(this.radioButton18_Click);
			// 
			// radioButton17
			// 
			this.radioButton17.Location = new System.Drawing.Point(8, 56);
			this.radioButton17.Name = "radioButton17";
			this.radioButton17.TabIndex = 2;
			this.radioButton17.Text = "Invalid address";
			this.radioButton17.Click += new System.EventHandler(this.radioButton17_Click);
			// 
			// radioButton16
			// 
			this.radioButton16.Location = new System.Drawing.Point(8, 33);
			this.radioButton16.Name = "radioButton16";
			this.radioButton16.Size = new System.Drawing.Size(120, 24);
			this.radioButton16.TabIndex = 1;
			this.radioButton16.Text = "Page file too small";
			this.radioButton16.Click += new System.EventHandler(this.radioButton16_Click);
			// 
			// noneMemoryRadio
			// 
			this.noneMemoryRadio.Checked = true;
			this.noneMemoryRadio.Location = new System.Drawing.Point(8, 16);
			this.noneMemoryRadio.Name = "noneMemoryRadio";
			this.noneMemoryRadio.Size = new System.Drawing.Size(56, 24);
			this.noneMemoryRadio.TabIndex = 0;
			this.noneMemoryRadio.TabStop = true;
			this.noneMemoryRadio.Text = "None";
			this.noneMemoryRadio.Click += new System.EventHandler(this.noneMemoryRadio_Click);
			// 
			// networkGroupBox
			// 
			this.networkGroupBox.Controls.AddRange(new System.Windows.Forms.Control[] {
																						  this.radioButton27,
																						  this.radioButton26,
																						  this.radioButton25,
																						  this.radioButton24,
																						  this.radioButton23,
																						  this.radioButton22,
																						  this.noneNetworkRadio});
			this.networkGroupBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.networkGroupBox.Location = new System.Drawing.Point(328, 8);
			this.networkGroupBox.Name = "networkGroupBox";
			this.networkGroupBox.Size = new System.Drawing.Size(184, 408);
			this.networkGroupBox.TabIndex = 4;
			this.networkGroupBox.TabStop = false;
			this.networkGroupBox.Text = "Network";
			// 
			// radioButton27
			// 
			this.radioButton27.Location = new System.Drawing.Point(8, 145);
			this.radioButton27.Name = "radioButton27";
			this.radioButton27.Size = new System.Drawing.Size(144, 24);
			this.radioButton27.TabIndex = 6;
			this.radioButton27.Text = "Network disconnected";
			this.radioButton27.Click += new System.EventHandler(this.radioButton27_Click);
			// 
			// radioButton26
			// 
			this.radioButton26.Location = new System.Drawing.Point(8, 120);
			this.radioButton26.Name = "radioButton26";
			this.radioButton26.Size = new System.Drawing.Size(144, 24);
			this.radioButton26.TabIndex = 5;
			this.radioButton26.Text = "Network not installed";
			this.radioButton26.Click += new System.EventHandler(this.radioButton26_Click);
			// 
			// radioButton25
			// 
			this.radioButton25.Location = new System.Drawing.Point(8, 96);
			this.radioButton25.Name = "radioButton25";
			this.radioButton25.Size = new System.Drawing.Size(168, 24);
			this.radioButton25.TabIndex = 4;
			this.radioButton25.Text = "Wrong version of  Winsock";
			this.radioButton25.Click += new System.EventHandler(this.radioButton25_Click);
			// 
			// radioButton24
			// 
			this.radioButton24.Location = new System.Drawing.Point(8, 74);
			this.radioButton24.Name = "radioButton24";
			this.radioButton24.Size = new System.Drawing.Size(120, 24);
			this.radioButton24.TabIndex = 3;
			this.radioButton24.Text = "Task limit reached";
			this.radioButton24.Click += new System.EventHandler(this.radioButton24_Click);
			// 
			// radioButton23
			// 
			this.radioButton23.Location = new System.Drawing.Point(8, 56);
			this.radioButton23.Name = "radioButton23";
			this.radioButton23.Size = new System.Drawing.Size(120, 24);
			this.radioButton23.TabIndex = 2;
			this.radioButton23.Text = "No ports available";
			this.radioButton23.Click += new System.EventHandler(this.radioButton23_Click);
			// 
			// radioButton22
			// 
			this.radioButton22.Location = new System.Drawing.Point(8, 33);
			this.radioButton22.Name = "radioButton22";
			this.radioButton22.TabIndex = 1;
			this.radioButton22.Text = "Network down";
			this.radioButton22.Click += new System.EventHandler(this.radioButton22_Click);
			// 
			// noneNetworkRadio
			// 
			this.noneNetworkRadio.Checked = true;
			this.noneNetworkRadio.Location = new System.Drawing.Point(8, 16);
			this.noneNetworkRadio.Name = "noneNetworkRadio";
			this.noneNetworkRadio.Size = new System.Drawing.Size(56, 24);
			this.noneNetworkRadio.TabIndex = 0;
			this.noneNetworkRadio.TabStop = true;
			this.noneNetworkRadio.Text = "None";
			this.noneNetworkRadio.Click += new System.EventHandler(this.noneNetworkRadio_Click);
			// 
			// limitsGroupBox
			// 
			this.limitsGroupBox.Controls.AddRange(new System.Windows.Forms.Control[] {
																						 this.netDownLabel,
																						 this.netDownTrackBar,
																						 this.label4,
																						 this.netUpLabel,
																						 this.netUpTrackBar,
																						 this.label3,
																						 this.memoryLabel,
																						 this.memoryTrackBar,
																						 this.label2,
																						 this.diskLabel,
																						 this.diskTrackBar,
																						 this.label1});
			this.limitsGroupBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.limitsGroupBox.Location = new System.Drawing.Point(520, 8);
			this.limitsGroupBox.Name = "limitsGroupBox";
			this.limitsGroupBox.Size = new System.Drawing.Size(224, 408);
			this.limitsGroupBox.TabIndex = 6;
			this.limitsGroupBox.TabStop = false;
			this.limitsGroupBox.Text = "Limits";
			// 
			// netDownLabel
			// 
			this.netDownLabel.Location = new System.Drawing.Point(112, 256);
			this.netDownLabel.Name = "netDownLabel";
			this.netDownLabel.Size = new System.Drawing.Size(88, 23);
			this.netDownLabel.TabIndex = 11;
			this.netDownLabel.Text = "0";
			this.netDownLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// netDownTrackBar
			// 
			this.netDownTrackBar.Location = new System.Drawing.Point(8, 256);
			this.netDownTrackBar.Name = "netDownTrackBar";
			this.netDownTrackBar.Size = new System.Drawing.Size(104, 50);
			this.netDownTrackBar.TabIndex = 10;
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 232);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(104, 23);
			this.label4.TabIndex = 9;
			this.label4.Text = "Network Download";
			// 
			// netUpLabel
			// 
			this.netUpLabel.Location = new System.Drawing.Point(120, 184);
			this.netUpLabel.Name = "netUpLabel";
			this.netUpLabel.Size = new System.Drawing.Size(88, 23);
			this.netUpLabel.TabIndex = 8;
			this.netUpLabel.Text = "0";
			this.netUpLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// netUpTrackBar
			// 
			this.netUpTrackBar.Location = new System.Drawing.Point(8, 184);
			this.netUpTrackBar.Name = "netUpTrackBar";
			this.netUpTrackBar.Size = new System.Drawing.Size(104, 50);
			this.netUpTrackBar.TabIndex = 7;
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 160);
			this.label3.Name = "label3";
			this.label3.TabIndex = 6;
			this.label3.Text = "Network Upload";
			// 
			// memoryLabel
			// 
			this.memoryLabel.Location = new System.Drawing.Point(120, 112);
			this.memoryLabel.Name = "memoryLabel";
			this.memoryLabel.Size = new System.Drawing.Size(88, 23);
			this.memoryLabel.TabIndex = 5;
			this.memoryLabel.Text = "0";
			this.memoryLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// memoryTrackBar
			// 
			this.memoryTrackBar.Location = new System.Drawing.Point(8, 112);
			this.memoryTrackBar.Name = "memoryTrackBar";
			this.memoryTrackBar.Size = new System.Drawing.Size(104, 50);
			this.memoryTrackBar.TabIndex = 4;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 88);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(48, 23);
			this.label2.TabIndex = 3;
			this.label2.Text = "Memory";
			// 
			// diskLabel
			// 
			this.diskLabel.Location = new System.Drawing.Point(120, 40);
			this.diskLabel.Name = "diskLabel";
			this.diskLabel.Size = new System.Drawing.Size(88, 23);
			this.diskLabel.TabIndex = 2;
			this.diskLabel.Text = "0";
			this.diskLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// diskTrackBar
			// 
			this.diskTrackBar.Location = new System.Drawing.Point(8, 40);
			this.diskTrackBar.Name = "diskTrackBar";
			this.diskTrackBar.Size = new System.Drawing.Size(104, 50);
			this.diskTrackBar.TabIndex = 1;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(32, 23);
			this.label1.TabIndex = 0;
			this.label1.Text = "Disk";
			// 
			// saveFileDialog
			// 
			this.saveFileDialog.FileName = "test1";
			this.saveFileDialog.Filter = "Test Files|*.xml|All Files|*.*";
			// 
			// recordTimer
			// 
			this.recordTimer.SynchronizingObject = this;
			this.recordTimer.Elapsed += new System.Timers.ElapsedEventHandler(this.recordTimer_Elapsed);
			// 
			// timerLabel
			// 
			this.timerLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.timerLabel.Location = new System.Drawing.Point(760, 304);
			this.timerLabel.Name = "timerLabel";
			this.timerLabel.Size = new System.Drawing.Size(96, 23);
			this.timerLabel.TabIndex = 7;
			this.timerLabel.Text = "0.000";
			this.timerLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// purposeBox
			// 
			this.purposeBox.Location = new System.Drawing.Point(752, 272);
			this.purposeBox.Name = "purposeBox";
			this.purposeBox.TabIndex = 8;
			this.purposeBox.Text = "test purpose";
			// 
			// TestForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(866, 424);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.purposeBox,
																		  this.timerLabel,
																		  this.limitsGroupBox,
																		  this.networkGroupBox,
																		  this.memoryGroupBox,
																		  this.recordButton,
																		  this.diskGroupBox,
																		  this.saveButton});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TestForm";
			this.ShowInTaskbar = false;
			this.Text = "Create New Test File";
			this.diskGroupBox.ResumeLayout(false);
			this.memoryGroupBox.ResumeLayout(false);
			this.networkGroupBox.ResumeLayout(false);
			this.limitsGroupBox.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.netDownTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.netUpTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.memoryTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.diskTrackBar)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.recordTimer)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		private void actionButtons(bool toggle) 
		{
			//toggles the action buttons
			saveButton.Enabled = toggle;
		}

		private void enableRadioButtons(bool toggle) 
		{
			//determines whether the buttons are enabled or disabled based on the toggle

			//disk buttons
			noneDiskRadio.Enabled = toggle;
			radioButton1.Enabled = toggle;
			radioButton2.Enabled = toggle;
			radioButton3.Enabled = toggle;
			radioButton4.Enabled = toggle;
			radioButton5.Enabled = toggle;
			radioButton6.Enabled = toggle;
			radioButton7.Enabled = toggle;
			radioButton8.Enabled = toggle;
			radioButton9.Enabled = toggle;
			radioButton10.Enabled = toggle;
			radioButton11.Enabled = toggle;
			radioButton12.Enabled = toggle;
			radioButton13.Enabled = toggle;
			radioButton14.Enabled = toggle;
			radioButton15.Enabled = toggle;
			radioButton21.Enabled = toggle;
			radioButton28.Enabled = toggle;
			radioButton29.Enabled = toggle;
			
			//memory buttons
			noneMemoryRadio.Enabled = toggle;
            radioButton16.Enabled = toggle;
			radioButton17.Enabled = toggle;
			radioButton18.Enabled = toggle;
			radioButton19.Enabled = toggle;
			radioButton20.Enabled = toggle;

			//network buttons
			noneNetworkRadio.Enabled = toggle;
			radioButton22.Enabled = toggle;
			radioButton23.Enabled = toggle;
			radioButton24.Enabled = toggle;
			radioButton25.Enabled = toggle;
			radioButton26.Enabled = toggle;
			radioButton27.Enabled = toggle;

			noneDiskRadio.Checked = true;
			noneMemoryRadio.Checked = true;
			noneNetworkRadio.Checked = true;
			
			timerLabel.Text = "0.000";
		}

		private void createRecord(int enabled, String function, int type, String name) 
		{
			thisTime.updateTime();
			Record tempRecord = new Record(enabled, function, type, name, thisTime.getTimeStamp()); 
			recordQ.Enqueue(tempRecord);
		}

		private void disable(Record lastRecIn) 
		{
			createRecord(0, lastRecIn.function, lastRecIn.type, lastRecIn.name);
		}

		private void initBool() 
		{
			//initialise the boolean values of the program
			isRecording = false;
		}

		private void initRecords() 
		{
			lastDisk = null;
			lastMemory = null;
			lastNetwork = null;
		}

		private void initInt() 
		{
			milliSecsElapsed = 0;
			secsElapsed = 0;
		}
		
		private void recordButton_Click(object sender, System.EventArgs e)
		{
			//record button has been pressed
			if (!isRecording) 
			{
				//start recording
				isRecording = true;
				recordButton.BackColor = System.Drawing.Color.Red;
				recordButton.Text = "&Stop Record";

				actionButtons(false);	
				initRecords();

				//create a new queue of recorded actions
				recordQ = new Queue();

				//enable the radio buttons
				enableRadioButtons(true);

				//start the timer
				recordTimer.Enabled = true;
			}

			else 
			{
				//stop recording
				
				//stop the timer
				recordTimer.Enabled = false;

				isRecording = false;
				recordButton.BackColor = System.Drawing.Color.Yellow;
				recordButton.Text = "&Record";

				actionButtons(true);

				//disable the radio buttons
				enableRadioButtons(false);
			}
		}

		private void noneDiskRadio_Click(object sender, System.EventArgs e)
		{
			//none disk clicked
			if (isRecording) 
			{
				//if we are recording and another disk button has been clicked
				if (lastDisk != null) 
				{
					//if there is something in the queue
					disable(lastDisk);
					lastDisk = null;
				}
			}
		}

		private void radioButton2_Click(object sender, System.EventArgs e)
		{
			//corrupt structure 
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Corrupt structure");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Corrupt structure", thisTime.getTimeStamp());
		}

		private void radioButton3_Click(object sender, System.EventArgs e)
		{
			//invalid path
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Invalid path");
			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Invalid path", thisTime.getTimeStamp());
		}

		private void saveButton_Click(object sender, System.EventArgs e)
		{
			//save button clicked
			if (saveFileDialog.ShowDialog(this) == DialogResult.OK) 
			{ 
				xmlWriter = new XmlTextWriter(saveFileDialog.FileName, new ASCIIEncoding());
				xmlWriter.Formatting = Formatting.Indented;
			
				xmlWriter.WriteStartElement("TestFile");
				xmlWriter.WriteAttributeString("Purpose", purposeBox.Text);
				xmlWriter.WriteStartElement("Recorded_Session");
			
				while (recordQ.Count != 0) 
				{
					Record tempRecord = (Record) recordQ.Dequeue();

					xmlWriter.WriteStartElement("Record");
				
					xmlWriter.WriteStartElement("Log");
					xmlWriter.WriteAttributeString("Index", "");
					xmlWriter.WriteAttributeString("Function", tempRecord.function);
					xmlWriter.WriteAttributeString("TimeDelta", "");
					xmlWriter.WriteAttributeString("TimeStamp", tempRecord.timeStamp);
					xmlWriter.WriteAttributeString("Session", "");
					xmlWriter.WriteAttributeString("AppName", "");
					xmlWriter.WriteEndElement();

					xmlWriter.WriteStartElement("Inject");
					xmlWriter.WriteAttributeString("Type", tempRecord.type.ToString());
					xmlWriter.WriteAttributeString("Name", tempRecord.name);
					xmlWriter.WriteAttributeString("Enabled", tempRecord.enabled.ToString());
			    
					xmlWriter.WriteEndElement();

					xmlWriter.WriteEndElement();
				}

				xmlWriter.WriteEndElement();
				xmlWriter.WriteEndElement();
				xmlWriter.Close();
			}
		}

		private void radioButton4_Click(object sender, System.EventArgs e)
		{
			//dir not empty
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Directory not empty");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Directory not empty", thisTime.getTimeStamp());
		}

		private void radioButton6_Click(object sender, System.EventArgs e)
		{
			//can't remove dir
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Can't remove directory");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Can't remove directory", thisTime.getTimeStamp());
		}

		private void radioButton7_Click(object sender, System.EventArgs e)
		{
			//can't find path
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Can't find path");
			
			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Can't find path", thisTime.getTimeStamp());
		}

		private void radioButton8_Click(object sender, System.EventArgs e)
		{
			//dir is read only
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Directory is read only");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Directory is read only", thisTime.getTimeStamp());
		}

		private void radioButton9_Click(object sender, System.EventArgs e)
		{
			//file name too long 
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File name too long");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File name too long", thisTime.getTimeStamp());
		}

		private void radioButton10_Click(object sender, System.EventArgs e)
		{
			//file can't be created 
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File can't be created");

	        thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File can't be created", thisTime.getTimeStamp());
		}

		private void radioButton11_Click(object sender, System.EventArgs e)
		{
			//file already exists

			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File already exists");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File already exists", thisTime.getTimeStamp());
		}

		private void radioButton12_Click(object sender, System.EventArgs e)
		{
			//disk full
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Disk full");
			
			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Disk full", thisTime.getTimeStamp());
		}

		private void radioButton13_Click(object sender, System.EventArgs e)
		{
			//file locked
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File locked");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File locked",  thisTime.getTimeStamp());
		}

		private void radioButton14_Click(object sender, System.EventArgs e)
		{
			//file in use
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File in use");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File in use",  thisTime.getTimeStamp());
		}

		private void radioButton15_Click(object sender, System.EventArgs e)
		{
			//can't seek disk 

			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Can't seek disk");
			
			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Can't seek disk",  thisTime.getTimeStamp());
		}

		private void radioButton1_Click(object sender, System.EventArgs e)
		{
			//data error 
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Data error");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Data error", thisTime.getTimeStamp());
		}

		private void radioButton5_Click(object sender, System.EventArgs e)
		{
			//file write protected 
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File is write protected");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File is write protected", thisTime.getTimeStamp());
		}

		private void radioButton21_Click(object sender, System.EventArgs e)
		{
			//access denied to file
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Access denied to file");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Access denied to file",  thisTime.getTimeStamp());
		}

		private void radioButton28_Click(object sender, System.EventArgs e)
		{
			//can't access file
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "Can't access file");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "Can't access file",  thisTime.getTimeStamp());
		}

		private void radioButton29_Click(object sender, System.EventArgs e)
		{
			//file not found
			if (lastDisk != null) 
			{
				//if there is something in the queue
				disable(lastDisk);
				lastDisk = null;
			}

			createRecord(2, "GetVersionExW", 21, "File not found");

			thisTime.updateTime();
			lastDisk = new Record(2, "GetVersionExW", 21, "File not found",  thisTime.getTimeStamp());
		}

		private void noneMemoryRadio_Click(object sender, System.EventArgs e)
		{
			//none memory clicked
			if (isRecording) 
			{
				//if we are recording
				if (lastMemory != null) 
				{
					//if there is something in the queue
					disable(lastMemory);
					lastMemory = null;
				}
			}
		}

		private void radioButton16_Click(object sender, System.EventArgs e)
		{
			//page file too small
			if (lastMemory != null) 
			{
				//if there is something in the queue
				disable(lastMemory);
				lastMemory = null;
			}

			createRecord(2, "GetModuleHandleA", 19, "Page file too small");

			thisTime.updateTime();
			lastMemory = new Record(2, "GetModuleHandleA", 19, "Page file too small", thisTime.getTimeStamp());
		}

		private void radioButton17_Click(object sender, System.EventArgs e)
		{
			//invalid address
			if (lastMemory != null) 
			{
				//if there is something in the queue
				disable(lastMemory);
				lastMemory = null;
			}

			createRecord(2, "GetModuleHandleA", 19, "Invalid address");

			thisTime.updateTime();
			lastMemory = new Record(2, "GetModuleHandleA", 19, "Invalid address",  thisTime.getTimeStamp());
		}

		private void radioButton18_Click(object sender, System.EventArgs e)
		{
			//segment locked 
			if (lastMemory != null) 
			{
				//if there is something in the queue
				disable(lastMemory);
				lastMemory = null;
			}

			createRecord(2, "GetModuleHandleA", 19, "Segment locked");

			thisTime.updateTime();
			lastMemory = new Record(2, "GetModuleHandleA", 19, "Segment locked",  thisTime.getTimeStamp());
		}

		private void radioButton19_Click(object sender, System.EventArgs e)
		{
			//invalid access
			if (lastMemory != null) 
			{
				//if there is something in the queue
				disable(lastMemory);
				lastMemory = null;
			}

			createRecord(2, "GetModuleHandleA", 19, "Invalid access");

			thisTime.updateTime();
			lastMemory = new Record(2, "GetModuleHandleA", 19, "Invalid access",  thisTime.getTimeStamp());
		}

		private void radioButton20_Click(object sender, System.EventArgs e)
		{
			//insufficient memory 
			if (lastMemory != null) 
			{
				//if there is something in the queue
				disable(lastMemory);
				lastMemory = null;
			}

			createRecord(2, "GetModuleHandleA", 19, "Insufficient memory");

			thisTime.updateTime();
			lastMemory = new Record(2, "GetModuleHandleA", 19, "Insufficient memory",  thisTime.getTimeStamp());
		}

		private void noneNetworkRadio_Click(object sender, System.EventArgs e)
		{
			//none network clicked
			if (isRecording) 
			{
				//if we are recording
				if (lastNetwork != null) 
				{
					//if there is something in the queue
					disable(lastNetwork);
					lastNetwork = null;
				}
			}
		}

		private void radioButton22_Click(object sender, System.EventArgs e)
		{
			//network down
			if (lastNetwork != null) 
			{
				//if there is something in the queue
				disable(lastNetwork);
				lastNetwork = null;
			}

			createRecord(2, "GetVersionExW", 23, "Network down");
			
			thisTime.updateTime();
			lastNetwork = new Record(2, "GetVersionExW", 23, "Network down", thisTime.getTimeStamp());
		}

		private void radioButton23_Click(object sender, System.EventArgs e)
		{
			//no ports available
			if (lastNetwork != null) 
			{
				//if there is something in the queue
				disable(lastNetwork);
				lastNetwork = null;
			}

			createRecord(2, "GetVersionExW", 23, "No ports available");

			thisTime.updateTime();
			lastNetwork = new Record(2, "GetVersionExW", 23, "No ports available", thisTime.getTimeStamp());
		}

		private void radioButton24_Click(object sender, System.EventArgs e)
		{
			//task limit reached 
			if (lastNetwork != null) 
			{
				//if there is something in the queue
				disable(lastNetwork);
				lastNetwork = null;
			}

			createRecord(2, "GetVersionExW", 23, "Task limit reached");

			thisTime.updateTime();
			lastNetwork = new Record(2, "GetVersionExW", 23, "Task limit reached", thisTime.getTimeStamp());
		}

		private void radioButton25_Click(object sender, System.EventArgs e)
		{
			//wrong version of winsock
			if (lastNetwork != null) 
			{
				//if there is something in the queue
				disable(lastNetwork);
				lastNetwork = null;
			}

			createRecord(2, "GetVersionExW", 23, "Wrong version of Winsock");

			thisTime.updateTime();
			lastNetwork = new Record(2, "GetVersionExW", 23, "Wrong version of Winsock", thisTime.getTimeStamp());
		}

		private void radioButton27_Click(object sender, System.EventArgs e)
		{
			//network disconnected
			if (lastNetwork != null) 
			{
				//if there is something in the queue
				disable(lastNetwork);
				lastNetwork = null;
			}

			createRecord(2, "GetVersionExW", 23, "Network disconnected");

			thisTime.updateTime();
			lastNetwork = new Record(2, "GetVersionExW", 23, "Network disconnected", thisTime.getTimeStamp());
		}

		private void recordTimer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
		{
			//update the time elapsed and the timer 
			milliSecsElapsed += 1;

			if (milliSecsElapsed >= 10) 
			{
				//roll over the seconds
				milliSecsElapsed = 0;
			    secsElapsed++;
			}

			//update the imter label
			timerLabel.Text = secsElapsed.ToString() + "." + milliSecsElapsed.ToString() + "00";
			timerLabel.Refresh();
		}

		private void radioButton26_Click(object sender, System.EventArgs e)
		{
			//network not installed
			if (lastNetwork != null) 
			{
				//if there is something in the queue
				disable(lastNetwork);
				lastNetwork = null;
			}

			createRecord(2, "GetVersionExW", 23, "Network not installed");

			thisTime.updateTime();
			lastNetwork = new Record(2, "GetVersionExW", 23, "Network not installed", thisTime.getTimeStamp());
		}
	}
}
