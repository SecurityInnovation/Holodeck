using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace GuiSupport
{
	public delegate void ScrollDelegate(int amount);

	/// <summary>
	/// Summary description for LogListView.
	/// </summary>
	public class LogListView : System.Windows.Forms.UserControl
	{
		public ScrollDelegate OnScrollUp;
		public ScrollDelegate OnScrollDown;
		public ScrollDelegate OnScrollLeft;
		public ScrollDelegate OnScrollRight;

		private System.Windows.Forms.ListView scrollableListView;
		private System.Windows.Forms.ColumnHeader Header_TimeStamp;
		private System.Windows.Forms.ColumnHeader Header_Category;
		private System.Windows.Forms.ColumnHeader Header_DLL;
		private System.Windows.Forms.ColumnHeader Header_Function;
		private System.Windows.Forms.ColumnHeader Header_ReturnValue;
		private System.Windows.Forms.ColumnHeader Header_ErrorCode;
		private System.Windows.Forms.ColumnHeader Header_Parameters;
		private System.Windows.Forms.ColumnHeader Header_Extra;
		private ToolTip lv_ToolTip;
		private System.Windows.Forms.ContextMenu createTestPopupMenu;
		private System.Windows.Forms.MenuItem createTestMenuItem;
		private System.Drawing.Point lastRightClickPoint;
		private ProcessMdiWindow parentWindow;
		private System.Windows.Forms.VScrollBar verticalScrollBar;
		private System.Windows.Forms.HScrollBar horizontalScrollBar;
		private System.ComponentModel.Container components = null;

		#region Properties

		public ListView.ListViewItemCollection Items
		{
			get { return scrollableListView.Items; }
		}

		public ListView.SelectedListViewItemCollection SelectedItems
		{
			get { return scrollableListView.SelectedItems; }
		}

		public int MaximumVerticalRange
		{
			get { return verticalScrollBar.Maximum; }
			set { verticalScrollBar.Maximum = value; }
		}

		public int MaximumHorizontalRange
		{
			get { return horizontalScrollBar.Maximum; }
			set { horizontalScrollBar.Maximum = value; }
		}

		public int CurrentVerticalPosition
		{
			get { return verticalScrollBar.Value; }
			set { verticalScrollBar.Value = value; }
		}

		public int CurrentHorizontalPosition
		{
			get { return horizontalScrollBar.Value; }
			set { horizontalScrollBar.Value = value; }
		}

		public Rectangle Viewport
		{
			get { return scrollableListView.ClientRectangle; }
		}

		public bool VerticalScrollingEnabled
		{
			get { return verticalScrollBar.Enabled; }
			set { verticalScrollBar.Enabled = value; }
		}

		public bool HorizontalScrollingEnabled
		{
			get { return horizontalScrollBar.Enabled; }
			set { horizontalScrollBar.Enabled = value; }
		}

		#endregion

		public LogListView(ProcessMdiWindow parent)
		{
			this.parentWindow = parent;

			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			lv_ToolTip = new ToolTip();
			lv_ToolTip.AutoPopDelay = 4000;
			lv_ToolTip.InitialDelay = 500;
			lv_ToolTip.ReshowDelay = 500;
			lv_ToolTip.ShowAlways = true;
		}

		public Rectangle GetItemRectangle(int index)
		{
			if ((index < 0) || (index >= this.scrollableListView.Items.Count))
				return new Rectangle(0, 0, 0, 0);

			return this.scrollableListView.GetItemRect(index);
		}

		public void RepaintLogList()
		{
			this.scrollableListView.Invalidate();
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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.scrollableListView = new System.Windows.Forms.ListView();
			this.Header_TimeStamp = new System.Windows.Forms.ColumnHeader();
			this.Header_Category = new System.Windows.Forms.ColumnHeader();
			this.Header_DLL = new System.Windows.Forms.ColumnHeader();
			this.Header_Function = new System.Windows.Forms.ColumnHeader();
			this.Header_ReturnValue = new System.Windows.Forms.ColumnHeader();
			this.Header_ErrorCode = new System.Windows.Forms.ColumnHeader();
			this.Header_Parameters = new System.Windows.Forms.ColumnHeader();
			this.Header_Extra = new System.Windows.Forms.ColumnHeader();
			this.verticalScrollBar = new System.Windows.Forms.VScrollBar();
			this.createTestPopupMenu = new System.Windows.Forms.ContextMenu();
			this.createTestMenuItem = new System.Windows.Forms.MenuItem();
			this.horizontalScrollBar = new System.Windows.Forms.HScrollBar();
			this.SuspendLayout();
			// 
			// scrollableListView
			// 
			this.scrollableListView.AccessibleRole = System.Windows.Forms.AccessibleRole.None;
			this.scrollableListView.Anchor = (((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right);
			this.scrollableListView.AutoArrange = true;
			this.scrollableListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																								 this.Header_TimeStamp,
																								 this.Header_Category,
																								 this.Header_DLL,
																								 this.Header_Function,
																								 this.Header_ReturnValue,
																								 this.Header_ErrorCode,
																								 this.Header_Parameters,
																								 this.Header_Extra});
			this.scrollableListView.FullRowSelect = true;
			this.scrollableListView.HideSelection = false;
			this.scrollableListView.ImeMode = System.Windows.Forms.ImeMode.Off;
			this.scrollableListView.LabelWrap = false;
			this.scrollableListView.MultiSelect = false;
			this.scrollableListView.Name = "scrollableListView";
			this.scrollableListView.Scrollable = false;

			// leave the width below large.  Since scrollable is false this is the 
			// only way for the list view to look right when resized.  Not sure why.
			this.scrollableListView.Size = new System.Drawing.Size(3000, 323);
			this.scrollableListView.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.scrollableListView.TabIndex = 2;
			this.scrollableListView.View = System.Windows.Forms.View.Details;
			this.scrollableListView.DoubleClick += new System.EventHandler(this.scrollableListView_DoubleClick);
			this.scrollableListView.MouseUp += new System.Windows.Forms.MouseEventHandler(this.scrollableListView_MouseUp);
			this.scrollableListView.MouseHover += new System.EventHandler(this.scrollableListView_MouseHover);
			// 
			// Header_TimeStamp
			// 
			this.Header_TimeStamp.Text = "TimeStamp";
			this.Header_TimeStamp.Width = 130;
			// 
			// Header_Category
			// 
			this.Header_Category.Text = "Category";
			this.Header_Category.Width = 80;
			// 
			// Header_DLL
			// 
			this.Header_DLL.Text = "Dll";
			this.Header_DLL.Width = 80;
			// 
			// Header_Function
			// 
			this.Header_Function.Text = "Function";
			this.Header_Function.Width = 80;
			// 
			// Header_ReturnValue
			// 
			this.Header_ReturnValue.Text = "Return Value";
			this.Header_ReturnValue.Width = 80;
			// 
			// Header_ErrorCode
			// 
			this.Header_ErrorCode.Text = "Error Code";
			this.Header_ErrorCode.Width = 80;
			// 
			// Header_Parameters
			// 
			this.Header_Parameters.Text = "Parameters";
			this.Header_Parameters.Width = 250;
			//
			// Header_Extra
			//
			this.Header_Extra.Text = "";
			this.Header_Extra.Width = -2;
			// 
			// verticalScrollBar
			// 
			this.verticalScrollBar.Dock = System.Windows.Forms.DockStyle.Right;
			this.verticalScrollBar.Enabled = false;
			this.verticalScrollBar.Location = new System.Drawing.Point(767, 0);
			this.verticalScrollBar.Maximum = 10;
			this.verticalScrollBar.Name = "verticalScrollBar";
			this.verticalScrollBar.Size = new System.Drawing.Size(17, 340);
			this.verticalScrollBar.TabIndex = 1;
			this.verticalScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.verticalScrollBar_Scroll);
			// 
			// createTestPopupMenu
			// 
			this.createTestPopupMenu.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																								this.createTestMenuItem});
			// 
			// createTestMenuItem
			// 
			this.createTestMenuItem.Index = 0;
			this.createTestMenuItem.Text = "Create a New Test";
			this.createTestMenuItem.Click += new System.EventHandler(this.createTestMenuItem_Click);
			// 
			// horizontalScrollBar
			// 
			this.horizontalScrollBar.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.horizontalScrollBar.Enabled = false;
			this.horizontalScrollBar.Location = new System.Drawing.Point(0, 323);
			this.horizontalScrollBar.Maximum = 10;
			this.horizontalScrollBar.Name = "horizontalScrollBar";
			this.horizontalScrollBar.Size = new System.Drawing.Size(767, 17);
			this.horizontalScrollBar.TabIndex = 3;
			this.horizontalScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.horizontalScrollBar_Scroll);
			// 
			// LogListView
			// 
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.horizontalScrollBar,
																		  this.verticalScrollBar,
																		  this.scrollableListView});
			this.Name = "LogListView";
			this.Size = new System.Drawing.Size(784, 340);
			this.ResumeLayout(false);

		}
		#endregion

		#region Event Handlers

		private void createTestMenuItem_Click(object sender, System.EventArgs e)
		{	
			if (parentWindow != null)
				parentWindow.ShowCreateTestWindow();
		}

		private void scrollableListView_DoubleClick(object sender, System.EventArgs e)
		{
			createTestMenuItem_Click(sender, e);
		}

		private void scrollableListView_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (e.Button == System.Windows.Forms.MouseButtons.Right)
			{
				lastRightClickPoint = new System.Drawing.Point(e.X, e.Y);
				createTestPopupMenu.Show(this, lastRightClickPoint);
			}
		}

		//Displays parameter information in a tooltip on hover.
		private void scrollableListView_MouseHover(object sender, System.EventArgs e)
		{
			Point currentPosition = MousePosition;
			String	toolTipString;

			currentPosition = this.scrollableListView.PointToClient(currentPosition);
			ListViewItem lvi_CurrentItem = this.scrollableListView.GetItemAt(currentPosition.X, currentPosition.Y);
			if (lvi_CurrentItem != null)
				toolTipString = lvi_CurrentItem.SubItems[6].Text;
			else
				toolTipString = null;
			
			lv_ToolTip.SetToolTip(this.scrollableListView, toolTipString);
		}

		private void verticalScrollBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			if (e.NewValue < 0)
				e.NewValue = 0;
			else if (e.NewValue > this.verticalScrollBar.Maximum)
				e.NewValue = this.verticalScrollBar.Maximum;

			if (e.NewValue > this.verticalScrollBar.Value)
				OnScrollDown(e.NewValue - this.verticalScrollBar.Value);
			else if (e.NewValue < this.verticalScrollBar.Value)
				OnScrollUp(this.verticalScrollBar.Value - e.NewValue);
		}

		private void horizontalScrollBar_Scroll(object sender, System.Windows.Forms.ScrollEventArgs e)
		{
			if (e.NewValue < 0)
				e.NewValue = 0;
			else if (e.NewValue > this.horizontalScrollBar.Maximum)
				e.NewValue = this.horizontalScrollBar.Maximum;

			if (e.NewValue > this.horizontalScrollBar.Value)
				OnScrollRight(e.NewValue - this.horizontalScrollBar.Value);
			else if (e.NewValue < this.horizontalScrollBar.Value)
				OnScrollLeft(this.horizontalScrollBar.Value - e.NewValue);
		}

		private void HeaderResized(int newHeaderWidth)
		{
			int difference = newHeaderWidth - this.Width;
			if (difference > 0)
			{
				this.MaximumHorizontalRange = difference;
				this.HorizontalScrollingEnabled = true;
			}
			else
			{
				this.MaximumHorizontalRange = 0;
				this.HorizontalScrollingEnabled = false;
			}
		}

		#endregion
	}
}
