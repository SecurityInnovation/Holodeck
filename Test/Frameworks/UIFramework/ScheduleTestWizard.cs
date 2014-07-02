using System;
using System.Xml;
using System.Collections;
using System.Collections.Specialized;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32;
using Holodeck;

namespace Holodeck
{
	public class ScheduleTestWizard1: Base
	{
		public static void StartScheduleTest (string functionName) 
		{		
		    // Go into native tree...
			SendKeyStrokes ("{RIGHT}"); // Right arrow	
            
			XmlTextReader xmlReader = new XmlTextReader("functions.xml");
			Hashtable FunctionTable = new Hashtable( );
			string function =  null;
			string categoryType = null;
			string category = null;
									
			while(xmlReader.Read())
			{
				if (xmlReader.Name=="FunctionName")
				{
					function = xmlReader.ReadString();
				}
				if ( xmlReader.Name=="Category" )
				{					
					if (function == functionName)
					{
						categoryType = xmlReader.ReadString();
						category = categoryType;
					}
					else
					{
						categoryType = xmlReader.ReadString();
					}

					if ( !FunctionTable.ContainsKey( categoryType ) )
					{
						FunctionTable[categoryType] = new ArrayList( );
						( (ArrayList)FunctionTable[categoryType] ).Add( function );
					}
					else
					{
						( (ArrayList)FunctionTable[categoryType] ).Add( function );
					}					
				}
			}

			ArrayList keyArrayList = new ArrayList( FunctionTable.Keys );
            
			// Sort the key array...
			keyArrayList.Sort( );

			// Sort all the arrays inside the hashtable...
			foreach ( string key in keyArrayList )
			{
				( (ArrayList)FunctionTable[key] ).Sort( );
			}
						
			int categoryCount = 0;
			categoryCount = keyArrayList.IndexOf( category );
			while (categoryCount>=0)
			{
				SendKeyStrokes ("{DOWN}");
				categoryCount--;
			}
			SendKeyStrokes ("{RIGHT}");
			
			int functionCount = 0;
			functionCount = ( ( ArrayList )FunctionTable[category] ).IndexOf( functionName );
			while (functionCount>=0)
			{
				SendKeyStrokes ("{DOWN}");
				functionCount--;
			}
			SendKeyStrokes ("{ENTER}");
		}
	}
}