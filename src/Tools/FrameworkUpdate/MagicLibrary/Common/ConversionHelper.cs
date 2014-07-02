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
using System.Text;
using System.Drawing;

namespace Crownwood.Magic.Common
{
    public class ConversionHelper
    {
		// Faster performance to cache the converters and type objects, rather
		// than keep recreating them each time a conversion is required
		protected static SizeConverter _sc = new SizeConverter();
		protected static PointConverter _pc = new PointConverter();

		public static string SizeToString(Size size)
		{
			return _sc.ConvertToInvariantString(size);
		}

		public static Size StringToSize(string str)
		{
			return (Size)_sc.ConvertFromInvariantString(str);
		}

		public static string PointToString(Point point)
		{
			return (string)_pc.ConvertToInvariantString(point);
		}

		public static Point StringToPoint(string str)
		{
			return (Point)_pc.ConvertFromInvariantString(str);
		}
    }
}