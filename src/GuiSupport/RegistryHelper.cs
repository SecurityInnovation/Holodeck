using System;
using Microsoft.Win32;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for RegistryHelper.
	/// </summary>
	public class RegistryHelper
	{
		public readonly string RegRoot;
		public readonly string RecentItemsRegRoot;
		public readonly RegistryKey RegRootKey;
		public readonly RegistryKey RecentItemsRegRootKey;
		public readonly int NumberOfRecentlyViewedItems;
		private static RegistryHelper instance = null;

		private RegistryHelper()
		{
			//
			// TODO: Add constructor logic here
			//
			RegRoot = "Software\\Security Innovation\\Holodeck Enterprise Edition";
			RecentItemsRegRoot = RegRoot + "\\Recent";
			RegRootKey = Registry.LocalMachine.CreateSubKey(RegRoot);
			RecentItemsRegRootKey = Registry.LocalMachine.CreateSubKey(RecentItemsRegRoot);
			NumberOfRecentlyViewedItems = 15;
		}

		public static RegistryHelper getInstance()
		{
			if (instance == null)
			{
				instance = new RegistryHelper();
			}
			return instance;
		}

		/**
		 * determine whether to run the wizard or launch pad
		 * returns 1 for wizard, 0 for launchpad
		 */
		public int GetWizardToRun()
		{
			if (RegRootKey != null)
			{
				// check for Basic Wizard or LaunchPad
				// 1 for Basic, 0 for Launchpad
				object wizard = RegRootKey.GetValue("BasicWizard");
				if (wizard != null)
				{
					return (int)wizard;
				}
				
				return 0;
			}
			
			return 0;
		}

		/**
		 * sets the need to run wizard flag to <run>
		 */
		public void SetNeedToRunWizardAtStartup(bool run)
		{
			if (RegRootKey != null)
			{
				RegRootKey.SetValue("RunWizardOnStartup", run ? 1 : 0);
			}
		}

		/**
		 * gets whether or not a wizard should be run on startup
		 */
		public bool GetNeedToRunWizardAtStartup()
		{
			if (RegRootKey != null)
			{
				return (RegRootKey.GetValue("RunWizardOnStartup").Equals(1));
			}

			return true;
		}

		/**
		 * sets the need to run launch pad flag to <run>
		 */
		public void SetNeedToRunLaunchPad(bool run)
		{
			if (RegRootKey != null)
			{
				RegRootKey.SetValue("BasicWizard", run ? 0 : 1);
			}
		}

		/**
		 * returns true if the registry is set to run the launchpad wizard.
		 * This happens when RunWizardOnStartup is 1 and BasicWizard is 0
		 */
		public bool GetNeedToRunLaunchPad()
		{
			if (RegRootKey != null)
			{
				object runWiz = RegRootKey.GetValue("RunWizardOnStartup");
				if (runWiz != null)
				{
					if (runWiz.Equals(1))
					{
						// check for Basic Wizard or LaunchPad
						// 1 for Basic, 0 for Launchpad
						object basic = RegRootKey.GetValue("BasicWizard");
						if (basic != null)
						{
							return (basic.Equals(0));
						}
						
						return false;
					}
					return false;
				}

				return false;
			}
			
			return false;
		}

		/**
		 * sets the need to run wizard flag to <run>
		 */
		public void SetNeedToRunWizard(bool run)
		{
			if (RegRootKey != null)
			{
				RegRootKey.SetValue("BasicWizard", run ? 1 : 0);
			}
		}

		/**
		 * returns true if the registry is set to run the wizard.
		 * This happens when RunWizardOnStartup is 1 and BasicWizard is 1 or
		 * either of the values do not exist
		 */
		public bool GetNeedToRunWizard()
		{
			if (RegRootKey != null)
			{
				object runWiz = RegRootKey.GetValue("RunWizardOnStartup");
				if (runWiz != null)
				{
					if (runWiz.Equals(1))
					{
						// check for Basic Wizard or LaunchPad
						// 1 for Basic, 0 for Launchpad
						object basic = RegRootKey.GetValue("BasicWizard");
						if (basic != null)
						{
							return (basic.Equals(1));
						}
						
						return true;
					}

					return false;
				}

				return true;
			}
			
			return true;
		}

		/**
		 * returns an array of the most recently loaded items
		 */
		public Array GetRecentlyLoadedItems()
		{
			Array retVal = Array.CreateInstance(typeof(String), 1, NumberOfRecentlyViewedItems);
			if (RecentItemsRegRootKey != null)
			{
				string[] names = RecentItemsRegRootKey.GetValueNames();
				int count = names.Length;
				if (count > NumberOfRecentlyViewedItems)
					count = NumberOfRecentlyViewedItems;

				for (int i = 0; i < count; i++)
				{
					string val = (string)RecentItemsRegRootKey.GetValue(names[i]);
					retVal.SetValue(val, 0, i);
				}
			}

			return retVal;
		}

		/**
		 * will add the item to the recent items and move the last one off the list if it is full
		 * it also will only add the item if it does not already exist in the list
		 */
		public void AddRecentItem(string item)
		{
			if (RecentItemsRegRootKey != null)
			{
				string[] names = RecentItemsRegRootKey.GetValueNames();
				int count = names.Length;
				if (count > (NumberOfRecentlyViewedItems - 1))
					count = NumberOfRecentlyViewedItems - 1;

				// check to see if it already is in the list
				int prevIndex = -1;
				for (int i = 0; i < names.Length; i++)
				{
					string val = (string)RecentItemsRegRootKey.GetValue(names[i]);
					if (val.ToUpper().CompareTo(item.ToUpper()) == 0)
					{
						prevIndex = i;
						break;
					}
				}

				// if we found it we only have to sort the list up to that point
				if (prevIndex != -1)
				{
					count = prevIndex + 1;
					for (int i = count - 1; i > 0; i--)
					{
						RecentItemsRegRootKey.SetValue("" + i, RecentItemsRegRootKey.GetValue(names[i - 1]));
					}
				}
				else
				{
					for (int i = count - 1; i >= 0; i--)
					{
						RecentItemsRegRootKey.SetValue("" + (i + 1), RecentItemsRegRootKey.GetValue(names[i]));
					}
				}

				RecentItemsRegRootKey.SetValue("0", item);
			}
		}

		/**
		 * Get the applications install path
		 */
		public string GetInstallationPath()
		{
			String installPath = "";
			String defaultValue = "C:\\Program Files\\Security Innovation\\Holodeck Enterprise Edition";
			if (RegRootKey != null)
			{
				installPath = (String)RegRootKey.GetValue("InstallPath");
				if (installPath != "")
					return installPath;
				else if (System.IO.Directory.Exists(defaultValue))
					return defaultValue;
				else
				{
					System.Windows.Forms.MessageBox.Show("The Holodeck installation path could not be read from the registry, Holodeck will exit now", "Holodeck");
					throw new Exception("Installation Path Not Found");
				}

			}
			else
			{
				if (System.IO.Directory.Exists(defaultValue))
					return installPath;
				else
				{
					System.Windows.Forms.MessageBox.Show("The Holodeck installation path could not be read from the registry, Holodeck will exit now", "Holodeck");
					throw new Exception("Installation Path Not Found");
				}
			}
		}

		/**
		 * Get the folder that stores the dtds
		 */
		public string GetDTDFolder()
		{
			String defaultValue = "C:\\Program Files\\Security Innovation\\Holodeck Enterprise Edition";
			String registryValue = "";
			if (RegRootKey != null)
			{
				registryValue =  (string)RegRootKey.GetValue("DtdPath");
				if (registryValue != "")
					return registryValue;
				else
					return defaultValue;
			}
			else
				return defaultValue;
		}

		/**
		 * Get the application's function database folder
		 */
		public string GetFunctionDatabaseFolderPath()
		{
			String path = "";
			if (RegRootKey != null)
			{
				path = GetInstallationPath();
				if (path.EndsWith("\\"))
					path += (string)RegRootKey.GetValue("FunctionDatabaseFolder");
				else
					path += "\\" + (string)RegRootKey.GetValue("FunctionDatabaseFolder");
				if (path == "\\")
					path += "function_db";
				return path;
			}
			else
			{
				path = this.GetInstallationPath();
				path += "\\function_db";
				return path;
			}
		}

		/**
		 * Get the application's last project save path
		 */
		public string GetLastProjectSavePath()
		{
			if (RegRootKey != null)
			{
				return (string)RegRootKey.GetValue("LastProjectSavePath");
			}
			else
				return this.GetInstallationPath();
		}

		/**
		 * Set the application's last project save path
		 */
		public void SetLastProjectSavePath(String lastProjectSavePath)
		{
			if ( (RegRootKey != null) && (lastProjectSavePath != null) )
			{
				//strip any trailing '\'
				char [] array = {'\\'};
				lastProjectSavePath = lastProjectSavePath.TrimEnd(array);
				RegRootKey.SetValue ("LastProjectSavePath", lastProjectSavePath);
			}

		}

		/**
		 * Get the application's last log save path
		 */
		public string GetLastLogSavePath()
		{
			string retValue = "";
			if (RegRootKey != null)
			{
				retValue = (string)RegRootKey.GetValue("LastLogSavePath");
				if (retValue == null)
					return "";
				else
					return retValue;
			}

			return null;
		}

		/**
		 * Set the application's last log save path
		 */
		public void SetLastLogSavePath(String lastLogSavePath)
		{
			if (RegRootKey != null)
				RegRootKey.SetValue ("LastLogSavePath", lastLogSavePath);
		}

		/**
		 * Get the application's major version number
		 */
		public int GetMajorVersion()
		{
			try
			{
				if (RegRootKey != null)
					return (int)RegRootKey.GetValue("MajorVersion");
				else
					return 0;
			}
			catch(Exception)
			{
				return 0;
			}
		}

		/**
		 * Get the application's minor version number
		 */
		public int GetMinorVersion()
		{
			try
			{
				if (RegRootKey != null)
					return (int)RegRootKey.GetValue("MinorVersion");
				else
					return 0;
			}
			catch(Exception)
			{
				return 0;
			}
		}

		/**
		 * Get the application's build number
		 */
		public int GetBuildNumber()
		{
			try
			{
				if (RegRootKey != null)
					return (int)RegRootKey.GetValue("BuildNumber");
				else
					return 0;
			}
			catch(Exception)
			{
				return 0;
			}
		}

		/**
		 * Get the application's full version string
		 */
		public string GetVersionString()
		{
			string retVal = String.Concat(GetMajorVersion().ToString(), ".");
			retVal = String.Concat(retVal, GetMinorVersion().ToString(), ".");
			retVal = String.Concat(retVal, GetBuildNumber());

			return retVal;
		}
	}
}
