// *****************************************************************************
// 
//  (c) Crownwood Consulting Limited 2002-2003
//  All rights reserved. The software and associated documentation 
//  supplied hereunder are the proprietary information of Crownwood Consulting 
//	Limited, Crownwood, Bracknell, Berkshire, England and are supplied subject 
//  to licence terms.
// 
//  Magic Version 1.7.3.0 	www.dotnetmagic.com
// *****************************************************************************

using System;
using Crownwood.Magic.Collections;
using Crownwood.Magic.Controls;

namespace Crownwood.Magic.Collections
{
	public class TabPageCollection : CollectionWithEvents
	{
		public TabPage Add(TabPage value)
		{
			// Use base class to process actual collection operation
			base.List.Add(value as object);

			return value;
		}

		public void AddRange(TabPage[] values)
		{
			// Use existing method to add each array entry
			foreach(TabPage page in values)
				Add(page);
		}

		public void Remove(TabPage value)
		{
			try
			{
				// Use base class to process actual collection operation
				base.List.Remove(value as object);
			}
			catch (Exception)
			{
			}
		}

		public void Insert(int index, TabPage value)
		{
			try
			{
				// Use base class to process actual collection operation
				base.List.Insert(index, value as object);
			}
			catch (Exception)
			{
			}
		}

		public bool Contains(TabPage value)
		{
			// Use base class to process actual collection operation
			return base.List.Contains(value as object);
		}

		public TabPage this[int index]
		{
				// Use base class to process actual collection operation
				get
				{
					try
					{
						return (base.List[index] as TabPage);
					}
					catch (Exception)
					{
						return (new TabPage());
					}
				}
        }

        public TabPage this[string title]
        {
            get 
            {
                // Search for a Page with a matching title
                foreach(TabPage page in base.List)
                    if (page.Title == title)
                        return page;

                return null;
            }
        }

        public int IndexOf(TabPage value)
        {
            // Find the 0 based index of the requested entry
            return base.List.IndexOf(value);
        }
    }
}
