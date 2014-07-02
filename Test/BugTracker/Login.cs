using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace BugTracker
{
	/// <summary>
	/// Summary description for Login.
	/// </summary>
	public class Login : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox userNameTextBox;
		private System.Windows.Forms.TextBox passwordTextBox;
		private System.Windows.Forms.Button loginButton;
		private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox serverComboBox;

		private MainWindow parentWindow;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Login( MainWindow mainWindow )
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();						

			parentWindow = mainWindow;

			foreach( string server in mainWindow.servers )
			{
				serverComboBox.Items.Add( server );
			}
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.userNameTextBox = new System.Windows.Forms.TextBox();
			this.passwordTextBox = new System.Windows.Forms.TextBox();
			this.loginButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			this.label4 = new System.Windows.Forms.Label();
			this.serverComboBox = new System.Windows.Forms.ComboBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(376, 32);
			this.label1.TabIndex = 0;
			this.label1.Text = "You must provide login information to access the database. Please enter your cred" +
				"entials below:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(48, 56);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(72, 16);
			this.label2.TabIndex = 1;
			this.label2.Text = "User Name:";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(48, 88);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(88, 24);
			this.label3.TabIndex = 2;
			this.label3.Text = "Password:";
			// 
			// userNameTextBox
			// 
			this.userNameTextBox.Location = new System.Drawing.Point(144, 56);
			this.userNameTextBox.Name = "userNameTextBox";
			this.userNameTextBox.Size = new System.Drawing.Size(184, 20);
			this.userNameTextBox.TabIndex = 1;
			this.userNameTextBox.Text = "";
			// 
			// passwordTextBox
			// 
			this.passwordTextBox.Location = new System.Drawing.Point(144, 88);
			this.passwordTextBox.Name = "passwordTextBox";
			this.passwordTextBox.PasswordChar = '*';
			this.passwordTextBox.Size = new System.Drawing.Size(184, 20);
			this.passwordTextBox.TabIndex = 2;
			this.passwordTextBox.Text = "";
			// 
			// loginButton
			// 
			this.loginButton.Location = new System.Drawing.Point(56, 160);
			this.loginButton.Name = "loginButton";
			this.loginButton.Size = new System.Drawing.Size(96, 24);
			this.loginButton.TabIndex = 4;
			this.loginButton.Text = "Login";
			this.loginButton.Click += new System.EventHandler(this.loginButton_Click);
			// 
			// cancelButton
			// 
			this.cancelButton.Location = new System.Drawing.Point(224, 160);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(96, 24);
			this.cancelButton.TabIndex = 5;
			this.cancelButton.Text = "Cancel";
			this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(48, 120);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(88, 24);
			this.label4.TabIndex = 7;
			this.label4.Text = "Server:";
			// 
			// serverComboBox
			// 
			this.serverComboBox.Location = new System.Drawing.Point(144, 120);
			this.serverComboBox.Name = "serverComboBox";
			this.serverComboBox.Size = new System.Drawing.Size(184, 21);
			this.serverComboBox.TabIndex = 3;
			// 
			// Login
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(392, 206);
			this.Controls.Add(this.serverComboBox);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.loginButton);
			this.Controls.Add(this.passwordTextBox);
			this.Controls.Add(this.userNameTextBox);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "Login";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Login";
			this.ResumeLayout(false);

		}
		#endregion

		private void cancelButton_Click(object sender, System.EventArgs e)
		{
			this.Close( );
		}

		private void loginButton_Click(object sender, System.EventArgs e)
		{
			MainWindow mainWindow = (MainWindow)parentWindow;

			mainWindow.username = userNameTextBox.Text;
			mainWindow.password = passwordTextBox.Text;
			mainWindow.server = serverComboBox.Text;
			
			// Connect to the database server...
			if ( mainWindow.connectToServer( ) )			
				this.Close( );
		}
	}
}
