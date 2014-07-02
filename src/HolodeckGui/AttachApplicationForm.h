
#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

#include <windows.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the attach to application on project open dialog
	//*************************************************************************
public __gc class AttachApplicationForm : public System::Windows::Forms::Form
	{
	public:
		AttachApplicationForm(String * fullExecutablePath);
		~AttachApplicationForm();

		__property int get_ProcessID() { return selProcId; }
		__property bool get_LaunchNewAut() { return launchAppRadio->Checked; }

	private:
		String * executablePath;
		int executableCount;
		ColumnHeader *columnHeader1;
		ColumnHeader *columnHeader2;
		ListView *processListView;
		LinkLabel *refreshLink;
		Label *description;
		Button *OkButton;
		//Button *CancelButton;
		RadioButton *attachAppRadio;
		RadioButton *launchAppRadio;
		int selProcId;

		void onRadioCheckChanged(Object *sender, EventArgs *e);
		void onRefreshClicked(Object *sender, EventArgs *e);
		void onSelectedIndexChanged(Object *sender, EventArgs *e);
		void onOkClicked(Object *sender, EventArgs *e);
		//void onSkipClicked(Object *sender, EventArgs *e);

	};
}