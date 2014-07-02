using System;

namespace ResourcePaneTestApps {
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class ResourcePaneTestApplication20 {
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
		static void Main (string[] args) {
			string tempPath = System.Environment.GetEnvironmentVariable ("TEMP");
			if (tempPath == null) {
				tempPath = System.Environment.GetEnvironmentVariable ("TMP");
			}
			if (tempPath == null) {
				tempPath = "..\\..";
			}

			string machineName = String.Empty;

			Exception exc = null;
			System.DateTime now = InicDateTime ();
			Object obj = null;
			string[] sL = null;
			Microsoft.Win32.RegistryKey rKey1 = null;
			Microsoft.Win32.RegistryKey rKey = null;
			Microsoft.Win32.RegistryKey rKey2 = null;


			System.IO.FileInfo fileInfo = new System.IO.FileInfo (tempPath + "\\resources20reg.txt");
			fileInfo.Create ().Close ();
			System.IO.StreamWriter outFile = fileInfo.AppendText ();
			System.Console.WriteLine (tempPath + "\\resources20reg.txt");

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.ClassesRoot, machineName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_CLASSES_ROOT");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.CurrentConfig, machineName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_CURRENT_CONFIG");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.DynData, machineName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_DYN_DATA");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.LocalMachine, machineName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.PerformanceData, machineName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_PERFORMACE_DATA");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.Users, machineName);
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_USERS");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.ClassesRoot.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\TestKey1");
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
				/*<Resource>*/Microsoft.Win32.Registry.ClassesRoot.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.ClassesRoot.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.ClassesRoot.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CLASSES_ROOT\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.ClassesRoot.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.CurrentConfig.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_CONFIG\\TestKey1");
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
				/*<Resource>*/Microsoft.Win32.Registry.CurrentConfig.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_CONFIG\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.CurrentConfig.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_CONFIG\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.CurrentConfig.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_CONFIG\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.CurrentConfig.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.CurrentUser.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_USER\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				now = System.DateTime.Now;
				/*<Resource>*/Microsoft.Win32.Registry.CurrentUser.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_USER\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.CurrentUser.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_USER\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.CurrentUser.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_CURRENT_USER\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.CurrentUser.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.DynData.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_DYN_DATA\\TestKey1");
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
				/*<Resource>*/Microsoft.Win32.Registry.DynData.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_DYN_DATA\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.DynData.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_DYN_DATA\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.DynData.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_DYN_DATA\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.DynData.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.LocalMachine.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_LOCAL_MACHINE\\TestKey1");
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
				/*<Resource>*/Microsoft.Win32.Registry.LocalMachine.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_LOCAL_MACHINE\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.LocalMachine.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_LOCAL_MACHINE\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.LocalMachine.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_LOCAL_MACHINE\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.LocalMachine.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.PerformanceData.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_PERFORMACE_DATA\\TestKey1");
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
				/*<Resource>*/Microsoft.Win32.Registry.PerformanceData.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_PERFORMACE_DATA\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.PerformanceData.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_PERFORMACE_DATA\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.PerformanceData.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_PERFORMACE_DATA\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.PerformanceData.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.Users.CreateSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_USERS");
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
				/*<Resource>*/Microsoft.Win32.Registry.Users.DeleteSubKey ("TestKey1\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_USERS\\TestKey1");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				rKey = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey = Microsoft.Win32.Registry.Users.CreateSubKey ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_USERS\\TestKey2");
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
				/*<Resource>*/Microsoft.Win32.Registry.Users.DeleteSubKeyTree ("TestKey2\\");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + "HKEY_USERS\\TestKey2");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + "");
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				Microsoft.Win32.Registry.Users.Close ();
			} catch (Exception) {
			}

			try {
				exc = null;
				rKey1 = null;
				now = System.DateTime.Now;
				/*<Resource>*/rKey1 = Microsoft.Win32.RegistryKey.OpenRemoteBaseKey (Microsoft.Win32.RegistryHive.LocalMachine, machineName);
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey1));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
				try {
					exc = null;
					now = System.DateTime.Now;
					/*<Resource>*/rKey1 = rKey1.OpenSubKey ("SOFTWARE\\HolodeckEE\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE");
					outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenSubKey(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (rKey1));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					rKey2 = null;
					now = System.DateTime.Now;
					/*<Resource>*/rKey2 = rKey1.CreateSubKey ("ResourcePaneTestKey1\\");
					outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1");
					outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.CreateSubKey(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (rKey2));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					try {
						exc = null;
						rKey = null;
						now = System.DateTime.Now;
						/*<Resource>*/rKey = rKey2.CreateSubKey ("newKey\\");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1\\newKey");
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
						/*<Resource>*/rKey2.DeleteSubKeyTree ("newKey\\");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1\\newKey");
						outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKeyTree(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						rKey = null;
						now = System.DateTime.Now;
						/*<Resource>*/rKey = rKey2.OpenSubKey ("NonExistent\\");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1\\NonExistent");
						outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenSubKey(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (rKey));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1");
					outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.CreateSubKey(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (rKey2));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					/*<Resource>*/rKey1.DeleteSubKey ("ResourcePaneTestKey1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1");
					outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteSubKey(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					rKey1.Close ();
				} catch (Exception) {
				}
			} catch (Exception e) {
				exc = e;
				outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE");
				outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.OpenRemoteBaseKey(RegistryHive, String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (rKey1));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}

			outFile.Flush ();
			outFile.Close ();
		} // Main
	} // class
} // namespace
