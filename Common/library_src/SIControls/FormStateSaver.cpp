//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		FormStateSaver.cpp
//
// DESCRIPTION: Contains implementation for the FormStateSaver class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#include "FormStateSaver.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		FormStateSaver
	// Description: Constructor for the FormStateSaver class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FormStateSaver::FormStateSaver()
	{
		allowSaveMinimized = false;
	}

	//*************************************************************************
	// Method:		~FormStateSaver
	// Description: Destructor for the FormStateSaver class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FormStateSaver::~FormStateSaver()
	{
	}

	//*************************************************************************
	// Method:		set_Parent
	// Description: sets the parent of the class
	//
	// Parameters:
	//	value - the value of the parent to set
	//
	// Return Value: None
	//*************************************************************************
	void FormStateSaver::set_Parent(Form *value)
	{
		parent = value;

		if (parent)
		{
			parent->Closing += new CancelEventHandler(this, OnClosing);
			parent->Resize += new EventHandler(this, OnResize);
			parent->Move += new EventHandler(this, OnMove);
			parent->Load += new EventHandler(this, OnLoad);

			normalWidth = parent->Width;
			normalHeight = parent->Height;
		}
		else
		{
			normalWidth = 0;
			normalHeight = 0;
		}
	}

	//*************************************************************************
	// Method:		OnResize
	// Description: Called when the parent form is resized
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FormStateSaver::OnResize(Object *sender, EventArgs *args)
	{
		if (parent && (parent->WindowState == FormWindowState::Normal))
		{
			normalWidth = parent->Width;
			normalHeight = parent->Height;
		}
	}

	//*************************************************************************
	// Method:		OnMove
	// Description: Called when the parent form is moved
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FormStateSaver::OnMove(Object *sender, EventArgs *args)
	{
		if (!parent)
			return;

		if (parent->WindowState == FormWindowState::Normal)
		{
			normalLeft = parent->Left;
			normalTop = parent->Top;
		}

		windowState = parent->WindowState;
	}

	//*************************************************************************
	// Method:		OnClosing
	// Description: Called just before the parent form is closed
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FormStateSaver::OnClosing(Object *sender, CancelEventArgs *args)
	{
		if (!regPath)
			return;

		try
		{
			RegistryKey *key = Registry::LocalMachine->CreateSubKey(regPath);
			key->SetValue("Left", __box(normalLeft));
			key->SetValue("Top", __box(normalTop));
			key->SetValue("Width", __box(normalWidth));
			key->SetValue("Height", __box(normalHeight));

			if (!allowSaveMinimized)
			{
				if (windowState == FormWindowState::Minimized)
					windowState = FormWindowState::Normal;
			}

			key->SetValue("WindowState", __box((int)windowState));

			// fire save event
			if (SaveState)
				SaveState->Invoke(this, key);
		}
		catch(Exception *)
		{
		}
	}

	//*************************************************************************
	// Method:		OnLoad
	// Description: Called when the parent form is loaded to the screen
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void FormStateSaver::OnLoad(Object *sender, EventArgs *args)
	{
		if (!regPath)
			return;

		try
		{
			RegistryKey *key = Registry::LocalMachine->OpenSubKey(regPath);
			if (key)
			{
				int left = Convert::ToInt32(key->GetValue("Left", __box(parent->Left)));
				int top = Convert::ToInt32(key->GetValue("Top", __box(parent->Top)));
				int width = Convert::ToInt32(key->GetValue("Width", __box(parent->Width)));
				int height = Convert::ToInt32(key->GetValue("Height", __box(parent->Height)));
				FormWindowState state = (FormWindowState)Convert::ToInt32(key->GetValue("WindowState", __box((int)parent->WindowState)));

				parent->Location = System::Drawing::Point(left, top);
				parent->Size = System::Drawing::Size(width, height);
				parent->WindowState = state;

				if (LoadState)
					LoadState->Invoke(this, key);
			}
		}
		catch(Exception *)
		{
		}
	}
}