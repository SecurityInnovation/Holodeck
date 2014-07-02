using System;
namespace TestApplication4 {
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


			System.IO.FileInfo fileInfo = new System.IO.FileInfo (tempPath + "\\resources4file.txt");
			fileInfo.Create ().Close ();
			System.IO.StreamWriter outFile = fileInfo.AppendText ();
			System.Console.WriteLine (tempPath + "\\resources4file.txt");


			try {
				exc = null;
				xc = null;
				now = System.DateTime.Now;
				xc = System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromCertFile (tempPath + "\\dummyFile1.txt");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + tempPath + "\\dummyFile1.txt");
				outFile.WriteLine ("Func: " + "System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromCertFile(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (xc));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
			try {
				exc = null;
				xc = null;
				now = System.DateTime.Now;
				xc = System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromSignedFile (tempPath + "\\dummyFile2.txt");
			} catch (Exception e) {
				exc = e;
			} finally {
				outFile.WriteLine ("Name: " + tempPath + "\\dummyFile2.txt");
				outFile.WriteLine ("Func: " + "System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromSignedFile(String)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (xc));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}
/*
			try {
			System.IO.BinaryWriter.Write ();
			System.IO.BinaryWriter.Seek ();
			System.IO.BinaryWriter.Flush ();
			System.IO.BinaryWriter.Close ();
			System.IO.BinaryWriter bw = new System.IO.BinaryWriter ();
			} catch (Exception e) {
			}

			try {
			System.IO.BufferedStream.WriteByte ();
			System.IO.BufferedStream.Write ();
			System.IO.BufferedStream.ReadByte ();
			System.IO.BufferedStream.Read ();
			System.IO.BufferedStream.SetLength ();
			System.IO.BufferedStream.Seek ();
			System.IO.BufferedStream.EndWrite ();
			System.IO.BufferedStream.BeginWrite ();
			System.IO.BufferedStream.EndRead ();
			System.IO.BufferedStream.BeginRead ();
			System.IO.BufferedStream.Flush ();
			System.IO.BufferedStream.Close ();
			System.IO.BufferedStream bs = new System.IO.BufferedStream ();
			} catch (Exception e) {
			}
*/
			try {
				exc = null;
				try {
					exc = null;
					sw = null;
					now = System.DateTime.Now;
					sw = new System.IO.StreamWriter (tempPath + "\\dummyFile3.txt");
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile3.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.ctor(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					try {
						exc = null;
						System.Console.SetOut (sw);
						try {
							exc = null;
							now = System.DateTime.Now;
							System.Console.WriteLine ();
						} catch (Exception e) {
							exc = e;
						} finally {
							outFile.WriteLine ("Name: " + tempPath + "\\dummyFile3.txt");
							outFile.WriteLine ("Func: " + "System.Console.WriteLine()");
							outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
							outFile.WriteLine ("Time: " + GetTime (now));
							outFile.WriteLine ("Retv: " + "");
							outFile.WriteLine ("Errc: " + "");
							outFile.WriteLine ("Exce: " + GetException (exc));
						}
						try {
							exc = null;
							now = System.DateTime.Now;
							System.Console.Out.Write ("hello");
						} catch (Exception e) {
							exc = e;
						} finally {
							outFile.WriteLine ("Name: " + tempPath + "\\dummyFile3.txt");
							outFile.WriteLine ("Func: " + "System.IO.TextWriter.Write(String)");
							outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
							outFile.WriteLine ("Time: " + GetTime (now));
							outFile.WriteLine ("Retv: " + "");
							outFile.WriteLine ("Errc: " + "");
							outFile.WriteLine ("Exce: " + GetException (exc));
						}
					} finally {
						try {
							sw.Close ();
						} catch (Exception) {
						}
					}
				} catch (Exception) {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile3.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.ctor(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}

				try {
					exc = null;
					sr = null;
					now = System.DateTime.Now;
					sr = new System.IO.StreamReader (tempPath + "\\dummyFile4.txt");
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile4.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamReader.ctor(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					try {
						System.Console.SetIn (sr);
						try {
							exc = null;
							now = System.DateTime.Now;
							System.Console.ReadLine ();
						} catch (Exception e) {
							exc = e;
						} finally {
							outFile.WriteLine ("Name: " + tempPath + "\\dummyFile4.txt");
							outFile.WriteLine ("Func: " + "System.Console.ReadLine()");
							outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
							outFile.WriteLine ("Time: " + GetTime (now));
							outFile.WriteLine ("Retv: " + "");
							outFile.WriteLine ("Errc: " + "");
							outFile.WriteLine ("Exce: " + GetException (exc));
						}
						try {
							exc = null;
							now = System.DateTime.Now;
							System.Console.In.ReadLine ();
						} catch (Exception e) {
							exc = e;
						} finally {
							outFile.WriteLine ("Name: " + tempPath + "\\dummyFile4.txt");
							outFile.WriteLine ("Func: " + "System.IO.TextReader.ReadLine()");
							outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
							outFile.WriteLine ("Time: " + GetTime (now));
							outFile.WriteLine ("Retv: " + "");
							outFile.WriteLine ("Errc: " + "");
							outFile.WriteLine ("Exce: " + GetException (exc));
						}
					} catch (Exception) {
					} finally {
						try {
							sr.Close ();
						} catch (Exception) {
						}
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile4.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamReader.ctor(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}

				try {
					exc = null;
					sw = null;
					now = System.DateTime.Now;
					sw = new System.IO.StreamWriter (tempPath + "\\dummyFile5.txt");
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.ctor(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					try {
						System.Console.SetError (sw);
						try {
							exc = null;
							now = System.DateTime.Now;
							System.Console.Error.WriteLine ();
						} catch (Exception e) {
							exc = e;
						} finally {
							outFile.WriteLine ("Name: " + tempPath + "\\dummyFile5.txt");
							outFile.WriteLine ("Func: " + "System.IO.TextWriter.WriteLine(String)");
							outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
							outFile.WriteLine ("Time: " + GetTime (now));
							outFile.WriteLine ("Retv: " + "");
							outFile.WriteLine ("Errc: " + "");
							outFile.WriteLine ("Exce: " + GetException (exc));
						}
					} finally {
						try {
							sw.Close ();
						} catch (Exception) {
						}
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile5.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.ctor(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				try {
					exc = null;
					di = null;
					now = System.DateTime.Now;
					di = System.IO.Directory.CreateDirectory (tempPath + "\\TestDir1");
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
					di = System.IO.Directory.GetParent (tempPath + "\\TestDir1");
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
					b = System.IO.Directory.Exists (tempPath + "\\TestDir1");
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
					System.IO.Directory.SetCreationTime (tempPath + "\\TestDir1", new System.DateTime (2003, 01, 01));
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
					dt = System.IO.Directory.GetCreationTime (tempPath + "\\TestDir1");
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
					System.IO.Directory.SetLastWriteTime (tempPath + "\\TestDir1", new System.DateTime (2003, 01, 01));
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
					dt = System.IO.Directory.GetLastWriteTime (tempPath + "\\TestDir1");
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
					System.IO.Directory.SetLastAccessTime (tempPath + "\\TestDir1", new System.DateTime (2003, 01, 01));
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
					dt = System.IO.Directory.GetLastAccessTime (tempPath + "\\TestDir1");
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
					sL = System.IO.Directory.GetFiles (tempPath + "\\TestDir1");
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
					sL = System.IO.Directory.GetDirectories (tempPath + "\\TestDir1");
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
					System.IO.Directory.GetFileSystemEntries (tempPath + "\\TestDir1");
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
					System.IO.Directory.SetCurrentDirectory (tempPath + "\\TestDir1\\..");
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
					System.IO.Directory.Move (tempPath + "\\TestDir1", tempPath + "\\TestDir2");
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
					System.IO.Directory.Delete (tempPath + "\\TestDir1");
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
					System.IO.Directory.Delete (tempPath + "\\TestDir2");
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
						di2 = di.CreateSubdirectory (tempPath + "\\TestDir3");
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
						di.MoveTo (tempPath + "\\TestDir4");
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
					di = new System.IO.DirectoryInfo (tempPath + "\\TestDir5");
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
						di.MoveTo (tempPath + "\\TestDir6");
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

			try {
				try {
					exc = null;
					sr = null;
					now = System.DateTime.Now;
					sr = System.IO.File.OpenText (tempPath + "\\dummyFile6.txt");
					sr.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile6.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.OpenText(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sw = null;
					now = System.DateTime.Now;
					sw = System.IO.File.CreateText (tempPath + "\\dummyFile7.txt");
					sw.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile7.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.CreateText(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sw = null;
					now = System.DateTime.Now;
					sw = System.IO.File.AppendText (tempPath + "\\dummyFile8.txt");
					sw.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile8.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.AppendText(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					fs = System.IO.File.Open (tempPath + "\\dummyFile9.txt", System.IO.FileMode.OpenOrCreate);
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.Open(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.File.SetCreationTime (tempPath + "\\dummyFile9.txt", new System.DateTime (2003, 01, 01));
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.SetCreationTime(String, DateTime)");
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
					dt = System.IO.File.GetCreationTime (tempPath + "\\dummyFile9.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.GetCreationTime(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (dt));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.File.SetLastAccessTime (tempPath + "\\dummyFile9.txt", new System.DateTime (2003, 01, 01));
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.SetLastAccessTime(String, DateTime)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					dt = System.IO.File.GetLastAccessTime (tempPath + "\\dummyFile9.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.GetLastAccessTime(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (dt));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.File.SetLastWriteTime (tempPath + "\\dummyFile9.txt", new System.DateTime (2003, 01, 01));
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.SetLastWriteTime(String, DateTime)");
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
					dt = System.IO.File.GetLastWriteTime (tempPath + "\\dummyFile9.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile9.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.GetLastWriteTime(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (dt));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					fs = System.IO.File.OpenRead (tempPath + "\\dummyFile10.txt");
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile10.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.OpenRead(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					fs = System.IO.File.OpenWrite (tempPath + "\\dummyFile11.txt");
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile11.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.OpenWrite(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					fs = System.IO.File.Create (tempPath + "\\testFile1.txt");
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile1.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.Create(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					System.IO.File.Move (tempPath + "\\testFile1.txt", tempPath + "\\testFile2.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile1.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.OpenWrite(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					//---
					outFile.WriteLine ("Name: " + tempPath + "\\testFile2.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.OpenWrite(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.File.Delete (tempPath + "\\testFile2.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile2.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.Delete(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					b = System.IO.File.Exists (tempPath + "\\testFile3.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile3.txt");
					outFile.WriteLine ("Func: " + "System.IO.File.Exists(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (b));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				fi = new System.IO.FileInfo (tempPath + "\\testFile4.txt");
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					fs = fi.Create ();
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile4.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.Create()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sr = null;
					now = System.DateTime.Now;
					sr = fi.OpenText ();
					sr.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile4.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.OpenText()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sw = null;
					now = System.DateTime.Now;
					sw = fi.CreateText ();
					sw.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile4.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.CreateText()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					sw = null;
					now = System.DateTime.Now;
					sw = fi.AppendText ();
					sw.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile4.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.AppendText()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (sw));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					fi = new System.IO.FileInfo (tempPath + "\\testFile5.txt");
					now = System.DateTime.Now;
					fs = fi.Open (System.IO.FileMode.Open);
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile5.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.Create(FileMode)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					fs = null;
					now = System.DateTime.Now;
					fs = fi.OpenWrite ();
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile5.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.OpenWrite()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (fs));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fi.MoveTo (tempPath + "\\testFile6.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\testFile5.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.MoveTo(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
					//---
					outFile.WriteLine ("Name: " + tempPath + "\\testFile6.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.MoveTo(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					char[] backSlash = new char[1];
					backSlash[0] = '\\';
					outFile.WriteLine ("Name: " + fi.FullName.TrimEnd (backSlash));
					now = System.DateTime.Now;
					fi.Delete ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Func: " + "System.IO.FileInfo.Delete()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				exc = null;
				byte[] array = new byte[1];
				array[0] = 0;
				fs = null;
				now = System.DateTime.Now;
				fs = System.IO.File.Open (tempPath + "\\dummyFile12.txt", System.IO.FileMode.OpenOrCreate);
				outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
				outFile.WriteLine ("Func: " + "System.IO.File.Open(String, FileMode)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (fs));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));

				try {
					exc = null;
					now = System.DateTime.Now;
					fs.Lock (0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Lock(Int64, Int64)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fs.Unlock (0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Unlock(Int64, Int64)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fs.WriteByte (0);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.WriteByte(Byte)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fs.Write (array, 0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Write(Byte[], Int32, Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					asr = null;
					now = System.DateTime.Now;
					asr = fs.BeginWrite (array, 0, 1, null, null);
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.BeginWrite(Byte[], Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));

					try {
						exc = null;
						now = System.DateTime.Now;
						fs.EndWrite (asr);
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
						outFile.WriteLine ("Func: " + "System.IO.FileStream.EndWrite(IAsyncResult)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.BeginWrite(Byte[], Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fs.SetLength (2);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.SetLength(Int64)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fs.Flush ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Flush()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = fs.ReadByte ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.ReadByte()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = fs.Read (array, 0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Read(Byte[], Int32, Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					l = 0;
					now = System.DateTime.Now;
					l = fs.Seek (0, System.IO.SeekOrigin.Begin);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Seek(Int64, SeekOrigin)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (l));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					asr = null;
					now = System.DateTime.Now;
					asr = fs.BeginRead (array, 0, 1, null, null);
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.BeginRead(Byte[], Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));

					try {
						exc = null;
						i = 0;
						now = System.DateTime.Now;
						i = fs.EndRead (asr);
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
						outFile.WriteLine ("Func: " + "System.IO.FileStream.EndRead(IAsyncResult)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (i));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.BeginRead(Byte[], Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					fs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
					outFile.WriteLine ("Func: " + "System.IO.FileStream.Close(IAsyncResult)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception e) {
				exc = e;
				outFile.WriteLine ("Name: " + tempPath + "\\dummyFile12.txt");
				outFile.WriteLine ("Func: " + "System.IO.File.Open(String, FileMode)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (fs));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}

			try {
				System.IO.TextWriter tw = new System.IO.StreamWriter (tempPath + "\\dummyFile13.txt");
				try {
					exc = null;
					now = System.DateTime.Now;
					tw.WriteLine ("hello");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextWriter.WriteLine(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					tw.Write ("12345678790");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextWriter.Write(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					tw.Flush ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextWriter.Flush()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					tw.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextWriter.Close()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				char[] array = new char[1];
				array[0] = 'a';
				System.IO.TextReader tr = new System.IO.StreamReader (tempPath + "\\dummyFile13.txt");
				try {
					exc = null;
					s = null;
					now = System.DateTime.Now;
					s = tr.ReadLine ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextReader.ReadLine()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (s));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = tr.ReadBlock (array, 0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextReader.ReadBlock(Char[], Int32, Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = tr.Read ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextReader.Read()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = tr.Read (array, 0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextReader.Read(Char[], Int32, Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					s = null;
					now = System.DateTime.Now;
					s = tr.ReadToEnd ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextReader.ReadToEnd()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (s));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					tr.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile13.txt");
					outFile.WriteLine ("Func: " + "System.IO.TextReader.Close()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				sw = new System.IO.StreamWriter (tempPath + "\\dummyFile14.txt");
				try {
					exc = null;
					now = System.DateTime.Now;
					sw.Write (0);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile14.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.Write(Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					sw.Flush ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile14.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.Flush()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					sw.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile14.txt");
					outFile.WriteLine ("Func: " + "System.IO.StreamWriter.Close()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

/*
			try {
				exc = null;
				System.IO.IsolatedStorage.IsolatedStorageScope iss = System.IO.IsolatedStorage.IsolatedStorageScope.User | System.IO.IsolatedStorage.IsolatedStorageScope.Assembly | System.IO.IsolatedStorage.IsolatedStorageScope.Domain;
				isf = null;
				now = System.DateTime.Now;
				isf = System.IO.IsolatedStorage.IsolatedStorageFile.GetStore (iss, null, null);

				try {
					exc = null;
					now = System.DateTime.Now;
					isf.Dispose ();
				} catch (Exception e) {
					exc = e;
				}
//			System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForDomain ();
//			System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForAssembly ();
//			System.IO.IsolatedStorage.IsolatedStorageFile.GetStore (System.IO.IsolatedStorage.IsolatedStorageScope.User | System.IO.IsolatedStorage.IsolatedStorageScope.Assembly | System.IO.IsolatedStorage.IsolatedStorageScope.Domain, null, null);
				try {
					exc = null;
					now = System.DateTime.Now;
					isf.CreateDirectory ("dummyDir");
				} catch (Exception e) {
					exc = e;
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isf.GetDirectoryNames ("*");
				} catch (Exception e) {
					exc = e;
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isf.DeleteFile ("dummyFile");
				} catch (Exception e) {
					exc = e;
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isf.DeleteDirectory ("dummyDir");
				} catch (Exception e) {
					exc = e;
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isf.GetFileNames ("*");
				} catch (Exception e) {
					exc = e;
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isf.Close ();
				} catch (Exception e) {
					exc = e;
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					System.IO.IsolatedStorage.IsolatedStorageFile.Remove (iss);
				} catch (Exception e) {
					exc = e;
				}
			} catch (Exception e) {
				exc = e;
			}
*/
			try {
				exc = null;
				byte[] array = new byte[1];
				array[0] = 0;
				isfs = null;
				now = System.DateTime.Now;
				isfs = new System.IO.IsolatedStorage.IsolatedStorageFileStream (tempPath + "\\dummyFile15.txt", System.IO.FileMode.OpenOrCreate);
				outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
				outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.ctor(String, FileMode)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (isfs));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));

				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.Lock (0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Lock(Int64, Int64)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.Unlock (0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Unlock(Int64, Int64)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.WriteByte (0);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.WriteByte(Byte)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.Write (array, 0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Write(Byte[], Int32, Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					asr = null;
					now = System.DateTime.Now;
					asr = isfs.BeginWrite (array, 0, 1, null, null);
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.BeginWrite(IAsyncResult, Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));

					try {
						exc = null;
						now = System.DateTime.Now;
						isfs.EndWrite (asr);
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
						outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.EndWrite(IAsyncResult)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + "");
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.BeginWrite(IAsyncResult, Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.SetLength (2);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.SetLength(Int64)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.Flush ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Flush()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					l = 0;
					now = System.DateTime.Now;
					l = isfs.Seek (0, System.IO.SeekOrigin.Begin);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Seek(Int64, SeekOrigin)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (l));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = isfs.ReadByte ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.ReadByte()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					i = 0;
					now = System.DateTime.Now;
					i = isfs.Read (array, 0, 1);
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Read(Byte[], Int32, Int32)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (i));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					asr = null;
					now = System.DateTime.Now;
					asr = isfs.BeginRead (array, 0, 1, null, null);
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.BeginRead(Byte[], Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));

					try {
						exc = null;
						i = 0;
						now = System.DateTime.Now;
						i = isfs.EndRead (asr);
					} catch (Exception e) {
						exc = e;
					} finally {
						outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
						outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.EndRead(IAsyncResult)");
						outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
						outFile.WriteLine ("Time: " + GetTime (now));
						outFile.WriteLine ("Retv: " + toString (i));
						outFile.WriteLine ("Errc: " + "");
						outFile.WriteLine ("Exce: " + GetException (exc));
					}
				} catch (Exception e) {
					exc = e;
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.BeginRead(Byte[], Int32, Int32, AsyncCallback, Object)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (asr));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					now = System.DateTime.Now;
					isfs.Close ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
					outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.Close()");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception e) {
				exc = e;
				outFile.WriteLine ("Name: " + tempPath + "\\dummyFile15.txt");
				outFile.WriteLine ("Func: " + "System.IO.IsolatedStorage.IsolatedStorageFileStream.ctor(String, FileMode)");
				outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
				outFile.WriteLine ("Time: " + GetTime (now));
				outFile.WriteLine ("Retv: " + toString (isfs));
				outFile.WriteLine ("Errc: " + "");
				outFile.WriteLine ("Exce: " + GetException (exc));
			}

			try {
				System.Net.WebClient wc = new System.Net.WebClient ();
				try {
					exc = null;
					now = System.DateTime.Now;
					wc.DownloadFile ("http://www.google.com", tempPath + "\\dummyFile16.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile16.txt");
					outFile.WriteLine ("Func: " + "System.Net.WebClient.DownloadFile(String, String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + "");
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					bL = null;
					now = System.DateTime.Now;
					bL = wc.UploadFile ("http://www.google.com", tempPath + "\\dummyFile16.txt");
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile16.txt");
					outFile.WriteLine ("Func: " + "System.Net.WebClient.UploadFile(String, String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + toString (bL));
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

			try {
				string processString = null;
				try {
					exc = null;
					p = null;
					now = System.DateTime.Now;
					p = System.Diagnostics.Process.Start (tempPath + "\\dummyFile16.txt");
					processString = toString (p);
					p.Kill ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile16.txt");
					outFile.WriteLine ("Func: " + "System.Diagnostics.Process.Start(String)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + processString);
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
				try {
					exc = null;
					System.Diagnostics.ProcessStartInfo psi = new System.Diagnostics.ProcessStartInfo (tempPath + "\\dummyFile16.txt");
					p = null;
					now = System.DateTime.Now;
					p = System.Diagnostics.Process.Start (psi);
					processString = toString (p);
					p.Kill ();
				} catch (Exception e) {
					exc = e;
				} finally {
					outFile.WriteLine ("Name: " + tempPath + "\\dummyFile16.txt");
					outFile.WriteLine ("Func: " + "System.Diagnostics.Process.Start(ProcessStartInfo)");
					outFile.WriteLine ("Proc: " + System.Diagnostics.Process.GetCurrentProcess ().Id);
					outFile.WriteLine ("Time: " + GetTime (now));
					outFile.WriteLine ("Retv: " + processString);
					outFile.WriteLine ("Errc: " + "");
					outFile.WriteLine ("Exce: " + GetException (exc));
				}
			} catch (Exception) {
			}

/*
			try {
				now = System.DateTime.Now;
				System.Configuration.AppSettingsReader asr = new System.Configuration.AppSettingsReader ();
				asr.GetValue ("key", System.Type.GetType ("System.Object", false));
			} catch (Exception e) {
			}
*/

/*
			try {
			System.Xml.XmlDocument.Save ();
			System.Xml.XmlDocument.LoadXml ();
			System.Xml.XmlDocument.WriteContentTo ();
			System.Xml.XmlDocument.WriteTo ();
			System.Xml.XmlDocument xd = new System.Xml.XmlDocument (System.Xml.XmlNameTable);
			System.Xml.XmlDocumentFragment.WriteContentTo ();
			System.Xml.XmlDocumentFragment.WriteTo ();
			System.Xml.XmlDocumentType.WriteContentTo ();
			System.Xml.XmlDocumentType.WriteTo ();
			} catch (Exception e) {
			}

			try {
			System.Xml.XmlWriter.WriteNode ();
			System.Xml.XmlWriter.WriteAttributes ();
			System.Xml.XmlWriter.WriteStartElement ();
			System.Xml.XmlWriter.WriteAttributeString ();
			System.Xml.XmlWriter.WriteStartAttribute ();
			System.Xml.XmlWriter.WriteElementString ();
			} catch (Exception e) {
			}

			try {
			System.Xml.XmlTextWriter xtw = System.Xml.XmlTextWriter (tempPath + "\\dummyFile.txt", System.Text.Encoding.ASCII);
			xtw.WriteNode ();
			xtw.WriteAttributes ();
			xtw.WriteQualifiedName ("localName", );
			xtw.WriteName ();
			xtw.WriteNmToken ();
			xtw.WriteBinHex ();
			xtw.WriteBase64 ();
			xtw.WriteRaw ();
			xtw.WriteChars ();
			xtw.WriteSurrogateCharEntity ();
			xtw.WriteString ();
			xtw.WriteWhitespace ();
			xtw.WriteCharEntity ();
			xtw.WriteEntityRef ();
			xtw.WriteProcessingInstruction ();
			xtw.WriteComment ();
			xtw.WriteCData ();
			xtw.WriteEndAttribute ();
			xtw.WriteStartAttribute ();
			xtw.WriteFullEndElement ();
			xtw.WriteEndElement ();
			xtw.WriteStartElement ();
			xtw.WriteDocType ();
			xtw.WriteEndDocument ();
			xtw.WriteStartDocument ();
			xtw.WriteAttributeString ();
			xtw.WriteElementString ();
			xtw.Flush ();
			xtw.Close ();
			} catch (Exception e) {
			}

			try {
			System.Xml.XmlReader.IsStartElement ();
			System.Xml.XmlReader.ReadEndElement ();
			System.Xml.XmlReader.ReadElementString ();
			System.Xml.XmlReader.ReadStartElement ();
			System.Xml.XmlReader.MoveToContent ();
			System.Xml.XmlReader.Skip ();
			System.Xml.XmlReader.IsName ();
			System.Xml.XmlReader.IsNameToken ();
			} catch (Exception e) {
			}

			try {
			System.Xml.XmlTextReader.ReadAttributeValue ();
			System.Xml.XmlTextReader.ResolveEntity ();
			System.Xml.XmlTextReader.LookupNamespace ();
			System.Xml.XmlTextReader.ReadOuterXml ();
			System.Xml.XmlTextReader.ReadInnerXml ();
			System.Xml.XmlTextReader.IsStartElement ();
			System.Xml.XmlTextReader.ReadEndElement ();
			System.Xml.XmlTextReader.ReadElementString ();
			System.Xml.XmlTextReader.ReadStartElement ();
			System.Xml.XmlTextReader.MoveToContent ();
			System.Xml.XmlTextReader.ReadString ();
			System.Xml.XmlTextReader.Skip ();
			System.Xml.XmlTextReader.Close ();
			System.Xml.XmlTextReader.Read ();
			System.Xml.XmlTextReader.MoveToElement ();
			System.Xml.XmlTextReader.MoveToNextAttribute ();
			System.Xml.XmlTextReader.MoveToFirstAttribute ();
			System.Xml.XmlTextReader.MoveToAttribute ();
			System.Xml.XmlTextReader.GetAttribute ();
			System.Xml.XmlTextReader.GetRemainder ();
			System.Xml.XmlTextReader.ReadChars ();
			System.Xml.XmlTextReader.ReadBase64 ();
			System.Xml.XmlTextReader.ReadBinHex ();
			System.Xml.XmlTextReader.ctor ();
			} catch (Exception e) {
			}

			try {
			System.Xml.XmlEntityReference.WriteContentTo ();
			System.Xml.XmlEntityReference.WriteTo ();
			System.Xml.XmlImplementation.CreateDocument ();
			System.Xml.XmlImplementation.ctor ();
			System.Xml.XmlText.WriteContentTo ();
			System.Xml.XmlText.WriteTo ();
			} catch (Exception e) {
			}
*/
			outFile.Flush ();
			outFile.Close ();

			try {
				sL = System.IO.Directory.GetFiles (tempPath, "tempFile*.txt");
				foreach (string str in sL) {
					try {
						System.IO.File.Delete (str);
					} catch (Exception) {
					}
				}
				sL = System.IO.Directory.GetFiles (tempPath, "dummyFile*.txt");
				foreach (string str in sL) {
					try {
						System.IO.File.Delete (str);
					} catch (Exception) {
					}
				}
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
