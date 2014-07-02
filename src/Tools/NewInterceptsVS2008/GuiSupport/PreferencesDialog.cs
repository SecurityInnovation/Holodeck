using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for PreferencesDialog.
	/// </summary>
	public class PreferencesDialog : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton startWithWizardRadioButton;
		private System.Windows.Forms.Button okButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.CheckBox displayStartupCheckBox;
		private System.Windows.Forms.RadioButton startWithLaunchPadRadioButton;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PreferencesDialog()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// get preferences and set the controls to match
			RegistryHelper helper = RegistryHelper.getInstance();
			bool displayStartup = helper.GetNeedToRunWizardAtStartup();

			this.displayStartupCheckBox.Checked = displayStartup;

			this.startWithLaunchPadRadioButton.Enabled = displayStartup;
			this.startWithWizardRadioButton.Enabled = displayStartup;

			this.startWithLaunchPadRadioButton.Checked = helper.GetWizardToRun().Equals(0);
			this.startWithWizardRadioButton.Checked = helper.GetWizardToRun().Equals(1);
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(PreferencesDialog));
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.displayStartupCheckBox = new System.Windows.Forms.CheckBox();
			this.startWithWizardRadioButton = new System.Windows.Forms.RadioButton();
			this.startWithLaunchPadRadioButton = new System.Windows.Forms.RadioButton();
			this.okButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.startWithLaunchPadRadioButton,
																					this.startWithWizardRadioButton,
																					this.displayStartupCheckBox});
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(288, 80);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "On Startup";
			// 
			// displayStartupCheckBox
			// 
			this.displayStartupCheckBox.Location = new System.Drawing.Point(16, 16);
			this.displayStartupCheckBox.Name = "displayStartupCheckBox";
			this.displayStartupCheckBox.Size = new System.Drawing.Size(160, 24);
			this.displayStartupCheckBox.TabIndex = 0;
			this.displayStartupCheckBox.Text = "Display a startup window";
			this.displayStartupCheckBox.CheckedChanged += new System.EventHandler(this.displayStartupCheckBox_CheckedChanged);
			// 
			// startWithWizardRadioButton
			// 
			this.startWithWizardRadioButton.Location = new System.Drawing.Point(40, 40);
			this.startWithWizardRadioButton.Name = "startWithWizardRadioButton";
			this.startWithWizardRadioButton.Size = new System.Drawing.Size(112, 16);
			this.startWithWizardRadioButton.TabIndex = 1;
			this.startWithWizardRadioButton.Text = "Start with Wizard";
			// 
			// startWithLaunchPadRadioButton
			// 
			this.startWithLaunchPadRadioButton.Location = new System.Drawing.Point(40, 56);
			this.startWithLaunchPadRadioButton.Name = "startWithLaunchPadRadioButton";
			this.startWithLaunchPadRadioButton.Size = new System.Drawing.Size(144, 16);
			this.startWithLaunchPadRadioButton.TabIndex = 2;
			this.startWithLaunchPadRadioButton.Text = "Start with LaunchPad";
			// 
			// okButton
			// 
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(71, 96);
			this.okButton.Name = "okButton";
			this.okButton.TabIndex = 1;
			this.okButton.Text = "OK";
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(159, 96);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.TabIndex = 2;
			this.cancelButton.Text = "Cancel";
			// 
			// PreferencesDialog
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(304, 126);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.cancelButton,
																		  this.okButton,
																		  this.groupBox1});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "PreferencesDialog";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Edit Holodeck Preferences";
			this.TopMost = true;
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void okButton_Click(object sender, System.EventArgs e)
		{
			RegistryHelper helper = RegistryHelper.getInstance();
			if (this.displayStartupCheckBox.Checked)
			{
				helper.SetNeedToRunWizardAtStartup(true);
				helper.SetNeedToRunLaunchPad(this.startWithLaunchPadRadioButton.Checked);
				helper.SetNeedToRunWizard(this.startWithWizardRadioButton.Checked);
			}
			else
			{
				helper.SetNeedToRunWizardAtStartup(false);
			}
		}

		private void displayStartupCheckBox_CheckedChanged(object sender, System.EventArgs e)
		{
			if (this.displayStartupCheckBox.Checked)
			{
				this.startWithLaunchPadRadioButton.Enabled = true;
				this.startWithWizardRadioButton.Enabled = true;
			}
			else
			{
				this.startWithLaunchPadRadioButton.Enabled = false;
				this.startWithWizardRadioButton.Enabled = false;
			}
		}
	}
}
