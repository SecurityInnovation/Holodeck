using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using FunctionDatabase;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for LogFilterDialog.
	/// </summary>
	public class LogFilterDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button logFilterOKButton;
		private System.Windows.Forms.Button logFilterCancelButton;
		private LogFilterTreeView logFilterTreeView;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ArrayList FilteredOutFunctionNames
		{
			get { return this.logFilterTreeView.FilteredOutFunctionNames; }
			set { this.logFilterTreeView.FilteredOutFunctionNames = value; }
		}

		public LogFilterDialog()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(LogFilterDialog));
			this.logFilterOKButton = new System.Windows.Forms.Button();
			this.logFilterCancelButton = new System.Windows.Forms.Button();
			this.logFilterTreeView = new LogFilterTreeView();
			this.SuspendLayout();

			//
			// logFilterTreeView
			//
			this.logFilterTreeView.Location = new System.Drawing.Point(8, 8);
			this.logFilterTreeView.TabIndex = 2;

			// 
			// logFilterOKButton
			// 
			this.logFilterOKButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.logFilterOKButton.Location = new System.Drawing.Point(135, 376);
			this.logFilterOKButton.Name = "logFilterOKButton";
			this.logFilterOKButton.TabIndex = 0;
			this.logFilterOKButton.Text = "OK";
			this.logFilterOKButton.Click += new System.EventHandler(this.logFilterOKButton_Click);
			// 
			// logFilterCancelButton
			// 
			this.logFilterCancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.logFilterCancelButton.Location = new System.Drawing.Point(231, 376);
			this.logFilterCancelButton.Name = "logFilterCancelButton";
			this.logFilterCancelButton.TabIndex = 1;
			this.logFilterCancelButton.Text = "Cancel";
			this.logFilterCancelButton.Click += new System.EventHandler(this.logFilterCancelButton_Click);
			// 
			// LogFilterDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(440, 414);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.logFilterTreeView,
																		  this.logFilterCancelButton,
																		  this.logFilterOKButton});
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "LogFilterDialog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Choose Functions To Display In Logs";
			this.Load += new System.EventHandler(this.LogFilterDialog_Load);
			this.ResumeLayout(false);

		}
		#endregion

		private void logFilterOKButton_Click(object sender, System.EventArgs e)
		{
			Close();
		}

		private void logFilterCancelButton_Click(object sender, System.EventArgs e)
		{
			Close();
		}

		private void LogFilterDialog_Load(object sender, System.EventArgs e)
		{
		}
	}
}
