using System;
using System.Collections;
using System.Windows.Forms;

namespace GuiSupport
{
	/// <summary>
	/// Summary description for AbstractLaunchWizard.
	/// </summary>
	public interface AbstractLaunchWizard
	{
		bool PredefineTests
		{
			get;
		}

		bool ProcessChain
		{
			get;
		}

		bool AppPaused
		{
			get;
			set;
		}

		bool UseProcessFile
		{
			get;
			set;
		}

		bool UseProcessID
		{
			get;
			set;
		}

		string ProcessPath
		{
			get;
			set;
		}

		ArrayList FilteredOutFunctions
		{
			get;
		}

		ListView ProcessList
		{
			get;
		}
	}
}
