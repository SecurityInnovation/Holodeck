//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		BalloonTip.cs
//
// DESCRIPTION: Contains classes for balloon tip
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Feb 2004		 Ady K		 File ported from samples on the web
//*************************************************************************

//*************************************************************************
//
//									Usage 
//
//*************************************************************************
//  USING MessageBalloon
//	m_mb = new MessageBalloon();
//	m_mb.Parent = groupBox1;
//	m_mb.Title = "Message Title";
//	m_mb.TitleIcon = TooltipIcon.Info;
//	m_mb.Text = "Message text";
//	BalloonAlignment ba = BalloonAlignment.BottomLeft;
//	m_mb.Align = ba;
//	m_mb.CenterStem = false;
//	m_mb.UseAbsolutePositioning = true;
//	m_mb.Show();
//
//
//  USING HoverBalloon
//  m_hb = new HoverBalloon();
//	m_hb.Title = "Title Text";
//	m_hb.TitleIcon = TooltipIcon.Info;
//	m_hb.SetToolTip(button1, "Message");



using System;
using System.Drawing;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace GuiSupport
{
	public enum TooltipIcon : int
	{
		None,
		Info,
		Warning,
		Error
	}

	public enum BalloonAlignment 
	{
		TopLeft,
		TopMiddle,
		TopRight,
		LeftMiddle,
		RightMiddle,
		BottomLeft,
		BottomMiddle,
		BottomRight,
	}

	public enum BalloonPosition
	{
		Absolute,
		Track
	}

	public delegate void DeActivateEventHandler();

	internal class MessageTool : NativeWindow
	{
		//private const int WM_LBUTTONDOWN = 0x0201;
		//private const int WM_NCLBUTTONDOWN = 0x00A1;
		private const int WM_MOUSEMSG = 132;
		public event DeActivateEventHandler DeActivate;
		
		protected override void WndProc(ref System.Windows.Forms.Message m)
		{
			if(m.Msg == WM_MOUSEMSG)
			{
				// allow the balloon to close if clicked upon
				if(DeActivate!=null)
				{
					DeActivate();
				}
			}

			base.WndProc(ref m);
		}
	}

	public class MessageBalloon : IDisposable
	{
		private MessageTool m_tool = null;
		private Control m_parent;
		private TOOLINFO ti;

		private int m_maxWidth = 250;
		private string m_text = "Control Display Message";
		private string m_title = "Tooltip Message";
		private TooltipIcon m_titleIcon = TooltipIcon.None;
		private BalloonAlignment m_align = BalloonAlignment.TopRight;
		private bool m_absPosn = false;
		private bool m_centerStem = false;

		private const string TOOLTIPS_CLASS = "tooltips_class32";
		private const int WS_POPUP = unchecked((int)0x80000000);
		private const int WM_USER = 0x0400;
		private readonly IntPtr HWND_TOPMOST = new IntPtr(-1);		
		private const int SWP_NOSIZE = 0x0001;
		private const int SWP_NOMOVE = 0x0002;
		private const int SWP_NOACTIVATE = 0x0010;
		private const int SWP_NOZORDER = 0x0004;
		
		[DllImport("User32", SetLastError=true)]
		private static extern int SetWindowPos(
			IntPtr hWnd,
			IntPtr hWndInsertAfter,
			int X,
			int Y,
			int cx,
			int cy,
			int uFlags);

		[DllImport("User32", SetLastError=true)]
		private static extern int GetClientRect(
			IntPtr hWnd,
			ref RECT lpRect);

		[DllImport("User32", SetLastError=true)]
		private static extern int ClientToScreen(
			IntPtr hWnd,
			ref RECT lpRect);

		[DllImport("User32", SetLastError=true)]
		private static extern int SendMessage(
			IntPtr hWnd,
			int Msg,
			int wParam,
			IntPtr lParam);

		[StructLayout(LayoutKind.Sequential)]
			private struct RECT
		{
			public int left;
			public int top;
			public int right;
			public int bottom;
		}

		private const int TTS_ALWAYSTIP = 0x01;
		private const int TTS_NOPREFIX = 0x02;
		private const int TTS_BALLOON = 0x40;
		private const int TTS_CLOSE = 0x80;
		
		private const int TTM_TRACKPOSITION = WM_USER + 18;
		private const int TTM_SETMAXTIPWIDTH = WM_USER + 24;
		private const int TTM_TRACKACTIVATE = WM_USER + 17;
		private const int TTM_ADDTOOL = WM_USER + 50;
		private const int TTM_SETTITLE = WM_USER + 33;

		private const int TTF_IDISHWND = 0x0001;
		private const int TTF_SUBCLASS = 0x0010;
		private const int TTF_TRACK = 0x0020;
		private const int TTF_ABSOLUTE = 0x0080;
		private const int TTF_TRANSPARENT = 0x0100;
		private const int TTF_CENTERTIP = 0x0002;
		private const int TTF_PARSELINKS = 0x1000;

		[StructLayout(LayoutKind.Sequential)]
			private struct TOOLINFO
		{
			public int cbSize;
			public int uFlags;
			public IntPtr hwnd;
			public IntPtr uId;
			public RECT rect;
			public IntPtr hinst;
			[MarshalAs(UnmanagedType.LPTStr)] 
			public string lpszText;
			public uint lParam;
		}
		
		/// <summary>
		/// Creates a new instance of the MessageBalloon.
		/// </summary>
		public MessageBalloon()
		{
			m_tool = new MessageTool();
			m_tool.DeActivate += new DeActivateEventHandler(this.Hide);
		}

		/// <summary>
		/// Creates a new instance of the MessageBalloon.
		/// </summary>
		/// <param name="parent">Set the parent control which will display.</param>
		public MessageBalloon(Control parent)
		{
			m_parent = parent;
			m_tool = new MessageTool();
			m_tool.DeActivate += new DeActivateEventHandler(this.Hide);

		}

		~MessageBalloon()
		{
			Dispose(false);
		}

		private bool disposed = false;
		public void Dispose()
		{
			Dispose(true);
			// Take yourself off the Finalization queue 
			// to prevent finalization code for this object
			// from executing a second time.
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
			if(!this.disposed)
			{
				if(disposing)
				{
					// release managed resources if any
				}
				
				// release unmanaged resource
				Hide();

				// Note that this is not thread safe.
				// Another thread could start disposing the object
				// after the managed resources are disposed,
				// but before the disposed flag is set to true.
				// If thread safety is necessary, it must be
				// implemented by the client.
			}
			disposed = true;
		}

		private void CreateTool()
		{
			System.Diagnostics.Debug.Assert(
				m_parent.Handle!=IntPtr.Zero, 
				"parent hwnd is null", "SetToolTip");

			CreateParams cp = new CreateParams();
			cp.ClassName = TOOLTIPS_CLASS;
			cp.Style = 
				WS_POPUP | 
				TTS_BALLOON | 
				TTS_NOPREFIX |	
				TTS_ALWAYSTIP |
				TTS_CLOSE;

			// create the tool
			m_tool.CreateHandle(cp);

			// create and fill in the tool tip info
			ti = new TOOLINFO();
			ti.cbSize = Marshal.SizeOf(ti);

			ti.uFlags = TTF_TRACK |
				TTF_IDISHWND |
				TTF_TRANSPARENT | 
				TTF_SUBCLASS |
				TTF_PARSELINKS;

			// absolute is used tooltip maynot be shown 
			// if coords exceed the corners of the screen
			if(m_absPosn)
			{
				ti.uFlags |= TTF_ABSOLUTE;
			}

			if(m_centerStem)
			{
				ti.uFlags |= TTF_CENTERTIP;
			}

			ti.uId = m_tool.Handle;
			ti.lpszText = m_text;
			ti.hwnd = m_parent.Handle;

			GetClientRect(m_parent.Handle, ref ti.rect);
			ClientToScreen(m_parent.Handle, ref ti.rect);

			// make sure we make it the top level window
			SetWindowPos(
				m_tool.Handle, 
				HWND_TOPMOST, 
				0, 0, 0, 0,
				SWP_NOACTIVATE | 
				SWP_NOMOVE | 
				SWP_NOSIZE);

			// add the tool tip
			IntPtr ptrStruct = Marshal.AllocHGlobal(Marshal.SizeOf(ti));
			Marshal.StructureToPtr(ti, ptrStruct, true);

			SendMessage(
				m_tool.Handle, TTM_ADDTOOL, 0, ptrStruct);

			ti = (TOOLINFO)Marshal.PtrToStructure(ptrStruct, 
				typeof(TOOLINFO));

			SendMessage(
				m_tool.Handle, TTM_SETMAXTIPWIDTH, 
				0, new IntPtr(m_maxWidth));

			IntPtr ptrTitle = Marshal.StringToHGlobalAuto(m_title);

			SendMessage(
				m_tool.Handle, TTM_SETTITLE, 
				(int)m_titleIcon, ptrTitle);

			SetBalloonPosition(ti.rect);

			Marshal.FreeHGlobal(ptrStruct);
			Marshal.FreeHGlobal(ptrTitle);
		}
		
		private void SetBalloonPosition(RECT rect)
		{
			int x = 0, y = 0;

			// calculate cordinates depending upon aligment
			switch(m_align)
			{
				case BalloonAlignment.TopLeft:
					x = rect.left;
					y = rect.top;
					break;
				case BalloonAlignment.TopMiddle:
					x = rect.left + (rect.right / 2);
					y = rect.top;
					break;
				case BalloonAlignment.TopRight:
					x = rect.left + rect.right;
					y = rect.top;
					break;
				case BalloonAlignment.LeftMiddle:
					x = rect.left;
					y = rect.top + (rect.bottom / 2);
					break;
				case BalloonAlignment.RightMiddle:
					x = rect.left + rect.right;
					y = rect.top + (rect.bottom / 2);
					break;
				case BalloonAlignment.BottomLeft:
					x = rect.left;
					y = rect.top + rect.bottom;
					break;
				case BalloonAlignment.BottomMiddle:
					x = rect.left + (rect.right / 2);
					y = rect.top + rect.bottom;
					break;
				case BalloonAlignment.BottomRight:
					x = rect.left + rect.right;
					y = rect.top + rect.bottom;
					break;
				default:
					System.Diagnostics.Debug.Assert(false, "undefined enum", "default case reached");
					break;
			}

			//int pt = MAKELONG(ti.rect.left, ti.rect.top);
			int pt = MAKELONG(x, y);
			IntPtr ptr = new IntPtr(pt);

			SendMessage(
				m_tool.Handle, TTM_TRACKPOSITION,
				0, ptr);
				
		}

		/// <summary>
		/// Shows or hides the tool.
		/// </summary>
		/// <param name="show">0 to hide, -1 to show</param>
		private void Display(int show)
		{
			IntPtr ptrStruct = Marshal.AllocHGlobal(Marshal.SizeOf(ti));
			Marshal.StructureToPtr(ti, ptrStruct, true);

			SendMessage(
				m_tool.Handle, TTM_TRACKACTIVATE,
				show, ptrStruct);

			Marshal.FreeHGlobal(ptrStruct);
		}

		/// <summary>
		/// Hides the message if visible.
		/// </summary>
		public void Hide()
		{
			Display(0);
			m_tool.DestroyHandle();
		}

		private int MAKELONG(int loWord, int hiWord)
		{
			return (hiWord << 16) | (loWord & 0xffff);
		}

		/// <summary>
		/// Sets or gets the Title.
		/// </summary>
		public string Title
		{
			get
			{
				return m_title;
			}
			set
			{
				m_title = value;
			}
		}

		/// <summary>
		/// Sets or gets the display icon.
		/// </summary>
		public TooltipIcon TitleIcon
		{
			get
			{
				return m_titleIcon;
			}
			set
			{
				m_titleIcon = value;
			}
		}

		/// <summary>
		/// Sets or get the display text.
		/// </summary>
		public string Text
		{
			get
			{
				return m_text;
			}
			set
			{
				m_text = value;
			}
		}

		/// <summary>
		/// Sets or gets the parent.
		/// </summary>
		public Control Parent
		{
			get
			{
				return m_parent;
			}
			set
			{
				m_parent = value;
			}
		}

		/// <summary>
		/// Sets or gets the placement of the balloon.
		/// </summary>
		public BalloonAlignment Align
		{
			get
			{
				return m_align;
			}
			set
			{
				m_align = value;
			}
		}

		/// <summary>
		/// Sets or gets the positioning of the balloon.
		/// TRUE : Positions using the exact co-ordinates,
		/// if the co-ordinates are outside the screen, tip wont be shown.
		/// FALSE : Positions using the co-ordinates as a reference.
		/// Regardless of the co-ordinates, the tip will 
		/// always be shown on the screen.
		/// </summary>
		public bool UseAbsolutePositioning
		{
			get
			{
				return m_absPosn;
			}
			set
			{
				m_absPosn = value;
			}
		}

		/// <summary>
		/// Sets or gets the stem position 
		/// in the tip. 
		/// TRUE : The stem of the tip is set to center.
		/// An attempt is made to show the tip with the stem
		/// centered, if that would make the tip to be 
		/// hidden partly, stem is not centered.
		/// FALSE: Stem is not centered.
		/// </summary>
		public bool CenterStem
		{
			get
			{
				return m_centerStem;
			}
			set
			{
				m_centerStem = value;
			}
		}

		/// <summary>
		/// Show the Message in a balloon tooltip.
		/// </summary>
		public void Show()
		{
			// recreate window always
			Hide();

			CreateTool();
			Display(-1);
		}

	}


	internal class BalloonTool : NativeWindow
	{
	}

	/// <summary>
	/// A sample class to manipulate ballon tooltips.
	/// Windows XP balloon-tips if used properly can 
	/// be very helpful.
	/// This class creates a balloon tooltip.
	/// This becomes useful for showing important information 
	/// quickly to the user.
	/// For example in a data-entry form full of 
	/// controls the most important
	/// and used control is the Order Placement button.
	/// Guide the user by using this hover balloon on it.
	/// This helps in a shorter learning cycle of the 
	/// application.
	/// </summary>
	public class HoverBalloon : IDisposable
	{
		private BalloonTool m_tool = null;
		
		private int m_maxWidth = 250;
		private string m_displayText = "Control Display text";
		private string m_title = "HoverBalloon";
		private TooltipIcon m_titleIcon = TooltipIcon.None;

		private const string TOOLTIPS_CLASS = "tooltips_class32";
		private const int WS_POPUP = unchecked((int)0x80000000);
		private const int SWP_NOSIZE = 0x0001;
		private const int SWP_NOMOVE = 0x0002;
		private const int SWP_NOACTIVATE = 0x0010;
		private const int WM_USER = 0x0400;
		private static readonly IntPtr HWND_TOPMOST = new IntPtr(-1);

		[StructLayout(LayoutKind.Sequential)]
		private struct TOOLINFO
		{
			public int cbSize;
			public int uFlags;
			public IntPtr hwnd;
			public IntPtr uId;
			public Rectangle rect;
			public IntPtr hinst;
			[MarshalAs(UnmanagedType.LPTStr)] 
			public string lpszText;
			public uint lParam;
		}
		private const int TTS_ALWAYSTIP = 0x01;
		private const int TTS_NOPREFIX = 0x02;
		private const int TTS_BALLOON = 0x40;
		private const int TTF_SUBCLASS = 0x0010;
		private const int TTF_TRANSPARENT = 0x0100;
		private const int TTM_ADDTOOL = WM_USER + 50;
		private const int TTM_SETMAXTIPWIDTH = WM_USER + 24;
		private const int TTM_SETTITLE = WM_USER + 33;

		[DllImport("User32", SetLastError=true)]
		private static extern bool SetWindowPos(
			IntPtr hWnd,
			IntPtr hWndInsertAfter,
			int X,
			int Y,
			int cx,
			int cy,
			int uFlags);
		[DllImport("User32", SetLastError=true)]
		private static extern int GetClientRect(
			IntPtr hWnd,
			ref Rectangle lpRect);
		[DllImport("User32", SetLastError=true)]
		private static extern int SendMessage(
			IntPtr hWnd,
			int Msg,
			int wParam,
			IntPtr lParam);

		public HoverBalloon()
		{
			m_tool = new BalloonTool();
		}

		private bool disposed = false;
		public void Dispose()
		{
			Dispose(true);
			// Take yourself off the Finalization queue 
			// to prevent finalization code for this object
			// from executing a second time.
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
			if(!this.disposed)
			{
				if(disposing)
				{
					// release managed resources if any
				}
				
				// release unmanaged resource
				m_tool.DestroyHandle();

				// Note that this is not thread safe.
				// Another thread could start disposing the object
				// after the managed resources are disposed,
				// but before the disposed flag is set to true.
				// If thread safety is necessary, it must be
				// implemented by the client.
			}
			disposed = true;
		}
		
		// Finalizer 
		~HoverBalloon()      
		{
			Dispose(false);
		}

		public void SetToolTip(Control parent, string tipText)
		{
			System.Diagnostics.Debug.Assert(parent.Handle!=IntPtr.Zero, "parent hwnd is null", "SetToolTip");
			
			m_displayText = tipText;
			
			CreateParams cp = new CreateParams();
			cp.ClassName = TOOLTIPS_CLASS;
			cp.Style = WS_POPUP | TTS_BALLOON | TTS_NOPREFIX |	TTS_ALWAYSTIP;

			cp.Parent = parent.Handle;

			// create the tool
			m_tool.CreateHandle(cp);

			// make sure we make it the top level window
			SetWindowPos(
				m_tool.Handle, 
				HWND_TOPMOST, 
				0, 0, 0, 0,
				SWP_NOACTIVATE | 
				SWP_NOMOVE | 
				SWP_NOSIZE);

			// create and fill in the tool tip info
			TOOLINFO ti = new TOOLINFO();
			ti.cbSize = Marshal.SizeOf(ti);
			ti.uFlags = TTF_TRANSPARENT | TTF_SUBCLASS;
			ti.hwnd = parent.Handle;
			ti.lpszText = m_displayText;
			
			// get the display co-ordinates
			GetClientRect(parent.Handle, ref ti.rect);

			// add the tool tip
			IntPtr ptrStruct = Marshal.AllocHGlobal(Marshal.SizeOf(ti));
			Marshal.StructureToPtr(ti, ptrStruct, true);

			SendMessage(
				m_tool.Handle, 
				TTM_ADDTOOL, 
				0, 
				ptrStruct);

			ti = (TOOLINFO)Marshal.PtrToStructure(ptrStruct, typeof(TOOLINFO));

			Marshal.FreeHGlobal(ptrStruct);

			SendMessage(
				m_tool.Handle, 
				TTM_SETMAXTIPWIDTH, 
				0, 
				new IntPtr(m_maxWidth));


			if(m_title != null || m_title!=string.Empty)
			{
				IntPtr ptrTitle = Marshal.StringToHGlobalAuto(m_title);

				SendMessage(
					m_tool.Handle, TTM_SETTITLE, 
					(int)m_titleIcon, ptrTitle);

				Marshal.FreeHGlobal(ptrTitle);
			}


		}



		/// <summary>
		/// Sets the maximum text width of the tooltip text.
		/// At that length the tooltip will start wrapping text 
		/// over to the next line. 
		/// </summary>
		public int MaximumTextWidth
		{
			get
			{
				return m_maxWidth;
			}
			set
			{
				m_maxWidth = value;
			}
		}

		/// <summary>
		/// Sets up the title of the tooltip.
		/// </summary>
		public string Title
		{
			get
			{
				return m_title;
			}
			set
			{
				m_title = value;
			}
		}

		/// <summary>
		/// Sets the icon for the tooltip.
		/// </summary>
		public TooltipIcon TitleIcon
		{
			get
			{
				return m_titleIcon;
			}
			set
			{
				m_titleIcon = value;
			}
		}

	}

	public class EditBalloon 
	{
		private Control m_parent;

		private string m_text = "Control Display Message";
		private string m_title = "Tooltip Message";
		private TooltipIcon m_titleIcon = TooltipIcon.None;

		private const int ECM_FIRST = 0x1500;
		private const int EM_SHOWBALLOONTIP = ECM_FIRST + 3;

		[DllImport("User32", SetLastError=true)]
		private static extern int SendMessage(
			IntPtr hWnd,
			int Msg,
			int wParam,
			IntPtr lParam);

		[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
			private struct EDITBALLOONTIP
		{
			public int cbStruct;
			public string pszTitle;
			public string pszText;
			public int ttiIcon;
		}

		public EditBalloon()
		{
		}

		public EditBalloon(Control parent)
		{
			m_parent = parent;
		}

		/// <summary>
		/// Show a balloon tooltip for edit control.
		/// </summary>
		public void Show()
		{
			EDITBALLOONTIP ebt = new EDITBALLOONTIP();

			ebt.cbStruct = Marshal.SizeOf(ebt);
			ebt.pszText = m_text;
			ebt.pszTitle = m_title;
			ebt.ttiIcon = (int)m_titleIcon;

			IntPtr ptrStruct = Marshal.AllocHGlobal(Marshal.SizeOf(ebt));
			Marshal.StructureToPtr(ebt, ptrStruct, true);

			System.Diagnostics.Debug.Assert(m_parent!=null, "Parent control is null", "Set parent before calling Show");

			int ret = SendMessage(m_parent.Handle, 
				EM_SHOWBALLOONTIP,
				0, ptrStruct);

			Marshal.FreeHGlobal(ptrStruct);
		}

		/// <summary>
		/// Sets or gets the Title.
		/// </summary>
		public string Title
		{
			get
			{
				return m_title;
			}
			set
			{
				m_title = value;
			}
		}

		/// <summary>
		/// Sets or gets the display icon.
		/// </summary>
		public TooltipIcon TitleIcon
		{
			get
			{
				return m_titleIcon;
			}
			set
			{
				m_titleIcon = value;
			}
		}

		/// <summary>
		/// Sets or gets the display text.
		/// </summary>
		public string Text
		{
			get
			{
				return m_text;
			}
			set
			{
				m_text = value;
			}
		}

		/// <summary>
		/// Sets or gets the parent.
		/// </summary>
		public Control Parent
		{
			get
			{
				return m_parent;
			}
			set
			{
				m_parent = value;
			}
		}

	}


}
