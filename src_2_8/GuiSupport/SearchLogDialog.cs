using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for SearchLogDialog.
	/// </summary>
	public class SearchLogDialog : System.Windows.Forms.Form
	{
		#region Member declarations
		private System.Windows.Forms.TextBox tb_SearchText;
		private System.Windows.Forms.Button btn_Find;
		private System.Windows.Forms.Button btn_Cancel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		
		private ProcessMdiWindow LogWindow;
		#endregion

		#region Non Event Driven Methods
		public void SetTextFocus()
		{
			this.tb_SearchText.SelectAll();
			this.tb_SearchText.Focus();
		}

		public SearchLogDialog(ProcessMdiWindow parentWindow)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.LogWindow = parentWindow;
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
		#endregion

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.tb_SearchText = new System.Windows.Forms.TextBox();
			this.btn_Find = new System.Windows.Forms.Button();
			this.btn_Cancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// tb_SearchText
			// 
			this.tb_SearchText.Location = new System.Drawing.Point(16, 17);
			this.tb_SearchText.Name = "tb_SearchText";
			this.tb_SearchText.Size = new System.Drawing.Size(312, 20);
			this.tb_SearchText.TabIndex = 0;
			this.tb_SearchText.Text = "";
			// 
			// btn_Find
			// 
			this.btn_Find.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btn_Find.Location = new System.Drawing.Point(344, 16);
			this.btn_Find.Name = "btn_Find";
			this.btn_Find.Size = new System.Drawing.Size(88, 24);
			this.btn_Find.TabIndex = 1;
			this.btn_Find.Text = "Find Next";
			this.btn_Find.Click += new System.EventHandler(this.btn_Find_Click);
			// 
			// btn_Cancel
			// 
			this.btn_Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btn_Cancel.Location = new System.Drawing.Point(344, 48);
			this.btn_Cancel.Name = "btn_Cancel";
			this.btn_Cancel.Size = new System.Drawing.Size(88, 24);
			this.btn_Cancel.TabIndex = 2;
			this.btn_Cancel.Text = "Cancel";
			this.btn_Cancel.Click += new System.EventHandler(this.btn_Cancel_Click);
			// 
			// SearchLog
			// 
			this.AcceptButton = this.btn_Find;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.btn_Cancel;
			this.ClientSize = new System.Drawing.Size(448, 78);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btn_Cancel,
																		  this.btn_Find,
																		  this.tb_SearchText});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "SearchLog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Find";
			this.TopMost = true;
			this.ResumeLayout(false);

		}
		#endregion

		#region Event Driven Methods
		private void btn_Cancel_Click(object sender, System.EventArgs e)
		{
			this.Hide();
		}

		private void btn_Find_Click(object sender, System.EventArgs e)
		{
			this.LogWindow.SearchInfo = tb_SearchText.Text;
			this.LogWindow.SearchLogFile ();
		}

		#endregion
	}
}
