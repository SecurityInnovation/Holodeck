#pragma once

#include "defines.h"
#include "registry.h"
#include "RegistrationMgr.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Data.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>

#using <NewIntercepts.dll>
using namespace AddNewIntercepts;

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Microsoft::Win32;
using namespace System::IO;

using namespace HolodeckRegistration;

namespace HolodeckGui
{
	/// <summary> 
	/// Summary for FaultyUserLibrariesDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class FaultyUserLibrariesDialog : public System::Windows::Forms::Form
	{
	public: 
		FaultyUserLibrariesDialog(void)
		{
			InitializeComponent();
		}

		void RemoveLibrary(String *dllFileName, String *xmlFileName);
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}


	private: System::Windows::Forms::ColumnHeader *  chReplDll;
	private: System::Windows::Forms::ColumnHeader *  chXMLFile;
	private: System::Windows::Forms::Button *  btnRemove;

	private: System::Windows::Forms::Label *  lblGeneralInfo;
	private: System::Windows::Forms::Label *  lblRedInfo;
	private: bool containsFaultyModules;
	private: System::Windows::Forms::ListView *  lvIntercepts;
	private: System::Windows::Forms::Button *  btnClose;
	private: System::Windows::Forms::Button *  btnAddNew;
	private: String *HolodeckInstallPath;
	private: String *replacementDir;
	private: String *functionDBFolder;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->lvIntercepts = new System::Windows::Forms::ListView();
			this->chReplDll = new System::Windows::Forms::ColumnHeader();
			this->chXMLFile = new System::Windows::Forms::ColumnHeader();
			this->btnRemove = new System::Windows::Forms::Button();
			this->lblGeneralInfo = new System::Windows::Forms::Label();
			this->lblRedInfo = new System::Windows::Forms::Label();
			this->btnClose = new System::Windows::Forms::Button();
			this->btnAddNew = new System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// lvIntercepts
			// 
			System::Windows::Forms::ColumnHeader* __mcTemp__1[] = new System::Windows::Forms::ColumnHeader*[2];
			__mcTemp__1[0] = this->chReplDll;
			__mcTemp__1[1] = this->chXMLFile;
			this->lvIntercepts->Columns->AddRange(__mcTemp__1);
			this->lvIntercepts->FullRowSelect = true;
			this->lvIntercepts->GridLines = true;
			this->lvIntercepts->HideSelection = false;
			this->lvIntercepts->Location = System::Drawing::Point(8, 48);
			this->lvIntercepts->Name = S"lvIntercepts";
			this->lvIntercepts->Size = System::Drawing::Size(416, 136);
			this->lvIntercepts->TabIndex = 0;
			this->lvIntercepts->View = System::Windows::Forms::View::Details;
			// 
			// chReplDll
			// 
			this->chReplDll->Text = S"Replacement Library";
			this->chReplDll->Width = 200;
			// 
			// chXMLFile
			// 
			this->chXMLFile->Text = S"Database File";
			this->chXMLFile->Width = 212;
			// 
			// btnRemove
			// 
			this->btnRemove->Location = System::Drawing::Point(112, 192);
			this->btnRemove->Name = S"btnRemove";
			this->btnRemove->Size = System::Drawing::Size(96, 24);
			this->btnRemove->TabIndex = 1;
			this->btnRemove->Text = S"Remove";
			this->btnRemove->Click += new System::EventHandler(this, btnRemove_Click);
			// 
			// lblGeneralInfo
			// 
			this->lblGeneralInfo->Location = System::Drawing::Point(8, 8);
			this->lblGeneralInfo->Name = S"lblGeneralInfo";
			this->lblGeneralInfo->Size = System::Drawing::Size(416, 32);
			this->lblGeneralInfo->TabIndex = 3;
			this->lblGeneralInfo->Text = S"The following is a list of libraries, specified by the user for interception by H" 
				S"olodeck. You may add or remove entries using this manager.";
			// 
			// lblRedInfo
			// 
			this->lblRedInfo->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblRedInfo->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->lblRedInfo->ForeColor = System::Drawing::SystemColors::WindowText;
			this->lblRedInfo->Location = System::Drawing::Point(8, 224);
			this->lblRedInfo->Name = S"lblRedInfo";
			this->lblRedInfo->Size = System::Drawing::Size(416, 128);
			this->lblRedInfo->TabIndex = 4;
			this->lblRedInfo->Text = S"Note: Entries highlighted in in red are missing some files. This could be because" 
				S":\r\n\r\n  - You did not yet build the project which was generated.\r\n\r\n  - You delet" 
				S"ed the replacement .dll file from the\r\n    [Holodeck]\\Replacement_Libraries fold" 
				S"er.\r\n\r\nYou may restore the files or remove the entry from the Holodeck database";
			// 
			// btnClose
			// 
			this->btnClose->Location = System::Drawing::Point(328, 192);
			this->btnClose->Name = S"btnClose";
			this->btnClose->Size = System::Drawing::Size(96, 24);
			this->btnClose->TabIndex = 5;
			this->btnClose->Text = S"Close";
			this->btnClose->Click += new System::EventHandler(this, btnClose_Click);
			// 
			// btnAddNew
			// 
			this->btnAddNew->Location = System::Drawing::Point(8, 192);
			this->btnAddNew->Name = S"btnAddNew";
			this->btnAddNew->Size = System::Drawing::Size(96, 24);
			this->btnAddNew->TabIndex = 6;
			this->btnAddNew->Text = S"Add new";
			this->btnAddNew->Click += new System::EventHandler(this, btnAddNew_Click);
			// 
			// FaultyUserLibrariesDialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(434, 361);
			this->Controls->Add(this->btnAddNew);
			this->Controls->Add(this->btnClose);
			this->Controls->Add(this->lblRedInfo);
			this->Controls->Add(this->lblGeneralInfo);
			this->Controls->Add(this->btnRemove);
			this->Controls->Add(this->lvIntercepts);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"FaultyUserLibrariesDialog";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"Holodeck Intercepts Manager";
			this->Load += new System::EventHandler(this, FaultyUserLibrariesDialog_Load);
			this->ResumeLayout(false);

		}		
	private: System::Void FaultyUserLibrariesDialog_Load(System::Object *  sender, System::EventArgs *  e)
			 {
				if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
					this->btnAddNew->Enabled = false;
				else
					this->btnAddNew->Enabled = true;
			 }

	public: void PerformCheckAndPopulateList()
			{
				Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
				HolodeckInstallPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
				replacementDir = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));
				replacementDir = String::Concat(HolodeckInstallPath, "\\", replacementDir);
				functionDBFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
				functionDBFolder = String::Concat(HolodeckInstallPath, "\\", functionDBFolder);

				String * nativeUserXMlFiles[];
				String * dotnetUserXMlFiles[];
				
				nativeUserXMlFiles = Directory::GetFiles (functionDBFolder, String::Concat (NATIVE_USER_FILENAME_DB_PREFIX, "*.xml"));
				dotnetUserXMlFiles = Directory::GetFiles (functionDBFolder, String::Concat (DOTNET_USER_FILENAME_DB_PREFIX, "*.xml"));
				ArrayList * xmlFiles = new ArrayList();

				for (int i=0; i < nativeUserXMlFiles->Length; i++)
					xmlFiles->Add (nativeUserXMlFiles->Item[i]);

				for (int i=0; i < dotnetUserXMlFiles->Length; i++)
					xmlFiles->Add (dotnetUserXMlFiles->Item[i]);

				String * replDllName = NULL;
				bool entryFound = false;

				this->lvIntercepts->Items->Clear();
				
				for (int i=0; i < xmlFiles->Count; i++)
				{
					String * xmlName = Path::GetFileName (dynamic_cast <String*> (xmlFiles->Item[i]));
					replDllName = xmlName;
					replDllName = replDllName->Substring (0, replDllName->Length - 4);
					replDllName = replDllName->Substring (7);

					//look for this replacement dll in the replacement folder
					entryFound = File::Exists (String::Concat (replacementDir, "\\", replDllName));
					
					if (!entryFound)
						this->containsFaultyModules = true;

					ListViewItem * lvi = new ListViewItem(replDllName);
					lvi->SubItems->Add (xmlName);

					if (entryFound)
						lvi->ForeColor = System::Drawing::SystemColors::WindowText;
					else
						lvi->ForeColor = System::Drawing::Color::Red;

					this->lvIntercepts->Items->Add (lvi);
				}

				if (this->containsFaultyModules)
					this->Height = 384;
				else
					this->Height = 245;

				if (this->lvIntercepts->Items->Count > 0)
					this->lvIntercepts->Items->Item[0]->Selected = true;
			}

	public:
		__property bool get_ContainsFaultyModules() {return this->containsFaultyModules;}

		private: System::Void btnClose_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 this->Close();
		 }

		private: System::Void btnRemove_Click(System::Object *  sender, System::EventArgs *  e)
		 {
			 if (this->lvIntercepts->SelectedItems->Count > 0)
			 {
				 String * replDllName = String::Concat (replacementDir, "\\", this->lvIntercepts->SelectedItems->Item[0]->Text);
				 String * XMLfileName = String::Concat (functionDBFolder, "\\", this->lvIntercepts->SelectedItems->Item[0]->SubItems->Item[1]->Text);

				 try
				 {
					RemoveLibrary(replDllName, XMLfileName);

					this->lvIntercepts->Items->Remove (this->lvIntercepts->SelectedItems->Item[0]);
				 }
				 catch (...)
				 {
					// do nothing
				 }

			 }
		 }

		private: System::Void btnAddNew_Click(System::Object *  sender, System::EventArgs *  e)
		{
			// Check if we are even to try running this feature
			if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
			{
				return;
			}

			// Check if .NET 2.0 "is" available
			RegistryKey *nkey = Microsoft::Win32::Registry::LocalMachine->OpenSubKey("Software\\Microsoft\\.NETFramework\\Policy\\v2.0");
			if (!nkey)
			{
				AddNewIntercepts::AddCustomInterceptWizard *p = new AddNewIntercepts::AddCustomInterceptWizard();
				p->ShowDialog(this);
			}
			else
			{
				System::Diagnostics::Process *ne	= new System::Diagnostics::Process();
				String *sNewInterceptExePath		= (String*)HolodeckInstallPath->Clone();
				sNewInterceptExePath				= String::Concat(sNewInterceptExePath, "\\NewIntercepts\\NewIntercepts.exe");

				ne->Start(sNewInterceptExePath);
			}
		}
	};
}