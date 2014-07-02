using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for SplashScreen.
	/// </summary>
	public class SplashScreen : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.Panel productInfoPanel;
		private System.Windows.Forms.LinkLabel siLinkLabel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public SplashScreen()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(SplashScreen));
			this.productInfoPanel = new System.Windows.Forms.Panel();
			this.label3 = new System.Windows.Forms.Label();
			this.siLinkLabel = new System.Windows.Forms.LinkLabel();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.pictureBox1 = new System.Windows.Forms.PictureBox();
			this.productInfoPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// productInfoPanel
			// 
			this.productInfoPanel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.productInfoPanel.Controls.AddRange(new System.Windows.Forms.Control[] {
																						   this.label3,
																						   this.siLinkLabel,
																						   this.label2,
																						   this.label1,
																						   this.pictureBox1});
			this.productInfoPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.productInfoPanel.Name = "productInfoPanel";
			this.productInfoPanel.Size = new System.Drawing.Size(490, 160);
			this.productInfoPanel.TabIndex = 0;
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(14, 119);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(100, 16);
			this.label3.TabIndex = 10;
			this.label3.Text = "Version 2.0";
			// 
			// siLinkLabel
			// 
			this.siLinkLabel.Location = new System.Drawing.Point(14, 135);
			this.siLinkLabel.Name = "siLinkLabel";
			this.siLinkLabel.Size = new System.Drawing.Size(128, 23);
			this.siLinkLabel.TabIndex = 9;
			this.siLinkLabel.TabStop = true;
			this.siLinkLabel.Text = "http://www.sisecure.com";
			this.siLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.siLinkLabel_LinkClicked);
			// 
			// label2
			// 
			this.label2.Font = new System.Drawing.Font("Book Antiqua", 21.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label2.Location = new System.Drawing.Point(202, 103);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(96, 32);
			this.label2.TabIndex = 8;
			this.label2.Text = "BETA";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(14, 103);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(168, 16);
			this.label1.TabIndex = 7;
			this.label1.Text = "Copyright 2002, System Integrity";
			// 
			// pictureBox1
			// 
			this.pictureBox1.Image = ((System.Drawing.Bitmap)(resources.GetObject("pictureBox1.Image")));
			this.pictureBox1.Name = "pictureBox1";
			this.pictureBox1.Size = new System.Drawing.Size(490, 93);
			this.pictureBox1.TabIndex = 6;
			this.pictureBox1.TabStop = false;
			// 
			// SplashScreen
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.SystemColors.Window;
			this.ClientSize = new System.Drawing.Size(490, 160);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.productInfoPanel});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.Name = "SplashScreen";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "SplashScreen";
			this.TopMost = true;
			this.productInfoPanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void siLinkLabel_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(siLinkLabel.Text);
		}
	}
}
