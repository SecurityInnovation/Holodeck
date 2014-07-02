//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		FormStateAndSizeSaver.cs
//
// DESCRIPTION: Contains the class FormStatusAndSizeSaver
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 May 2004		 B Shirey	 Created
//*************************************************************************
using System;
using Microsoft.Win32;
using System.Drawing;
using System.Windows.Forms;

namespace GuiSupport
{
	//*********************************************************************
	// class to easily save and restore a form's size and state
	//*********************************************************************
	public class FormStateAndSizeSaver
	{
		public FormStateAndSizeSaver()
		{
		}

		// event info that allows form to persist extra window state data
		public delegate void WindowStateDelegate(object sender, RegistryKey key);
		public event WindowStateDelegate LoadStateEvent;
		public event WindowStateDelegate SaveStateEvent;

		private Form parent;
		private string regPath;
		private int normalLeft;
		private int normalTop;
		private int normalWidth;
		private int normalHeight;
		private FormWindowState windowState;
		private bool allowSaveMinimized = false;

		public Form Parent
		{
			set
			{
				parent = value;

				// subscribe to parent form's events
				parent.Closing += new System.ComponentModel.CancelEventHandler(OnClosing);
				parent.Resize += new System.EventHandler(OnResize);
				parent.Move += new System.EventHandler(OnMove);
				parent.Load += new System.EventHandler(OnLoad);

				// get initial width and height in case form is never resized
				normalWidth = parent.Width;
				normalHeight = parent.Height;
			}
			get
			{
				return parent;
			}
		}

		// registry key should be set in parent form's constructor
		public string RegistryPath
		{
			set
			{
				regPath = value;        
			}
			get
			{
				return regPath;
			}
		}

		public bool AllowSaveMinimized
		{
			set
			{
				allowSaveMinimized = value;
			}
		}

		private void OnResize(object sender, System.EventArgs e)
		{
			// save width and height
			if(parent.WindowState == FormWindowState.Normal)
			{
				normalWidth = parent.Width;
				normalHeight = parent.Height;
			}
		}

		private void OnMove(object sender, System.EventArgs e)
		{
			// save position
			if(parent.WindowState == FormWindowState.Normal)
			{
				normalLeft = parent.Left;
				normalTop = parent.Top;
			}
			// save state
			windowState = parent.WindowState;
		}

		private void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			try
			{
				// save position, size and state
				RegistryKey key = Registry.LocalMachine.CreateSubKey(regPath);
				key.SetValue("Left", normalLeft);
				key.SetValue("Top", normalTop);
				key.SetValue("Width", normalWidth);
				key.SetValue("Height", normalHeight);

				// check if we are allowed to save the state as minimized (not normally)
				if(!allowSaveMinimized)
				{
					if(windowState == FormWindowState.Minimized)
						windowState = FormWindowState.Normal;
				}

				key.SetValue("WindowState", (int)windowState);
	        
				// fire SaveState event
				if(SaveStateEvent != null)
					SaveStateEvent(this, key);
			}
			catch(Exception)
			{
			}
		}

		private void OnLoad(object sender, System.EventArgs e)
		{
			try
			{
				// attempt to read state from registry
				RegistryKey key = Registry.LocalMachine.OpenSubKey(regPath);
				if(key != null)
				{
					int left = (int)key.GetValue("Left", parent.Left);
					int top = (int)key.GetValue("Top", parent.Top);
					int width = (int)key.GetValue("Width", parent.Width);
					int height = (int)key.GetValue("Height", parent.Height);
					FormWindowState windowState = (FormWindowState)key.GetValue("WindowState", (int)parent.WindowState);

					parent.Location = new Point(left, top);
					parent.Size = new Size(width, height);
					parent.WindowState = windowState;

					// fire LoadState event
					if(LoadStateEvent != null)
						LoadStateEvent(this, key);
				}
			}
			catch(Exception)
			{
			}
		}
	}
}
