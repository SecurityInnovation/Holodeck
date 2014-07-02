using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for WaitDialog.
	/// </summary>
	public class WaitDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.Label lblMessage;
		private System.Windows.Forms.ProgressBar pbComplete;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public WaitDialog()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		public string Title
		{
			set { this.Text = value; }
			get { return this.Text; }
		}

		public string Message
		{
			set {this.lblMessage.Text = value;}
			get {return this.lblMessage.Text;}
		}

		public int Progress
		{
			set {this.pbComplete.Value = value;}
			get {return this.pbComplete.Value;}
		}

		public bool DisplayProgress
		{
			set {this.pbComplete.Visible = value;}
			get {return this.pbComplete.Visible;}
		}

		public EventHandler Cancel_ClickHandler
		{
			set {this.btnCancel.Click += value;}
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
			this.lblMessage = new System.Windows.Forms.Label();
			this.btnCancel = new System.Windows.Forms.Button();
			this.pbComplete = new System.Windows.Forms.ProgressBar();
			this.SuspendLayout();
			// 
			// lblMessage
			// 
			this.lblMessage.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lblMessage.Location = new System.Drawing.Point(8, 16);
			this.lblMessage.Name = "lblMessage";
			this.lblMessage.Size = new System.Drawing.Size(312, 40);
			this.lblMessage.TabIndex = 0;
			this.lblMessage.Text = "Default Text For the Wait Dialog";
			this.lblMessage.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// btnCancel
			// 
			this.btnCancel.Location = new System.Drawing.Point(120, 88);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(96, 24);
			this.btnCancel.TabIndex = 1;
			this.btnCancel.Text = "Cancel";
			// 
			// pbComplete
			// 
			this.pbComplete.Location = new System.Drawing.Point(8, 64);
			this.pbComplete.Name = "pbComplete";
			this.pbComplete.Size = new System.Drawing.Size(312, 16);
			this.pbComplete.TabIndex = 2;
			this.pbComplete.Visible = false;
			// 
			// WaitDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(326, 124);
			this.ControlBox = false;
			this.Controls.Add(this.pbComplete);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.lblMessage);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "WaitDialog";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.ResumeLayout(false);

		}
		#endregion
	}
}
