using System;

namespace ResourcePaneTestApps
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class ResourcePaneTestApplication8
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

			string machineName = "\\\\127.0.0.1";

			Exception exc = null;
			System.DateTime now = InicDateTime ();
			Object obj = null;
			string[] sL = null;
			Microsoft.Win32.RegistryKey rKey1 = null;
			Microsoft.Win32.RegistryKey rKey = null;
			Microsoft.Win32.RegistryKey rKey2 = null;


			System.IO.FileInfo fileInfo = new System.IO.FileInfo (tempPath + "\\resources8reg.txt");
			fileInfo.Create ().Close ();
			System.IO.StreamWriter outFile = fileInfo.AppendText ();
			System.Console.WriteLine (tempPath + "\\resources8reg.txt");

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
					/*<Resource>*/rKey1 = rKey1.OpenSubKey ("SOFTWARE\\HolodeckEE");
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
					/*<Resource>*/rKey2 = rKey1.CreateSubKey ("ResourcePaneTestKey1");
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
						/*<Resource>*/rKey = rKey2.CreateSubKey ("newKey");
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
						/*<Resource>*/rKey2.DeleteSubKeyTree ("newKey");
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
						now = System.DateTime.Now;
						/*<Resource>*/rKey2.SetValue ("newValue", "newData");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1\\newValue");
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
						/*<Resource>*/obj = rKey2.GetValue ("newValue");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1\\newValue");
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
						/*<Resource>*/rKey2.DeleteValue ("newValue");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1\\newValue");
						outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.DeleteValue(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						now = System.DateTime.Now;
						/*<Resource>*/rKey2.Flush ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1");
						outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.Flush()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						sL = null;
						now = System.DateTime.Now;
						/*<Resource>*/sL = rKey2.GetValueNames ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + Extend (machineName) + "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1");
						outFile.WriteLine ("Func: " + "Microsoft.Win32.RegistryKey.GetValueNames()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (sL));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						rKey = null;
						now = System.DateTime.Now;
						/*<Resource>*/rKey = rKey2.OpenSubKey ("NonExistent");
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
					/*<Resource>*/rKey1.DeleteSubKey ("ResourcePaneTestKey1");
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
