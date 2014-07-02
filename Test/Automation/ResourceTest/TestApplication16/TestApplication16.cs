using System;

namespace ResourcePaneTestApps
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class ResourcePaneTestApplication16
	{
		private static string GetTime (System.DateTime dt) {
			string retString = string.Empty;
			
			if (dt.Month < 10) {
				retString += " ";
			}
			retString += dt.Month.ToString () + "/";
			if (dt.Day < 10) {
				retString += " ";
			}
			retString += dt.Day.ToString () + "/";
			retString += dt.Year.ToString () + "  ";
			if (dt.Hour < 10) {
				retString += " ";
			}
			retString += dt.Hour.ToString () + ":";
			if (dt.Minute < 10) {
				retString += " ";
			}
			retString += dt.Minute.ToString () + ":";
			if (dt.Second < 10) {
				retString += " ";
			}
			retString += dt.Second.ToString () + ":";
			if (dt.Millisecond < 100) {
				retString += " ";
			} else if (dt.Millisecond < 10) {
				retString += "  ";
			}
			retString += dt.Millisecond.ToString ();

			return retString;
		}

		private static string Extend (string str) {
			if (str == String.Empty) {
				return str;
			} else {
				return str + "\\";
			}
		}

		private static string toString (Object obj) {
			if (obj == null) {
				return "";
			} else {
				return obj.ToString ();
			}
		}

		private static System.DateTime InicDateTime () {
			System.DateTime dt = System.DateTime.MinValue;
			return dt;
		}

		private static string GetException (Exception e) {
			if (e != null) {
				string excMsg = e.ToString ();
				int colonIndex = 0;
				int dotIndex = 0;
				
				colonIndex = excMsg.IndexOf (":");
				if (colonIndex == -1) {
					colonIndex = excMsg.Length;
				}

				dotIndex = excMsg.Substring (0, colonIndex).LastIndexOf (".");
				if (dotIndex == -1) {
					dotIndex = 0;
				} else {
					dotIndex++;
				}

				return excMsg.Substring (dotIndex, colonIndex - dotIndex);
			} else {
				return "";
			}
		}

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main (string[] args)
		{
			string tempPath = System.Environment.GetEnvironmentVariable ("TEMP");
			if (tempPath == null) {
				tempPath = System.Environment.GetEnvironmentVariable ("TMP");
			}
			if (tempPath == null) {
				tempPath = "..\\..";
			}

			string machineName = String.Empty;
			string specKeyName = "ώдΘžҖשچ€∞╛⅞♪♂אַﺅﻅﻵŊΔ";
			string specValName = "ώдΘžҖשچ€∞╛⅞♪♂אַﺅﻅﻵŊΔ";

			Exception exc = null;
			System.DateTime now = InicDateTime ();
			Object obj = null;
			string[] sL = null;
			Microsoft.Win32.RegistryKey rKey1 = null;
			Microsoft.Win32.RegistryKey rKey = null;
			Microsoft.Win32.RegistryKey rKey2 = null;


			System.IO.FileInfo fileInfo = new System.IO.FileInfo (tempPath + "\\resources16reg.txt");
			fileInfo.Create ().Close ();
			System.IO.StreamWriter outFile = fileInfo.AppendText ();
			System.Console.WriteLine (tempPath + "\\resources16reg.txt");


			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.ClassesRoot.CreateSubKey (specKeyName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\" + specKeyName);
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.CreateSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				now = System.DateTime.Now;
				/*<Resource>*/Microsoft.Win32.Registry.ClassesRoot.DeleteSubKey (specKeyName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\" + specKeyName);
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				now = System.DateTime.Now;
				/*<Resource>*/Microsoft.Win32.Registry.ClassesRoot.SetValue (specValName, "newData");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\" + specValName);
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.SetValue(String, Object)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				obj = null;
				now = System.DateTime.Now;
				/*<Resource>*/obj = Microsoft.Win32.Registry.ClassesRoot.GetValue (specValName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\" + specValName);
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.GetValue(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (obj));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				now = System.DateTime.Now;
				/*<Resource>*/Microsoft.Win32.Registry.ClassesRoot.DeleteValue (specValName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\" + specValName);
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteValue(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}

			outFile.Flush ();
			outFile.Close ();
		} // Main
	} // class
} // namespace
