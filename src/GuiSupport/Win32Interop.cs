using System;
using System.Runtime.InteropServices;

namespace GuiSupport 
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct LVDISPINFO 
	{
        public NMHDR hdr;
        public LVITEM item;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct NMHDR 
	{
        public IntPtr hwndFrom;
        public int idFrom;
        public int code;
    }

    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Unicode)]
    internal struct LVITEM 
	{
        public uint mask;
        public int iItem;
        public int iSubItem;
        public uint state;
        public uint stateMask;
        public IntPtr pszText;
        public int cchTextMax;
        public int iImage;
        public IntPtr lParam;
        //#if (_WIN32_IE >= 0x0300)
        public int iIndent;
        //#endif
        //#if (_WIN32_WINNT >= 0x501)
        public int iGroupId;
        public uint cColumns;
        public IntPtr puColumns;
        //#endif
    }

	internal enum ListViewStates : short
	{
		LVIS_FOCUSED            = 0x0001,
		LVIS_SELECTED           = 0x0002,
		LVIS_CUT                = 0x0004,
		LVIS_DROPHILITED        = 0x0008,
		LVIS_GLOW               = 0x0010,
		LVIS_ACTIVATING         = 0x0020
	}

	internal enum ListViewItemMask : short 
	{
        LVIF_TEXT              = 0x0001,
        LVIF_IMAGE             = 0x0002,
        LVIF_PARAM             = 0x0004,
        LVIF_STATE             = 0x0008,

        LVIF_INDENT            = 0x0010,
        LVIF_NORECOMPUTE       = 0x0800,
        LVIF_GROUPID           = 0x0100,
        LVIF_COLUMNS           = 0x0200
    }

    internal enum ListViewMessages 
	{
        LVM_FIRST           = 0x1000,
        LVM_GETITEMCOUNT    = (LVM_FIRST + 4), 
        LVM_GETITEMSTATE    = (LVM_FIRST + 44),
        LVM_GETSUBITEMRECT  = (LVM_FIRST + 56),
        LVM_SETITEMCOUNT    = (LVM_FIRST + 47),
		LVM_SETITEMSTATE	= (LVM_FIRST + 43),
		LVM_ENSUREVISIBLE	= (LVM_FIRST + 19)

    }

    internal enum ListViewStyles : short 
	{
        LVS_OWNERDATA         =  0x1000,
        LVS_SORTASCENDING     =  0x0010,
        LVS_SORTDESCENDING    =  0x0020,
        LVS_SHAREIMAGELISTS   =  0x0040,
        LVS_NOLABELWRAP       =  0x0080,
        LVS_AUTOARRANGE       =  0x0100
    }

    internal enum ListViewStylesICF : uint 
	{
        LVSICF_NOINVALIDATEALL = 0x00000001,
        LVSICF_NOSCROLL        = 0x00000002
    }

    internal enum WindowsMessage : uint 
	{
		WM_DESTROY				  = 0x0002,
        WM_NOTIFY                 = 0x004E,
        WM_USER                   = 0x0400,
        WM_REFLECT                = WM_USER + 0x1c00
    }

    internal enum ListViewNotices : int 
	{
        LVN_FIRST           = (0-100),
        LVN_LAST            = (0-199),
		LVN_BEGINDRAG		= ((int)LVN_FIRST-9),
		LVN_BEGINRDRAG		= ((int)LVN_FIRST-11),
        LVN_GETDISPINFOA    = ((int)LVN_FIRST-50),
        LVN_GETDISPINFOW    = ((int)LVN_FIRST-77),
        LVN_SETDISPINFOA    = ((int)LVN_FIRST-51),
        LVN_SETDISPINFOW    = ((int)LVN_FIRST-78),
        LVN_ODCACHEHINT     = ((int)LVN_FIRST-13),
        LVN_ODFINDITEMW     = ((int)LVN_FIRST-79)
    }

    
    internal struct WindowsFunction 
	{
        [DllImport("user32.dll", CharSet=CharSet.Auto)]
        public static extern int SendMessage(IntPtr hWnd, int msg, int wParam, int lParam);
		[DllImport("user32.dll", CharSet=CharSet.Auto)]
		public static extern int SendMessage(IntPtr hWnd, int msg, int wParam, ref LVITEM lParam);
		[DllImport("user32.dll", CharSet=CharSet.Auto)]
		public static extern int SendMessage(IntPtr hWnd, int msg, int wParam, bool lParam);
    }
}