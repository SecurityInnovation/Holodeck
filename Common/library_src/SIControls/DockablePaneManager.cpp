//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		DockablePaneManager.cpp
//
// DESCRIPTION: Contains implementation for the class NonDockablePaneManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#include "DockablePaneManager.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		DockablePaneManager
	// Description: Constructor for the DockablePaneManager class
	//
	// Parameters:
	//	control - the control that this dockable pane manager manages panes for
	//	style - the visual style to use for the docking manager
	//
	// Return Value: None
	//*************************************************************************
	DockablePaneManager::DockablePaneManager(System::Windows::Forms::ContainerControl *control, VisualStyle style)
	{
		visualStyle = style;
		parentControl = control;
		dockManager = new DockingManager(control, visualStyle);
		dockManager->ContentHidden += new DockingManager::ContentHandler(this, OnPaneVisibilityChangedToHidden);
		dockManager->ContentShown += new DockingManager::ContentHandler(this, OnPaneVisibilityChangedToShown);

		groupToContentListTable = new Hashtable();
		groupPositionsTable = new Hashtable();

		imageList = new System::Windows::Forms::ImageList();
		// add an empty item
		imageList->Images->Add(new System::Drawing::Bitmap(1, 1));

		PaneNameChangedHandler = new PaneNameChangedDelegate(this, OnPaneNameChanged);
		PaneTitleChangedHandler = new PaneTitleChangedDelegate(this, OnPaneTitleChanged);
		PaneImageIndexChangedHandler = new PaneImageIndexChangedDelegate(this, OnPaneImageIndexChanged);
	}

	//*************************************************************************
	// Method:		~DockablePaneManager
	// Description: Destructor for the DockablePaneManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DockablePaneManager::~DockablePaneManager()
	{
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the control's image list
	//
	// Parameters:
	//	fileName - the name of the file that holds the image
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int DockablePaneManager::AddImageToList(String *fileName)
	{
		try
		{
			Bitmap *bitmap = new Bitmap(fileName);
			return imageList->Images->Add(bitmap, Color::Empty);
		}
		catch(...)
		{
		}

		return -1;
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the control's image list
	//
	// Parameters:
	//	image - image to add
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int DockablePaneManager::AddImageToList(Bitmap *image)
	{
		return imageList->Images->Add(image, Color::Empty);
	}

	//*************************************************************************
	// Method:		AddPane
	// Description: Adds a pane to the manager
	//
	// Parameters:
	//	pane - the pane to add
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::AddPane(DockablePane *pane)
	{
		if (!pane || !pane->Group || !pane->Name)
			return;

		pane->OnNameChanged += PaneNameChangedHandler;
		pane->OnTitleChanged += PaneTitleChangedHandler;
		pane->OnImageIndexChanged += PaneImageIndexChangedHandler;

		Content *paneContent = NULL;

		if (!groupToContentListTable->Contains(pane->Group))
		{
			// need to make a new vertical content
			paneContent = dockManager->Contents->Add(pane, pane->Title, this->ImageList, pane->ImageIndex);

			switch(pane->PreferredOrientation)
			{
			case DockablePaneOrientation::VerticalLeft:
				if (!lastVerticalLeftContentAdded)
					dockManager->AddContentWithState(paneContent, State::DockLeft);
				break;
			case DockablePaneOrientation::VerticalRight:
				if (!lastVerticalRightContentAdded)
					dockManager->AddContentWithState(paneContent, State::DockRight);
				break;
			case DockablePaneOrientation::HorizontalTop:
				if (!lastHorizontalTopContentAdded)
					dockManager->AddContentWithState(paneContent, State::DockTop);
				break;
			case DockablePaneOrientation::HorizontalBottom:
				if (!lastHorizontalBottomContentAdded)
					dockManager->AddContentWithState(paneContent, State::DockBottom);
				break;
			}

			WindowContent *groupContent = paneContent->ParentWindowContent;
			
			Zone *zone = NULL;
			Content *lastAdded = NULL;
			switch(pane->PreferredOrientation)
			{
			case DockablePaneOrientation::VerticalLeft:
				lastAdded = lastVerticalLeftContentAdded;
				break;
			case DockablePaneOrientation::VerticalRight:
				lastAdded = lastVerticalRightContentAdded;
				break;
			case DockablePaneOrientation::HorizontalTop:
				lastAdded = lastHorizontalTopContentAdded;
				break;
			case DockablePaneOrientation::HorizontalBottom:
				lastAdded = lastHorizontalBottomContentAdded;
				break;
			}

			if (lastAdded && lastAdded->ParentWindowContent)
			{
				zone = lastAdded->ParentWindowContent->ParentZone;
				if (zone)
				{
					DockablePanePosition groupPosition = DockablePanePosition::Normal;
					if (groupPositionsTable->ContainsKey(pane->Group))
						groupPosition = *dynamic_cast<__box DockablePanePosition *>(groupPositionsTable->get_Item(pane->Group));

					int insertPos = GetInsertPositionForWindowContentList(zone->Windows, groupPosition);
					dockManager->AddContentToZone(paneContent, zone, insertPos);
				}
			}

			ArrayList *newList = new ArrayList();
			newList->Add(paneContent);
			groupToContentListTable->Add(pane->Group, newList);
			dockManager->ShowContent(paneContent);
		}
		else
		{
			ArrayList *list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(pane->Group));
			if (!list || list->Count == 0)
				return;
			Content *oldContent = dynamic_cast<Content *>(list->get_Item(0));

			int insertPos = GetInsertPositionForContentList(list, pane);
			paneContent = dockManager->Contents->Add(pane, pane->Title, this->ImageList, pane->ImageIndex);
			dockManager->InsertContentIntoWindowContent(insertPos, paneContent, oldContent->ParentWindowContent);
			dockManager->ShowContent(paneContent);

			list->Add(paneContent);
		}

		if (paneContent)
		{
			switch(pane->PreferredOrientation)
			{
			case DockablePaneOrientation::VerticalLeft:
				lastVerticalLeftContentAdded = paneContent;
				break;
			case DockablePaneOrientation::VerticalRight:
				lastVerticalRightContentAdded = paneContent;
				break;
			case DockablePaneOrientation::HorizontalTop:
				lastHorizontalTopContentAdded = paneContent;
				break;
			case DockablePaneOrientation::HorizontalBottom:
				lastHorizontalBottomContentAdded = paneContent;
				break;
			}
		}
	}

	//*************************************************************************
	// Method:		GetPane
	// Description: gets a dockable pane based on group and name
	//
	// Parameters:
	//	group - the group name of the pane
	//	name - the name of the pane
	//
	// Return Value: the pane if found, NULL otherwise
	//*************************************************************************
	DockablePane *DockablePaneManager::GetPane(String *group, String *name)
	{
		if (!group || !name)
			return NULL;

		ArrayList *list;
		IEnumerator *enumerator;

		// check the pane list
		list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(group));
		if (list && (list->Count > 0))
		{
			enumerator = list->GetEnumerator();
			while (enumerator->MoveNext())
			{
				Content *currContent = dynamic_cast<Content *>(enumerator->Current);
				if (!currContent)
					continue;

				DockablePane *currPane = dynamic_cast<DockablePane *>(currContent->Control);
				if (currPane && (currPane->Name->Equals(name)))
					return currPane;
			}
		}

		// not found, return null
		return NULL;
	}

	//*************************************************************************
	// Method:		RemovePane
	// Description: removes a pane from the control
	//
	// Parameters:
	//	pane - the pane to remove
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::RemovePane(DockablePane *pane)
	{
		if (!pane || !pane->Group || !pane->Name)
			return;

		Content *content = GetContentForPane(pane);
		if (!content)
			return;

		ArrayList *list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(pane->Group));
		if (!list)
			return;

		pane->OnNameChanged -= PaneNameChangedHandler;
		pane->OnTitleChanged -= PaneTitleChangedHandler;
		pane->OnImageIndexChanged -= PaneImageIndexChangedHandler;

		DockablePaneOrientation orientation = pane->PreferredOrientation;
		list->Remove(content);
		if (dockManager->Contents && dockManager->Contents->Contains(content))
			dockManager->Contents->Remove(content);

		if (list->Count == 0)
			groupToContentListTable->Remove(pane->Group);

		switch(orientation)
		{
		case DockablePaneOrientation::VerticalLeft:
			lastVerticalLeftContentAdded = NULL;
			break;
		case DockablePaneOrientation::VerticalRight:
			lastVerticalRightContentAdded = NULL;
			break;
		case DockablePaneOrientation::HorizontalTop:
			lastHorizontalTopContentAdded = NULL;
			break;
		case DockablePaneOrientation::HorizontalBottom:
			lastHorizontalBottomContentAdded = NULL;
			break;
		}

		// make some other pane the last vertical or horizontal
		IEnumerator *enumerator = groupToContentListTable->Values->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList *currList = dynamic_cast<ArrayList *>(enumerator->Current);
			if (!currList)
				continue;

			IEnumerator *listEnum = currList->GetEnumerator();
			while (listEnum->MoveNext())
			{
				Content *currContent = dynamic_cast<Content *>(listEnum->Current);
				if (!currContent)
					continue;

				DockablePane *currPane = dynamic_cast<DockablePane *>(currContent->Control);
				if (!currPane)
					continue;

				if (currPane->PreferredOrientation == orientation)
				{
					switch(orientation)
					{
					case DockablePaneOrientation::VerticalLeft:
						lastVerticalLeftContentAdded = currContent;
						break;
					case DockablePaneOrientation::VerticalRight:
						lastVerticalRightContentAdded = currContent;
						break;
					case DockablePaneOrientation::HorizontalTop:
						lastHorizontalTopContentAdded = currContent;
						break;
					case DockablePaneOrientation::HorizontalBottom:
						lastHorizontalBottomContentAdded = currContent;
						break;
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		RemovePane
	// Description: removes a pane from the control
	//
	// Parameters:
	//	group - the group name of the pane to remove
	//	name - the name of the pane to remove
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::RemovePane(String *group, String *name)
	{
		RemovePane(GetPane(group, name));
	}

	//*************************************************************************
	// Method:		SetPaneIsVisible
	// Description: sets whether or not a pane is visible
	//
	// Parameters:
	//	group - the group name of the pane
	//	name - the name of the pane
	//	isVisible - true to make the pane visible, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::SetPaneIsVisible(String *group, String *name, bool isVisible)
	{
		SetPaneIsVisible(GetPane(group, name), isVisible);
	}
	
	//*************************************************************************
	// Method:		SetPaneIsVisible
	// Description: sets whether or not a pane is visible
	//
	// Parameters:
	//	pane - the pane to set the visibility of
	//	isVisible - true to make the pane visible, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::SetPaneIsVisible(DockablePane *pane, bool isVisible)
	{
		if (!pane)
			return;

		ArrayList *list;
		IEnumerator *enumerator;

		// check the pane list for the pane
		list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(pane->Group));
		if (list && (list->Count > 0))
		{
			enumerator = list->GetEnumerator();
			while (enumerator->MoveNext())
			{
				Content *currContent = dynamic_cast<Content *>(enumerator->Current);
				if (!currContent)
					continue;

				DockablePane *currPane = dynamic_cast<DockablePane *>(currContent->Control);
				if (currPane && (currPane->Name->Equals(pane->Name)))
				{
					if (isVisible)
						dockManager->ShowContent(currContent);
					else
						dockManager->HideContent(currContent);
				}
			}
		}
	}

	//*************************************************************************
	// Method:		GetIsPaneVisible
	// Description: gets whether or not a pane is visible
	//
	// Parameters:
	//	group - the group name of the pane to set the visibility of
	//	name - the name of the pane to set the visibility of
	//
	// Return Value: true if the pane is visible, false otherwise
	//*************************************************************************
	bool DockablePaneManager::GetIsPaneVisible(String *group, String *name)
	{
		return GetIsPaneVisible(GetPane(group, name));
	}
	
	//*************************************************************************
	// Method:		GetIsPaneVisible
	// Description: gets whether or not a pane is visible
	//
	// Parameters:
	//	pane - the pane to set the visibility of
	//
	// Return Value: true if the pane is visible, false otherwise
	//*************************************************************************
	bool DockablePaneManager::GetIsPaneVisible(DockablePane *pane)
	{
		if (!pane)
			return false;

		ArrayList *list;
		IEnumerator *enumerator;

		// check the pane list for the pane
		list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(pane->Group));
		if (list && (list->Count > 0))
		{
			enumerator = list->GetEnumerator();
			while (enumerator->MoveNext())
			{
				Content *currContent = dynamic_cast<Content *>(enumerator->Current);
				if (!currContent)
					continue;

				DockablePane *currPane = dynamic_cast<DockablePane *>(currContent->Control);
				if (currPane && (currPane->Name->Equals(pane->Name)))
				{
					return currContent->Visible;
				}
			}
		}

		return false;
	}

	//*************************************************************************
	// Method:		OnPaneNameChanged
	// Description: called when a pane's name changes
	//
	// Parameters:
	//	pane - the pane that has changed
	//	oldName - the old name of the pane
	//	newName - the new name of the pane
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::OnPaneNameChanged(Pane *pane, String *oldName, String *newName)
	{
	}
	
	//*************************************************************************
	// Method:		OnPaneTitleChanged
	// Description: called when a pane's title changes
	//
	// Parameters:
	//	pane - the pane that has changed
	//	paneName - the name of the pane
	//	oldTitle - the old title of the pane
	//	newTitle - the new title of the pane
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::OnPaneTitleChanged(Pane *pane, String *paneName, String *oldTitle, String *newTitle)
	{
		DockablePane *dockPane = dynamic_cast<DockablePane *>(pane);
		if (!dockPane)
			return;

		Content *content = GetContentForPane(dockPane);
		if (!content)
			return;

		content->Title = newTitle;
		content->FullTitle = newTitle;
	}

	//*************************************************************************
	// Method:		OnPaneImageIndexChanged
	// Description: called when a pane's image index changes
	//
	// Parameters:
	//	pane - the pane that has changed
	//	paneName - the name of the pane
	//	oldIndex - the old image index of the pane
	//	newIndex - the new image index of the pane
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::OnPaneImageIndexChanged(Pane *pane, String *paneName, int oldIndex, int newIndex)
	{
		DockablePane *dockPane = dynamic_cast<DockablePane *>(pane);
		if (!dockPane)
			return;

		Content *content = GetContentForPane(dockPane);
		if (!content)
			return;

		content->ImageIndex = newIndex;
	}

	//*************************************************************************
	// Method:		GetContentForPane
	// Description: gets the content that contains the specified pane
	//
	// Parameters:
	//	pane - the pane to get the content for
	//
	// Return Value: the content containing the pane, or NULL if failed
	//*************************************************************************
	Content *DockablePaneManager::GetContentForPane(DockablePane *pane)
	{
		ArrayList *list;
		IEnumerator *enumerator;

		// check the pane list for the pane
		list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(pane->Group));
		if (list && (list->Count > 0))
		{
			enumerator = list->GetEnumerator();
			while (enumerator->MoveNext())
			{
				Content *currContent = dynamic_cast<Content *>(enumerator->Current);
				if (!currContent)
					continue;

				DockablePane *currPane = dynamic_cast<DockablePane *>(currContent->Control);
				if (currPane && (currPane == pane))
					return currContent;
			}
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		SetGroupPosition
	// Description: sets the position of a group relative to other groups
	//
	// Parameters:
	//	group - the name of the group to set the position for
	//	position - the position to set
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::SetGroupPosition(String *group, DockablePanePosition position)
	{
		if (groupPositionsTable->ContainsKey(group))
			groupPositionsTable->set_Item(group, __box(position));
		else
			groupPositionsTable->Add(group, __box(position));
	}
	
	//*************************************************************************
	// Method:		GetGroupPosition
	// Description: sets the position of a group relative to other groups
	//
	// Parameters:
	//	group - the name of the group to get the position for
	//
	// Return Value: the position of the group
	//*************************************************************************
	DockablePanePosition DockablePaneManager::GetGroupPosition(String *group)
	{
		if (groupPositionsTable->ContainsKey(group))
			return *dynamic_cast<__box DockablePanePosition *>(groupPositionsTable->get_Item(group));
		return DockablePanePosition::Normal;
	}

	//*************************************************************************
	// Method:		RefreshLayout
	// Description: refreshes the layout of the pane groups based on positions
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::RefreshLayout()
	{
		lastVerticalLeftContentAdded = NULL;
		lastVerticalRightContentAdded = NULL;
		lastHorizontalTopContentAdded = NULL;
		lastHorizontalBottomContentAdded = NULL;

		ArrayList *groupNames = new ArrayList();
		IEnumerator *enumerator = groupToContentListTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			String *group = dynamic_cast<String *>(enumerator->Current);
			if (!group)
				continue;

			groupNames->Add(group);
		}

		enumerator = groupNames->GetEnumerator();
		while (enumerator->MoveNext())
		{
			String *group = dynamic_cast<String *>(enumerator->Current);
			if (!group)
				continue;

			RefreshLayoutOfGroup(group);
		}
	}

	//*************************************************************************
	// Method:		RefreshLayoutOfGroup
	// Description: refreshes the layout of the panes in a group based on positions
	//
	// Parameters:
	//	group - the group to refresh
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::RefreshLayoutOfGroup(String *group)
	{
		ArrayList *list = dynamic_cast<ArrayList *>(groupToContentListTable->get_Item(group));
		if (!list || list->Count == 0)
			return;

		WindowContent *windowContent;

		// put them in the correct order in a new list
		ArrayList *orderedList = new ArrayList();
		IEnumerator *enumerator = list->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Content *content = dynamic_cast<Content *>(enumerator->Current);
			if (!content)
				continue;
			DockablePane *pane = dynamic_cast<DockablePane *>(content->Control);
			if (!pane)
				continue;

			windowContent = content->ParentWindowContent;

			int insertPos = GetInsertPositionForContentList(orderedList, pane);
			orderedList->Insert(insertPos, content);
		}

		// remove all the panes
		enumerator = orderedList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Content *content = dynamic_cast<Content *>(enumerator->Current);
			if (!content)
				continue;

			if (dockManager->Contents && dockManager->Contents->Contains(content))
				dockManager->Contents->Remove(content);
			list->Remove(content);
		}
		groupToContentListTable->Remove(group);

		// put them back in the right order
		enumerator = orderedList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Content *content = dynamic_cast<Content *>(enumerator->Current);
			if (!content)
				continue;
			DockablePane *pane = dynamic_cast<DockablePane *>(content->Control);
			if (!pane)
				continue;

			AddPane(pane);
		}
	}

	//*************************************************************************
	// Method:		GetInsertPositionForContentList
	// Description: gets the position in the list to insert the content at
	//
	// Parameters:
	//	list - the list to insert into
	//	pane - the pane to check
	//
	// Return Value: the position in the list to insert the content at
	//*************************************************************************
	int DockablePaneManager::GetInsertPositionForContentList(ArrayList *list, DockablePane *pane)
	{
		if (!list || !pane)
			return 0;

		int insertPos;
		for (insertPos = 0; insertPos < list->Count; insertPos++)
		{
			Content *currContent = dynamic_cast<Content *>(list->get_Item(insertPos));
			if (!currContent)
				continue;
			DockablePane *currPane = dynamic_cast<DockablePane *>(currContent->Control);
			if (!pane)
				continue;

			bool insertNow = false;
			switch(pane->Position)
			{
			case DockablePanePosition::First:
				// Insert before all "normal" and "last" positions
				if ((currPane->Position == DockablePanePosition::Normal) || (currPane->Position == DockablePanePosition::Last))
					insertNow = true;
				break;
			case DockablePanePosition::Normal:
				// Insert before all "last" positions
				if (currPane->Position == DockablePanePosition::Last)
					insertNow = true;
				break;
			case DockablePanePosition::Last:
				// always at end, never break out early
				break;
			}

			if (insertNow)
				break;
		}
		return insertPos;
	}

	//*************************************************************************
	// Method:		GetInsertPositionForWindowContentList
	// Description: gets the position in the list to insert the content at
	//
	// Parameters:
	//	list - the list to insert into
	//	positionToInsert - the position of the content to insert
	//
	// Return Value: the position in the list to insert the content at
	//*************************************************************************
	int DockablePaneManager::GetInsertPositionForWindowContentList(WindowCollection *list, DockablePanePosition positionToInsert)
	{
		if (!list)
			return 0;

		int insertPos;
		for (insertPos = 0; insertPos < list->Count; insertPos++)
		{
			WindowContent *currContent = dynamic_cast<WindowContent *>(list->get_Item(insertPos));
			if (!currContent)
				continue;

			String *groupName = NULL;
			// need to get the group name of the groups in the windowcontent
			if (currContent->Contents->Count > 0)
			{
				Content *content = currContent->Contents->get_Item(0);
				if (content)
				{
					DockablePane *pane = dynamic_cast<DockablePane *>(content->Control);
					if (pane)
					{
						groupName = pane->Group;
					}
				}
			}

			if (!groupName)
				continue;
			
			DockablePanePosition groupPosition = DockablePanePosition::Normal;
			if (groupPositionsTable->ContainsKey(groupName))
				groupPosition = *dynamic_cast<__box DockablePanePosition *>(groupPositionsTable->get_Item(groupName));

			bool insertNow = false;
			switch(positionToInsert)
			{
			case DockablePanePosition::First:
				// Insert before all "normal" and "last" positions
				if ((groupPosition == DockablePanePosition::Normal) || (groupPosition == DockablePanePosition::Last))
					insertNow = true;
				break;
			case DockablePanePosition::Normal:
				// Insert before all "last" positions
				if (groupPosition == DockablePanePosition::Last)
					insertNow = true;
				break;
			case DockablePanePosition::Last:
				// always at end, never break out early
				break;
			}

			if (insertNow)
				break;
		}
		return insertPos;
	}

	//*************************************************************************
	// Method:		OnPaneVisibilityChangedToHidden
	// Description: Handles a docking pane's close event.
	//
	// Parameters:
	//	c - the content object that was changed
	//	args - the event arguments representing the event
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::OnPaneVisibilityChangedToHidden(Content *c, EventArgs *args)
	{
		DockablePane *pane = dynamic_cast<DockablePane *>(c->Control);
		if (!pane)
			return;

		if (pane->OnHidden)
			pane->OnHidden->Invoke();
		if (this->OnPaneHidden)
			this->OnPaneHidden->Invoke(pane);
	}

	//*************************************************************************
	// Method:		OnPaneVisibilityChangedToShown
	// Description: Handles a docking pane's open event.
	//
	// Parameters:
	//	c - the content object that was changed
	//	args - the event arguments representing the event
	//
	// Return Value: None
	//*************************************************************************
	void DockablePaneManager::OnPaneVisibilityChangedToShown(Content *c, EventArgs *args)
	{
		DockablePane *pane = dynamic_cast<DockablePane *>(c->Control);
		if (!pane)
			return;

		if (pane->OnShown)
			pane->OnShown->Invoke();
		if (this->OnPaneShown)
			this->OnPaneShown->Invoke(pane);
	}
}