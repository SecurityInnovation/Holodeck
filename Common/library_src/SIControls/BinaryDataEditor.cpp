//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		BinaryDataEditor.cpp
//
// DESCRIPTION: Contains implementation for the class BinaryDataEditor
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 6 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#include <stdio.h>
#include "BinaryDataEditor.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		BinaryDataEditor
	// Description: Constructor for the BinaryDataEditor class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	BinaryDataEditor::BinaryDataEditor()
	{
		overwrittenAddresses = new ArrayList();
		insertedAddresses = new ArrayList();
		undoableOperationsHistory = new Stack();
		redoableOperationsHistory = new Stack();
		invalidByteAddresses = new Hashtable();
		changedTextColor = ForeColor;
		insertedTextColor = ForeColor;
		BackColor = SystemColors::Window;
		selectionActive = false;
		selectionOccurring = false;
		selectionStartAddress = 0;
		selectionEndAddress = 0;
		editMode = EditMode::Overwrite;
		rowHeight = 18;
		visible = false;
		inPartialEditMode = false;
		autoCalculateNumColumns = false;
		allowModeSwitch = true;

		// make the default context menu
		contextMenu = new SIContextMenu();
		contextMenu->AddMenuGroup("CopyPaste", SIMenuItemPosition::Normal);
		contextMenu->AddMenuItem("CopyPaste->Cut", SIMenuItemPosition::Normal, Shortcut::None, "Cu&t", 
			contextMenu->AddImageToList(IconLibrary::Instance->GetBitmapForType(IconType::CutIconType)),
			new MenuItemQueryStateDelegate(this, OnQueryCutMenuItemState),
			new MenuItemClickedDelegate(this, OnCutMenuItemClicked), NULL);
		contextMenu->AddMenuItem("CopyPaste->Copy", SIMenuItemPosition::Normal, Shortcut::None, "&Copy", 
			contextMenu->AddImageToList(IconLibrary::Instance->GetBitmapForType(IconType::CopyIconType)),
			new MenuItemQueryStateDelegate(this, OnQueryCopyMenuItemState),
			new MenuItemClickedDelegate(this, OnCopyMenuItemClicked), NULL);
		contextMenu->AddMenuItem("CopyPaste->Paste", SIMenuItemPosition::Normal, Shortcut::None, "&Paste", 
			contextMenu->AddImageToList(IconLibrary::Instance->GetBitmapForType(IconType::PasteIconType)),
			new MenuItemQueryStateDelegate(this, OnQueryPasteMenuItemState),
			new MenuItemClickedDelegate(this, OnPasteMenuItemClicked), NULL);

		Columns = 16;
		StartAddress = 0;
		EndAddress = 0;

		ForeColor = Color::Black;
		Font = new System::Drawing::Font("Courier New", 8);
		BackColor = Color::White;

		SetStyle((ControlStyles)(ControlStyles::AllPaintingInWmPaint | ControlStyles::ResizeRedraw |
			ControlStyles::Opaque | ControlStyles::UserPaint | ControlStyles::DoubleBuffer |
			ControlStyles::Selectable | ControlStyles::UserMouse), true);

		hScrollBar = new HScrollBar();
		hScrollBar->Parent = this;
		hScrollBar->Minimum = 0;
		hScrollBar->Maximum = 0;
		hScrollBar->SmallChange = 10;
		hScrollBar->Hide();
		hScrollBar->ValueChanged += new EventHandler(this, OnScroll);

		vScrollBar = new VScrollBar();
		vScrollBar->Parent = this;
		vScrollBar->Minimum = 0;
		vScrollBar->Maximum = 0;
		vScrollBar->SmallChange = 1;
		vScrollBar->Hide();
		vScrollBar->ValueChanged += new EventHandler(this, OnScroll);

		fieldSeparationWidth = 0;
		columnSeparationWidth = 0;
		dataBytesFieldXOffset = 0;
		asciiBytesFieldXOffset = 0;
		totalFieldsWidth = 0;
		hSize = 0;
		vSize = 0;
	}
	
	//*************************************************************************
	// Method:		~BinaryDataEditor
	// Description: Destructor for the BinaryDataEditorRow class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	BinaryDataEditor::~BinaryDataEditor()
	{
	}

	//*************************************************************************
	// Method:		OverwriteByte
	// Description: overwrites data and stores internally so we can display and undo
	//	correctly
	//
	// Parameters:
	//	address - the address of the changed data
	//	oldData - the original data at that address
	//	currentData - the current data at that address
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OverwriteByte(unsigned long address, unsigned char oldData, unsigned char currentData)
	{
		Byte oldDataBytes[] = new Byte[1];
		Byte currentDataBytes[] = new Byte[1];

		oldDataBytes[0] = oldData;
		currentDataBytes[0] = currentData;

		OverwriteBytes(address, oldDataBytes, currentDataBytes);
	}

	//*************************************************************************
	// Method:		OverwriteBytes
	// Description: overwrites data and stores internally so we can display and undo
	//	correctly
	//
	// Parameters:
	//	address - the address of the changed data
	//	oldData - the original data at that address
	//	currentData - the current data at that address
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OverwriteBytes(unsigned long startAddress, Byte oldData[], Byte currentData[])
	{
		// only overwrite the shorter of the 2 lengths.  hopefully they'll be equal
		int byteCount = oldData->Length;
		if (currentData->Length < byteCount)
			byteCount = currentData->Length;

		UndoableOperation *operation = new UndoableOperation();
		operation->changedDataBytes = new ArrayList();
		operation->editMode = EditMode::Overwrite;
		operation->startAddress = startAddress;
		operation->endAddress = startAddress + byteCount - 1;

		for (int i = 0; i < byteCount; i++)
		{
			ChangedDataByte *changedData = new ChangedDataByte();
			changedData->address = startAddress + i;
			changedData->originalByte = oldData[i];
			changedData->currentByte = currentData[i];
			operation->changedDataBytes->Add(changedData);
		}

		DoOverwrite(operation);
	}

	//*************************************************************************
	// Method:		UndoOverwrite
	// Description: undoes a byte overwrite operation
	//
	// Parameters:
	//	operation - the overwrite operation to undo
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::UndoOverwrite(UndoableOperation *operation)
	{
		if (editMode == EditMode::None)
			return;

		if (!operation)
			return;

		if (OnBytesOverwritten && operation->changedDataBytes)
		{
			unsigned long byteCount = operation->endAddress - operation->startAddress + 1;
			Byte oldData[] = new Byte[byteCount];
			Byte currentData[] = new Byte[byteCount];

			int i = 0;
			IEnumerator *enumerator = operation->changedDataBytes->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ChangedDataByte *changedByte = dynamic_cast<ChangedDataByte *>(enumerator->Current);
				if (!changedByte)
				{
					i++;
					continue;
				}

				oldData[i] = changedByte->originalByte;
				currentData[i] = changedByte->currentByte;

				// remove the address from the list of overwritten addresses.  just remove the first instance though
				// as it may have been overwritten more than once
				bool removed = false;
				unsigned long index = 0;
				IEnumerator *enumerator = overwrittenAddresses->GetEnumerator();
				while (!removed && enumerator->MoveNext())
				{
					unsigned long address = *dynamic_cast<__box unsigned long *>(enumerator->Current);
					if (address == (operation->startAddress + i))
					{
						overwrittenAddresses->RemoveAt(index);
						removed = true;
					}
					index++;
				}

				i++;
			}

			OnBytesOverwritten->Invoke(operation->startAddress, operation->endAddress, currentData, oldData);
		}
	}

	//*************************************************************************
	// Method:		DoOverwrite
	// Description: does the requested overwrite operation
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DoOverwrite(UndoableOperation *operation)
	{
		if (editMode == EditMode::None)
			return;

		if (!operation || !operation->changedDataBytes)
			return;

		int byteCount = operation->endAddress - operation->startAddress + 1;
		Byte overwrittenOldBytes[] = new Byte[byteCount];
		Byte overwrittenCurrentBytes[] = new Byte[byteCount];

		IEnumerator *enumerator = operation->changedDataBytes->GetEnumerator();
		int i = 0;
		while (enumerator->MoveNext())
		{
			ChangedDataByte *changedData = dynamic_cast<ChangedDataByte *>(enumerator->Current);

			overwrittenOldBytes[i] = changedData->originalByte;
			overwrittenCurrentBytes[i] = changedData->currentByte;

			Object *addressObj = __box(changedData->address);
			overwrittenAddresses->Add(addressObj);

			i++;
		}

		undoableOperationsHistory->Push(operation);

		if (OnBytesOverwritten)
			OnBytesOverwritten->Invoke(operation->startAddress, operation->endAddress, overwrittenOldBytes, overwrittenCurrentBytes);
	}

	//*************************************************************************
	// Method:		InsertByte
	// Description: inserts a single byte at an address
	//
	// Parameters:
	//	address - the address to insert at
	//	insertedData - the data to insert
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::InsertByte(unsigned long address, Byte insertedData)
	{
		Byte currentDataBytes[] = new Byte[1];

		currentDataBytes[0] = insertedData;

		InsertBytes(address, currentDataBytes);
	}

	//*************************************************************************
	// Method:		InsertBytes
	// Description: inserts bytes at an address
	//
	// Parameters:
	//	startAddress - the address to start inserting at
	//	insertedData - the data to insert
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::InsertBytes(unsigned long startAddress, Byte insertedData[])
	{
		int byteCount = insertedData->Length;

		UndoableOperation *operation = new UndoableOperation();
		operation->changedDataBytes = new ArrayList();
		operation->editMode = EditMode::Insert;
		operation->startAddress = startAddress;
		operation->endAddress = startAddress + byteCount - 1;

		for (int i = 0; i < byteCount; i++)
		{
			ChangedDataByte *changedData = new ChangedDataByte();
			changedData->address = startAddress + i;
			changedData->originalByte = insertedData[i];
			changedData->currentByte = insertedData[i];
			operation->changedDataBytes->Add(changedData);
		}

		DoInsert(operation);
	}

	//*************************************************************************
	// Method:		DoInsert
	// Description: performs an insert operation
	//
	// Parameters:
	//	operation - the operation to perform
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DoInsert(UndoableOperation *operation)
	{
		if (editMode == EditMode::None)
			return;

		if (!operation || !operation->changedDataBytes)
			return;

		int byteCount = operation->endAddress - operation->startAddress + 1;
		Byte insertedBytes[] = new Byte[byteCount];

		Hashtable *changesTable = new Hashtable();
		// fix up existing overwritten addresses to account for the newly inserted ones
		IEnumerator *enumerator = overwrittenAddresses->GetEnumerator();
		unsigned long i = 0;
		while (enumerator->MoveNext())
		{
			unsigned long thisAddress = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			if (thisAddress >= operation->startAddress)
			{
				thisAddress += operation->changedDataBytes->Count;
				changesTable->Add(__box(i), __box(thisAddress));
			}
			i++;
		}

		enumerator = changesTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			unsigned long index = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			overwrittenAddresses->set_Item(index, changesTable->get_Item(enumerator->Current));
		}

		// fix up existing inserted addresses to account for the newly inserted ones
		enumerator = insertedAddresses->GetEnumerator();
		i = 0;
		changesTable->Clear();
		while (enumerator->MoveNext())
		{
			unsigned long thisAddress = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			if (thisAddress >= operation->startAddress)
			{
				thisAddress += operation->changedDataBytes->Count;
				changesTable->Add(__box(i), __box(thisAddress));
			}
			i++;
		}

		enumerator = changesTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			unsigned long index = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			insertedAddresses->set_Item(index, changesTable->get_Item(enumerator->Current));
		}

		// now insert the bytes
		enumerator = operation->changedDataBytes->GetEnumerator();
		i = 0;
		while (enumerator->MoveNext())
		{
			ChangedDataByte *changedData = dynamic_cast<ChangedDataByte *>(enumerator->Current);

			insertedBytes[i] = changedData->currentByte;
			insertedAddresses->Add(__box(changedData->address));

			i++;
		}

		undoableOperationsHistory->Push(operation);

		if (OnBytesInserted)
			OnBytesInserted->Invoke(operation->startAddress, operation->endAddress, insertedBytes);
	}
	
	//*************************************************************************
	// Method:		UndoInsert
	// Description: performs an undo of an insert operation
	//
	// Parameters:
	//	operation - the operation to undo
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::UndoInsert(UndoableOperation *operation)
	{
		if (editMode == EditMode::None)
			return;

		if (!operation)
			return;

		if (operation->changedDataBytes)
		{
			unsigned long byteCount = operation->endAddress - operation->startAddress + 1;

			int i = 0;
			IEnumerator *enumerator = operation->changedDataBytes->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ChangedDataByte *changedByte = dynamic_cast<ChangedDataByte *>(enumerator->Current);
				if (!changedByte)
				{
					i++;
					continue;
				}

				// remove the address from the list of inserted addresses
				bool removed = false;
				unsigned long index = 0;
				IEnumerator *enumerator = insertedAddresses->GetEnumerator();
				while (!removed && enumerator->MoveNext())
				{
					unsigned long address = *dynamic_cast<__box unsigned long *>(enumerator->Current);
					if (address == (operation->startAddress + i))
					{
						insertedAddresses->RemoveAt(index);
						removed = true;
					}
					index++;
				}

				i++;
			}

			DeleteBytes(operation->startAddress, operation->endAddress);
			// we want to delete, but we don't want to store it on the history list, so
			// pop it off now
			undoableOperationsHistory->Pop();
		}
	}

	//*************************************************************************
	// Method:		DeleteByte
	// Description: deletes a single byte
	//
	// Parameters:
	//	address - the address of the byte to delete
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DeleteByte(unsigned long address)
	{
		DeleteBytes(address, address);
	}

	//*************************************************************************
	// Method:		DeleteBytes
	// Description: deletes a range of bytes
	//
	// Parameters:
	//	startAddress - the first byte to delete
	//	endAddress - the last byte to delete
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DeleteBytes(unsigned long startAddress, unsigned long endAddress)
	{
		int byteCount = endAddress - startAddress + 1;

		UndoableOperation *operation = new UndoableOperation();
		operation->changedDataBytes = new ArrayList();
		operation->editMode = EditMode::Delete;
		operation->startAddress = startAddress;
		operation->endAddress = endAddress;

		if (!OnQueryDataRange)
			return;

		Byte data[] = OnQueryDataRange->Invoke(startAddress, endAddress);
		if (data->Length != byteCount)
			return;

		for (int i = 0; i < byteCount; i++)
		{
			ChangedDataByte *changedData = new ChangedDataByte();
			changedData->address = startAddress + i;
			changedData->originalByte = data[i];
			changedData->currentByte = data[i];
			operation->changedDataBytes->Add(changedData);
		}

		DoDelete(operation);
	}

	//*************************************************************************
	// Method:		DoDelete
	// Description: performs a delete operation
	//
	// Parameters:
	//	operation - the operation to perform
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DoDelete(UndoableOperation *operation)
	{
		if (!operation || !operation->changedDataBytes)
			return;

		int byteCount = operation->endAddress - operation->startAddress + 1;

		Hashtable *changesTable = new Hashtable();
		// fix up existing overwritten addresses to account for the newly deleted ones
		IEnumerator *enumerator = overwrittenAddresses->GetEnumerator();
		unsigned long i = 0;
		while (enumerator->MoveNext())
		{
			unsigned long thisAddress = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			if (thisAddress > operation->endAddress)
			{
				thisAddress -= operation->changedDataBytes->Count;
				changesTable->Add(__box(i), __box(thisAddress));
			}
			i++;
		}

		enumerator = changesTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			unsigned long index = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			overwrittenAddresses->set_Item(index, changesTable->get_Item(enumerator->Current));
		}

		// fix up existing inserted addresses to account for the newly deleted ones
		enumerator = insertedAddresses->GetEnumerator();
		i = 0;
		changesTable->Clear();
		while (enumerator->MoveNext())
		{
			unsigned long thisAddress = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			if (thisAddress > operation->endAddress)
			{
				thisAddress -= operation->changedDataBytes->Count;
				changesTable->Add(__box(i), __box(thisAddress));
			}
			i++;
		}

		enumerator = changesTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			unsigned long index = *dynamic_cast<__box unsigned long *>(enumerator->Current);
			insertedAddresses->set_Item(index, changesTable->get_Item(enumerator->Current));
		}

		for (i = operation->startAddress; i <= operation->endAddress; i++)
		{
			// remove the address from the list of inserted addresses
			bool removed = false;
			unsigned long index = 0;
			enumerator = insertedAddresses->GetEnumerator();
			while (!removed && enumerator->MoveNext())
			{
				unsigned long address = *dynamic_cast<__box unsigned long *>(enumerator->Current);
				if (address == i)
				{
					insertedAddresses->RemoveAt(index);
					removed = true;
				}
				index++;
			}

			// remove the address from the list of overwritten addresses
			removed = false;
			index = 0;
			enumerator = overwrittenAddresses->GetEnumerator();
			while (!removed && enumerator->MoveNext())
			{
				unsigned long address = *dynamic_cast<__box unsigned long *>(enumerator->Current);
				if (address == i)
				{
					overwrittenAddresses->RemoveAt(index);
					removed = true;
				}
				index++;
			}
		}

		// now delete the bytes
		undoableOperationsHistory->Push(operation);

		if (OnBytesDeleted)
			OnBytesDeleted->Invoke(operation->startAddress, operation->endAddress);
	}

	//*************************************************************************
	// Method:		UndoDelete
	// Description: performs an undo of a delete operation
	//
	// Parameters:
	//	operation - the operation to undo
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::UndoDelete(UndoableOperation *operation)
	{
		if (!operation)
			return;

		if (operation->changedDataBytes)
		{
			unsigned long byteCount = operation->endAddress - operation->startAddress + 1;
			Byte data[] = new Byte[byteCount];

			int i = 0;
			IEnumerator *enumerator = operation->changedDataBytes->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ChangedDataByte *changedByte = dynamic_cast<ChangedDataByte *>(enumerator->Current);
				if (!changedByte)
				{
					i++;
					continue;
				}

				data[i] = changedByte->currentByte;

				i++;
			}

			InsertBytes(operation->startAddress, data);
			// we want to insert, but we don't want to store it on the history list, so
			// pop it off now
			undoableOperationsHistory->Pop();
		}
	}

	//*************************************************************************
	// Method:		UndoLastOperation
	// Description: undoes the last data change to the previous value
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::UndoLastOperation()
	{
		if (undoableOperationsHistory->Count == 0)
			return;

		ArrayList *operationsToUndoList;
		Object *nextItemInHistory = undoableOperationsHistory->Pop();
		if (nextItemInHistory->GetType()->ToString()->Equals("SIControls.UndoableOperationSet"))
		{
			UndoableOperationSet *set = dynamic_cast<UndoableOperationSet *>(nextItemInHistory);
			operationsToUndoList = set->operations;
		}
		else if (nextItemInHistory->GetType()->ToString()->Equals("SIControls.UndoableOperation"))
		{
			operationsToUndoList = new ArrayList();
			operationsToUndoList->Add(nextItemInHistory);
		}

		if (!operationsToUndoList)
			return;

		// do them in reverse of how they were put in the array list
		for (int i = operationsToUndoList->Count - 1; i >= 0; i--)
		{
			UndoableOperation *operation = dynamic_cast<UndoableOperation *>(operationsToUndoList->get_Item(i));
			if (!operation)
				continue;

			switch(operation->editMode)
			{
			case EditMode::Overwrite:
				UndoOverwrite(operation);
				break;
			case EditMode::Insert:
				UndoInsert(operation);
				break;
			case EditMode::Delete:
				UndoDelete(operation);
				break;
			}

			caretAddress = operation->startAddress;
		}

		redoableOperationsHistory->Push(nextItemInHistory);
	}

	//*************************************************************************
	// Method:		RedoLastOperation
	// Description: redoes the last data change to the previous value
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::RedoLastOperation()
	{
		if (redoableOperationsHistory->Count == 0)
			return;

		ArrayList *operationsToRedoList;
		Object *nextItemInHistory = redoableOperationsHistory->Pop();
		if (nextItemInHistory->GetType()->ToString()->Equals("SIControls.UndoableOperationSet"))
		{
			UndoableOperationSet *set = dynamic_cast<UndoableOperationSet *>(nextItemInHistory);
			operationsToRedoList = set->operations;
		}
		else if (nextItemInHistory->GetType()->ToString()->Equals("SIControls.UndoableOperation"))
		{
			operationsToRedoList = new ArrayList();
			operationsToRedoList->Add(nextItemInHistory);
		}

		if (!operationsToRedoList)
			return;

		for (int i = 0; i < operationsToRedoList->Count; i++)
		{
			UndoableOperation *operation = dynamic_cast<UndoableOperation *>(operationsToRedoList->get_Item(i));
			if (!operation)
				continue;

			switch(operation->editMode)
			{
			case EditMode::Overwrite:
				DoOverwrite(operation);
				//undoableOperationsHistory->Pop();
				break;
			case EditMode::Insert:
				DoInsert(operation);
				//undoableOperationsHistory->Pop();
				break;
			case EditMode::Delete:
				DoDelete(operation);
				//undoableOperationsHistory->Pop();
				break;
			}

			caretAddress = operation->startAddress;
		}

		undoableOperationsHistory->Push(nextItemInHistory);
	}

	//*************************************************************************
	// Method:		CutSelectedDataToClipboard
	// Description: copies selected data to the clipboard and deletes the selection
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::CutSelectedDataToClipboard()
	{
		if (!selectionActive)
			return;

		if (editMode != EditMode::Insert)
			return;

		CopySelectedDataToClipboard();

		DeleteBytes(selectionStartAddress, selectionEndAddress);
	}

	//*************************************************************************
	// Method:		CopySelectedDataToClipboard
	// Description: copies selected data to the clipboard
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::CopySelectedDataToClipboard()
	{
		if (!selectionActive)
			return;

		Byte data[] = GetDataInRange(selectionStartAddress, selectionEndAddress);
		if (data->Length == 0)
			return;

		DataObject *objectToCopy = new DataObject();
		StringBuilder *builder = new StringBuilder();

		for (int i = 0; i < data->Length; i++)
		{
			unsigned char highNibble = ((unsigned char)data[i] & 0xF0) >> 4;
			unsigned char lowNibble = (unsigned char)data[i] & 0x0F;

			char highChar = highNibble;
			if ((highChar >= 0) && (highChar <= 9))
				highChar = highNibble + 48;
			else
				highChar = highNibble + 87;

			char lowChar = lowNibble;
			if ((lowChar >= 0) && (lowChar <= 9))
				lowChar = lowNibble + 48;
			else
				lowChar = lowNibble + 87;

			String *highStr = new String(highChar, 1);
			String *lowStr = new String(lowChar, 1);

			builder->Append(highStr);
			builder->Append(lowStr);

			if (i < (data->Length - 1))
				builder->Append(L" ");
		}

		objectToCopy->SetData(System::Windows::Forms::DataFormats::Text, builder->ToString());
		Clipboard::SetDataObject(objectToCopy, true);
	}

	//*************************************************************************
	// Method:		PasteDataFromClipboard
	// Description: pastes data from the clipboard
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::PasteDataFromClipboard()
	{
		if (editMode == EditMode::None)
			return;

		System::Windows::Forms::IDataObject *dataObject = Clipboard::GetDataObject();
		if (!dataObject || !dataObject->GetDataPresent(System::Windows::Forms::DataFormats::Text))
			return;

		// make sure the data is only 0-9, a-f
		bool isValid = true;
		int i;
		String *text = dynamic_cast<String*>(dataObject->GetData(System::Windows::Forms::DataFormats::Text));
		for (i = 0; i < text->Length; i++)
		{
			isValid = isValid && (Uri::IsHexDigit(text->get_Chars(i)) || (text->get_Chars(i) == L' '));
		}

		if (!isValid)
			return;

		Char separators[] = { L' ' };
		String *byteStrings[] = text->Split(separators);
		ArrayList *pastedDataArray = new ArrayList();

		for (i = 0; i < byteStrings->Length; i++)
		{
			String *thisStr = byteStrings[i];
			if (thisStr->Length != 2)
				continue;

			// convert the string to an char *
			const __wchar_t __pin *thisString = PtrToStringChars(thisStr);
			char *str = new char[thisStr->Length + 1];
			WideCharToMultiByte(CP_ACP, 0, thisString, -1, str, thisStr->Length + 1, NULL, NULL);
			str[thisStr->Length] = '\0';

			char *stopString;
			unsigned char byteVal = (unsigned char)strtoul(str, &stopString, 16);

			pastedDataArray->Add(__box(byteVal));

			delete[] str;
		}
		
		if (selectionActive)
			caretAddress = selectionStartAddress;

		Byte currentData[] = GetDataInRange(caretAddress, caretAddress + pastedDataArray->Count);
		Byte pastedData[] = new Byte[pastedDataArray->Count];

		IEnumerator *enumerator = pastedDataArray->GetEnumerator();
		i = 0;
		while (enumerator->MoveNext())
		{
			Byte currentByte = *dynamic_cast<__box Byte *>(enumerator->Current);
			pastedData[i] = currentByte;
			i++;
		}

		if (editMode == EditMode::Overwrite)
			OverwriteBytes(caretAddress, currentData, pastedData);
		else if (editMode == EditMode::Insert)
		{
			if (selectionActive)
			{
				UndoableOperationSet *set = new UndoableOperationSet();
				set->operations = new ArrayList();
				DeleteBytes(selectionStartAddress, selectionEndAddress);
				UndoableOperation *deleteOperation = dynamic_cast<UndoableOperation *>(undoableOperationsHistory->Pop());
				set->operations->Add(deleteOperation);

				InsertBytes(selectionStartAddress, pastedData);
				UndoableOperation *insertOperation = dynamic_cast<UndoableOperation *>(undoableOperationsHistory->Pop());
				set->operations->Add(insertOperation);

				undoableOperationsHistory->Push(set);
			}
			else
				InsertBytes(caretAddress, pastedData);
		}

		Invalidate();
	}

	//*************************************************************************
	// Method:		DeleteSelectedData
	// Description: deletes the current selection, or the byte to the right of 
	//	the caret if nothing is selected
	//
	// Parameters:
	//	None
	//
	// Return Value: the data in the specified range
	//*************************************************************************
	void BinaryDataEditor::DeleteSelectedData()
	{
		if (editMode == EditMode::None)
			return;

		if (editMode == EditMode::Overwrite)
		{
			Byte data[];
			if (OnQueryDataRange)
			{
				// if overwrite mode and in partial edit mode, overwrite the partial edit with 0
				// if overwrite mode and not in partial edit mode, back up 1 caret position and overwrite with 0x00
				if (selectionActive)
				{
					data = OnQueryDataRange->Invoke(selectionStartAddress, selectionEndAddress);
					unsigned long byteCount = selectionEndAddress - selectionStartAddress + 1;

					Byte newVal[] = new Byte[byteCount];
					for (unsigned long i = 0; i < byteCount; i++)
						newVal[i] = 0x00;

					OverwriteBytes(selectionStartAddress, data, newVal);
					caretAddress = selectionStartAddress;
				}
				else
				{
					data = OnQueryDataRange->Invoke(caretAddress, caretAddress);

					if (data->Length > 0)
						OverwriteByte(caretAddress, data[0], 0x00);

					inPartialEditMode = false;
				}
			}
		}
		else if (editMode == EditMode::Insert)
		{
			if (selectionActive)
			{
				DeleteBytes(selectionStartAddress, selectionEndAddress);
			}
			else
			{
				DeleteByte(caretAddress);
			}

			inPartialEditMode = false;
		}

		ShowCursor();
	}

	//*************************************************************************
	// Method:		GetDataInRange
	// Description: gets data in the range of addresses
	//
	// Parameters:
	//	startAddress - the first address of the range of addresses
	//	endAddress - the last address in the range of addresses
	//
	// Return Value: the data in the specified range
	//*************************************************************************
	Byte BinaryDataEditor::GetDataInRange(unsigned long startAddress, unsigned long endAddress)[]
	{
		if (startAddress > endAddress)
		{
			unsigned long temp = endAddress;
			endAddress = startAddress;
			startAddress = temp;
		}
		unsigned long byteCount = endAddress - startAddress + 1;

		if (!OnQueryDataRange)
			return NULL;

		return OnQueryDataRange->Invoke(startAddress, endAddress);
	}

	//*************************************************************************
	// Method:		SwitchEditMode
	// Description: toggles the edit mode between insert and overwrite
	//
	// Parameters:
	//	None
	//
	// Return Value: the data in the specified range
	//*************************************************************************
	void BinaryDataEditor::SwitchEditMode()
	{
		// are we allowed to do this?
		if (!AllowEditingModeSwitch)
			return;

		inPartialEditMode = false;
		switch(editMode)
		{
		case EditMode::Insert:
			editMode = EditMode::Overwrite;
			DestroyCaret();
			CreateCaret((HWND)this->Handle.ToPointer(), NULL, dataByteWidth, rowHeight);
			break;
		case EditMode::Overwrite:
			editMode = EditMode::Insert;
		default:					// fall through intentional
			DestroyCaret();
			CreateCaret((HWND)this->Handle.ToPointer(), NULL, 2, rowHeight);
			break;
		}

		if (caretVisible)
			ShowCursor();
	}

	//*************************************************************************
	// Method:		OnPaint
	// Description: called when the control needs to be painted
	//
	// Parameters:
	//	args - args to the paint command
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnPaint(PaintEventArgs *args)
	{
		try
		{
			char hexByteStr[3];
			visible = true;

			Graphics *g = args->Graphics;
			System::Drawing::Rectangle r = ClientRectangle;

			fieldSeparationWidth = (int)g->MeasureString("FF", Font).Width;
			columnSeparationWidth = (int)(g->MeasureString("F", Font).Width / 2);
			dataByteWidth = fieldSeparationWidth;
			asciiByteWidth = (int)g->MeasureString("F", Font).Width;

			RecalculateNumberOfColumns(g);

			if ((hSize > 0) && (vSize > 0))
				g->FillRectangle(new SolidBrush(vScrollBar->BackColor), r.Left + hScrollBar->Width, r.Top + vScrollBar->Height, vSize, hSize);
			g->Clip = new System::Drawing::Region(System::Drawing::Rectangle(r.Left, r.Top, r.Width - vSize, r.Height - hSize));
			g->FillRectangle(new SolidBrush(BackColor), r);
			if (!OnQueryDataRange)
				return;

			unsigned int firstAddress = GetTopVisibleAddress();
			unsigned int lastAddress = GetBottomVisibleAddress();

			Byte defaultData[] = OnQueryDataRange->Invoke(firstAddress, lastAddress);

			int currentYOffset = 2;
			int currentXOffset = 0;

			// figure out where the x offset of the data bytes column is
			dataBytesFieldXOffset = (int)g->MeasureString("0xFFFFFFFF", Font).Width + fieldSeparationWidth + 4;

			// figure out where the x offset of the ascii column is
			asciiBytesFieldXOffset = dataBytesFieldXOffset;
			asciiBytesFieldXOffset += (dataByteWidth * columns);
			asciiBytesFieldXOffset += (columns - 1) * columnSeparationWidth;
			asciiBytesFieldXOffset += fieldSeparationWidth;

			totalFieldsWidth = asciiBytesFieldXOffset + (columns * asciiByteWidth);

			int caretX = dataBytesFieldXOffset + (caretAddress % columns) * dataByteWidth;
			if (caretAddress % columns > 0)
				caretX += (caretAddress % columns) * columnSeparationWidth;

			__int64 caretY = (__int64)(caretAddress / columns) * rowHeight;

			if (hScrollBar->Visible)
				caretX -= hScrollBar->Value;
			if (vScrollBar->Visible)
				caretY -= (__int64)vScrollBar->Value * rowHeight;
			SetCaretPos(caretX, (int)caretY);

			AdjustScrollBars();

			// draw the data
			bool noMoreBytesToDraw = false;
			for (int i = 0; !noMoreBytesToDraw && (i < defaultData->Length); i++)
			{
				if (i % columns == 0)
				{
					// see if we are on a new row
					if (i > 0)
						currentYOffset += rowHeight;

					// draw the address column
					char address[11];
					sprintf(address, "0x%08X", firstAddress + i);
					g->DrawString(address, Font, new SolidBrush(ForeColor), (float)4 - hScrollBar->Value, (float)currentYOffset);

					// set the x offset for the data bytes
					currentXOffset = dataBytesFieldXOffset;
				}

				unsigned char currentByte = defaultData[i];

				bool selected = false;
				if ((selectionOccurring || selectionActive) && 
					(((firstAddress + i) >= selectionStartAddress) && ((firstAddress + i) <= selectionEndAddress)))
				{
					selected = true;
				}

				Brush *brush;
				if (overwrittenAddresses->Contains(__box(firstAddress + i)))
					brush = new SolidBrush(changedTextColor);
				else if (insertedAddresses->Contains(__box(firstAddress + i)))
					brush = new SolidBrush(insertedTextColor);
				else if (selected)
					brush = SystemBrushes::HighlightText;
				else
					brush = new SolidBrush(ForeColor);

				sprintf(hexByteStr, "%02x", currentByte);
				int stringWidth = (int)g->MeasureString(hexByteStr, Font).Width;

				if (selected)
				{
					g->FillRectangle(SystemBrushes::Highlight, (float)currentXOffset - hScrollBar->Value - (columnSeparationWidth / 2), (float)currentYOffset, (float)(columnSeparationWidth + dataByteWidth), (float)rowHeight);
					g->FillRectangle(SystemBrushes::Highlight, (float)(asciiBytesFieldXOffset - hScrollBar->Value + ((i % columns) * asciiByteWidth)), (float)currentYOffset, (float)asciiByteWidth, (float)rowHeight);
				}
				else if (invalidByteAddresses->ContainsKey(__box((unsigned long)(firstAddress + i))))
				{
					g->FillRectangle(new SolidBrush(Color::LightGray), (float)currentXOffset - hScrollBar->Value - (columnSeparationWidth / 2), (float)currentYOffset, (float)(columnSeparationWidth + dataByteWidth), (float)rowHeight);
				}

				if (invalidByteAddresses->ContainsKey(__box((unsigned long)(firstAddress + i))))
				{
					// output ?? to mark as invalid
					g->DrawString("??", Font, brush, (float)currentXOffset - hScrollBar->Value, (float)currentYOffset);
				}
				else
				{
					// output the byte
					g->DrawString(hexByteStr, Font, brush, (float)currentXOffset - hScrollBar->Value, (float)currentYOffset);
				}

				// update the drawing x location
				currentXOffset += dataByteWidth;
				currentXOffset += columnSeparationWidth;

				// add to the ascii string
				if ((currentByte >= 32) && (currentByte <= 127))
					g->DrawString(new String((wchar_t)currentByte, 1), Font, brush, (float)(asciiBytesFieldXOffset - hScrollBar->Value + ((i % columns) * asciiByteWidth)), (float)currentYOffset);
				else
					g->DrawString(S".", Font, brush, (float)(asciiBytesFieldXOffset - hScrollBar->Value + ((i % columns) * asciiByteWidth)), (float)currentYOffset);

				if ((firstAddress + i) == EndAddress)
					noMoreBytesToDraw = true;
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		GetTopVisibleAddress
	// Description: gets the first visible address in the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: the first visible address in the list view
	//*************************************************************************
	unsigned int BinaryDataEditor::GetTopVisibleAddress()
	{
		int topItemIndex = vScrollBar->Value;

		return topItemIndex * columns;
	}


	//*************************************************************************
	// Method:		GetBottomVisibleAddress
	// Description: gets the last visible address in the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: the last visible address in the list view
	//*************************************************************************
	unsigned int BinaryDataEditor::GetBottomVisibleAddress()
	{
		int bottomRowIndex = vScrollBar->Value + ((ClientRectangle.Height - hSize) / rowHeight);
		__int64 lastAddress = ((__int64)(bottomRowIndex) * columns) - 1;
		if (lastAddress > (__int64)EndAddress)
			lastAddress = EndAddress;

		return (unsigned int)lastAddress;
	}

	//*************************************************************************
	// Method:		set_RowCount
	// Description: sets the number of rows in the editor
	//
	// Parameters:
	//	value - the number of rows in the editor
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::set_RowCount(unsigned long value)
	{
		rowCount = value;
	}

	//*************************************************************************
	// Method:		AdjustScrollBars
	// Description: resizes the scrollbars to take into account new rows, rows
	//	removed, and resizing
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::AdjustScrollBars()
	{
		try
		{
			if (RowCount > 0)
			{
				hSize = hScrollBar->Height;
				vSize = vScrollBar->Width;

				hScrollBar->Left = ClientRectangle.Left;
				hScrollBar->Width = ClientRectangle.Width - vSize;
				hScrollBar->Top = ClientRectangle.Top + ClientRectangle.Height - hScrollBar->Height;
				hScrollBar->Maximum = totalFieldsWidth;
				hScrollBar->LargeChange = (ClientRectangle.Width - vSize - 4 > 0) ? ClientRectangle.Width - vSize - 4 : 0;
				if (totalFieldsWidth > ClientRectangle.Width - vSize)
				{
					hScrollBar->Show();
					hSize = hScrollBar->Height;
				}
				else
				{
					hScrollBar->Hide();
					hSize = 0;
					hScrollBar->Value = 0;
				}

				vScrollBar->Left = ClientRectangle.Left + ClientRectangle.Width - vScrollBar->Width;
				vScrollBar->Top = ClientRectangle.Top;
				vScrollBar->Height = ClientRectangle.Height - hSize;
				vScrollBar->SmallChange = 1;
				vScrollBar->Maximum = RowCount - 1;
				vScrollBar->LargeChange = ClientRectangle.Height / rowHeight;

				if (RowCount > (unsigned long)(ClientRectangle.Height - hSize) / rowHeight)
				{
					vScrollBar->Show();
					vSize = vScrollBar->Width;
				}
				else
				{
					vScrollBar->Hide();
					vSize = 0;
					vScrollBar->Value = 0;
				}
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnResize
	// Description: called when the control is resized
	//
	// Parameters:
	//	e - the args representing the resize event
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnResize(EventArgs *e)
	{
		Control::OnResize(e);
		AdjustScrollBars();
		Invalidate();
	}

	//*************************************************************************
	// Method:		set_StartAddress
	// Description: sets the first address that this control can handle
	//
	// Parameters:
	//	value - the first address for the control to handle
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::set_StartAddress(unsigned long value)
	{
		startAddress = value;
		caretAddress = value;
		unsigned int difference = endAddress - startAddress;
		unsigned int numRows = 0;

		if (difference % columns == 0)
			numRows = difference / columns;
		else
			numRows = (difference / columns) + 1;

		if (numRows >= 0)
			RowCount = numRows;
	}
	
	//*************************************************************************
	// Method:		set_EndAddress
	// Description: sets the last address that this control can handle
	//
	// Parameters:
	//	value - the last address for the control to handle
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::set_EndAddress(unsigned long value)
	{
		endAddress = value;
		unsigned int difference = endAddress - startAddress;
		unsigned int numRows = 0;

		if (difference % columns == 0)
			numRows = difference / columns;
		else
			numRows = (difference / columns) + 1;

		if (numRows >= 0)
			RowCount = numRows;
	}

	//*************************************************************************
	// Method:		set_Columns
	// Description: sets the number of byte columns that this control can handle
	//
	// Parameters:
	//	value - the number of bytes per row for the control to handle
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::set_Columns(int value)
	{
		columns = value;
		if (columns <= 1)
			columns = 1;

		unsigned int difference = endAddress - startAddress;
		unsigned int numRows = 0;

		if (difference % columns == 0)
			numRows = difference / columns;
		else
			numRows = (difference / columns) + 1;

		if (numRows >= 0)
			RowCount = numRows;

		AdjustScrollBars();
	}

	//*************************************************************************
	// Method:		set_Visible
	// Description: sets whether or not the control is visible
	//
	// Parameters:
	//	value - true if the control is to be visible, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::set_Visible(bool value)
	{
		visible = true;
		__super::set_Visible(value);
	}
	
	//*************************************************************************
	// Method:		get_Visible
	// Description: gets whether or not the control is visible
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the control is visible, false otherwise
	//*************************************************************************
	bool BinaryDataEditor::get_Visible()
	{
		return __super::get_Visible();
	}

	//*************************************************************************
	// Method:		set_AutoCalculateColumns
	// Description: sets whether or not the editor automatically figures out
	//	the correct number of columns or not
	//
	// Parameters:
	//	value - true if the control is to figure out the correct number of 
	//		columns, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::set_AutoCalculateColumns(bool value)
	{
		autoCalculateNumColumns = value;
		Invalidate();
	}

	//*************************************************************************
	// Method:		RecalculateNumberOfColumns
	// Description: determines the correct number of columns for the current size
	//
	// Parameters:
	//	g - the graphics object used to draw the data
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::RecalculateNumberOfColumns(Graphics *g)
	{
		if (!AutoCalculateColumns)
			return;

		int totalWidth = ClientRectangle.Width - vSize;
		
		int i = 0;
		int currentWidth = 0;
		while(currentWidth < totalWidth)
		{
			// figure out where the x offset of the ascii column would be
			int asciiStringXOffset = dataBytesFieldXOffset;
			asciiStringXOffset += (dataByteWidth * i);
			asciiStringXOffset += (i - 1) * columnSeparationWidth;
			asciiStringXOffset += fieldSeparationWidth;

			currentWidth = asciiStringXOffset + (asciiByteWidth * i);

			if (currentWidth <= totalWidth)
				i++;
		}

		Columns = i - 1;
	}

	//*************************************************************************
	// Method:		OnScroll
	// Description: called when the user scrolls
	//
	// Parameters:
	//	sender - the sender of the scroll event
	//	args - the args representing the scroll event
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnScroll(Object *sender, EventArgs *args)
	{
		Invalidate();
	}

	//*************************************************************************
	// Method:		ScrollToAddress
	// Description: moves the window to make the specified address visible
	//
	// Parameters:
	//	address - the address to make visible
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::ScrollToAddress(unsigned long address)
	{
		try
		{
			if ((address < startAddress) || (address > endAddress))
				return;

			int numVisibleRows = (ClientRectangle.Height - hSize) / rowHeight;
			unsigned long topAddress = GetTopVisibleAddress();
			unsigned long bottomAddress = GetBottomVisibleAddress();
		
			unsigned long row = address / columns;
			unsigned long topRow = topAddress / columns;
			unsigned long bottomRow = bottomAddress / columns;

			bool needScroll = false;
			if ((row > topRow) && (row > bottomRow))
			{
				needScroll = true;
				row = row - numVisibleRows + 1;
			}
			
			if (row < topRow)
			{
				needScroll = true;
			}

			if (needScroll)
				vScrollBar->Value = row;

			caretAddress = address;
			MakeCaretVisible();
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnMouseMove
	// Description: called when the mouse cursor moves
	//
	// Parameters:
	//	args - the args representing the state of the mouse when called
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnMouseMove(MouseEventArgs *args)
	{
		int x = args->X;
		__int64 y = args->Y;
		if (hScrollBar->Visible)
			x += hScrollBar->Value;
		if (vScrollBar->Visible)
			y += (__int64)vScrollBar->Value * rowHeight;

		if ((x > dataBytesFieldXOffset) && (x < totalFieldsWidth))
			Cursor::Current = Cursors::IBeam;
		else
			Cursor::Current = Cursors::Default;

		if (selectionOccurring)
		{
			DetermineSelectionAddresses(mouseDownPointForSelectionX, mouseDownPointForSelectionY, x, y);
			
			if (!selectionSizeToolTip)
			{
				selectionSizeToolTip = new HoverBalloon();
				selectionSizeToolTip->Parent = this;
				selectionSizeToolTip->TitleIcon = TooltipIcon::None;
				selectionSizeToolTip->Text = "0h";
			}
			
			__int64 yDiff = mouseDownPointForSelectionY - y;
			if ((abs(mouseDownPointForSelectionX - x) >= 5) ||
				(yDiff < -5) || (yDiff > 5))
			{
				somethingSelected = true;
			}

			int selectedCount = selectionEndAddress - selectionStartAddress + 1;
			String *text = String::Concat(selectedCount.ToString("X"), "h");
			if (!text->Equals(selectionSizeToolTip->Text))
				selectionSizeToolTip->Text = text;

			if (!selectionSizeToolTip->IsVisible)
			{
				POINT point;
				int px = mouseDownPointForSelectionX - 35;
				__int64 py = mouseDownPointForSelectionY - 20;

				if (hScrollBar->Visible)
					px -= hScrollBar->Value;
				if (vScrollBar->Visible)
					py -= (__int64)vScrollBar->Value * rowHeight;

				point.x = px;
				point.y = (int)py;
				ClientToScreen((HWND)this->Handle.ToPointer(), &point);

				selectionSizeToolTip->Location = Point(point.x, point.y);
				
				selectionSizeToolTip->Show();
			}

			Invalidate();
		}
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: called when a mouse button is clicked down
	//
	// Parameters:
	//	args - the args representing the state of the mouse when called
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnMouseDown(MouseEventArgs *args)
	{
		if (args->X >= totalFieldsWidth)
			return;

		mouseDownPointForSelectionX = args->X;
		mouseDownPointForSelectionY = args->Y;

		if (hScrollBar->Visible)
			mouseDownPointForSelectionX += hScrollBar->Value;
		if (vScrollBar->Visible)
			mouseDownPointForSelectionY += (__int64)vScrollBar->Value * rowHeight;

		if ((args->Button == MouseButtons::Left) && (args->X < totalFieldsWidth))
		{
			// might be a selection event, start tracking it
			selectionOccurring = true;
			selectionActive = false;
			selectionStartAddress = -1;
			selectionEndAddress = -1;
			somethingSelected = false;

			HideCursor();
		}
		else if (args->Button == MouseButtons::Right)
		{
			POINT pt;
			pt.x = mouseDownPointForSelectionX;
			pt.y = (int)mouseDownPointForSelectionY;
			ClientToScreen((HWND)this->Handle.ToPointer(), &pt);

			contextMenu->Refresh();
			contextMenu->TrackPopup(System::Drawing::Point(pt.x, pt.y), false);
		}
	}
	
	//*************************************************************************
	// Method:		OnMouseUp
	// Description: called when a mouse button is released
	//
	// Parameters:
	//	args - the args representing the state of the mouse when called
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnMouseUp(MouseEventArgs *args)
	{
		if (selectionOccurring)
		{
			selectionOccurring = false;

			if (selectionSizeToolTip && selectionSizeToolTip->IsVisible)
				selectionSizeToolTip->Hide();

			selectionSizeToolTip = NULL;

			int x = args->X;
			__int64 y = args->Y;
			if (hScrollBar->Visible)
				x += hScrollBar->Value;
			if (vScrollBar->Visible)
				y += (__int64)vScrollBar->Value * rowHeight;

			DetermineSelectionAddresses(mouseDownPointForSelectionX, mouseDownPointForSelectionY, x, y);
			unsigned long newCaretAddress = selectionStartAddress;
			int selectedCount = selectionEndAddress - selectionStartAddress + 1;
			if ((selectedCount == 1) && !somethingSelected)
			{
				int col = newCaretAddress % columns;
				unsigned long row = newCaretAddress / columns;

				if (caretAddress != newCaretAddress)
					inPartialEditMode = false;
				caretAddress = newCaretAddress;

				int caretX = dataBytesFieldXOffset + col * dataByteWidth;
				if (caretAddress % columns > 0)
					caretX += col * columnSeparationWidth;

				__int64 caretY = (__int64)row * rowHeight;

				if (hScrollBar->Visible)
					caretX -= hScrollBar->Value;
				if (vScrollBar->Visible)
					caretY -= (__int64)vScrollBar->Value * rowHeight;

				ShowCursor();
				SetCaretPos(caretX, (int)caretY);
			}
			else
			{
				inPartialEditMode = false;
				selectionActive = true;
			}

			somethingSelected = false;
		}

		Invalidate();
	}

	//*************************************************************************
	// Method:		DetermineSelectionAddresses
	// Description: figures out which addresses have been selected between
	//	the start and end points specified
	//
	// Parameters:
	//	startX - the x coord of the start of the selection
	//	startY - the y coord of the start of the selection
	//	endX - the x coord of the end of the selection
	//	endY - the y coord of the end of the selection
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DetermineSelectionAddresses(int startX, __int64 startY, int endX, __int64 endY)
	{
		int point1X = startX;
		int point2X = endX;
		__int64 point1Y = startY;
		__int64 point2Y = endY;
		
		bool switched = false;

		// get the points in the right order top to bottom so that we don't have to worry about it later
		if (startY > endY)
		{
			point1X = endX;
			point1Y = endY;
			point2X = startX;
			point2Y = startY;
			switched = true;
		}
		else if ((startY == endY) && (startX > endX))
		{
			point1X = endX;
			point1Y = endY;
			point2X = startX;
			point2Y = startY;
			switched = true;
		}

		unsigned long row1 = (unsigned long)(point1Y / rowHeight);
		unsigned long row2 = (unsigned long)(point2Y / rowHeight);

		// get the starting column
		int col1 = 0;
		if (point1X <= dataBytesFieldXOffset)
			col1 = 0;
		else if (point1X < asciiBytesFieldXOffset)
		{
			// they are selecting a data byte
			int offsetIntoDataBytes = point1X - dataBytesFieldXOffset;
			int currentOffset = 0;
			while (currentOffset < offsetIntoDataBytes)
			{
				currentOffset += dataByteWidth;
				currentOffset += columnSeparationWidth;
				if (currentOffset < offsetIntoDataBytes)
					col1++;
			}
		}
		else if (point1X < totalFieldsWidth)
		{
			// they are selecting an ascii byte
			int offsetIntoAsciiBytes = point1X - asciiBytesFieldXOffset;
			int currentOffset = 0;
			while (currentOffset < offsetIntoAsciiBytes)
			{
				currentOffset += asciiByteWidth;
				if (currentOffset < offsetIntoAsciiBytes)
					col1++;
			}
		}
		else
		{
			// they are selecting to the right of all data
			row1++;
			col1 = 0;
		}

		// get the ending column
		int col2 = 0;
		if (point2X <= dataBytesFieldXOffset)
			col2 = 0;
		else if (point2X < asciiBytesFieldXOffset)
		{
			// they are selecting a data byte
			int offsetIntoDataBytes = point2X - dataBytesFieldXOffset;
			int currentOffset = 0;
			while (currentOffset < offsetIntoDataBytes)
			{
				currentOffset += dataByteWidth;
				currentOffset += columnSeparationWidth;

				if (currentOffset < offsetIntoDataBytes)
					col2++;
			}
		}
		else if (point2X < totalFieldsWidth)
		{
			// they are selecting an ascii byte
			int offsetIntoAsciiBytes = point2X - asciiBytesFieldXOffset;
			int currentOffset = 0;
			while (currentOffset < offsetIntoAsciiBytes)
			{
				currentOffset += asciiByteWidth;
				if (currentOffset < offsetIntoAsciiBytes)
					col2++;
			}
		}
		else
		{
			// they are selecting to the right of all data
			col2 = columns - 1;
		}

		selectionStartAddress = row1 * columns + col1;
		selectionEndAddress = row2 * columns + col2;

		// see if they are somehow backwards
		if (selectionStartAddress > selectionEndAddress)
		{
			unsigned long temp = selectionStartAddress;
			selectionStartAddress = selectionEndAddress;
			selectionEndAddress = temp;
		}
	}

	//*************************************************************************
	// Method:		OnMouseWheel
	// Description: called when a mouse wheel is scrolled
	//
	// Parameters:
	//	args - the args representing the state of the mouse when called
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnMouseWheel(MouseEventArgs *args)
	{
		if (args->Delta > 0)
		{
			if (vScrollBar->Visible)
			{
				int newVal = (vScrollBar->Value - vScrollBar->SmallChange * (args->Delta / 100));
				vScrollBar->Value = (newVal < 0) ? 0 : newVal;
			}
			else if (hScrollBar->Visible)
			{
				int newVal = (hScrollBar->Value - hScrollBar->SmallChange * (args->Delta / 100));
				hScrollBar->Value = (newVal < 0) ? 0 : newVal;
			}
		}
		else if (args->Delta < 0)
		{
			if (vScrollBar->Visible)
			{
				int newVal = (vScrollBar->Value - vScrollBar->SmallChange * (args->Delta / 100));
				int max = vScrollBar->Maximum - vScrollBar->LargeChange;
				vScrollBar->Value = (newVal > max) ? max : newVal;
			}
			else if (hScrollBar->Visible)
			{
				int newVal = (hScrollBar->Value - hScrollBar->SmallChange * (args->Delta / 100));
				int max = hScrollBar->Maximum - hScrollBar->LargeChange;
				hScrollBar->Value = (newVal > max) ? max : newVal;
			}
		}
	}

	//*************************************************************************
	// Method:		WndProc
	// Description: main window message handling procedure for the class
	//
	// Parameters:
	//	m - the message to process
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::WndProc(Message *msg)
	{
		__super::WndProc(msg);
		if (msg->Msg == WM_GETDLGCODE)
		{
			// this list makes arrow and tab key events cause OnKeyXXX events to fire
			msg->Result = IntPtr(DLGC_WANTCHARS | DLGC_WANTARROWS | msg->Result.ToInt32());
		}
	}

	//*************************************************************************
	// Method:		OnKeyDown
	// Description: called when a key is pressed down
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnKeyDown(KeyEventArgs *args)
	{
		switch(args->KeyCode)
		{
		case Keys::Home:
		case Keys::End:
		case Keys::PageUp:
		case Keys::PageDown:
			OnPageKeys(args);
			inPartialEditMode = false;
			break;
		case Keys::Up:
		case Keys::Down:
			OnUpDownKeys(args);
			inPartialEditMode = false;
			break;
		case Keys::Left:
		case Keys::Right:
			OnLeftRightKeys(args);
			inPartialEditMode = false;
			break;
		case Keys::A:
		case Keys::B:
		case Keys::C:
		case Keys::D:
		case Keys::E:
		case Keys::F:
		case Keys::D0:
		case Keys::D1:
		case Keys::D2:
		case Keys::D3:
		case Keys::D4:
		case Keys::D5:
		case Keys::D6:
		case Keys::D7:
		case Keys::D8:
		case Keys::D9:
		case Keys::NumPad0:
		case Keys::NumPad1:
		case Keys::NumPad2:
		case Keys::NumPad3:
		case Keys::NumPad4:
		case Keys::NumPad5:
		case Keys::NumPad6:
		case Keys::NumPad7:
		case Keys::NumPad8:
		case Keys::NumPad9:
			if ((args->KeyCode == Keys::C) && (args->Control))
				OnCopyKey(args);
			else
				OnEditDataKeys(args);
			break;
		case Keys::V:
			if (args->Control)
				OnPasteKey(args);
			inPartialEditMode = false;
			break;
		case Keys::Z:
			if (args->Control)
				OnUndoKey(args);
			inPartialEditMode = false;
			break;
		case Keys::Y:
			if (args->Control)
				OnRedoKey(args);
			inPartialEditMode = false;
			break;
		case Keys::Insert:
			SwitchEditMode();
			break;
		case Keys::Delete:
			OnDeleteKey(args);
			break;
		case Keys::Back:
			OnBackSpaceKey(args);
			break;
		default:
			Control::OnKeyDown(args);
			break;
		}
		MakeCaretVisible();
		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		OnPageKeys
	// Description: called when a page key is pressed down
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnPageKeys(KeyEventArgs *args)
	{
		switch(args->KeyCode)
		{
		case Keys::Home:
			caretAddress = StartAddress;

			if (hScrollBar->Visible)
				hScrollBar->Value = 0;

			args->Handled = true;
			break;
		case Keys::End:
			caretAddress = EndAddress;

			if (hScrollBar->Visible)
				hScrollBar->Value = hScrollBar->Maximum - hScrollBar->LargeChange;

			args->Handled = true;
			break;
		case Keys::PageUp:
			if (vScrollBar->Visible)
			{
				if (vScrollBar->LargeChange > vScrollBar->Value)
				{
					caretAddress = StartAddress;
				}
				else
				{
					caretAddress -= vScrollBar->LargeChange * columns;
					if (caretAddress < StartAddress)
						caretAddress = StartAddress;
				}
			}

			args->Handled = true;
			break;
		case Keys::PageDown:
			if (vScrollBar->Visible)
			{
				if (vScrollBar->Value + vScrollBar->LargeChange > vScrollBar->Maximum - vScrollBar->LargeChange)
				{
					caretAddress = EndAddress;
				}
				else
				{
					caretAddress += vScrollBar->LargeChange * columns;
					if (caretAddress > EndAddress)
						caretAddress = EndAddress;
				}
			}

			args->Handled = true;
			break;
		}

		ScrollToAddress(caretAddress);
	}

	//*************************************************************************
	// Method:		OnUpDownKeys
	// Description: called when an up or down key is pressed down
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnUpDownKeys(KeyEventArgs *args)
	{
		if (RowCount > 0)
		{
			switch(args->KeyCode)
			{
			case Keys::Down:
				if (caretAddress >= EndAddress - columns)
					caretAddress = EndAddress;
				else
					caretAddress += columns;
				
				args->Handled = true;
				break;
			case Keys::Up:
				if (caretAddress <= StartAddress + columns)
					caretAddress = StartAddress;
				else
					caretAddress -= columns;

				args->Handled = true;
				break;
			}

			ScrollToAddress(caretAddress);
		}
	}

	//*************************************************************************
	// Method:		OnLeftRightKeys
	// Description: called when a left or right key is pressed down
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnLeftRightKeys(KeyEventArgs *args)
	{
		if (RowCount > 0)
		{
			switch(args->KeyCode)
			{
			case Keys::Left:
				if (caretAddress > StartAddress)
					caretAddress--;

				args->Handled = true;
				break;
			case Keys::Right:
				if (caretAddress < EndAddress)
					caretAddress++;

				args->Handled = true;
				break;
			}
		}
	}

	//*************************************************************************
	// Method:		MakeCaretVisible
	// Description: updates the horizontal scrollbar to make the caret visible
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::MakeCaretVisible()
	{
		if (hScrollBar->Visible)
		{
			// see if we have to scroll
			int totalColumnDataByteWidth = ((caretAddress % columns) + 1) * dataByteWidth;
			int totalColumnSeparation = 0;
			if (caretAddress % columns > 0)
				totalColumnSeparation = ((caretAddress % columns) - 1) * columnSeparationWidth;
			int xPositionOfCaret = dataBytesFieldXOffset + totalColumnDataByteWidth + totalColumnSeparation;

			int newVal = hScrollBar->Value;
			// see if we have to scroll right
			if (hScrollBar->Value + ClientRectangle.Width <= (xPositionOfCaret + dataByteWidth))
				newVal = xPositionOfCaret - ClientRectangle.Width + dataByteWidth + columnSeparationWidth;
			// see if we have to scroll left
			else if (hScrollBar->Value >= (xPositionOfCaret - dataByteWidth))
				newVal = xPositionOfCaret - dataByteWidth - columnSeparationWidth;

			hScrollBar->Value = newVal;
		}
	}

	//*************************************************************************
	// Method:		OnEditDataKeys
	// Description: updates the horizontal scrollbar to make the caret visible
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnEditDataKeys(KeyEventArgs *args)
	{
		if (editMode == EditMode::None)
			return;

		if (!OnQueryDataRange)
			return;

		// can't edit invalid data
		if (invalidByteAddresses->ContainsKey(__box(caretAddress)))
			return;

		unsigned char hexValuePressed;
		switch(args->KeyCode)
		{
		case D0:
		case D1:
		case D2:
		case D3:
		case D4:
		case D5:
		case D6:
		case D7:
		case D8:
		case D9:
			hexValuePressed = (unsigned char)args->KeyCode - 48;
			break;
		case NumPad0:
		case NumPad1:
		case NumPad2:
		case NumPad3:
		case NumPad4:
		case NumPad5:
		case NumPad6:
		case NumPad7:
		case NumPad8:
		case NumPad9:
			hexValuePressed = (unsigned char)(args->KeyCode - 96);
			break;
		case A:
		case B:
		case C:
		case D:
		case E:
		case F:
			hexValuePressed = (unsigned char)(args->KeyCode - 55);
			break;
		}
		
		Byte data[] = OnQueryDataRange->Invoke(caretAddress, caretAddress);
		if (data->Length < 1)
			return;

		if (editMode == EditMode::Overwrite)
		{
			Byte currentByte = data[0];

			unsigned char newValue = 0;
			// get the new value of the byte
			if (inPartialEditMode)
			{
				// changing the lower nibble
				unsigned char currentHighNibble = (unsigned char)currentByte & 0xF0;
				newValue = currentHighNibble + hexValuePressed;
			}
			else
			{
				// changing the high nibble
				unsigned char currentLowNibble = (unsigned char)currentByte & 0x0F;
				newValue = (hexValuePressed << 4) + currentLowNibble;
			}

			OverwriteByte(caretAddress, currentByte, newValue);
		}
		else if (editMode == EditMode::Insert)
		{
			Byte currentByte = 0;
			unsigned char newValue = 0;

			if (inPartialEditMode)
			{
				// byte has the form of 0x, should become xy
				currentByte = data[0];
				unsigned char currentLowNibble = (unsigned char)currentByte & 0x0F;
				newValue = (currentLowNibble << 4) + hexValuePressed;

				if (OnBytesDeleted)
				{
					// get the partial edit operation off the stack
					Stack *stack = new Stack();
					UndoableOperation *operationToMatch = dynamic_cast<UndoableOperation *>(undoableOperationsHistory->Pop());
					while ((undoableOperationsHistory->Count > 0) && (operationToMatch != partialEditOperation))
					{
						stack->Push(operationToMatch);
						operationToMatch = dynamic_cast<UndoableOperation *>(undoableOperationsHistory->Pop());
					}
					// now push the popped ones back on
					while (stack->Count > 0)
						undoableOperationsHistory->Push(stack->Pop());

					DeleteByte(caretAddress);
					// we don't want to store this event, so pop it off now
					undoableOperationsHistory->Pop();

					InsertByte(caretAddress, newValue);
				}
			}
			else
			{
				newValue = hexValuePressed;
				InsertByte(caretAddress, newValue);
				partialEditOperation = dynamic_cast<UndoableOperation *>(undoableOperationsHistory->Peek());
			}			
		}

		// do we need to advance the caret?
		if (inPartialEditMode)
		{
			caretAddress++;
			if (caretAddress > EndAddress)
				caretAddress = EndAddress;
		}

		inPartialEditMode = !inPartialEditMode;
	}

	//*************************************************************************
	// Method:		OnUndoKey
	// Description: undoes the last edit event
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnUndoKey(KeyEventArgs *args)
	{
		UndoLastOperation();
	}

	//*************************************************************************
	// Method:		OnRedoKey
	// Description: redoes the last undone edit event
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnRedoKey(KeyEventArgs *args)
	{
		RedoLastOperation();
	}

	//*************************************************************************
	// Method:		OnCopyKey
	// Description: copies selected data to the clipboard
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnCopyKey(KeyEventArgs *args)
	{
		CopySelectedDataToClipboard();
	}

	//*************************************************************************
	// Method:		OnPasteKey
	// Description: pastes selected data from the clipboard
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnPasteKey(KeyEventArgs *args)
	{
		PasteDataFromClipboard();
	}

	//*************************************************************************
	// Method:		OnDeleteKey
	// Description: deletes selected data
	//
	// Parameters:
	//	args - the args representing the key(s) pressed
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnDeleteKey(KeyEventArgs *args)
	{
		DeleteSelectedData();
	}

	//*************************************************************************
	// Method:		OnDoubleClick
	// Description: called when the user double clicks on the control
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnDoubleClick(EventArgs *args)
	{
		int xOffset = mouseDownPointForSelectionX;
		
		if (xOffset > totalFieldsWidth)
			return;

		// select the whole row?
		if (xOffset < dataBytesFieldXOffset)
		{
			unsigned long row = (unsigned long)(mouseDownPointForSelectionY / rowHeight);
			selectionStartAddress = row * columns;
			selectionEndAddress = selectionStartAddress + columns - 1;
			selectionActive = true;
			selectionOccurring = false;
			somethingSelected = true;
			inPartialEditMode = false;
			Invalidate();
			return;
		}

		// select a data byte/ascii byte
		selectionStartAddress = caretAddress;
		selectionEndAddress = caretAddress;
		selectionActive = true;
		selectionOccurring = false;
		somethingSelected = true;
		inPartialEditMode = false;
		Invalidate();
	}

	//*************************************************************************
	// Method:		OnBackSpaceKey
	// Description: called when the user presses the backspace key
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnBackSpaceKey(KeyEventArgs *args)
	{
		if (editMode == EditMode::None)
			return;

		if (editMode == EditMode::Overwrite)
		{
			Byte data[];
			if (OnQueryDataRange)
			{
				// if overwrite mode and in partial edit mode, overwrite the partial edit with 0
				// if overwrite mode and not in partial edit mode, back up 1 caret position and overwrite with 0x00
				if (selectionActive)
				{
					data = OnQueryDataRange->Invoke(selectionStartAddress, selectionEndAddress);
					unsigned long byteCount = selectionEndAddress - selectionStartAddress + 1;

					Byte newVal[] = new Byte[byteCount];
					for (unsigned long i = 0; i < byteCount; i++)
						newVal[i] = 0x00;

					OverwriteBytes(selectionStartAddress, data, newVal);
					caretAddress = selectionStartAddress;
				}
				else
				{
					data = OnQueryDataRange->Invoke(caretAddress, caretAddress);

					if (!inPartialEditMode && (caretAddress > StartAddress))
						caretAddress--;

					if (data->Length > 0)
						OverwriteByte(caretAddress, data[0], 0x00);

					inPartialEditMode = false;
				}
			}
		}
		else if (editMode == EditMode::Insert)
		{
			// if insert mode and in partial edit mode, delete the current byte
			// if insert mode and not in partial edit mode, back up 1 caret position and delete the byte
			if (selectionActive)
			{
				DeleteBytes(selectionStartAddress, selectionEndAddress);
				caretAddress = selectionStartAddress;
			}
			else
			{
				if (!inPartialEditMode && (caretAddress > StartAddress))
				{
					caretAddress--;
					DeleteByte(caretAddress);
				}
				else if (inPartialEditMode)
				{
					inPartialEditMode = false;
					DeleteByte(caretAddress);
				}
			}
		}
	}

	//*************************************************************************
	// Method:		OnQueryCutMenuItemState
	// Description: called when the menu needs to get the state of the item
	//
	// Parameters:
	//	itemName - the name of the menu item clicked
	//	tag - the user-defined object associated with the menu item
	//
	// Return Value: the state of the menu item
	//*************************************************************************
	SIMenuItemState BinaryDataEditor::OnQueryCutMenuItemState(String *itemName, Object *tag)
	{
		if (selectionActive && (editMode == EditMode::Insert))
			return SIMenuItemState::Enabled;

		return SIMenuItemState::Disabled;
	}

	//*************************************************************************
	// Method:		OnQueryCopyMenuItemState
	// Description: called when the menu needs to get the state of the item
	//
	// Parameters:
	//	itemName - the name of the menu item clicked
	//	tag - the user-defined object associated with the menu item
	//
	// Return Value: the state of the menu item
	//*************************************************************************
	SIMenuItemState BinaryDataEditor::OnQueryCopyMenuItemState(String *itemName, Object *tag)
	{
		if (selectionActive)
			return SIMenuItemState::Enabled;

		return SIMenuItemState::Disabled;
	}

	//*************************************************************************
	// Method:		OnQueryPasteMenuItemState
	// Description: called when the menu needs to get the state of the item
	//
	// Parameters:
	//	itemName - the name of the menu item clicked
	//	tag - the user-defined object associated with the menu item
	//
	// Return Value: the state of the menu item
	//*************************************************************************
	SIMenuItemState BinaryDataEditor::OnQueryPasteMenuItemState(String *itemName, Object *tag)
	{
		return SIMenuItemState::Enabled;
	}

	//*************************************************************************
	// Method:		OnCutMenuItemClicked
	// Description: called when the user clicks the cut context menu item
	//
	// Parameters:
	//	itemName - the name of the menu item clicked
	//	tag - the user-defined object associated with the menu item
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnCutMenuItemClicked(String *itemName, Object *tag)
	{
		CutSelectedDataToClipboard();
		Invalidate();
	}

	//*************************************************************************
	// Method:		OnCopyMenuItemClicked
	// Description: called when the user clicks the copy context menu item
	//
	// Parameters:
	//	itemName - the name of the menu item clicked
	//	tag - the user-defined object associated with the menu item
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnCopyMenuItemClicked(String *itemName, Object *tag)
	{
		CopySelectedDataToClipboard();
	}
	
	//*************************************************************************
	// Method:		OnPasteMenuItemClicked
	// Description: called when the user clicks the paste context menu item
	//
	// Parameters:
	//	itemName - the name of the menu item clicked
	//	tag - the user-defined object associated with the menu item
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnPasteMenuItemClicked(String *itemName, Object *tag)
	{
		if (!selectionActive)
		{
			// where did they click?
			DetermineSelectionAddresses(mouseDownPointForSelectionX, mouseDownPointForSelectionY, mouseDownPointForSelectionX, mouseDownPointForSelectionY);
		}

		caretAddress = selectionStartAddress;
		PasteDataFromClipboard();
	}

	//*************************************************************************
	// Method:		OnGotFocus
	// Description: called when the control gets focus
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnGotFocus(EventArgs *args)
	{
		EnableCursor();
		ShowCursor();
		Control::OnGotFocus(args);
	}

	//*************************************************************************
	// Method:		OnLostFocus
	// Description: called when the control loses focus
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::OnLostFocus(EventArgs *args)
	{
		HideCursor();
		DisableCursor();
		Control::OnGotFocus(args);
	}

	//*************************************************************************
	// Method:		HideCursor
	// Description: hides the caret
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::HideCursor()
	{
		if (!caretEnabled)
			return;
		if (!caretVisible)
			return;

		HideCaret((HWND)this->Handle.ToPointer());
		caretVisible = false;
	}

	//*************************************************************************
	// Method:		ShowCursor
	// Description: shows the caret
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::ShowCursor()
	{
		if (!caretEnabled)
			return;

		if (!caretVisible)
			ShowCaret((HWND)this->Handle.ToPointer());
		caretVisible = true;
	}

	//*************************************************************************
	// Method:		DisableCursor
	// Description: Disables the cursor.  Calls to ShowCursor will be ignored.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::DisableCursor()
	{
		if (currentCursorWindow == Handle)
		{
			DestroyCaret();
			currentCursorWindow = IntPtr::Zero;
		}
		caretEnabled = false;
	}


	//*************************************************************************
	// Method:		EnableCursor
	// Description: Enables the cursor
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::EnableCursor()
	{
		if ((currentCursorWindow != Handle) && (currentCursorWindow != IntPtr::Zero))
			DestroyCaret();

		inPartialEditMode = false;
		int caretWidth = 2;
		if (editMode == EditMode::Insert)
			caretWidth = dataByteWidth;

		currentCursorWindow = Handle;
		CreateCaret((HWND)this->Handle.ToPointer(), NULL, caretWidth, rowHeight);
		caretEnabled = true;
	}

	//*************************************************************************
	// Method:		EnableCursor
	// Description: Enables the cursor
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::SetInvalidAddressRange(unsigned long startAddress, unsigned long endAddress)
	{
		unsigned long byteCount = endAddress - startAddress + 1;
		for (unsigned long i = 0; i < byteCount; i++)
		{
			if (!invalidByteAddresses->ContainsKey(__box(startAddress + i)))
				invalidByteAddresses->Add(__box(startAddress + i), __box(startAddress + i));
		}
	}
	
	//*************************************************************************
	// Method:		EnableCursor
	// Description: Enables the cursor
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::RemoveRangeFromInvalidAddresses(unsigned long startAddress, unsigned long endAddress)
	{
		unsigned long byteCount = endAddress - startAddress + 1;
		for (unsigned long i = 0; i < byteCount; i++)
		{
			if (!invalidByteAddresses->ContainsKey(__box(startAddress + i)))
				invalidByteAddresses->Remove(__box(startAddress + i));
		}
	}

	//*************************************************************************
	// Method:		MoveToCaret
	// Description: makes the caret in the current viewport
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BinaryDataEditor::MoveToCaret()
	{
		ScrollToAddress(caretAddress);
		MakeCaretVisible();
	}
}