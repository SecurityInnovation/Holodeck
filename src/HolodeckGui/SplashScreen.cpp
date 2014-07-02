//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SplashScreen.cpp
//
// DESCRIPTION: Contains implementation for the class SplashScreen
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "SplashScreen.h"
#include "defines.h"
#include "ImageListManager.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		SplashScreen
	// Description: Constructor for the SplashScreen class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SplashScreen::SplashScreen()
	{
		InitializeUI();
	}

	//*************************************************************************
	// Method:		~SplashScreen
	// Description: Destructor for the SplashScreen class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SplashScreen::~SplashScreen()
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
	void SplashScreen::InitializeUI()
	{
		Label *copyRightLabel = new Label();
		copyRightLabel->Location = Point(14, 103);
		copyRightLabel->Size = System::Drawing::Size(490, 16);
		copyRightLabel->Text = "Copyright 2002-2008, Security Innovation, Inc.";

		Label *versionLabel = new Label();
		versionLabel->Location = Point(14, 119);
		versionLabel->Size = System::Drawing::Size(100, 16);
		versionLabel->Text = String::Concat("Version ", GuiSupport::RegistryHelper::getInstance()->GetVersionString());

		webSiteLinkLabel = new Label();
		webSiteLinkLabel->Location = Point(14, 135);
		webSiteLinkLabel->Size = System::Drawing::Size(300, 23);
		webSiteLinkLabel->TabStop = true;
		webSiteLinkLabel->Text = "http://www.securityinnovation.com";
		//webSiteLinkLabel->LinkClicked += new LinkLabelLinkClickedEventHandler(this, webSiteLinkClicked);

		PictureBox *logoBox = new PictureBox();
		logoBox->Image = ImageListManager::getInstance()->LogoImage;
		logoBox->Location = Point(0, 0);
		logoBox->Size = System::Drawing::Size(490, 96);

		Panel *productInfoPanel = new Panel();
		productInfoPanel->Dock = DockStyle::Fill;

		productInfoPanel->Controls->Add(copyRightLabel);
		productInfoPanel->Controls->Add(versionLabel);
		productInfoPanel->Controls->Add(webSiteLinkLabel);
		productInfoPanel->Controls->Add(logoBox);

		this->ClientSize = System::Drawing::Size(490, 166);
		this->FormBorderStyle = FormBorderStyle::None;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ShowInTaskbar = false;
		this->StartPosition = FormStartPosition::CenterScreen;
		this->TopMost = true;
		this->Controls->Add(productInfoPanel);

		this->Icon = ImageListManager::getInstance()->AppIcon;
	}
/*  removed link label as it caused an error dlg in some instances when link was clicked
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
	void SplashScreen::webSiteLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args)
	{
		try
		{
			System::Diagnostics::Process::Start("iexplore.exe", webSiteLinkLabel->Text);
		}
		catch (System::ComponentModel::Win32Exception *e)
		{
			String *msg = String::Concat("Could not open the browser to the location: ", webSiteLinkLabel->Text->ToString(), " - \nReason: ", e->Message->ToString());
			UserNotification::ErrorNotify(msg);
		}
	}*/
}