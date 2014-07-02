//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		VirtualListView.cs
//
// DESCRIPTION: Contains the VirtualListView class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 May 2003		 B. Shirey	 File created.
//*************************************************************************
using System;
using System.Windows.Forms;
using System.Diagnostics;

namespace GuiSupport
{
    public delegate void QueryItemTextHandler(int item, int subItem, out string text);
    public delegate void QueryItemImageHandler(int item, int subItem, out int imageIndex);
    public delegate void QueryItemIndentHandler(int item, out int itemIndent);

    public class VirtualListView  : ListView 
	{
        // store the item count to prevent the call to SendMessage(LVM_GETITEMCOUNT)
        private int itemCount = 0;
		public event QueryItemTextHandler QueryItemText;
		public event QueryItemImageHandler QueryItemImage;
		public event QueryItemIndentHandler QueryItemIndent;

		//*************************************************************************
		// ItemCount property
		//*************************************************************************
        public int ItemCount 
		{
            get { return itemCount; }
            set 
			{
                itemCount = value;
                int result;
                result = WindowsFunction.SendMessage(this.Handle, (int)ListViewMessages.LVM_SETITEMCOUNT, itemCount, 3);
            }
        }

		//*************************************************************************
		// SelectedItem property
		//*************************************************************************
		public int SelectedItem
		{
			set 
			{
				int result;
				LVITEM lvItem = new LVITEM();
				lvItem.iItem = value;
				lvItem.mask = (uint) ListViewItemMask.LVIF_STATE;
				lvItem.state = (uint) ListViewStates.LVIS_SELECTED;
				lvItem.stateMask = (uint) ListViewStates.LVIS_SELECTED;
				result = WindowsFunction.SendMessage(this.Handle, (int)ListViewMessages.LVM_SETITEMSTATE, value, ref lvItem);
				result = WindowsFunction.SendMessage (this.Handle, (int)ListViewMessages.LVM_ENSUREVISIBLE, value, false); 
			}
		}

		//*************************************************************************
		// CreateParams property
		//*************************************************************************
		protected override System.Windows.Forms.CreateParams CreateParams 
		{
			get 
			{
				CreateParams cp = base.CreateParams;
				// LVS_OWNERDATA style must be set when the control is created
				cp.Style |= (int)ListViewStyles.LVS_OWNERDATA;
				return cp;
			}
		}

		//*************************************************************************
		// View property
		//*************************************************************************
		public new System.Windows.Forms.View View 
		{
			get { return new System.Windows.Forms.View(); }
			set 
			{
				if (value == View.LargeIcon || value == View.SmallIcon) 
				{
					throw new ArgumentException("Icon views are invalid for virtual ListViews", "View");
				}
				base.View = value;
			}
		}
        
		//*************************************************************************
		// Method:		VirtualListView
		// Description: Constructor for the VirtualListView class
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
        public VirtualListView ()
		{
            // virtual listviews must be Details or List view with no sorting
            View = View.Details;
            Sorting = SortOrder.None;
		}

		//*************************************************************************
		// Method:		OnDispInfoNotice
		// Description: called when the control is notified of display info changes
		//
		// Parameters:
		//	m - message sent to the control
		//
		// Return Value: None
		//*************************************************************************
        void OnDispInfoNotice(ref Message m) 
		{
            LVDISPINFO info = (LVDISPINFO)m.GetLParam(typeof(LVDISPINFO)); 
            string lvtext = null;
          
            if ((info.item.mask & (uint)ListViewItemMask.LVIF_TEXT) > 0) 
			{
                if (QueryItemText != null) 
				{
                    QueryItemText(info.item.iItem, info.item.iSubItem, out lvtext);
                    if (lvtext != null) 
					{
                        try 
						{
							int maxIndex = Math.Min(info.item.cchTextMax-1, lvtext.Length);
							char[] data = new char[maxIndex+1];
							lvtext.CopyTo(0, data, 0, lvtext.Length);
							data[maxIndex] = '\0';
							System.Runtime.InteropServices.Marshal.Copy(data, 0, info.item.pszText, data.Length);
                        }
						catch (Exception e) 
						{
                            Debug.WriteLine("Failed to copy text name from client: " + e.ToString(), "VirtualListView.OnDispInfoNotice");
                        }
                    }
                }
            }

            if((info.item.mask & (uint)ListViewItemMask.LVIF_IMAGE) > 0) 
			{
                int imageIndex = 0;
                if (QueryItemImage != null) 
				{
                    QueryItemImage(info.item.iItem, info.item.iSubItem, out imageIndex);
                }
                info.item.iImage = imageIndex;
            }

            if ((info.item.mask & (uint)ListViewItemMask.LVIF_INDENT) > 0) 
			{
                int itemIndent = 0;
                if (QueryItemIndent != null) 
				{
                    QueryItemIndent(info.item.iItem, out itemIndent);
                }
                info.item.iIndent = itemIndent;
            }

            m.Result = new IntPtr(0);
        }

		//*************************************************************************
		// Method:		WndProc
		// Description: Handles window messages to the listview
		//
		// Parameters:
		//	m - the message sent to the control
		//
		// Return Value: None
		//*************************************************************************
        protected override void WndProc(ref System.Windows.Forms.Message m) 
		{
            NMHDR nm1;
            bool messageProcessed = false;
            switch (m.Msg) 
			{
                case (int)WindowsMessage.WM_REFLECT + (int)WindowsMessage.WM_NOTIFY:
                    nm1 = (NMHDR) m.GetLParam(typeof(NMHDR));
                    switch(nm1.code) 
					{
                        case (int)ListViewNotices.LVN_GETDISPINFOW:
                            OnDispInfoNotice(ref m);
                            messageProcessed = true;
                            break;
						case (int)ListViewNotices.LVN_BEGINDRAG:
						case (int)ListViewNotices.LVN_BEGINRDRAG:
							messageProcessed = true;
							break;
                        default:
                            break;
                    }
                    break;
				case (int)WindowsMessage.WM_DESTROY:
					messageProcessed = true;
					break;
                default:
                    break;
            }

            if (!messageProcessed) 
			{
				try
				{
					base.WndProc(ref m);
				}
				catch (ArgumentOutOfRangeException)
				{
					// vs.net 2003 and later throws an exception on get and set focus
					// but just catching it solves the problem
				}

            }
        }
    }
}
