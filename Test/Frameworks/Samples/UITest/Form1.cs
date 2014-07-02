using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;

namespace UITest
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class HolodeckTestAutomation : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button StartTestButton;
		private System.Windows.Forms.Button CrashHolodeckButton;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public HolodeckTestAutomation()
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
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );

//			Holodeck.HolodeckPane.Stop ();
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.StartTestButton = new System.Windows.Forms.Button();
			this.CrashHolodeckButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// StartTestButton
			// 
			this.StartTestButton.Location = new System.Drawing.Point(16, 24);
			this.StartTestButton.Name = "StartTestButton";
			this.StartTestButton.Size = new System.Drawing.Size(104, 24);
			this.StartTestButton.TabIndex = 4;
			this.StartTestButton.Text = "Start Test";
			this.StartTestButton.Click += new System.EventHandler(this.StartTestButton_Click);
			// 
			// CrashHolodeckButton
			// 
			this.CrashHolodeckButton.Location = new System.Drawing.Point(16, 56);
			this.CrashHolodeckButton.Name = "CrashHolodeckButton";
			this.CrashHolodeckButton.Size = new System.Drawing.Size(104, 24);
			this.CrashHolodeckButton.TabIndex = 5;
			this.CrashHolodeckButton.Text = "Crash Holodeck";
			this.CrashHolodeckButton.Click += new System.EventHandler(this.CrashHolodeckButton_Click);
			// 
			// HolodeckTestAutomation
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 266);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.CrashHolodeckButton,
																		  this.StartTestButton});
			this.Name = "HolodeckTestAutomation";
			this.Text = "HolodeckTestAutomation";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>


		[DllImportAttribute ("user32.dll")]
		public static extern int MessageBoxA (int Modal, string Message, string Caption, int Options);


		[STAThread]
		static void Main () 
		{
			Application.Run (new HolodeckTestAutomation ());
		}

		private void StartTestButton_Click(object sender, System.EventArgs e)
		{
			try 
			{
				Holodeck.HolodeckProcess.Start ();
			} 
			catch (Holodeck.HolodeckExceptions.CannotStartHolodeckException)
			{
				MessageBoxA (0, "Cannot start Holodeck!", "error", 0);
				return;
			}
			catch (Holodeck.HolodeckExceptions.IncorrectRegistryException)
			{
				MessageBoxA (0, "Incorrect registry settings! Try reinstalling Holodeck Enterprise Edition...", "error", 0);
				return;
			}
			catch (Holodeck.HolodeckExceptions.CannotFindHolodeckMainWindowException)
			{
				MessageBoxA (0, "Cannot find Holodeck main window!", "error", 0);
				return;
			}

			Holodeck.HolodeckPane.Reset ();
			Holodeck.HolodeckPane.NewProject ();
			Holodeck.NewProjectPane1.Reset ();
			Holodeck.NewProjectPane1.SetProjectLocation ("C:\\project2.hdp");
			Holodeck.NewProjectPane1.Next ();
			Holodeck.NewProjectPane2.Reset ();
			Holodeck.NewProjectPane2.SetApplicationName ("C:\\WINDOWS\\notepad.exe");
			Holodeck.NewProjectPane2.Next ();
			Holodeck.NewProjectPane3.Reset ();
			Holodeck.NewProjectPane3.SetPauseApplication (true);
			try 
			{
				Holodeck.NewProjectPane3.Finish ();
			}
			catch (Holodeck.HolodeckExceptions.HolodeckCrashedException)
			{
				MessageBoxA (0, "Holodeck crashed!", "error", 0);
				return;
			}
			catch (System.Exception)
			{
				MessageBoxA (0, "Something bad happened!", "error", 0);
				return;
			}
		}

		private void CrashHolodeckButton_Click(object sender, System.EventArgs e)
		{
			try 
			{
				Holodeck.HolodeckProcess.Start ();
			} 
			catch (Holodeck.HolodeckExceptions.CannotStartHolodeckException)
			{
				MessageBoxA (0, "Cannot start Holodeck!", "error", 0);
				return;
			}
			catch (Holodeck.HolodeckExceptions.IncorrectRegistryException)
			{
				MessageBoxA (0, "Incorrect registry settings! Try reinstalling Holodeck Enterprise Edition...", "error", 0);
				return;
			}
			catch (Holodeck.HolodeckExceptions.CannotFindHolodeckMainWindowException)
			{
				MessageBoxA (0, "Cannot find Holodeck main window!", "error", 0);
				return;
			}

			Holodeck.HolodeckPane.Reset ();
			Holodeck.HolodeckPane.NewProject ();
			Holodeck.NewProjectPane1.Reset ();
			Holodeck.NewProjectPane1.SetProjectLocation ("C:\\project2.hdp");
			Holodeck.NewProjectPane1.Next ();
			Holodeck.NewProjectPane2.Reset ();
			Holodeck.NewProjectPane2.SetApplicationName ("C:\\WINDOWS\\notepad.exe");
			Holodeck.NewProjectPane2.Next ();
			Holodeck.NewProjectPane3.Reset ();
			Holodeck.NewProjectPane3.Back ();
			Holodeck.NewProjectPane2.Reset ();
			Holodeck.NewProjectPane2.AttachToApplication ();
			try 
			{
				Holodeck.NewProjectPane2.Finish ();
			}
			catch (Holodeck.HolodeckExceptions.HolodeckCrashedException)
			{
				MessageBoxA (0, "Holodeck crashed!", "error", 0);
				return;
			}
			catch (Holodeck.HolodeckExceptions.UnaccessableControlException) 
			{
				MessageBoxA (0, "Trying to access an unaccessable exception!", "error", 0);
				return;
			}
			catch (System.Exception)
			{
				MessageBoxA (0, "Something bad happened!", "error", 0);
				return;
			}
		}

/*
		private void CloseHolodeckButton_Click (object sender, System.EventArgs e)
		{
			Holodeck.HolodeckProcess.Stop ();
		}
*/
	} // class HolodeckTestAutomation
}
