using System;
namespace TestApplication18 {
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1 {
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
		static void Main(string[] args) {
			string tempPath = System.Environment.GetEnvironmentVariable ("TEMP");
			if (tempPath == null) {
				tempPath = System.Environment.GetEnvironmentVariable ("TMP");
			}
			if (tempPath == null) {
				tempPath = "..\\..";
			}

			Exception exc = null;
			System.DateTime now = System.DateTime.Now;
			System.Security.Cryptography.X509Certificates.X509Certificate xc = null;
			System.IO.StreamWriter sw = null;
			System.IO.StreamReader sr = null;
			System.IO.DirectoryInfo di = null;
			bool b = false;
			System.DateTime dt = InicDateTime ();
			string[] sL = null;
			System.IO.FileInfo[] fiL = null;
			System.IO.DirectoryInfo[] diL = null;
			System.IO.FileSystemInfo[] fsiL = null;
			System.IO.FileStream fs = null;
			System.IO.FileInfo fi = null;
			System.IAsyncResult asr = null;
			int i = 0;
			long l = 0;
			string s = null;
			System.IO.IsolatedStorage.IsolatedStorageFile isf = null;
			System.IO.IsolatedStorage.IsolatedStorageFileStream isfs = null;
			byte[] bL = null;
			System.Diagnostics.Process p = null;


			System.IO.FileInfo fileInfo = new System.IO.FileInfo (tempPath + "\\resources18file.txt");
			fileInfo.Create ().Close ();
			System.IO.StreamWriter outFile = fileInfo.AppendText ();
			System.Console.WriteLine (tempPath + "\\resources18file.txt");


			try {
				try {
					exc = null;
					di = null;
					now = System.DateTime.Now;
					di = System.IO.Directory.CreateDirectory (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.CreateDirectory(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (di));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					di = null;
					now = System.DateTime.Now;
					di = System.IO.Directory.GetParent (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath);
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetParent(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (di));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					b = System.IO.Directory.Exists (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.Exists(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (b));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.SetCreationTime (tempPath + "\\TestDir1\\", new System.DateTime (2003, 01, 01));
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.SetCreationTime(String, DateTime)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					dt = InicDateTime ();
					now = System.DateTime.Now;
					dt = System.IO.Directory.GetCreationTime (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetCreationTime(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (dt));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.SetLastWriteTime (tempPath + "\\TestDir1\\", new System.DateTime (2003, 01, 01));
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.SetLastWriteTime(String, DateTime)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					dt = InicDateTime ();
					now = System.DateTime.Now;
					dt = System.IO.Directory.GetLastWriteTime (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetLastWriteTime(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (dt));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.SetLastAccessTime (tempPath + "\\TestDir1\\", new System.DateTime (2003, 01, 01));
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.SetLastAccessTime(String, DateTime)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					dt = InicDateTime ();
					now = System.DateTime.Now;
					dt = System.IO.Directory.GetLastAccessTime (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetLastAccessTime(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (dt));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sL = null;
					now = System.DateTime.Now;
					sL = System.IO.Directory.GetFiles (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetFiles(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sL));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sL = null;
					now = System.DateTime.Now;
					sL = System.IO.Directory.GetDirectories (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetDirectories(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sL));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.GetFileSystemEntries (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.GetFileSystemEntries(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sL));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.SetCurrentDirectory (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath);
					outFile.WriteLine ("Func: " + "System.IO.Directory.SetCurrentDirectory(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.Move (tempPath + "\\TestDir1\\", tempPath + "\\TestDir2\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.Move(String, String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					//---
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir2");
					outFile.WriteLine ("Func: " + "System.IO.Directory.Move(String, String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.Delete (tempPath + "\\TestDir1\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir1");
					outFile.WriteLine ("Func: " + "System.IO.Directory.Delete(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.Directory.Delete (tempPath + "\\TestDir2\\");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\TestDir2");
					outFile.WriteLine ("Func: " + "System.IO.Directory.Delete(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				di = null;
				di = new System.IO.DirectoryInfo (tempPath);
				System.IO.DirectoryInfo di2 = null;
				try {
					try {
						exc = null;
						di2 = null;
						now = System.DateTime.Now;
						di2 = di.CreateSubdirectory (tempPath + "\\TestDir3\\");
						di = di2;
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir3");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.CreateSubdirectory(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (di2));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						fiL = null;
						now = System.DateTime.Now;
						fiL = di.GetFiles ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir3");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.GetFiles()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (fiL));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						diL = null;
						now = System.DateTime.Now;
						diL = di.GetDirectories ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir3");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.GetDirectories()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (diL));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						fsiL = null;
						now = System.DateTime.Now;
						fsiL = di.GetFileSystemInfos ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir3");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.GetFileSystemInfos()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (fsiL));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						now = System.DateTime.Now;
						di.MoveTo (tempPath + "\\TestDir4\\");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir3");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.MoveTo(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
						//---
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir4");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.MoveTo(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} finally {
					try {
						exc = null;
						char[] backSlash = new char[1];
						backSlash[0] = '\\';
						outFile.WriteLine ("Name: " + di.FullName.TrimEnd (backSlash));
						now = System.DateTime.Now;
						di.Delete ();
					} catch  (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.Delete()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				}
				try {
					di = null;
					di = new System.IO.DirectoryInfo (tempPath + "\\TestDir5\\");
					try {
						exc = null;
						now = System.DateTime.Now;
						di.Create ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir5");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.Create()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
					try {
						exc = null;
						now = System.DateTime.Now;
						di.MoveTo (tempPath + "\\TestDir6\\");
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir5");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.MoveTo(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
						//---
						outFile.WriteLine ("Name: " + tempPath + "\\TestDir6");
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.MoveTo(String)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} catch (Exception) {
				} finally {
					try {
						exc = null;
						char[] backSlash = new char[1];
						backSlash[0] = '\\';
						outFile.WriteLine ("Name: " + di.FullName.TrimEnd (backSlash));
						now = System.DateTime.Now;
						di.Delete ();
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Func: " + "System.IO.DirectoryInfo.Delete()");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				}
			} catch (Exception) {
			}

			outFile.Flush ();
			outFile.Close ();

			try {
				sL = System.IO.Directory.GetDirectories (tempPath, "TempDir*");
				foreach (string str in sL) {
					try {
						System.IO.Directory.Delete (str);
					} catch (Exception) {
					}
				}
			} catch (Exception) {
			}
		} // Main
	} // class
} // namespace
