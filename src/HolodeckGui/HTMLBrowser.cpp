//*************************************************************************
// Copyright (C) Security Innovation Corp., 2002-2004 – All Rights Reserved.
//
// FILE:		HTMLBrowser.cpp
//
// DESCRIPTION: Contains implementation for the class HTMLBrowser
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 06 Aug 2004		 Ady K.		 File created.
//*************************************************************************
#include "HTMLBrowser.h"

namespace HolodeckGui
{

	//*************************************************************************
	// Method:		HTMLBrowser
	// Description: Constructor for the HTMLBrowser class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HTMLBrowser::HTMLBrowser()
	{
		this->browser = new AxSHDocVw::AxWebBrowser();
		this->browser->BeginInit();
		this->browser->Parent = new Label(); // this is stupid, but needed so that I dont get an InvalidActiveXStateException
		this->browser->TabIndex = 1;
		this->browser->Dock = DockStyle::Fill;
		this->browser->TitleChange += new AxSHDocVw::DWebBrowserEvents2_TitleChangeEventHandler(this, title_ChangedHandler);
		this->browser->BeforeNavigate2 += new AxSHDocVw::DWebBrowserEvents2_BeforeNavigate2EventHandler(this, before_NavigateHandler);
		this->browser->EndInit();

		this->AllowNavigationInThisBrowser = true;
		this->currentUrl = "";
		this->targetFrame = 0;
	}

	//*************************************************************************
	// Method:		~HTMLBrowser
	// Description: Destructor for the HTMLBrowser class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HTMLBrowser::~HTMLBrowser()
	{

	}

	//*************************************************************************
	// Method:		Navigate
	// Description: Navigates to the specified URL
	//
	// Parameters:
	//	url - the url to navigate to
	//
	// Return Value: None
	//*************************************************************************
	void HTMLBrowser::Navigate (String * url)
	{
		if (browser)
		{
			try
			{
				this->Navigate (url, 0);
			}
			catch (...)
			{
				
			}
		}
	}

	//*************************************************************************
	// Method:		Navigate
	// Description: Navigates to the specified URL in the specified frame
	//
	// Parameters:
	//	url - the url to navigate to
	//	targetFrame - the frame to use for displaying the page
	//
	// Return Value: None
	//*************************************************************************
	void HTMLBrowser::Navigate (String * url, String * targetFrame)
	{
		bool ieInstalled = true;

		try
		{
			Object * o = System::Reflection::Missing::Value;

			this->targetFrame = targetFrame;
			if ((File::Exists(url)) ||
				(url->ToLower()->StartsWith(S"about:")) ||
				(url->ToLower()->StartsWith(S"file:")))
			{
				try
				{
					browser->Navigate(url, &o, &o, &o, &o);
				}
				catch (...)
				{
				}
			}
			else
			{
				String * externalStr = S"external*";
				if (url->ToLower()->StartsWith(externalStr))
				{
					// Request to make any link external, remove tag
					url = url->Substring(externalStr->Length);
				}

				String * iePath = 0;
				RegistryKey * iePathKey = Registry::LocalMachine->OpenSubKey(S"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\iexplore.exe");
				if (iePathKey != 0)
					iePath = dynamic_cast <String*> (iePathKey->GetValue(0));

				System::Diagnostics::Process * p = new System::Diagnostics::Process();
				if (iePath != 0)
				{
					p->StartInfo->FileName = iePath;
					p->StartInfo->WorkingDirectory = Path::GetDirectoryName(iePath);

					if (!File::Exists(iePath))
					{
						ieInstalled = false;
						throw new Exception();
					}
				}
				else
				{
					ieInstalled = false;
					p->StartInfo->FileName = "iexplore.exe";
				}

				p->StartInfo->Arguments = url;
				p->StartInfo->ErrorDialog = true;
				p->StartInfo->UseShellExecute = false;
				p->Start();
			}
		}
		catch(Exception *)
		{
			String * msg = String::Concat(S"Could not launch Internet Explorer to display this link.\nIf you have another browser installed, please navigate to the page ", url, S" in that browser.");
			
			// if we couldn't get the IE reg key, and got an exception, IE probably wasn't installed so tell the user
			if (!ieInstalled)
				UserNotification::ErrorNotify (msg);
				
			// else some other error happened, but don't do anything about it
		}
	}


	//*************************************************************************
	// Method:		title_ChangedHandler
	// Description: Handles the event in which the title of the page is changed
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the object with the event information
	//
	// Return Value: None
	//*************************************************************************
	void HTMLBrowser::title_ChangedHandler(Object * sender, DWebBrowserEvents2_TitleChangeEvent * e)
    {
      if (this->OnTitleChanged != 0)
		  this->OnTitleChanged (this, e->text);
    }

	//*************************************************************************
	// Method:		before_NavigateHandler
	// Description: Handles the event in which a link is clicked
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the object with the event information
	//
	// Return Value: None
	//*************************************************************************
	void HTMLBrowser::before_NavigateHandler(Object * sender, DWebBrowserEvents2_BeforeNavigate2Event * e)
    {
		String * url= e->uRL->ToString();

		// if we don't care that the link shows up in this pane, let it
		if (this->AllowNavigationInThisBrowser)
		{
			currentUrl = url;
			targetFrame = 0;
			return;
		}

		// if this is the first time here, let the page display
		if ((currentUrl->CompareTo("") == 0) && (!e->headers || e->headers->ToString()->CompareTo("") == 0))
		{
			currentUrl = url;
			targetFrame = 0;
			return;
		}

		// this is used to stop link clicks from the dynamic help pane go through
		// while letting focus events and other navigations go through
		if ((targetFrame == 0) || (targetFrame->CompareTo(S"") == 0))
		{
			this->OnLinkClicked(this, url);
			e->cancel = true;
		}

		targetFrame = 0;
		currentUrl = url;
    }

}