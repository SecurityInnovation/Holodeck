//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogListView.cpp
//
// DESCRIPTION: Contains implementation for the class LogListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogListView.h"
#include "defines.h"
#include "LogPane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		LogListView
	// Description: Constructor for the LogListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogListView::LogListView()
	{
		this->View = System::Windows::Forms::View::Details;
		this->FullRowSelect = true;
		this->Scrollable = true;
		this->MultiSelect = false;
		this->AllowColumnReorder = true;
		this->HideSelection = false;

		Filter_OKClick = new EventHandler (this, filter_OKClick);

		allColumns = new ArrayList();

		timeStampHeader = new ColumnHeader();
		timeStampHeader->Text = TIME_STAMP_HEADER_TEXT;
		timeStampHeader->Width = 140;

		threadIDHeader = new ColumnHeader();
		threadIDHeader->Text = THREAD_ID_HEADER_TEXT;
		threadIDHeader->Width = 50;

		categoryHeader = new ColumnHeader();
		categoryHeader->Text = CATEGORY_HEADER_TEXT;
		categoryHeader->Width = 80;

		dllHeader = new ColumnHeader();
		dllHeader->Text = DLL_HEADER_TEXT;
		dllHeader->Width = 80;

		functionHeader = new ColumnHeader();
		functionHeader->Text = FUNCTION_HEADER_TEXT;
		functionHeader->Width = 80;

		returnValueHeader = new ColumnHeader();
		returnValueHeader->Text = RETURN_VALUE_HEADER_TEXT;
		returnValueHeader->Width = 80;

		errorCodeHeader = new ColumnHeader();
		errorCodeHeader->Text = ERROR_CODE_HEADER_TEXT;
		errorCodeHeader->Width = 80;

		exceptionHeader = new ColumnHeader();
		exceptionHeader->Text = EXCEPTION_HEADER_TEXT;
		exceptionHeader->Width = 80;

		this->AddColumnHeader (timeStampHeader, new EventHandler (this, customfilter_TimeStamp));
		this->AddColumnHeader (threadIDHeader, new EventHandler (this, customfilter_ThreadID));
		this->AddColumnHeader (categoryHeader, new EventHandler (this, customfilter_Category));
		this->AddColumnHeader (dllHeader, new EventHandler (this, customfilter_DLL));
		this->AddColumnHeader (functionHeader, new EventHandler (this, customfilter_Functions));
		this->AddColumnHeader (returnValueHeader, new EventHandler (this, customfilter_ReturnValue));
		this->AddColumnHeader (errorCodeHeader, new EventHandler (this, customfilter_ErrorCode));
		this->AddColumnHeader (exceptionHeader, new EventHandler (this, customfilter_Exception));

		allColumns->Add(timeStampHeader);
		allColumns->Add(threadIDHeader);
		allColumns->Add(categoryHeader);
		allColumns->Add(dllHeader);
		allColumns->Add(functionHeader);
		allColumns->Add(returnValueHeader);
		allColumns->Add(errorCodeHeader);
		allColumns->Add(exceptionHeader);

		parameterHeaders = new ColumnHeader *[MAX_PARAMETER_COLUMNS];
		for (int i = 0; i < MAX_PARAMETER_COLUMNS; i++)
		{
			parameterHeaders[i] = new ColumnHeader();
			parameterHeaders[i]->Text = String::Concat(PARAM_HEADER_TEXT_PREFIX, (__box(i + 1))->ToString());
			parameterHeaders[i]->Width = 80;

			this->AddColumnHeader (parameterHeaders[i], new EventHandler (this, customfilter_Params));
			allColumns->Add(parameterHeaders[i]);
		}

		
		// call the custom filter methods so that they create the form objects
		this->customfilter_TimeStamp (NULL, EventArgs::Empty);
		this->customfilter_ThreadID (NULL, EventArgs::Empty);
		this->customfilter_Category (NULL, EventArgs::Empty);
		this->customfilter_DLL (NULL, EventArgs::Empty);
		this->customfilter_Functions (NULL, EventArgs::Empty);
		this->customfilter_Params (NULL, EventArgs::Empty);
		this->customfilter_ReturnValue (NULL, EventArgs::Empty);
		this->customfilter_ErrorCode (NULL, EventArgs::Empty);
		this->customfilter_Exception (NULL, EventArgs::Empty);
	}

	//*************************************************************************
	// Method:		~LogListView
	// Description: Destructor for the LogListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogListView::~LogListView()
	{
	}

	//*************************************************************************
	// Method:		customfilter_timeStamp
	// Description: The custom filter method for TimeStamp
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_TimeStamp(System::Object * sender, EventArgs * e)
	{
		if (timeStampFilterForm == NULL)
			timeStampFilterForm = new TimeStampFilterForm(Filter_OKClick, NULL);

		if (sender != NULL)
			timeStampFilterForm->ShowDialog(this);
	}

	//*************************************************************************
	// Method:		customfilter_threadID
	// Description: The custom filter method for TimeStamp
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_ThreadID(System::Object * sender, EventArgs * e)
	{
		if (threadIDFilterForm == NULL)
			threadIDFilterForm = new GenericListFilterForm (NULL, NULL, "Available Threads", "Include only these",
													false, "Thread Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_ThreadID_RefreshValues));
		if (sender != NULL)
			threadIDFilterForm->ShowDialog(this);
	}

	//*************************************************************************
	// Method:		customfilter_Category
	// Description: The custom filter method for Category header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_Category(System::Object * sender, EventArgs * e)
	{
		if (categoryFilterForm == NULL)
		{
			// initialize the function database
			String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
			interceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			if (!interceptedFunctionDB->IsLoaded)
				interceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

			// get the categories from the database and add it to the "selected" listbox
			System::Array * nativeCategories = interceptedFunctionDB->GetCategoryArray();

			// initialize the .NET function database
			interceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			if (!interceptedFunctionDB->IsLoaded)
				interceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

			// get the categories from the .NET database
			System::Array * dotNetCategories = interceptedFunctionDB->GetCategoryArray();

			// merge the two category lists
			System::Collections::ArrayList* mergedCategoryList = new System::Collections::ArrayList(nativeCategories);
			for (int i=0; i<dotNetCategories->Length; i++)
			{
				if (!mergedCategoryList->Contains(dotNetCategories->Item[i]))
					mergedCategoryList->Add(dotNetCategories->Item[i]);
			}

			System::Array* categories = mergedCategoryList->ToArray();

			categoryFilterForm = new GenericListFilterForm (categories, NULL, "Available Categories", "Include only these",
													false, "Category Filter", Filter_OKClick, NULL, NULL);
		}

		if (sender != NULL)
			categoryFilterForm->ShowDialog(this);
	}
	//*************************************************************************
	// Method:		customfilter_DLL
	// Description: The custom filter method for DLL header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_DLL(System::Object * sender, EventArgs * e)
	{
		if (dllFilterForm == NULL)
		{
			// initialize the native function database
			String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
			interceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			if (!interceptedFunctionDB->IsLoaded)
				interceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

			// get the categories from the native database
			System::Array * nativeDlls = interceptedFunctionDB->GetDLLArray();

			// initialize the .NET function database
			interceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			if (!interceptedFunctionDB->IsLoaded)
				interceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

			// get the categories from the .NET database
			System::Array * dotNetDlls = interceptedFunctionDB->GetDLLArray();

			// merge the two category lists
			System::Collections::ArrayList* mergedDllList = new System::Collections::ArrayList(nativeDlls);
			for (int i=0; i<dotNetDlls->Length; i++)
			{
				if (!mergedDllList->Contains(dotNetDlls->Item[i]))
					mergedDllList->Add(dotNetDlls->Item[i]);
			}

			System::Array* dlls = mergedDllList->ToArray();

			//  add it to the "selected" listbox
			dllFilterForm = new GenericListFilterForm (dlls, NULL, "Available Libraries", "Include only these", true,
												"DLL Filter", Filter_OKClick, NULL, NULL);
		}

		if (sender != NULL)
			dllFilterForm->ShowDialog(this);
	}
	//*************************************************************************
	// Method:		customfilter_Functions
	// Description: The custom filter method for Functions header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_Functions(System::Object * sender, EventArgs * e)
	{
		if (functionsFilterForm == NULL)
			functionsFilterForm = new FunctionsFilterForm(Filter_OKClick, NULL);

		if (sender != NULL)
			functionsFilterForm->ShowDialog(this);
	}

	//*************************************************************************
	// Method:		customfilter_Params
	// Description: The custom filter method for Params header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************	
	void LogListView::customfilter_Params(System::Object * sender, EventArgs * e)
	{
		if (paramFilterForm == NULL)
		{
			paramFilterForm = new ParamFilterForm(Filter_OKClick, NULL);
		}

		if (sender != NULL)
			paramFilterForm->ShowDialog (this);

	}
	//*************************************************************************
	// Method:		customfilter_ReturnValue
	// Description: The custom filter method for Return Value header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_ReturnValue(System::Object * sender, EventArgs * e)
	{
		if (returnValueFilterForm == NULL)
			returnValueFilterForm = new GenericListFilterForm (NULL, NULL, "Return Values", "Only include these", true,
												"Return Value Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_ReturnValue_RefreshValues));
		if (sender != NULL)
			returnValueFilterForm->ShowDialog(this);
	}
	//*************************************************************************
	// Method:		customfilter_ErrorCode
	// Description: The custom filter method for Error Code header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_ErrorCode(System::Object * sender, EventArgs * e)
	{
		if (errorCodeFilterForm == NULL)
			errorCodeFilterForm = new GenericListFilterForm (NULL, NULL, "Error Codes", "Include only these", true,
												"Error Code Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_ErrorCode_RefreshValues));
		if (sender != NULL)
			errorCodeFilterForm->ShowDialog(this);
	}

	//*************************************************************************
	// Method:		customfilter_Exception
	// Description: The custom filter method for Exception header
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_Exception(System::Object * sender, EventArgs * e)
	{
		if (exceptionFilterForm == NULL)
			exceptionFilterForm = new GenericListFilterForm (NULL, NULL, "Exceptions", "Include only these", true,
												"Exception Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_Exception_RefreshValues));

		if (sender != NULL)
			exceptionFilterForm->ShowDialog(this);
	}


	//Event Handlers

	//*************************************************************************
	// Method:		filter_OKClick
	// Description: Called whenever an OK button is clicked on any filter form
	//				Responsible for invoking the FilterUpdate delegate.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::filter_OKClick(System::Object * sender, EventArgs * e)
	{
		this->OnFilterUpdate->Invoke();
	}

	//*************************************************************************
	// Method:		customfilter_ThreadID_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_ThreadID_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->threadIDFilterForm->Visible)
		{
			LogPane * logPane = dynamic_cast <LogPane *> (this->Parent);
			ArrayList * threadIDList = new ArrayList();
			String * itemText;
			//Get the index of the threadID header
			int tidColumnIndex = this->Columns->IndexOf (this->threadIDHeader);
			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; threadIDList->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				TreeListNode *item = logPane->onTreeListViewQueryItem(i);
				if (!item)
					continue;

				itemText = item->SubItems->get_Item(tidColumnIndex)->ToString();
				if (!threadIDList->Contains (itemText))
					threadIDList->Add (itemText);
			}
			
			// sort the values
			threadIDList->Sort ();
			
			// convert to an array
			Array * tidArray = threadIDList->ToArray();

			this->threadIDFilterForm->AvailableList = tidArray;
		}
	}
	//*************************************************************************
	// Method:		customfilter_ReturnValue_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_ReturnValue_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->returnValueFilterForm->Visible)
		{
			LogPane * logPane = dynamic_cast <LogPane *> (this->Parent);
			ArrayList * someReturnVals = new ArrayList();
			String * itemText;
			//Get the index of the return value header
			int rvColumnIndex = this->Columns->IndexOf (this->returnValueHeader);
			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; someReturnVals->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				TreeListNode *item = logPane->onTreeListViewQueryItem(i);
				if (!item)
					continue;

				itemText = item->SubItems->get_Item(rvColumnIndex)->ToString();
				if (!someReturnVals->Contains (itemText))
					someReturnVals->Add (itemText);
			}
			
			// sort the values
			someReturnVals->Sort ();
			
			// convert to an array
			Array * returnValList = someReturnVals->ToArray();

			this->returnValueFilterForm->AvailableList = returnValList;
		}
	}
	//*************************************************************************
	// Method:		customfilter_ErrorCode_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_ErrorCode_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->errorCodeFilterForm->Visible)
		{
			LogPane * logPane = dynamic_cast <LogPane *> (this->Parent);
			ArrayList * someErrorCodes = new ArrayList();
			String * itemText;
			//Get the index of the errorCode header
			int ecColumnIndex = this->Columns->IndexOf (this->errorCodeHeader);
			//Get the last 500 return values from the current log
			for (int i=0; someErrorCodes->Count < MAX_DISPLAY_ERRORCODES && i < logPane->LogCount; i++)
			{
				TreeListNode *item = logPane->onTreeListViewQueryItem(i);
				if (!item)
					continue;

				itemText = item->SubItems->get_Item(ecColumnIndex)->ToString();
				if (!someErrorCodes->Contains (itemText))
					someErrorCodes->Add (itemText);
			}
			
			// sort the values
			someErrorCodes->Sort ();
			
			// convert to an array
			Array * errorCodeList = someErrorCodes->ToArray();
			this->errorCodeFilterForm->AvailableList = errorCodeList;
		}
	}
	//*************************************************************************
	// Method:		customfilter_Exception_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void LogListView::customfilter_Exception_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->exceptionFilterForm->Visible)
		{
			LogPane * logPane = dynamic_cast <LogPane *> (this->Parent);
			ArrayList * someExceptions = new ArrayList();
			String * itemText;
			//Get the index of the errorCode header
			int ecColumnIndex = this->Columns->IndexOf (this->exceptionHeader);
			//Get the last 500 return values from the current log
			for (int i=0; someExceptions->Count < MAX_DISPLAY_ERRORCODES && i < this->Items->Count; i++)
			{
				TreeListNode *item = logPane->onTreeListViewQueryItem(i);
				if (!item)
					continue;

				itemText = item->SubItems->get_Item(ecColumnIndex)->ToString();
				if (!someExceptions->Contains (itemText))
					someExceptions->Add (itemText);
			}
			
			// sort the values
			someExceptions->Sort ();
			
			// convert to an array
			Array * exceptionList = someExceptions->ToArray();
			this->exceptionFilterForm->AvailableList = exceptionList;
		}
	}	
	
}
