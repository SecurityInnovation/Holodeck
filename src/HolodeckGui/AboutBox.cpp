//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AboutBox.cpp
//
// DESCRIPTION: Contains implementation for the class AboutBox
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "AboutBox.h"
#include "defines.h"
#include "ImageListManager.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		AboutBox
	// Description: Constructor for the AboutBox class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AboutBox::AboutBox()
	{
		InitializeUI();
	}

	//*************************************************************************
	// Method:		~AboutBox
	// Description: Destructor for the AboutBox class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AboutBox::~AboutBox()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes all UI components for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AboutBox::InitializeUI()
	{
		Button *closeButton = new Button();
		closeButton->DialogResult = DialogResult::OK;
		closeButton->Text = "OK";
		closeButton->Location = Point(400, 120);

		Label *copyRightLabel = new Label();
		copyRightLabel->Location = Point(14, 103);
		copyRightLabel->Size = System::Drawing::Size(258, 16);
		copyRightLabel->Text = "Copyright 2002-2008, Security Innovation, Inc.";

		Label *versionLabel = new Label();
		versionLabel->Location = Point(14, 119);
		versionLabel->Size = System::Drawing::Size(100, 16);
		versionLabel->Text = String::Concat("Version ", GuiSupport::RegistryHelper::getInstance()->GetVersionString());

		webSiteLinkLabel = new LinkLabel();
		webSiteLinkLabel->Location = Point(14, 135);
		webSiteLinkLabel->Size = System::Drawing::Size(300, 23);
		webSiteLinkLabel->TabStop = true;
		webSiteLinkLabel->Text = "http://www.securityinnovation.com";
		webSiteLinkLabel->LinkClicked += new LinkLabelLinkClickedEventHandler(this, webSiteLinkClicked);

		Label *magicLibraryLabel = new Label();
		magicLibraryLabel->Location = Point(14, 180);
		magicLibraryLabel->Size = System::Drawing::Size(135, 16);
		magicLibraryLabel->Text = "GUI Library provided by:";

		magicLibraryLinkLabel = new LinkLabel();
		magicLibraryLinkLabel->Location = Point(149, 180);
		magicLibraryLinkLabel->Size = System::Drawing::Size(150, 23);
		magicLibraryLinkLabel->TabStop = true;
		magicLibraryLinkLabel->Text = "http://www.dotnetmagic.com";
		magicLibraryLinkLabel->LinkClicked += new LinkLabelLinkClickedEventHandler(this, magicLibraryLinkClicked);

		PictureBox *logoBox = new PictureBox();
		logoBox->Image = ImageListManager::getInstance()->LogoImage;
		logoBox->Location = System::Drawing::Point(0, 0);
		logoBox->Size = System::Drawing::Size(490, 96);

		Panel *productInfoPanel = new Panel();
		productInfoPanel->Dock = DockStyle::Fill;

		productInfoPanel->Controls->Add(closeButton);
		productInfoPanel->Controls->Add(copyRightLabel);
		productInfoPanel->Controls->Add(versionLabel);
		productInfoPanel->Controls->Add(webSiteLinkLabel);
		productInfoPanel->Controls->Add(magicLibraryLabel);
		productInfoPanel->Controls->Add(magicLibraryLinkLabel);
		productInfoPanel->Controls->Add(logoBox);

		this->Text = "About Holodeck Enterprise Edition";
		this->ClientSize = System::Drawing::Size(490, 196);
		this->FormBorderStyle = FormBorderStyle::FixedSingle;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ShowInTaskbar = false;
		this->StartPosition = FormStartPosition::CenterScreen;
		this->Controls->Add(productInfoPanel);

		this->Icon = ImageListManager::getInstance()->AppIcon;
	}

	//****************************************************************************
	// Method:		webSiteLinkClicked
	// Description: Handles the click of the si website link
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void AboutBox::webSiteLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args)
	{
		try
		{
			if (UIEventRegistry::getInstance()->OnURLClicked)
				UIEventRegistry::getInstance()->OnURLClicked->Invoke(webSiteLinkLabel->Text);
		}
		catch (Exception *e)
		{
			String *msg = String::Concat("Could not open the browser to the location: ", webSiteLinkLabel->Text->ToString(), " - \nReason: ", e->Message->ToString());
			UserNotification::ErrorNotify(msg);
		}
	}

	//****************************************************************************
	// Method:		magicLibraryLinkClicked
	// Description: Handles the click of the magic library website link
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void AboutBox::magicLibraryLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args)
	{
		try
		{
			if (UIEventRegistry::getInstance()->OnURLClicked)
				UIEventRegistry::getInstance()->OnURLClicked->Invoke(magicLibraryLinkLabel->Text);
		}
		catch (Exception *e)
		{
			String *msg = String::Concat("Could not open the browser to the location: ", magicLibraryLinkLabel->Text->ToString(), " - \nReason: ", e->Message->ToString());
			UserNotification::ErrorNotify(msg);
		}
	}
}