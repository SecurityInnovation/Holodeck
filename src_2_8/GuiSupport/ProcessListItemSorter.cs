using System;
using System.Collections;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for ProcessListItemSorter.
	/// </summary>
	public class ProcessListItemSorter : IComparer
	{
		private bool ascending;
		private int columnToSort;

		public bool Ascending
		{
			get
			{
				return this.ascending;
			}
			set
			{
				this.ascending = value;
			}
		}

		public ProcessListItemSorter(int columnToSort)
		{
			this.ascending = true;
			this.columnToSort = columnToSort;
		}

		public int Compare(object a, object b)
		{
			ListViewItem aItem = (ListViewItem)a;
			ListViewItem bItem = (ListViewItem)b;

			switch(columnToSort)
			{
				case 0:
					return ComparePID(aItem.SubItems[0].Text, bItem.SubItems[0].Text);
				case 1:
					return CompareProcess(aItem.SubItems[1].Text, bItem.SubItems[1].Text);
				default:
					CaseInsensitiveComparer c = new CaseInsensitiveComparer();
					return c.Compare(a, b);
			}
		}

		private int ComparePID(object a, object b)
		{
			int aInt = int.Parse(a.ToString());
			int bInt = int.Parse(b.ToString());

			if (Ascending)
			{
				return (aInt - bInt);
			}
			
			return (bInt - aInt);
		}

		private int CompareProcess(object a, object b)
		{
			CaseInsensitiveComparer c = new CaseInsensitiveComparer();

			if (Ascending)
			{
				return c.Compare(a, b);
			}

			return c.Compare(b, a);
		}
	}
}
